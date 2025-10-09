/*
 * Head.h
 *
 *  Created on: Sep 29, 2025
 *      Author: vicpa
 */

#ifndef INC_GAME_GRAPHICS_GRAPHOBJECTS_HEAD_H_
#define INC_GAME_GRAPHICS_GRAPHOBJECTS_HEAD_H_

#include "Game/Graphics/GraphObjects.h"

class Head : public GraphObject {
public:
	Head() = default;
	virtual ~Head() = default;
	void draw() override;
	void clear() override;


	Rect getOldHead() const {return oldHeadPosition;}
		void setOldHead(uint16_t x, uint16_t y) {
			oldHeadPosition.setX1(x);
			oldHeadPosition.setY1(y);
		}

private:
	Rect oldHeadPosition;

};



#endif /* INC_GAME_GRAPHICS_GRAPHOBJECTS_HEAD_H_ */
