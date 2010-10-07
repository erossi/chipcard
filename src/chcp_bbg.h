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
  \file chcp_bbg.h
  \brief Bit banging routine to SLE4442.

  Low level bit banging driver to read and write to
  Sle4442 card.

   green led
   red led
   chcp present
   chcp reset
   chcp alim
   chcp ck
   chcp io

   rtc PC6/7
   rs232 PD0/1
   in circuit prg pb567
 */

#ifndef CHCP_BBG_H
#define CHCP_BBG_H

/*!
  \def CHPC_PORT
  Port the card's reader is conneted to.

  \def CHPC_PRESENT
  PIN connected to the present card switch

  \def CHPC_RST
  PIN connected to the reset pin of the card.

  \def CHPC_CK
  clock pin of the card

  \def CHPC_IO
  I/O pin of the card.
*/

#define CHPC_PORT PORTA
#define CHPC_PIN PINA
#define CHPC_DDR DDRA
#define CHPC_PRESENT 0
#define CHPC_RST 4
#define CHPC_CK 6
#define CHPC_IO 7
#define OUT 2
#define IN 3

/*!
  \def ck_delay()
  The delay in usec for half of the wave.
  10Khz = 100usec
  20Khz = 50usec, half wave = 25 usec
  Remeber delay is only HALF of the freq.
  20Khz total

  \def ck_delay_front()
  The MIN delay for a front phase between fronts of ck
  and/or rst and/or IO

  \def ck_delay_reset()
  delay reset = 1.
  \def set_ck_1
  CK line to 1.
  \def set_ck_0
  CK line to 0.
  \def set_rst_1
  RST line to 1.
  \def set_rst_0
  RST line to 0.
 */

#define ck_delay() _delay_us(25)
#define ck_delay_front() _delay_us(4)
#define ck_delay_reset() _delay_us(50)
#define set_ck_1 CHPC_PORT |= (1<<CHPC_CK)
#define set_ck_0 CHPC_PORT &= ~(1<<CHPC_CK)
#define set_rst_1 CHPC_PORT |= (1<<CHPC_RST)
#define set_rst_0 CHPC_PORT &= ~(1<<CHPC_RST)

void ck_pulse(void);
void set_io(const uint8_t io);
uint8_t read_byte(void);
void send_byte(uint8_t byte);
void send_rst(uint8_t *atr);
void send_cmd(const uint8_t control, const uint8_t address, const uint8_t data);
uint8_t processing(void);

#endif

