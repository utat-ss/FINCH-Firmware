/*
 * hal.h
 *
 *  Created on: Jul. 24, 2021
 *      Author: bruno
 *
 * This header file is for convenience, providing a single header file you can
 * include in application code which will automatically determine the MCU series
 * and include the correct HAL.
 */

#ifndef COMMON_STM32_MCU_HAL_H_
#define COMMON_STM32_MCU_HAL_H_

// Check that one (and only one) MCU series is defined
// Taken from https://stackoverflow.com/questions/4301471/c-macro-to-test-if-more-than-one-defined
#if defined(STM32G4) + defined(STM32H7) != 1
#error Must define one MCU series!
#endif

// Check that one (and only one) MCU model is defined
#if defined(STM32G474xx) + defined(STM32H743xx) != 1
#error Must define one MCU model!
#endif


// Check that the MCU model is matched to the correct MCU series

#if defined(STM32G4)
// If there are multiple models for a series, should sum the defined(...)
// checks, similar to above
#if defined(STM32G474xx) != 1
#error MCU model does not match G4 MCU series!
#endif
#endif

#if defined(STM32H7)
#if defined(STM32H743xx) != 1
#error MCU model does not match H7 MCU series!
#endif
#endif


// Include the HAL for the selected MCU series
// Only need to include one header file which includes the entire HAL
#ifdef STM32G4
#include <stm32g4xx_hal.h>
#endif
#ifdef STM32H7
#include <stm32h7xx_hal.h>
#endif

#endif /* COMMON_STM32_MCU_HAL_H_ */
