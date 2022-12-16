/*
 * embbededFLASH.h
 *
 *  Created on: 2 dic 2022
 *      Author: jurl9
 */

#ifndef EMBEDDEDFLASH_H_
#define EMBEDDEDFLASH_H_

#include <stdint.h>

#define FLASH_BASE                        (0x40023C00UL)

#define FLASH_ACR                         (*((volatile unsigned int *) (FLASH_BASE + 0x00)))

#define FLASH_ACR_PRFTEN_MSK              (1UL)
#define FLASH_ACR_PRFTEN_POS              (8UL)
#define FLASH_ACR_PRFTEN_SET              (FLASH_ACR_PRFTEN_MSK<<FLASH_ACR_PRFTEN_POS)

#define FLASH_ACR_ICEN_MSK                (1UL)
#define FLASH_ACR_ICEN_POS                (9UL)
#define FLASH_ACR_ICEN_SET                (FLASH_ACR_ICEN_MSK<<FLASH_ACR_ICEN_POS)

#define FLASH_ACR_DCEN_MSK                (1UL)
#define FLASH_ACR_DCEN_POS                (10UL)
#define FLASH_ACR_DCEN_SET                (FLASH_ACR_DCEN_MSK<<FLASH_ACR_DCEN_POS)

void embbFLash_Conf(uint8_t latency);

#endif /* EMBEDDEDFLASH_H_ */
