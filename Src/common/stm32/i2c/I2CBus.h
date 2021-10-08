/*
 * I2CBus.h
 *
 *  Created on: Oct. 6, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_I2C_I2CBUS_H_
#define COMMON_STM32_I2C_I2CBUS_H_

#include <common/stm32/uart/Log.h>

typedef struct {
    Log log;

    GPIOAltFunc scl_gpio;
    GPIOAltFunc sda_gpio;

    I2C_HandleTypeDef handle;
} I2CBus;

#endif /* COMMON_STM32_I2C_I2CBUS_H_ */
