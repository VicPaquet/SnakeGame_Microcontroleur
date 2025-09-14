#ifndef VICTORYSCREEN_H_
#define VICTORYSCREEN_H_

/**
  * @file VictoryScreen.h
  * @date June 2025
  * @brief The file contains the declaration of the VictoryScreen class that represents
  * the part of the game where the winner of the combat and/or overall game is shown and
  * celebrated.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "Interfaces/Display/Display.h"
#include "Interfaces/Communication/Communication.h"
#include "Game/Player/PlayerManager.h"

namespace ELE3312 {

	/** @brief Represents the different states of the menu phase of the game.
	  */
	enum class VictoryState {
		Init, Run
	};

/** @brief The Menu class represents the menu that is displayed at the beginning of the game
  * where the player chooses it's game character (PacMan or Ghost).
  */
class VictoryScreen {
public:
	VictoryScreen();
	void setup(Display *disp,
			Communication *comm,
			PlayerManager *players);
	virtual ~VictoryScreen() = default;

	bool run();
private:
	void initialize();
	void update();
	Display *disp = nullptr;
	Communication *comm = nullptr;
	PlayerManager *players = nullptr;
	VictoryState state = VictoryState::Init;
};

} /* namespace ELE3312 */

/**
  * @}
  */

#endif /* VICTORYSCREEN_H_ */
