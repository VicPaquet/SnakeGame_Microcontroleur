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
#include "Interfaces/Geometry/Point.h"

namespace ELE3312 {
	/** @brief Representation of a rectangle.
	  * @details The class models a rectangle containing it's position and dimensions.
	  */
template<class T>
class Rect {
public:
	Rect(T x1, T y1, T x2, T y2) : p1{x1, y1}, p2{x2, y2} {}

	Rect(T x, T y) : p1{x, y}, p2{x, y} {}

	Rect(const Point<T> & p1, const Point<T> & p2) : p1{p1}, p2{p2} {}

	Rect(const Point<T> & p1, T width, T height) : p1{p1}, p2{p1.getX() + width,  p1.getY() + height} {}

	Rect() : p1{0, 0}, p2{0, 0} {}

	Rect(const Rect &rect) : p1{rect.p1}, p2{rect.p2} {}

	virtual ~Rect() {}

	T getWidth() const  {
		return p2.getX() - p1.getX();
	}

	T getHeight() const {
		return p2.getY() - p1.getY();
	}

	T getX1() const {
		return p1.getX();
	}

	T getX2() const {
		return p2.getX();
	}

	T getY1() const {
		return p1.getY();
	}

	T getY2() const {
		return p2.getY();
	}
	
	// Setters 

	void setX1(T x) {
		p1.setX(x);
	}

	void setX2(T x) {
		p2.setX(x);
	}

	void setY1(T y) {
		p1.setY(y);
	}

	void setY2(T y) {
		p2.setY(y);
	}

	void setWidth(T width) {
		p2.setX(p1.getX() + width);
	}

	void setHeight(T height) {
		p2.setY(p1.getY() + height);
	}
private:
	Point<T> p1; //!< The lower left corner of the rectangle.
	Point<T> p2; //!< The upper right corner of the rectangle.
};

} // End of the namespace ELE3312

/**
  *@}
  */ // End of group ELE3312

#endif /* RECT_H_ */
