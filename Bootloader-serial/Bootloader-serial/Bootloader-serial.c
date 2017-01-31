/*
 * Bootloader-serial.c
 *
 * Created: 24/01/2017 21:50:49
 * Author : Fabian
 */ 

//Other Linker Flags: -Wl,--section-start=.text=0x1800
#define F_CPU 8000000UL
#define __AVR_ATmega8__
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/boot.h>
#include <util/delay.h>
#include "uart.h"

#define BOOT_UART_BAUD_RATE     2400     //Baudrate
#define XON                     17       //XON Zeichen
#define XOFF                    19       //XOFF Zeichen
#define START_SIGN              ':'      //Hex-Datei Zeilenstartzeichen

//Zustände des Bootloader-Programms
#define BOOT_STATE_EXIT	        0
#define BOOT_STATE_PARSER       1

//Zustände des Hex-File-Parsers
#define PARSER_STATE_START      0
#define PARSER_STATE_SIZE       1
#define PARSER_STATE_ADDRESS    2
#define PARSER_STATE_TYPE       3
#define PARSER_STATE_DATA       4
#define PARSER_STATE_CHECKSUM   5
#define PARSER_STATE_ERROR      6

#define IRTX        (1 << PB3)

////////////////Title:    Interrupt UART library with receive/transmit circular buffers
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

/*
 *  constants and macros
 */

/* size of RX/TX buffers */
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1)
#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1)

#if ( UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK )
#error RX buffer size is not a power of 2
#endif
#if ( UART_TX_BUFFER_SIZE & UART_TX_BUFFER_MASK )
#error TX buffer size is not a power of 2
#endif

#if defined(__AVR_AT90S2313__) \
 || defined(__AVR_AT90S4414__) || defined(__AVR_AT90S4434__) \
 || defined(__AVR_AT90S8515__) || defined(__AVR_AT90S8535__) \
 || defined(__AVR_ATmega103__)
 /* old AVR classic or ATmega103 with one UART */
 #define AT90_UART
 #define UART0_RECEIVE_INTERRUPT   UART_RX_vect 
 #define UART0_TRANSMIT_INTERRUPT  UART_UDRE_vect
 #define UART0_STATUS   USR
 #define UART0_CONTROL  UCR
 #define UART0_DATA     UDR  
 #define UART0_UDRIE    UDRIE
#elif defined(__AVR_AT90S2333__) || defined(__AVR_AT90S4433__)
 /* old AVR classic with one UART */
 #define AT90_UART
 #define UART0_RECEIVE_INTERRUPT   UART_RX_vect 
 #define UART0_TRANSMIT_INTERRUPT  UART_UDRE_vect
 #define UART0_STATUS   UCSRA
 #define UART0_CONTROL  UCSRB
 #define UART0_DATA     UDR 
 #define UART0_UDRIE    UDRIE
#elif  defined(__AVR_ATmega8__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) \
  || defined(__AVR_ATmega323__)
  /* ATmega with one USART */
 #define ATMEGA_USART
 #define UART0_RECEIVE_INTERRUPT   USART_RXC_vect
 #define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
 #define UART0_STATUS   UCSRA
 #define UART0_CONTROL  UCSRB
 #define UART0_DATA     UDR
 #define UART0_UDRIE    UDRIE
#elif defined (__AVR_ATmega8515__) || defined(__AVR_ATmega8535__)
 #define ATMEGA_USART
 #define UART0_RECEIVE_INTERRUPT   USART_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
 #define UART0_STATUS   UCSRA
 #define UART0_CONTROL  UCSRB
 #define UART0_DATA     UDR
 #define UART0_UDRIE    UDRIE
#elif defined(__AVR_ATmega163__)
  /* ATmega163 with one UART */
 #define ATMEGA_UART
 #define UART0_RECEIVE_INTERRUPT   UART_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  UART_UDRE_vect
 #define UART0_STATUS   UCSRA
 #define UART0_CONTROL  UCSRB
 #define UART0_DATA     UDR
 #define UART0_UDRIE    UDRIE
#elif defined(__AVR_ATmega162__) 
 /* ATmega with two USART */
 #define ATMEGA_USART0
 #define ATMEGA_USART1
 #define UART0_RECEIVE_INTERRUPT   USART0_RXC_vect
 #define UART1_RECEIVE_INTERRUPT   USART1_RXC_vect
 #define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
 #define UART1_TRANSMIT_INTERRUPT  USART1_UDRE_vect
 #define UART0_STATUS   UCSR0A
 #define UART0_CONTROL  UCSR0B
 #define UART0_DATA     UDR0
 #define UART0_UDRIE    UDRIE0
 #define UART1_STATUS   UCSR1A
 #define UART1_CONTROL  UCSR1B
 #define UART1_DATA     UDR1
 #define UART1_UDRIE    UDRIE1
#elif defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) 
 /* ATmega with two USART */
 #define ATMEGA_USART0
 #define ATMEGA_USART1
 #define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
 #define UART1_RECEIVE_INTERRUPT   USART1_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
 #define UART1_TRANSMIT_INTERRUPT  USART1_UDRE_vect
 #define UART0_STATUS   UCSR0A
 #define UART0_CONTROL  UCSR0B
 #define UART0_DATA     UDR0
 #define UART0_UDRIE    UDRIE0
 #define UART1_STATUS   UCSR1A
 #define UART1_CONTROL  UCSR1B
 #define UART1_DATA     UDR1
 #define UART1_UDRIE    UDRIE1
#elif defined(__AVR_ATmega161__)
 /* ATmega with UART */
 #error "AVR ATmega161 currently not supported by this libaray !"
#elif defined(__AVR_ATmega169__) 
 /* ATmega with one USART */
 #define ATMEGA_USART
 #define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
 #define UART0_STATUS   UCSRA
 #define UART0_CONTROL  UCSRB
 #define UART0_DATA     UDR
 #define UART0_UDRIE    UDRIE
#elif defined(__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) \
 || defined(__AVR_ATmega3250__) || defined(__AVR_ATmega3290__) ||defined(__AVR_ATmega6450__) || defined(__AVR_ATmega6490__)
 /* ATmega with one USART */
 #define ATMEGA_USART0
 #define UART0_RECEIVE_INTERRUPT   USART_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
 #define UART0_STATUS   UCSR0A
 #define UART0_CONTROL  UCSR0B
 #define UART0_DATA     UDR0
 #define UART0_UDRIE    UDRIE0
#elif defined(__AVR_ATtiny2313__) 
 #define ATMEGA_USART
 #define UART0_RECEIVE_INTERRUPT   USART_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
 #define UART0_STATUS   UCSRA
 #define UART0_CONTROL  UCSRB
 #define UART0_DATA     UDR
 #define UART0_UDRIE    UDRIE
#elif defined(__AVR_ATmega329__) || \
      defined(__AVR_ATmega649__) || \
      defined(__AVR_ATmega325__) || \
      defined(__AVR_ATmega645__) 
  /* ATmega with one USART */
  #define ATMEGA_USART0
  #define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
  #define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
  #define UART0_STATUS   UCSR0A
  #define UART0_CONTROL  UCSR0B
  #define UART0_DATA     UDR0
  #define UART0_UDRIE    UDRIE0
#elif defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega1280__)  || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega640__)
/* ATmega with two USART */
  #define ATMEGA_USART0
  #define ATMEGA_USART1
  #define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
  #define UART1_RECEIVE_INTERRUPT   USART1_RX_vect
  #define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
  #define UART1_TRANSMIT_INTERRUPT  USART1_UDRE_vect
  #define UART0_STATUS   UCSR0A
  #define UART0_CONTROL  UCSR0B
  #define UART0_DATA     UDR0
  #define UART0_UDRIE    UDRIE0
  #define UART1_STATUS   UCSR1A
  #define UART1_CONTROL  UCSR1B
  #define UART1_DATA     UDR1
  #define UART1_UDRIE    UDRIE1  
#elif defined(__AVR_ATmega644__)
 /* ATmega with one USART */
 #define ATMEGA_USART0
 #define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
 #define UART0_STATUS   UCSR0A
 #define UART0_CONTROL  UCSR0B
 #define UART0_DATA     UDR0
 #define UART0_UDRIE    UDRIE0
#elif defined(__AVR_ATmega164P__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644P__)
 /* ATmega with two USART */
 #define ATMEGA_USART0
 #define ATMEGA_USART1
 #define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
 #define UART1_RECEIVE_INTERRUPT   USART1_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
 #define UART1_TRANSMIT_INTERRUPT  USART1_UDRE_vect
 #define UART0_STATUS   UCSR0A
 #define UART0_CONTROL  UCSR0B
 #define UART0_DATA     UDR0
 #define UART0_UDRIE    UDRIE0
 #define UART1_STATUS   UCSR1A
 #define UART1_CONTROL  UCSR1B
 #define UART1_DATA     UDR1
 #define UART1_UDRIE    UDRIE1
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB647__) || defined(__AVR_AT90USB1287__)
 /* AT90USBxx with one USART */
 #define AT90USB_USART
 #define UART0_RECEIVE_INTERRUPT   USART1_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART1_UDRE_vect
 #define UART0_STATUS   UCSR1A
 #define UART0_CONTROL  UCSR1B
 #define UART0_DATA     UDR1
 #define UART0_UDRIE    UDRIE1
#else
 #error "no UART definition for MCU available"
#endif


/*
 *  module global variables
 */
static volatile unsigned char UART_TxBuf[UART_TX_BUFFER_SIZE];
static volatile unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
static volatile unsigned char UART_TxHead;
static volatile unsigned char UART_TxTail;
static volatile unsigned char UART_RxHead;
static volatile unsigned char UART_RxTail;
static volatile unsigned char UART_LastRxError;

#if defined( ATMEGA_USART1 )
static volatile unsigned char UART1_TxBuf[UART_TX_BUFFER_SIZE];
static volatile unsigned char UART1_RxBuf[UART_RX_BUFFER_SIZE];
static volatile unsigned char UART1_TxHead;
static volatile unsigned char UART1_TxTail;
static volatile unsigned char UART1_RxHead;
static volatile unsigned char UART1_RxTail;
static volatile unsigned char UART1_LastRxError;
#endif



ISR (UART0_RECEIVE_INTERRUPT)	
/*************************************************************************
Function: UART Receive Complete interrupt
Purpose:  called when the UART has received a character
**************************************************************************/
{
    unsigned char tmphead;
    unsigned char data;
    unsigned char usr;
    unsigned char lastRxError;
 
 
    /* read UART status register and UART data register */ 
    usr  = UART0_STATUS;
    data = UART0_DATA;
    
    /* */
#if defined( AT90_UART )
    lastRxError = (usr & (_BV(FE)|_BV(DOR)) );
#elif defined( ATMEGA_USART )
    lastRxError = (usr & (_BV(FE)|_BV(DOR)) );
#elif defined( ATMEGA_USART0 )
    lastRxError = (usr & (_BV(FE0)|_BV(DOR0)) );
#elif defined ( ATMEGA_UART )
    lastRxError = (usr & (_BV(FE)|_BV(DOR)) );
#elif defined( AT90USB_USART )
    lastRxError = (usr & (_BV(FE1)|_BV(DOR1)) );
#endif
        
    /* calculate buffer index */ 
    tmphead = ( UART_RxHead + 1) & UART_RX_BUFFER_MASK;
    
    if ( tmphead == UART_RxTail ) {
        /* error: receive buffer overflow */
        lastRxError = UART_BUFFER_OVERFLOW >> 8;
    }else{
        /* store new index */
        UART_RxHead = tmphead;
        /* store received data in buffer */
        UART_RxBuf[tmphead] = data;
    }
    UART_LastRxError |= lastRxError;   
}


ISR (UART0_TRANSMIT_INTERRUPT)
/*************************************************************************
Function: UART Data Register Empty interrupt
Purpose:  called when the UART is ready to transmit the next byte
**************************************************************************/
{
    unsigned char tmptail;

    
    if ( UART_TxHead != UART_TxTail) {
        /* calculate and store new buffer index */
        tmptail = (UART_TxTail + 1) & UART_TX_BUFFER_MASK;
        UART_TxTail = tmptail;
        /* get one byte from buffer and write it to UART */
        UART0_DATA = UART_TxBuf[tmptail];  /* start transmission */
    }else{
        /* tx buffer empty, disable UDRE interrupt */
        UART0_CONTROL &= ~_BV(UART0_UDRIE);
    }
}


/*************************************************************************
Function: uart_init()
Purpose:  initialize UART and set baudrate
Input:    baudrate using macro UART_BAUD_SELECT()
Returns:  none
**************************************************************************/
void uart_init(unsigned int baudrate)
{
    UART_TxHead = 0;
    UART_TxTail = 0;
    UART_RxHead = 0;
    UART_RxTail = 0;
    
#if defined( AT90_UART )
    /* set baud rate */
    UBRR = (unsigned char)baudrate; 

    /* enable UART receiver and transmmitter and receive complete interrupt */
    UART0_CONTROL = _BV(RXCIE)|_BV(RXEN)|_BV(TXEN);

#elif defined (ATMEGA_USART)
    /* Set baud rate */
    if ( baudrate & 0x8000 )
    {
    	 UART0_STATUS = (1<<U2X);  //Enable 2x speed 
    	 baudrate &= ~0x8000;
    }
    UBRRH = (unsigned char)(baudrate>>8);
    UBRRL = (unsigned char) baudrate;
   
    /* Enable USART receiver and transmitter and receive complete interrupt */
    UART0_CONTROL = _BV(RXCIE)|(1<<RXEN)|(1<<TXEN);
    
    /* Set frame format: asynchronous, 8data, no parity, 1stop bit */
    #ifdef URSEL
    UCSRC = (1<<URSEL)|(3<<UCSZ0);
    #else
    UCSRC = (3<<UCSZ0);
    #endif 
    
#elif defined (ATMEGA_USART0 )
    /* Set baud rate */
    if ( baudrate & 0x8000 ) 
    {
   		UART0_STATUS = (1<<U2X0);  //Enable 2x speed 
   		baudrate &= ~0x8000;
   	}
    UBRR0H = (unsigned char)(baudrate>>8);
    UBRR0L = (unsigned char) baudrate;

    /* Enable USART receiver and transmitter and receive complete interrupt */
    UART0_CONTROL = _BV(RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
    
    /* Set frame format: asynchronous, 8data, no parity, 1stop bit */
    #ifdef URSEL0
    UCSR0C = (1<<URSEL0)|(3<<UCSZ00);
    #else
    UCSR0C = (3<<UCSZ00);
    #endif 

#elif defined ( ATMEGA_UART )
    /* set baud rate */
    if ( baudrate & 0x8000 ) 
    {
    	UART0_STATUS = (1<<U2X);  //Enable 2x speed 
    	baudrate &= ~0x8000;
    }
    UBRRHI = (unsigned char)(baudrate>>8);
    UBRR   = (unsigned char) baudrate;

    /* Enable UART receiver and transmitter and receive complete interrupt */
    UART0_CONTROL = _BV(RXCIE)|(1<<RXEN)|(1<<TXEN);

#elif defined ( AT90USB_USART )
   /* set baud rate */
    if ( baudrate & 0x8000 ) 
    {
    	UART0_STATUS = (1<<U2X1 );  //Enable 2x speed 
    	baudrate &= ~0x8000;
    }
    UBRR1H = (unsigned char)(baudrate>>8);
    UBRR1L = (unsigned char) baudrate;

    /* Enable UART receiver and transmitter and receive complete interrupt */
    UART0_CONTROL = _BV(RXCIE1)|(1<<RXEN1)|(1<<TXEN1);
    
    /* Set frame format: asynchronous, 8data, no parity, 1stop bit */
    UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
#endif

}/* uart_init */


/*************************************************************************
Function: uart_getc()
Purpose:  return byte from ringbuffer  
Returns:  lower byte:  received byte from ringbuffer
          higher byte: last receive error
**************************************************************************/
unsigned int uart_getc(void)
{    
    unsigned char tmptail;
    unsigned char data;


    if ( UART_RxHead == UART_RxTail ) {
        return UART_NO_DATA;   /* no data available */
    }
    
    /* calculate /store buffer index */
    tmptail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;
    UART_RxTail = tmptail; 
    
    /* get data from receive buffer */
    data = UART_RxBuf[tmptail];
    
    data = (UART_LastRxError << 8) + data;
    UART_LastRxError = 0;
    return data;

}/* uart_getc */


/*************************************************************************
Function: uart_putc()
Purpose:  write byte to ringbuffer for transmitting via UART
Input:    byte to be transmitted
Returns:  none          
**************************************************************************/
void uart_putc(unsigned char data)
{
    unsigned char tmphead;

    
    tmphead  = (UART_TxHead + 1) & UART_TX_BUFFER_MASK;
    
    while ( tmphead == UART_TxTail ){
        ;/* wait for free space in buffer */
    }
    
    UART_TxBuf[tmphead] = data;
    UART_TxHead = tmphead;

    /* enable UDRE interrupt */
    UART0_CONTROL    |= _BV(UART0_UDRIE);

}/* uart_putc */


/*************************************************************************
Function: uart_puts()
Purpose:  transmit string to UART
Input:    string to be transmitted
Returns:  none          
**************************************************************************/
void uart_puts(const char *s )
{
    while (*s) 
      uart_putc(*s++);

}/* uart_puts */


/*************************************************************************
Function: uart_puts_p()
Purpose:  transmit string from program memory to UART
Input:    program memory string to be transmitted
Returns:  none
**************************************************************************/
void uart_puts_p(const char *progmem_s )
{
    register char c;
    
    while ( (c = pgm_read_byte(progmem_s++)) ) 
      uart_putc(c);

}/* uart_puts_p */


/*
 * these functions are only for ATmegas with two USART
 */
#if defined( ATMEGA_USART1 )

ISR(UART1_RECEIVE_INTERRUPT)
/*************************************************************************
Function: UART1 Receive Complete interrupt
Purpose:  called when the UART1 has received a character
**************************************************************************/
{
    unsigned char tmphead;
    unsigned char data;
    unsigned char usr;
    unsigned char lastRxError;
 
 
    /* read UART status register and UART data register */ 
    usr  = UART1_STATUS;
    data = UART1_DATA;
    
    /* */
    lastRxError = (usr & (_BV(FE1)|_BV(DOR1)) );
        
    /* calculate buffer index */ 
    tmphead = ( UART1_RxHead + 1) & UART_RX_BUFFER_MASK;
    
    if ( tmphead == UART1_RxTail ) {
        /* error: receive buffer overflow */
        lastRxError = UART_BUFFER_OVERFLOW >> 8;
    }else{
        /* store new index */
        UART1_RxHead = tmphead;
        /* store received data in buffer */
        UART1_RxBuf[tmphead] = data;
    }
    UART1_LastRxError |= lastRxError;   
}


ISR(UART1_TRANSMIT_INTERRUPT)
/*************************************************************************
Function: UART1 Data Register Empty interrupt
Purpose:  called when the UART1 is ready to transmit the next byte
**************************************************************************/
{
    unsigned char tmptail;

    
    if ( UART1_TxHead != UART1_TxTail) {
        /* calculate and store new buffer index */
        tmptail = (UART1_TxTail + 1) & UART_TX_BUFFER_MASK;
        UART1_TxTail = tmptail;
        /* get one byte from buffer and write it to UART */
        UART1_DATA = UART1_TxBuf[tmptail];  /* start transmission */
    }else{
        /* tx buffer empty, disable UDRE interrupt */
        UART1_CONTROL &= ~_BV(UART1_UDRIE);
    }
}


/*************************************************************************
Function: uart1_init()
Purpose:  initialize UART1 and set baudrate
Input:    baudrate using macro UART_BAUD_SELECT()
Returns:  none
**************************************************************************/
void uart1_init(unsigned int baudrate)
{
    UART1_TxHead = 0;
    UART1_TxTail = 0;
    UART1_RxHead = 0;
    UART1_RxTail = 0;
    

    /* Set baud rate */
    if ( baudrate & 0x8000 ) 
    {
    	UART1_STATUS = (1<<U2X1);  //Enable 2x speed 
      baudrate &= ~0x8000;
    }
    UBRR1H = (unsigned char)(baudrate>>8);
    UBRR1L = (unsigned char) baudrate;

    /* Enable USART receiver and transmitter and receive complete interrupt */
    UART1_CONTROL = _BV(RXCIE1)|(1<<RXEN1)|(1<<TXEN1);
    
    /* Set frame format: asynchronous, 8data, no parity, 1stop bit */   
    #ifdef URSEL1
    UCSR1C = (1<<URSEL1)|(3<<UCSZ10);
    #else
    UCSR1C = (3<<UCSZ10);
    #endif 
}/* uart_init */


/*************************************************************************
Function: uart1_getc()
Purpose:  return byte from ringbuffer  
Returns:  lower byte:  received byte from ringbuffer
          higher byte: last receive error
**************************************************************************/
unsigned int uart1_getc(void)
{    
    unsigned char tmptail;
    unsigned char data;


    if ( UART1_RxHead == UART1_RxTail ) {
        return UART_NO_DATA;   /* no data available */
    }
    
    /* calculate /store buffer index */
    tmptail = (UART1_RxTail + 1) & UART_RX_BUFFER_MASK;
    UART1_RxTail = tmptail; 
    
    /* get data from receive buffer */
    data = UART1_RxBuf[tmptail];
    
    data = (UART1_LastRxError << 8) + data;
    UART1_LastRxError = 0;
    return data;

}/* uart1_getc */


/*************************************************************************
Function: uart1_putc()
Purpose:  write byte to ringbuffer for transmitting via UART
Input:    byte to be transmitted
Returns:  none          
**************************************************************************/
void uart1_putc(unsigned char data)
{
    unsigned char tmphead;

    
    tmphead  = (UART1_TxHead + 1) & UART_TX_BUFFER_MASK;
    
    while ( tmphead == UART1_TxTail ){
        ;/* wait for free space in buffer */
    }
    
    UART1_TxBuf[tmphead] = data;
    UART1_TxHead = tmphead;

    /* enable UDRE interrupt */
    UART1_CONTROL    |= _BV(UART1_UDRIE);

}/* uart1_putc */


/*************************************************************************
Function: uart1_puts()
Purpose:  transmit string to UART1
Input:    string to be transmitted
Returns:  none          
**************************************************************************/
void uart1_puts(const char *s )
{
    while (*s) 
      uart1_putc(*s++);

}/* uart1_puts */


/*************************************************************************
Function: uart1_puts_p()
Purpose:  transmit string from program memory to UART1
Input:    program memory string to be transmitted
Returns:  none
**************************************************************************/
void uart1_puts_p(const char *progmem_s )
{
    register char c;
    
    while ( (c = pgm_read_byte(progmem_s++)) ) 
      uart1_putc(c);

}/* uart1_puts_p */

#endif
/////end lib





void program_page (uint32_t page, uint8_t *buf)
{
	uint16_t i;
	uint8_t sreg;
	
	//Disable interrupts
	sreg = SREG;
	cli();
	
	eeprom_busy_wait ();
	
	boot_page_erase (page);
	boot_spm_busy_wait ();      //Wait until the memory is erased.
	
	for (i=0; i<SPM_PAGESIZE; i+=2)
	{
		//Set up little-endian word.
		uint16_t w = *buf++;
		w += (*buf++) << 8;
		
		boot_page_fill (page + i, w);
	}
	
	boot_page_write (page);     //Store buffer in flash page.
	boot_spm_busy_wait();       //Wait until the memory is written.
	
	//Reenable RWW-section again. We need this if we want to jump back
	//to the application after bootloading.
	boot_rww_enable ();
	
	//Re-enable interrupts (if they were ever enabled).
	SREG = sreg;
}

static uint16_t hex2num (const uint8_t * ascii, uint8_t num)
{
	uint8_t  i;
	uint16_t val = 0;
	
	for (i=0; i<num; i++)
	{
		uint8_t c = ascii[i];
		
		//Hex-Ziffer auf ihren Wert abbilden
		if (c >= '0' && c <= '9')            c -= '0';
		else if (c >= 'A' && c <= 'F')       c -= 'A' - 10;
		else if (c >= 'a' && c <= 'f')       c -= 'a' - 10;
		
		val = 16 * val + c;
	}
	
	return val;
}

int main()
{
	//Empfangenes Zeichen + Statuscode
	uint16_t    c = 0,
				hex_addr = 0,						//Intel-HEX Zieladresse
				flash_page = 0,						//Zu schreibende Flash-Page
				hex_check = 0,						//Intel-HEX Checksumme zum Überprüfen des Daten
				flash_cnt = 0;						//Positions zum Schreiben in der Datenpuffer
	uint8_t		temp,								//temporäre Variable
				boot_state = BOOT_STATE_EXIT,		//Flag zum Steuern des Programmiermodus
				parser_state = PARSER_STATE_START,	//Empfangszustandssteuerung
				flash_page_flag = 1,				//Flag zum Ermitteln einer neuen Flash-Page
				flash_data[SPM_PAGESIZE],			//Datenpuffer für die Hexdaten
				hex_cnt = 0,						//Position zum Schreiben in den HEX-Puffer
				hex_buffer[5],						//Puffer für die Umwandlung der ASCII in Binärdaten
				hex_size = 0,						//Intel-HEX Datenlänge
				hex_data_cnt = 0,					//Zähler für die empfangenen HEX-Daten einer Zeile
				hex_type = 0,						//Intel-HEX Recordtype
				hex_checksum=0;						//empfangene HEX-Checksumme
	void(*start)( void ) = 0x0000;					//Funktionspointer auf 0x0000
	
	
	//  prepare 36kHz for IR - Communication
//	TCCR2 = (1 << WGM21) | (1 << COM20) | (1 << CS20);
//	OCR2  = 0x6E; // 36kHz @8MHz
//	DDRB |= IRTX;

	//Füllen der Puffer mit definierten Werten
	memset(hex_buffer, 0x00, sizeof(hex_buffer));
	memset(flash_data, 0xFF, sizeof(flash_data));
	
	//Interrupt Vektoren verbiegen
	temp = GICR;						//General Interrupt Control Register
	GICR = temp | (1<<IVCE);			//enable change of the IVSEL bit
	GICR = temp | (1<<IVSEL);			//Interrupt Vectors are moved to the beginning of the Boot Loader section

	uart_init( UART_BAUD_SELECT(BOOT_UART_BAUD_RATE,F_CPU) );	//calculate baudrate
	sei();														//enable global interupts
	
	//self added:
	DDRB = (1<<PB2);
	PORTB |= (1<<PB2);

	uart_puts("Hallo hier ist der echte Bootloader\n\r");
	_delay_ms(2000);
	
// 	while (1)
// 	{
// 		c = uart_getc();
// 		uart_putc('a');
// 		uart_putc(c);
// 		_delay_ms(500);
// 	}
	
	do
	{
		c = uart_getc();
//		uart_putc('a');
//		uart_putc(c);
		if(!(c & UART_NO_DATA))
		{
			//Programmzustand: Parser
			if(boot_state == BOOT_STATE_PARSER){
				switch(parser_state){
					case PARSER_STATE_START:		//Warte auf Zeilen-Startzeichen
						if((uint8_t)c == START_SIGN)
						{
							uart_putc(XOFF);
							parser_state = PARSER_STATE_SIZE;
							hex_cnt = 0;
							hex_check = 0;
							uart_putc(XON);
						}
						break;
					case PARSER_STATE_SIZE:			//Parse Datengröße
						hex_buffer[hex_cnt++] = (uint8_t)c;
						if(hex_cnt == 2)
						{
							uart_putc(XOFF);
							parser_state = PARSER_STATE_ADDRESS;
							hex_cnt = 0;
							hex_size = (uint8_t)hex2num(hex_buffer, 2);
							hex_check += hex_size;
							uart_putc(XON);
						}
						break;
					case PARSER_STATE_ADDRESS:		//Parse Zieladresse
						hex_buffer[hex_cnt++] = (uint8_t)c;
						if(hex_cnt == 4)
						{
							uart_putc(XOFF);
							parser_state = PARSER_STATE_TYPE;
							hex_cnt = 0;
							hex_addr = hex2num(hex_buffer, 4);
							hex_check += (uint8_t) hex_addr;
							hex_check += (uint8_t) (hex_addr >> 8);
							if(flash_page_flag)
							{
								flash_page = hex_addr - hex_addr % SPM_PAGESIZE;
								flash_page_flag = 0;
							}
							uart_putc(XON);
						}
						break;
					case PARSER_STATE_TYPE:			//Parse Zeilentyp
						hex_buffer[hex_cnt++] = (uint8_t)c;
						if(hex_cnt == 2){
							uart_putc(XOFF);
							hex_cnt = 0;
							hex_data_cnt = 0;
							hex_type = (uint8_t)hex2num(hex_buffer, 2);
							hex_check += hex_type;
							switch(hex_type){
								case 0:  parser_state = PARSER_STATE_DATA;		break;
								case 1:  parser_state = PARSER_STATE_CHECKSUM;	break;
								default: parser_state = PARSER_STATE_DATA;		break;
							}
							uart_putc(XON);
						}
						break;		
					case PARSER_STATE_DATA:			//Parse Flash-Daten
						hex_buffer[hex_cnt++] = (uint8_t)c;
						if(hex_cnt == 2){
							uart_putc(XOFF);
							uart_putc('.');
							hex_cnt = 0;
							flash_data[flash_cnt] = (uint8_t)hex2num(hex_buffer, 2);
							hex_check += flash_data[flash_cnt];
							flash_cnt++;
							hex_data_cnt++;
							if(hex_data_cnt == hex_size){
								parser_state = PARSER_STATE_CHECKSUM;
								hex_data_cnt=0;
								hex_cnt = 0;
							}
							//Puffer voll -> schreibe Page
							if(flash_cnt == SPM_PAGESIZE){
								uart_puts("P\n\r");
								_delay_ms(100);
								program_page((uint16_t)flash_page, flash_data);
								memset(flash_data, 0xFF, sizeof(flash_data));
								flash_cnt = 0;
								flash_page_flag = 1;
							}
							uart_putc(XON);
						}
						break;
					case PARSER_STATE_CHECKSUM:			//Parse Checksumme
						hex_buffer[hex_cnt++] = (uint8_t)c;
						if(hex_cnt == 2){
							uart_putc(XOFF);
							hex_checksum = (uint8_t)hex2num(hex_buffer, 2);
							hex_check += hex_checksum;
							hex_check &= 0x00FF;
							//Dateiende -> schreibe Restdaten
							if(hex_type == 1){
								uart_puts("P\n\r");
								_delay_ms(100);
								program_page((uint16_t)flash_page, flash_data);
								boot_state = BOOT_STATE_EXIT;
							}
							if(hex_check == 0){		//Überprüfe Checksumme -> muss '0' sein
								parser_state = PARSER_STATE_START;
							}else{
								parser_state = PARSER_STATE_ERROR;
							}
							uart_putc(XON);
						}
						break;
					case PARSER_STATE_ERROR:		//Parserfehler (falsche Checksumme)
						uart_putc('#');
						break;
					default:
						break;
				}
			}
			//Programmzustand: UART Kommunikation
			else if(boot_state != BOOT_STATE_PARSER){
				switch((uint8_t)c){
					case 'p':
						boot_state = BOOT_STATE_PARSER;
						uart_puts("Programmiere den Flash!\n\r");
						uart_puts("Kopiere die Hex-Datei und füge sie"
						" hier ein (rechte Maustaste)\n\r");
						break;
					case 'q':
						boot_state = BOOT_STATE_EXIT;
						uart_puts("Verlasse den Bootloader!\n\r");
						break;
					default:
						uart_puts("Du hast folgendes Zeichen gesendet: ");
						uart_putc((unsigned char)c);
						uart_puts("\n\r");
						break;
				}
			}
		}
	}while(boot_state!=BOOT_STATE_EXIT);
	
	uart_puts("Reset AVR!\n\r");
	_delay_ms(1000);
	
	//Interrupt Vektoren wieder gerade biegen
	temp = GICR;					//General Interrupt Control Register
	GICR = temp | (1<<IVCE);		//enable change of the IVSEL bit
	GICR = temp & ~(1<<IVSEL);		//Interrupt Vectors are moved to the beginning of the Flash
	
	start();		//Reset

	return 0;
}
