#ifndef _SOFT_UART_H
#define _SOFT_UART_H

enum suart_state {
	WAIT_RESET,
	IN_RESET,
	WAIT_START_FALLEDGE,
	WAIT_START_ETU07,
	RX_BITS,
	POST_RX_WAIT_HIGH,
};

enum suart_convention {
	DIRECT_CONVENTION,
	INVERSE_CONVENTION,
};

struct suart_data {
	uint32_t samplerate;	/* samples per second input rate */
	uint32_t recip_etu;	/* 1/etu */
	uint8_t num_bits;

	enum suart_state state;
	enum suart_convention convention;

	uint32_t sample_after_sbit;
	uint8_t bits_pending;

	uint8_t rx_char;
};

int suart_process_sample_bit(struct suart_data *suart, uint8_t clk_bit, uint8_t rst_bit, uint8_t sample_bit);
void suart_init(struct suart_data *su);

#endif
