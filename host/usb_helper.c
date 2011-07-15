/* usb_helper - Low-Level USB routines for SimTrace
 *
 * (C) 2006-2010 by Harald Welte <hwelte@hmw-consulting.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 
 *  as published by the Free Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

#include <sys/types.h>

#include <usb.h>

static struct usb_device *find_usb_device (uint16_t vendor_id, uint16_t product_id)
{
	struct usb_bus *bus;

	for (bus = usb_busses; bus; bus = bus->next) {
		struct usb_device *dev;
		for (dev = bus->devices; dev; dev = dev->next) {
			if (dev->descriptor.idVendor == vendor_id &&
			    dev->descriptor.idProduct == product_id)
				return dev;
		}
	}
	return NULL;
}

struct usb_dev_handle *usb_find_open(uint16_t vendor_id, uint16_t product_id)
{
	struct usb_device *dev;
	struct usb_dev_handle *hdl;

	usb_init();
	usb_find_busses();
	usb_find_devices();

	dev = find_usb_device(vendor_id, product_id);
	if (!dev) {
		fprintf(stderr, "Cannot find matching USB Device. "
			"Are you sure it is connected?\n");
		exit(1);
	}

	hdl = usb_open(dev);
	if (!hdl) {
		fprintf(stderr, "Unable to open usb device: %s\n",
			usb_strerror());
		exit(1);
	}

	if (usb_claim_interface(hdl, 0) < 0) {
		fprintf(stderr, "Unable to claim usb interface "
			"1 of device: %s\n", usb_strerror());
		exit(1);
	}

	return hdl;
}
