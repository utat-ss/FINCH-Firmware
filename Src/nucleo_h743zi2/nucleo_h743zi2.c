/*
 * nucleo_h743zi2.c
 *
 *  Created on: Jul. 25, 2021
 *      Author: bruno
 */

#include <nucleo_h743zi2/nucleo_h743zi2.h>

void h743zi2_init(H743ZI2 *dev_kit) {
	mcu_init(&dev_kit->mcu, H743ZI2_BOARD);
	h743zi2_init_uart(&dev_kit->uart, &dev_kit->mcu);
	log_init(&dev_kit->log, &dev_kit->mcu, &dev_kit->uart);
	info(&dev_kit->log, "Initialized Nucleo H743ZI2 dev kit");
}

/*
 * Initializes only the UART (not the entire dev kit struct).
 */
void h743zi2_init_uart(UART *uart, MCU *mcu) {
    uart_init(uart, mcu, H743ZI2_UART_INST, UART_BAUD_115200, H743ZI2_UART_AF,
    		H743ZI2_UART_TX_PORT, H743ZI2_UART_TX_PIN,
			H743ZI2_UART_RX_PORT, H743ZI2_UART_RX_PIN);
}
