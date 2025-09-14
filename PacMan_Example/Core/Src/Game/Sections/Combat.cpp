/**
  * @file Combat.cpp
  * @date June 2025
  * @brief This file implements the combat phase of the game.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "Game/Sections/Combat.h"
#include "Game/Graphics/GraphObjects/PacMan.h"
#include "Game/Graphics/GraphObjects/Ghost.h"
#include "Game/ComMessages/CombatMessage.h"
#include "main.h"
#include <cstdlib>

using namespace std::literals;

namespace ELE3312 {

/** @brief Setup routine of the Combat class.
  * @param [in] graph Pointer to a Graphics object that is used to display the graphical elements of the combat.
  * @param [in] sound Pointer to a Sound object that is used generate the sounds used in the combat phase.
  * @param [in] distance Pointer to a Distance object that is used to measure the distance of the players hand to the detector, and 
  * control the player's token.
  * @param [in] comm Pointer to a Communication object used to exchange the player progress with another player.
  * @param [in] players Pointer to a PlayerManager object that contains the meta data of the participating players.
  */
void Combat::setup(Display *disp, Sound *sound,
		AnalogInput *analogInput,
		Distance *distance,
		Communication *comm,
		PlayerManager *players) {
	sound->setWaveform(Waveform::Sinus);
	this->disp = disp;
	this->sound = sound;
	this->analogInput = analogInput;
	this->distance = distance;
	this->players = players;
	this->comm = comm;
}

/** @brief Constructor for the Combat class.
  */
Combat::Combat(){
}
/** @brief Initializes the combat game phase.
  */
void Combat::initialize(){
	GraphObject *playerCharacter;
	GraphObject *opponentCharacter;
	Color leftColor ;
	Color rightColor ;
	if (players->getPlayer().getCharacter() == PlayerCharacter::Pacman) {
		pacMan.setup(Rect{180, 150, 0, 0}, disp);
		playerCharacter = &pacMan;
		ghost.setup(Rect{{50, 60}, 100, 100 }, disp, Color::RED);
		opponentCharacter = &ghost;
		leftColor = Color::RED;
		rightColor = Color::YELLOW;
	} else {
		pacMan.setup(Rect{50, 150, 0, 0}, disp);
		opponentCharacter = &pacMan;
		ghost.setup(Rect{{180, 60}, 100, 100 }, disp, Color::RED);
		playerCharacter = &ghost;
		leftColor = Color::YELLOW;
		rightColor = Color::RED;
	}
	disp->clearScreen();

	// The left bar is currently always associated with the player
	leftBar.setup({{5,0}, 34, disp->getScreenHeight()}, disp,  Orientation::LEFT , leftColor);
	rightBar.setup({{288, 0}, 34, disp->getScreenHeight()}, disp, Orientation::RIGHT, rightColor);
	// Seed random generator
	std::srand(HAL_GetTick());
	// Initialize combat screen
	disp->drawString(80, 20, "Let's Fight !!!"s,  Color::WHITE);
	playerCharacter->draw();
	opponentCharacter->draw();

	rightBar.pickNewNote();
	leftBar.draw();
	rightBar.draw();
	state = CombatState::Run;
	distance->enableMeasurement();
	
}

/** @brief Updates the players progress/power bar.
  * @details This method is responsible to update the players progress/power bar depending 
  * on it's capacity to match his token with the target indicator. The method therefore 
  * reads the distance value, checks if the token and the target coincide, and updates all 
  * graphical objects involved.
  */
void Combat::updatePlayerBar(){
	float rawDistance = distance->getDistance();
	uint16_t playerPos = ceil(rawDistance*10.0f - 30.0f);
	rightBar.updatePlayerPosition(playerPos);
	if (rightBar.isMatched()){
		rightBar.incrementValue(1);
		// Send new bar value
		if(comm){
			CombatMessage msg;
			msg.setBarValue(rightBar.getValue());
			comm->send(&msg);
		}
		if(!sound->isEnabled()){
			sound->setNote(rightBar.getCurrentNote());
			sound->enableSound();
			sound->update();
		}
		if(rightBar.isNoteComplete()){
			rightBar.pickNewNote();
			if (rightBar.isDone()) {
				sound->disableSound();
				state = CombatState::PlayerWins;
				players->getPlayer().setWinner(true);
				players->getOpponent().setWinner(false);
				players->getOpponent().removeLive();
			}
		}
	} else {
		if(sound->isEnabled()){
			sound->disableSound();
		}
	}


}

/** @brief The run method controls the game logic and mechanics. 
  */
bool Combat::run(){
	uint16_t volume = 0;
	switch(state) {
		case CombatState::Initialization:
			initialize();
			state = CombatState::Run;
			// Run combat
		case CombatState::Run:
			analogInput->update();
			volume = analogInput->getPercentage();
			if(sound->getAmplitude() != volume){
				sound->setAmplitude(volume);
				sound->update();
			}
			updatePlayerBar();
			if (state != CombatState::Run) {
				state = CombatState::DisplayWinner;
			}
			break;

		case CombatState::DisplayWinner:
		case CombatState::OponentWins:
		case CombatState::PlayerWins:
			// Show Winner
			distance->disableMeasurement();

			state = CombatState::Initialization;
			return true; // Indicates that the combat stage has finished.
	}
	return false; // Indicates that the combat stage is not finished.
}

/** @brief The method handles incoming messages containing the 
  * progress of the opponent.
  * @param [in] msg A CombatMessage containing the progress of the opponent.
  */
void Combat::handleRemote(CombatMessage msg){
	if(!msg.isValid()){
		return;
	}
	leftBar.updateValue(msg.getBarValue());
	if (leftBar.isDone()){
		state = CombatState::OponentWins;
		players->getPlayer().setWinner(false);
		players->getPlayer().removeLive();
		players->getOpponent().setWinner(true);
	}
}

/** @brief Handles the input from the user by reading the corresponding sensor data.
  */
void Combat::handleUserInput(){
	distance->measuer();
}
} /* namespace ELE3312 */

/**
  * @}
  */ // End of documentation group ELE3312
