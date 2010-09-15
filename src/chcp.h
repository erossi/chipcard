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

#ifndef CHCP_H_
#define CHCP_H_

#define CHCP_CMD_DUMP_MEMORY 0x30
#define CHCP_CMD_DUMP_SECMEM 0x31
#define CHCP_CMD_DUMP_PRT_MEMORY 0x34

/* Keep in mind AtMega chips have only 1 KB RAM */
struct chcp_t {
	uint8_t *atr; /* 4 bytes */
	uint8_t *main_memory; /* 256 bytes */
	uint8_t *protected_memory; /* 32 bytes */
	uint8_t *security_memory; /* 4 Bytes */
	uint8_t card_present;
};

struct chcp_t* chcp_init(void);
void chcp_free(struct chcp_t *chcp);
void chcp_reset(uint8_t *atr);
uint8_t chcp_present(struct chcp_t *chcp);
void chcp_dump_memory(uint8_t *mm);
void chcp_dump_prt_memory(uint8_t *mm);
void chcp_dump_secmem(uint8_t *mm);

#endif

