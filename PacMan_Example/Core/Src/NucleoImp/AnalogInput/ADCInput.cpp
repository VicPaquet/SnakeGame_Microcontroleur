/**
  * @file ADCInput.cpp
  * @date June 2025
  * @brief This file implements the ADCInput class that is used to read 
  * a voltage from an analog input pin of the micro controller.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "NucleoImp/AnalogInput/ADCInput.h"
#include <cmath>

using namespace ELE3312;

/**
 * @brief Constructor of the ADCInput class.
 */
ADCInput::ADCInput(): hadc{nullptr} {
}

/** @brief Initializes the ADCInput class.
 * @param adcHandle Handle to ADC micro-controller periphery.
 */
void ADCInput::setup(ADC_HandleTypeDef *adcHandle){
	hadc = adcHandle;
	// Determine the number of bits used for the analog to digital conversion
	uint16_t raw_res = (hadc->Instance->CR1 >> 24) & 0x3;
	switch(raw_res) {
		case 0x00: // 12 Bit resolution
			res = 12;
			break;
		case 0x01: // 10 Bit resolution
			res = 10;
			break;
		case 0x02: // 8 Bit resolution
			res = 8;
			break;
		case 0x03: // 4 Bit resolution
			res = 4;
			break;
	}
	maxValue = (uint32_t) pow(2, res);
}

/**
 * Destructor of the ADCInput class.
 */
ADCInput::~ADCInput() {
	if (hadc != nullptr){
		HAL_ADC_Stop(hadc);
	}
}

/**
 * @brief Reads the current value from the ADC and stores it in the object for later
 * usage.
 */
void ADCInput::update()  {
	HAL_ADC_Start(hadc); // Activates the ADC
	HAL_StatusTypeDef status = HAL_ADC_PollForConversion(hadc, 10);
	if( status == HAL_OK){
		raw = HAL_ADC_GetValue(hadc);
	} 
}


/**
 * @brief Returns the raw value of the ADC.
 * @return The raw value from the ADC.
 */
int32_t ADCInput::getRawValue() const  {
	return raw;
}

/**
 * @breif Returns an integral percentage of the current ADC value with respect
 * to the maximum value as defined by the ADC resolution.
 * @return Percentage value of the current raw ADC vale with respect to it's resolution.
 */
uint16_t ADCInput::getPercentage() const {
	return (raw*100)/maxValue;
}

/**
  * @}
  */ // End of documentation group ElE3312
