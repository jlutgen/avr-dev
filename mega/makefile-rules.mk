############################################################
#
# Makefile for Whittier College AVR projects
#
# Jeff Lutgen
#
#############################################################

# Define SRCS in Makefile in project directory, then include this file

# Processor type
DEVICE=atmega2560

ifdef WSL_DISTRO_NAME        # Windows Subsystem for Linux
	PORT=todo
else                         # MacOS
	PORT=/dev/cu.usbmodem14101
endif

CROSS=avr

# The C compiler
CC=$(CROSS)-gcc

# The linker
LD=$(CROSS)-gcc

# The bin creator
HX=$(CROSS)-objcopy

# The object dumper
OBJDMP=$(CROSS)-objdump

# The size-reporting tool
SIZE=$(CROSS)-size

# The utility for writing a binary file to the MCU.
WRITE=avrdude

# The output target $(TARGET).hex
TARGET=out

OBJS = $(SRCS:%.c=%.o)

INCLUDES =

CFLAGS += -ggdb3 -Os -Wall
CFLAGS += -std=gnu11
CFLAGS += -mmcu=$(DEVICE)
CFLAGS += -Wextra -Wshadow -Wno-unused-variable
CFLAGS += -Wredundant-decls -Wstrict-prototypes
CFLAGS += -Wno-error=narrowing
CFLAGS += -MMD

LDFLAGS += -mmcu=$(DEVICE)
LDFLAGS += -Wl,-Map=$(TARGET).map
LDFLAGS += -Wl,--cref
LDFLAGS += -Wl,--gc-sections

# What to do for "make all"
all: $(TARGET).hex $(TARGET).dis size.stdout

# Turn the elf file into a hex file.
$(TARGET).hex: $(TARGET).elf
	@echo
	@echo Creating hex file $@
	$(HX) -j .text -j .data -O ihex $(TARGET).elf $@

size.stdout: $(TARGET).elf
	$(SIZE) $<

# Generate disassembly file.
$(TARGET).dis: $(TARGET).elf
	@echo
	@echo Creating disassembly file $@
	$(OBJDMP) -h -S $< > $@

# Compile a source file to an object file
%.o: %.c
	@echo
	@echo Compiling source file $< to object file $@
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -o $@ -c $<

# Link all the object files into an elf file.
$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	@echo
	@echo Linking objects into elf file $@
	$(LD) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(TARGET).elf 

# Delete map, object, elf, and hex files, as well as other assorted crud
clean:
	$(RM) *.map *.o *.elf *.hex *.d *.dis *~

write: $(TARGET).hex $(TARGET).dis
	$(WRITE) -v -c wiring -p $(DEVICE) -P $(PORT) -b 115200 \
	-D -U flash:w:$(TARGET).hex:i

.PHONY: all clean write size.stdout
-include $(OBJS:.o=.d)