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

//NVIC Position

//NVIC Base
#define NVIC_BASE      0xE000E000

//NVIC offset 	       Indica el offset para cada registro de interrupción y su respectivo rango de vectores.  	
#define R_0            0x00  //0-31
#define R_1            0x04  //32-63
#define R_2            0x08  //64-95
#define R_3            0x0C  //96-127
#define R_4            0x10  //128-159
#define R_5            0x14  //160-191
#define R_6            0x18  //191-223
#define R_7            0x1C  //223-239

//NVIC Offset base     
#define NVIC_ISER      0x100 // 
#define NVIC_ICER      0x180 //
#define NVIC_ISPR      0x200 //
#define NVIC_ICPR      0x280 //
#define NVIC_IABR      0x300 //
#define NVIC_IPR       0x400 //
#define NVIC_STIRx     0xE00 //

//NVIC Registers
#define NVIC_ISER0     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_0) ) )
#define NVIC_ISER1     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_1) ) )
#define NVIC_ISER2     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_2) ) )
#define NVIC_ISER3     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_3) ) )
#define NVIC_ISER4     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_4) ) )
#define NVIC_ISER5     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_5) ) )
#define NVIC_ISER6     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_6) ) )
#define NVIC_ISER7     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISER + R_7) ) )
//--------------------------------------------------------------------------------------
#define NVIC_ICER0     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_0) ) )
#define NVIC_ICER1     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_1) ) )
#define NVIC_ICER2     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_2) ) )
#define NVIC_ICER3     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_3) ) )
#define NVIC_ICER4     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_4) ) )
#define NVIC_ICER5     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_5) ) )
#define NVIC_ICER6     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_6) ) )
#define NVIC_ICER7     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICER + R_7) ) )
//--------------------------------------------------------------------------------------
#define NVIC_ISPR0     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_0) ) )
#define NVIC_ISPR1     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_1) ) )
#define NVIC_ISPR2     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_2) ) )
#define NVIC_ISPR3     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_3) ) )
#define NVIC_ISPR4     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_4) ) )
#define NVIC_ISPR5     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_5) ) )
#define NVIC_ISPR6     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_6) ) )
#define NVIC_ISPR7     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ISPR + R_7) ) )
//--------------------------------------------------------------------------------------
#define NVIC_ICPR0     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_0) ) )
#define NVIC_ICPR1     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_1) ) )
#define NVIC_ICPR2     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_2) ) )
#define NVIC_ICPR3     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_3) ) )
#define NVIC_ICPR4     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_4) ) )
#define NVIC_ICPR5     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_5) ) )
#define NVIC_ICPR6     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_6) ) )
#define NVIC_ICPR7     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_ICPR + R_7) ) )
//--------------------------------------------------------------------------------------
#define NVIC_IABR0     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_0) ) )
#define NVIC_IABR1     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_1) ) )
#define NVIC_IABR2     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_2) ) )
#define NVIC_IABR3     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_3) ) )
#define NVIC_IABR4     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_4) ) )
#define NVIC_IABR5     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_5) ) )
#define NVIC_IABR6     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_6) ) )
#define NVIC_IABR7     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IABR + R_7) ) )
//------------------------------------------------------------------------------------
#define NVIC_IPR0     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_0) ) )
#define NVIC_IPR1     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_1) ) )
#define NVIC_IPR2     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_2) ) )
#define NVIC_IPR3     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_3) ) )
#define NVIC_IPR4     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_4) ) )
#define NVIC_IPR5     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_5) ) )
#define NVIC_IPR6     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_6) ) )
#define NVIC_IPR7     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + R_7) ) )
#define NVIC_IPR8     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0x20) ) )
#define NVIC_IPR9     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0x24) ) )
#define NVIC_IPR10     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0x28) ) )
#define NVIC_IPR11     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0x2C) ) )
#define NVIC_IPR12     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X30) ) )
#define NVIC_IPR13     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X34) ) )
#define NVIC_IPR14     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X38) ) )
#define NVIC_IPR15     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X3C) ) )
#define NVIC_IPR16     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X40) ) )
#define NVIC_IPR17     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X44) ) )
#define NVIC_IPR18     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X48) ) )
#define NVIC_IPR19     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X4C) ) )
#define NVIC_IPR20     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X50) ) )
#define NVIC_IPR21     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X54) ) )
#define NVIC_IPR22     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X58) ) )
#define NVIC_IPR23     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X5C) ) )
#define NVIC_IPR24     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X60) ) )
#define NVIC_IPR25     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X64) ) )
#define NVIC_IPR26     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X68) ) )
#define NVIC_IPR27     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X6C) ) )
#define NVIC_IPR28     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X70) ) )
#define NVIC_IPR29     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X74) ) )
#define NVIC_IPR30     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X78) ) )
#define NVIC_IPR31     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X7C) ) )
#define NVIC_IPR32     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X80) ) )
#define NVIC_IPR33     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X84) ) )
#define NVIC_IPR34     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X88) ) )
#define NVIC_IPR35     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X8C) ) )
#define NVIC_IPR36     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X90) ) )
#define NVIC_IPR37     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X94) ) )
#define NVIC_IPR38     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X98) ) )
#define NVIC_IPR39     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0X9C) ) )
#define NVIC_IPR40     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XA0) ) )
#define NVIC_IPR41     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XA4) ) )
#define NVIC_IPR42     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XA8) ) )
#define NVIC_IPR43     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XAC) ) )
#define NVIC_IPR44     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XB0) ) )
#define NVIC_IPR45     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XB4) ) )
#define NVIC_IPR46     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XB8) ) )
#define NVIC_IPR47     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XBC) ) )
#define NVIC_IPR48     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XC0) ) )
#define NVIC_IPR49     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XC4) ) )
#define NVIC_IPR50     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XC8) ) )
#define NVIC_IPR51     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XCC) ) )
#define NVIC_IPR52     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XD0) ) )
#define NVIC_IPR53     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XD4) ) )
#define NVIC_IPR54     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XD8) ) )
#define NVIC_IPR55     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XDC) ) )
#define NVIC_IPR56     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XE0) ) )
#define NVIC_IPR57     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XE4) ) )
#define NVIC_IPR58     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XE8) ) )
#define NVIC_IPR59     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_IPR + 0XEC) ) )
//------------------------------------------------------------------------------------
#define NVIC_STIR     (*( ( volatile unsigned int * ) (NVIC_BASE + NVIC_STIRx) ) )


//Función para asignar prioridad. 
void NVIC_SetCFGR(uint8_t position, uint8_t priority);

#endif /* NVIC_H_ */
