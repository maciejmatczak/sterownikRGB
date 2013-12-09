/*
 * makra.h
 *
 *  Created on: 2012-01-13
 *      Author: ellox
 */

#ifndef MAKRA_H_
#define MAKRA_H_

//#ifndef SBIT
//struct bits {
//   uint8_t b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
// } __attribute__((__packed__));
// #define SBIT_(port,pin) ((*(volatile struct bits*)&port).b##pin)
// #define   SBIT(x,y)       SBIT_(x,y)
//#endif

#define INPUT 0
#define OUTPUT 1
#define HI 1
#define LO 0

//#define PWM1          SBIT( PORTC, 5 )
//#define PWM1_DDR      SBIT( DDRC, 5 )
//#define PWM2          SBIT( PORTB, 2 )
//#define PWM2_DDR      SBIT( DDRB, 2 )


#endif /* MAKRA_H_ */
