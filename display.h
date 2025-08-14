#ifndef DISPLAY_H
#define DISPLAY_H

#define MENU_LOAD 0
#define MENU_NORMAL 1
#define MENU_TEMP 2
#define MENU_ALARME 3

#define CONST_A -200.0
#define CONST_B 1024.0 
#define CONST_C 120.0

void inicia_menu();
void controla_menu(unsigned int, unsigned char, unsigned char, unsigned int, unsigned char, unsigned char*);
char get_key_char(unsigned int keypad);

#endif