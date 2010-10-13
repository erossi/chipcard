MCU = atmega16
PWD = $(shell pwd)
INC = -I/usr/avr/include/ -I$(PWD)/../include
OPTLEV = 2
FCPU = 4000000UL
CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
LFLAGS = -L$(PWD)/../lib -lsle4442

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

uart_obj = uart.o debug.o
objects = $(uart_obj) counter.o tools.o led.o
slave_obj = slave.o credit.o
master_obj = master.o

GITRELEASE = "\"$(shell git describe --tags)\""

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects) $(master_obj) $(slave_obj)
	$(CC) $(CFLAGS) -o $(PRGNAME).elf main.c -D MASTERSLAVE $(objects) $(master_obj) $(slave_obj) $(LFLAGS)
	$(OBJCOPY) $(PRGNAME).elf $(PRGNAME).hex

master: $(objects) $(master_obj)
	$(CC) $(CFLAGS) -o $(PRGNAME).elf $(PRGNAME).c -D MASTERONLY $(objects) $(master_obj) $(LFLAGS)
	$(OBJCOPY) $(PRGNAME).elf $(PRGNAME).hex

slave: $(objects) $(slave_obj)
	$(CC) $(CFLAGS) -o $(PRGNAME).elf $(PRGNAME).c -D SLAVEONLY $(objects) $(slave_obj) $(LFLAGS)
	$(OBJCOPY) $(PRGNAME).elf $(PRGNAME).hex

debug.o:
	$(CC) $(CFLAGS) -D GITREL=$(GITRELEASE) -c debug.c

program:
	$(DUDE)

clean:
	$(REMOVE) $(PRGNAME).elf $(PRGNAME).hex $(objects) $(master_obj) $(slave_obj)

