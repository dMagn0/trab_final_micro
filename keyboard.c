
#include <mega16.h>

typedef unsigned char byte;
// store here every key state as a bit,
// bit 0 will be KEY0, bit 1 KEY1,...
unsigned keys;

void init_keypad(void)
{
    DDRC=0xf0;
    PORTC=0xff;
    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: 57.600 kHz
    // Mode: Normal top=FFh
    // OC0 output: Disconnected
    //TCCR0=0x03;
    //INIT_TIMER0;
    TCCR0=0x04;
    TCNT0=0x8D;
    OCR0=0x00;

    // External Interrupts are off
    //MCUCR=0x00;
    //EMCUCR=0x00;
    // Timer 0 overflow interrupt is on
    //TIMSK=0x02;
    // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=0x01;
    #asm("sei")
}

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
static byte key_pressed_counter=10;
static byte key_released_counter,column=FIRST_COLUMN;
static unsigned row_data,crt_key;
// Reinitialize Timer 0 value
TCNT0 = 0x8D; // para 2ms
// Place your code here
row_data <<= 4;
// get a group of 4 keys in in row_data
row_data |= ~KEYIN&0xf;
column >>= 1;
if (column == (LAST_COLUMN >> 1))
{
    column = FIRST_COLUMN;
    if (row_data == 0) 
    {
        goto new_key;
    }
    if (key_released_counter)
    { 
        --key_released_counter;
    }
    else
    {
        if (--key_pressed_counter == 9) 
        {
            crt_key = row_data;
        }
        else
        {
            if(row_data != crt_key)
            {
new_key:
                key_pressed_counter = 10;
                key_released_counter = 0;
                goto end_key;
            }
            if(!key_pressed_counter)
            {
                keys = row_data;
                key_released_counter = 20;
            }
         }
    }
end_key:
   row_data = 0;
}
// select next column, inputs will be with pull-up
KEYOUT = ~column;
}

unsigned inkey(void)
{
    unsigned k;
    if (k=keys) 
        keys=0;
    return k;
}
