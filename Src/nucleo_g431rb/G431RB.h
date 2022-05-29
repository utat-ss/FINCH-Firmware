/*
G431RB.h

Date Created: May 28, 2022
Last Modified: May 29, 2022

Author: Eric Leus
*/

#ifndef NUCLEO_G431RB_G431RB_H_
#define NUCLEO_G431RB_G431RB_H_

#include <common/stm32/uart/Log.h>

// Struct to represent the MCU on the dev kit and all peripherals/configuration
typedef struct {
    MCU mcu;
    UART uart;
    Log log;
    GPIOInput blue_button;
    GPIOOutput green_led;
} G431RB;

void g431rb_init(G431RB* dev_kit);

#endif /* NUCLEO_G431RB_G431RB_H_ */