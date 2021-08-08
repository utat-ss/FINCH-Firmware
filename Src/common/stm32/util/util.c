/*
 * util.c
 *
 *  Created on: Aug. 8, 2021
 *      Author: bruno
 */

#include <common/stm32/util/util.h>

void util_safe_memcpy(uint8_t *destination, size_t sizeof_destination,
		uint8_t *source, size_t count) {
	// See https://www.cplusplus.com/reference/cstring/memcpy/

	// memcpy() is not safe if the destination and source overlap
	// Besides, there is no point in copying if the destination and source
	// are the same
	if (destination == source) {
		return;
	}

	// Make sure count doesn't overflow the destination buffer size
	if (count > sizeof_destination) {
		count = sizeof_destination;
	}

	// Copy from source to destination
	memcpy(destination, source, count);
}

/*
 * Calls strncat(), calculating the maximum number of characters to copy.
 *
 * Calling sizeof(destination) within this function would not work because this
 * function only has a pointer to destination. It cannot know the size of the
 * array, so the caller must pass in sizeof(destination) as the
 * sizeof_destination argument.
 */
void util_safe_strncat(char *destination, size_t sizeof_destination,
		char *source) {
	// See https://www.cplusplus.com/reference/cstring/strncat/

	// Note that the third argument to strncat() is NOT the total size of
	// the destination buffer, but rather it is the maximum number of
	// characters (NOT including the null character) to copy from the source
	// to the destination

	// To calculate that number, take the buffer size, subtract 1 (for the
	// new null character), then subtract the length of the string already
	// in the buffer (not including the existing null character)
	size_t count = (sizeof_destination - 1) - strlen(destination);

	// Note size_t is an unsigned integer
	// If the TX buffer is full,
	// i.e. strlen(destination) == sizeof_destination - 1,
	// count will evaluate to 0 so we shouldn't have to worry about underflow

	strncat(destination, source, count);
}
