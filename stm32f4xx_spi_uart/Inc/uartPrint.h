#ifndef UART_PRINT_H
#define UART_PRINT_H

#include "rs232_menu.h"

void uartPrintEnter(void);
void uartPrintString(char* string);
void uartPrintMenu(const struct rs232_menu *menu);
void uartPrintMenuOption(const struct rs232_menu *menu, int i);
void uartPrintNumber(long int number);
void uartPrintDacValue(uint16_t dacValue);

#endif
