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

char get_key_char(unsigned int keypad){
    if(keypad & BOTAO_1){
        return '1';
    }
    if(keypad & BOTAO_2){
        return '2';
    }
    if(keypad & BOTAO_3){
        return '3';
    }
    if(keypad & BOTAO_4){
        return '4';
    }
    if(keypad & BOTAO_5){
        return '5';
    }
    if(keypad & BOTAO_6){
        return '6';
    }
    if(keypad & BOTAO_7){
        return '7';
    }
    if(keypad & BOTAO_8){
        return '8';
    }
    if(keypad & BOTAO_9){
        return '9';
    }
    if(keypad & BOTAO_0){
        return '0';
    }
    return 'x';

}

void controla_menu(unsigned int a_t1, unsigned char d_vibracao, unsigned char d_movimento, unsigned int keypad, unsigned char d_time, unsigned char* is_alarme_ativado){
    static unsigned char status_menu = 0;
    static unsigned char tempo_percorrido = 0;
    static unsigned char cursor = 1;
    char temp_buffer[16];
    float temperatura;
    if(d_vibracao){
        lcd_clear();
        tempo_percorrido = 0;
        status_menu = 0;
        cursor = 1;
        return;
    }

    if(*is_alarme_ativado){
        switch(status_menu){
            case 1:
                if(keypad != 0){
                    status_menu = 2;
                    cursor = 0;
                }
            break;
            case 2:
                if(tempo_percorrido == 0){
                    lcd_gotoxy(0,0);
                    lcd_putsf("DIGITE A SENHA:"); 
                    lcd_gotoxy(0,1);
                    tempo_percorrido ++;
                }

                if(keypad == 0){
                    tempo_percorrido++;
                    if(tempo_percorrido >= 230){
                        status_menu = 1;
                        cursor = 0;
                    }
                    break;
                }
                tempo_percorrido = 0;

                if(keypad & BOTAO_RETURN){
                    lcd_clear();
                    lcd_gotoxy(0,0);
                    lcd_putsf("DIGITE A SENHA:"); 
                    lcd_gotoxy(0,1);
                    cursor = 0;
                    break;
                }

                if(keypad & BOTAO_ENTER){
                    if(cursor == 4){
                        if(temp_buffer[0] == '0'){
                            if(temp_buffer[1] == '0'){
                                if(temp_buffer[2] == '0'){
                                    if(temp_buffer[3] == '0'){
                                        *is_alarme_ativado = 0;
                                        status_menu = 0;
                                        tempo_percorrido = 0;
                                        cursor = 1;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    status_menu = 1;
                    cursor = 0;
                    tempo_percorrido = 0;
                    break;
                }
                
                temp_buffer[cursor] = get_key_char();

                if(temp_buffer[cursor] == 'x'){
                    break;
                }
                lcd_putchar(temp_buffer[cursor]);
                if(cursor<15){cursor++;}
            break;
        }
        

        return;
    }

    switch(status_menu){
        case MENU_LOAD:
            tempo_percorrido ++;
            if(tempo_percorrido == 0){
                inicia_menu();
            }
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
                cursor = 1;
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
                        lcd_putsf("-> MOTOR(°C)");
                        lcd_gotoxy(0,1);
                        lcd_putsf("ATIVAR ALARME"); 
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
             if(keypad & BOTAO_RETURN){
                status_menu = MENU_NORMAL;
                tempo_percorrido = 0;
                break;
            }

            if(tempo_percorrido == 0){
                lcd_gotoxy(0,0);
                lcd_putsf("ENTER PARA");
                lcd_gotoxy(0,1);
                lcd_putsf("ATIVAR ALARME");
                tempo_percorrido = 1;
            }

            if((tempo_percorrido == 1) && (keypad & BOTAO_ENTER)){
                lcd_gotoxy(0,0);
                lcd_putsf("ALARME ATIVADO");
                tempo_percorrido ++;
            }

            if(tempo_percorrido >=2){
                if(tempo_percorrido >=230){
                    *is_alarme_ativado = 1; 
                    lcd_clear();
                    tempo_percorrido = 0;
                    status_menu = 1;
                    cursor = 1;
                }
                tempo_percorrido++;
            }
        break;
    }
}

