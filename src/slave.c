/* This file is part of chpc
 * Copyright (C) 2010, 2011 Enrico Rossi
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
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "slave.h"

static void auth_stuff(struct sle_t *sle, struct debug_t *debug)
{
	/* set_sleep_mode(SLEEP_MODE_EXT_STANDBY); */
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sle_disable_port();
	sleep_enable();
	/* sleep_bod_disable(); */
	/* IRQ must be enabled to decrement credit_bucks */

	if (debug->active) {
		while (credit_bucks) {
			counter_start();
			sleep_cpu();
			counter_stop();
			debug_print_P(PSTR("Awake with bucks: "), debug);
			debug->string = itoa(credit_bucks, debug->string, 10);
			strcpy(debug->line, debug->string);
			strcat(debug->line, "\n");
			debug_print(debug);
			/*! Don't go to sleep until uart chars has been sent */
			_delay_ms(200);
		}
	} else {
		counter_start();

		while (credit_bucks) {
			cli();
			sleep_bod_disable();
			sei();
			sleep_cpu();
		}

		counter_stop();
	}

	sleep_disable();
	sle_enable_port();
}

void slave(struct sle_t *sle, struct debug_t *debug)
{
	debug_print_P(PSTR("Slave mode\n"), debug);
	chcp_gpio_init();

	for (;;) {
		/*! \bug SLEEP if power is missing */
		while (!sle_present(sle))
			_delay_ms(1000);

		/*! Default to non-auth */
		sle->auth = 0;
		led_set(RED, ON);
		/*! Wait between card insertion and begin */
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

			/*! The card is valid and have credits */
			if (credit_check(sle)) {
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
				sle_auth(sle, CHCP_PIN1, CHCP_PIN2, CHCP_PIN3);
				debug_proc_counts(sle->ck_proc, debug);
				debug_secmem(sle->security_memory, debug);
			} else
				debug_print_P(PSTR("\n Error! - Non initialized card!\n"), debug);
		}

		/*! auth op: 1st get credit */
		if (sle->auth) {
			credit_bucks = credit_suck(sle);
			debug_memory(sle->main_memory, debug);
			led_set(GREEN, ON);
			chcp_gpio_set(ON);
		}

		debug_print_P(PSTR("Now you can remove the card!\n"), debug);

		/*! auth op: 2nd do the counting */
		if (sle->auth)
			auth_stuff(sle, debug);
		else
			while (sle_present(sle))
				led_set(RED, BLINK);

		led_set(NONE, OFF);
		chcp_gpio_set(OFF);
	}
}
