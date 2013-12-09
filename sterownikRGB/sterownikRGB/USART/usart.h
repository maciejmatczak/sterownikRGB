/*
 * usart.h
 *
 *  Created on: 20-02-2012
 *      Author: ellox
 */

#ifndef USART_H_
#define USART_H_

#define RS_BAUD 38400
#define RS_UBRR 8000000 / 16 / RS_BAUD - 1

void usart_init();
void usart_t( unsigned char );
void usart_str(const char * );
void usart_int(char *, int );

extern int i, odbieraj, odebrano_ramke;
extern char ramka[14];


#endif /* USART_H_ */
