/*
 * main.c
 *
 *  Created on: 07-12-2011
 *      Author: ellox
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "usart.h"

int i=0, odbieraj=0, odebrano_ramke=0;
char ramka[14];

void usart_init()
{
	/* Set baud rate */
	UBRRH = (unsigned char) ( (RS_UBRR) >>8);
	UBRRL = (unsigned char) (RS_UBRR);
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void usart_t( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
		;
	/* Put data into buffer, sends the data */
	UDR = data;
}


void usart_str(const char *s )
{
	while (*s)
		usart_t(*s++);
}

void usart_int(char *string, int liczba){
	char tab[10];
	itoa(liczba,tab,10);
		usart_str(string);
		usart_str(tab);
}

// BUFOR DANYCH Z USARTA
volatile unsigned char usart_buf;

ISR(USART_RXC_vect) //przerwanie od odbioru danych
{
	usart_buf=UDR;

	if((usart_buf=='R')&&(odebrano_ramke==0)){
		odbieraj=1;
		i=0;
	}


	if(odbieraj){
		ramka[i]=usart_buf;
		if(ramka[i]=='\n') {
			if(ramka[i-1]=='\r'){
				odebrano_ramke=1; odbieraj=0;
			}
		}
		i++;
	}

}
