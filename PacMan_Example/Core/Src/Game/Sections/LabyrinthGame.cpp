/**
  * @file Game.cpp
  * @date June 2025
  * @brief This file implements labyrinth part of the PacMan game.
  * @defgroup ELE3312
  * @{
  */

#include "Game/Sections/LabyrinthGame.h"
#include "Game/ComMessages/LabyrinthMessage.h"
#include "main.h"
#include <stdio.h>

namespace ELE3312 {
LabyrinthGame::LabyrinthGame() {}

/** @brief Costructor for the Labyrinth game section
  * @param [in] disp Pointer to a Display object that is used to display the game graphics.
  * @param [in] input Pointer to a MotionInput object that is used to handle user movement input.
  * @param [in] comm Pointer to a Communication object that is used to exchange game data with another player.
  * @param [in] players Pointer to a PlayerManager object that manages the meta data of all players
 */
void LabyrinthGame::setup(Display *disp, MotionInput *input,
		Communication *comm,
		PlayerManager *players){
	this->disp = disp;
	this->input = input;
	this->comm = comm;
	this->players = players;
	opponentEncountered = false;
}

/** @brief Initializes the labyrinth game part.
  */
void LabyrinthGame::initialize() {
	if(newMaze){
		maze.setup(Rect{{0, topHeight}, disp->getScreenWidth(), (uint16_t)(disp->getScreenHeight() - topHeight)}, disp, players);
		newMaze = false;
	}else {
		Player *p = &players->getPlayer();
		p->setX(p->getStartPositionX());
		p->setY(p->getStartPositionY());
		p = &players->getOpponent();
		p->setX(p->getStartPositionX());
		p->setY(p->getStartPositionY());
	}
	opponentEncountered = false;
	maze.setNumberLives(players->getPlayer().getNumberLives());
	maze.draw();
}

/** @brief The helper function determines if the last digit 
  * is a 5.
  * @param value The value that is going to be checked.
  * @retval true The last digit of the value is a 5.
  * @retval false The last digit of the value is not a 5.
  */
template<typename T>
bool endsOnFive(T value){
	return ( value % 5) == 0 && (value & 0x1) == 0x1;
}

/** @brief The helper function returns the sign of the specified
  * numerical value.
  * @param value The numeric value of which the sign is going to be determined.
  * @return The sign (-1, 0, 1) of the specified value.
  */
template<typename T>
int sign(T value) {
	return ((T(0) < value) - ( value < T(0)));
}

/** @brief This method takes a new position change of the player and updates it's position 
  * on the screen if the new position is valid (does not intersect with a solid object).
  * @param [in] x The position change in x direction.
  * @param [in] y The position change in y direction.
  * @retval true The player has encountered his opponent.
  * @retval false The player has not encountered his opponent.
  */
bool LabyrinthGame::updatePlayerPosition(float x, float y){
	// Manage input sensibility
	float sensibility = 0.1f;
	if ( ((x > 0 && x < sensibility) || ( x < 0 && x > - sensibility)) &&
	     ((y > 0 && y < sensibility) || ( y < 0 && y > - sensibility))) {
		return false;
	}
	//printf("x=%f, y=%f\r\n", x, y);
	Player *p = &players->getPlayer();
	bool xInputIsGreater =  x*x > y*y ;
	uint16_t newXPos = p->getX() + sign(x);
	uint16_t newYPos = p->getY() + sign(y);
	uint16_t xPos = p->getX();
	uint16_t yPos = p->getY();
	
	Collision cType  = {CollisionType::Unknown, 0, 0};
	Collision checkX = maze.checkPosition(newXPos, yPos, p, &players->getOpponent()); 
	Collision checkY = maze.checkPosition(xPos, newYPos, p, &players->getOpponent()); 
	CollisionType cTypeX = checkX.type;
	CollisionType cTypeY = checkY.type;

	if(xPos > 200 && yPos > 200) {
	__asm("BKPT");
	}
	// Check if the player enters a tunnel    // 2280521 & 2283434
	if (cTypeY == CollisionType::TunnelTop) {
		newYPos = disp->getScreenHeight() - p->getSprite()->getHeight();
		p->updatePosition(p->getX(), newYPos);
		cType.type = CollisionType::None;
	}

	if (cTypeY == CollisionType::TunnelButtom) {
		newYPos = maze.getRect().getY1() +  p->getSprite()->getHeight();
		p->updatePosition(p->getX(), newYPos);
		cType.type  = CollisionType::None;
	}

	if (cTypeX == CollisionType::TunnelLeft) {
		newXPos = disp->getScreenWidth() -  p->getSprite()->getWidth();
		p->updatePosition(newXPos, p->getY());	
		cType.type  = CollisionType::None;
	}

	if (cTypeX == CollisionType::TunnelRight) {
		newXPos = p->getSprite()->getWidth();
		p->updatePosition(newXPos, p->getY());	
		cType.type  = CollisionType::None;
	}

	// Check for permitted movements
	if( xInputIsGreater && endsOnFive(yPos) && (cTypeX == CollisionType::None || cTypeX == CollisionType::Coin)) {
				p->updatePosition(newXPos, p->getY());	
				cType.type  = cTypeX;
				if(cTypeX == CollisionType::Coin){
					cType = checkX;
				}
	}

	if (!xInputIsGreater && endsOnFive(yPos) && (cTypeX == CollisionType::None || cTypeX == CollisionType::Coin) && (cTypeY == CollisionType::Wall || !endsOnFive(xPos))){
				p->updatePosition(newXPos, p->getY());	
				cType.type  = cTypeX;
				if(cTypeX == CollisionType::Coin){
					cType = checkX;
				}
	}

	if (!xInputIsGreater && endsOnFive(xPos) && (cTypeY == CollisionType::None || cTypeY == CollisionType::Coin)) {
				p->updatePosition(p->getX(), newYPos);
				cType.type  = cTypeY;
				if(cTypeY == CollisionType::Coin){
					cType = checkY;
				}
	}

	if (xInputIsGreater && endsOnFive(xPos) && (cTypeY == CollisionType::None || cTypeY == CollisionType::Coin) && (cTypeX == CollisionType::Wall ||  !endsOnFive(yPos))){
				p->updatePosition(p->getX(), newYPos);
				cType.type  = cTypeY;
				if(cTypeY == CollisionType::Coin){
					cType = checkY;
				}
	}

	// Check if the player encounters it's opponent
	if (cTypeX == CollisionType::Opponent || cTypeY == CollisionType::Opponent){
		cType.type  = CollisionType::Opponent;
	}

	if (cType.type  == CollisionType::Coin){
		char msg[60] = {0};   // 2280521 & 2283434
		sprintf(msg, "2280521 & 2283434 - Collision Coin x=%f, y=%f", input->getX(), input->getY());
		printf("%s\r\n", msg);


		// Remove Coin from maze
		maze.removeCoin(cType.x, cType.y);
		// Increment player's coins
		p->addCoin();
		// Show new coins score
		char buff[20] = {};
		sprintf(buff, "%d", p->getCoins());
		disp->drawString(0,0,buff);

		// Send position of coin 
		if(comm){

			LabyrinthMessage msg(cType.x, 
					cType.y, cType.type);
			comm->send(&msg);
		}
		cType.type = CollisionType::None;
	}


	// Transmit new position to opponent
	if(cType.type == CollisionType::None ) {
		maze.update();
		// Send new position
		if(comm){
			LabyrinthMessage msg(p->getX(), 
					p->getY());
			comm->send(&msg);
		}
		return false;
	}
	if(cType.type == CollisionType::Opponent){
		if(comm){
			LabyrinthMessage msg(p->getX(),
					p->getY());
			msg.setCollisionType(cType.type);
			comm->send(&msg);
		}
		return true;
	}
	return false;
}

/** @brief This method runs the labyrinth game part for a single frame.
  */
bool LabyrinthGame::run() {
	switch(state){
		case LabyrinthGameState::Initialization:
			initialize();
			state = LabyrinthGameState::Run;
		case LabyrinthGameState::Run:
		// Read movement change from accelerometer
		input->update();
		if(updatePlayerPosition(input->getY(), input->getX()) || opponentEncountered){
			// Found ghost
			state = LabyrinthGameState::Initialization;
			return true; // Run finished
		}
	}
	return false; // Run incomplete
}

/** @brief This method handles incoming LabyrinthMessage message's.
  * @param [in] msg A shared pointer to a new LabyrinthMessage message.
  */
void LabyrinthGame::handleRemote(LabyrinthMessage msg){
	if(!msg.isValid()){
		return;
	}
	switch(msg.getCollisionType()){
		case CollisionType::Opponent:
			players->getOpponent().updatePosition(msg.getX(), msg.getY());
			opponentEncountered = msg.getCollisionType() == CollisionType::Opponent;
			maze.update();
	break;
		case CollisionType::Coin:
			maze.removeCoin(msg.getX(), msg.getY());
			break;
	default:
			break;
	}
}

/** @brief Reinitializes the maze for a new game.
 */
void LabyrinthGame::restart(){
	newMaze= true;
}

} /* End of namespace ELE3312 */

/**
  * @}
  */
