/*
Timer.h

Date created: 2021-10-19
Last modified: 2021-11-19
Author: Cameron Rodriguez

Header file for timer driver.
*/

#ifndef COMMON_STM32_TIMER_TIMER_H_
#define COMMON_STM32_TIMER_TIMER_H_

#include <common/stm32/mcu/MCU.h>

typedef struct {
    TIM_HandleTypeDef handle; // Includes TIM_Base_InitTypeDef
    TIM_ClockConfigTypeDef clock_config;
    uint8_t interrupts_enabled;
} Timer;

void timer_setup(Timer* timer, uint32_t prescaler,
    uint32_t period, uint8_t it_enabled);
void timer_customize(Timer* timer, TIM_TypeDef* timer_reg, uint8_t count_up, 
    uint8_t disable_autoreload, uint8_t repetitions);
HAL_StatusTypeDef timer_setup_callback(Timer* timer, void (*callback_func)());
HAL_StatusTypeDef timer_init(Timer* timer);
HAL_StatusTypeDef timer_deinit(Timer* timer);

HAL_StatusTypeDef timer_start(Timer* timer);
HAL_StatusTypeDef timer_stop(Timer* timer);
void timer_init_clock_irq(Timer* timer);

#endif /* COMMON_STM32_TIMER_TIMER_H_ */
