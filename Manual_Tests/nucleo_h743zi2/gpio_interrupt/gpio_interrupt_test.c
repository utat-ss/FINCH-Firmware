/*
 * gpio_interrupt_test.c
 *
 *  Created on: Aug. 6, 2020
 *      Author: bruno
 */

#include <common/stm32/gpio/gpio_interrupt.h>
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

	GPIOInterrupt interrupt;
	// Use rising mode because the input signal is low by default (when the
	// button is not pressed)
	gpio_interrupt_init(&interrupt, &dk.mcu, dk.blue_button.port,
			dk.blue_button.pin, GPIO_MODE_IT_RISING, GPIO_NOPULL, callback);

	while (1) {
		if (received) {
			info(&dk.log, "Received interrupt");
			received = false;
		}
	}

	return 0;
}
