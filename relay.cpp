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

int relay_get_state(const char * path) {
	hid_device *dev;
	unsigned char buf[RELAY_COMMAND_SIZE];

	if((dev = hid_open_path(path)) == NULL) {
		return -1;
	}
	buf[0] = 0x01; // get first page
	if(hid_get_feature_report(dev, buf, sizeof(buf)) == -1) {
		return -1;
	}
	/*
	printf("buffer -> %d %d %d %d %d %d %d %d %d\n", 
			buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8]);
	*/
	return buf[7] & 1;
}

int doScan() {
	struct hid_device_info *devs, *cur_dev;
	
	if (hid_init()) {
		return -1;
	};

	devs = hid_enumerate(0x0, 0x0);
	for (cur_dev = devs ; cur_dev; cur_dev = cur_dev->next) {
		printf("====>\n");
		printf("\tvendor_id: %04hx\n", cur_dev->vendor_id);
		printf("\tproduct_id: %04hx\n", cur_dev->product_id);
		printf("\tpath: %s\n", cur_dev->path);
		printf("\tserial_number: %ls\n", cur_dev->serial_number);
		printf("\tmanufacturer string: %ls\n", cur_dev->manufacturer_string);
		printf("\tproduct string: %ls\n", cur_dev->product_string);
		printf("\trelease number: %hx\n", cur_dev->release_number);
		printf("\tinterface number%d\n",  cur_dev->interface_number);
		if (cur_dev->vendor_id == RELAY_VENDOR_ID || cur_dev->product_id == RELAY_PRODUCT_ID)
		{
			// will not work with more than one realy per card
			unsigned int state = relay_get_state(cur_dev->path);
			switch (state) {
				case 0: printf("\tstate off\n"); break;
				case 1: printf("\tstate on\n"); break;
				default: printf("\tstate unknown\n"); break;
			}
		}
		printf("\n");
	}
	hid_free_enumeration(devs);
	hid_exit();

	return 0;
}

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
	} else if (strcmp(argv[1], "scan") == 0) {
		return doScan();
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
