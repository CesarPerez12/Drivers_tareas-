/*
 * SYSTICK.c
 *
 *  Created on: 2 dic 2022
 *      Author: jurl9
 */

#include "SYSTICK.h"

void STK_CONF(uint8_t CLK_Freq){
	STK_CTRL |= (STK_CTRL_CLKSOURCE_SET|STK_CTRL_ENABLE_SET|STK_CTRL_TICKINT_SET);
	STK_LOAD = (CLK_Freq * 1000) - 1;//1ms=LOAD/Freq; LOAD = (Freq[MHz]) * 1ms = Freq * 1 (10^3)
}

