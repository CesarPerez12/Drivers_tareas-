/*
 * NVIC.c
 *
 *  Created on: 15 nov 2022
 *      Author: jurl9
 */

#include "NVIC.h"

/*
 * NVIC Configuration
 */
void NVIC_SetCFGR(uint8_t position, uint8_t priority){
	uint8_t div1 = position / 32, div2 = position / 4;
	unsigned int * NVIC_ICERn = ( ( unsigned int * ) (NVIC_BASE + NVIC_ICER + (0x04*div1)) )  ;
	unsigned int * NVIC_ISERn = ( ( unsigned int * ) (NVIC_BASE + NVIC_ISER + (0x04*div1)) )  ;
	unsigned int * NVIC_IPRn = ( (  unsigned int * ) (NVIC_BASE + NVIC_IPR + (0x04*div2)) )  ;

	while(div1>0){
		position -= 32;
		div1--;
	}
	div1=4;
	while(div1>0){
		div2 = ( ( 10 * (position - div1 + 1)) /4 ) ;//Obtenemos el valor con decimales
		div2 = div2 - ( 10 * ( (position - div1 + 1) / 4) );//Recuperamos el decimal
		if(div2){//No corresponde a la posición
			div1--; //Seguimos bucando coincidencia
		}
		else{//Coincidencia de posición
			div2=div1-1;//Recuperamos la posición
			div1=0;
		}
	}

	//*NVIC_ICERn |= (1<<position);//Limpia posible bandera pendiente, deshabilitando
	*NVIC_ISERn |= (1<<position);//Habilita la interrupción
	*NVIC_IPRn|= ((priority<<(div2*8))<<4);//Coloca Prioridad 4*x+3; x= 8 y 7 I2C1
	//Únicamente se usan los bits 4-7 para la prioridad
}
 
