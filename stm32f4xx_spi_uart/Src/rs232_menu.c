/*
 * a menu through RS232
 * Author: Min Bao
 * Datum: 14.02.2018
 */

#include "rs232_menu.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "ad5641.h"
#include "adf4159.h"
#include "bgt24mtr11.h"

//interrupt variables
static char uart1DataRx;			//receive interrupt Bit
static uint8_t uart1TxCplt;			//Interrupt complete Bit

//uart print variables
char uart1BufferTx[100];			//the Buffer of printf
uint8_t bufferLen;

extern UART_HandleTypeDef huart1;

static uint8_t optionSelect;
static uint16_t spi3DataTx = DAC_START;


struct rs232_menu{
	char menuName[50];
	int menuLength;
	char *menuOption[10];
};

static struct rs232_menu mainMenu = {"Main Menu",
	2,
	{"DAC value setting",
	"HF Board configuration"}
};

static struct rs232_menu ad5641Menu = {"DAC value setting",
	5,
	{"+1",
	 "-2",
	 "x2",
	 "/2",
	 "back"
	}
};

static struct rs232_menu hfBoardMenu = {"HF Board configuration",
	4,
	{"Ramp mode on",
	 "BGT24 Power up",
	 "BGT24 Power down",
	 "back"
	}
};



//Main menu
static void mainMenuExcution(const struct rs232_menu *menu);

/*Submenu*/
static void ad5641MenuExcution(const struct rs232_menu *menu);
static void hfBoardMenuExcution(const struct rs232_menu *menu);


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

	do {
		//print function through uart1, print Menu Name and options
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

		HAL_UART_Receive_IT(&huart1, (uint8_t *) &uart1DataRx, 1); 				//active the uart1 to accept interrupt
		while (!uart1TxCplt) {
		};																		//stop the program until the interrupt is accepted

		optionSelect = uart1DataRx - '0';										//convert the char to int
		uart1TxCplt = 0;														//reset transfer complete variable

		switch (optionSelect) {
		case 1:
			ad5641MenuExcution(&ad5641Menu);
			break;
		case 2:
			hfBoardMenuExcution(&hfBoardMenu);
		}
	} while (optionSelect < 1 || optionSelect > menu->menuLength);
}

static void ad5641MenuExcution(const struct rs232_menu *menu) {
	do {
		//print options through uart1
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
			HAL_UART_Receive_IT(&huart1, (uint8_t *) &uart1DataRx, 1); 		//active the uart1 to accept interrupt
			while (!uart1TxCplt) {};										//stop the program until the interrupt is accepted

			optionSelect = uart1DataRx - '0';								//convert the char to int
			uart1TxCplt = 0;												//reset transfer complete variable

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
	} while (optionSelect < 1 || optionSelect > menu->menuLength);
}

static void hfBoardMenuExcution(const struct rs232_menu *menu) {
	bgt24Init();	//initial the spi interface of bgt24mtr11, PA and 64k prescaler disabled
	adf4159Init();
	do {
		//print options through uart1
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
			HAL_UART_Receive_IT(&huart1, (uint8_t *) &uart1DataRx, 1); 	//active the uart1 to accept interrupt
			while (!uart1TxCplt) {};									//stop the program until the interrupt is accepted

			optionSelect = uart1DataRx - '0';							//convert the char to int
			uart1TxCplt = 0;
			switch (optionSelect) {
			case 1:
				adf4159RampOn();
				break;
			case 2:
				bgt24PowerOn();
				break;
			case 3:
				bgt24PowerDown();
				break;
			case 4:
				mainMenuExcution(&mainMenu);
			}
		} while (optionSelect > 0 && optionSelect <= menu->menuLength);
	} while (optionSelect < 1 || optionSelect > menu->menuLength);
}
