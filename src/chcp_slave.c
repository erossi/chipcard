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
#include "chcp_counter.h"
#include "chcp_slave.h"

void slave(struct chcp_t *chcp, struct debug_t *debug)
{
	if (debug->active)
		debug_print_P(PSTR("Slave mode\n"), debug);

	for (;;) {
		/* FIX ME!! SLEEP if power is missing */
		while (!chcp_present(chcp))
			_delay_ms(1000);

		chcp->auth = 0; /* Default to non-auth */
		led_set(RED, ON);
		_delay_ms(500);
		chcp_reset(chcp->atr);

		/* FIX ME - do a better check */
		if (*(chcp->atr) == 162) {
			/* I don't need to dump the entire memory */
			chcp_dump_prt_memory(chcp->protected_memory);
			chcp_dump_secmem(chcp->security_memory);
			chcp_dump_memory(chcp->main_memory);

			if (debug->active) {
				debug_atr(chcp->atr, debug);
				debug_prt_memory(chcp->protected_memory, debug);
				debug_secmem(chcp->security_memory, debug);
				debug_memory(chcp->main_memory, debug);
			}

			if (credit_check(chcp)) {
				if (debug->active) {
					debug_print_P(PSTR("\n Valid card with credit\n"), debug);
					debug_print_P(PSTR("\n Auth? (y/N): "), debug);
					/*! Infinite loop until y is pressed */
					while (!debug_wait_for_y(debug));
				}

				/* Do auth */
				chcp_auth(chcp, CHCP_PIN1, CHCP_PIN2, CHCP_PIN3);

				if (debug->active) {
					debug_proc_counts(chcp->ck_proc, debug);
					debug_secmem(chcp->security_memory, debug);
				}
			} else {
				if (debug->active)
					debug_print_P(PSTR("\n Error! - Non initialized card!\n"), debug);
			}
		}

		/*! authenticaded operations goes here */
		if (chcp->auth) {
			credit_bucks = credit_suck(chcp);

			if (debug->active)
				debug_memory(chcp->main_memory, debug);

			led_set(GREEN, ON);
		}

		if (debug->active)
			debug_print_P(PSTR("Now you can remove the card!\n"), debug);

		if (!chcp->auth)
			while (chcp_present(chcp))
				led_set(RED, BLINK);
		else {
			/* test the counter */
			sleep_enable();

			while (credit_bucks) {
				/* start the counter */
				counter_start();
				/* sleep */
				sleep_cpu();
				/*! awakening via IRQ */
				/* stop the counter */
				counter_stop();

				if (debug->active) {
					debug_print_P(PSTR("Awake with bucks: "), debug);
					debug->string = itoa(credit_bucks, debug->string, 10);
					strcpy(debug->line, debug->string);
					strcat(debug->line, "\n");
					debug_print(debug);
				}

				/* FIX ME: Why ? */
				_delay_ms(200);
			}

			sleep_disable();
		}

		led_set(NONE, OFF);
	}
}

