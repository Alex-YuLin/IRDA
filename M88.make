## General Flags
MCU = atmega88
CC = avr-gcc
CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99  -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
# CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -Wl,-u,vfprintf  -Wl,-u,vfscanf -Wl,-Map=$*.map

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature
HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

## Libraries
LIBS = -lm -lprintf_flt -lscanf_flt

## Objects that must be built in order to link
OBJECTS =

## Objects explicitly added by the user
LINKONLYOBJECTS = ".\ASA_Lib.o"

AVR_PORT = 6
IRDA_BAUD = 4800
## Build
# all: IRDA_M128_171211.hex
# 	@cmd_ASA_loader.exe -p $(AVR_PORT) -h IRDA_M128_171211.hex
# 	@terminal.exe -P $(AVR_PORT) -B $(IRDA_BAUD)
 

help:
	@echo "use \"make {filename}.hex\" to complie .c to .hex in dir ./hex"

## LIB OBJECTS Compile

##Link
%.o: %.c
	$(CC) $(include) $(CFLAGS) -c $< -o $@

%.elf: %.o $(OBJECTS)
	 $(CC) $(LDFLAGS) $< $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $@



%.hex: %.elf
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.a: %.elf %.hex
	@echo
	@avr-size -C --mcu=${MCU} $<
	@-mkdir -p hex
	@-mv $*.hex hex/
	@-rm -rf $(OBJECTS) $*.map $*.o $<


%.eep: %.elf
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: %.elf
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) *.elf *.hex ./hex/*
