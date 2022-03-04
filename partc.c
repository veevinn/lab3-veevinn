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

volatile int rising = 0;
volatile int start = 0;
volatile int end = 0;
volatile int dist = 0;
char String[25];

void Initialize() {

	cli();
	
	// Echo Pin (Input)
	DDRB &= ~(1<<DDB0);
	
	//Trig Pin (Output)
	DDRD |= (1<<DDD5);
	
	// Initialize Sensor
	PORTD |= (1<<PORTD5);
	_delay_us(10);
	PORTD &= ~(1<<PORTD5);
	
	// Prescale Timer by 8 (2MHz)
	TCCR1B &= ~(1<<CS10);
	TCCR1B |= (1<<CS11);
	TCCR1B &= ~(1<<CS12);
	
	//Timer 1 to Normal Mode
	TCCR1A &= ~(1<<WGM10);	
	TCCR1A &= ~(1<<WGM11);
	TCCR1A &= ~(1<<WGM12);
	TCCR1A &= ~(1<<WGM13);
	
	// Input Capture
	TIMSK1 |= (1<<ICIE1);
	TCCR1B |= (1<<ICES1);
	TIFR1 |= (1<<ICF1);
	
	// Overflow
	TIMSK1 |= (1<<TOIE1);
	TIFR1 |= (1<<TOV1);
}

ISR(TIMER1_CAPT_vect){
	if (rising == 1) {
		start = ICR1;
		TCCR1B &= ~(1<<ICES1);
		rising = 0;
	}
	else if (rising == 0) {
		end = ICR1;
		dist = ((end - start)*0.034/2)*0.5;
		TCCR1B |= (1<<ICES1);
		rising = 1;
	}
}

ISR(TIMER1_OVF_vect) {
	if (dist >=0 && dist <= 400) {
		sprintf(String, "Distance %u cm\n", dist);
		UART_putstring(String);
		dist = 0;
	}
	
	PORTD |= (1<<PORTD5);
	_delay_us(10);
	PORTD &= ~(1<<PORTD5);
	start = 0;
	end = 0;
}
	
int main(void)
{
    Initialize();
    UART_init(BAUD_PRESCALER);
	while (1);
}
