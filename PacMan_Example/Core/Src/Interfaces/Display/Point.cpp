/**
 * @file Point.cpp
 * @date June 2025
 * @brief Represents a Point in two dimensional space
 * 
 * @defgroup ELE3312
 * @{
 */

#include "Interfaces/Display/Point.h"
#include <cmath>

namespace ELE3312 {

	/** @brief default destructor
	*/
	Point::~Point() {
	}

	/** @brief Default constructor for the Point class.
	*/
	Point::Point(): x{0}, y{0} {

	}
	/** @brief Constructs a Point object.
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 */
	Point::Point(uint16_t x, uint16_t y): x{x}, y{y} {
	}


	/** @brief Copy-Constructor for the Point class.
	 * @param [in] other Const reference to another point object.
	 */
	Point::Point(const Point &other) {
		x = other.x;
		y = other.y;
	}

	/** @brief Equals operator for the Point class.
	 * @param [in] other A const reference to another Point object which x and y
	 * position will be copied into the Point object on the left side.
	 * @return A reference to the Point object on the left side of the equal sign.
	 */
	Point & Point::operator= (const Point &other) {
		if (this != &other){
			x = other.x;
			y = other.y;
		}
		return *this;
	}

	/** @brief Returns the x coordinate.
	 * @retval The x coordinate.
	 */
	uint16_t Point::getX() const{
		return x;
	}

	/** @brief Returns the y coordinate.
	 * @retval The y coordinate.
	 */
	uint16_t Point::getY() const{
		return y;
	}

	/**
	 * @brief Sets the new x position of the point. 
	 * @param new_x uint16_t integer value of the new x position.
	 */
	void Point::setX(uint16_t new_x) {
		x = new_x;
	}

	/**
	 * @brief Sets the new y position of the point.
	 * @param new_y uint16_t integer value of the new y position.
	 */
	void Point::setY(uint16_t new_y) {
		y = new_y;
	}


	/** @brief Calculates the euclidean distance between two points.
	 * @param other The other point to which the distance is calculated.
	 * @retval The euclidean distance between the current point and the specified other point.
	 */
	uint16_t Point::getDistance(const Point &other) const {
		return ceil(sqrt((x - other.getX())*(x - other.getX()) + (y - other.getY())*(y - other.getY())));
	}

	/** @brief Calculates the distance between the x coordinates of the 
	 * two points.
	 * @details The functions returns the absolute value of the x component
	 * of the current point and the specified other point.
	 * @param other The other point to which the distance in x direction is calculated.
	 * @retval The distance of the current point to the specified other point in x direction.
	 */
	uint16_t Point::getDistanceX(const Point &other) const {
		uint16_t distance = x - other.getX();
		return distance > 0 ? distance : -distance;
	}

	/** @brief Calculates the distance between the y coordinates of two points.
	 * @details The functions returns the distance in y direction of the 
	 * current point and the specified other point. The returned value is 
	 * the absolute value of this distance.
	 * @param other The other point to which the distance in y direction is calculated.
	 * @retval The absolute value of the distance of the two points in y direction.
	 */
	uint16_t Point::getDistanceY(const Point &other) const {
		uint16_t distance = y - other.getY();
		return distance > 0 ? distance : -distance;
	}

} /* namespace ELE3312 */

/**
 *@}
 */
