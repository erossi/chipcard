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
#include <string.h>
#include <avr/io.h>
#include "tools.h"

uint8_t check_sle_atr(struct sle_t *sle)
{
	/*! \bug should use eeprom area instead RAM */
	uint8_t atr[4] = {0xa2, 0x13, 0x10, 0x91};

	if (memcmp(sle->atr, &atr, 4))
		return(0);
	else
		return(1);
}

void chcp_gpio_set(const uint8_t io)
{
	if (io) {
		CHCP_GPIO_PORT &= ~_BV(CHCP_GPIO_0);
		CHCP_GPIO_PORT |= _BV(CHCP_GPIO_1);
	} else {
		CHCP_GPIO_PORT &= ~_BV(CHCP_GPIO_1);
		CHCP_GPIO_PORT |= _BV(CHCP_GPIO_0);
	}
}

void chcp_gpio_init(void)
{
	CHCP_GPIO_DDR |= _BV(CHCP_GPIO_0) | _BV(CHCP_GPIO_1);
	chcp_gpio_set(OFF);
}
