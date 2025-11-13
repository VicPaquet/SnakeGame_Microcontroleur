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


#include <stdint.h>
#include <cstddef>
#include <iostream>
#include <time.h>

#include "main.h"
#include <cpp_main.h>
#include "Game/Game.h"



// Définitions des variables globales
Game *p_game = nullptr;
Game game;

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


void cpp_main(peripheral_handles *handles) {
    game.setup(handles);
    p_game = &game;
    game.run();
}
