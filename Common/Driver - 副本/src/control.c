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
#if LCD_TYPE==0
	if(IdleFlg & 0x02) {
		buf[0] = Rs485Adr + 1 + '0';
		buf[1] = '\0';
		DisplayOnLine();
    	DisplayString4x5(42,2,TmpBuf);
		DisplayString4x5(36,2,(unsigned char *)buf);
		DisBufToLcd();
	}
#else 
	static const char addInfo[] = "GILBARCO";
	if(IdleFlg & 0x02) {
		buf[0] = Rs485Adr + 1 + '0';
		buf[1] = '\0';
		ClearLcd(3,3,317,19);
		DisplayOnLine();
		DisplayString12(64,3,(unsigned char *)buf);
		unsigned char len = strlen(addInfo);
		unsigned short x = 72 + ((160 - 72 - (len << 3)) >> 1);
		DisplayString12(x,3,(unsigned char*)addInfo);
		DisplayString12(160,3,TmpBuf);
		DrawVerticalline(72,0,20);
		DrawVerticalline(158,0,20);
	}
#endif
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
#if LCD_TYPE==0		//OK
			ClearLcd(1,1,126,7);
			DrawHorizontalLine(0, 128, 8);
			DrawVerticalline(40, 0, 8);
#endif
		}
		IdleFlgSta = IdleFlg;
	}
	//bit0 = 1:显示请插卡,否则不显示
	//bit1 = 1:显示系统时间，否则不显示
	
#if LCD_TYPE==0
	if(IdleFlg & 0x01)
	{
		if(val != 0)
		{
			DisplayCartoon(val & 0x03);
		}
	}
	else 
	{
		LcdStatus = 0;
	}
#endif
#if LCD_TYPE==1
	static unsigned int iconCount = 0;
	if(IdleFlg & 0x01)
	{
		if(val != 0) {
			if((pit_val.current_val & 0x01) == 0x00) {
				if(iconCount > 100) {
					ClearLcd(2,21,317,218);
					iconCount = 0;
				} else {
					iconCount++;
				}
				DisplayCartoon((pit_val.current_val >> 1) & 0x03);
			}
		}
	} else {
		if(iconCount) {
			iconCount = 0;
		}
	}
#endif	
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

#ifdef OUT_TIME_MAX

void lampOpening(void* para)
{
	Output_t* pOutput;
	unsigned char no;
	
	pOutput = (Output_t*)para;
	no = pOutput->no;
	lampValue |= (1 << no);
	WRITE_DATA8(OutputAdr,lampValue);
	ResetOutTimer(pOutput->closeTmr);
}

void lampClosing(void* para)
{
	Output_t* pOutput;
	unsigned char no;
	
	pOutput = (Output_t*)para;
	no = pOutput->no;
	lampValue &= (~(1 << no));
	WRITE_DATA8(OutputAdr,lampValue);
	if(pOutput->count > 0)
	{
		pOutput->count--;
	}
	if(pOutput->count != 0)
	{
		ResetOutTimer(pOutput->openTmr);
	}
}

unsigned char lampControl(unsigned char no,unsigned char typ,int openTime,int closeTime,int count)
{
	Output_t* pOutput;
	
	if(no >= OUT_MAX)
	{
		return 1;
	}
	pOutput = &output[no];
	pOutput->type = typ;
	pOutput->no = no;

	if(typ & OUT_OPEN)
	{
		lampValue |= (1 << no);
		WRITE_DATA8(OutputAdr,lampValue);
		if(pOutput->openTmr)
		{
			StopOutTimer(pOutput->openTmr);
		}
		if(pOutput->closeTmr)
		{
			StopOutTimer(pOutput->closeTmr);
		}
	}
	else if(typ & OUT_CLOSE)
	{
		lampValue &= (~(1 << no));
		WRITE_DATA8(OutputAdr,lampValue);
		if(pOutput->openTmr)
		{
			StopOutTimer(pOutput->openTmr);
		}
		if(pOutput->closeTmr)
		{
			StopOutTimer(pOutput->closeTmr);
		}
	}
	else if(typ & OUT_REPLAY_AUTO)
	{
		if(pOutput->openTmr)
		{
			ResetOutTimer(pOutput->openTmr);
		}
		else if(openTime == 0)
		{
			return 3;
		}
		else
		{
			pOutput->openTmr = CreateOutTimer(TIMER_ONE_STOP,openTime,lampOpening,pOutput);
		}
		if(pOutput->closeTmr)
		{
			ResetOutTimer(pOutput->closeTmr);
		}
		else if(closeTime == 0)
		{
			return 4;
		}
		else
		{
			pOutput->closeTmr = CreateOutTimer(TIMER_ONE_STOP,closeTime,lampClosing,pOutput);
		}
		StopOutTimer(pOutput->closeTmr);
		if(count == 0)
		{
			count = -1;
		}
		pOutput->count = count;
	}
	else if(typ & OUT_AUTO)
	{
		if(pOutput->openTmr)
		{
			if(ChangeOutTime(pOutput->openTmr,openTime))
			{
				DeleteOutTimer(pOutput->openTmr);
				pOutput->openTmr = (OutTmr*)0;
			}
		}
		if(pOutput->openTmr == (OutTmr*)0)
		{
			pOutput->openTmr = CreateOutTimer(TIMER_ONE_STOP,openTime,lampOpening,pOutput);
		}
		if(pOutput->closeTmr)
		{
			if(ChangeOutTime(pOutput->closeTmr,closeTime))
			{
				DeleteOutTimer(pOutput->closeTmr);
				pOutput->closeTmr = (OutTmr*)0;
			}
		}
		if(pOutput->closeTmr == (OutTmr*)0)
		{
			pOutput->closeTmr = CreateOutTimer(TIMER_ONE_STOP,closeTime,lampClosing,pOutput);
		}
		pOutput->count = count;
	}
	else
	{
		return 2;
	}
	return 0;
}

unsigned char lampOpen(unsigned char no)
{	
	return lampControl(no,OUT_OPEN,0,0,0);
}

unsigned char lampClose(unsigned char no)
{
	return lampControl(no,OUT_CLOSE,0,0,0);
}

unsigned char lampGlint(unsigned char no,int openTime,int closeTime,int count)
{
	return lampControl(no,OUT_AUTO,openTime,closeTime,count);
}

unsigned char lampAfreshGlint(unsigned char no)
{
	return lampControl(no,OUT_REPLAY_AUTO,0,0,0);
}

#endif

unsigned char WriteNoFormatData(unsigned char*s,unsigned int len)
{
	At45* pAt45;
	unsigned int adr;
	unsigned int pageMax;
	unsigned int pageOffset;
	unsigned char buf[128];
	
	if(len > 256)
	{
		return 1;
	}
	pAt45= &at45[0];
#if LCD_TYPE==0
	pageMax = (256 * 1024);
#else 
	pageMax = (512 * 1024);
#endif
	pageOffset = pageMax % AT45_PageSize(pAt45);
	pageMax /= AT45_PageSize(pAt45);
	pageMax++;
	memcpy(buf,s,len);
	adr = pageMax * AT45_PageSize(pAt45) + pageOffset;
	WRITE(ZIMO_ID,buf,adr,len);
	return 0;
}

unsigned char ReadNoFormatData(unsigned char* s,unsigned int len)
{
	At45* pAt45;
	unsigned int adr;
	unsigned int pageMax;
	unsigned int pageOffset;
	
	if(len > 256)
	{
		return 1;
	}
	pAt45= &at45[0];
#if LCD_TYPE==0
	pageMax = (256 * 1024);
#else 
	pageMax = (512 * 1024);
#endif
	pageOffset = pageMax % AT45_PageSize(pAt45);
	pageMax /= AT45_PageSize(pAt45);
	pageMax++;
	adr = pageMax * AT45_PageSize(pAt45) + pageOffset;
	READ(ZIMO_ID,s,adr,len);
	return 0;
}