/*
Timer.h

Date created: 2021-10-19
Last modified: 2021-10-19
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
    uint8_t callback_enabled
    volatile void (*callback_func)();
} Timer;

Timer timer_init(MCU* mcu, uint32_t clk_frequency, uint32_t target_frequency, uint8_t it_enabled);
void timer_customize(Timer* timer,  TIM_TypeDef* register, uint8_t count_up, uint8_t autoreload,
    uint8_t repetitions);
uint8_t timer_setup_callback();
void timer_deinit(Timer timer_struct);

HAL_StatusTypeDef timer_start();
HAL_StatusTypeDef timer_stop();

#endif /* COMMON_STM32_TIMER_TIMER_H_ */