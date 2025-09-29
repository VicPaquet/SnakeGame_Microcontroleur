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
#include <vector>


enum class SnakeGameState {
	Initialization, Run
};

class SnakeGame{
public:
	SnakeGame();
	void setup(Display *disp); // Add MotionInput, Communication et Player Manager Class

	virtual ~SnakeGame() = default;
	bool run();
	//void handleRemote(SnakeGameMessage msg);

    Head* getHead() const { return head; }
    std::vector<BodyPart>* getBody() const { return body; }
	void restart();
private:
	Display *disp = nullptr;
	//MotionInput *input = nullptr;
	//Communication *comm = nullptr;
	//PlayerManager *players = nullptr;
    Head* head;                // Pointeur vers la tête du serpent
    std::vector<BodyPart> body; // Pointeur vers le vecteur contenant le corps
    tile fruits[MAX_FRUITS];
    int fruit_count;
    int score;

};

#endif /* INC_GAME_SNAKEGAME_H_ */
