TARGET = main
DEBUG = 1
OPT = -Og

BUILD_DIR = build
PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

C_SOURCES =  \
HARDWARE/MySPI/MySPI.c\
HARDWARE/W25Q128/W25Q128.c\
HARDWARE/LCD/lcd.c\
HARDWARE/scr/bluetooth.c\
USER/main.c\
USER/global.c\
HARDWARE/scr/delay.c\
HARDWARE/scr/usart1.c\
HARDWARE/scr/stm32f10x_it.c\
HARDWARE/scr/led.c\
HARDWARE/scr/timer2.c\
HARDWARE/scr/sys.c\
HARDWARE/scr/beep.c\
CORE/core_cm3.c\
CORE/system_stm32f10x.c\
STLIB/src/misc.c\
STLIB/src/stm32f10x_rcc.c\
STLIB/src/stm32f10x_tim.c\
STLIB/src/stm32f10x_usart.c\
STLIB/src/stm32f10x_gpio.c\
STLIB/src/stm32f10x_exti.c\
STLIB/src/stm32f10x_flash.c\

ASM_SOURCES =  \
startup_stm32f103xb.s

CPU = -mcpu=cortex-m3

MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

AS_DEFS = 

C_DEFS =  \
-DARM_GCC \
-DSTM32F10X_MD \
-DUSE_STDPERIPH_DRIVER\
-DSTM32F10X_MD\

AS_INCLUDES =

C_INCLUDES =  \
-IHARDWARE/MySPI\
-IHARDWARE/W25Q128\
-IHARDWARE/LCD\
-IUSER\
-ICORE\
-ISTLIB/inc\
-IHMAC/inc\
-IHARDWARE/inc\

ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
CFLAGS+=-std=c11
endif

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -finput-charset=UTF-8 -fexec-charset=GBK

LDSCRIPT = STM32F103C8Tx_FLASH.ld

LIBS = -lc -lm -lnosys -u _printf_float -u _scanf_float
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@echo build $@
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo build $@
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo build $@
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@

clean:
	-rm -fR $(BUILD_DIR)

-include $(wildcard $(BUILD_DIR)/*.d)
