#ifndef RGBLIGHT_H_
#define RGBLIGHT_H_

/**
  * @file RGBLight.h
  * @date June 2025
  * @brief The class defines the interface to an RGB light source connected to the micro controller. 
  * 
  * @defgroup ELE3312
  * @{
  */

#include <cstdint>

namespace ELE3312 {
	class RGBLight{
		public:
			virtual void setColorRGB(uint8_t red, uint8_t green, uint8_t blue) = 0;
			virtual void toggle()= 0;
			virtual void turnOn() = 0;
			virtual void turnOff() = 0;
			virtual bool isOn() = 0;
		protected:
			bool lightIsOn = false;

	};
}// End of namespace ELE3312

/**
  * @}
  */

#endif
