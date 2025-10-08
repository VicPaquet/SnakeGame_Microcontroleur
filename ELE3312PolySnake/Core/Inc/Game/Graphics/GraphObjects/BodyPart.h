#ifndef INC_GAME_GRAPHICS_GRAPHOBJECTS_BODYPART_H_
#define INC_GAME_GRAPHICS_GRAPHOBJECTS_BODYPART_H_

#include "Game/Graphics/GraphObjects.h"

class BodyPart : public GraphObject {
public:

	BodyPart(Display* disp, const Rect &rect) {
        setup(rect, disp);
    }

	virtual ~BodyPart() = default;
	void draw() override;
	void clear() override;
};

#endif /* INC_GAME_GRAPHICS_GRAPHOBJECTS_BODYPART_H_ */
