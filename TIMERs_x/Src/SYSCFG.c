/*
 * SYSCFG.c
 *
 *  Created on: 1 dic 2022
 *      Author: jurl9
 */
#include "SYSCFG.h"
//I/O Compensation Cell enabled
void SYSCFG_SetCMP_PD(){
	SYSCFG_CMPCR |= SYSCFG_CMP_PD_EN;
}

void SYSCFG_WaitREADY(){
	while((SYSCFG_CMPCR&SYSCFG_READY_0)==0);
}
