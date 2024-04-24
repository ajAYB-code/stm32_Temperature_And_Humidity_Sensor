
PROJ = main


# Define linker script file here
LDSCRIPT = config/STM32F411RE_FLASH.lds

# Define optimisation level here
OPT = -O0

# Debug
DEBUG = -g -D__DEBUG__

# List all user C define here
UDEFS = -DSTM32F411xE

# Define ASM defines here
UADEFS = 

# List C source files here
SRC  = startup/stm32f411_periph.c startup/sys_handlers.c \
       startup/system_stm32f4xx.c \
       STM32F4xx_HAL_Driver/src/stm32f4xx_ll_rcc.c \
       STM32F4xx_HAL_Driver/src/stm32f4xx_hal_i2c.c \
       STM32F4xx_HAL_Driver/src/util.c \
       STM32F4xx_HAL_Driver/src/stm32f4xx_hal.c \
       STM32F4xx_HAL_Driver/src/stm32f4xx_hal_cortex.c \
       STM32F4xx_HAL_Driver/src/stm32f4xx_hal_uart.c \
       STM32F4xx_HAL_Driver/src/stm32f4xx_hal_gpio.c \
       STM32F4xx_HAL_Driver/src/stm32f4xx_hal_tim.c \
       STM32F4xx_HAL_Driver/src/stm32f4xx_hal_spi.c \
       mbed_shield/src/leds.c \
       mbed_shield/src/sw.c \
       mbed_shield/src/lm75.c \
       mbed_shield/src/lcd_128x32.c \
       src/stm32f4xx_hal_msp.c \
       src/stm32f4xx_it.c \
       src/${PROJ}.c 

# List ASM source files here
ASRC = startup/startup_stm32f411xe.s

# List all user directories here
UINCDIR = .

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS = #lib/libstm32.a

#
# End of user defines
#############################################################################
INCLUDE = $(addprefix -I,$(INC_DIRS))

TARGET  = arm-none-eabi-
CC      = $(TARGET)gcc
OBJCOPY = $(TARGET)objcopy
AS      = $(TARGET)gcc -x assembler-with-cpp -c
SIZE    = $(TARGET)size
OBJDUMP = $(TARGET)objdump

ARCH      = armv7e-m
FLOAT-ABI = hard
CPU       = cortex-m4
CPUFLAGS  = -mthumb
FPU       = fpv4-sp-d16

# List all default C defines here, like -D_DEBUG=1
DDEFS =-march=$(ARCH) -mfloat-abi=$(FLOAT-ABI) -mcpu=$(CPU) -mfpu=$(FPU) $(CPUFLAGS)

# List all default ASM defines here, like -D_DEBUG=1
DADEFS = -D__ASSEMBLY__

# List all default directories to look for include files here
DINCDIR =  STM32F4xx_HAL_Driver/inc/ include/ mbed_shield/inc/

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS = 

#
# End of default section
##############################################################################################

INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)
OBJS    = $(SRC:.c=.o) $(ASRC:.s=.o)
LIBS    = $(DLIBS) $(ULIBS)

ASFLAGS = $(INCDIR) $(DEFS) -Wa,--gdwarf2 $(ADEFS)
CFLAGS = -std=c99 $(INCDIR) $(DEBUG) $(OPT) -Wall $(DEFS) -Wextra -Warray-bounds -Wno-unused-parameter -fomit-frame-pointer
LDFLAGS = $(DEFS) -T$(LDSCRIPT) -lc -lgcc -lgcov -lm -Wl,-Map=$@.map,--gc-sections $(LIBDIR)

# Generate dependency information
CFLAGS += -MD -MP -MF .dep/$(@F).d
ASFLAGS += -MD -MP -MF .dep/$(@F).d

#
# makefile rules
#
all: $(OBJS) main.elf main.hex main.bin

%o: %c
	$(CC) -c $(CFLAGS) $< -o $@

%o: %s
	$(AS) $(ASFLAGS) $< -o $@

%.elf: $(OBJS) $(LDSCRIPT)
	$(CC) -o $@ $(filter-out %.lds, $^) $(LDFLAGS) $(LIBS)
	$(OBJDUMP) -h $@
	$(SIZE) $@
	
%hex: %elf
	$(OBJCOPY) -O ihex $< $@

%bin: %elf
	$(OBJCOPY) -O binary $< $@
	
clean:
	-rm -f $(OBJS)
	-rm -f *.elf
	-rm -f *.map
	-rm -f *.bin
	-rm -f *.hex
	-rm -fR .dep/*

# 
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

.PHONY: clean

