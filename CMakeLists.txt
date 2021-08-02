# This file is adapted from https://dev.to/younup/cmake-on-stm32-the-beginning-3766

# Define the minimum version of CMake required
# As of 2021-01-02 the latest version is 3.19.2, but use 3.16.3 as the minimum
# because that is the version used for Ubuntu 20.04 LTS
cmake_minimum_required(VERSION 3.16.3)
# Define the entire project name
project(Firmware
        DESCRIPTION "FINCH Firmware"
)

# Make sure MCU model parameter passed into CMake is valid
if ((${MCU} MATCHES "G474") OR (${MCU} MATCHES "H743"))
else()
        message(FATAL_ERROR "Invalid MCU model: must be G474 or H743")
endif()

# Enable using C and Assembly source files
enable_language(C ASM)
# Use the C11 standard to match the CubeIDE project
set(CMAKE_C_STANDARD 11)
# Enforce that we must use the C11 standard
set(CMAKE_C_STANDARD_REQUIRED ON)
# Must request GNU compiler-specific extensions (i.e. use gnu11 instead of c11)
# Described here: https://community.st.com/s/question/0D53W000003N839/build-stm32l071cbt-project-with-stm32cubeide-121-failedsolve
# If this is off, will get the following error:
#       .../Firmware/System/STM32H7/syscalls.c:41:27: error:expected '=', ',', ';', 'asm' or '__attribute__' before 'asm'
#       register char * stack_ptr asm("sp");
set(CMAKE_C_EXTENSIONS ON)

# Create a shared library (object library) that compiles all library source files
# only once rather than repeating compilation for each test target
# https://stackoverflow.com/questions/29634101/cmake-how-to-define-target-without-linking-compilation-only
# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#object-libraries
# https://cmake.org/cmake/help/latest/command/add_library.html
set(LIBRARIES_TARGET libraries)
add_library(${LIBRARIES_TARGET} OBJECT)

# Normally it is not recommended to glob for all source files, because CMake
# does not know to regenerate if a file is added or removed
# https://stackoverflow.com/questions/2110795/how-to-use-all-c-files-in-a-directory-with-the-cmake-build-system
# https://cmake.org/cmake/help/v3.7/command/file.html
# But we are going to do it anyway so we don't need to change the source file
# list every time we add a new file
# It's very fast to just delete the build folder and recreate it anyways when
# adding a new file

# Recursively find all source (.c and .s) files under the Src directory
file(GLOB_RECURSE SRC_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "Src/*.c" "Src/*.s")
# Add Src files to libraries target
target_sources(${LIBRARIES_TARGET} PRIVATE
        ${SRC_SOURCES}
)

# Add source files for the particular MCU series in use
# The ^ matches a string at the beginning (checks what the string starts with)
# https://cmake.org/cmake/help/v3.7/command/string.html#regex-specification
if (${MCU} MATCHES "^G4")
        # Recursively find all source (.c and .s) files under the Drivers/STM32G4xx_HAL_Driver directory
        file(GLOB_RECURSE DRIVERS_G4_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "Drivers/STM32G4xx_HAL_Driver/*.c" "Drivers/STM32G4xx_HAL_Driver/*.s")
        # Recursively find all source (.c and .s) files under the System/STM32G4 directory
        file(GLOB_RECURSE SYSTEM_G4_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "System/STM32G4/*.c" "System/STM32G4/*.s")

        # Add G4 source files to libraries target
        target_sources(${LIBRARIES_TARGET} PRIVATE
                ${DRIVERS_G4_SOURCES}
                ${SYSTEM_G4_SOURCES}
        )
elseif (${MCU} MATCHES "^H7")
        # Recursively find all source (.c and .s) files under the Drivers/STM32H7xx_HAL_Driver directory
        file(GLOB_RECURSE DRIVERS_H7_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "Drivers/STM32H7xx_HAL_Driver/*.c" "Drivers/STM32H7xx_HAL_Driver/*.s")
        # Recursively find all source (.c and .s) files under the System/STM32H7 directory
        file(GLOB_RECURSE SYSTEM_H7_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "System/STM32H7/*.c" "System/STM32H7/*.s")

        # Add H7 source files to libraries target
        target_sources(${LIBRARIES_TARGET} PRIVATE
                ${DRIVERS_H7_SOURCES}
                ${SYSTEM_H7_SOURCES}
        )
endif()

# Recursively find all test source (.c) files under the Manual_Tests directory
file(GLOB_RECURSE TEST_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "Manual_Tests/*.c")

# Loop through all test source files
foreach(TEST_SOURCE IN LISTS TEST_SOURCES)
        # Get the source file's parent directory path
        get_filename_component(TEST_DIR ${TEST_SOURCE} DIRECTORY)
        # Append the parent directory path to the list of test directories
        list(APPEND TEST_DIRS ${TEST_DIR})
endforeach()
# In case there are multiple .c files in any one test directory, remove
# duplicated test directories
list(REMOVE_DUPLICATES TEST_DIRS)

# Loop through all test directories
foreach(TEST_DIR IN LISTS TEST_DIRS)
        # A CMake target can't have slashes in its name, so replace all forward
        # slashes in the test directory path with dashes to create the target
        # name
        # Don't just use the last directory in the path because that could lead
        # to naming conflicts
        string(REPLACE "/" "-" TEST_TARGET ${TEST_DIR})
        # Append the target name to the list of test target names
        list(APPEND TEST_TARGETS ${TEST_TARGET})

        # Recursively find all test source (.c) files under the test directory
        file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${TEST_DIR}/*.c")

        # Add a new executable target for this test
        # Combine object files compiled from the libraries target with the test
        # sources specifically for this test
        add_executable(${TEST_TARGET} $<TARGET_OBJECTS:${LIBRARIES_TARGET}> ${SOURCES})
endforeach()

# Save common options between the compiler and linker in variables so we don't
# have to repeat them for both
# To see settings used in the IDE project and their descriptions/explanations,
# under IDE project properties, go to C/C++ Build > Settings > Tool Settings
set(OPTIONS
        -mfloat-abi=hard
        -mthumb
        -specs=nano.specs
)
set(G4_OPTIONS
        -mcpu=cortex-m4
        -mfpu=fpv4-sp-d16
)
set(H7_OPTIONS
        -mcpu=cortex-m7
        -mfpu=fpv5-d16
)

# Loop through all targets (both libraries and tests since these settings apply
# to both)
foreach(TARGET IN LISTS LIBRARIES_TARGET TEST_TARGETS)
        # Specify all header (.h) file directories
        # Similar to source files, explicitly list all files rather than globbing 
        target_include_directories(${TARGET} PRIVATE
                Drivers/CMSIS/Include
                Src
        )

        # Specify compiler definitions (macros/symbols)
        target_compile_definitions(${TARGET} PRIVATE
                # Define USE_FULL_ASSERT here instead of uncommenting the line
                # in stm32g4xx_hal_conf.h (or similar files) to guarantee all
                # files will see it, whether or not they included the header file
                -DUSE_FULL_ASSERT
                -DUSE_HAL_DRIVER
        )

        # Add compiler options
        target_compile_options(${TARGET} PRIVATE
                ${OPTIONS}

                -fdata-sections
                -ffunction-sections
                -fstack-usage

                # Maximum debugging information
                # https://stackoverflow.com/questions/10475040/gcc-g-vs-g3-gdb-flag-what-is-the-difference
                -g3

                # Optimization level
                $<$<CONFIG:Debug>:-Og>

                -Wall
        )

        # Add MCU series-specific settings
        if (${MCU} MATCHES "^G4")
                target_include_directories(${TARGET} PRIVATE
                        Drivers/CMSIS/Device/ST/STM32G4xx/Include
                        Drivers/STM32G4xx_HAL_Driver/Inc
                        System/STM32G4
                )

                target_compile_definitions(${TARGET} PRIVATE
                        -DSTM32G4
                )
                if (${MCU} MATCHES "^G474")
                        target_compile_definitions(${TARGET} PRIVATE
                                -DSTM32G474xx
                        )
                endif()

                target_compile_options(${TARGET} PRIVATE
                        ${G4_OPTIONS}
                )
        elseif (${MCU} MATCHES "^H7")
                target_include_directories(${TARGET} PRIVATE
                        Drivers/CMSIS/Device/ST/STM32H7xx/Include
                        Drivers/STM32H7xx_HAL_Driver/Inc
                        System/STM32H7
                )

                target_compile_definitions(${TARGET} PRIVATE
                        -DSTM32H7
                )
                if (${MCU} MATCHES "^H743")
                        target_compile_definitions(${TARGET} PRIVATE
                                -DSTM32H743xx
                        )
                endif()

                target_compile_options(${TARGET} PRIVATE
                        ${H7_OPTIONS}
                )
        endif()
endforeach()

# Loop through test targets
# These settings only apply to test targets (because they are linked and produce
# a final binary)
foreach(TARGET IN LISTS TEST_TARGETS)
        # Add linker options
        target_link_options(${TARGET} PRIVATE
                ${OPTIONS}
                
                -static

                # Generate map file, which provides linker info and symbols
                -Wl,-Map=${TARGET}.map
                -Wl,--gc-sections

                # Use C math library
                # end-group must come after start-group or else we get the
                # linker error: "group ended before it began"
                -Wl,--start-group
                -lc
                -lm
                -Wl,--end-group

                # Enable printf-style formatting/printing of floating-point
                # numbers
                -u_printf_float
                # Enable scanf-style reading/input of floating-point numbers
                -u_scanf_float
        )

        # Add MCU series-specific settings
        # Specify linker script
        if (${MCU} MATCHES "^G4")
                target_link_options(${TARGET} PRIVATE
                        ${G4_OPTIONS}
                )
                if (${MCU} MATCHES "G474")
                        target_link_options(${TARGET} PRIVATE
                                -T${CMAKE_SOURCE_DIR}/Projects/STM32G474RETX/STM32G474RETX_FLASH.ld
                        )
                endif()
        elseif (${MCU} MATCHES "^H7")
                target_link_options(${TARGET} PRIVATE
                        ${H7_OPTIONS}
                )
                if (${MCU} MATCHES "H743")
                        target_link_options(${TARGET} PRIVATE
                                -T${CMAKE_SOURCE_DIR}/Projects/STM32H743ZITX/STM32H743ZITX_FLASH.ld
                        )
                endif()
        endif()

        # Print executable size
        add_custom_command(TARGET ${TARGET}
                POST_BUILD
                COMMAND arm-none-eabi-size ${TARGET}
        )

        # Create hex file
        add_custom_command(TARGET ${TARGET}
                POST_BUILD
                COMMAND arm-none-eabi-objcopy -O ihex ${TARGET} ${TARGET}.hex
                COMMAND arm-none-eabi-objcopy -O binary ${TARGET} ${TARGET}.bin
        )
endforeach()
