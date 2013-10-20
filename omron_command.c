/*
 * A test for 'omron_command' subroutine for NUT, Network UPS Tools.
 *
 * Based on ippon_command() and cypress_command() on blazer_usb.c in
 * nut-2.6.5 distribution.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
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
	char	tmp[64], tmp2[64];
	int	ret, len1;
	size_t	i;

	snprintf(tmp, sizeof(tmp), "%s", cmd);

	for (i = 0; i < strlen(tmp); i += ret) {

		/* Write data in 8-byte chunks */
		ret = usb_control_msg(udev, USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
			0x09, 0x200, 0, &tmp[i], 8, 1000);
		if (ret <= 0) {
			fprintf(stderr, "send: %s", (ret != -ETIMEDOUT) ? usb_strerror() : "Connection timed out");
			return ret;
		}
	}

	printf("send:[%.*s]\n", (int)strcspn(tmp, "\r"), tmp);

	/* Read all 64 bytes of the reply in one large chunk */
	ret = usb_interrupt_read(udev, 0x81, tmp, sizeof(tmp), 1000);

	/*
	 * Any errors here mean that we are unable to read a reply (which
	 * will happen after successfully writing a command to the UPS)
	 */
	if (ret <= 0) {
		fprintf(stderr, "read: %s", (ret != -ETIMEDOUT) ? usb_strerror() : "Connection timed out");
		return ret;
	}

	snprintf(buf, buflen, "%.*s", ret, tmp);
	len1 = ret;

	printf("read1:[%.*s]\n", (int)strcspn(buf, "\r"), buf);

	/*
	 * XXX: On OpenBSD, it seems that we can not get whole data at once.
	 * So we wait a while (1 sec here), and read the rest.
	 */

	sleep(1);

	/* Read all 64 bytes of the reply in one large chunk, second time */
	ret = usb_interrupt_read(udev, 0x81, tmp2, sizeof(tmp2), 1000);

	/*
	 * Any errors here mean that we are unable to read a reply (which
	 * will happen after successfully writing a command to the UPS)
	 */
	if (ret <= 0) {
		fprintf(stderr, "read2: %s", (ret != -ETIMEDOUT) ? usb_strerror() : "Connection timed out");
		return ret;
	}

	snprintf(&buf[len1], buflen - len1, "%.*s", ret, tmp2);

	printf("read2:[%.*s]\n", (int)strcspn(&buf[len1], "\r"), &buf[len1]);

	printf("buf:[%.*s]\n", (int)strcspn(buf, "\r"), buf);

	return len1 + ret;
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

	printf("send:[%.*s]\n", (int)strcspn(tmp, "\r"), tmp);

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

	printf("read:[%.*s]\n", (int)strcspn(buf, "\r"), buf);
	return i;
}
#endif /* of cypress */
