/*
 * Project : SnakeGame
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
Menu Game::menu;  // AJOUT DU MENU

// uart
Ringbuffer Game::uartBuffer;
uint8_t Game::buff[BUFFER_SIZE] = {0};

Game::Game() {}

/** @brief Constructor for the Game class.
  * @details The constructor creates a game object that encapsulates all aspects of the game.
  * @param [in] handles A struct with pointers to the different periphery interface handles.
  */
void Game::setup(peripheral_handles *handles) {
    this->handles = handles;

    // Setup des périphériques
    distance.setup(3.0f, 27.0f, handles->htim_distance);
    motionInput.setup(handles->hi2c);
    keypad.setup(handles->gpio_keypad);
    display.setup(handles->hspi_tft);
    display.clearScreen();
    uart.setup(handles->huart, 5);
    adcInput.setup(handles->hadc);

    // Setup audio
    sound.setup(handles->hdac, handles->htim_dac, 84000000UL);
    musicPlayer.setup(&sound);

    // Setup des sections de jeu
    menu.setup(&display, &keypad, &distance, &rgbLed);
    snakeGame.setup(&display, &keypad, &uart, &motionInput);


    // Démarrer le timer d'input capture pour le capteur de distance
    HAL_TIM_IC_Start_IT(handles->htim_distance, TIM_CHANNEL_1);

    // Activer la mesure de distance pour le menu
    distance.enableMeasurement();
}
void Game::run() {
    HAL_Delay(100);

    extern TIM_HandleTypeDef htim7;
    HAL_TIM_Base_Start_IT(&htim7);

    musicPlayer.playMelody(FRERE_JACQUES, 32);

    while(1) {
        switch(gameState) {
            case GameState::Menu: {
                bool menuDone = menu.run();

                if (menuDone) {
                    distance.disableMeasurement();

                    // ===== CONFIGURATION SELON LE CHOIX =====
                    if (menu.choiceSinglePlayer()) {
                        // MODE SINGLEPLAYER
                        snakeGame.setSinglePlayer(true);
                        snakeGame.setIsMaster(true);
                        snakeGame.setSeed(HAL_GetTick());
                    } else {
                        // MODE MULTIPLAYER
                        snakeGame.setSinglePlayer(false);
                        // is_master et seed seront définis dans waitForSyncAndGetSeed()
                    }

                    gameState = GameState::Play;
                }
                break;
            }

            case GameState::Play: {
                snakeGame.run();
                break;
            }

            case GameState::ResultScreen: {
                gameState = GameState::Menu;
                distance.enableMeasurement();
                break;
            }
        }

        musicPlayer.update();
    }
}


/** @brief Method that takes data provided by the uart interrupt.
  * @details The received data is stored internally and complete messages are
  * dispatched to the corresponding game sections.
  * @param [in] data A byte of data received from the serial interface.
  */
void Game::handleUART(uint8_t data) {
    uartBuffer.write(&data, 1);
}

/** @brief Method that takes data provided by the uart interrupt.
  * @details The received data is stored internally and complete messages are
  * dispatched to the corresponding game sections.
  * @param [in] data A byte array of data received from the serial interface.
  * @param [in] size The number of bytes in the buffer to be written.
  */
void Game::handleUART(uint8_t *data, uint16_t size) {
    uartBuffer.write(data, size);
}
