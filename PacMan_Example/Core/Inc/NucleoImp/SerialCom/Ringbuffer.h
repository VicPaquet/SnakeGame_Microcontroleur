/**
  * @file Ringbuffer.h 
  * @date June 2025
  * @brief Declares the Ringbuffer class that is used to store incoming bytes from 
  * the communication interface.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef RINGBUFFER_INCLUDE_GUARD
#define RINGBUFFER_INCLUDE_GUARD

#include <cstdint>
#include <cstdlib>

namespace ELE3312 {

/** @brief The Ringbuffer class implements a circular buffer that is used to 
  * store incoming bytes from the communication interface.
  */
class Ringbuffer {
public:
	Ringbuffer(); 
	void setup(size_t size); 
	~Ringbuffer() ;
	bool write(uint8_t *w_data, size_t w_size); 
	size_t read(uint8_t *outputBuffer, size_t bufferSize);
	size_t getCount() const;
	uint8_t *getDataBuffer();
private:
	size_t getWriteAddr() const;
	size_t getReadAddr() const;
	size_t readAddr  = 0;
	size_t writeAddr = 0;
	size_t size;  // Contains the size of the buffer (how many bytes)
	size_t count; // Contains the number of bytes in the buffer
	uint8_t *data;
};

} // End namespace ELE3312

/**
  * @}
  */ // End of documentation group ELE3312
#endif
