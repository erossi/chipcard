MCU = atmega16
INC = -I/usr/avr/include/
OPTLEV = 2
FCPU = 4000000UL
CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
LFLAGS = -L$(shell pwd) -lm -lchcp

AR = avr-ar
CC = avr-gcc

DUDEPORT = /dev/ttyUSB0
DUDEDEV = stk500v1
PRGNAME = chipcard
OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size
DUDE = avrdude -c $(DUDEDEV) -p $(MCU) -P $(DUDEPORT) -e -U flash:w:$(PRGNAME).hex
REMOVE = rm -f

chcp_lib_obj = chcp_bbg.o chcp.o
uart_obj = uart.o debug.o
objects = $(uart_obj) chcp_counter.o
objects += chcp_master.o chcp_slave.o chcp_credit.o

GITRELEASE = "\"$(shell git describe --tags)\""

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o $(PRGNAME).elf $(PRGNAME).c $(objects) $(LFLAGS)
	$(OBJCOPY) $(PRGNAME).elf $(PRGNAME).hex

debug.o:
	$(CC) $(CFLAGS) -D GITREL=$(GITRELEASE) -c debug.c

lib: $(chcp_lib_obj)
	$(AR) rcs libchcp.a $(chcp_lib_obj)

program:
	$(DUDE)

libclean:
	$(REMOVE) $(chcp_lib_obj) libchcp.a

clean:
	$(REMOVE) $(PRGNAME).elf $(PRGNAME).hex $(objects)

allclean: clean libclean

