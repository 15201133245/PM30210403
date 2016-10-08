#include "usart.h"
#include "global.h"

void UsartStructInit(unsigned char len,...)
{
    Serial_t *pUs;
    va_list arg;
    
    va_start(arg,len);
    if ( len > 0 )
    {
        pUs = va_arg(arg,Serial_t *);
    }
    if ( len > 1 )
    {
        pUs->receviceback = va_arg(arg,UsartCallback);
    }
    if ( len > 2 )
    {
        pUs->sendback = va_arg(arg,UsartCallback);
    }
    if ( len > 3 )
    {
        pUs->CrcBack = va_arg(arg,UsartCallback);
    }
    if ( len > 4 )
    {
        pUs->TimeOut = va_arg(arg,UsartCallback);
    }
    if ( len > 5 )
    {
        pUs->TimeFlg = va_arg(arg,unsigned char);
    }
    if ( len > 6 )
    {
        pUs->pTransfer = va_arg(arg,unsigned char *);
    }
    if ( len > 7 )
    {
        pUs->TransferLen = va_arg(arg,unsigned int);
    }
    if ( len > 8 )
    {
        pUs->pReceiver = va_arg(arg,unsigned char *);
    }
    if ( len > 9 )
    {
        pUs->ReceiveLen = va_arg(arg,unsigned int);
    }
    if ( len > 10 )
    {
        pUs->HeadLen = va_arg(arg,unsigned char);
    }
    if ( len > 11 )
    {
        pUs->RevCom = va_arg(arg,unsigned char);
    }
    if ( len > 12 )
    {
        pUs->TrunChar = va_arg(arg,unsigned char);
    }
    if ( len > 13 )
    {
        pUs->ErrType = va_arg(arg,unsigned char);
    }
    if ( len > 14 )
    {
        pUs->pRevQue = va_arg(arg,unsigned char *);
    }
    va_end(arg);
}

void Usart_InitCallback(Serial_t *pUs,
						UsartCallback receviceback,
						UsartCallback sendback,
						UsartCallback CrcBack,
						UsartCallback TimeOut)
{
    Usart_InitReceviceBack(pUs,receviceback);
    Usart_InitSendBack(pUs,sendback); 
    Usart_InitCrcBack(pUs,CrcBack);
    Usart_InitTimeOut(pUs,TimeOut);
}

void USART_ConfigStruct(Serial_t *pUs,AT91S_USART *pUsartHw)
{
    pUs->pUsartHw = pUsartHw;
}

int WriteToUsart(Serial_t *pUs,unsigned char *pBuffer,unsigned int len)
{
    AT91S_USART *pUsart;
	unsigned int mode;
    
    pUsart = pUs->pUsartHw;
    pUs->pTransfer = pBuffer;
    pUs->TransferLen = len;
	if(pUsart == AT91C_BASE_US0)
	{
		PIO_SetPeripheralA(usart0.pio,usart0.mask,ENABLE_PULLUP);
		//PMC
		PMC_EnablePeripheral(AT91C_ID_US0);
		//AIC
		AIC_ConfigureIT(AT91C_ID_US0,
					USART0_INTERRUPT_PRIORITY |AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
					ISR_usart0);
		//US
		USART_ConfigStruct(&Serial0,AT91C_BASE_US0);
		
		mode = AT91C_US_USMODE_RS485 | AT91C_US_CHRL_8_BITS | 
		AT91C_US_NBSTOP_1_BIT | AT91C_US_PAR_NONE | AT91C_US_CLKS_CLOCK;
		/*mode = AT91C_US_USMODE_RS485 | AT91C_US_CHRL_8_BITS | 
		AT91C_US_NBSTOP_1_BIT | AT91C_US_PAR_NONE | AT91C_US_CLKS_CLOCK | AT91C_US_CHMODE_LOCAL;*/
		USART_Configure(AT91C_BASE_US0,mode,USART0_BAUD,MAIN_MCK);
		//WRITE_USART(Serial0.pUsartHw,US_CR,AT91C_US_RTSEN);
		PDC_DisableRx((AT91PS_PDC)(&AT91C_BASE_US0->US_RPR));
		PDC_DisableTx((AT91PS_PDC)(&AT91C_BASE_US0->US_RPR));
		//INT
		US_EnableIt(AT91C_BASE_US0, 
				AT91C_US_RXRDY | AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE);
		WRITE_USART(Serial0.pUsartHw,US_CR,AT91C_US_TXEN | AT91C_US_RXEN);
		AIC_EnableIT(AT91C_ID_US0);
	}
	else
	{
		//IO
		PIO_SetPeripheralA(usart1.pio,usart1.mask,ENABLE_PULLUP);
		//PMC
		PMC_EnablePeripheral(AT91C_ID_US1);
		//AIC
		AIC_ConfigureIT(AT91C_ID_US1,
					USART1_INTERRUPT_PRIORITY |AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
					ISR_usart1);
		//US
		USART_ConfigStruct(&Serial1,AT91C_BASE_US1);
		mode = AT91C_US_USMODE_NORMAL | AT91C_US_CHRL_8_BITS | 
		AT91C_US_NBSTOP_1_BIT | AT91C_US_PAR_NONE | AT91C_US_CLKS_CLOCK;
		/*mode = AT91C_US_USMODE_NORMAL | AT91C_US_CHRL_8_BITS | 
		AT91C_US_NBSTOP_1_BIT | AT91C_US_PAR_NONE | AT91C_US_CLKS_CLOCK | AT91C_US_CHMODE_LOCAL;*/
		USART_Configure(AT91C_BASE_US1,mode,USART1_BAUD,MAIN_MCK);
		PDC_DisableRx((AT91PS_PDC)(&AT91C_BASE_US1->US_RPR));
		PDC_DisableTx((AT91PS_PDC)(&AT91C_BASE_US1->US_RPR));
		//INT
		US_EnableIt(AT91C_BASE_US1, 
				AT91C_US_RXRDY | AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE);
		WRITE_USART(Serial1.pUsartHw,US_CR,AT91C_US_TXEN | AT91C_US_RXEN);
		AIC_EnableIT(AT91C_ID_US1);
		
	}
    
    WRITE_USART(pUsart,US_PTCR,AT91C_PDC_TXTDIS | AT91C_PDC_RXTDIS);
    
    WRITE_USART(pUsart,US_RPR,(unsigned int)pUs->pReceiver);
    WRITE_USART(pUsart,US_RCR,pUs->ReceiveLen);
    WRITE_USART(pUsart,US_TPR,(unsigned int)pUs->pTransfer);
    WRITE_USART(pUsart,US_TCR,pUs->TransferLen);
    if ( pUs->ReceiveLen == 0 )
    {
		WRITE_USART(pUsart,US_IER,AT91C_US_TXBUFE | AT91C_US_RXRDY);
        WRITE_USART(pUsart,US_IER,AT91C_US_TXBUFE |
        		AT91C_US_RXRDY |
        		  AT91C_US_OVRE |
        			AT91C_US_FRAME |
        			  AT91C_US_PARE );
        WRITE_USART(pUsart,US_CR,AT91C_US_TXEN | AT91C_US_RXEN);
        WRITE_USART(pUsart,US_PTCR,AT91C_PDC_TXTEN);
    }
    else
    {
        WRITE_USART(pUsart,US_IDR,AT91C_US_RXRDY);
        WRITE_USART(pUsart,US_IER,AT91C_US_TXBUFE |
        		AT91C_US_RXBUFF | 
        		  AT91C_US_OVRE |
        			AT91C_US_FRAME |
        			  AT91C_US_PARE);
        WRITE_USART(pUsart,US_PTCR,AT91C_PDC_RXTEN | AT91C_PDC_TXTEN);
    }
    //WRITE_USART(pUsart,US_CR,AT91C_US_TXEN | AT91C_US_RXEN);
    return pUs->TransferLen;
}

int ReadFromUsart(Serial_t *pUs,unsigned char *pBuffer,unsigned int len)
{
	pUs = pUs;
	pBuffer = pBuffer;
	len = len;
	return 0;
    /*unsigned int i,length;
    Queue *pQue;
    if ( pUs->pRevQue == (unsigned char *)0 )
    {
        return POINTER_ERR;
    }
    pQue = (Queue *)(pUs->pRevQue);
    length = GetQueCharNumber(pQue);
    if ( length > len )
    {
        length = len;
    }
    for ( i = 0; i < length; i++ )
    {
        if ( ReadFromQue(pQue,&pBuffer[i]) )
        {
            return POINTER_ERR;
        }
    }
    return length;*/
}