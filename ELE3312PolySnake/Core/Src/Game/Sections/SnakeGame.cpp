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
#include "NucleoImp/SerialCom/SerialFrame.h"
#include "NucleoImp/SerialCom/UART.h"
#include "Game/ComMessages/SnakeGameMessage.h"
#include "Game/Game.h" // Added include for the global Game::uartBuffer


SnakeGame::SnakeGame() :
    fruitEncountered(false),
    fruit_count(0),
    score(0),
    state(SnakeGameState::Initialization)
    {

    // Initialiser avec des unique_ptr
    checkboard = std::make_unique<Checkboard>();
    mySnake = std::make_unique<MySnake>();
    snakeOpponent = std::make_unique<MySnake>();

    // Seed aléatoire
    srand(9 * HAL_GetTick());
}

SnakeGame::~SnakeGame() {}

void SnakeGame::setup(Display *disp, Keypad *keypad, Communication *comm, MotionInput *motionInput) {
    this->comm = comm;
    this->motionInput = motionInput;
    this->disp = disp;
    this->keypad = keypad;
    fruitEncountered = false;
    fruit_count = 0;
    score = 0;

    // Setup le checkboard
    if (checkboard && disp) {
        Rect gameArea{0, 0, disp->getScreenWidth(), disp->getScreenHeight()};
        checkboard->setup(gameArea, disp, this);
    }

    // Setup le serpent local
    if (mySnake && disp) {
        Rect gameArea{0, 0, disp->getScreenWidth(), disp->getScreenHeight()};
        mySnake->setup(gameArea, disp);
    }

    // Setup le serpent adverse
    snakeOpponent = std::make_unique<MySnake>();
    if (snakeOpponent && disp) {
        Rect gameArea{0, 0, disp->getScreenWidth(), disp->getScreenHeight()};
        snakeOpponent->setup(gameArea, disp);
    }
}

void SnakeGame::sendSnakePosition() {
	if (!comm) return;

    SnakeGameMessage msg(mySnake->getHeadX(),
    		mySnake->getHeadY(), cType);
    comm->send(&msg);
}

void SnakeGame::handleRemoteSnakeGame(SnakeGameMessage msg) {
    switch(msg.getType()) {
        case MessageType::Position:
            updateOpponentSnake(msg.getX(), msg.getY());
            // msg.getCollisionType()
            break;
        default:
        	break;
    }
}

void SnakeGame::updateOpponentSnake(uint8_t x, uint8_t y) {
    if (snakeOpponent) {
        snakeOpponent->setHeadPosition(x, y);
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

void SnakeGame::waitForSyncAndGetSeed() {
    uint32_t seed = 0;
    bool is_master = false;
    bool shouldExit = false;

    // Réinitialisation explicite du keypad et délai pour stabilisation
    keypad->update();
    HAL_Delay(100);

    disp->fillScreen(Color::BLACK);
    disp->drawString(50, 100, "Appuyez sur la touche", Color::WHITE);
    disp->drawString(50, 120, "du haut pour demarrer", Color::WHITE);

    // Boucle d'attente
    while (!shouldExit) {
        // Mettre à jour l'état du keypad
        keypad->update();

        // Vérifier d'abord si une touche est pressée
        if (keypad->isAnnyKeyPressed()) {
            Direction direction = Direction(keypad->getDirection());

            // Vérifie si la direction nord est pressée
            if (direction == Direction::NORTH) {
                // Ce microcontrôleur sera le maître
                seed = HAL_GetTick();
                setSeed(seed);

                handShakeMessage message(seed);
                message.setType(MessageType::Ack);
                bool send_ok = comm->send(&message);
                if (!send_ok) {
                    disp->fillScreen(Color::RED);
                    disp->drawString(20, 120, "Erreur: envoi UART!", Color::WHITE);
                    HAL_Delay(2000);
                }

                is_master = true;
                setIsMaster(is_master);
                shouldExit = send_ok; // Ne sortir que si l'envoi a réussi
            }
        }

        // Vérifier si on reçoit un message de l'autre microcontrôleur
        if (!shouldExit) {
            // NOTE: The HAL interrupt writes into the global Game::uartBuffer (via Game::handleUART).
            // Il faut lire dans Game::uartBuffer (buffer global), pas dans le membre local uartBuffer !
            if (Game::uartBuffer.read(buff, BUFFER_SIZE) != 0) {
                frame.setMessage(buff, BUFFER_SIZE);

                if (frame.getMessageType() == MessageType::Ack) {
                    handleRemoteAck(frame.getHandShakeMessage());
                    setIsMaster(false);
                    shouldExit = true;
                }
            }
        }

        // Petit délai pour éviter la surcharge du CPU
        HAL_Delay(50);
    }

    // Affiche un message de confirmation
    disp->fillScreen(Color::BLACK);
    disp->drawString(50, 120, is_master ? "Master - Starting game" :
            "Slave - Starting game", Color::WHITE);
    HAL_Delay(2000);

    return;
}

void SnakeGame::handleRemoteAck(handShakeMessage msg) {
    switch(msg.getType()) {
        case MessageType::Ack:
        	setSeed(msg.getSeed());
        default:
        	break;
    }
}

bool SnakeGame::run() {
    switch(state) {
        case SnakeGameState::Initialization:
            waitForSyncAndGetSeed();
            initialize();
            state = SnakeGameState::Run;
            break;

        case SnakeGameState::Run:
            // Check UART and dispatch messages
            // Read from the global Game::uartBuffer (filled by HAL callback)
            if (Game::uartBuffer.read(buff, BUFFER_SIZE) != 0) {
                frame.setMessage(buff, BUFFER_SIZE);

                switch (frame.getMessageType()) {
                    case MessageType::Position:
                        handleRemoteSnakeGame(frame.getSnakeGameMessage());
                        break;
                    default:
                        break;
                }
            }

            // Gestion des entrées du serpent local
            Direction direction = Direction(keypad->getDirection());
            if (direction != Direction::UNKNOWN) {
                mySnake->setDirection(direction);
            }

            // Mouvement du serpent local
            moveSnake(0);

            // Envoyer la position à l'autre microcontrôleur
            sendSnakePosition();

            // Mettre à jour l'affichage
            if (checkboard) {
                checkboard->update();
            }

            // Dessiner le serpent adverse
            if (snakeOpponent) {
                snakeOpponent->draw();
            }

            // Délai basé sur l'accéléromètre
            uint32_t delay_ms = computeDelayFromAccel(motionInput);
            HAL_Delay(delay_ms);
            break;
    }
    return false;
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
