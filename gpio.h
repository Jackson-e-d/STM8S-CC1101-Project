#ifndef GPIO_H
#define GPIO_H

#include "stm8s.h"
#include "config.h"

/* ============================================
   GPIO Initialization
   ============================================ */

void GPIO_Init(void);

/* ============================================
   LED Control Functions
   ============================================ */

/* LED ON (active LOW) */
void LED_On(void);

/* LED OFF (active LOW) */
void LED_Off(void);

/* LED Flash sequence (count times) */
void LED_Flash(unsigned char count);

/* ============================================
   SPI Pin Configuration
   ============================================ */

void SPI_GPIO_Init(void);

/* ============================================
   CC1101 Pin Control
   ============================================ */

/* CS/NSS pin HIGH */
void CC1101_CSN_High(void);

/* CS/NSS pin LOW */
void CC1101_CSN_Low(void);

/* Check GDO0 pin state */
unsigned char CC1101_GDO0_Read(void);

#endif /* GPIO_H */