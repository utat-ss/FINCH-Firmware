# It may seem strange to have a Makefile to call CMake, which in turn calls Make
# This Makefile is here to provide simpler commands to execute which are more
# intuitive to remember and use
# It also allows commands to be run from the root of the repository without
# having to cd into the build folder
# It also makes it easier to specify paths to manual tests without having to add
# .. to go up a directory
# It uses default CMake configuration parameters (such as the toolchain file
# and build type) which can be overriden if desired

# TODO - test STLink software with H7 MCUs
# As of January 2021, the STLink software package does not support the H7 series
# H7 support has been merged into the development branch
# (https://github.com/stlink-org/stlink/pull/1033), but it will not be available
# in a release until March 2021 (https://github.com/stlink-org/stlink/milestone/15)

# TODO - try to detect MCU model/series automatically (maybe using st-info?)
# TODO - allow user to specify which MCU/STLink if multiple are connected
# https://github.com/stlink-org/stlink/blob/develop/doc/man/st-flash.md
# https://github.com/stlink-org/stlink/issues/318

# For some commands, must assign the MCU variable when calling Make to specify
# the MCU series (currently can be "G4" or "H7")
# e.g. make ... MCU=G4
# e.g. make ... MCU=H7

# Default build type
BUILD = Debug
# Base folder name for CMake configuration and builds
BUILD_DIR_BASE = Build
# Folder for CMake configuration and builds
BUILD_DIR = $(BUILD_DIR_BASE)-$(MCU)

# Directory path to use for `make clean` command
ifeq ($(MCU),)
	CLEAN_PATH = $(BUILD_DIR_BASE)-*
else
	CLEAN_PATH = $(BUILD_DIR)
endif

# Strip trailing slash in the test path if there is one
# Use both forward slash (/) and backward slash (\) for Windows compatibility
# Need to use a simply expanded variable (:=) instead of a recursively expanded
# variable (=) or else TEST_TARGET would be recursively defined in terms of
# itself
TEST_TARGET := $(TEST)
TEST_TARGET := $(patsubst %/,%,$(TEST_TARGET))
TEST_TARGET := $(patsubst %\,%,$(TEST_TARGET))
# Replace every slash (/ or \) with a dash (-) - this is necessary to match our
# CMake configuration with how it names targets since a target name cannot
# contain a slash
# https://www.gnu.org/software/make/manual/html_node/Flavors.html
TEST_TARGET := $(subst /,-,$(TEST_TARGET))
TEST_TARGET := $(subst \,-,$(TEST_TARGET))

# Detect operating system
ifeq ($(OS),Windows_NT)
	WINDOWS = 1
endif

# Compile all test programs for each MCU series
# This command is useful to run before committing or merging code, to at least
# make sure all programs still compile correctly
.PHONY: all
all:
ifeq ($(MCU),)
	make all MCU=G4
	make all MCU=H7
else
	make $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
	cmake --build . -- -j 1 && \
	cd ..
endif

# Compile one test program
.PHONY: compile
compile: $(BUILD_DIR)
ifeq ($(TEST),)
	@echo "ERROR: Parameter TEST must be defined"
	exit 1
endif
	@cd $(BUILD_DIR) && \
	cmake --build . --target $(TEST_TARGET) -- -j 1 && \
	cd ..

# Compile and upload one test program to MCU
# Need the --reset option to reset the MCU (to start executing the program)
# after writing the program to memory, or else the program doesn't start running
# automatically and you need to press the hardware reset button to start it
.PHONY: upload
upload: compile
	st-flash --reset write $(BUILD_DIR)/$(TEST_TARGET).bin 0x8000000

# Download (read) current firmware on MCU
# BYTES defines the number of bytes to read (as a hex value, e.g.
# `make download BYTES=0x1000` to read 4096 bytes)
.PHONY: download
download:
ifeq ($(BYTES),)
	@echo "ERROR: Parameter BYTES must be defined"
	exit 1
endif
	st-flash read downloaded_fw.bin 0x8000000 $(BYTES)

# Erase current firmware on MCU
.PHONY: erase
erase:
	st-flash erase

# Create the build directory and convert the CMake configuration to Makefiles
# Want to use the -p switch for mkdir so it doesn't give an error if the
# directory already exists
# mkdir is emulated by New-Item on Windows, but the -p switch doesn't work so we
# call New-Item directly instead
# Based on https://docs.microsoft.com/en-us/powershell/module/microsoft.powershell.management/new-item?view=powershell-7.1
# (Example 8)
$(BUILD_DIR):
ifeq ($(MCU),)
	@echo "ERROR: Parameter MCU must be defined"
	exit 1
endif
ifeq ($(WINDOWS),1)
	powershell New-Item -Path $(BUILD_DIR) -ItemType Directory -Force
else
	mkdir -p $(BUILD_DIR)
endif
	@cd $(BUILD_DIR) && \
	cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=$(BUILD) -DMCU_SERIES=$(MCU) .. && \
	cd ..

# Remove the build directory
# This forces CMake to regenerate the Makefiles next time a program is compiled
# Similar to the $(BUILD_DIR) target, rm is emulated by Remove-Item on Windows
# The -f switch does not work, so call Remove-Item directly instead
.PHONY: clean
clean:
ifeq ($(WINDOWS),1)
	powershell Remove-Item -Path $(CLEAN_PATH) -Recurse -ErrorAction Ignore
else
	rm -rf $(CLEAN_PATH)
endif
