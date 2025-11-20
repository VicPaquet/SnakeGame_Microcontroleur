#include <Game/ComMessages/SnakeGameMessage.h>

/** @brief Constructor for a LabyrinthMessage.
  * @param [in] x The x position of the player sending the message.
  * @param [in] y The y position of the player sending the message.
  * @param [in] isCollision Indicates that a collision with the opponent was detected.
  */
SnakeGameMessage::SnakeGameMessage(uint16_t x, uint16_t y, uint8_t direction, CollisionType collisionType) : Message{MessageType::Position} {
	data.x = x;
	data.y = y;
	data.direction = direction;
	data.collision = collisionType;
}

/** @brief Default constructor.
  * @details The default constructor initializes the coordinates of the
  * the player sending the message to zero.
  */
SnakeGameMessage::SnakeGameMessage() : Message{MessageType::Position}{
	data.x = 0;
	data.y = 0;
	data.direction = 0;
	data.collision  = CollisionType::None;
}

// Setters

/** @brief Sets the type of the message.
  * @param [in] newType the type of the message.
  */
void SnakeGameMessage::setType(MessageType newType){
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
 void SnakeGameMessage::setData(uint8_t *newData, size_t length) {
	if(length != sizeof(SnakeGameMessageData)){
		return;
	}
	data.x = ((SnakeGameMessageData*)newData)->x;
	data.y = ((SnakeGameMessageData*)newData)->y;
	data.direction = ((SnakeGameMessageData*)newData)->direction;
	data.collision = ((SnakeGameMessageData*)newData)->collision;
 }

/** @brief Sets the x coordinate of the player.
  * @param [in] newX The x coordinate of the player.
  */
void SnakeGameMessage::setX(uint16_t newX){
	data.x = newX;
}

/** @brief Sets the y coordinate of the player.
  * @param [in] newY The y coordinate of the player.
  */
void SnakeGameMessage::setY(uint16_t newY){
	data.y = newY;
}

/** @brief Sets whether or not a collision was detected.
  * @param collision The type of the detected collision.
  */
void SnakeGameMessage::setDirection(uint8_t direction) {
	data.direction = direction;
}

void SnakeGameMessage::setCollisionType(CollisionType collision) {
	data.collision = collision;
}

// Getters

/** @brief Returns the type of the message.
  * @retval The type of the message as a MessageType enum.
  */
MessageType SnakeGameMessage::getType(){
	return type;
}

/** @brief Returns the size/length of the message.
  * @retval The number of bytes constituting the binary representation of the message.
  */
size_t SnakeGameMessage::getSize() const {
	return sizeof(SnakeGameMessageData);
}

/** @brief Returns a pointer to an array containing the binary representation of the 
  * the message's content.
  * @retval An array containing the binary representation of the message's content.
  */
const uint8_t *SnakeGameMessage::getData() const {
	return (uint8_t *)&data;
}

/** @brief Returns a textual representation of the content of the message.
  * @retval A textual representation of the content of the message.
  */
std::string SnakeGameMessage::toString() const {
	return "[" + std::to_string(data.x) + ", " + std::to_string(data.y) + "]";
}

/** @brief Returns the x coordinate of the sending player in the labyrinth.
  * @retval The players x coordinate.
  */
uint16_t SnakeGameMessage::getX() const{
	return data.x;
}

/** @brief Returns the y coordinate of the sending player in the labyrinth.
  * @retval The y coordinated of the sending player in the labyrinth.
  */
uint16_t SnakeGameMessage::getY() const{
	return data.y;
}

uint8_t SnakeGameMessage::getDirection() const {
	return data.direction;
}

/**
  * @brief Returns the type of the collision.
  * @return The type of the detected collision.
  */
CollisionType SnakeGameMessage::getCollisionType() const {
	return data.collision;
}


