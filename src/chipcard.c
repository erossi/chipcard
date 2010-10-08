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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include "chcp.h"
#include "chcp_counter.h"
#include "debug.h"
#include "chcp_master.h"
#include "chcp_slave.h"

extern int credit_bucks;

int main(void)
{
	struct chcp_t *chcp;
	struct debug_t *debug;

	chcp = chcp_init();
	debug = debug_init();

	DDRC = 3; /* PC0 - RED and PC1 - GREEN OUT */
	PORTC = 3; /* leds off */

	counter_setup();
	/*
	set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
	*/
	set_sleep_mode(SLEEP_MODE_IDLE);

	sei();

	if (PINC & _BV(7))
		slave(chcp, debug);
	else
		master(chcp, debug);

	cli();
	debug_free(debug);
	chcp_free(chcp);
	return(0);
}

