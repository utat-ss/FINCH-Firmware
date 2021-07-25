/*
 * nucleo_g474re.c
 *
 *  Created on: Jul. 25, 2021
 *      Author: bruno
 */

#include <nucleo_g474re/nucleo_g474re.h>

void g474re_init(G474RE *dev_kit) {
	mcu_init(&dev_kit->mcu, G474RE_BOARD);
	g474re_init_uart(&dev_kit->uart);
	log_init(&dev_kit->log, &dev_kit->uart);
	info(&dev_kit->log, "Initialized Nucleo G474RE dev kit");
}

/*
 * Initializes only the UART (not the entire dev kit struct).
 */
void g474re_init_uart(UART *uart) {
	// 115,200 baud should work for LPUART1 with a higher frequency clock than a
	// 32.768kHz LSE
	// If this doesn't work, try 9,600 baud
    uart_init(uart, G474RE_UART_INST, UART_BAUD_115200, G474RE_UART_AF,
    		G474RE_UART_TX_PORT, G474RE_UART_TX_PIN,
			G474RE_UART_RX_PORT, G474RE_UART_RX_PIN);
}
