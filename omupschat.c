/*
 * omupschat - chat test program with OMRON UPS, connected by USB.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <usb.h>
#if defined(__linux__)
#include <bsd/stdio.h>
#endif

#define TARGET_VENDOR_ID	0x0590	/* OMRON */
#define TARGET_PRODUCT_ID	0x0080	/* BY35S */

#define DEBUG	1

struct usb_dev_handle	*udev = NULL;

extern int omron_command(const char *, char *, size_t);
struct usb_device * search_target(struct usb_bus *);

int
main()
{
	struct usb_bus *busses = NULL;
	struct usb_device *target_dev = NULL;
	int ret, i;
	size_t com_len;

	char *p, com[256], buf[256];

	/* libusb initialize */
	usb_init();
	usb_find_busses();
	usb_find_devices();

	busses = usb_get_busses();

	target_dev = search_target(busses);
	if (target_dev == NULL) {
		printf("Can not find VendorID=0x%04x, ProductID=0x%04x\n",
			TARGET_VENDOR_ID, TARGET_PRODUCT_ID);
		return 1;
	}

	udev = usb_open(target_dev);

	if (!udev) {
		printf("fail to open the USB device\n");
		return 1;
	}

	if ((ret = usb_claim_interface(udev, 0)) < 0) {
		printf("Can not claim USB device, ret = %d\n", ret);
		usb_close(udev);
		return 1;
	}

	if ((ret = usb_set_altinterface(udev, 0)) < 0) {
		printf("Can not set alt-interface, ret = %d\n", ret);
		usb_close(udev);
		return 1;
	}

	do {
		/* Clear buffer */
		memset((void *)com, 0x0, sizeof(com));
		memset((void *)buf, 0x0, sizeof(buf));
#if 1
		p = fgetln(stdin, &com_len);

		if (com_len >= sizeof(com)) {
			printf("Too long command\n");
			continue;
		}
			
		for (i = 0; i < sizeof(com); i++) {
			com[i] = p[i];
			if (p[i] == '\n') com[i] = '\0';
		}
#else
		gets(com);
#endif
		strlcat(com, "\r", sizeof(com));
#if 0
		/* Dump */
		for(i = 0; i < 256; i++) {
			if ((i % 8) == 0)
				printf("%03i ", i);
			printf("%c[%02x]",
				isprint((int)com[i]) ? com[i] : '.', com[i]);
			if ((i % 8) == 7)
				printf("\n");
			else
				printf(" ");
		}
#endif
		ret = omron_command(com, buf, sizeof(buf)); 
		printf("ret = %d\n", ret);
	} while (com[0] != 0x0);

	usb_close(udev);

	return 0;
}

/*
 * Search the target device and return the pointer of the target device.
 * Return NULL if not found.
 */
struct usb_device *
search_target(struct usb_bus *busses) {
	struct usb_bus *bus;
	struct usb_device *dev, *target = NULL;
	int bcount = 0, dcount = 0;

	for (bus = busses; bus; bus = bus->next) {

		for (dev = bus->devices; dev; dev = dev->next) {
#if DEBUG
			printf("bus %d, device %d,", bcount, dcount);
			printf(" VendorID:0x%04x, ProductID:0x%04x,",
				dev->descriptor.idVendor,
				dev->descriptor.idProduct);
			printf(" at (%s/%s)\n", bus->dirname, dev->filename);
#endif
			if ((dev->descriptor.idVendor == TARGET_VENDOR_ID) &&
				(dev->descriptor.idProduct == TARGET_PRODUCT_ID)) {
					target = dev;
					printf("** Target device found\n");
			}
			dcount++;
		}
		bcount++;
	}
	return target;
}
