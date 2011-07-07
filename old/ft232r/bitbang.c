/* This program is distributed under the GPL, version 2 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ftdi.h>

#include "soft_uart.h"

static unsigned char last;
static struct suart_data su;

#define CLK_BIT	0x20
#define RST_BIT	0x08
#define IO_BIT	0x02

static void handle_sample(unsigned char c)
{
	uint8_t rst = 0, io = 0, clk = 0;

	if (c & RST_BIT)
		rst = 1;

	if (c & CLK_BIT)
		clk = 1;

	if (c & IO_BIT)
		io = 1;

#if 0
	if ((c & 0xf) != last) {
		printf("%02x ", c & ~(CLK_BIT|IO_BIT)); 
	}
	last = c;
#endif
#if 0
	if (c & RST_BIT)
		printf("-");
	else
		printf("_");
#endif
#if 1
	if (c & IO_BIT)
		printf("-");
	else
		printf("_");
#endif
	//suart_process_sample_bit(&su, clk, rst, io);
}

#define OVERSAMPLING 4

int main(int argc, char **argv)
{
    struct ftdi_context ftdic;
    int f,i;
    unsigned char buf[1<<16];
	//unsigned int sample_rate = 12900 * OVERSAMPLING; /* 192kS/s */
	unsigned int sample_rate = (1 << 18);

	memset(&su, 0, sizeof(su));

    if (ftdi_init(&ftdic) < 0)
    {
        fprintf(stderr, "ftdi_init failed\n");
        return EXIT_FAILURE;
    }

    f = ftdi_usb_open(&ftdic, 0x0403, 0x6001);

    if (f < 0 && f != -5)
    {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", f, ftdi_get_error_string(&ftdic));
        exit(-1);
    }

    printf("ftdi open succeeded: %d\n",f);

    printf("enabling bitbang mode\n");
    //ftdi_disable_bitbang(&ftdic);
    //ftdi_set_line_property(&ftdic, 8, 1, EVEN);
    ftdi_set_bitmode(&ftdic, 0x0, BITMODE_BITBANG);
    ftdi_read_data_set_chunksize(&ftdic, sizeof(buf));
    ftdi_set_latency_timer(&ftdic, 255);
    //ftdi_set_latency_timer(&ftdic, 1);
    //f = ftdi_set_baudrate(&ftdic, 300000);
    f = ftdi_set_baudrate(&ftdic, sample_rate/16);
    if (f < 0) {
	fprintf(stderr, "error setting baudrate\n");
	exit(1);
    }

	su.samplerate = sample_rate;
	su.recip_etu = sample_rate / OVERSAMPLING;
	su.num_bits = 8;
	suart_init(&su);

    while (1) {
	memset(buf, 0, sizeof(buf));
	f = ftdi_read_data(&ftdic, buf, sizeof(buf));
        if (f < 0) {
		fprintf(stderr,"read failed for 0x%x, error %d (%s)\n",buf[0],f, ftdi_get_error_string(&ftdic));
		exit(1);
        }
	//printf("ftdi returned %u (of %u)\n", f, sizeof(buf));
	for (i = 0; i < f; i++) {
		handle_sample(buf[i]);
	}
	fflush(stdout);
	//num_samples += f;
    }

    printf("\n");

    printf("disabling bitbang mode\n");
    ftdi_disable_bitbang(&ftdic);

    ftdi_usb_close(&ftdic);
    ftdi_deinit(&ftdic);
}
