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

#define CC110x_GET_GDO0()            (gpio_read(GDO0_DEV))
#define CC110x_GET_GDO2()            (gpio_read(GDO2_DEV))
#define CC110x_GET_GDO1_MISO()        (gpio_read(SPI_0_MISO_GPIO))

#define SPI_SELECT()        (gpio_clear(SPI_0_CS_GPIO))
#define SPI_UNSELECT()        (gpio_set(SPI_0_CS_GPIO))

#define CC110x_MISO_LOW_RETRY        (100)        // max. retries for MISO to go low
#define CC110x_MISO_LOW_COUNT        (2700)        // loop count (timeout ~ 500 us) to wait

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

void spi_init_gpio(void)
{
    /* Configure MISO pin */
    gpio_init_in(SPI_0_MISO_GPIO, GPIO_NOPULL);

    /* Configure MOSI pin */
    gpio_init_out(SPI_0_MOSI_GPIO, GPIO_PULLUP);
    SPI_0_MOSI_PORT->CRL |= (0xb << (SPI_0_MOSI_PIN * 4));

    /* Configure SCLK pin */
    gpio_init_out(SPI_0_SCLK_GPIO, GPIO_PULLUP);
    SPI_0_SCLK_PORT->CRL |= (0xb << (SPI_0_SCLK_PIN * 4));

    /* Configure CSn pin */
    gpio_init_out(SPI_0_CS_GPIO, GPIO_NOPULL);
}

void cc110x_spi_init(void)
{
    int retval = 0;

    DEBUG("DBG: cc110x_driver.c cc110x_spi_init()\n");
    spi_init_gpio();

    retval = spi_init_master(SPI_DEV, SPI_CONF_FIRST_RISING, SPI_SPEED_1MHZ);
    if (retval < 0) {
        puts("Can't initialize SPI!\n");
        return;
    }
    /* Disable SPI interrupt */
    SPI_0_DEV->CR2 &= 0x0;

    spi_poweron(SPI_DEV);
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
    gpio_irq_enable(GDO0_DEV);
}

void cc110x_gdo2_enable(void)
{
    gpio_irq_enable(GDO2_DEV);
}

void cc110x_gdo0_disable(void)
{
    gpio_irq_disable(GDO0_DEV);
}

void cc110x_gdo2_disable(void)
{
    gpio_irq_disable(GDO2_DEV);
}

void gpio_init_interrupt(void)
{
    int retval = 0;

    /* Initialize interrupt for GDO0 */
    retval = gpio_init_int(GDO0_DEV, GPIO_PULLUP, GPIO_RISING, (gpio_cb_t)cc110x_gdo0_irq, NULL);
    if (retval < 0) {
        puts("Can't initialize GPIO interrupt!\n");
        return;
    }

    /* Initialize interrupt for GDO2 */
    retval = gpio_init_int(GDO2_DEV, GPIO_PULLUP, GPIO_FALLING, (gpio_cb_t)cc110x_gdo2_irq, NULL);
    if (retval < 0) {
        puts("Can't initialize GPIO interrupt!\n");
        return;
    }
}

void cc110x_init_interrupts(void)
{
    gpio_init_interrupt();
}
