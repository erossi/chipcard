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
#include "chcp_bbg.h"
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
	chcp->protected_memory = malloc(4);
	chcp->security_memory = malloc(4);
	/* Normally we should use only 1, but during the auth
	   it's wise to keep all the 5 processing result */
	chcp->ck_proc = malloc(5);
	return(chcp);
}

void chcp_free(struct chcp_t *chcp)
{
	free(chcp->ck_proc);
	free(chcp->security_memory);
	free(chcp->protected_memory);
	free(chcp->main_memory);
	free(chcp->atr);
	free(chcp);
}

void chcp_reset(uint8_t *atr) {
	send_rst(atr);
}

/*! \brief Check for the presence of the card. */
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

	for (i=0; i<4; i++)
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

void chcp_auth(struct chcp_t *chcp, const uint8_t pin1, const uint8_t pin2, const uint8_t pin3)
{
	/* dump secmem */
	chcp_dump_secmem(chcp->security_memory);

	/* Check error = 7, all available error bit = 1 */
	if (*(chcp->security_memory) == 7) {

		/* write 0 to bit 3 */
		send_cmd(CHCP_CMD_UPDATE_SECMEM, 0, 3);
		*(chcp->ck_proc) = processing();

		/* Compare 3 byte PIN */
		send_cmd(CHCP_CMD_COMPARE_VERIFICATION_DATA, 1, pin1);
		*(chcp->ck_proc + 1) = processing();
		send_cmd(CHCP_CMD_COMPARE_VERIFICATION_DATA, 2, pin2);
		*(chcp->ck_proc + 2) = processing();
		send_cmd(CHCP_CMD_COMPARE_VERIFICATION_DATA, 3, pin3);
		*(chcp->ck_proc + 3) = processing();

		/* write 0xff to error */
		send_cmd(CHCP_CMD_UPDATE_SECMEM, 0, 0xff);
		*(chcp->ck_proc + 4) = processing();

		/* redump secmem */
		chcp_dump_secmem(chcp->security_memory);
	}

	/* if error = 7 then auth is OK */
	if (*(chcp->security_memory) == 7)
		chcp->auth = 1;
}

void chcp_write_memory(struct chcp_t *chcp, const uint8_t base, const uint8_t len)
{
	uint8_t i, addr;

	if (chcp->auth) {
		for (i=0; i<len; i++) {
			addr = base + i;
		send_cmd(CHCP_CMD_UPDATE_MEMORY, addr, *(chcp->main_memory + addr));
		*(chcp->ck_proc) = processing();
		}
	}
}

