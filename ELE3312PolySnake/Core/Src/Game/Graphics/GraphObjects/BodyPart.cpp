#include "Game/Graphics/GraphObjects/BodyPart.h"
#include "Interfaces/Display/Display.h"

void BodyPart::draw() {
	disp->fillRect(Color::GREEN, rect.getX1(), rect.getY1(), rect.getWidth(), rect.getHeight());
}


void BodyPart::clear() {
	disp->fillRect(disp->getBackgroundColor(), rect.getX1(), rect.getY1(), rect.getWidth(),  rect.getHeight());
}

