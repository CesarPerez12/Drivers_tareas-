/*
 * I2Cx.c
 *
 *  Created on: Sep 23, 2022
 *      Author: jurl9
 */

#include "I2Cx.h"
#include "RCC.h"
#include "GPIOx.h"
#include "NVIC.h"
/*
 * I2C1: PB6 -> SCL, PB7 -> SDA
 * I2C2: PH4 -> SCL, PH5 -> SDA
 * I2C3: PH7 -> SCL, PH8 -> SDA
 */
uint8_t currentAPB1=0, bufferI2C[8], bufferI2CRx[8];

I2C_HandlerDef I2C1_Struct, I2C2_Struct, I2C3_Struct;

//Configuración de Puertos asociados a I2C
bool I2Cx_GPIO_Init(bool PUR, uint8_t x){
	if(x==1){//I2C1: PB6 -> SCL, PB7 -> SDA
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN ;//Habilitar Fuente de reloj B antes de configurar
		GPIOx_InitAF(GPIOB, 6, GPIO_OTYPER_OD, GPIO_AFR_AFSEL_I2C);//Pin 6
		GPIOx_InitAF(GPIOB, 7, GPIO_OTYPER_OD, GPIO_AFR_AFSEL_I2C);//Pin 7
		if(PUR){
			GPIOB->PUPDR |= (GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0);//Resistencias Pull UP para modo esclavo
		}
		else{
			GPIOB->PUPDR &= ((~GPIO_PUPDR_PUPD6) & (~GPIO_PUPDR_PUPD7));
		}
	}
	else if(x==2){//I2C2: PH4 -> SCL, PH5 -> SDA
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIOHEN ;//Habilitar Fuente de reloj H antes de configurar
		GPIOx_InitAF(GPIOH, 4, GPIO_OTYPER_OD, GPIO_AFR_AFSEL_I2C);//Pin 6
		GPIOx_InitAF(GPIOH, 5, GPIO_OTYPER_OD, GPIO_AFR_AFSEL_I2C);//Pin 7
		if(PUR){
			GPIOH->PUPDR |= (GPIO_PUPDR_PUPD4_0 | GPIO_PUPDR_PUPD5_0);//Resistencias Pull UP para modo esclavo
		}
		else{
			GPIOH->PUPDR &= ((~GPIO_PUPDR_PUPD4) & (~GPIO_PUPDR_PUPD5));
		}
	}
	else if(x==3){// I2C3: PH7 -> SCL, PH8 -> SDA
		RCC_AHB1ENR |= RCC_AHB1ENR_GPIOHEN ;//Habilitar Fuente de reloj H antes de configurar
		GPIOx_InitAF(GPIOH, 7, GPIO_OTYPER_OD, GPIO_AFR_AFSEL_I2C);//Pin 6
		GPIOx_InitAF(GPIOH, 8, GPIO_OTYPER_OD, GPIO_AFR_AFSEL_I2C);//Pin 7
		if(PUR){
			GPIOH->PUPDR |= (GPIO_PUPDR_PUPD7_0 | GPIO_PUPDR_PUPD8_0);//Resistencias Pull UP para modo esclavo
		}
		else{
			GPIOH->PUPDR &= ((~GPIO_PUPDR_PUPD7) & (~GPIO_PUPDR_PUPD8));
		}

	}
	return true;

}
//Configuración de la interfaz I2C
//APB1 en MHz se puede calcular, almacenar o pasar el valor correcto
//En este caso se debe ingresar el valor correcto
bool I2Cx_Init(uint8_t APB1_freq, uint8_t speed,  I2C_HandlerDef * i2c){
	uint32_t aux = 0;
	RCC_EnI2Cx(i2c);//Fuente de reloj //-----------------------------------------------------------------------------------
	I2Cx_SetSWRST(i2c);//Reset I2C
	I2Cx_ResetSWRST(i2c);
	I2Cx_ResetPE(i2c);//Deshabilitar
	I2Cx_SetFREQ(APB1_freq, i2c);//Frecuencia del APB1
	if((speed==1)&&(APB1_freq>=4)){//Fast Mode
		I2Cx_SetMode(speed, i2c);
		aux = APB1_freq / 10;//Eliminamos el número de menor valor
		aux = APB1_freq - (aux*10) ;//Obtenermos el número de menor valor
		if(aux){//Si no es múltiplo de 10MHz
			//tlow=2*thigh, tlow+thigh=2500ns
			aux = (APB1_freq*100)/(12);//Calcula el valor de CCR, Escalamos los valores
		    aux = aux - (((APB1_freq*10)/(12))*10);
		    if(aux==0){//No se redondea
		    	aux = (APB1_freq*10)/(12);//Calcula el valor de CCR, Escalamos los valores
		    }
		    else{//redondeamos
		    	aux = (APB1_freq*10)/(12) + 1;//Calcula el valor de CCR, Escalamos los valores
		    }
			if(aux==0){
				aux=1;//Valor permitido
			}
			I2Cx_SetCCR(aux, 0, i2c);
		    // THIGH = CCR * TCKLP1 = 833ns, TCLKP1-> 1/APB1_freq
			//Duty = 0
		}
		else{//Múltiplo de 10MHz
			//tlow=(16/9)*thigh, tlow+thigh=2500ns
			aux = (APB1_freq*10)/(100);//Calcula el valor de CCR, escalamos valores
			if(aux==0){
				aux=1;//Valor permitido
			}
			I2Cx_SetCCR(aux, 1, i2c);
			// THIGH = 9 * CCR * TCKLP1 = 900ns, TCLKP1-> 1/APB1_freq
			//Duty = 1
		}
		aux = ((APB1_freq*10)/33) + 1;//Calcula el valor de Trise, 300ns FM, Escalamos
		I2Cx_SetTRISE(aux, i2c);//300ns/TCKLP1 + 1 = 11; MÁX 32
		I2Cx_SetBit14OAR1(i2c);
		if(aux<4){
			aux=4;//Valor permitido
		}
		I2Cx_SetFLTR(1, APB1_freq, i2c);
		// 2-10 0; 10-20 1; 20-30 7; 30-40 13; 40-50 15 Fast Mode
	}
	else{//Standard Mode
		I2Cx_SetMode(speed, i2c);
		aux = (APB1_freq*10)/2;//Calcula el valor de CCR
		I2Cx_SetCCR(aux, 0, i2c);
		//tLow=1000ns+4.7us, thigh=300ns+4us, CCR = 50, THIGH = CCR * TCKLP1 = 5000ns TCLKP1-> 1/10MHz
		aux = (APB1_freq) + 1;//Calcula el valor de Trise, 1000ns SM
		I2Cx_SetTRISE(aux, i2c);//1000ns/TCKLP1 + 1 = 11; MÁX 32.
		I2Cx_SetBit14OAR1(i2c);

		I2Cx_SetFLTR(0, APB1_freq, i2c);
		//2-5 2; 5-10 12; 10-50 15 Standard Mode
	}
	//Spread Spectrum
	I2Cx_EnableIT(i2c);
	I2Cx_SetPE(i2c);//Habilitar
	currentAPB1 = APB1_freq;
	return true;

}

/*
 * Configuración de dirección como esclavo
 * OAR1 se puede configurar para 10 o 7 bits
 * OAR2 se configura para segunda dirección de 7 bits
 * Nota: Colocar únicamente la frecuencia cuando se configura el modo de 10 bits en Maestro
 */

void I2Cx_ADDRSet(uint16_t address, uint8_t modeAddressing, uint8_t APB1_freq, I2C_HandlerDef * i2c){
	uint16_t aux=0;
	if(address!=0){
		if(address!=(i2c->Parameter.own_address)){//Si ya se asignó el valor, no asignar
			i2c->Parameter.own_address=address;
			i2c->Parameter.modeAddressing=modeAddressing;
		}
		I2Cx_SetOAR1(address, modeAddressing, i2c);
	}
	else{
		i2c->Parameter.modeAddressing=modeAddressing;
		I2Cx_SetADDMODE(1, i2c);//10 bits de dirección
		if(((i2c->Parameter.modeAddressing)==1)&&((I2Cx_GetCCR(i2c)&I2C_CCR_FS)==0)){//Indicaría modo Maestro, SM
			//Se limita a 80kHz de acuerdo con el documento de erratas en velocidad estándar
			aux = (APB1_freq*100)/(16);//Calcula el valor de CCR
			I2Cx_SetCCR(aux, 0, i2c);
			// THIGH = CCR * TCKLP1 = 6250ns, TCLKP1-> 1/APB1_freq
		}
	}
	//I2C1_OAR2
	I2Cx_SetACK(i2c);//Activamos ACK
}
/*
 * Se usa para eliminar los glitch generados al operar en modo esclavo
 */
void I2Cx_ResetSlave(I2C_HandlerDef * i2c){
	//Antes de reiniciar se debe asegurar que el bus esté libre
	I2Cx_Init(currentAPB1,i2c->Parameter.speed, i2c);//Se considera que todos operarán a la misma velocidad
	I2Cx_ADDRSet(i2c->Parameter.own_address, i2c->Parameter.modeAddressing, currentAPB1, i2c);// 0-> APB1_freq (Set correct Frequency when 10 bits addressing mode and Master, and Standard)
	I2Cx_SetACK(i2c);//Activamos ACK
}

//Start of frame
bool I2Cx_SOF(I2C_HandlerDef * i2c){
	I2Cx_SetSTART(i2c);
	I2Cx_WaitMSLSet(i2c);
	I2Cx_WaitSBSet(i2c);
	return true;
}
//Address of slave
void I2Cx_ADDR7Seq(uint8_t rw, uint16_t address, I2C_HandlerDef * i2c){
	I2Cx_ADDR7(address, rw, i2c);
	I2Cx_WaitSBReset(i2c);
	I2Cx_WaitADDRSet(i2c);
}

void I2Cx_ADDR10SeqTx(uint16_t address,I2C_HandlerDef * i2c){
	I2Cx_ADDR10Header(address, 0, i2c);//rw=0
	I2Cx_WaitADD10Set(i2c);
	I2Cx_ADDR10LastADDR(address, i2c);//Last 8 bits address
	I2Cx_WaitADDRSet(i2c);
	I2Cx_WaitTXESet(i2c);
	I2Cx_WaitTRASet(i2c);
}

void I2Cx_ADDR10Seq(uint16_t address,I2C_HandlerDef * i2c){
	//I2Cx_SetSTART(i2c);//Esta secuencia ya se lleva a cabo
	//I2Cx_WaitSBSet(i2c);
	//I2Cx_WaitMSLSet(i2c);
	I2Cx_ADDR10Header(address, 0, i2c);//rw=0
	I2Cx_WaitADD10Set(i2c);
	I2Cx_ADDR10LastADDR(address, i2c);//Last 8 bits address
	I2Cx_WaitADDRSet(i2c);
	I2Cx_WaitTXESet(i2c);
	I2Cx_WaitTRASet(i2c);
	I2Cx_ClearADDR(i2c);//
	I2Cx_SetSTART(i2c);//Restart
	I2Cx_WaitSBSet(i2c);
	I2Cx_WaitMSLSet(i2c);
	I2Cx_WaitBUSYSet(i2c);
	I2Cx_ADDR10Delay();
	I2Cx_ADDR10Header(address, 1, i2c);//rw=1
	I2Cx_WaitADDRSet(i2c);

}

//Función para enviar datos por la línea SDA
bool I2C_DataTransfer(uint8_t data, I2C_HandlerDef * i2c){
	I2Cx_WaitTXESet(i2c);//Espera confirmación de transmisión en curso
	I2Cx_WriteDR(data,i2c);
	return true;
}
//Solicitud de secuencia finalizada
bool I2Cx_Stop(I2C_HandlerDef * i2c){
	I2Cx_SetSTOP(i2c);
	return true;
}
//Función para leer los datos de la línea
uint8_t I2C_ReadByteReceived(I2C_HandlerDef * i2c){
	uint8_t data;
	I2Cx_WaitRXNESet(i2c);//Byte is copied into DR (RxNE=1)
	data=I2Cx_ReadDR(i2c);
	return data;
}
//Pasos para recibir un byte en modo maestro
void I2C_1ByteReceive(I2C_HandlerDef * i2c){
	I2Cx_ResetACK(i2c);
	I2Cx_ClearADDR(i2c);
}
//Pasos para recibir 2 bytes en modo maestro
void I2C_2ByteReceive(I2C_HandlerDef * i2c){
	I2Cx_ResetACK(i2c);
	I2Cx_SetPOS(i2c);
	I2Cx_ClearADDR(i2c);
}
//Pasos para recibir 3 o más bytes en modo maestro
void I2C_NByteReceive(uint8_t * buffer, uint8_t size, I2C_HandlerDef * i2c){
	uint8_t i=0;
	I2Cx_SetACK(i2c);
	I2Cx_ClearADDR(i2c);
	do {
		buffer[i]=I2C_ReadByteReceived(i2c);
		i++;
		if(I2Cx_GetFlagSR1(I2C_SR1_BTF, i2c)){
			buffer[i]=I2C_ReadByteReceived(i2c);
			i++;
		}
	} while (i<(size-2));
	I2Cx_WaitBTFSet(i2c);
	I2Cx_ResetACK(i2c);
	buffer[i]=I2C_ReadByteReceived(i2c);
	I2Cx_WaitBTFSet(i2c);
}

//------------------------------------------------------------------------------------
//                                 I2C Without Interrupts
//------------------------------------------------------------------------------------
//address 16 bits
bool I2Cx_MasterTx(uint16_t address, uint8_t * buffer, uint8_t size, I2C_HandlerDef * i2c){
	uint8_t i=0;
	I2Cx_ResetPOS(i2c);
	I2Cx_SOF(i2c);
	if(I2Cx_GetARLOBit(i2c)==0){//Loss Arbitration Detected for Multimaster capability
		if((i2c->Parameter.modeAddressing)==1){//10 bits
			I2Cx_ADDR10SeqTx(address, i2c);
		}
		else{//7 bits
			I2Cx_ADDR7Seq(0, address, i2c);
		}
		I2Cx_ClearADDR(i2c);
		if(size>1){//Se envía más de un dato
			for (i = 0; i < size; i++) {
				I2C_DataTransfer(buffer[i], i2c);
				if((I2Cx_GetFlagSR1(I2C_SR1_BTF, i2c))&&(i<size)){//BTF=1 y además el tamaño no sobrepasa al deseado
					I2C_DataTransfer(buffer[i], i2c);
					i++;
				}
			}
		}
		else if(size==1){
			I2C_DataTransfer(buffer[i], i2c);
			I2Cx_WaitBTFSet(i2c);
		}
		//else{//Error
		//}
		I2Cx_WaitBTFSet(i2c);
		I2Cx_Stop(i2c);
		I2Cx_WaitMSLReset(i2c);
	}
	else{
		I2Cx_ResetARLO(i2c);//Limpia bandera
	}
	return true;
}
//16 bits addr
bool I2Cx_MasterRx(uint16_t address, uint8_t * buffer, uint8_t size, I2C_HandlerDef * i2c){

	I2Cx_ResetPOS(i2c);
	I2Cx_SetACK(i2c);
	I2Cx_SOF(i2c);
	if(I2Cx_GetARLOBit(i2c)==0){//Loss Arbitration Detected for Multimaster capability
		if((i2c->Parameter.modeAddressing)==1){//Secuencia para 10 bits en modo recepción
			I2Cx_ADDR10Seq(address, i2c);
		}
		else{//Secuencia para 7 bits en modo recepción
			I2Cx_ADDR7Seq(1, address, i2c);
		}
		//if(mode==0){//0 byte received
		//}
		if(size==1){//1 byte received
			I2C_1ByteReceive(i2c);
			I2Cx_Stop(i2c);
			buffer[0]=I2C_ReadByteReceived(i2c);
			I2Cx_WaitRXNEReset(i2c);//
		}
		else if(size==2){//2 byte received
			I2C_2ByteReceive(i2c);
			I2Cx_WaitBTFSet(i2c);
			I2Cx_Stop(i2c);
			buffer[0]=I2C_ReadByteReceived(i2c);
			buffer[1]=I2C_ReadByteReceived(i2c);
			I2Cx_WaitRXNEReset(i2c);//
		}
		else{//N > 2 byte received and last bytes
			I2C_NByteReceive(buffer, size, i2c);
			I2Cx_Stop(i2c);
			//buffer[size-2]=I2C_ReadByteReceived(i2c);
			buffer[size-1]=I2C_ReadByteReceived(i2c);
			//I2Cx_WaitRXNEReset(i2c);//
		}
		I2Cx_WaitMSLReset(i2c);
	}
	else{
		I2Cx_ResetARLO(i2c);//Limpia bandera
	}
	return true;
}

bool I2Cx_SlaveTx(uint8_t * buffer, uint8_t size, I2C_HandlerDef * i2c){
	uint16_t i=0;
	I2Cx_ResetPOS(i2c);
	//En 10 bits se espera también el ADDRset después de obtener el HEADER con 0x1 al final
	I2Cx_WaitADDRSet(i2c);
	if((i2c->Parameter.modeAddressing)==1){//Secuencia para modo transmisor
		I2Cx_ClearADDR(i2c);
		I2Cx_WaitADDRSet(i2c);
	}
	I2Cx_ClearADDR(i2c);
	//I2Cx_WaitTRASet(i2c);//Se puede usar como verificación de que va recibir o transmitir
	while(I2Cx_GetAFBit(i2c)==0){//(i<size)&&(I2Cx_GetAFBit(i2c)==0) Probar con la bandera mejor
		I2C_DataTransfer(buffer[i], i2c);
		i++;
		if((I2Cx_GetFlagSR1(I2C_SR1_BTF, i2c))&&(i<size)){
			I2C_DataTransfer(buffer[i], i2c);
			i++;
		}
	}
	//for (i = 0; i < 1000000; ++i);//Delay para Generar una correcta secuencia, EN CASO DE NO USAR LA BANDERA AF en el envío
	I2Cx_WaitAFSet(i2c);//Esperar la secuencia de paro
	I2Cx_ResetAF(i2c);
	I2Cx_ResetACK(i2c);
	I2Cx_SetSTOP(i2c);//Libera las líneas después del último byte, se asegura que el bus está libre
	//for (i = 0; i < 1250; i++) ;//Este delay se usa exclusivamente cuando en el while no se verifica el bit AF
	I2Cx_ResetSlave(i2c);//Se reinicia para evitar glitchs
	//En modo esclavo ocurre un error de UNDERRUN y OVERRUN, para este caso se reinicia la interfaz
	return true;
}

bool I2Cx_SlaveRx(uint8_t * buffer, uint8_t size, I2C_HandlerDef * i2c){
	uint8_t i=0;
	I2Cx_ResetPOS(i2c);
	I2Cx_WaitADDRSet(i2c);
	I2Cx_ClearADDR(i2c);
	//I2Cx_WaitTRAReset(i2c);//Se puede usar como verificación de que va recibir o transmitir
	while(i<size){//(i<size)&&(I2Cx_GetSTOPFBit(i2c)==0) Probar con la bandera mejor
		buffer[i]=I2C_ReadByteReceived(i2c);
		i++;
		if(I2Cx_GetFlagSR1(I2C_SR1_BTF, i2c)){
			buffer[i]=I2C_ReadByteReceived(i2c);
			i++;
		}
	}
	I2Cx_WaitSTOPFSet(i2c);//Esperar la secuencia de paro
	I2Cx_ClearSTOPF(i2c);
	I2Cx_ResetACK(i2c);
	I2Cx_SetSTOP(i2c);//Libera las líneas después del último byte, se asegura que el bus está libre
	I2Cx_ResetSlave(i2c);//Se reinicia para evitar glitchs
	//En modo esclavo ocurre un error de UNDERRUN y OVERRUN, para este caso se reinicia la interfaz
	return true;
}
//------------------------------------------------------------------------------------
//                                 I2C With Interrupts
//------------------------------------------------------------------------------------
//Master Tx mode with interrupt
void I2Cx_MasterTx_IT(uint16_t address, uint8_t size, uint8_t *buffer, I2C_HandlerDef * i2c){//Activar interrupciones aquí
	i2c->Parameter.TX_RX_mode=0;
	i2c->sizeI2C=size;
	i2c->auxsizeI2C=size;
	i2c->Parameter.ADDRDevice=address;
	i2c->currentState=0;
	i2c->pBuffer = buffer;
	I2Cx_EnableIT(i2c);
	I2Cx_ResetPOS(i2c);
	I2Cx_SetSTART(i2c);
}

//Master RX mode with interrupt
void I2Cx_MasterRx_IT(uint16_t address, uint8_t size, uint8_t *buffer , I2C_HandlerDef * i2c){//Activar interrupciones aqui
	i2c->Parameter.TX_RX_mode=1;
	i2c->sizeI2C=size;
	i2c->auxsizeI2C=size;
	i2c->Parameter.ADDRDevice=address;
	i2c->currentState=0;
	i2c->pBuffer = buffer;
	I2Cx_EnableIT(i2c);
	I2Cx_ResetPOS(i2c);
	I2Cx_SetACK(i2c);
	I2Cx_SetSTART(i2c);
}
//Slave Tx mode with interrupt
void I2Cx_SlaveTx_IT(uint8_t size, uint8_t *buffer, I2C_HandlerDef * i2c){
	i2c->Parameter.TX_RX_mode=0;
	i2c->auxsizeI2C=size;
	i2c->currentState=0;
	i2c->pBuffer = buffer;
}
//Slave Rx mode with interrupt
void I2Cx_SlaveRx_IT(uint8_t size, uint8_t *buffer ,I2C_HandlerDef * i2c){
	i2c->Parameter.TX_RX_mode=1;
	i2c->auxsizeI2C=size;
	i2c->currentState=0;
	i2c->pBuffer = buffer;
}
//Handler IR
//Interrupt Event
void I2C1_EV_IRQHandler(){
	I2Cx_EV_CallBack(I2C1, &I2C1_Struct);
}

void I2C1_ER_IRQHandler(){
	I2Cx_ER_CallBack(I2C1, &I2C1_Struct);
}

void I2C2_EV_IRQHandler(){
	I2Cx_EV_CallBack(I2C2, &I2C2_Struct);
}

void I2C2_ER_IRQHandler(){
	I2Cx_ER_CallBack(I2C2, &I2C2_Struct);
}

void I2C3_EV_IRQHandler(){
	I2Cx_EV_CallBack(I2C3, &I2C3_Struct);
}

void I2C3_ER_IRQHandler(){
	I2Cx_ER_CallBack(I2C3, &I2C3_Struct);
}

void I2Cx_EV_CallBack(uint8_t x, I2C_HandlerDef * i2c){
	I2Cx_SetICPREV(x);//Limpia posible bandera pendiente
	if(I2Cx_GetFlagSR2(I2C_SR2_MSL, i2c)){//Master mode
		if(I2Cx_GetFlagSR1(I2C_SR1_SB, i2c)){//SB set
			if((i2c->Parameter.modeAddressing)==1){//10 bits
				if((i2c->Parameter.TX_RX_mode)==0){//Secuencia Modo Tx
					I2Cx_ADDR10Header(i2c->Parameter.ADDRDevice, 0, i2c);//Primera cabecera
				}
				else{//Secuencia Modo Rx
					if((i2c->currentState)==0){
						I2Cx_ADDR10Header(i2c->Parameter.ADDRDevice, 0, i2c);//Primera cabecera
					}
					else{
						I2Cx_ADDR10Header(i2c->Parameter.ADDRDevice, 1, i2c);//Segunda cabecera
					}
				}
			}
			else{//7 bits
				I2Cx_ADDR7(i2c->Parameter.ADDRDevice, i2c->Parameter.TX_RX_mode, i2c);//Cambiar parámetros
			}
		}
		else if(I2Cx_GetFlagSR1(I2C_SR1_ADD10, i2c)){//ADD10 set
			I2Cx_ADDR10LastADDR(i2c->Parameter.ADDRDevice, i2c);//Cambiar parámetros
		}
		else if(I2Cx_GetFlagSR1(I2C_SR1_ADDR, i2c)){//ADDR set

			if(((i2c->Parameter.TX_RX_mode)==1)&&((i2c->Parameter.modeAddressing)==1)&&((i2c->currentState)==0)){//Verificamos Secuencia para 10 bits Rx
				I2Cx_ClearADDR(i2c);
				I2Cx_SetSTART(i2c);
				i2c->currentState++;
			}
			else{

				if((i2c->auxsizeI2C)==1){
					I2C_1ByteReceive(i2c);
				}
				else if((i2c->auxsizeI2C)==2){
					I2C_2ByteReceive(i2c);
				}
				else{
					I2Cx_ClearADDR(i2c);
					i2c->currentState=0;
				}
			}

			//Recordar las posibilidades para 1 byte y 2 bytes de transferencia
		}
		else if(I2Cx_GetFlagSR2(I2C_SR2_TRA, i2c)){//Master transmit
			//TXE=1, BTF=0
			if ((I2Cx_GetFlagSR1(I2C_SR1_TXE, i2c))&&(!I2Cx_GetFlagSR1(I2C_SR1_BTF, i2c))){
				if((i2c->sizeI2C)>0){
					i2c->Registers->DR = *i2c->pBuffer;
					i2c->pBuffer++;
					i2c->sizeI2C--;
				}
			}
			//TXE=1, BTF=1
			else if(I2Cx_GetFlagSR1(I2C_SR1_BTF, i2c)){
				if((i2c->sizeI2C)>0){
					i2c->Registers->DR = *i2c->pBuffer;
					i2c->pBuffer++;
					i2c->sizeI2C--;
				}
				else{
					I2Cx_SetSTOP(i2c);
				}
			}
		}
		else{//Master Receive
			//Deshabilitar interrupción por buffer para realizar la subrutina de los últimos 3 bytes
			//RXNE=1, BTF=0
			if ((I2Cx_GetFlagSR1(I2C_SR1_TXE, i2c))&&(!I2Cx_GetFlagSR1(I2C_SR1_BTF, i2c))){
				if((i2c->sizeI2C)>3){
					*i2c->pBuffer = i2c->Registers->DR;
					i2c->pBuffer++;
					i2c->sizeI2C--;
				}
				else if((i2c->sizeI2C)==1){
					//I2C1_CR2 &= (~I2C_CR2_ITBUFEN);
					I2Cx_DisableITBUFEN(i2c);
					//I2Cx_ResetACK(i2c);
					I2Cx_Stop(i2c);
					*i2c->pBuffer = i2c->Registers->DR;
					i2c->sizeI2C--;
				}
				else{
					//I2C1_CR2 &= (~I2C_CR2_ITBUFEN);
					I2Cx_DisableITBUFEN(i2c);
				}
			}
			//RXNE=1, BTF=1
			else if(I2Cx_GetFlagSR1(I2C_SR1_BTF, i2c)){
				if((i2c->sizeI2C)==3){
					//I2C1_CR2 &= (~I2C_CR2_ITBUFEN);
					I2Cx_DisableITBUFEN(i2c);
					if((i2c->currentState)==0){
						I2Cx_ResetACK(i2c);
						i2c->currentState++;
					}
					*i2c->pBuffer = i2c->Registers->DR;
					i2c->pBuffer++;
					i2c->sizeI2C--;
				}
				else if((i2c->sizeI2C)==2){
					if((i2c->currentState)==0){
						I2Cx_ResetACK(i2c);
						i2c->currentState++;
					}
					else if((i2c->currentState)!=0){
						I2Cx_Stop(i2c);
						if((i2c->sizeI2C)==2){
							*i2c->pBuffer = i2c->Registers->DR;
							i2c->pBuffer++;
							i2c->sizeI2C--;
							*i2c->pBuffer = i2c->Registers->DR;
							i2c->sizeI2C--;
						}
						else{
							*i2c->pBuffer = i2c->Registers->DR;
						    i2c->pBuffer++;
							i2c->sizeI2C--;
							*i2c->pBuffer = i2c->Registers->DR;
					        i2c->pBuffer++;
							i2c->sizeI2C--;
						}
						i2c->currentState++;//CAMBIADO
					}
				}
				else{
					*i2c->pBuffer = i2c->Registers->DR;
					i2c->pBuffer++;
					i2c->sizeI2C--;
				}
			}
		}
	}
	else{//Slave Mode
		if(I2Cx_GetFlagSR1(I2C_SR1_ADDR, i2c)){//ADDR set
			i2c->sizeI2C=0;
			i2c->auxsizeI2C=7;//Declarado para trnsmisión continúa (pruebas)
			I2Cx_ClearADDR(i2c);
		}
		else if(I2Cx_GetFlagSR1(I2C_SR1_STOPF, i2c)){
			I2Cx_ClearSTOPF(i2c);
			I2Cx_ResetACK(i2c);
			I2Cx_ResetACK(i2c);
			I2Cx_SetSTOP(i2c);//Libera las líneas después del último byte, se asegura que el bus está libre
			I2Cx_ResetSlave(i2c);//Se reinicia para evitar glitchs
			//En modo esclavo ocurre un error de UNDERRUN y OVERRUN, para este caso se reinicia la interfaz
			I2Cx_EnableIT(i2c);
			i2c->sizeI2C=0;
		}
		else if((I2Cx_GetFlagSR2(I2C_SR2_TRA, i2c))&&((i2c->Parameter.TX_RX_mode)==0)){//Modo transmisor
			//TXE=1, BTF=0
			if ((I2Cx_GetFlagSR1(I2C_SR1_TXE, i2c))&&(!I2Cx_GetFlagSR1(I2C_SR1_BTF, i2c))){
				if((i2c->auxsizeI2C)>0){
					i2c->Registers->DR = *i2c->pBuffer;
				    i2c->pBuffer++;
					i2c->sizeI2C++;
					i2c->auxsizeI2C--;
				}
				if((i2c->auxsizeI2C)==0){
					//I2C1_CR2 &= (~I2C_CR2_ITBUFEN);//Desactivamos Interrupción por buffer
					I2Cx_DisableITBUFEN(i2c);
				}
			}
			//TXE=1, BTF=1
			else if(I2Cx_GetFlagSR1(I2C_SR1_BTF, i2c)){
				if((i2c->auxsizeI2C)>0){
					i2c->Registers->DR = *i2c->pBuffer;
					i2c->pBuffer++;
					i2c->sizeI2C++;
					i2c->auxsizeI2C--;
				}
				//if(auxsizeI2C==0){
					//I2C1_CR2 &= (~I2C_CR2_ITBUFEN);//Desactivamos Interrupción por buffer
				//}
			}
		}
		else{//Modo receptor
			if((i2c->Parameter.TX_RX_mode)==1){//Aseguramos que se configuró la recepción
				//RXNE=1, BTF=0
				if ((I2Cx_GetFlagSR1(I2C_SR1_TXE, i2c))&&(!I2Cx_GetFlagSR1(I2C_SR1_BTF, i2c))){
					if((i2c->auxsizeI2C)>0){
						*i2c->pBuffer = i2c->Registers->DR;
						i2c->pBuffer++;
						i2c->sizeI2C++;
						i2c->auxsizeI2C--;
					}
					if((i2c->auxsizeI2C)==0){
						//I2C1_CR2 &= (~I2C_CR2_ITBUFEN);//Desactivamos Interrupción por buffer
						I2Cx_DisableITBUFEN(i2c);
					}
				}
				//RXNE=1, BTF=1
				else if(I2Cx_GetFlagSR1(I2C_SR1_BTF, i2c)){
					if((i2c->auxsizeI2C)>0){
						*i2c->pBuffer = i2c->Registers->DR;
						i2c->pBuffer++;
						i2c->sizeI2C++;
						i2c->auxsizeI2C--;
					}
					//if(auxsizeI2C==0){
						//I2C1_CR2 &= (~I2C_CR2_ITBUFEN);//Desactivamos Interrupción por buffer
					//}
				}
			}
		}
	}
}
//Interrupt ERROR
void I2Cx_ER_CallBack(uint8_t x, I2C_HandlerDef * i2c){
	char currErr[4];
	I2Cx_SetICPRER(x);
	I2Cx_GetCurrentError(currErr, i2c);
	if((currErr[0]=='B')&&(currErr[1]=='E')&&(currErr[2]=='R')){//BERR
		//Set Flag
		I2Cx_ResetBERR(i2c);//Clear Flag
	}
	else if((currErr[0]=='A')&&(currErr[1]=='R')&&(currErr[2]=='L')){//ARLO
		//Set Flag
		I2Cx_ResetARLO(i2c);//Clear Flag
	}
	else if((currErr[0]=='A')&&(currErr[1]=='F')){//AF
		//Set Flag
		I2Cx_ResetAF(i2c);
		I2Cx_ResetACK(i2c);
		I2Cx_SetSTOP(i2c);//Libera las líneas después del último byte, se asegura que el bus está libre
		I2Cx_ResetSlave(i2c);//Se reinicia para evitar glitchs
		//En modo esclavo ocurre un error de UNDERRUN y OVERRUN, para este caso se reinicia la interfaz
		I2Cx_EnableIT(i2c);
	}
	if((currErr[0]=='O')&&(currErr[1]=='V')&&(currErr[2]=='R')){//OVR
		//Set Flag
		I2Cx_ResetOVR(i2c);//Clear Flag
	}
}
/*
 * Flags which may cause an interrupt
 * 1-> Means set
 * 0-> Means reset
 */
bool I2Cx_GetFlagSR1(uint8_t flag, I2C_HandlerDef * i2c){
	bool flag_Set = false;
	uint8_t Flags_SR1 = I2Cx_GetFlagsSR1(i2c) ;

	if((flag&Flags_SR1)!=0){
		flag_Set = true;
	}

	 return flag_Set;
}

bool I2Cx_GetFlagSR2(uint8_t flag, I2C_HandlerDef * i2c){
	bool flag_Set = false;
	uint8_t Flags_SR2 = I2Cx_GetFlagsSR2(i2c) ;

	if((flag&Flags_SR2)!=0){
		flag_Set = true;
	}

	return flag_Set;
}

/*
 * Regresa las banderas de estados SR1
 */
uint8_t I2Cx_GetFlagsSR1(I2C_HandlerDef * i2c){
	uint8_t FlagsBits;
	FlagsBits = ((i2c->Registers->SR1) & 0xFF);
	 return FlagsBits;
}
/*
 * Regresa las banderas de estados SR2
 */
uint8_t I2Cx_GetFlagsSR2(I2C_HandlerDef * i2c){
	uint8_t FlagsBits;
	FlagsBits = ((i2c->Registers->SR2) & 0x7);
	return FlagsBits;
}

uint16_t I2Cx_GetCCR(I2C_HandlerDef * i2c){
	uint16_t CCRBits;
	CCRBits = (i2c->Registers->CCR);
	return CCRBits;
}

/*
 * Error conditions which may cause communication to fail.
 * 1. BERR,
 * 2. ARLO,
 * 3. AF,
 * 4. OVR
 * Esta función prioriza los errores existentes, por lo que BERR es el que myor afecta a la comunicación
 */
void I2Cx_GetCurrentError(char * error, I2C_HandlerDef * i2c){
	uint8_t ErrorBits = I2Cx_GetErrorsSR1(i2c);
	uint8_t i = 4;
	while(i>0){
		if(ErrorBits&8){
			switch (i) {
			case 1:
				error[0]='B';
				error[1]='E';
				error[2]='R';
				error[3]='R';
				break;
			case 2:
				error[0]='A';
				error[1]='R';
				error[2]='L';
				error[3]='O';
				break;
			case 3:
				error[0]='A';
				error[1]='F';
				break;
			case 4:
				error[0]='O';
				error[1]='V';
				error[2]='R';
				break;
			default:
				break;
			}
		}
		ErrorBits <<= 1;
		i--;
	}
 }
/*
 * Regresa el regitro de estados SR1
 */
 uint8_t I2Cx_GetErrorsSR1(I2C_HandlerDef * i2c){
	 uint8_t ErrorBits;
	 ErrorBits = (((i2c->Registers->SR1)&0x0F00)>>I2C_SR1_BERR_Pos);
	 return ErrorBits;
 }

/*
 * Lectura de registros de estados de I2C
 */
//Se leen los registros SR1, seguido de SR2 para limpiar el bit ADDR
void I2Cx_ClearADDR(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_ADDR)==0);//Lee SR1
	while(((i2c->Registers->SR2)&&I2C_SR2_BUSY)==0);//Lee SR2
	//Se limpia ADDR
}
//Limpia la bandera STOP
void I2Cx_ClearSTOPF(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_STOPF)==0);//Lee SR1
	i2c->Registers->CR1 |= I2C_CR1_PE;//Escribe en CR1
}
/*
 * Data Byte transfer finished: 0-> not done, 1-> succeeded
 * In reception when RxNE=1
 * In Transmission TxE=1
 */
void I2Cx_WaitBTFSet(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_BTF)==0);//Esperar hasta que BTF = 1
}
/*
 * Start Bit: 1-> Start condition generated, 0-> No start
 */
void I2Cx_WaitSBSet(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_SB)==0);
}

/*
 * Address sent (Master): 0-> No end of ADDR, 1-> End of ADDR
 * matched(slave) : 1-> ADDR matched
 */
void I2Cx_WaitADDRSet(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_ADDR)==0);
}
/*
 * 0: No ADD10 event occurred
 * 1: Master has sent first address byte (header).
 */
void I2Cx_WaitADD10Set(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_ADD10)==0);
}
/*
 * 0->Data register empty in transmission
 * 1->Data register no empty in transmission
 * TxE is not cleared by writing the first data being transmitted, or by writing data when
BTF is set, as in both cases the data register is still empty.
 */
void I2Cx_WaitTXESet(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_TXE)==0);//Registro de datos no vacío
}
/*
 * 0 -> data register is not empty in receiver mode
 * 1 -> data register is empty in receiver mode
 * RxNE is not cleared by reading data when BTF is set, as the data register is still full
 */
void I2Cx_WaitRXNESet(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_RXNE)==0);//Registro de datos vacío
}
/*
 * 0->Slave mode
 * 1->Master Mode
 */
void I2Cx_WaitMSLSet(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR2)&&I2C_SR2_MSL)==0);
}
/*
 * 0->Data Bytes received
 * 1->Data Bytes Transmitted
 */
void I2Cx_WaitTRASet(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR2)&&I2C_SR2_TRA)==0);
}
/*
 * 0->No communication
 * 1->Communitaction goin on the bus
 */
void I2Cx_WaitBUSYSet(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR2)&&I2C_SR2_BUSY)==0);
}
/*
 * STOP DETECTION (slave mode)
 * 0 -> No stop condition detected
 * 1 -> Stop condition detected
 */
uint16_t I2Cx_GetSTOPFBit(I2C_HandlerDef * i2c){
	uint16_t value;
	value=((i2c->Registers->SR1)&I2C_SR1_STOPF);
	return value;
}

void I2Cx_WaitSTOPFSet(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_STOPF)==0);
}
/*
 * 0->No acknowledge failure
 * 1->Acknowledge failure
 */
uint16_t I2Cx_GetAFBit(I2C_HandlerDef * i2c){
	uint16_t value;
	value=((i2c->Registers->SR1)&I2C_SR1_AF);
	return value;
}

void I2Cx_WaitAFSet(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_AF)==0);
}
/* ARLO, Set by hardware when the interface loses the arbitration of the bus to another master
 * 0->No Arbitration Lost detected
 * 1->Arbitration Lost detected
 */
uint16_t I2Cx_GetARLOBit(I2C_HandlerDef * i2c){
	uint16_t value;
	value=((i2c->Registers->SR1)&I2C_SR1_ARLO);
	return value;
}
void I2Cx_WaitBTFReset(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_BTF)!=0);//Esperar hasta que BTF = 0
}

void I2Cx_WaitSBReset(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_SB)!=0);
}

void I2Cx_WaitADDRReset(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_ADDR)!=0);
}

void I2Cx_WaitTXEReset(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_TXE)!=0);//Registro de datos no vacío
}

void I2Cx_WaitRXNEReset(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_RXNE)!=0);//Registro de datos vacío}
}

void I2Cx_WaitMSLReset(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR2)&&I2C_SR2_MSL)!=0);
}

void I2Cx_WaitTRAReset(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR2)&&I2C_SR2_TRA)!=0);
}

void I2Cx_WaitBUSYReset(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR2)&&I2C_SR2_BUSY)!=0);
}

void I2Cx_WaitSTOPFReset(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_STOPF)!=0);//Registro de datos vacío
}

void I2Cx_WaitAFReset(I2C_HandlerDef * i2c){
	while(((i2c->Registers->SR1)&&I2C_SR1_AF)!=0);//
}

void I2Cx_SetSTART(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 |= I2C_CR1_START;
}
//Indica
void I2Cx_SetSTOP(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 |= I2C_CR1_STOP;
}
void I2Cx_SetPOS(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 |= I2C_CR1_POS;//POS High
}

void I2Cx_SetACK(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 |= I2C_CR1_ACK;
}

void I2Cx_SetPE(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 |= I2C_CR1_PE ;//Deshabilitar
}

void I2Cx_SetSWRST(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 |= I2C_CR1_SWRST;//Reset I2C
}

void I2Cx_ResetACK(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 &= (~I2C_CR1_ACK);//ACK Low
}

void I2Cx_ResetSTART(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 &= (~I2C_CR1_START);
}

void I2Cx_ResetAF(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 &= (~I2C_SR1_AF);
}

void I2Cx_ResetPOS(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 &= (~I2C_CR1_POS);//POS Low
}

void I2Cx_ResetSTOP(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 &= (~I2C_CR1_STOP);
}

void I2Cx_ResetPE(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 &= (~I2C_CR1_PE) ;//Deshabilitar
}

void I2Cx_ResetSWRST(I2C_HandlerDef * i2c){
	i2c->Registers->CR1 &= (~I2C_CR1_SWRST);//Reset I2C
}

void I2Cx_ResetBERR(I2C_HandlerDef * i2c){
	i2c->Registers->SR1 &= (~I2C_SR1_BERR);
}

void I2Cx_ResetARLO(I2C_HandlerDef * i2c){
	i2c->Registers->SR1 &= (~I2C_SR1_ARLO);
}

void I2Cx_ResetOVR(I2C_HandlerDef * i2c){
	i2c->Registers->SR1 &= (~I2C_SR1_OVR);
}

void I2Cx_SetICPREV(uint8_t x){
	if(x==1){
		NVIC_ICPR0 |= (1<<NVIC_I2C1EV_ICPR0_POS); //I2C1 EVENT Limpia posible bandera pendiente
	}
	else if(x==2){
		NVIC_ICPR1 |= (1<<NVIC_I2C2EV_ICPR1_POS); //I2C1 EVENT Limpia posible bandera pendiente
	}
	else if(x==3){
		NVIC_ICPR2 |= (1<<NVIC_I2C3EV_ICPR2_POS); //I2C1 EVENT Limpia posible bandera pendiente
	}
}

void I2Cx_SetICPRER(uint8_t x){
	if(x==1){
		NVIC_ICPR0 |= (1<<NVIC_I2C1ER_ICPR1_POS); //I2C1 EVENT Limpia posible bandera pendiente
	}
	else if(x==2){
		NVIC_ICPR1 |= (1<<NVIC_I2C2ER_ICPR1_POS); //I2C1 EVENT Limpia posible bandera pendiente
	}
	else if(x==3){
		NVIC_ICPR2 |= (1<<NVIC_I2C3ER_ICPR2_POS); //I2C1 EVENT Limpia posible bandera pendiente
	}
}


void I2Cx_SetFLTR(uint8_t mode, uint8_t APB1_freq, I2C_HandlerDef * i2c){
	uint8_t valueFLTR=0;
	if((mode<=0)||(mode>1)){//Standard Mode
		if(APB1_freq<=5){
			valueFLTR=2;
		}
		else if((APB1_freq>5)&&(APB1_freq<=10)){
			valueFLTR=12;
		}
		else{
			valueFLTR=15;
		}
	}
	else if(mode==1){//Fast Mode

		if(APB1_freq<=10){
			valueFLTR=0;
		}
		else if((APB1_freq>10)&&(APB1_freq<=20)){
			valueFLTR=1;
		}
		else if((APB1_freq>20)&&(APB1_freq<=30)){
			valueFLTR=7;
		}
		else if((APB1_freq>30)&&(APB1_freq<=40)){
			valueFLTR=13;
		}
		else{
			valueFLTR=15;
		}
	}

	i2c->Registers->FLTR |= (valueFLTR<<I2C_FLTR_DNF_Pos); //
	i2c->Registers->FLTR &= (~I2C_FLTR_ANOFF);

}

void I2Cx_SetFREQ(uint8_t APB1_freq, I2C_HandlerDef * i2c){
	i2c->Registers->CR2 |= APB1_freq;//10MHz APB1
}

void RCC_EnI2Cx(I2C_HandlerDef * i2c){
	if(i2c==&(I2C1_Struct)){
		RCC_APB1ENR |= (RCC_APB1ENR_PWREN|RCC_APB1ENR_I2C1EN);//Habilitar Fuente de reloj antes de configurar
	}
	else if(i2c==&(I2C2_Struct)){
		RCC_APB1ENR |= (RCC_APB1ENR_PWREN|RCC_APB1ENR_I2C2EN);//Habilitar Fuente de reloj antes de configurar
	}
	else if(i2c==&(I2C3_Struct)){
		RCC_APB1ENR |= (RCC_APB1ENR_PWREN|RCC_APB1ENR_I2C3EN);//Habilitar Fuente de reloj antes de configurar
	}
}

void I2Cx_SetMode(uint8_t mode, I2C_HandlerDef * i2c){

	if((mode<=0)||(mode>1)){//Standard Mode
		i2c->Registers->CCR &= (~I2C_CCR_FS);//Standard speed
		i2c->Parameter.speed = 0;
	}
	else if(mode==1){//Fast Mode
		i2c->Registers->CCR |= (I2C_CCR_FS);//Fast speed
		i2c->Parameter.speed = 1;
	}
}

void I2Cx_SetCCR(uint16_t CCR, uint8_t DUTY, I2C_HandlerDef * i2c){
	i2c->Registers->CCR |= CCR;// THIGH = CCR * TCKLP1 = 833ns, TCLKP1-> 1/APB1_freq
	if(DUTY){
		i2c->Registers->CCR |= I2C_CCR_DUTY;
	}
	else{
		i2c->Registers->CCR &= (~I2C_CCR_DUTY);
	}
}

void I2Cx_SetTRISE(uint8_t TRISE, I2C_HandlerDef * i2c){
	i2c->Registers->TRISE = TRISE;//
}

void I2Cx_SetBit14OAR1(I2C_HandlerDef * i2c){
	i2c->Registers->OAR1 |= 0x4000;//Bit se debe poner en alto por software
}

void I2Cx_EnableIT(I2C_HandlerDef * i2c){
	i2c->Registers->CR2 |= (I2C_CR2_ITBUFEN|I2C_CR2_ITERREN|I2C_CR2_ITEVTEN);//Habilitando interrupciones
}

void I2Cx_DisableIT(I2C_HandlerDef * i2c){
	i2c->Registers->CR2 &= ((~I2C_CR2_ITBUFEN)&(~I2C_CR2_ITERREN)&(~I2C_CR2_ITEVTEN));//dehabilitando interrupciones
}

void I2Cx_DisableITBUFEN(I2C_HandlerDef * i2c){
	i2c->Registers->CR2 &= (~I2C_CR2_ITBUFEN);//Desactivamos Interrupción por buffer
}


void I2Cx_SetOAR1(uint16_t address, uint8_t modeAddressing, I2C_HandlerDef * i2c){
	i2c->Registers->OAR1 |= 0x4000;//Bit se debe poner en alto por software
	i2c->Registers->OAR1 |= (modeAddressing<<I2C_OAR1_ADDMODE_Pos);//Selecciona modo de dirección (7 o 10 bits), bit 14 siempre en alto
	if(modeAddressing<1){//7 bits addressing
		i2c->Registers->OAR1 |= (address<<I2C_OAR1_ADD1_Pos);//Agrega la dirección
	}
	else{//10 bits addressing
		i2c->Registers->OAR1 |= address;//Agrega la dirección
	}
}

void I2Cx_ADDR10Header(uint16_t address, uint8_t rw, I2C_HandlerDef * i2c){
	(i2c->Registers->SR1)&&I2C_SR1_SB;//Lee SR1
	i2c->Registers->DR = (((uint8_t)(((uint8_t)((address&((uint16_t)(0x0300)))>>7))|((uint8_t)HEADER)))|rw);
}

void I2Cx_ADDR10LastADDR(uint16_t address, I2C_HandlerDef * i2c){
	(i2c->Registers->SR1)&&I2C_SR1_ADD10;//Lee SR1
	i2c->Registers->DR = ((uint8_t)(((uint16_t)0x00FF)&address));
}

void I2Cx_ADDR7(uint8_t address, uint8_t rw, I2C_HandlerDef * i2c){
	(i2c->Registers->SR1)&&I2C_SR1_ADDR;//Lee SR1
	i2c->Registers->DR = (address<<1)|(rw);
}

void I2Cx_WriteDR(uint8_t data, I2C_HandlerDef * i2c){
	i2c->Registers->DR = data;
}

uint8_t I2Cx_ReadDR(I2C_HandlerDef * i2c){
	uint8_t valueDR=0;
	valueDR=i2c->Registers->DR;
	return valueDR;
}

void  I2Cx_SetADDMODE(uint8_t modeAddressing, I2C_HandlerDef * i2c){
	i2c->Registers->OAR1 |= (modeAddressing<<I2C_OAR1_ADDMODE_Pos);//Selecciona modo de dirección (7 o 10 bits), bit 14 siempre en alto
}
/*
 * 10 instrucciones por un for
 * t = #instrucciones / SYSCLK_FREQ
 * 1010 ns son necesarios a 79MHz, se requiere un delay de 1us
 */
void I2Cx_ADDR10Delay(){
	uint8_t i, load;
	load = ((currentSYSCLK*12) /100); //Cálculo de la carga, se deja a 1200us y escalamos por 10
	for (i = 0; i < load; ++i);//Delay para Generar una correcta secuencia
}

