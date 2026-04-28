#include "hcsr04.h"

volatile uint16_t echo_start = 0;
volatile uint16_t echo_end = 0;

void hcsr04_init(void){
	DDRB |= 1; //setting PB0 as output (trigger)
	DDRD &= ~(1<<PD2); //setting PD2 as input (echo)	 
	EICRA = (1<< ISC00) | (1<< ISC01); // setting trigger at rising edge
	EIMSK = (1 << INT0);
}

void hcsr04_trigger(void){
	PORTB |= (1 << PB0);
	_delay_us(10);
	PORTB &= ~(1 << PB0);
}

uint16_t get_distance_cm(void){
	uint16_t distance = ((echo_end - echo_start) / 58);
	return distance;
}

ISR(INT0_vect){
	if(PIND & (1<<PD2)){
		//PIN IS HIGH = RISING EDGE
		echo_start = TCNT1;
		EICRA = (1<< ISC01); // changing settings to falling edge
	} else {
		// PIN IN LOW = FALLING EDGE
		echo_end = TCNT1;
	}	
}


