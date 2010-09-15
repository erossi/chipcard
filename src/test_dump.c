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
#include "print_uart.h"

int main(void)
{
	struct chcp_t *chcp;
	char *line;
	char *string;

	chcp_init_io();
	uart_init();

	DDRC = 1; /* PC0 OUT */
	PORTC = 1; /* PC0 led off */

	chcp = malloc(sizeof(struct chcp_t));
	chcp->atr = malloc(4);
	chcp->main_memory = malloc(256);
	chcp->protected_memory = malloc(32);
	chcp->security_memory = malloc(4);
	line = malloc(80);
	string = malloc(20);

	strcpy_P (line, PSTR("Chipcard SLE4447 DUMP Memory test.\n"));
	uart_printstr (line);

	for (;;) {
		while (!chcp_present(chcp))
			_delay_ms(1000);

		uart_putchar ('\n');
		PORTC=0;
		_delay_ms(1000);

		chcp_reset(chcp->atr);
		print_atr(chcp->atr, line, string);
		chcp_dump_memory(chcp->main_memory);
		print_memory(chcp->main_memory, line, string);
		chcp_dump_prt_memory(chcp->protected_memory);
		print_prt_memory(chcp->protected_memory, line, string);
		chcp_dump_secmem(chcp->security_memory);
		print_secmem(chcp->security_memory, line, string);

		while (chcp_present(chcp))
			_delay_ms(1000);

		PORTC=1;
		_delay_ms(1000);
	}

	free(chcp->atr);
	free(chcp->main_memory);
	free(chcp->protected_memory);
	free(chcp->security_memory);
	free(chcp);
	free(line);
	free(string);
}

