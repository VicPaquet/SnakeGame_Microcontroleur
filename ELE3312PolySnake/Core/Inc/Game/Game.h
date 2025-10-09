/**
  * Project Snake
  * file Game.h
  * date June 2025
  * brief Declares the Game class that combines the different parts of the game.
  *
  * ELE3312
  */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include "main.h"

#include "NucleoImp/AnalogInput/ADCInput.h"
#include "NucleoImp/Display/ILI9341Display.h"
#include "NucleoImp/Distance/HCSR04Distance.h"
#include "NucleoImp/Keypad/GPIOKeypad.h"
#include "NucleoImp/MotionInput/MPU6050MotionInput.h"
#include "NucleoImp/RGBLight/RGBLED.h"
//#include "NucleoImp/SerialCom/UART.h"
//#include "NucleoImp/Sound/DACSound.h"
//#include "NucleoImp/SerialCom/Ringbuffer.h"
//#include "Game/Sections/Menu.h"
//#include "Game/Sections/Combat.h"
//#include "Game/Sections/LabyrinthGame.h"
//#include "Game/Sections/VictoryScreen.h"
#include "cpp_main.h"
//#include "Game/Player/Player.h"
#include <memory>

#define BUFFER_SIZE 128

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Represents the entry point for the C++ implementation of the game.
  * @param handlers A structure that contains the different resource handles used to interact with
  * the peripherals of the micro controller.
  */
void main_run(peripheral_handles *handlers) ;

	/** @brief Represents the different phases of the game.
	  */
	enum class GameState {
		Menu, Play, ResultScreen
	};

	/* @biref The game class represents the PacMan game. It contains
	 * all aspects of the game as well as it's own event loop.
	 */

	class Game{
	public:
		Game();
		void setup(peripheral_handles *handles);
		virtual ~Game() = default;
		void run();
		void handleUART(uint8_t data);
		void handleUART(uint8_t *data, uint16_t size);
	private:
		peripheral_handles *handles = nullptr;
		// Peripherals
		static ADCInput adcInput;
		static MPU6050MotionInput motionInput;
		static ILI9341Display display;
		static HCSR04Distance distance;
		static GPIOKeypad keypad;
		static RGBLED rgbLed;
		//static UART uart;
		//static DACSound sound;
		// Players
		//static PlayerManager players; not used yet
		// Game sections
		/*
		static Menu menu;
		static PLay play;
		static ResultScreen victoryScreen;
		*/
		// State
		GameState state = GameState::Menu;
		// uart
		//Ringbuffer uartBuffer;

	};
	#ifdef __cplusplus
	 }
	#endif
	/**
	  * @}
	  */
	#endif /* INC_GAME_H_ */
