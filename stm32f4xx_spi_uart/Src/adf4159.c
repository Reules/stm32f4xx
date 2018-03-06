#include "adf4159.h"
#include "stm32f4xx_hal.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;
extern uint8_t bufferLen;
extern char uart1BufferTx[100];

extern void MX_SPI1_ADF4159_Init(void);

void adf4159Spi1Tx(uint32_t adfAddress, uint32_t data);
void Spi1Adf4159Tran(const uint16_t *data);

void adf4159Init(void){
	MX_SPI1_ADF4159_Init();

	//set CE, LE
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);

	adf4159Spi1Tx(PLL_REG_R7_DELAY, 0L);
	adf4159Spi1Tx(PLL_REG_R6_STEP, 0L);
	adf4159Spi1Tx(PLL_REG_R6_STEP, 1L << R6_SHIFT_STEP_SEL); 							// second step register is not used
	adf4159Spi1Tx(PLL_REG_R5_DEVIATION, 0L);
	adf4159Spi1Tx(PLL_REG_R5_DEVIATION, 1L << R5_SHIFT_DEV_SEL);						// second deviation register is not used
	adf4159Spi1Tx(PLL_REG_R4_CLOCK, (3L << R4_SHIFT_CLK_DIV_MODE)						// Ramp mode is selected
									| (1L<< R4_SHIFT_CLK2_DIV_VALUE));
	adf4159Spi1Tx(PLL_REG_R4_CLOCK,  (3L << R4_SHIFT_CLK_DIV_MODE)						// Clock is used for ramps
									| (1 << R4_SHIFT_CLK2_DIV_VALUE)
									| (1L << R4_SHIFT_CLK_DIV_SEL));
	adf4159Spi1Tx(PLL_REG_R3_FUNCTION, (6L << R3_SHIFT_NEG_BLEED_CURRENT)				// negative bleed current 454.7 uA; but not actually enabled
									| (1L << R3_SHIFT_RESERVED_1)						// reserved; must be 1
									| (1L << R3_SHIFT_LOL)								// disable "loss of lock" feature; not using it makes things supposedly more robust
									| (1L << R3_SHIFT_N_SEL)							// 1 = delay N word load by 4 cycles; makes transition between sweeps smoother
									| (1L << R3_SHIFT_LDP)								// "lock detect precision"; writing 1makes things supposedly more robust
									| (1L << R3_SHIFT_SIGMA_DELTA_RESET)				// 1 = disable sigma-delta reset on every write
									| (0L << R3_SHIFT_PD_POL));							// phase detector polarity is negativ
	adf4159Spi1Tx(PLL_REG_R2_R_DIVIDER, (7L << R2_SHIFT_CP_CURRENT_SETTING)				// CP current 2.5 mA
									| (0L << R2_SHIFT_RDIV2)							// f= REF x [(1+D)/(Rx(1+T))] #(D=0,T=0,R=1 and REF=64MHz)
									| (0L << R2_SHIFT_REF_DOUBLER)
									| (1L << R2_SHIFT_R_COUNTER)
									| (0L << R2_SHIFT_CLK1_DIV_VALUE));
//	//the output freqency is 24GHz/16=1.5GHz, and 1.5GHz/64MHz(Ref.)=23.4375
	adf4159Spi1Tx(PLL_REG_R1_LSB_FRAC, (0L << R1_SHIFT_LSB_FRACTIONAL_VALUE));			//Fraction LSB is 0
	adf4159Spi1Tx(PLL_REG_R0_FRAC_INT, (23L << R0_SHIFT_INTEGER_VALUE)					//Int value is 23
								    | (1792L << R0_SHIFT_MSB_FRACTIONAL_VALUE));		//Fraction MSB is 1792

	bufferLen = sprintf(uart1BufferTx, "adf4159 was initialed\n\r");
	HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen, 1000);
}

//void adf4159Spi1PllConfig(void){
//	MX_SPI1_ADF4159_Init();
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
//	adf4159Spi1Tx(PLL_REG_R7_DELAY, 0L << R7_SHIFT_TX_DATA_TRIGGER );
//	adf4159Spi1Tx(PLL_REG_R6_STEP, 0L << R6_SHIFT_STEP_SEL);
//	adf4159Spi1Tx(PLL_REG_R6_STEP, 1L << R6_SHIFT_STEP_SEL); 							// second step register is not used
//	adf4159Spi1Tx(PLL_REG_R5_DEVIATION, 0L << R5_SHIFT_DEV_SEL);
//	adf4159Spi1Tx(PLL_REG_R5_DEVIATION, 1L << R5_SHIFT_DEV_SEL);						// second deviation register is not used
////	adf4159Spi1Tx(PLL_REG_R4_CLOCK, (3L << R4_SHIFT_CLK_DIV_MODE)						// Ramp mode is selected
////									| (1L<< R4_SHIFT_CLK2_DIV_VALUE));
////	adf4159Spi1Tx(PLL_REG_R4_CLOCK,  (3L << R4_SHIFT_CLK_DIV_MODE)						// Clock is used for ramps
////									| (1 << R4_SHIFT_CLK2_DIV_VALUE)
////									| (1L << R4_SHIFT_CLK_DIV_SEL));
//
//	adf4159Spi1Tx(PLL_REG_R3_FUNCTION, (6L << R3_SHIFT_NEG_BLEED_CURRENT)				// negative bleed current 454.7 uA; but not actually enabled
//									| (1L << R3_SHIFT_RESERVED_1)						// reserved; must be 1
//									| (1L << R3_SHIFT_LOL)								// disable "loss of lock" feature; not using it makes things supposedly more robust
//									| (1L << R3_SHIFT_N_SEL)							// 1 = delay N word load by 4 cycles; makes transition between sweeps smoother
//									| (1L << R3_SHIFT_LDP)								// "lock detect precision"; writing 1makes things supposedly more robust
//									| (1L << R3_SHIFT_SIGMA_DELTA_RESET)				// 1 = disable sigma-delta reset on every write
//									| (0L << R3_SHIFT_PD_POL));							// phase detector polarity is negativ
//	adf4159Spi1Tx(PLL_REG_R2_R_DIVIDER, (7L << R2_SHIFT_CP_CURRENT_SETTING)				// CP current 2.5 mA
//									| (0L << R2_SHIFT_RDIV2)							// f= REF x [(1+D)/(Rx(1+T))] #(D=0,T=0,R=1 and REF=64MHz)
//									| (0L << R2_SHIFT_REF_DOUBLER)
//									| (1L << R2_SHIFT_R_COUNTER)
//									| (0L << R2_SHIFT_CLK1_DIV_VALUE));
////	//the output freqency is 24GHz/16=1.5GHz, and 1.5GHz/64MHz(Ref.)=23.4375
//	adf4159Spi1Tx(PLL_REG_R1_LSB_FRAC, (0L << R1_SHIFT_LSB_FRACTIONAL_VALUE));			//Fraction LSB is 0
//	adf4159Spi1Tx(PLL_REG_R0_FRAC_INT, (23L << R0_SHIFT_INTEGER_VALUE)					//Int value is 23
//								    | (1792L << R0_SHIFT_MSB_FRACTIONAL_VALUE));		//Fraction MSB is 1792
//
//	bufferLen = sprintf(uart1BufferTx, "PLL was configured\n\r");
//	HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen, 1000);
//}

//adf4159 ramp mode set
void adf4159RampOn(void) {
	MX_SPI1_ADF4159_Init();


	//disable ramp function before programming
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	//set the center Frequency in cw
//	adf4159Spi1Tx(PLL_REG_R5_DEVIATION, 0L << R5_SHIFT_DEVIATION_WORD);
//	adf4159Spi1Tx(PLL_REG_R0_FRAC_INT, (23L << R0_SHIFT_INTEGER_VALUE)
//										| (1792L << R0_SHIFT_MSB_FRACTIONAL_VALUE));
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);


	//DEV_OFFSET=1, STEP_WORD=150, DEVIATION_WORD=26215, CLK2=1, CLK1=640
	adf4159Spi1Tx(PLL_REG_R7_DELAY, 1L << R7_SHIFT_TX_DATA_TRIGGER); 					//TX_DATA_TRIGGER enabled
	adf4159Spi1Tx(PLL_REG_R6_STEP, 150L << R6_SHIFT_STEP_WORD);
	adf4159Spi1Tx(PLL_REG_R6_STEP, 1L << R6_SHIFT_STEP_SEL); 							// second step register is not used
	adf4159Spi1Tx(PLL_REG_R5_DEVIATION, (26215L << R5_SHIFT_DEVIATION_WORD)
										|(1L << R5_SHIFT_DEVIATION_OFFSET));
	adf4159Spi1Tx(PLL_REG_R5_DEVIATION, 1L << R5_SHIFT_DEV_SEL);						// second deviation register is not used
	adf4159Spi1Tx(PLL_REG_R4_CLOCK, (1L << R4_SHIFT_LE_SEL)
									| (3L << R4_SHIFT_CLK_DIV_MODE)						// Ramp mode is selected
									| (1L<< R4_SHIFT_CLK2_DIV_VALUE));
	adf4159Spi1Tx(PLL_REG_R4_CLOCK, (1L << R4_SHIFT_LE_SEL)								// enable synchronization of LE pin; avoids timing problems in chip
									| (3L << R4_SHIFT_CLK_DIV_MODE)						// Clock is used for ramps
									| (1 << R4_SHIFT_CLK2_DIV_VALUE)
									| (1L << R4_SHIFT_CLK_DIV_SEL));
	adf4159Spi1Tx(PLL_REG_R3_FUNCTION, (6L << R3_SHIFT_NEG_BLEED_CURRENT)				// negative bleed current 454.7 uA; but not actually enabled
									| (1L << R3_SHIFT_RESERVED_1)						// reserved; must be 1
									| (1L << R3_SHIFT_LOL)								// disable "loss of lock" feature; not using it makes things supposedly more robust
									| (1L << R3_SHIFT_N_SEL)							// 1 = delay N word load by 4 cycles; makes transition between sweeps smoother
									| (1L << R3_SHIFT_SIGMA_DELTA_RESET)				// 1 = disable sigma-delta reset on every write
									| (1L << R3_SHIFT_RAMP_MODE)						// ramp mode CONTINUOUS TRIANGULAR was selected
									| (1L << R3_SHIFT_LDP)								// "lock detect precision"; writing 1makes things supposedly more robust
									| (0L << R3_SHIFT_PD_POL));							// phase detector polarity is negativ
	adf4159Spi1Tx(PLL_REG_R2_R_DIVIDER, (7L << R2_SHIFT_CP_CURRENT_SETTING)				// CP current 2.5 mA
									| (0L << R2_SHIFT_RDIV2)							// f= REF x [(1+D)/(Rx(1+T))] #(D=0,T=0,R=1 and REF=64MHz)
									| (0L << R2_SHIFT_REF_DOUBLER)
									| (1L << R2_SHIFT_R_COUNTER)
									| (640L << R2_SHIFT_CLK1_DIV_VALUE));
	//the output freqency is 24GHz/16=1.5GHz, and 1.5GHz/64MHz(Ref.)=23.4375
	adf4159Spi1Tx(PLL_REG_R1_LSB_FRAC, (0L << R1_SHIFT_LSB_FRACTIONAL_VALUE));
	adf4159Spi1Tx(PLL_REG_R0_FRAC_INT, (1UL << R0_SHIFT_RAMP_ON)						//enable ramps
								    | (23L << R0_SHIFT_INTEGER_VALUE)
								    | (1792L << R0_SHIFT_MSB_FRACTIONAL_VALUE));

									//ramp mode with TXdata trigger
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

//	bufferLen = sprintf(uart1BufferTx, "Ramp Mode is on\n\r");
//	HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);
}

void adf4159Spi1Tx(uint32_t adfAddress, uint32_t data){
	uint32_t data_address;
	data = data & 0xFFFFFFF8;
	adfAddress = adfAddress & 0x00000007;
	data_address = data | adfAddress;
	uint16_t dataTx[2]={data_address >> 16, data_address};
	Spi1Adf4159Tran(dataTx);
}

void Spi1Adf4159Tran(const uint16_t *data){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) data, 2, 5000);
	//HAL_SPI_Transmit(&hspi1, (uint8_t*) data_lsb, 1, 5000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
}