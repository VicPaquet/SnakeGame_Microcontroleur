/**
 * @file HCSR04Distance.h
 * @date June 2025
 * @brief The file contains an implementation for the HC-SR04 distance sensor.
 * @defgroup ELE3312
 * @{
 */
#ifndef HCSR04DISTANCE_H_
#define HCSR04DISTANCE_H_

#include <main.h>
#include "Interfaces/Distance/Distance.h"


	class HCSR04Distance : public Distance{
		public:
			HCSR04Distance();
			void setup(float minDistance, float maxDistance, TIM_HandleTypeDef *timerHandle);
			virtual ~HCSR04Distance() = default;
			void measuer() override;
			float getDistance() const override;
			float getMinDistance() const override;
			float getMaxDistance() const override;
			void  setMinDistance(float min);
			void  setMaxDistance(float max);

			void enableMeasurement() override;
			void disableMeasurement() override;

			volatile static uint32_t echoReadyFlag; // TODO: can you make those variables private ? (encapsulation)
			volatile static uint32_t echoDuration;

		private:
			float distance; //!< Stores the measured distance.
			float minDistance; //!< Stores the minimal measurable distance.
			float maxDistance; //!< Stores the maximal measurable distance.
			TIM_HandleTypeDef *timerHandle; //!< Stores a handle to the time used for the PWM signal
	};


/** 
 *@}
 */ // End of documentation group ELE3312
#endif /* HCSR04DISTANCE_H_ */
