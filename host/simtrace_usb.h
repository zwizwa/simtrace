#ifndef SIMTRACE_USB_H
#define SIMTRACE_USB_H

//#include <stdint.h>

/* this is kept compatible with OpenPCD protocol */
struct simtrace_hdr {
	u_int8_t cmd;
	u_int8_t flags;
	u_int8_t res[2];
	u_int8_t data[0];
} __attribute__ ((packed));

enum simtrace_usb_msgt {
	SIMTRACE_MSGT_NULL,
	SIMTRACE_MSGT_DATA,
	SIMTRACE_MSGT_RESET,		/* reset was asserted, no more data */
};

/* flags for MSGT_DATA */
#define SIMTRACE_FLAG_ATR		0x01	/* ATR immediately after reset */
#define SIMTRACE_FLAG_WTIME_EXP		0x04	/* work waiting time expired */

#endif /* SIMTRACE_USB_H */
