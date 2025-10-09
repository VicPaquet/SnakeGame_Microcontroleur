/**
 * @file HCSR04Distance.cpp
 * @date June 2025
 * @brief This file contains the implementation of the HCSR04Distance class
 * that implements the Distance interface used to measure distances with an 
 * ultrasonic sensor.
 * @defgroup ELE3312
 * @{
 */

#include "NucleoImp/Distance/HCSR04Distance.h"

#ifdef __cplusplus
extern "C" {
#endif

volatile uint32_t IsFirstCaptured = 0; //!< Flag that indicates the detection of the rising edge of the echo signal.

/** @brief Handler function that is called for every input capture event of the timer.
  * @param [in] htim A Pointer to the timer periphery handler.
  */
void captureCallback(TIM_HandleTypeDef *htim) {

	static uint32_t start_echo;
	static uint32_t end_echo;

	if (IsFirstCaptured == 0) {
		start_echo = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // Read current Timer value
		IsFirstCaptured = 1;  // Set the first captured as true
	} else {
		end_echo = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // Read current Timer value

		if (start_echo < end_echo){
			HCSR04Distance::echoDuration = end_echo - start_echo;
		}else{
			HCSR04Distance::echoDuration = (200000 - start_echo) + end_echo;
		}

		IsFirstCaptured = 0;
		HCSR04Distance::echoReadyFlag = 1;
	}

}

#ifdef __cplusplus
}
#endif

volatile uint32_t HCSR04Distance::echoDuration = 0; //!< Variable to calculate the duration of the echo signal from the sensor
volatile uint32_t HCSR04Distance::echoReadyFlag = 0; //!< Flag that indicates that a new distance measure value is ready.


/** @brief Enables the measurement of the distance between 
  * the HC-SR04 sensor and a close object.
  * @details The function starts the a PWM signal that
  * generates 10 microsecond pulses at the specified GPIO pin.
  * The sensor initiates a distance measure when it receives 
  * such a pulse. Furthermore the pulse is generated repeatedly
  * at the PWM's frequency.
  */
void HCSR04Distance::enableMeasurement(){
  HAL_TIM_PWM_Start(timerHandle, TIM_CHANNEL_2);
}

/** @brief Disables the distance measurement.
  * @details To disable the distance measurement the PWM signal is stopped.
  * Without the PWM signal no measurement is triggered since there is no 
  * 10 microsecond pulse that would start a measurement with the HC-SR04 sensor.
  */
void HCSR04Distance::disableMeasurement(){
  HAL_TIM_PWM_Stop(timerHandle, TIM_CHANNEL_2);
}

/** @brief Setup routine for the HCSR04Distance class.
  * @param [in] minDistance The minimal distance the sensor can reliably measure. 
  * @param [in] maxDistance The maximal distance the sensor can reliably measure. Values that lie
  * beyond the maxDistance value are replaced with the maxDistance value. 
  * @param [in] timerHandle A handle to a timer periphery that is used to manage the generation of the PWM signal 
  * that is used to trigger distance measurements with the ultrasonic sensor.
  */
void HCSR04Distance::setup(float minDistance, float maxDistance, TIM_HandleTypeDef *timerHandle) {
			this->minDistance = minDistance;
			this->maxDistance = maxDistance;
			this->timerHandle = timerHandle;
}

/** @brief Constructor for the HCSR04Distance class.
  */
HCSR04Distance::HCSR04Distance(){
	distance = 0.0f;
	minDistance = 0.0f;
	maxDistance = 0.0f;
	timerHandle = nullptr;
}

/** @brief Measures the distance to an object with the ultrasonic 
  * sensor HCSR04.
  * @details The function depends on the fact that a measurement is 
  * is ready. The echoReadyFlag indicates that a raw measurement is ready.
  * If the flag is not set, the function returns without changing the 
  * the distance measure.
  */
void HCSR04Distance::measuer(){
	if(echoReadyFlag != 1){
		return;
	} 
	echoReadyFlag = 0; // Don't read the same value multiple times.
	// Calculate distance
	float result = 0.034f * (echoDuration/2.0f);
	if (result < minDistance ){
		distance = minDistance;
		return;
	}
	if (result > maxDistance) {
		distance = maxDistance;
		return;
	}
	distance = result;
}

/** @brief Returns the measured distance to an object in centimeters.
  * @retval The distance to an object in centimeters.
  */
float HCSR04Distance::getDistance() const{
	return distance;
}

/** @brief Returns the minimal measurable distance.
  * @details If the distance measured is actually closer than 
  * the predefined minimal distance the object will return 
  * the defined minimal value. It can thus be necessary to 
  * retrieve the currently defined minimal distance.
  * @retval The minimal distance in centimeters.
  */
float HCSR04Distance::getMinDistance() const {
	return minDistance;
}

/** @brief Returns the maximum measurable distance.
  * @details If the measured distance is actually farther than
  * the predefined maximal distance the object will return 
  * the predefined maximal distance instead of the actual 
  * measured distance.
  * @retval The maximal measurable distance.
  */
float HCSR04Distance::getMaxDistance() const {
	return maxDistance;
}

/** @brief Sets the minimal distance.
  * @param [in] min The minimal measurable distance in centimeters.
  */
void  HCSR04Distance::setMinDistance(float min){
	minDistance = min;
}

/** @brief Sets the maximal distance.
  * @param [in] max The maximal measurable distance.
  */
void  HCSR04Distance::setMaxDistance(float max){
	maxDistance = max;
}


