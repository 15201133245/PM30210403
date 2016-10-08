#ifndef	_USART_H
#define	_USART_H
#include "AT91SAM7SE512.h"
#include <stdarg.h>

#define LENGTH_ERR			(-0xfc)
#define POINTER_ERR			(-0xfd)

//Write USART register
#define WRITE_USART(pUsart,regName,value)	pUsart->regName = (value)

//Read USART register
#define READ_USART(pUsart,regName)			(pUsart->regName)

typedef void (*UsartCallback )(unsigned char *,unsigned int, unsigned char);

typedef struct _Serial_t
{
  	AT91S_USART   *pUsartHw;
  	UsartCallback receviceback;
	UsartCallback sendback;
	UsartCallback CrcBack;
	UsartCallback TimeOut;
	unsigned char TimeFlg;
	unsigned char *pTransfer;
	unsigned int  TransferLen;
	unsigned char *pReceiver;
	unsigned int  ReceiveLen;
	unsigned char HeadLen;
	unsigned char RevCom;
	unsigned char TrunChar;
	unsigned char ErrType;
	unsigned char *pRevQue;
}Serial_t;



inline void USART_Configure(AT91S_USART *usart,
                            unsigned int mode,
                            unsigned int baudrate,
                            unsigned int masterClock)
{
	unsigned int val;
  	//Reset and disable receiver & transmitter
	usart->US_CR = AT91C_US_RSTRX | AT91C_US_RSTTX
	   | AT91C_US_RXDIS | AT91C_US_TXDIS;
	 
	 //Configure mode
	 usart->US_MR = mode;
	 
	 //Configure baudrate
	 //Asynchronous, no oversamling
	 if((mode & AT91C_US_SYNC) == 0)
	 {
		 val = masterClock / baudrate;
		 if((val & 0x07) == 0)
		 {
			 mode |= AT91C_US_OVER;
			 usart->US_MR = mode;
			 val >>= 3;
		 }
		 else if((val & 0x0f) > 7)
		 {
			 val = (val >> 4) + 1;
		 }
		 else
		 {
			 val >>= 4;
		 }
		 usart->US_BRGR = val;
	 }
}

inline void US_EnableIt(AT91S_USART *us,
						unsigned int flag)
{
  	us->US_IER = flag;
}

inline void DbugUsart_Configure(AT91S_DBGU *usart,
                            unsigned int mode,
                            unsigned int baudrate,
                            unsigned int masterClock)
{
	unsigned int val,res;
  	//Reset and disable receiver & transmitter
	usart->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RSTTX 
		| AT91C_US_RXDIS | AT91C_US_TXDIS;
	
	//Configure mode
	usart->DBGU_MR = mode;
	val = (masterClock >> 4) / baudrate;
	res = (masterClock >> 4) % baudrate;
	if(res >= (baudrate >> 1))
	{
		val++;
	}
	usart->DBGU_BRGR = val;
}

inline void DBUG_US_EnableIt(AT91S_DBGU *us,
						unsigned int flag)
{
	us->DBGU_IER = flag;
}

inline void Usart_InitReceviceBack(Serial_t *pUs,UsartCallback receviceback)
{
  	pUs->receviceback = receviceback;
}

inline void Usart_InitSendBack(Serial_t *pUs,UsartCallback sendback)
{
  	pUs->sendback = sendback;
}

inline void Usart_InitCrcBack(Serial_t *pUs,UsartCallback CrcBack)
{
  	pUs->CrcBack = CrcBack;
}

inline void Usart_InitTimeOut(Serial_t *pUs,UsartCallback TimeOut)
{
  	pUs->TimeOut = TimeOut;
}

void UsartStructInit(unsigned char len,...);

void Usart_InitCallback(Serial_t *pUs,
						UsartCallback receviceback,
						UsartCallback sendback,
						UsartCallback CrcBack,
						UsartCallback TimeOut);

void USART_ConfigStruct(Serial_t *pUs,AT91S_USART *pUsartHw);

int WriteToUsart(Serial_t *pUs,unsigned char *pBuffer,unsigned int len);

int ReadFromUsart(Serial_t *pUs,unsigned char *pBuffer,unsigned int len);

#endif