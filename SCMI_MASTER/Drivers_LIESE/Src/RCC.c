/*
 * RCC.c
 *
 *  Created on: 15 nov 2022
 *      Author: jurl9
 */

#include "RCC.h"
#include "SYSCFG.h"
#include "embeddedFLASH.h"
#include "GPIOx.h"
RCC_CLKCFG SYS_CLK;//Variable a utilizar
/*RCC es 16MHz de manera predeterminada
//Configuración predeterminada RCC
//PLLM: 0.95MHz-2.1MHz
//PLLN:100MHz-432MHz
//PLLP_R: 24MHz-180MHz
//AHB1: 2MHz-180MHz
//APB1: 2MHz-45MHz
 * SYSCLOCK = ( (SOURCEPLL / PRE_PLLM) * MUL_PLLN ) / PRE_PLLP -> Seguir en este orden la configuración
 * para cumplir con el rango de valores mostrados arriba
 * APB1CLOCK = ( SYSCLOCK / PRE_AHB1 ) / APB1
 * APB1CLK in MHz
 */
bool SystClock_Init(RCC_CLKCFG *SYSCLKCFG, uint8_t sourceSYS, uint8_t sourcePLL, uint8_t preAHB1, uint8_t preAPB1, uint8_t preAPB2){
	uint8_t SYSCLK = SYSCLKCFG->SYSCLK;
	RCC_APB1ENR |= RCC_APB1ENR_PWREN;
	PWR_CR &= (~(0x3<<PWR_CR_VOS_Pos));//Limpiamos
	PWR_CR |= (1<<PWR_CR_VOS_Pos);//Selecciona modo 3 (120MHz Max)
	//Se ignora sourcePLL y SYSCLK si se escoge directamente el valor de la fuente
	if(SYSCLK==HSICLK){//HSI
		RCC_CFGR = RCC_CFGR_SW_HSI; //HSI como fuente del sistema
		SystCLK_SetPres(SYSCLKCFG, preAHB1, preAPB1, preAPB2);
		SystCLK_SetHSION();
	}
	else if(SYSCLK==HSECLK){//HSE
		RCC_CFGR = RCC_CFGR_SW_HSE; //HSE como fuente del sistema
		SystCLK_SetPres(SYSCLKCFG, preAHB1, preAPB1, preAPB2);
		SystCLK_SetHSEON();
	}
	else{
		if((SYSCLK>=24)&&(SYSCLK<=180)){//Min out CLK 24MHz, max 180MHz, puede ser PLLP o PLLR
			if(sourcePLL==0){//Source PLL -> HSI = 16 MHz, se trabajan números enteros
				SystCLK_CalculatePLLCFGR(SYSCLKCFG, sourceSYS, HSICLK, preAHB1, preAPB1, preAPB2, 8);//Configura e inicializa
			}
			else{//Source PLL -> HSE
				RCC_EnPort(GPIOH);
				SystCLK_CalculatePLLCFGR(SYSCLKCFG, sourceSYS, HSECLK, preAHB1, preAPB1, preAPB2, 1);//Configura e inicializa
			}
		}
		else{//Configuración predeterminada en PLL
			SystCLK_SetPLLPredet();//SYS = 40MHz, APB1 = 10MHz
		}
	}
	return true;
}

//
uint8_t Calculate_Pot2(uint8_t pot){
	uint8_t value=1;
	while(pot>0){
		value*=2;
		pot--;
	}
	return value;
}
//
void SystCLK_SetPres(RCC_CLKCFG *SYSCLKCFG, uint8_t preAHB1, uint8_t preAPB1, uint8_t preAPB2){
	uint8_t SYSCLK = SYSCLKCFG->SYSCLK;
	uint8_t divAHB1 = (SYSCLK) / (Calculate_Pot2(preAHB1));
	uint8_t divAPB1 = 0;//(SYSCLKCFG->SYSCLK) / (Calculate_Pot2(preAHB1+preAPB1));
	uint8_t divAPB2 = 0;//(SYSCLKCFG->SYSCLK) / (Calculate_Pot2(preAHB1+preAPB2));
	uint8_t i=0;
	uint32_t decimal;

	if((SYSCLK)&1){//Si es un número impar
		SYSCLK++;//Lo hacemos Par
		SYSCLKCFG->SYSCLK++;
		divAHB1 = (SYSCLK) / (Calculate_Pot2(preAHB1));
	}

	if(preAHB1>4){
		divAHB1 /= 2;
	}

	if(divAHB1>=2){//Dentro del rango
		if(preAHB1!=0){
			RCC_CFGR |= ((7+preAHB1)<<RCC_CFGR_HPRE_Pos);
		}
		SYSCLKCFG->AHB1CLK = divAHB1;
	}
	else{
		RCC_CFGR |= (RCC_CFGR_HPRE_DIV1);//Dividir por 1 AHB1
		SYSCLKCFG->AHB1CLK = SYSCLK;
		preAHB1 = 0;
	}

	//Calculamos nuevos valores
	divAPB1 = (SYSCLK) / (Calculate_Pot2(preAHB1+preAPB1));
	divAPB2 = (SYSCLK) / (Calculate_Pot2(preAHB1+preAPB2));

	if(preAHB1>4){
		divAPB1 /= 2;
		divAPB2 /= 2;
	}

	if((divAPB1>=2)&&(divAPB1<=45)){
	    if(preAPB1!=0){
	    	RCC_CFGR |= ((3+preAPB1)<<RCC_CFGR_PPRE1_Pos);
		}
	    decimal = (((uint32_t)(divAPB1))*100) - ((100*(SYSCLK)) / (Calculate_Pot2(preAHB1+preAPB1)));
		SYSCLKCFG->APB1CLK = divAPB1;
	}
	else{
		i=0;
		while((divAPB1<=45)||(i>=5)) {
			divAPB1 = (SYSCLK) / (Calculate_Pot2(i+preAHB1));
			if(preAHB1>4){
				divAPB1 /= 2;
			}
		}
		if(i!=0){
			RCC_CFGR |= ((3+i)<<RCC_CFGR_PPRE1_Pos);
		}
		decimal = (((uint32_t)(divAPB1))*100) - ((100*(SYSCLK)) / (Calculate_Pot2(preAHB1+i)));
		SYSCLKCFG->APB1CLK = divAPB1;
	}

	if((divAPB2>=2)&&(divAPB2<=45)){
		if(preAPB2!=0){
			RCC_CFGR |= ((3+preAPB2)<<RCC_CFGR_PPRE2_Pos);
		}
		decimal = (((uint32_t)(divAPB2))*100) - ((100*(SYSCLK)) / (Calculate_Pot2(preAHB1+preAPB2)));
		SYSCLKCFG->APB2CLK = divAPB2;
	}
	else{
		i=0;
		while((divAPB2<=45)||(i>=5)) {
			divAPB2 = (SYSCLK) / (Calculate_Pot2(i+preAHB1));
			if(preAHB1>4){
				divAPB2 /= 2;
			}
		}
		if(i!=0){
			RCC_CFGR |= ((3+i)<<RCC_CFGR_PPRE2_Pos);
		}
		decimal = (((uint32_t)(divAPB2))*100) - ((100*(SYSCLK)) / (Calculate_Pot2(preAHB1+i)));
		SYSCLKCFG->APB2CLK = divAPB2;
	}

	if(decimal){
		SYSCLKCFG->precision = false;
	}
	else{
		SYSCLKCFG->precision = true;
	}

	//Al trabajar con más de 50MHz en los periféricos se recomienda habilitar la configuración
	if(divAHB1>=50){
		RCC_APB2ENR |= RCC_APB2ENR_SYSCFGEN;//Revisa cap. 8 del manual rm009
		SYSCFG_SetCMP_PD();
		SYSCFG_WaitREADY();
	}

	embbFLash_Conf(SYSCLKCFG->AHB1CLK);//Se configuran los ciclos de la memoria Flash
}

void SystCLK_SetHSION(){
	RCC_CR = ((0x10<<RCC_CR_HSITRIM_Pos)|(0x68<<RCC_CR_HSICAL_Pos));//Configuración Para calibración
	RCC_CR |= (RCC_CR_HSION);
	while((RCC_CR&&RCC_CR_HSIRDY)==0);
}

void SystCLK_SetHSEON(){
	RCC_CR = (RCC_CR_HSEON);
	while((RCC_CR&&RCC_CR_HSERDY)==0);
}

void SystCLK_SetPLLON(uint8_t sourcePLL){
	if(sourcePLL==1){
		RCC_CR = (RCC_CR_PLLON|RCC_CR_HSEON);//PLL ON, HSE ON, HSE-> Osc. PLL
	}
	else{
		RCC_CR = ((0x10<<RCC_CR_HSITRIM_Pos)|(0x68<<RCC_CR_HSICAL_Pos));//Configuración Para calibración
		RCC_CR |= (RCC_CR_PLLON|RCC_CR_HSION);//PLL ON, HSI ON, HSI-> Osc. PLL
	}
	while((RCC_CR&&RCC_CR_PLLRDY)==0);

}
//
void SystCLK_CalculatePLLCFGR(RCC_CLKCFG *SYSCLKCFG, uint8_t sourceSYS, uint8_t PLLCLK,uint8_t preAHB1, uint8_t preAPB1, uint8_t preAPB2, uint8_t up){
	uint32_t auxClk = 0 ;
	uint16_t SYSCLK = 100*(SYSCLKCFG->SYSCLK);
	uint8_t PLLM_ = 0, PLL_P_R_ = 2;
	uint16_t PLLN_ = 50;
	if(up!=8){
		PLLM_=2;
		auxClk =  ( (100*PLLCLK) / (PLLM_));
		PLLM_=SystCLK_GetPLLMStart(auxClk, PLLCLK, PLLM_, 1);//Obtiene el valor de PLLM para iterar
	}
	else{
		PLLM_=8;
		auxClk =  ( (100*PLLCLK) / (PLLM_));
	}
	while((PLLM_<=PLLCLK)&&(auxClk!=SYSCLK)){//Si las dos décimas son diferente de cero se itera
		PLLN_=50;
		auxClk =  ( (100*PLLCLK) / (PLLM_)) * PLLN_ ;
		PLLN_=SystCLK_GetPLLNStart(auxClk,PLLCLK, PLLM_, PLLN_);//Obtiene el valor de PLLN inicial
		while((PLLN_<=432)&&(auxClk!=SYSCLK)&&(auxClk<=43200)){
			if(((auxClk/7)<SYSCLK)||((auxClk/8)<SYSCLK)){
				PLL_P_R_=2;//PLLP o PLLR
				auxClk /= PLL_P_R_;
				if(sourceSYS==2){//PLLP values: 2, 4, 6, 8
					PLL_P_R_=SystCLK_CalculatePLL_P_R(auxClk, PLLCLK, SYSCLK, PLLM_, PLLN_, PLL_P_R_, 2);
				}
				else{//PLLR values: 2-7
					PLL_P_R_=SystCLK_CalculatePLL_P_R(auxClk, PLLCLK, SYSCLK, PLLM_, PLLN_, PLL_P_R_, 1);
				}
				if(PLL_P_R_>8){
					PLLN_++;
					auxClk = ( 100*PLLCLK / PLLM_) * PLLN_;
				}
				else{
					auxClk = (( 100*PLLCLK / PLLM_) * PLLN_) / PLL_P_R_;//Se obtienen los valores esperados
				}
			}
			else{
				PLLN_=433;
			}
		}
		if(auxClk!=SYSCLK){
			PLLM_+=up;
		}
	}

	if((auxClk!=SYSCLK)&&(PLL_P_R_>8)&&(PLLM_>PLLCLK)&&(PLLN_>432)){//No se ecnontró un valor adecuado
		SystCLK_SetPLLPredet(); //SYS = 80MHz, APB1, APB2 = 40MHz
		SYSCLKCFG->SYSCLK = 80;
	}
	else{//Se llenan los valores
		SYSCLKCFG->SYSCLK = auxClk/100;
		if(sourceSYS==2){//PLLP como fuente del sistema
			PLL_P_R_= ( (PLL_P_R_/2) - 1 );//
			RCC_CFGR = RCC_CFGR_SW_PLL;
			SystCLK_SetPres(SYSCLKCFG, preAHB1, preAPB1, preAPB2);
			RCC_PLLCFGR = ((PLLM_<<RCC_PLLCFGR_PLLM_Pos)|(PLLN_<<RCC_PLLCFGR_PLLN_Pos)|(PLL_P_R_<<RCC_PLLCFGR_PLLP_Pos)|RCC_PLLCFGR_PLLQ_1|RCC_PLLCFGR_PLLR_1);//Configurar antes de activar
		}
		else{//PLLR como fuente del sistema
			RCC_CFGR = RCC_CFGR_SW_PLLR;
			SystCLK_SetPres(SYSCLKCFG, preAHB1, preAPB1, preAPB2);
			RCC_PLLCFGR = ((PLLM_<<RCC_PLLCFGR_PLLM_Pos)|(PLLN_<<RCC_PLLCFGR_PLLN_Pos)|(PLL_P_R_<<RCC_PLLCFGR_PLLR_Pos)|RCC_PLLCFGR_PLLQ_1|RCC_PLLCFGR_PLLR_1);//Configurar antes de activar
		}
		if(PLLCLK==HSECLK){//HSE PLL SOURCE
			RCC_PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
			SystCLK_SetPLLON(1);
		}
		else{//HSI PLL SOURCE
			RCC_PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;
			SystCLK_SetPLLON(0);
		}
	}
}

uint8_t SystCLK_GetPLLMStart(uint32_t freq_100, uint8_t valueCLK, uint8_t PLLM, uint8_t up){
	uint8_t PLLM_=PLLM;
	while(freq_100>200){//Busca el valor de M desde el cuál se puede iterar
		PLLM_+=up;
		freq_100 = ( (100*valueCLK) / PLLM_);
	}
	return PLLM_;
}

uint16_t SystCLK_GetPLLNStart(uint32_t freq_100, uint8_t valueCLK, uint8_t PLLM, uint16_t PLLN){
	uint16_t PLLN_=PLLN;
	while(freq_100<10000){//Busca el valor de N desde el cuál se puede iterar
		PLLN_++;
		freq_100 = ( (100*valueCLK) / PLLM) * PLLN;
	}
	return PLLN_;
}

uint8_t SystCLK_CalculatePLL_P_R(uint32_t freq_100, uint8_t valueCLK, uint16_t SYSCLK, uint8_t PLLM, uint16_t PLLN, uint8_t PLL_P_R, uint8_t up){
	uint8_t PLL_P_R_=PLL_P_R;
	while(freq_100>18000){//Busca el valor desde el cuál se puede iterar
		PLL_P_R_ += up;
		freq_100 = ( ( (100*valueCLK) / PLLM) * PLLN ) / PLL_P_R_;
	}
	if (freq_100>=2400){
		//freq_100=1;//aseguramos que etre al
		while((PLL_P_R_<=8)&&(freq_100!=SYSCLK)){
			PLL_P_R_ += up;
			freq_100 = ( ( (100*valueCLK) / PLLM) * PLLN ) / PLL_P_R_;
			if((freq_100!=SYSCLK)&&((PLL_P_R_==8)|(PLL_P_R_==7))){//Value >= 10
				PLL_P_R_ += 2;
			}
		}
	}
	else{
		PLL_P_R_=9;
	}
	return PLL_P_R_;
}
//
void SystCLK_SetPLLPredet(){
	RCC_CFGR = (RCC_CFGR_SW_PLL|RCC_CFGR_HPRE_DIV1|RCC_CFGR_PPRE1_DIV2|RCC_CFGR_PPRE2_DIV2) ;//SW = PLLP Source, AHB=1, APB1=2, APB2=2, APB1 Source 40MHz para I2C
	RCC_PLLCFGR = (RCC_PLLCFGR_PLLM_1|(80<<RCC_PLLCFGR_PLLN_Pos)|(1<<RCC_PLLCFGR_PLLP_Pos)|RCC_PLLCFGR_PLLQ_1|RCC_PLLCFGR_PLLR_1);//PLL source = HSE, PLLM =80 , PLLN = 40, PLLP = 4. SYS = 80 MHz; configurar antes de activar
	RCC_PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	embbFLash_Conf(80);//Se configuran los ciclos de la memoria Flash
	SystCLK_SetPLLON(1);
}
