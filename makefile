BUILD_DIR = build
TARGET_NAME := rom.gba
THIS_MAKEFILE := $(firstword $(MAKEFILE_LIST))
SOURCE_PATH := $(dir $(realpath $(THIS_MAKEFILE)))

TEXTURE_DIRECTORIES := textures/
TEXTURE_SOURCES := $(foreach d, $(TEXTURE_DIRECTORIES), $(wildcard $(d)*.png))
TEXTURES := $(patsubst $(SOURCE_DIR)%.png, $(BUILD_DIR)/%.tex, $(TEXTURE_SOURCES))
PALETTES := $(patsubst %.tex, %.pal, $(TEXTURES))
CONVERTER := tools/png2gbatiles

SOURCE_DIRECTORIES := src/ src/common/ src/mmio/
SOURCES := $(foreach d, $(SOURCE_DIRECTORIES), $(wildcard $(d)*.cpp))
OBJECTS := $(patsubst $(SOURCE_DIR)%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPFILES := $(patsubst %.o,%.d,$(OBJECTS))
-include $(DEPFILES)

INCLUDE_FLAGS := -Isrc/
CXXFLAGS = --std=c++20 -nostartfiles -fno-exceptions -mthumb-interwork -mthumb -nodefaultlibs -O3 -Wall -Wextra
LDFLAGS = -T lnkscript $(CXXFLAGS)

$(BUILD_DIR)/%.tex $(BUILD_DIR)/%.pal: $(SOURCE_PATH)%.png $(THIS_MAKEFILE) $(CONVERTER)
	@mkdir -p "$(dir $@)"
	@$(CONVERTER) "$<" $(notdir $*)
	@echo "[$(notdir $(CONVERTER))] $(notdir $*).tex, $(notdir $*).pal"
	arm-none-eabi-ld -b binary -r $(notdir $*).tex -o $(BUILD_DIR)/$*.tex
	arm-none-eabi-ld -b binary -r $(notdir $*).pal -o $(BUILD_DIR)/$*.pal
	@rm $(notdir $*).pal $(notdir $*).tex

$(BUILD_DIR)/%.o: $(SOURCE_PATH)%.cpp $(THIS_MAKEFILE)
	@mkdir -p "$(dir $@)"
	@echo "[CXX] $(notdir $@)"
	arm-none-eabi-g++ $(LDFLAGS_REQ) $(CXXFLAGS) $(INCLUDE_FLAGS) -MMD -MP -c "$<" -o "$@"

$(TARGET_NAME): $(OBJECTS) $(TEXTURES) $(PALETTES) lnkscript src/init.s
	@echo "[LD] $(notdir $@)"
	arm-none-eabi-gcc  $(OBJECTS) $(TEXTURES) $(PALETTES) src/init.s $(LDFLAGS) -o $(TARGET_NAME)
	@echo "Stripping and formatting ROM"
	@arm-none-eabi-objcopy -O binary $(TARGET_NAME)

$(CONVERTER): tools/png2gbatiles.cpp
	echo "[TOOL] $(CONVERTER)"
	g++ $<  -g3 -o3 -lpng -o $@

all: $(TARGET_NAME)

clean:
	rm -rf rom*
	rm -rf build/

