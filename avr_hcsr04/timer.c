
#include "timer.h"

volatile uint8_t trigger_flag = 0;no go

//for 60ms measurement
void timer1_init(void){
	TCCR1B |= (1 << WGM12); //CTC mode
	TCCR1B |= (1 << CS11); // Prescaler = 8
	TIMSK1 = (1 << OCIE1A);  // enable compare match interrupt
	OCR1A = TIMER1_60MS;
}

//for buzzer freq
void timer0_pwm_init(void){ 
	TCCR0A |= (1 << WGM00); // PWM mode, phase correct, 8-bit
	TCCR0A |= (1 << COM0B1); // OC0B as output
	TCCR0B |= (1 << CS01); // Prescaler = 8 
	OCR0B = 0; //speaker silent
}

ISR(TIMER1_COMPA_vect){
	//set triger flag as 1
	trigger_flag = 1;
}