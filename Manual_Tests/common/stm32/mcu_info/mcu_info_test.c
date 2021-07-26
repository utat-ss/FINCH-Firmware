#include <common/stm32/mcu/mcu.h>
#include <common/stm32/uart/log.h>
#include <nucleo_g474re/nucleo_g474re.h>
#include <nucleo_h743zi2/nucleo_h743zi2.h>

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
    if (board == MCU_BOARD_NUCLEO_G474RE) {
    	g474re_init_uart(&uart, &mcu);
    } else if (board == MCU_BOARD_NUCLEO_H743ZI2) {
    	h743zi2_init_uart(&uart, &mcu);
    }

    Log log;
    log_init(&log, &mcu, &uart);


    info(&log, "Starting MCU info test");

    info(&log, "Dev ID: 0x%lx", mcu_get_dev_id());
    info(&log, "Rev ID: 0x%lx", mcu_get_rev_id());
    info(&log, "Board: 0x%lx", mcu_get_board());
    info(&log, "Model: 0x%lx", mcu_get_model_for_board(mcu_get_board()));

	info(&log, "Done MCU info test");
	while (1) {}

    return 0;
}
