/**
  * @file Ringbuffer.cpp
  * @date June 2025
  * @brief This file implements a circular buffer that is used to store
  * incoming bytes from the UART interface.
  * 
  * @defgroup ELE3312
  * @{
  */
#include "NucleoImp/SerialCom/Ringbuffer.h"

using namespace ELE3312;

#ifdef LOCAL_DEBUG

#include <cstdio>

/** @brief Helper function to print an array to the console.
  * @param [in] input The array to be printed.
  * @param [in] length The length of the array.
  */
void printArray2(void *input, size_t length) {
	uint8_t *buffer = (uint8_t *) input;
	for (size_t i=0; i<length-1; ++i) {
		printf("%2x ", *(buffer+i));
	}
	printf("%2x\n", *(buffer + length - 1));
}

#endif



/** @brief Default constructor for a Ringbuffer object.
  */
Ringbuffer::Ringbuffer() { 
}

/** @brief Setup routine for a Ringbuffer object.
  * @param [in] size The size of the buffer (number of bytes)
  */
void Ringbuffer::setup(size_t size) {
	readAddr = 0;
	writeAddr = 0;
	count = 0;
	this->size = size;
	
	data = new uint8_t[size];
}

/** @brief Destructor for the Ringbuffer class. 
  */
Ringbuffer::~Ringbuffer() {
	delete [] data;
}

/** @brief Writes new data in the buffer.
  * @param [in] w_data A pointer to the data to be written in the buffer.
  * @param [in] w_size The size of the provided data array (number of bytes).
  * @retval Boolean value that indicates whether the write command succeeded (true).
  * If the command was unable to write the data into the buffer it will return false. 
  */
bool Ringbuffer::write(uint8_t *w_data, size_t w_size) {
	if ( w_size + count > size) {
		// Not enough space in the buffer to store the data
		return false;
	}

	for (size_t i=0; i<w_size; ++i){
		// TODO: check for 0 bytes in order to count serial frames
		// This way the code using the buffer can only attempt to read
		// a frame when there are frames detected in the buffer. (I thought I did this already but
		// it somehow disappeared).
		*(data+writeAddr) = *(w_data + i);
		writeAddr = (writeAddr + 1) % size;
	}
	count += w_size;
	return true;
}

/** @breif Reads a data frame from the buffer
  * @param [out] outputBuffer The buffer the data is read into
  * @param [in] bufferSize The size of the output buffer
  * @retval the number of bytes in the frame
  */
size_t Ringbuffer::read(uint8_t *outputBuffer, size_t bufferSize){
	if (outputBuffer == nullptr  || count == 0) {
		return 0;
	}
	uint8_t *byte = data + readAddr; // Encoded input byte pointer to the buffer data
	uint8_t *decode =  outputBuffer; // Decoded output byte pointer to the output buffer
	size_t writeCount = 0;
	size_t readCount = 0;
	
	bool foundEOF = false;
	bool dataError = false;

    // The block variable gives the number of continuous non zero bytes in the byte-stream.
	// The code variable is used to detect the overhead byte and the end of the package.
	for (uint8_t code = 0xff, block = 0; writeCount < bufferSize && readCount < count; --block) {
		if (block) { // Decode block byte, as long as block is different from 0 the bytes are written to the output buffer (block gets decremented in the for-loop)
			if (*byte == 0) {
				dataError = true;
				break;
			}
			*decode = *byte; // Write byte to output buffer
			decode += 1; // Increment write position
			if ( byte == data + size -1) { // Check for circular buffer right border (we are traversing the ringbuffer from left to ritht)
				// Do wrap around when the byte pointer reaches the end of the buffer
				byte = data; // Set current read position to start of circular buffer
			} else {
				byte += 1; // Set read pointer to next byte in circular buffer
			}
			readCount += 1; // Increment the count of read bytes
			writeCount += 1; // Increment the count of written bytes
		} else {
			block = *byte; // Fetch the next block length
			if ( byte == data + size -1 ) { // Check for the right border of the circular buffer
				// Do wrap around when the byte pointer reaches the end of the buffer
				byte = data;
			} else {
				byte += 1;
			}
			readCount += 1; // Increment read count
			if (block && (code != 0xff)) { // Encoded zero, write it unless it's delimiter.
				*decode++ = 0;
				writeCount += 1;
			}
			code = block;
			if (!code) { // Delimiter (an unencoded 0 value) found 
				foundEOF = true;
				break;
			}

		}
	}
	if (foundEOF ) {
		readAddr = (readAddr + readCount ) % size;
		if (count >= readCount) {
		count -= readCount;
		} else {

		#ifdef LOCAL_DEBUG
			printf("ERROR: byteCount > count should never happen (count = %d, readCount =%d)!!!!\n", count , readCount);
		#endif
		}
	}else {
		writeCount = 0; // Indicating that no frame was read
	}
	
	if (dataError) {
		readAddr = (readAddr + readCount + 1 ) % size;
		if (count >= readCount) {
		count -= readCount + 1;

		#ifdef LOCAL_DEBUG
		printf("FrameError\n");
		} else {
			printf("ERROR: byteCount > count should never happen (count = %d, readCount =%d)!!!!\n", count , readCount);
		#endif
		}
		writeCount = 0; // Indicating that no frame data was read
	}
	return writeCount ;
}

/** @brief Returns the current write address.
  * @retval The current write address.
  */
size_t Ringbuffer::getWriteAddr() const{
	return writeAddr;
}

/** @brief Returns the current read address.
  * @retval The current read address.
  */
size_t Ringbuffer::getReadAddr() const{
	return readAddr;
}

/** @brief Returns the number of bytes currently stored in the buffer.
  * @retval Number of bytes stored in the buffer.
  */
size_t Ringbuffer::getCount() const{
	return count;
}

/** @brief Returns a pointer to the buffer storage.
  * @details This method is mainly intended for debug purposes. 
  * The internal data structure of the buffer should not directly manipulated.
  * @todo Better remove the access methods to the internal state of the buffer.
  * @retval Pointer to the internal data storage of the buffer.
  */
uint8_t *Ringbuffer::getDataBuffer(){
	return data;
}

/**
  * @}
  */ // End of the documentation group ELE3312

