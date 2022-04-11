BUILD_DIR = build
TARGET_NAME := rom.gba
THIS_MAKEFILE := $(firstword $(MAKEFILE_LIST))
SOURCE_PATH := $(dir $(realpath $(THIS_MAKEFILE)))

TEXTURE_DIRECTORIES := textures/
TEXTURE_SOURCES := $(foreach d, $(TEXTURE_DIRECTORIES), $(wildcard $(d)*.png))
TEXTURES := $(patsubst $(SOURCE_DIR)%.png, $(BUILD_DIR)/%.tex, $(TEXTURE_SOURCES))
CONVERTER := tools/png2gbatiles

SOURCE_DIRECTORIES := src/ src/common/ src/mmio/
SOURCES := $(foreach d, $(SOURCE_DIRECTORIES), $(wildcard $(d)*.cpp))
OBJECTS := $(patsubst $(SOURCE_DIR)%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPFILES := $(patsubst %.o,%.d,$(OBJECTS))
-include $(DEPFILES)

INCLUDE_FLAGS := -Isrc/
CXXFLAGS = --std=c++20 -nostartfiles -fno-exceptions -mthumb-interwork -mthumb -nodefaultlibs -O3 -Wall -Wextra
LDFLAGS = -T lnkscript $(CXXFLAGS)

LD = arm-none-eabi-ld
CXX = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy

$(BUILD_DIR)/%.tex: $(SOURCE_PATH)%.png $(THIS_MAKEFILE) $(CONVERTER)
	@mkdir -p $(@D)
	@$(CONVERTER) $< $(*F)
	@echo "[$(notdir $(CONVERTER))] $(@F)"
	$(LD) -b binary -r $(*F).tex $(*F).pal -o $@ \
		--defsym=$(*F)_tex=_binary_$(*F)_tex_start --defsym=$(*F)_tex_size=_binary_$(*F)_tex_size \
		--defsym=$(*F)_pal=_binary_$(*F)_pal_start --defsym=$(*F)_pal_size=_binary_$(*F)_pal_size
	@rm $(*F).pal $(*F).tex

$(BUILD_DIR)/%.o: $(SOURCE_PATH)%.cpp $(THIS_MAKEFILE)
	@mkdir -p "$(dir $@)"
	@echo "[CXX] $(notdir $@)"
	$(CXX) $(LDFLAGS_REQ) $(CXXFLAGS) $(INCLUDE_FLAGS) -MMD -MP -c "$<" -o "$@"

$(TARGET_NAME): $(OBJECTS) $(TEXTURES) lnkscript src/init.s
	@echo "[LD] $(notdir $@)"
	$(CXX) $(OBJECTS) $(TEXTURES) src/init.s $(LDFLAGS) -o $(TARGET_NAME)
	@echo "Stripping and formatting ROM"
	$(OBJCOPY) -O binary $(TARGET_NAME)

$(CONVERTER): tools/png2gbatiles.cpp
	echo "[TOOL] $(CONVERTER)"
	g++ $<  -g3 -o3 -lpng -o $@

all: $(TARGET_NAME)

clean:
	rm -rf rom*
	rm -rf build/

