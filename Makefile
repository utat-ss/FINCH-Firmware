# It may seem strange to have a Makefile to call CMake, which in turn calls Make
# This Makefile is here to provide simpler commands to execute which are more
# intuitive to remember and use
# It also allows commands to be run from the root of the repository without
# having to cd into the build folder
# It also makes it easier to specify paths to manual tests without having to add
# .. to go up a directory
# It uses default CMake configuration parameters (such as the toolchain file
# and build type) which can be overriden if desired

# Folder for CMake configuration and builds
BUILD_DIR = Build
# Default built type
BUILD = Debug

# Strip trailing slash in the test path if there is one
TEST_TARGET := $(patsubst %/,%,$(TEST))
# Replace every slash (/) with a dash (-) - this is necessary to match our CMake
# configuration with how it names targets since a target name cannot contain a
# slash
# Need to use a simply expanded variable (:=) instead of a recursively expanded
# variable (=) or else TEST_TARGET would be recursively defined in terms of
# itself
# https://www.gnu.org/software/make/manual/html_node/Flavors.html
TEST_TARGET := $(subst /,-,$(TEST_TARGET))

# Compile all test programs
all: $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
	cmake --build . -- -j 1 && \
	cd ..

# Compile one test program
compile: $(BUILD_DIR)
ifeq ($(TEST),)
	@echo "ERROR: Parameter TEST must be defined"
	exit 1
endif
	@cd $(BUILD_DIR) && \
	cmake --build . --target $(TEST_TARGET) -- -j 1 && \
	cd ..

# Compile and upload one test program
upload: compile
# TODO - call stlink?

# Create the build directory and convert the CMake configuration to Makefiles
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
	cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=$(BUILD) .. && \
	cd ..

# Remove the build directory
# This forces CMake to regenerate the Makefiles next time a program is compiled
clean:
	rm -r $(BUILD_DIR)
