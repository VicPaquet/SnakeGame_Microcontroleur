/**
 * @file GPIOKeypad.cpp
 * @date June 2025
 * @brief This file implements the Keypad class interface for a numeric 
 * keypad connected to the GPIO (General Purpose Input Output) port of the
 * NUCLEO board..
 * @defgroup ELE3312
 * @{
 */
#include "NucleoImp/Keypad/GPIOKeypad.h"

using namespace ELE3312;

/**
  * @brief Setup routine fort the GPIOKeypad class.
  * @param [in] gpio Pointer to a GPIO HAL handle. 
  */
void GPIOKeypad::setup(GPIO_TypeDef * gpio) {
	this->gpio = gpio;
	selectRow(numRows);
}

/**
  * @brief Constructor fort the GPIOKeypad class.
  */
GPIOKeypad::GPIOKeypad() {
	gpio = nullptr;
}
/**
  * @brief Function that returns the number of individual keys of 
  * the keypad that were pressed. The function does not return the 
  * number of times a key was pressed rather the number of simultaneously 
  * pressed keys.
  * @return Number of individual keys pressed simultaneously.
  */
uint32_t GPIOKeypad::numberOfKeysPressed() const {
	if(!keyPressDetected){
		return 0;
	}
	uint32_t numKeysPressed = 0;
	for (uint32_t i = 0; i < numKeys; ++i){
		numKeysPressed = (keysPressed >> i) & 0x1;
	}
	return numKeysPressed;
}

/**
  * @brief Checks whether a certain key was pressed when the state
  * of the keypad was read.
  * @param code The Key-Code of the key that is going to be checked for.
  * @retval true The specified key was pressed when the keypad status was read.
  * @retval false The specified key was not pressed.
  */
bool GPIOKeypad::isKeyPressed(const KeyCode code) const {
	if(!keyPressDetected){
		return false;
	}
	return (static_cast<uint32_t>(code) & keysPressed) > 0;
}

/**
  * @brief Returns the Key-Code of the first key of the keypad 
  * that was pressed when the state of the keypad was read.
  * @return The Key-Code of the first key pressed.
  */
KeyCode GPIOKeypad::getFirstKeyPressed() {
	KeyCode code = KeyCode::UNKNOWN;
	if(!keyPressDetected){
		return code;
	}
	uint32_t testCode = 0;
	for(uint32_t idx = 0; idx < numKeys; ++idx){
		testCode = 1 <<  idx;
		if( keysPressed & testCode){
			// Found non zero value -> first key on the keypad pressed
			code = static_cast<KeyCode>(testCode);
			break;
		}
	}
	keysPressed = 0; // Reset internal key storage
	keyPressDetected = false;
	return code;
}

/**
  * @brief Returns the ASCII character of the first key of the 
  * keypad that was pressed when the state of the keypad was read.
  * @return ASCII character of the first key pressed.
  */
char GPIOKeypad::getFirstKeyCharacter(){
	char result = '-';
	if(!keyPressDetected){
		return result;
	}

	switch(getFirstKeyPressed()){
		case KeyCode::ZERO:
			result = '0';
			break;
		case KeyCode::ONE:
			result = '1';
			break;
		case KeyCode::TWO:
			result = '2';
			break;
		case KeyCode::THREE:
			result = '3';
			break;
		case KeyCode::FOUR:
			result = '4';
			break;
		case KeyCode::FIVE:
			result = '5';
			break;
		case KeyCode::SIX:
			result = '6';
			break;
		case KeyCode::SEVEN:
			result = '7';
			break;
		case KeyCode::EIGHT:
			result = '8';
			break;
		case KeyCode::NINE:
			result = '9';
			break;
		case KeyCode::A:
			result = 'A';
			break;
		case KeyCode::B:
			result = 'B';
			break;
		case KeyCode::C:
			result = 'C';
			break;
		case KeyCode::D:
			result = 'D';
			break;
		case KeyCode::STAR:
			result = '*';
			break;
		case KeyCode::OCTOTHORPE:
			result = '#';
			break;
		default:
			result = '-'; // Unknown
	}
	keysPressed = 0; // Reset internal key storage
	keyPressDetected = false;
	return result;
}

/**
  * @brief Indicates that a key-press was detected.
  * @retval true At least one key was pressed.
  * @retval false No key was pressed.
  */
bool GPIOKeypad::isAnnyKeyPressed() const {
	return keyPressDetected;
}

/**
  * @brief Reads a row from the connected keypad and prepares the
  * next row for reading it's state. 
  */
void GPIOKeypad::update() {
	if(keyPressDetected){
		return;
	}
	// Shift all values in keysPressed to the left in order to make space for the new values
	keysPressed = keysPressed << numCols;
	// Add the GPIO values (which we assume to be continues and starting at 0) to the pressed keys variable
	keysPressed |= (~gpio->IDR) & rowMask;

	currentRow -= 1;

	if(currentRow == 0){
		currentRow = numRows;
		// When at least one key-press was detected and all rows have been read
		// stop further acquisition until the program has accessed the keypad object.
		// This way a key-press does not get lost.
		if(keysPressed > 0 ){
			keyPressDetected = true;
		}
	}
	// Prepare next row for reading
	selectRow(currentRow);
}

/**
  * @brief Internal function that selects a row of the keypad for readout.
  * @param row The number of the row starting at 0.
  */
void GPIOKeypad::selectRow(int32_t row) const{
	constexpr uint32_t allPinsHigh = ROW1_Pin | ROW2_Pin | ROW3_Pin | ROW4_Pin; // Prepare bitmask to activate all four rows
	gpio->ODR |= allPinsHigh; // Make all pins high -> no row is selected
	switch(row){ // Make the pin of the corresponding row low -> selects a row of buttons to read out
		case 1:
			gpio->ODR &= ~ROW1_Pin;
			break;
		case 2:
			gpio->ODR &= ~ROW2_Pin;
			break;
		case 3:
			gpio->ODR &= ~ROW3_Pin;
			break;
		case 4:
			gpio->ODR &= ~ROW4_Pin;
			break;
	}
}

/**
  * @}
  */ // End of documentation group ELE3312
