/*
 * tim.c
 *
 *  Created on: Jul 22, 2020
 *      Author: Cameron Rodriguez
 *
 * Overview: Wrapper code for the HAL code for timer functions on the STM32G4 MCU
 *
 * When certain inputs are permitted for a parameter, the potential inputs are shown as follows: (USE: NAME_OF_INPUT[_SWAP/_THESE])
 * --> This can receive either NAME_OF_INPUT_SWAP or NAME_OF_INPUT_THESE
 */

#include "../STM32_tim/tim.h"

/* Config functions
 * - These function generate basic TimFunc struct for each timer. Further changes can be made by manually adjusting values within the struct, or
 * using macros to enable interrupt flags and build other configuration initialization definitions (tim_config_xxx();).
 * - Functions using DMA will automatically be generated
 * - The prescaler equation is final_frequency = clock_frequency / (prescaler + 1) / (period + 1). These values can be calculated with the
 * macros tim_calc_prescaler() and tim_calc_period().
 * - A config function for the Encoder is currently not included, but can still be manually created, and uses the function constant TIM_Encoder in the struct.
 */

/* Populate the handle for a timer
 * @param TIM_HandleTypeDef *handle - the handle to be populated
 * @param TIM_TypeDef *instance - the register where the specific timer is location
 * @param uint32_t prescaler - used in the prescaler calculation to divide the clock
 * @param uint32_t time_counter_mode - counting mode (USE: TIM_COUNTERMODE[_UP/_DOWN/_CENTERALIGNED1/_CENTERALIGNED2/_CENTERALIGNED3])
 * @param uint32_t period - value to reset to or at; part of the prescaler calculation
 * @param uint32_t clock_divison - value to divide clock by (USE: TIM_CLOCKDIVISION[_DIV1/_DIV2/_DIV4])
 * @param uint32_t repitition_counter - (period - 1) until an update event occurs; requires the timer update flag
 * @param uint32_t autoreload_preload - disable/enable auto-reloading of timer at min/max (USE: TIM_AUTORELOAD_PRELOAD[_DISABLE/_ENABLE])
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


/* Initializes a TimFunc timer struct with zeroed values.
 * @return TimFunc* - address of an initialized TimFunc struct
 */
TimFunc *tim_init_struct(){
	TimFunc timer = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, NULL};
	TimFunc* timer_addr = &timer;
	return timer_addr;
}

TIM_HandleTypeDef *tim_init_handle(TIM_TypeDef *instance, uint32_t prescaler, uint32_t period){
	TIM_HandleTypeDef handle = {.Instance = instance};
	handle.Init.Prescaler = prescaler;
	handle.Init.Period = period;
	handle.Init.RepetitionCounter = 0;
	TIM_HandleTypeDef *handle_addr = &handle;
	return handle_addr;
}

/* Configure a basic struct for a timer with Base feature.
 * @param TIM_TypeDef *instance - the register where the specific timer is location (ex. TIM2)
 * @param TimMode *mode - the mode the timer runs in (normal, interrupt, or DMA) and associated values
 * @param uint32_t prescaler - used in the prescaler calculation to divide the clock
 * @param uint32_t period - value to reset to or at; part of the prescaler calculation
 * @return TimFunc* - the address of the TimFunc timer struct
 */
TimFunc *tim_config_base(TIM_TypeDef *instance, TimMode *mode, uint32_t prescaler, uint32_t period){
	TimFunc *base_timer = tim_init_struct();
	TIM_HandleTypeDef *handle = tim_init_handle(instance, prescaler, period);

	base_timer->function = TIM_Base;
	base_timer->mode = mode;
	base_timer->handle = handle;

	return base_timer;
}

/* Configure a basic struct for a timer with the Input Capture feature.
 * @param TIM_TypeDef *instance - the register where the specific timer is location (ex. TIM2)
 * @param TimMode *mode - the mode the timer runs in (normal, interrupt, or DMA) and associated values
 * @param uint32_t prescaler - used in the prescaler calculation to divide the clock
 * @param uint32_t period - value to reset to or at; part of the prescaler calculation
 * @param uint32_t ic_polarity - specify the input's polarity (USE: TIM_INPUTCHANNELPOLARITY[_RISING/_FALLING/_BOTHEDGE])
 * @param uint32_t ic_selection - specify how IC registers are connected to timer input channels (USE: TIM_IC_SELECTION[_DIRECTTI/_INDIRECTTI/_TRC)
 * --> Direct (the default) uses TIM Input 1 (TI1) to IC1, TI2->IC2, TI3->IC3, TI4->IC4; Indirect uses TI2->IC1, TI1->IC2, TI4->IC3, TI3->IC4; TRC connects to timer interconnect
 * @param uint32_t ic_prescaler - capture every X events (USE: TIM_ICPSC[_DIV1/_DIV2/_DIV4/_DIV8])
 * @param uint32_t ic_filter - specify the IC filter, between 0x0 and 0xF
 * @return TimFunc* - the address of the TimFunc timer struct
 */
TimFunc *tim_config_ic(TIM_TypeDef *instance, TimMode *mode, uint32_t prescaler, uint32_t period, uint32_t ic_polarity,
		uint32_t ic_selection, uint32_t ic_prescaler, uint32_t ic_filter){
	TimFunc *ic_timer = tim_init_struct();
	TIM_HandleTypeDef *handle = tim_init_handle(instance, prescaler, period);
	TIM_IC_InitTypeDef ic_config = {.ICPolarity = ic_polarity, .ICSelection = ic_selection, .ICPrescaler = ic_prescaler, .ICFilter = ic_filter};


	ic_timer->function = TIM_IC;
	ic_timer->mode = mode;
	ic_timer->handle = handle;
	ic_timer->ic_cfg = &ic_config;

	return ic_timer;
}

/* Configure a basic struct for a timer with the Output Compare feature.
 * @param TIM_TypeDef *instance - the register where the specific timer is location (ex. TIM2)
 * @param TimMode *mode - the mode the timer runs in (normal, interrupt, or DMA) and associated values
 * @param uint32_t prescaler - used in the prescaler calculation to divide the clock
 * @param uint32_t period - value to reset to or at; part of the prescaler calculation
 * @param uint32_t oc_mode - specify the timer mode, some options shown (USE: TIM_OCMODE[_TIMING/_ACTIVE/_INACTIVE/_TOGGLE/_FORCED_ACTIVE/_FORCED_INACTIVE])
 * --> Timing - generates a timing base, no external effect ; Active/Inactive - set channel on match; Toggle - toggles on match, Forced Active/Inactive - remain active/inactive
 * @param uint32_t oc_pulse - value loaded into the Capture Compare Register using the OC Pulse equation (max. 0xFFFF, 0xFFEF with dithering)
 * @param uint32_t oc_polarity - specify the output's polarity (USE: TIM_OCPOLARITY[_HIGH/_LOW])
 * @return TimFunc* - the address of the TimFunc timer struct
 */
TimFunc *tim_config_oc(TIM_TypeDef *instance, TimMode *mode, uint32_t prescaler, uint32_t period, uint32_t oc_mode, uint32_t oc_pulse,
		uint32_t oc_polarity){
	TimFunc *oc_timer = tim_init_struct();
	TIM_HandleTypeDef *handle = tim_init_handle(instance, prescaler, period);
	TIM_OC_InitTypeDef oc_config = {.OCMode = oc_mode, .Pulse = oc_pulse, .OCPolarity = oc_polarity};


	oc_timer->function = TIM_OC;
	oc_timer->mode = mode;
	oc_timer->handle = handle;
	oc_timer->oc_cfg = &oc_config;

	return oc_timer;
}

/* Configure a basic struct for a timer with the PWM feature.
 * @param TIM_TypeDef *instance - the register where the specific timer is location (ex. TIM2)
 * @param TimMode *mode - the mode the timer runs in (normal, interrupt, or DMA) and associated values
 * @param uint32_t prescaler - used in the prescaler calculation to divide the clock
 * @param uint32_t period - value to reset to or at; part of the prescaler calculation
 * @param uint32_t pwm_mode - specify the timer mode (USE: TIM_OCMODE[_PWM1/_PWM2/_ASSYMETRIC_PWM1/_ASSYMETRIC_PWM1/_COMBINED_PWM1/_COMBINED_PWM2])
 * --> PWM1 is active before hitting CCR value, PWM2 is active after hitting CCR value; Assymetric allows for a phase shift; Combined lets PWMs combine delays and phase shifts
 * @param uint32_t pwm_pulse - value loaded into the Capture Compare Register using the OC Pulse equation (max. 0xFFFF, 0xFFEF with dithering)
 * @param uint32_t pwm_polarity - specify the output's polarity (USE: TIM_OCPOLARITY[_HIGH/_LOW])
 * @return TimFunc* - the address of the TimFunc timer struct
 */
TimFunc *tim_config_pwm(TIM_TypeDef *instance, TimMode *mode, uint32_t prescaler, uint32_t period, uint32_t pwm_mode, uint32_t pwm_pulse,
		uint32_t pwm_polarity){
	TimFunc *pwm_timer = tim_init_struct();
	TIM_HandleTypeDef *handle = tim_init_handle(instance, prescaler, period);
	TIM_OC_InitTypeDef pwm_config = {.OCMode = pwm_mode, .Pulse = pwm_pulse, .OCPolarity = pwm_polarity};


	pwm_timer->function = TIM_PWM;
	pwm_timer->mode = mode;
	pwm_timer->handle = handle;
	pwm_timer->oc_cfg = &pwm_config;

	return pwm_timer;
}

/* Configure a basic struct for a timer with the OnePulse feature.
 * @param TIM_TypeDef *instance - the register where the specific timer is location (ex. TIM2)
 * @param TimMode *mode - the mode the timer runs in (normal, interrupt, or DMA) and associated values
 * @param uint32_t prescaler - used in the prescaler calculation to divide the clock
 * @param uint32_t period - value to reset to or at; part of the prescaler calculation
 * @param uint32_t oc_mode - specify the timer mode, some options shown (USE: TIM_OCMODE[_TIMING/_ACTIVE/_INACTIVE/_TOGGLE/_FORCED_ACTIVE/_FORCED_INACTIVE])
 * --> Timing - generates a timing base, no external effect ; Active/Inactive - set channel on match; Toggle - toggles on match, Forced Active/Inactive - remain active/inactive
 * @param uint32_t oc_pulse - value loaded into the Capture Compare Register using the OC Pulse equation (max. 0xFFFF, 0xFFEF with dithering)
 * @param uint32_t oc_polarity - specify the output's polarity (USE: TIM_OCPOLARITY[_HIGH/_LOW])
 * @param uint32_t ic_polarity - specify the input's polarity (USE: TIM_INPUTCHANNELPOLARITY[_RISING/_FALLING/_BOTHEDGE])
 * @param uint32_t ic_selection - specify how IC registers are connected to timer input channels (USE: TIM_IC_SELECTION[_DIRECTTI/_INDIRECTTI/_TRC)
 * --> Direct (the default) uses TIM Input 1 (TI1) to IC1, TI2->IC2, TI3->IC3, TI4->IC4; Indirect uses TI2->IC1, TI1->IC2, TI4->IC3, TI3->IC4; TRC connects to timer interconnect
 * @param uint32_t ic_filter - specify the IC filter, between 0x0 and 0xF
 * @return TimFunc* - the address of the TimFunc timer struct
 */
TimFunc *tim_config_onepulse(TIM_TypeDef *instance, TimMode *mode, uint32_t prescaler, uint32_t period, uint32_t oc_mode, uint32_t oc_pulse,
		uint32_t oc_polarity, uint32_t ic_polarity, uint32_t ic_selection, uint32_t ic_filter){
	TimFunc *op_timer = tim_init_struct();
	TIM_HandleTypeDef *handle = tim_init_handle(instance, prescaler, period);
	TIM_OnePulse_InitTypeDef op_config = {.OCMode = oc_mode, .Pulse = oc_pulse, .OCPolarity = oc_polarity, .ICPolarity = ic_polarity,
		.ICSelection = ic_selection, .ICFilter = ic_filter};


	op_timer->function = TIM_OnePulse;
	op_timer->mode = mode;
	op_timer->handle = handle;
	op_timer->onepulse_cfg = &op_config;

	return op_timer;
}

/* Configure the timer to detect update events
 *
 * NOTE: If this option is selected, the timer MUST be run in interrupt mode
 * @param TimFunc *timer - the timer struct
 * @param uint16_t period_repetitions - number of periods to count before an update event is triggered (uses flag TIM_FLAG_UPDATE)
 * @return uint8_t - whether the repetitions were successfully implemented
 */
void tim_config_repetition(TimFunc *timer, uint16_t period_repetitions){
	// Configure struct
	timer->mode->mode = TIM_Mode_IT;
	timer->handle->Init.RepetitionCounter = period_repetitions - 1;
	timer->handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	// Enable flag
	tim_enable_IT_flag(timer->handle, TIM_FLAG_UPDATE);
}

// Timer start and stop functions

/* Start the timer with the given mode and options
 *
 * @param TimFunc *timer - the timer struct
 * @return uint8_t - confirmation of successful timer start
 */
uint8_t tim_start(TimFunc *timer){
	uint8_t successful = 1;

	// Split by function
	if(timer->function==TIM_Base) {
		HAL_TIM_Base_MspInit(timer->handle);
		tim_init_clock(timer);
		HAL_TIM_Base_Init(timer->handle);

		if(timer->mode->mode==TIM_Mode_None){
			HAL_TIM_Base_Start(timer->handle);
		} else if (timer->mode->mode==TIM_Mode_IT){
			HAL_TIM_Base_Start_IT(timer->handle);
		} else if (timer->mode->mode==TIM_Mode_DMA){
			HAL_TIM_Base_Start_DMA(timer->handle, timer->mode->src_addr, timer->mode->length);
		} else {
			successful = 0;
		}
	}else if(timer->function==TIM_IC) {
		HAL_TIM_IC_MspInit(timer->handle);
		tim_init_clock(timer);
		HAL_TIM_IC_ConfigChannel(timer->handle, timer->ic_cfg, timer->mode->channel);
		HAL_TIM_IC_Init(timer->handle);

		if(timer->mode->mode==TIM_Mode_None){
			HAL_TIM_IC_Start(timer->handle, timer->mode->channel);
		} else if (timer->mode->mode==TIM_Mode_IT){
			HAL_TIM_IC_Start_IT(timer->handle, timer->mode->channel);
		} else if (timer->mode->mode==TIM_Mode_DMA){
			HAL_TIM_IC_Start_DMA(timer->handle, timer->mode->channel, timer->mode->src_addr, timer->mode->length);
		} else {
			successful = 0;
		}
	}else if(timer->function==TIM_OC) {
		HAL_TIM_OC_MspInit(timer->handle);
		tim_init_clock(timer);
		HAL_TIM_OC_ConfigChannel(timer->handle, timer->oc_cfg, timer->mode->channel);
		HAL_TIM_OC_Init(timer->handle);

		if(timer->mode->mode==TIM_Mode_None){
			HAL_TIM_OC_Start(timer->handle, timer->mode->channel);
		} else if (timer->mode->mode==TIM_Mode_IT){
			HAL_TIM_OC_Start_IT(timer->handle, timer->mode->channel);
		} else if (timer->mode->mode==TIM_Mode_DMA){
			HAL_TIM_OC_Start_DMA(timer->handle, timer->mode->channel, timer->mode->src_addr, timer->mode->length);
		} else {
			successful = 0;
		}
	}else if(timer->function==TIM_PWM) {
		HAL_TIM_PWM_MspInit(timer->handle);
		tim_init_clock(timer);
		HAL_TIM_PWM_ConfigChannel(timer->handle, timer->oc_cfg, timer->mode->channel);
		HAL_TIM_PWM_Init(timer->handle);

		if(timer->mode->mode==TIM_Mode_None){
			HAL_TIM_PWM_Start(timer->handle, timer->mode->channel);
		} else if (timer->mode->mode==TIM_Mode_IT){
			HAL_TIM_PWM_Start_IT(timer->handle, timer->mode->channel);
		} else if (timer->mode->mode==TIM_Mode_DMA){
			HAL_TIM_PWM_Start_DMA(timer->handle, timer->mode->channel, timer->mode->src_addr, timer->mode->length);
		} else {
			successful = 0;
		}
	}else if(timer->function==TIM_OnePulse) {
		HAL_TIM_OnePulse_MspInit(timer->handle);
		tim_init_clock(timer);
		HAL_TIM_OnePulse_ConfigChannel(timer->handle, timer->onepulse_cfg, timer->mode->output_channel, timer->mode->input_channel);
		HAL_TIM_OnePulse_Init(timer->handle, timer->mode->onepulse_mode);

		if(timer->mode->mode==TIM_Mode_None){
			HAL_TIM_OnePulse_Start(timer->handle, timer->mode->channel);
		} else if (timer->mode->mode==TIM_Mode_IT){
			HAL_TIM_OnePulse_Start_IT(timer->handle, timer->mode->channel);
		} else {
			successful = 0;
		}
	}else if(timer->function==TIM_Encoder) {
		HAL_TIM_Encoder_MspInit(timer->handle);
		tim_init_clock(timer);
		HAL_TIM_Encoder_Init(timer->handle, timer->encoder_cfg);

		if(timer->mode->mode==TIM_Mode_None){
			HAL_TIM_Encoder_Start(timer->handle, timer->mode->channel);
		} else if (timer->mode->mode==TIM_Mode_IT){
			HAL_TIM_Encoder_Start_IT(timer->handle, timer->mode->channel);
		} else if (timer->mode->mode==TIM_Mode_DMA){
			HAL_TIM_Encoder_Start_DMA(timer->handle, timer->mode->channel, timer->mode->src_addr, timer->mode->src_addr2, timer->mode->length);
		} else {
			successful = 0;
		}
	}else{
		successful = 0;
	}

	return successful;
}

/* Starts the appropriate timer clock, and runs clock config if present.
 * NOTE: the timers listed are exist only on STM32G474RE series, and does not implement low-power/high-res timers. Other chips will require edits to this list.
 *
 * @param TimFunc *timer - the timer struct
 */
void tim_init_clock(TimFunc *timer){
	if(timer->handle->Instance==TIM1){
		 __HAL_RCC_TIM1_CLK_ENABLE();
	}else if(timer->handle->Instance==TIM2){
		 __HAL_RCC_TIM2_CLK_ENABLE();
	}else if(timer->handle->Instance==TIM3){
		 __HAL_RCC_TIM3_CLK_ENABLE();
	}else if(timer->handle->Instance==TIM4){
		 __HAL_RCC_TIM4_CLK_ENABLE();
	}else if(timer->handle->Instance==TIM5){
		 __HAL_RCC_TIM5_CLK_ENABLE();
	}else if(timer->handle->Instance==TIM6){
		 __HAL_RCC_TIM6_CLK_ENABLE();
	}else if(timer->handle->Instance==TIM7){
		 __HAL_RCC_TIM7_CLK_ENABLE();
	}else if(timer->handle->Instance==TIM15){
		 __HAL_RCC_TIM15_CLK_ENABLE();
	}else if(timer->handle->Instance==TIM16){
		 __HAL_RCC_TIM16_CLK_ENABLE();
	}else if(timer->handle->Instance==TIM17){
		 __HAL_RCC_TIM17_CLK_ENABLE();
	}else if(timer->handle->Instance==TIM20){
		 __HAL_RCC_TIM20_CLK_ENABLE();
	}

	if(timer->clk_cfg != NULL){
		tim_config_clock(timer->handle, timer->clk_cfg);
	}
}

/* Stops a given timer
 *
 * @param TimFunc *timer - the timer struct
 * @return uint8_t - if the operation inputs were valid
 */
uint8_t tim_stop(TimFunc *timer){
	uint8_t successful = 1;

	if(timer->function==TIM_Base){
		if(timer->mode->mode==TIM_Mode_None){
			HAL_TIM_Base_Stop(timer->handle);
		}else if(timer->mode->mode==TIM_Mode_IT){
			HAL_TIM_Base_Stop_IT(timer->handle);
		}else if(timer->mode->mode==TIM_Mode_DMA){
			HAL_TIM_Base_Stop_DMA(timer->handle);
		}else{
			successful = 0;
		}
	}else if(timer->function==TIM_IC){
		if(timer->mode->mode==TIM_Mode_None){
			HAL_TIM_IC_Stop(timer->handle, timer->mode->channel);
		}else if(timer->mode->mode==TIM_Mode_IT){
			HAL_TIM_IC_Stop_IT(timer->handle, timer->mode->channel);
		}else if(timer->mode->mode==TIM_Mode_DMA){
			HAL_TIM_IC_Stop_DMA(timer->handle, timer->mode->channel);
		}else{
			successful = 0;
		}
	}else if(timer->function==TIM_OC){
		if(timer->mode->mode==TIM_Mode_None){
			HAL_TIM_OC_Stop(timer->handle, timer->mode->channel);
		}else if(timer->mode->mode==TIM_Mode_IT){
			HAL_TIM_OC_Stop_IT(timer->handle, timer->mode->channel);
		}else if(timer->mode->mode==TIM_Mode_DMA){
			HAL_TIM_OC_Stop_DMA(timer->handle, timer->mode->channel);
		}else{
			successful = 0;
		}
	}else if(timer->function==TIM_PWM){
		if(timer->mode->mode==TIM_Mode_None){
			HAL_TIM_PWM_Stop(timer->handle, timer->mode->channel);
		}else if(timer->mode->mode==TIM_Mode_IT){
			HAL_TIM_PWM_Stop_IT(timer->handle, timer->mode->channel);
		}else if(timer->mode->mode==TIM_Mode_DMA){
			HAL_TIM_PWM_Stop_DMA(timer->handle, timer->mode->channel);
		}else{
			successful = 0;
		}
	}else if(timer->function==TIM_OnePulse){
		if(timer->mode->mode==TIM_Mode_None){
			HAL_TIM_OnePulse_Stop(timer->handle, timer->mode->channel);
		}else if(timer->mode->mode==TIM_Mode_IT){
			HAL_TIM_OnePulse_Stop_IT(timer->handle, timer->mode->channel);
		}else{
			successful = 0;
		}
	}else if(timer->function==TIM_Encoder){
		if(timer->mode->mode==TIM_Mode_None){
			HAL_TIM_Encoder_Stop(timer->handle, timer->mode->channel);
		}else if(timer->mode->mode==TIM_Mode_IT){
			HAL_TIM_Encoder_Stop_IT(timer->handle, timer->mode->channel);
		}else if(timer->mode->mode==TIM_Mode_DMA){
			HAL_TIM_Encoder_Stop_DMA(timer->handle, timer->mode->channel);
		}else{
			successful = 0;
		}
	}else{
		successful = 0;
	}

	return successful;
}

/* Deinitializes a given timer
 *
 * @param TimFunc *timer - the timer struct
 * @return uint8_t - if the operation inputs were valid
 */
uint8_t tim_deinit(TimFunc *timer){
	if(timer->function==TIM_Base){
		HAL_TIM_Base_DeInit(timer->handle);
	}else if(timer->function==TIM_IC){
		HAL_TIM_IC_DeInit(timer->handle);
	}else if(timer->function==TIM_OC){
		HAL_TIM_OC_DeInit(timer->handle);
	}else if(timer->function==TIM_PWM){
		HAL_TIM_PWM_DeInit(timer->handle);
	}else if(timer->function==TIM_OnePulse){
		HAL_TIM_OnePulse_DeInit(timer->handle);
	}else if(timer->function==TIM_Encoder){
		HAL_TIM_Encoder_DeInit(timer->handle);
	}else{
		return 0;
	}

	return 1;
}
