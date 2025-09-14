/**
  * @file Message.cpp
  * @date June 2025
  * @brief This file implements the Message class that serves as a base class and interface for
  * specialized message classes of the different game phases.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "Interfaces/Communication/Message.h"

using namespace ELE3312;

/** @brief Constructor for a Message object.
  * @param [in] type The type of the message.
  */
Message::Message(MessageType type) : type{type} {
}

// Setters

/** @brief Sets the type of the message.
  * @param [in] newType The type of the message.
  */
void Message::setType(MessageType newType){
	type = newType;
}


/** @brief Sets the valid property of the message.
  * @param [in] value Boolean value whether or not the message is valid.
  */
void Message::setValid(bool value){
	valid = value;
}
// Getters

/** @brief Returns the type of the message.
  * @retval The type of the message.
  */
MessageType Message::getType(){
	return type;
}

/** @brief Indicates the validity of a message.
  * @retval true The message is valid.
  * @retval false The message is not valid.
  */
bool Message::isValid(){
	return valid;
}

/**
  * @}
  */ // End of ELE3312 documentation group
