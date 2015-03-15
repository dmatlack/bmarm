##############################################################################
# 
# BMARM Makefile
#
##############################################################################

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
ARM_CPU = cortex-a8

##############################################################################
# Compiler options

#
# For C++ builds, also include:
#   -fno-rtti: disables generation of information about every class with
#     virtual functions. Eliminates several KB of support code from the
#     C++ runtime library.
#   -fno-exceptions: Disables generation of extra code needed to
#     propagate exceptions. Saves many KB of support code.
#

#
# Target ARM processor (tells gcc which kinds of instructions it can
# emit when generating assembly code).
#
CFLAGS += -mcpu=$(ARM_CPU)

#
# Allow freely mixing Thumb and ARM instructions
#
CFLAGS += -mthumb-interwork

#
# Perform function calls by first loading the address of the function
# into a register and then performing a subroutine call (BX) on this
# register. This allows the function to be at any 32-bit address.
#
CFLAGS += -mlong-calls

#
# Place each function into its own section in the output file. Function
# foo is placed in section .text.foo.
#
CFLAGS += -ffunction-sections

#
# Warnings
#
CFLAGS += -Wall -Werror

#
# Debug Options
#
CFLAGS += -g

#
# Includes
#
CFLAGS += -Iinclude

##############################################################################
# Source Files

SRCDIRS = .

EXCLUDE = ! -path "./toolchain/*"

CFILES = $(shell find $(SRCDIRS) -type f -name "*.c" $(EXCLUDE) -print)
SFILES = $(shell find $(SRCDIRS) -type f -name "*.S" $(EXCLUDE) -print)
HFILES = $(shell find $(SRCDIRS) -type f -name "*.h" $(EXCLUDE) -print)

OFILES = $(patsubst %.c,%.o,$(CFILES)) $(patsubst %.S,%.o,$(SFILES))
DFILES = $(patsubst %.o,%.d,$(OFILES))

##############################################################################
# Misc Defines

#
# Host connected to beagleboard UART port.
#
SERIAL_SERVER = "david@raspberrypi"

##############################################################################
# Build Targets

.PHONY: default clean install

default: link

install: link
	scp kernel.bin boot.kermit $(SERIAL_SERVER):
	ssh $(SERIAL_SERVER)

link: $(OFILES) linker.ld
	$(CC) -T linker.ld -o kernel.o -ffreestanding -nostdlib $(OFILES) -lgcc
	[ `nm kernel.o | grep "T _start" | cut -f1 -d" "` = "80200000" ]
	$(OBJCOPY) kernel.o -O binary kernel.bin

-include $(DFILES)

%.o: %.S
	$(CC) $(CFLAGS) -DASSEMBLER -c -o $@ $<
	$(CC) -MM -MF $(patsubst %.o,%.d,$@) $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	$(CC) -MM -MF $(patsubst %.o,%.d,$@) $<

clean:
	rm -rf kernel.o
	rm -rf kernel.bin
	rm -rf $(DFILES)
	rm -rf $(OFILES)
