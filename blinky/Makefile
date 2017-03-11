OBJECTS=blinky.o

GCC_DIR = ~/ti/msp430_gcc/bin
SUPPORT_FILE_DIRECTORY = ~/ti/msp430_gcc/include

# Please set your device here
DEVICE  = msp430f5529
#DEVICE  = msp430afe222
CC      = $(GCC_DIR)/msp430-elf-gcc
GDB     = $(GCC_DIR)/msp430-elf-gdb

CFLAGS = -I $(SUPPORT_FILE_DIRECTORY) -mmcu=$(DEVICE) -O2 -g
LFLAGS = -L $(SUPPORT_FILE_DIRECTORY) -T $(DEVICE).ld

all: ${OBJECTS}
	$(CC) $(CFLAGS) $(LFLAGS) $? -o $(DEVICE).out

debug: all
	$(GDB) $(DEVICE).out

clean:
	rm -rf *.o *.out
