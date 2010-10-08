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
#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

void led_set(const uint8_t led, const uint8_t status)
{
	switch (status) {
		case ON:
			led_set(NONE, OFF);

			if ((led == RED) || (led == BOTH))
				 LED_PORT |= _BV(LED_RED);

			if ((led == GREEN) || (led == BOTH))
				LED_PORT |= _BV(LED_GREEN);

			break;
		case BLINK:
			if (led == RED) {
				LED_DELAY;
				LED_PORT |= _BV(LED_RED);
				LED_DELAY;
			}

			if (led == GREEN) {
				LED_DELAY;
				LED_PORT |= _BV(LED_GREEN);
				LED_DELAY;
			}

			break;
		default:
			LED_PORT &= ~(_BV(LED_RED) | _BV(LED_GREEN));
	}
}

void led_init(void)
{
	LED_DDR |= (_BV(LED_RED) | _BV(LED_GREEN));
	led_set(NONE, OFF);
}

