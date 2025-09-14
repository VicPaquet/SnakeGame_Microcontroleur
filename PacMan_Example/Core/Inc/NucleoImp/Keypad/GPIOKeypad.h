#ifndef GPIOKEYPAD_INCLUDE_GUARD
#define GPIOKEYPAD_INCLUDE_GUARD

/**
  * @file GPIOKeypad.h
  * @date June 2025
  * @brief This file contains an implementation of the Keypad class that represents an abstract
  * interface to a numeric key pad.
  * @defgroup ELE3312
  * @{
  */

#include "Interfaces/Keypad/Keypad.h"
#include "main.h"


namespace ELE3312{

	class GPIOKeypad: public Keypad {
		public:
			GPIOKeypad();
			void setup(GPIO_TypeDef * gpio);
			~GPIOKeypad() = default;
			virtual uint32_t numberOfKeysPressed() const override;
			virtual bool isKeyPressed(const KeyCode code) const override;
			virtual bool isAnnyKeyPressed() const override;
			virtual KeyCode getFirstKeyPressed() override;
			virtual char getFirstKeyCharacter() override;
			virtual void update() override;
		private:
			GPIO_TypeDef * gpio;
			uint32_t keysPressed = 0;
			const uint32_t numKeys = 16;
			const uint32_t numRows = 4;
			const uint32_t numCols = 4;
			const uint32_t rowMask = 0xF;
			uint32_t currentRow = numRows;
			bool keyPressDetected = false;
			// Private methods

			void selectRow(int32_t row) const;

	};

} // End of namespace ELE3312

/**
  * @}
  */ // End of documentation group ELE3312
#endif
