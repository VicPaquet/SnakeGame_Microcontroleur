#ifndef CPP_MAIN_H_
#define CPP_MAIN_H_

/**
  * @file cpp_main.cpp
  * @date June 2025
  * @brief Contains the declarations for the main_run function and the structure that is
  * used to pass the periphery handles to the C++ classes.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "main.h"
/** @brief Structure that encapsulates the handles to the micro controllers 
  * peripherals.
  */
typedef struct {
	DAC_HandleTypeDef *hdac;
	TIM_HandleTypeDef *htim_dac;
	SPI_HandleTypeDef *hspi_tft;
	TIM_HandleTypeDef *htim_distance;
	TIM_HandleTypeDef *htim_led;
	I2C_HandleTypeDef *hi2c;
	UART_HandleTypeDef *huart;
	UART_HandleTypeDef *debug_uart;
	ADC_HandleTypeDef *hadc;
	GPIO_TypeDef *gpio_keypad;
} peripheral_handles;


#ifdef __cplusplus
extern "C" {
#endif

/** @brief Main function that is run in C++ environment.
  * @param handlers A structure that contains pointers to the different micro controller 
  * peripherals used in the game.
  */
void cpp_main(peripheral_handles *handles) ;

#ifdef __cplusplus
}
#endif

/**
  * @}
  */ // End of documentation group ELE3312
#endif /* CPP_MAIN_H_ */
