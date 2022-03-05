#include "parta.h"
#include <avr/io.h>

void PARTA_init(int prescalar)
{
  
  /*Set baud rate */
  UBRR0H = (unsigned char)(prescalar>>8);
  UBRR0L = (unsigned char)prescalar;
  //Enable receiver and transmitter 
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);

  UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // 8 data bits
  UCSR0C |= (1<<USBS0); // 2 stop bits
}

void PARTA_output(char* strpt)
{
  while(*strpt != 0x00)
  {
    while(!(UCSR0A & (1<<UDRE0)));  
    // Put data into buffer and send data
    UDR0 = *strpt;
    strpt++;
    }
}
