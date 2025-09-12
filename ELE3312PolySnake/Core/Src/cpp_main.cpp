/**
  * @file cpp_main.cpp
  * @date June 2025
  * @brief 
  * 
  * @defgroup ELE3312
  * @{
  */

#include <memory>
#include <cstdio>
#include <string>

#include "main.h"
#include <cpp_main.h>
#include <NucleoImp/Display/ILI9341Display.h>

using namespace ELE3312;

ILI9341Display display;

/** @brief main function of the game.
  * @param [in] handles A pointer to a structure with peripheral handles from the HAL.
  */
 void cpp_main(peripheral_handles *handles) {
	 display.setup(handles->hspi_tft);
	 display.clearScreen();
     display.drawString(50,50, "PolySnake");
     display.fillRect(Color::GREEN, {50, 80, 100, 100});
     display.fillRect(Color::GREEN, {80, 100, 100, 130});
     display.fillRect(Color::GREEN, {100, 110, 110, 130});
     display.fillRect(Color::YELLOW, {110, 110, 130, 130});
     display.fillRect(Color::RED, {150, 110, 170, 130});
	 while(1) {
		 //Your infinite CPP loop
	 }
}


