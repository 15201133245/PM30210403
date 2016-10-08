#ifndef	_M1172_H
#define	_M1172_H
#include "spi.h"
#include "board.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define IDLE				0
#define TX_STATUS			1

#define	M1172_ERROR_LOCK	1
#define M1172_ERROR_SPI		2

#define LENGTH_ERR			(-0xfc)
#define POINTER_ERR			(-0xfd)

#define M1172_OSC           ((unsigned int)14.7456e6)

//parity method
#define M1172_UART_PARITY_NONE       (0x00<<3)          // No Parity      
#define M1172_UART_PARITY_ODD        (0x01<<3)          // Odd Parity 
#define M1172_UART_PARITY_EVEN       (0x03<<3)          // Even Parity  
#define M1172_UART_PARITY_MARK       (0x05<<3)          // Parity forced to 1 (Mark) 
#define M1172_UART_PARITY_SPACE      (0x07<<3)          // Parity forced to 0 (Space) 

//length
#define M1172_UART_LENGTH_5      0x00       // Character Length: 5 bits 
#define M1172_UART_LENGTH_6      0x01       // Character Length: 6 bits 
#define M1172_UART_LENGTH_7      0x02       // Character Length: 7 bits 
#define M1172_UART_LENGTH_8      0x03       // Character Length: 8 bits 

//STOP
#define M1172_UART_STOP_1      (0x00<<2)    // 1 stop bit 
#define M1172_UART_STOP_2      (0x01<<2)    // 2 stop bit 

//485 option
#define M1172_UART_AUTO485_DISABLE      (0x00<<4)
#define M1172_UART_AUTO485_RHTL         (0x01<<4)
#define M1172_UART_AUTO485_RLTH         (0x03<<4)

//Transmit FIFO trigger (in FCR)
#define M1172_CHANNEL_TX_FIFOTRIGGER_8      (0x00<<4)
#define M1172_CHANNEL_TX_FIFOTRIGGER_16     (0x01<<4)
#define M1172_CHANNEL_TX_FIFOTRIGGER_32     (0x02<<4)
#define M1172_CHANNEL_TX_FIFOTRIGGER_56     (0x03<<4)


//Receive FIFO trigger (in FCR)
#define M1172_CHANNEL_RX_FIFOTRIGGER_8      (0x00<<6)
#define M1172_CHANNEL_RX_FIFOTRIGGER_16     (0x01<<6)
#define M1172_CHANNEL_RX_FIFOTRIGGER_56     (0x02<<6)
#define M1172_CHANNEL_RX_FIFOTRIGGER_60     (0x03<<6)

//Interrupt mask
#define M1172_RX_TIMEOUT					(0x03 << 2)
#define M1172_RXRDY							(0x01 << 2)
#define M1172_TXRDY							(0x01 << 1)
#define M1172_MSR							(0)
#define	M1172_GPIO							(0x03 << 4)
#define M1172_RX_Xoff						(0x01 << 4)
#define M1172_CTS							(0x01 << 5)
#define M1172_LSR							(0x03 << 1)

//m1172 command
#define CMD_READ		(0x01 << 7)
#define	CMD_WRITE		(0x00 << 7)

//m1172 channel
#define	CHA				(0x00 << 1)
#define CHB				(0x01 << 1)

//m1172 register
#define	REG_RHR			(0x00 << 3)			//LCR[7]=0,R
#define	REG_THR			(0x00 << 3)			//LCR[7]=0,W
#define REG_DLL			(0x00 << 3)			//LCR[7]=1.LCR!=0xBF,R/W
#define REG_DLM			(0x01 << 3)			//LCR[7]=1,LCR!=0xBF,R/W
#define REG_DLD			(0x02 << 3)			//LCR[7]=1,LCR!=0xBF,EFR[4]=1,R/W
#define	REG_IER			(0x01 << 3)			//LCR[7]=0,R/W
#define REG_ISR			(0x02 << 3)			//LCR[7]=0,R
#define REG_FCR			(0x02 << 3)			//LCR[7]=0,W
#define REG_LCR			(0x03 << 3)			//R/W
#define REG_MCR			(0x04 << 3)			//LCR!=0xBF,R/W
#define	REG_LSR			(0x05 << 3)			//LCR!=0xBF,R
#define REG_MSR			(0x06 << 3)			//EFR[4]=0 or (EFR[4]=1 and MCR[2]=0),R
#define REG_SPR			(0x07 << 3)			//EFR[4]=0 or (EFR[e]=1 and MCR[2]=0),R/W
#define REG_TCR			(0x06 << 3)			//EFR[4]=1 and MCR[2]=1,R/W
#define REG_TLR			(0x07 << 3)			//EFR[4]=1 and MCR[2]=1,R/W
#define REG_TXLVL		(0x08 << 3)			//LCR[7]=0,R
#define REG_RXLVL		(0x09 << 3)			//LCR[7]=0,R
#define REG_IODir		(0x0a << 3)			//LCR[7]=0,R/W
#define REG_IOState		(0x0b << 3)			//LCR[7]=0,R/W
#define	REG_IOIntEna	(0x0c << 3)			//LCR[7]=0,R/W

#define REG_IOControl	(0x0e << 3)			//LCR[7]=0,R/W
#define REG_EFCR		(0x0f << 3)			//LCR[7]=0,R/W
#define REG_EFR			(0x02 << 3)			//LCR=0xBF,R/W
#define	REG_Xon1		(0x04 << 3)			//LCR=0xBF,R/W
#define REG_Xon2		(0x05 << 3)			//LCR=0xBF,R/W
#define REG_Xoff1		(0x06 << 3)			//LCR=0xBF,R/W
#define REG_Xoff2		(0x07 << 3)			//LCR=0xBF,R/W


typedef void (*Callback )(unsigned char *, unsigned char);
typedef void (*pTimeOut)(void);

typedef struct _M1172_ChCtrl
{
  	unsigned char channelStatus;
	unsigned char txStatus;
	unsigned char rxStatus;
	unsigned char channel;
	unsigned int baud;
	unsigned char parity;
	unsigned char data_length;
	unsigned char stop;
	unsigned char TxFIFOLvl;
	unsigned char RxFIFOLvl;
	unsigned char efcr_mode;
	unsigned char *TxBuf;
	unsigned char *RxBuf;
	unsigned int TxLen;
	unsigned int RxLen;
	Callback sendback;
	Callback receviceback;
	
}M1172_ChCtrl;

typedef struct _SPI_USART
{
  	/// Pointer to Spi Structure (SPI low level driver).
	Spid *pSpid;
    /// Current SPI command sent to the SPI low level driver.
	SpidCmd command;
	/// Buffer to store the current command (opcode + dataflash address.
	unsigned char pCmdBuffer[8];
	M1172_ChCtrl *pChannel;
}M1172;

#define BUF_MAX			512		//2**n 

typedef struct _Queue
{
  	unsigned int head;
	unsigned int tail;
	unsigned char buf[BUF_MAX];
}Queue;

extern	M1172 m1172;

void WriteToQue(Queue *pQue,unsigned char val);

unsigned char ReadFromQue(Queue *pQue,unsigned char *p);

unsigned char Que_IsFull(Queue *pQue);

unsigned char Que_IsEmpty(Queue *pQue);

unsigned int GetQueCharNumber(Queue *pQue);

unsigned char M1172_Configure(M1172 *pM1172,Spid *pSpid,unsigned char spiCs);

void SpiUsartConfigure(unsigned char len,...);

void M1172_ConfigureChannel(M1172_ChCtrl *pChannel,unsigned char ier,unsigned char efr);

unsigned char M1172_SendCommand(M1172 *pM1172,
								unsigned char cmd,
								unsigned char *pdata,
								unsigned int dataSize);

unsigned char M1172_IsBusy(M1172 *pM1172);

unsigned char M1172_WriteReg(unsigned char channel,
							 unsigned char reg,
							 unsigned char val);

unsigned char M1172_ReadReg(unsigned char channel,
							unsigned char reg,
							unsigned char *p);

unsigned char M1172_LoadToTxFIFO(M1172 *pM1172);

unsigned char M1172_SaveFromRxFIFO(M1172 *pM1172);

int WriteToSpiUsart(M1172 *pM1172,unsigned char *pBuffer,unsigned int len);

int ReadFromSpiUsart(M1172 *pM1172,unsigned char *pBuffer,unsigned int len);

void SpiUsart(void);

void M1172Check(void);

#endif