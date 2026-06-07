#include "gpio.h"

/*
 * Initialize GPIO ports
 */
void GPIO_Init(void)
{
    /* Configure LED pin (PB5) as output, push-pull */
    GPIOB_DDR |= (1 << LED_PIN);   /* Output */
    GPIOB_CR1 |= (1 << LED_PIN);   /* Push-pull */
    GPIOB_CR2 &= ~(1 << LED_PIN);  /* Normal speed */
    
    LED_Off();  /* LED OFF initially (active LOW) */
    
    /* Configure CC1101 CSN pin (PC3) as output, push-pull */
    GPIOC_DDR |= (1 << CC1101_CSN_PIN);   /* Output */
    GPIOC_CR1 |= (1 << CC1101_CSN_PIN);   /* Push-pull */
    GPIOC_CR2 &= ~(1 << CC1101_CSN_PIN);  /* Normal speed */
    
    CC1101_CSN_High();  /* CSN HIGH initially */
    
    /* Configure GDO0 pin (PD2) as input */
    GPIOD_DDR &= ~(1 << CC1101_GDO0_PIN);  /* Input */
    GPIOD_CR1 |= (1 << CC1101_GDO0_PIN);   /* Pull-up */
    GPIOD_CR2 &= ~(1 << CC1101_GDO0_PIN);  /* Normal speed */
}

/*
 * SPI GPIO Initialization (PC5=SCK, PC6=MOSI, PC7=MISO)
 */
void SPI_GPIO_Init(void)
{
    /* PC5 (SCK) - output, push-pull */
    GPIOC_DDR |= (1 << SPI_SCK_PIN);
    GPIOC_CR1 |= (1 << SPI_SCK_PIN);
    GPIOC_CR2 |= (1 << SPI_SCK_PIN);  /* High speed for SPI */
    
    /* PC6 (MOSI) - output, push-pull */
    GPIOC_DDR |= (1 << SPI_MOSI_PIN);
    GPIOC_CR1 |= (1 << SPI_MOSI_PIN);
    GPIOC_CR2 |= (1 << SPI_MOSI_PIN);  /* High speed for SPI */
    
    /* PC7 (MISO) - input */
    GPIOC_DDR &= ~(1 << SPI_MISO_PIN);
    GPIOC_CR1 |= (1 << SPI_MISO_PIN);  /* Pull-up */
    GPIOC_CR2 |= (1 << SPI_MISO_PIN);  /* High speed for SPI */
}

/*
 * LED ON (active LOW - pull to ground)
 */
void LED_On(void)
{
    GPIOB_ODR &= ~(1 << LED_PIN);  /* Set LOW */
}

/*
 * LED OFF (active LOW - release to +3.3V through pullup)
 */
void LED_Off(void)
{
    GPIOB_ODR |= (1 << LED_PIN);   /* Set HIGH */
}

/*
 * LED Flash sequence
 */
void LED_Flash(unsigned char count)
{
    unsigned char i;
    unsigned int j;
    
    for (i = 0; i < count; i++)
    {
        LED_On();
        /* Delay ~50ms */
        for (j = 0; j < 20000; j++)
            __asm("nop");
        
        LED_Off();
        /* Delay ~100ms */
        for (j = 0; j < 40000; j++)
            __asm("nop");
    }
}

/*
 * CC1101 CSN HIGH
 */
void CC1101_CSN_High(void)
{
    GPIOC_ODR |= (1 << CC1101_CSN_PIN);
}

/*
 * CC1101 CSN LOW
 */
void CC1101_CSN_Low(void)
{
    GPIOC_ODR &= ~(1 << CC1101_CSN_PIN);
}

/*
 * Read GDO0 pin state
 */
unsigned char CC1101_GDO0_Read(void)
{
    return (GPIOD_IDR & (1 << CC1101_GDO0_PIN)) ? 1 : 0;
}