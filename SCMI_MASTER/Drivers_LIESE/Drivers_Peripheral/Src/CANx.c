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

void CANx_ConfFilter(CAN_Handler * canBus, bool dual_mode, uint32_t ID, uint32_t Mask){

	if(!dual_mode){//Un solo módulo CAN para usar
		canBus->Register->FMR |= CAN_FMR_FINIT;//Initialization mode for the filters.
		canBus->Register->FMR &= ~(0x3F<<CAN_FMR_CAN2SB_Pos);//All 28 filters managed by one can
		canBus->Register->FA1R &= ~0xFFFFFFF;//Desactive Filters
		canBus->Register->FS1R |= CAN_FS1R_FSC|CAN_FS1R_FSC10; //Single 32 bits scale, for all 28 filters
		canBus->Register->FiR[1].FiR1 |= ID;//ID for 32 bits scale
		canBus->Register->FiR[1].FiR2 |= Mask;//Mask for 32 bits scale, 1->Compare
		canBus->Register->FM1R &= ~0xFFFFFFF; //Filter mode. 0: Two 32-bit registers of filter bank x are in Identifier Mask mode.
		canBus->Register->FFA1R &= ~0xFFFFFFF;// assigned to FIFO 0
		canBus->Register->FA1R |= 0xFFFFFFF;//active Filters
		canBus->Register->FMR &= ~CAN_FMR_FINIT;//Initialization mode off
	}
}


//MAILBOX Rgister
