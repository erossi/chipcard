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

#define CHPC_PORT PORTA
#define CHPC_PIN PINA
#define CHPC_DDR DDRA
#define CHPC_PRESENT 0
#define CHPC_RST 4
#define CHPC_CK 6
#define CHPC_IO 7
#define OUT 2
#define IN 3
/* 10Khz = 100usec */
#define CHPC_USEC 100

/* Macro */
#define ck_delay() _delay_us(100)
#define set_ck_1 CHPC_PORT |= (1<<CHPC_CK)
#define set_ck_0 CHPC_PORT &= ~(1<<CHPC_CK)
#define set_rst_1 CHPC_PORT |= (1<<CHPC_RST)
#define set_rst_0 CHPC_PORT &= ~(1<<CHPC_RST)

void set_io(const uint8_t io);
/*
   void set_rst(const uint8_t rst);
   void set_ck(const uint8_t ck);
 */
uint8_t read_byte(void);
void send_byte(uint8_t byte);
void send_rst(uint8_t *atr);

