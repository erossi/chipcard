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

#ifndef CHCP_CREDIT_H
#define CHCP_CREDIT_H

/*!
This is the multiplier of the credit stored in the card.
for example 2 bucks in the card become 900 in the memory.
900 cycle each of 8 sec is a total of:
900*8s = 7200s (2h).
 */
#define CREDIT_MULTIPLIER 450

uint8_t credit_check(struct chcp_t *chcp);
unsigned int credit_suck(struct chcp_t *chcp);

#endif

