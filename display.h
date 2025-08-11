#ifndef DISPLAY_H
#define DISPLAY_H

#define MENU_LOAD 0
#define MENU_NORMAL 1
#define MENU_TEMP 2
#define MENU_ALARME 3
#define CONST_A -1
#define CONST_B 6 
#define CONST_C 100
void inicia_menu();
void controla_menu(unsigned int, unsigned char, unsigned char, unsigned int, unsigned char);

#endif