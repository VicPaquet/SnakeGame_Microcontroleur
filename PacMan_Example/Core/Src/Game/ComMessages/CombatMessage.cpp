/**
  * @file CombatMessage.cpp
  * @date June 2025
  * @brief This file contains the implementation of the CombatMessage class
  * that is used to exchange game information of the combat phase of the game.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "Game/ComMessages/CombatMessage.h"

using namespace ELE3312;

/** @brief The default constructor of a CombatMessage object.
  */
CombatMessage::CombatMessage(): Message{MessageType::PowerValue}, barValue{0}{}

// Setters

/** @brief Sets the type of the message.
  * @param newType The type of the message.
  */
void CombatMessage::setType(MessageType newType){
	type = newType;
}

/** @brief Sets the data of the message.
  * @details This method is intended for deserialization of a CombatMessge from
  * it's binary representation.
  * @param newData The binary data of the message.
  * @param length The length of the uint8_t array containing the data of the message.
  */
 void CombatMessage::setData(uint8_t *newData, size_t length) {
	 if (length != sizeof(barValue)){
		 return;
	 }
	 barValue = *(uint16_t*) newData;
}

/** @brief Sets the bar value for the current user. 
  * @param [in] value The current value of the progress/power bar of the 
  * corresponding player.
  */
void CombatMessage::setBarValue(uint16_t value){
	barValue = value;
}

// Getters

/** @brief Returns the type of the message.
  * @retval The type of the message as a MessageType.
  */
MessageType CombatMessage::getType(){
	return type;
}

/** @brief Returns the size of the message as in bytes of
  * the serialized message.
  * @retval The number of bytes for the binary representation of the 
  * message.
  */
 size_t CombatMessage::getSize() const {
	return sizeof(barValue);
 }

/** @brief Returns the binary representation of the message as
  * a pointer to an array of uint8_t integers.
  * @retval Binary representation of the message.
  */
 const uint8_t *CombatMessage::getData() const {
	return (uint8_t*)&barValue;
 }

/** @brief Returns a textual representation of the message.
  * @details The toString method can be used for debug purposes
  * to display the content of a message.
  * @retval A textual representation of the content of the message.
  */
 std::string CombatMessage::toString() const {
	return "<" + std::to_string(barValue) + ">";
 }

/** @brief Returns the value of the progress/power bar contained in
  * the message.
  * @retval The integer value of the progress/power bar contained in the message.
  */
uint16_t CombatMessage::getBarValue() const{
	return barValue;
}

/**
  * @}
  */ // Close ELE3312 documentation group
