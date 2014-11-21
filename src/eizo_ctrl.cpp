#include "eizo_ctrl.h"
#include <stdlib.h>
#include <string.h>

eizo_ctrl_monitor *eizo_ctrl_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number)
{
	eizo_ctrl_monitor *monitor;
	monitor = (eizo_ctrl_monitor *)malloc(sizeof(eizo_ctrl_monitor));
	if (!monitor)
		return NULL;

	monitor->backlight_status = 0;
	memset(monitor->buffer, 0, EM_MONITOR_BUFFER_SIZE);
	monitor->handle = hid_open(vendor_id, product_id, serial_number);

	if (!monitor->handle) {
		free(monitor);
		return false;
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
