#include "global.h"

void Sys_irq(void)
{
    unsigned int status;
    unsigned int imr;
    unsigned int val;
    unsigned char rhr;
	AT91S_DBGU *pDbugUsart;
	DbugSerial_t *pDbug;
    
    status = AT91C_BASE_PITC->PITC_PISR;
    if ( status & AT91C_PITC_PITS )
    {
        val = AT91C_BASE_PITC->PITC_PIVR;
        val >>= 20;
        pit_val.current_val += val;
    }
	pDbug = &DbugSerial;
	pDbugUsart = pDbug->pUsartHw;
	imr = pDbugUsart->DBGU_IMR;
	status = pDbugUsart->DBGU_CSR;
    if ( status & AT91C_US_RXRDY )
    {
        rhr = AT91C_BASE_DBGU->DBGU_RHR;
		if(pDbug->receviceback)
		{
			pDbug->receviceback(&rhr,1,0);
		}
    }
    if ( (status & AT91C_US_TXBUFE) && (imr & AT91C_US_TXBUFE) )
    {
        pDbugUsart->DBGU_IDR = AT91C_US_TXBUFE;
		if(pDbug->sendback)
		{
			pDbug->sendback(0,0,0);
		}
    }
    if ( (status & AT91C_US_RXBUFF) && (imr & AT91C_US_RXBUFF) )
    {
		if(pDbug->receviceback)
		{
			pDbug->receviceback(pDbug->pReceiver,pDbug->ReceiveLen,0);
		}
    }
    AT91C_BASE_AIC->AIC_EOICR = 0;
}

void ISR_irq0(void)
{
	CardInInt++;
	AT91C_BASE_AIC->AIC_EOICR = 0;
}

void ISR_irq1(void)
{
	spi_status++;
    AT91C_BASE_AIC->AIC_EOICR = 0;
}

void ISR_fiq(void)
{
	PIO_SetInput(fiq.pio,fiq.mask,DISABLE_PULLUP,ENABLE_FILETER);
	PMC_EnablePeripheral(AT91C_ID_PIOB);
	AT91C_BASE_AIC->AIC_EOICR = 0;
	AIC_DisableIT(AT91C_ID_FIQ);
	AT91C_BASE_AIC->AIC_EOICR = 0;
	CreateTimer(TIMER_ONE_DELETE,50,PowerDownCheck);
}

/*void ISR_fiq(void)
{
	spi_status = 0;
	AIC_DisableIT(AT91C_ID_IRQ1);
	PowerDownFlg = 1;
	PIO_SetInput(fiq.pio,fiq.mask,DISABLE_PULLUP,ENABLE_FILETER);
	PMC_EnablePeripheral(AT91C_ID_PIOB);
	AT91C_BASE_AIC->AIC_EOICR = 0;
	AIC_DisableIT(AT91C_ID_FIQ);
	AT91C_BASE_AIC->AIC_EOICR = 0;
}*/

void ISR_usart0(void)
{
    Serial_t *pUs;
    AT91S_USART *pUsart;
    volatile unsigned int usartSr;
    volatile unsigned int usartImr;
    unsigned char val,error = 0;
    
    pUs = &Serial0;
    pUsart = Serial0.pUsartHw;
    usartSr = READ_USART(pUsart,US_CSR);
    usartImr = READ_USART(pUsart,US_IMR);
    if ( (usartSr & AT91C_US_TXBUFE) && (usartImr & AT91C_US_TXBUFE) )
    {
        WRITE_USART(pUsart,US_IDR,AT91C_US_TXBUFE );
		WRITE_USART(pUsart,US_CR,AT91C_US_TXDIS );
        if ( Serial0.sendback )
        {
            Serial0.sendback(0,0,0);
        }
    }
    if ( (usartSr & AT91C_US_RXBRK) && (usartImr & AT91C_US_RXBRK) )
    {
        error |= AT91C_US_RXBRK;
        WRITE_USART(pUsart,US_IDR,AT91C_US_RXBRK );
    }
    if ( (usartSr & AT91C_US_OVRE) && (usartImr & AT91C_US_OVRE) )
    {
        error |= AT91C_US_OVRE;
        WRITE_USART(pUsart,US_IDR,AT91C_US_OVRE );
    }
    if ( (usartSr & AT91C_US_FRAME) && (usartImr & AT91C_US_FRAME) )
    {
        error |= AT91C_US_FRAME;
        WRITE_USART(pUsart,US_IDR,AT91C_US_FRAME );
    }
    if ( (usartSr & AT91C_US_PARE) && (usartImr & AT91C_US_PARE) )
    {
        error |= AT91C_US_PARE;
        WRITE_USART(pUsart,US_IDR,AT91C_US_PARE );
    }
	if ( (usartSr & AT91C_US_RXRDY) && (usartImr & AT91C_US_RXRDY) )
	{
		val = READ_USART(pUsart,US_RHR);
		
		/*if((Serial0.pReceiver) && (error == 0))
		{
			WriteToQue(&quePump,val);
		}
		else if(error)
		{
			UsartError = error;
		}*/
		if ( Serial0.receviceback )
		{
			Serial0.receviceback(&val,1,error);
		}
	}
	if ( (usartSr & AT91C_US_RXBUFF) && (usartImr & AT91C_US_RXBUFF) )
    {
		WRITE_USART(pUsart,US_IDR,AT91C_US_RXBUFF);
        if ( Serial0.receviceback )
        {
            Serial0.receviceback(pUs->pReceiver,pUs->ReceiveLen,0);
        }
    }
    AT91C_BASE_AIC->AIC_EOICR = 0;
}

void ISR_usart1(void)
{
	if(IcTypeFlg)
	{
		Iso7816 *pIso;
		AT91S_USART *pUsart;
		volatile unsigned int usartSr;
		volatile unsigned int usartImr;
		unsigned char val;
		unsigned char rx = 0;
		unsigned char error = 0;
		
		pIso = &iso;
		pUsart = pIso->pUsartHw;
		usartSr = READ_USART(pUsart,US_CSR);
		usartImr = READ_USART(pUsart,US_IMR);
		if(usartSr & AT91C_US_ITERATION)
		{
			AT91C_BASE_US1->US_CR = AT91C_US_RSTSTA | AT91C_US_RSTIT | AT91C_US_RSTNACK;
			WRITE_USART(pUsart,US_PTCR,AT91C_PDC_TXTDIS | AT91C_PDC_RXTDIS);
			error = 1;
		}
		if ( (usartSr & AT91C_US_RXBUFF) && (usartImr & AT91C_US_RXBUFF) )
		{
			rx = 1;
			WRITE_USART(pUsart,US_IDR,AT91C_US_RXBUFF);
			if ( pIso->receviceback )
			{
				pIso->receviceback(pIso->pReceiver,pIso->ReceiveLen,error);
			}
		}
		if ( (usartSr & AT91C_US_TXBUFE) && (usartImr & AT91C_US_TXBUFE) )
		{
			WRITE_USART(pUsart,US_IDR,AT91C_US_TXBUFE );
			/*WRITE_USART(pUsart,US_PTCR,AT91C_PDC_TXTDIS | AT91C_PDC_RXTDIS);
			while(!(usartSr & AT91C_US_TXEMPTY))
			{
				usartSr = READ_USART(pUsart,US_CSR);
			}*/
			
			if ( pIso->sendback )
			{
				pIso->sendback(0,0,error);
			}
		}
		if((usartSr & AT91C_US_TXRDY) && (usartImr & AT91C_US_TXRDY))
		{
			if ( pIso->sendback )
			{
				pIso->sendback(0,1,error);
			}
		}
		if(!rx)
		{
			if ( usartSr & AT91C_US_RXRDY )
			{
				val = READ_USART(pUsart,US_RHR);
				if ( pIso->receviceback )
				{
					pIso->receviceback(&val,1,error);
				}
			}
		}
	}
	else
	{
		Serial_t *pUs;
		AT91S_USART *pUsart;
		volatile unsigned int usartSr;
		volatile unsigned int usartImr;
		unsigned char val,error = 0;
		unsigned char rx = 0;
		
		pUs = &Serial1;
		pUsart = Serial1.pUsartHw;
		usartSr = READ_USART(pUsart,US_CSR);
		usartImr = READ_USART(pUsart,US_IMR);
		if ( (usartSr & AT91C_US_RXBUFF) && (usartImr & AT91C_US_RXBUFF) )
		{
			WRITE_USART(pUsart,US_IDR,AT91C_US_RXBUFF);
			if ( Serial1.receviceback )
			{
				Serial1.receviceback(pUs->pReceiver,pUs->ReceiveLen,0);
			}
		}
		if ( (usartSr & AT91C_US_TXBUFE) && (usartImr & AT91C_US_TXBUFE) )
		{
			rx = 1;
			WRITE_USART(pUsart,US_IDR,AT91C_US_TXBUFE );
			if ( Serial1.sendback )
			{
				Serial1.sendback(0,0,0);
			}
		}
		if ( usartSr & AT91C_US_RXBRK )
		{
			error = AT91C_US_RXBRK;
			WRITE_USART(pUsart,US_IDR,AT91C_US_RXBRK);
		}
		if ( usartSr & AT91C_US_OVRE )
		{
			error = AT91C_US_OVRE;
			WRITE_USART(pUsart,US_IDR,AT91C_US_OVRE);
		}
		if ( usartSr & AT91C_US_FRAME )
		{
			error = AT91C_US_FRAME;
			WRITE_USART(pUsart,US_IDR,AT91C_US_FRAME);
		}
		if ( usartSr & AT91C_US_PARE )
		{
			error = AT91C_US_PARE;
			WRITE_USART(pUsart,US_IDR,AT91C_US_PARE);
		}
		if(!rx)
		{
			if ( usartSr & AT91C_US_RXRDY )
			{
				val = READ_USART(pUsart,US_RHR);
				if ( Serial1.receviceback )
				{
					Serial1.receviceback(&val,1,error);
				}
			}
		}
	}
	AT91C_BASE_AIC->AIC_EOICR = 0;
}

void ISR_timer0(void)
{
    AT91C_BASE_TC0->TC_SR;
    timer_val.current_val++;
	timer1_val.current_val++;
    AT91C_BASE_AIC->AIC_EOICR = 0;
}

void ISR_timer1(void)
{
    AT91C_BASE_TC1->TC_SR;
    if ( pTimer1!= (pTimeFunc )0 )
    {
        pTimer1();
    }
    AT91C_BASE_AIC->AIC_EOICR = 0;
}

void ISR_Spi(void)
{
    SPI_Handler(&spid);
    AT91C_BASE_AIC->AIC_EOICR = 0;
}
