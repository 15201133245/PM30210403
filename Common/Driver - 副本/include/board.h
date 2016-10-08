#ifndef _BOARD_H
#define _BOARD_H

/// The pin is controlled by the associated signal of peripheral A.
#define PIO_PERIPH_A                0
/// The pin is controlled by the associated signal of peripheral B.
#define PIO_PERIPH_B                1
/// The pin is an input.
#define PIO_INPUT                   2
/// The pin is an output and has a default level of 0.
#define PIO_OUTPUT_0                3
/// The pin is an output and has a default level of 1.
#define PIO_OUTPUT_1                4

/// Default pin configuration (no attribute).
#define PIO_DEFAULT                 (0 << 0)
/// The internal pin pull-up is active.
#define PIO_PULLUP                  (1 << 0)
/// The internal glitch filter is active.
#define PIO_DEGLITCH                (1 << 1)
/// The pin is open-drain.
#define PIO_OPENDRAIN               (1 << 2)

#define TWI_DEFAULT_CLK     		(unsigned int)100e3
#define MAIN_MCK					(unsigned int)48e6
//SPI clock
/// SPI clock frequency, in Hz.
#define AT45_SPI_MCK				(unsigned int)1e7
#define SPCK        				(unsigned int)5e6//(unsigned int)(1e6)

//TWI EEPROM address
#define	AT24_ADDR					0x50
//TWI IIC Card address
#define AT83_ADDR1					0x21
#define	AT83_ADDR2					0x23
#define AT83_ADDR3					0x25
#define AT83_ADDR4					0x27
//SPI
#define BOARD_AT45_NPCS0			0
#define	BOARD_AT45_NPCS1			1
#define BOARD_M1172_NPCS			2

//PA0
#define RUN_LED  	{1 << 0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
//PA1
#define BEEP		{1 << 1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
//PA2
#define LCD_BL		{1 << 2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
//PA23
#define	TWI_ADR		{1 << 23, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_PULLUP}
//PA24
#define USBCNX		{1 << 24, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP}
//PA25
#define PRINT_CTS	{1 << 25, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP}
//PA30
#define	IRQ1		{1 << 30, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
//PB0
#define	JUMP		{1 << 0, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_PULLUP}
//PB19
#define FIQ			{1 << 19, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_PULLUP}
//PB20
#define IRQ0		{1 << 20, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_PULLUP}
//PB21
#define CARD_OPEN	{1 << 21, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
//PB22
#define CARD_IN		{1 << 22, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_PULLUP}

/// TWI pins definition.
/// TWI pins definition.
#define PINS_TWI  	{0x00000018, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_OPENDRAIN}
//USART0
#define USART0		{0x000000e0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
//USART1
#define USART1		{0x00600000, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
//ISO7816
#define ISO7816		{0x00c00000, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
//DBUG_USART
#define DBUG_USART	{0x00000600, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
//SPI
#define SPI			{0x00007800, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define SPI_NCS1	{1 << 9,AT91C_BASE_PIOB,AT91C_ID_PIOB,PIO_PERIPH_A,PIO_DEFAULT}
#define SPI_NCS2	{1 << 30, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}

//SMC
#define DATA_BUS	{0xff, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_OPENDRAIN}
#define ADDR_BUS	{0x001f0000, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_OPENDRAIN}
#define CTRL_BUS	{0x00e00000, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_B, PIO_PULLUP}

//ADDRESS
#define	RS485Adr					(0x10000000)
#define KeyIn						(0x10000000)
#define	KeyOut						(0x10100000)
#define	InputAdr					(0x10080000)
#define	OutputAdr					(0x10180000)
#define	LcdLeftStateAdr				(0x10580000)
#define LcdRightStateAdr			(0x10540000)
#define LcdLeftComWriteAdr			(0x10480000)
#define	LcdRightComWriteAdr			(0x10440000)
#define LcdLeftDataWriteAdr			(0x10680000)
#define LcdRightDataWriteAdr		(0x10640000)

#define	LcdComAdr					(0x10240000)
#define LcdDataAdr					(0x10040000)

/*#define LcdStatusAdr				(0x10540000)
#define LcdReadDataAdr				(0x10740000)
#define LcdWriteDataAdr				(0x10440000)
#define LcdComAdr					(0x10640000)
#define LcdResetAdr					(0x10400000)*/

typedef struct _SinopecRev
{
	unsigned short len;
	unsigned char buf[254];
}SinopecRev;

#endif