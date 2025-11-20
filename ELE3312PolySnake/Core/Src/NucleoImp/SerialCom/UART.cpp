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
#include "Game/ComMessages/handShakeMessage.h"
#include "Game/Game.h"  // Pour accéder à Game::uartBuffer




/** @brief Setup routine for the UART class 
  * @details The constructor creates an object of the UART class that is
  * used to communicate via the serial interface.
  * @param [in] huart A pointer to the microcontroller periphery interface.
  * @param [in] maxBufferSize The maximum size of the send buffer.
  */
void UART::setup(UART_HandleTypeDef *huart, uint16_t maxBufferSize) {
    this->huart = huart;
    this->maxBufferSize = maxBufferSize;
    receiveBuffer.setup();  // Initialisation du RingBuffer
    
    // Activer l'interruption UART en mode réception
    HAL_UART_Receive_IT(huart, tempBuffer, 1);
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
bool UART::send(Message  *msg) {
    SerialFrame frame{msg};
    frame.setup(encodeBuffer, TEMP_BUFFER_SIZE, tempBuffer, TEMP_BUFFER_SIZE);
    auto byteArray = frame.getByteArray();
    messageLength = std::get<1>(byteArray);
    if (messageLength == 0 || messageLength > TEMP_BUFFER_SIZE) {
        return false;
    }
    for (size_t i=0; i< messageLength; ++i){
        transmitBuffer[i] = std::get<0>(byteArray)[i];
    }
    transmitBuffer[messageLength] = 0; // Add 0 to indicate end of frame
    //HAL_UART_Transmit(huart, transmitBuffer, messageLength+1, 1000);
    HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(huart, transmitBuffer, messageLength+1);
    return (status == HAL_OK);
}


/** @brief Receive a Message via UART
  * @return A pointer to a Message object, or nullptr if no complete message is available.
  */

void UART::handleInterruptData(uint8_t data) {
    uint8_t buffer[1] = {data};
    receiveBuffer.write(buffer, 1);
}


/**
  * @}
  */ // End of documentation group ELE3312