MCU = atmega16
INC = -I/usr/avr/include/
LIBS = m
OPTLEV = 2
FCPU = 4000000UL
CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
LFLAGS = -L$(shell pwd) -l$(LIBS) -lchcp
AR = avr-ar
CC = avr-gcc

PRGNAME = chipcard
OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size
DUDE = avrdude -c stk500v1 -p $(MCU) -P /dev/ttyUSB0 -e -U flash:w:$(PRGNAME).hex
REMOVE = rm -f

chcp_lib_obj = chcp_bbg.o chcp.o
objects = uart.o print_uart.o chcp_counter.o
objects += chcp_master.o chcp_slave.o chcp_credit.o

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o $(PRGNAME).elf $(PRGNAME).c $(objects) $(LFLAGS)
	$(OBJCOPY) $(PRGNAME).elf $(PRGNAME).hex

lib: $(chcp_lib_obj)
	$(AR) rcs libchcp.a $(chcp_lib_obj)

program:
	$(DUDE)

libclean:
	$(REMOVE) $(chcp_lib_obj) libchcp.a

clean:
	$(REMOVE) $(PRGNAME).elf $(PRGNAME).hex $(objects)

allclean: clean libclean

