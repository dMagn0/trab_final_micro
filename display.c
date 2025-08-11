#include <mega16.h>
#include <alcd.h>
#include "keyboard.h"
#include <stdio.h>    


void inicia_menu()
{
    lcd_gotoxy(0,0);
    lcd_putsf("DISPLAY ");
    lcd_gotoxy(0,1);
    lcd_putsf("DO CARRO");
}
void controla_menu(unsigned int a_t1, unsigned char d_vibracao, unsigned char d_movimento, unsigned int keypad, unsigned char d_time){
    static unsigned char status_menu = 0;
    static unsigned char tempo_percorrido = 0;
    static unsigned char cursor = 1;
    char temp_buffer[16];
    float temperatura;
    switch(status_menu){
        case MENU_LOAD:
            tempo_percorrido ++;
            if(tempo_percorrido > 40){
                status_menu = MENU_NORMAL;
                tempo_percorrido = 0;
            }
        break;
        case MENU_NORMAL:
            if(tempo_percorrido == 0){
                lcd_gotoxy(0,0);
                lcd_putsf("-> MOTOR(°C)");
                lcd_gotoxy(0,1);
                lcd_putsf("ATIVAR ALARME"); 
                tempo_percorrido ++;
            }
            switch(cursor){
                case 1:
                    if(keypad & BOTAO_BAIXO){
                        cursor = 2;
                        lcd_gotoxy(0,0);
                        lcd_putsf("MOTOR(°C)");
                        lcd_gotoxy(0,1);
                        lcd_putsf("-> ATIVAR ALARME"); 
                    }else if(keypad & BOTAO_ENTER){
                        status_menu = MENU_TEMP;
                        tempo_percorrido = 0;
                    }
                break;
                case 2:
                    if(keypad & BOTAO_CIMA){
                        cursor = 1;
                        lcd_gotoxy(0,0);
                        lcd_putsf("MOTOR(°C)");
                        lcd_gotoxy(0,1);
                        lcd_putsf("-> ATIVAR ALARME"); 
                    }else if(keypad & BOTAO_ENTER){
                        status_menu = MENU_ALARME;
                        tempo_percorrido = 0;
                    }
                break;
            }
        break;
        case MENU_TEMP:
            if(keypad & BOTAO_RETURN){
                status_menu = MENU_NORMAL;
                tempo_percorrido = 0;
                break;
            }

            if(tempo_percorrido == 0){
                lcd_gotoxy(0,0);
                lcd_putsf("TEMP DO MOTOR:");
                tempo_percorrido = 101;
            }
            if(tempo_percorrido >= 101){
                temperatura = ((float)a_t1 * (CONST_A/CONST_B)) + CONST_C;
                sprintf(temp_buffer, "%.2f", temperatura); 
                lcd_gotoxy(0,1);
                lcd_putsf(temp_buffer); 
                lcd_putchar(0xDF); 
                lcd_putchar('C');
                tempo_percorrido = 1;  
            }
            tempo_percorrido ++;
        break;
        case MENU_ALARME: 
            if(tempo_percorrido == 0){
                lcd_gotoxy(0,0);
                lcd_putsf("TEMP DO MOTOR:");
                tempo_percorrido = 101;
            }
        break;
    }
}

