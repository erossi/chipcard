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

/*! \file master.c
  \brief Device works in writing/recharge mode.
  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "master.h"

/*! Initialize a new card auth with default pin, usually set to 0xff,
  with my secret pins and with my 7 chars code.
  The "code" will permit me to non-authenticate a non-pre-initialized
  card failing the auth.  
  */
static void card_init(struct sle_t *sle, struct debug_t *debug)
{
	debug_print_P(PSTR("\n New card! Init the card? (y/N): "), debug);

	/*! Infinite loop until y is pressed */
	while (!debug_wait_for_y(debug));

	/*! Do default auth */
	sle_auth(sle, CHCP_DEF_PIN1, CHCP_DEF_PIN2, CHCP_DEF_PIN3);
	debug_proc_counts(sle->ck_proc, debug);
	debug_secmem(sle->security_memory, debug);
	debug_print_P(PSTR("\n Write the secret PIN? (y/N): "), debug);

	/*! Infinite loop until y is pressed */
	while (!debug_wait_for_y(debug));

	*(sle->security_memory + 1) = CHCP_PIN1;
	*(sle->security_memory + 2) = CHCP_PIN2;
	*(sle->security_memory + 3) = CHCP_PIN3;
	sle_write_secmem(sle);
	debug_proc_counts(sle->ck_proc, debug);
	debug_secmem(sle->security_memory, debug);
	debug_print_P(PSTR("\n New PINs has been written.\n"), debug);
}

/*! Write an auth card with a precompiled recharge value to a
  precompiled memory position */
static void recharge(struct sle_t *sle, struct debug_t *debug)
{
	debug_print_P(PSTR("\n Recharging cards: "), debug);
	memcpy(sle->main_memory + 32, CHCP_SLE_CODE, 7);
	*(sle->main_memory + 40) = RECHARGE_BUCKS;
	sle_write_memory(sle, 32, 9);
	led_set(GREEN, ON);
	debug_print_P(PSTR("done!\n"), debug);
	debug_memory(sle->main_memory, debug);
}

void master(struct sle_t *sle, struct debug_t *debug)
{
	debug_print_P(PSTR("Master mode\n"), debug);

	for (;;) {
		/*! \bug should SLEEP if power is missing */
		while (!sle_present(sle))
			_delay_ms(1000);

		sle->auth = 0; /* Default to non-auth */
		led_set(RED, ON);
		_delay_ms(500);
		sle_reset(sle->atr);

		/*! check the correct SLE4442 atr */
		if (check_sle_atr(sle)) {
			/* I don't need to dump the entire memory */
			sle_dump_allmem(sle);
			debug_atr(sle->atr, debug);
			debug_prt_memory(sle->protected_memory, debug);
			debug_secmem(sle->security_memory, debug);
			debug_memory(sle->main_memory, debug);

			/*! If the card is new */
			if (credit_card_new(sle))
				card_init(sle, debug);
			else {
				debug_print_P(PSTR("\n Initialized card! Auth? (press y): "), debug);
				/*! Infinite loop until y is pressed 
				    only if debug is active */
				while (!debug_wait_for_y(debug));

				/* Do auth */
				sle_auth(sle, CHCP_PIN1, CHCP_PIN2, CHCP_PIN3);
				debug_proc_counts(sle->ck_proc, debug);
				debug_secmem(sle->security_memory, debug);
			}
		}

		/*! auth op goes here */
		if (sle->auth) {
			recharge(sle, debug);
		}

		debug_print_P(PSTR("Remove the card!\n"), debug);

		/*! loop until the card remain inserted */
		while (sle_present(sle)) {
			if (!sle->auth)
				led_set(RED, BLINK);

			_delay_ms(500);
		}

		led_set(NONE, OFF);
	}
}

