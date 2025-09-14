/**
  * @file TextMessage.cpp
  * @date June 2025
  * @brief This file implements the TextMessage class that is used to 
  * exchange text messages via the serial interface.
  * 
  * @defgroup ELE3312
  * @{
  */
#include "Game/ComMessages/TextMessage.h"

using namespace ELE3312;

/** @brief Constructor for the TextMessage class.
  * @pram [in] type The message type.
  */
TextMessage::TextMessage(MessageType type) : Message{type}, text{} {
}

/** @brief Default constructor for the TextMessage class.
  */
TextMessage::TextMessage() : Message{MessageType::Text}, text{} {
}

// Setters
/** @brief Sets the text content of the message.
  * @param [in] text The text content of the message.
  */
void TextMessage::setText(const std::string &text){
	this->text = text;
}

/** @brief Sets the binary representation of the message.
  * @details This method can be used to initialize the message with 
  * a binary representation of the it's content so that the content 
  * can later be retrieved with it's corresponding access methods.
  * This method can thus be used to deserialize a TextMessage object.
  * @param [in] newData A pointer to an array of bytes containing the message's data.
  * @param [in] length The length of the byte array (number of bytes contained).
  */
void TextMessage::setData(uint8_t *newData, size_t length) {
	text = std::string(reinterpret_cast<char*>(newData), length);
}

// Getters

/** @brief Returns the text contained in the message.
  * @retval The text contained in the TextMessage object.
  */
const std::string & TextMessage::getText() const{
	return text;
}

// Implementation of virtual methods

/** @brief Returns the number of bytes of the binary representation of the message.
  * @retval Number of bytes of the binary representation of the message.
  */
size_t TextMessage::getSize() const {
	return text.length();
}

/** @brief Returns the binary representation of the message's content.
  * @retval Pointer to an array of bytes containing the message's content.
  */
const uint8_t *TextMessage::getData() const{
	return reinterpret_cast<const uint8_t *>(text.c_str());
}

/** @brief Stream operator to write the text content of the message to
  * an output stream.
  * @details This method is mainly intended for debug purposes.
  * @param [out] out Reference to an output stream.
  * @param [in] msg Reference to a TextMessage object.
  * @retval The output stream the message has been written to.
  */
std::ostream & operator<<(std::ostream &out, const TextMessage &msg){
	return out << msg.getText();
}

/** @brief Returns a textual representation of the message.
  * @retval A textual representation of the content of the message.
  */
std::string TextMessage::toString() const{
	return text;
}

/**
  * @}
  */ // End of the documentation group ELE3312
