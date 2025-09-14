/**
  * @file Menu.cpp
  * @date June 2025
  * @brief This file implements the Menu part of the PacMan game. 
  * 
  * @defgroup ELE3312
  * @{
  */
#include "Game/Sections/Menu.h"
#include "Game/Graphics/GraphObjects/Ghost.h"
#include "Game/Graphics/GraphObjects/PacMan.h"
#include "Game/ComMessages/PlayerChoiceMessage.h"
#include "main.h"


namespace ELE3312 {

	/**@brief Default constructor for the Menu class.
	  */
	Menu::Menu() {}

/** @brief Setup routine for a Menu object.
  * @param graph Pointer to a Graphics object that is used to display the menu graphics.
  * @param keypad Pointer to a Keypad object that is used to select the player's game character.
  * @param comm Pointer to a Communication object that is used to exchange messages with another player.
  * @param rgb Pointer to an RGBLicht object, used to indicate the player's color.
  * @param players Pointer to a PlayerManager object that manages player meta data.
  */
void Menu::setup(Display *disp,
		Keypad *keypad,
		Communication *comm,
		RGBLight *rgb,
		PlayerManager *players) {
	this->disp = disp;
	this->keypad = keypad;
	this->comm = comm;
	this->rgb = rgb;
	this->players = players;
	x = cursorStartPosX; // x position of the indicator token
	y = cursorStartPosY; // y position of the indicator token

}

/** @brief Method that contains the mechanics of the menu. It will update one frame.
  * @retval true Indicates that the player has chosen a game character.
  * @retval false Indicates that the player has not yet chosen a game character.
  */
bool Menu::run(){
	switch(state){
		case MenuState::Initialization:
			//printf("\n2280521 & 2283434 - Entrée dans l'état : Initialization\n\r"); // 2280521 & 2283434
			initialize();
			//printf("2280521 & 2283434 - Sortie dans l'état : Initialization\n\r"); // 2280521 & 2283434
			state = MenuState::Run;
			break;
		case MenuState::Run:
			//printf("2280521 & 2283434 - Entrée dans l'état : Run\n\r"); // 2280521 & 2283434
			if (players->getPlayer().getCharacter() != PlayerCharacter::Unknown){
				state = MenuState::Initialization;
				//printf("2280521 & 2283434 - Sortie dans l'état : Run\n\r"); // 2280521 & 2283434
				disp->clearScreen();
				return true;
			}

			uint16_t step = 0;
			if(keypad->isAnnyKeyPressed()){
			   switch(keypad->getFirstKeyPressed()){
				   case KeyCode::FOUR:
					   step = -2;
					   break;
				   case KeyCode::SIX:
					   step = 2;
					   break;
				   default :
					step = 0;
			   }
			}

			if (step != 0){
				// update pointer
				disp->fillCircle(Color::BLACK, x, y, circleboxRadiusCursor); // Remove previous pointer
				x += step;
				disp->fillCircle(Color::ORANGE, x, y, circleboxRadiusCursor); // Draw pointer at new x position
			}
			if (x < 76) {
				// Ghost was chosen
				choiceGhost_ = true;
				disp->drawRect(Color::MAGENTA, chbxLeftX, chbxLeftY, checkboxWidth, checkboxHeight);
				players->getOpponent().setCharacter(PlayerCharacter::Pacman);
				players->getPlayer().setCharacter(PlayerCharacter::Ghost);
				if(comm){
					PlayerChoiceMessage msg(PlayerCharacter::Ghost);
					comm->send(&msg);
				}
				state = MenuState::Initialization;
				rgb->setColorRGB(50, 0, 0);
				printf("2280521 & 2283434 - Sortie dans l'état : Run x < 76\n\r"); // 2280521 & 2283434
				disp->clearScreen();
				return true; // Run completed
			} else if (x > 230) {
				// PacMan was chosen
				choiceGhost_ = false;
				disp->drawRect(Color::MAGENTA, chbxRightX, chbxRightY, checkboxWidth, checkboxHeight);
				players->getPlayer().setCharacter(PlayerCharacter::Pacman);
				players->getOpponent().setCharacter(PlayerCharacter::Ghost);
				if(comm){
					PlayerChoiceMessage msg(PlayerCharacter::Pacman);
					comm->send(&msg);
				}
				state = MenuState::Initialization;
				rgb->setColorRGB(100,50, 0);
				printf("2280521 & 2283434 - Sortie dans l'état : Run x > 230 \n\r"); // 2280521 & 2283434
				disp->clearScreen();
				return true; // Run completed
			} else {
				disp->drawCircle(Color::ORANGE, circleboxLeftX, circleboxLeftY, circleboxRadius);
				disp->drawCircle(Color::ORANGE, circleboxRightX, circleboxRightY, circleboxRadius);
			}
	}
	return false; // Run incomplete
}

/** @brief Initializes the menu.
  */
void Menu::initialize(){
	// Initialize the two possible players 
	printf("2280521 & 2283434 - Intialisation tâche 2"); // 2280521 & 2283434
	players->getPlayer().reset();
	players->getOpponent().reset();

	// Set initial position for the choice token that is controlled by the user
	// with the accelerometer
	x = cursorStartPosX;
	y = cursorStartPosY;
	// Draw the graphics of the menu screen
	PacMan pacMan;
	Ghost  ghost;
	pacMan.setup({{190, 130}, 50, 50}, disp);
	ghost.setup( {{41, 40}, 100, 100 }, disp, Color::RED);
	disp->clearScreen();
	pacMan.draw();
	ghost.draw();
	disp->drawCircle(Color::ORANGE, circleboxLeftX, circleboxLeftY, circleboxRadius);
	disp->drawCircle(Color::ORANGE, circleboxRightX, circleboxRightY, circleboxRadius);
	disp->fillCircle(Color::ORANGE, x, y, circleboxRadiusCursor);

	disp->drawString(0, 0, "2283434 & 2280521", Color::ORANGE) ;

	rgb->setColorRGB(50, 50, 50); // Whiteish
	rgb->turnOn();
}

/** @brief Handles incoming messages.
  * @param [in] msg PlayerChoiceMessage containing the character choice of the other player.
  */
void Menu::handleRemote(PlayerChoiceMessage msg){
	if (msg.getPlayerChoice() == PlayerCharacter::Pacman) {
		players->getOpponent().setCharacter(PlayerCharacter::Pacman);
		players->getPlayer().setCharacter(PlayerCharacter::Ghost);
	} else {
		players->getOpponent().setCharacter(PlayerCharacter::Ghost);
		players->getPlayer().setCharacter(PlayerCharacter::Pacman);
	}
}
} /* namespace ELE3312 */

/**
  * @}
  */
