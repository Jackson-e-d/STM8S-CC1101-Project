#ifndef CONFIG_H
#define CONFIG_H

/* ============================================
   STM8S008F Configuration
   ============================================ */

/* Oscillator frequency: 16 MHz HSE */
#define HSE_VALUE ((unsigned long)16000000)

/* SPI Configuration */
#define SPI_BAUDRATE_PRESCALER 4  /* fSPI = fMaster / 4 = 4 MHz */

/* ============================================
   CC1101 Configuration
   ============================================ */

/* Frequency: 433.500 MHz */
#define CC1101_FREQ_MHZ 433.5
#define CC1101_FREQ_REG_HIGH 0x0F
#define CC1101_FREQ_REG_MID 0x6E
#define CC1101_FREQ_REG_LOW 0x80

/* Bandwidth: 2.4 kHz */
#define CC1101_BANDWIDTH_E 0
#define CC1101_BANDWIDTH_M 0

/* Modulation: FSK, deviation ~3.8 kHz */
#define CC1101_MOD_FORMAT 0    /* FSK */
#define CC1101_DEV_E 3
#define CC1101_DEV_M 3

/* Data rate: 1.2 kbaud (for 2.4 kHz bandwidth) */
#define CC1101_DRATE_E 2
#define CC1101_DRATE_M 205

/* Power output: 0 dBm */
#define CC1101_PA_TABLE {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

/* RX/TX FIFO size: 64 bytes */
#define CC1101_FIFO_SIZE 64

/* ============================================
   Pin Configuration
   ============================================ */

/* SPI pins (STM8S008F) */
#define SPI_PORT GPIOC
#define SPI_MOSI_PIN 6    /* PC6 */
#define SPI_MISO_PIN 7    /* PC7 */
#define SPI_SCK_PIN 5     /* PC5 */

/* CC1101 Control pins */
#define CC1101_CSN_PORT GPIOC
#define CC1101_CSN_PIN 3      /* PC3 - Chip Select */

#define CC1101_GDO0_PORT GPIOD
#define CC1101_GDO0_PIN 2     /* PD2 - GPIO0 (RX/TX indicator) */

/* Status LED */
#define LED_PORT GPIOB
#define LED_PIN 5             /* PB5 - Active LOW (cathode to PB5) */

/* ============================================
   Communication Parameters
   ============================================ */

#define PACKET_SIZE 64        /* Payload size */
#define TX_SEQUENCE_START 0xFF
#define TX_SEQUENCE_END 0x00
#define LED_FLASH_COUNT 5
#define LED_FLASH_DURATION 50  /* ms */
#define LED_PAUSE_DURATION 100 /* ms */

#endif /* CONFIG_H */
