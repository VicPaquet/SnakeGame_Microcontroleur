#include "Game/Graphics/GraphObjects/Head.h"
#include "Interfaces/Display/Display.h"

void Head::draw() {
	disp->fillRect(Color::DARKGREEN, rect.getX1(), rect.getY1(), rect.getWidth(), rect.getHeight());
}


void Head::clear() {
	disp->fillRect(disp->getBackgroundColor(), rect.getX1(), rect.getY1(), rect.getWidth(),  rect.getHeight());
}

