/**
  * @file Ghost.h 
  * @date June 2025
  * @brief Declares the Ghost graphical object that paints an image of a ghost on the
  * screen.
  * 
  * @defgroup ELE3312
  * @{
  */

#ifndef GHOST_H_
#define GHOST_H_

#include "Game/Graphics/GraphObject.h"

namespace ELE3312 {
/** @brief The Ghost class is used to draw an image of a ghost on the screen
  */
class Ghost : public GraphObject{
public:
	Ghost();
	void setup(const Rect &rect, Display *disp, Color color);
	virtual ~Ghost() = default;
	void draw() override;
	void clear() override;
private:
	Color color;
};
} // End of namespace ELE3312
/**
  * @}
  */
#endif /* GHOST_H_ */
