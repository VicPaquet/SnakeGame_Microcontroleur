/**
  * @file MPU6050MotionInput.cpp
  * @date June 2025
  * @brief Implementation of the MPU6050MotionInput class for the
  * MPU6050 accelerometer and gyroscope IC.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "NucleoImp/MotionInput/MPU6050MotionInput.h"


namespace ELE3312 {

	/** @brief Setup routine for the MPU6050MotionInput class.
	 * @param [in] hi2c STM32 HAL I2C periphery handle used to communicate
	 * with the MPU6050 IC.
	 */
	void MPU6050MotionInput::setup(I2C_HandleTypeDef *hi2c) {
		mpu6050.hi2c = hi2c;
		MPU6050_Init(&mpu6050);
		MPU6050_Setup(&mpu6050);
	}

	/** @brief Constructor for the MPU6050MotionInput class.
	 */
	MPU6050MotionInput::MPU6050MotionInput() {
	}

	/** @brief Updates the movement information by reading the 
	  * the x, y and, z value of the accelerometer via I2C.
	  */
void MPU6050MotionInput::update(){
	MPU6050_ReadData(&mpu6050, &data);
}

	/** @brief Returns the  change in movement in x direction.
	  * @retval The movement change in x direction.
	  */
float MPU6050MotionInput::getX() const {
	return data.acc_x;
}

	/** @brief Returns the movement change in y direction.
	  * @retval The movement change in y direction.
	  */
float MPU6050MotionInput::getY() const {
	return data.acc_y;
}


	/** @brief Returns the movement change in z direction.
	  * @retval The movement change in z direction.
	  */
float MPU6050MotionInput::getZ() const {
	return data.acc_z;
}


} /* End of namespace ELE3312 */

/**
  * @}
  */ // End of group ELE3312
