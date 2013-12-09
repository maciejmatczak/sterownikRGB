
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "makra.h"
#include "IR_DECODE/ir_decode.h"
#include "IR_DECODE/remote_key.h"
#include "USART/usart.h"
#include "RGB_functions/RGB_functions.h"
#include "PWM/pwm.h"
#include "system_timer/system_timer.h"

void exec_eff(void (*fun)(void));

int main(void) {

	// Inicjalizacja komponentow
	pwm_init();
	system_timer_init();
	usart_init();
	ir_init();
	sei();

	// Zmienne pomocnicze
	void (*fun_to_exec)(void);
	//char tab[10];
	uint16_t i;
	enum {
		mode_red,
		mode_green,
		mode_blue,
		mode_lightness,
		mode_null,
		mode_fala,
		mode_rand,
		mode_xmas
	};
	int cursor_mode = mode_fala;

//	uint16_t nastawa=0;
//	char strobo=1;

//	 Tresc komunikatu powitalnego
	usart_str("Siema!\r\n");

//	 Rozjasnianie poczatkowe
	for (i = 0; i <= 511; i++) {
		pwmr_cnt = i;
		pwmg_cnt = i;
		pwmb_cnt = (uint16_t) ((double) i / 511 * 300);
		_delay_ms(1);
	}

//	Funkcja poczatkowa
	fun_to_exec = eff_fala;

	while (1) {

//		 Sprawdzenie nacisniecia przycisku na pilocie
		if (Ir_key_press_flag) {
			Ir_key_press_flag = 0;

//			Reakcja na dana komende
			switch (command) {

//==============================================================
//          Przyciski odpowiedzialne za efekty swietlne
//==============================================================
				case RK_FUN_1: {
					fun_to_exec = eff_fala;
					cursor_mode = mode_fala;
					break;
				}
				case RK_FUN_2: {
					fun_to_exec = eff_rand;
					cursor_mode = mode_rand;
					break;
				}
				case RK_FUN_3: {
					fun_to_exec = eff_strobo;
					cursor_mode = mode_null;
					break;
				}
				case RK_FUN_4: {
					fun_to_exec = eff_xmas;
					cursor_mode = mode_xmas;
					break;
				}

	//==============================================================
	//        Przyciski odpowiedzialne za kolor nastawialny
	//==============================================================
				case RK_RED: {
					fun_to_exec = eff_null;
					cursor_mode = mode_red;
					break;
				}
				case RK_GREEN: {
					fun_to_exec = eff_null;
					cursor_mode = mode_green;
					break;
				}
				case RK_BLUE: {
					fun_to_exec = eff_null;
					cursor_mode = mode_blue;
					break;
				}
				case RK_YELLOW: {
					fun_to_exec = eff_null;
					cursor_mode = mode_lightness;
					RGB_save();
					break;
				}

	//==============================================================
	//             Przyciski odpowiedzialne kursor
	//==============================================================
				case RK_UP: {
					switch (cursor_mode) {
						case mode_red: {
							RGB_delta(10, 0, 0);
							break;
						}
						case mode_green: {
							RGB_delta(0, 10, 0);
							break;
						}
						case mode_blue: {
							RGB_delta(0, 0, 10);
							break;
						}
						case mode_lightness: {
							RGB_change_li(10);
							break;
						}
						case mode_fala: {
							RGB_global_max(10);
							break;
						}
						case mode_rand: {
							RGB_global_max(10);
							break;
						}
						case mode_xmas: {
							RGB_global_max(10);
							break;
						}
					}
					break;
				}

				case RK_DOWN: {
					switch (cursor_mode) {
						case mode_red: {
							RGB_delta(-10, 0, 0);
							break;
						}
						case mode_green: {
							RGB_delta(0, -10, 0);
							break;
						}
						case mode_blue: {
							RGB_delta(0, 0, -10);
							break;
						}
						case mode_lightness: {
							RGB_change_li(-10);
							break;
						}
						case mode_fala: {
							RGB_global_max(-10);
							break;
						}
						case mode_rand: {
							RGB_global_max(-10);
							break;
						}
						case mode_xmas: {
							RGB_global_max(-10);
							break;
						}
					}
					break;
				}

				case RK_LEFT: {
					switch (cursor_mode) {
						case mode_fala: {
							eff_change_cycle_fala(1);
							break;
						}
						case mode_rand: {
							eff_change_cycle_rand(1);
							break;
						}
						case mode_xmas: {
							eff_change_cycle_xmas(1);
							break;
						}
					}
					break;
				}

				case RK_RIGHT: {
					switch (cursor_mode) {
						case mode_fala: {
							eff_change_cycle_fala(-1);
							break;
						}
						case mode_rand: {
							eff_change_cycle_rand(-1);
							break;
						}
						case mode_xmas: {
							eff_change_cycle_xmas(-1);
							break;
						}
					}
					break;
				}

				case RK_POWER: {
					fun_to_exec = eff_null;
					cursor_mode = mode_null;
					pwmr_cnt = 0;
					pwmg_cnt = 0;
					pwmb_cnt = 0;

					break;
				}

			}

			// Dodatkowa informacja o otrzymanej komendzie
			/*itoa(eff_rand_cycle,tab,10);
			 usart_str("\r\nrand_cyc:");
			 usart_str(tab);
			 itoa(pwmg_cnt,tab,10);
			 usart_str(" g:");
			 usart_str(tab);
			 itoa(pwmb_cnt,tab,10);
			 usart_str(" b:");
			 usart_str(tab);
			 usart_str("\r\n");
			 itoa(r_h,tab,10);
			 usart_str("rh:");
			 usart_str(tab);
			 itoa(g_h,tab,10);
			 usart_str(" gh:");
			 usart_str(tab);
			 itoa(b_h,tab,10);
			 usart_str(" bh:");
			 usart_str(tab);
			 usart_str("\r\n");
			 itoa(lightness,tab,10);
			 usart_str(" lightness:");
			 usart_str(tab);
			 usart_str("\r\n");
			 */
		}

//==============================================================
//                      Wywolanie funkcji
//==============================================================
		exec_eff(fun_to_exec);


		 //Funkcja odpowiedzialna za odbior ramki
		 if(odebrano_ramke==1){
			fun_to_exec=eff_null;

			command=0;

			odebrano_ramke=0;

			pwmr_cnt=atol(ramka+1);
			i=1;
			while(ramka[i]!='G') i++;
			pwmg_cnt=atol(ramka+i+1);
			while(ramka[i]!='B') i++;
			pwmb_cnt=atol(ramka+i+1);
		 }
	}
}

void exec_eff(void (*fun)(void)) {
	cli();
	tim_1 = tim_1_isr;
	sei();

	fun();
}
