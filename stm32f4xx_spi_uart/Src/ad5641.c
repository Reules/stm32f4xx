#include "ad5641.h"
#include "stm32f4xx_hal.h"
#include "uartPrint.h"

extern SPI_HandleTypeDef hspi3;

uint16_t spi3DataTx = DAC_START;

void dacInit(void){
	Spi3DacTran();
	uartPrintDacValue(spi3DataTx);
}

void dacAddition(void){
	spi3DataTx = spi3DataTx + 1;
	if(spi3DataTx > DAC_MAX){
		uartPrintString("out of range!\n\r");
		spi3DataTx = spi3DataTx - 1;
	}
	Spi3DacTran();
	uartPrintDacValue(spi3DataTx);
}

void dacSubtraction(void){
	spi3DataTx = spi3DataTx - 1;
	if (spi3DataTx < DAC_MIN || spi3DataTx > DAC_MAX) {
		uartPrintString("out of range!\n\r");
		spi3DataTx = spi3DataTx + 1;
	}
	Spi3DacTran();
	uartPrintDacValue(spi3DataTx);
}

void dacMultiplication(void){
	spi3DataTx = spi3DataTx * 2;
	if (spi3DataTx > DAC_MAX) {
		uartPrintString("out of range!\n\r");
		spi3DataTx = spi3DataTx / 2;
	}
	Spi3DacTran();
	uartPrintDacValue(spi3DataTx);
}

void dacDivision(void){
	spi3DataTx = spi3DataTx / 2;
	Spi3DacTran();
	uartPrintDacValue(spi3DataTx);
}

void turnDacUp(void){
	spi3DataTx = spi3DataTx + 20;
	if(spi3DataTx > DAC_MAX){
		uartPrintString("out of range!\n\r");
		spi3DataTx = spi3DataTx - 20;
	}
	Spi3DacTran();
	uartPrintDacValue(spi3DataTx);
}

void turnDacDown(void){
	spi3DataTx = spi3DataTx - 20;
	if(spi3DataTx > DAC_MAX){
		uartPrintString("out of range!\n\r");
		spi3DataTx = spi3DataTx + 20;
	}
	Spi3DacTran();
	uartPrintDacValue(spi3DataTx);
}

void Spi3DacTran(void){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, (uint8_t *)&spi3DataTx, 1, 5000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
