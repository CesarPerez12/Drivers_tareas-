/*
 * CANx.c
 *
 *  Created on: 23 ene 2023
 *      Author: jurl9
 */

#include "CANx.h"
#include "GPIOx.h"
#include "RCC.h"

void CANx_GPIO(GPIO_TypeDef *Port_, uint8_t Pin_){
	RCC_EnPort(Port_);
	GPIOx_InitAF(Port_, Pin_, GPIO_OTYPER_OD, GPIO_OSPEEDR_HS, GPIO_MODER_MODE_AF);
}

void CANx_Init(CAN_Handler * canBus){

	RCC_APB1ENR |= RCC_APB1ENR_CAN1EN | RCC_APB1ENR_CAN2EN;

	canBus->Register->MCR |= CAN_MCR_INRQ;//Initialization request
	canBus->Register->MCR &= ~CAN_MCR_TXFP;//Priority driven by the identifier of the message
	canBus->Register->MCR &= ~CAN_MCR_RFLM;//Receive FIFO not locked on overrun.
	canBus->Register->MCR &= ~CAN_MCR_NART;//The CAN hardware will automatically retransmit the message until it has been
	//successfully transmitted according to the CAN standard.
	canBus->Register->MCR &= ~CAN_MCR_AWUM;//The Sleep mode is left on software request
	canBus->Register->MCR &= ~CAN_MCR_ABOM;//The Bus-Off state is left on software request,
	canBus->Register->MCR &= ~CAN_MCR_TTCM;//Time Triggered Communication mode disabled.
	canBus->Register->MCR &= ~CAN_MCR_RESET;//Normal operation.
	canBus->Register->MCR &= ~CAN_MCR_DBF;//CAN working during debug

}
