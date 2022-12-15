/*
 * SYSCFG.h
 *
 *  Created on: 1 dic 2022
 *      Author: jurl9
 */

#ifndef SYSCFG_H_
#define SYSCFG_H_
#include "BaseReg.h"
#include <stdint.h>

#define SYSCFG_BASE                  (0x40013800UL)

#define SYSCFG_CMPCR                 (*( ( volatile unsigned int * ) (SYSCFG_BASE + 0x20UL)  ) )

#define SYSCFG_CMP_PD_0              (1UL)
#define SYSCFG_CMP_PD_POS            (0UL)
#define SYSCFG_CMP_PD_EN             (SYSCFG_CMP_PD_0<<SYSCFG_CMP_PD_POS)

#define SYSCFG_READY                 (1UL)
#define SYSCFG_READY_POS             (8UL)
#define SYSCFG_READY_0               (SYSCFG_READY<<SYSCFG_READY_POS)

void SYSCFG_SetCMP_PD();
void SYSCFG_WaitREADY();

#endif /* SYSCFG_H_ */
