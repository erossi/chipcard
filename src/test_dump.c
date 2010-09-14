/* This file is part of chpc
 * Copyright (C) 2010 Enrico Rossi
 *
 * Chpc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chpc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "chcp.h"
#include "uart.h"

void print_mm(uint8_t *mm, char *line, char *string, const int max)
{
	int i;

	uart_putchar ('\n');

	for (i=0; i<max; i++) {
		strcpy_P (line, PSTR("Byte: "));
		string = utoa(i, string, 10);
		strcat (line, string);
		strcpy_P (string, PSTR(" = "));
		strcat (line, string);
		string = utoa(*(mm+i), string, 10);
		strcat (line, string);
		uart_printstr (line);
		uart_putchar ('\n');
	}
}

void print_atr(uint8_t *atr, char *line, char *string)
{
	strcpy_P (line, PSTR("-> ATR Bytes ----\n"));
	print_mm(atr, line, string, 4);
}

void print_memory(uint8_t *atr, char *line, char *string)
{
	strcpy_P (line, PSTR("-> Main Memory Bytes ----\n"));
	print_mm(atr, line, string, 256);
}

void print_prt_memory(uint8_t *atr, char *line, char *string)
{
	strcpy_P (line, PSTR("-> Protected Memory Bytes ----\n"));
	print_mm(atr, line, string, 32);
}

void print_secmem(uint8_t *atr, char *line, char *string)
{
	strcpy_P (line, PSTR("-> Security Memory Bytes ----\n"));
	print_mm(atr, line, string, 4);
}

int main(void)
{
	uint8_t *atr;
	uint8_t *main_memory;
	char *line;
	char *string;

	chcp_init_io();
	uart_init();

	DDRC = 1; /* PC0 OUT */
	PORTC = 1; /* PC0 led off */

	atr = malloc(4);
	main_memory = malloc(256);
	line = malloc(80);
	string = malloc(20);

	strcpy_P (line, PSTR("Chipcard SLE4447 DUMP Memory test.\n"));
	uart_printstr (line);

	for (;;) {
		while (!chcp_present())
			_delay_ms(1000);

		uart_putchar ('\n');
		PORTC=0;
		_delay_ms(1000);

		chcp_reset(atr);
		print_atr(atr, line, string);
		chcp_dump_memory(main_memory);
		print_memory(main_memory, line, string);
		chcp_dump_prt_memory(main_memory);
		print_prt_memory(main_memory, line, string);
		chcp_dump_secmem(main_memory);
		print_secmem(main_memory, line, string);

		while (chcp_present())
			_delay_ms(1000);

		PORTC=1;
		_delay_ms(1000);
	}

	free(atr);
	free(main_memory);
	free(line);
	free(string);
}
