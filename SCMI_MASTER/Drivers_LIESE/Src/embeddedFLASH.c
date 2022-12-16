/*
 * embeddedFLASH.c
 *
 *  Created on: 2 dic 2022
 *      Author: jurl9
 */


#include "embeddedFLASH.h"
//AHBCLK MHz
void embbFLash_Conf(uint8_t AHBCLK){
	uint8_t latency;
	FLASH_ACR |= (FLASH_ACR_DCEN_SET|FLASH_ACR_ICEN_SET|FLASH_ACR_PRFTEN_SET);
	//Number of wait states according to CPU clock (HCLK) frequency, Voltage Range 2.7[V]-3.6[V]
	if((AHBCLK>0)&&(AHBCLK<=30)){
		latency=0;
	}
	else if((AHBCLK>30)&&(AHBCLK<=60)){
		latency=1;
	}
	else if((AHBCLK>60)&&(AHBCLK<=90)){
		latency=2;
	}
	else if((AHBCLK>90)&&(AHBCLK<=120)){
		latency=3;
	}
	else if((AHBCLK>120)&&(AHBCLK<=150)){
		latency=4;
	}
	else if((AHBCLK>150)&&(AHBCLK<=180)){
		latency=5;
	}
	FLASH_ACR |= latency;
}
