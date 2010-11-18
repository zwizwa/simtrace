#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <usb.h>
#include <sys/ioctl.h>
#include "usb.h"
#include <linux/usbdevice_fs.h>

#define MAX_READ_WRITE	4096

#define USB_ERROR_STR(ret, x, args...)	return ret

static int usb_get_fd(usb_dev_handle *uh)
{
	return *((int *)uh);
}

int __usb_bulk_write(usb_dev_handle *dev, int ep, char *bytes, int length,
		     int timeout)
{
	struct usbdevfs_bulktransfer bulk;
	int ret, sent = 0;

	/* Ensure the endpoint address is correct */
	ep &= ~USB_ENDPOINT_IN;

	do {
		bulk.ep = ep;
		bulk.len = length - sent;
		if (bulk.len > MAX_READ_WRITE)
			bulk.len = MAX_READ_WRITE;
		bulk.timeout = timeout;
		bulk.data = (unsigned char *)bytes + sent;

		ret = ioctl(usb_get_fd(dev), USBDEVFS_BULK, &bulk);
		if (ret < 0)
			USB_ERROR_STR(ret,
				      "error writing to bulk endpoint %d: %s",
				      ep, strerror(errno));

		sent += ret;
	} while (ret > 0 && sent < length);

	return sent;
}

int __usb_bulk_read(usb_dev_handle *dev, int ep, char *bytes, int size,
		    int timeout)
{
	struct usbdevfs_bulktransfer bulk;
	int ret, retrieved = 0, requested;

	/* Ensure the endpoint address is correct */
	ep |= USB_ENDPOINT_IN;

	do {
		bulk.ep = ep;
		requested = size - retrieved;
		if (requested > MAX_READ_WRITE)
			requested = MAX_READ_WRITE;
		bulk.len = requested;
		bulk.timeout = timeout;
		bulk.data = (unsigned char *)bytes + retrieved;

		ret = ioctl(usb_get_fd(dev), USBDEVFS_BULK, &bulk);
		if (ret < 0)
			USB_ERROR_STR(ret,
				      "error reading from bulk endpoint 0x%x: %s",
				      ep, strerror(errno));

		retrieved += ret;
	} while (ret > 0 && retrieved < size && ret == requested);

	return retrieved;
}

int __usb_reattach_kernel_driver_np(usb_dev_handle *dev, int interface)
{
	struct usbdevfs_ioctl command;

	command.ifno = interface;
	command.ioctl_code = USBDEVFS_CONNECT;
	command.data = NULL;

	return ioctl(usb_get_fd(dev), USBDEVFS_IOCTL, &command);
}
