/*
 * G474RE.h
 *
 *  Created on: Jul. 25, 2021
 *      Author: bruno
 */

#ifndef NUCLEO_G474RE_G474RE_H_
#define NUCLEO_G474RE_G474RE_H_

#include <common/stm32/uart/Log.h>

// Struct to represent the MCU on the dev kit and all peripherals/configuration
typedef struct {
    MCU mcu;
    UART uart;
    Log log;
    GPIOInput blue_button;
    GPIOOutput green_led;
} G474RE;

void g474re_init(G474RE* dev_kit);

#endif /* NUCLEO_G474RE_G474RE_H_ */
