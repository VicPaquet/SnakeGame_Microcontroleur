#include <Game/ComMessages/gameOverMessage.h>

/** @brief Constructor for a LabyrinthMessage.
  * @param [in] x The x position of the player sending the message.
  * @param [in] y The y position of the player sending the message.
  * @param [in] isCollision Indicates that a collision with the opponent was detected.
  */
gameOverMessage::gameOverMessage(uint32_t initialGameOver) : Message{MessageType::GameOver} {
    data.gameOver = initialGameOver;
}

gameOverMessage::gameOverMessage() : Message{MessageType::GameOver} {
	data.gameOver = 0;  // Initialise à 0 par défaut
}


/** @brief Sets the type of the message.
  * @param [in] newType the type of the message.
  */
void gameOverMessage::setType(MessageType newType){
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
// void handShakeMessage::setData(uint8_t *newData, size_t length) {
//	if(length != sizeof(handShakeMessage)){
//		return;
//	}
//	seed = ((uint32_t*)newData);
// }

void gameOverMessage::setData(uint8_t *newData, size_t length) {
    if(length != sizeof(gameOverMessageData)) {
        return;
    }
    data.gameOver = ((gameOverMessageData*)newData)->gameOver;  // Dereference le pointeur pour obtenir la valeur
}

/** @brief Sets the x coordinate of the player.
  * @param [in] newX The x coordinate of the player.
  */
void gameOverMessage::setGameOver(uint32_t newGameOver){
	data.gameOver = newGameOver;
}


// Getters

/** @brief Returns the type of the message.
  * @retval The type of the message as a MessageType enum.
  */
MessageType gameOverMessage::getType() const{
	return type;
}

/** @brief Returns the size/length of the message.
  * @retval The number of bytes constituting the binary representation of the message.
  */
size_t gameOverMessage::getSize() const {
	return sizeof(uint32_t);  // On retourne la taille du seed uniquement
}

/** @brief Returns a pointer to an array containing the binary representation of the 
  * the message's content.
  * @retval An array containing the binary representation of the message's content.
  */
const uint8_t *gameOverMessage::getData() const {
	return (uint8_t *)&data.gameOver;
}


/** @brief Returns the x coordinate of the sending player in the labyrinth.
  * @retval The players x coordinate.
  */
