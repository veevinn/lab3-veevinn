/*
 * GccApplication1.c
 *
 * Created: 2/25/2022 10:28:59 AM
 * Author : veevinn
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)


/*--------------------Libraries---------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uart.h"

void Initialize() {
	
	CLKPR = (1<<CLKPCE);
	CLKPR |= (1<<CLKPS0);
	
	cli();
	
	// set output pin
	DDRD |= (1<<DDD6);
	PORTD |= (1<<PORTD6);

	// prescale clock by 8
	TCCR0B &= ~(1<<CS00);
	TCCR0B |= (1<<CS01);
	TCCR0B &= ~(1<<CS02);
	
	// clock in normal PWM mode
	TCCR0A &= ~(1<<WGM00);
	TCCR0A &= ~(1<<WGM01);
	TCCR0B &= ~(1<<WGM02);
	
//	TCCR0A |= (1<<COM0B1);
//	TCCR0A |= (1<<COM0A0);
//	OCR0A = 30;
//	OCR0B = 15;

	TIMSK0 |= (1<<TOIE0);
//	TIMSK0 |= (1<<OCIE0A);
//	TIFR0 |= (1<<OCF0B);
	TIFR0 |= (1<<TOV0);
	sei();
}

ISR(TIMER0_OVF_vect) {
	PORTD ^= (1<<PORTD6);
	//TCNT0 = 0;
}

int main(void)
{
    Initialize();
    while (1);
}


