#include <common/stm32/uart/log.h>

int main() {
    // Try to automatically detect board based on MCU UID
	MCUBoard board = mcu_get_board();
	// If the UID is not matched, try to guess the board based on the device ID
	if (board == MCU_BOARD_NONE) {
		MCUDevID dev_id = mcu_get_dev_id();
		if (dev_id == MCU_DEV_ID_STM32G471_473_474_483_484) {
			board = MCU_BOARD_NUCLEO_G474RE;
		}
		else if (dev_id == MCU_DEV_ID_STM32H742_743_753_750) {
			board = MCU_BOARD_NUCLEO_H743ZI2;
		}
	}

	MCU mcu;
	mcu_init(&mcu, board);
	UART uart;
	uart_init_for_board(&uart, &mcu);
	Log log;
	log_init(&log, &uart);


    info(&log, "Starting UART RX test");
    info(&log, "testing dma 1");
    info(&log, "testing dma 2");
    info(&log, "testing dma 3");
    info(&log, "testing dma 4");
    info(&log, "testing dma 5");

    while (1) {
        info(&log, "Enter uint:");
        uint32_t uint = uart_read_uint(&uart);
        info(&log, "Read %lu", uint);

        info(&log, "Enter int:");
        int32_t sint = uart_read_int(&uart);
        info(&log, "Read %ld", sint);

        info(&log, "Enter char:");
        char c = uart_read_char(&uart);
        info(&log, "Read %c", c);
    }

    return 0;
}
