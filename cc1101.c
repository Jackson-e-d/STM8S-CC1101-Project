#include "cc1101.h"

/* ============================================
   CC1101 Register Configuration Table
   for 433.5 MHz, 2.4 kHz bandwidth
   ============================================ */

static const unsigned char CC1101_CONFIG[47] =
{
    0x0B,  /* IOCFG2       GDO2 output pin config */
    0x0B,  /* IOCFG1       GDO1 output pin config */
    0x3F,  /* IOCFG0       GDO0 output pin config - RX/TX indicator */
    0x0E,  /* FIFOTHR      FIFO threshold */
    0xD3,  /* SYNC1        Sync word high byte */
    0x91,  /* SYNC0        Sync word low byte */
    0xFF,  /* PKTLEN       Packet length */
    0x04,  /* PKTCTRL1     Packet automation control */
    0x45,  /* PKTCTRL0     Packet automation control */
    0x00,  /* ADDR         Device address */
    0x00,  /* CHANNR       Channel number */
    0x0F,  /* FSCTRL1      Frequency synthesizer control */
    0x00,  /* FSCTRL0      Frequency synthesizer control */
    0x0F,  /* FREQ2        Frequency control (433.5 MHz) */
    0x6E,  /* FREQ1        Frequency control */
    0x80,  /* FREQ0        Frequency control */
    0x5A,  /* MDMCFG4      Modem configuration - bandwidth 2.4 kHz */
    0xCD,  /* MDMCFG3      Modem configuration - data rate */
    0x13,  /* MDMCFG2      Modem configuration - FSK modulation */
    0x23,  /* MDMCFG1      Modem configuration */
    0x85,  /* MDMCFG0      Modem configuration */
    0x63,  /* DEVIATN      Modem deviation setting */
    0x41,  /* MCSM2        Main Radio Control State Machine config */
    0x3F,  /* MCSM1        Main Radio Control State Machine config */
    0x18,  /* MCSM0        Main Radio Control State Machine config */
    0x16,  /* FOCCFG       Frequency Offset Compensation config */
    0x6C,  /* BSCFG        Bit Synchronization config */
    0x03,  /* AGCCTRL2     AGC control */
    0x40,  /* AGCCTRL1     AGC control */
    0x91,  /* AGCCTRL0     AGC control */
    0x02,  /* WOERD        Wake On Radio threshold */
    0x78,  /* WORCTRL      Wake On Radio control */
    0x16,  /* FREND1       Front end RX configuration */
    0x6E,  /* FREND0       Front end TX configuration */
    0x7F,  /* FSCAL3       Frequency synthesizer calibration */
    0x3F,  /* FSCAL2       Frequency synthesizer calibration */
    0x00,  /* FSCAL1       Frequency synthesizer calibration */
    0x11,  /* FSCAL0       Frequency synthesizer calibration */
    0x41,  /* RCCTRL1      RC oscillator config */
    0x00,  /* RCCTRL0      RC oscillator config */
    0x00,  /* Unused */
    0xFF,  /* TEST2 */
    0x81,  /* TEST1 */
    0x35,  /* TEST0 */
    0x00,  /* Unused */
    0x00,  /* Unused */
    0x00   /* Unused */
};

/*
 * Initialize CC1101 radio module
 */
void CC1101_Init(void)
{
    unsigned char i;
    
    /* Reset CC1101 */
    CC1101_Reset();
    
    /* Configure all registers */
    for (i = 0; i < 47; i++)
    {
        CC1101_WriteReg(i, CC1101_CONFIG[i]);
    }
    
    /* Set PA table (power output) */
    unsigned char pa_table[] = {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    CC1101_WriteBurstReg(0x3E, pa_table, 8);
    
    /* Go to IDLE state */
    CC1101_Idle();
}

/*
 * Reset CC1101
 */
void CC1101_Reset(void)
{
    CC1101_CSN_Low();
    SPI_SendByte(CC1101_SRES);
    CC1101_CSN_High();
    
    /* Wait for reset to complete */
    unsigned int delay;
    for (delay = 0; delay < 100000; delay++)
        __asm("nop");
}

/*
 * Write single register
 */
void CC1101_WriteReg(unsigned char addr, unsigned char value)
{
    CC1101_CSN_Low();
    SPI_SendByte(addr);
    SPI_SendByte(value);
    CC1101_CSN_High();
}

/*
 * Read single register
 */
unsigned char CC1101_ReadReg(unsigned char addr)
{
    unsigned char result;
    
    CC1101_CSN_Low();
    SPI_SendByte(addr | CC1101_READ_SINGLE);
    result = SPI_SendByte(0xFF);
    CC1101_CSN_High();
    
    return result;
}

/*
 * Write burst registers (multiple)
 */
void CC1101_WriteBurstReg(unsigned char addr, unsigned char *buffer, unsigned char length)
{
    unsigned char i;
    
    CC1101_CSN_Low();
    SPI_SendByte(addr | CC1101_WRITE_BURST);
    for (i = 0; i < length; i++)
    {
        SPI_SendByte(buffer[i]);
    }
    CC1101_CSN_High();
}

/*
 * Read burst registers (multiple)
 */
void CC1101_ReadBurstReg(unsigned char addr, unsigned char *buffer, unsigned char length)
{
    unsigned char i;
    
    CC1101_CSN_Low();
    SPI_SendByte(addr | CC1101_READ_BURST);
    for (i = 0; i < length; i++)
    {
        buffer[i] = SPI_SendByte(0xFF);
    }
    CC1101_CSN_High();
}

/*
 * Send strobe command
 */
void CC1101_SendStrobe(unsigned char strobe)
{
    CC1101_CSN_Low();
    SPI_SendByte(strobe);
    CC1101_CSN_High();
}

/*
 * Get current state
 */
unsigned char CC1101_GetState(void)
{
    unsigned char status;
    
    CC1101_CSN_Low();
    status = SPI_SendByte(CC1101_SNOP);
    CC1101_CSN_High();
    
    return (status >> 4) & 0x07;
}

/*
 * Set TX mode
 */
void CC1101_SetTxMode(void)
{
    CC1101_SendStrobe(CC1101_STX);
}

/*
 * Set RX mode
 */
void CC1101_SetRxMode(void)
{
    CC1101_SendStrobe(CC1101_SRX);
}

/*
 * Go to IDLE mode
 */
void CC1101_Idle(void)
{
    CC1101_SendStrobe(CC1101_SIDLE);
    
    /* Wait for state change */
    unsigned int delay;
    for (delay = 0; delay < 100000; delay++)
        __asm("nop");
}

/*
 * Send packet
 */
unsigned char CC1101_SendPacket(unsigned char *data, unsigned char length)
{
    unsigned char i;
    unsigned int timeout;
    
    if (length > CC1101_FIFO_SIZE - 1)
        return 0;  /* Packet too large */
    
    /* Go to IDLE first */
    CC1101_Idle();
    
    /* Flush TX FIFO */
    CC1101_SendStrobe(CC1101_SFTX);
    
    /* Write length byte to TX FIFO */
    CC1101_WriteReg(CC1101_TXFIFO, length);
    
    /* Write data to TX FIFO */
    for (i = 0; i < length; i++)
    {
        CC1101_WriteReg(CC1101_TXFIFO, data[i]);
    }
    
    /* Enable TX */
    CC1101_SetTxMode();
    
    /* Wait for TX to complete (GDO0 goes high during TX) */
    timeout = 0;
    while (CC1101_GDO0_Read() == 0 && timeout < 100000)
    {
        timeout++;
    }
    
    /* Wait for TX end (GDO0 goes low) */
    timeout = 0;
    while (CC1101_GDO0_Read() != 0 && timeout < 200000)
    {
        timeout++;
    }
    
    /* Return to IDLE */
    CC1101_Idle();
    
    return 1;  /* Success */
}

/*
 * Receive packet
 */
unsigned char CC1101_ReceivePacket(unsigned char *data)
{
    unsigned char length;
    unsigned char i;
    unsigned int timeout;
    
    /* Enable RX */
    CC1101_SetRxMode();
    
    /* Wait for RX packet (GDO0 goes high when packet received) */
    timeout = 0;
    while (CC1101_GDO0_Read() == 0 && timeout < 500000)
    {
        timeout++;
    }
    
    if (timeout >= 500000)
        return 0;  /* Timeout - no packet */
    
    /* Delay to ensure full packet is received */
    for (i = 0; i < 50; i++)
    {
        unsigned int j;
        for (j = 0; j < 10000; j++)
            __asm("nop");
    }
    
    /* Return to IDLE */
    CC1101_Idle();
    
    /* Read length byte */
    length = CC1101_ReadReg(CC1101_RXFIFO);
    
    if (length > CC1101_FIFO_SIZE)
        return 0;  /* Invalid length */
    
    /* Read packet data */
    for (i = 0; i < length; i++)
    {
        data[i] = CC1101_ReadReg(CC1101_RXFIFO);
    }
    
    return length;  /* Return number of bytes received */
}

/*
 * Get RSSI value
 */
unsigned char CC1101_GetRSSI(void)
{
    return CC1101_ReadReg(CC1101_RSSI);
}

/*
 * Get LQI value
 */
unsigned char CC1101_GetLQI(void)
{
    return CC1101_ReadReg(CC1101_LQI) & 0x7F;
}