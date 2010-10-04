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
  \file chcp.h
  \brief This is the API avariable to access to SLE4442.

  Details.
 */

#ifndef CHCP_H_
#define CHCP_H_

/*!
  \def CHCP_CMD_DUMP_MEMORY
  \brief SLE4442 Hex command to access the main memory.

  \def CHCP_CMD_DUMP_SECMEM
  \brief SLE4442 Hex command to access the security memory.

  \def CHCP_CMD_DUMP_PRT_MEMORY
  \brief SLE4442 Hex command to access the protected memory.
 */

#define CHCP_VERSION 0.1
#define CHCP_CMD_DUMP_MEMORY 0x30
#define CHCP_CMD_DUMP_SECMEM 0x31
#define CHCP_CMD_DUMP_PRT_MEMORY 0x34
#define CHCP_CMD_COMPARE_VERIFICATION_DATA 0x33
#define CHCP_CMD_UPDATE_SECMEM 0x39
#define CHCP_CMD_UPDATE_MEMORY 0x38

/* The top secret pin */
#define CHCP_PIN1 0xFF
#define CHCP_PIN2 0xFF
#define CHCP_PIN3 0xFF

/*!
  \struct chcp_t chcp.h "chcp.h"
  \brief The main struct which represent the status of the card.

  Keep in mind AtMega chips have only 1 KB RAM, dumping all the
  card memory into RAM may cause an out of memory.

  \var uint8_t *chcp_t::atr
  \brief ptr to a 4 bytes ATR header returned by the ATZ command.

  \var uint8_t *chcp_t::main_memory
  \brief ptr to a 256 bytes copy of the SLE 4442 memory.

  \var uint8_t *chcp_t::protected_memory
  \brief ptr to a 32 bytes copy of the SLE 4442 memory.

  \var uint8_t* chcp_t::security_memory
  \brief 4 bytes

  \var uint8_t* chcp_t::ck_proc
  \brief processing clock counts

  \var uint8_t chcp_t::card_present
  \brief card present 0=no; 1=yes

  \var uint8_t chcp_t::auth
  \brief card auth 1 - ok, 0 - writing forbidden

  auth ok, I can write

 */

struct chcp_t {
	uint8_t *atr;
	uint8_t *main_memory;
	uint8_t *protected_memory;
	uint8_t *security_memory;
	uint8_t *ck_proc;
	uint8_t card_present;
	uint8_t auth;
};

struct chcp_t* chcp_init(void);
void chcp_free(struct chcp_t *chcp);
void chcp_reset(uint8_t *atr);
uint8_t chcp_present(struct chcp_t *chcp);
void chcp_dump_memory(uint8_t *mm);
void chcp_dump_prt_memory(uint8_t *mm);
void chcp_dump_secmem(uint8_t *mm);
void chcp_auth(struct chcp_t *chcp, const uint8_t pin1, const uint8_t pin2, const uint8_t pin3);
void chcp_write_memory(struct chcp_t *chcp, const uint8_t addr, const uint8_t len);

#endif

