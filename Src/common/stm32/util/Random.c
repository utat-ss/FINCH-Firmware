/*
 * Random.c
 *
 *  Created on: Aug. 10, 2021
 *      Author: bruno
 *
 * Random number generation library
 *
 * When random is initialized, use the RNG peripheral inside the MCU to generate
 * a true random number. Use that number as the seed for the C random number
 * generator, then use the C random number generator to generate all numbers
 * going forward.
 *
 * The reason for only using the RNG peripheral once to generate the seed is so
 * that we can reproduce the same sequence of random numbers again (for testing
 * purposes) by configuring the C random number library with the same seed. If
 * we generated all the random numbers with the RNG peripheral, they would all
 * be truly random and it would be impossible to reproduce that sequence of
 * numbers deterministically.
 *
 * NOTE: This library is only intended to be used for randomized testing
 * programs, not for anything cryptographically secure.
 */

#include <common/stm32/mcu/Errors.h>
#include <common/stm32/util/Random.h>
#include <common/stm32/util/Util.h>
#include <stdlib.h>


// Global pointer to "default" Random struct - for use in IRQ handler
Random* g_random_def = NULL;


void random_init(Random* random, UART* uart) {
    // Initialize the log specifically for random
    log_init(&random->log, uart);

    // Initialize the MSP before calling HAL_RNG_Init() to initialize the handle

    /** Initializes the peripherals clock
     */
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
    PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /* RNG clock enable */
    __HAL_RCC_RNG_CLK_ENABLE();

    /* RNG interrupt Init */
    // Use preemption priority of 15 for lowest priority
#if defined(STM32G4)
    const IRQn_Type irq = RNG_IRQn;
#elif defined(STM32H7)
    const IRQn_Type irq = HASH_RNG_IRQn;
#endif
    HAL_NVIC_SetPriority(irq, 15, 0);
    HAL_NVIC_EnableIRQ(irq);

    // Initialize handle
    random->handle.Instance = RNG;
    random->handle.Init.ClockErrorDetection = RNG_CED_ENABLE;
    if (HAL_RNG_Init(&random->handle) != HAL_OK) {
        Error_Handler();
    }

    // Generate a random number from the RNG peripheral and use it to set the
    // seed of the C random library
    random->seed = 0;
    HAL_RNG_GenerateRandomNumber(&random->handle, &random->seed);
    srand(random->seed);

    // Save pointer to default Random
    if (g_random_def == NULL) {
        g_random_def = random;
    }

    // Log the random seed and instructions for reproducing the random sequence
    info(&random->log, "Initialized random with random seed %lu", random->seed);
    info(&random->log, "Random numbers can be reproduced using "
            "random_set_seed(..., %lu)", random->seed);
}

/*
 * Manually sets the seed value, overriding the seed set in random_init() from
 * the RNG peripheral.
 */
void random_set_seed(Random* random, uint32_t seed) {
    random->seed = seed;
    srand(random->seed);
    info(&random->log, "Manually set random seed to %lu", random->seed);
}

/*
 * Gets a raw value from rand() between 0 and INT32_MAX.
 */
uint32_t random_get_raw(Random* random) {
    // rand() generates a number between 0 and RAND_MAX (equal to INT32_MAX)
    uint32_t number = rand();
    verbose(&random->log, "Raw value from rand(): %lu", number);
    return number;
}

/*
 * Gets a random unsigned integer value in the range [low, high].
 * low and high are both INCLUSIVE
 * high can be up to 2^31 - 1
 */
uint32_t random_get_uint(Random* random, uint32_t low, uint32_t high) {
    uint32_t number = (random_get_raw(random) % (high - low + 1)) + low;
    debug(&random->log, "Random uint: %lu", number);
    return number;
}

/*
 * Gets a random unsigned integer value in the range [0, 2^32 - 1].
 *
 * Need this function because random_get_uint() can only produce up to 2^31 - 1.
 */
uint32_t random_get_uint32(Random* random) {
    uint32_t raw1 = random_get_raw(random);
    uint32_t raw2 = random_get_raw(random);
    // raw1 only has 31 random bits (bits 30:0), so take bit 0 of raw2 and make
    // that bit 31 in the result
    uint32_t value = raw1 | ((raw2 & bit(0)) << 31);
    debug(&random->log, "Random uint32: 0x%lX", value);
    return value;
}

/*
 * Gets a random double value in the range [low, high].
 * low and high are both INCLUSIVE
 */
double random_get_double(Random* random, double low, double high) {
    // Translate to a double value between 0.0 and 1.0
    double zero_to_one = (double) random_get_raw(random) / (double) RAND_MAX;
    // Translate to a double value in the desired range
    double number = (zero_to_one * (high - low)) + low;
    debug(&random->log, "Random double: %lf", number);
    return number;
}




// -----------------------------------------------------------------------------
// Interrupts

/**
 * @brief This function handles RNG global interrupts.
 *
 * The generated code from CubeMX calls this HASH_RNG_IRQHandler(), but that is
 * just an alias for RNG_IRQHandler(), which is the name used in the
 * startup_(...).s files.
 */
void RNG_IRQHandler(void) {
    // Call the IRQ handler function in the HAL
    if (g_random_def != NULL) {
        HAL_RNG_IRQHandler(&g_random_def->handle);
    }
}

/**
 * @brief  RNG error callbacks.
 * @param  hrng pointer to a RNG_HandleTypeDef structure that contains
 *                the configuration information for RNG.
 * @retval None
 *
 * This overrides the weak function of the same name in the HAL.
 * This function is called by HAL_RNG_IRQHandler() in the HAL.
 */
void HAL_RNG_ErrorCallback(RNG_HandleTypeDef* hrng) {
    Error_Handler();
}
