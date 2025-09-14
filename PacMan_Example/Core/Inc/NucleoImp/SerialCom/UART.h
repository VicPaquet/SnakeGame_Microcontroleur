/**
  * @file UART.h
  * @date June 2025
  * @brief Implements the UART Communication interface that is used to exchange 
  * messages via the serial interface of the micro controller.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef UART_INCLUDE_GUARD
#define UART_INCLUDE_GUARD 

#include "Interfaces/Communication/Communication.h"
#include "main.h" 
#include <queue>

#define TEMP_BUFFER_SIZE 256 


namespace ELE3312 {
	/** @brief Implements the Communication class interface for the serial (USART) interface of the 
	  * micro controller. It is used to exchange messages between two players (each one with it's own 
	  * NUCLEO development board).
	  */
	class UART : public Communication {
		public:
			UART();
			void setup(UART_HandleTypeDef *huart, uint16_t maxBufferSize);
			virtual void send(Message *msg) override;
			size_t getTransmittedLength() const;
		private:
			UART_HandleTypeDef *huart;
			uint16_t maxBufferSize; // The size of the receive message buffer

			uint8_t tempBuffer[TEMP_BUFFER_SIZE] = {0};
			uint8_t encodeBuffer[TEMP_BUFFER_SIZE] = {0};
			uint8_t transmitBuffer[TEMP_BUFFER_SIZE] = {0};


			size_t messageLength = 0;
			// Private methods

	};

} // End of namespace ELE3312

/**
  * @}
  */
#endif
