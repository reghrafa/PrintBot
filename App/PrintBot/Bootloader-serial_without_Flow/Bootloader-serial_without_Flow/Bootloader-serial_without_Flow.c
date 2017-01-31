/*
 * Bootloader-serial_without_Flow.c
 * without Flow Control
 * Created: 25/01/2017 19:57:39
 * Author : Fabian
 */ 

//Other Linker Flags: -Wl,--section-start=.text=0x7800
//efuse=0x05 hfuse=0xD2 lfuse=0xE2

//use 0x7000 with hfuse=0xD0
#define F_CPU 8000000UL
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/boot.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "uart.h"

#define BOOT_UART_BAUD_RATE     9600     //Baudrate
#define XON                     17       //XON Zeichen
#define XOFF                    19       //XOFF Zeichen
#define START_SIGN              ':'      //Hex-Datei Zeilenstartzeichen

//states of the bootloader programms
#define BOOT_STATE_EXIT	        0
#define BOOT_STATE_PARSER       1

//states of the hex file parsers
#define PARSER_STATE_START      0
#define PARSER_STATE_SIZE       1
#define PARSER_STATE_ADDRESS    2
#define PARSER_STATE_TYPE       3
#define PARSER_STATE_DATA       4
#define PARSER_STATE_CHECKSUM   5
#define PARSER_STATE_ERROR      6

void WDT_off(void)
{
	cli();
	wdt_reset();
	MCUSR &= ~(1<<WDRF);  //WDRF: Watchdog System Reset Flag
	// Write logical one to WDCE and WDE /
	// Keep old prescaler setting to prevent unintentional time-out /
	WDTCSR |= (1<<WDCE) | (1<<WDE);  // WDCE: Watchdog Change Enable (must be set to change WDE)
	//Turn off WDT
	WDTCSR = 0x00;
	//sei();
}

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
				hex_check = 0,						//Intel-HEX Checksumme zum ?berpr?fen des Daten
				flash_cnt = 0;						//Positions zum Schreiben in der Datenpuffer
	uint8_t		temp,								//tempor?re Variable
				boot_state = BOOT_STATE_EXIT,		//Flag zum Steuern des Programmiermodus
				parser_state = PARSER_STATE_START,	//Empfangszustandssteuerung
				flash_page_flag = 1,				//Flag zum Ermitteln einer neuen Flash-Page
				flash_data[SPM_PAGESIZE],			//Datenpuffer f?r die Hexdaten
				hex_cnt = 0,						//Position zum Schreiben in den HEX-Puffer
				hex_buffer[5],						//Puffer f?r die Umwandlung der ASCII in Bin?rdaten
				hex_size = 0,						//Intel-HEX Datenl?nge
				hex_data_cnt = 0,					//Z?hler f?r die empfangenen HEX-Daten einer Zeile
				hex_type = 0,						//Intel-HEX Recordtype
				hex_checksum=0;						//empfangene HEX-Checksumme
	void(*start)( void ) = 0x0000;					//Funktionspointer auf 0x0000
	
	WDT_off();

	//self added:
	DDRB |= (1<<PB1) | (1<<PB2);
	PORTB |= (1<<PB1);

	//F?llen der Puffer mit definierten Werten
	memset(hex_buffer, 0x00, sizeof(hex_buffer));
	memset(flash_data, 0xFF, sizeof(flash_data));
	
	//Interrupt Vektoren verbiegen
	temp = MCUCR;						//General Interrupt Control Register
	MCUCR = temp | (1<<IVCE);			//enable change of the IVSEL bit
	MCUCR = temp | (1<<IVSEL);			//Interrupt Vectors are moved to the beginning of the Boot Loader section

	uart_init( UART_BAUD_SELECT(BOOT_UART_BAUD_RATE,F_CPU) );	//calculate baudrate
	sei();														//enable global interupts


	uart_puts("start real bootloader\n");
	_delay_ms(2000);
//	PORTB |= (1<<PB2);
	
// 	while (1)
// 	{
// 		uart_putc('L');
// 		c = uart_getc();
// 		uart_putc('a');
// 		uart_putc(c);
// 		_delay_ms(500);
// 	}
	
	do
	{
		c = uart_getc();
		if(!(c & UART_NO_DATA))
		{
			//Programmzustand: Parser
			if(boot_state == BOOT_STATE_PARSER){
				switch(parser_state){
					case PARSER_STATE_START:		//Warte auf Zeilen-Startzeichen
						if((uint8_t)c == START_SIGN)
						{
							parser_state = PARSER_STATE_SIZE;
							hex_cnt = 0;
							hex_check = 0;
						}
						break;
					case PARSER_STATE_SIZE:			//Parse Datengr??e
						hex_buffer[hex_cnt++] = (uint8_t)c;
						if(hex_cnt == 2)
						{
							parser_state = PARSER_STATE_ADDRESS;
							hex_cnt = 0;
							hex_size = (uint8_t)hex2num(hex_buffer, 2);
							hex_check += hex_size;
						}
						break;
					case PARSER_STATE_ADDRESS:		//Parse Zieladresse
						hex_buffer[hex_cnt++] = (uint8_t)c;
						if(hex_cnt == 4)
						{
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
						}
						break;
					case PARSER_STATE_TYPE:			//Parse Zeilentyp
						hex_buffer[hex_cnt++] = (uint8_t)c;
						if(hex_cnt == 2){
							hex_cnt = 0;
							hex_data_cnt = 0;
							hex_type = (uint8_t)hex2num(hex_buffer, 2);
							hex_check += hex_type;
							switch(hex_type){
								case 0:  parser_state = PARSER_STATE_DATA;		break;
								case 1:  parser_state = PARSER_STATE_CHECKSUM;	break;
								default: parser_state = PARSER_STATE_DATA;		break;
							}
						}
						break;		
					case PARSER_STATE_DATA:			//Parse Flash-Daten
						hex_buffer[hex_cnt++] = (uint8_t)c;
						if(hex_cnt == 2){
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
								uart_puts("P\n");
								_delay_ms(100);
								program_page((uint16_t)flash_page, flash_data);
								memset(flash_data, 0xFF, sizeof(flash_data));
								flash_cnt = 0;
								flash_page_flag = 1;
							}
						}
						break;
					case PARSER_STATE_CHECKSUM:			//Parse Checksumme
						hex_buffer[hex_cnt++] = (uint8_t)c;
						if(hex_cnt == 2){
							hex_checksum = (uint8_t)hex2num(hex_buffer, 2);
							hex_check += hex_checksum;
							hex_check &= 0x00FF;
							//Dateiende -> schreibe Restdaten
							if(hex_type == 1){
								uart_puts("P-End\n");
								_delay_ms(100);
								program_page((uint16_t)flash_page, flash_data);
								boot_state = BOOT_STATE_EXIT;
							}
							if(hex_check == 0){		//?berpr?fe Checksumme -> muss '0' sein
								parser_state = PARSER_STATE_START;
							}else{
								parser_state = PARSER_STATE_ERROR;
							}
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
						uart_puts("program the flash!\n");
						uart_puts("paste the hex file\n");
						break;
					case 'q':
						boot_state = BOOT_STATE_EXIT;
						uart_puts("exit the bootloader!\n");
						break;
					default:
						uart_puts("you've sent: ");
						uart_putc((unsigned char)c);
						uart_putc('\n');
						break;
				}
			}
		}
	}while(boot_state!=BOOT_STATE_EXIT);
	
	uart_puts("Reset AVR!\n");
	_delay_ms(1000);
	
//	//self added
// 	cli();
// 	_delay_ms(1000);
// 	PORTB |= (1<<PB2);
	
	//Interrupt Vektoren wieder gerade biegen
	temp = MCUCR;					//General Interrupt Control Register
	MCUCR = temp | (1<<IVCE);		//enable change of the IVSEL bit
	MCUCR = temp & ~(1<<IVSEL);		//Interrupt Vectors are moved to the beginning of the Flash
	
	start();		//Reset

	return 0;
}
