/**
  * @file PacMan.cpp
  * @date June 2025
  * @brief This file implements the PacMan graphical object class that draws 
  * the PacMan character on the screen.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "Game/Graphics/GraphObjects/PacMan.h"

namespace ELE3312 {


/** @brief Draws the PacMan character on the screen.
  */
void PacMan::draw() {
	const uint16_t radius = 50;
	// Draw Body
	disp->fillArc(rect.getX1() + radius, rect.getY1() - radius, 0, radius, -150, 150, Color::YELLOW);// y = 120, x = 200
													  // Draw Eye
	disp->fillCircle(Color::WHITE, rect.getX1() + radius,rect.getY1() - radius - 25, 8);
	disp->fillCircle(Color::BLACK, rect.getX1() + radius - 2, rect.getY1() - radius - 28, 2);// y = 92
}

/** @brief Removes the PacMan character from the screen by repainting it with the background color.
  */
void PacMan::clear() {
	const uint16_t radius = 50;
	// Draw Body
	disp->fillArc(rect.getX1() + radius, rect.getY1() - radius, 0, radius, -150, 150, Color::BLACK);// y = 120, x = 200
	// Draw Eye
	disp->fillCircle(Color::BLACK, rect.getX1() + radius,rect.getY1() - radius - 25, 8);
}

} /* namespace ELE3312 */

/**
  * @}
  */ // End of documentation group ELE3312
