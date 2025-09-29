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
	virtual ~Head() = default;
	void draw() override;
	void clear() override;
};



#endif /* INC_GAME_GRAPHICS_GRAPHOBJECTS_HEAD_H_ */
