#ifndef COBS_INCLUDE_GUARD
#define COBS_INCLUDE_GUARD

#include <cstdint>
#include <cstdlib>

/** @brief Consistent Overhead Byte Stuffing (COBS) encode data to buffer
 *  @param data Pointer to input data to encode
 *  @param length Number of bytes to encode
 *  @param buffer Pointer to encoded output buffer
 *  @return Encoded buffer lenght in bytes
 *  @note Does not output delimiter byte
 */
size_t cobsEncode(const void *data, size_t length, uint8_t *buffer); 


/** COBS decode data from buffer
 *  @param buffer Pointer to encoded input bytes
 *  @param length Number of bytes to decode
 *  @param data Pointer to decode output data
 *  @return Number of bytes successfully decoded
 *  @note Stops decoding if delimiter byte is found
 */
size_t cobsDecode(const uint8_t *buffer, size_t length, void *data); 
#endif
