/*
 * Project : SnakeGame `
 * Game.cpp
 *
 *  Created on: Oct 3, 2025
 *      Author: hugod
 */

#include "Game/Game.h"
#include "Game/Sections/MusicPlayer.h"
#include "Game/Sections/SnakeGame.h"
#include "interfaces/sound/Melodie.h"
#include "NucleoImp/SerialCom/SerialFrame.h"
#include "NucleoImp/RGBLight/RGBLED.h"
#include "NucleoImp/AnalogInput/ADCInput.h"
#include "NucleoImp/Keypad/GPIOKeypad.h"

// Peripherals
ADCInput Game::adcInput;
MPU6050MotionInput Game::motionInput;
ILI9341Display Game::display;
HCSR04Distance Game::distance;
GPIOKeypad Game::keypad;
RGBLED Game::rgbLed;
UART Game::uart;


SnakeGame Game::snakeGame;
DACSound Game::sound;
MusicPlayer Game::musicPlayer;

// Players
//PlayerManager Game::players;
// Game sections
/*
Menu Game::menu;
LabyrinthGame Game::labyrinth;
Combat Game::combat;
VictoryScreen Game::victoryScreen;
*/

// uart
Ringbuffer Game::uartBuffer;
uint8_t Game::buff[BUFFER_SIZE]= {0};



Game::Game(){}

/** @brief Constructor for the Game class.
  * @details The constructor creates a game object that encapsulates all aspects of the game.
  * @param [in] handles A struct with pointers to the different periphery interface handles.
  */

void Game::setup(peripheral_handles *handles) {
	 this->handles = handles;
	 motionInput.setup(handles->hi2c);
	 snakeGame.setup(&display,&keypad, &uart, &motionInput);
	 keypad.setup(handles->gpio_keypad);
	 display.setup(handles->hspi_tft);
	 display.clearScreen();
	 uart.setup(handles->huart, 5);

	 adcInput.setup(handles->hadc);

	 sound.setup(handles->hdac, handles->htim_dac, 84000000UL);
	 musicPlayer.setup(&sound);
}

void Game::run(){
	HAL_Delay(100);
	musicPlayer.playMelody(FRERE_JACQUES, 32);

	while(1){
		snakeGame.run();
		musicPlayer.update();
	}
}


/** @brief Method that takes data provided by the uart interrupt.
  * @details The received data is stored internally and complete messages are
  * dispatched to the corresponding game sections.
  * @param [in] data A byte of data received from the serial interface.
  */
void Game::handleUART(uint8_t data){
	uartBuffer.write(&data, 1);
}

/** @brief Method that takes data provided by the uart interrupt.
  * @details The received data is stored internally and complete messages are
  * dispatched to the corresponding game sections.
  * @param [in] data A byte array of data received from the serial interface.
  * @param [in] size The number of bytes in the buffer to be written.
  */
void Game::handleUART(uint8_t *data, uint16_t size){
	uartBuffer.write(data, size);
}






