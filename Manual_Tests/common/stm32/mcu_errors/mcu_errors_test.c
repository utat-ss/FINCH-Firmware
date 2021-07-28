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


    info(&log, "Starting MCU errors test");

    Error_Handler();
    assert_param(1);
    assert_param(0);
    // Must cast __FILE__ or else it produces the warning "pointer targets in
    // passing argument 1 of 'assert_failed' differ in signedness [-Wpointer-sign]"
    assert_failed((uint8_t*) __FILE__, __LINE__);

	info(&log, "Done MCU errors test");
	while (1) {}

    return 0;
}
