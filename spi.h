#ifndef SPI_H
#define SPI_H

#include "stm8s.h"
#include "config.h"

/* ============================================
   SPI Initialization and Control
   ============================================ */

void SPI_Init(void);

/* ============================================
   SPI Data Transfer
   ============================================ */

/* Send single byte and receive response */
unsigned char SPI_SendByte(unsigned char byte);

/* Send buffer */
void SPI_SendBuffer(unsigned char *buffer, unsigned char length);

/* Receive buffer */
void SPI_ReceiveBuffer(unsigned char *buffer, unsigned char length);

#endif /* SPI_H */