/*******************************************************
 Windows HID simplification

 Alan Ott
 Signal 11 Software

 8/22/2009

 Copyright 2009
 
 This contents of this file may be used by anyone
 for any reason without any conditions and may be
 used as a starting point for your own applications
 which use HIDAPI.
********************************************************/

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "hidapi.h"

#include <unistd.h>


#define RELAY_VENDOR_ID 0x16c0
#define RELAY_PRODUCT_ID  0x05df
#define RELAY_COMMAND_SIZE 9

#define RELAY_NUMBER    0x01
#define RELAY_STATE_ON	0xff
#define RELAY_STAT_OFF  0xfd

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[RELAY_COMMAND_SIZE];
	hid_device *handle;
	unsigned char command;

	if (argc != 2) {
		fprintf(stderr, "usage: %s [on|off]\n", argv[0]);
		exit(1);
	}
	if (strcmp(argv[1], "on") == 0) {
		command = RELAY_STATE_ON;
	} else if (strcmp(argv[1], "off") == 0) {
		command = RELAY_STAT_OFF;
	} else {
		fprintf(stderr, "usage: %s [on|off]\n", argv[0]);
		exit(1);
	}

	if (hid_init()) {
		fprintf(stderr, "can't init hid lib\n");
		exit(1);
	}

	// Set up the command buffer.
	memset(buf,0x00,sizeof(buf));
	buf[1] = command;
	buf[2] = RELAY_NUMBER;
	

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	////handle = hid_open(0x4d8, 0x3f, L"12345");
	handle = hid_open(RELAY_VENDOR_ID, RELAY_PRODUCT_ID,  NULL);
	if (!handle) {
		printf("unable to open device\n");
 		return 1;
	}

	res = hid_write(handle, buf, sizeof(buf));

	if (res < 0) {
		printf("Unable to send command\n");
	}


	hid_close(handle);

	/* Free static HIDAPI objects. */
	hid_exit();

	exit(0);
}
