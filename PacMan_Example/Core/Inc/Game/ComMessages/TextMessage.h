/**
  * @file TextMessage.h 
  * @date June 2025
  * @brief Declares the TextMessage class that represents a message
  * object that carries a text string.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef STRINGMESSAGE_INCLUDE_GUARD
#define STRINGMESSAGE_INCLUDE_GUARD

#include "Interfaces/Communication/Message.h"
#include <string>
#include <iostream>

namespace ELE3312 {
	/** @brief The TextMessage class is used to exchange text messages between two players.
	  */
	class TextMessage : public Message {
		public:
			TextMessage(MessageType type);
			TextMessage();
			// Setters
			virtual void setData(uint8_t *newData, size_t length) override;
			void setText(const std::string &text);
			// Getters
			const std::string & getText() const;
			// Inherited methods
			virtual size_t getSize() const override;
			virtual const uint8_t *getData() const override;
			
			virtual std::string toString() const override;
		private:
			std::string text;


	};


std::ostream & operator<<(std::ostream &out, const TextMessage &msg);

} // End of namespace ELE3312

/**
  * @}
  */ // End of documentation group ELE3312
#endif
