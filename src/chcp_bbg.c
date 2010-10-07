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
  \file chcp_bbg.c
  \brief Bit banging routine to SLE4442.

  Low level bit banging driver to read and write to
  Sle4442 card.
  */

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "chcp_bbg.h"

/*! single clock pulse */
void ck_pulse(void)
{
	set_ck_1;
	ck_delay();
	set_ck_0;
	ck_delay();
}

/*! Set's I/O line to IN, OUT, 0 or 1.
  \details When in In mode the 0 or 1 command internal pull up resistor.
  \param[in] io 0 - 3 = 0, 1, IN, OUT
 */
void set_io(const uint8_t io)
{
	switch (io) {
		case 0:
			CHPC_PORT &= ~_BV(CHPC_IO);
			break;
		case 1:
			CHPC_PORT |= _BV(CHPC_IO);
			break;
		case OUT:
			CHPC_DDR |= _BV(CHPC_IO);
			break;
		default:
			CHPC_PORT |= _BV(CHPC_IO);
			CHPC_DDR &= ~_BV(CHPC_IO);
	}
}

/*! Send the START sequence.
  \warning This will also sets IO to OUT.
 */
void send_start(void)
{
	set_ck_0; /* redundancy */
	set_io(OUT);
	set_io(1);
	set_ck_1;
	ck_delay();
	set_io(0);
	ck_delay_front();
	set_ck_0;
	ck_delay();
}

/*! Send the STOP sequence.
  \warinig This sets IO from OUT to IN
 */
void send_stop(void)
{
	set_io(0);
	ck_delay_front();
	set_ck_1;
	ck_delay_front();
	set_io(IN); /* with pull up IO goes 1 */
	ck_delay();
	set_ck_0;
	ck_delay();
}

/*! read io bit on the 1 phase of the ck line */
uint8_t read_byte(void)
{
	uint8_t i;
	uint8_t byte=0;

	for (i=0;i<8;i++) {
		set_ck_1;

		if (CHPC_PIN & _BV(CHPC_IO))
			byte |= _BV(i);

		ck_delay();
		set_ck_0;
		ck_delay();
	}

	return(byte);
}

/*! write io bit on the 0 phase of the ck line */
void send_byte(uint8_t byte)
{
	uint8_t i;

	for (i=0; i<8; i++) {
		if (byte & _BV(i))
			set_io(1);
		else
			set_io(0);

		ck_delay_front();
		set_ck_1;
		ck_delay();
		set_ck_0;
		ck_delay();
	}
}

/*! send the rst sequence to the card */
void send_rst(uint8_t *atr)
{
	uint8_t i;

	set_io(IN);
	set_rst_1;
	ck_delay_front();
	set_ck_1;
	ck_delay_reset();
	set_ck_0;
	ck_delay_front();
	set_rst_0;
	ck_delay();

	for (i=0; i<4; i++) {
		/* read bit in */
		*(atr+i) = read_byte();
	}
}

/*! send a complete command sequence.
  \warning After a command IO in IN.
 */
void send_cmd(const uint8_t control, const uint8_t address, const uint8_t data)
{
	send_start();
	send_byte(control);
	send_byte(address);
	send_byte(data);
	send_stop();
}

/*! wait for the card to process the command.
  \return the number of clock cycle waited.
 */
uint8_t processing(void)
{
	uint8_t i = 0;

	while (!(CHPC_PIN & _BV(CHPC_IO))) {
		ck_pulse();
		i++;
	}

	ck_pulse();
	return (i);
}

