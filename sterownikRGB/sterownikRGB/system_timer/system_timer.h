/*
 * system_timer.h
 *
 *  Created on: 20-02-2012
 *      Author: ellox
 */

#ifndef SYSTEM_TIMER_H_
#define SYSTEM_TIMER_H_

extern volatile uint16_t tim_1, tim_1_isr;

void system_timer_init(void);

#endif /* SYSTEM_TIMER_H_ */
