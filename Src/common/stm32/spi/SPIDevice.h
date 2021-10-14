/*
 * SPIDevice.h
 *
 *  Created on: Oct. 13, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_SPI_SPIDEVICE_H_
#define COMMON_STM32_SPI_SPIDEVICE_H_

#include <common/stm32/spi/SPIBus.h>
#include <common/stm32/uart/Log.h>

typedef enum {
    SPI_MODE_0, // CPOL=0, CPHA=0
    SPI_MODE_1, // CPOL=0, CPHA=1
    SPI_MODE_2, // CPOL=1, CPHA=1
    SPI_MODE_3  // CPOL=1, CPHA=0
} SPIMode;

typedef struct {
    Log log;

    SPIBus* bus;
    GPIOOutput cs_gpio;
    // Could add a string here to identify this device in log messages

    // SPI mode used for this device (clock polarity and phase)
    SPIMode mode;
    // Frequency to operate this device at (in Hz)
    double freq_hz;
} SPIDevice;

#endif /* COMMON_STM32_SPI_SPIDEVICE_H_ */
