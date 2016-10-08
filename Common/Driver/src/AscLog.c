#include "global.h"

static const unsigned char LoadingData[] = "正在加载数据";
static const unsigned char LogErr[] = "日志地址错误";

#define LOG_MAXLEN		66

unsigned char AscLog(unsigned char *s)
{
    At45 *pAt45;
    unsigned int len;
    unsigned char *p,result = 0;
	unsigned char buf[LOG_MAXLEN];
    unsigned int adr;
	unsigned short page;
    
    pAt45 = &at45[LOG_ID];
	//liuzg start 20130326 pointer and len control
	if(s == (unsigned char*)0)
	{
		return -1;
	}
	if(memchr(s,'\0',64) == (void*)0)
	{
		return 2;
	}
	//liuzg end 20130326 pointer and len control
    len = strlen((char const *)s);
    p = buf;
    if ( (len + 3) > LOG_MAXLEN )
    {
        return 2;
    }
	//liuzg20111209
	if((cr_protected.LogWriteCurrentPageAddr < 0x202) || (cr_protected.LogWriteCurrentPageAddr > 0xfff))
	{
		DisPlayHelp(LogErr);
		ReBuildLogProtected();
		ResetAllTimer();
	}
	//end
	//liuzg start 20130516
	if((cr_protected.LogWritePageNumber + 1) == (AT45_PageSize(pAt45) / LOG_MAXLEN))
	{
		page = cr_protected.LogWriteCurrentPageAddr;
		if(page == 0xfff)
		{
			page = 0x201;
		}
		page++;
		SPID_ConfigureCS(pAt45->pSpid, LOG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		AT45_ErasePage(pAt45,page * AT45_PageSize(pAt45));
	}
	//liuzg end 20130516
    if ( (cr_protected.LogWritePageNumber + 1) > (AT45_PageSize(pAt45) / LOG_MAXLEN) )
    {
        cr_protected.LogWriteCurrentPageAddr++;
        if ( cr_protected.LogWriteCurrentPageAddr > 0xfff )
        {
            cr_protected.LogWriteCurrentPageAddr = 0x202;
        }
        cr_protected.LogWritePageNumber  = 0;
    }
	
    adr = cr_protected.LogWriteCurrentPageAddr * AT45_PageSize(pAt45);
    memcpy(p,(char *)(&cr_protected.LogWriteFlag),2);
    memcpy(&p[2],s,len + 1);
    adr = cr_protected.LogWriteCurrentPageAddr * AT45_PageSize(pAt45);
    adr += cr_protected.LogWritePageNumber * LOG_MAXLEN;
    if ( WRITE(LOG_ID,p,adr,len + 3) > 0 )
    {
	//	dbug_printf("%6u::%s\r\n",cr_protected.LogWriteFlag,s);
        cr_protected.LogWriteFlag++;
        cr_protected.LogWritePageNumber++;
        if ( (cr_protected.LogWritePageNumber % (AT45_PageSize(pAt45) / LOG_MAXLEN)) == 0 )
        {
            cr_protected.LogWritePageNumber = 0;
            cr_protected.LogWriteCurrentPageAddr++;
            if ( cr_protected.LogWriteCurrentPageAddr > 0xfff )
            {
                cr_protected.LogWriteCurrentPageAddr = 0x202;
            }
        }
		result = 1;
    }
	if((cr_protected.LogWriteCurrentPageAddr < 0x202) || (cr_protected.LogWriteCurrentPageAddr > 0xfff))
	{
		DisPlayHelp(LogErr);
		ReBuildLogProtected();
		ResetAllTimer();
	}
	CalculateCrc();
	return(result);
}

static unsigned char PowerOnDelayFlg = 0;

void PowerOnDelay(void)
{
	PowerOnDelayFlg = 1;
}

unsigned char ReadBackupWriteTTC(unsigned char *s,unsigned int len)
{
	unsigned int adr;
	unsigned char i;
	At45 *pAt45;
	
	pAt45 = &at45[TRANS_ID];
	
	adr = PROTECTED_BACKUP_ADDR * AT45_PageSize(pAt45);
	for ( i = 0; i < 3; i++ )
	{
		if ( READ(TRANS_ID,s,adr,len) == len )
		{
			break;
		}
	}
	if(i == 3)
	{
		return(1);
	}
	return(0);
}

unsigned char LoadConfig(unsigned char *s,unsigned int len)
{
    unsigned char *p;
    int leng;
    DataProtected *pProtect;
    At45 *pAt45;
    unsigned short crc,crctmp,PriceFlg;
    unsigned char j,flg = 0;
	//unsigned char load_flg = 0;
    unsigned int adr,length;
	unsigned char buf[528];
    
	//liuzg start 20130326 pointer and len control
	if((len > 0) && (s == (unsigned char*)0))
	{
		return -1;
	}
	if((len > 0) && (len > (526 - sizeof(DataProtected))))
	{
		len = 526 - sizeof(DataProtected);
	}
	//liuzg end 20130326 pointer and len control
	DisPlayHelp(LoadingData);
    p = buf;
    pAt45 = &at45[CONFIG_ID];
    j = 0;
    adr = PROTECTED_START * AT45_PageSize(pAt45);
	if(len > 0)
	{
    	length = PROTECTED_LEN + 2 + len + 2;
	}
	else
	{
		length = PROTECTED_LEN + 2;
	}
    ListFlg.BaseBlackListPageAddr = BASE_BLACK_LIST_ADDRESS;
    ListFlg.AddBlackListPageAddr = ADD_BLACK_LIST_ADDRESS;
    ListFlg.DelBlackListPageAddr = DEL_BLACK_LIST_ADDRESS;
    ListFlg.WhiteListPageAddr = WHITE_LIST_ADDRESS;
    ListFlg.GradePricePageAddr = GRADE_PRICE_ADDRESS;
    ListFlg.OilStopPageAddr = OIL_STOP_ADDRESS;
    ListFlg.BaseBlackListCount = 0;
    ListFlg.AddBlackListCount = 0;
    ListFlg.DelBlackListCount = 0;
    ListFlg.WhiteListCount = 0;
    ListFlg.GradePriceCount = 0;
    ListFlg.OilStopCount = 0;
    ListFlg.crc = 0;
    do{
        leng = READ(CONFIG_ID,p,adr,length);
        /* EasyCODE - */
        WatchDog();
        j++;
        if ( j > 5 )
        {
			ClearLcd(2,51,126,62);
			DisBufToLcd();
            return 1;
        }
    }while ( leng < 0 );
    pProtect = (DataProtected *)(p);
    crc = Crc16(p,RECORD_LEN,0);
    if ( crc == pProtect->RecordCrc )
    {
        crc = Crc16(p,PROTECTED_LEN,0);
        if ( crc == pProtect->crc )
        {
			//start liuzg 20120206
            memcpy((unsigned char *)(&cr_protected),p,PROTECTED_LEN);
			CreateTimer(TIMER_ONE_DELETE,10 * 1000,PowerOnDelay);
			while(!PowerOnDelayFlg)
			{
                                WatchDog();
				CheckTimerGo();
			}
			SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
            AT45_ErasePage(pAt45,PROTECTED_START * 
            		AT45_PageSize(pAt45));
            AT45_ErasePage(pAt45,(PROTECTED_START + 1) *
            		AT45_PageSize(pAt45));
			if((cr_protected.RecordReadTTC == 0) || (cr_protected.RecordWriteTTC == 0))
			{
				 ReBuildRecordProtected();
			}
			if((cr_protected.PriceUpdateFlg > 2) && (cr_protected.PriceUpdateFlg < 10))
			{
				SetPriceUpdateFlg(2);
			}
			ListIntegrality();
			ReadOpenOrCloseLcdLightTime();
			//flg = 1;
			//end liuzg 20120206
        }
		else
		{
			flg = 1;
			//liuzg start 20111226
			/*SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
			AT45_ErasePage(pAt45,PROTECTED_START * 
					AT45_PageSize(pAt45));
			AT45_ErasePage(pAt45,(PROTECTED_START + 1) *
					AT45_PageSize(pAt45));
			cr_protected.RecordStartPageAddr = RECORD_START_PAGE;
			cr_protected.RecordEndPageAddr = RECORD_END_PAGE;
			cr_protected.RecordWriteTTC = 1;
			//cr_protected.RecordCurrentPageAddr = RECORD_START_PAGE;
			//cr_protected.RecordCountInPage = 0;
			cr_protected.RecordReadTTC = 1;
			cr_protected.UpTransferPageAddr = RECORD_START_PAGE;
			cr_protected.UpTransferOffsetInPage = 0;
			ListIntegrality();
			ReadOpenOrCloseLcdLightTime();*/
			//liuzg end 20111226
		}
    }
    else
    {
        flg = 1;
		//liuzg start 20111226
		/*SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		AT45_ErasePage(pAt45,PROTECTED_START * 
				AT45_PageSize(pAt45));
		AT45_ErasePage(pAt45,(PROTECTED_START + 1) *
				AT45_PageSize(pAt45));
        cr_protected.RecordStartPageAddr = RECORD_START_PAGE;
        cr_protected.RecordEndPageAddr = RECORD_END_PAGE;
        cr_protected.RecordWriteTTC = 1;
        //cr_protected.RecordCurrentPageAddr = RECORD_START_PAGE;
        //cr_protected.RecordCountInPage = 0;
        cr_protected.RecordReadTTC = 1;
        cr_protected.UpTransferPageAddr = RECORD_START_PAGE;
        cr_protected.UpTransferOffsetInPage = 0;
		ListIntegrality();
		ReadOpenOrCloseLcdLightTime();*/
		//liuzg end 20111226
    }
	if(len > 0)
	{
		crc = Crc16(&p[PROTECTED_LEN + 2],len,0);
		memcpy((unsigned char *)(&crctmp),&p[PROTECTED_LEN + 2 + len],2);
		if(crc == crctmp)
		{
			memcpy(s,&p[PROTECTED_LEN + 2],len);
			ErasePowerDownTrans();
		}
		else
		{
			LoadPowerDownTrans(s,len);
			//load_flg = 1;
		}
	}
    if ( flg == 1 )
    {
		//liuzg start 20111226
		SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		AT45_ErasePage(pAt45,PROTECTED_START * 
				AT45_PageSize(pAt45));
		AT45_ErasePage(pAt45,(PROTECTED_START + 1) *
				AT45_PageSize(pAt45));
		cr_protected.RecordStartPageAddr = RECORD_START_PAGE;
		cr_protected.RecordEndPageAddr = RECORD_END_PAGE;
		cr_protected.RecordWriteTTC = 1;
		cr_protected.RecordReadTTC = 1;
		cr_protected.UpTransferPageAddr = RECORD_START_PAGE;
		cr_protected.UpTransferOffsetInPage = 0;
		ListIntegrality();
		ReadOpenOrCloseLcdLightTime();
		//liuzg end 20111226
		
		//liuzg start 20121105
		ReBuildLogProtected();
		ReBuildRecordProtected();
		//liuzg end 20121105
        
        ReBuildListProtected();
        PriceFlg = ReadPriceFlgFromDataFlash();
		if((PriceFlg < 2) || (PriceFlg == 10))
		{
			SetPriceUpdateFlg(PriceFlg);
		}
		else if((PriceFlg > 2) && (PriceFlg < 10))
		{
			SetPriceUpdateFlg(2);
		}
		else
		{
			SetPriceUpdateFlg(0);
		}
        SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
        AT45_ErasePage(pAt45,PROTECTED_START * 
        AT45_PageSize(pAt45));
        AT45_ErasePage(pAt45,(PROTECTED_START + 1) *
        AT45_PageSize(pAt45));
    }
	memset(p,0,8);
    if ( memcmp((unsigned char *)(&cr_protected.BaseBlackListIntegrality),p,8) == 0 )
    {
        if ( LoadListHead(0xff) )
        {
			ClearLcd(2,51,126,62);
			DisBufToLcd();
            return 5;
        }
        BuildListLink(0xff);
		LoadGradePriceList();
		LoadGradePriceTime();
        
		LoadOilStopMessage();
    }
    else
    {
        if ( cr_protected.BaseBlackListIntegrality == 0 )
        {
            if ( LoadListHead(BASEBLACKLIST) )
            {
                cr_protected.BaseBlackListLength = 0;
				ClearLcd(2,51,126,62);
				DisBufToLcd();
                return 5;
            }
            BuildListLink(0);
        }
		else
		{
			SetListHead(BASEBLACKLIST);
		}
        if ( cr_protected.AddBlackListIntegrality == 0 )
        {
            if ( LoadListHead(ADDBLACKLIST) )
            {
                cr_protected.AddBlackListLength = 0;
				ClearLcd(2,51,126,62);
				DisBufToLcd();
                return 5;
            }
            BuildListLink(1);
        }
		else
		{
			SetListHead(ADDBLACKLIST);
		}
        if ( cr_protected.DelBlackListIntegrality == 0 )
        {
            if ( LoadListHead(DELBLACKLIST) )
            {
                cr_protected.DelBlackListLength = 0;
				ClearLcd(2,51,126,62);
				DisBufToLcd();
                return 5;
            }
            BuildListLink(2);
        }
		else
		{
			SetListHead(DELBLACKLIST);
		}
        if ( cr_protected.WhiteListIntegrality == 0 )
        {
            if ( LoadListHead(WHITELIST) )
            {
                cr_protected.WhiteListLength = 0;
				ClearLcd(2,51,126,62);
				DisBufToLcd();
                return 5;
            }
            BuildListLink(3);
        }
		else
		{
			SetListHead(WHITELIST);
		}
		if(cr_protected.GradePriceIntegrality == 0)
		{
			LoadGradePriceList();
			LoadGradePriceTime();
		}
		else
		{
			cr_protected.GradePriceIntegrality = 0xffff;
			cr_protected.GradePriceLength = 0;
			SetPriceUpdateFlg(0);
			errno = PRICE_ERROR_FLAGS;
		}
		if(cr_protected.OilStopIntegrality == 0)
		{
			LoadOilStopMessage();
		}
    }
	if(PriceUpdateFlg != 0xffff)
	{
		if((PriceUpdateFlg < 2) || (PriceUpdateFlg == 10))
		{
			SetPriceUpdateFlg(PriceUpdateFlg);
		}
		else if((PriceUpdateFlg > 2) && (PriceUpdateFlg < 10))
		{
			SetPriceUpdateFlg(2);
		}
		else
		{
			SetPriceUpdateFlg(0);
		}
	}
	CalculateCrc();
	ClearLcd(2,51,126,62);
	DisBufToLcd();
    WriteDriveLog_Reset(reset_typ,Reset_Adr);
	if((HandlerAbort == 0) || (HandlerAbort == 1))
	{
		WriteDriveLog_Reset(9 + HandlerAbort,0);
	}
    return 0;
}

unsigned char SaveConfig(unsigned char *s,unsigned int len)
{
    unsigned char i;
    At45 *pAt45;
    unsigned int adr;
    unsigned char *p;
	unsigned short crc;
	unsigned char buf[528];
     
	//liuzg start 20130326 pointer and len control
	if((len > 0) && (s == (unsigned char*)0))
	{
		return -1;
	}
	if((len > 0) && (len > (526 - sizeof(DataProtected))))
	{
		len = 526 - sizeof(DataProtected);
	}
	//liuzg end 20130326 pointer and len control
	AIC_DisableIT(AT91C_ID_IRQ1);
    pAt45 = &at45[CONFIG_ID];
    adr = PROTECTED_START * AT45_PageSize(pAt45);
    cr_protected.RecordCrc = Crc16((unsigned char *)(&cr_protected.RecordStartPageAddr),RECORD_LEN,0);
    cr_protected.crc = Crc16((unsigned char *)(&cr_protected.RecordStartPageAddr),PROTECTED_LEN,0);
    p = buf;
    memcpy(p,(unsigned char *)(&cr_protected),PROTECTED_LEN + 2);
	if(len > 0)
	{
		memcpy(&p[PROTECTED_LEN + 2],s,len);
		crc = Crc16(s,len,0);
		memcpy(&p[PROTECTED_LEN + 2 + len],(unsigned char *)(&crc),2);
		len += PROTECTED_LEN + 2 + 2;
	}
    for ( i = 0; i < 3; i++ )
    {
        if ( WRITE(CONFIG_ID,p,adr,len) > 0 )
        {
            break;
        }
    }
    if ( i == 3 )
    {
        return 1;
    }
	/*adr = PROTECTED_BACKUP_ADDR * AT45_PageSize(pAt45);
	for(i = 0; i < 3; i++)
	{
		 if ( WRITE(LOG_ID,p,adr,len) > 0 )
        {
            break;
        }
	}
	if ( i == 3 )
    {
        return 2;
    }*/
    return 0;
}

unsigned char ReBuildLogProtected(void)
{
    unsigned int i,j,adr;
    At45 *pAt45;
    unsigned short number = 0,tmp,page,offset;
    unsigned char flg = 0;
	unsigned char buf[528];
	unsigned char buf1[66];
    
    pAt45 = &at45[TRANS_ID];
    number = 0;
    page = 0x202;
    offset = -1;
    
	memset(buf1,0xff,66);
	for(i = 0x202; i < 0x1000; ++i)
	{
		WatchDog();
		adr = i * AT45_PageSize(pAt45);
		if(READ(TRANS_ID,buf,adr,AT45_PageSize(pAt45)) != AT45_PageSize(pAt45))
		{
			flg = 1;
			break;
		}
		for(j = 0; j < (AT45_PageSize(pAt45) / LOG_MAXLEN); ++j)
		{
			if(memcmp(&buf[j * LOG_MAXLEN],buf1,66) == 0)
			{
				if(number == 0)
                {
                    number = -1;
                }
				break;
			}
			memcpy((unsigned char*)(&tmp),&buf[j * LOG_MAXLEN],2);
			if(number == 0)
		    {
				if(tmp != 0)
				{
					number = tmp;
					page = i;
					offset = j;
				}
				else
                {
                    continue;
                }
			}
			else if((number + 1) == tmp)
			{
				number = tmp;
				page = i;
				offset = j;
			}
			else
			{
				break;
			}
		}
		if(j != (AT45_PageSize(pAt45) / LOG_MAXLEN))
		{
			break;
		}
	}
    if ( flg )
    {
        cr_protected.LogWriteFlag = 0;
        cr_protected.LogWriteCurrentPageAddr = 0x202;
        cr_protected.LogWritePageNumber = 0;
    }
    else
    {
		cr_protected.LogWriteFlag = number + 1;
		if((offset + 1) == (AT45_PageSize(pAt45) / LOG_MAXLEN))
		{
			if(page == 0xfff)
			{
				page = 0x201;
			}
			cr_protected.LogWriteCurrentPageAddr = page + 1;
        	cr_protected.LogWritePageNumber = 0;
		}
		else
		{
			cr_protected.LogWriteCurrentPageAddr = page;
			cr_protected.LogWritePageNumber = offset + 1;
		}
    }
	CalculateCrc();
    return flg;
}

void CheckUpdatePriceTime(void)
{
    if ( (cr_protected.PriceUpdateFlg == 1) && (IdleFlg & 0x04) )
    {
        if ( memcmp((unsigned char *)(&SysBCDTime),(unsigned char *)(&PriceUpdateTime),6) >= 0 )
        {
            SetPriceUpdateFlg(2);
			CalculateCrc();
        }
    }
}

void ReadCrConfig(unsigned char *s,unsigned short len)
{
	At45 *pAt45;
	unsigned int adr;
	unsigned char buf[528];
	unsigned short crc,crctmp;
	
	if(s == (unsigned char*)0)
	{
		return;
	}
	if(len > 526)
	{
		len = 526;
	}
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = CR_CONFIG_ADDRESS * AT45_PageSize(pAt45);
	READ(CONFIG_ID,buf,adr,len + 2);
	crc = Crc16(buf,len,0);
	memcpy((unsigned char *)(&crctmp),&buf[len],2);
	if(crc == crctmp)
	{
		memcpy(s,buf,len);
	}
	else
	{
		memset(s,0xff,len);
	}
}

void WriteCrConfig(unsigned char *s,unsigned short len)
{
	At45 *pAt45;
	unsigned int adr;
	unsigned char buf[528];
	unsigned short crc;
	
	if(s == (unsigned char*)0)
	{
		return;
	}
	if(len > 526)
	{
		len = 526;
	}
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = CR_CONFIG_ADDRESS * AT45_PageSize(pAt45);
	memcpy(buf,s,len);
	crc = Crc16(buf,len,0);
	memcpy(&buf[len],(unsigned char *)(&crc),2);
	WRITE(CONFIG_ID,buf,adr,len + 2);
}

void ReadOpenOrCloseLcdLightTime(void)
{
	At45 *pAt45;
	unsigned int adr;
	unsigned char buf[4];
	unsigned short crc,crctmp;
	
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = LIST_INTEGRALITY * AT45_PageSize(pAt45) + 264;
	READ(CONFIG_ID,buf,adr,4);
	crc = Crc16(buf,2,0);
	memcpy((unsigned char *)(&crctmp),&buf[2],2);
	if(crc == crctmp)
	{
		cr_protected.OpenTime = buf[0];
		cr_protected.CloseTime = buf[1];
	}
	else
	{
		cr_protected.OpenTime = 0x10;
		cr_protected.CloseTime = 0x09;
		WriteOpenOrCloseLcdLightTime(0,0x10);
		WriteOpenOrCloseLcdLightTime(1,0x09);
	}
	CalculateCrc();
}

void WriteOpenOrCloseLcdLightTime(unsigned char sta,unsigned char val)
{
	At45 *pAt45;
	unsigned int adr;
	unsigned char buf[4];
	unsigned short crc;
	
	sta &= 0x01;
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = LIST_INTEGRALITY * AT45_PageSize(pAt45) + 264;
	READ(CONFIG_ID,buf,adr,2);
	buf[sta] = val;
	crc = Crc16(buf,2,0);
	memcpy(&buf[2],(unsigned char *)(&crc),2);
	WRITE(CONFIG_ID,buf,adr,4);
}

void SavePowerDownTrans(unsigned char *s,unsigned short len)
{
	At45 *pAt45;
	unsigned int adr;
	unsigned char buf[528];
	unsigned short crc;
	
	if(s == (unsigned char*)0)
	{
		return;
	}
	if(len > 520)
	{
		len = 520;
	}
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = POWERDOWN_TRANS * AT45_PageSize(pAt45);
	memcpy(buf,s,len);
	memcpy(&buf[len],(unsigned char *)(&cr_protected.RecordReadTTC),4);
	memcpy(&buf[len + 4],(unsigned char *)(&cr_protected.PriceUpdateFlg),2);
	crc = Crc16(buf,len + 6,0);
	memcpy(&buf[len + 6],(unsigned char *)(&crc),2);
	WRITE(CONFIG_ID,buf,adr,len + 8);
}

void LoadPowerDownTrans(unsigned char *s,unsigned short len)
{
	At45 *pAt45;
	unsigned int adr;
	unsigned char buf[528];
	unsigned short crc,crctmp;
	
	if(s == (unsigned char*)0)
	{
		return;
	}
	if(len > 520)
	{
		len = 520;
	}
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = POWERDOWN_TRANS * AT45_PageSize(pAt45);
	READ(CONFIG_ID,buf,adr,len + 8);
	crc = Crc16(buf,len + 6,0);
	memcpy((unsigned char *)(&crctmp),&buf[len + 6],2);
	if(crc == crctmp)
	{
		memcpy(s,buf,len);
		memcpy((unsigned char *)(&PowerDownSaveReadTTC),&buf[len],4);
		memcpy((unsigned char *)(&PriceUpdateFlg),&buf[len + 4],2);
		SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
        AT45_ErasePage(pAt45,adr);
	}
	else
	{
		memset(s,0xff,len);
	}
}

void ErasePowerDownTrans(void)
{
	At45 *pAt45;
	unsigned int adr;
	
	pAt45 = &at45[CONFIG_ID];
	adr = POWERDOWN_TRANS * AT45_PageSize(pAt45);
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	AT45_ErasePage(pAt45,adr);
}

static const char LimitCode[][15] = 
{	"基础黑名单超限",
	"新增黑名单超限",
	"新删黑名单超限",
	"白名单超限",
	"油品油价表超限",
	"油站信息超限",
	"POS ON LINE",
	"POS修改时间",
	"PUMP修改时间",
	"指令异常",
	"数据异常",
    "复位类型",
    "液晶忙",
	"交易CRC错误"
};
void WriteDriveLog_Reset(unsigned char typ,unsigned int adr)
{
    unsigned char buf[63];
	unsigned char timebuf[7];
	unsigned char len = 0;
    unsigned char k;
    
    ReadSystimer(timebuf);
	memset(buf,0,sizeof(buf));
	sprintf(( char * )&buf, "%02X", timebuf[2] );
	len += 2;
	sprintf(( char * )&buf[len], "%02X", timebuf[3]);
	len += 2;
	sprintf(( char * )&buf[len], "%02X", timebuf[4]);
	len += 2;
	sprintf(( char * )&buf[len], "%02X", timebuf[5]);
	len += 2;
	sprintf(( char * )&buf[len], "%02X", timebuf[6]);
	len += 2;
	buf[len++] = ':';
	buf[len++] = ' ';
    
    memcpy(&buf[len],&LimitCode[11],strlen(LimitCode[11]));
	len += strlen(LimitCode[11]);
    buf[len++] = typ + '0';
    buf[len++] = ' ';
    k = sprintf((char *)(&buf[len]),"%.8X",adr);
    len += k;
    buf[len++] = '\0';
    AscLog(buf);
}

void WriteDriveLog(unsigned char typ,unsigned char *s)
{
	unsigned char buf[63];
	unsigned char timebuf[7];
	unsigned char len = 0;
	
	ReadSystimer(timebuf);
	memset(buf,0,sizeof(buf));
	sprintf(( char * )&buf, "%02X", timebuf[2] );
	len += 2;
	sprintf(( char * )&buf[len], "%02X", timebuf[3]);
	len += 2;
	sprintf(( char * )&buf[len], "%02X", timebuf[4]);
	len += 2;
	sprintf(( char * )&buf[len], "%02X", timebuf[5]);
	len += 2;
	sprintf(( char * )&buf[len], "%02X", timebuf[6]);
	len += 2;
	buf[len++] = ':';
	buf[len++] = ' ';
	if(s == (unsigned char *)0)
	{
		memcpy(&buf[len],&LimitCode[typ],strlen(LimitCode[typ]) + 1);
		len += strlen(LimitCode[typ]) + 1;
	}
	else
	{
		memcpy(&buf[len],&LimitCode[typ],strlen(LimitCode[typ]));
		len += strlen(LimitCode[typ]);
	}
	if(s != (unsigned char *)0)
	{
		memcpy(timebuf,s,7);
		sprintf(( char * )&buf[len], "%02X", timebuf[0] );
		len += 2;
		sprintf(( char * )&buf[len], "%02X", timebuf[1]);
		len += 2;
		sprintf(( char * )&buf[len], "%02X", timebuf[2] );
		len += 2;
		sprintf(( char * )&buf[len], "%02X", timebuf[3]);
		len += 2;
		sprintf(( char * )&buf[len], "%02X", timebuf[4]);
		len += 2;
		sprintf(( char * )&buf[len], "%02X", timebuf[5]);
		len += 2;
		sprintf(( char * )&buf[len], "%02X", timebuf[6]);
		len += 2;
		buf[len++] = '\0';
	}
	AscLog(buf);
}
