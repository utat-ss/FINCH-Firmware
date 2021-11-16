/*
Timer.c

Date created: 2021-10-19
Last modified: 2021-10-19
Author: Cameron Rodriguez

Wrapper for the HAL timer (TIM) drivers. Configured with TIM5 peripheral enabled
by default. Currently supported are base timers with confiigurations and
interrupts, for general-purpose and low-power timers.

Not currently supported
  * output compare (OC) - modify signal when counter == catch/capture register
  * pulse width modulation (PWM)
  * OnePulse mode (variant of Oc)
  * input capture (IC) - latch counter value after a signal is recieved
  * DMA burst mode
  * HRTIM1

Still need to determine which timers will be enabled
 */

#include <common/stm32/timer/Timer.h>

// Initializes the timer struct, and optionally enables interrupts.
Timer timer_setup(MCU* mcu, uint32_t clk_frequency, uint32_t prescaler,
    uint32_t period, uint8_t it_enabled) {
    Timer timer_struct;

    // Setup timer handle, default to TIM5
    TIM_Base_InitTypeDef base_timer = {
        .Prescaler = prescaler,
        .Period = period,
        .CounterMode = TIM_COUNTERMODE_DOWN,
        .ClockDivision = TIM_CLOCKDIVISION_DIV1,
        .RepetitionCounter = 0,
        .AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE
    };
    TIM_HandleTypeDef blank_handle = {.Instance = TIM5, .Init = base_timer};
    timer_struct.handle = blank_handle;
    timer_struct.interrupts_enabled = it_enabled;
    timer_struct.callback_enabled = 0;
    
    return timer_struct;
}

/* Add additional customizations to the timer driver

TIM_TypeDef* timer_reg: specific timer register to use, use NULL to make no
                        change
uint8_t count_up: run timer in upcounter mode (default countdown)
uint8_t disable_autoreload: disable automatic reloading of timer once limit is 
                            reached
uint8_t repetitions: number of timer loops to execute before update interrupt is
                     raised (default 0)
*/
void timer_customize(Timer* timer_struct, TIM_TypeDef* timer_reg, uint8_t count_up, 
    uint8_t disable_autoreload, uint8_t repetitions) {
    
    if (timer_reg != NULL) {
        timer_struct->handle.Instance = timer_reg;
    }
    if (count_up) {
        timer_struct->handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    }
    if (disable_autoreload) {
        timer_struct->handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    }
    if (repetitions != 0) {
        timer_struct->handle.Init.RepetitionCounter = repetitions;
    }
}

// Registers a callback to trigger when the counter runs out
HAL_StatusTypeDef timer_setup_callback(Timer* timer_struct, void (*callback_func)()) {
    return HAL_TIM_RegisterCallback(&(timer_struct->handle), HAL_TIM_PERIOD_ELAPSED_CB_ID, callback_func);
}

// Initializes the timer, must be called before starting the timer
HAL_StatusTypeDef timer_init(Timer* timer_struct) {
	HAL_TIM_Base_MspInit(timer_struct->handle.Instance);
    timer_init_clock_irq(timer_struct);    
    return HAL_TIM_Base_Init(&(timer_struct->handle));
}

// Deinitializes the timer after it has been stopped
HAL_StatusTypeDef timer_deinit(Timer* timer_struct) {
    return HAL_TIM_Base_DeInit(&(timer_struct->handle));
}

// Starts the timer (in interrupt mode if specified during timer setup)
HAL_StatusTypeDef timer_start(Timer* timer_struct) {    
    if(timer_struct->interrupts_enabled) {
        return HAL_TIM_Base_Start_IT(&(timer_struct->handle));
    } else {
        return HAL_TIM_Base_Start(&(timer_struct->handle));
    }
}

// Stops the timer (in interrupt mode if specified during timer setup)
HAL_StatusTypeDef timer_stop(Timer* timer_struct) {
    if(timer_struct->interrupts_enabled) {
        return HAL_TIM_Base_Stop_IT(&(timer_struct->handle));
    } else {
        return HAL_TIM_Base_Stop(&(timer_struct->handle));
    }
}

void timer_init_clock_irq(Timer* timer_struct) {
    if(timer_struct->handle.Instance==TIM2){
		__HAL_RCC_TIM2_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}else if(timer_struct->handle.Instance==TIM3){
		__HAL_RCC_TIM3_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
	}else if(timer_struct->handle.Instance==TIM4){
		__HAL_RCC_TIM4_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM4_IRQn);
	}else if(timer_struct->handle.Instance==TIM5){
		__HAL_RCC_TIM5_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM5_IRQn);
	}else if(timer_struct->handle.Instance==TIM6){
		__HAL_RCC_TIM6_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	}

	// MCU-specific timers
#ifdef STM32G474xx
	if (timer_struct->handle.Instance==TIM1){
		__HAL_RCC_TIM1_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	}else if(timer_struct->handle.Instance==TIM7){
		__HAL_RCC_TIM7_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM7_DAC_IRQn);
	}else if(timer_struct->handle.Instance==TIM15){
		__HAL_RCC_TIM15_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
	}else if(timer_struct->handle.Instance==TIM16){
		__HAL_RCC_TIM16_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	}else if(timer_struct->handle.Instance==TIM17){
		__HAL_RCC_TIM17_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);
	}else if(timer_struct->handle.Instance==TIM20){
		__HAL_RCC_TIM20_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM20_UP_IRQn);
	}
#elif defined(STM32H743xx)
	if(timer_struct->handle.Instance==TIM1){
		__HAL_RCC_TIM1_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	}else if(timer_struct->handle.Instance==TIM7){
		__HAL_RCC_TIM7_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM7_IRQn);
	}else if(timer_struct->handle.Instance==TIM12){
		__HAL_RCC_TIM12_CLK_ENABLE();
	}else if(timer_struct->handle.Instance==TIM13){
		__HAL_RCC_TIM13_CLK_ENABLE();
	}else if(timer_struct->handle.Instance==TIM14){
		__HAL_RCC_TIM14_CLK_ENABLE();
	}else if(timer_struct->handle.Instance==TIM15){
		__HAL_RCC_TIM15_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM15_IRQn);
	}else if(timer_struct->handle.Instance==TIM16){
		__HAL_RCC_TIM16_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM16_IRQn);
	}else if(timer_struct->handle.Instance==TIM17){
		__HAL_RCC_TIM17_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM17_IRQn);
	}
#endif
}