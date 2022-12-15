/*
 * RCC.c
 *
 *  Created on: 15 nov 2022
 *      Author: jurl9
 */

#include "RCC.h"
 uint8_t currentSYSCLK=0;
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
bool SystClock_Init(uint8_t sourceSYS, uint8_t sourcePLL, uint8_t SYSCLK, uint8_t preAHB1, uint8_t preAPB1, uint8_t preAPB2){
	RCC_APB1ENR |= RCC_APB1ENR_PWREN;
	PWR_CR &= (~(0x3<<PWR_CR_VOS_Pos));//Limpiamos
	PWR_CR |= (1<<PWR_CR_VOS_Pos);//Selecciona modo 3 (120MHz Mai2c)
	if((sourceSYS>=0)&&(sourceSYS<2)){//Fuente SYSCLK HSI o HSE
		//Se ignora sourcePLL y SYSCLK si se escoge directamente el valor de la fuentee
		if(sourceSYS==0){//HSI
			currentSYSCLK=HSICLK;//16MHz
			RCC_CFGR = RCC_CFGR_SW_HSI; //HSI como fuente del sistema
			SystCLK_SetPres(preAHB1, preAPB1, preAPB2);
			SystCLK_SetHSION();
		}
		else{//HSE
			currentSYSCLK=HSECLK;//16MHz
			RCC_CFGR = RCC_CFGR_SW_HSE; //HSE como fuente del sistema
			SystCLK_SetPres(preAHB1, preAPB1, preAPB2);
			SystCLK_SetHSEON();
		}
	}
	else if((sourceSYS>=2)&&(sourceSYS<4)){
		if((SYSCLK>=24)&&(SYSCLK<=180)){//Min out CLK 24MHz, max 180MHz, puede ser PLLP o PLLR
			if(sourcePLL==0){//Source PLL -> HSI = 16 MHz, se trabajan números enteros
				SystCLK_CalculatePLLCFGR(sourceSYS, HSICLK, 100*SYSCLK, preAHB1, preAPB1, preAPB2, 8);//Configura e inicializa
			}
			else{//Source PLL -> HSE
				SystCLK_CalculatePLLCFGR(sourceSYS, HSECLK, 100*SYSCLK, preAHB1, preAPB1, preAPB2, 1);//Configura e inicializa
			}
		}
		else{//Configuración predeterminada en PLL
			SystCLK_SetPLLPredet();//SYS = 40MHz, APB1 = 10MHz
		}
	}
	else{//Se selecciona configuración predeterminada
		SystCLK_SetPLLPredet();//SYS = 40MHz, APB1 = 10MHz, HSI ON
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
void SystCLK_SetPres(uint8_t preAHB1, uint8_t preAPB1, uint8_t preAPB2){
	uint8_t divAHB1 = currentSYSCLK / (Calculate_Pot2(preAHB1));
	uint8_t divAPB1 = currentSYSCLK / (Calculate_Pot2(preAHB1+preAPB1));
	uint8_t divAPB2 = currentSYSCLK / (Calculate_Pot2(preAHB1+preAPB2));;

	if(divAHB1>=2){
		if(currentSYSCLK&1){//Si es un número impar
			//Dejamos valores enteros
			RCC_CFGR |= (RCC_CFGR_HPRE_DIV1|RCC_CFGR_PPRE1_DIV1|RCC_CFGR_PPRE2_DIV1); //Dividir por 1 todo
		}
		else{//número par
			if((divAPB1>=2)&&(divAPB2>=2)){//Verificamos el rango del preescalador, si permite un valor mayor a 2MHz
				RCC_CFGR |= (((7+preAHB1)<<RCC_CFGR_HPRE_Pos)|((3+preAPB1)<<RCC_CFGR_PPRE1_Pos)|((3+preAPB2)<<RCC_CFGR_PPRE2_Pos));
			}
			else if((divAPB1>=2)&&(divAPB2<=2)){
				RCC_CFGR |= (((7+preAHB1)<<RCC_CFGR_HPRE_Pos)|((3+preAPB1)<<RCC_CFGR_PPRE1_Pos)|(RCC_CFGR_PPRE2_DIV1));
			}
			else if((divAPB1<=2)&&(divAPB2>=2)){
				RCC_CFGR |= (((7+preAHB1)<<RCC_CFGR_HPRE_Pos)|(RCC_CFGR_PPRE1_DIV1)|((3+preAPB2)<<RCC_CFGR_PPRE2_Pos));
			}
			else{
				RCC_CFGR |= (((7+preAHB1)<<RCC_CFGR_HPRE_Pos)|RCC_CFGR_PPRE1_DIV1|RCC_CFGR_PPRE2_DIV1); //Dividir por 1 preescaladores APB1 y APB2
			}
		}
	}
	else{
		RCC_CFGR |= (RCC_CFGR_HPRE_DIV1|RCC_CFGR_PPRE1_DIV1|RCC_CFGR_PPRE2_DIV1); //Dividir por 1 todo
	}
}

void SystCLK_SetHSION(){
	RCC_CR = ((0x10<<RCC_CR_HSITRIM_Pos)|(0x68<<RCC_CR_HSICAL_Pos));//Configuración Para calibración
	RCC_CR |= (RCC_CR_HSION|(0x68<<RCC_CR_HSICAL_Pos));
	while((RCC_CR&&RCC_CR_HSIRDY)==0);
}

void SystCLK_SetHSEON(){
	RCC_CR = (RCC_CR_HSEON|RCC_CR_HSEBYP|(0x68<<RCC_CR_HSICAL_Pos));
	while((RCC_CR&&RCC_CR_HSERDY)==0);
}

void SystCLK_SetPLLON(uint8_t sourcePLL){
	if(sourcePLL==1){
		RCC_CR = (RCC_CR_PLLON|RCC_CR_HSEON|RCC_CR_HSEBYP|(0x68<<RCC_CR_HSICAL_Pos));//|RCC_CR_HSEON|RCC_CR_HSEBYP);//PLL ON, HSE ON, HSE-> Osc. PLL
	}
	else{
		RCC_CR = ((0x10<<RCC_CR_HSITRIM_Pos)|(0x68<<RCC_CR_HSICAL_Pos));//Configuración Para calibración
		RCC_CR |= (RCC_CR_PLLON|RCC_CR_HSION|(0x68<<RCC_CR_HSICAL_Pos));//|RCC_CR_HSEON|RCC_CR_HSEBYP);//PLL ON, HSI ON, HSI-> Osc. PLL
	}
	while((RCC_CR&&RCC_CR_PLLRDY)==0);

}
//
void SystCLK_CalculatePLLCFGR(uint8_t sourceSYS, uint8_t PLLCLK, uint16_t SYSCLK,uint8_t preAHB1, uint8_t preAPB1, uint8_t preAPB2, uint8_t up){
	uint32_t auxClk = 0 ;
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
		while((PLLN_<=432)&&(auxClk!=SYSCLK)){
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
		SystCLK_SetPLLPredet(); //SYS = 40MHz, APB1 = 10MHz
		currentSYSCLK = 40;
	}
	else{//Se llenan los valores
		currentSYSCLK = auxClk/100;
		if(sourceSYS==2){//PLLP como fuente del sistema
			PLL_P_R_= ( (PLL_P_R_/2) - 1 );//
			RCC_CFGR = RCC_CFGR_SW_PLL;
			SystCLK_SetPres(preAHB1, preAPB1, preAPB2);
			RCC_PLLCFGR = ((PLLM_<<RCC_PLLCFGR_PLLM_Pos)|(PLLN_<<RCC_PLLCFGR_PLLN_Pos)|(PLL_P_R_<<RCC_PLLCFGR_PLLP_Pos)|RCC_PLLCFGR_PLLQ_1|RCC_PLLCFGR_PLLR_1);//Configurar antes de activar
		}
		else{//PLLR como fuente del sistema
			RCC_CFGR = RCC_CFGR_SW_PLLR;
			SystCLK_SetPres(preAHB1, preAPB1, preAPB2);
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
	RCC_CFGR = (RCC_CFGR_SW_PLL|RCC_CFGR_HPRE_DIV1|RCC_CFGR_PPRE1_DIV4|RCC_CFGR_PPRE2_DIV1) ;//SW = PLLP Source, AHB=1, APB1=4, APB2=1, APB1 Source 10MHz para I2C
	RCC_PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSI|RCC_PLLCFGR_PLLM_4|(160<<RCC_PLLCFGR_PLLN_Pos)|(1<<RCC_PLLCFGR_PLLP_Pos)|RCC_PLLCFGR_PLLQ_1|RCC_PLLCFGR_PLLR_1);//PLL source = HSI, PLLM = 16, PLLN = 160, PLLP = 4. SYS = 40 MHz; configurar antes de activar
	SystCLK_SetPLLON(0);
}



