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
#include <avr/sleep.h>
#include <util/delay.h>
#include "chcp.h"
#include "chcp_pin.h"
#include "uart.h"
#include "print_uart.h"
#include "chcp_credit.h"
#include "chcp_master.h"

void master(struct chcp_t *chcp, char *line, char *string)
{
	strcpy_P (line, PSTR("Master mode\n"));
	uart_printstr (line);

	for (;;) {
		while (!chcp_present(chcp))
			_delay_ms(1000);

		chcp->auth = 0; /* Default to non-auth */
		uart_putchar ('\n');
		PORTC = 2; /* RED on */
		_delay_ms(500);

		chcp_reset(chcp->atr);

		if (*(chcp->atr) == 162) {
			chcp_dump_prt_memory(chcp->protected_memory);
			chcp_dump_secmem(chcp->security_memory);
			chcp_dump_memory(chcp->main_memory);
			print_atr(chcp->atr, line, string);
			print_prt_memory(chcp->protected_memory, line, string);
			print_secmem(chcp->security_memory, line, string);
			print_memory(chcp->main_memory, line, string);

			if (credit_check(chcp)) {
				strcpy_P (line, PSTR("\n Valid card with credit\n"));
				uart_printstr (line);
				strcpy_P (line, PSTR("\n Insert a blank card please! \n"));
				uart_printstr (line);

			} else {
				strcpy_P (line, PSTR("\n New card! Press 7 to auth \n"));
				uart_printstr (line);
				loop_until_bit_is_clear(PINC, 7);

				/* Do auth */
				chcp_auth(chcp, CHCP_PIN1, CHCP_PIN2, CHCP_PIN3);
				print_proc_counts(chcp->ck_proc, line, string);
				print_secmem(chcp->security_memory, line, string);
			}
		}

		if (chcp->auth) {
			/* authenticaded operations */
			strcpy_P (line, PSTR("\n Writing cards with credit \n"));
			uart_printstr (line);
			memcpy(chcp->main_memory + 32, "CHARLIE", 7);
			*(chcp->main_memory + 40) = 0x0a; /* 10 bucks */
			chcp_write_memory(chcp, 32, 9);
			strcpy_P (line, PSTR("done!\n"));
			uart_printstr (line);
			print_memory(chcp->main_memory, line, string);
			/* green on */
			PORTC = 1;
		}

		strcpy_P (line, PSTR("Now you can remove the card!\n"));
		uart_printstr (line);

		while (chcp_present(chcp)) {
			if (!chcp->auth) {
				/* blink red */
				PORTC = 3; /* off */
				_delay_ms(500);
				PORTC = 2; /* RED on */
			}

			_delay_ms(500);
		}

		PORTC = 3; /* leds off */
	}
}

