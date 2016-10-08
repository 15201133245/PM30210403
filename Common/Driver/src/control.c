#include "global.h"

unsigned char ReadCardStatus(void)
{
	PIO pio;
	
	pio.pio = AT91C_BASE_PIOB->PIO_PDSR;
	if(pio.P22 == 0)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

unsigned char ReadCardConfig(void)
{
	PIO pio;
	
	pio.pio = AT91C_BASE_PIOB->PIO_PDSR;
	if(pio.P0 == 0)
	{
		return(0);
	}
	else
	{
		return(1);
	}
}

void SetCardOut1(void)
{
	CardFlg = 0;
}

void SetCardOut(void)
{
	PIO_Set(&card_open);
	CardFlg = 2;
	CreateTimer(TIMER_ONE_DELETE,750,SetCardOut1);
}

void CardOut(void)
{	
	if(!CardFlg)
	{
		PIO_Clear(&card_open);
		CardFlg = 1;
		CreateTimer(TIMER_ONE_DELETE,50,SetCardOut);
		while(CardFlg == 1)
		{
			CheckTimerGo();
		}
	}
}


unsigned char CheckPrint(void)
{
	PIO pio;
	
	pio.pio = AT91C_BASE_PIOA->PIO_PDSR;
	if(pio.P25 == 0)
	{
		return(0);
	}
	else
	{
		return(1);
	}
}

unsigned char CheckPowerDownPin(void)
{
	PIO pio;
	
	pio.pio =  AT91C_BASE_PIOB->PIO_PDSR;
	if(pio.P19 == 0)
	{
		return(0);
	}
	else
	{
		return(1);
	}
}

void RunLed(void)
{
    PIO pio;
    
    pio.pio = AT91C_BASE_PIOA->PIO_ODSR;
    if ( pio.P0 == 0 )
    {
        PIO_Set(&run_led);
    }
    else
    {
        PIO_Clear(&run_led);
    }
}

void BeepOpen(void)
{
    PIO_Clear(&beep);
}

void BeepClose(void)
{
    PIO_Set(&beep);
    BeepFlg = 0;
}

void BeepClose1(void)
{
    PIO_Set(&beep);
    BeepFlg = 2;
}

void ClearBeepFlg(void)
{
    BeepFlg = 0;
}

Tmr *BeepControl(unsigned int time)
{
    BeepFlg = 1;
    BeepOpen();
    return (CreateTimer(TIMER_ONE_DELETE,time,BeepClose));
}

Tmr *BeepControl2(unsigned int time)
{
    BeepFlg = 1;
    BeepOpen();
    return (CreateTimer(TIMER_ONE_DELETE,time,BeepClose1));
}

Tmr *BeepDelay(unsigned int time)
{
    BeepFlg = 3;
    return (CreateTimer(TIMER_ONE_DELETE,time,ClearBeepFlg));
}

void EnableLcdBlackLight(void)
{
    PIO_Clear(&lcd_bl);
}

//void DisableLcdBlackLight(void *p)
void DisableLcdBlackLight()
{
    PIO_Set(&lcd_bl);
}

void OpenLcdLight(unsigned char typ)
{
	PIO_Clear(&lcd_bl);
	if(typ == 0)
	{
		if(pLcdTmr)
		{
			ResetTimer(pLcdTmr);
		}
		LcdLightFlg |= LCD_LIGHT_30S;
	}
	else
	{
		if(pLcdTmr)
		{
			StopTimer(pLcdTmr);
		}
		LcdLightFlg |= LCD_LIGHT_OPEN;
	}
}

void CloseLcdLightOil(void)
{
	if((LcdLightFlg & LCD_LIGHT_OPEN) == LCD_LIGHT_OPEN)
	{
		LcdLightFlg &= (~LCD_LIGHT_OPEN);
		OpenLcdLight(0);
	}
}

void CloseLcdLight(void)
{
	if((LcdLightFlg & LCD_LIGHT_30S)== LCD_LIGHT_30S)
	{
		LcdLightFlg &= (~LCD_LIGHT_30S);
		if(!LcdLightFlg )
		{
			PIO_Set(&lcd_bl);
		}
	}
}

void LcdBackLightControl(void)
{
	if(pLcdTmr == (Tmr *)0)
	{
		OpenLcdLight(0);
		pLcdTmr = CreateTimer(TIMER_ONE,LIGHT_TIME,CloseLcdLight);
	}
	else
	{
		OpenLcdLight(0);
		ResetTimer(pLcdTmr);
	}
}

void LCD_BlackLightControl(unsigned int time)
{
    EnableLcdBlackLight();
    CreateTimer(TIMER_ONE_DELETE,time,DisableLcdBlackLight);
}

void HexTimeToComBcdTime(void)
{
    char buf[5];
    unsigned int tmp;
    unsigned int i;
    unsigned char TmpBuf[20];
    
    
    memset(&SysBCDTime,0,sizeof(Time));
    tmp = SysTime.year;
    sprintf(buf,"%.4ld",tmp);
    memcpy(TmpBuf,buf,4);
    TmpBuf[4] = '-';
    for ( i = 0; i < 4; i++ )
    {
        SysBCDTime.year <<= 4;
        SysBCDTime.year += (buf[i] - '0');
    }
    tmp = SysTime.month;
    sprintf(buf,"%.2ld",tmp);
    memcpy(&TmpBuf[5],buf,2);
    TmpBuf[7] = '-';
    SysBCDTime.month = buf[0] - '0';
    SysBCDTime.month <<= 4;
    SysBCDTime.month += buf[1] - '0';
    tmp = SysTime.day;
    sprintf(buf,"%.2ld",tmp);
    memcpy(&TmpBuf[8],buf,2);
    TmpBuf[10] = ' ';
    SysBCDTime.day = buf[0] - '0';
    SysBCDTime.day <<= 4;
    SysBCDTime.day += buf[1] - '0';
    tmp = SysTime.hour;
    sprintf(buf,"%.2ld",tmp);
    memcpy(&TmpBuf[11],buf,2);
    TmpBuf[13] = ':';
    SysBCDTime.hour = buf[0] - '0';
    SysBCDTime.hour <<= 4;
    SysBCDTime.hour += buf[1] - '0';
    tmp = SysTime.minute;
    sprintf(buf,"%.2ld",tmp);
    memcpy(&TmpBuf[14],buf,2);
    TmpBuf[16] = ':';
    SysBCDTime.minute = buf[0] - '0';
    SysBCDTime.minute <<= 4;
    SysBCDTime.minute += buf[1] - '0';
    tmp = SysTime.second;
    sprintf(buf,"%.2ld",tmp);
    memcpy(&TmpBuf[17],buf,2);
    TmpBuf[19] = '\0';
    SysBCDTime.second = buf[0] - '0';
    SysBCDTime.second <<= 4;
    SysBCDTime.second += buf[1] - '0';
	if(IdleFlg & 0x02)
	{
		DisplayOnLine();
    	DisplayString4x5(42,2,TmpBuf);
		buf[0] = Rs485Adr + 1 + '0';
		buf[1] = '\0';
		DisplayString4x5(36,2,(unsigned char *)buf);
    	DisBufToLcd();
	}
	CheckUpdatePriceTime();
	OpenOrCloseLcdLight();
}

static unsigned char IdleFlgSta;

void SysTimer(void)
{
    unsigned char leap = 0;
    unsigned int val;
    
    val = pit_val.current_val - pit_val.last_val;
	if(IdleFlgSta != IdleFlg)
	{
		if(IdleFlg & 0x02)
		{
			ClearLcd(1,1,126,7);
			DrawHorizontalLine(0, 128, 8);
			DrawVerticalline(40, 0, 8);
		}
		IdleFlgSta = IdleFlg;
	}
	//bit0 = 1:显示请插卡,否则不显示
	//bit1 = 1:显示系统时间，否则不显示
	if(IdleFlg & 0x01)
	{
		if ( val != 0 )
		{
			DisplayCartoon(val & 0x03);
		}
	}
	else 
	{
		LcdStatus = 0;
	}
	if ( val < 4 )
	{
		return ;
	}
    pit_val.last_val += 4;
    if ( (((SysTime.year % 4) == 0) && ((SysTime.year % 100) != 0))
         || ((SysTime.year % 400) == 0) )
    {
        leap = 1;
    }
    if ( SysTime.second < 59 )
    {
        SysTime.second++;
        HexTimeToComBcdTime();
        return ;
    }
    SysTime.second = 0;
    if ( SysTime.minute < 59 )
    {
        SysTime.minute++;
        HexTimeToComBcdTime();
        return ;
    }
    SysTime.minute = 0;
    if ( SysTime.hour < 23 )
    {
        SysTime.hour++;
        HexTimeToComBcdTime();
        return ;
    }
    SysTime.hour = 0;
    if ( SysTime.day < YEAR[leap][SysTime.month] )
    {
        SysTime.day++;
        HexTimeToComBcdTime();
        return ;
    }
    SysTime.day = 1;
    if ( SysTime.month < 12 )
    {
        SysTime.month++;
        HexTimeToComBcdTime();
        return ;
    }
    SysTime.month = 1;
    SysTime.year++;
    HexTimeToComBcdTime();
}

void SetNoPrint(void)
{
	PrintBusy = 1;
}

void CheckPrintStatus(void)
{
	unsigned char length;
	M1172_ChCtrl *pChannel;
	Tmr *pTmr;
	
	SPID_ConfigureCS(&spid, 2, SPI_CSR(MAIN_MCK, SPCK));
	m1172.pChannel = &chCtrl[1];
    pChannel = m1172.pChannel;
	pTmr = CreateTimer(TIMER_ONE_DELETE,50,SetNoPrint);
	while(!spi_status)
	{
		CheckTimerGo();
		if(PrintBusy)
		{
			M1172_ConfigureChannel(&chCtrl[0],0x07, 0x10);
			break;
		}
	}
	if(spi_status)
	{
		if(PrintBusy == 0)
		{
			DeleteTimer(pTmr);
		}
		if ( pChannel->channelStatus == TX_STATUS )
		{
			if ( pChannel->TxLen == 0 )
			{
				pChannel->channelStatus = IDLE;
				
				/*M1172_ReadReg(pChannel->channel,REG_LSR,&length);
				M1172_ReadReg(pChannel->channel,REG_LSR,&length);
				if((length & 0x60) == 0)
				{
					M1172_ConfigureChannel(&chCtrl[0],0x07, 0x10);
					spi_status = 0;
					PrintBusy = 1;
					return;
				}*/
				M1172_ReadReg(pChannel->channel,REG_MSR,&length);
				if((length & 0x10) == 0)
				{
					M1172_ConfigureChannel(&chCtrl[0],0x07, 0x10);
					spi_status = 0;
					PrintBusy = 1;
					return;
				}
				M1172_ConfigureChannel(&chCtrl[0],0x07, 0x10);
				spi_status = 0;
				PrintBusy = 0;
			 }
		}
	}
}