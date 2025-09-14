/**
  * @file Rect.h
  * @date June 2025
  * @brief Representation of a rectangle.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef RECT_H_
#define RECT_H_
#include <cstdint>
#include "Interfaces/Display/Point.h"

namespace ELE3312 {
	/** @brief Representation of a rectangle.
	  * @details The class models a rectangle contining it's position and dimensions.
	  */
class Rect {
public:
	Rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

	Rect(uint16_t x, uint16_t y);

	Rect(const Point & p1, const Point & p2);

	Rect(const Point & p1, uint16_t width, uint16_t height);

	Rect & operator=(const Rect &other);
	Rect();

	Rect(const Rect &rect);

	virtual ~Rect();

	uint16_t getWidth() const ;

	uint16_t getHeight() const ;

	uint16_t getX1() const ;

	uint16_t getX2() const ;

	uint16_t getY1() const ;

	uint16_t getY2() const ;
	
	// Setters 

	void setX1(uint16_t x) ;

	void setX2(uint16_t x) ;

	void setY1(uint16_t y) ;

	void setY2(uint16_t y) ;

	void setWidth(uint16_t width) ;

	void setHeight(uint16_t height) ;
private:
	Point p1; //!< The lower left corner of the rectangle.
	Point p2; //!< The upper right corner of the rectangle.
};

} // End of the namespace ELE3312

/**
  *@}
  */ // End of group ELE3312

#endif /* RECT_H_ */
