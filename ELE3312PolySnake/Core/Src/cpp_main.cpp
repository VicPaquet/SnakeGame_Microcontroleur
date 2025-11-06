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
ILI9341Display display;
Game game;

// Fonction pour attendre la synchronisation et obtenir le seed
uint32_t waitForSyncAndGetSeed(peripheral_handles *handles) {
    uint32_t seed = 0;
    uint8_t seed_bytes[4];
    bool is_master = false;

    // Initialiser l'écran pour afficher le message d'attente
    display.setup(handles->hspi_tft);
    display.fillScreen(Color::BLACK);
    display.drawString(50, 100, "Appuyez sur un bouton", Color::WHITE);
    display.drawString(70, 120, "pour demarrer", Color::WHITE);

    // Boucle d'attente
    while (1) {
        // Vérifie si un bouton est pressé (exemple avec le bouton bleu)
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET) {
            // Ce microcontrôleur sera le maître
            seed = HAL_GetTick();
            seed_bytes[0] = (seed >> 24) & 0xFF;
            seed_bytes[1] = (seed >> 16) & 0xFF;
            seed_bytes[2] = (seed >> 8) & 0xFF;
            seed_bytes[3] = seed & 0xFF;
            
            // Envoyer le seed à l'autre microcontrôleur
            HAL_UART_Transmit(handles->huart, seed_bytes, 4, HAL_MAX_DELAY);
            is_master = true;
            break;
        }

        // Vérifie si un seed est reçu
        if (HAL_UART_Receive(handles->huart, seed_bytes, 4, 10) == HAL_OK) {
            seed = (seed_bytes[0] << 24) | (seed_bytes[1] << 16) | 
                   (seed_bytes[2] << 8) | seed_bytes[3];
            is_master = false;
            break;
        }
    }

    // Affiche un message de confirmation
    display.fillScreen(Color::BLACK);
    display.drawString( 50, 120,is_master ? "Master - Starting game" :
            "Slave - Starting game",  Color::WHITE);
    HAL_Delay(1000); // Pause pour voir le message

    return seed;
}

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
    // Attendre la synchronisation et obtenir le seed
    uint32_t seed = waitForSyncAndGetSeed(handles);
    
    // Initialiser le générateur de nombres aléatoires avec le seed
    srand(seed);
    
    // Démarrer le jeu
    game.setup(handles);
    p_game = &game;
    game.run();
}
