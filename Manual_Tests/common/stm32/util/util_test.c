#include <common/stm32/uart/log.h>
#include <common/stm32/util/util.h>

int main() {
	// Try to automatically detect board based on MCU UID
	MCUBoard board = mcu_get_board();

    MCU mcu;
    mcu_init(&mcu, board);
    UART uart;
    uart_init_for_board(&uart, &mcu);
    Log log;
    log_init(&log, &uart);

    info(&log, "Starting utilities test");

    info(&log, "bit(0) = 0x%lX", bit(0));
    info(&log, "bit(1) = 0x%lX", bit(1));
    info(&log, "bit(11) = 0x%lX", bit(11));

    info(&log, "lsb_bits(0) = 0x%lX", lsb_bits(0));
    info(&log, "lsb_bits(1) = 0x%lX", lsb_bits(1));
    info(&log, "lsb_bits(5) = 0x%lX", lsb_bits(5));

    info(&log, "bits(12, 12) = 0x%lX", bits(12, 12));
    info(&log, "bits(15, 12) = 0x%lX", bits(15, 12));
    info(&log, "bits(11, 1) = 0x%lX", bits(11, 1));

	info(&log, "Done utilities test");
	while (1) {}

    return 0;
}
