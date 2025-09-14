/**
 * @file GraphObject.cpp
 * @date June 2025
 * @brief This file implements a common function for the general GraphObject interface
 * for graphical objects.
 *
 * @defgroup ELE3312
 * @{
 */

#include "Game/Graphics/GraphObject.h"

namespace ELE3312 {
/** @brief The drawGrid method is used in development to draw 
  * a grid lines on the square that contains the graphical object.
  * @details The method is intended for development to help to position
  * graphical primitives that constitute the graphical object.
  */
void GraphObject::drawGrid() {
	uint16_t width = rect.getWidth();
	uint16_t height = rect.getHeight();
	constexpr uint16_t gridSize = 10;
	uint16_t numVerticalLines = height / gridSize;
	uint16_t numHorizontalLines = width / gridSize;
	Color gridColor = Color::DARKGREY;
	disp->drawRect(gridColor, rect.getX1(), rect.getY1(), width, height);
	for (uint16_t i=0; i<numVerticalLines; ++i) {
		disp->drawLine(gridColor, rect.getX1(), rect.getY1() + i*gridSize, rect.getX2(), rect.getY1() + i*gridSize);
	}
	for(uint16_t i=0; i<numHorizontalLines; ++i) {
		disp->drawLine(gridColor, rect.getX1() + i*gridSize, rect.getY1(), rect.getX1() + i*gridSize, rect.getY2());
	}

}
/** @brief Returns a reference to the rectangle that defines the position and size of the GraphObject.
  * @return Reference to a Rect object.
  */
const Rect &GraphObject::getRect() const {
	return rect;
}

}
