/**
  * @file SnakeGame.cpp
  * @date September 2025
  * @brief This file implements the Snake game logic.
  * @defgroup ELE3312
  * @{
  */

#include "Game/Sections/SnakeGame.h"
#include "Game/Graphics/GraphObjects/Checkboard.h"
#include "Interfaces/Display/Rect.h"
#include <cpp_main.h>
#include <NucleoImp/Display/ILI9341Display.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>

SnakeGame::SnakeGame() : 
    fruitEncountered(false),
    fruit_count(0),
    score(0),
    state(SnakeGameState::Initialization) {
    // Initialize checkboard as unique_ptr
    checkboard = std::make_unique<Checkboard>();
    
    // Initialize MySnake
    mySnake = std::make_unique<MySnake>();
    
    // Initialize random seed
    srand(9 * HAL_GetTick());
}

SnakeGame::~SnakeGame() {
    // unique_ptr will automatically delete checkboard, mySnake, and fruits
}


void SnakeGame::setup(Display *disp) {
	this->disp = disp;
	fruitEncountered = false;
    fruit_count = 0;
    score = 0;
    
    // Initialize the checkboard
    if (checkboard && disp) {
        Rect gameArea{0, 0, disp->getScreenWidth(), disp->getScreenHeight()};
        checkboard->setup(gameArea, disp, this);
    }
    
    // Initialize MySnake
    if (mySnake && disp) {
        Rect gameArea{0, 0, disp->getScreenWidth(), disp->getScreenHeight()};
        mySnake->setup(gameArea, disp);
    }
}

/** @brief Initializes the snake game.
  */
void SnakeGame::initialize() {
    if (newCheckboard && checkboard) {
        checkboard->draw();
        newCheckboard = false;
    }
    
    fruitEncountered = false;
    
    // Initialize the snake
    initializeSnake();
    
    // Initialize fruits
    initializeFruits();
    
    // Update the display to show the snake and fruits
    if (checkboard) {
        checkboard->update();
    }
}


/** @brief Runs the snake game for a single frame.
  * @retval true The game has finished (game over).
  * @retval false The game continues.
  */
bool SnakeGame::run() {
    switch(state) {
        case SnakeGameState::Initialization:
            initialize();
            state = SnakeGameState::Run;
            break;
            
        case SnakeGameState::Run:
            // Logique de jeu selon les spécifications du laboratoire
            
            // Pour l'instant, on fait juste avancer le serpent automatiquement
            // TODO: Intégrer la lecture des entrées (clavier/accéléromètre)
            moveSnake(0); // eat = 0 pour un mouvement normal
            
            // Mettre à jour l'affichage avec Checkboard
            if (checkboard) {
                checkboard->update();
            }
            
            // Délai pour contrôler la vitesse (selon les spécifications du labo)
            HAL_Delay(200); // 200ms de délai
            break;
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
        int initial_length = randomRange(3, 10);
        
        // Direction initiale aléatoire
        Direction start_direction = static_cast<Direction>(randomRange(0, 3));
        
        // Initialiser le serpent
        mySnake->initializeSnake(start_x, start_y, start_direction, initial_length);
    }
}

void SnakeGame::initializeFruits() {
    // Vider les fruits existants
    fruits.clear();
    
    fruit_count = randomRange(1, MAX_FRUITS); // Entre 1 et 10 fruits
    
    for (int i = 0; i < fruit_count; i++) {
        int x, y;
        do {
            x = randomRange(0, BOARD_WIDTH - 1);
            y = randomRange(0, BOARD_HEIGHT - 1);
        } while (!isPositionFree(x, y));
        
        // Créer un nouveau fruit avec un type aléatoire (seulement Apple et Banana disponibles dans spriteData.h)
        FruitType fruitType = static_cast<FruitType>(randomRange(0, 1)); // 2 types de fruits disponibles
        auto fruit = std::make_unique<Fruit>(fruitType, x, y);
        
        if (disp) {
            Rect gameArea{0, 0, disp->getScreenWidth(), disp->getScreenHeight()};
            fruit->setup(gameArea, disp);
        }
        
        fruits.push_back(std::move(fruit));
    }
}

// Nouvelles méthodes selon les spécifications du laboratoire

void SnakeGame::moveSnake(int eat) {
    if (!mySnake) return;
    
    // Faire avancer le serpent
    mySnake->move(eat);
    
    // Vérifier les collisions
    if (checkFruitCollision()) {
        // Le serpent a mangé un fruit, ne pas manger la queue
        mySnake->move(1); // eat = 1 pour faire grandir le serpent
        score += 10;
    }
    
    // Vérifier les collisions avec les murs et le corps
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

bool SnakeGame::checkFruitCollision() {
    if (!mySnake) return false;
    
    uint16_t headX = mySnake->getHeadX();
    uint16_t headY = mySnake->getHeadY();
    
    // Vérifier collision avec chaque fruit actif
    for (auto& fruit : fruits) {
        if (fruit->isActive() && fruit->isAtPosition(headX, headY)) {
            fruit->consume(); // Marquer le fruit comme consommé
            return true;
        }
    }
    
    return false;
}

bool SnakeGame::checkWallCollision() {
    if (!mySnake) return false;
    
    uint16_t headX = mySnake->getHeadX();
    uint16_t headY = mySnake->getHeadY();
    
    // Vérifier les limites de l'écran
    return (headX >= BOARD_WIDTH || headY >= BOARD_HEIGHT);
}

bool SnakeGame::checkSelfCollision() {
    if (!mySnake) return false;
    
    uint16_t headX = mySnake->getHeadX();
    uint16_t headY = mySnake->getHeadY();
    
    // Vérifier collision avec le corps
    return mySnake->checkCollision(headX, headY);
}

/**
  * @}
  */
