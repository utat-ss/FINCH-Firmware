/*
 * util.h
 *
 *  Created on: Aug. 8, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_UTIL_UTIL_H_
#define COMMON_STM32_UTIL_UTIL_H_

#include <stdint.h>
#include <string.h>


/*
 * Gets a value with only a single bit at the specified index set to 1.
 * e.g. bit(0) = 0x1, bit(1) = 0x2, bit(11) = 0x800
 *
 * The inline function is supposed to go in the .h file instead of the .c file
 * so it can be seen by each translation unit during compilation and properly
 * inlined for optimization.
 *
 * Need `static` or else it produces the error "undefined reference to `bit'".
 */
static inline uint64_t bit(uint32_t index) {
    // Use "1LLU" (long long unsigned) instead of "1" to make sure the constant
    // is 64 bits instead of 32 bits
    return 1LLU << index;
}

/*
 * Gets a value with the specified number of bits (always in the least
 * significant bits) set to 1.
 * e.g. lsb_bits(0) = 0x0, lsb_bits(1) = 0x1, lsb_bits(5) = 0x1F
 */
static inline uint64_t lsb_bits(uint32_t count) {
    return bit(count) - 1;
}

/*
 * Gets a value with the specified bits (MSB to LSB, inclusive) set to 1.
 * e.g. bits(12, 12) = 0x1000, bits(15, 12) = 0xF000, bits(11, 1) = 0xFFE
 */
static inline uint64_t bits(uint32_t msb, uint32_t lsb) {
    return lsb_bits(msb - lsb + 1) << lsb;
}

/*
 * Gets the value of the bit at the specified index from the given value.
 * The return value will always be shifted all the way to the right (to the
 * least significant bit), i.e. will always return 0 or 1.
 * e.g. get_bit(0xA3, 7) = 1, get_bit(0xA3, 6) = 0, get_bit(0xA3, 5) = 1
 */
static inline uint64_t get_bit(uint64_t value, uint32_t index) {
    return (value >> index) & bit(0);
}

/*
 * Gets the value of the bits (MSB to LSB, inclusive) from the given value.
 * The return value will always be shifted all the way to the right (to the
 * least significant bits).
 * e.g. get_bits(0xA3, 7, 5) = 0b101, get_bits(0xA3, 7, 4) = 0b1010
 */
static inline uint64_t get_bits(uint64_t value, uint32_t msb, uint32_t lsb) {
    return (value >> lsb) & lsb_bits(msb - lsb + 1);
}


void serialize_be_bytes(uint64_t value, uint8_t* bytes, uint32_t count);
uint64_t deserialize_be_bytes(uint8_t* bytes, uint32_t count);
void serialize_le_bytes(uint64_t value, uint8_t* bytes, uint32_t count);
uint64_t deserialize_le_bytes(uint8_t* bytes, uint32_t count);

void util_safe_memcpy(uint8_t* destination, size_t sizeof_destination,
        uint8_t* source, size_t count);
void util_safe_strncat(char* destination, size_t sizeof_destination,
        char* source);

#endif /* COMMON_STM32_UTIL_UTIL_H_ */
