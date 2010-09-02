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

#define CHPC_PORT PORTC
#define CHPC_PIN PINC
#define CHPC_DDR DDRC
#define CHPC_PRESENT 0
#define CHPC_RST 4
#define CHPC_CK 6
#define CHPC_IO 7

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
		/*
		   PortC the chip card reader, PortD Leds
		 */

		CHPC_DDR = 0;
		CHPC_PORT = 0xff; /* PULL UP */
		DDRD = 0xff; /* All OUT */
		PORTD = 0xff; /* All leds off */

		for (;;) {
				loop_until_bit_is_set(CHPC_PIN, CHPC_PRESENT);
				PORTD=0;
				_delay_ms(1000);
				loop_until_bit_is_clear(CHPC_PIN, CHPC_PRESENT);
				PORTD=0xff;
				_delay_ms(1000);
		}

}
