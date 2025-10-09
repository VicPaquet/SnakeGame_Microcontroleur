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

// Instance globale du jeu Snake
SnakeGame snakeGame;
ILI9341Display display;

/**
 * brief Fonction principale pour tester la génération du snake
 */

void cpp_main(peripheral_handles *handles) {
    // Initialiser l'affichage TFT
    display.setup(handles->hspi_tft);
    display.clearScreen();
    
    // Afficher le titre du test
    display.drawString(20, 100, "PolySnake - Test Generation", Color::WHITE);
    HAL_Delay(2000);
    
    // Configurer le jeu Snake avec l'affichage
    snakeGame.setup(&display);
    
    // Exécuter le jeu (initialisation + affichage)
    while (1){
    	snakeGame.run();
    	HAL_Delay(5000);
        display.drawString(20, 120, "Redemarrage...", Color::WHITE);
        HAL_Delay(1000);
    }
    
    
//    // Attendre un peu pour voir le résultat
//    HAL_Delay(5000);
//
//    // Optionnel: redémarrer le jeu pour tester
//    display.drawString(20, 120, "Redemarrage...", Color::WHITE);
//    HAL_Delay(1000);
//
//    snakeGame.restart();
//    snakeGame.run();
}
