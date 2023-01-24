/*
 * CANx.h
 *
 *  Created on: 23 ene 2023
 *      Author: jurl9
 */

#ifndef DRIVERS_PERIPHERAL_INC_CANX_H_
#define DRIVERS_PERIPHERAL_INC_CANX_H_

#include "stdio.h"
#include "stdbool.h"

#define CAN_BASE    0x40006400

//x=0,1,2.

typedef struct
{
	volatile uint32_t     TIxR;
	volatile uint32_t    TDTxR;
	volatile uint32_t    TDLxR;
	volatile uint32_t    TDHxR;

}CAN_TypeDef_MailBoxTx;

typedef struct
{
	volatile uint32_t     RIxR;
	volatile uint32_t    RDTxR;
	volatile uint32_t    RDLxR;
	volatile uint32_t    RDHxR;

}CAN_TypeDef_MailBoxFIFORx;

typedef struct
{
	//i=0,1,...,27
	volatile uint32_t     FiR1;
	volatile uint32_t     FiR2;

}CAN_TypeDef_FilterBankRegister;

typedef struct
{
	volatile uint32_t                       MCR;
	volatile uint32_t                       MSR;
	volatile uint32_t                       TSR;
	volatile uint32_t                      RF0R;
	volatile uint32_t                      RF1R;
	volatile uint32_t                       IER;
	volatile uint32_t                       ESR;
	volatile uint32_t                       BTR;
    uint32_t                      RESERVED0[88];
	CAN_TypeDef_MailBoxTx          MailBoxTx[3];
	CAN_TypeDef_MailBoxFIFORx  MailBoxFIFORx[2];
	uint32_t                      RESERVED1[12];
	volatile uint32_t                       FMR;
	volatile uint32_t                      FM1R;
	uint32_t                          RESERVED2;
	volatile uint32_t                      FS1R;
	uint32_t                          RESERVED3;
	volatile uint32_t                     FFA1R;
	uint32_t                          RESERVED4;
	volatile uint32_t                      FA1R;
	uint32_t                       RESERVED5[8];
	CAN_TypeDef_FilterBankRegister      FiR[28];

}CAN_TypeDef;

#define CAN1    ((CAN_TypeDef *)(CAN_BASE + 0x000UL))
#define CAN2    ((CAN_TypeDef *)(CAN_BASE + 0x400UL))

#endif /* DRIVERS_PERIPHERAL_INC_CANX_H_ */
