#ifndef RGBLED_H_
#define RGBLED_H_

/**
  * @file RGBLight.h
  * @date June 2025
  * @brief The class defines the interface to an RGB light source connected to the micro controller. 
  * 
  * @defgroup ELE3312
  * @{
  */
#include "Interfaces/RGBLight/RGBLight.h"
#include "main.h"

namespace ELE3312 {
	class RGBLED : public RGBLight {
		public:
			RGBLED();
			void setup(TIM_HandleTypeDef *htim, uint32_t channelRed, uint32_t channelGreen, uint32_t channelBlue);
			~RGBLED() = default;
			virtual void setColorRGB(uint8_t red, uint8_t green, uint8_t blue) override;
			virtual void toggle() override;
			virtual void turnOn() override;
			virtual void turnOff() override;
			virtual bool isOn() override;
		private:
			TIM_HandleTypeDef *htim;
			uint32_t channelRed;
			uint32_t channelGreen;
			uint32_t channelBlue;
			uint8_t red = 0;
			uint8_t green = 0;
			uint8_t blue = 0;

			// Private methods
			void setDutyCycleForChannel(uint32_t channel, uint32_t dc);

	};

} // End of namespace ELE3312

/**
  * @}
  */ // End of ELE3312 documentation group

#endif
