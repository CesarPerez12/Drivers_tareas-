/*
 * UARTx.c
 *
 *  Created on: 22 ene 2023
 *      Author: ErickGómezCrz
 */

#include "UARTx.h"
#include "RCC.h"
#include "GPIOx.h"
#include "NVIC.h"


/*
 For UART4 		----> IO A0 (Tx) and IO A1 (Rx) 	UART4 (Rx/Tx) 	----> AF8
 For UART2 		----> IO A2 (Tx) and IO D6 (Rx)		UART2 (Tx/Rx)	----> AF7
 For UART3 		----> IO B10 (Tx) and IO B11 (Rx)	UART3 (Rx/Tx)	----> AF7
 For UART6		----> IO G14 (Tx) and IO C7 (Rx)	UART6 (Rx/Tx) 	----> AF8
 */

UART_HandlerDef UART1_Struct,UART2_Struct, UART4_Struct, UART3_Struct, UART6_Struct;

void UARTx_GPIO_Init(UART_HandlerDef * uart){
	if(uart == &(UART2_Struct)){//UART2
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
		GPIOx_InitAF(GPIOA, 2, GPIO_OTYPER_PP, GPIO_OSPEEDR_HS, GPIO_AFR_AFSEL_UART2);
		GPIOx_InitAF(GPIOA, 3, GPIO_OTYPER_PP, GPIO_OSPEEDR_HS, GPIO_AFR_AFSEL_UART2);
	}else if(uart == &(UART4_Struct)){
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
		GPIOx_InitAF(GPIOA, 0, GPIO_OTYPER_PP, GPIO_OSPEEDR_HS, GPIO_AFR_AFSEL_UART4);
		GPIOx_InitAF(GPIOA, 1, GPIO_OTYPER_PP, GPIO_OSPEEDR_HS, GPIO_AFR_AFSEL_UART4);
	}else if(uart == &(UART3_Struct)){
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
		GPIOx_InitAF(GPIOB, 10, GPIO_OTYPER_PP, GPIO_OSPEEDR_HS, GPIO_AFR_AFSEL_UART3);
		GPIOx_InitAF(GPIOB, 11, GPIO_OTYPER_PP, GPIO_OSPEEDR_HS, GPIO_AFR_AFSEL_UART3);
	}else if(uart == &(UART6_Struct)){
		RCC_AHB1ENR |= (RCC_AHB1ENR_GPIOCEN|RCC_AHB1ENR_GPIOGEN);
		GPIOx_InitAF(GPIOG, 14, GPIO_OTYPER_PP, GPIO_OSPEEDR_HS, GPIO_AFR_AFSEL_UART6);
		GPIOx_InitAF(GPIOC, 7, GPIO_OTYPER_PP, GPIO_OSPEEDR_HS, GPIO_AFR_AFSEL_UART6);
	}else if(uart == &(UART1_Struct)){
		RCC_AHB1ENR |= (RCC_AHB1ENR_GPIOAEN);
		GPIOx_InitAF(GPIOA, 9, GPIO_OTYPER_PP, GPIO_OSPEEDR_HS, GPIO_AFR_AFSEL_UART1);
		GPIOx_InitAF(GPIOA, 10, GPIO_OTYPER_PP, GPIO_OSPEEDR_HS, GPIO_AFR_AFSEL_UART1);
	}

}

uint16_t* baudRate_calculate(UART_HandlerDef * uart, uint32_t baudRate){
	static uint16_t RegbaudRate[2];
	uint16_t num;
	uint16_t DIV_Mantissa;
	uint8_t DIV_Fraction=0;
	if(uart->Parameter.OVER8==1){
		num = (SYS_CLK.APB1CLK*1000000)/(baudRate*8);
		DIV_Mantissa = (uint16_t) num;
		num = (SYS_CLK.APB1CLK*1000000*100)/(baudRate*8);								//Para obtener el primer decimal incrementamos
		num = (num - (DIV_Mantissa*100));							//Obtenemos el decimal
		num *= 8;
		DIV_Fraction = num - ((num/100)*100);

		if(DIV_Fraction > 50){
			num+=100;
		}

		DIV_Fraction = num/100;

		RegbaudRate[0] = DIV_Fraction&(0x0007);
		RegbaudRate[1] = DIV_Mantissa;

	}else{
		num = (SYS_CLK.APB1CLK*1000000)/(baudRate*16);
		DIV_Mantissa = (uint16_t) num;
		num = (SYS_CLK.APB1CLK*1000000*100)/(baudRate*16);								//Para obtener el primer decimal incrementamos
		num = (num - (DIV_Mantissa*100));							//Obtenemos el decimal
		num *= 16;
		DIV_Fraction = num - ((num/100)*100);

		if(DIV_Fraction > 50){
			num+=100;
		}

		DIV_Fraction = num/100;
		RegbaudRate[0] = DIV_Fraction;
		RegbaudRate[1] = DIV_Mantissa;
	}
	return RegbaudRate;
}

void RCC_EnUART(UART_HandlerDef * uart){
	if(uart == &(UART2_Struct)){
		RCC_APB1ENR |= RCC_APB1ENR_USART2EN;
	}else if(uart == &(UART4_Struct)){
		RCC_APB1ENR |= RCC_APB1ENR_UART4EN;
	}else if(uart == &(UART3_Struct)){
		RCC_APB1ENR |= RCC_APB1ENR_USART3EN;
	}else if(uart == &(UART6_Struct)){
		RCC_APB2ENR |= RCC_APB2ENR_USART6EN;
	}else if(uart == &(UART1_Struct)){
		RCC_APB2ENR |= RCC_APB2ENR_USART1EN;
	}
	uart->Registers->CR1 |= UART_CR1_UE;
}

void UARTx_TxRxEnable(UART_HandlerDef * uart,bool Rx, bool Tx){
	if(Rx){
		uart->Registers->CR1 |= UART_CR1_RE;
		uart->Parameter.Rx_Enable = true;
	}else{
		uart->Registers->CR1 &= ~UART_CR1_RE;
		uart->Parameter.Rx_Enable = false;
	}
	if(Tx){
		uart->Registers->CR1 |= UART_CR1_TE;
		uart->Parameter.Tx_Enable = true;
	}else{
		uart->Registers->CR1 &= ~UART_CR1_TE;
		uart->Parameter.Tx_Enable = false;
	}
}

void UARTx_wordLenght(UART_HandlerDef * uart, uint8_t wordLenght){
	if(wordLenght == 0){
		uart->Registers->CR1 &= ~UART_CR1_M;
		uart->Parameter.wordLenght &= ~UART_CR1_M;
	}else{
		uart->Registers->CR1 |= UART_CR1_M;
		uart->Parameter.wordLenght |= UART_CR1_M;
	}
}

void UARTx_OVER8(UART_HandlerDef * uart, uint8_t OVER8){
	if(OVER8 == 1){
		uart->Registers->CR1 |= UART_CR1_OVER8;
		uart->Parameter.OVER8 = 1;
	}else{
		uart->Registers->CR1 &= ~UART_CR1_OVER8;
		uart->Parameter.OVER8 = 0;
	}
}

uint8_t UARTx_receiveData(UART_HandlerDef * uart){
	uint8_t aux;
	while(!(uart->Registers->SR&UART_SR_RXNE));
	aux = uart->Registers->DR;
	return aux;
}

void UARTx_sendData(UART_HandlerDef * uart, uint8_t dato){
	while((uart->Registers->SR&UART_CR1_TXEIE)==0);
	uart->Registers->DR=dato;
}

void UARTx_ParitySelection(UART_HandlerDef * uart, uint8_t PS){
	if(PS==1){
		uart->Registers->CR1 |= UART_CR1_PCE;
		uart->Registers->CR1 |= UART_CR1_PS;
		uart->Parameter.PS = 1;
	}else if(PS==2){
		uart->Registers->CR1 |= UART_CR1_PCE;
		uart->Registers->CR1 &= ~UART_CR1_PS;
		uart->Parameter.PS = 2;
	}
	else{
		uart->Registers->CR1 &= ~UART_CR1_PCE;
		uart->Parameter.PS = 0;
	}

}

void UARTx_Init(uint32_t baudRate, uint8_t OVER8, uint8_t wordLenght, uint8_t PS,bool Rx, bool Tx, UART_HandlerDef * uart){
	uint16_t*BRR;
	RCC_EnUART(uart);
	UARTx_TxRxEnable(uart, Rx, Tx);
	UARTx_wordLenght(uart,wordLenght);
	UARTx_OVER8(uart,OVER8);
	UARTx_ParitySelection(uart,PS);
	BRR = baudRate_calculate(uart, baudRate);
	uart->Registers->BRR |= BRR[0] << 0;										//Se recorre al bit 0 para abarcar bits 0-3
	uart->Registers->BRR |= BRR[1] << 4;
}

void UARTx_ResetStruct(UART_HandlerDef * uart ,UART_TypeDef  * uart_base){
    uart->Registers = uart_base;
    uart->Parameter.OVER8 = 0;
    uart->Parameter.baudRate=0;
    uart->Parameter.wordLenght=0;
    uart->Parameter.Rx_Enable=false;
    uart->Parameter.Tx_Enable=false;
}
void UARTx_RXNE_Init(UART_HandlerDef * uart){
	uart->Registers->CR1 |= UART_CR1_RXNEIE;
}
void UARTx_TC_Init(UART_HandlerDef * uart){
	uart->Registers->CR1 |= UART_CR1_TCIE;
}
void UARTx_TXE_Init(UART_HandlerDef * uart){
	uart->Registers->CR1 |= UART_CR1_TXEIE;
}
void UARTx_InterruptEN(UART_HandlerDef * uart,bool RXNE, bool TC, bool TXE,uint8_t priority){
	if(RXNE)
		UARTx_RXNE_Init(uart);
	if(TC)
		UARTx_TC_Init(uart);
	if(TXE)
		UARTx_TXE_Init(uart);
	if(uart == &(UART2_Struct)){
		NVIC_SetCFGR(38,priority);
	}else if(uart == &(UART4_Struct)){
		NVIC_SetCFGR(52,priority);
	}else if(uart == &(UART3_Struct)){
		NVIC_SetCFGR(39,priority);
	}else if(uart == &(UART6_Struct)){
		NVIC_SetCFGR(71,priority);
	}else if(uart == &(UART1_Struct)){
		NVIC_SetCFGR(37,priority);
	}
}
uint8_t UARTx_GetCurrentErrors(UART_HandlerDef * uart){
	uint8_t currentErrors = (uart->Registers->SR&0x0F);
	return currentErrors;
}
uint8_t UARTx_GetInterruptFlags(UART_HandlerDef * uart){
	uint8_t interruptFlags = (uart->Registers->SR&0xF0);
	return interruptFlags;
}
bool UARTx_SolveCurrentErrors(UART_HandlerDef * uart, uint8_t cE){
	uint8_t currentErrors = cE;
	bool IsCurrentErrorsSolve;
	if(currentErrors==0){
		IsCurrentErrorsSolve=true;
	}else{
		IsCurrentErrorsSolve=false;
		if(currentErrors&UART_SR_PE){
			while((uart->Registers->SR&UART_SR_RXNE)==0);		//Espera la bandera antes de limpiar
			uart->Registers->SR;								//Lectura de registro de estado
			uart->Registers->DR;								//Lectura de registro de datos
															//Se limpia bandera
			if(!(currentErrors&UART_SR_PE)){					//Confirma bandera limpia

			}
		}
		if(currentErrors&UART_SR_FE){
			uart->Registers->SR;								//Lectura de registro de estado
			uart->Registers->DR;
			//Se limpia bandera
			if(!(currentErrors&UART_SR_FE)){//Confirma bandera limpia

			}
		}
		if(currentErrors&UART_SR_NF){
			uart->Registers->SR;								//Lectura de registro de estado
			uart->Registers->DR;
			//Se limpia bandera
			if(!(currentErrors&UART_SR_NF)){//Confirma bandera limpia

			}
	}
		if(currentErrors&UART_SR_ORE){
			uart->Registers->SR;								//Lectura de registro de estado
			uart->Registers->DR;
			if(!(currentErrors&UART_SR_ORE)){						//Confirma bandera limpia

			}
		}
		IsCurrentErrorsSolve=true;
	}


	return IsCurrentErrorsSolve;
}
void UARTx_HalfDuplexSingleWire(UART_HandlerDef * uart){
	uart->Registers->CR3 |= UART_CR3_HDSEL;
	uart->Registers->CR3 &= ~(UART_CR3_SCEN|UART_CR3_IREN);
	uart->Registers->CR2 &= ~(UART_CR2_LINEN|UART_CR2_CLKEN);
}



