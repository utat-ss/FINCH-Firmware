#include <common/stm32/util/random.h>

int main() {
    // Try to automatically detect board based on MCU UID
    MCUBoard board = mcu_get_board();

    MCU mcu;
    mcu_init(&mcu, board);
    UART uart;
    uart_init_for_board(&uart, &mcu);
    Log log;
    log_init(&log, &uart);

    info(&log, "Starting random test");

    Random random;
    random_init(&random, &uart);

    // See more internal logging from random library
    // log_set_level(&random.log, LOG_LEVEL_DEBUG);
    // log_set_level(&random.log, LOG_LEVEL_VERBOSE);

    // Save original seed
    uint32_t seed = random.seed;

    // Get 5 random numbers 3 times in a row
    // Start with the same seed all 3 times to show it is deterministic
    info(&log, "Reproducing values:");
    for (uint32_t count = 0; count < 3; count++) {
        for (uint32_t i = 0; i < 5; i++) {
            info(&log, "%lu", random_get_raw(&random));
        }
        // Set random back to original seed
        random_set_seed(&random, seed);
    }

    info(&log, "uint values:");
    for (uint32_t i = 0; i < 20; i++) {
        info(&log, "%lu", random_get_uint(&random, 1, 10));
    }

    info(&log, "uint32 values:");
    for (uint32_t i = 0; i < 5; i++) {
        info(&log, "0x%lX", random_get_uint32(&random));
    }

    info(&log, "double values:");
    for (uint32_t i = 0; i < 20; i++) {
        info(&log, "%lf", random_get_double(&random, -3.0, 5.5));
    }

    info(&log, "Done random test");
    while (1) {}

    return 0;
}
