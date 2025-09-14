/**
 * @file Rect.cpp
 * @date June 2025
 * @brief Representation of a rectangle.
 * 
 * @defgroup ELE3312
 * @{
 */
#include "Interfaces/Display/Rect.h"
#include <cstdint>

using namespace ELE3312;


/** @brief Constructor for a Rect object.
 * @details This constructor uses the lower left and upper right corner
 * to construct a rectangle object at the specified position with 
 * it's corresponding dimensions.
 * @param [in] x1 The x coordinate of the lower left corner.
 * @param [in] y1 The y coordinate of the lower left corner.
 * @param [in] x2 The x coordinate of the upper right corner.
 * @param [in] y2 the y coordinate of the upper right corner.
 * @retval A Rect object that represents a rectangle.
 */
Rect::Rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) : p1{x1, y1}, p2{x2, y2} {}


/** @brief Constructor for a Rect object.
 * @details This constructor creates a rectangle object with 
 * zero size (height and width are zero). It can be used if the 
 * position is known at creation time but not it's width or height.
 * @param [in] x The x coordinate of the lower left corner.
 * @param [in] y The y coordinate of the upper right corner.
 * @retval A Rect object with zero width and height.
 */
Rect::Rect(const Point & p1, const Point & p2) : p1{p1}, p2{p2} {}

/** @brief Constructor for a Rect object.
 * @details The constructor creates a rectangle object 
 * based on the lower left and upper right corner.
 * @param [in] p1 The coordinate of the lower left corner of the rectangle.
 * @param [in] p2 The coordinate of the upper right corner of the rectangle.
 * @retval A rect object at the position of p1 with width and height derived
 * from the two specified points.
 */
Rect::Rect(const Point & p1, uint16_t width, uint16_t height) : p1{p1}, p2{0,0} {
	p2.setX(p1.getX() + width);
	p2.setY(p1.getY() + height);
}

/** @brief Constructor for ta Rect object.
 * @details The constructor creates a rectangle object which 
 * lower left corner is positioned at the coordinate p1 and which has
 * the specified width and height.
 * @param [in] p1 The coordinate of the lower left corner.
 * @param [in] width The width of the rectangle.
 * @param [in] height The height of the rectangle.
 * @retval A Rect object at the position p1 with the specified 
 * width and height.
 */
Rect::Rect(uint16_t x, uint16_t y) : p1{x,y}, p2{x,y} {}

/** @brief Default constructor for a Rect object.
 * @details The default constructor initializes the Rect object
 * with all values to zero.
 * @retval A default initialized Rect object.
 */
Rect::Rect() : p1{0,0}, p2{0,0} {}

/** @brief Copy constructor for a Rect object.
 * @details The copy constructor creates a Rect object
 * based on the specified object. The created object will
 * represent a rectangle at the same position with the same dimensions 
 * as the specified object.
 * @param [in] rect A Rect object that is going to be copied.
 * @retval A copy of the specified Rect object.
 */
Rect::Rect(const Rect &rect): p1{rect.p1}, p2{rect.p2} {
}


/**@brief Equals operator for the Rect class.
  *@param [in] other A reference to another Rect object from which
  * the position information is going to be copied.
  * @return A reference to the object.
  */
Rect & Rect::operator=(const Rect &other){
	p1.setX(other.p1.getX());
	p1.setY(other.p1.getY());
	p2.setX(other.p2.getX());
	p2.setY(other.p2.getY());
	return *this;
}
/** @brief Default destructor
*/
Rect::~Rect() {
}

/** @brief Returns the width of the rectangle.
 * @retval The width of the rectangle.
 */
uint16_t Rect::getWidth() const {
	if (p1.getX() < p2.getX()) {
		return p2.getX() - p1.getX();
	} 
	return p1.getX() - p2.getX();
}

/** @brief Returns the height of the rectangle.
 * @retval The height of the object.
 */
uint16_t Rect::getHeight() const {
	if (p1.getY() < p2.getY()) {
		return p2.getY() - p1.getY();
	}
	return p1.getY() - p2.getY();
}

/** @brief Returns the x coordinate of the lower left corner
 * of the rectangle.
 * @retval The x coordinate of the lower left corner.
 */
uint16_t Rect::getX1() const {
	return p1.getX();
}

/** @brief Returns the x coordinate of the upper right corner 
 * of the rectangle.
 * @retval The x coordinate of the upper right corner.
 */
uint16_t Rect::getX2() const {
	return p2.getX();
}

/** @brief Returns the y coordinate of the lower left corner
 * of the rectangle.
 * @retval The y coordinate of the lower left corner.
 */
uint16_t Rect::getY1() const {
	return p1.getY();
}

/** @brief Returns the y coordinate of the upper right corner
 * of the rectangle.
 * @retval The y coordinate of the upper right corner.
 */
uint16_t Rect::getY2() const {
	return p2.getY();
}

// Setters

/** @brief Set the x coordinate of the lower left corner.
 * @param [in] x The x coordinate of the lower left corner.
 */
void Rect::setX1(const uint16_t x) {
	p1.setX(x);
}

/** @brief Set the x coordinate of the upper right corner.
 * @param [in] x The x coordinate of the upper right corner.
 */
void Rect::setX2(const uint16_t x) {
	p2.setX(x);

}

/** @brief Set the y coordinate of the lower left corner.
 * @param [in] y The y coordinate of the lower left corner.
 */
void Rect::setY1(const uint16_t y) {
	p1.setY(y);
}

/** @brief Set the y coordinate of the upper right corner.
 * @param [in] y The y coordinate of the upper right corner.
 */
void Rect::setY2( const uint16_t y) {
	p2.setY(y);
}

/** @brief Set the width of the rectangle.
 * @param [in] width The width of the rectangle.
 */
void Rect::setWidth( const uint16_t width) {
	p2.setX( p1.getX() + width);
}

/** @brief Set the height of the rectangle.
 * @param [in] height The height of the rectangle.
 */
void Rect::setHeight( const uint16_t height) {
	p2.setY( p1.getY() + height);
}

/**
 *@}
 */ // End of group ELE3312
