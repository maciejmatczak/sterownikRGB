/*
 * system_timer.c
 *
 *  Created on: 20-02-2012
 *      Author: ellox
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "system_timer.h"

volatile uint16_t tim_1, tim_1_isr;//, timer1ms_2;

void system_timer_init(void){
	// Timer2, tryb CTC
	TCCR2 |= (1<<WGM21); // WGM21:0=10 -> Tryb CTC
	TCCR2 |= (1<<CS22)|(1<<CS21)|(1<<CS20); // prescaler = 1024
	OCR2=8;
	TIMSK |= (1<<OCIE2);
	// COM21:0=00 -> OC2 odlaczony, funkcjonuje jako zwykly port
	// OCIE2=1 -> zezwolenie na przerwanie od CTC
	// F_CPU/prescaler/OCR2 = 976.5625Hz, T=1.024ms
}


ISR( TIMER2_COMP_vect){
	uint16_t x;
	x=tim_1_isr;
	if (x>0) tim_1_isr=--x;
}
