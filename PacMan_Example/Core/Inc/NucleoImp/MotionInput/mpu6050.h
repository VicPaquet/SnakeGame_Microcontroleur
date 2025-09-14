/*
 * mpu6050.h
 *
 *  Created on: May 14, 2025
 *      Author: joerg
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#include "main.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Define register base addresses */
#define MPU6050_SELF_TEST_X 0x0D
#define MPU6050_SELF_TEST_Y 0x0E
#define MPU6050_SELF_TEST_Z 0x0F
#define MPU6050_SELF_TEST_A 0x10
#define MPU6050_SMPLRT_DIV  0x19
#define MPU6050_CONFIG  0x1A
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_FIFO_EN 0x23
#define MPU6050_I2C_MST_CTRL 0x24
#define MPU6050_I2C_SLV0_ADDR 0x25
#define IMPU6050_2C_SLV0_REG 0x26
#define IMPU6050_2C_SLV0_CTRL 0x27
#define IMPU6050_2C_SLV1_ADDR 0x28
#define MPU6050_I2C_SLV1_REG 0x29
#define MPU6050_I2C_SLV1_CTRL 0x2A
#define MPU6050_I2C_SLV2_ADDR 0x2B
#define MPU6050_I2C_SLV2_REG  0x2C
#define MPU6050_I2C_SLV2_CTRL 0x2D
#define MPU6050_I2C_SLV3_ADDR 0x2E
#define MPU6050_I2C_SLV3_REG 0x2F
#define MPU6050_I2C_SLV3_CTRL 0x30
#define MPU6050_I2C_SLV4_ADDR 0x31
#define MPU6050_I2C_SLV4_REG 0x32
#define MPU6050_I2C_SLV4_DO 0x33
#define MPU6050_I2C_SLV4_CTRL 0x34
#define MPU6050_I2C_SLV4_DI 0x35
#define MPU6050_I2C_MST_STATUS 0x36
#define MPU6050_INT_PIN_CFG 0x37
#define MPU6050_INT_ENABLE 0x38
#define MPU6050_INT_STATUS 0x3A
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40
#define MPU6050_TEMP_OUT_H 0x41
#define MPU6050_TEMP_OUT_L 0x42
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_XOUT_L 0x44
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_YOUT_L 0x46
#define MPU6050_GYRO_ZOUT_H 0x47
#define MPU6050_GYRO_ZOUT_L 0x48
#define MPU6050_EXT_SENS_DATA_00 0x49
#define MPU6050_EXT_SENS_DATA_01 0x4A
#define MPU6050_EXT_SENS_DATA_02 0x4B
#define MPU6050_EXT_SENS_DATA_03 0x4C
#define MPU6050_EXT_SENS_DATA_04 0x4D
#define MPU6050_EXT_SENS_DATA_05 0x4E
#define MPU6050_EXT_SENS_DATA_06 0x4F
#define MPU6050_EXT_SENS_DATA_07 0x50
#define MPU6050_EXT_SENS_DATA_08 0x51
#define MPU6050_EXT_SENS_DATA_09 0x52
#define MPU6050_EXT_SENS_DATA_10 0x53
#define MPU6050_EXT_SENS_DATA_11 0x54
#define MPU6050_EXT_SENS_DATA_12 0x55
#define MPU6050_EXT_SENS_DATA_13 0x56
#define MPU6050_EXT_SENS_DATA_14 0x57
#define MPU6050_EXT_SENS_DATA_15 0x58
#define MPU6050_EXT_SENS_DATA_16 0x59
#define MPU6050_EXT_SENS_DATA_17 0x5A
#define MPU6050_EXT_SENS_DATA_18 0x5B
#define MPU6050_EXT_SENS_DATA_19 0x5C
#define MPU6050_EXT_SENS_DATA_20 0x5D
#define MPU6050_EXT_SENS_DATA_21 0x5E
#define MPU6050_EXT_SENS_DATA_22 0x5F
#define MPU6050_EXT_SENS_DATA_23 0x60
#define MPU6050_I2C_SLV0_DO 0x63
#define MPU6050_I2C_SLV1_DO 0x64
#define MPU6050_I2C_SLV2_DO 0x65
#define MPU6050_I2C_SLV3_DO 0x66
#define MPU6050_I2C_MST_DELAY_CTRL 0x67
#define MPU6050_SIGNAL_PATH_RESET 0x68
#define MPU6050_USER_CTRL 0x6A
// Register to control the power management, the reset value is 0x40
// which indicates that the "sleep" bit is set and the chip is in sleep mode
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_PWR_MGMT_2 0x6C
#define MPU6050_FIFO_COUNTH 0x72
#define MPU6050_FIFO_COUNTL 0x73
#define MPU6050_FIFO_R_W 0x74
// Register that contains the value 0x68 that identifies the chip
#define MPU6050_WHO_AM_I 0x75

// I2C addresses based on the level of AD0
#define MPU6050_ADDR0 0b11010000 // Pin AD0 is logic low

#define MPU6050_ADDR1 0b11010010U // Pin AD0 is logic high

// Define the timeout used when communicating via I2C with the device using HAL functions
#define MPU6050_TIMEOUT 100


/*
 * Status type used to indicate success and failure of driver
 * functions.
 */
typedef enum {
	MPU6050_OK = 0x00,
	MPU6050_ERR = 0x01,
	MPU6050_I2C_ERR = 0x02,
	MPU6050_VALUE_ERR = 0x03
}MPU6050_Status;

/*
 * Defines the possible clock sources that can be configured in register MPU6050_PWR_MGMT_1
 * Bits 0-2
 */

typedef enum {
	MPU6050_CLKSEL_INTERNAL = 0, // Internal 8MHz oscillator
	MPU6050_CLKSEL_PLL_X    = 1, // PLL with X axis gyroscope reference
	MPU6050_CLKSEL_PLL_Y    = 2, // PLL with Y axis gyroscope reference
	MPU6050_CLKSEL_PLL_Z    = 3, // PLL with Z axis gyroscope reference
	MPU6050_CLKSEL_PLL_EX1  = 4, // PLL with external 32.768kHz reference
	MPU6050_CLKSEL_PLL_EX2  = 5, // PLL with external 19.2MHz reference
	MPU6050_CLKSEL_STOP     = 7  // Stops the clock and keeps the timing generator in reset
}MPU6050_CLKSEL_T;



/*
 * Defines the full scale range of the gyroscope configured in register MPU6050_GYRO_CONFIG
 * Bits 3-4
 */
typedef enum{
	MPU6050_GY_FS_SEL_250   = 0, // Selects the full scale range of plus minus 250 degrees per second
	MPU6050_GY_FS_SEL_500   = 1, // Selects the full scale range of plus minus 500 degrees per second
	MPU6050_GY_FS_SEL_1000  = 2, // Selects the full scale range of plus minus 1000 degrees per second
	MPU6050_GY_FS_SEL_2000  = 3	// Selects the full scale range of plus minus 2000 degrees per second
}MPU6050_GY_FS_SEL_T;

/*
 * Defines the full scale range of the accelerometer configured in register MPU6050_ACCEL_CONFIG
 * Bits 3-4
 */
typedef enum{
	MPU6050_ACC_FS_SEL_2G   = 0, // Selects the full scale range of plus minus 2g
	MPU6050_ACC_FS_SEL_4G   = 1, // Selects the full scale range of plus minus 4g
	MPU6050_ACC_FS_SEL_8G   = 2, // Selects the full scale range of plus minus 8g
	MPU6050_ACC_FS_SEL_16G  = 3	 // Selects the full scale range of plus minus 16g
}MPU6050_ACC_FS_SEL_T;


/* Structure that contains connection informations
 * used to configure the MPU6050 device and to communicate
 * with it.
 */
typedef struct  {
	 I2C_HandleTypeDef *hi2c;
	 uint16_t addr;
	 MPU6050_CLKSEL_T clk_source; // Configure clock source of the chip
	 MPU6050_GY_FS_SEL_T gy_fs; // Full scale range gyro
	 MPU6050_ACC_FS_SEL_T acc_fs; // Full scale range accelerometer
	 float gy_lsb ; // Least significant byte sensitivity depending on full scale range
	 float acc_lsb; // Least significant byte sensitivity depending on full scale range
} MPU6050_HandleTypeDef;

typedef struct{
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float acc_x;
	float acc_y;
	float acc_z;
	float temp;
}MPU6050_Data_T;

/*============================== Function Prototypes =====================*/

/*
 * @brief Initialize MPU6050 handler structure
 * @param hmpu6050 specifies the MPU6050 handle
 * @retval MPU6050_Status
 */
MPU6050_Status MPU6050_Init(MPU6050_HandleTypeDef *hmpu6050);

/*
 * @brief Setup the MPU6050 Device
 * @param hmpu6050 specifies the MPU6050 handle
 * @retval MPU6050_Status
 */
MPU6050_Status MPU6050_Setup(MPU6050_HandleTypeDef *hmpu6050);

/*
 * @brief Resets MPU6050 Device
 * @param hmpu6050 specifies the MPU6050 handle
 * @retval MPU6050_Status
 */
MPU6050_Status MPU6050_Reset(MPU6050_HandleTypeDef *hmpu6050);

MPU6050_Status MPU6050_ReadData(MPU6050_HandleTypeDef *hmpu6050, MPU6050_Data_T *data);

// Maybe not necessary to put those helper functions in the header file
void MPU6050_ExtractTemperature(MPU6050_HandleTypeDef *hmpu6050, MPU6050_Data_T *data, const uint8_t *buff);
void MPU6050_ExtractGyro(MPU6050_HandleTypeDef *hmpu6050, MPU6050_Data_T *data, const uint8_t *buff);
void MPU6050_ExtractAcc(MPU6050_HandleTypeDef *hmpu6050, MPU6050_Data_T *data, const uint8_t *buff);

#ifdef __cplusplus
}
#endif


#endif /* MPU6050_H_ */
