/*
 * plus4parallel.c - Parallel cable handling for the Plus4.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "vice.h"

#include "drive.h"
#include "drivetypes.h"
#include "iecdrive.h"
#include "maincpu.h"
#include "plus4parallel.h"
#include "ted.h"
#include "types.h"

static uint8_t parallel_cable_cpu_value = 0xff;
static uint8_t parallel_cable_drive_value[NUM_DISK_UNITS] = { 0xff, 0xff, 0xff, 0xff };

void parallel_cable_drive_write(int port, uint8_t data, int handshake, unsigned int dnr)
{
    parallel_cable_drive_value[dnr] = data;
}

uint8_t parallel_cable_drive_read(int type, int handshake)
{
    return parallel_cable_cpu_value & parallel_cable_drive_value[0] & parallel_cable_drive_value[1];
}

void parallel_cable_cpu_write(int type, uint8_t data)
{
    if (!(diskunit_context[0]->enable)
        && !(diskunit_context[1]->enable)) {
        return;
    }

    drive_cpu_execute_all(last_write_cycle);

    parallel_cable_cpu_value = data;
}

uint8_t parallel_cable_cpu_read(int type, uint8_t data)
{
    if (!(diskunit_context[0]->enable)
        && !(diskunit_context[1]->enable)) {
        return 0;
    }

    drive_cpu_execute_all(maincpu_clk);

    return data & (parallel_cable_cpu_value & parallel_cable_drive_value[0] & parallel_cable_drive_value[1]);
}

void parallel_cable_cpu_undump(int type, uint8_t data)
{
    parallel_cable_cpu_value = data;
}
