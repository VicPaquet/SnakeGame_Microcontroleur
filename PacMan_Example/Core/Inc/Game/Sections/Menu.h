#ifndef MENU_H_
#define MENU_H_

/**
  * @file Menu.h
  * @date June 2025
  * @brief Declares the Menu class that represents the menu phase of the game where 
  * the player chooses it's game character.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "Interfaces/Display/Display.h"
#include "Interfaces/Keypad/Keypad.h"
#include "Interfaces/RGBLight/RGBLight.h"
#include "Interfaces/Communication/Communication.h"
#include "Game/Player/PlayerManager.h"
#include "Game/ComMessages/PlayerChoiceMessage.h"
#include <memory>

namespace ELE3312 {
	/** @brief Represents the different states of the menu phase of the game.
	  */
	enum class MenuState {
		Initialization, Run
	};

/** @brief The Menu class represents the menu that is displayed at the beginning of the game
  * where the player chooses it's game character (PacMan or Ghost).
  */
class Menu {
public:
	Menu();
	void setup(Display *disp,
			Keypad *keypad,
			Communication *comm,
			RGBLight *rgb,
			PlayerManager *players);
	virtual ~Menu() = default;

	bool run();
	bool choiceGhost() const { return choiceGhost_; }
	bool choicePackman() const { return !choiceGhost_; }
	void handleRemote(PlayerChoiceMessage msg);
private:
	void initialize();
	Display *disp = nullptr;
	Keypad *keypad = nullptr;
	Communication *comm = nullptr;
	RGBLight *rgb = nullptr;
	PlayerManager *players = nullptr;
	uint16_t x = 0;
	uint16_t y = 0;
	const uint16_t cursorWidth = 30;
	const uint16_t cursorHeight = 30;
	const uint16_t checkboxWidth = 40;

	const uint16_t circleboxRadius = 25;
	const uint16_t circleboxRadiusCursor = 20;
	const uint16_t circleboxLeftX = 90;
	const uint16_t circleboxLeftY = 170;
	const uint16_t circleboxRightX = 235;
	const uint16_t circleboxRightY = 170;


	const uint16_t checkboxHeight = 40;
	const uint16_t chbxLeftX = 70;
	const uint16_t chbxLeftY = 150;
	const uint16_t chbxRightX = 225;
	const uint16_t chbxRightY = 150;

	const uint16_t cursorStartPosX = 160;
	const uint16_t cursorStartPosY = 170;
	bool choiceGhost_ = false;
	MenuState state = MenuState::Initialization;
};

} /* namespace ELE3312 */

/**
  * @}
  */

#endif /* MENU_H_ */
