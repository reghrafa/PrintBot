#define F_CPU 12000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "PrintBot_Library.h"


main()
{
       PrintBot_Library_Initialization();

       while (1)
       {
              DDRA |= (1<<PA3);
              PORTA ^= (1<<PA3);
              _delay_ms(1000);
       }

       while (1);
}
