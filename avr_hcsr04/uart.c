#include "uart.h"

void uart_init(void){
	// Set baud rate
	UBRR0H = (unsigned char)(MYUBRR >> 8);
	UBRR0L = (unsigned char)MYUBRR;

	// Enable RX and TX  ? (correct register)
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit(uint8_t data){
	// Wait until transmit buffer is empty
	while(!(UCSR0A & (1 << UDRE0)));

	// Send data
	UDR0 = data;
}

void uart_print_string(const char* str){
	// NULL check
	if (!*str) return;

	// Loop until null terminator
	while (*str != '\0'){
		uart_transmit(*str);
		str++;
	}
}

uint8_t uart_receive_timeout(uint16_t timeout_ms, uint8_t *received){
	uint32_t counts = (uint32_t)timeout_ms * 1600;

	while(counts--){
		// Check if data is received (RXC0 = 1 means data ready)
		if (UCSR0A & (1 << RXC0)){
			*received = UDR0;
			return 1;
		}
	}

	return 0;
}