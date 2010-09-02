MCU = atmega8
INC = -I/usr/avr/include/
LIBS = m
OPTLEV = 2
FCPU = 4000000UL
CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
LFLAGS = -l$(LIBS)
CC = avr-gcc

PRGNAME = test_chpc
OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size
DUDE = avrdude -c stk500v1 -p $(MCU) -P /dev/ttyUSB0 -e -U flash:w:$(PRGNAME).hex
REMOVE = rm -f

objects = 

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o $(PRGNAME).elf $(PRGNAME).c $(objects) $(LFLAGS)
	$(OBJCOPY) $(PRGNAME).elf $(PRGNAME).hex

program:
	$(DUDE)

clean:
	$(REMOVE) $(PRGNAME).elf $(PRGNAME).hex $(objects)

