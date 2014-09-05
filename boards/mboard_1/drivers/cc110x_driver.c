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

#define CC110x_GET_GDO0()			(gpio_read(GDO0_DEV))
#define CC110x_GET_GDO2()			(gpio_read(GDO2_DEV))
#define CC110x_GET_GDO1_MISO()		(gpio_read(SPI_0_MISO_GPIO))

#define SPI_SELECT()		(gpio_clear(SPI_0_CS_GPIO))
#define SPI_UNSELECT()		(gpio_set(SPI_0_CS_GPIO))

#define CC110x_MISO_LOW_RETRY		(100)		// max. retries for MISO to go low
#define CC110x_MISO_LOW_COUNT		(2700)		// loop count (timeout ~ 500 us) to wait

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
static
void init_gpio_spi(void)
{
	/* Configure MISO pin */
	gpio_init_in(SPI_0_MISO_GPIO, GPIO_NOPULL);

	/* Configure MOSI pin */
	gpio_init_out(SPI_0_MOSI_GPIO, GPIO_PULLUP);

	/* Configure SCLK pin */
	gpio_init_out(SPI_0_SCLK_GPIO, GPIO_PULLUP);

	/* Configure CSn pin */
	gpio_init_out(SPI_0_CS_GPIO, GPIO_PULLUP);
}

void cc110x_spi_init(void)
{
	init_gpio_spi();

	spi_init_master(SPI_DEV, SPI_CONF_FIRST_FALLING, SPI_SPEED_5MHZ);

	spi_poweron(SPI_DEV);

//	GPIO_InitTypeDef GPIO_InitStructure;
//	SPI_InitTypeDef SPI_InitStructure;
//
//	/* RCC */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
//
//	/* GPIO */
//	/* Configure SPI MASTER pins */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	/*
//	 * SPI
//	 * NOTE: APB2 is 72MHz, prescaler 16 => SPI @ 4.5 MHz, radio spi max is 7.5MHz
//	 * Clock idle low, rising edge
//	 */
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//	SPI_InitStructure.SPI_CRCPolynomial = 7;
//	SPI_Init(SPI1, &SPI_InitStructure);
//
//	/* Enable SPI */
//	SPI_Cmd(SPI1, ENABLE);
}

uint8_t cc110x_txrx(uint8_t value)
{
    uint8_t retval = 0;

    spi_transfer_byte(SPI_DEV, retval, (char *)&value);

    return retval;
}

void spi_cs(void)
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

    spi_cs();	//CS to low
    while(CC110x_GET_GDO1_MISO()) {
    	spi_cs();
		abort_count++;
		if (abort_count > CC110x_MISO_LOW_COUNT) {
			retry_count++;
			if (retry_count > CC110x_MISO_LOW_RETRY) {
				puts("[CC1100 SPI] fatal error\n");
				return;
			}
			cc110x_spi_unselect();		// CS to high
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

void init_interrupt(void)
{
	/* Initialize interrupt for GDO0 */
	gpio_init_int(GDO0_DEV, GPIO_NOPULL, GPIO_RISING, (void*)&cc110x_gdo0_irq, NULL);

	/* Initialize interrupt for GDO2 */
	gpio_init_int(GDO2_DEV, GPIO_NOPULL, GPIO_FALLING, (void*)&cc110x_gdo2_irq, NULL);

//	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//	/* Enable AFIO clock */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//
//	/* Connect EXTI0 and EXTI1 Line to PB0 and PB1 pin */
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
//
//	/* Configure EXTI0 and EXTI1 line */
//	enable_EXTI_interrupt(EXTI_Trigger_Rising, GDO0_line);
//	enable_EXTI_interrupt(EXTI_Trigger_Falling, GDO2_line);
//
//	/* Enable and set EXTI4 Interrupt to the lowest priority */
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
//	NVIC_Init(&NVIC_InitStructure);
}

void cc110x_init_interrupts(void)
{
	init_interrupt();
}

//extern void cc110x_gdo0_irq(void);

//extern void cc110x_gdo2_irq(void);
