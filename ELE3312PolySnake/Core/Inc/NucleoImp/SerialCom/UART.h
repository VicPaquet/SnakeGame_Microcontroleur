#ifndef UART_INCLUDE_GUARD
#define UART_INCLUDE_GUARD 

#include "Interfaces/Communication/Communication.h"
#include "main.h" 
#include "NucleoImp/SerialCom/Ringbuffer.h"
#include <queue>

#define TEMP_BUFFER_SIZE 256 

	/** @brief Implements the Communication class interface for the serial (USART) interface of the 
	  * micro controller. It is used to exchange messages between two players (each one with it's own 
	  * NUCLEO development board).
	  */
	class UART : public Communication {
		public:
			UART();
			void setup(UART_HandleTypeDef *huart, uint16_t maxBufferSize);
			// Modifié : retourne un booléen de succès
			virtual bool send(Message *msg) override;
			size_t getTransmittedLength() const;
			
			// Méthode pour écrire dans le buffer depuis l'interrupt
			void handleInterruptData(uint8_t data);
			
		private:
			UART_HandleTypeDef *huart;
			uint16_t maxBufferSize; // The size of the receive message buffer

			uint8_t tempBuffer[TEMP_BUFFER_SIZE] = {0};
			uint8_t encodeBuffer[TEMP_BUFFER_SIZE] = {0};
			uint8_t transmitBuffer[TEMP_BUFFER_SIZE] = {0};

			Ringbuffer receiveBuffer;  // Buffer pour les données reçues
			size_t messageLength = 0;
	};

// End of namespace ELE3312

/**
  * @}
  */
#endif