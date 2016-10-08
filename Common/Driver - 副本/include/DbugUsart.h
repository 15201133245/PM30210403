#ifndef _DbugUsart_H
#define _DbugUsart_H
#include "AT91SAM7SE512.h"
#include "usart.h"
#include <stdarg.h>
#include <stdio.h>


typedef struct _DbugSerial_t
{
	AT91S_DBGU *pUsartHw;
	UsartCallback receviceback;
	UsartCallback sendback;	
	unsigned char *pTransfer;
	unsigned int  TransferLen;
	unsigned char *pReceiver;
	unsigned int  ReceiveLen;
}DbugSerial_t;


int dbug_printf(const char *fmt,...);

void WriteToDBUG_USART(unsigned char *s,unsigned int len);

// Bellow LLLLA
//extern void DbugCashSentOver(unsigned char *s,unsigned int len,unsigned char error);

#endif