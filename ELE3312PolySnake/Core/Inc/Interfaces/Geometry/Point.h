/**
  * @file Point.h
  * @date June 2025
  * @brief Represents a Point in two dimensional space
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef SRC_GRAPHICS_POINT_H_
#define SRC_GRAPHICS_POINT_H_


namespace ELE3312 {
/**
  * @brief The class represents a point in two dimensional space
  */
template<typename T>
class Point {
public:

	/** @brief Default constructor for the Point class.
	*/
	Point() : x{0}, y{0} {}

	/** @brief Constructs a Point object.
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 */
	Point(T x, T y) : x{x}, y{y} {}

	/** @brief Copy-Constructor for the Point class.
	 * @param [in] other Const reference to another point object.
	 */
	Point(const Point &other) : x{other.x}, y{other.y} {}

	/** @brief default destructor
	*/
	~Point() {}


	/** @brief Equals operator for the Point class.
	 * @param [in] other A const reference to another Point object whose x and y
	 * position will be copied into the Point object on the left side.
	 * @return A reference to the Point object on the left side of the equal sign.
	 */
	Point & operator= (const Point &other) {
		if (this != &other){
			x = other.x;
			y = other.y;
		}
		return *this;
	}


	/** @brief Addition operator for the Point class.
	  * @param [in] other Const reference to the other point object whose x and y 
	  * positions are added to the current object's.
	  * @return Reference to the current Point object.
	  */
	Point & operator+ (const Point &other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	template<class O>
	friend Point<O> & operator+ (const Point<O> &p1, const Point<O> &p2);

	template<typename O>
	friend bool operator== (const Point<O> &p1, const Point<O> &p2);

	/** @brief Returns the x coordinate.
	 * @retval The x coordinate.
	 */
	T getX() const{
		return x;
	}

	/** @brief Returns the y coordinate.
	 * @retval The y coordinate.
	 */
	T getY() const{
		return y;
	}

	/**
	 * @brief Sets the new x position of the point. 
	 * @param new_x  integer value of the new x position.
	 */
	void setX(T newX){
		x = newX;
	}

	/**
	 * @brief Sets the new y position of the point.
	 * @param new_y integer value of the new y position.
	 */
	void setY(T newY){
		y = newY;
	}


	/** @brief Calculates the euclidean distance between two points.
	 * @param other The other point to which the distance is calculated.
	 * @retval The euclidean distance between the current point and the specified other point.
	 */
	T getDistance(const Point &other) const {
		return ceil(sqrt((x - other.getX())*(x - other.getX()) + (y - other.getY())*(y - other.getY())));
	}

	/** @brief Calculates the distance between the x coordinates of the 
	 * two points.
	 * @details The functions returns the absolute value of the x component
	 * of the current point and the specified other point.
	 * @param other The other point to which the distance in x direction is calculated.
	 * @retval The distance of the current point to the specified other point in x direction.
	 */
	T getDistanceX(const Point &other) const {
		T distance = x - other.getX();
		return distance > 0 ? distance : -distance;
	}

	/** @brief Calculates the distance between the y coordinates of two points.
	 * @details The functions returns the distance in y direction of the 
	 * current point and the specified other point. The returned value is 
	 * the absolute value of this distance.
	 * @param other The other point to which the distance in y direction is calculated.
	 * @retval The absolute value of the distance of the two points in y direction.
	 */
	T getDistanceY(const Point &other) const {
		T distance = y - other.getY();
		return distance > 0 ? distance : -distance;
	}

private:
	T x; //!< The x coordinate of the point.
	T y; //!< The y coordinate of the point.
};

template<typename O>
Point<O> & operator+ (const Point<O> &p1, const Point<O> &p2){
	p1.x = p1.x + p2.x;
	p1.y = p1.y + p2.y;
	return p1;
}


template<typename O>
bool operator== (const Point<O> &p1, const Point<O> &p2) {
	return (p1.x == p2.x) && (p1.y == p2.y);
}

} /* namespace ELE3312 */

/**
  *@}
  */
#endif /* SRC_GRAPHICS_POINT_H_ */
