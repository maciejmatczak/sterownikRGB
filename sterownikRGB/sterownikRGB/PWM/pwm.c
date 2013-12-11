/*
 * pwm.c
 *
 *  Created on: 20-02-2012
 *      Author: ellox
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "pwm.h"

volatile uint16_t pwmr_cnt=0, pwmg_cnt=0, pwmb_cnt=0;


void pwm_init(void){
	PWMR_DDR=OUTPUT;
	PWMG_DDR=OUTPUT;
	PWMB_DDR=OUTPUT;
	// Timer0, tryb Normal
	TCCR0 |= (1<<CS00);
	TIMSK |= (1<<TOIE0);
	// CS02:1 -> prescaler = 1
	// TOIE0=1 -> zezwolenie na przerwanie od przepelnienia
	// Prescaler = 1, wiec tykniece timera sprzetowego nastepuje z f=8MHz
}

// Przerwanie od timera 0, realizacja programowych kanalow PWM o rozdzielczosci 9 bit
// Czestotliwosc pracy kanalu PWM :
// F_CPU/prescaler/rozdzielczosc_timera/rozdzielczosc_kanalu_PWM
// W tym wypadku: f=61Hz, T=16.4ms
// OVF timera: f=F_CPU/prescaler/rozdz_tim = 31 250 Hz

ISR( TIMER0_OVF_vect){
	static uint16_t cnt=0;
	if(cnt>=pwmr_cnt) PWMR=LO; else PWMR=HI;
	if(cnt>=pwmg_cnt) PWMG=LO; else PWMG=HI;
	if(cnt>=pwmb_cnt) PWMB=LO; else PWMB=HI;
	cnt++;
	if(cnt>511) cnt=0;
}

