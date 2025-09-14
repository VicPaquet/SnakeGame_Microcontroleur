/**
  * @file PlayerChoice.cpp
  * @date June 2025
  * @brief This file implements the PlayerChiceMessage class that is used to
  * exchange the player's character choice.
  * 
  * @defgroup ELE3312
  * @{
  */
#include "Game/ComMessages/PlayerChoiceMessage.h"

using namespace ELE3312;

/** @brief Constructor for the PlayerChoiceMessage class.
  * @param [in] playerCharacter The character the player has chosen in the menu (Pacman or Ghost).
  */
PlayerChoiceMessage::PlayerChoiceMessage(PlayerCharacter playerCharacter) : Message{MessageType::PlayerChoice},
	playerCharacter{playerCharacter} {
}

/** @brief Default constructor for the PlayerChoiceMessage class.
  */
PlayerChoiceMessage::PlayerChoiceMessage() : Message{MessageType::PlayerChoice}, playerCharacter{PlayerCharacter::Unknown} {}

// Setters

/** @brief Sets the type of the message.
  * @param [in] newType The type of the message.
  */
void PlayerChoiceMessage::setType(MessageType newType){
	type = newType;
}

/** @brief Sets the contents of the message by providing it's binary representation.
  * @param [in] newData A pointer to an array of bytes containing the binary representation of the message's data.
  * @param [in] length The number of bytes contained in the specified array of bytes.
  */
void PlayerChoiceMessage::setData(uint8_t *newData, size_t length){
	if (length < sizeof(PlayerCharacter)){
		playerCharacter = PlayerCharacter::Unknown;
		return;
	}
	playerCharacter = *(PlayerCharacter*)newData;
}

// Getters

/** @brief Returns the type of the message.
  * @retval The type of the message.
  */
MessageType PlayerChoiceMessage::getType(){
	return type;
}

/** @brief Returns the size of the message.
  * @retval The number of bytes contained in the binary representation of the message's content.
  */
size_t PlayerChoiceMessage::getSize() const{
	return sizeof(PlayerCharacter);
}

/** @brief Returns the binary representation of the message's content.
  * @retval An array of bytes representing the contents of the message.
  */
const uint8_t *PlayerChoiceMessage::getData() const{
	return  (uint8_t*)&playerCharacter;
}

/** @brief Returns a textual representation of the message's content.
  * @retval A text string containing the message's content.
  */
std::string PlayerChoiceMessage::toString() const{
	switch(playerCharacter){
		case PlayerCharacter::Pacman :
			return "Pacman";
		case PlayerCharacter::Ghost : 
			return "Ghost";
		case PlayerCharacter::Unknown:
		default :
			return "Unknown";

	}
	return "Unknown";
}

/** @brief Returns the character the player has chosen.
  * @retval The chosen character (e.g. PacMan or Ghost).
  */
PlayerCharacter PlayerChoiceMessage::getPlayerChoice() const{
	return playerCharacter;
}

/**
  *@}
  */ //End of documentation group ELE3312
