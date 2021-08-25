/*
 * gpio_it_input.c
 *
 *  Created on: Aug 6, 2021
 *      Author: bruno
 *
 * A library for GPIO input pins that trigger an interrupt on the rising edge,
 * the falling edge, or both.
 *
 * There are several GPIO ports (A, B, C, ...) and 16 GPIO pins per port (0 to
 * 15)
 * It seems that the EXTI (extended interrupt and event controller) only
 * supports 16 interrupt sources corresponding to the 16 GPIO pins
 * This means all GPIO pins on different ports but with the same pin number
 * (e.g. A12, B12, F12) are connected to the same interrupt source
 */

#include <common/stm32/gpio/GPIOITInput.h>
#include <common/stm32/mcu/errors.h>
#include <common/stm32/util/Util.h>

// This should set all callbacks by default to NULL
GPIOITInputCB g_gpio_it_input_cbs[GPIO_IT_INPUT_EXTI_COUNT] = {NULL};

/*
 * @param mode - one of GPIO_MODE_IT_RISING, GPIO_MODE_IT_FALLING,
 *               GPIO_MODE_IT_RISING_FALLING
 * @param pull - one of GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
 */
void gpio_it_input_init(GPIOITInput* gpio, MCU* mcu, GPIO_TypeDef* port,
        uint16_t pin, uint32_t mode, uint32_t pull, GPIOITInputCB callback) {

    // Initialize GPIOInput struct within GPIOInterrupt struct
    gpio->input.mcu = mcu;
    gpio->input.port = port;
    gpio->input.pin = pin;

    // Set function pointer for interrupt callback
    for (uint32_t i = 0; i < GPIO_IT_INPUT_EXTI_COUNT; i++) {
        if (bit(i) == pin) {
            g_gpio_it_input_cbs[i] = callback;
        }
    }

    // Initialize pin with HAL library
    GPIO_InitTypeDef init = {0};
    init.Pin = pin;
    init.Mode = mode;
    init.Pull = pull;
    HAL_GPIO_Init(port, &init);

    /* EXTI interrupt init*/
    IRQn_Type irq;
    if (pin == GPIO_PIN_0) {
        irq = EXTI0_IRQn;
    }
    else if (pin == GPIO_PIN_1) {
        irq = EXTI1_IRQn;
    }
    else if (pin == GPIO_PIN_2) {
        irq = EXTI2_IRQn;
    }
    else if (pin == GPIO_PIN_3) {
        irq = EXTI3_IRQn;
    }
    else if (pin == GPIO_PIN_4) {
        irq = EXTI4_IRQn;
    }
    else if (GPIO_PIN_9 >= pin && pin >= GPIO_PIN_5) {
        irq = EXTI9_5_IRQn;
    }
    else if (GPIO_PIN_15 >= pin && pin >= GPIO_PIN_10) {
        irq = EXTI15_10_IRQn;
    }
    else {
        Error_Handler();
        // Default value to prevent -Wmaybe-uninitialized warning
        irq = EXTI0_IRQn;
    }
    // Choose preemption priority as 4 since it should be fairly high priority,
    // but probably not the highest priority
    HAL_NVIC_SetPriority(irq, 4, 0);
    HAL_NVIC_EnableIRQ(irq);
}




// -----------------------------------------------------------------------------
// Interrupt handlers

/*
 * high and low are both INCLUSIVE
 */
void gpio_exti_irq_handler(uint16_t high, uint16_t low) {
    // e.g. if high = GPIO_PIN_15, low = GPIO_PIN_10, pin goes from
    // GPIO_PIN_15 -> GPIO_PIN_14 -> ... -> GPIO_PIN_10
    for (uint16_t pin = high; pin >= low; pin >>= 1) {
        HAL_GPIO_EXTI_IRQHandler(pin);
    }
}

/**
 * @brief This function handles EXTI line[0] interrupts.
 */
void EXTI0_IRQHandler(void) {
    gpio_exti_irq_handler(GPIO_PIN_0, GPIO_PIN_0);
}

/**
 * @brief This function handles EXTI line[1] interrupts.
 */
void EXTI1_IRQHandler(void) {
    gpio_exti_irq_handler(GPIO_PIN_1, GPIO_PIN_1);
}

/**
 * @brief This function handles EXTI line[2] interrupts.
 */
void EXTI2_IRQHandler(void) {
    gpio_exti_irq_handler(GPIO_PIN_2, GPIO_PIN_2);
}

/**
 * @brief This function handles EXTI line[3] interrupts.
 */
void EXTI3_IRQHandler(void) {
    gpio_exti_irq_handler(GPIO_PIN_3, GPIO_PIN_3);
}

/**
 * @brief This function handles EXTI line[4] interrupts.
 */
void EXTI4_IRQHandler(void) {
    gpio_exti_irq_handler(GPIO_PIN_4, GPIO_PIN_4);
}

/**
 * @brief This function handles EXTI line[9:5] interrupts.
 */
void EXTI9_5_IRQHandler(void) {
    gpio_exti_irq_handler(GPIO_PIN_9, GPIO_PIN_5);
}

/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
void EXTI15_10_IRQHandler(void) {
    gpio_exti_irq_handler(GPIO_PIN_15, GPIO_PIN_10);
}

/**
 * @brief  EXTI line detection callback.
 * @param  gpio_pin: Specifies the port pin connected to corresponding EXTI
 *                   line.
 * @retval None
 *
 * This function overrides the weak version of this function in the HAL.
 * This function is called by HAL_GPIO_EXTI_IRQHandler() in the HAL.
 */
void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin) {
    for (uint32_t i = 0; i < GPIO_IT_INPUT_EXTI_COUNT; i++) {
        if (bit(i) == gpio_pin && g_gpio_it_input_cbs[i] != NULL) {
            g_gpio_it_input_cbs[i]();
        }
    }
}
