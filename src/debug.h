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

/*! \file debug.h
  \brief Functions to activare and display (rs232) debug.
  */

#ifndef CHCP_DEBUG_H
#define CHCP_DEBUG_H

#define QUOTEME_(x) #x
#define QUOTEME(x) QUOTEME_(x)

/*! Maximum number of char a line can be */
#define MAX_LINE_LENGHT 80

/*! Number of char a substring can be made of.
Substrings are used to compose the line when, for example,
conversions have to be made from int to string etc. */
#define MAX_STRING_LENGHT 20

/*! The main debug structure, it has to be allocated,
  eventually, if debug is not active, you can avoid the
  allocation of the two members char *.
  */
struct debug_t {
	char *line;
	char *string;
	uint8_t active;
};

void debug_print_P(PGM_P string, struct debug_t *debug);
void debug_print(struct debug_t *debug);
void debug_atr(uint8_t *atr, struct debug_t *debug);
void debug_memory(uint8_t *mm, struct debug_t *debug);
void debug_prt_memory(uint8_t *pm, struct debug_t *debug);
void debug_secmem(uint8_t *sm, struct debug_t *debug);
void debug_proc_counts(uint8_t *pc, struct debug_t *debug);
struct debug_t *debug_init(void);
void debug_free(struct debug_t *debug);

#endif

