/*
 * Copyright (C) 2014 Freie Universität Berlin, Ho Chi Minh City University of Technology
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     mboard1
 * @{
 *
 * @file        periph_conf.h
 * @brief       Peripheral MCU configuration for the MBoard1 board
 *
 * @author		Thomas Eichinger <thomas.eichinger@fu-berlin.de>
 * @author      Hien Van Nguyen <nvhien1992@gmail.com>
 */
#ifndef __PERIPH_CONF_H
#define __PERIPH_CONF_H

/**
 * @name Clock system configuration
 * @{
 **/
#define CLOCK_HSE           (16000000U)             /* frequency of external oscillator */
#define CLOCK_CORECLOCK     (72000000U)             /* targeted core clock frequency */
/* configuration of PLL prescaler and multiply values */
/* CORECLOCK := HSE / PLL_HSE_DIV * PLL_HSE_MUL */
#define CLOCK_PLL_HSE_DIV   RCC_CFGR_PLLXTPRE_HSE_Div2
#define CLOCK_PLL_HSE_MUL   RCC_CFGR_PLLMULL9
/* configuration of peripheral bus clock prescalers */
#define CLOCK_AHB_DIV       RCC_CFGR_HPRE_DIV1      /* AHB clock -> 72MHz */
#define CLOCK_APB2_DIV      RCC_CFGR_PPRE2_DIV1     /* APB2 clock -> 72MHz */
#define CLOCK_APB1_DIV      RCC_CFGR_PPRE1_DIV2     /* APB1 clock -> 36MHz */
/* configuration of flash access cycles */
#define CLOCK_FLASH_LATENCY FLASH_ACR_LATENCY_2
/** @} */

/**
 * @brief Timer configuration
 * @{
 */
#define TIMER_NUMOF         (2U)
#define TIMER_0_EN          1
#define TIMER_1_EN          1

/* Timer 0 configuration */
#define TIMER_0_DEV         TIM2
#define TIMER_0_CHANNELS    4
#define TIMER_0_PRESCALER   (36000U)
#define TIMER_0_MAX_VALUE   (0xffff)
#define TIMER_0_CLKEN()     (RCC->APB1ENR |= RCC_APB1ENR_TIM2EN)
#define TIMER_0_ISR         isr_tim2
#define TIMER_0_IRQ_CHAN    TIM2_IRQn
#define TIMER_0_IRQ_PRIO    1

/* Timer 1 configuration */
#define TIMER_1_DEV         TIM3
#define TIMER_1_CHANNELS    2
#define TIMER_1_PRESCALER   (36000U)
#define TIMER_1_MAX_VALUE   (0xffff)
#define TIMER_1_CLKEN()     (RCC->APB1ENR |= RCC_APB1ENR_TIM3EN)
#define TIMER_1_ISR         isr_tim3
#define TIMER_1_IRQ_CHAN    TIM3_IRQn
#define TIMER_1_IRQ_PRIO    1
/** @} */

/**
 * @brief UART configuration
 */
#define UART_NUMOF          (2U)
#define UART_0_EN           1
#define UART_1_EN           0
#define UART_IRQ_PRIO       1

/* UART 0 device configuration */
#define UART_0_DEV          USART1
#define UART_0_CLKEN()      (RCC->APB2ENR |= RCC_APB2ENR_USART1EN)
#define UART_0_IRQ          USART1_IRQn
#define UART_0_ISR          isr_usart1
#define UART_0_BUS_FREQ     72000000
/* UART 0 pin configuration */
#define UART_0_PORT         GPIOA
#define UART_0_PORT_CLKEN() (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)
#define UART_0_RX_PIN       10
#define UART_0_TX_PIN       9
#define UART_0_AF           0

/**
 * @brief present enable/disable GPIO channel configuration
 */
#define GPIO_NUMOF          16
#define GPIO_0_EN           0        /* not configured */
#define GPIO_1_EN           0        /* not configured */
#define GPIO_2_EN           0        /* not configured */
#define GPIO_3_EN           0        /* not configured */
#define GPIO_4_EN           0        /* not configured */
#define GPIO_5_EN           0        /* not configured */
#define GPIO_6_EN           1        /* GDO0 */
#define GPIO_7_EN           1        /* GDO2 */
#define GPIO_8_EN           1        /* SCLK */
#define GPIO_9_EN           1        /* MISO */
#define GPIO_10_EN          1        /* MOSI */
#define GPIO_11_EN          1        /* CSn */
#define GPIO_12_EN          0        /* not configured */
#define GPIO_13_EN          0        /* not configured */
#define GPIO_14_EN          0        /* not configured */
#define GPIO_15_EN          0        /* not configured */

#define GPIO_IRQ_PRIO       1        /* priority */

/* Define GPIO attributes for GDO0 and GDO2 */
#define GDO0_DEV            GPIO_6        //set channel
#define GDO0_PORT           GPIOB        //GPIOC
#define GDO0_PIN            0            //7
#define GDO0_EXTI_LINE      GDO0_PIN
#define GDO0_CLKEN()        (RCC->APB2ENR |= RCC_APB2ENR_IOPBEN)         //(RCC->APB2ENR |= RCC_APB2ENR_IOPCEN)
#define GDO0_EXTI_CFG()     (AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI0_PB)    //(AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI7_PC)
#define GDO0_IRQ_INDEX      GPIO_0        //GPIO_7
#define GDO0_IRQ_HANDLER    EXTI0_IRQn    //EXTI9_5_IRQn

#define GDO2_DEV            GPIO_7        //set channel
#define GDO2_PORT           GPIOB        //GPIOC
#define GDO2_PIN            1            //11
#define GDO2_EXTI_LINE      GDO2_PIN
#define GDO2_CLKEN()        (RCC->APB2ENR |= RCC_APB2ENR_IOPBEN)        //(RCC->APB2ENR |= RCC_APB2ENR_IOPCEN)
#define GDO2_EXTI_CFG()     (AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PB)    //(AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI11_PC)
#define GDO2_IRQ_INDEX      GPIO_1        //GPIO_11
#define GDO2_IRQ_HANDLER    EXTI1_IRQn    //EXTI15_10_IRQn

/* IRQ configurations that are the index of callback function in the callback function table.
 * Therefore, many pins can use a common callback function */
#define GPIO_IRQ_0          GPIO_0    /* not configured */
#define GPIO_IRQ_1          GPIO_0    /* not configured */
#define GPIO_IRQ_2          GPIO_0    /* not configured */
#define GPIO_IRQ_3          GPIO_0    /* not configured */
#define GPIO_IRQ_4          GPIO_0    /* not configured */
#define GPIO_IRQ_5          GPIO_0    /* not configured */
#define GPIO_IRQ_6          GDO0_IRQ_INDEX    /* Index of GDO0 callback function */
#define GPIO_IRQ_7          GDO2_IRQ_INDEX    /* Index of GDO2 callback function */
#define GPIO_IRQ_8          GPIO_0    /* not configured */
#define GPIO_IRQ_9          GPIO_0    /* not configured */
#define GPIO_IRQ_10         GPIO_0    /* not configured */
#define GPIO_IRQ_11         GPIO_0    /* not configured */
#define GPIO_IRQ_12         GPIO_0    /* not configured */
#define GPIO_IRQ_13         GPIO_0    /* not configured */
#define GPIO_IRQ_14         GPIO_0    /* not configured */
#define GPIO_IRQ_15         GPIO_0    /* not configured */

/* GPIO channel 0 config */
#define GPIO_0_PORT         GPIOA
#define GPIO_0_PIN          3
#define GPIO_0_CLKEN()      (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)
#define GPIO_0_EXTI_CFG()   (AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI3_PA)
#define GPIO_0_EXTI_LINE    4
#define GPIO_0_IRQ          EXTI4_IRQn
/* GPIO channel 1 config */
#define GPIO_1_PORT         GPIOA
#define GPIO_1_PIN          8
#define GPIO_1_CLKEN()      (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)
#define GPIO_1_EXTI_CFG()   (AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI8_PA)
#define GPIO_1_EXTI_LINE    4
#define GPIO_1_IRQ          EXTI4_IRQn
/* GPIO channel 2 config */
#define GPIO_2_PORT         GPIOA
#define GPIO_2_PIN          12
#define GPIO_2_CLKEN()      (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)
#define GPIO_2_EXTI_CFG()   (AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI12_PA)
#define GPIO_2_EXTI_LINE    4
#define GPIO_2_IRQ          EXTI4_IRQn
/* GPIO channel 3 config */
#define GPIO_3_PORT         GPIOB
#define GPIO_3_PIN          8
#define GPIO_3_CLKEN()      (RCC->APB2ENR |= RCC_APB2ENR_IOPBEN)
#define GPIO_3_EXTI_CFG()   (AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI8_PB)
#define GPIO_3_EXTI_LINE    4
#define GPIO_3_IRQ          EXTI4_IRQn
/* GPIO channel 4 config */
#define GPIO_4_PORT         GPIOB
#define GPIO_4_PIN          9
#define GPIO_4_CLKEN()      (RCC->APB2ENR |= RCC_APB2ENR_IOPBEN)
#define GPIO_4_EXTI_CFG()   (AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI9_PB)
#define GPIO_4_EXTI_LINE    4
#define GPIO_4_IRQ          EXTI4_IRQn
/* GPIO channel 5 config */
#define GPIO_5_PORT         GPIOC
#define GPIO_5_PIN          7
#define GPIO_5_CLKEN()      (RCC->APB2ENR |= RCC_APB2ENR_IOPCEN)
#define GPIO_5_EXTI_CFG()   (AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI7_PC)
#define GPIO_5_EXTI_LINE    4
#define GPIO_5_IRQ          EXTI4_IRQn
/* =============================================== */
/* GPIO channel 6 config */
#define GPIO_6_PORT         GDO0_PORT
#define GPIO_6_PIN          GDO0_PIN        /* GDO0 */
#define GPIO_6_CLKEN()      GDO0_CLKEN()
#define GPIO_6_EXTI_CFG()   GDO0_EXTI_CFG()
#define GPIO_6_EXTI_LINE    GDO0_EXTI_LINE
#define GPIO_6_IRQ          GDO0_IRQ_HANDLER
/* GPIO channel 7 config */
#define GPIO_7_PORT         GDO2_PORT
#define GPIO_7_PIN          GDO2_PIN        /* GDO2 */
#define GPIO_7_CLKEN()      GDO2_CLKEN()
#define GPIO_7_EXTI_CFG()   GDO2_EXTI_CFG()
#define GPIO_7_EXTI_LINE    GDO2_EXTI_LINE
#define GPIO_7_IRQ          GDO2_IRQ_HANDLER
/* GPIO channel 8 config */
#define GPIO_8_PORT         GPIOA
#define GPIO_8_PIN          5
#define GPIO_8_CLKEN()      (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)
#define GPIO_8_EXTI_CFG()   (AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI5_PA)
#define GPIO_8_EXTI_LINE    4
#define GPIO_8_IRQ          EXTI4_IRQn
/* GPIO channel 9 config */
#define GPIO_9_PORT         GPIOA
#define GPIO_9_PIN          6
#define GPIO_9_CLKEN()      (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)
#define GPIO_9_EXTI_CFG()   (AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI6_PA)
#define GPIO_9_EXTI_LINE    4
#define GPIO_9_IRQ          EXTI4_IRQn
/* GPIO channel 10 config */
#define GPIO_10_PORT        GPIOA
#define GPIO_10_PIN         7
#define GPIO_10_CLKEN()     (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)
#define GPIO_10_EXTI_CFG()  (AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI7_PA)
#define GPIO_10_EXTI_LINE   4
#define GPIO_10_IRQ         EXTI4_IRQn
/* GPIO channel 11 config */
#define GPIO_11_PORT        GPIOA
#define GPIO_11_PIN         4        /* CSn */
#define GPIO_11_CLKEN()     (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)
#define GPIO_11_EXTI_CFG()  (AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI4_PA)    /* don't care if not use interrupt */
#define GPIO_11_EXTI_LINE   4            /* don't care if not use interrupt */
#define GPIO_11_IRQ         EXTI4_IRQn    /* don't care if not use interrupt */
/* =============================================== */
/* GPIO channel 12 config */
#define GPIO_12_PORT        GPIOC
#define GPIO_12_PIN         4
#define GPIO_12_CLKEN()     (RCC->APB2ENR |= RCC_APB2ENR_IOPCEN)
#define GPIO_12_EXTI_CFG()  (AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI4_PC)
#define GPIO_12_EXTI_LINE   4
#define GPIO_12_IRQ         EXTI4_IRQn
/* GPIO channel 13 config */
#define GPIO_13_PORT        GPIOC
#define GPIO_13_PIN         1
#define GPIO_13_CLKEN()     (RCC->APB2ENR |= RCC_APB2ENR_IOPCEN)
#define GPIO_13_EXTI_CFG()  (AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PC)
#define GPIO_13_EXTI_LINE   4
#define GPIO_13_IRQ         EXTI4_IRQn
/* GPIO channel 14 config */
#define GPIO_14_PORT        GPIOA
#define GPIO_14_PIN         2
#define GPIO_14_CLKEN()     (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)
#define GPIO_14_EXTI_CFG()  (AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI2_PA)
#define GPIO_14_EXTI_LINE   4
#define GPIO_14_IRQ         EXTI4_IRQn
/* GPIO channel 15 config */
#define GPIO_15_PORT        GPIOC
#define GPIO_15_PIN         15
#define GPIO_15_CLKEN()     (RCC->APB2ENR |= RCC_APB2ENR_IOPCEN)
#define GPIO_15_EXTI_CFG()  (AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI15_PC)
#define GPIO_15_EXTI_LINE   4
#define GPIO_15_IRQ         EXTI4_IRQn

/**
 * @brief SPI configuration
 */
#define SPI_NUMOF       (1U)
#define SPI_0_EN        1

/* SPI 0 device configuration */
#define SPI_0_DEV       		SPI1
#define SPI_0_CLKEN()       	(RCC->APB2ENR |= RCC_APB2ENR_SPI1EN)
#define SPI_0_CLKDIS()      	(RCC->APB2ENR &= ~(RCC_APB2ENR_SPI1EN))
#define SPI_0_BUS_DIV       	1   /* 1 -> SPI runs with full CPU clock, 0 -> half CPU clock */

/* Define channel/pin/port for SCLK */
#define SPI_0_SCLK_PORT     	GPIOA
#define SPI_0_SCLK_PIN      	5
#define SPI_0_CLK_PORT_CLKEN()  (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)
/* Define channel/pin/port for MISO */
#define SPI_0_MOSI_PORT         GPIOA
#define SPI_0_MOSI_PIN          7
#define SPI_0_MOSI_PORT_CLKEN() (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)
/* Define channel/pin/port for MOSI */
#define SPI_0_MISO_PORT         GPIOA
#define SPI_0_MISO_PIN          6
#define SPI_0_MISO_PORT_CLKEN() (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)

#endif /* __PERIPH_CONF_H */
/** @} */
