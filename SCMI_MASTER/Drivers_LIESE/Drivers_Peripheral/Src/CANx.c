/*
 * CANx.c
 *
 *  Created on: 23 ene 2023
 *      Author: jurl9
 */

#include "CANx.h"
#include "RCC.h"


void CANx_GPIO(GPIO_TypeDef *Port_, uint8_t Pin_){
	RCC_EnPort(Port_);
	GPIOx_InitAF(Port_, Pin_, GPIO_OTYPER_OD, GPIO_OSPEEDR_HS, GPIO_MODER_MODE_AF);
}

void CANx_Init(CAN_Handler * canBus, CAN_FilterTypeDef * fltr, bool dual_mode, uint8_t nofltrCANslave,  uint8_t nofltrArray){

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

	CANx_BitTiming(canBus, 1000, 16, 0);

	CANx_CfgFilters(canBus, fltr, dual_mode, nofltrCANslave, nofltrArray);

	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_INRQ);//Initialization request off

}

/*
 * indexFltr: Select 0-27 filter to configure
 * bitscale: 16 or 32 bits scale filter
 * ID: identifier filter
 * Mask: Mask filter
 * modeFltr: Select between Identifier Mask Mode or Identifier List Mode
 * FIFO: Select in which FIFO will be stored data when filter achieves
 */
void CANx_SetCfgFilter(CAN_Handler * canBus, CAN_FilterTypeDef * fltr){
	//SET_BIT(canBus->Register->FMR, CAN_FMR_FINIT);//Initialization mode for the filters.
	CLEAR_BIT(canBus->Register->FA1R, (1UL<<fltr->indexFltr));//Desactive Filter
	SET_BIT(canBus->Register->FS1R, (fltr->bitscale&1UL)<<fltr->indexFltr);//Dual 16 bits scale or Single 32 bits scale, for all 28 filters
	SET_BIT(canBus->Register->FiR[fltr->indexFltr].FiR1, fltr->ID);//ID for 16 or 32 bits scale
	SET_BIT(canBus->Register->FiR[fltr->indexFltr].FiR2, fltr->Mask);//Mask for 16 or 32 bits scale, 0->Not compare, 1->Compare
	SET_BIT(canBus->Register->FM1R, (fltr->modeFltr&1UL)<<fltr->indexFltr);//Filter modes. 0: Two 32-bit registers of filter bank x are in Identifier Mask mode.
	//1: registers of filter bank x are in Identifier List mode.
	SET_BIT(canBus->Register->FFA1R, (fltr->FIFO&1UL)<<fltr->indexFltr);// assigned to FIFO 0 or 1
	SET_BIT(canBus->Register->FA1R, 1UL<<fltr->indexFltr);//active Filter
	//CLEAR_BIT(canBus->Register->FMR, CAN_FMR_FINIT);//Initialization mode off
}

void CANx_CfgFilters(CAN_Handler * canBus, CAN_FilterTypeDef * fltr, bool dual_mode, uint8_t nofltrCANslave,  uint8_t nofltrArray){

	uint8_t i=0;

	SET_BIT(canBus->Register->FMR, CAN_FMR_FINIT);//Initialization mode for the filters.

	for (i = 0; i < nofltrArray; ++i) {
		CANx_SetCfgFilter(canBus, &fltr[i]);
	}

	if(!dual_mode){//Un solo módulo CAN para usar

		SET_BIT(canBus->Register->FMR, (28<<CAN_FMR_CAN2SB_Pos));//All 28 filters managed by one can

	}
	else{
		SET_BIT(canBus->Register->FMR, (nofltrCANslave<<CAN_FMR_CAN2SB_Pos));//filters managed by can slave
	}

	CLEAR_BIT(canBus->Register->FMR, CAN_FMR_FINIT);//Initialization mode off
}

/*
 * kbps Data bit rate Kilobits/seg
 * ntq number of time quanta
 * SJW value for resynchronization
 */
bool CANx_BitTiming(CAN_Handler * canBus, uint16_t kbps, uint8_t ntq, uint8_t SJW){

	bool flag=true;
	uint8_t nt1t2= ntq - 1; //Define total time for tSeg1+tSeg2
	uint8_t nt1, nt2; //Segment 1 and segment 2. Maximum value Segment 2: Value programmable (8-1)=7
	uint16_t fq ;//fq=1/tq -> tq = tbits/ntq
	uint16_t BRP ;//(ClockFreq/fq) - 1

	if(ntq > 25){//Supera el máximo número de tiempo de cuantización
		ntq = 25; //Colocamos el máximo valor por defecto
	}

	if(kbps>1000){//Máxima tasa de transferencia 1Mbps CAN 2.0
		kbps = 1000; //1Mbps
	}

	fq = kbps * ntq;//Calculando frecuencia del tiempo cuántico
	BRP = ((currentAHB1CLK*1000) / (fq)) - 1;//Baud rate prescaler

	if((BRP<0)||(BRP>1023)){
		flag=false;//Can't preformance bit rate with Clock frequency
	}
	else{
		flag=true;
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
			SET_BIT(canBus->Register->BTR , (SJW<<CAN_BTR_SJW_Pos));
		}
		else{
			CLEAR_BIT(canBus->Register->BTR , (3<<CAN_BTR_SJW_Pos));
		}
	}

	return flag;

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

