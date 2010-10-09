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
#include "chcp.h"
#include "chcp_credit.h"

uint8_t credit_check(struct chcp_t *chcp)
{
	int result;
	uint8_t bucks;

	result = memcmp(chcp->main_memory + 32, "CHARLIE", 7);
	bucks = *(chcp->main_memory + 40);

	if (bucks && !result)
		return(1);
	else
		return(0);
}

unsigned int credit_suck(struct chcp_t *chcp)
{
	uint8_t bucks;

	bucks = *(chcp->main_memory + 40);

	/* clear bucks in the card */
	if (bucks) {
		*(chcp->main_memory + 40) = 0;
		chcp_write_memory(chcp, 40, 1);
	}

	/*! Buck should not be > 145 */
	return(bucks * CREDIT_MULTIPLIER);
}

