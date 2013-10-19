/*
 * A test for 'omron_command' subroutine for NUT, Network UPS Tools.
 *
 * Based on ippon_command() and cypress_command() on blazer_usb.c in
 * nut-2.6.5 distribution.
 */

#include <errno.h>
#include <stdio.h>
#include <usb.h>

#define COMMAND_IPPON
/* #define COMMAND_CYPRESS */

extern struct usb_dev_handle	*udev;
int omron_command(const char *, char *, size_t);

#if defined(COMMAND_IPPON) /* based on ippon_command */
/*
 * command driver
 * originally taken from blazer_usb.c:ippon_command()
 */

int
omron_command(const char *cmd, char *buf, size_t buflen)
{
	char	tmp[64];
	int	ret;
	size_t	i;

	snprintf(tmp, sizeof(tmp), "%s", cmd);

	for (i = 0; i < strlen(tmp); i += ret) {

		/* Write data in 8-byte chunks */
#if 0 /* original */
		ret = usb_control_msg(udev, USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
			0x09, 0x2, 0, &tmp[i], 8, 1000);
#else
		ret = usb_control_msg(udev, USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
			0x09, 0x200, 0, &tmp[i], 8, 1000);
#endif
		if (ret <= 0) {
			fprintf(stderr, "send: %s", (ret != -ETIMEDOUT) ? usb_strerror() : "Connection timed out");
			return ret;
		}
	}

	printf("send: %.*s", (int)strcspn(tmp, "\r"), tmp);

	/* Read all 64 bytes of the reply in one large chunk */
	ret = usb_interrupt_read(udev, 0x81, tmp, sizeof(tmp), 1000);

	/*
	 * Any errors here mean that we are unable to read a reply (which
	 * will happen after successfully writing a command to the UPS)
	 */
	if (ret <= 0) {
		printf("read: %s", (ret != -ETIMEDOUT) ? usb_strerror() : "Connection timed out");
		return ret;
	}

	snprintf(buf, buflen, "%.*s", ret, tmp);

	printf("read: %.*s", (int)strcspn(buf, "\r"), buf);

	return ret;
}

#endif /* of ippon */

#if defined(COMMAND_CYPRESS) /* based on cypress_command */
/*
 * command driver
 * based on blazer_usb.c:cypress_command()
 */

int
omron_command(const char *cmd, char *buf, size_t buflen)
{
	char	tmp[64];
	int	ret;
	size_t	i;

	memset(tmp, 0, sizeof(tmp));
	snprintf(tmp, sizeof(tmp), "%s", cmd);

	for (i = 0; i < strlen(tmp); i += ret) {

		/* Write data in 8-byte chunks */
		/* ret = usb->set_report(udev, 0, (unsigned char *)&tmp[i], 8); */
		ret = usb_control_msg(udev, USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
			0x09, 0x200, 0, &tmp[i], 8, 5000);

		if (ret <= 0) {
			printf("send: %s", ret ? usb_strerror() : "timeout");
			return ret;
		}
	}

	printf("send: %.*s", (int)strcspn(tmp, "\r"), tmp);

	memset(buf, 0, buflen);

	for (i = 0; (i <= buflen-8) && (strchr(buf, '\r') == NULL); i += ret) {

		/* Read data in 8-byte chunks */
		/* ret = usb->get_interrupt(udev, (unsigned char *)&buf[i], 8, 1000); */
		ret = usb_interrupt_read(udev, 0x81, &buf[i], 8, 1000);

		/*
		 * Any errors here mean that we are unable to read a reply (which
		 * will happen after successfully writing a command to the UPS)
		 */
		if (ret <= 0) {
			printf("read: %s", ret ? usb_strerror() : "timeout");
			return ret;
		}
	}

	printf("read: %.*s", (int)strcspn(buf, "\r"), buf);
	return i;
}
#endif /* of cypress */
