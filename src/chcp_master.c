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
#include <avr/sleep.h>
#include <util/delay.h>
#include "chcp.h"
#include "chcp_pin.h"
#include "debug.h"
#include "led.h"
#include "chcp_credit.h"
#include "chcp_master.h"

void master(struct chcp_t *chcp, struct debug_t *debug)
{
	if (debug->active)
		debug_print_P(PSTR("Master mode\n"), debug);

	for (;;) {
		while (!chcp_present(chcp))
			_delay_ms(1000);

		chcp->auth = 0; /* Default to non-auth */
		led_set(RED, ON);
		_delay_ms(500);
		chcp_reset(chcp->atr);

		if (*(chcp->atr) == 162) {
			chcp_dump_prt_memory(chcp->protected_memory);
			chcp_dump_secmem(chcp->security_memory);
			chcp_dump_memory(chcp->main_memory);
			debug_atr(chcp->atr, debug);
			debug_prt_memory(chcp->protected_memory, debug);
			debug_secmem(chcp->security_memory, debug);
			debug_memory(chcp->main_memory, debug);

			if (credit_check(chcp)) {
				debug_print_P(PSTR("\n Valid card with credit\n"), debug);
				debug_print_P(PSTR("\n Insert a blank card please! \n"), debug);
			} else {
				debug_print_P(PSTR("\n New card! Press 7 to auth \n"), debug);
				/* FIX ME */
				loop_until_bit_is_clear(PINC, 7);

				/* Do auth */
				chcp_auth(chcp, CHCP_PIN1, CHCP_PIN2, CHCP_PIN3);
				debug_proc_counts(chcp->ck_proc, debug);
				debug_secmem(chcp->security_memory, debug);
			}
		}

		if (chcp->auth) {
			/* authenticaded operations */
			debug_print_P(PSTR("\n Writing cards with credit \n"), debug);
			memcpy(chcp->main_memory + 32, "CHARLIE", 7);
			*(chcp->main_memory + 40) = 0x0a; /* 10 bucks */
			chcp_write_memory(chcp, 32, 9);
			debug_print_P(PSTR("done!\n"), debug);
			debug_memory(chcp->main_memory, debug);
			/* green on */
			led_set(GREEN, ON);
		}

		debug_print_P(PSTR("Now you can remove the card!\n"), debug);

		while (chcp_present(chcp)) {
			if (!chcp->auth)
				led_set(RED, BLINK);

			_delay_ms(500);
		}

		led_set(NONE, OFF);
	}
}

