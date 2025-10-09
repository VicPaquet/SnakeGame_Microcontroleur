/*
 * SnakeGame.h
 *
 *  Created on: Sep 27, 2025
 *      Author: vicpa
 */

#ifndef INC_GAME_SNAKEGAME_H_
#define INC_GAME_SNAKEGAME_H_

#include "Interfaces/Display/Display.h"
#include "Game/Graphics/GraphObjects/MySnake.h"
#include "Game/Graphics/GraphObjects/Fruit.h"
#include "Interfaces/Display/Point.h"
#include <vector>
#include <memory>

// Forward declaration to avoid circular dependency
class Checkboard;

#define MAX_FRUITS 10
#define MAX_SNAKE_LENGTH 101
#define BOARD_WIDTH 32   // 32 tuiles de 10x10 = 320 pixels
#define BOARD_HEIGHT 24  // 24 tuiles de 10x10 = 240 pixels
#define NUMBER_OF_DIRECTION 4 // Pour générer une direction aléatoire


enum class SnakeGameState {
	Initialization, Run
};

class SnakeGame{
public:
	SnakeGame();
	void setup(Display *disp); // Add MotionInput, Communication et Player Manager Class

	virtual ~SnakeGame();
	bool run();
	//void handleRemote(SnakeGameMessage msg);

    // Getters pour le nouveau système
    MySnake* getMySnake() const { return mySnake.get(); }
    const std::vector<std::unique_ptr<Fruit>>& getFruits() const { return fruits; }
    int getFruitCount() const { return fruit_count; }
    
    // Getters utilise MySnake
    Head* getHead() const { 
        return mySnake ? mySnake->getHead() : nullptr; 
    }
    const std::vector<BodyPart>& getBody() const { 
        static std::vector<BodyPart> emptyBody;
        return mySnake ? mySnake->getBody() : emptyBody; 
    }
    
	void restart();
	
	// Public initialization methods
	void initializeSnake();
	void initializeFruits();
	
	// Méthodes de contrôle du serpent selon les spécifications du labo
	void moveSnake(int eat = 0);  // Faire avancer le serpent
	void turnSnakeLeft();         // Tourner à gauche
	void turnSnakeRight();        // Tourner à droite
	
	// Méthodes de jeu
	bool checkFruitCollision();   // Vérifier collision avec fruits
	bool checkWallCollision();    // Vérifier collision avec murs
	bool checkSelfCollision();    // Vérifier collision avec le corps
private:
	Display *disp = nullptr;
	//MotionInput *input = nullptr;
	//Communication *comm = nullptr;
	//PlayerManager *players = nullptr;
    
    // Système principal avec MySnake et Fruit GraphObjects
    std::unique_ptr<MySnake> mySnake;    // Le serpent principal
    std::vector<std::unique_ptr<Fruit>> fruits; // Les fruits comme GraphObjects
    
    std::unique_ptr<Checkboard> checkboard;
    bool newCheckboard = true;
    bool fruitEncountered;
    int fruit_count;
    int score;

    SnakeGameState state = SnakeGameState::Initialization;

	void initialize();
	
	// Helper functions
	int randomRange(int min, int max);
	bool isPositionFree(int x, int y);
};

#endif /* INC_GAME_SNAKEGAME_H_ */
