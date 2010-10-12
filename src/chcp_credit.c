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
#include "sle.h"
#include "pin.h"
#include "chcp_credit.h"

uint8_t credit_card_new(struct sle_t *sle)
{
	return(memcmp(sle->main_memory + 32, CHCP_SLE_CODE, 7));
}

uint8_t credit_check(struct sle_t *sle)
{
	uint8_t bucks;

	bucks = *(sle->main_memory + 40);

	if (bucks && !credit_card_new(sle))
		return(1);
	else
		return(0);
}

unsigned int credit_suck(struct sle_t *sle)
{
	uint8_t bucks;

	bucks = *(sle->main_memory + 40);

	/* clear bucks in the card */
	if (bucks) {
		*(sle->main_memory + 40) = 0;
		sle_write_memory(sle, 40, 1);
	}

	/*! Buck should not be > 145 */
	return(bucks * CREDIT_MULTIPLIER);
}

