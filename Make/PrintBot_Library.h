#define BAUD 9600UL	//baudrate
#define UBRR_VALUE ((F_CPU / (8UL * BAUD) ) - 1UL) //Asynchronous Double Speed mode (U2Xn = 1)

void UART_init()
{
		UBRR0 = UBRR_VALUE;		//UBRRL_VALUE;
		
		UCSR0B = (1<<TXEN0)|(1<<RXEN0);		//Enable receiver and transmitter
		//UCSR0B |= (1<<RXCIE0); //receive interrupt enabled
		UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);	// Set frame format: 8bit data, 1stop bit, Parity disabled
		UCSR0A |= (1 << U2X0);				//U2X-Modus: Verdopplung der Transferrate von 4800 auf 9600
		UCSR0B |= (1<<RXCIE0);				//enable Receive Complete Interrupt
		sei();								//enable Global Interrupt							
}

void UART_Transmit(unsigned char data)
{
	while (!(UCSR0A & (1<<UDRE0)));	//Wait for empty transmit buffer
	_delay_ms(5);
	UDR0 = data;				//Put data into buffer and sends the data
}

void UART_Transmit_String(unsigned char string[])
{
    while (*string)
    {
        UART_Transmit(*string);
        string++;
    }
}

ISR(USART0_RX_vect)
{
	UART_Transmit(UDR0 + 1);
}

void Timer0_init()		//8-Bit Timer
{
	DDRB |= (1<<PB3);					//OC0A
	DDRB |= (1<<PB4);					//OC0B
	TCCR0A = (1<<COM0A1) | (1<<COM0B1);	//clear OC0A on Compare Match
	TCCR0A |= (1<<WGM01) | (1<<WGM00);	//8-bit Fast-PWM
	TCCR0B = (1<<CS00);					//no prescaling
}

void Timer1_init()		//16-Bit Timer
{
	DDRD |= (1<<PD5);					//OC1A
	DDRD |= (1<<PD4);					//OC1B
	TCCR1A = (1<<COM1A1) | (1<<COM1B1);	//clear OC0A on Compare Match
	TCCR1A |= (1<<WGM11) | (1<<WGM10);	//10-bit Phase-Correct-PWM --> MAX = 0x3FF = 1023
	TCCR1B = (1<<CS00);					//no prescaling
}

void Timer2_init()		//8-Bit Timer
{
	DDRD |= (1<<PD7);					//OC2A
	DDRD |= (1<<PD6);					//OC2B
	TCCR2A = (1<<COM2A1) | (1<<COM2B1);	//clear OC0A on Compare Match
	TCCR2A |= (1<<WGM21) | (1<<WGM20);	//8-bit Fast-PWM
	TCCR2B = (1<<CS20);					//no prescaling
}

uint8_t ADC_Value(unsigned int channel)
{
	ADCSRA = 5;					//Prescaler 32
	ADMUX = (1<<REFS0);			//AVCC = reference, external capacitor at AREF
	ADMUX |= (1<<ADLAR);		//left adjust the result
	ADMUX |= channel;			//channel 5 = AD5 = PA5
	ADCSRA |= (1<<ADEN);		//enable ADC
	ADCSRA |= (1<<ADSC);		//start measurement
    while (ADCSRA & (1<<ADSC));	//Warten, bis ADSC wieder 0 ist
	return ADCH;				//Messung auslesen
}

void PrintBot_Library_Initialization()
{
	UART_init();
	Timer0_init();
	Timer2_init();
	DDRA |= (1<<PA4) | (1<<PA5) | (1<<PA6) | (1<<PA7);
}