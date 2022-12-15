/*
 * GPIOx.c
 *
 *  Created on: 15 nov 2022
 *      Author: jurl9
 */
#include "GPIOx.h"


//Inicializa en la función alterna los puertos
void GPIOx_InitAF(GPIO_TypeDef *Port_, uint8_t Pin_, uint8_t OTYPER_, uint8_t AFR_ ){
	Port_->MODER |= (GPIO_MODER_MODE_AF<<(Pin_*2));//Alternate Function on Pin x=1,...,15
	Port_->OTYPER |= ((OTYPER_&1)<<(Pin_));//Opend Drain or push pull
	Port_->OSPEEDR |= (GPIO_OSPEEDR_HS<<(Pin_*2));//High Speed
	if(Pin_>7){
		Port_->AFRH |= (AFR_<<((Pin_-8)*4));
	}
	else{
		Port_->AFRL |= (AFR_<<(Pin_*4));
	}
	//Veificar IDR? Debe estar en 1 los puertos
}

