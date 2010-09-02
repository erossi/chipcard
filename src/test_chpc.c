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

#define CHPC_PORT PORTD
#define CHPC_PIN PIND
#define CHPC_DDR DDRD
#define CHPC_PRESENT 0
#define CHPC_RST 4
#define CHPC_CK 6
#define CHPC_IO 7
#define OUT 1
#define IN 0
/* 10Khz = 100usec */
#define CHPC_USEC 100
#define ck_delay() _delay_us(100)

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

/* In or OUT */
void set_io(const uint8_t io)
{
	if (io) {
		CHPC_PORT &= ~(1<<CHPC_IO);
		CHPC_DDR |= (1<<CHPC_IO);
	} else {
		CHPC_PORT |= (1<<CHPC_IO);
		CHPC_DDR &= ~(1<<CHPC_IO);
	}
}

/* 1 or 0 */
void set_rst(const uint8_t rst)
{
	if (rst)
		CHPC_PORT |= (1<<CHPC_RST);
	else
		CHPC_PORT &= ~(1<<CHPC_RST);

}

/* 1 or 0 */
void set_ck(const uint8_t ck)
{
	if (ck)
		CHPC_PORT |= (1<<CHPC_CK);
	else
		CHPC_PORT &= ~(1<<CHPC_CK);

}

uint8_t read_byte(void)
{
}

void send_rst(void)
{
	uint8_t i;

	set_io(IN);
	set_rst(1);
	set_ck(1);
	ck_delay();
	set_ck(0);
	set_rst(0);
	ck_delay();

	for (i=0; i<32; i++) {
		/* read bit in */
		set_ck(1);
		ck_delay();
		set_ck(0);
		ck_delay();
	}

}

void init(void)
{
		/*
		   Inital PORT setup:
		   Card present 1 Pulled UP by micro
		   Reset 0
		   Clock 0
		   IO 1 Pulled UP when Reading
		   */
		
		CHPC_PORT = (1 << CHPC_PRESENT) | (1 << CHPC_IO);

		/*
		   Initial DDR setup:
		   Card Present - IN
		   Reset - OUT
		   Clock - OUT
		   IO - BiDirectional begin IN
		   */

		CHPC_DDR = (1 << CHPC_RST) | (1 << CHPC_CK);
}

int main(void)
{
	*char atr;

	init();

	DDRC = 1; /* PC0 OUT */
	PORTC = 1; /* PC0 led off */

	atr = malloc(4);

	for (;;) {
		loop_until_bit_is_set(CHPC_PIN, CHPC_PRESENT);
		PORTC=0;
		_delay_ms(1000);

		send_rst(atr);

		loop_until_bit_is_clear(CHPC_PIN, CHPC_PRESENT);
		PORTC=1;
		_delay_ms(1000);
	}

	free(atr);
}

/* vim: set nu cindent ts=4 sw=4: */
