#include "Game/Sections/SnakeGame.h"
#include "Game/Graphics/GraphObjects/Checkboard.h"
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
#include "Game/Game.h"
#include "Resources/spriteData.h"


SnakeGame::SnakeGame() :
    fruitEncountered(false),
    fruit_count(0),
    score(0),
    state(SnakeGameState::Initialization),
    is_master_(false),
	isSinglePlayer_(false),
    seed_(0)
    {

    // Initialiser avec des unique_ptr
    checkboard = std::make_unique<Checkboard>();
    mySnake = std::make_unique<MySnake>();
    snakeOpponent = std::make_unique<MySnake>();

    // Seed aléatoire temporaire (sera remplacé par le seed synchronisé)
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


void SnakeGame::sendSnakeDirection() {
    if (!comm || !mySnake) return;

    // Envoyer seulement la direction actuelle
    uint8_t direction = static_cast<uint8_t>(mySnake->getCurrentDirection());
    SnakeGameMessage msg(0, 0, direction, cType);
    comm->send(&msg);
}


void SnakeGame::handleRemoteSnakeGame(SnakeGameMessage msg) {
    switch(msg.getType()) {
        case MessageType::Position: {
            Direction newDirection = static_cast<Direction>(msg.getDirection());
            if (snakeOpponent) {
                snakeOpponent->setDirection(newDirection);
            }
            break;
        }
        case MessageType::GameOver: {
            // L'autre joueur a perdu, on redémarre aussi
            restart();
            break;
        }
        default:
            break;
    }
}

void SnakeGame::updateOpponentSnake(uint8_t x, uint8_t y, Direction direction) {
    if (!snakeOpponent) return;
    
    // Si c'est la première fois qu'on reçoit une position (serpent non initialisé)
    if (snakeOpponent->getLength() == 0) {
        // Initialiser le serpent adverse avec une longueur par défaut
        snakeOpponent->initializeSnake(x, y, direction, 3);
        snakeOpponent->draw(); // Dessiner immédiatement
        return;
    }
    
    // Obtenir la position actuelle de la tête
    uint16_t currentX = snakeOpponent->getHeadX();
    uint16_t currentY = snakeOpponent->getHeadY();
    
    // Vérifier si la position a changé
    if (x != currentX || y != currentY) {
        // Mettre à jour la direction
        snakeOpponent->setDirection(direction);
        
        // Vérifier si le serpent adverse a mangé un fruit à la NOUVELLE position
        bool ateFruit = checkFruitCollision(snakeOpponent.get());
        
        snakeOpponent->setHeadPosition(x, y);

        // Déplacer le corps OU ajouter un segment
        if (ateFruit) {
            snakeOpponent->addBodyPart();
        } else {
            Rect oldTail = snakeOpponent->getOldTail();
            snakeOpponent->moveBodyOnly();
            if (checkboard) {
                uint16_t oldTailX = oldTail.getX1();
                uint16_t oldTailY = oldTail.getY1();
                checkboard->erasePosition(oldTailX, oldTailY);
            }
        }
        
        // Si un fruit a été mangé, générer un nouveau fruit
        if (ateFruit) {
            generateNewFruit();
        }
    } else {
        // Même position, juste mettre à jour la direction
        snakeOpponent->setDirection(direction);
    }
}
void SnakeGame::initialize() {
    srand(seed_);
    
    if (newCheckboard && checkboard) {
        checkboard->draw();
        newCheckboard = false;
    }
    fruitEncountered = false;

    // ====== INITIALISER SELON LE MODE ======
    if (isSinglePlayer_) {
        // MODE SINGLEPLAYER : Un seul serpent
        if (mySnake) {
            mySnake->setColor(COLOR_GREEN, COLOR_DARKGREEN);
            mySnake->initializeSnake(16, 12, Direction::EAST, 3);  // Centre de l'écran
        }
        // Ne pas initialiser snakeOpponent

    } else {
        // MODE MULTIPLAYER : Deux serpents
        if (is_master_) {
            if (mySnake) {
                mySnake->setColor(COLOR_GREEN, COLOR_DARKGREEN);
                mySnake->initializeSnake(5, 5, Direction::EAST, 3);
            }
            if (snakeOpponent) {
                snakeOpponent->setColor(COLOR_BLUE, COLOR_BLUE);
                snakeOpponent->initializeSnake(26, 18, Direction::WEST, 3);
            }
        } else {
            if (snakeOpponent) {
                snakeOpponent->setColor(COLOR_GREEN, COLOR_DARKGREEN);
                snakeOpponent->initializeSnake(5, 5, Direction::EAST, 3);
            }
            if (mySnake) {
                mySnake->setColor(COLOR_BLUE, COLOR_BLUE);
                mySnake->initializeSnake(26, 18, Direction::WEST, 3);
            }
        }
    }

    // ====== INITIALISER LES FRUITS ======
    initializeFruits();

    // ====== DESSINER TOUT ======
    if (checkboard) {
        checkboard->update();
    }

    if (mySnake) {
        mySnake->draw();
    }

    // Dessiner l'adversaire seulement en multijoueur
    if (!isSinglePlayer_ && snakeOpponent) {
        snakeOpponent->draw();
    }

    // Envoyer direction initiale seulement en multijoueur
    if (!isSinglePlayer_ && mySnake) {
        sendSnakeDirection();
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
        HAL_Delay(30);
    }

    // Affiche un message de confirmation
    disp->fillScreen(Color::BLACK);
    disp->drawString(50, 120, is_master ? "Master - Starting game" :
            "Slave - Starting game", Color::WHITE);
    HAL_Delay(3000);

    return;
}

void SnakeGame::handleRemoteAck(handShakeMessage msg) {
    switch(msg.getType()) {
        case MessageType::Ack:
            setSeed(msg.getSeed());
            break;

        default:
            break;
    }
}


bool SnakeGame::run() {
    switch(state) {
        case SnakeGameState::Initialization:
            // En mode multijoueur, attendre la synchro
            if (!isSinglePlayer_) {
                waitForSyncAndGetSeed();
            }
            // En singleplayer, seed déjà défini dans Game.cpp

            initialize();
            state = SnakeGameState::Run;
            break;

        case SnakeGameState::Run:
            // ====== RÉCEPTION UART (SEULEMENT EN MULTIJOUEUR) ======
            if (!isSinglePlayer_) {
                if (Game::uartBuffer.read(buff, BUFFER_SIZE) != 0) {
                    frame.setMessage(buff, BUFFER_SIZE);
                    switch (frame.getMessageType()) {
                        case MessageType::Position:
                            handleRemoteSnakeGame(frame.getSnakeGameMessage());
                            break;
                        case MessageType::GameOver:
                            restart();
                            return false;
                        default:
                            break;
                    }
                }
            }

            // ====== GESTION DU SERPENT LOCAL ======
            if (mySnake && mySnake->getLength() > 0) {
                keypad->update();

                Direction inputDirection = Direction(keypad->getDirection());
                if (inputDirection != Direction::UNKNOWN) {
                    Direction oldDirection = mySnake->getCurrentDirection();

                    // Empêcher demi-tour
                    if ((inputDirection == Direction::NORTH && oldDirection == Direction::SOUTH) ||
                        (inputDirection == Direction::SOUTH && oldDirection == Direction::NORTH) ||
                        (inputDirection == Direction::EAST && oldDirection == Direction::WEST) ||
                        (inputDirection == Direction::WEST && oldDirection == Direction::EAST)) {
                        inputDirection = oldDirection;
                    }

                    mySnake->setDirection(inputDirection);

                    if (!isSinglePlayer_ && inputDirection != oldDirection) {
                        sendSnakeDirection();
                    }
                }

                // 1. Sauvegarder oldTail AVANT de bouger
                Rect oldTail = mySnake->getOldTail();

                // 2. Vérifier collision avec fruit AVANT de bouger
                bool ateFruit = checkFruitCollision(mySnake.get());

                // 3. Bouger le serpent (eat=1 si on a mangé, 0 sinon)
                mySnake->move(ateFruit ? 1 : 0);


                checkboard->erasePositionPixels(oldTail.getX1(), oldTail.getY1());


                // 5. Générer nouveau fruit si on a mangé
                if (ateFruit) {
                    generateNewFruit();
                }

                // 6. Dessiner le serpent UNE SEULE FOIS
                mySnake->draw();

                // 7. Vérifier collisions mortelles
                bool collision = checkWallCollision() || checkSelfCollision();

                if (!isSinglePlayer_) {
                    collision = collision || checkOpponentCollision();
                }

                if (collision) {
                    if (!isSinglePlayer_) {
                        SnakeGameMessage gameOverMsg(0, 0, 0, CollisionType::None);
                        gameOverMsg.setType(MessageType::GameOver);
                        comm->send(&gameOverMsg);
                    }
                    restart();
                    return false;
                }
            }


            // ====== GESTION SERPENT ADVERSE (SEULEMENT EN MULTIJOUEUR) ======
            if (!isSinglePlayer_ && snakeOpponent && snakeOpponent->getLength() > 0) {
                // Sauvegarder oldTail
                Rect oldTailOpponent = snakeOpponent->getOldTail();

                // Vérifier collision AVANT de bouger
                bool ateFruitOpponent = checkFruitCollision(snakeOpponent.get());

                // Déplacer
                snakeOpponent->move(ateFruitOpponent ? 1 : 0);


                checkboard->erasePositionPixels(oldTailOpponent.getX1(), oldTailOpponent.getY1());


                // Générer fruit si mangé
                if (ateFruitOpponent) {
                    generateNewFruit();
                }

                snakeOpponent->draw();
            }

            // ====== DESSINER LES FRUITS ======
            const auto& fruits = getFruits();
            for (const auto& fruit : fruits) {
                if (fruit->isActive()) {
                    fruit->draw();
                }
            }

            // ====== DÉLAI ======
            if (isSinglePlayer_){
            	uint32_t delay_ms = computeDelayFromAccel(motionInput);
            	HAL_Delay(delay_ms);
            }
            else{
            	uint32_t delay_ms = 160; // Si multiplayer on ne met pas accel
            	HAL_Delay(delay_ms);
            }
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
    fruit_count = 4;

    for (int i = 0; i < fruit_count; i++) {
    	generateNewFruit();
    }
}


//bool SnakeGame::checkFruitCollisionOponnent(MySnake* snake) {
//    if (!snake) return false;
//
//    uint16_t headX = snake->getHeadX();
//    uint16_t headY = snake->getHeadY();
//
//    for (auto it = fruits.begin(); it != fruits.end(); ++it) {
//        auto& fruit = *it;
//
//        if (fruit->isActive() && fruit->isAtPosition(headX, headY)) {
//            fruit->consume();
//            fruits.erase(it);
//            return true;
//        }
//    }
//
//    return false;
//}

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

bool SnakeGame::checkFruitCollision(MySnake* snake) {
    if (!snake) return false;

    uint16_t headX = snake->getHeadX();
    uint16_t headY = snake->getHeadY();

    for (auto it = fruits.begin(); it != fruits.end(); ++it) {
        auto& fruit = *it;

        if (fruit->isActive() && fruit->isAtPosition(headX, headY)) {

            // Juste désactiver et supprimer
        	fruit->consume();
            fruits.erase(it);
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

bool SnakeGame::checkOpponentCollision() {
    if (!mySnake) return false;

    return snakeOpponent->isPositionOccupied(mySnake->getHeadX(), mySnake->getHeadY());
}

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
