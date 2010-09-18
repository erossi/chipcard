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
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "chcp.h"
#include "uart.h"
#include "print_uart.h"

/* Global used in interrupt */
int credit_bucks;

ISR(TIMER1_OVF_vect)
{
	/*
	   Weaking up from sleep, decrement credit by 1
	   */
	credit_bucks--;
}

uint8_t credit_check(struct chcp_t *chcp)
{
	int result;
	uint8_t bucks;

	result = memcmp(chcp->main_memory + 32, "CHARLIE", 7);
	bucks = *(chcp->main_memory + 40);

	if (bucks && !result)
		return(1);
	else
		return(0);
}

uint8_t credit_suck(struct chcp_t *chcp)
{
	uint8_t bucks;

	bucks = *(chcp->main_memory + 40);

	/* clear bucks in the card */
	if (bucks) {
		*(chcp->main_memory + 40) = 0;
		chcp_write_memory(chcp, 40, 1);
	}

	return(bucks);
}

void counter_setup(void)
{
	TCCR1A = 0;
	TCCR1B = 0;

	/* enable interrupt on timer 1 overflow
	   use include deprecated.h to use the function */
	TIMSK = _BV(TOIE1);
}

void counter_start(void)
{
	/* counter prescaler 1024 */
	TCCR1B = _BV(CS12) | _BV(CS10);
}

void counter_stop(void)
{
	TCCR1B = 0;
}

void mastermind(struct chcp_t *chcp, char *line, char *string)
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

void poor_slave(struct chcp_t *chcp, char *line, char *string)
{
	strcpy_P (line, PSTR("Slave mode\n"));
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
				strcpy_P (line, PSTR("\n Press 7 to auth \n"));
				uart_printstr (line);

				loop_until_bit_is_clear(PINC, 7);

				/* Do auth */
				chcp_auth(chcp, CHCP_PIN1, CHCP_PIN2, CHCP_PIN3);
				print_proc_counts(chcp->ck_proc, line, string);
				print_secmem(chcp->security_memory, line, string);
			} else {
				strcpy_P (line, PSTR("\n Error! - Non initialized card!\n"));
				uart_printstr (line);
			}
		}

		if (chcp->auth) {
			/* authenticaded operations */
			credit_bucks = credit_suck(chcp);
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

		/* test the counter */
		sleep_enable();

		while (credit_bucks) {
			/* start the counter */
			counter_start();
			/* sleep */
			sleep_cpu();
			/* awakening */
			/* stop the counter */
			counter_stop();
			strcpy_P (line, PSTR("Awake with bucks: "));
			string = itoa(credit_bucks, string, 10);
			strcat (line, string);
			uart_printstr (line);
			uart_putchar ('\n');
			_delay_ms(200);
		}

		sleep_disable();

		PORTC = 3; /* leds off */
	}
}

int main(void)
{
	struct chcp_t *chcp;
	char *line;
	char *string;

	chcp = chcp_init();
	uart_init();

	DDRC = 3; /* PC0 - RED and PC1 - GREEN OUT */
	PORTC = 3; /* leds off */

	counter_setup();
	/*
	set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
	*/
	set_sleep_mode(SLEEP_MODE_IDLE);

	sei();

	line = malloc(80);
	string = malloc(20);

	strcpy_P (line, PSTR("Chipcard SLE4442 rel: CHCP_VERSION \n\n"));
	uart_printstr (line);

	if (PINC & _BV(7))
		poor_slave(chcp, line, string);
	else
		mastermind(chcp, line, string);

	cli();
	chcp_free(chcp);
	free(line);
	free(string);
	return(0);
}

