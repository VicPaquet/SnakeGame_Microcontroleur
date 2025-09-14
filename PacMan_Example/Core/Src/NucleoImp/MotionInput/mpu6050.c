/*
 * mpu6050.c
 *
 *  Created on: May 14, 2025
 */

#include <assert.h>
#include "NucleoImp/MotionInput/mpu6050.h"

//#define MPU6050_DEBUG

#ifdef MPU6050_DEBUG
#include "stdio.h"
#endif

MPU6050_Status MPU6050_Setup(MPU6050_HandleTypeDef *hmpu6050){
	// Verify that the configuration struct is not NULL
	assert(hmpu6050 != NULL);
	// Verify that the address given is one of the two allowed ones
	assert(hmpu6050->addr == MPU6050_ADDR0 || hmpu6050->addr == MPU6050_ADDR1);

	uint8_t buff[1] = {0};
	// Check that the device is addressable by the cortex m4
	HAL_StatusTypeDef status;
	status = HAL_I2C_IsDeviceReady(hmpu6050->hi2c, hmpu6050->addr, 1, MPU6050_TIMEOUT);
	if(status != HAL_OK){
		#ifdef MPU6050_DEBUG
		printf("ERROR: unable to communicate with gyro!\r\n");
		#endif
		return MPU6050_I2C_ERR;
	} else {
		#ifdef MPU6050_DEBUG
		printf("MPU6050 chip is there!\r\n");
		#endif
	}

	// Check identity of the the chip
	status = HAL_I2C_Mem_Read (hmpu6050->hi2c, hmpu6050->addr,
				MPU6050_WHO_AM_I, 1, buff, sizeof(buff), MPU6050_TIMEOUT);
	if(status != HAL_OK || buff[0] != 0x68){
		#ifdef MPU6050_DEBUG
		printf("ERROR: unable to read identity of the MPU6050 sensor !\r\n");
		#endif
		return MPU6050_I2C_ERR;
	} else {
		#ifdef MPU6050_DEBUG
		printf("MPU6050 responded with identity 0x68 !\r\n");
		#endif
	}


	//############# Wake up device and configure clock source #####################
	status = HAL_I2C_Mem_Write(hmpu6050->hi2c, hmpu6050->addr, MPU6050_PWR_MGMT_1, 1,
			&hmpu6050->clk_source, 1, MPU6050_TIMEOUT);
	if(status != HAL_OK){
			#ifdef MPU6050_DEBUG
			printf("ERROR: unable to wake up the MPU6050 sensor!\r\n");
			#endif
			return MPU6050_I2C_ERR;
		} else {
			#ifdef MPU6050_DEBUG
			printf("Reset MPU6050 chip!\r\n");
			#endif
		}

	// ################### Enable Interrupt #################################
	uint8_t data = 4;
//	status = HAL_I2C_Mem_Write(hmpu6050->hi2c, hmpu6050->addr, MPU6050_INT_PIN_CFG, 1,
//					&data, 1, MPU6050_TIMEOUT);
//	if(status != HAL_OK){
//			#ifdef MPU6050_DEBUG
//			printf("ERROR: unable to activate interrupt the MPU6050 sensor!\r\n");
//			#endif
//			return MPU6050_I2C_ERR;
//		} else {
//			#ifdef MPU6050_DEBUG
//			printf("Enabled interrupt MPU6050 chip!\r\n");
//			#endif
//		}
	data = 1;
	status = HAL_I2C_Mem_Write(hmpu6050->hi2c, hmpu6050->addr, MPU6050_INT_ENABLE, 1,
				&data, 1, MPU6050_TIMEOUT);
		if(status != HAL_OK){
				#ifdef MPU6050_DEBUG
				printf("ERROR: unable to activate interrupt the MPU6050 sensor!\r\n");
				#endif
				return MPU6050_I2C_ERR;
			} else {
				#ifdef MPU6050_DEBUG
				printf("Enabled interrupt MPU6050 chip!\r\n");
				#endif
			}
	data = 1;
	status = HAL_I2C_Mem_Write(hmpu6050->hi2c, hmpu6050->addr, 0x1A, 1,
					&data, 1, MPU6050_TIMEOUT);
		if(status != HAL_OK){
				#ifdef MPU6050_DEBUG
				printf("ERROR: unable to activate interrupt the MPU6050 sensor!\r\n");
				#endif
				return MPU6050_I2C_ERR;
			} else {
				#ifdef MPU6050_DEBUG
				printf("Enabled interrupt MPU6050 chip!\r\n");
				#endif
			}

	//############# Configure the gyroscope    ##############
	buff[0] = hmpu6050->gy_fs << 3; // Set gyro full scale value and disable selftest
	// Update the gyro LSB sensitivity
	switch(hmpu6050->gy_fs){
	case MPU6050_GY_FS_SEL_250:
		hmpu6050->gy_lsb = 131.0f;
		break;
	case MPU6050_GY_FS_SEL_500:
			hmpu6050->gy_lsb = 65.5f;
			break;
	case MPU6050_GY_FS_SEL_1000:
			hmpu6050->gy_lsb = 32.8f;
			break;
	case MPU6050_GY_FS_SEL_2000:
			hmpu6050->gy_lsb = 16.4f;
			break;
	default:
		return MPU6050_VALUE_ERR; // Unknown value -> ERROR
	};
	status = HAL_I2C_Mem_Write(hmpu6050->hi2c, hmpu6050->addr, MPU6050_GYRO_CONFIG, 1,
				&buff[0], 1, MPU6050_TIMEOUT);
	if(status != HAL_OK){
			#ifdef MPU6050_DEBUG
			printf("ERROR: unable to configure gyroscope configuration the MPU6050 sensor!\r\n");
			#endif
			return MPU6050_I2C_ERR;
		} else {
			#ifdef MPU6050_DEBUG
			printf("Set gyroscope FS_SEL to %d !\r\n", hmpu6050->gy_fs);
			#endif
		}
	//############# Configure the gyroscope    ##############
	buff[0] = hmpu6050->acc_fs << 3; // Set accelerometer full scale value and disable selftest
		// Update the gyro LSB sensitivity
		switch(hmpu6050->acc_fs){
		case MPU6050_ACC_FS_SEL_2G:
			hmpu6050->gy_lsb = 16384.0f;
			break;
		case MPU6050_ACC_FS_SEL_4G:
				hmpu6050->gy_lsb = 16384.0f;
				break;
		case MPU6050_ACC_FS_SEL_8G:
				hmpu6050->gy_lsb = 4096.0f;
				break;
		case MPU6050_ACC_FS_SEL_16G:
				hmpu6050->gy_lsb = 2048.0f;
				break;
		default:
			return MPU6050_VALUE_ERR; // Unknown value -> ERROR
		};
		status = HAL_I2C_Mem_Write(hmpu6050->hi2c, hmpu6050->addr, MPU6050_ACCEL_CONFIG, 1,
					&buff[0], 1, MPU6050_TIMEOUT);
		if(status != HAL_OK){
				#ifdef MPU6050_DEBUG
				printf("ERROR: unable to configure gyroscope configuration the MPU6050 sensor!\r\n");
				#endif
				return MPU6050_I2C_ERR;
			} else {
				#ifdef MPU6050_DEBUG
				printf("Set gyroscope FS_SEL to %d !\r\n", hmpu6050->gy_fs);
				#endif
			}
		return MPU6050_OK;
}

MPU6050_Status MPU6050_Init(MPU6050_HandleTypeDef *hmpu6050){
	hmpu6050->addr = MPU6050_ADDR0;
	hmpu6050->clk_source = MPU6050_CLKSEL_INTERNAL;
	hmpu6050->gy_fs = MPU6050_GY_FS_SEL_250;
	hmpu6050->acc_fs = MPU6050_ACC_FS_SEL_2G;
	hmpu6050->gy_lsb = 131.0f;
	hmpu6050->acc_lsb = 16384.0f;
	return MPU6050_OK;
}

MPU6050_Status MPU6050_Reset(MPU6050_HandleTypeDef *hmpu6050){
	// Verify that the configuration struct is not NULL
	assert(hmpu6050 != NULL);

	HAL_StatusTypeDef status;
	uint8_t data = 0x80;
	status = HAL_I2C_Mem_Write(hmpu6050->hi2c, hmpu6050->addr, MPU6050_PWR_MGMT_1, 1,&data, 1, MPU6050_TIMEOUT);
	if(status != HAL_OK){
			#ifdef MPU6050_DEBUG
			printf("ERROR: unable to reset the MPU6050 sensor!\r\n");
			#endif
			return MPU6050_I2C_ERR;
		} else {
			#ifdef MPU6050_DEBUG
			printf("Reset MPU6050 chip!\r\n");
			#endif
		}
	return MPU6050_OK;
}

void MPU6050_ExtractAcc(MPU6050_HandleTypeDef *hmpu6050, MPU6050_Data_T *data, const uint8_t *buff){
	int16_t ACC_X_RAW = (int16_t)(buff[0] << 8 | buff [1]);
	int16_t ACC_Y_RAW = (int16_t)(buff[2] << 8 | buff [3]);
	int16_t ACC_Z_RAW = (int16_t)(buff[4] << 8 | buff [5]);

	data->acc_x = (float)ACC_X_RAW / hmpu6050->acc_lsb;
	data->acc_y = (float)ACC_Y_RAW / hmpu6050->acc_lsb;
	data->acc_z = (float)ACC_Z_RAW / hmpu6050->acc_lsb;
}

void MPU6050_ExtractGyro(MPU6050_HandleTypeDef *hmpu6050, MPU6050_Data_T *data, const uint8_t *buff){
	int16_t GYR_X_RAW = (int16_t)(buff[0] << 8 | buff [1]);
	int16_t GYR_Y_RAW = (int16_t)(buff[2] << 8 | buff [3]);
	int16_t GYR_Z_RAW = (int16_t)(buff[4] << 8 | buff [5]);

	data->gyro_x = (float)GYR_X_RAW / hmpu6050->gy_lsb;
	data->gyro_y = (float)GYR_Y_RAW / hmpu6050->gy_lsb;
	data->gyro_z = (float)GYR_Z_RAW / hmpu6050->gy_lsb;
}

void MPU6050_ExtractTemperature(MPU6050_HandleTypeDef *hmpu6050, MPU6050_Data_T *data, const uint8_t *buff){
	int16_t TEMP_RAW = (int16_t)((buff[0]<<8) | buff[1]);
	data->temp = (((float)TEMP_RAW)/360.0f) + 36.53f;
}

MPU6050_Status MPU6050_ReadData(MPU6050_HandleTypeDef *hmpu6050, MPU6050_Data_T *data){
	assert(hmpu6050 != NULL);
	assert(data != NULL);
	uint8_t buff[14] = {0};
	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Read (hmpu6050->hi2c, hmpu6050->addr,
			MPU6050_ACCEL_XOUT_H, 1, buff, sizeof(buff), MPU6050_TIMEOUT);
	if(status != HAL_OK){
		#ifdef MPU6050_DEBUG
		printf("ERROR: unable to read the MPU6050 sensor data!\r\n");
		#endif
		return MPU6050_I2C_ERR;
	} else {
		#ifdef MPU6050_DEBUG
		//printf("Reset MPU6050 chip!\r\n");
		#endif
	}

	// Calculate data values from raw data
	MPU6050_ExtractAcc(hmpu6050, data, buff);
	MPU6050_ExtractGyro(hmpu6050, data, &buff[8]);
	MPU6050_ExtractTemperature(hmpu6050, data, &buff[6]);

	return MPU6050_OK;
}

