/*
 * GccApplication1.c
 *
 * Created: 2/25/2022 10:28:59 AM
 * Author : veevinn
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "uart.h"

char String[25];
volatile int dist = 0;
volatile int start = 0;
volatile int end = 0;
volatile int rising = 1;
volatile int ocra = 956;
volatile int cont_mode = 1;

void Initialize(){
		
	cli();

	// set output pin
	DDRD |= (1<<DDD6);
	PORTD |= (1<<PORTD6);

	// prescale timer0 by 8
	TCCR0B &= ~(1<<CS00);
	TCCR0B |= (1<<CS01);
	TCCR0B &= ~(1<<CS02);
		
	// clock in PWM mode
	TCCR0A |= (1<<WGM00);
	TCCR0A &= ~(1<<WGM01);
	TCCR0B |= (1<<WGM02);
		
	TCCR0A |= (1<<COM0A0);
	TCCR0A |= (1<<COM0B1);
		
	// 440 Hz output capture
	OCR0A = ocra;
	OCR0B = ocra / 2;

	TIMSK0 |= (1<<OCIE0A);
	TIFR0 |= (1<<OCF0A);
		
	//GPIO PIN SETUP
	//Ultrasound Sensor Echo Pin
	DDRB &= ~(1<<DDB0);//Set Pin 8 (PB0) to be input pin
	//Ultrasound Sensor Trig Pin
	DDRD |= (1<<DDD7); //Set Pin 7 (PD7) to output

	//10us INITIAL TRIGGER PULSE
	PORTD |= (1<<PORTD7); //Toggle Pin 7 (D7) HIGH
	_delay_us(10);
	PORTD &= ~(1<<PORTD7); //Toggle Pin 7 (D7) LOW
	
	//TIMER1 ADJUSTMENTS
	//Enable clock at 2MHz with (clkio/8 prescaling)
	TCCR1B &= ~(1<<CS10); //0
	TCCR1B |= (1<<CS11); //1
	TCCR1B &= ~(1<<CS12); //0
	
	//Enable Normal mode for ICR1 register (WGM10 - 0, WGM11 - 0, WGM12 - 0, WGM13 - 0)
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	
	TIMSK1 |= (1<<ICIE1); //Enable input capture interrupt
	TCCR1B |= (1<<ICES1); //Look for rising edge
	TIFR1 |= (1<<ICF1); //Clear interrupt flag
	
	//Enable overflow interrupt
	TIMSK1 |= (1<<TOIE1);
	TIFR1 |= (1<<TOV1);
	
	DDRB &= ~(1<<DDB1);
	PCICR |= (1<<PCIE0);
	PCMSK0 |= (1<<PCINT1);
	
	sei();//Enable global interrupts
}

ISR(PCINT0_vect) {
	if(PINB && (1<<PINB1)){
		if(cont_mode == 1) {
			cont_mode = 0;
		}
		else {
			cont_mode = 1;
		}
	}
}

//Input Capture Interrupt that measures the distance being outputted by the Ultrasonic Sensor
ISR(TIMER1_CAPT_vect)
{
	if (rising == 1)
	{
		start = ICR1;
		TCCR1B &= ~(1<<ICES1);
		rising = 0;
	}
	else if (rising == 0)
	{
		end = ICR1;
		// Calculate the distance in centimeters with calibration factor 0.5, calculation deals with cm and us
		dist = ((end-start)*0.034/2)*0.5;
		if(cont_mode == 1){
			ocra = (dist)*1.724 + 477;
		}
		else {
			if(dist > 0 && dist <= 33) {
				ocra = 477;
			}
			if(dist > 33 && dist <= 66) {
				ocra = 506;
			}
			if(dist > 66 && dist <= 100) {
				ocra = 568;
			}
			if(dist > 100 && dist <= 133) {
				ocra = 637;
			}
			if(dist > 133 && dist <= 166) {
				ocra = 715;
			}
			if(dist > 166 && dist <= 200) {
				ocra = 758;
			}
			if(dist > 200 && dist <= 233) {
				ocra = 851;
			}
			if(dist > 233 && dist <= 267) {
				ocra = 956;
			}
		}
						
		TCCR1B |= (1<<ICES1);
		rising = 1;
	}
}

//Timer1 Overflow Interrupt that periodically sends a trigger signal to the Ultrasonic Sensor
ISR(TIMER1_OVF_vect)
{
	//Output the distance to the serial monitor
	if (dist >= 0 && dist <= 400)
	{		
		sprintf(String,"Distance: %u cm\n", dist);
		UART_putstring(String);
		dist = 0;
	}
	
	//10us trigger pulse
	PORTD |= (1<<PORTD7); //Toggle Pin 7 (D7) HIGH
	_delay_us(10);
	PORTD &= ~(1<<PORTD7); //Toggle Pin 7 (D7) LOW
	start = 0;
	end = 0;
}

ISR(TIMER0_COMPA_vect){
	
	OCR0A = ocra;
	OCR0B = ocra / 2;
}

int main(void)
{
	Initialize();
	UART_init(BAUD_PRESCALER);
	while(1);
}
