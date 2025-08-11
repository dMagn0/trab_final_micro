
// KEYBOARD.h
#ifndef KEYBOARD_H
#define KEYBOARD_H

// PINC0..3 will be row inputs
#define KEYIN PINC
// PORTC4..7 will be column outputs
#define KEYOUT PORTC
#define FIRST_COLUMN 0x80
#define LAST_COLUMN 0x10

/*botoes da matrix*/
#define BOTAO_1 0x0001
#define BOTAO_2 0x0010
#define BOTAO_3 0x0100
#define BOTAO_4 0x0002
#define BOTAO_5 0x0020
#define BOTAO_6 0x0200
#define BOTAO_7 0x0004
#define BOTAO_8 0x0040
#define BOTAO_9 0x0400
#define BOTAO_0 0x0080
#define BOTAO_CIMA 0x1000
#define BOTAO_BAIXO 0x2000
#define BOTAO_ENTER 0x0008
#define BOTAO_RETURN 0x0800


void init_keypad(void);
unsigned inkey(void);

#endif
