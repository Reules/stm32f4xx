/*
 * a menu through RS232
 * Author: Min Bao
 * Datum: 14.02.2018
 */

#include "rs232_menu.h"
#include "stm32f4xx_hal.h"
#include "main.h"
extern void MX_SPI1_ADF4159_Init(void);
extern void MX_SPI1_BGT24_Init(void);

extern UART_HandleTypeDef huart1;
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;

static char uart1DataRx;			//recieve interrupt Bit
static char uart1BufferTx[100];		//the Buffer of printf
static uint8_t uart1TxCplt;			//Interrupt complete Bit
static uint8_t bufferLen;

static uint8_t optionSelect;
static uint16_t spi3DataTx = DAC_START;
static uint16_t spi1DataTxBgt = BGT_CONF_SPI;

struct rs232_menu{
	char menuName[50];
	int menuLength;
	char *menuOption[10];
};

static struct rs232_menu mainMenu = {"Main Menu",
	3,
	{"DAC value setting",
	"ADF4159 configuration",
	"BGT24MTR11 configuration"}
};

static struct rs232_menu dacMenu = {"DAC value setting",
	5,
	{"+1",
	 "-2",
	 "x2",
	 "/2",
	 "back"
	}
};

static struct rs232_menu adf4159Menu = {"ADF4159 configuration",
	3,
	{"PLL configuration",
	 "Ramp Set",
	 "back"
	}
};

static struct rs232_menu bgt24Menu = {"BGT24MTR11 configuration",
	2,
	{"BGT24 SPI configuration",
	 "back"
	}
};


//Main menu
static void mainMenuExcution(const struct rs232_menu *menu);

/*Submenu*/
static void dacMenuExcution(const struct rs232_menu *menu);
static void adf4159MenuExcution(const struct rs232_menu *menu);
static void bgt24MenuExcution(const struct rs232_menu *menu);

/*the functions of submenu*/
static void dacAddtion(void);
static void dacSubtraction(void);
static void dacMultiplication(void);
static void dacDivision(void);

static void adf4159Spi1PllConfig(void);
static void adf4159RampSet(void);

static void bgt24Spi1Config(void);

/*the tranmission functions*/
static void Spi3DacTran(void);
static void Spi1Adf4159Tran(const uint16_t *data);
static void Spi1Bgt24Tran(void);

static void adf4159Spi1Tx(uint32_t adfAddress, uint32_t data);

//Interrupt call back function
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1)  //current UART
		uart1TxCplt = 1;          //transfer complete, data is ready to read
}

//the interface function to outside
void rs232_menu(void){
	mainMenuExcution(&mainMenu);
}

//the menu function definition
static void mainMenuExcution(const struct rs232_menu *menu)
{

	do {          //print function through uart1, print Menu Name and options
		bufferLen = sprintf(uart1BufferTx, "%s\n\r", menu->menuName);
		HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen, 1000);
		for (int i = 0; i < menu->menuLength; i++) {
			bufferLen = sprintf(uart1BufferTx, "%d: %s\n\r", i + 1,
					menu->menuOption[i]);
			HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen,
					1000);
		};

		//print enter
		bufferLen = sprintf(uart1BufferTx, "\n\r");
		HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen, 1000);

		HAL_UART_Receive_IT(&huart1, (uint8_t *) &uart1DataRx, 1); //active the uart1 to accept interrupt
		while (!uart1TxCplt) {
		};					//stop the program until the interrupt is accepted

		optionSelect = uart1DataRx - '0';			//convert the char to int
		uart1TxCplt = 0;					//reset transfer complete variable

		switch (optionSelect) {
		case 1:
			dacMenuExcution(&dacMenu);
			break;
		case 2:
			adf4159MenuExcution(&adf4159Menu);
			break;
		case 3:
			bgt24MenuExcution(&bgt24Menu);
		}
	} while (optionSelect < 1 || optionSelect > 3);
}

static void dacMenuExcution(const struct rs232_menu *menu) {
	do {
		//print function through uart1
		bufferLen = sprintf(uart1BufferTx, "%s\n\r", menu->menuName);
		HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen, 1000);

		for (int i = 0; i < menu->menuLength; i++) {
			bufferLen = sprintf(uart1BufferTx, "%d: %s\n\r", i + 1,
					menu->menuOption[i]);
			HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen,
					1000);
		};

		//print current dac value and transmit to dac
		Spi3DacTran();
		bufferLen = sprintf(uart1BufferTx, "current DAC value is: %d\r",
				spi3DataTx);
		HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen, 1000);

		bufferLen = sprintf(uart1BufferTx, "\n\r");
		HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen, 1000);

		do {
			HAL_UART_Receive_IT(&huart1, (uint8_t *) &uart1DataRx, 1); //active the uart1 to accept interrupt
			while (!uart1TxCplt) {
			};				//stop the program until the interrupt is accepted

			optionSelect = uart1DataRx - '0';		//convert the char to int
			uart1TxCplt = 0;				//reset transfer complete variable

			switch (optionSelect) {
			case 1:
				dacAddtion();
				break;
			case 2:
				dacSubtraction();
				break;
			case 3:
				dacMultiplication();
				break;
			case 4:
				dacDivision();
				break;
			case 5:
				mainMenuExcution(&mainMenu);
			}
		} while (optionSelect > 0 && optionSelect <= menu->menuLength);
	} while (optionSelect < 1 || optionSelect > 5);
}

static void adf4159MenuExcution(const struct rs232_menu *menu) {

	do {
		//print function through uart1
		bufferLen = sprintf(uart1BufferTx, "%s\n\r", menu->menuName);
		HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen, 1000);

		for (int i = 0; i < menu->menuLength; i++) {
			bufferLen = sprintf(uart1BufferTx, "%d: %s\n\r", i + 1,
					menu->menuOption[i]);
			HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen,
					1000);
		};

		bufferLen = sprintf(uart1BufferTx, "\n\r");
		HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen, 1000);

		do {
			HAL_UART_Receive_IT(&huart1, (uint8_t *) &uart1DataRx, 1); //active the uart1 to accept interrupt
			while (!uart1TxCplt) {
			};				//stop the program until the interrupt is accepted

			optionSelect = uart1DataRx - '0';		//convert the char to int
			uart1TxCplt = 0;
			switch (optionSelect) {
			case 1:
				adf4159Spi1PllConfig();
				break;
			case 2:
				adf4159RampSet();
				break;
			case 3:
				mainMenuExcution(&mainMenu);
			}
		} while (optionSelect > 0 && optionSelect <= menu->menuLength);
	} while (optionSelect < 1 || optionSelect > 3);
}

static void bgt24MenuExcution(const struct rs232_menu *menu) {
	do {
		//print function through uart1
		bufferLen = sprintf(uart1BufferTx, "%s\n\r", menu->menuName);
		HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen, 1000);

		for (int i = 0; i < menu->menuLength; i++) {
			bufferLen = sprintf(uart1BufferTx, "%d: %s\n\r", i + 1,
					menu->menuOption[i]);
			HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen,
					1000);
		};

		bufferLen = sprintf(uart1BufferTx, "\n\r");
		HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, bufferLen, 1000);

		do {
			HAL_UART_Receive_IT(&huart1, (uint8_t *) &uart1DataRx, 1); //active the uart1 to accept interrupt
			while (!uart1TxCplt) {
			};				//stop the program until the interrupt is accepted

			optionSelect = uart1DataRx - '0';		//convert the char to int
			uart1TxCplt = 0;
			switch (optionSelect) {
			case 1:
				bgt24Spi1Config();
				break;
			case 2:
				mainMenuExcution(&mainMenu);
			}
		} while (optionSelect > 0 && optionSelect <= menu->menuLength);
	} while (optionSelect < 1 || optionSelect > 2);
}


static void dacAddtion(void){
	spi3DataTx = spi3DataTx + 1;
	if(spi3DataTx > DAC_MAX){
		bufferLen = sprintf(uart1BufferTx, "out of range!\n\r");
		HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);
		spi3DataTx = spi3DataTx - 1;
	}
	Spi3DacTran();
	bufferLen = sprintf(uart1BufferTx,"current DAC value is: %d\r", spi3DataTx);
	HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);
}

static void dacSubtraction(void){
	spi3DataTx = spi3DataTx - 1;
	if (spi3DataTx < DAC_MIN || spi3DataTx > DAC_MAX) {
		bufferLen = sprintf(uart1BufferTx, "out of range!\n\r");
		HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);
		spi3DataTx = spi3DataTx + 1;
	}
	Spi3DacTran();
	bufferLen = sprintf(uart1BufferTx,"current DAC value is: %d\r", spi3DataTx);
	HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);
}

static void dacMultiplication(void){
	spi3DataTx = spi3DataTx * 2;
	if (spi3DataTx > DAC_MAX) {
	bufferLen = sprintf(uart1BufferTx, "out of range!\n\r");
		HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);
		spi3DataTx = spi3DataTx / 2;
	}
	Spi3DacTran();
	bufferLen = sprintf(uart1BufferTx,"current DAC value is: %d\r", spi3DataTx);
	HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);
}

static void dacDivision(void){
	spi3DataTx = spi3DataTx / 2;
	Spi3DacTran();
	bufferLen = sprintf(uart1BufferTx,"current DAC value is: %d\r", spi3DataTx);
	HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);
}


static void adf4159Spi1PllConfig(void){
//	MX_SPI1_BGT24_Init();
//		Spi1Bgt24Tran();
//		bufferLen = sprintf(uart1BufferTx, "BGT24 was configured\n\r");
//		HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);
	MX_SPI1_ADF4159_Init();
//	adf4159Spi1Tx(5, 3073829280); //only for test
//	adf4159Spi1Tx(PLL_REG_R7_DELAY, 1L << R7_SHIFT_TX_DATA_TRIGGER); 					//TX_DATA_TRIGGER enabled
//	adf4159Spi1Tx(PLL_REG_R6_STEP, 0L << R6_SHIFT_STEP_SEL);
//	adf4159Spi1Tx(PLL_REG_R6_STEP, 1L << R6_SHIFT_STEP_SEL); 							// second step register is not used
//	adf4159Spi1Tx(PLL_REG_R5_DEVIATION, 0L << R5_SHIFT_DEV_SEL);
//	adf4159Spi1Tx(PLL_REG_R5_DEVIATION, 1L << R5_SHIFT_DEV_SEL);							// second deviation register is not used
//	adf4159Spi1Tx(PLL_REG_R4_CLOCK, 0L << R4_SHIFT_CLK_DIV_SEL);
//	adf4159Spi1Tx(PLL_REG_R4_CLOCK, 1L << R4_SHIFT_CLK_DIV_SEL);
//	adf4159Spi1Tx(PLL_REG_R3_FUNCTION, (6L << R3_SHIFT_NEG_BLEED_CURRENT)				// negative bleed current 454.7 uA; but not actually enabled
//									| (1L << R3_SHIFT_RESERVED_1)						// reserved; must be 1
//									| (1L << R3_SHIFT_LOL)								// disable "loss of lock" feature; not using it makes things supposedly more robust
//									| (1L << R3_SHIFT_N_SEL)							// 1 = delay N word load by 4 cycles; makes transition between sweeps smoother
//									| (1L << R3_SHIFT_LDP)								// "lock detect precision"; writing 1makes things supposedly more robust
//									| (1L << R3_SHIFT_SIGMA_DELTA_RESET)				// 1 = disable sigma-delta reset on every write
//									| (0L << R3_SHIFT_PD_POL));							// phase detector polarity is negativ
//	adf4159Spi1Tx(PLL_REG_R2_R_DIVIDER, (7L << R2_SHIFT_CP_CURRENT_SETTING)				// CP current 2.5 mA
//									| (R2_REFERENCE_DIV2_VALUE << R2_SHIFT_RDIV2)
//									| (R2_REF_IN_DOUBLER_VALUE << R2_SHIFT_REF_DOUBLER)
//									| (R2_R_COUNTER_VALUE << R2_SHIFT_R_COUNTER)
//									| (0L << R2_SHIFT_CLK1_DIV_VALUE));

	adf4159Spi1Tx(PLL_REG_R1_LSB_FRAC, (0L << R1_SHIFT_LSB_FRACTIONAL_VALUE));			//Fraction LSB is 0 8200
	adf4159Spi1Tx(PLL_REG_R0_FRAC_INT, (23L << R0_SHIFT_INTEGER_VALUE)				//Int value is 375 18
									| (1792L << R0_SHIFT_MSB_FRACTIONAL_VALUE));		//Fraction MSB is 1792 3072 1788

	bufferLen = sprintf(uart1BufferTx, "PLL was configured\n\r");
	HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);

}

static void adf4159RampSet(void){

	MX_SPI1_ADF4159_Init();
//	adf4159Spi1Tx(5, 3073829280); //only for test
	adf4159Spi1Tx(PLL_REG_R7_DELAY, 1L << R7_SHIFT_TX_DATA_TRIGGER); 					//TX_DATA_TRIGGER enabled
	adf4159Spi1Tx(PLL_REG_R6_STEP, 150L << R6_SHIFT_STEP_WORD);//150
	adf4159Spi1Tx(PLL_REG_R6_STEP, 1L << R6_SHIFT_STEP_SEL); 							// second step register is not used
	adf4159Spi1Tx(PLL_REG_R5_DEVIATION, (26215L << R5_SHIFT_DEVIATION_WORD)//26215
										|(1L << R5_SHIFT_DEVIATION_OFFSET));//1
	adf4159Spi1Tx(PLL_REG_R5_DEVIATION, 1L << R5_SHIFT_DEV_SEL);							// second deviation register is not used
	adf4159Spi1Tx(PLL_REG_R4_CLOCK,  (1L << R4_SHIFT_LE_SEL)
									| (3L << R4_SHIFT_CLK_DIV_MODE)				// Ramp mode is selected
									| (CLK2_DIV_VALUE << R4_SHIFT_CLK2_DIV_VALUE));
//	adf4159Spi1Tx(PLL_REG_R4_CLOCK, 1L << R4_SHIFT_CLK_DIV_SEL);
	adf4159Spi1Tx(PLL_REG_R4_CLOCK, (1L << R4_SHIFT_LE_SEL)					// enable synchronization of LE pin; avoids timing problems in chip
									| (3L << R4_SHIFT_CLK_DIV_MODE)				// Clock is used for ramps
									| (CLK2_DIV_VALUE << R4_SHIFT_CLK2_DIV_VALUE)
									| (1L << R4_SHIFT_CLK_DIV_SEL));
	adf4159Spi1Tx(PLL_REG_R3_FUNCTION, (6L << R3_SHIFT_NEG_BLEED_CURRENT)				// negative bleed current 454.7 uA; but not actually enabled
									| (1L << R3_SHIFT_RESERVED_1)						// reserved; must be 1
									| (1L << R3_SHIFT_LOL)								// disable "loss of lock" feature; not using it makes things supposedly more robust
									| (1L << R3_SHIFT_N_SEL)							// 1 = delay N word load by 4 cycles; makes transition between sweeps smoother
									| (1L << R3_SHIFT_RAMP_MODE)
									| (1L << R3_SHIFT_LDP)								// "lock detect precision"; writing 1makes things supposedly more robust
									| (1L << R3_SHIFT_SIGMA_DELTA_RESET)				// 1 = disable sigma-delta reset on every write
									| (PD_POLARITY << R3_SHIFT_PD_POL));							// phase detector polarity is negativ
	adf4159Spi1Tx(PLL_REG_R2_R_DIVIDER, (7L << R2_SHIFT_CP_CURRENT_SETTING)				// CP current 2.5 mA
									| (R2_REFERENCE_DIV2_VALUE << R2_SHIFT_RDIV2)
									| (R2_REF_IN_DOUBLER_VALUE << R2_SHIFT_REF_DOUBLER)
									| (R2_R_COUNTER_VALUE << R2_SHIFT_R_COUNTER)
									| (640L << R2_SHIFT_CLK1_DIV_VALUE));//640
	adf4159Spi1Tx(PLL_REG_R1_LSB_FRAC, (0L << R1_SHIFT_LSB_FRACTIONAL_VALUE));			//Fraction LSB is 0 8200
	adf4159Spi1Tx(PLL_REG_R0_FRAC_INT, (1UL << R0_SHIFT_RAMP_ON)
										| (23L << R0_SHIFT_INTEGER_VALUE)				//Int value is 375 18
										| (1792L << R0_SHIFT_MSB_FRACTIONAL_VALUE));		//Fraction MSB is 1792 3072 1788


//		MX_SPI1_BGT24_Init();
//		Spi1Bgt24Tran();
//		bufferLen = sprintf(uart1BufferTx, "BGT24 was configured\n\r");
//		HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);

	bufferLen = sprintf(uart1BufferTx, "Ramp was setted\n\r");
	HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);
}

static void bgt24Spi1Config(void){
	MX_SPI1_BGT24_Init();
	Spi1Bgt24Tran();
	bufferLen = sprintf(uart1BufferTx, "BGT24 was configured\n\r");
	HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);
}

static void adf4159Spi1Tx(uint32_t adfAddress, uint32_t data){
	uint32_t data_address;
	data = data & 0xFFFFFFF8;
	adfAddress = adfAddress & 0x00000007;
	data_address = data | adfAddress;
	uint16_t dataTx[2]={data_address >> 16, data_address};
	//uint16_t data_msb = data_address >> 16;
	//uint16_t data_lsb = data_address;
	Spi1Adf4159Tran(dataTx);
}

static void Spi3DacTran(void){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, (uint8_t *)&spi3DataTx, 1, 5000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

static void Spi1Adf4159Tran(const uint16_t *data){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) data, 2, 5000);
	//HAL_SPI_Transmit(&hspi1, (uint8_t*) data_lsb, 1, 5000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
}

static void Spi1Bgt24Tran(void){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &spi1DataTxBgt, 1, 5000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}
