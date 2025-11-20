/**
  * @file COBS.cpp
  * @date June 2025
  * @brief This file implements the Consistent Overhead Byte Stuffing algorithm which is used
  * to encode messages exchanged over the serial interface. For more information see [COBS wikipedia]{https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing}
  * 
  * @defgroup ELE3312
  * @{
  */
#include "NucleoImp/SerialCom/COBS.h"


/** @brief Consistent Overhead Byte Stuffing (COBS) encode data to buffer
 *  @param data Pointer to input data to encode
 *  @param length Number of bytes to encode
 *  @param buffer Pointer to encoded output buffer
 *  @return Encoded buffer lenght in bytes
 *  @note Does not output delimiter byte
 */
size_t cobsEncode(const void *data, size_t length, uint8_t *buffer) {
	if (data == nullptr || buffer == nullptr) {
		return 0;
	}

	uint8_t *encode = buffer; // Encoded byte pointer
	uint8_t *codep = encode++; // Output code poiter
	uint8_t code = 1; // Code value
	for(size_t i=0; i< 20; ++i){
		*(encode+i) = 0;
	}

	for (const uint8_t *byte = (const uint8_t *)data; length--; ++byte) {
		if (*byte) { // Byte not zero, write it to output
			//*encode++ = *byte, ++code;
			*encode = *byte;
			encode += 1;
			code   += 1;
		}
		if (!*byte || code == 0xff) { // Input is zero or block completed, restart
			*codep = code;
			code = 1;
			codep = encode;
			if (!*byte || length) { // byte = zero or length != zero
				++encode;
			}
		}
	}
	*codep = code;
	// Add trailing 0 to indicate end of frame
	//++encode;
	//*encode = 0;

	return (size_t)(encode - buffer);
}


/** COBS decode data from buffer
 *  @param buffer Pointer to encoded input bytes
 *  @param length Number of bytes to decode
 *  @param data Pointer to decode output data
 *  @return Number of bytes successfully decoded
 *  @note Stops decoding if delimiter byte is found
 */
size_t cobsDecode(const uint8_t *buffer, size_t length, void *data) {
	if (buffer == nullptr || data == nullptr) {
		return 0;
	}

	const uint8_t *byte = buffer; // Encoded input byte pointer
	uint8_t *decode = (uint8_t *) data; // Decoded output byte pointer

	for (uint8_t code = 0xff, block = 0; byte < buffer + length; --block) {
		if (block) { // Decode block byte
			*decode++ = *byte++;
		} else {
			block = *byte++; // Fetch the next block length
			if (block && (code != 0xff)) { // Encoded zero, write it unless it's delimiter.
				*decode++ = 0;
			}
			code = block;
			if (!code) { // Delimiter (an unencoded 0 value) found 
				break;
			}

		}
	}

	return (size_t)(decode - (uint8_t *) data);
}

/**
  * @}
  */ // End of documentation group ELE3312
