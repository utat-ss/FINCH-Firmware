/*
 * tim_test.c
 *
 *  Created on: Jul 22, 2020
 *      Author: Cameron Rodriguez
 *
 * Overview: A timer that uses LD2 to flash 'FINCH' in Morse code on the NUCLEO-G474RE. Based on https://visualgdb.com/tutorials/arm/stm32/timers/hal/
 */


#include <STM32_tim/tim.h>

#ifdef STM32G474xx
#include <stm32g4xx_hal_gpio.h>
#define LED_PORT GPIOA
#define LED_PIN GPIO_PIN_5
#elif defined(STM32H743xx)
#include <stn32h7xx_hal_gpio.h>
#define LED_PORT GPIOB
#define LED_PIN GPIO_PIN_7
#endif

static TimFunc timer;
static GPIO_OUTPUT led_struct;
volatile uint8_t time_unit = 0;
volatile uint8_t led_toggle[] = {1,1,1,1,1,0,0,1,1,1,0,0,
						  1,1,1,1,0,0,
						  1,0,0,1,1,1,0,0,
						  1,0,0,1,1,1,1,0,0,1,1,1,0,0,
						  1,1,1,1,1,1,1,1,0,0,0,0,0,0}; // Each line is a letter and the trailing space or end of word.

// Interrupt handler, advances at the end of each period
void TIM2_IRQHandler(){
	if(tim_check_flag(&timer, TIM_IT_UPDATE)){
		if(led_toggle[time_unit]){
			gpio_toggle(led_struct);
		}

		time_unit++;
		if(time_unit==54){ //Reset the cycle
			time_unit=0;
		}
	}
}

// Setup the timer and interrupt functions
void setup_tim(){
	HAL_Init();
	clock_init();
	TimMode mode = {.mode = TIM_Mode_IT};

	timer = tim_config_base(TIM2, mode, 62499, 1631);
}

/* Flashes 'FINCH' on the evaluation kit LD2 LED.
 *
 * In this context, a time unit is 150 ms (20/6 Hz), so the entire message (54 time units) is transmitted in 8.1 seconds.
 * Update events occur at each other time unit (300 ms or 20/3 Hz).
 */
int main(){
	setup_tim();
	led_struct = gpio_init_output(LED_PORT, LED_PIN);

	tim_start(&timer);

	return 0;
}
