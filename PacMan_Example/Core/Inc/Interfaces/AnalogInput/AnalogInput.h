#ifndef ANALOG_INPUT_INCLUDE_GUARD
#define ANALOG_INPUT_INCLUDE_GUARD

/**
  * @file AnalogInput.h
  * @date June 2025
  * @brief This file contains the declaration of the AnalogInput class which
  * declares an abstract interface for an analog input.
  * @defgroup ELE3312
  * @{
  */

#include <cstdint>

namespace ELE3312 {
	class AnalogInput{
		public:
			virtual void update() = 0;
			virtual int32_t getRawValue() const = 0;
			virtual uint16_t getPercentage() const = 0;
		protected:
			int32_t raw;
	};
} // End of namespace ELE3312

/**
  * @}
  */ // End of documentation group ElE3312
#endif
