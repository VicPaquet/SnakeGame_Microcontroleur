/**
  * @file UART.cpp
  * @date June 2025
  * @brief This file contains the implementation of the UART class that 
  * implements the Communication interface.
  * @details The UART class implements an interface for the USART interface
  * as present on the STM32 Cortex M4 micro controller used for the STM32
  * NUCLEO 64 development board.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "NucleoImp/SerialCom/UART.h"
#include "NucleoImp/SerialCom/SerialFrame.h"


using namespace ELE3312;

/** @brief Setup routine for the UART class 
  * @details The constructor creates an object of the UART class that is
  * used to communicate via the serial interface.
  * @param [in] huart A pointer to the microcontroller periphery interface.
  * @param [in] maxBufferSize The maximum size of the send buffer.
  */
void UART::setup(UART_HandleTypeDef *huart, uint16_t maxBufferSize) {
	this->huart = huart;
	this->maxBufferSize = maxBufferSize;
}

/** @brief Constructor for the UART class 
  */
UART::UART() {
}
/** @brief Returns the length of the transmitted message.
  * @detail The returned length will be greater than the size of the sent data 
  * because of the applied COBS encoding.
  * @retval The length of the transmitted serial data frame. 
  */
size_t UART::getTransmittedLength() const{
	return messageLength;
}

/** @brief Sends a Message via UART
  * @param [in] msg A shared pointer to a Message object.
  */
void UART::send(Message  *msg) {
	SerialFrame frame{msg};
	frame.setup(tempBuffer, TEMP_BUFFER_SIZE, encodeBuffer, TEMP_BUFFER_SIZE);
	auto byteArray = frame.getByteArray();
	messageLength = std::get<1>(byteArray) ;
	for (size_t i=0; i< messageLength; ++i){
		transmitBuffer[i] = std::get<0>(byteArray)[i];
	}
	transmitBuffer[messageLength] = 0; // Add 0 to indicate end of frame
	HAL_UART_Transmit(huart, transmitBuffer, messageLength+1, 1000);
}


/**
  * @}
  */ // End of documentation group ELE3312
