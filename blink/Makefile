CUBE_ROOT := ../STM32Cube_FW_F1_V1.4.0
CMSIS_ROOT := $(CUBE_ROOT)/Drivers/CMSIS
HAL_ROOT := $(CUBE_ROOT)/Drivers/STM32F1xx_HAL_Driver

GNU_PREFIX := arm-none-eabi
GCC_ROOT := /c/gcc-arm-none-eabi
GCC_BIN := $(GCC_ROOT)/bin
CC := $(GCC_BIN)/arm-none-eabi-gcc
LD := $(GCC_BIN)/arm-none-eabi-ld

OBJDUMP := "$(GCC_BIN)/$(GNU_PREFIX)-objdump"
OBJCOPY := "$(GCC_BIN)/$(GNU_PREFIX)-objcopy"
SIZE := "$(GCC_BIN)/$(GNU_PREFIX)-size"

MAKE := make
MK := mkdir
RM := rm -rf

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

OUTPUT_FILENAME := blink

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

C_SOURCE_FILES += \
$(abspath blink.c) \
$(abspath $(CMSIS_ROOT)/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c)

ASM_SOURCE_FILES  = $(abspath $(CMSIS_ROOT)/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s)

LIBS := -lrust_src

LINKER_SCRIPT := $(abspath STM32F103XB_FLASH.ld)

INC_PATHS := -I$(abspath include)
INC_PATHS += -I$(abspath $(CMSIS_ROOT)/Device/ST/STM32F1xx/Include)
INC_PATHS += -I$(abspath $(CMSIS_ROOT)/Include)
INC_PATHS += -I$(abspath $(HAL_ROOT)/Inc)

CFLAGS := -Wall -Werror -Og
CFLAGS += --std=gnu99
CFLAGS += -g
CFLAGS += -DSTM32F103xB
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m3

LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys
LDFLAGS += -L$(abspath rust_src/target/thumbv7m-none-eabi/debug)

ASMFLAGS := -x assembler-with-cpp

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

ifeq ("$(VERBOSE)","1")
NO_ECHO :=
else
NO_ECHO := @
endif

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

default: all

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo CC $(notdir $<)
	$(NO_ECHO) $(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<
	$(NO_ECHO)$(SIZE) $@

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo AS $(notdir $<)
	$(NO_ECHO) $(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<

# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).elf: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo "LD $(notdir $(OBJECTS)) $(LIBS) -> $(OUTPUT_FILENAME).elf"
	$(NO_ECHO) $(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).elf

all: clean _build/blink.elf
#	-@echo ""
#	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).elf
#	-@echo ""
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).elf $(OBJECTS)

clean:
	$(RM) $(BUILD_DIRECTORIES)


build-rust:
	cargo build --target thumbv7m-none-eabi --manifest-path rust_src\Cargo.toml