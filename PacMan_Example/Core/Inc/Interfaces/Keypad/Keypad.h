#ifndef KEYPAD_INCLUDE_GUARD
#define KEYPAD_INCLUDE_GUARD

/**
  * @file Keypad.h
  * @date June 2025
  * @brief This file contains the definition of the abstract Keypad class that defines 
  * an interface to a numeric key pad.
  * @defgroup ELE3312
  * @{
  */

#include <cstdint>

namespace ELE3312{
	/** Key codes for a number keypad
	  * The values of the enumerations are chosen so that each
	  * bit in the key code represents a button on the key pad. 
	  * The bits are following the same order as the buttons on the 
	  * key pad.
	  */
	enum class KeyCode : int{
		// First row
		ONE = 0x1,
		TWO = 0x2,
		THREE = 0x4,
		A = 0x8,
		// Second row
		FOUR = 0x10,
		FIVE = 0x20,
		SIX = 0x40,
		B = 0x80,
		// Third row
		SEVEN = 0x100,
		EIGHT = 0x200,
		NINE = 0x400,
		C = 0x800,
		// Fourth row
		STAR = 0x1000,
		ZERO = 0x2000,
		OCTOTHORPE = 0x4000,
		D = 0x8000,
		// Special value 
		UNKNOWN = 0xFFFF
	};

	class Keypad {
		public:
			virtual uint32_t numberOfKeysPressed() const  = 0;
			virtual bool isKeyPressed(const KeyCode code) const = 0;
			virtual bool isAnnyKeyPressed() const = 0;
			virtual KeyCode getFirstKeyPressed() = 0;
			virtual char getFirstKeyCharacter() = 0;
			virtual void update() = 0;

	};


} // End of namespace ELE3312

/**
  * @}
  */ //End of documentation group ELE3312

#endif
