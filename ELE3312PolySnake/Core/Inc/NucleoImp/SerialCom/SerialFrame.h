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
#include "NucleoImp/SerialCom/COBS.h"


#include "Game/ComMessages/SnakeGameMessage.h"
#include "Game/ComMessages/handShakeMessage.h"

#include <memory>
#include <tuple>




	class SerialFrame{
		public:
			SerialFrame();
			SerialFrame(Message *msg);
			void setup(uint8_t *p_encodeBuffer, size_t encodeSize, uint8_t *p_tempBuffer, size_t outputSize); 
			// Getters
			std::tuple<uint8_t*, size_t> getByteArray();
			SnakeGameMessage getSnakeGameMessage();
			handShakeMessage getHandShakeMessage();
			//PlayerChoiceMessage getPlayerChoiceMessage();
			//CombatMessage getCombatMessage();
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

#endif
