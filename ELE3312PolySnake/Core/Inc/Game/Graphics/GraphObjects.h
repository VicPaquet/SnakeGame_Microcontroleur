/*
 * GraphObjects.h
 *
 *  Created on: Sep 27, 2025
 *      Author: vicpa
 */

#ifndef INC_GAME_GRAPHICS_GRAPHOBJECTS_GRAPHOBJECTS_H_
#define INC_GAME_GRAPHICS_GRAPHOBJECTS_GRAPHOBJECTS_H_

#include "Interfaces/Display/Display.h"
#include "Interfaces/Display/Rect.h"
#include <memory>

class GraphObject{
public:
	GraphObject() {}
	virtual void setup(const Rect &rect, Display *disp) {
		this->disp = disp;
		this->rect = rect;
	}
	virtual void draw() = 0;
	virtual void clear() = 0;
	void setPosition(uint16_t x, uint16_t y) {
		rect.setX1(x);
		rect.setY1(y);
	}
	void translateX(uint16_t dist){
		rect.setX1(rect.getX1() + dist);
	}

	void translateY(uint16_t dist){
		rect.setY1(rect.getY1() + dist);
	}
	void drawGrid();
	const Rect &getRect() const;
protected:
	Rect rect; //!< Used to store size and position information of the graphical object.
	Display *disp; //!< Stores a pointer to a Graphics object that is used to draw on the screen.
};




#endif /* INC_GAME_GRAPHICS_GRAPHOBJECTS_GRAPHOBJECTS_H_ */
