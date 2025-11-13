#ifndef MESSAGE_INCLUDE_GUARD
#define MESSAGE_INCLUDE_GUARD

#include <memory>
#include <cstdint>
#include <string>

enum class MessageType{
	Ack, NAck, PlayerChoice, Position, Ready, Data, Text, Unknown
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

#endif // __cplusplus

/** @} */
