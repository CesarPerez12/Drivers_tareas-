/*
 * CANx.c
 *
 *  Created on: 23 ene 2023
 *      Author: jurl9
 */

#include "CANx.h"
#include "RCC.h"

bool CAN1emptyTx[3], CAN2emptyTx[3];
uint8_t errorStatus;
uint32_t *buffRx;
CAN_Handler *can1, *can2;

void CANx_GPIO(GPIO_TypeDef *Port_, uint8_t Pin_){
	RCC_EnPort(Port_);
	GPIOx_InitAF(Port_, Pin_, GPIO_OTYPER_OD, GPIO_OSPEEDR_HS, GPIO_MODER_MODE_AF);
}

/*
 * dual_mode: Indicates if both CAN are used
 * nofltrCANslave: Number of filter for CAN2 as slave
 * nofltrArray: Number of filters to configure
 */
void CANx_Init(CAN_Handler *canBus, CAN_FilterTypeDef *fltr, CAN_BitTimingTypeDef *tq, bool dual_mode, uint8_t nofltrCANslave,  uint8_t nofltrArray){

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
	CANx_WaitSetFlag(&(canBus->Register->MSR), CAN_MSR_INAK);//Wait for confirmation

	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_TXFP);//Priority driven by the identifier of the message
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_RFLM);//Receive FIFO not locked on overrun.
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_NART);//The CAN hardware will automatically retransmit the message until it has been
	//successfully transmitted according to the CAN standard.
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_AWUM);//The Sleep mode is left on software request
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_ABOM);//The Bus-Off state is left on software request,
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_TTCM);//Time Triggered Communication mode disabled.
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_RESET);//Normal operation.
	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_DBF);//CAN working during debug

	CANx_BitTiming(canBus, tq);

	CANx_CfgFilters(canBus, fltr, dual_mode, nofltrCANslave, nofltrArray);

	CLEAR_BIT(canBus->Register->MCR, CAN_MCR_INRQ);//Initialization request off
	CANx_WaitResetFlag(&(canBus->Register->MSR), CAN_MSR_INAK);


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

	if(((canBus->Register->FS1R)&&((fltr->bitscale&1UL)<<fltr->indexFltr))){//32 bits scale
		SET_BIT(canBus->Register->FiR[fltr->indexFltr].FiR1, ((fltr->ID_L&0xFFFF)|((fltr->ID_H&0xFFFF)<<16)));//ID for 32 bits scale
		SET_BIT(canBus->Register->FiR[fltr->indexFltr].FiR2, ((fltr->Mask_L&0xFFFF)|((fltr->Mask_H&0xFFFF)<<16)));//Mask for 32 bits scale, 0->Not compare, 1->Compare
	}
	else{//Dual 16 bits scale: ID[15:0], MASK[16:31]
		SET_BIT(canBus->Register->FiR[fltr->indexFltr].FiR1, 0xFFFF&fltr->ID_L);//ID for 16 bits
		SET_BIT(canBus->Register->FiR[fltr->indexFltr].FiR1, 0xFFFF&fltr->Mask_L);//Mask for 16 bits scale

		SET_BIT(canBus->Register->FiR[fltr->indexFltr].FiR2,  0xFFFF&fltr->ID_H);//ID for 16 bits scale
		SET_BIT(canBus->Register->FiR[fltr->indexFltr].FiR2,  0xFFFF&fltr->Mask_H);//Mask for 16 bits scale, 0->Not compare, 1->Compare
	}

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
		CANx_SetCfgFilter(canBus, &fltr[i]);//Recorremos el arreglo de estructuras
	}

	if(!dual_mode){//Un solo módulo CAN para usar

		SET_BIT(canBus->Register->FMR, (28<<CAN_FMR_CAN2SB_Pos));//All 28 filters managed by one can

	}
	else{
		SET_BIT(canBus->Register->FMR, ((28-nofltrCANslave)<<CAN_FMR_CAN2SB_Pos));//filters managed by can slave
	}

	CLEAR_BIT(canBus->Register->FMR, CAN_FMR_FINIT);//Initialization mode off
}

/*
 * kbps Data bit rate Kilobits/seg
 * ntq number of time quanta
 * SJW value for resynchronization
 */
bool CANx_BitTiming(CAN_Handler * canBus, CAN_BitTimingTypeDef *tq){

	bool flag=true;
	uint8_t nt1t2= tq->ntq - 1; //Define total time for tSeg1+tSeg2
	uint8_t nt1, nt2; //Segment 1 and segment 2. Maximum value Segment 2: Value programmable (8-1)=7
	uint16_t fq ;//fq=1/tq -> tq = tbits/ntq
	uint16_t BRP ;//(ClockFreq/fq) - 1

	if(tq->ntq > 25){//Supera el máximo número de tiempo de cuantización
		tq->ntq = 25; //Colocamos el máximo valor por defecto
	}

	if(tq->kbps>1000){//Máxima tasa de transferencia 1Mbps CAN 2.0
		tq->kbps = 1000; //1Mbps
	}

	fq = tq->kbps * tq->ntq;//Calculando frecuencia del tiempo cuántico
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

		if(tq->SJW){//Resynchronization
			SET_BIT(canBus->Register->BTR , (tq->SJW<<CAN_BTR_SJW_Pos));
		}
		else{
			CLEAR_BIT(canBus->Register->BTR , (3<<CAN_BTR_SJW_Pos));
		}
	}

	return flag;

}


//MAILBOX Configuration
//Trama de datos
/*
 * DataH and DataL: Data to send
 * DLC: Data length code
 * ExID: Extended Identifier
 * indexMailBox: Number of Mailbox Tx to use
 */
void CANx_TxData(CAN_Handler * canBus, uint32_t ID, uint32_t DataL, uint32_t DataH, uint8_t DLC, bool ExID, uint8_t indexMailBox){

	if(ExID){
		SET_BIT(canBus->Register->MailBoxTx[indexMailBox].TIxR, CAN_TI0R_IDE);//Extended identifier.
		SET_BIT(canBus->Register->MailBoxTx[indexMailBox].TIxR, ID<<CAN_TI0R_EXID_Pos);//ID
	}
	else{
		CLEAR_BIT(canBus->Register->MailBoxTx[indexMailBox].TIxR, CAN_TI0R_IDE);//Standard identifier.
		SET_BIT(canBus->Register->MailBoxTx[indexMailBox].TIxR, ID<<CAN_TI0R_STID_Pos);//ID
	}
	CLEAR_BIT(canBus->Register->MailBoxTx[indexMailBox].TIxR, CAN_TI1R_RTR);//Data frame
	SET_BIT(canBus->Register->MailBoxTx[indexMailBox].TDTxR, DLC);//Data length max 8 bytes
	SET_BIT(canBus->Register->MailBoxTx[indexMailBox].TDLxR , DataL);//Low DATA
	SET_BIT(canBus->Register->MailBoxTx[indexMailBox].TDHxR , DataH);//High DATA
	SET_BIT(canBus->Register->MailBoxTx[indexMailBox].TIxR , CAN_TI0R_TXRQ);//Transmission request

	CANx_TxSuccess(&(canBus->Register->TSR), indexMailBox);
	//Success

}
//Trama Remota
void CANx_TxRemote(CAN_Handler * canBus, uint32_t ID, bool ExID, uint8_t indexMailBox){

	if(ExID){
		SET_BIT(canBus->Register->MailBoxTx[indexMailBox].TIxR, CAN_TI0R_IDE);//Extended identifier.
		SET_BIT(canBus->Register->MailBoxTx[indexMailBox].TIxR, ID<<CAN_TI0R_EXID_Pos);//ID
	}
	else{
		CLEAR_BIT(canBus->Register->MailBoxTx[indexMailBox].TIxR, CAN_TI0R_IDE);//Standard identifier.
		SET_BIT(canBus->Register->MailBoxTx[indexMailBox].TIxR, ID<<CAN_TI0R_STID_Pos);//ID
	}
	SET_BIT(canBus->Register->MailBoxTx[1].TIxR, CAN_TI1R_RTR);//Remote frame
	//SET_BIT(canBus->Register->MailBoxTx[2].TDTxR, DLC);//Data length max 8 bytes
	SET_BIT(canBus->Register->MailBoxTx[1].TIxR , CAN_TI0R_TXRQ);//Transmission request

	CANx_TxSuccess(&(canBus->Register->TSR), indexMailBox);
	//Success
}

uint8_t CANx_RxFIFO0(CAN_Handler * canBus, uint32_t * RxData ){
	if(canBus->Register->MailBoxFIFORx[0].RIxR && CAN_RI0R_IDE){//Extended Identifier
		RxData[0] = (canBus->Register->MailBoxFIFORx[0].RIxR & 0xFFFFFFF8)>>CAN_RI0R_EXID_Pos;
	}
	else{//Standard Identifier
		RxData[0] = (canBus->Register->MailBoxFIFORx[0].RIxR & 0xFFE00000)>>CAN_RI0R_STID_Pos;
	}

	RxData[1]=canBus->Register->MailBoxFIFORx[0].RDTxR & 0xF;//Data Length Code

	if(!(canBus->Register->MailBoxFIFORx[0].RIxR && CAN_RI0R_RTR)){//Data frame
		RxData[2]=canBus->Register->MailBoxFIFORx[0].RDLxR;
		RxData[3]=canBus->Register->MailBoxFIFORx[0].RDHxR;
		RxData[4]=(canBus->Register->MailBoxFIFORx[0].RDTxR&0xFF00)>>CAN_RDT0R_FMI_Pos;//Filter Match Index
	}
	else{
		RxData[2]= (canBus->Register->MailBoxFIFORx[0].RDTxR&0xFF00)>>CAN_RDT0R_FMI_Pos;//Filter Match Index
	}

	SET_BIT(canBus->Register->RF0R, CAN_RF0R_RFOM0);//Release FIFO output

	return canBus->Register->RF0R & CAN_RF0R_FMP0;//Return status

}

uint8_t CANx_RxFIFO1(CAN_Handler * canBus, uint32_t * RxData){
	if(canBus->Register->MailBoxFIFORx[1].RIxR && CAN_RI1R_IDE){//Extended Identifier
		RxData[0] = (canBus->Register->MailBoxFIFORx[1].RIxR & 0xFFFFFFF8)>>CAN_RI1R_EXID_Pos;
	}
	else{//Standard Identifier
		RxData[0] = (canBus->Register->MailBoxFIFORx[1].RIxR & 0xFFE00000)>>CAN_RI1R_STID_Pos;
	}

	RxData[1]=canBus->Register->MailBoxFIFORx[1].RDTxR & 0xF;//Data Length Code

	if(!(canBus->Register->MailBoxFIFORx[1].RIxR && CAN_RI1R_RTR)){//Data frame
		RxData[2]=canBus->Register->MailBoxFIFORx[1].RDLxR;
		RxData[3]=canBus->Register->MailBoxFIFORx[1].RDHxR;
		RxData[4]=(canBus->Register->MailBoxFIFORx[1].RDTxR&0xFF00)>>CAN_RDT1R_FMI_Pos;//Filter Match Index
	}
	else{
		RxData[2]= (canBus->Register->MailBoxFIFORx[1].RDTxR&0xFF00)>>CAN_RDT1R_FMI_Pos;//Filter Match Index
	}

	SET_BIT(canBus->Register->RF1R, CAN_RF1R_RFOM1);//Release FIFO output

	return canBus->Register->RF0R & CAN_RF1R_FMP1;//Return status
}

void CANx_BusOffRecovery(CAN_Handler * canBus){//Bit TEC>255
	if(CANx_GetError(canBus, CAN_ESR_BOFF)){
		SET_BIT(canBus->Register->MCR, CAN_MCR_ABOM);//SET ABOM: Recovering sequence automatic
	}
}

void CANx_CallBackRX0(CAN_Handler *can){
	if(can->Register->RF0R && CAN_RF0R_FMP0){//New Message
		CANx_RxFIFO0(can, buffRx);//Lee el nuevo mensaje
	}
	else if(can->Register->RF0R && CAN_RF0R_FULL0){//FULL FIFO
		while(CANx_RxFIFO0(can, buffRx)){//Lee los mensajes
			buffRx+=32;//Cambiamos de dirección
		}
	}
	else if(can->Register->RF0R && CAN_RF0R_FOVR0){//OVER FIFO
		while(CANx_RxFIFO0(can, buffRx)){//Lee los mensajes
			buffRx+=32;//Cambiamos de dirección
		}
	}
}

void CANx_CallBackRX1(CAN_Handler *can){
	if(can->Register->RF1R && CAN_RF1R_FMP1){//New Message
		CANx_RxFIFO1(can, buffRx);//Lee el nuevo mensaje
	}
	else if(can->Register->RF1R && CAN_RF1R_FULL1){//FULL FIFO
		while(CANx_RxFIFO1(can, buffRx)){//Lee los mensajes
			buffRx+=32;//Cambiamos de dirección
		}
	}
	else if(can->Register->RF1R && CAN_RF1R_FOVR1){//OVER FIFO
		while(CANx_RxFIFO1(can, buffRx)){//Lee los mensajes
			buffRx+=32;//Cambiamos de dirección
		}
	}
}

void CANx_CallBackSCE(CAN_Handler *can){
	if(CANx_GetError(can, CAN_ESR_EWGF)){
		errorStatus=1;//TEC or REC >= 96
	}
	else if(CANx_GetError(can, CAN_ESR_EPVF)){
		errorStatus=2;//TEC or REC >= 127
	}
	else if(CANx_GetError(can, CAN_ESR_BOFF)){
		errorStatus=3;//TEC or REC >= 255
		CANx_BusOffRecovery(can);//Enters in recovery mode
	}
	else if(CANx_GetError(can, CAN_ESR_LEC)){
		errorStatus=1;//Set error status
	}
	else if(can->Register->MSR&&CAN_MSR_WKUI){
		//Sleep mode
	}
	else if(can->Register->MSR&&CAN_MSR_SLAKI){
		//Wake up mode
	}
}

void CAN1_TX_IRQHandler(){
	/* CAN1 TX interrupts                                                 */
	//Becomes Empty
	if(CAN1->TSR && CAN_TSR_RQCP0){//Tx0 empty
		CAN1emptyTx[0]=true;
	}
	else if(CAN1->TSR && CAN_TSR_RQCP1){//Tx1 empty
		CAN1emptyTx[1]=true;
	}
	else if(CAN1->TSR && CAN_TSR_RQCP2){//Tx2 empty
		CAN1emptyTx[2]=true;
	}
}

void CAN1_RX0_IRQHandler(){
	/* CAN1 RX0 interrupts                                                */
	CANx_CallBackRX0(can1);

}
void CAN1_RX1_IRQHandler(){
	/* CAN1 RX1 interrupts                                                */
	CANx_CallBackRX1(can1);
}
void CAN1_SCE_IRQHandler(){
	/* CAN1 SCE interrupt                                                 */
	CANx_CallBackSCE(can1);
}

void CAN2_TX_IRQHandler(){
	/* CAN2 TX interrupts                                                 */
	//Becomes Empty
	if(CAN2->TSR && CAN_TSR_RQCP0){//Tx0 empty
		CAN2emptyTx[0]=true;
	}
	else if(CAN2->TSR && CAN_TSR_RQCP1){//Tx1 empty
		CAN2emptyTx[1]=true;
	}
	else if(CAN2->TSR && CAN_TSR_RQCP2){//Tx2 empty
		CAN2emptyTx[2]=true;
	}
}
void CAN2_RX0_IRQHandler(){
	/* CAN1 RX0 interrupts                                                */
	CANx_CallBackRX0(can2);
}
void CAN2_RX1_IRQHandler(){
	/* CAN1 RX1 interrupts                                                */
	CANx_CallBackRX1(can2);
}
void CAN2_SCE_IRQHandler(){
	/* CAN1 SCE interrupt                                                 */
	CANx_CallBackSCE(can2);
}

void CANx_SetInt(CAN_Handler * canBus, uint32_t bitReg){
	SET_BIT(canBus->Register->IER, bitReg);
}

void CANx_ResetInt(CAN_Handler * canBus, uint32_t bitReg){
	CLEAR_BIT(canBus->Register->IER, bitReg);
}

uint32_t CANx_GetError(CAN_Handler * canBus, uint32_t bitReg){
	return canBus->Register->ESR & bitReg;
}

bool CANx_TxSuccess(volatile uint32_t *SR, uint8_t indexMailBox){
	bool flag=true;
	if(indexMailBox == 0){
		while((!((*SR) & CAN_TSR_RQCP0))||(!((*SR) & CAN_TSR_TXOK0))||(!((*SR) & CAN_TSR_ALST0))||(!((*SR) & CAN_TSR_TERR0)));//Espera alguna bandera

		if(((*SR) & CAN_TSR_ALST0)||((*SR) & CAN_TSR_TERR0)){//Error de transmisión
			flag = false;
		}
		else if(((*SR) & CAN_TSR_RQCP0)){
			while(((*SR) & CAN_TSR_TXOK0));
			flag=true;
		}
		else if(((*SR) & CAN_TSR_TXOK0)){
			while(((*SR) & CAN_TSR_RQCP0));
			flag=true;
		}

	}
	else if(indexMailBox ==1){
		while((!((*SR) & CAN_TSR_RQCP1))||(!((*SR) & CAN_TSR_TXOK1))||(!((*SR) & CAN_TSR_ALST1))||(!((*SR) & CAN_TSR_TERR1)));//Espera alguna bandera

		if(((*SR) & CAN_TSR_ALST1)||((*SR) & CAN_TSR_TERR1)){//Error de transmisión
			flag = false;
		}
		else if(((*SR) & CAN_TSR_RQCP1)){
			while(((*SR) & CAN_TSR_TXOK1));
			flag=true;
		}
		else if(((*SR) & CAN_TSR_TXOK1)){
			while(((*SR) & CAN_TSR_RQCP1));
			flag=true;
		}
	}
	else if(indexMailBox == 2){
		while((!((*SR) & CAN_TSR_RQCP2))||(!((*SR) & CAN_TSR_TXOK2))||(!((*SR) & CAN_TSR_ALST2))||(!((*SR) & CAN_TSR_TERR2)));//Espera alguna bandera

		if(((*SR) & CAN_TSR_ALST2)||((*SR) & CAN_TSR_TERR2)){//Error de transmisión
			flag = false;
		}
		else if(((*SR) & CAN_TSR_RQCP2)){
			while(((*SR) & CAN_TSR_TXOK2));
			flag=true;
		}
		else if(((*SR) & CAN_TSR_TXOK2)){
			while(((*SR) & CAN_TSR_RQCP2));
			flag=true;
		}
	}

	return flag;
}

void CANx_WaitSetFlag(volatile uint32_t *SR, uint32_t BitReg){
	//uint32_t flag = (*SR) & BitReg;
	while(!((*SR) & BitReg)){
		//flag = (*SR) & BitReg;
	}
}

void CANx_WaitResetFlag(volatile uint32_t *SR, uint32_t BitReg){
	//uint32_t flag = (*SR) & BitReg;
	while(((*SR) & BitReg)){
		//flag = (*SR) & BitReg;
	}
}

