/**
  * @file RGBLED.cpp
  * @date June 2025
  * @brief This file contains the implementation of the RGBLED class which is a concrete implementation for
  * the RGBLight interface. The purpose of this class is to control an RGB LED connected to the micro controller.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "NucleoImp/RGBLight/RGBLED.h"

using namespace ELE3312;

/**
  * @brief Setup routine for the RGBLED class that is used to control an RGB LED connected to three GPIO
  * ports in PWM mode.
  * @param [in] htim Pointer to timer handle that is used to control the PWM signal.
  * @param [in] channelRed Percentage of the duty cycle for the red led.
  * @param [in] channelGreen Percentage of the duty cycle for the green led.
  * @param [in] channelBlue Percentage of the duty cycle for the blue led.
  */
void RGBLED::setup(TIM_HandleTypeDef *htim, uint32_t channelRed, uint32_t channelGreen, uint32_t channelBlue) {
	this->htim = htim;
	this->channelRed   = channelRed;
	this->channelGreen = channelGreen;
	this->channelBlue  = channelBlue;
	// Set a defined initial state of the PWM generation
	setDutyCycleForChannel(channelRed, 0);
	setDutyCycleForChannel(channelGreen, 0);
	setDutyCycleForChannel(channelBlue, 0);
	
}

/**
  * @brief Constructor for the RGBLED class that is used to control an RGB LED connected to three GPIO
  */
RGBLED::RGBLED() {
	// Set a defined initial state of the PWM generation
	red = 0;
	green = 0;
	blue = 0;
}

/**
  * @brief Sets the duty cycle for a specific PWM channel.
  * @param channel The channel for which the PWM duty cycle is going to be changed.
  * @param dc The new duty cycle value for the specified channel.
  */
void RGBLED::setDutyCycleForChannel(uint32_t channel, uint32_t dc) {
	if (htim == nullptr){
		return;
	}
	switch (channel){
		case TIM_CHANNEL_1:
			htim->Instance->CCR1 = dc;
			break;
		case TIM_CHANNEL_2:
			htim->Instance->CCR2 = dc;
			break;
		case TIM_CHANNEL_3:
			htim->Instance->CCR3 = dc;
			break;
		case TIM_CHANNEL_4:
			htim->Instance->CCR4 = dc;
			break;
	};
}

/**
  * @brief Sets the color of the RGB LED which is a combination of the 3 primary colors (reg, green, blue).
  * @param red Percentage of red.
  * @param green Percentage of green.
  * @param blue Percentage of blue.
  */
void RGBLED::setColorRGB(uint8_t red, uint8_t green, uint8_t blue){
	this->red = red;
	this->green = green;
	this->blue = blue;
	// Set Duty cycle of color PWM signal based on the color value
	setDutyCycleForChannel(channelRed, red*100);
	setDutyCycleForChannel(channelGreen, green*100);
	setDutyCycleForChannel(channelBlue, blue*100);

}

/**
  * @brief Toggles the state of the RGB LED (between ON and OFF).
  */
void RGBLED::toggle(){
	if(lightIsOn){
		turnOff();
	} else {
		turnOn();
	}
}

/** 
  * @brief Turns the RGB LED on.
  */
void RGBLED::turnOn(){
	// Start PWM
	HAL_TIM_PWM_Start(htim, channelRed);
	HAL_TIM_PWM_Start(htim, channelGreen);
	HAL_TIM_PWM_Start(htim, channelBlue);
	lightIsOn = true;
}

/**
  * @brief Turns the RGB LED on.
  */
void RGBLED::turnOff(){
	// Stop PWM
	HAL_TIM_PWM_Stop(htim, channelRed);
	HAL_TIM_PWM_Stop(htim, channelGreen);
	HAL_TIM_PWM_Stop(htim, channelBlue);
	lightIsOn = false;
}

/**
  * @brief Returns whether or not the LED is turned on or off
  * @retval true The LED is on.
  * @retval false The LED is off.
  */
bool RGBLED::isOn(){
	return lightIsOn; 
}


/**
  * @}
  */ // End of documentation group ELE3312
