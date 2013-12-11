/*
 * pwm.h
 *
 *  Created on: 20-02-2012
 *      Author: ellox
 */

#ifndef PWM_H_
#define PWM_H_

#ifndef SBIT
struct bits {
   uint8_t b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
 } __attribute__((__packed__));
 #define SBIT_(port,pin) ((*(volatile struct bits*)&port).b##pin)
 #define   SBIT(x,y)       SBIT_(x,y)
#endif

#define PWMR          SBIT( PORTC, 5 )
#define PWMR_DDR      SBIT( DDRC, 5 )
#define PWMG          SBIT( PORTC, 3 )
#define PWMG_DDR      SBIT( DDRC, 3 )
#define PWMB          SBIT( PORTC, 1 )
#define PWMB_DDR      SBIT( DDRC, 1 )


#define INPUT 0
#define OUTPUT 1
#define HI 1
#define LO 0

extern volatile uint16_t pwmr_cnt, pwmg_cnt, pwmb_cnt;

void pwm_init(void);
//void system_timer_init(void);


#endif /* PWM_H_ */
