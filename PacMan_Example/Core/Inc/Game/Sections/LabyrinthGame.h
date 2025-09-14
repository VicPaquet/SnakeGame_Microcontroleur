/**
  * @file LabyrinthGame.h 
  * @date June 2025
  * @brief Declares the LabyrinthGame class that represents the labyrinth path 
  * of the game where the players collect coins and try to catch each other.
  * 
  * @defgroup ELE3312
  * @{
  */

#ifndef LABYRINTHGAME_H_
#define LABYRINTHGAME_H_

#include "Interfaces/MotionInput/MotionInput.h"
#include "Interfaces/Display/Display.h"
#include "Game/Player/PlayerManager.h"
#include "Interfaces/Communication/Communication.h"
#include "Game/ComMessages/LabyrinthMessage.h"
#include "Game/Graphics/GraphObjects/Maze.h"
#include <memory>
#include <vector>

namespace ELE3312 {
	/** @brief Represent the different states of the labyrinth game phase.
	  */
	enum class LabyrinthGameState {
		Initialization, Run
	};

/** @brief Represents the labyrinth phase of the game where the players collect 
  * coins and try to catch each other in the displayed maze.
  */
class LabyrinthGame {
public:
	LabyrinthGame();	
	void setup(Display *disp, 
			MotionInput *input,
			Communication *comm,
			PlayerManager *players);
	virtual ~LabyrinthGame() = default;
	bool run();
	void handleRemote(LabyrinthMessage msg);
	void restart();
private:
	Display *disp = nullptr;
	MotionInput *input = nullptr;
	Communication *comm = nullptr;
	PlayerManager *players = nullptr;
	Maze maze;
	bool newMaze = true;
	bool opponentEncountered; //!< Indicates that the opponent was encountered.
	LabyrinthGameState state = LabyrinthGameState::Initialization;
	const uint16_t topHeight = 20;


	void initialize();
	bool updatePlayerPosition(float x, float y);


};

} /* End of namespace ELE3312 */

/**
  * @}
  */

#endif /* LABYRINTHGAME_H_ */
