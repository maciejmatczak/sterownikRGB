/*
 * RGB_functions.c
 *
 *  Created on: 20-02-2012
 *      Author: ellox
 */

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "RGB_functions.h"
#include "../USART/usart.h"
#include "../IR_DECODE/ir_decode.h"
#include "../system_timer/system_timer.h"
#include "../PWM/pwm.h"
#include <util/delay.h>

volatile uint16_t r_mem, g_mem, b_mem;
volatile int lightness;
volatile uint16_t global_max = 511;
volatile uint16_t eff_fala_cycle = 8, eff_rand_cycle = 100, eff_xmas_cycle = 24;
double step_up[3], step_down[3];
volatile uint8_t toggle_h;

void eff_null(void) {

}

void eff_fala(void) {
	//static int stan_fali=0;
	static int i = 0;

	if (!tim_1) {
		tool_tim_1_update(eff_fala_cycle);

		RGB_hue_to_rgb_mem(i);
		RGB_load();
		i++;
		if (i > 3071)
			i = 0;
	}
}

void eff_rand(void) {
	if (!tim_1) {
		tool_tim_1_update(eff_rand_cycle);

		RGB_load();
		RGB_rand_mem();
	}
}

// TODO: floaty
void eff_strobo(void) {
	if (!tim_1) {

		pwmr_cnt = 300;
		pwmg_cnt = 300;
		pwmb_cnt = 511;

		_delay_ms(25);

		pwmr_cnt = 0;
		pwmg_cnt = 0;
		pwmb_cnt = 0;

		tool_tim_1_update(
				(uint16_t) (((double) rand() / (double) RAND_MAX) * 200 + 100));
	}
}

// todo: floaty
void eff_xmas(void) {
	static uint8_t state_xmas = 0, i = 0;

	if (!tim_1) {
		tool_tim_1_update(eff_xmas_cycle);

		switch (state_xmas) {
			case 0: {
				pwmr_cnt = 0;
				pwmg_cnt = 0;
				pwmb_cnt = 0;
				state_xmas = 1;
				break;
			}
			case 1: {
				RGB_rand_mem();

				//step_down[0] = (r_mem) / 100.0;
				//step_down[1] = (g_mem) / 100.0;
				//step_down[2] = (b_mem) / 100.0;

				state_xmas = 2;

				break;
			}
			case 2: {
				pwmr_cnt = r_mem * (i / 101.0);
				pwmg_cnt = g_mem * (i / 101.0);
				pwmb_cnt = b_mem * (i / 101.0);
				//pwmr_cnt=change_ch_lvl(r_mem,step_down[0],(i-101));
				//pwmg_cnt=change_ch_lvl(g_mem,step_down[1],(i-101));
				//pwmb_cnt=change_ch_lvl(b_mem,step_down[2],(i-101));
				i++;
				break;
			}
			case 3: {
				//RGB_load();
				i++;
				break;
			}
			case 4: {
				pwmr_cnt = r_mem * ((101 - i) / 101.0);
				pwmg_cnt = g_mem * ((101 - i) / 101.0);
				pwmb_cnt = b_mem * ((101 - i) / 101.0);
				i++;
				break;
			}
		}
		if (i > 101) {
			i = 0;
			state_xmas = (state_xmas + 1) % 5;
		}
	}
}

void eff_col_set(uint16_t hue) {
	RGB_hue_to_rgb_mem(hue);
	RGB_load();
}

void eff_change_cycle_fala(int i) {
	tool_change_cyc_tog((uint16_t *) &eff_fala_cycle, i, 1, 64);
}

void eff_change_cycle_rand(int i) {
	tool_change_cyc_tog((uint16_t *) &eff_rand_cycle, i, 25, 800);
}

void eff_change_cycle_xmas(int i) {
	tool_change_cyc_tog((uint16_t *) &eff_xmas_cycle, i, 2, 128);
}

void RGB_global_max(int i) {
	global_max = tool_sat(global_max + i, 0, 511);
}

void RGB_delta(int r, int g, int b) {
	pwmr_cnt = tool_sat(pwmr_cnt + r, 0, 511);
	pwmg_cnt = tool_sat(pwmg_cnt + g, 0, 511);
	pwmb_cnt = tool_sat(pwmb_cnt + b, 0, 511);
}

// todo: floaty
void RGB_hue_to_rgb_mem(uint16_t hue) {
	/*
	 * Hue from 0 to 3071 !!!
	 * http://upload.wikimedia.org/wikipedia/commons/5/5d/HSV-RGB-comparison.svg
	 */
	if (hue < 512) {
		r_mem = global_max;
		g_mem = (global_max / 511.0) * hue;
		b_mem = 0;
		return;
	}
	if (hue < 1024) {
		r_mem = -(global_max / 511.0) * hue + (global_max / 511.0) * (double) 1023;
		g_mem = global_max;
		b_mem = 0;
		return;
	}
	if (hue < 1536) {
		r_mem = 0;
		g_mem = global_max;
		b_mem = (global_max / 511.0) * hue - (global_max / 511.0) * 1024;
		return;
	}
	if (hue < 2048) {
		r_mem = 0;
		g_mem = -(global_max / 511.0) * hue + (global_max / 511.0) * 2047;
		b_mem = global_max;
		return;
	}
	if (hue < 2560) {
		r_mem = (global_max / 511.0) * hue - (global_max / 511.0) * 2048;
		g_mem = 0;
		b_mem = global_max;
		return;
	}
	if (hue < 3072) {
		r_mem = global_max;
		g_mem = 0;
		b_mem = -(global_max / 511.0) * hue + (global_max / 511.0) * 3071;
		return;
	}
}

//TODO: floaty
void RGB_rand_mem(void) {
	static uint16_t randzik;
	randzik = (uint16_t) (randzik + 384
	+ (((float) rand() / (float) RAND_MAX) * 2304)) % 3072;
	RGB_hue_to_rgb_mem(randzik);
}

void RGB_save(void) {
	r_mem = pwmr_cnt;
	g_mem = pwmg_cnt;
	b_mem = pwmb_cnt;

	step_up[0] = (511 - r_mem) / 100.0;
	step_up[1] = (511 - g_mem) / 100.0;
	step_up[2] = (511 - b_mem) / 100.0;

	step_down[0] = (r_mem) / 100.0;
	step_down[1] = (g_mem) / 100.0;
	step_down[2] = (b_mem) / 100.0;

	lightness = 0;
}

void RGB_load(void) {
	pwmr_cnt = r_mem;
	pwmg_cnt = g_mem;
	pwmb_cnt = b_mem;
}

void RGB_change_li(int i) {
	lightness = tool_sat(lightness + i, -100, 100);
	if (lightness == 0) {
		pwmr_cnt = 0;
		pwmg_cnt = 0;
		pwmb_cnt = 0;
		_delay_ms(500);
		pwmr_cnt = r_mem;
		pwmg_cnt = g_mem;
		pwmb_cnt = b_mem;
	} else if (lightness > 0) {
		pwmr_cnt = change_ch_lvl(r_mem, step_up[0], lightness);
		pwmg_cnt = change_ch_lvl(g_mem, step_up[1], lightness);
		pwmb_cnt = change_ch_lvl(b_mem, step_up[2], lightness);
	} else {
		pwmr_cnt = change_ch_lvl(r_mem, step_down[0], lightness);
		pwmg_cnt = change_ch_lvl(g_mem, step_down[1], lightness);
		pwmb_cnt = change_ch_lvl(b_mem, step_down[2], lightness);
	}
}

uint16_t change_ch_lvl(uint16_t in, double step, int in2) {
	return tool_sat(in + (int) (step * (double) in2), 0, 511);
}

void tool_change_cyc_tog(uint16_t *in, int i, uint16_t min, uint16_t max) {
	if (toggle_h ^ toggle_bit) {
		toggle_h = toggle_bit;
		if (i == 1)
			(*in) = tool_sat((*in) * 2, min, max);
		if (i == -1)
			(*in) = tool_sat((*in) / 2, min, max);
	}
}

void tool_tim_1_update(uint16_t timer) {
	cli();
	tim_1_isr = timer;
	sei();
}

int tool_sat(int in, int lb, int ub) {
	if (in > ub)
		return ub;
	else {
		if (in < lb)
			return lb;
		else
			return in;
	}
}
