/*
 * SnakeGame.h
 *
 *  Created on: Sep 27, 2025
 *      Author: vicpa
 */

#ifndef INC_GAME_SNAKEGAME_H_
#define INC_GAME_SNAKEGAME_H_

#include "Interfaces/Display/Display.h"
#include "Game/ComMessages/SnakeGameMessage.h"
//#include "Game/ComMessages/handShakeMessage.h"
#include "Game/Graphics/GraphObjects/MySnake.h"
#include "Game/Graphics/GraphObjects/Fruit.h"
#include "Interfaces/MotionInput/MotionInput.h"
#include "Interfaces/Communication/Communication.h"
#include "NucleoImp/MotionInput/MPU6050MotionInput.h"
#include "Interfaces/Display/Point.h"
#include "NucleoImp/SerialCom/SerialFrame.h"
#include "NucleoImp/SerialCom/Ringbuffer.h"
#include "cpp_main.h"
#include <vector>
#include <memory>

// Pour éviter la dépendance circulaire avec checkboard
class Checkboard;

#define MAX_FRUITS 3		  // Nombre de fruits max qui peuvent apparaitre au début
#define MAX_SNAKE_LENGTH 10   // Longueur max que peut avoir le serpent
#define BOARD_WIDTH 32   	  // 32 tuiles de 10x10 = 320 pixels
#define BOARD_HEIGHT 24  	  // 24 tuiles de 10x10 = 240 pixels
#define NUMBER_OF_DIRECTION 4 // Pour générer une direction aléatoire


enum class SnakeGameState {
	Initialization, Run
};


#define UART_BUFFER_SIZE 32

class SnakeGame{
public:
	SnakeGame();
	void setup(Display *disp, Keypad *keypad, Communication *comm, MotionInput  *motionInput, bool is_master); // Add MotionInput, Communication et Player Manager Class
	virtual ~SnakeGame();

	// ===== Communication =====
	void handleRemoteSnakeGameMessage(SnakeGameMessage msg);
	void handleRemoteAck(handShakeMessage msg);

	void sendSnakePosition();
	void initializeOpponentSnake();
	void processUARTMessage(uint8_t* data, size_t size);
    void updateOpponentSnake(uint8_t x, uint8_t y);

    // ===== Setters ====
    void setIsMaster(bool is_master);
    void setSeed(uint32_t seed);

    // ===== Getters =====
    MySnake* getMySnake() const { return mySnake.get(); }
    MySnake* getSnakeOpponent() const { return snakeOpponent.get(); }
    uint32_t getSeed() const    { return seed_; };

    Keypad* getKeypad() const   { return keypad; }
    int getFruitCount() const 	{ return fruit_count; }
    Head* getHead() const	    { return mySnake ? mySnake->getHead() : nullptr; }

    const std::vector<std::unique_ptr<Fruit>>& getFruits() const { return fruits; }
    const std::vector<BodyPart>& getBody() const { 
        static std::vector<BodyPart> emptyBody;
        return mySnake ? mySnake->getBody() : emptyBody; 
    }
    
    // Méthode principales
	void restart();
	bool run();
	void initialize();
	void initializeSnake();
	void initializeFruits();
	
	// Méthodes de contrôle du serpent
	void moveSnake(int eat = 0);  // Faire avancer le serpent
	void turnSnakeLeft();         // Tourner à gauche
	void turnSnakeRight();        // Tourner à droite
	
	// Méthodes de jeu
	bool checkFruitCollision();   // Vérifier collision avec fruits
	bool checkWallCollision();    // Vérifier collision avec murs
	bool checkSelfCollision();    // Vérifier collision avec le corps

	// Méthodes de contrôle du serpent adverse
    void moveSnakeOpponent(int eat = 0);  // Faire avancer le serpent adverse
    bool checkOpponentWallCollision();    // Vérifier collision avec murs pour l'adversaire

	// Méthodes d'aide
	int randomRange(int min, int max);
	bool isPositionFree(int x, int y);
	void generateNewFruit();
	uint32_t computeDelayFromAccel(MotionInput* motionInput);
	void waitForSyncAndGetSeed();

private:
	// Périphériques
	Display *disp = nullptr;
	Keypad *keypad = nullptr;
	MotionInput *motionInput = nullptr;
	Communication *comm = nullptr;

	//PlayerManager *players = nullptr;
    
    // Système principal avec MySnake et Fruit GraphObjects
    std::unique_ptr<MySnake> mySnake;   	    // Le serpent (est un GraphObjects)
    std::vector<std::unique_ptr<Fruit>> fruits; // Les fruits (sont des GraphObjects)
    std::unique_ptr<Checkboard> checkboard;
    bool newCheckboard = true;
    bool fruitEncountered;
    int fruit_count;
    int score;

    // État du système
    SnakeGameState state = SnakeGameState::Initialization;
    CollisionType cType = CollisionType::None;

    // Ajout du serpent adversaire
    std::unique_ptr<MySnake> snakeOpponent;
    bool is_master_;  // Indique si ce microcontrôleur est le maître
    uint32_t seed_;

    // Communication UART
    static constexpr size_t BUFFER_SIZE = 32;
    uint8_t buff[BUFFER_SIZE];
    Ringbuffer uartBuffer;  // Buffer circulaire pour UART
    SerialFrame frame;      // Pour gérer les messages
    
};

#endif /* INC_GAME_SNAKEGAME_H_ */
