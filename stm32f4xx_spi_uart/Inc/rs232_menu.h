/*
 * a menu through RS232
 * Author: Min Bao
 * Datum: 14.02.2018
 */

#ifndef _RSMENU_H
#define _RSMENU_H

#define DAC_START 		8191
#define DAC_MAX 		16383
#define DAC_MIN 		0
#define BGT_CONF_SPI	6215

// Analog Devices ADF4159 Registers
#define PLL_REG_R0_FRAC_INT		0x00    // Adress R0 , size 4 Bytes
#define PLL_REG_R1_LSB_FRAC		0x01 	// Adress R1 , size 4 Bytes
#define PLL_REG_R2_R_DIVIDER	0x02	// Adress R2 , size 4 Bytes
#define PLL_REG_R3_FUNCTION		0x03	// Adress R3 , size 4 Bytes
#define PLL_REG_R4_CLOCK		0x04	// Adress R4 , size 4 Bytes
#define PLL_REG_R5_DEVIATION	0x05	// Adress R5 , size 4 Bytes
#define PLL_REG_R6_STEP			0x06	// Adress R6 , size 4 Bytes
#define PLL_REG_R7_DELAY		0x07 	// Adress R7 , size 4 Bytes

//data shift value in register
#define R7_SHIFT_TX_DATA_TRIGGER		20
#define R6_SHIFT_STEP_WORD				3
#define R6_SHIFT_STEP_SEL				23
#define R5_SHIFT_DEVIATION_WORD			3
#define R5_SHIFT_DEVIATION_OFFSET		19
#define R5_SHIFT_DEV_SEL				23
#define R4_SHIFT_LE_SEL					31
#define R4_SHIFT_CLK_DIV_MODE			19
#define R4_SHIFT_CLK2_DIV_VALUE			7
#define R4_SHIFT_CLK_DIV_SEL			6
#define R3_SHIFT_NEG_BLEED_CURRENT		22
#define R3_SHIFT_RESERVED_1				17
#define R3_SHIFT_LOL					16
#define R3_SHIFT_N_SEL					15
#define R3_SHIFT_SIGMA_DELTA_RESET		14
#define R3_SHIFT_RAMP_MODE				10
#define R3_SHIFT_LDP					7
#define R3_SHIFT_PD_POL					6
#define R2_SHIFT_CP_CURRENT_SETTING		24
#define R2_SHIFT_RDIV2					21
#define R2_SHIFT_REF_DOUBLER			20
#define R2_SHIFT_R_COUNTER				15
#define R2_SHIFT_CLK1_DIV_VALUE			3
#define R1_SHIFT_LSB_FRACTIONAL_VALUE	15
#define R0_SHIFT_RAMP_ON				31
#define R0_SHIFT_INTEGER_VALUE			15
#define R0_SHIFT_MSB_FRACTIONAL_VALUE	3

#define PLL_F_CLOCK				SYSTEM_CLOCK_ADF4159
#define R2_R_COUNTER_VALUE		1L
#define R2_REF_IN_DOUBLER_VALUE	0L
#define R2_REFERENCE_DIV2_VALUE	0L
#define PLL_REF_IN				SYSTEM_CLOCK_ADF4159
#define PLL_F_PFD				((double)PLL_REF_IN * (1+R2_REF_IN_DOUBLER_VALUE) / (R2_R_COUNTER_VALUE * (1 + R2_REFERENCE_DIV2_VALUE)))
#define CLK1_DIV_VALUE			2L
#define CLK2_DIV_VALUE			1L
#define PD_POLARITY				0

void rs232_menu(void);
#endif
