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

void CANx_Init(CAN_Handler * canBus, bool dual_mode){

	if(dual_mode){
		SET_BIT(RCC_APB1ENR , RCC_APB1ENR_CAN1EN | RCC_APB1ENR_CAN2EN);
	}
	else{//One BusCan used
		if(canBus->Register==CAN1){
			SET_BIT(RCC_APB1ENR , RCC_APB1ENR_CAN1EN);

		}
		else if(canBus->Register==CAN2){
			SET_BIT(RCC_APB1ENR , RCC_APB1ENR_CAN2EN);
		}
	}

	SET_BIT(canBus->Register->MCR, CAN_MCR_INRQ);//Initialization request
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_TXFP);//Priority driven by the identifier of the message
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_RFLM);//Receive FIFO not locked on overrun.
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_NART);//The CAN hardware will automatically retransmit the message until it has been
	//successfully transmitted according to the CAN standard.
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_AWUM);//The Sleep mode is left on software request
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_ABOM);//The Bus-Off state is left on software request,
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_TTCM);//Time Triggered Communication mode disabled.
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_RESET);//Normal operation.
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_DBF);//CAN working during debug

}

void CANx_CfgFilter(CAN_Handler * canBus, bool dual_mode, uint32_t ID, uint32_t Mask){

	if(!dual_mode){//Un solo módulo CAN para usar

		SET_BIT(canBus->Register->FMR, CAN_FMR_FINIT);//Initialization mode for the filters.
		CLEAR_BIT(canBus->Register->FMR, (0x3F<<CAN_FMR_CAN2SB_Pos));//All 28 filters managed by one can
		CLEAR_BIT(canBus->Register->FA1R, 0xFFFFFFF);//Desactive Filters
		SET_BIT(canBus->Register->FS1R, CAN_FS1R_FSC|CAN_FS1R_FSC10);//Single 32 bits scale, for all 28 filters
		SET_BIT(canBus->Register->FiR[1].FiR1, ID);//ID for 32 bits scale
		SET_BIT(canBus->Register->FiR[1].FiR2, Mask);//Mask for 32 bits scale, 1->Compare
		CLEAR_BIT(canBus->Register->FM1R, 0xFFFFFFF);//Filter mode. 0: Two 32-bit registers of filter bank x are in Identifier Mask mode.
		CLEAR_BIT(canBus->Register->FFA1R, 0xFFFFFFF);// assigned to FIFO 0
		SET_BIT(canBus->Register->FA1R, 0xFFFFFFF);//active Filters
		CLEAR_BIT(canBus->Register->FMR, CAN_FMR_FINIT);//Initialization mode off

	}
}

/*
 * Mbps Data bit rate Megabits/seg
 * ntq number of time quanta
 * SJW value for resynchronization
 */
void CANx_BitTiming(CAN_Handler * canBus, uint16_t Mbps, uint8_t ntq, uint8_t SJW){

	uint8_t nt1t2= ntq - 1; //Define total time for t1+t2 = 15
	uint8_t nt2; //Maximum value Segment 2: Value programmable (8-1)=7
	uint8_t nt1 = nt1t2 - nt2; //Segment 1
	uint8_t fq = Mbps * ntq;//fq=1/tq -> tq = tbits/ntq
	uint8_t BRP = (16 / fq) - 1 ;//(ClockFreq/fq) - 1

	if(ntq > 25){//Supera el máximo número de tiempo de cuantización
		ntq = 25; //Colocamos el máximo valor por defecto
	}

	nt2 = nt1t2 / 2;

	if(nt2>=8){
		nt2 = 8;//Máximo valor Segmento 2
		nt1 = nt1t2 - nt2;//Valor Segmento 1
	}
	else{
		nt1t2 = 2; //Rehusamos variables
		nt2-=nt1t2;//Decrementamos el valor
		nt1+=nt1t2;//Incrementamos el valor


		while((nt2<1)||(nt2>8)||(nt1t2==0)){
			nt1t2--;
			nt2-=nt1t2;
			nt1+=nt1t2;
		}

		if((nt2<1)||(nt2>8)){//Verificamos coincidencia
			nt2 = 1;//Mínimo valor Segmento 2
		    nt1 = nt1t2 - nt2;//Valor Segmento 1
		}

	}

	SET_BIT(canBus->Register->BTR , (BRP<<CAN_BTR_BRP_Pos));//Baud rate prescaler
	SET_BIT(canBus->Register->BTR , ((nt1-1)<<CAN_BTR_TS1_Pos));//number of time quanta for tS1
	SET_BIT(canBus->Register->BTR , ((nt2-1)<<CAN_BTR_TS2_Pos));//number of time quanta for tS2

	if(SJW){//Resynchronization


	}

}


//MAILBOX Configuration
//Trama de datos
void CANx_TxData(CAN_Handler * canBus){

	CLEAR_BIT(canBus->Register->MailBoxTx[1].TIxR, CAN_TI0R_IDE);//Standard identifier.
	CLEAR_BIT(canBus->Register->MailBoxTx[1].TIxR, CAN_TI1R_RTR);//Data frame
	SET_BIT(canBus->Register->MailBoxTx[2].TDTxR, 8);//Data length max 8 bytes
	SET_BIT(canBus->Register->MailBoxTx[2].TDLxR , 0xFFFFFFFF);//Low DATA
	SET_BIT(canBus->Register->MailBoxTx[2].TDHxR , 0xFFFFFFFF);//High DATA
	SET_BIT(canBus->Register->MailBoxTx[1].TIxR , CAN_TI0R_TXRQ);//Transmission request

}
//Trama Remota
void CANx_TxRemote(CAN_Handler * canBus){

	CLEAR_BIT(canBus->Register->MailBoxTx[1].TIxR, CAN_TI0R_IDE);//Standard identifier.
	SET_BIT(canBus->Register->MailBoxTx[1].TIxR, CAN_TI1R_RTR);//Remote frame
	SET_BIT(canBus->Register->MailBoxTx[2].TDTxR, 8);//Data length max 8 bytes
	SET_BIT(canBus->Register->MailBoxTx[1].TIxR , CAN_TI0R_TXRQ);//Transmission request

}

