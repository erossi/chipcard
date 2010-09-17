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
#include <avr/pgmspace.h>
#include "chcp.h"
#include "uart.h"
#include "print_uart.h"

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

	uart_putchar ('\n');
}

void print_atr(uint8_t *atr, char *line, char *string)
{
	strcpy_P (line, PSTR("-> ATR Bytes ----\n"));
	uart_printstr (line);
	print_mm(atr, line, string, 4);
}

void print_memory(uint8_t *mm, char *line, char *string)
{
	strcpy_P (line, PSTR("-> Main Memory Bytes ----\n"));
	uart_printstr (line);
	print_mm(mm, line, string, 256);
}

void print_prt_memory(uint8_t *pm, char *line, char *string)
{
	strcpy_P (line, PSTR("-> Protected Memory Bytes ----\n"));
	uart_printstr (line);
	print_mm(pm, line, string, 4);
}

void print_secmem(uint8_t *sm, char *line, char *string)
{
	strcpy_P (line, PSTR("-> Security Memory Bytes ----\n"));
	uart_printstr (line);
	print_mm(sm, line, string, 4);
}

void print_proc_counts(uint8_t *pc, char *line, char *string)
{
	strcpy_P (line, PSTR("-> Auth Proccessing times ----\n"));
	uart_printstr (line);
	print_mm(pc, line, string, 5);
}

