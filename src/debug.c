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
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "chcp.h"
#include "uart.h"
#include "debug.h"

void debug_print_P(PGM_P string, struct debug_t *debug)
{
	strcpy_P(debug->line, string);
	uart_printstr(debug->line);
}

void debug_print(struct debug_t *debug)
{
	uart_printstr(debug->line);
}

static void hello(struct debug_t *debug)
{
        debug_print_P(PSTR("\n\n\n"), debug);
        debug_print_P(PSTR("Chipcard SLE4442 Rel: "), debug);
        debug_print_P(PSTR(GITREL), debug);
        debug_print_P(PSTR("\n\n"), debug);
        debug_print_P(PSTR("Andrea Marabini <info@marabo.it>\n"), debug);
        debug_print_P(PSTR("Enrico Rossi <e.rossi@tecnobrain.com>\n"), debug);
        debug_print_P(PSTR("URL: http://tecnobrain.com/\n"), debug);
        debug_print_P(PSTR("GNU GPL v3 - use at your own risk!\n"), debug);
        debug_print_P(PSTR("\n\n"), debug);
}

static uint8_t ask_activate(void)
{
	return(1);
}

static void dbg_mem(uint8_t *mm, const int max, struct debug_t *debug)
{
	int i=0;
	uint8_t j;

	while (i<max) {
		strcpy_P (debug->line, PSTR("Byte ["));
		debug->string = utoa(i, debug->string, 16);
		strcat (debug->line, debug->string);
		strcpy_P (debug->string, PSTR(" - "));
		strcat (debug->line, debug->string);
		debug->string = utoa(i + PRINT_VALUE_X_LINE - 1, debug->string, 16);
		strcat (debug->line, debug->string);
		strcpy_P (debug->string, PSTR("]: "));
		strcat (debug->line, debug->string);

		for (j=0; j < PRINT_VALUE_X_LINE; j++)
			if ((i+j) < max) {
				debug->string = utoa(*(mm+i+j), debug->string, 16);
				strcat (debug->line, debug->string);
				strcpy_P (debug->string, PSTR(" "));
				strcat (debug->line, debug->string);
			}

		uart_printstr (debug->line);
		uart_putchar ('\n');
		i += j;
	}

	uart_putchar ('\n');
}

void debug_atr(uint8_t *atr, struct debug_t *debug)
{
	debug_print_P(PSTR("-> ATR Bytes ----\n"), debug);
	dbg_mem(atr, 4, debug);
}

void debug_memory(uint8_t *mm, struct debug_t *debug)
{
	debug_print_P(PSTR("-> Main Memory Bytes ----\n"), debug);
	dbg_mem(mm, 256, debug);
}

void debug_prt_memory(uint8_t *pm, struct debug_t *debug)
{
	debug_print_P(PSTR("-> Protected Memory Bytes ----\n"), debug);
	dbg_mem(pm, 4, debug);
}

void debug_secmem(uint8_t *sm, struct debug_t *debug)
{
	debug_print_P(PSTR("-> Security Memory Bytes ----\n"), debug);
	dbg_mem(sm, 4, debug);
}

void debug_proc_counts(uint8_t *pc, struct debug_t *debug)
{
	debug_print_P(PSTR("-> Auth Proccessing times ----\n"), debug);
	dbg_mem(pc, 5, debug);
}

struct debug_t *debug_init(void)
{
	struct debug_t *debug;

	uart_init();
	debug = malloc(sizeof(struct debug_t));
	debug->active = ask_activate();

	if (debug->active) {
		debug->line = malloc(MAX_LINE_LENGHT);
		debug->string = malloc(MAX_STRING_LENGHT);
		hello(debug);
	}

	return(debug);
}

void debug_free(struct debug_t *debug)
{
	if (debug->active) {
		free(debug->string);
		free(debug->line);
	}

	free(debug);
}

