/*
 * I2CDevice.h
 *
 *  Created on: Oct. 6, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_I2C_I2CDEVICE_H_
#define COMMON_STM32_I2C_I2CDEVICE_H_

#include <common/stm32/i2c/I2CBus.h>
#include <common/stm32/uart/Log.h>

typedef enum {
    I2C_FREQ_100_KHZ,   // Standard mode
    I2C_FREQ_400_KHZ,   // Fast mode
    I2C_FREQ_1_MHZ      // Fast mode plus
} I2CFreq;

typedef struct {
    Log log;

    I2CBus* bus;

    // Addressing mode
    uint32_t addr_mode;
    // I2C address of this device on the bus in 7-BIT MODE (NOT 8-bit mode)
    // This is a uint16_t instead of a uint8_t to support 10-bit addressing if
    // it is used
    uint16_t addr;
    // Could add a string here to identify this device in log messages

    // Frequency to operate this device at
    // TODO - library needs to automatically determine the value of the I2C_TIMINGR register based on this value
    I2CFreq freq;
} I2CDevice;

#endif /* COMMON_STM32_I2C_I2CDEVICE_H_ */
