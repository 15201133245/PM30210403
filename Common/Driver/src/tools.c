#include "global.h"

unsigned char GetRs485Adr(unsigned int adr)
{
    unsigned char tmp;
    
    tmp = READ_DATA8(adr);
    tmp >>= 6;
    return (tmp);
}

int WRITE(unsigned int id,unsigned char *pBuffer,
		  unsigned int address,unsigned int len)
{
	if(pBuffer == (unsigned char*)0)
	{
		return -1;
	}
    int length = 0;
    switch ( id )
    {
    case CONFIG_ID :
        SPID_ConfigureCS(&spid, 0, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
        length = AT45_Write(&at45[0],pBuffer,address,len);
        break;
    case TRANS_ID :
        SPID_ConfigureCS(&spid, 1, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
        length = AT45_Write(&at45[1],pBuffer,address,len);
        break;
    case EEPROM_ID :
        length = WriteToIIC(&at24,pBuffer,address,len);
        break;
    case USART0_ID :
        length = WriteToUsart(&Serial0,pBuffer,len);
        break;
    case USART1_ID :
        //length = WriteToUsart(&Serial1,pBuffer,len);
        break;
    case USART2_ID :
        m1172.pChannel = &chCtrl[0];
        SPID_ConfigureCS(&spid, 2, SPI_CSR(MAIN_MCK, SPCK));
		chCtrl[0].efcr_mode = 0x02;
        M1172_ConfigureChannel(&chCtrl[0],0x02,0x10);
        length = WriteToSpiUsart(&m1172,pBuffer,len);
        break;
    case USART3_ID :
        m1172.pChannel = &chCtrl[1];
        SPID_ConfigureCS(&spid, 2, SPI_CSR(MAIN_MCK, SPCK));
		M1172_ConfigureChannel(&chCtrl[1],0x02,0x90);
        length = WriteToSpiUsart(&m1172,pBuffer,len);
        break;
    default:
        length = ID_ERROR;
        break;
    }
    return length;
}

int READ(unsigned int id,unsigned char *pBuffer,
		 unsigned int address,unsigned int len)
{
    int length = 0;
    switch ( id )
    {
    case CONFIG_ID :
        SPID_ConfigureCS(&spid, 0, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
        length = AT45_Read(&at45[0],pBuffer,address,len);
        break;
    case TRANS_ID :
        SPID_ConfigureCS(&spid, 1, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
        length = AT45_Read(&at45[1],pBuffer,address,len);
        break;
    case EEPROM_ID :
        length = ReadFromIIC(&at24,pBuffer,address,len);
        break;
    case USART0_ID :
        length = ReadFromUsart(&Serial0,pBuffer,len);
        break;
    case USART1_ID :
        //length = ReadFromUsart(&Serial1,pBuffer,len);
        break;
    case USART2_ID :
        m1172.pChannel = &chCtrl[0];
        length = ReadFromSpiUsart(&m1172,pBuffer,len);
        break;
    case USART3_ID :
        m1172.pChannel = &chCtrl[1];
        length = ReadFromSpiUsart(&m1172,pBuffer,len);
        break;
    default:
        length = ID_ERROR;
        break;
    }
    return length;
}

unsigned int IntEndianConvert(unsigned int val)
{
    asm("	STMFD	SP!,	{R1}				\n"
    	"	EOR		R1,		R0,		R0,	ROR	#16	\n"
    	"	BIC		R1,		R1,		#0x00FF0000	\n"
    	"	MOV		R0,		R0,		ROR	#8		\n"
    	"	EOR		R0,		R0,		R1,	LSR	#8	\n"
    	"	LDMFD	SP!,	{R1}");
    return val;
}

unsigned short ShortEndianConvert(unsigned short val)
{
    asm("	STMFD	SP!,	{R1}				\n"
    	"	AND		R1,		R0,		#0xFF		\n"
    	"	MOV		R0,		R0,		LSR	#8		\n"
    	"	EOR		R0,		R0,		R1,	LSL	#8	\n"
    	"	LDMFD	SP!,	{R1}");
    return val;
}

unsigned long long LongLongEndianConvert(unsigned long long val)
{
    asm("	STMFD	SP!,		{R2,LR}			\n"
    	"	BL		IntEndianConvert			\n"
    	"	MOV		R2,		R0					\n"
    	"	MOV		R0,		R1					\n"
    	"	BL		IntEndianConvert			\n"
    	"	MOV		R1,		R2					\n"
    	"	LDMFD	SP!,	{R2,LR}");
    return val;
}

void WatchDog(void)
{
    AT91F_WDTRestart(AT91C_BASE_WDTC);
}

unsigned short Crc16(unsigned char *s,unsigned short len,unsigned short LastCrc)
{
    unsigned char i;
    unsigned char tmp;
    unsigned short crc;
    
    tmp=*s;
    crc=LastCrc;
    while ( len-- != 0 )
    {
        for ( i = 0x01; i != 0; i <<= 1 )
        {
            if ( (crc & 0x0001) != 0 )
            {
                crc >>= 1;
                crc ^= 0xa001;
            }
            else
            {
                crc >>= 1;
            }
            if ( (tmp & i) != 0 )
            {
                crc ^= 0xa001;
            }
        }
        s++;
        tmp=*s;
    }
    return (crc);
}


void AdjustSystime(unsigned char *s,unsigned char typ)
{
    unsigned short year;
    unsigned char tmp;
    Time time;
    
    year = s[0] >> 4;
    year *= 10;
    year += (s[0] & 0x0f);
    year *= 10;
    year += (s[1] >> 4);
    year *= 10;
    year += (s[1] & 0x0f);
    time.year = year;
    
    tmp = s[2] >> 4;
    tmp *= 10;
    tmp += (s[2] & 0x0f);
    time.month = tmp;
    
    tmp = s[3] >> 4;
    tmp *= 10;
    tmp += (s[3] & 0x0f);
    time.day = tmp;
    
    tmp = s[4] >> 4;
    tmp *= 10;
    tmp += (s[4] & 0x0f);
    time.hour = tmp;
    
    tmp = s[5] >> 4;
    tmp *= 10;
    tmp += (s[5] & 0x0f);
    time.minute = tmp;
    
    tmp = s[6] >> 4;
    tmp *= 10;
    tmp += (s[6] & 0x0f);
    time.second = tmp;
    
    tmp = 0;
    if ( time.year != SysTime.year )
    {
        tmp = 1;
    }
	else if(time.month != SysTime.month)
	{
		tmp = 1;
	}
	else if(time.day != SysTime.day)
	{
		tmp = 1;
	}
	else if(time.hour != SysTime.hour)
	{
		tmp = 1;
	}
	else if(time.minute != SysTime.minute)
	{
		//if((time.minute > ((SysTime.minute + 1) % 60)) || (SysTime.minute > ((time.minute + 1) % 60)))
		if((time.minute > (SysTime.minute + 1)) || (SysTime.minute > (time.minute + 1)))
		{
			tmp = 1;
		}
	}
    if ( tmp )
    {
		typ += 7;
		WriteDriveLog(typ,s);
        memcpy((unsigned char *)(&SysTime),(unsigned char *)(&time),7);
        HexTimeToComBcdTime();
    }
}

unsigned short AddFA(unsigned char *s,unsigned short len)
{
    unsigned char *p;
    unsigned short i,j,k;
	unsigned char buf[256];
    
    p = buf;
    k = len;
    for ( i = 6,j = 0; i < len; i++,j++ )
    {
        if ( s[i] == 0xfa )
        {
            k++;
            p[j++] = 0xfa;
            p[j] = 0xfa;
        }
        else
        {
            p[j] = s[i];
        }
    }
    len = k;
    memcpy(&s[6],p,j);
    return (len);
}

void ReadSystimer(unsigned char *s)
{
	unsigned short year;
	
	year = ShortEndianConvert(SysBCDTime.year);
    memcpy(s,(unsigned char *)(&year),2);
	memcpy(s + 2,(unsigned char *)(&SysBCDTime.month),5);
}

static unsigned char PowerDelayFlg = 0;

void PowerDelay(void)
{
	PowerDelayFlg = 1;
}

void PowerDownDispose(unsigned char *s,unsigned int len)
{
	//AIC_DisableIT(AT91C_ID_IRQ1);
	PIO_Set(&lcd_bl);
	AIC_DisableIT(AT91C_ID_IRQ0);  
	AIC_DisableIT(AT91C_ID_IRQ1);
	AIC_DisableIT(AT91C_ID_US0);
	AIC_DisableIT(AT91C_ID_US1);
	//AIC_DisableIT(AT91C_ID_TC0);
	AIC_DisableIT(AT91C_ID_TC1);
	AIC_DisableIT(AT91C_ID_SYS);
	SaveConfig(s,len);
	while(1)
	{
		while(!CheckPowerDownPin())
		{
			WatchDog();
		}
		PowerDelayFlg = 0;
		CreateTimer(TIMER_ONE_DELETE,1000,PowerDelay);
		while(!PowerDelayFlg)
		{
			CheckTimerGo();
			WatchDog();
		}
		if(CheckPowerDownPin())
		{
			while(1)
			{
			}
		}
	}
}

void OpenOrCloseLcdLight(void)
{
	if((SysTime.hour >= cr_protected.OpenTime) || (SysTime.hour < cr_protected.CloseTime))
	{
		if(LcdLightFlg == LCD_LIGHT_CLOSE)
		{
			LcdLightFlg |= LCD_LIGHT_OPEN_TIME;
		}
		else 
		{
			LcdLightFlg |= LCD_LIGHT_OPEN_TIME;
			PIO_Clear(&lcd_bl);			
		}
	}
	else if((SysTime.hour >= cr_protected.CloseTime) && (SysTime.hour < cr_protected.OpenTime))
	{
		if((LcdLightFlg & LCD_LIGHT_OPEN_TIME) == LCD_LIGHT_OPEN_TIME)
		{
			LcdLightFlg &= (~LCD_LIGHT_OPEN_TIME);
			if(!LcdLightFlg)
			{
				PIO_Set(&lcd_bl);
			}
		}
	}
}
void HexToComBcd(unsigned short val,unsigned char *s)
{
	unsigned char i;
	
	i = (unsigned char)(val / 1000);
	i <<= 4;
	val %= 1000;
	i += (unsigned char)(val / 100);
	val %= 100;
	s[0] = i;
	
	i = (unsigned char)(val / 10);
	i <<= 4;
	val %= 10;
	i += (unsigned char)(val);
	s[1] = i;
}

unsigned char CrcEor(unsigned char *s,unsigned short len)
{
	unsigned short i;
	unsigned char bcc = 0;
	
	for(i = 0; i < len; i++)
	{
		bcc ^= s[i];
	}
	return(bcc);
}

void PowerDownCheck(void)
{
	if(CheckPowerDownPin())
	{
		PIO_SetPeripheralA(fiq.pio,fiq.mask,PIO_PULLUP);
		PMC_EnablePeripheral(AT91C_ID_FIQ);
		AIC_ConfigureIT(AT91C_ID_FIQ,IRQ_INTERRUPT_PRIORITY | AT91C_AIC_SRCTYPE_INT_POSITIVE_EDGE,ISR_fiq);
		AIC_ClearIT(AT91C_ID_FIQ);
		AIC_EnableIT(AT91C_ID_FIQ);
	}
	else
	{
		spi_status = 0;
		AIC_DisableIT(AT91C_ID_IRQ1);
		PowerDownFlg = 1;
	}
}

void UpdateSoftwareCloseAIC(void)
{     
	spi_status = 0xff;
	AIC_DisableIT(AT91C_ID_IRQ1);
	//AIC_DisableIT(AT91C_ID_SPI);
}

unsigned short CheckCardTime(unsigned char *s)
{
	unsigned short year;
	unsigned char val;
	unsigned char month;
	unsigned char day1,day2;
	unsigned char leap = 0;
	
	val = *s++;
	if((val & 0xf0) > 0x90)
	{
		return(1);
	}
	if((val & 0x0f) > 0x09)
	{
		return(1);
	}
	year = val >> 4;
	year *= 10;
	year += (val & 0x0f);
	year *= 10;
	
	val = *s++;
	if((val & 0xf0) > 0x90)
	{
		return(1);
	}
	if((val & 0x0f) > 0x09)
	{
		return(1);
	}
	year += (val >> 4);
	year *= 10;
	year += (val & 0x0f);
	
	val = *s++;
	if((val & 0xf0) > 0x10)
	{
		return(1);
	}
	if((val & 0x0f) > 0x09)
	{
		return(1);
	}
	month = (val >> 4);
	month *= 10;
	month += (val & 0x0f);
	if(month == 0)
	{
		return(1);
	}
	if(month > 12)
	{
		return(1);
	}
	
	if((((year % 4) == 0) && ((year % 100) != 0))
		|| ((year % 400) == 0))
	{
		leap = 1;
	}
	day2 = YEAR[leap][month];
	
	val = *s;
	if((val & 0xf0) > 0x30)
	{
		return(1);
	}
	if((val & 0x0f) > 0x09)
	{
		return(1);
	}
	day1 = val >> 4;
	day1 *= 10;
	day1 += (val & 0x0f);
	if(day1 == 0)
	{
		return(1);
	}
	if(day1 > day2)
	{
		return(1);
	}
	return(0x9000);
}

void ForceRebuildProtected(void)
{
	while(1)
	{
	}
}