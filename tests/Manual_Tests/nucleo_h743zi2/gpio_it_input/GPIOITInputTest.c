/*
 * GPIOITInputTest.c
 *
 *  Created on: Sept. 28, 2021
 *      Author: bruno
 * 
 * Tests receiving an interrupt from a GPIO input when the blue button is
 * pressed.
 */

#include <nucleo_h743zi2/h743zi2.h>

// This must be volatile since it is modified inside the ISR callback
volatile bool received = false;

void callback(void) {
    received = true;
}

int main() {
    H743ZI2 dk;
    h743zi2_init(&dk);

    // The board init sets the blue button to a standard GPIO input, so deinit
    // that and initialize it as an interrupt input
    HAL_GPIO_DeInit(dk.blue_button.port, dk.blue_button.pin);

    GPIOITInput interrupt;
    // Use rising mode because the input signal is low by default (when the
    // button is not pressed)
    gpio_it_input_init(&interrupt, &dk.mcu, dk.blue_button.port,
            dk.blue_button.pin, GPIO_MODE_IT_RISING, GPIO_NOPULL, callback);

    while (1) {
        if (received) {
            info(&dk.log, "Received interrupt");
            received = false;
        }
    }

    return 0;
}
