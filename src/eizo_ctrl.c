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

#include "eizo_ctrl.h"
#include <stdlib.h>
#include <string.h>

void eizo_ctrl_internal_close(void)
{
    hid_exit();
}

int eizo_ctrl_internal_init(void)
{
    static signed char init_done = 0;
    if (init_done)
        return 0;

    if (hid_init())
        return -1;

    init_done = 1;
    atexit(eizo_ctrl_internal_close);
    return 0;
}

int eizo_ctrl_detect(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number)
{
    struct hid_device_info *devs, *cur_dev;
    int count = 0;

    if (eizo_ctrl_internal_init() < 0)
        return -1;

    devs = hid_enumerate(vendor_id, product_id);
    cur_dev = devs;
    while (cur_dev) {
        // TODO: test serial if !NULL
        // store cur_dev->vendor_id, cur_dev->product_id somewhere ?
        count++;
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    return count;
}

eizo_ctrl_monitor *eizo_ctrl_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number)
{
    eizo_ctrl_monitor *monitor;

    if (eizo_ctrl_internal_init() < 0)
        return NULL;

    monitor = (eizo_ctrl_monitor *)malloc(sizeof(eizo_ctrl_monitor));
    if (!monitor)
        return NULL;

    monitor->backlight_status = 0;
    memset(monitor->buffer, 0, EM_MONITOR_BUFFER_SIZE);
    monitor->handle = hid_open(vendor_id, product_id, serial_number);

    if (!monitor->handle) {
        free(monitor);
        return NULL;
    }

    return monitor;
}


int eizo_ctrl_command(eizo_ctrl_monitor *monitor, unsigned char command, unsigned char arg1, unsigned char arg2)
{
    int res;

    monitor->buffer[0] = command;
    monitor->buffer[1] = arg1;
    monitor->buffer[2] = arg2;

    res = hid_send_feature_report(monitor->handle, monitor->buffer, EM_MONITOR_BUFFER_SIZE);
    if (res < 0) {
        // deal with error
        return -1;
    }
    return res;
}
