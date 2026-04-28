#ifndef HCSR04_H_
#define HCSR04_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

extern volatile uint16_t echo_start; 
extern volatile uint16_t echo_end; 

void hcsr04_init(void);
void hcsr04_trigger(void);
uint16_t get_distance_cm(void);

#endif /* HCSR04_H_ */