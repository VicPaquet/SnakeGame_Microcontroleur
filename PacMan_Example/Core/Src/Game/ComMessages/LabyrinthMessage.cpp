/**
  * @file LabyrinthMessage.cpp
  * @date June 2025
  * @brief This file implements the LabyrinthMessage class that defines 
  * messages exchanged in the labyrinth phase of the game.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "Game/ComMessages/LabyrinthMessage.h"

using namespace ELE3312;

/** @brief Constructor for a LabyrinthMessage.
  * @param [in] x The x position of the player sending the message.
  * @param [in] y The y position of the player sending the message.
  * @param [in] isCollision Indicates that a collision with the opponent was detected.
  */
LabyrinthMessage::LabyrinthMessage(uint16_t x, uint16_t y, CollisionType collisionType) : Message{MessageType::Position} {
	data.x = x;
	data.y = y;
	data.collision = collisionType;
}

/** @brief Default constructor.
  * @details The default constructor initializes the coordinates of the
  * the player sending the message to zero.
  */
LabyrinthMessage::LabyrinthMessage() : Message{MessageType::Position}{
	data.x = 0;
	data.y = 0;
	data.collision  = CollisionType::None;
}

// Setters

/** @brief Sets the type of the message.
  * @param [in] newType the type of the message.
  */
void LabyrinthMessage::setType(MessageType newType){
	type = newType;
}

/** @brief Sets the binary data of the message.
  * @details This method can be used to initialize a message object 
  * by providing the binary representation of it's content and then use
  * the object to access the messages content and thus effectively 
  * deserialize the message object from it's binary representation.
  * @param [in] newData An array of bytes representing the message content.
  * @param [in] length The length of the provided data array.
  */
 void LabyrinthMessage::setData(uint8_t *newData, size_t length) {
	if(length != sizeof(LabyrinthMessageData)){
		return;
	}
	data.x = ((LabyrinthMessageData*)newData)->x;
	data.y = ((LabyrinthMessageData*)newData)->y;
	data.collision = ((LabyrinthMessageData*)newData)->collision;
 }

/** @brief Sets the x coordinate of the player.
  * @param [in] newX The x coordinate of the player.
  */
void LabyrinthMessage::setX(uint16_t newX){
	data.x = newX;
}

/** @brief Sets the y coordinate of the player.
  * @param [in] newY The y coordinate of the player.
  */
void LabyrinthMessage::setY(uint16_t newY){
	data.y = newY;
}

/** @brief Sets whether or not a collision was detected.
  * @param collision The type of the detected collision.
  */
void LabyrinthMessage::setCollisionType(CollisionType collision) {
	data.collision = collision;
}

// Getters

/** @brief Returns the type of the message.
  * @retval The type of the message as a MessageType enum.
  */
MessageType LabyrinthMessage::getType(){
	return type;
}

/** @brief Returns the size/length of the message.
  * @retval The number of bytes constituting the binary representation of the message.
  */
size_t LabyrinthMessage::getSize() const {
	return sizeof(LabyrinthMessageData);
}

/** @brief Returns a pointer to an array containing the binary representation of the 
  * the message's content.
  * @retval An array containing the binary representation of the message's content.
  */
const uint8_t *LabyrinthMessage::getData() const {
	return (uint8_t *)&data;
}

/** @brief Returns a textual representation of the content of the message.
  * @retval A textual representation of the content of the message.
  */
std::string LabyrinthMessage::toString() const {
	return "[" + std::to_string(data.x) + ", " + std::to_string(data.y) + "]";
}

/** @brief Returns the x coordinate of the sending player in the labyrinth.
  * @retval The players x coordinate.
  */
uint16_t LabyrinthMessage::getX() const{
	return data.x;
}

/** @brief Returns the y coordinate of the sending player in the labyrinth.
  * @retval The y coordinated of the sending player in the labyrinth.
  */
uint16_t LabyrinthMessage::getY() const{
	return data.y;
}

/**
  * @brief Returns the type of the collision.
  * @return The type of the detected collision.
  */
CollisionType LabyrinthMessage::getCollisionType() const {
	return data.collision;
}

/**
  *@}
  */ // End of the ELE3312 documentation group
