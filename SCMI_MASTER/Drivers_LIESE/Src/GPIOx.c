/*
 * GPIOx.c
 *
 *  Created on: 15 nov 2022
 *      Author: jurl9
 */
#include "GPIOx.h"
#include "RCC.h"


//Inicializa en la función alterna los puertos
void GPIOx_InitAF(GPIO_TypeDef *Port_, uint8_t Pin_, uint8_t OTYPER_, uint8_t OSPEEDR_, uint8_t AFR_ ){
	Port_->MODER |= (GPIO_MODER_MODE_AF<<(Pin_*2));//Alternate Function on Pin x=1,...,15
	Port_->OTYPER |= ((OTYPER_&1)<<(Pin_));//Opend Drain or push pull
	Port_->OSPEEDR |= (OSPEEDR_<<(Pin_*2));//High Speed
	if(Pin_>7){
		Port_->AFRH |= (AFR_<<((Pin_-8)*4));
	}
	else{
		Port_->AFRL |= (AFR_<<(Pin_*4));
	}
	//Veificar IDR? Debe estar en 1 los puertos
}

void GPIOx_InitIO(GPIO_TypeDef *Port_, uint8_t Pin_, uint8_t IO, bool PUR){
	if(IO==GPIO_MODER_OUTPUT){
		Port_->MODER |= (GPIO_MODER_OUTPUT<<(Pin_*2)); // Salida
	}
	else{
		Port_->MODER &= ~(0x3<<(Pin_*2)); // Entrada
	}
	if(PUR){
		Port_->PUPDR |= (GPIO_PUPDR_PUPD0_0<<(Pin_*2));//Resistencias Pull UP
	}
	else{
		Port_->PUPDR &= (~(GPIO_PUPDR_PUPD0)<<(Pin_*2));//Desactiva
	}
}

void RCC_EnPort(GPIO_TypeDef *port){
	if(port==GPIOA){
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN ;
	}
	else if(port==GPIOB){
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN ;
	}
	else if(port==GPIOC){
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN ;
	}
	else if(port==GPIOD){
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIODEN ;
	}
	else if(port==GPIOE){
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIOEEN ;
	}
	else if(port==GPIOF){
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIOGEN ;
	}
	else if(port==GPIOH){
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIOHEN ;
	}
}
