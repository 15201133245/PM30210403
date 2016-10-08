#include "m1172.h"
#include "tc.h"
#include "DbugUsart.h"

void WriteToQue(Queue *pQue,unsigned char val)
{
    if ( !Que_IsFull(pQue) )
    {
        pQue->buf[pQue->tail] = val;
        pQue->tail = (pQue->tail + 1) & (BUF_MAX - 1);
    }
}

unsigned char ReadFromQue(Queue *pQue,unsigned char *p)
{
    unsigned int len;
    
    len = GetQueCharNumber(pQue);
    if ( len == 0 )
    {
        return 1;
    }
    pQue->head = (pQue->head + 1) & (BUF_MAX - 1);
    *p = pQue->buf[pQue->head];
    return 0;
}

unsigned char Que_IsFull(Queue *pQue)
{
    if ( pQue->head == pQue->tail )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char Que_IsEmpty(Queue *pQue)
{
    if ( GetQueCharNumber(pQue) )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned int GetQueCharNumber(Queue *pQue)
{
    unsigned int len;
    if ( pQue->head < pQue->tail )
    {
        len = pQue->tail - pQue->head - 1;
    }
    else
    {
        len = BUF_MAX + pQue->tail - pQue->head - 1;
    }
    return len;
}

unsigned char M1172_Configure(M1172 *pM1172,Spid *pSpid,unsigned char spiCs)
{
    SpidCmd *pCommand;
    
    //Initialize the At45  instance;
    pM1172->pSpid = pSpid;
    memset(pM1172->pCmdBuffer,0,8);
    
    //Iniialize the spidCmd structure
    pCommand = &(pM1172->command);
    pCommand->pCmd = pM1172->pCmdBuffer;
    pCommand->callback = 0;
    pCommand->pArgument = 0;
    pCommand->spiCs = spiCs;
    pM1172->pChannel = 0;
    return 0;
}

void SpiUsartConfigure(unsigned char len,...)
{
    M1172 *pM1172;
    M1172_ChCtrl *pChannel;
    va_list arg;
    
    va_start(arg,len);
    if ( len > 0 )
    {
        pM1172 = va_arg(arg,M1172 *);
        pChannel = pM1172->pChannel;
    }
    if ( len > 1 )
    {
        pChannel->channelStatus = va_arg(arg,unsigned char);
    }
    if ( len > 2 )
    {
        pChannel->txStatus = va_arg(arg,unsigned char);
    }
    if ( len > 3 )
    {
        pChannel->rxStatus = va_arg(arg,unsigned char);
    }
    if ( len > 4 )
    {
        pChannel->channel = va_arg(arg, unsigned char);
    }
    if ( len > 5 )
    {
        pChannel->baud = va_arg(arg,unsigned int);
    }
    if ( len > 6 )
    {
        pChannel->parity = va_arg(arg,unsigned char);
    }
    if ( len > 7 )
    {
        pChannel->data_length = va_arg(arg,unsigned char);
    }
    if ( len > 8 )
    {
        pChannel->stop = va_arg(arg,unsigned char);
    }
    if ( len > 9 )
    {
        pChannel->TxFIFOLvl = va_arg(arg,unsigned char);
    }
    if ( len > 10 )
    {
        pChannel->RxFIFOLvl = va_arg(arg,unsigned char);
    }
    if ( len > 11 )
    {
        pChannel->efcr_mode = va_arg(arg,unsigned char);
    }
    if ( len > 12 )
    {
        pChannel->TxBuf = va_arg(arg,unsigned char *);
    }
    if ( len > 13 )
    {
        pChannel->RxBuf = va_arg(arg,unsigned char *);
    }
    if ( len > 14 )
    {
        pChannel->TxLen = va_arg(arg,unsigned int);
    }
    if ( len > 15 )
    {
        pChannel->RxLen = va_arg(arg,unsigned int);
    }
    if ( len > 16 )
    {
        pChannel->sendback = va_arg(arg,Callback);
    }
    if ( len > 17 )
    {
        pChannel->receviceback = va_arg(arg,Callback);
    }
    va_end(arg);
}

static void M1172_CaculateBaud(unsigned int baud,
						unsigned char *pdll,
						unsigned char *pdlm,
						unsigned char *pdld)
{
    unsigned int i,j;
    
    i = M1172_OSC / (baud * 16);
    j = M1172_OSC % (baud * 16);
    j /= baud;
    
    *pdll = i & 0xff;
    *pdlm = i >> 8;
    *pdld = j;
}

void M1172_ConfigureChannel(M1172_ChCtrl *pChannel,unsigned char ier,unsigned char efr)
{
    unsigned char dll;
    unsigned char dlm;
    unsigned char dld;
    unsigned char val;
    
    M1172_CaculateBaud(pChannel->baud,&dll,&dlm,&dld);
    
    M1172_WriteReg(pChannel->channel,REG_LCR,0xbf);
    M1172_WriteReg(pChannel->channel,REG_EFR,0x10);
    M1172_WriteReg(pChannel->channel,REG_LCR,0x80);
    M1172_WriteReg(pChannel->channel,REG_DLM,dlm);
    M1172_WriteReg(pChannel->channel,REG_DLL,dll);
    M1172_WriteReg(pChannel->channel,REG_DLD,dld);
	M1172_WriteReg(pChannel->channel,REG_LCR,0xbf);
    M1172_WriteReg(pChannel->channel,REG_EFR,efr);
	M1172_ReadReg(pChannel->channel,REG_EFR,&val);
    M1172_WriteReg(pChannel->channel,REG_LCR,
    		   pChannel->parity | 
    			 pChannel->data_length |
    			   pChannel->stop);
    M1172_WriteReg(pChannel->channel,REG_FCR,0x07 |
    		   pChannel->TxFIFOLvl |
    			 pChannel->RxFIFOLvl);
    M1172_WriteReg(pChannel->channel,REG_IER,ier);
    M1172_WriteReg(pChannel->channel,REG_EFCR,
    		   pChannel->efcr_mode);
    M1172_ReadReg(pChannel->channel,REG_ISR,&val);
}

unsigned char M1172_SendCommand(M1172 *pM1172,
								unsigned char cmd,
								unsigned char *pdata,
								unsigned int dataSize)
{
    SpidCmd *pCommand;
    if ( M1172_IsBusy(pM1172) )
    {
        return M1172_ERROR_LOCK;
    }
    pM1172->pCmdBuffer[0] = cmd;
    pCommand = &(pM1172->command);
    
    pCommand->cmdSize = 1;
    pCommand->pData = pdata;
    pCommand->dataSize = dataSize;
    if ( SPID_SendCommand(pM1172->pSpid,pCommand) )
    {
        return M1172_ERROR_SPI;
    }
    return 0;
}

unsigned char M1172_IsBusy(M1172 *pM1172)
{
    return (SPID_IsBusy(pM1172->pSpid));
}

unsigned char M1172_WriteReg(unsigned char channel,
							 unsigned char reg,
							 unsigned char val)
{
    unsigned char error;
    unsigned char status;
    M1172 *pM1172;
    
    pM1172 = &m1172;
    status = val;
    error = M1172_SendCommand(pM1172,
    					  (CMD_WRITE | channel | reg),
    					  &status,1);
    if ( error )
    {
        return error;
    }
    while ( M1172_IsBusy(pM1172) )
    {
    }
    return 0;
}

unsigned char M1172_ReadReg(unsigned char channel,
							unsigned char reg,
							unsigned char *p)
{
    unsigned char error;
    M1172 *pM1172;
    
    pM1172 = &m1172;
    
    error = M1172_SendCommand(pM1172,
    					  (CMD_READ | channel | reg),p,1);
    if ( error )
    {
        return error;
    }
    while ( M1172_IsBusy(pM1172) )
    {
    }
    return 0;
}

unsigned char M1172_LoadToTxFIFO(M1172 *pM1172)
{
    unsigned char error;
    unsigned char len,length;
    unsigned char *p;
	unsigned char buf[64];
    M1172_ChCtrl *pChannel;
	extern	unsigned char PrintBusy;
    
	p = buf;
    pChannel = pM1172->pChannel;
    if ( pChannel->TxLen == 0 )
    {
        pChannel->channelStatus = IDLE;
        if ( pChannel->txStatus )
        {
            M1172_ReadReg(pChannel->channel,REG_EFCR,&len);
            length = 0x40;
            while ( length )
            {
                M1172_ReadReg(pChannel->channel,REG_LSR,&length);
                length &= 0x40;
            }
            M1172_WriteReg(pChannel->channel,REG_EFCR,len & 0xfd);
        }
        M1172_ReadReg(pChannel->channel,REG_LSR,&length);
		if(pChannel->channel == CHA)
		{
			while ( (length & 0x60) != 0x60 )
			{
				M1172_ReadReg(pChannel->channel,REG_LSR,&length);
			}
		}
		else if((pChannel->channel == CHB) && (PrintBusy == 0))
		{
			while ( (length & 0x60) != 0x60 )
			{
				M1172_ReadReg(pChannel->channel,REG_LSR,&length);
			}
		}
        if ( pChannel->sendback )
        {
            pChannel->sendback(0,0);
        }
		if(pChannel->channel == CHA)
		{
			//extern	Queue 	queue;
			pChannel->efcr_mode = M1172_UART_AUTO485_DISABLE;
        	M1172_ConfigureChannel(pChannel,0x07,0x10);
			//pChannel->RxBuf = (unsigned char *)(&queue);
		}
		else
		{
			M1172_ConfigureChannel(pChannel,0x07,0x90);
		}
        return 0;
    }
    error = M1172_ReadReg(pChannel->channel,REG_TXLVL,&len);
    if ( error )
    {
        return error;
    }
    while ( M1172_IsBusy(pM1172) )
    {
    }
    if ( pChannel->TxLen > len )
    {
        length = len;
        memcpy(p,pChannel->TxBuf,length);
        pChannel->TxBuf += length;
        pChannel->TxLen -= length;
    }
    else
    {
        length = pChannel->TxLen;
        memcpy(p,pChannel->TxBuf,length);
        pChannel->TxBuf = (unsigned char *)0;
        pChannel->TxLen = 0;
    }
    error = M1172_SendCommand(pM1172,
    						  (CMD_WRITE | pChannel->channel | REG_THR),
    						  p,length);
    if ( error )
    {
        return error;
    }
    while ( M1172_IsBusy(pM1172) )
    {
    }
    return 0;
}

unsigned char M1172_SaveFromRxFIFO(M1172 *pM1172)
{
    unsigned char error;
    unsigned char i,len;
    unsigned char val;
    M1172_ChCtrl *pChannel;
    Queue *pQue;
    
    pChannel = pM1172->pChannel;
    pQue = (Queue *)(pChannel->RxBuf);
    if ( pChannel->RxBuf == (unsigned char *)0 )
    {
        return (unsigned char)POINTER_ERR;
    }
    error = M1172_ReadReg(pChannel->channel,REG_RXLVL,&len);
    if ( error )
    {
        return error;
    }
    while ( M1172_IsBusy(pM1172) )
    {
    }
    for ( i = 0; i < len; i++ )
    {
        error = M1172_ReadReg(pChannel->channel,REG_RHR,&val);
        if ( error )
        {
            return error;
        }
        WriteToQue(pQue,val);
    }
    return 0;
}

int WriteToSpiUsart(M1172 *pM1172,unsigned char *pBuffer,unsigned int len)
{
    unsigned char error;
    unsigned char status;
    M1172_ChCtrl *pChannel;
    
    pChannel = pM1172->pChannel;
    error = M1172_ReadReg(pChannel->channel,
    				  REG_ISR,&status);
    if ( error )
    {
        return (-error);
    }
    error = M1172_ReadReg(pChannel->channel,
    					  REG_TXLVL,&status);
    if ( error )
    {
        return (-error);
    }
    if ( pChannel->channelStatus == IDLE )
    {
        pChannel->channelStatus = TX_STATUS;
        pChannel->TxBuf = pBuffer;
        pChannel->TxLen = len;
        if ( pChannel->txStatus )
        {
            M1172_ReadReg(pChannel->channel,REG_EFCR,&status);
            M1172_WriteReg(pChannel->channel,REG_EFCR,status | 0x02);
        }
        error = M1172_LoadToTxFIFO(pM1172);
    }
    else
    {
        if ( pChannel->channelStatus == TX_STATUS )
        {
            error = M1172_LoadToTxFIFO(pM1172);
            if ( error )
            {
                return (-error);
            }
        }
    }
    return len;
}

int ReadFromSpiUsart(M1172 *pM1172,unsigned char *pBuffer,unsigned int len)
{
    M1172_ChCtrl *pChannel;
    Queue *pQue;
    unsigned int i,length;
    
    pChannel = pM1172->pChannel;
    pQue = (Queue *)(pChannel->RxBuf);
    if ( pChannel->RxBuf == (unsigned char *)0 )
    {
        return POINTER_ERR;
    }
    M1172_SaveFromRxFIFO(pM1172);
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
    return length;
}

extern	void InitM1172(unsigned char typ);
extern	void InitM1172_Count(void);

void SpiUsart(void)
{
    extern Spid 	spid;
	extern M1172_ChCtrl chCtrl[];
	extern volatile unsigned char spi_status;
    M1172 *pM1172;
    M1172_ChCtrl *pChannel;
    unsigned char i,error;
    unsigned char status;
    
    if (( spi_status ) && (spi_status != 0xff))
    {
		pM1172 = &m1172;
    	SPID_ConfigureCS(&spid, 2, SPI_CSR(MAIN_MCK, SPCK));
        for ( i = 0; i < 2; i++ )
        {
            pM1172->pChannel = &chCtrl[i];
            pChannel = pM1172->pChannel;
            error = M1172_ReadReg(pChannel->channel,REG_ISR,&status);
            if ( error )
            {
                return ;
            }
			if ( (status & 0x01) == 0 )
            {
                if ( (status & 0x3f) == M1172_RX_TIMEOUT )
                {
                    if ( pChannel->receviceback )
                    {
                        pChannel->receviceback((unsigned char *)pM1172,0);
                    }
                    else
                    {
                        if ( pChannel->RxBuf != NULL )
                        {
                            M1172_SaveFromRxFIFO(pM1172);
                        }
                    }
                }
                if ( (status & 0x3f) == M1172_RXRDY )
                {
                    //if(((status & 0x3f) == M1172_RXRDY) || ((status & 0x3f) == M1172_RX_TIMEOUT))
                    if ( pChannel->receviceback )
                    {
                        pChannel->receviceback((unsigned char *)pM1172,0);
                    }
                    else
                    {
                        if ( pChannel->RxBuf != NULL )
                        {
                            M1172_SaveFromRxFIFO(pM1172);
                        }
                    }
                }
                if ( (status & 0x3f) == M1172_TXRDY )
                {
                    error = M1172_LoadToTxFIFO(pM1172);
                    if ( error )
                    {
                        return ;
                    }
                }
                if ( (status & 0x3f) == M1172_MSR )
                {
                }
                if ( (status & 0x3f) == M1172_GPIO )
                {
                }
                if ( (status & 0x3f) == M1172_RX_Xoff )
                {
                }
                if ( (status & 0x3f) == M1172_CTS )
                {
                }
                if ( (status & 0x3f) == M1172_LSR )
                {
                    error = M1172_ReadReg(pChannel->channel,REG_LSR,&status);
                    if ( error )
                    {
                        return ;
                    }
                    if ( (status & 0x0f) == 0x01 )
                    {
                        if ( pChannel->receviceback )
                        {
                            pChannel->receviceback((unsigned char *)pM1172,0);
                        }
                        else
                        {
                            if ( pChannel->RxBuf != NULL )
                            {
                                M1172_SaveFromRxFIFO(pM1172);
                            }
                        }
                    }
                    else
                    {
                        if ( ((status & 0x02) == 0x02) || ((status & 0x80) == 0x80) )
                        {
                            if ( pChannel->receviceback )
                            {
                                pChannel->receviceback((unsigned char *)pM1172,0);
                            }
                            else
                            {
                                if ( pChannel->RxBuf != NULL )
                                {
                                    M1172_SaveFromRxFIFO(pM1172);
                                }
                            }
                        }
                    }
                }
            }
			else if((i == 0) && ((status & 0xc0) != 0xc0))
			{
				InitM1172(1);
#ifdef USE_DBUG
				InitM1172_Count();
#endif
			}
            else
            {
                if ( pChannel->channelStatus == TX_STATUS )
                {
                    error = M1172_LoadToTxFIFO(pM1172);
                    if ( error )
                    {
                        return ;
                    }
                }
				if ( pChannel->receviceback )
				{
					pChannel->receviceback((unsigned char *)pM1172,0);
				}
				else
				{
					if ( pChannel->RxBuf != NULL )
					{
						M1172_SaveFromRxFIFO(pM1172);
					}
				}
            }
        }
        spi_status--;
    }
}

void M1172Check(void)
{
	extern volatile unsigned char spi_status;
	
	if(!spi_status)
	{
		spi_status++;
	}
}