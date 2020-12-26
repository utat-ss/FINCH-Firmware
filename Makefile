# It may seem strange to have a Makefile to call CMake, which in turn calls Make
# This Makefile is here to provide simpler commands to execute which are more
# intuitive to remember and use
# It also allows commands to be run from the root of the repository without
# having to cd into the build folder
# It also makes it easier to specify paths to manual tests without having to add
# .. to go up a directory
# It uses default CMake configuration parameters (such as the toolchain file
# and build type) which can be overriden if desired

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
.PHONY: upload
upload: compile
	st-flash write $(BUILD_DIR)/$(TEST_TARGET).bin 0x8000000

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
$(BUILD_DIR):
ifeq ($(MCU),)
	@echo "ERROR: Parameter MCU must be defined"
	exit 1
endif
	mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
	cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=$(BUILD) -DMCU_SERIES=$(MCU) .. && \
	cd ..

# Remove the build directory
# This forces CMake to regenerate the Makefiles next time a program is compiled
.PHONY: clean
clean:
ifeq ($(MCU),)
	rm -rf $(BUILD_DIR_BASE)-G4
	rm -rf $(BUILD_DIR_BASE)-H7
else
	rm -rf $(BUILD_DIR)
endif
