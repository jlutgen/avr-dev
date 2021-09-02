############################################################
#
# Makefile for Whittier College AVR projects
#
# Jeff Lutgen
#
#############################################################

# Processor type
PROCESSOR=atmega2560

ifdef WSL_DISTRO_NAME        # Windows Subsystem for Linux
	PORT=todo
else
	PORT=/dev/cu.usbmodem14101
endif

CROSS=avr

# The C compiler
CC=$(CROSS)-gcc

# The bin creator
HX=$(CROSS)-objcopy

# The object dumper
OBJDMP=$(CROSS)-objdump

# The size-reporting tool
SIZE=$(CROSS)-size

# The utility for writing a .bin file to the MCU.
WRITE=avrdude

# The output target $(TARGET).hex
TARGET=out

CFLAGS = \
	-ggdb3 -Os -Wall \
	-std=gnu11 \
	-mmcu=$(PROCESSOR)

LINKFLAGS=-Wl,-Map=$(TARGET).map
