/*
 * nvic.h
 *
 *  Created on: Nov 23, 2020
 *      Author: Cameron Rodriguez
 */

#ifndef DRIVERS_STM32_NVIC_NVIC_H_
#define DRIVERS_STM32_NVIC_NVIC_H_

#ifdef STM32G474xx
#include <drivers/STM32G4_clock/clock.h>
#include <stm32g4xx_hal.h>
#include <stm32g4_startup.h>
#include <stm32g4xx_hal_cortex.h>
#elif defined(STM32H743xx)
#include <drivers/STM32H7_clock/clock.h>
#include <stm32h7xx_hal.h>
#include <stm32h7_startup.h>
#include <stm32h7xx_hal_cortex.h>
#endif

#endif /* DRIVERS_STM32_NVIC_NVIC_H_ */
