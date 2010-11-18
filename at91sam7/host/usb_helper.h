#ifndef _USB_HELPER_H
#define _USB_HELPER_H

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

#include <stdint.h>

const char *hexdump(const void *data, unsigned int len);

struct usb_dev_handle *usb_find_open(uint16_t vendor_id, uint16_t product_id);

#endif
