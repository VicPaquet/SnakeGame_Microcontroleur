/**
  * @file Combat.h 
  * @date June 2025
  * @brief Declares the Combat class that represents the combat phase of the game. 
  * 
  * @defgroup ELE3312
  * @{
  */

#ifndef COMBAT_H_
#define COMBAT_H_

#include <memory>
#include "Interfaces/Sound/Sound.h"
#include "Interfaces/AnalogInput/AnalogInput.h"
#include "Interfaces/Distance/Distance.h"
#include "Interfaces/Display/Display.h"
#include "Interfaces/Communication/Communication.h"
#include "Game/Graphics/GraphObjects/PowerBar.h"
#include "Game/Graphics/GraphObjects/PacMan.h"
#include "Game/Graphics/GraphObjects/Ghost.h"
#include "Game/Player/PlayerManager.h"
#include "Game/ComMessages/CombatMessage.h"

namespace ELE3312 {

/** @brief The CombatState represents the different states of the combat phase of the game.
  */
enum class CombatState {
	Initialization, Run, PlayerWins, OponentWins, DisplayWinner
};


/* @brief The Combat class contains all functions necessary for the combat 
 * part of the game. It has its own event loop and is responsible for
 * the drawing of the combat graphics and the user interactions with 
 * the game at this state. *
 */
class Combat {
	
public:
	Combat();
	void setup(Display *disp, Sound *sound, 
			AnalogInput *analogInput,
			Distance *distance,
			Communication *comm,
			PlayerManager *players);
	virtual ~Combat() = default;
	void initialize();
	bool run();
	void handleRemote(CombatMessage msg);
	void handleUserInput();
private:
	Display *disp = nullptr;
	Sound *sound = nullptr;
	AnalogInput *analogInput = nullptr;
	Distance *distance = nullptr;
	Communication *comm = nullptr;
	PlayerManager *players = nullptr;	
	PowerBar leftBar;
	PowerBar rightBar;
	CombatState state = CombatState::Initialization;
	uint32_t counter = 0;

	PacMan pacMan;
	Ghost ghost;

	void updatePlayerBar();
};

} /* namespace ELE3312 */

/**
  * @}
  */

#endif /* COMBAT_H_ */
