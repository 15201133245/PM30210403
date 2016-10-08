#include "DbugUsart.h"
#include <string.h>

//extern  DbugSerial_t DbugSerial;

#ifdef	USE_DBUG
unsigned char dbgu_buf[512];

int dbug_printf(const char *fmt,...)
{
    va_list args;
    int i;
    
    va_start(args,fmt);
    i = vsprintf((char *)dbgu_buf,fmt,args);
    va_end(args);
    WriteToDBUG_USART(dbgu_buf,i);
    return i;
}

void testCardStatus(void)
{
	extern	unsigned int CardStatus;
	dbug_printf("%s%08x\n\r","CardStatus:0x",CardStatus);
}


static unsigned int SendTimeCount;

void Pump_SendTimeOut(void)
{
	SendTimeCount++;
	dbug_printf("%s%0.10lu\n\r","SendTimeCount:",SendTimeCount);
}

static unsigned int InitM1172Count;

void InitM1172_Count(void)
{
	InitM1172Count++;
	dbug_printf("%s%0.10lu\n\r","InitM1172Count:",InitM1172Count);
}

void SendPowerDown(void)
{
	dbug_printf("%s\n\r","Save Last Trans and Power Down!");
}

void SendOver(void)
{
	dbug_printf("%s\n\r","Pump Usart OVER!!!");
}

#endif

void WriteToDBUG_USART(unsigned char *s,unsigned int len)
{
    AT91S_DBGU *pUsart;
    
    pUsart = AT91C_BASE_DBGU;
    pUsart->DBGU_PTCR = AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS;
    pUsart->DBGU_RPR = (unsigned int)s;
    pUsart->DBGU_RCR = 0;
    pUsart->DBGU_TPR = (unsigned int)s;
    pUsart->DBGU_TCR = len;
    
    pUsart->DBGU_IDR = AT91C_US_RXRDY | AT91C_US_RXBUFF;
    pUsart->DBGU_IER = AT91C_US_TXBUFE;//| AT91C_US_RXRDY;
    pUsart->DBGU_PTCR = AT91C_PDC_TXTEN;
}


