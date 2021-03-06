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

/*! \file main.c
  \brief Main.
 */

#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "sle.h"
#include "debug.h"
#include "led.h"
#include "master.h"
#include "slave.h"
#include "counter.h"

int main(void)
{
	struct sle_t *sle;
	struct debug_t *debug;

	/* Init sequence, turn on both led */
	led_init();
	sle = sle_init();
	debug = debug_init();
	counter_setup();
	led_set(BOTH, OFF);

	sei();

#ifdef MASTERSLAVE
	debug_print_P(PSTR("\nMaster mode? (y/N): "), debug);

	/*! by default go into slave mode */
	if (debug->active && debug_wait_for_y(debug))
		master(sle, debug);
	else
		slave(sle, debug);
#endif

#ifdef MASTERONLY
	master(sle, debug);
#endif

#ifdef SLAVEONLY
	slave(sle, debug);
#endif

	cli();
	debug_free(debug);
	sle_free(sle);
	return(0);
}

