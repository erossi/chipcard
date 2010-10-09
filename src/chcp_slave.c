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

static uint8_t check_sle_atr(struct chcp_t *chcp) {
	uint8_t atr[4] = {0xa2, 0x13, 0x10, 0x91};

	if (memcmp(chcp->atr, &atr, 4))
		return(0);
	else
		return(1);
}

void slave(struct chcp_t *chcp, struct debug_t *debug)
{
	debug_print_P(PSTR("Slave mode\n"), debug);

	for (;;) {
		/* FIX ME!! SLEEP if power is missing */
		while (!chcp_present(chcp))
			_delay_ms(1000);

		/*! Default to non-auth */
		chcp->auth = 0;
		led_set(RED, ON);
		/*! Wait between card insertion and begin */
		_delay_ms(500);
		chcp_reset(chcp->atr);

		/*! check the correct SLE4442 atr */
		if (check_sle_atr(chcp)) {
			/* I don't need to dump the entire memory */
			chcp_dump_prt_memory(chcp->protected_memory);
			chcp_dump_secmem(chcp->security_memory);
			chcp_dump_memory(chcp->main_memory);

			debug_atr(chcp->atr, debug);
			debug_prt_memory(chcp->protected_memory, debug);
			debug_secmem(chcp->security_memory, debug);
			debug_memory(chcp->main_memory, debug);

			if (credit_check(chcp)) {
				debug_print_P(PSTR("\n Valid card with credit\n"), debug);
				debug_print_P(PSTR("\n Auth? (y/N): "), debug);
				/*!
				 Infinite loop until y is pressed.
				 Have to check if debug is active or
				 you'll lock the code.
				 */
				if (debug->active)
					while (!debug_wait_for_y(debug));

				/* Do auth */
				chcp_auth(chcp, CHCP_PIN1, CHCP_PIN2, CHCP_PIN3);
				debug_proc_counts(chcp->ck_proc, debug);
				debug_secmem(chcp->security_memory, debug);
			} else
				debug_print_P(PSTR("\n Error! - Non initialized card!\n"), debug);
		}

		/*! authenticaded operations goes here */
		if (chcp->auth) {
			credit_bucks = credit_suck(chcp);
			debug_memory(chcp->main_memory, debug);
			led_set(GREEN, ON);
		}

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

				debug_print_P(PSTR("Awake with bucks: "), debug);

				/*! Protect from unallocated memory */
				if (debug->active) {
					debug->string = itoa(credit_bucks, debug->string, 10);
					strcpy(debug->line, debug->string);
					strcat(debug->line, "\n");
					debug_print(debug);
				}

				/*! Don't go to sleep until
				 uart chars has been sent */
				if (debug->active)
					_delay_ms(200);
			}

			sleep_disable();
		}

		led_set(NONE, OFF);
	}
}

