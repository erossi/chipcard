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
#include "chcp_i2c.h"
#include "chcp.h"

void chcp_init_io(void)
{
	/*
	   Inital PORT setup:
	   Card present 1 Pulled UP by micro
	   Reset 0
	   Clock 0
	   IO 1 Pulled UP when Reading
	 */

	CHPC_PORT = (1 << CHPC_PRESENT) | (1 << CHPC_IO);

	/*
	   Initial DDR setup:
	   Card Present - IN
	   Reset - OUT
	   Clock - OUT
	   IO - BiDirectional begin IN
	 */

	CHPC_DDR = (1 << CHPC_RST) | (1 << CHPC_CK);
}

void chcp_reset(uint8_t *atr) {
	send_rst(atr);
}

/* return 0 = card not preset or some other integer */
uint8_t chcp_present(struct chcp_t *chcp) {

	if (CHPC_PIN & _BV(CHPC_PRESENT))
		chcp->card_present=1;
	else
		chcp->card_present=0;

	return (chcp->card_present);
}

void chcp_dump_memory(uint8_t *mm) {
	uint8_t i = 0;

	send_cmd(CHCP_CMD_DUMP_MEMORY, 0, 0);

	do {
		*(mm+i) = read_byte();
		i++;
	} while (i);

	ck_pulse(); /* leave the card to high imp. I/O line */
}

void chcp_dump_prt_memory(uint8_t *mm) {
	uint8_t i;

	send_cmd(CHCP_CMD_DUMP_PRT_MEMORY, 0, 0);
	set_io(IN);

	for (i=0; i<32; i++)
		*(mm+i) = read_byte();

	ck_pulse(); /* leave the card to high imp. I/O line */
}

void chcp_dump_secmem(uint8_t *mm) {
	uint8_t i;

	send_cmd(CHCP_CMD_DUMP_SECMEM, 0, 0);
	set_io(IN);

	for (i=0; i<4; i++)
		*(mm+i) = read_byte();

	ck_pulse(); /* leave the card to high imp. I/O line */
}

