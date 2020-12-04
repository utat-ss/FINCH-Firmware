/*
 * nvic_test.c
 *
 *  Created on: Dec 4, 2020
 *      Author: Cameron Rodriguez
 *
 * Overview: Keeps an LED on, and then uses a higher priority interrupt to occasionally turn it off. Assumes clock speed of 64 MHz.
 */

#include <drivers/STM32_nvic/nvic.h>
#include <drivers/STM32_tim/tim.h>
#include <drivers/STM32_gpio/gpio.h>

#ifdef STM32G474xx
#include <stm32g4xx_hal_gpio.h>
#define LED_PORT GPIOA
#define LED_PIN GPIO_PIN_5
#elif defined(STM32H743xx)
#include <stm32h7xx_hal_gpio.h>
#define LED_PORT GPIOB
#define LED_PIN GPIO_PIN_7
#endif

static TimFunc low_priority_timer, high_priority_timer;
static GPIO_OUTPUT led_struct;

void led_on(){
	gpio_set_high(led_struct);
}

void led_off(){
	gpio_set_low(led_struct);
}

// Setup timers with different priority levels, and NVIC layout
void setup_timers(){
	HAL_Init();
	clock_init();
	nvic_set_priority_grouping(NVIC_PRIORITYGROUP_1);
	led_struct = gpio_init_output(LED_PORT, LED_PIN);


	low_priority_timer = tim_quickstart_void_function(TIM3, 64000000, 4, led_on);
	nvic_set_priority(TIM3_IRQn, 1, 0);
	high_priority_timer = tim_quickstart_void_function(TIM3, 64000000, 1, led_off);
	nvic_set_priority(TIM4_IRQn, 0, 0);
}

int main(){
	setup_timers();
	// Runs autonomously from here on
	return 0;

}
