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
  \file chcp_counter.h

  Counter setups, counting and IRQ.
  */

#ifndef CHCP_COUNTER_H
#define CHCP_COUNTER_H

#include "sle.h"

/*!
  Global used in interrupt.
  This rapprensent the numner of cycle the counter has
  todo before stop the TV.
  Since the credit is in number of hours, and a cycle is 8 sec long,
  this number can be at maximum:
  255h * 450 = 114750 which is far beyond max int!
  thus the maximum number of hour loadable in the card can be (rounded):
  65535/450 is 145h about 6 days.
 */
volatile unsigned int credit_bucks;

void counter_setup(void);
void counter_start(void);
void counter_stop(void);

#endif

