/*
 * tim.h
 *
 *  Created on: Jul 22, 2020
 *      Author: Cameron Rodriguez
 */

#ifndef DRIVERS_STM32_TIM_TIM_H_
#define DRIVERS_STM32_TIM_TIM_H_

#include "../../Core/Inc/main.h"

// Struct for timer handle and associated values, and template struct
typedef struct {
	TIM_HandleTypeDef *handle;
	TIM_IC_InitTypeDef *ic_cfg;
	TIM_OC_InitTypeDef *oc_cfg;
	TIM_OnePulse_InitTypeDef *onepulse_cfg;
	TIM_Encoder_InitTypeDef *encoder_cfg;
	TIM_ClockConfigTypeDef *clk_cfg;
	TIM_SlaveConfigTypeDef *slave_cfg;
	uint16_t function;
	uint32_t channel;
	uint32_t input_channel;
	uint32_t onepulse_mode;
} TimFunc;

// Constants for timer functions
enum tim_func_constants {TIM_Base, TIM_IC, TIM_OC, TIM_PWM, TIM_OnePulse, TIM_Encoder};

// Helper calculator functions
uint16_t tim_calc_prescaler(uint32_t tim_clk, uint16_t count_clk);
uint16_t tim_calc_period(uint32_t tim_clk, uint16_t prescaler, uint16_t frequency);

// User-friendly functions for macros, including interrupt flags
uint32_t tim_get_counter(TIM_HandleTypeDef *handle);
void tim_enable_IT_flag(TIM_HandleTypeDef *handle, uint32_t flag);
void tim_disable_IT_flag(TIM_HandleTypeDef *handle, uint32_t flag);
uint8_t tim_get_IT_flag(TIM_HandleTypeDef *handle, uint32_t flag);
uint8_t tim_clear_IT_flag(TIM_HandleTypeDef *handle, uint32_t flag);

// Timer config functions
TIM_HandleTypeDef *tim_populate_handle(TIM_HandleTypeDef *handle, TIM_TypeDef *instance, uint32_t prescaler, uint32_t time_counter_mode, uint32_t period,
		uint32_t clock_division, uint32_t repitition_counter, uint32_t autoreload_preload);
HAL_StatusTypeDef tim_config_ic (TIM_HandleTypeDef *handle, TIM_IC_InitTypeDef *ic_cfg, uint32_t channel);
HAL_StatusTypeDef tim_config_oc (TIM_HandleTypeDef *handle, TIM_OC_InitTypeDef *oc_cfg, uint32_t channel);
HAL_StatusTypeDef tim_config_pwm (TIM_HandleTypeDef *handle, TIM_OC_InitTypeDef *oc_cfg, uint32_t channel);
HAL_StatusTypeDef tim_config_onepulse (TIM_HandleTypeDef *handle, TIM_OnePulse_InitTypeDef *onepulse_cfg, uint32_t output_channel, uint32_t input_channel);
HAL_StatusTypeDef tim_config_clock (TIM_HandleTypeDef *handle, TIM_ClockConfigTypeDef *clk_cfg);
HAL_StatusTypeDef tim_config_slave (TIM_HandleTypeDef *handle, TIM_SlaveConfigTypeDef *slave_cfg);
HAL_StatusTypeDef tim_config_slave_IT (TIM_HandleTypeDef *handle, TIM_SlaveConfigTypeDef *slave_cfg);

// Timer init/start/stop functions
uint8_t tim_init(TIM_HandleTypeDef *handle, uint8_t function, uint32_t onepulse_mode, TIM_Encoder_InitTypeDef *encoder_cfg);
uint8_t tim_deinit(TIM_HandleTypeDef *handle, uint8_t function);
uint8_t tim_start(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel);
uint8_t tim_start_IT(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel);
uint8_t tim_start_DMA(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel, uint32_t *src_addr, uint32_t *src_addr2, uint16_t length);
uint8_t tim_stop(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel);
uint8_t tim_stop_IT(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel);
uint8_t tim_stop_DMA(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel);

#endif /* DRIVERS_STM32_TIM_TIM_H_ */
