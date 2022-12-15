/*
 * I2Cx.h
 *
 *  Created on: Sep 23, 2022
 *      Author: jurl9
 */

#ifndef I2CX_H_
#define I2CX_H_
#include <stdint.h>
#include <stdbool.h>
//
extern uint16_t addressS;
extern uint8_t modeAddressingS, bufferI2C[8], bufferI2CRx[8];
//Definición de variables
#define I2C1          1
#define I2C2          1
#define I2C3          1
//Definifición para dirección de 10 bits
#define HEADER        0B11110000//11110xx0 xx denotes MSB of address

//Estructura de Parámetros
typedef struct
{
  uint8_t  modeAddressing; //Modo de Direccionamiento para esclavo
  uint8_t  speed;          //Velocidad de transmisión SM o FM
  uint16_t own_address;    //Dirección para modo esclavo
  uint8_t  TX_RX_mode;     //Indica transmisión o recepción
  uint16_t ADDRDevice;     //Addres of slave to comunicate

} I2C_ParameterDef;
//Estructura de Registros
typedef struct
{
  volatile uint32_t CR1;        /*!< I2C Control register 1,     Address offset: 0x00 */
  volatile uint32_t CR2;        /*!< I2C Control register 2,     Address offset: 0x04 */
  volatile uint32_t OAR1;       /*!< I2C Own address register 1, Address offset: 0x08 */
  volatile uint32_t OAR2;       /*!< I2C Own address register 2, Address offset: 0x0C */
  volatile uint32_t DR;         /*!< I2C Data register,          Address offset: 0x10 */
  volatile uint32_t SR1;        /*!< I2C Status register 1,      Address offset: 0x14 */
  volatile uint32_t SR2;        /*!< I2C Status register 2,      Address offset: 0x18 */
  volatile uint32_t CCR;        /*!< I2C Clock control register, Address offset: 0x1C */
  volatile uint32_t TRISE;      /*!< I2C TRISE register,         Address offset: 0x20 */
  volatile uint32_t FLTR;       /*!< I2C FLTR register,          Address offset: 0x24 */
} I2C_TypeDef;
//Manejar I2C
typedef struct
{
	I2C_TypeDef        *Registers;     //Apunta a los Registros de I2C
	I2C_ParameterDef   Parameter;      //Parámetros de I2C
	uint8_t            *pBuffer;       //Apuntador a buffer
	uint8_t            sizeI2C;        //Tamaño de bytes a transmitir o recibir
	uint8_t            auxsizeI2C;     //Auxiliar para realizar conteo
	uint8_t            currentState;   //To count

} I2C_HandlerDef;

extern I2C_HandlerDef I2C1_Struct, I2C2_Struct, I2C3_Struct;

//Definición de Registros

//I2Cx APB1
#define I2C1_BASE   0x40005400UL
#define I2C2_BASE   0x40005800UL
#define I2C3_BASE   0x40005C00UL
//
#define I2C1_R                ((I2C_TypeDef *) I2C1_BASE)
#define I2C2_R                ((I2C_TypeDef *) I2C2_BASE)
#define I2C3_R                ((I2C_TypeDef *) I2C3_BASE)

/*
 *Definición de bits en registros
 */
/******************************************************************************/
/*                                                                            */
/*                      Inter-integrated Circuit Interface                    */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for I2C_CR1 register  ********************/
#define I2C_CR1_PE_Pos            (0U)
#define I2C_CR1_PE_Msk            (0x1UL << I2C_CR1_PE_Pos)                     /*!< 0x00000001 */
#define I2C_CR1_PE                I2C_CR1_PE_Msk                               /*!<Peripheral Enable                             */
#define I2C_CR1_SMBUS_Pos         (1U)
#define I2C_CR1_SMBUS_Msk         (0x1UL << I2C_CR1_SMBUS_Pos)                  /*!< 0x00000002 */
#define I2C_CR1_SMBUS             I2C_CR1_SMBUS_Msk                            /*!<SMBus Mode                                    */
#define I2C_CR1_SMBTYPE_Pos       (3U)
#define I2C_CR1_SMBTYPE_Msk       (0x1UL << I2C_CR1_SMBTYPE_Pos)                /*!< 0x00000008 */
#define I2C_CR1_SMBTYPE           I2C_CR1_SMBTYPE_Msk                          /*!<SMBus Type                                    */
#define I2C_CR1_ENARP_Pos         (4U)
#define I2C_CR1_ENARP_Msk         (0x1UL << I2C_CR1_ENARP_Pos)                  /*!< 0x00000010 */
#define I2C_CR1_ENARP             I2C_CR1_ENARP_Msk                            /*!<ARP Enable                                    */
#define I2C_CR1_ENPEC_Pos         (5U)
#define I2C_CR1_ENPEC_Msk         (0x1UL << I2C_CR1_ENPEC_Pos)                  /*!< 0x00000020 */
#define I2C_CR1_ENPEC             I2C_CR1_ENPEC_Msk                            /*!<PEC Enable                                    */
#define I2C_CR1_ENGC_Pos          (6U)
#define I2C_CR1_ENGC_Msk          (0x1UL << I2C_CR1_ENGC_Pos)                   /*!< 0x00000040 */
#define I2C_CR1_ENGC              I2C_CR1_ENGC_Msk                             /*!<General Call Enable                           */
#define I2C_CR1_NOSTRETCH_Pos     (7U)
#define I2C_CR1_NOSTRETCH_Msk     (0x1UL << I2C_CR1_NOSTRETCH_Pos)              /*!< 0x00000080 */
#define I2C_CR1_NOSTRETCH         I2C_CR1_NOSTRETCH_Msk                        /*!<Clock Stretching Disable (Slave mode)         */
#define I2C_CR1_START_Pos         (8U)
#define I2C_CR1_START_Msk         (0x1UL << I2C_CR1_START_Pos)                  /*!< 0x00000100 */
#define I2C_CR1_START             I2C_CR1_START_Msk                            /*!<Start Generation                              */
#define I2C_CR1_STOP_Pos          (9U)
#define I2C_CR1_STOP_Msk          (0x1UL << I2C_CR1_STOP_Pos)                   /*!< 0x00000200 */
#define I2C_CR1_STOP              I2C_CR1_STOP_Msk                             /*!<Stop Generation                               */
#define I2C_CR1_ACK_Pos           (10U)
#define I2C_CR1_ACK_Msk           (0x1UL << I2C_CR1_ACK_Pos)                    /*!< 0x00000400 */
#define I2C_CR1_ACK               I2C_CR1_ACK_Msk                              /*!<Acknowledge Enable                            */
#define I2C_CR1_POS_Pos           (11U)
#define I2C_CR1_POS_Msk           (0x1UL << I2C_CR1_POS_Pos)                    /*!< 0x00000800 */
#define I2C_CR1_POS               I2C_CR1_POS_Msk                              /*!<Acknowledge/PEC Position (for data reception) */
#define I2C_CR1_PEC_Pos           (12U)
#define I2C_CR1_PEC_Msk           (0x1UL << I2C_CR1_PEC_Pos)                    /*!< 0x00001000 */
#define I2C_CR1_PEC               I2C_CR1_PEC_Msk                              /*!<Packet Error Checking                         */
#define I2C_CR1_ALERT_Pos         (13U)
#define I2C_CR1_ALERT_Msk         (0x1UL << I2C_CR1_ALERT_Pos)                  /*!< 0x00002000 */
#define I2C_CR1_ALERT             I2C_CR1_ALERT_Msk                            /*!<SMBus Alert                                   */
#define I2C_CR1_SWRST_Pos         (15U)
#define I2C_CR1_SWRST_Msk         (0x1UL << I2C_CR1_SWRST_Pos)                  /*!< 0x00008000 */
#define I2C_CR1_SWRST             I2C_CR1_SWRST_Msk                            /*!<Software Reset                                */

/*******************  Bit definition for I2C_CR2 register  ********************/
#define I2C_CR2_FREQ_Pos          (0U)
#define I2C_CR2_FREQ_Msk          (0x3FUL << I2C_CR2_FREQ_Pos)                  /*!< 0x0000003F */
#define I2C_CR2_FREQ              I2C_CR2_FREQ_Msk                             /*!<FREQ[5:0] bits (Peripheral Clock Frequency)   */
#define I2C_CR2_FREQ_0            (0x01UL << I2C_CR2_FREQ_Pos)                  /*!< 0x00000001 */
#define I2C_CR2_FREQ_1            (0x02UL << I2C_CR2_FREQ_Pos)                  /*!< 0x00000002 */
#define I2C_CR2_FREQ_2            (0x04UL << I2C_CR2_FREQ_Pos)                  /*!< 0x00000004 */
#define I2C_CR2_FREQ_3            (0x08UL << I2C_CR2_FREQ_Pos)                  /*!< 0x00000008 */
#define I2C_CR2_FREQ_4            (0x10UL << I2C_CR2_FREQ_Pos)                  /*!< 0x00000010 */
#define I2C_CR2_FREQ_5            (0x20UL << I2C_CR2_FREQ_Pos)                  /*!< 0x00000020 */

#define I2C_CR2_ITERREN_Pos       (8U)
#define I2C_CR2_ITERREN_Msk       (0x1UL << I2C_CR2_ITERREN_Pos)                /*!< 0x00000100 */
#define I2C_CR2_ITERREN           I2C_CR2_ITERREN_Msk                          /*!<Error Interrupt Enable  */
#define I2C_CR2_ITEVTEN_Pos       (9U)
#define I2C_CR2_ITEVTEN_Msk       (0x1UL << I2C_CR2_ITEVTEN_Pos)                /*!< 0x00000200 */
#define I2C_CR2_ITEVTEN           I2C_CR2_ITEVTEN_Msk                          /*!<Event Interrupt Enable  */
#define I2C_CR2_ITBUFEN_Pos       (10U)
#define I2C_CR2_ITBUFEN_Msk       (0x1UL << I2C_CR2_ITBUFEN_Pos)                /*!< 0x00000400 */
#define I2C_CR2_ITBUFEN           I2C_CR2_ITBUFEN_Msk                          /*!<Buffer Interrupt Enable */
#define I2C_CR2_DMAEN_Pos         (11U)
#define I2C_CR2_DMAEN_Msk         (0x1UL << I2C_CR2_DMAEN_Pos)                  /*!< 0x00000800 */
#define I2C_CR2_DMAEN             I2C_CR2_DMAEN_Msk                            /*!<DMA Requests Enable     */
#define I2C_CR2_LAST_Pos          (12U)
#define I2C_CR2_LAST_Msk          (0x1UL << I2C_CR2_LAST_Pos)                   /*!< 0x00001000 */
#define I2C_CR2_LAST              I2C_CR2_LAST_Msk                             /*!<DMA Last Transfer       */

/*******************  Bit definition for I2C_OAR1 register  *******************/
#define I2C_OAR1_ADD1_7           0x000000FEU                                  /*!<Interface Address */
#define I2C_OAR1_ADD8_9           0x00000300U                                  /*!<Interface Address */

#define I2C_OAR1_ADD0_Pos         (0U)
#define I2C_OAR1_ADD0_Msk         (0x1UL << I2C_OAR1_ADD0_Pos)                  /*!< 0x00000001 */
#define I2C_OAR1_ADD0             I2C_OAR1_ADD0_Msk                            /*!<Bit 0 */
#define I2C_OAR1_ADD1_Pos         (1U)
#define I2C_OAR1_ADD1_Msk         (0x1UL << I2C_OAR1_ADD1_Pos)                  /*!< 0x00000002 */
#define I2C_OAR1_ADD1             I2C_OAR1_ADD1_Msk                            /*!<Bit 1 */
#define I2C_OAR1_ADD2_Pos         (2U)
#define I2C_OAR1_ADD2_Msk         (0x1UL << I2C_OAR1_ADD2_Pos)                  /*!< 0x00000004 */
#define I2C_OAR1_ADD2             I2C_OAR1_ADD2_Msk                            /*!<Bit 2 */
#define I2C_OAR1_ADD3_Pos         (3U)
#define I2C_OAR1_ADD3_Msk         (0x1UL << I2C_OAR1_ADD3_Pos)                  /*!< 0x00000008 */
#define I2C_OAR1_ADD3             I2C_OAR1_ADD3_Msk                            /*!<Bit 3 */
#define I2C_OAR1_ADD4_Pos         (4U)
#define I2C_OAR1_ADD4_Msk         (0x1UL << I2C_OAR1_ADD4_Pos)                  /*!< 0x00000010 */
#define I2C_OAR1_ADD4             I2C_OAR1_ADD4_Msk                            /*!<Bit 4 */
#define I2C_OAR1_ADD5_Pos         (5U)
#define I2C_OAR1_ADD5_Msk         (0x1UL << I2C_OAR1_ADD5_Pos)                  /*!< 0x00000020 */
#define I2C_OAR1_ADD5             I2C_OAR1_ADD5_Msk                            /*!<Bit 5 */
#define I2C_OAR1_ADD6_Pos         (6U)
#define I2C_OAR1_ADD6_Msk         (0x1UL << I2C_OAR1_ADD6_Pos)                  /*!< 0x00000040 */
#define I2C_OAR1_ADD6             I2C_OAR1_ADD6_Msk                            /*!<Bit 6 */
#define I2C_OAR1_ADD7_Pos         (7U)
#define I2C_OAR1_ADD7_Msk         (0x1UL << I2C_OAR1_ADD7_Pos)                  /*!< 0x00000080 */
#define I2C_OAR1_ADD7             I2C_OAR1_ADD7_Msk                            /*!<Bit 7 */
#define I2C_OAR1_ADD8_Pos         (8U)
#define I2C_OAR1_ADD8_Msk         (0x1UL << I2C_OAR1_ADD8_Pos)                  /*!< 0x00000100 */
#define I2C_OAR1_ADD8             I2C_OAR1_ADD8_Msk                            /*!<Bit 8 */
#define I2C_OAR1_ADD9_Pos         (9U)
#define I2C_OAR1_ADD9_Msk         (0x1UL << I2C_OAR1_ADD9_Pos)                  /*!< 0x00000200 */
#define I2C_OAR1_ADD9             I2C_OAR1_ADD9_Msk                            /*!<Bit 9 */

#define I2C_OAR1_ADDMODE_Pos      (15U)
#define I2C_OAR1_ADDMODE_Msk      (0x1UL << I2C_OAR1_ADDMODE_Pos)               /*!< 0x00008000 */
#define I2C_OAR1_ADDMODE          I2C_OAR1_ADDMODE_Msk                         /*!<Addressing Mode (Slave mode) */

/*******************  Bit definition for I2C_OAR2 register  *******************/
#define I2C_OAR2_ENDUAL_Pos       (0U)
#define I2C_OAR2_ENDUAL_Msk       (0x1UL << I2C_OAR2_ENDUAL_Pos)                /*!< 0x00000001 */
#define I2C_OAR2_ENDUAL           I2C_OAR2_ENDUAL_Msk                          /*!<Dual addressing mode enable */
#define I2C_OAR2_ADD2_Pos         (1U)
#define I2C_OAR2_ADD2_Msk         (0x7FUL << I2C_OAR2_ADD2_Pos)                 /*!< 0x000000FE */
#define I2C_OAR2_ADD2             I2C_OAR2_ADD2_Msk                            /*!<Interface address           */

/********************  Bit definition for I2C_DR register  ********************/
#define I2C_DR_DR_Pos             (0U)
#define I2C_DR_DR_Msk             (0xFFUL << I2C_DR_DR_Pos)                     /*!< 0x000000FF */
#define I2C_DR_DR                 I2C_DR_DR_Msk                                /*!<8-bit Data Register         */

/*******************  Bit definition for I2C_SR1 register  ********************/
#define I2C_SR1_SB_Pos            (0U)
#define I2C_SR1_SB_Msk            (0x1UL << I2C_SR1_SB_Pos)                     /*!< 0x00000001 */
#define I2C_SR1_SB                I2C_SR1_SB_Msk                               /*!<Start Bit (Master mode)                         */
#define I2C_SR1_ADDR_Pos          (1U)
#define I2C_SR1_ADDR_Msk          (0x1UL << I2C_SR1_ADDR_Pos)                   /*!< 0x00000002 */
#define I2C_SR1_ADDR              I2C_SR1_ADDR_Msk                             /*!<Address sent (master mode)/matched (slave mode) */
#define I2C_SR1_BTF_Pos           (2U)
#define I2C_SR1_BTF_Msk           (0x1UL << I2C_SR1_BTF_Pos)                    /*!< 0x00000004 */
#define I2C_SR1_BTF               I2C_SR1_BTF_Msk                              /*!<Byte Transfer Finished                          */
#define I2C_SR1_ADD10_Pos         (3U)
#define I2C_SR1_ADD10_Msk         (0x1UL << I2C_SR1_ADD10_Pos)                  /*!< 0x00000008 */
#define I2C_SR1_ADD10             I2C_SR1_ADD10_Msk                            /*!<10-bit header sent (Master mode)                */
#define I2C_SR1_STOPF_Pos         (4U)
#define I2C_SR1_STOPF_Msk         (0x1UL << I2C_SR1_STOPF_Pos)                  /*!< 0x00000010 */
#define I2C_SR1_STOPF             I2C_SR1_STOPF_Msk                            /*!<Stop detection (Slave mode)                     */
#define I2C_SR1_RXNE_Pos          (6U)
#define I2C_SR1_RXNE_Msk          (0x1UL << I2C_SR1_RXNE_Pos)                   /*!< 0x00000040 */
#define I2C_SR1_RXNE              I2C_SR1_RXNE_Msk                             /*!<Data Register not Empty (receivers)             */
#define I2C_SR1_TXE_Pos           (7U)
#define I2C_SR1_TXE_Msk           (0x1UL << I2C_SR1_TXE_Pos)                    /*!< 0x00000080 */
#define I2C_SR1_TXE               I2C_SR1_TXE_Msk                              /*!<Data Register Empty (transmitters)              */
#define I2C_SR1_BERR_Pos          (8U)
#define I2C_SR1_BERR_Msk          (0x1UL << I2C_SR1_BERR_Pos)                   /*!< 0x00000100 */
#define I2C_SR1_BERR              I2C_SR1_BERR_Msk                             /*!<Bus Error                                       */
#define I2C_SR1_ARLO_Pos          (9U)
#define I2C_SR1_ARLO_Msk          (0x1UL << I2C_SR1_ARLO_Pos)                   /*!< 0x00000200 */
#define I2C_SR1_ARLO              I2C_SR1_ARLO_Msk                             /*!<Arbitration Lost (master mode)                  */
#define I2C_SR1_AF_Pos            (10U)
#define I2C_SR1_AF_Msk            (0x1UL << I2C_SR1_AF_Pos)                     /*!< 0x00000400 */
#define I2C_SR1_AF                I2C_SR1_AF_Msk                               /*!<Acknowledge Failure                             */
#define I2C_SR1_OVR_Pos           (11U)
#define I2C_SR1_OVR_Msk           (0x1UL << I2C_SR1_OVR_Pos)                    /*!< 0x00000800 */
#define I2C_SR1_OVR               I2C_SR1_OVR_Msk                              /*!<Overrun/Underrun                                */
#define I2C_SR1_PECERR_Pos        (12U)
#define I2C_SR1_PECERR_Msk        (0x1UL << I2C_SR1_PECERR_Pos)                 /*!< 0x00001000 */
#define I2C_SR1_PECERR            I2C_SR1_PECERR_Msk                           /*!<PEC Error in reception                          */
#define I2C_SR1_TIMEOUT_Pos       (14U)
#define I2C_SR1_TIMEOUT_Msk       (0x1UL << I2C_SR1_TIMEOUT_Pos)                /*!< 0x00004000 */
#define I2C_SR1_TIMEOUT           I2C_SR1_TIMEOUT_Msk                          /*!<Timeout or Tlow Error                           */
#define I2C_SR1_SMBALERT_Pos      (15U)
#define I2C_SR1_SMBALERT_Msk      (0x1UL << I2C_SR1_SMBALERT_Pos)               /*!< 0x00008000 */
#define I2C_SR1_SMBALERT          I2C_SR1_SMBALERT_Msk                         /*!<SMBus Alert                                     */

/*******************  Bit definition for I2C_SR2 register  ********************/
#define I2C_SR2_MSL_Pos           (0U)
#define I2C_SR2_MSL_Msk           (0x1UL << I2C_SR2_MSL_Pos)                    /*!< 0x00000001 */
#define I2C_SR2_MSL               I2C_SR2_MSL_Msk                              /*!<Master/Slave                                    */
#define I2C_SR2_BUSY_Pos          (1U)
#define I2C_SR2_BUSY_Msk          (0x1UL << I2C_SR2_BUSY_Pos)                   /*!< 0x00000002 */
#define I2C_SR2_BUSY              I2C_SR2_BUSY_Msk                             /*!<Bus Busy                                        */
#define I2C_SR2_TRA_Pos           (2U)
#define I2C_SR2_TRA_Msk           (0x1UL << I2C_SR2_TRA_Pos)                    /*!< 0x00000004 */
#define I2C_SR2_TRA               I2C_SR2_TRA_Msk                              /*!<Transmitter/Receiver                            */
#define I2C_SR2_GENCALL_Pos       (4U)
#define I2C_SR2_GENCALL_Msk       (0x1UL << I2C_SR2_GENCALL_Pos)                /*!< 0x00000010 */
#define I2C_SR2_GENCALL           I2C_SR2_GENCALL_Msk                          /*!<General Call Address (Slave mode)               */
#define I2C_SR2_SMBDEFAULT_Pos    (5U)
#define I2C_SR2_SMBDEFAULT_Msk    (0x1UL << I2C_SR2_SMBDEFAULT_Pos)             /*!< 0x00000020 */
#define I2C_SR2_SMBDEFAULT        I2C_SR2_SMBDEFAULT_Msk                       /*!<SMBus Device Default Address (Slave mode)       */
#define I2C_SR2_SMBHOST_Pos       (6U)
#define I2C_SR2_SMBHOST_Msk       (0x1UL << I2C_SR2_SMBHOST_Pos)                /*!< 0x00000040 */
#define I2C_SR2_SMBHOST           I2C_SR2_SMBHOST_Msk                          /*!<SMBus Host Header (Slave mode)                  */
#define I2C_SR2_DUALF_Pos         (7U)
#define I2C_SR2_DUALF_Msk         (0x1UL << I2C_SR2_DUALF_Pos)                  /*!< 0x00000080 */
#define I2C_SR2_DUALF             I2C_SR2_DUALF_Msk                            /*!<Dual Flag (Slave mode)                          */
#define I2C_SR2_PEC_Pos           (8U)
#define I2C_SR2_PEC_Msk           (0xFFUL << I2C_SR2_PEC_Pos)                   /*!< 0x0000FF00 */
#define I2C_SR2_PEC               I2C_SR2_PEC_Msk                              /*!<Packet Error Checking Register                  */

/*******************  Bit definition for I2C_CCR register  ********************/
#define I2C_CCR_CCR_Pos           (0U)
#define I2C_CCR_CCR_Msk           (0xFFFUL << I2C_CCR_CCR_Pos)                  /*!< 0x00000FFF */
#define I2C_CCR_CCR               I2C_CCR_CCR_Msk                              /*!<Clock Control Register in Fast/Standard mode (Master mode) */
#define I2C_CCR_DUTY_Pos          (14U)
#define I2C_CCR_DUTY_Msk          (0x1UL << I2C_CCR_DUTY_Pos)                   /*!< 0x00004000 */
#define I2C_CCR_DUTY              I2C_CCR_DUTY_Msk                             /*!<Fast Mode Duty Cycle                                       */
#define I2C_CCR_FS_Pos            (15U)
#define I2C_CCR_FS_Msk            (0x1UL << I2C_CCR_FS_Pos)                     /*!< 0x00008000 */
#define I2C_CCR_FS                I2C_CCR_FS_Msk                               /*!<I2C Master Mode Selection                                  */

/******************  Bit definition for I2C_TRISE register  *******************/
#define I2C_TRISE_TRISE_Pos       (0U)
#define I2C_TRISE_TRISE_Msk       (0x3FUL << I2C_TRISE_TRISE_Pos)               /*!< 0x0000003F */
#define I2C_TRISE_TRISE           I2C_TRISE_TRISE_Msk                          /*!<Maximum Rise Time in Fast/Standard mode (Master mode) */

/******************  Bit definition for I2C_FLTR register  *******************/
#define I2C_FLTR_DNF_Pos          (0U)
#define I2C_FLTR_DNF_Msk          (0xFUL << I2C_FLTR_DNF_Pos)                   /*!< 0x0000000F */
#define I2C_FLTR_DNF              I2C_FLTR_DNF_Msk                             /*!<Digital Noise Filter */
#define I2C_FLTR_ANOFF_Pos        (4U)
#define I2C_FLTR_ANOFF_Msk        (0x1UL << I2C_FLTR_ANOFF_Pos)                 /*!< 0x00000010 */
#define I2C_FLTR_ANOFF            I2C_FLTR_ANOFF_Msk                           /*!<Analog Noise Filter OFF */

//Prototipos
bool I2Cx_GPIO_Init(bool PUR, uint8_t x);
bool I2Cx_Init(uint8_t APB1_freq, uint8_t speed,  I2C_HandlerDef * i2c);
void I2Cx_ADDRSet(uint16_t address, uint8_t modeAddressing, uint8_t APB1_freq, I2C_HandlerDef * i2c);
void I2Cx_ResetSlave(I2C_HandlerDef * i2c);
bool I2Cx_MasterTx(uint16_t address, uint8_t * buffer, uint8_t size, I2C_HandlerDef * i2c);
bool I2Cx_MasterRx(uint16_t address, uint8_t * buffer, uint8_t size, I2C_HandlerDef * i2c);
bool I2Cx_SlaveTx(uint8_t * buffer, uint8_t size, I2C_HandlerDef * i2c);
bool I2Cx_SlaveRx(uint8_t * buffer, uint8_t size, I2C_HandlerDef * i2c);
void I2Cx_MasterTx_IT(uint16_t address, uint8_t size, uint8_t *buffer, I2C_HandlerDef * i2c);
void I2Cx_MasterRx_IT(uint16_t address, uint8_t size, uint8_t *buffer, I2C_HandlerDef * i2c);
void I2Cx_SlaveTx_IT(uint8_t size, uint8_t *buffer, I2C_HandlerDef * i2c);
void I2Cx_SlaveRx_IT(uint8_t size, uint8_t *buffer, I2C_HandlerDef * i2c);
void I2C1_EV_IRQHandler();
void I2C1_ER_IRQHandler();
void I2C2_EV_IRQHandler();
void I2C2_ER_IRQHandler();
void I2C3_EV_IRQHandler();
void I2C3_ER_IRQHandler();
void I2Cx_EV_CallBack(uint8_t x, I2C_HandlerDef * i2c);
void I2Cx_ER_CallBack(uint8_t x, I2C_HandlerDef * i2c);
bool I2Cx_SOF(I2C_HandlerDef * i2c);
bool I2Cx_Stop(I2C_HandlerDef * i2c);
void I2Cx_GetCurrentError(char * error, I2C_HandlerDef * i2c);
uint8_t I2Cx_GetErrorsSR1(I2C_HandlerDef * i2c);
void I2Cx_ClearADDR(I2C_HandlerDef * i2c);
void I2Cx_ClearSTOPF(I2C_HandlerDef * i2c);
void I2Cx_WaitBTFSet(I2C_HandlerDef * i2c);
void I2Cx_WaitSBSet(I2C_HandlerDef * i2c);
void I2Cx_WaitADDRSet(I2C_HandlerDef * i2c);
void I2Cx_WaitADD10Set(I2C_HandlerDef * i2c);
void I2Cx_WaitTXESet(I2C_HandlerDef * i2c);
void I2Cx_WaitRXNESet(I2C_HandlerDef * i2c);
void I2Cx_WaitMSLSet(I2C_HandlerDef * i2c);
void I2Cx_WaitSTOPFSet(I2C_HandlerDef * i2c);
void I2Cx_WaitAFSet(I2C_HandlerDef * i2c);
void I2Cx_WaitBTFReset(I2C_HandlerDef * i2c);
void I2Cx_WaitSBReset(I2C_HandlerDef * i2c);
void I2Cx_WaitADDRReset(I2C_HandlerDef * i2c);
void I2Cx_WaitTXEReset(I2C_HandlerDef * i2c);
void I2Cx_WaitRXNEReset(I2C_HandlerDef * i2c);
void I2Cx_WaitMSLReset(I2C_HandlerDef * i2c);
void I2Cx_SetSTART(I2C_HandlerDef * i2c);
void I2Cx_SetSTOP(I2C_HandlerDef * i2c);
void I2Cx_SetPOS(I2C_HandlerDef * i2c);
void I2Cx_ResetPOS(I2C_HandlerDef * i2c);
void I2Cx_SetACK(I2C_HandlerDef * i2c);
void I2Cx_ResetACK(I2C_HandlerDef * i2c);
void I2Cx_SetICPREV(uint8_t x);
void I2Cx_SetICPRER(uint8_t x);
void I2Cx_WaitTRAReset(I2C_HandlerDef * i2c);
void I2Cx_WaitTRASet(I2C_HandlerDef * i2c);
void I2Cx_ResetSTART(I2C_HandlerDef * i2c);
void I2Cx_WaitBUSYSet(I2C_HandlerDef * i2c);
void I2Cx_WaitBUSYReset(I2C_HandlerDef * i2c);
void I2Cx_WaitSTOPFReset(I2C_HandlerDef * i2c);
uint16_t I2Cx_GetSTOPFBit(I2C_HandlerDef * i2c);
void I2Cx_ResetAF(I2C_HandlerDef * i2c);
uint16_t I2Cx_GetAFBit(I2C_HandlerDef * i2c);
void I2Cx_WaitAFReset(I2C_HandlerDef * i2c);
uint16_t I2Cx_GetARLOBit(I2C_HandlerDef * i2c);
void I2Cx_ResetSTOP(I2C_HandlerDef * i2c);
void I2Cx_SetPE(I2C_HandlerDef * i2c);
void I2Cx_SetSWRST(I2C_HandlerDef * i2c);
void I2Cx_ResetPE(I2C_HandlerDef * i2c);
void I2Cx_ResetSWRST(I2C_HandlerDef * i2c);
void I2Cx_ResetARLO(I2C_HandlerDef * i2c);
void I2Cx_ResetBERR(I2C_HandlerDef * i2c);
void I2Cx_ResetARLO(I2C_HandlerDef * i2c);
void I2Cx_ResetOVR(I2C_HandlerDef * i2c);
uint8_t I2Cx_GetFlagsSR1(I2C_HandlerDef * i2c);
uint8_t I2Cx_GetFlagsSR2(I2C_HandlerDef * i2c);
bool I2Cx_GetFlagSR1(uint8_t flag, I2C_HandlerDef * i2c);
bool I2Cx_GetFlagSR2(uint8_t flag, I2C_HandlerDef * i2c);
void I2Cx_SetFLTR(uint8_t mode, uint8_t APB1_freq, I2C_HandlerDef * i2c);
void I2Cx_SetFREQ(uint8_t APB1_freq, I2C_HandlerDef * i2c);
void RCC_EnI2Cx(I2C_HandlerDef * i2c);
void I2Cx_SetMode(uint8_t mode, I2C_HandlerDef * i2c);
void I2Cx_SetCCR(uint16_t CCR, uint8_t DUTY, I2C_HandlerDef * i2c);
void I2Cx_SetTRISE(uint8_t TRISE, I2C_HandlerDef * i2c);
void I2Cx_SetBit14OAR1(I2C_HandlerDef * i2c);
void I2Cx_EnableIT(I2C_HandlerDef * i2c);
void I2Cx_DisableIT(I2C_HandlerDef * i2c);
void I2Cx_DisableITBUFEN(I2C_HandlerDef * i2c);
void I2Cx_SetOAR1(uint16_t address, uint8_t modeAddressing, I2C_HandlerDef * i2c);
uint16_t I2Cx_GetCCR(I2C_HandlerDef * i2c);
void I2Cx_ADDR10Header(uint16_t address, uint8_t rw, I2C_HandlerDef * i2c);
void I2Cx_ADDR10LastADDR(uint16_t address, I2C_HandlerDef * i2c);
void I2Cx_ADDR7(uint8_t address, uint8_t rw, I2C_HandlerDef * i2c);
void I2Cx_WriteDR(uint8_t data, I2C_HandlerDef * i2c);
uint8_t I2Cx_ReadDR(I2C_HandlerDef * i2c);
void  I2Cx_SetADDMODE(uint8_t modeAddressing, I2C_HandlerDef * i2c);
void I2Cx_ADDR7Seq(uint8_t rw, uint16_t address, I2C_HandlerDef * i2c);
void I2Cx_ADDR10SeqTx(uint16_t address,I2C_HandlerDef * i2c);
void I2Cx_ADDR10Seq(uint16_t address,I2C_HandlerDef * i2c);
void I2Cx_ADDR10Delay();
#endif /* I2CX_H_ */
