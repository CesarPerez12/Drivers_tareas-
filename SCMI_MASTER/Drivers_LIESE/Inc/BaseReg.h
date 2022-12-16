/*
 * BaseReg.h
 *
 *  Created on: 15 nov 2022
 *      Author: jurl9
 */

#ifndef BASEREG_H_
#define BASEREG_H_

//PERIPHERAL BASE REGISTERS
#define PERIPH_BASE           0x40000000UL // Peripheral base address                               */

#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x10000000UL)


#endif /* BASEREG_H_ */
