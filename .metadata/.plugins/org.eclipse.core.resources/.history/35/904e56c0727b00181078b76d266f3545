#include "stm32f4xx_hal.h"
#include "uartPrint.h"

extern UART_HandleTypeDef huart1;

char uart1BufferTx[100];			//the Buffer of printf
uint8_t uart1BufferLen;

void uartPrintEnter(void)
{
	uart1BufferLen = sprintf(uart1BufferTx, "\n\r");
	HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, uart1BufferLen, 1000);
}

void uartPrintString(char* string)
{
	uart1BufferLen = sprintf(uart1BufferTx, string);
	HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, uart1BufferLen, 1000);
}

void uartPrintMenu(const struct rs232_menu *menu)
{
	uart1BufferLen = sprintf(uart1BufferTx, "%s\n\r", menu->menuName);
	HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, uart1BufferLen, 1000);
}

void uartPrintMenuOption(const struct rs232_menu *menu, int i)
{
	uart1BufferLen = sprintf(uart1BufferTx, "%d: %s\n\r", i + 1,
			menu->menuOption[i]);
	HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, uart1BufferLen, 1000);
}

void uartPrintNumber(long int number)
{
	uart1BufferLen = sprintf(uart1BufferTx, "%d\n\r", number);
	HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, uart1BufferLen, 1000);
}

void uartPrintDacValue(uint16_t dacValue)
{
	uart1BufferLen = sprintf(uart1BufferTx, "current DAC value is: %d\r",
			dacValue);
	HAL_UART_Transmit(&huart1, (uint8_t *) uart1BufferTx, uart1BufferLen, 1000);
}
