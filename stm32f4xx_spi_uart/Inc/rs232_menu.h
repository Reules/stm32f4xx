/*
 * a menu through RS232
 * Author: Min Bao
 * Datum: 14.02.2018
 */

#ifndef _RSMENU_H
#define _RSMENU_H

struct rs232_menu{
	char menuName[50];
	int menuLength;
	char *menuOption[10];
};

void rs232_menu(void);

#endif
