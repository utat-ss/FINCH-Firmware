/*
 * tim.c
 *
 *  Created on: Jul 22, 2020
 *      Author: Cameron Rodriguez
 *
 * Overview: Wrapper code for the HAL code for timer functions on the STM32G4 MCU
 */

#include "../STM32_tim/tim.h"

// Helper calculator functions

/* Calculate the prescaler value required to acquire a giver counter clock frequency.
 * The equation is tim_clk = count_clk * (prescaler + 1)
 *
 * @param uint32_t tim_clk - the input clock frequency in Hz
 * @param uint16_t count_clk - the desired counter frequency in Hz
 * @return uint16_t - the prescaler value required
 */
uint16_t tim_calc_prescaler(uint32_t tim_clk, uint16_t count_clk){
	uint16_t prescaler = __HAL_TIM_CALC_PSC(tim_clk, count_clk);
	return prescaler;
}

/* Calculate the period value required for a given frequency.
 * The equation is tim_clk = (period + 1) * (prescaler + 1)
 *
 * @param uint32_t tim_clk - the input clock frequency in Hz
 * @param uint16_t prescaler - the inputted prescaler value
 * @param uint16_t frequency - the desired frequency in Hz
 * @return uint16_t - the required period value
 */
uint16_t tim_calc_period(uint32_t tim_clk, uint16_t prescaler, uint16_t frequency){
	uint16_t period = __HAL_TIM_CALC_PERIOD(tim_clk, prescaler, frequency);
	return period;
}

// User-friendly functions for macros

/* Return the current value on the given timer */
uint32_t tim_get_counter(TIM_HandleTypeDef *handle){
	uint32_t counterValue = __HAL_TIM_GET_COUNTER(handle);
	return counterValue;
}

/* Enables an interrupt flag, such as TIM_IT_UPDATE for update events.
 * @param TIM_HandleTypeDef *handle - the timer handle
 * @param uint32_t flag - the flag to enable
 */
void tim_enable_IT_flag(TIM_HandleTypeDef *handle, uint32_t flag){
	__HAL_TIM_ENABLE_IT(handle, flag);
}

/* Disables an interrupt flag.
 * @param TIM_HandleTypeDef *handle - the timer handle
 * @param uint32_t flag - the flag to disable
 */
void tim_disable_IT_flag(TIM_HandleTypeDef *handle, uint32_t flag){
	__HAL_TIM_DISABLE_IT(handle, flag);
}

/* Get the status of an interrupt flag.
 * @param TIM_HandleTypeDef *handle - the timer handle
 * @param uint32_t flag - the flag to check
 * @return uint8_t - the current status of the flag
 */
uint8_t tim_get_IT_flag(TIM_HandleTypeDef *handle, uint32_t flag){
	uint8_t status = __HAL_TIM_GET_FLAG(handle, flag);
	return status;
}

/* Clear an interrupt flag.
 * @param TIM_HandleTypeDef *handle - the timer handle
 * @param uint32_t flag - the flag to clear
 * @return uint8_t - the new status of the flag
 */
uint8_t tim_clear_IT_flag(TIM_HandleTypeDef *handle, uint32_t flag){
	uint8_t status = __HAL_TIM_CLEAR_FLAG(handle, flag);
	return status;
}

// Config functions

/* Populate the handle for a timer
 * @param TIM_HandleTypeDef *handle - the handle to be populated
 * @param TIM_TypeDef *instance - the register where the specific timer is location
 * @param uint32_t prescaler - used in the prescaler calculation to divide the clock
 * @param uint32_t time_counter_mode - counting mode (TIM_COUNTERMODE[_UP/_DOWN/_CENTERALIGNED1/_CENTERALIGNED2/_CENTERALIGNED3])
 * @param uint32_t period - value to reset to or at; part of the prescaler calculation
 * @param uint32_t clock_divison - value to divide clock by (TIM_CLOCKDIVISION[_DIV1/_DIV2/_DIV4])
 * @param uint32_t repitition_counter - (period - 1) until an update event occurs; requires the timer update flag
 * @param uint32_t autoreload_preload - disable/enable auto-reloading of timer at min/max (TIM_AUTORELOAD_PRELOAD[_DISABLE/_ENABLE])
 * @return TIM_HandleTypeDef* - the populated handle
 */
TIM_HandleTypeDef *tim_populate_handle(TIM_HandleTypeDef *handle, TIM_TypeDef *instance, uint32_t prescaler, uint32_t time_counter_mode, uint32_t period,
		uint32_t clock_division, uint32_t repitition_counter, uint32_t autoreload_preload){
	handle->Instance = instance;
	handle->Init.Prescaler = prescaler;
	handle->Init.CounterMode = time_counter_mode;
	handle->Init.Period = period;
	handle->Init.ClockDivision = clock_division;
	handle->Init.RepetitionCounter = repitition_counter;
	handle->Init.AutoReloadPreload = autoreload_preload;

	return handle;
}

/* Configures the Input Capture channels
 * @param TIM_HandleTypeDef *handle - the timer handle
 * @param TIM_IC_InitTypeDef *ic_cfg - the input capture configuration
 * @param uint32_t channel - the timer channel to use
 * @return HAL_StatusTypeDef - status of the HAL operation
 */
HAL_StatusTypeDef tim_config_ic (TIM_HandleTypeDef *handle, TIM_IC_InitTypeDef *ic_cfg, uint32_t channel){
	HAL_StatusTypeDef status = HAL_TIM_IC_ConfigChannel(handle, ic_cfg, channel);
	return status;
}

/* Configures the Output Capture channels
 * @param TIM_HandleTypeDef *handle - the timer handle
 * @param TIM_OC_InitTypeDef *oc_cfg - the output capture configuration
 * @param uint32_t channel - the timer channel to use
 * @return HAL_StatusTypeDef - status of the HAL operation
 */
HAL_StatusTypeDef tim_config_oc (TIM_HandleTypeDef *handle, TIM_OC_InitTypeDef *oc_cfg, uint32_t channel){
	HAL_StatusTypeDef status = HAL_TIM_OC_ConfigChannel(handle, oc_cfg, channel);
	return status;
}

/* Configures the PWM channels
 * @param TIM_HandleTypeDef *handle - the timer handle
 * @param TIM_OC_InitTypeDef *oc_cfg - the output capture (PWM) configuration
 * @param uint32_t channel - the timer channel to use
 * @return HAL_StatusTypeDef - status of the HAL operation
 */
HAL_StatusTypeDef tim_config_pwm (TIM_HandleTypeDef *handle, TIM_OC_InitTypeDef *oc_cfg, uint32_t channel){
	HAL_StatusTypeDef status = HAL_TIM_PWM_ConfigChannel(handle, oc_cfg, channel);
	return status;
}

/* Configures the OnePulse channels
 * @param TIM_HandleTypeDef *handle - the timer handle
 * @param TIM_OnePulse_InitTypeDef *onepulse_cfg - the OnePulse configuration
 * @param uint32_t output_channel - the timer output channel to use
 * @param uint32_t input_channel - the timer input channel to use
 * @return HAL_StatusTypeDef - status of the HAL operation
 */
HAL_StatusTypeDef tim_config_onepulse (TIM_HandleTypeDef *handle, TIM_OnePulse_InitTypeDef *onepulse_cfg, uint32_t output_channel, uint32_t input_channel){
	HAL_StatusTypeDef status = HAL_TIM_OnePulse_ConfigChannel(handle, onepulse_cfg, output_channel, input_channel);
	return status;
}

/* Configures the clock source to be used
 * @param TIM_HandleTypeDef *handle - the timer handle
 * @param TIM_OC_InitTypeDef *clk_cfg - the clock configuration
 * @return HAL_StatusTypeDef - status of the HAL operation
 */
HAL_StatusTypeDef tim_config_clock (TIM_HandleTypeDef *handle, TIM_ClockConfigTypeDef *clk_cfg){
	HAL_StatusTypeDef status = HAL_TIM_ConfigClockSource(handle, clk_cfg);
	return status;
}

/* Configures into slave mode
 * @param TIM_HandleTypeDef *handle - the timer handle
 * @param TIM_OC_InitTypeDef *slave_cfg - the slave configuration
 * @return HAL_StatusTypeDef - status of the HAL operation
 */
HAL_StatusTypeDef tim_config_slave (TIM_HandleTypeDef *handle, TIM_SlaveConfigTypeDef *slave_cfg){
	HAL_StatusTypeDef status = HAL_TIM_SlaveConfigSynchro(handle, slave_cfg);
	return status;
}

/* Configures into slave mode with interrupts
 * @param TIM_HandleTypeDef *handle - the timer handle
 * @param TIM_OC_InitTypeDef *slave_cfg - the slave configuration
 * @return HAL_StatusTypeDef - status of the HAL operation
 */
HAL_StatusTypeDef tim_config_slave_IT (TIM_HandleTypeDef *handle, TIM_SlaveConfigTypeDef *slave_cfg){
	HAL_StatusTypeDef status = HAL_TIM_SlaveConfigSynchro(handle, slave_cfg);
	return status;
}

// Initialization, start, and stop functions

/* Initialize a specific timer function
 * @param TIM_HandleTypeDef* handle - the handle for the timer
 * @param uint16_t function - the timer function to be enabled (see tim.h for potential values)
 * @param uint32_t onepulse_mode - select OnePulse mode (TIM_OPMODE[_SINGLE/_REPETITIVE])\
 * @param TIM_Encoder_InitTypeDef *encoder_cfg - Encoder interface config handle
 * @return uint8_t - successful/failed initialization
 */
uint8_t tim_init(TIM_HandleTypeDef *handle, uint8_t function, uint32_t onepulse_mode, TIM_Encoder_InitTypeDef *encoder_cfg){
	if(function==TIM_Base){
		HAL_TIM_Base_Init(handle);
	}else if(function==TIM_IC){
		HAL_TIM_IC_Init(handle);
	}else if(function==TIM_OC){
		HAL_TIM_OC_Init(handle);
	}else if(function==TIM_PWM){
		HAL_TIM_PWM_Init(handle);
	}else if(function==TIM_OnePulse){
		HAL_TIM_OnePulse_Init(handle, onepulse_mode);
	}else if(function==TIM_Encoder){
		HAL_TIM_Encoder_Init(handle, encoder_cfg);
	}else{
		return 0;
	}

	return 1;
}

/* DEinitialize a specific timer function
 * @param TIM_HandleTypeDef* handle - the handle for the timer
 * @param uint16_t function - the timer function to be disabled (see tim.h for potential values)
 * @return uint8_t - successful/failed deinitialization
 */
uint8_t tim_deinit(TIM_HandleTypeDef *handle, uint8_t function){
	if(function==TIM_Base){
			HAL_TIM_Base_DeInit(handle);
		}else if(function==TIM_IC){
			HAL_TIM_IC_DeInit(handle);
		}else if(function==TIM_OC){
			HAL_TIM_OC_DeInit(handle);
		}else if(function==TIM_PWM){
			HAL_TIM_PWM_DeInit(handle);
		}else if(function==TIM_OnePulse){
			HAL_TIM_OnePulse_DeInit(handle);
		}else if(function==TIM_Encoder){
			HAL_TIM_Encoder_DeInit(handle);
		}else{
			return 0;
		}

		return 1;
}

/* Starts a timer peripheral, without DMA or interrupts
 * @param TIM_HandleTypeDef* handle - the handle for the timer
 * @param uint16_t function - the function of the timer
 * @param channel - the timer channel to use
 * @return uint8_t - successful/failed start
 */

uint8_t tim_start(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel){
	if(function==TIM_Base){
		HAL_TIM_Base_Start(handle);
	}else if(function==TIM_IC){
		HAL_TIM_IC_Start(handle, channel);
	}else if(function==TIM_OC){
		HAL_TIM_OC_Start(handle, channel);
	}else if(function==TIM_PWM){
		HAL_TIM_PWM_Start(handle, channel);
	}else if(function==TIM_OnePulse){
		HAL_TIM_OnePulse_Start(handle, channel);
	}else if(function==TIM_Encoder){
		HAL_TIM_Encoder_Start(handle, channel);
	}else{
		return 0;
	}

	return 1;
}

/* Starts a timer peripheral with interrupts
 * @param TIM_HandleTypeDef* handle - the handle for the timer
 * @param uint16_t function - the function of the timer
 * @param channel - the timer channel to use
 * @return uint8_t - successful/failed start
 */

uint8_t tim_start_IT(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel){

	if(function==TIM_Base){
		HAL_TIM_Base_Start_IT(handle);
	}else if(function==TIM_IC){
		HAL_TIM_IC_Start_IT(handle, channel);
	}else if(function==TIM_OC){
		HAL_TIM_OC_Start_IT(handle, channel);
	}else if(function==TIM_PWM){
		HAL_TIM_PWM_Start_IT(handle, channel);
	}else if(function==TIM_OnePulse){
		HAL_TIM_OnePulse_Start_IT(handle, channel);
	}else if(function==TIM_Encoder){
		HAL_TIM_Encoder_Start_IT(handle, channel);
	}else{
		return 0;
	}

	return 1;
}

/* Starts a timer peripheral with direct memory access
 * @param TIM_HandleTypeDef* handle - the handle for the timer
 * @param uint16_t function - the function of the timer
 * @param channel - the timer channel to use
 * @param uint32_t src_addr - the source buffer address
 * @param uint32_t src_addr - second source buffer address (for Encoder)
 * @param uint16_t length - length of the data to transfer from memory to the peripheral
 * @return uint8_t - successful/failed start
 */

uint8_t tim_start_DMA(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel, uint32_t *src_addr, uint32_t *src_addr2, uint16_t length){
	if(function==TIM_Base){
		HAL_TIM_Base_Start_DMA(handle, src_addr, length);
	}else if(function==TIM_IC){
		HAL_TIM_IC_Start_DMA(handle, channel, src_addr, length);
	}else if(function==TIM_OC){
		HAL_TIM_OC_Start_DMA(handle, channel, src_addr, length);
	}else if(function==TIM_PWM){
		HAL_TIM_PWM_Start_DMA(handle, channel, src_addr, length);
	}else if(function==TIM_Encoder){
		HAL_TIM_Encoder_Start_DMA(handle, channel, src_addr, src_addr2, length);
	}else{
		return 0;
	}

	return 1;
}

/* Stops a timer peripheral, without DMA or interrupts
 * @param TIM_HandleTypeDef* handle - the handle for the timer
 * @param uint16_t function - the function of the timer
 * @param channel - the timer channel to use
 * @return uint8_t - successful/failed stop
 */

uint8_t tim_stop(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel){
	if(function==TIM_Base){
		HAL_TIM_Base_Stop(handle);
	}else if(function==TIM_IC){
		HAL_TIM_IC_Stop(handle, channel);
	}else if(function==TIM_OC){
		HAL_TIM_OC_Stop(handle, channel);
	}else if(function==TIM_PWM){
		HAL_TIM_PWM_Stop(handle, channel);
	}else if(function==TIM_OnePulse){
		HAL_TIM_OnePulse_Stop(handle, channel);
	}else if(function==TIM_Encoder){
		HAL_TIM_Encoder_Stop(handle, channel);
	}else{
		return 0;
	}

	return 1;
}

/* Stops a timer peripheral with interrupts
 * @param TIM_HandleTypeDef* handle - the handle for the timer
 * @param uint16_t function - the function of the timer
 * @param channel - the timer channel to use
 * @return uint8_t - successful/failed stop
 */

uint8_t tim_stop_IT(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel){
	if(function==TIM_Base){
		HAL_TIM_Base_Stop_IT(handle);
	}else if(function==TIM_IC){
		HAL_TIM_IC_Stop_IT(handle, channel);
	}else if(function==TIM_OC){
		HAL_TIM_OC_Stop_IT(handle, channel);
	}else if(function==TIM_PWM){
		HAL_TIM_PWM_Stop_IT(handle, channel);
	}else if(function==TIM_OnePulse){
		HAL_TIM_OnePulse_Stop_IT(handle, channel);
	}else if(function==TIM_Encoder){
		HAL_TIM_Encoder_Stop_IT(handle, channel);
	}else{
		return 0;
	}

	return 1;
}

/* Stops a timer peripheral with direct memory access
 * @param TIM_HandleTypeDef* handle - the handle for the timer
 * @param uint16_t function - the function of the timer
 * @param channel - the timer channel to use
 * @return uint8_t - successful/failed stop
 */

uint8_t tim_stop_DMA(TIM_HandleTypeDef *handle, uint8_t function, uint32_t channel){
	if(function==TIM_Base){
		HAL_TIM_Base_Stop_DMA(handle);
	}else if(function==TIM_IC){
		HAL_TIM_IC_Stop_DMA(handle, channel);
	}else if(function==TIM_OC){
		HAL_TIM_OC_Stop_DMA(handle, channel);
	}else if(function==TIM_PWM){
		HAL_TIM_PWM_Stop_DMA(handle, channel);
	}else if(function==TIM_Encoder){
		HAL_TIM_Encoder_Stop_DMA(handle, channel);
	}else{
		return 0;
	}

	return 1;
}