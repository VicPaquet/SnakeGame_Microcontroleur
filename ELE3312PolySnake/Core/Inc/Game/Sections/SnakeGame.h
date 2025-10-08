/*
 * SnakeGame.h
 *
 *  Created on: Sep 27, 2025
 *      Author: vicpa
 */

#ifndef INC_GAME_SNAKEGAME_H_
#define INC_GAME_SNAKEGAME_H_

#include "Interfaces/Display/Display.h"
#include "Game/Graphics/GraphObjects/Head.h"
#include "Game/Graphics/GraphObjects/BodyPart.h"
#include "Game/Graphics/GraphObjects.h"
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

struct tile {
    uint16_t x;
    uint16_t y;
    bool active;
};


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

    Head* getHead() const { return head; }
    const std::vector<BodyPart>& getBody() const { return body; }
    const tile* getFruits() const { return fruits; }
    int getFruitCount() const { return fruit_count; }
	void restart();
	
	// Public initialization methods
	void initializeSnake();
	void initializeFruits();
private:
	Display *disp = nullptr;
	//MotionInput *input = nullptr;
	//Communication *comm = nullptr;
	//PlayerManager *players = nullptr;
    Head* head = nullptr;                // Pointeur vers la tête du serpent
    std::vector<BodyPart> body; // Vecteur contenant le corps du serpent
    tile fruits[MAX_FRUITS];
    std::unique_ptr<Checkboard> checkboard;
    bool newCheckboard = true;
    bool fruitEncountered;
    int fruit_count;
    int score;

    SnakeGameState state = SnakeGameState::Initialization;

	void initialize();
	bool updateSnakePosition(float x, float y);
	
	// Helper functions
	int randomRange(int min, int max);
	bool isPositionFree(int x, int y);
};

#endif /* INC_GAME_SNAKEGAME_H_ */
