/*
 * I2CRead.h
 *
 *  Created on: Oct. 6, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_I2C_I2CREAD_H_
#define COMMON_STM32_I2C_I2CREAD_H_

#include <stdint.h>

#define I2C_READ_BUF_SIZE 50

typedef struct {
    // If using C++, should make the buffer size a template parameter
    uint8_t buf[I2C_READ_BUF_SIZE];
    // Number of bytes to transfer
    uint32_t count;
} I2CRead;

#endif /* COMMON_STM32_I2C_I2CREAD_H_ */
