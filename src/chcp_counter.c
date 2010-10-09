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

/*!
  \file chcp_counter.c
  \brief Counters managemet and IRQ routine.

  Counter setups, counting and IRQ.
  */

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "chcp.h"
#include "chcp_counter.h"

/*!
  IRQ wakes up on the timer oveflow and decrement the
  remaining credit.
 */
ISR(TIMER2_OVF_vect)
{
	credit_bucks--;
}

void counter_setup(void)
{
	ASSR = _BV(AS2);
	TCCR2 = 0;

	/*! enable interrupt on timer 2 overflow */
	TIMSK = _BV(TOIE2);
}

void counter_start(void)
{
	/*! counter prescaler 1024 */
	TCCR2 = _BV(CS22) | _BV(CS21) | _BV(CS20);
	loop_until_bit_is_clear(ASSR, TCR2UB);
}

void counter_stop(void)
{
	TCCR2 = 0;
	loop_until_bit_is_clear(ASSR, TCR2UB);
}

