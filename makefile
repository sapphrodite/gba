SOURCE_DIRECTORIES := src/ src/common/ src/mmio/
TARGET_NAME := rom.gba
BUILD_DIR = build

THIS_MAKEFILE := $(firstword $(MAKEFILE_LIST))
SOURCE_PATH := $(dir $(realpath $(THIS_MAKEFILE)))
SOURCES := $(foreach d, $(SOURCE_DIRECTORIES), $(wildcard $(d)*.s))
SOURCES += $(foreach d, $(SOURCE_DIRECTORIES), $(wildcard $(d)*.cpp))
OBJECTS := $(patsubst $(SOURCE_DIR)%, $(BUILD_DIR)/%.o, $(SOURCES))

INCLUDE_FLAGS := -Isrc/
CXXFLAGS = --std=c++20 -mcpu=arm7tdmi -nostartfiles -fno-exceptions -mthumb -nodefaultlibs -O3
LDFLAGS = -T lnkscript $(CXXFLAGS)

$(BUILD_DIR)/%.o: $(SOURCE_PATH)% $(THIS_MAKEFILE)
	@mkdir -p "$(dir $@)"
	@echo "[CXX] $@"
	arm-none-eabi-g++ $(LDFLAGS_REQ) $(CXXFLAGS) $(INCLUDE_FLAGS) -c "$<" -o "$@"
	
$(TARGET_NAME): $(OBJECTS) lnkscript
	@echo "[LD] $(TARGET_NAME)"
	arm-none-eabi-gcc $(OBJECTS) $(LDFLAGS) -o $(TARGET_NAME)
	@echo "Stripping and formatting ROM"
	#@arm-none-eabi-objcopy -O binary $(TARGET_NAME)

all: $(TARGET_NAME)

clean:
	rm -rf rom*
	rm -rf build/

