#include "stm8s.h"
#include "config.h"
#include "gpio.h"
#include "spi.h"
#include "cc1101.h"

/* ============================================
   Function prototypes
   ============================================ */

void CLK_Init(void);
void UART_Init(void);
void delay_ms(unsigned int ms);

/* Device mode: 0 = Receiver, 1 = Transmitter */
#define DEVICE_MODE 1

/* ============================================
   Clock Initialization
   ============================================ */

void CLK_Init(void)
{
    /* Enable HSE (High Speed External oscillator) */
    CLK_ECKR |= 0x01;  /* HSEEN = 1 */
    
    /* Wait for HSE ready */
    while (!(CLK_ECKR & 0x02))  /* HSERDY */
        ;
    
    /* Configure PLL: fPLL = 16 MHz * (9+1) / 2 = 80 MHz */
    /* CLKDIV = 0 (fMaster = fPLL) */
    CLK_CKDIVR = 0x00;
    
    /* Switch to HSE as system clock */
    CLK_SWR = 0xB4;  /* Select HSE */
    
    /* Wait for clock switch */
    while ((CLK_SWCR & 0x08) == 0)  /* SWIF flag */
        ;
}

/* ============================================
   Millisecond Delay
   ============================================ */

void delay_ms(unsigned int ms)
{
    unsigned int i;
    unsigned int j;
    
    for (i = 0; i < ms; i++)
        for (j = 0; j < 4000; j++)  /* 4MHz / 4 iterations ≈ 1ms */
            __asm("nop");
}

/* ============================================
   Main Program
   ============================================ */

void main(void)
{
    unsigned char i, j;
    unsigned char tx_buffer[256];
    unsigned char rx_buffer[256];
    unsigned char rx_length;
    
    /* Initialize clock */
    CLK_Init();
    
    /* Initialize GPIO */
    GPIO_Init();
    SPI_GPIO_Init();
    
    /* Initialize SPI */
    SPI_Init();
    
    /* Initialize CC1101 */
    CC1101_Init();
    
    /* Small delay after initialization */
    delay_ms(100);
    
    /* LED startup indication: 3 quick flashes */
    LED_Flash(3);
    delay_ms(500);
    
#if DEVICE_MODE == 1
    /* ============================================
       TRANSMITTER MODE
       ============================================ */
    
    while (1)
    {
        /* Prepare transmit buffer with sequence FF to 00 */
        for (i = 0; i < 256; i++)
        {
            tx_buffer[i] = 0xFF - i;  /* FF, FE, FD, ..., 01, 00 */
        }
        
        /* Send packet */
        if (CC1101_SendPacket(tx_buffer, 256))
        {
            /* Transmission successful - flash LED 5 times */
            LED_Flash(5);
        }
        
        /* Wait before next transmission */
        delay_ms(1000);
    }

#else
    /* ============================================
       RECEIVER MODE
       ============================================ */
    
    while (1)
    {
        /* Try to receive packet */
        rx_length = CC1101_ReceivePacket(rx_buffer);
        
        if (rx_length > 0)
        {
            /* Packet received successfully - flash LED 5 times */
            LED_Flash(5);
            
            /* Optional: verify received sequence */
            unsigned char valid = 1;
            for (i = 0; i < rx_length; i++)
            {
                if (rx_buffer[i] != (0xFF - i))
                {
                    valid = 0;
                    break;
                }
            }
            
            if (!valid)
            {
                /* Data corruption - flash LED once as error indicator */
                LED_Flash(1);
            }
        }
        
        /* Small delay to avoid CPU hogging */
        delay_ms(10);
    }

#endif
}

/* ============================================
   Interrupt Vectors
   (Required for STM8S with Cosmic C)
   ============================================ */

#pragma section @near [vector_section]

void @far @interrupt void NonHandledInterrupt(void)
{
    /* Do nothing */
}

/* Define all interrupt vectors here if needed */
