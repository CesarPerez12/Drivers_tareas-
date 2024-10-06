/*
 * none.h
 *
 *  Created on: Dec 16, 2022
 *      Author: jurl9
 */

#ifndef DRIVERS_PERIPHERAL_INC_UARTX_H_
#define DRIVERS_PERIPHERAL_INC_UARTX_H_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define UART2		2
#define UART3		3
#define UART4		4
#define UART6		6

typedef struct {
	uint16_t baudRate;
	uint16_t OVER8;
	uint16_t wordLenght;
	uint16_t PS;
	bool Tx_Enable;
	bool Rx_Enable;
}UART_ParameterDef;

typedef struct{
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t GTPR;
}UART_TypeDef;

typedef struct{
	UART_TypeDef 				*Registers;
	UART_ParameterDef			Parameter;
}UART_HandlerDef;


#define UART1_BASE  0x40011000UL
#define UART2_BASE 	0x40004400UL
#define UART3_BASE	0x40004800UL
#define UART4_BASE 	0x40004C00UL
#define UART6_BASE	0x40011400UL

#define UART1_R		((UART_TypeDef *) UART1_BASE)
#define UART2_R 	((UART_TypeDef *) UART2_BASE)
#define UART3_R 	((UART_TypeDef *) UART3_BASE)
#define UART4_R 	((UART_TypeDef *) UART4_BASE)
#define UART6_R 	((UART_TypeDef *) UART6_BASE)

extern UART_HandlerDef UART1_Struct, UART2_Struct, UART4_Struct, UART3_Struct, UART6_Struct;

#define UART_SR_PE_Pos 				(0U)
#define UART_SR_PE_Msk				(0x1UL << UART_SR_PE_Pos)
#define UART_SR_PE 					UART_SR_PE_Msk
#define UART_SR_FE_Pos 				(1U)
#define UART_SR_FE_Msk				(0x1UL << UART_SR_FE_Pos)
#define UART_SR_FE 					UART_SR_PE_Msk
#define UART_SR_NF_Pos 				(2U)
#define UART_SR_NF_Msk				(0x1UL << UART_SR_NF_Pos)
#define UART_SR_NF 					UART_SR_NF_Msk
#define UART_SR_ORE_Pos 			(3U)
#define UART_SR_ORE_Msk				(0x1UL << UART_SR_ORE_Pos)
#define UART_SR_ORE					UART_SR_ORE_Msk
#define UART_SR_IDLE_Pos 			(4U)
#define UART_SR_IDLE_Msk			(0x1UL << UART_SR_IDLE_Pos)
#define UART_SR_IDLE 				UART_SR_IDLE_Msk
#define UART_SR_RXNE_Pos 			(5U)
#define UART_SR_RXNE_Msk			(0x1UL << UART_SR_RXNE_Pos)
#define UART_SR_RXNE 				UART_SR_RXNE_Msk
#define UART_SR_TC_Pos 				(6U)
#define UART_SR_TC_Msk				(0x1UL << UART_SR_TC_Pos)
#define UART_SR_TC 					UART_SR_TC_Msk
#define UART_SR_TXE_Pos 			(7U)
#define UART_SR_TXE_Msk				(0x1UL << UART_SR_TXE_Pos)
#define UART_SR_TXE 				UART_SR_TXE_Msk
#define UART_SR_LBD_Pos 			(8U)
#define UART_SR_LBD_Msk				(0x1UL << UART_SR_LBD_Pos)
#define UART_SR_LBD 				UART_SR_LBD_Msk
#define UART_SR_CTS_Pos 			(9U)
#define UART_SR_CTS_Msk				(0x1UL << UART_SR_CTS_Pos)
#define UART_SR_CTS 				UART_SR_CTS_Msk

#define URAT_DR_Pos					(0U)
#define UART_DR_Msk					(0x1UL << UART_DR_Pos)
#define UART_DR						UART_DR_Msk

#define UART_BRR_DIV_Fraction_Pos	(0U)
#define UART_BRR_DIV_Fraction_Msk	(0x1UL << UART_BRR_DIV_Fraction_Pos)
#define UART_BRR_DIV_Fraction 		UART_BRR_DIV_Fraction_Msk
#define UART_BRR_DIV_Mantissa_Pos	(4U)
#define UART_BRR_DIV_Mantissa_Msk	(0x1UL << UART_BRR_DIV_Mantissa_Pos)
#define UART_BRR_DIV_Mantissa 		UART_BRR_DIV_Fraction_Msk

#define UART_CR1_SBK_Pos			(0U)
#define UART_CR1_SBK_Msk			(0x1UL << UART_CR1_SBK_Pos)
#define UART_CR1_SBK				UART_CR1_SBK_Msk
#define UART_CR1_RWU_Pos			(1U)
#define UART_CR1_RWU_Msk			(0x1UL << UART_CR1_RWU_Pos)
#define UART_CR1_RWU				UART_CR1_RWU_Msk
#define UART_CR1_RE_Pos				(2U)
#define UART_CR1_RE_Msk				(0x1UL << UART_CR1_RE_Pos)
#define UART_CR1_RE					UART_CR1_RE_Msk
#define UART_CR1_TE_Pos				(3U)
#define UART_CR1_TE_Msk				(0x1UL << UART_CR1_TE_Pos)
#define UART_CR1_TE					UART_CR1_TE_Msk
#define UART_CR1_IDLEIE_Pos			(4U)
#define UART_CR1_IDLEIE_Msk			(0x1UL << UART_CR1_IDLEIE_Pos)
#define UART_CR1_IDLEIE				UART_CR1_IDLEIE_Msk
#define UART_CR1_RXNEIE_Pos			(5U)
#define UART_CR1_RXNEIE_Msk			(0x1UL << UART_CR1_RXNEIE_Pos)
#define UART_CR1_RXNEIE				UART_CR1_RXNEIE_Msk
#define UART_CR1_TCIE_Pos			(6U)
#define UART_CR1_TCIE_Msk			(0x1UL << UART_CR1_TCIE_Pos)
#define UART_CR1_TCIE				UART_CR1_TCIE_Msk
#define UART_CR1_TXEIE_Pos			(7U)
#define UART_CR1_TXEIE_Msk			(0x1UL << UART_CR1_TXEIE_Pos)
#define UART_CR1_TXEIE				UART_CR1_TXEIE_Msk
#define UART_CR1_PEIE_Pos			(8U)
#define UART_CR1_PEIE_Msk			(0x1UL << UART_CR1_PEIE_Pos)
#define UART_CR1_PEIE				UART_CR1_PEIE_Msk
#define UART_CR1_PS_Pos				(9U)
#define UART_CR1_PS_Msk				(0x1UL << UART_CR1_PS_Pos)
#define UART_CR1_PS					UART_CR1_SBK_Msk
#define UART_CR1_PCE_Pos			(10U)
#define UART_CR1_PCE_Msk			(0x1UL << UART_CR1_PCE_Pos)
#define UART_CR1_PCE				UART_CR1_PCE_Msk
#define UART_CR1_WAKE_Pos			(11U)
#define UART_CR1_WAKE_Msk			(0x1UL << UART_CR1_WAKE_Pos)
#define UART_CR1_WAKE				UART_CR1_WAKE_Msk
#define UART_CR1_M_Pos				(12U)
#define UART_CR1_M_Msk				(0x1UL << UART_CR1_M_Pos)
#define UART_CR1_M					UART_CR1_M_Msk
#define UART_CR1_UE_Pos				(13U)
#define UART_CR1_UE_Msk				(0x1UL << UART_CR1_UE_Pos)
#define UART_CR1_UE					UART_CR1_UE_Msk
#define UART_CR1_OVER8_Pos			(15U)
#define UART_CR1_OVER8_Msk			(0x1UL << UART_CR1_OVER8_Pos)
#define UART_CR1_OVER8				UART_CR1_OVER8_Msk

#define UART_CR2_LBDL_Pos			(5U)
#define UART_CR2_LBDL_Msk			(0x1UL << UART_CR2_LBDL_Pos)
#define UART_CR2_LBDL				UART_CR2_LBDL_Msk
#define UART_CR2_LBDIE_Pos			(6U)
#define UART_CR2_LBDIE_Msk			(0x1UL << UART_CR2_LBDIE_Pos)
#define UART_CR2_LBDIE				UART_CR2_LBDIE_Msk
#define UART_CR2_LBCL_Pos			(8U)
#define UART_CR2_LBCL_Msk			(0x1UL << UART_CR2_LBCL_Pos)
#define UART_CR2_LBCL				UART_CR2_LBCL_Msk
#define UART_CR2_CPHA_Pos			(9U)
#define UART_CR2_CPHA_Msk			(0x1UL << UART_CR2_CPHA_Pos)
#define UART_CR2_CPHA				UART_CR2_CPHA_Msk
#define UART_CR2_CPOL_Pos			(10U)
#define UART_CR2_CPOL_Msk			(0x1UL << UART_CR2_CPOL_Pos)
#define UART_CR2_CPOL				UART_CR2_CPOL_Msk
#define UART_CR2_CLKEN_Pos			(11U)
#define UART_CR2_CLKEN_Msk			(0x1UL << UART_CR2_CLKEN_Pos)
#define UART_CR2_CLKEN				UART_CR2_CLKEN_Msk
#define UART_CR2_STOP_Pos			(12U)
#define UART_CR2_STOP_Msk			(0x1UL << UART_CR2_STOP_Pos)
#define UART_CR2_STOP				UART_CR2_STOP_Msk
#define UART_CR2_LINEN_Pos			(14U)
#define UART_CR2_LINEN_Msk			(0x1UL << UART_CR2_LINEN_Pos)
#define UART_CR2_LINEN				UART_CR2_LINEN_Msk

#define UART_CR3_EIE_Pos			(0U)
#define UART_CR3_EIE_Msk			(0x1UL << UART_CR3_EIE_Pos)
#define UART_CR3_EIE				UART_CR3_EIE_Msk
#define UART_CR3_IREN_Pos			(1U)
#define UART_CR3_IREN_Msk			(0x1UL << UART_CR3_IREN_Pos)
#define UART_CR3_IREN				UART_CR3_IREN_Msk
#define UART_CR3_IRLP_Pos			(2U)
#define UART_CR3_IRLP_Msk			(0x1UL << UART_CR3_IRLP_Pos)
#define UART_CR3_IRLP				UART_CR3_IRLP_Msk
#define UART_CR3_HDSEL_Pos			(3U)
#define UART_CR3_HDSEL_Msk			(0x1UL << UART_CR3_HDSEL_Pos)
#define UART_CR3_HDSEL				UART_CR3_HDSEL_Msk
#define UART_CR3_NACK_Pos			(4U)
#define UART_CR3_NACK_Msk			(0x1UL << UART_CR3_NACK_Pos)
#define UART_CR3_NACK				UART_CR3_NACK_Msk
#define UART_CR3_SCEN_Pos			(5U)
#define UART_CR3_SCEN_Msk			(0x1UL << UART_CR3_SCEN_Pos)
#define UART_CR3_SCEN				UART_CR3_SCEN_Msk
#define UART_CR3_DMAR_Pos			(6U)
#define UART_CR3_DMAR_Msk			(0x1UL << UART_CR3_DMAR_Pos)
#define UART_CR3_DMAR				UART_CR3_DMAR_Msk
#define UART_CR3_DMAT_Pos			(7U)
#define UART_CR3_DMAT_Msk			(0x1UL << UART_CR3_DMAT_Pos)
#define UART_CR3_DMAT				UART_CR3_DMAT_Msk
#define UART_CR3_RTSE_Pos			(8U)
#define UART_CR3_RTSE_Msk			(0x1UL << UART_CR3_RTSE_Pos)
#define UART_CR3_RTSE				UART_CR3_RTSE_Msk
#define UART_CR3_CTSE_Pos			(0U)
#define UART_CR3_CTSE_Msk			(0x1UL << UART_CR3_CTSE_Pos)
#define UART_CR3_CTSE				UART_CR3_CTSE_Msk
#define UART_CR3_CTSIE_Pos			(0U)
#define UART_CR3_CTSIE_Msk			(0x1UL << UART_CR3_CTSIE_Pos)
#define UART_CR3_CTSIE				UART_CR3_CTSIE_Msk
#define UART_CR3_ONEBIT_Pos			(0U)
#define UART_CR3_ONEBIT_Msk			(0x1UL << UART_CR3_ONEBIT_Pos)
#define UART_CR3_ONEBIT				UART_CR3_ONEBIT_Msk

#define UART_GTPR_PSC_Pos			(0U)
#define UART_GTPR_PSC_Msk			(01UL << UART_GTPR_PSC_Pos)
#define UART_GTPR_PSC				UART_GTPR_PSC_Msk
#define UART_GTPR_GT_Pos			(8U)
#define UART_GTPR_GT_Msk			(01UL << UART_GTPR_GT_Pos)
#define UART_GTPR_GT				UART_GTPR_GT_Msk

void UARTx_GPIO_Init(UART_HandlerDef * uart);
void UARTx_Init(uint32_t baudRate, uint8_t OVER8, uint8_t wordLenght, uint8_t PS,bool Rx, bool Tx, UART_HandlerDef * uart);
void UARTx_TxRxEnable(UART_HandlerDef * uart, bool Tx, bool Rx);
uint16_t* baudRate_calculate(UART_HandlerDef * uart,uint32_t baudRate);
void RCC_EnUARTx(UART_HandlerDef * uart);
void UARTx_wordLenght(UART_HandlerDef * uart, uint8_t wordLenght);
void UARTx_OVER8(UART_HandlerDef * uart, uint8_t OVER8);
uint8_t UARTx_receiveData(UART_HandlerDef * uart);
void UARTx_sendData(UART_HandlerDef * uart, uint8_t dato);
void UARTx_ResetStruct(UART_HandlerDef * uart ,UART_TypeDef  * uart_base);
void UARTx_RXNE_Init(UART_HandlerDef * uart);
void UARTx_TC_Init(UART_HandlerDef * uart);
void UARTx_TXE_Init(UART_HandlerDef * uart);
bool UARTx_SolveCurrentErrors(UART_HandlerDef * uart, uint8_t currentErrors);
uint8_t UARTx_GetInterruptFlags(UART_HandlerDef * uart);
uint8_t UARTx_GetCurrentErrors(UART_HandlerDef * uart);
void UARTx_InterruptEN(UART_HandlerDef * uart,bool RXNE, bool TC, bool TXE,uint8_t priority);
void UARTx_HalfDuplexSingleWire(UART_HandlerDef * uart);

#endif /* DRIVERS_PERIPHERAL_INC_UARTX_H_ */
