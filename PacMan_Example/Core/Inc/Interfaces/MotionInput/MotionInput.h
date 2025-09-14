#ifndef MOTIONINPUT_H_
#define MOTIONINPUT_H_
/**
  * @file MotionInput.h
  * @date June 2025
  * @brief Abstraction of a user input mecanism. 
  * 
  * @defgroup ELE3312
  * @{
  */

namespace ELE3312 {

/** @brief Abstract class that models user input
  * in the x, y and z dimensions.
  */
class MotionInput {
public:
	/** @brief Updates the movement information (by receiving
	  * @details Updates the stored movement direction information
	  * in x, y and, z direction. This can be achieved by reading 
	  * the sensor data.
	  */
	virtual void update() = 0;

	/** @brief Returns the  change in movement in x direction.
	  * @retval The movement change in x direction.
	  */
	virtual float getX() const = 0;

	/** @brief Returns the movement change in y direction.
	  * @retval The movement change in y direction.
	  */
	virtual float getY() const = 0;

	/** @brief Returns the movement change in z direction.
	  * @retval The movement change in z direction.
	  */
	virtual float getZ() const = 0;
};

} /* namespace ELE3312 */

/** 
  * @}
  */
#endif /* MOTIONINPUT_H_ */
