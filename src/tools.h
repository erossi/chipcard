/* This file is part of chpc
 * Copyright (C) 2010, 2011 Enrico Rossi
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

/*! \file tools.h
  \brief General utility.
  */

#ifndef CHPC_TOOLS_H
#define CHPC_TOOLS_H

#include "sle.h"

#define CHCP_GPIO_PORT PORTA
#define CHCP_GPIO_DDR DDRA
#define CHCP_GPIO_0 PA4
#define CHCP_GPIO_1 PA6
#define OFF 0
#define ON 1

uint8_t check_sle_atr(struct sle_t *sle);
void chcp_gpio_init(void);
void chcp_gpio_set(const uint8_t io);

#endif
