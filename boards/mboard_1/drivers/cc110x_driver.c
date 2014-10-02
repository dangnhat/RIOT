/*
 * Copyright (C) 2014 Ho Chi Minh City University of Technology
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     mboard1
 * @{
 *
 * @file
 * @brief       Low level diver for cc110x.
 *
 * @author      Hien Van Nguyen <nvhien1992@gmail.com>
 */
#include <stdio.h>
#include <stddef.h>

#include "cpu.h"
#include "sched.h"
#include "vtimer.h"

#include "periph_conf.h"
#include "periph/gpio.h"
#include "periph/spi.h"

/* drivers  */
#include "cc110x_ng.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

#define CC110x_GET_GDO0()            (gpio_read(CC1101_GDO0))
#define CC110x_GET_GDO2()            (gpio_read(CC1101_GDO2))
#define CC110x_GET_GDO1_MISO()        (gpio_read(CC1101_MISO))

#define SPI_SELECT()        (gpio_clear(CC1101_CS))
#define SPI_UNSELECT()        (gpio_set(CC1101_CS))

#define CC110x_MISO_LOW_RETRY        (100)        // max. retries for MISO to go low
#define CC110x_MISO_LOW_COUNT        (2700)        // loop count (timeout ~ 500 us) to wait

static void gpio_init_interrupt(void);

int cc110x_get_gdo0(void)
{
    return CC110x_GET_GDO0();
}

int cc110x_get_gdo1(void)
{
    return CC110x_GET_GDO1_MISO();
}

int cc110x_get_gdo2(void)
{
    return CC110x_GET_GDO2();
}

void cc110x_spi_init(void)
{
    int retval = 0;

    DEBUG("DBG: cc110x_driver.c cc110x_spi_init()\n");
    /* Configure CSn pin */
    gpio_init_out(CC1101_CS, GPIO_NOPULL);

    retval = spi_init_master(CC1101_SPI, SPI_CONF_FIRST_RISING, SPI_SPEED_1MHZ);
    if (retval < 0) {
        puts("Can't initialize SPI!\n");
        return;
    }
    /* Disable SPI interrupt */
    SPI_0_DEV->CR2 &= 0x0;

    spi_poweron(CC1101_SPI);
}

uint8_t cc110x_txrx(uint8_t value)
{
    while ((SPI_0_DEV->SR & SPI_SR_TXE) == RESET);
    SPI_0_DEV->DR = value;

    while ((SPI_0_DEV->SR & SPI_SR_RXNE) == RESET);
    return SPI_0_DEV->DR;
}

void cc110x_spi_cs(void)
{
    SPI_SELECT();
}

void cc110x_spi_unselect(void)
{
    SPI_UNSELECT();
}

void cc110x_spi_select(void)
{
    volatile int retry_count = 0;
    volatile int abort_count;

    cc110x_spi_cs();    //CS to low
    while(CC110x_GET_GDO1_MISO() != 0) {
        cc110x_spi_cs();
        abort_count++;
        if (abort_count > CC110x_MISO_LOW_COUNT) {
            retry_count++;
            if (retry_count > CC110x_MISO_LOW_RETRY) {
                puts("[CC110x SPI] fatal error\n");
                return;
            }
            cc110x_spi_unselect();        // CS to high
        }
    }
}

void cc110x_before_send(void)
{
    //Disable GDO2 interrupt before sending packet
    cc110x_gdo2_disable();
}

void cc110x_after_send(void)
{
    //Enable GDO2 interrupt after sending packet
    cc110x_gdo2_enable();
}

void cc110x_gdo0_enable(void)
{
    gpio_irq_enable(CC1101_GDO0);
}

void cc110x_gdo2_enable(void)
{
    gpio_irq_enable(CC1101_GDO2);
}

void cc110x_gdo0_disable(void)
{
    gpio_irq_disable(CC1101_GDO0);
}

void cc110x_gdo2_disable(void)
{
    gpio_irq_disable(CC1101_GDO2);
}

static void gpio_init_interrupt(void)
{
    int retval = 0;

    /* Initialize interrupt for GDO0 */
    retval = gpio_init_int(CC1101_GDO0, GPIO_PULLUP, GPIO_RISING, (gpio_cb_t)cc110x_gdo0_irq, NULL);
    if (retval < 0) {
        puts("Can't initialize GPIO interrupt!\n");
        return;
    }

    /* Initialize interrupt for GDO2 */
    retval = gpio_init_int(CC1101_GDO2, GPIO_PULLUP, GPIO_FALLING, (gpio_cb_t)cc110x_gdo2_irq, NULL);
    if (retval < 0) {
        puts("Can't initialize GPIO interrupt!\n");
        return;
    }
}

void cc110x_init_interrupts(void)
{
    gpio_init_interrupt();
}
