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

void Initialize()
{
	cli();//disable global interrupts
	
	//ADC Set Up
	//Disable ADC Power Reduction
	PRR &= ~(1<<PRADC);
	
	//Select the reference voltage as AVcc
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	//Set the ADC Clock prescaler to 16M/128 = 125kHz (clock must be between 50 and 200kHz)
	ADCSRA |= (1<<ADPS0);
	ADCSRA |= (1<<ADPS1);
	ADCSRA |= (1<<ADPS2);
	
	//Select Pin A0 channel to read
	ADMUX &= ~(1<<MUX0);
	ADMUX &= ~(1<<MUX1);
	ADMUX &= ~(1<<MUX2);
	ADMUX &= ~(1<<MUX3);
	
	//Set ADC to be free running
	ADCSRA |= (1<<ADATE);
	ADCSRB &= ~(1<<ADTS0);
	ADCSRB &= ~(1<<ADTS1);
	ADCSRB &= ~(1<<ADTS2);
	
	//Disable input buffer
	DIDR0 |= (1<<ADC0D);
	
	//Enable ADC
	ADCSRA |= (1<<ADEN);
	
	//Start conversion
	ADCSRA |= (1<<ADSC);
	
	sei();//Enable global interrupts
}


int main(void)
{
	Initialize();
	UART_init(BAUD_PRESCALER);
	while(1)
	{
		if (ADC >= 2 && ADC < 103)
		{
			sprintf(String,"ADC Value: %u | Duty Cycle: 5%%\n", ADC);
			UART_putstring(String);
		}
		else if(ADC >= 104 && ADC < 205)
		{
			sprintf(String,"ADC Value: %u | Duty Cycle: 10%%\n", ADC);
			UART_putstring(String);
		}
		else if(ADC >= 206 && ADC < 307)
		{
			sprintf(String,"ADC Value: %u | Duty Cycle: 15%%\n", ADC);
			UART_putstring(String);
		}
		else if(ADC >= 308 && ADC < 409)
		{
			sprintf(String,"ADC Value: %u | Duty Cycle: 20%%\n", ADC);
			UART_putstring(String);
		}
		else if(ADC >= 410 && ADC < 511)
		{
			sprintf(String,"ADC Value: %u | Duty Cycle: 25%%\n", ADC);
			UART_putstring(String);
		}
		else if(ADC >= 512 && ADC < 613)
		{
			sprintf(String,"ADC Value: %u | Duty Cycle: 30%%\n", ADC);
			UART_putstring(String);
		}
		else if(ADC >= 614 && ADC < 715)
		{
			sprintf(String,"ADC Value: %u | Duty Cycle: 35%%\n", ADC);
			UART_putstring(String);
		}
		else if(ADC >= 716 && ADC < 817)
		{
			sprintf(String,"ADC Value: %u | Duty Cycle: 40%%\n", ADC);
			UART_putstring(String);
		}
		else if(ADC >= 818 && ADC < 919)
		{
			sprintf(String,"ADC Value: %u | Duty Cycle: 45%%\n", ADC);
			UART_putstring(String);
		}
		else if(ADC >= 920 && ADC < 1022)
		{
			sprintf(String,"ADC Value: %u | Duty Cycle: 50%%\n", ADC);
			UART_putstring(String);
		}
	}
}
