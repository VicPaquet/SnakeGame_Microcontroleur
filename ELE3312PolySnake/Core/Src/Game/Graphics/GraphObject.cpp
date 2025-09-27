#include "Game/Graphics/GraphObjects.h"

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

const Rect &GraphObject::getRect() const {
	return rect;
}

