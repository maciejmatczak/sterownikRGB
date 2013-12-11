/*
 * RGB_functions.h
 *
 *  Created on: 20-02-2012
 *      Author: ellox
 */

#ifndef RGB_FUNCTIONS_H_
#define RGB_FUNCTIONS_H_

#define RGB_COL_1 0
#define RGB_COL_2 204
#define RGB_COL_3 409
#define RGB_COL_4 614
#define RGB_COL_5 819
#define RGB_COL_6 1024
#define RGB_COL_7 1228
#define RGB_COL_8 1433
#define RGB_COL_9 1638
#define RGB_COL_10 1843
#define RGB_COL_11 2048
#define RGB_COL_12 2252
#define RGB_COL_13 2457
#define RGB_COL_14 2662
#define RGB_COL_15 2867

void eff_fala(void);
void eff_change_cycle_fala(int);
void eff_null(void);
void eff_rand(void);
void eff_change_cycle_rand(int);
void eff_strobo(void);
void eff_xmas(void);
void eff_col_set(uint16_t);
void eff_change_cycle_xmas(int);

void RGB_global_max(int);
void RGB_delta(int, int, int);
void RGB_hue_to_rgb_mem(uint16_t);
void RGB_rand_mem(void);
void RGB_save(void);
void RGB_load(void);
void RGB_change_li(int i);

int tool_sat(int, int, int);
void tool_change_cyc_tog(uint16_t* in,int i,uint16_t min, uint16_t max);
void tool_tim_1_update(uint16_t );

uint16_t change_ch_lvl(uint16_t , double, int);


#endif /* RGB_FUNCTIONS_H_ */
