#ifndef CC1101_H
#define CC1101_H

#include "stm8s.h"
#include "config.h"
#include "spi.h"
#include "gpio.h"

/* ============================================
   CC1101 Register Map
   ============================================ */

#define CC1101_IOCFG2       0x00
#define CC1101_IOCFG1       0x01
#define CC1101_IOCFG0       0x02
#define CC1101_FIFOTHR      0x03
#define CC1101_SYNC1        0x04
#define CC1101_SYNC0        0x05
#define CC1101_PKTLEN       0x06
#define CC1101_PKTCTRL1     0x07
#define CC1101_PKTCTRL0     0x08
#define CC1101_ADDR         0x09
#define CC1101_CHANNR       0x0A
#define CC1101_FSCTRL1      0x0B
#define CC1101_FSCTRL0      0x0C
#define CC1101_FREQ2        0x0D
#define CC1101_FREQ1        0x0E
#define CC1101_FREQ0        0x0F
#define CC1101_MDMCFG4      0x10
#define CC1101_MDMCFG3      0x11
#define CC1101_MDMCFG2      0x12
#define CC1101_MDMCFG1      0x13
#define CC1101_MDMCFG0      0x14
#define CC1101_DEVIATN      0x15
#define CC1101_MCSM2        0x16
#define CC1101_MCSM1        0x17
#define CC1101_MCSM0        0x18
#define CC1101_FOCCFG       0x19
#define CC1101_BSCFG        0x1A
#define CC1101_AGCCTRL2     0x1B
#define CC1101_AGCCTRL1     0x1C
#define CC1101_AGCCTRL0     0x1D
#define CC1101_WOERD        0x1E
#define CC1101_WORCTRL      0x1F
#define CC1101_FREND1       0x20
#define CC1101_FREND0       0x21
#define CC1101_FSCAL3       0x22
#define CC1101_FSCAL2       0x23
#define CC1101_FSCAL1       0x24
#define CC1101_FSCAL0       0x25
#define CC1101_RCCTRL1      0x26
#define CC1101_RCCTRL0      0x27
#define CC1101_FSTEST       0x29
#define CC1101_PTEST        0x2A
#define CC1101_AGCTEST      0x2B
#define CC1101_TEST2        0x2C
#define CC1101_TEST1        0x2D
#define CC1101_TEST0        0x2E
#define CC1101_PARTNUM      0x30
#define CC1101_VERSION      0x31
#define CC1101_FREQEST      0x32
#define CC1101_LQI          0x33
#define CC1101_RSSI         0x34
#define CC1101_MARCSTATE    0x35
#define CC1101_WORTIME1     0x36
#define CC1101_WORTIME0     0x37
#define CC1101_PKT_LEN      0x3E

/* Status Read-Back Register */
#define CC1101_SNOP         0x3F

/* ============================================
   Strobe Commands
   ============================================ */

#define CC1101_SRES         0x30  /* Reset chip */
#define CC1101_SFSTXON      0x31  /* Enable and calibrate frequency synthesizer */
#define CC1101_SXOFF        0x32  /* Turn off crystal oscillator */
#define CC1101_SCAL         0x33  /* Calibrate frequency synthesizer */
#define CC1101_SRX          0x34  /* Enable RX */
#define CC1101_STX          0x35  /* Enable TX */
#define CC1101_SIDLE        0x36  /* Exit RX/TX */
#define CC1101_SAFC         0x37  /* AFC adjustment of frequency synthesizer */
#define CC1101_SWOR         0x38  /* Start automatic RX polling sequence (Wake-On-Radio) */
#define CC1101_SPWD         0x39  /* Enter power down mode */
#define CC1101_SFRX         0x3A  /* Flush the RX FIFO buffer */
#define CC1101_SFTX         0x3B  /* Flush the TX FIFO buffer */
#define CC1101_SWORRST      0x3C  /* Reset real time clock */
#define CC1101_SNOP_CMD     0x3D  /* No operation */

/* ============================================
   FIFO and Register Access
   ============================================ */

#define CC1101_TXFIFO       0x3F
#define CC1101_RXFIFO       0x3F

#define CC1101_READ_BURST   0x40
#define CC1101_WRITE_BURST  0x00

#define CC1101_READ_SINGLE  0x80
#define CC1101_WRITE_SINGLE 0x00

/* ============================================
   CC1101 States (MARCSTATE) */

#define CC1101_STATE_SLEEP      0x00
#define CC1101_STATE_IDLE       0x01
#define CC1101_STATE_XOFF       0x02
#define CC1101_STATE_VCOON_MC   0x03
#define CC1101_STATE_REGON_MC   0x04
#define CC1101_STATE_MANCAL     0x05
#define CC1101_STATE_VCOON      0x06
#define CC1101_STATE_REGON      0x07
#define CC1101_STATE_STARTCAL   0x08
#define CC1101_STATE_BWBOOST    0x09
#define CC1101_STATE_FS_LOCK    0x0A
#define CC1101_STATE_IFADCON    0x0B
#define CC1101_STATE_ENDCAL     0x0C
#define CC1101_STATE_RX         0x0D
#define CC1101_STATE_RX_END     0x0E
#define CC1101_STATE_RX_RST     0x0F
#define CC1101_STATE_TXRX_SWITCH 0x10
#define CC1101_STATE_RXFIFO_OVER 0x11
#define CC1101_STATE_FSTXON     0x12
#define CC1101_STATE_TX         0x13
#define CC1101_STATE_TX_END     0x14
#define CC1101_STATE_RXTX_SWITCH 0x15
#define CC1101_STATE_TXFIFO_UNDER 0x16

/* ============================================
   Function Prototypes
   ============================================ */

void CC1101_Init(void);
void CC1101_Reset(void);
void CC1101_WriteReg(unsigned char addr, unsigned char value);
unsigned char CC1101_ReadReg(unsigned char addr);
void CC1101_WriteBurstReg(unsigned char addr, unsigned char *buffer, unsigned char length);
void CC1101_ReadBurstReg(unsigned char addr, unsigned char *buffer, unsigned char length);
void CC1101_SendStrobe(unsigned char strobe);
unsigned char CC1101_GetState(void);
void CC1101_SetTxMode(void);
void CC1101_SetRxMode(void);
void CC1101_Idle(void);
unsigned char CC1101_SendPacket(unsigned char *data, unsigned char length);
unsigned char CC1101_ReceivePacket(unsigned char *data);
unsigned char CC1101_GetRSSI(void);
unsigned char CC1101_GetLQI(void);

#endif /* CC1101_H */
