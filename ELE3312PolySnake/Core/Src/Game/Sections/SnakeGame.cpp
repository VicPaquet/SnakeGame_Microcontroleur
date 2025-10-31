#include "Game/Sections/SnakeGame.h"
#include "Game/Graphics/GraphObjects/Checkboard.h"
#include "Interfaces/Display/Rect.h"
#include "Interfaces/Display/Rect.h"
#include <cpp_main.h>
#include <NucleoImp/Display/ILI9341Display.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include "Game/Graphics/GraphObjects/MySnake.h"


SnakeGame::SnakeGame() : 
    fruitEncountered(false),
    fruit_count(0),
    score(0),
    state(SnakeGameState::Initialization) {

    // Initialiser avec des unique_ptr
    checkboard = std::make_unique<Checkboard>();
    mySnake = std::make_unique<MySnake>();
    
    // Seed aléatoire
    srand(9 * HAL_GetTick());
}

SnakeGame::~SnakeGame() {}

void SnakeGame::setup(Display *disp, Keypad *keypad, MotionInput *motionInput) {
	this->motionInput = motionInput;
	this->disp = disp;
	this->keypad = keypad;
	fruitEncountered = false;
    fruit_count = 0;
    score = 0;
    
    // Setup la classe le checkboard qui est le background d'affichage du jeu s'il n'existe pas encore (crée un graphObject)
    if (checkboard && disp) {
        Rect gameArea{0, 0, disp->getScreenWidth(), disp->getScreenHeight()};
        checkboard->setup(gameArea, disp, this);
    }
    
    // Setup la classe mySnake s'il n'existe pas encore (crée un graphObject)
    if (mySnake && disp) {
        Rect gameArea{0, 0, disp->getScreenWidth(), disp->getScreenHeight()};
        mySnake->setup(gameArea, disp);
    }
}

void SnakeGame::initialize() {

	// Dessine le checboard s'il a été setup
    if (newCheckboard && checkboard) {
        checkboard->draw();
        newCheckboard = false;
    }
    fruitEncountered = false;
    
    // Initialise le
    initializeSnake();
    
    // Initialize fruits
    initializeFruits();
    
    // Update the display to show the snake and fruits
    if (checkboard) {
        checkboard->update();
    }
}

bool SnakeGame::run() {
    switch(state) {
        case SnakeGameState::Initialization:
            initialize();
            state = SnakeGameState::Run;
            break;
            
        case SnakeGameState::Run:
        	// On va chercher la direction du keypad
        	Direction direction = Direction(keypad->getDirection());

        	// On vient setter la nouvelle direction si c'est une nouvelle direction
        	if (direction != Direction::NORTH && direction !=  Direction::SOUTH  && direction != Direction::EAST && direction != Direction::WEST){
        		direction = mySnake->getCurrentDirection();
        		mySnake->setDirection(direction);
        	}
        	else{
        		mySnake->setDirection(direction);
        	}

        	// On bouge le snake
        	moveSnake(0); // eat = 0 pour un mouvement normal
            
            // Mettre à jour l'affichage du Checkboard
            if (checkboard) {
                checkboard->update();
            }

            //Modifier la vitesse depuis l'accelerometre
            uint32_t delay_ms = computeDelayFromAccel(motionInput);
            HAL_Delay(delay_ms);
    }
    return false; // Game continues
}

/** @brief Restarts the snake game.
  */
void SnakeGame::restart() {
    newCheckboard = true;
    fruitEncountered = false;
    fruit_count = 0;
    score = 0;
    state = SnakeGameState::Initialization;
    
    // Reset MySnake
    if (mySnake) {
        mySnake->reset();
    }
    
    // Clear fruits
    fruits.clear();
}

int SnakeGame::randomRange(int min, int max) {
    return min + (rand() % (max - min + 1));
}

bool SnakeGame::isPositionFree(int x, int y) {
    // Vérifier le serpent MySnake
    if (mySnake && mySnake->isPositionOccupied(x, y)) {
        return false;
    }
    
    // Vérifier les fruits
    for (const auto& fruit : fruits) {
        if (fruit->isActive() && fruit->isAtPosition(x, y)) {
            return false;
        }
    }
    return true;
}


void SnakeGame::initializeSnake() {
    // Initialiser MySnake avec un tampon circulaire
    if (mySnake && disp) {
        // Position de départ aléatoire mais valide (au moins 5 cases des bordures)
        int start_x = randomRange(5, BOARD_WIDTH - 5);
        int start_y = randomRange(5, BOARD_HEIGHT - 5);
        
        // Longueur initiale du serpent (entre 3 et 10 tuiles)
        int initial_length = randomRange(3, MAX_SNAKE_LENGTH);
        
        // Direction initiale aléatoire
        Direction start_direction = static_cast<Direction>(randomRange(0, 3));
        
        // Initialiser le serpent
        mySnake->initializeSnake(start_x, start_y, start_direction, initial_length);
    }
}

void SnakeGame::initializeFruits() {
    // Vider les fruits existants s'il y en a
    fruits.clear();
    fruit_count = randomRange(1, MAX_FRUITS); // Entre 1 et 10 fruits
    
    for (int i = 0; i < fruit_count; i++) {
    	generateNewFruit();
    }
}

void SnakeGame::moveSnake(int eat) {
    if (!mySnake) return;
    
    // Faire avancer le serpent: cela bouge le mySnake dans le vecteurs (update le vecteur)
    mySnake->move(eat);
    
    if (checkFruitCollision()) {
        mySnake->move(1); // eat = 1 pour faire grandir le serpent si on a collision avec un fruit
        score += 10;      // Pas encore de logique de pointage, peut être utile pour la suite
        generateNewFruit();
        MySnake* snake = this->getMySnake();
        if (snake) {
            snake->draw();
        }
    }
    
    if (checkWallCollision() || checkSelfCollision()) {
        // Game over
        restart();
    }
}

void SnakeGame::turnSnakeLeft() {
    if (mySnake) {
        mySnake->turn(0); // 0 = gauche
    }
}

void SnakeGame::turnSnakeRight() {
    if (mySnake) {
        mySnake->turn(1); // 1 = droite
    }
}

void SnakeGame::generateNewFruit(){
    uint16_t x = rand() % (BOARD_WIDTH);
    uint16_t y = rand() % (BOARD_HEIGHT);

    FruitType fruitType = static_cast<FruitType>(rand() % 2);
    auto fruit = std::make_unique<Fruit>(fruitType, x, y);
    Rect gameArea{0, 0, disp->getScreenWidth(), disp->getScreenHeight()};
                    fruit->setup(gameArea, disp);
    fruits.push_back(std::move(fruit));
}

//bool SnakeGame::checkFruitCollision() {
//    if (!mySnake) return false;
//
//    uint16_t headX = mySnake->getHeadX();
//    uint16_t headY = mySnake->getHeadY();
//
//    for (auto it = fruits.begin(); it != fruits.end(); ++it) {
//        auto& fruit = *it;
//
//        if (fruit->isActive() && fruit->isAtPosition(headX, headY)) {
//            fruit->consume();
//
//            // Supprimer du vecteur
//            fruits.erase(it);
//            return true;
//        }
//    }
//
//    return false;
//}

bool SnakeGame::checkWallCollision() {
    if (!mySnake) return false;
    
    uint16_t headX = mySnake->getHeadX();
    uint16_t headY = mySnake->getHeadY();
    
    // Vérifier les limites de l'écran
    return (headX >= BOARD_WIDTH || headY >= BOARD_HEIGHT);
}

//bool SnakeGame::checkSelfCollision() {
//    if (!mySnake) return false;
//
//    uint16_t headX = mySnake->getHeadX();
//    uint16_t headY = mySnake->getHeadY();
//
//    // Vérifier collision avec le corps
//    return mySnake->checkCollision(headX, headY);
//}

uint32_t SnakeGame::computeDelayFromAccel(MotionInput* motionInput)
{
    // Constantes de configuration
    const uint32_t BASE_MS = 180;
    const float    DEAD    = 0.07f;
    const float    K       = 1.5f;
    const uint32_t MIN_MS  = 60;
    const uint32_t MAX_MS  = 400;

    // --- Lecture de l'accéléromètre ---
    if (motionInput) motionInput->update();

    float ax = 0.0f;
    if (motionInput) {
        ax = motionInput->getX();
    }

    // Traitement de l'accélération
    float a = std::abs(ax);
    if (a < DEAD)
        a = 0.0f;
    else
        a -= DEAD;

    // Calcul du facteur et du délai
    float factor = 1.0f / (1.0f + K * a);
    uint32_t delay_ms = static_cast<uint32_t>(BASE_MS * factor);

    // Saturation aux bornes
    if (delay_ms < MIN_MS) delay_ms = MIN_MS;
    if (delay_ms > MAX_MS) delay_ms = MAX_MS;

    return delay_ms;
}

