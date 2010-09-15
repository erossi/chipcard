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

struct chcp_t* chcp_init(void)
{
	struct chcp_t *chcp;

	/*
	   Inital PORT setup:
	   Card present 1 Pulled UP by micro
	   Reset 0
	   Clock 0
	   IO 1 Pulled UP when Reading
	 */

	CHPC_PORT = _BV(CHPC_PRESENT) | _BV(CHPC_IO);

	/*
	   Initial DDR setup:
	   Card Present - IN
	   Reset - OUT
	   Clock - OUT
	   IO - BiDirectional begin IN
	 */

	CHPC_DDR = _BV(CHPC_RST) | _BV(CHPC_CK);

	chcp = malloc(sizeof(struct chcp_t));
	chcp->atr = malloc(4);
	chcp->main_memory = malloc(256);
	chcp->protected_memory = malloc(32);
	chcp->security_memory = malloc(4);
	return(chcp);
}

void chcp_free(struct chcp_t *chcp)
{
	free(chcp->security_memory);
	free(chcp->protected_memory);
	free(chcp->main_memory);
	free(chcp->atr);
	free(chcp);
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

	return(chcp->card_present);
}

void chcp_dump_memory(uint8_t *mm) {
	int i;

	send_cmd(CHCP_CMD_DUMP_MEMORY, 0, 0);

	for (i=0; i<256; i++)
		*(mm+i) = read_byte();

	ck_pulse(); /* leave the card to high imp. I/O line */
}

void chcp_dump_prt_memory(uint8_t *mm) {
	uint8_t i;

	send_cmd(CHCP_CMD_DUMP_PRT_MEMORY, 0, 0);

	for (i=0; i<32; i++)
		*(mm+i) = read_byte();

	ck_pulse(); /* leave the card to high imp. I/O line */
}

void chcp_dump_secmem(uint8_t *mm) {
	uint8_t i;

	send_cmd(CHCP_CMD_DUMP_SECMEM, 0, 0);

	for (i=0; i<4; i++)
		*(mm+i) = read_byte();

	ck_pulse(); /* leave the card to high imp. I/O line */
}

