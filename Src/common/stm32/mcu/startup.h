/*
 * startup.h
 */

#ifndef COMMON_STM32_MCU_STARTUP_H_
#define COMMON_STM32_MCU_STARTUP_H_

// Model-specific libraries
#ifdef STM32G4
#include "stm32g4xx_hal.h"
#endif
#ifdef STM32H7
#include "stm32h7xx_hal.h"
#endif

// General Libraries
#include <stdint.h>

void Error_Handler(void);

#endif /* COMMON_STM32_MCU_STARTUP_H_ */
