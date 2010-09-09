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

void print_atr(uint8_t *atr, char *line, char *string)
{
	uint8_t i;

	for (i=0; i<4; i++) {
		strcpy_P (line, PSTR("ATR Byte: "));
		string = utoa(*(atr+i), string, 10);
		strcat (line, string);
		uart_printstr (line);
		uart_putchar ('\n');
	}
}

int main(void)
{
	uint8_t *atr;
	char *line;
	char *string;

	chcp_init_io();
	uart_init();

	DDRC = 1; /* PC0 OUT */
	PORTC = 1; /* PC0 led off */

	atr = malloc(4);
	line = malloc(80);
	string = malloc(20);

	strcpy_P (line, PSTR("Chipcard SLE4447 ATR test.\n"));
	uart_printstr (line);

	for (;;) {
		while (!chcp_present())
			_delay_ms(1000);

		PORTC=0;
		_delay_ms(1000);

		chcp_reset(atr);
		print_atr(atr, line, string);
		strcpy_P (line, PSTR("----\n"));
		uart_printstr (line);

		while (chcp_present())
			_delay_ms(1000);

		PORTC=1;
		_delay_ms(1000);
	}

	free(atr);
	free(line);
	free(string);
}

