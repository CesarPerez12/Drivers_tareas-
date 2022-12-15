/*
 * NVIC.h
 *
 *  Created on: 15 nov 2022
 *      Author: jurl9
 */

#ifndef NVIC_H_
#define NVIC_H_

#include <stdint.h>
#include <stdbool.h>

//Definición de poición para Interrupciones
//NVIC Position
#define NVIC_I2C1EV_POSITION     31
#define NVIC_I2C1ER_POSITION     32
#define NVIC_I2C2EV_POSITION     33
#define NVIC_I2C2ER_POSITION     34
#define NVIC_I2C3EV_POSITION     72
#define NVIC_I2C3ER_POSITION     73
//Bit position
#define NVIC_I2C1EV_ICPR0_POS     31
#define NVIC_I2C1ER_ICPR1_POS      0
#define NVIC_I2C2EV_ICPR1_POS      1
#define NVIC_I2C2ER_ICPR1_POS      2
#define NVIC_I2C3EV_ICPR2_POS      8
#define NVIC_I2C3ER_ICPR2_POS      9

//NVIC Base
#define NVIC_BASE      0xE000E000
//NVIC offset
#define R_0            0x00  //0-31
#define R_1            0x04  //32-63
#define R_2            0x08  //64-95
#define R_3            0x0C  //96-127
#define R_4            0x10  //128-159
#define R_5            0x14  //160-191
#define R_6            0x18  //191-223
#define R_7            0x1C  //223-239
//NVIC Offset base
#define NVIC_ISER      0x100
#define NVIC_ICER      0x180
#define NVIC_ISPR      0x200
#define NVIC_ICPR      0x280
#define NVIC_IABR      0x300
#define NVIC_IPR       0x400
//NVIC Registers
#define NVIC_ISER0     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_0) ) )
#define NVIC_ISER1     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_1) ) )
#define NVIC_ISER2     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_2) ) )
#define NVIC_ISER3     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_3) ) )
#define NVIC_ISER4     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_4) ) )
#define NVIC_ISER5     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_5) ) )
#define NVIC_ISER6     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_6) ) )
#define NVIC_ISER7     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_7) ) )
//
#define NVIC_ICER0     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_0) ) )
#define NVIC_ICER1     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_1) ) )
#define NVIC_ICER2     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_2) ) )
#define NVIC_ICER3     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_3) ) )
#define NVIC_ICER4     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_4) ) )
#define NVIC_ICER5     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_5) ) )
#define NVIC_ICER6     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_6) ) )
#define NVIC_ICER7     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_7) ) )
//
#define NVIC_ISPR0     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_0) ) )
#define NVIC_ISPR1     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_1) ) )
#define NVIC_ISPR2     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_2) ) )
#define NVIC_ISPR3     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_3) ) )
#define NVIC_ISPR4     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_4) ) )
#define NVIC_ISPR5     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_5) ) )
#define NVIC_ISPR6     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_6) ) )
#define NVIC_ISPR7     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_7) ) )
//
#define NVIC_ICPR0     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_0) ) )
#define NVIC_ICPR1     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_1) ) )
#define NVIC_ICPR2     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_2) ) )
#define NVIC_ICPR3     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_3) ) )
#define NVIC_ICPR4     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_4) ) )
#define NVIC_ICPR5     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_5) ) )
#define NVIC_ICPR6     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_6) ) )
#define NVIC_ICPR7     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_7) ) )
//
#define NVIC_IABR0     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_0) ) )
#define NVIC_IABR1     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_1) ) )
#define NVIC_IABR2     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_2) ) )
#define NVIC_IABR3     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_3) ) )
#define NVIC_IABR4     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_4) ) )
#define NVIC_IABR5     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_5) ) )
#define NVIC_IABR6     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_6) ) )
#define NVIC_IABR7     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_7) ) )
//
#define NVIC_IPR0     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_0) ) )
#define NVIC_IPR1     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_1) ) )
#define NVIC_IPR2     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_2) ) )
#define NVIC_IPR3     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_3) ) )
#define NVIC_IPR4     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_4) ) )
#define NVIC_IPR5     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_5) ) )
#define NVIC_IPR6     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_6) ) )
#define NVIC_IPR7     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_7) ) )
#define NVIC_IPR8     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0x20) ) )

//
void NVIC_SetCFGR(uint8_t position, uint8_t priority);

#endif /* NVIC_H_ */
