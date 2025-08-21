/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.0 Advanced
Automatic Program Generator
� Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 31/07/2025
Author  : www.Eca.ir *** www.Webkade.ir
Company : 
Comments: 


Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 14,745600 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*****************************************************/

#include <mega16.h>
#include <delay.h>
#include "keyboard.h"
#include "keyboard.c"
#include "display.h"
#include "display.c"

// Alphanumeric LCD Module functions
#include <alcd.h>

// Standard Input/Output functions
#include <stdio.h>
/*
  PA2 = temp_ky013
  PD2(INT0) = vibracao digital (batida)
  PD3(INT1) = movimento (alarme)
  PB  = LCD 16
  PC  = teclado
  PA4 = LED BATIDA
  PA5 = LED TEMP ALTA
  PA6 = COOLER
  PA7 = BUZZER
*/
#define ADC_VREF_TYPE 0b01000000

/*entrada anaogica*/
#define MUX_TEMP_1 2    //PA2 -> ativacao para valor < 300

/*entrada digital*/
#define VIBRACAO_IN PIND.2 // INT0
#define MOVIMENTO_IN PIND.3 // INT1


#define DELTA_T 10

char int_vibracao = 0;
interrupt [EXT_INT0] void interrupt_de_vibracao(void)
{
  int_vibracao = 1;
}
char int_movimento = 0;
interrupt [EXT_INT1] void interrupt_de_movimento(void)
{
  int_movimento = 1;
}

unsigned int read_adc(unsigned char adc_input)
{
  ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
  delay_us(2);
  ADCSRA|=0x40;
  while ((ADCSRA & 0x10)==0);
  ADCSRA|=0x10;
  return ADCW;
}

void atualiza_entrada(unsigned int *a_t1, unsigned char *d_vibracao, unsigned char *d_movimento, unsigned int *keypad)
{
  *a_t1 = read_adc(MUX_TEMP_1);
  *d_vibracao = int_vibracao;
  int_vibracao = 0;
  *d_movimento = int_movimento;
  int_movimento = 0;
  *keypad = inkey();
}

void ativa_buzzer() {
    PORTA.7 = 0;
}

void desativa_buzzer() {
    PORTA.7 = 1;
}

void ativa_led_batida() {
    PORTA.4 = 0;
}

void desativa_led_batida() {
    PORTA.4 = 1;
}

void ativa_cooler() {
    PORTA.6 = 0;
}

void desativa_cooler() {
    PORTA.6 = 1;
}

void ativa_led_temperatura_alta() {
    PORTA.5 = 0;
}

void desativa_led_temperatura_alta() {
    PORTA.5 = 1;
}




void main(void)
{    

  unsigned int a_t1, keypad;
  unsigned char d_vibracao, d_movimento, is_alarme_ativado;
  is_alarme_ativado = 0;

  // Input/Output Ports initialization
  // Port A initialization
  // Func7=IN Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State7=0 State6=0 State5=0 State4=T State3=T State2=T State1=T State0=T 
  DDRA=0xf0;
  PORTA=0x80;

  // Port B initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
  PORTB=0x00;
  DDRB=0x00;

  // Port C initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
  PORTC=0x00;
  DDRC=0x00;

  // Port D initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
  PORTD=0x00;
  DDRD=0x00;

  // Timer/Counter 0 initialization
  // Clock source: System Clock
  // Clock value: Timer 0 Stopped
  // Mode: Normal top=0xFF
  // OC0 output: Disconnected
  TCCR0=0x00;
  TCNT0=0x00;
  OCR0=0x00;

  // Timer/Counter 1 initialization
  // Clock source: System Clock
  // Clock value: Timer1 Stopped
  // Mode: Normal top=0xFFFF
  // OC1A output: Discon.
  // OC1B output: Discon.
  // Noise Canceler: Off
  // Input Capture on Falling Edge
  // Timer1 Overflow Interrupt: Off
  // Input Capture Interrupt: Off
  // Compare A Match Interrupt: Off
  // Compare B Match Interrupt: Off
  TCCR1A=0x00;
  TCCR1B=0x00;
  TCNT1H=0x00;
  TCNT1L=0x00;
  ICR1H=0x00;
  ICR1L=0x00;
  OCR1AH=0x00;
  OCR1AL=0x00;
  OCR1BH=0x00;
  OCR1BL=0x00;

  // Timer/Counter 2 initialization
  // Clock source: System Clock
  // Clock value: Timer2 Stopped
  // Mode: Normal top=0xFF
  // OC2 output: Disconnected
  ASSR=0x00;
  TCCR2=0x00;
  TCNT2=0x00;
  OCR2=0x00;

  // External Interrupt(s) initialization
  // INT0: On
  // INT1: Off
  // INT2: Off

  GICR = 0xC0;

  MCUCR=0x06;
  MCUCSR=0x00;

  // Timer(s)/Counter(s) Interrupt(s) initialization
  TIMSK=0x00;

  // USART initialization
  // Communication Parameters: 8 Data, 1 Stop, No Parity
  // USART Receiver: On
  // USART Transmitter: On
  // USART Mode: Asynchronous
  // USART Baud Rate: 19200
  UCSRA=0x00;
  UCSRB=0x18;
  UCSRC=0x86;
  UBRRH=0x00;
  UBRRL=0x2F;

  // Analog Comparator initialization
  // Analog Comparator: Off
  // Analog Comparator Input Capture by Timer/Counter 1: Off
  ACSR=0x80;
  SFIOR=0x00;

  // ADC initialization
  // ADC Clock frequency: 230,400 kHz
  // ADC Voltage Reference: AREF pin
  // ADC Auto Trigger Source: ADC Stopped
  ADMUX=ADC_VREF_TYPE & 0xff;
  ADCSRA=0x86;

  // SPI initialization
  // SPI disabled
  SPCR=0x00;

  // TWI initialization
  // TWI disabled
  TWCR=0x00;

  // Alphanumeric LCD initialization
  // Connections specified in the
  // Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
  // RS - PORTB Bit 0
  // RD - PORTB Bit 1
  // EN - PORTB Bit 2
  // D4 - PORTB Bit 4
  // D5 - PORTB Bit 5
  // D6 - PORTB Bit 6
  // D7 - PORTB Bit 7
  // Characters/line: 16
  #asm("sei") ;
  lcd_init(16);
  init_keypad();
  desativa_buzzer();
  desativa_led_batida();
  desativa_cooler();
  desativa_led_temperatura_alta();

  
  while (1){
    atualiza_entrada( &a_t1, &d_vibracao, &d_movimento, &keypad);

    controla_menu(a_t1, d_vibracao, d_movimento, keypad, DELTA_T, &is_alarme_ativado);
    
    if(d_vibracao){
      desativa_cooler();
      desativa_led_temperatura_alta();
      ativa_buzzer();
      ativa_led_batida();
      break;
    }
    
    if((d_movimento) && (is_alarme_ativado)){
      ativa_buzzer();
    }
    if(!is_alarme_ativado){
      desativa_buzzer();
    }
    
    if(a_t1<=230){
      ativa_cooler();
        if(a_t1<=300){
          ativa_led_temperatura_alta();
        }else{
          desativa_led_temperatura_alta();
        }
    }else{
      desativa_cooler();
    }
    


    delay_ms(DELTA_T);  
  }
}
  //  if (keypad & BOTAO_1) {
  //   printf("1");
  //   }
  //   if (keypad & BOTAO_2) {
  //       printf("2");
  //   }
  //   if (keypad & BOTAO_3) {
  //       printf("3");
  //   }
  //   if (keypad & BOTAO_4) {
  //       printf("4");
  //   }
  //   if (keypad & BOTAO_5) {
  //       printf("5");
  //   }
  //   if (keypad & BOTAO_6) {
  //       printf("6");
  //   }
  //   if (keypad & BOTAO_7) {
  //       printf("7");
  //   }
  //   if (keypad & BOTAO_8) {
  //       printf("8");
  //   }
  //   if (keypad & BOTAO_9) {
  //       printf("9");
  //   }
  //   if (keypad & BOTAO_CIMA) {
  //       printf("CIMA");
  //   }
  //   if (keypad & BOTAO_BAIXO) {
  //       printf("BOTAO_BAIXO");
  //   }
  //   if (keypad & BOTAO_ENTER) {
  //       printf("BOTAO_ENTER");
  //   }
  //   if (keypad & BOTAO_RETURN) {
  //       printf("BOTAO_RETURN");
  //   }
  /*
    MOTOR(°C)
    ATIVAR ALARME
  */
  /*
    DESATIVAR ALARME
  */

  // printf("\r\nentradas digitais:\r\n",);
  // printf("    vibracao: %d\r\n",d_vibracao);
  // printf("    movimento: %d\r\n",d_movimento);
  // printf("entradas analogicas:\r\n");
  // printf("    temperatura ky13: %d\r\n",a_t1);
  // printf("    keypad: %d\r\n",keypad);
  // lcd_gotoxy(0,0);
  // lcd_putsf("DISPLAY ");
  // lcd_gotoxy(0,1);
  // lcd_putsf("DO CARRO");
  // delay_ms(400);
  // lcd_gotoxy(0,0);
  // lcd_putsf("-> MOTOR(°C)");
  // lcd_gotoxy(0,1);
  // lcd_putsf("ATIVAR ALARME");
  // delay_ms(200);
  // switch(){
  //   case MENU_1:
  //     if(keypad & BOTAO_CIMA){

  //     }
  //   break;
  //   case MENU_2:
  //   break;
