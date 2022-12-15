/*
 * SYSTICK.h
 *
 *  Created on: 2 dic 2022
 *      Author: jurl9
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>

#define STK_BASE                          (0xE000E010UL)

#define STK_CTRL                          (*((volatile unsigned int *)(STK_BASE + 0x0UL)))
#define STK_LOAD                          (*((volatile unsigned int *)(STK_BASE + 0x4UL)))

#define STK_CTRL_ENABLE_MSK               (1UL)
#define STK_CTRL_ENABLE_POS               (0UL)
#define STK_CTRL_ENABLE_SET               (STK_CTRL_ENABLE_MSK<<STK_CTRL_ENABLE_POS)

#define STK_CTRL_TICKINT_MSK              (1UL)
#define STK_CTRL_TICKINT_POS              (1UL)
#define STK_CTRL_TICKINT_SET              (STK_CTRL_TICKINT_MSK<<STK_CTRL_TICKINT_POS)

#define STK_CTRL_CLKSOURCE_MSK            (1UL)
#define STK_CTRL_CLKSOURCE_POS            (2UL)
#define STK_CTRL_CLKSOURCE_SET            (STK_CTRL_CLKSOURCE_MSK<<STK_CTRL_CLKSOURCE_POS)

void STK_CONF(uint8_t CLK_Freq);

#endif /* SYSTICK_H_ */
