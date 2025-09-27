/*
 * SnakeGame.h
 *
 *  Created on: Sep 27, 2025
 *      Author: vicpa
 */

#ifndef INC_GAME_SNAKEGAME_H_
#define INC_GAME_SNAKEGAME_H_

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
	void restart();
private:
	Display *disp = nullptr;
	//MotionInput *input = nullptr;
	//Communication *comm = nullptr;
	//PlayerManager *players = nullptr;


};

#endif /* INC_GAME_SNAKEGAME_H_ */
