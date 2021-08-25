/*
 * h743zi2.h
 *
 *  Created on: Jul. 25, 2021
 *      Author: bruno
 */

#ifndef NUCLEO_H743ZI2_H743ZI2_H_
#define NUCLEO_H743ZI2_H743ZI2_H_

#include <common/stm32/uart/Log.h>

// Struct to represent the MCU on the dev kit and all peripherals/configuration
typedef struct {
    MCU mcu;
    UART uart;
    Log log;
    GPIOInput blue_button;
    GPIOOutput red_led;
} H743ZI2;

void h743zi2_init(H743ZI2* dev_kit);

#endif /* NUCLEO_H743ZI2_H743ZI2_H_ */
