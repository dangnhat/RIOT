/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     mboard1
 * @{
 *
 * @file        cc110x_reconfig.c
 * @brief       Re-configure ouput power (PATABLE) and frequency carrier for MBoard1.
 *
 * @author      Hien Van Nguyen <nvhien1992@gmail.com>
 */
#include "cc110x_ng.h"
#include "cc110x_reconfig.h"

#define PA_INDEX_DEFAULT	(4)

uint8_t pa_table_433[] = {
		0x12,         ///< -30 dBm
		0x0E,         ///< -20 dBm
		0x1D,         ///< -15 dBm
		0x34,         ///< -10 dBm
		0x60,         ///<   0 dBm
		0x84,         ///< + 5 dBm
		0xC8,         ///< + 7 dBm
		0xC0          ///< +10 dBm
};

char conf_table_433[] = {
	    0x06, // IOCFG2
	    0x2E, // IOCFG1
	    0x0E, // IOCFG0
	    0x0F, // FIFOTHR
	    0x9B, // SYNC1
	    0xAD, // SYNC0
	    0x3D, // PKTLEN     (maximum value of packet length byte = 61)
	    0x06, // PKTCTRL1
	    0x45, // PKTCTRL0   (variable packet length)
	    0xFF, // ADDR
	    CC1100_DEFAULT_CHANNR * 10, // CHANNR
	    0x0B, // FSCTRL1
	    0x00, // FSCTRL0
	    0x10, // FREQ2
	    0xB1, // FREQ1
	    0x3B, // FREQ0
	    0x2D, // MDMCFG4
	    0xF8, // MDMCFG3
	    0x73, // MDMCFG2
	    0x42, // MDMCFG1
	    0xF8, // MDMCFG0
	    0x00, // DEVIATN
	    0x07, // MCSM2
	    0x03, // MCSM1
	    0x18, // MCSM0
	    0x1D, // FOCCFG
	    0x1C, // BSCFG
	    0xC0, // AGCCTRL2
	    0x49, // AGCCTRL1, (old value was 0x49 -> made carrier sense less sensitive!)
	          //            0x47 - 7 dB above MAGN_TARGET setting
	    0xB2, // AGCCTRL0
	    0x87, // WOREVT1
	    0x6B, // WOREVT0
	    0xF8, // WORCTRL
	    0xB6, // FREND1
	    0x10, // FREND0
	    0xEA, // FSCAL3
	    0x2A, // FSCAL2
	    0x00, // FSCAL1
	    0x1F, // FSCAL0
	    0x00  // padding to 4 bytes
};

void cc110x_set_up_patable(uint8_t PA_value) {
	cc110x_write_reg(0x3E, PA_value);
}

void cc110x_set_up_freq_carrier(uint8_t freq2, uint8_t freq1, uint8_t freq0) {
	cc110x_write_reg(0x0D, freq2);
	cc110x_write_reg(0x0E, freq1);
	cc110x_write_reg(0x0F, freq0);
}

void cc110x_reconfig(void) {
	cc110x_writeburst_reg(0x00, conf_table_433, CC1100_CONF_SIZE);
	cc110x_set_up_patable(pa_table_433[PA_INDEX_DEFAULT]);
}
