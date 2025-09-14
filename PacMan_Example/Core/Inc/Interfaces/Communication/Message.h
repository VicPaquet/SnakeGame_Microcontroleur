/**
  * @file Message.h
  * @date June 2025
  * @brief This file declares the Message class that serves as an interface
  * for messages exchanged between two players of the game.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef MESSAGE_INCLUDE_GUARD
#define MESSAGE_INCLUDE_GUARD

#include <memory>
#include <cstdint>
#include <string>

namespace ELE3312 {
	/** A MessageType describes the purpose of a message.
	  */
	enum class MessageType{
		Ack,NAck, PlayerChoice, Position, PowerValue, Winner, Ready, Data, Text, Unknown
	};
	/** @brief Abstract class that defines the interface of a message.
	  */
	class Message{
		public:
			Message(MessageType type);
			// Setters
			void setType(MessageType newType);
			virtual void setData(uint8_t *newData, size_t length) = 0;
			void setValid(bool value);
			// Getters
			MessageType getType();
			virtual size_t getSize() const = 0;
			virtual const uint8_t *getData() const = 0;
			virtual std::string toString() const = 0;
			bool isValid();
		protected:
			MessageType type; //!< Stores the type of the message.
			bool valid = false;
	};

} // End of namespace ELE3312

/**
  * @}
  */ // End of documentation group ELE3312
#endif
