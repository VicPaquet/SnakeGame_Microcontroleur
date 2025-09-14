#ifndef MPU6050MOTIONINPUT_H_
#define MPU6050MOTIONINPUT_H_
/**
  * @file MPU6050MotionInput.h
  * @date June 2025
  * @brief Declaration of the MotionInput class for the
  * MPU6050 accelerometer and gyroscope IC.
  * 
  * @defgroup ELE3312
  * @{
  */

#include <Interfaces/MotionInput/MotionInput.h>
#include <NucleoImp/MotionInput/mpu6050.h>

namespace ELE3312 {


/** @brief The MPU6050MotionInput class implements the MotionInput
  * interface for the MPU6050 accelerometer and gyroscope sensor.
  */
class MPU6050MotionInput : public MotionInput{
public:
	MPU6050MotionInput();
	void setup(I2C_HandleTypeDef *hi2c);
	
	/** @brief Default destructor.
	  */
	virtual ~MPU6050MotionInput() = default;

	void update() override;

	float getX() const override;

	float getY() const override;
	
	float getZ() const override;
private:
	MPU6050_Data_T data; //!< Data structure to store MPU6050 data.
	MPU6050_HandleTypeDef mpu6050; //!< Handle to the MPU6050 driver meta data.
};

} /* namespace ELE3312 */

/**
  * @}
  */ // End of group ELE3312
#endif /* MPU6050MOTIONINPUT_H_ */
