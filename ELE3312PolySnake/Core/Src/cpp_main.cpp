/**
  * @file cpp_main.cpp
  * @date September 2025
  * @brief Test préliminaire de génération du snake sur l'écran
  * 
  * Ce fichier teste l'initialisation et l'affichage du jeu Snake
  * avec la nouvelle architecture orientée objet.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "main.h"
#include <cpp_main.h>
#include <NucleoImp/Display/ILI9341Display.h>
#include "Game/Sections/SnakeGame.h"
#include "Game/Game.h"

Game *p_game = nullptr;
#ifdef __cplusplus
extern "C" {
#endif


void  handleUART(uint8_t data) {
	if (p_game != nullptr){
		p_game->handleUART(data);
	}
}

void handleUARTData(uint8_t *data, uint16_t size){
	if(p_game != nullptr){
		p_game->handleUART(data, size);
	}
}


#ifdef __cplusplus
}
#endif


// Instance globale du jeu Snake

//TIM_HandleTypeDef *distance_timer_handle;
ILI9341Display display;
Game game;

void cpp_main(peripheral_handles *handles) {
	//distance_timer_handle = handles->htim_distance;
    game.setup(handles);
	p_game = &game;
	game.run();
}
