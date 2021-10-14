/*
 * SPIBus.h
 *
 *  Created on: Oct. 13, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_SPI_SPIBUS_H_
#define COMMON_STM32_SPI_SPIBUS_H_

#include <common/stm32/uart/Log.h>

typedef struct {
    Log log;

    GPIOAltFunc sck_gpio;
    GPIOAltFunc mosi_gpio;
    GPIOAltFunc miso_gpio;

    SPI_HandleTypeDef handle;
} SPIBus;

#endif /* COMMON_STM32_SPI_SPIBUS_H_ */
