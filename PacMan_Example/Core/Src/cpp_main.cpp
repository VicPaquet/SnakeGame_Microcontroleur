/**
  * @file cpp_main.cpp
  * @date June 2025
  * @brief 
  * 
  * @defgroup ELE3312
  * @{
  */

#include <cpp_main.h>
#include "Game/Game.h"

using namespace ELE3312;

Game *p_game = nullptr;
#ifdef __cplusplus
extern "C" {
#endif

// Implemented in a different C++ file.
/** @brief Handles a single byte received via the serial interface (UART)
  * @param [in] data A data byte received via UART.
  */
void  handleUART(uint8_t data) {
	if (p_game != nullptr){
		p_game->handleUART(data);	
	}
}


/** @brief Handles an array of bytes received via the serial interface (UART)
  * @param [in] data A data byte received via UART.
  * @param [in] size The size (number of bytes) of the received data.
  */
void handleUARTData(uint8_t *data, uint16_t size){
	if(p_game != nullptr){
		p_game->handleUART(data, size);
	}
}


#ifdef __cplusplus
}
#endif

TIM_HandleTypeDef *distance_timer_handle;
Game game; // The object that implements the PacMan game.

/** @brief Main function that is run in C++ environment.
  * @param handlers A structure that contains pointers to the different micro controller 
  * peripherals used in the game.
  */
 void cpp_main(peripheral_handles *handles) {
	// This is our entry point to the C++ domain
	distance_timer_handle = handles->htim_distance;
	game.setup(handles);
	p_game = &game;
	game.run();

}


