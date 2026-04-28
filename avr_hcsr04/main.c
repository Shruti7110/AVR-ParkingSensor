
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "uart.h"
#include "timer.h"
#include "hcsr04.h"


int main(void)
{
	uart_init();
	timer1_init();
	timer0_pwm_init();
	hcsr04_init();
	sei();
	char buf[10];
    DDRB |= (1<< PB1) | (1<< PB2) | (1<< PB3);
    while (1) 
    {
		if(trigger_flag){
			trigger_flag = 0;
			hcsr04_trigger();
			uint16_t distance = get_distance_cm();
			itoa(distance, buf, 10);
			uart_print_string("Remaining distance = ");
			uart_print_string(buf);
			uart_print_string(" cm\r\n");
			PORTB &= ~((1<<PB1) |(1<<PB2) |(1<<PB3));
			if(distance > 50) {
				PORTB |= (1<<PB1);
				OCR0B = 0; //SILENT
			}
			else if(distance > 20) {
				PORTB |= (1<<PB2);
				OCR0B = 64; //MEDIUM BEEP
			}
			else {
				PORTB |= (1<<PB3);
				OCR0B = 128; //LOUD BEEP
			}			
		}
	}

}

