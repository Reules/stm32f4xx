#include "ad5641.h"
#include "stm32f4xx_hal.h"
#include "main.h"

extern SPI_HandleTypeDef hspi3;
extern UART_HandleTypeDef huart1;

uint16_t spi3DataTx = DAC_START;
extern uint8_t bufferLen;
extern char uart1BufferTx[100];

void dacAddtion(void){
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

void dacSubtraction(void){
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

void dacMultiplication(void){
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

void dacDivision(void){
	spi3DataTx = spi3DataTx / 2;
	Spi3DacTran();
	bufferLen = sprintf(uart1BufferTx,"current DAC value is: %d\r", spi3DataTx);
	HAL_UART_Transmit(&huart1, (uint8_t *)uart1BufferTx, bufferLen, 1000);
}

void Spi3DacTran(void){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, (uint8_t *)&spi3DataTx, 1, 5000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
