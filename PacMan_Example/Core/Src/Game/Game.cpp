/**
  * @file Game.cpp
  * @date June 2025
  * @brief This file implements the main game logic. 
  * 
  * @defgroup ELE3312
  * @{
  */
#include "Game/Game.h"
#include "NucleoImp/SerialCom/SerialFrame.h"
#include "NucleoImp/RGBLight/RGBLED.h"
#include "NucleoImp/AnalogInput/ADCInput.h"
#include "NucleoImp/Keypad/GPIOKeypad.h"


using namespace ELE3312;
// Peripherals
ADCInput Game::adcInput;
MPU6050MotionInput Game::motionInput;
ILI9341Display Game::display;
HCSR04Distance Game::distance;
GPIOKeypad Game::keypad;
RGBLED Game::rgbLed;
UART Game::uart;
DACSound Game::sound;
// Players
PlayerManager Game::players;
// Game sections
Menu Game::menu;
LabyrinthGame Game::labyrinth;
Combat Game::combat;
VictoryScreen Game::victoryScreen;

/** @brief Default constructor for the Game class.
  */
 Game::Game(){
 }

/** @brief Constructor for the Game class.
  * @details The constructor creates a game object that encapsulates all aspects of the game.
  * @param [in] handles A struct with pointers to the different periphery interface handles.
  */
 void Game::setup(peripheral_handles *handles) {
	 this->handles = handles;
	 sound.setup(handles->hdac, handles->htim_dac, 84000000UL);
	 distance.setup(3.0f, 27.0f, handles->htim_distance);
	 motionInput.setup(handles->hi2c);
	 uart.setup(handles->huart, 5);
	 display.setup(handles->hspi_tft);
	 rgbLed.setup(handles->htim_led, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4);
	 adcInput.setup(handles->hadc);
	 display.clearScreen();
	 keypad.setup(handles->gpio_keypad);
	 // Game Parts
	 menu.setup(&display, &keypad, &uart, &rgbLed, &players);
	 labyrinth.setup(&display, &motionInput, &uart, &players);
	 combat.setup(&display, &sound, &adcInput, &distance, &uart, &players);
	 victoryScreen.setup(&display, &uart, &players);
	 uartBuffer.setup(512);
 }

extern volatile uint16_t game_delay;

 /** @brief The run method starts the game.
   * @details The method starts the game. It contains it's own event loop. 
   */
void Game::run(){
	uint8_t buff[BUFFER_SIZE]= {0};
	SerialFrame frame;
	while(1){
		// Check UART and dispatch messages
		if (uartBuffer.read(buff, BUFFER_SIZE) != 0) {
			frame.setMessage(buff, BUFFER_SIZE);

			switch (frame.getMessageType()){
				case MessageType::PlayerChoice:
					if (state == GameState::Menu) { // Only dispatch messages for the current state
						menu.handleRemote(frame.getPlayerChoiceMessage());
					}
					break;
				case MessageType::Position :
					if (state == GameState::Labyrinth) { // Only dispatch messages for the current state
						labyrinth.handleRemote(frame.getLabyrinthMessage());
					}
					break;

				case MessageType::PowerValue :
					if (state == GameState::Combat) { // Only dispatch messages for the current state
						combat.handleRemote(frame.getCombatMessage());
					}
					break;

				default:
					break;
			}

		}
		// Handle user input

		switch(state){
			case GameState::Menu:
				keypad.update();
				break;
			case GameState::Labyrinth:
				break;
			case GameState::Combat:
				combat.handleUserInput();
				break;
			case GameState::VictoryScreen:
				break;
		}
		// Update game 
		if (game_delay == 1) {
			game_delay = 0;

			switch(state){
				case GameState::Menu:
					if(menu.run()){
						state = GameState::Labyrinth;
					} 
					break;

				case GameState::Labyrinth:
					if (labyrinth.run() ){
						state = GameState::Combat;
					} 
					break;

				case GameState::Combat:
					if (combat.run()) {
						state = GameState::VictoryScreen;
					} 
					break;

				case GameState::VictoryScreen:
					if (victoryScreen.run()){
						if(players.getPlayer().getNumberLives() == 0 || players.getOpponent().getNumberLives() == 0) {
							state = GameState::Menu;
							// Reset the labyrinth
							labyrinth.restart();
						} else {
							state = GameState::Labyrinth;
						}
					}
					break;
			}
		}
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
/**
  * @}
  */
