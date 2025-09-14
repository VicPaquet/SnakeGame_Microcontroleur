/**
  * @file VictoryScreen.cpp
  * @date June 2025
  * @brief This file implements the VictoryScreen part of the PacMan game. 
  * 
  * @defgroup ELE3312
  * @{
  */

#include "Game/Sections/VictoryScreen.h"
#include "Game/Graphics/GraphObjects/PacMan.h"
#include "Game/Graphics/GraphObjects/Ghost.h"

volatile extern uint32_t victory_screen_delay;

namespace ELE3312 {

	/**@brief Default constructor for the VictoryScreen class.
	  */
	VictoryScreen::VictoryScreen() {}

	/** @brief Setup routine  for the VictoryScreen class. 
	  * @param [in] graph A shared pointer to a Graphics object that is used to display graphics on the TFT screen.
	  * @param [in] comm A shared pointer to a Communication object that is used to exchange messages with another NUCLEO card.
	  * @param [in] players A shared pointer to a PlayerManager objects that encapsulates player meta data.
	  */
	void VictoryScreen::setup(Display *disp,
			Communication *comm,
			PlayerManager *players){
		this->disp = disp;
		this->comm = comm;
		this->players = players;
		state = VictoryState::Init;
	}




	/** @brief The run method updates the screen and controls the logic of the game at 
	  * this stage.
	  */
	bool VictoryScreen::run() {
		switch(state) {
			case VictoryState::Init:
				initialize();
				state = VictoryState::Run;
				victory_screen_delay = 0;
				break;
			case VictoryState::Run:
				if (victory_screen_delay == 1){
					state = VictoryState::Init;
					return true;
				}
				break;
		}
		return false;
	}

	/** @brief Initializes the victory screen.
	  */
	void VictoryScreen::initialize() {
		Player &winner = players->getPlayer().isWinner() ? players->getPlayer() : players->getOpponent();
		disp->clearScreen();
		char text[40] = {0};
		PacMan pacMan;
		Ghost  ghost;
		pacMan.setup(Rect(Point(100, 160), 50, 50), disp);
		ghost.setup({{100, 80}, 100, 100 }, disp, Color::RED);
		Color textColor = Color::WHITE;
		bool wasLastMatch = false;
		if(players->getPlayer().getNumberLives() == 0 || players->getOpponent().getNumberLives() == 0) {
			wasLastMatch = true;
		}
		switch (winner.getCharacter() ){
			case PlayerCharacter::Pacman:
				if(wasLastMatch) {
					sprintf(text, "The final winner is PacMan !");
				} else {
					sprintf(text, "The winner is PacMan !");
				}
				textColor = Color::YELLOW;
				pacMan.draw();
				break;
			case PlayerCharacter::Ghost :

				if(wasLastMatch) {
					sprintf(text, "The final winner is the Ghost !");
				} else {
					sprintf(text, "The final winner is the Ghost !");
				}
				textColor = Color::RED;
				ghost.draw();
				break;
			default:
				sprintf(text, "The Winner is Unknown ???");
				break;
		}
		disp->drawString(20, 20, text, textColor);
		sprintf(text, "%d Coins", winner.getCoins());
		disp->drawString(disp->getScreenWidth()/2, 180, text, textColor);
	}

} /* namespace ELE3312 */

/**
  * @}
  */
