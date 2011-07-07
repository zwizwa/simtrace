
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#include "soft_uart.h"

static inline uint32_t samples_per_etu(struct suart_data *su)
{
	return su->samplerate / su->recip_etu;
}

static const char *state_strings[] = {
	[WAIT_RESET]		= "WAIT_RESET",
	[IN_RESET]		= "IN_RESET",
	[WAIT_START_FALLEDGE]	= "WAIT_START_FALLEDGE",
	[WAIT_START_ETU07]	= "WAIT_START_ETU07",
	[RX_BITS]		= "RX_BITS",
	[POST_RX_WAIT_HIGH]	= "POST_RX_WAIT_HIGH",
};

static void change_state(struct suart_data *su, enum suart_state new_state)
{
	printf("State change: %s -> %s\n", state_strings[su->state], state_strings[new_state]);
	su->state = new_state;
}

/* According to ISO/IEC 7816-3 Section 6.1.2 */
int suart_process_sample_bit(struct suart_data *suart, uint8_t clk_bit, uint8_t rst_bit, uint8_t sample_bit)
{
	uint32_t samp_per_etu = samples_per_etu(suart);
	uint8_t next_bit;

	if (rst_bit == 0 && suart->state != IN_RESET) {
		change_state(suart, IN_RESET);
		/* FIXME: reset some other things? */
	}

	switch (suart->state) {
	case WAIT_RESET:
		if (rst_bit == 0)
			change_state(suart, IN_RESET);
		break;
	case IN_RESET:
		if (rst_bit == 1)
			change_state(suart, WAIT_START_FALLEDGE);
		break;
	case WAIT_START_FALLEDGE:
		if (sample_bit == 0) {
			suart->sample_after_sbit = 0;
			suart->bits_pending = suart->num_bits + 1;
			change_state(suart, WAIT_START_ETU07);
		}
		break;
	case WAIT_START_ETU07:
		if (suart->sample_after_sbit > (samp_per_etu/2)+1) {
			if (sample_bit != 0) {
				printf("!!!!!!!!!!!!!!!!!!!!! start bit after 0.7 * ETU no longer low\n");
			} else {
				change_state(suart, RX_BITS);
				suart->rx_char = 0;
			}
		}
		/* else stay in this state until the condition is true */
		break;
	case RX_BITS:
		next_bit = suart->num_bits+1 - suart->bits_pending;
#if 0
		printf("\tRX_BITS: IO = %u, next_bit = %u, sample_after_sbit = %u, samp_per_etu = %u, required_after_sbit = %u\n",
			sample_bit, next_bit, suart->sample_after_sbit, samp_per_etu,
			(samp_per_etu/2) + ((next_bit+1) * samp_per_etu));
#endif
		if (suart->sample_after_sbit > (samp_per_etu/2) + ((next_bit+1) * samp_per_etu)) {
			/* check if this is the parity bit */
			//printf("new_bit = %u\n", sample_bit);
			if (next_bit == suart->num_bits) {
				/* FIXME calculate parity */
			} else {
				/* an actual data bit */
				/* Section 6.1.4.1 */
				if (suart->convention == INVERSE_CONVENTION) {
					if (sample_bit == 1)
						sample_bit = 0x00;
					else
						sample_bit = 0x80;	
					/* shift existing patter one to the right */
					suart->rx_char = suart->rx_char >> 1;
					/* mask in the additional bit */
					suart->rx_char |= sample_bit;
				} else {
					suart->rx_char |= (sample_bit << next_bit);
				}
			}
			suart->bits_pending--;
		}

		if (suart->bits_pending == 0) {

			/* output the character that we decoded */
			printf("Output Byte: %02x\n", suart->rx_char);

			change_state(suart, POST_RX_WAIT_HIGH);
		}
		break;
	case POST_RX_WAIT_HIGH:
		if (sample_bit == 0x01) {
			/* return to initial state */
			change_state(suart, WAIT_START_FALLEDGE);
		}
		break;
	}
	suart->sample_after_sbit++;
}

void suart_init(struct suart_data *su)
{
	su->state = WAIT_RESET;
	su->convention = DIRECT_CONVENTION;
	printf("Samplerate = %u\n", su->samplerate);
	printf("etu = 1 / %u\n", su->recip_etu);
	printf("samp_per_etu = %u\n", samples_per_etu(su));
}
