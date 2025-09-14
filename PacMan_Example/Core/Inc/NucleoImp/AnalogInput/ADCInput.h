#ifndef ADCINPUT_INCLUDE_GUARD
#define ADCINPUT_INCLUDE_GUARD

/**
  * @file ADCInclude.h
  * @date June 2025
  * @brief This file declares the ADCInput class that implements the AnalogInput interface.
  * It is used to read an analog input at one of the micro controllers GPIO pins.
  * @defgroup ELE3312
  * @{
  */

#include <cstdint>
#include "Interfaces/AnalogInput/AnalogInput.h"
#include "main.h"

namespace ELE3312 {
	class ADCInput : public AnalogInput{
		public:
			ADCInput();
			void setup(ADC_HandleTypeDef *hadc);
			~ADCInput();
			virtual void update() override;
			virtual int32_t getRawValue() const override;
			virtual uint16_t getPercentage() const override;
		private:
			uint16_t res; //!< The number of bits used for the ADC conversion.
			uint32_t maxValue; //!< The maximum value the ADC can reach
			ADC_HandleTypeDef *hadc;
	};
} // End of namespace ELE3312

/**
  * @}
  */ // End of documentation group ElE3312
#endif
