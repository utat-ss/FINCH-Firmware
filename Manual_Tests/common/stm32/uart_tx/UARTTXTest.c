#include <common/stm32/uart/log.h>
#include <string.h>

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


    uint8_t buf1[20] = "testing\r\n";
    // Need to cast `buf1` or else we get an error about mismatched signedness
    uart_write(&uart, buf1, strlen((const char *) buf1));

    error(&log, "error %d", 1);
    warning(&log, "warning %d", 2);
    info(&log, "info %d", 3);
    debug(&log, "debug %d", 4);
    verbose(&log, "verbose %d", 5);

    HAL_Delay(100);
    error(&log, "error %f", 5.12);

    log_set_level(&log, LOG_LEVEL_VERBOSE);
    info(&log, "info %d", 3);
    debug(&log, "debug %d", 4);
    verbose(&log, "verbose %d", 5);

    log_set_level(&log, LOG_LEVEL_INFO);

    log_set_global_level(LOG_LEVEL_VERBOSE);
    info(&log, "info %d", 3);
    debug(&log, "debug %d", 4);
    verbose(&log, "verbose %d", 5);

    log_set_global_level(LOG_LEVEL_WARNING);
    info(&log, "info %d", 3);
    debug(&log, "debug %d", 4);
    verbose(&log, "verbose %d", 5);

    uint8_t bytes[] = {0x03, 0xA4, 0xFF, 0x00, 0x26};

    error_bytes(&log, bytes, 1, "error %s", "#1");
    warning_bytes(&log, bytes, 2, "warning %s", "#2");
    info_bytes(&log, bytes, 3, "info %s", "#3");
    debug_bytes(&log, bytes, 4, "debug %s", "#4");
    verbose_bytes(&log, bytes, 5, "verbose %s", "#5");

    info_bytes(&log, bytes, 0, "info %s", "test");
    info_bytes(&log, bytes, 5, "");

    while (1) {
        info(&log, "done");
        HAL_Delay(5000);
    }

    return 0;
}
