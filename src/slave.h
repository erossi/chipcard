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

/*! \file slave.h
  more info needed */

#ifndef CHCP_SLAVE_H
#define CHCP_SLAVE_H

#include "sle.h"
#include "pin.h"
#include "debug.h"
#include "tools.h"
#include "led.h"
#include "credit.h"
#include "counter.h"

void slave(struct sle_t *sle, struct debug_t *debug);

#endif
