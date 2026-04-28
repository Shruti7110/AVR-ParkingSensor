#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

//Timer 1 CTC - triggers measurement very 60ms
#define TIMER_PRESCALAR	8UL
#define TIMER1_60MS		((F_CPU / (TIMER_PRESCALAR * 1000UL / 60UL))-1) //60ms

extern volatile uint8_t trigger_flag; // set by timer1 ISR, tells main to trigger HC-SR04

void timer1_init(void); //60ms measurements interval 
void timer0_pwm_init(void); //buzzer pwm base

#endif /* TIMER_H_ */