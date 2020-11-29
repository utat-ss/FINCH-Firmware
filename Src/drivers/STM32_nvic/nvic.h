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

// NVIC reset & priority functions
void nvic_system_reset();
uint32_t nvic_get_priority_grouping();
void nvic_set_priority_grouping(uint32_t grouping_bits);
uint8_t nvic_validate_priority_inputs(uint32_t group, uint32_t subpriority);
void nvic_get_priority(IRQn_Type interrupt, uint32_t *group, uint32_t *subpriority);
void nvic_set_priority(IRQn_Type interrupt, uint32_t group, uint32_t subpriority);

// Interrupt (IRQ) operation functions
uint32_t nvic_get_active_interrupt(IRQn_Type interrupt);
void nvic_enable_interupt(IRQn_Type interrupt);
void nvic_disable_interrupt(IRQn_Type interrupt);
uint32_t nvic_get_pending_interrupt(IRQn_Type interrupt);
void nvic_set_pending_interrupt(IRQn_Type interrupt);
void nvic_clear_pending_interrupt(IRQn_Type interrupt);

#endif /* DRIVERS_STM32_NVIC_NVIC_H_ */
