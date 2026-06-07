#include "spi.h"

/*
 * Initialize SPI peripheral
 * Master mode, 4 MHz clock, MSB first, CPOL=0, CPHA=0
 */
void SPI_Init(void)
{
    /* SPI_GPIO_Init should be called first */
    
    /* SPI Control Register 1 */
    /* CPOL=0, CPHA=0, MSTR=1, SPE=1, BR=2 (4MHz) */
    SPI_CR1 = 0x04;  /* BR=0 (divide by 2), MSTR=1 */
    
    /* Adjust baud rate: BR field in CR1 bits 5-3 */
    /* BR=2 means divide by 8 (1:000) -> fSPI = 16/8 = 2 MHz */
    /* BR=3 means divide by 16 (0:001) -> fSPI = 16/16 = 1 MHz */
    /* BR=1 means divide by 4 (0:100) -> fSPI = 16/4 = 4 MHz */
    SPI_CR1 &= 0xC7;  /* Clear BR bits */
    SPI_CR1 |= 0x08;  /* Set BR to 1 (divide by 4) = 4 MHz */
    
    /* SPI Control Register 2 */
    SPI_CR2 = 0x00;  /* SSM=0 (hardware NSS), MSTR mode */
    
    /* Enable SPI */
    SPI_CR1 |= 0x40;  /* SPE=1 */
}

/*
 * Send single byte via SPI and receive response
 */
unsigned char SPI_SendByte(unsigned char byte)
{
    /* Wait for transmit buffer empty */
    while (!(SPI_SR & 0x02))  /* TXE flag */
        ;
    
    /* Send byte */
    SPI_DR = byte;
    
    /* Wait for receive buffer full */
    while (!(SPI_SR & 0x01))  /* RXNE flag */
        ;
    
    /* Read received byte */
    return SPI_DR;
}

/*
 * Send buffer via SPI
 */
void SPI_SendBuffer(unsigned char *buffer, unsigned char length)
{
    unsigned char i;
    
    for (i = 0; i < length; i++)
    {
        SPI_SendByte(buffer[i]);
    }
}

/*
 * Receive buffer via SPI (dummy bytes sent)
 */
void SPI_ReceiveBuffer(unsigned char *buffer, unsigned char length)
{
    unsigned char i;
    
    for (i = 0; i < length; i++)
    {
        buffer[i] = SPI_SendByte(0xFF);  /* Send dummy byte, receive data */
    }
}