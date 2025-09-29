#ifndef SRC_GRAPHICS_POINT_HPP_
#define SRC_GRAPHICS_POINT_HPP_

#include <cstdint>
class Point {
public:
	Point();
	Point(uint16_t x, uint16_t y);

	Point(const Point &other);

	virtual ~Point();


	Point & operator= (const Point &other);

	uint16_t getX() const;

	uint16_t getY() const;

	void setX(uint16_t new_x);

	void setY(uint16_t new_y);

	uint16_t getDistance(const Point &other) const;

	uint16_t getDistanceX(const Point &other) const;

	uint16_t getDistanceY(const Point &other) const;

private:
	uint16_t x; //!< The x coordinate of the point.
	uint16_t y; //!< The y coordinate of the point.
};
#endif /* SRC_GRAPHICS_POINT_H_ */
