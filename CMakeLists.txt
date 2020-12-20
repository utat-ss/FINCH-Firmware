# This file is adapted from https://dev.to/younup/cmake-on-stm32-the-beginning-3766

cmake_minimum_required(VERSION 3.15.3)

project(nucleo-f413zh)

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# It seems that it is a bad idea to use globbing to find all .c (or other)
# files, so we will explicitly list all files
# Source: https://stackoverflow.com/questions/2110795/how-to-use-all-c-files-in-a-directory-with-the-cmake-build-system
set(STM32CUBEMX_GENERATED_FILES

        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c

        Src/drivers/STM32H7_clock/clock.c
        Src/drivers/STM32_gpio/gpio.c

        Startup/STM32H7/stm32h7xx_hal_conf.h
        Startup/STM32H7/stm32h7xx_it.h

        Manual_Tests/STM32_gpio/gpio_test.c
        Startup/STM32H7/stm32h7xx_it.c
        Startup/STM32H7/system_stm32h7xx.c
        Startup/STM32H7/stm32h7_startup.c

        Startup/STM32H7/startup_stm32h743zitx.s)

set(EXECUTABLE ${PROJECT_NAME}.out)

add_executable(${EXECUTABLE} ${STM32CUBEMX_GENERATED_FILES})

target_compile_definitions(${EXECUTABLE} PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32H743xx
        )

target_include_directories(${EXECUTABLE} PRIVATE
        Src
        Drivers/STM32H7xx_HAL_Driver/Inc
        Drivers/CMSIS/Device/ST/STM32H7xx/Include
        Drivers/CMSIS/Include
        Startup/STM32H7
        BSP/Drivers/STM32F4xx_HAL_Driver/Inc
        BSP/Drivers/CMSIS/Device/ST/STM32F4xx/Include
        BSP/Drivers/CMSIS/Include
        )

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -fdata-sections
        -ffunction-sections

        -Wall

        $<$<CONFIG:Debug>:-Og>
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_SOURCE_DIR}/Projects/STM32H743ZITX/STM32H743ZITX_FLASH.ld
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -specs=nano.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        )

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)
