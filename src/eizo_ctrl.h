#ifndef EZMON_EIZO_CTRL
#define EZMON_EIZO_CTRL

/*
   Copyright (C) 2014 Xfennec, CQFD Corp.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "hidapi.h"

#define EM_EIZO_FG_2421_VENDORID        0x056d
#define EM_EIZO_FG_2421_PRODUCTID       0x0002

#define EM_EIZO_FG_2421_CMD_MODE        0x03
#define EM_EIZO_FG_2421_CMD_VOLUME      0x36
#define EM_EIZO_FG_2421_CMD_BACKLIGHT   0x28

#define EM_EIZO_FG_2421_MODE_USER1      0x16
#define EM_EIZO_FG_2421_MODE_USER2      0x17
#define EM_EIZO_FG_2421_MODE_USER3      0x18
#define EM_EIZO_FG_2421_MODE_FPS1       0x28
#define EM_EIZO_FG_2421_MODE_FPS2       0x29
#define EM_EIZO_FG_2421_MODE_RTS        0x2a
#define EM_EIZO_FG_2421_MODE_WEB        0x2b

#define EM_MONITOR_BUFFER_SIZE  3

typedef struct eizo_ctrl_monitor {
    hid_device *handle;
    unsigned char buffer[EM_MONITOR_BUFFER_SIZE];
    unsigned char backlight_status;
} eizo_ctrl_monitor;

eizo_ctrl_monitor *eizo_ctrl_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number);
int eizo_ctrl_command(eizo_ctrl_monitor *monitor, unsigned char command, unsigned char arg1, unsigned char arg2);

#endif
