/*
 * random.h
 *
 *  Created on: Aug. 10, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_UTIL_RANDOM_H_
#define COMMON_STM32_UTIL_RANDOM_H_

#include <common/stm32/uart/log.h>

typedef struct {
    Log log;
    RNG_HandleTypeDef handle;
    uint32_t seed;
} Random;

void random_init(Random *random, UART *uart);
void random_set_seed(Random *random, uint32_t seed);

uint32_t random_get_raw(Random *random);
uint32_t random_get_uint(Random *random, uint32_t low, uint32_t high);
uint32_t random_get_uint32(Random *random);
double random_get_double(Random *random, double low, double high);

#endif /* COMMON_STM32_UTIL_RANDOM_H_ */
