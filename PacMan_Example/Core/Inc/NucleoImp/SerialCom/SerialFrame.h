/**
  * @file SerialFrame.h 
  * @date June 2025
  * @brief Declares the SerialFrame class that represents a data frame as the 
  * base structure for exchanging data via the serial interface. A serial data frame
  * consists of a header with meta data describing the content of the message and 
  * a payload that represents the actual message content.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef SERIALFRAME_INCLUDE_GUARD
#define SERIALFRAME_INCLUDE_GUARD

#include "Interfaces/Communication/Message.h"
#include "Game/ComMessages/PlayerChoiceMessage.h"
#include "Game/ComMessages/LabyrinthMessage.h"
#include "Game/ComMessages/CombatMessage.h"
#include <memory>
#include <tuple>



namespace ELE3312 {
	/** @brief SerialFrame class defining the underlying protocol for the serial communication.
	  */
	class SerialFrame{
		public:
			SerialFrame();
			SerialFrame(Message *msg);
			void setup(uint8_t *p_encodeBuffer, size_t encodeSize, uint8_t *p_tempBuffer, size_t outputSize); 
			// Getters
			std::tuple<uint8_t*, size_t> getByteArray();
			LabyrinthMessage getLabyrinthMessage();
			PlayerChoiceMessage getPlayerChoiceMessage();
			CombatMessage getCombatMessage();
			MessageType getMessageType();

			// Setters
			void setMessage(uint8_t* byteArray, size_t size);

		private:
			Message *msg = nullptr;
			size_t tempBufferSize = 0;
			size_t encodeBufferSize = 0;
			uint8_t *tempBuffer = nullptr;
			uint8_t *encodeBuffer = nullptr;
			uint8_t *msgBuffer = nullptr;
			size_t msgSize = 0;
			MessageType messageType = MessageType::Unknown;	
			
			// Helper methods
			std::tuple<uint8_t*, size_t> msgToByteArray(Message *message); 

	};

} // End of namespace ELE3312.

/**
  * @}
  */ // End of documentation group ELE3312
#endif
