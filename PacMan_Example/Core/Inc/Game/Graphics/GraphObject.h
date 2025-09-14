/**
  * @file GraphObject.h 
  * @date June 2025
  * @brief Contains the declaration of the GraphObject class that serves as an abstract interface
  * for graphical objects in the game.
  * 
  * @defgroup ELE3312
  * @{
  */

#ifndef INC_GRAPHOBJECT_H_
#define INC_GRAPHOBJECT_H_

#include "Interfaces/Display/Display.h"
#include "Interfaces/Display/Rect.h"
#include <memory>


namespace ELE3312 {
	/** @brief Declares an abstract interface for graphical objects in the game.
	  */
	class GraphObject {
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
} // End of namespace ELE3312
/**
  * @}
  */

#endif /* INC_GRAPHOBJECT_H_ */
