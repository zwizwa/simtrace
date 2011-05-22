/* simtrace - main program for the host PC
 *
 * (C) 2010-2011 by Harald Welte <hwelte@hmw-consulting.de>
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
#define _GNU_SOURCE
#include <getopt.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <usb.h>

#include "usb_helper.h"
#include "simtrace.h"
#include "simtrace_usb.h"
#include "apdu_split.h"

#include <osmocom/core/gsmtap.h>
#include <osmocom/core/gsmtap_util.h>

static struct usb_dev_handle *udev;
static struct apdu_split *as;
static struct gsmtap_inst *g_gti;

static int gsmtap_send_sim(const uint8_t *apdu, unsigned int len)
{
	struct gsmtap_hdr *gh;
	unsigned int gross_len = len + sizeof(*gh);
	uint8_t *buf = malloc(gross_len);
	int rc;

	if (!buf)
		return -ENOMEM;

	memset(buf, 0, sizeof(*gh));
	gh = (struct gsmtap_hdr *) buf;
	gh->version = GSMTAP_VERSION;
	gh->hdr_len = sizeof(*gh)/4;
	gh->type = GSMTAP_TYPE_SIM;

	memcpy(buf + sizeof(*gh), apdu, len);

	rc = write(gsmtap_inst_fd(g_gti), buf, gross_len);
	if (rc < 0) {
		perror("write gsmtap");
		free(buf);
		return rc;
	}

	free(buf);
	return 0;
}

static void apdu_out_cb(uint8_t *buf, unsigned int len, void *user_data)
{
	printf("APDU: %s\n", hexdump(buf, len));
	gsmtap_send_sim(buf, len);
}

static int process_usb_msg(uint8_t *buf, int len)
{
	struct simtrace_hdr *sh = buf;
	uint8_t *payload = buf += sizeof(*sh);
	int payload_len = len - sizeof(*sh);

	if (payload_len < 0)
		return -EINVAL;
	
	switch (sh->cmd) {
	case SIMTRACE_MSGT_DATA:
		/* special treatment for ATR */
		if (sh->flags & SIMTRACE_FLAG_ATR) {
			printf("ATR ");
			apdu_out_cb(payload, payload_len, NULL);
			break;
		}
		/* everything else goes into APDU splitter */
		apdu_split_in(as, payload, payload_len);
#if 0
		/* If waiting time has expired, signal explicit boundary */
		if (sh->flags & SIMTRACE_FLAG_WTIME_EXP)
			apdu_split_boundary(as);
#endif
		break;
	case SIMTRACE_MSGT_RESET:
	default:
		printf("unknown simtrace msg type 0x%02x\n", sh->cmd);
		break;
	}
}

static void print_welcome(void)
{
	printf("simtrace - GSM SIM and smartcard tracing\n"
	       "(C) 2010 by Harald Welte <laforge@gnumonks.org>\n\n");
}

static void print_help(void)
{
	printf( "\t-i\t--gsmtap-ip\tA.B.C.D\n"
		"\t-a\t--skip-atr\n"
		"\t-h\t--help\n"
		"\n"
		);
}

static const struct option opts[] = {
	{ "gsmtap-ip", 1, 0, 'i' },
	{ "skip-atr", 0, 0, 'a' },
	{ "help", 0, 0, 'h' },
	{ NULL, 0, 0, 0 }
};

int main(int argc, char **argv)
{
	char buf[16*265];
	char *gsmtap_host = "127.0.0.1";
	int rc, c;
	int skip_atr = 0;
	unsigned int msg_count, byte_count;

	print_welcome();

	while (1) {
		int option_index = 0;

		c = getopt_long(argc, argv, "i:ah", opts, &option_index);
		if (c == -1)
			break;
		switch (c) {
		case 'h':
			print_help();
			exit(0);
			break;
		case 'i':
			gsmtap_host = optarg;
			break;
		case 'a':
			skip_atr = 1;
			break;
		}
	}

	g_gti = gsmtap_source_init(gsmtap_host, GSMTAP_UDP_PORT, 0);
	if (!g_gti) {
		perror("unable to open GSMTAP");
		exit(1);
	}
	gsmtap_source_add_sink(g_gti);

	udev = usb_find_open(SIMTRACE_USB_VENDOR, SIMTRACE_USB_PRODUCT);
	if (!udev) {
		perror("opening USB device");
		exit(1);
	}

	as = apdu_split_init(&apdu_out_cb, NULL);
	if (!as)
		exit(1);

	printf("Entering main loop\n");
	while (1) {
		rc = usb_bulk_read(udev, SIMTRACE_IN_EP, buf, sizeof(buf), 100000);
		if (rc < 0 && rc != -EAGAIN) {
			fprintf(stderr, "Error submitting BULK IN urb: %s\n", usb_strerror());
			exit(1);
		}
		if (rc > 0) {
			//printf("URB: %s\n", hexdump(buf, rc));
			process_usb_msg(buf, rc);
			msg_count++;
			byte_count += rc;
		}
	}
}
