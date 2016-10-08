#include "global.h"

static unsigned char ListBuf[482];
static ListHead BaseBlackListHead;
static ListHead AddBlackListHead;
static ListHead DelBlackListHead;
static ListHead WhiteListHead;

static unsigned char ListLimit;

const char format_config_ok[] = "format config complete...\n\r";
const char format_record_ok[] = "format record complete...\n\r";
const char format_protected_ok[] = "format protected complete...\n\r";
const char format_log_ok[] = "format log complete...\n\r";
const char format_all_ok[] = "format all complete...\n\r";

void FormatConfig(void)
{
	At45 *pAt45;
	unsigned char flg;
	unsigned int i;
	
	memset((char *)(&cr_protected.BaseBlackListIntegrality),0xff,6 * sizeof(short) / sizeof(char));
	SetPriceUpdateFlg(0);
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
	pAt45 = &at45[CONFIG_ID];
	for ( i = LIST_START; i <LIST_END; i++ )
	{
		SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		do
		{
			flg = AT45_ErasePage(pAt45, i * AT45_PageSize(pAt45));
			WatchDog();
			CheckTimerGo();
		}while ( flg );
	}
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	AT45_ErasePage(pAt45, LIST_INTEGRALITY * AT45_PageSize(pAt45));
	WriteToDBUG_USART((unsigned char *)&format_config_ok,strlen(format_config_ok));
	CalculateCrc();
}

void FormatRecord(void)
{
	At45 *pAt45;
	unsigned char flg;
	unsigned int i,adr;
	
	cr_protected.RecordStartPageAddr = RECORD_START_PAGE;
	cr_protected.RecordEndPageAddr = RECORD_END_PAGE;
	cr_protected.RecordWriteTTC = 1;
	//cr_protected.RecordCurrentPageAddr = RECORD_START_PAGE;
	//cr_protected.RecordCountInPage = 0;
	cr_protected.RecordReadTTC = 1;
	cr_protected.UpTransferPageAddr = RECORD_START_PAGE;
	cr_protected.UpTransferOffsetInPage = 0;
	pAt45 = &at45[TRANS_ID];
	for ( i = RECORD_START_PAGE; i < RECORD_END_PAGE + 1; i++ )
	{
		SPID_ConfigureCS(pAt45->pSpid, TRANS_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		do
		{
			adr = i * AT45_PageSize(pAt45);
			flg = AT45_ErasePage(pAt45,adr);
			WatchDog();
			CheckTimerGo();
		}while ( flg );
	}
	WriteToDBUG_USART((unsigned char *)&format_record_ok,strlen(format_record_ok));
	CalculateCrc();
}

void FormatProtected(void)
{
	At45 *pAt45;
	unsigned char flg;
	unsigned int adr;
	
	pAt45 = &at45[0];
	SPID_ConfigureCS(pAt45->pSpid, 0, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = PROTECTED_START * AT45_PageSize(pAt45);
	do
	{
		flg = AT45_ErasePage(pAt45,adr);
		WatchDog();
		CheckTimerGo();
	}while ( flg );
	SPID_ConfigureCS(pAt45->pSpid, 0, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = CR_CONFIG_ADDRESS * AT45_PageSize(pAt45);
	do
	{
		flg = AT45_ErasePage(pAt45,adr);
		WatchDog();
		CheckTimerGo();
	}while ( flg );
	SPID_ConfigureCS(pAt45->pSpid, 0, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = POWERDOWN_TRANS * AT45_PageSize(pAt45);
	do
	{
		flg = AT45_ErasePage(pAt45,adr);
		WatchDog();
		CheckTimerGo();
	}while ( flg );
	SPID_ConfigureCS(pAt45->pSpid, 1, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = PROTECTED_BACKUP_ADDR * AT45_PageSize(pAt45);
	do
	{
		flg = AT45_ErasePage(pAt45,adr);
		WatchDog();
		CheckTimerGo();
	}while ( flg );
	
	//memset((unsigned char *)(&cr_protected),0,sizeof(cr_protected));	
	WriteToDBUG_USART((unsigned char *)&format_protected_ok,strlen(format_protected_ok));
	SetCrcError(0xff);
}

void FormatLog(void)
{
	unsigned int i,adr;
	unsigned char flg;
	At45 *pAt45;
	
	pAt45 = &at45[LOG_ID];
	cr_protected.LogWriteFlag = 0;
	cr_protected.LogWriteCurrentPageAddr = 0x202;
	cr_protected.LogWritePageNumber = 0;
	for(i = 0x202; i < 0x1000; i++)
	{
		SPID_ConfigureCS(pAt45->pSpid, TRANS_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		do
		{
			adr = i *AT45_PageSize(pAt45);
			flg = AT45_ErasePage(pAt45,adr);
			WatchDog();
			CheckTimerGo();
		}while(flg);
	}
	WriteToDBUG_USART((unsigned char *)&format_log_ok,strlen(format_log_ok));
	CalculateCrc();
}

void FormatAll(void)
{
	unsigned int i,adr;
	unsigned char flg;
	At45 *pAt45;
	
	pAt45 = &at45[0];
	for(i = 498; i < 4096; i++)
	{
		SPID_ConfigureCS(pAt45->pSpid, 0, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		do
		{
			adr = i *AT45_PageSize(pAt45);
			flg = AT45_ErasePage(pAt45,adr);
			WatchDog();
			CheckTimerGo();
		}while(flg);
	}
	pAt45 = &at45[1];
	for(i = 0; i < 4096; i++)
	{
		SPID_ConfigureCS(pAt45->pSpid, 1, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		do
		{
			adr = i *AT45_PageSize(pAt45);
			flg = AT45_ErasePage(pAt45,adr);
			WatchDog();
			CheckTimerGo();
		}while(flg);
	}
	WriteToDBUG_USART((unsigned char *)&format_all_ok,strlen(format_all_ok));
}

void FormatDataFlash(unsigned char typ)
{
    unsigned int i,adr;
    At45 *pAt45;
    unsigned char flg;

	AIC_DisableIT(AT91C_ID_IRQ0);  
	AIC_DisableIT(AT91C_ID_IRQ1);
	AIC_DisableIT(AT91C_ID_US0);
	AIC_DisableIT(AT91C_ID_US1);
	AIC_DisableIT(AT91C_ID_TC0);
	AIC_DisableIT(AT91C_ID_TC1);
	AIC_DisableIT(AT91C_ID_SYS);
    
	if ( typ == LIST_CONFIG )
    {
        memset(&cr_protected.BaseBlackListIntegrality,0xffff,6 * sizeof(short) / sizeof(char));
		SetPriceUpdateFlg(0);
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
        pAt45 = &at45[CONFIG_ID];
		SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		AT45_ErasePage(pAt45, LIST_INTEGRALITY * AT45_PageSize(pAt45));
        for ( i = LIST_START; i <LIST_END; i++ )
        {
            SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
            do
			{
            	flg = AT45_ErasePage(pAt45, i * AT45_PageSize(pAt45));
                WatchDog();
            }while ( flg );
        }
		if(IdleFlg & RECLAIM_WRITETTC)
		{
			if(!WriteTTCDec())
			{
				WriteTTCDec();
			}
		}
        do
		{
        	flg = SaveConfig(0,0);
            WatchDog();
        }while ( flg );
		ReBuildRecordProtected();
		ReBuildLogProtected();
		CalculateCrc();
        while ( 1 )
        {
        }
    }
    else
    {
        cr_protected.RecordStartPageAddr = RECORD_START_PAGE;
        cr_protected.RecordEndPageAddr = RECORD_END_PAGE;
        //cr_protected.RecordWriteTTC = 1;
		//liuzg 20100902
		cr_protected.RecordReadTTC = cr_protected.RecordWriteTTC;
        //cr_protected.RecordCurrentPageAddr = RECORD_START_PAGE;
        //cr_protected.RecordCountInPage = 0;
        //cr_protected.RecordReadTTC = 1;
        cr_protected.UpTransferPageAddr = RECORD_START_PAGE;
        cr_protected.UpTransferOffsetInPage = 0;
        pAt45 = &at45[TRANS_ID];
        for ( i = RECORD_START_PAGE; i < RECORD_END_PAGE + 1; i++ )
        {
            SPID_ConfigureCS(pAt45->pSpid, TRANS_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
            do
			{
                adr = i * AT45_PageSize(pAt45);
                flg = AT45_ErasePage(pAt45,adr);
                WatchDog();
            }while ( flg );
        }
        do
		{
            flg = SaveConfig(0,0);
            WatchDog();
        }while ( flg );
		ReBuildRecordProtected();
		ReBuildLogProtected();
		CalculateCrc();
        while ( 1 )
        {
        }
    }
}

static unsigned int WriteListNumber;

unsigned char WriteList(unsigned char typ,unsigned char *s,unsigned int len,unsigned char style)
{
    At45 *pAt45;
    unsigned int adr,start;
	pAt45 = &at45[CONFIG_ID];
	unsigned char buf[8];
	unsigned short crc;
	//unsigned int length;
	unsigned int number,j;
	int page;
	
	if(s == (unsigned char*)0)
	{
		return -1;
	}
    if ( (len < 160) && (style == 0) )
    {
        return 3;
    }
	else if(len > 480)
	{
		return 3;
	}
    switch ( typ )
    {
    case BASEBLACKLIST :
		if(style != 0)
		{
			/*length = len / 10;
			if(len % 10)
			{
				length *= 10;
				memset(&s[length],0xff,len - length);
			}*/
			memcpy((unsigned char *)(&number),BaseBlackListHead.ListAmount,4);
			number = IntEndianConvert(number);
			if(number > 20000)
			{
				number = 20000;
			}
			j = number - WriteListNumber;
			j *= 10;
			memset(&s[j],0xff,len - j);
		}
        if ( ListFlg.BaseBlackListPageAddr >= BASE_BLACK_LIST_END )
        {
            return 4;
        }
        if ( cr_protected.BaseBlackListIntegrality == 0 )
        {
            return 0;
        }
        else
        {
            if ( cr_protected.BaseBlackListIntegrality == 1 )
            {
                if (  DestroyListIntegrality(typ) )
                {
                    return 2;
                }
            }
        }
        if ( ListFlg.BaseBlackListCount == 0  )
        {
            memset(ListBuf,0xff,480);
        }
        start = ListFlg.BaseBlackListCount * 160;
        memcpy(&ListBuf[start],s,len);
        if ( style == 1 )
        {
            ListFlg.crc = Crc16(ListBuf,480,0);
            memcpy(&ListBuf[480],(unsigned char *)(&ListFlg.crc),2);
            adr = ListFlg.BaseBlackListPageAddr * AT45_PageSize(pAt45);
            if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
            {
                return 1;
            }
			else if(ListLimit)
			{
				ListLimit = 0;
				cr_protected.BaseBlackListIntegrality = 0xffff;
				CalculateCrc();
                ListFlg.BaseBlackListPageAddr = BASE_BLACK_LIST_ADDRESS;
				SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
				adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
				READ(CONFIG_ID,buf,adr,8);
				buf[0] = 0xff;
				crc = Crc16(buf,6,0);
				memcpy(&buf[6],(unsigned char *)(&crc),2);
				WRITE(CONFIG_ID,buf,adr,8);
			}
            else
            {
                cr_protected.BaseBlackListIntegrality = 0;
				CalculateCrc();
				SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
				adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
				READ(CONFIG_ID,buf,adr,8);
				buf[0] = 0;
				crc = Crc16(buf,6,0);
				memcpy(&buf[6],(unsigned char *)(&crc),2);
				WRITE(CONFIG_ID,buf,adr,8);
				ListFlg.BaseBlackListCount = 0;
				memcpy(&BBLLink.CardNumber[ListFlg.BaseBlackListPageAddr - BASE_BLACK_LIST_ADDRESS],ListBuf,10);
				ListFlg.BaseBlackListPageAddr = BASE_BLACK_LIST_ADDRESS;
				page = (cr_protected.BaseBlackListLength - 16) / 480;
				if((cr_protected.BaseBlackListLength - 16) % 480)
				{
					page++;
				}
				if(page < (BASE_BLACK_LIST_END - BASE_BLACK_LIST_ADDRESS))
				{
					page += BASE_BLACK_LIST_ADDRESS;
				}
				else
				{
					page = BASE_BLACK_LIST_END;
				}
				//ListBuf[479] += 1;
				memset(&ListBuf[470],0xff,10);
				memcpy(&BBLLink.CardNumber[page - BASE_BLACK_LIST_ADDRESS],&ListBuf[470],10);
				BBLLink.crc = Crc16(&BBLLink.CardNumber[0][0], 10 * (BASEBLACK_LINKMAX + 1), 0);
               /* if ( BuildListLink(typ) == 0 )
                {
                    ListFlg.BaseBlackListCount = 0;
                }
                else
                {
                    return 1;
                }*/
            }
        }
        else
        {
            if ( style > 1 )
            {
                adr = ListFlg.BaseBlackListPageAddr * AT45_PageSize(pAt45);
                if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
                {
                    return 1;
                }
				else if(ListLimit)
				{
					ListLimit = 0;
					cr_protected.BaseBlackListIntegrality = 0xffff;
					CalculateCrc();
                    ListFlg.BaseBlackListPageAddr = BASE_BLACK_LIST_ADDRESS;
					SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
					adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
					READ(CONFIG_ID,buf,adr,8);
					buf[0] = 0xff;
					crc = Crc16(buf,6,0);
					memcpy(&buf[6],(unsigned char *)(&crc),2);
					WRITE(CONFIG_ID,buf,adr,8);
				}
                else
                {
                    cr_protected.BaseBlackListIntegrality = 0;
					CalculateCrc();
					SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
					adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
					READ(CONFIG_ID,buf,adr,8);
					buf[0] = 0;
					crc = Crc16(buf,6,0);
					memcpy(&buf[6],(unsigned char *)(&crc),2);
					WRITE(CONFIG_ID,buf,adr,8);
					ListFlg.BaseBlackListCount = 0;
					memcpy(&BBLLink.CardNumber[ListFlg.BaseBlackListPageAddr - BASE_BLACK_LIST_ADDRESS],ListBuf,10);
					ListFlg.BaseBlackListPageAddr = BASE_BLACK_LIST_ADDRESS;
					page = (cr_protected.BaseBlackListLength - 16) / 480;
					if((cr_protected.BaseBlackListLength - 16) % 480)
					{
						page++;
					}
					if(page < (BASE_BLACK_LIST_END - BASE_BLACK_LIST_ADDRESS))
					{
						page += BASE_BLACK_LIST_ADDRESS;
					}
					else
					{
						page = BASE_BLACK_LIST_END;
					}
					ListBuf[479] += 1;
					memcpy(&BBLLink.CardNumber[page - BASE_BLACK_LIST_ADDRESS],&ListBuf[470],10);
					BBLLink.crc = Crc16(&BBLLink.CardNumber[0][0], 10 * (BASEBLACK_LINKMAX + 1), 0);
                    /*if ( BuildListLink(typ) == 0 )
                    {
                        ListFlg.BaseBlackListCount = 0;
                    }
                    else
                    {
                        return 1;
                    }*/
                }
            }
            else
            {
                if ( ListFlg.BaseBlackListCount == 2 )
                {
                    ListFlg.crc = Crc16(ListBuf,480,0);
                    memcpy(&ListBuf[480],(unsigned char *)(&ListFlg.crc),2);
                    adr = ListFlg.BaseBlackListPageAddr * AT45_PageSize(pAt45);
                    if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
                    {
                        return 1;
                    }
                    else
                    {
						memcpy(&BBLLink.CardNumber[ListFlg.BaseBlackListPageAddr - BASE_BLACK_LIST_ADDRESS],ListBuf,10);
                        ListFlg.BaseBlackListPageAddr++;
                        ListFlg.BaseBlackListCount = 0;
                    }
                }
                else
                {
                    ListFlg.BaseBlackListCount++;
                }
				WriteListNumber += 16;
            }
        }
        break;
    case ADDBLACKLIST :
		if(style != 0)
		{
			/*length = len / 10;
			if(len % 10)
			{
				length *= 10;
				memset(&s[length],0xff,len - length);
			}*/
			memcpy((unsigned char *)(&number),AddBlackListHead.ListAmount,4);
			number = IntEndianConvert(number);
			j = number - WriteListNumber;
			j *= 10;
			memset(&s[j],0xff,len - j);
		}
        if ( ListFlg.AddBlackListPageAddr >= ADD_BLACK_LIST_END )
        {
            return 4;
        }
        if ( cr_protected.AddBlackListIntegrality == 0 )
        {
            return 0;
        }
        else
        {
            if ( cr_protected.AddBlackListIntegrality == 1 )
            {
                if ( DestroyListIntegrality(typ) )
                {
                    return 2;
                }
            }
            if ( ListFlg.AddBlackListCount == 0 )
            {
				memset(ListBuf,0xff,480);
            }
            start = ListFlg.AddBlackListCount * 160;
            memcpy(&ListBuf[start],s,len);
            if ( style == 1 )
            {
                ListFlg.crc = Crc16(ListBuf,480,0);
                memcpy(&ListBuf[480],(unsigned char *)(&ListFlg.crc),2);
                adr = ListFlg.AddBlackListPageAddr * AT45_PageSize(pAt45);
                if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
                {
                    return 1;
                }
				else if(ListLimit)
				{
					ListLimit = 0;
					cr_protected.AddBlackListIntegrality = 0xffff;
					CalculateCrc();
                    ListFlg.AddBlackListPageAddr = ADD_BLACK_LIST_ADDRESS;
					SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
					adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
					READ(CONFIG_ID,buf,adr,8);
					buf[1] = 0xff;
					crc = Crc16(buf,6,0);
					memcpy(&buf[6],(unsigned char *)(&crc),2);
					WRITE(CONFIG_ID,buf,adr,8);
				}
                else
                {
                    cr_protected.AddBlackListIntegrality = 0;
					CalculateCrc();
                    ListFlg.AddBlackListPageAddr = ADD_BLACK_LIST_ADDRESS;
					SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
					adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
					READ(CONFIG_ID,buf,adr,8);
					buf[1] = 0;
					crc = Crc16(buf,6,0);
					memcpy(&buf[6],(unsigned char *)(&crc),2);
					WRITE(CONFIG_ID,buf,adr,8);
                    if ( BuildListLink(typ) == 0  )
                    {
                        ListFlg.AddBlackListCount = 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if ( style > 1 )
                {
                    adr = ListFlg.AddBlackListPageAddr * AT45_PageSize(pAt45);
                    if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
                    {
                        return 1;
                    }
					else if(ListLimit)
					{
						ListLimit = 0;
						cr_protected.AddBlackListIntegrality = 0xffff;
						CalculateCrc();
                        ListFlg.AddBlackListPageAddr = ADD_BLACK_LIST_ADDRESS;
						SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
						adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
						READ(CONFIG_ID,buf,adr,8);
						buf[1] = 0xff;
						crc = Crc16(buf,6,0);
						memcpy(&buf[6],(unsigned char *)(&crc),2);
						WRITE(CONFIG_ID,buf,adr,8);
					}
                    else
                    {
                        cr_protected.AddBlackListIntegrality = 0;
						CalculateCrc();
                        ListFlg.AddBlackListPageAddr = ADD_BLACK_LIST_ADDRESS;
						SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
						adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
						READ(CONFIG_ID,buf,adr,8);
						buf[1] = 0;
						crc = Crc16(buf,6,0);
						memcpy(&buf[6],(unsigned char *)(&crc),2);
						WRITE(CONFIG_ID,buf,adr,8);
                        if ( BuildListLink(typ) == 0 )
                        {
                            ListFlg.AddBlackListCount = 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if ( ListFlg.AddBlackListCount == 2 )
                    {
                        ListFlg.crc = Crc16(ListBuf,480,0);
                        memcpy(&ListBuf[480],(unsigned char *)(&ListFlg.crc),2);
                        adr = ListFlg.AddBlackListPageAddr * AT45_PageSize(pAt45);
                        if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
                        {
                            return 1;
                        }
                        else
                        {
                            ListFlg.AddBlackListPageAddr++;
                            ListFlg.AddBlackListCount = 0;
                        }
                    }
                    else
                    {
                        ListFlg.AddBlackListCount++;
                    }
					WriteListNumber += 16;
                }
            }
        }
        break;
    case DELBLACKLIST :
		if(style != 0)
		{
			/*length = len / 10;
			if(len % 10)
			{
				length *= 10;
				memset(&s[length],0xff,len - length);
			}*/
			memcpy((unsigned char *)(&number),DelBlackListHead.ListAmount,4);
			number = IntEndianConvert(number);
			j = number - WriteListNumber;
			j *= 10;
			memset(&s[j],0xff,len - j);
		}
        if ( ListFlg.DelBlackListPageAddr >= DEL_BLACK_LIST_END )
        {
            return 4;
        }
        if ( cr_protected.DelBlackListIntegrality == 0 )
        {
            return 0;
        }
        else
        {
            if ( cr_protected.DelBlackListIntegrality == 1 )
            {
                if ( DestroyListIntegrality(typ) )
                {
                    return 2;
                }
            }
        }
        if ( ListFlg.DelBlackListCount == 0 )
        {
			memset(ListBuf,0xff,480);
        }
        start = ListFlg.DelBlackListCount * 160;
        memcpy(&ListBuf[start],s,len);
        if ( style == 1 )
        {
            ListFlg.crc = Crc16(ListBuf,480,0);
            memcpy(&ListBuf[480],(unsigned char *)(&ListFlg.crc),2);
            adr = ListFlg.DelBlackListPageAddr * AT45_PageSize(pAt45);
            if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
            {
                return 1;
            }
			else if(ListLimit)
			{
				ListLimit = 0;
				cr_protected.DelBlackListIntegrality = 0xffff;
				CalculateCrc();
                ListFlg.DelBlackListPageAddr = DEL_BLACK_LIST_ADDRESS;
				SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
				adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
				READ(CONFIG_ID,buf,adr,8);
				buf[2] = 0xff;
				crc = Crc16(buf,6,0);
				memcpy(&buf[6],(unsigned char *)(&crc),2);
				WRITE(CONFIG_ID,buf,adr,8);
			}
            else
            {
                cr_protected.DelBlackListIntegrality = 0;
				CalculateCrc();
                ListFlg.DelBlackListPageAddr = DEL_BLACK_LIST_ADDRESS;
				SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
				adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
				READ(CONFIG_ID,buf,adr,8);
				buf[2] = 0;
				crc = Crc16(buf,6,0);
				memcpy(&buf[6],(unsigned char *)(&crc),2);
				WRITE(CONFIG_ID,buf,adr,8);
                if ( BuildListLink(typ) == 0 )
                {
                    ListFlg.DelBlackListCount = 0;
                }
                else
                {
                    return 1;
                }
            }
        }
        else
        {
            if (  style > 1 )
            {
                adr = ListFlg.DelBlackListPageAddr * AT45_PageSize(pAt45);
                if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
                {
                    return 1;
                }
				else if(ListLimit)
				{
					ListLimit = 0;
					cr_protected.DelBlackListIntegrality = 0xffff;
					CalculateCrc();
                    ListFlg.DelBlackListPageAddr = DEL_BLACK_LIST_ADDRESS;
					SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
					adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
					READ(CONFIG_ID,buf,adr,8);
					buf[2] = 0xff;
					crc = Crc16(buf,6,0);
					memcpy(&buf[6],(unsigned char *)(&crc),2);
					WRITE(CONFIG_ID,buf,adr,8);
				}
                else
                {
                    cr_protected.DelBlackListIntegrality = 0;
					CalculateCrc();
                    ListFlg.DelBlackListPageAddr = DEL_BLACK_LIST_ADDRESS;
					SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
					adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
					READ(CONFIG_ID,buf,adr,8);
					buf[2] = 0;
					crc = Crc16(buf,6,0);
					memcpy(&buf[6],(unsigned char *)(&crc),2);
					WRITE(CONFIG_ID,buf,adr,8);
                    if ( BuildListLink(typ) == 0 )
                    {
                        ListFlg.DelBlackListCount = 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if ( ListFlg.DelBlackListCount == 2 )
                {
                    ListFlg.crc = Crc16(ListBuf,480,0);
                    memcpy(&ListBuf[480],(unsigned char *)(&ListFlg.crc),2);
                    adr = ListFlg.DelBlackListPageAddr * AT45_PageSize(pAt45);
                    if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
                    {
                        return 1;
                    }
                    else
                    {
                        ListFlg.DelBlackListPageAddr++;
                        ListFlg.DelBlackListCount = 0;
                    }
                }
                else
                {
                    ListFlg.DelBlackListCount++;
                }
				WriteListNumber += 16;
            }
        }
        break;
    case WHITELIST :
		if(style != 0)
		{
			/*length = len / 10;
			if(len % 10)
			{
				length *= 10;
				memset(&s[length],0xff,len - length);
			}*/
			memcpy((unsigned char *)(&number),WhiteListHead.ListAmount,4);
			number = IntEndianConvert(number);
			j = number - WriteListNumber;
			j *= 10;
			memset(&s[j],0xff,len - j);
		}
        if ( ListFlg.WhiteListPageAddr >= WHITE_LIST_END )
        {
            return 4;
        }
        if ( cr_protected.WhiteListIntegrality ==0 )
        {
            return 0;
        }
        else
        {
            if ( cr_protected.WhiteListIntegrality == 1 )
            {
                if ( DestroyListIntegrality(typ) )
                {
                    return 2;
                }
            }
        }
        if ( ListFlg.WhiteListCount == 0 )
        {
			memset(ListBuf,0xff,480);
        }
        start = ListFlg.WhiteListCount * 160;
        memcpy(&ListBuf[start],s,len);
        if ( style == 1 )
        {
            ListFlg.crc = Crc16(ListBuf,480,0);
            memcpy(&ListBuf[480],(unsigned char *)(&ListFlg.crc),2);
            adr = ListFlg.WhiteListPageAddr * AT45_PageSize(pAt45);
            if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
            {
                return 1;
            }
			else if(ListLimit)
			{
				ListLimit = 0;
				cr_protected.WhiteListIntegrality = 0xffff;
				CalculateCrc();
                ListFlg.WhiteListPageAddr = WHITE_LIST_ADDRESS;
				SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
				adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
				READ(CONFIG_ID,buf,adr,8);
				buf[3] = 0xff;
				crc = Crc16(buf,6,0);
				memcpy(&buf[6],(unsigned char *)(&crc),2);
				WRITE(CONFIG_ID,buf,adr,8);
			}
            else
            {
                cr_protected.WhiteListIntegrality = 0;
				CalculateCrc();
                ListFlg.WhiteListPageAddr = WHITE_LIST_ADDRESS;
				SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
				adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
				READ(CONFIG_ID,buf,adr,8);
				buf[3] = 0;
				crc = Crc16(buf,6,0);
				memcpy(&buf[6],(unsigned char *)(&crc),2);
				WRITE(CONFIG_ID,buf,adr,8);
                if ( BuildListLink(typ) == 0 )
                {
                    ListFlg.WhiteListCount = 0;
                }
                else
                {
                    return 1;
                }
            }
        }
        else
        {
            if ( style > 1 )
            {
                adr = ListFlg.WhiteListPageAddr * AT45_PageSize(pAt45);
                if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
                {
                    return 1;
                }
				else if(ListLimit)
				{
					ListLimit = 0;
					cr_protected.WhiteListIntegrality = 0xffff;
					CalculateCrc();
                    ListFlg.WhiteListPageAddr = WHITE_LIST_ADDRESS;
					SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
					adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
					READ(CONFIG_ID,buf,adr,8);
					buf[3] = 0xff;
					crc = Crc16(buf,6,0);
					memcpy(&buf[6],(unsigned char *)(&crc),2);
					WRITE(CONFIG_ID,buf,adr,8);
				}
                else
                {
                    cr_protected.WhiteListIntegrality = 0;
					CalculateCrc();
                    ListFlg.WhiteListPageAddr = WHITE_LIST_ADDRESS;
					SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
					adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
					READ(CONFIG_ID,buf,adr,8);
					buf[3] = 0;
					crc = Crc16(buf,6,0);
					memcpy(&buf[6],(unsigned char *)(&crc),2);
					WRITE(CONFIG_ID,buf,adr,8);
                    if ( BuildListLink(typ) == 0 )
                    {
                        ListFlg.WhiteListCount = 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if ( ListFlg.WhiteListCount == 2 )
                {
                    ListFlg.crc = Crc16(ListBuf,480,0);
                    memcpy(&ListBuf[480],(unsigned char *)(&ListFlg.crc),2);
                    adr = ListFlg.WhiteListPageAddr * AT45_PageSize(pAt45);
                    if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
                    {
                        return 1;
                    }
                    else
                    {
                        ListFlg.WhiteListPageAddr++;
                        ListFlg.WhiteListCount = 0;
                    }
                }
                else
                {
                    ListFlg.WhiteListCount++;
                }
				WriteListNumber += 16;
            }
        }
        break;
    case GRADEPRICE :
        if ( ListFlg.GradePricePageAddr > GRADE_PRICE_ADDRESS  )
        {
            return 4;
        }
        if ( cr_protected.GradePriceIntegrality == 0 )
        {
			LoadGradePriceList();
            return 0;
        }
        else
        {
            if ( cr_protected.GradePriceIntegrality == 1 )
            {
                if ( DestroyListIntegrality(typ) )
                {
                    return 2;
                }
            }
        }
        if ( ListFlg.GradePriceCount == 0 )
        {
            memset(ListBuf,0xff,480);
        }
        start = ListFlg.GradePriceCount * 160;
        memcpy(&ListBuf[start],s,len);
        if ( style == 1 )
        {
            ListFlg.crc = Crc16(ListBuf,480,0);
            memcpy(&ListBuf[480],(unsigned char *)(&ListFlg.crc),2);
            adr = ListFlg.GradePricePageAddr * AT45_PageSize(pAt45);

            if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
            {
                return 1;
            }
            else
            {
                cr_protected.GradePriceIntegrality = 0;
				CalculateCrc();
                ListFlg.GradePricePageAddr = GRADE_PRICE_ADDRESS;    
				SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
				adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
				READ(CONFIG_ID,buf,adr,8);
				buf[4] = 0;
				crc = Crc16(buf,6,0);
				memcpy(&buf[6],(unsigned char *)(&crc),2);
				WRITE(CONFIG_ID,buf,adr,8);
                ListFlg.GradePriceCount = 0;
				if(!CombinationPriceList())
				{
				if(LoadGradePriceList() == 3)
				{
					SetPriceUpdateFlg(1);
					CalculateCrc();
	                LoadGradePriceTime();
				}
            }
				else
				{
					return -1;
				}
			}
        }
        else
        {
            if ( style > 1 )
            {
                adr = ListFlg.GradePricePageAddr * AT45_PageSize(pAt45);
                if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
                {
                    return 1;
                }
                else
                {
                    cr_protected.GradePriceIntegrality = 0;
					CalculateCrc();
                    ListFlg.GradePricePageAddr = GRADE_PRICE_ADDRESS; 
					SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
					adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
					READ(CONFIG_ID,buf,adr,8);
					buf[4] = 0;
					crc = Crc16(buf,6,0);
					memcpy(&buf[6],(unsigned char *)(&crc),2);
					WRITE(CONFIG_ID,buf,adr,8);
                    ListFlg.GradePriceCount = 0;
					SetPriceUpdateFlg(1);
					CalculateCrc();
					if(!CombinationPriceList())
					{
						if(!LoadGradePriceList())
						{
                    LoadGradePriceTime();
                }
						else if(errno == PRICE_ERROR_FLAGS)
						{
							return -1;
						}
							
					}
					else
					{
						return -1;
					}
                }
            }
            else
            {
                if ( ListFlg.GradePriceCount == 2 )
                {
                    ListFlg.crc = Crc16(ListBuf,480,0);
                    memcpy(&ListBuf[480],(unsigned char *)(&ListFlg.crc),2);
                    adr = ListFlg.GradePricePageAddr * AT45_PageSize(pAt45);
                    if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
                    {
                        return 1;
                    }
                    else
                    {
                        ListFlg.GradePricePageAddr++;
                        ListFlg.GradePriceCount = 0;
                    }
                }
                else
                {
                    ListFlg.GradePriceCount++;
                }
            }
        }
        break;
    case OILSTOP :
        if ( ListFlg.OilStopPageAddr > OIL_STOP_ADDRESS )
        {
            return 4;
        }
        if ( cr_protected.OilStopIntegrality == 0 )
        {
			LoadOilStopMessage();
            return 0;
        }
        else
        {
            if ( cr_protected.OilStopIntegrality == 1 )
            {
                if ( DestroyListIntegrality(typ) )
                {
                    return 2;
                }
            }
        }
        if ( ListFlg.OilStopCount == 0 )
        {
            memset(ListBuf,0xff,480);
        }
        start = ListFlg.OilStopCount * 160;
        memcpy(&ListBuf[start],s,len);
        if ( style == 1 )
        {
            ListFlg.crc = Crc16(ListBuf,480,0);
            memcpy(&ListBuf[480],(unsigned char *)(&ListFlg.crc),2);
            adr = ListFlg.OilStopPageAddr * AT45_PageSize(pAt45);
            if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
            {
                return 1;
            }
            else
            {
                cr_protected.OilStopIntegrality = 0;
				CalculateCrc();
                ListFlg.OilStopPageAddr = OIL_STOP_ADDRESS;
				SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
				adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
				READ(CONFIG_ID,buf,adr,8);
				buf[5] = 0;
				crc = Crc16(buf,6,0);
				memcpy(&buf[6],(unsigned char *)(&crc),2);
				WRITE(CONFIG_ID,buf,adr,8);
                ListFlg.OilStopCount = 0;
                LoadOilStopMessage();
            }
        }
        else
        {
            if ( style > 1 )
            {
                adr = ListFlg.OilStopPageAddr * AT45_PageSize(pAt45);
                if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482  )
                {
                    return 1;
                }
                else
                {
                    cr_protected.OilStopIntegrality = 0;
					CalculateCrc();
                    ListFlg.OilStopPageAddr = OIL_STOP_ADDRESS;
					SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
					adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
					READ(CONFIG_ID,buf,adr,8);
					buf[5] = 0;
					crc = Crc16(buf,6,0);
					memcpy(&buf[6],(unsigned char *)(&crc),2);
					WRITE(CONFIG_ID,buf,adr,8);
                    ListFlg.OilStopCount = 0;
                    LoadOilStopMessage();
                }
            }
            else
            {
                if ( ListFlg.OilStopCount == 2 )
                {
                    ListFlg.crc = Crc16(ListBuf,480,0);
                    memcpy(&ListBuf[480],(unsigned char *)(&ListFlg.crc),2);
                    adr = ListFlg.OilStopPageAddr * AT45_PageSize(pAt45);
                    if ( WRITE(CONFIG_ID,ListBuf,adr,482) != 482 )
                    {
                        return 1;
                    }
                    else
                    {
                        ListFlg.OilStopPageAddr++;
                        ListFlg.OilStopCount = 0;
                    }
                }
                else
                {
                    ListFlg.OilStopCount++;
                }
            }
        }
        break;
    default:
        return 3;
    }
	return 0;
}

unsigned char FindList(unsigned char typ,unsigned char *s)
{
    unsigned char buf[10];
    unsigned char result;
    
	if(s == (unsigned char*)0)
	{
		return -1;
	}
    memset(buf,0,10);
    if ( memcmp(buf,s,10) == 0 )
    {
        return 2;
    }
    memset(buf,0xff,10);
    if ( memcmp(buf,s,10) == 0 )
    {
        return 2;
    }
    switch ( typ )
    {
    case BASEBLACKLIST :
        result = FindBaseBlackList(s);
        break;
    case ADDBLACKLIST :
        result = FindAddBlackList(s);
        break;
    case DELBLACKLIST :
        result = FindDelBlackList(s);
        break;
    case WHITELIST :
        result = FindWhiteList(s);
        break;
    default:
        result = 6;
        break;
    }
    return result;
}

void LoadGradePriceTime(void)
{
    At45 *pAt45;
    unsigned int adr;
    unsigned char i;
    unsigned char buf[6];
    
    pAt45 = &at45[CONFIG_ID];
    adr = ListFlg.GradePricePageAddr * AT45_PageSize(pAt45);
    adr += 1;
    for ( i = 0; i < 3; i++ )
    {
        if ( READ(CONFIG_ID,buf,adr,6) == 6 )
        {
            memcpy((unsigned char *)(&PriceUpdateTime),buf,6);
            PriceUpdateTime.year = ShortEndianConvert(PriceUpdateTime.year);
            PriceUpdateTime.second = 0;
            return ;
        }
    }
    memset((unsigned char *)(&PriceUpdateTime),0xff,7);
}

unsigned char BuildListLink(unsigned char typ)
{
    unsigned char flg;
    switch ( typ )
    {
    case BASEBLACKLIST :
        flg = BuildBaseBlackListLink();
        if ( flg )
        {
            flg = 1;
        }
        break;
    case ADDBLACKLIST :
        flg = BuildAddBlackListLink();
        if ( flg )
        {
            flg = 2;
        }
        break;
    case DELBLACKLIST :
        flg = BuildDelBlackListLink();
        if ( flg )
        {
            flg = 3;
        }
        break;
    case WHITELIST :
        flg = BuildWhiteListLink();
        if ( flg )
        {
            flg = 4;
        }
        break;
    default:
        flg = BuildBaseBlackListLink();
        if ( flg )
        {
            flg = 1;
            break;
        }
        flg = BuildAddBlackListLink();
        if ( flg )
        {
            flg = 2;
            break;
        }
        flg = BuildDelBlackListLink();
        if ( flg )
        {
            flg = 3;
            break;
        }
        flg = BuildWhiteListLink();
        if ( flg )
        {
            flg = 4;
        }
        break;
    }
    return flg;
}

void LoadOilStopMessage(void)
{
    unsigned char buf[482];
    At45 *pAt45;
    unsigned int adr;
    unsigned short crc,crctmp;
    unsigned char len;
    
    pAt45 = &at45[CONFIG_ID];
    adr = ListFlg.OilStopPageAddr * AT45_PageSize(pAt45);
    if ( READ(CONFIG_ID,buf,adr,482) != 482 )
    {
        cr_protected.OilStopIntegrality = 0xffff;
        cr_protected.OilStopLength = 0;
		CalculateCrc();
        return ;
    }
    crc = Crc16(buf,480,0);
    memcpy((unsigned char *)(&crctmp),&buf[480],2);
    if ( crc != crctmp )
    {
        cr_protected.OilStopIntegrality = 0xffff;
        cr_protected.OilStopLength = 0;
		CalculateCrc();
        return ;
    }
	len = buf[12];
	if(len > 250)
	{
		return;
	}
    memcpy(&OilMessage.ver,buf,13);
    len = buf[12];
    memcpy(&OilMessage.nzn,&buf[13],len);
#ifdef MAKE_CARD_MODE2
	Pos2MessageTime = buf[13 + len];
#endif	
    OilMessage.flg = 1;
}

unsigned char ReBuildListProtected(void)
{
    
	if(cr_protected.BaseBlackListIntegrality == 0)
	{
		BuildBaseBlackListLink();
	}
	if(cr_protected.AddBlackListIntegrality == 0)
	{
		BuildAddBlackListLink();
	}
	if(cr_protected.DelBlackListIntegrality == 0)
	{
		BuildDelBlackListLink();
	}
	if(cr_protected.WhiteListIntegrality == 0)
	{
		BuildWhiteListLink();
	}
    return 0;
}

unsigned char FindBaseBlackList(unsigned char *s)
{
    int result1,result2;
    unsigned short crc,crctmp;
    unsigned int i,j,adr;
    unsigned int left,right;
    unsigned char flg;
    unsigned char buf[482];
    At45 *pAt45;
    
	if(s == (unsigned char*)0)
	{
		return -1;
	}
    flg = 0;
    pAt45 = &at45[CONFIG_ID];
    if ( cr_protected.BaseBlackListIntegrality != 0 )
    {
        return 5;
    }
    crc = Crc16(&BBLLink.CardNumber[0][0], 10 * (BASEBLACK_LINKMAX + 1), 0);
    if ( crc  != BBLLink.crc )
    {
        return 3;
    }
    result1 = memcmp(s,&BBLLink.CardNumber[0][0],10);
    if ( result1 == 0 )
    {
        return 0;
    }
    else
    {
        if ( result1 < 0 )
        {
            return 1;
        }
    }
    result1 = memcmp(s,&BBLLink.CardNumber[BASEBLACK_LINKMAX][0],10);
    if ( result1 >= 0 )
    {
        return 1;
    }
    left = 0;
    right = BASEBLACK_LINKMAX;
    for ( i = 0; i < 9; i++ )
    {
        j = (left + right) >> 1;
        result1 = memcmp(s,&BBLLink.CardNumber[j][0],10);
        result2 = memcmp(s,&BBLLink.CardNumber[j+1][0],10);
        if ( (result1 == 0) || (result2 == 0) )
        {
            return 0;
        }
        else
        {
            if ( (result1 > 0) && (result2 < 0) )
            {
                flg = 1;
                break;
            }
            else
            {
                if ( result1 > 0 )
                {
                    left = j;
                }
                else
                {
                    right = j;
                }
            }
        }
    }
    if ( flg == 0 )
    {
        return 1;
    }
    adr = (ListFlg.BaseBlackListPageAddr + j) * AT45_PageSize(pAt45);
    if ( READ(CONFIG_ID,buf,adr,482) != 482 )
    {
        return 4;
    }
    crc = Crc16(buf,480,0);
    memcpy((unsigned char *)(&crctmp),&buf[480],2);
    if ( crc != crctmp )
    {
        return 5;
    }
    left = 0;
    right = 48;
    for ( i = 0; i < 6; i++ )
    {
        j = (left + right) >> 1;
        result1 = memcmp(s,&buf[j * 10],10);
        result2 = memcmp(s,&buf[(j + 1) * 10],10);
        if ( (result1 == 0) || (result2 == 0) )
        {
            return 0;
        }
        else
        {
            if ( (result1 > 0) && (result2 < 0) )
            {
                return 1;
            }
            else
            {
                if ( result1 > 0 )
                {
                    left = j;
                }
                else
                {
                    right = j;
                }
            }
        }
    }
    return 1;
}

unsigned char FindAddBlackList(unsigned char *s)
{
    int result1,result2;
    unsigned short crc,crctmp;
    unsigned int i,j,adr;
    unsigned int left,right;
    unsigned char flg;
    unsigned char buf[482];
    At45 *pAt45;
    
	if(s == (unsigned char*)0)
	{
		return -1;
	}
    flg = 0;
    pAt45 = &at45[CONFIG_ID];
    if ( cr_protected.AddBlackListIntegrality != 0 )
    {
        return 5;
    }
    crc = Crc16(&ABLLink.CardNumber[0][0],10 * (ADDBLACK_LINKMAX + 1),0);
    if ( crc != ABLLink.crc )
    {
        return 2;
    }
    for ( i = 0; i < ADDBLACK_LINKMAX; i++ )
    {
        result1 = memcmp(s,&ABLLink.CardNumber[i][0],10);
        result2 = memcmp(s,&ABLLink.CardNumber[i + 1],10);
        if ( result1 == 0 )
        {
            return 0;
        }
        else
        {
            if ( (result1 > 0) && (result2 < 0) )
            {
                flg = 1;
                break;
            }
        }
    }
    if ( flg == 0 )
    {
        return 1;
    }
    adr = (ListFlg.AddBlackListPageAddr + i) * AT45_PageSize(pAt45);
    if ( READ(CONFIG_ID,buf,adr,482) != 482 )
    {
        return 4;
    }
    crc = Crc16(buf,480,0);
    memcpy((unsigned char *)(&crctmp),&buf[480],2);
    if ( crc != crctmp )
    {
        return 5;
    }
    left = 0;
    right = 48;
    for ( i = 0; i < 6; i++ )
    {
        j = (left + right) >> 1;
        result1 = memcmp(s,&buf[j * 10],10);
        result2 = memcmp(s,&buf[(j + 1) * 10],10);
        if ( (result1 == 0) || (result2 == 0) )
        {
            return 0;
        }
        else
        {
            if ( (result1 > 0) && (result2 < 0) )
            {
                return 1;
            }
            else
            {
                if ( result1 > 0 )
                {
                    left = j;
                }
                else
                {
                    right = j;
                }
            }
        }
    }
    return 1;
}

unsigned char FindDelBlackList(unsigned char *s)
{
    int result1,result2;
    unsigned short crc,crctmp;
    unsigned int i,j,adr;
    unsigned int left,right;
    unsigned char flg;
    unsigned char buf[482];
    At45 *pAt45;
    
	if(s == (unsigned char*)0)
	{
		return -1;
	}
    flg = 0;
    pAt45 = &at45[CONFIG_ID];
    if ( cr_protected.DelBlackListIntegrality != 0 )
    {
        return 5;
    }
    crc = Crc16(&DBLLink.CardNumber[0][0],10 * (DELBLACK_LINKMAX + 1),0);
    if ( crc != DBLLink.crc )
    {
        return 2;
    }
    for ( i = 0; i < DELBLACK_LINKMAX; i++ )
    {
        result1 = memcmp(s,&DBLLink.CardNumber[i][0],10);
        result2 = memcmp(s,&DBLLink.CardNumber[i + 1][0],10);
        if ( result1 == 0 )
        {
            return 0;
        }
        else
        {
            if ( (result1 > 0) && (result2 < 0) )
            {
                flg = 1;
                break;
            }
        }
    }
    if ( flg == 0 )
    {
        return 1;
    }
    adr = (ListFlg.DelBlackListPageAddr + i) * AT45_PageSize(pAt45);
    if ( READ(CONFIG_ID,buf,adr,482) != 482 )
    {
        return 4;
    }
    crc = Crc16(buf,480,0);
    memcpy((unsigned char *)(&crctmp),&buf[480],2);
    if ( crc != crctmp )
    {
        return 5;
    }
    left = 0;
    right = 48;
    for ( i = 0; i < 6; i++ )
    {
        j = (left + right) >> 1;
        result1 = memcmp(s,&buf[j * 10],10);
        result2 = memcmp(s,&buf[(j + 1) * 10],10);
        if ( (result1 == 0) || (result2 == 0) )
        {
            return 0;
        }
        else
        {
            if ( (result1 > 0) && (result2 < 0) )
            {
                return 1;
            }
            else
            {
                if ( result1 > 0 )
                {
                    left = j;
                }
                else
                {
                    right = j;
                }
            }
        }
    }
    return 1;
}

/*unsigned char FindWhiteList(unsigned char *s)
{
	unsigned int i,j,k;
	unsigned int number,adr;
	unsigned char result;
	unsigned char buf[482];
	unsigned short crc,crctmp;
	At45 *pAt45;
    
    pAt45 = &at45[CONFIG_ID];
	j = 0;
	memcpy((unsigned char *)(&number),WhiteListHead.ListAmount,4);
	number = IntEndianConvert(number);
	for ( i = 0; i < WHITE_LINKMAX; i++ )
	{
		adr = (ListFlg.WhiteListPageAddr + i) * AT45_PageSize(pAt45);
		if ( READ(CONFIG_ID,buf,adr,482) != 482 )
		{
			return 4;
		}
		crc = Crc16(buf,480,0);
		memcpy((unsigned char *)(&crctmp),&buf[480],2);
		if ( crc != crctmp )
		{
			return 5;
		}
		for(k = 0; k < 48; k++)
		{
			if(j < number)
			{
				result = memcmp(s,&buf[k * 10],10);
				if(!result)
				{
					return 0;
				}
				j++;
			}
			else
			{
				return 1;
			}
		}
	}
	return 1;
}*/

unsigned char FindWhiteList(unsigned char *s)
{
    int result1,result2;
    unsigned short crc,crctmp;
    unsigned int i,j,adr;
    unsigned int left,right;
    unsigned char flg;
    unsigned char buf[482];
    At45 *pAt45;
    
	if(s == (unsigned char*)0)
	{
		return -1;
	}
    flg = 0;
    pAt45 = &at45[CONFIG_ID];
    if ( cr_protected.WhiteListIntegrality != 0 )
    {
        return 5;
    }
    crc = Crc16(&WLLink.CardNumber[0][0],10 * (WHITE_LINKMAX + 1),0);
    if ( crc != WLLink.crc )
    {
        return 2;
    }
    for ( i = 0; i < WHITE_LINKMAX; i++ )
    {
        result1 = memcmp(s,&WLLink.CardNumber[i][0],10);
        result2 = memcmp(s,&WLLink.CardNumber[i + 1][0],10);
        if ( result1 == 0 )
        {
            return 0;
        }
        else
        {
            if ( (result1 > 0) && (result2 < 0) )
            {
                flg = 1;
                break;
            }
        }
    }
    if ( flg == 0 )
    {
        return 1;
    }
    adr = (ListFlg.WhiteListPageAddr + i) * AT45_PageSize(pAt45);
    if ( READ(CONFIG_ID,buf,adr,482) != 482 )
    {
        return 4;
    }
    crc = Crc16(buf,480,0);
    memcpy((unsigned char *)(&crctmp),&buf[480],2);
    if ( crc != crctmp )
    {
        return 5;
    }
    left = 0;
    right = 48;
    for ( i = 0; i < 6; i++ )
    {
        j = (left + right) >> 1;
        result1 = memcmp(s,&buf[j * 10],10);
        result2 = memcmp(s,&buf[(j + 1) * 10],10);
        if ( (result1 == 0) || (result2 == 0) )
        {
            return 0;
        }
        else
        {
            if ( (result1 > 0) && (result2 < 0) )
            {
                return 1;
            }
            else
            {
                if ( result1 > 0 )
                {
                    left = j;
                }
                else
                {
                    right = j;
                }
            }
        }
    }
    return 1;
}

unsigned char BuildBaseBlackListLink(void)
{
    unsigned char buf[482];
    unsigned short crc,crctmp;
    unsigned short ListCrc;
    unsigned int i,page,adr;
    At45 *pAt45;
    
    pAt45 = &at45[CONFIG_ID];
    ListCrc = 0;
	page = (cr_protected.BaseBlackListLength - 16) / 480;
	if((cr_protected.BaseBlackListLength - 16) % 480)
	{
		page++;
	}
	if(page < (BASE_BLACK_LIST_END - BASE_BLACK_LIST_ADDRESS))
	{
		page += BASE_BLACK_LIST_ADDRESS;
	}
	else
	{
		page = BASE_BLACK_LIST_END;
	}
    memset(&BBLLink.CardNumber[0],0xff,(BASEBLACK_LINKMAX + 1) * 10);
    BBLLink.crc = 0;
    if ( cr_protected.BaseBlackListIntegrality != 0 )
    {
        return 5;
    }
    for ( i = BASE_BLACK_LIST_ADDRESS; i < page; i++ )
    {
        WatchDog();
        adr = i * AT45_PageSize(pAt45);
        if ( READ(CONFIG_ID,buf,adr,482) != 482 )
        {
            return 4;
        }
        crc = Crc16(buf,480,0);
        memcpy((unsigned char *)(&crctmp),&buf[480],2);
        if ( crc != crctmp )
        {
            return 5;
        }
        memcpy(&BBLLink.CardNumber[i - BASE_BLACK_LIST_ADDRESS],buf,10);
        //ListCrc = Crc16(buf,10,ListCrc);
    }
    //buf[479] += 1;
	memset(&buf[470],0xff,10);
    memcpy(&BBLLink.CardNumber[i - BASE_BLACK_LIST_ADDRESS],&buf[470],10);
    //ListCrc = Crc16(BBLLink.CardNumber[i - BASE_BLACK_LIST_ADDRESS],10,ListCrc);
	/*for(i = BASE_BLACK_LIST_ADDRESS; i <= BASE_BLACK_LIST_END; i++)
	{
		ListCrc = Crc16(BBLLink.CardNumber[i],10,ListCrc);
	}*/
	ListCrc = crc = Crc16(&BBLLink.CardNumber[0][0], 10 * (BASEBLACK_LINKMAX + 1), 0);
    BBLLink.crc = ListCrc;
    return 0;
}

unsigned char BuildAddBlackListLink(void)
{
    unsigned char buf[482];
    unsigned short crc,crctmp;
    unsigned short ListCrc;
    unsigned int i,page,adr;
    At45 *pAt45;
    
    pAt45 = &at45[CONFIG_ID];
    ListCrc = 0;
	page = (cr_protected.AddBlackListLength - 16) / 480;
	if((cr_protected.AddBlackListLength - 16) % 480)
	{
		page++;
	}
	if(page < (ADD_BLACK_LIST_END - ADD_BLACK_LIST_ADDRESS))
	{
		page += ADD_BLACK_LIST_ADDRESS;
	}
	else
	{
		page = ADD_BLACK_LIST_END;
	}
    memset(&ABLLink.CardNumber[0],0xff,(ADDBLACK_LINKMAX + 1) * 10);
    ABLLink.crc = 0;
    if ( cr_protected.AddBlackListIntegrality != 0 )
    {
        return 5;
    }
    for ( i = ADD_BLACK_LIST_ADDRESS; i < page; i++ )
    {
        adr = i * AT45_PageSize(pAt45);
        if ( READ(CONFIG_ID,buf,adr,482) != 482 )
        {
            return 4;
        }
        crc = Crc16(buf,480,0);
        memcpy((unsigned char *)(&crctmp),&buf[480],2);
        if ( crc != crctmp )
        {
            return 5;
        }
        memcpy(&ABLLink.CardNumber[i - ADD_BLACK_LIST_ADDRESS],buf,10);
        //ListCrc = Crc16(buf,10,ListCrc);
    }
    //buf[479] += 1;
	memset(&buf[470],0xff,10);
    memcpy(&ABLLink.CardNumber[i - ADD_BLACK_LIST_ADDRESS],&buf[470],10);
    //ListCrc = Crc16(ABLLink.CardNumber[i - ADD_BLACK_LIST_ADDRESS],10,ListCrc);
	/*for(i = ADD_BLACK_LIST_ADDRESS; i <= ADD_BLACK_LIST_END; i++)
	{
		ListCrc = Crc16(ABLLink.CardNumber[i],10,ListCrc);
	}*/
	ListCrc = crc = Crc16(&ABLLink.CardNumber[0][0], 10 * (ADDBLACK_LINKMAX + 1), 0);
    ABLLink.crc = ListCrc;
    return 0;
}

unsigned char BuildDelBlackListLink(void)
{
    unsigned char buf[482];
    unsigned short crc,crctmp;
    unsigned short ListCrc;
    unsigned int i,page,adr;
    At45 *pAt45;
    
    pAt45 = &at45[CONFIG_ID];
    ListCrc = 0;
	page = (cr_protected.DelBlackListLength - 16) / 480;
	if((cr_protected.DelBlackListLength - 16) % 480)
	{
		page++;
	}
	if(page < (DEL_BLACK_LIST_END - DEL_BLACK_LIST_ADDRESS))
	{
		page += DEL_BLACK_LIST_ADDRESS;
	}
	else
	{
		page = DEL_BLACK_LIST_END;
	}
    memset(DBLLink.CardNumber[0],0xff,(DELBLACK_LINKMAX + 1) * 10);
    DBLLink.crc = 0;
    if ( cr_protected.DelBlackListIntegrality != 0 )
    {
        return 5;
    }
    for ( i = DEL_BLACK_LIST_ADDRESS; i < page; i++ )
    {
        adr = i * AT45_PageSize(pAt45);
        if ( READ(CONFIG_ID,buf,adr,482) != 482 )
        {
            return 4;
        }
        crc = Crc16(buf,480,0);
        memcpy((unsigned char *)(&crctmp),&buf[480],2);
        if ( crc != crctmp )
        {
            return 5;
        }
        memcpy(DBLLink.CardNumber[i - DEL_BLACK_LIST_ADDRESS],buf,10);
        //ListCrc = Crc16(buf,10,ListCrc);
    }
    //buf[479] += 1;
	memset(&buf[470],0xff,10);
    memcpy(DBLLink.CardNumber[i - DEL_BLACK_LIST_ADDRESS],&buf[470],10);
    //ListCrc = Crc16(DBLLink.CardNumber[i - DEL_BLACK_LIST_ADDRESS],10,ListCrc);
	/*for(i = DEL_BLACK_LIST_ADDRESS; i <= DEL_BLACK_LIST_END; i++)
	{
		ListCrc = Crc16(DBLLink.CardNumber[i],10,ListCrc);
	}*/
	ListCrc = crc = Crc16(&DBLLink.CardNumber[0][0], 10 * (DELBLACK_LINKMAX + 1), 0);
    DBLLink.crc = ListCrc;
    return 0;
}

unsigned char BuildWhiteListLink(void)
{
    unsigned char buf[482];
    unsigned short crc,crctmp;
    unsigned short ListCrc;
    unsigned int i,page,adr;
    At45 *pAt45;
    
    pAt45 = &at45[CONFIG_ID];
    ListCrc = 0;
	page = (cr_protected.WhiteListLength - 16) / 480;
	if((cr_protected.WhiteListLength - 16) %480)
	{
		page++;
	}
	if(page < (WHITE_LIST_END - WHITE_LIST_ADDRESS))
	{
		page += WHITE_LIST_ADDRESS;
	}
	else
	{
		page = WHITE_LIST_END;
	}
    memset(WLLink.CardNumber[0],0xff,(WHITE_LINKMAX + 1) * 10);
    WLLink.crc = 0;
    if ( cr_protected.WhiteListIntegrality != 0 )
    {
        return 5;
    }
    for ( i = WHITE_LIST_ADDRESS; i < page ; i++ )
    {
        adr = i * AT45_PageSize(pAt45);
        if ( READ(CONFIG_ID,buf,adr,482) != 482 )
        {
            return 4;
        }
        crc = Crc16(buf,480,0);
        memcpy((unsigned char *)(&crctmp),&buf[480],2);
        if ( crc != crctmp )
        {
            return 5;
        }
        memcpy(WLLink.CardNumber[i - WHITE_LIST_ADDRESS],buf,10);
        //ListCrc = Crc16(buf,10,ListCrc);
    }
    //buf[479] += 1;
	memset(&buf[470],0xff,10);
        memcpy(WLLink.CardNumber[i - WHITE_LIST_ADDRESS],&buf[470],10);
    //ListCrc = Crc16(WLLink.CardNumber[i - WHITE_LIST_ADDRESS],10,ListCrc);
	/*for(i = WHITE_LIST_ADDRESS; i <= WHITE_LIST_END; i++)
	{
		ListCrc = Crc16(WLLink.CardNumber[i],10,ListCrc);
	}*/
	ListCrc = crc = Crc16(&WLLink.CardNumber[0][0], 10 * (WHITE_LINKMAX + 1), 0);
    WLLink.crc = ListCrc;
    return 0;
}

unsigned char DestroyListIntegrality(unsigned char typ)
{
    unsigned int adr;
    unsigned char j,flg;
    At45 *pAt45;
	unsigned char buf[8];
	unsigned short crc;
    
    pAt45 = &at45[CONFIG_ID];
    switch ( typ )
    {
    case BASEBLACKLIST :
		SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
		READ(CONFIG_ID,buf,adr,8);
		buf[0] = 0xff;
		crc = Crc16(buf,6,0);
		memcpy(&buf[6],(unsigned char *)(&crc),2);
		WRITE(CONFIG_ID,buf,adr,8);
        cr_protected.BaseBlackListIntegrality = 0xffff;
		CalculateCrc();
        ListFlg.BaseBlackListPageAddr = BASE_BLACK_LIST_ADDRESS;
        ListFlg.BaseBlackListCount = 0;
        memset(&BBLLink.CardNumber[0],0xff,(BASEBLACK_LINKMAX + 1) * 10);
    	BBLLink.crc = 0;
        break;
    case ADDBLACKLIST :
		SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
		READ(CONFIG_ID,buf,adr,8);
		buf[1] = 0xff;
		crc = Crc16(buf,6,0);
		memcpy(&buf[6],(unsigned char *)(&crc),2);
		WRITE(CONFIG_ID,buf,adr,8);
        cr_protected.AddBlackListIntegrality = 0xffff;
		CalculateCrc();
        ListFlg.AddBlackListPageAddr = ADD_BLACK_LIST_ADDRESS;
        ListFlg.AddBlackListCount = 0;
        /*for ( i = ADD_BLACK_LIST_ADDRESS; i < ADD_BLACK_LIST_END; i++ )
        {
            adr = i * AT45_PageSize(pAt45);
            for ( j = 0; j < 3; j++ )
            {
                SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
                flg = AT45_ErasePage(pAt45,adr);
                if ( flg == 0 )
                {
                    break;
                }
            }
            if ( j == 3 )
            {
                cr_protected.AddBlackListIntegrality = 1;
                break;
            }
        }*/
        break;
    case DELBLACKLIST :
		SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
		READ(CONFIG_ID,buf,adr,8);
		buf[2] = 0xff;
		crc = Crc16(buf,6,0);
		memcpy(&buf[6],(unsigned char *)(&crc),2);
		WRITE(CONFIG_ID,buf,adr,8);
        cr_protected.DelBlackListIntegrality = 0xffff;
		CalculateCrc();
        ListFlg.DelBlackListPageAddr = DEL_BLACK_LIST_ADDRESS;
        ListFlg.DelBlackListCount = 0;
        /*for ( i = DEL_BLACK_LIST_ADDRESS; i < DEL_BLACK_LIST_END; i++ )
        {
            adr = i * AT45_PageSize(pAt45);
            for ( j = 0; j < 3; j++ )
            {
                SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
                flg = AT45_ErasePage(pAt45,adr);
                if ( flg == 0 )
                {
                    break;
                }
            }
            if ( j == 3 )
            {
                cr_protected.DelBlackListIntegrality = 1;
                break;
            }
        }*/
        break;
    case WHITELIST :
		SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
		READ(CONFIG_ID,buf,adr,8);
		buf[3] = 0xff;
		crc = Crc16(buf,6,0);
		memcpy(&buf[6],(unsigned char *)(&crc),2);
		WRITE(CONFIG_ID,buf,adr,8);
        cr_protected.WhiteListIntegrality = 0xffff;
		CalculateCrc();
        ListFlg.WhiteListPageAddr = WHITE_LIST_ADDRESS;
        ListFlg.WhiteListCount = 0;
        /*for ( i = WHITE_LIST_ADDRESS; i < WHITE_LIST_END; i++ )
        {
            adr = i * AT45_PageSize(pAt45);
            for ( j = 0; j < 3; j++ )
            {
                SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
                flg = AT45_ErasePage(pAt45,adr);
                if ( flg == 0 )
                {
                    break;
                }
            }
        }
        if ( j == 3 )
        {
            cr_protected.WhiteListIntegrality = 1;
            break;
        }*/
        break;
    case GRADEPRICE :
		SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
		READ(CONFIG_ID,buf,adr,8);
		buf[4] = 0xff;
		crc = Crc16(buf,6,0);
		memcpy(&buf[6],(unsigned char *)(&crc),2);
		WRITE(CONFIG_ID,buf,adr,8);
        cr_protected.GradePriceIntegrality = 0xffff;
		CalculateCrc();
        ListFlg.GradePricePageAddr = GRADE_PRICE_ADDRESS;
        ListFlg.GradePriceCount = 0;
        adr = GRADE_PRICE_ADDRESS * AT45_PageSize(pAt45);
        for ( j = 0; j < 3; j++ )
        {
            SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
            flg = AT45_ErasePage(pAt45,adr);
            if ( flg == 0 )
            {
                break;
            }
        }
        if ( j == 3 )
        {
            cr_protected.GradePriceIntegrality = 1;
			CalculateCrc();
        }
        break;
    case OILSTOP :
		SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
		adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
		READ(CONFIG_ID,buf,adr,8);
		buf[5] = 0xff;
		crc = Crc16(buf,6,0);
		memcpy(&buf[6],(unsigned char *)(&crc),2);
		WRITE(CONFIG_ID,buf,adr,8);
        cr_protected.OilStopIntegrality = 0xffff;
		CalculateCrc();
        ListFlg.OilStopPageAddr = OIL_STOP_ADDRESS;
        ListFlg.OilStopCount = 0;
        adr = OIL_STOP_ADDRESS * AT45_PageSize(pAt45);
        for ( j = 0; j < 3; j++ )
        {
            SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
            flg = AT45_ErasePage(pAt45,adr);
            if ( flg == 0 )
            {
                break;
            }
        }
        if ( j == 3 )
        {
            cr_protected.OilStopIntegrality = 1;
			CalculateCrc();
        }
        break;
    default:
        flg = 2;
        break;
    }
    return flg;
}

unsigned char WriteListHead(unsigned char typ,unsigned char *s,unsigned char len)
{
    unsigned short crc;
    unsigned int adr;
    unsigned char buf[26];
	//unsigned int length;
	unsigned int number;
    At45 *pAt45;
    
	if(s == (unsigned char*)0)
	{
		return -1;
	}
    pAt45 = &at45[CONFIG_ID];
	memcpy((unsigned char *)(&number),&s[12],4);
	number = IntEndianConvert(number);
    switch ( typ )
    {
    case BASEBLACKLIST :
        if ( len != 16 )
        {
            return 3;
        }
		if(number > 20000)
		{
			number = 0;
			ListLimit = 1;
			number = IntEndianConvert(number);
			memcpy(&buf[12],(unsigned char *)(&number),4);
		}
        memcpy(buf,s,16);
        adr = BASE_BLACK_LIST_ADDRESS;
        memcpy(&buf[16],(unsigned char *)(&adr),4);
        adr = BASE_BLACK_LIST_END;
        memcpy(&buf[20],(unsigned char *)(&adr),4);
        crc = Crc16(buf,24,0);
        memcpy(&buf[24],(unsigned char *)(&crc),2);
        adr = LIST_HEAD_ADDRESS * AT45_PageSize(pAt45);
        memcpy((unsigned char *)(&BaseBlackListHead),buf,26);
        if ( WRITE(CONFIG_ID,buf,adr, 26) != 26 )
        {
            return 1;
        }
		/*memcpy((unsigned char *)(&length),&s[13],4);
		length = IntEndianConvert(length);
		if(WriteListLength(BASEBLACKLIST,length))
		{
			return 1;
		} */
        break;
    case ADDBLACKLIST :
        if ( len != 16 )
        {
            return 3;
        }
		if(number > 200)
		{
			number = 0;
			ListLimit = 1;
			number = IntEndianConvert(number);
			memcpy(&buf[12],(unsigned char *)(&number),4);
		}
        memcpy(buf,s,16);
        adr = ADD_BLACK_LIST_ADDRESS;
        memcpy(&buf[16],(unsigned char *)(&adr),4);
        adr = ADD_BLACK_LIST_END;
        memcpy(&buf[20],(unsigned char *)(&adr),4);
        crc = Crc16(buf,24,0);
        memcpy(&buf[24],(unsigned char *)(&crc),2);
        adr = LIST_HEAD_ADDRESS * AT45_PageSize(pAt45) + sizeof(ListHead);
        memcpy((unsigned char *)(&AddBlackListHead),buf,26);
        if ( WRITE(CONFIG_ID,buf,adr,26) != 26 )
        {
            return 1;
        }
		/*memcpy((unsigned char *)(&length),&s[13],4);
		length = IntEndianConvert(length);
		if(WriteListLength(ADDBLACKLIST,length))
		{
			return 1;
		}*/
        break;
    case DELBLACKLIST :
        if ( len != 16 )
        {
            return 3;
        }
		if(number > 200)
		{
			number = 0;
			ListLimit = 1;
			number = IntEndianConvert(number);
			memcpy(&buf[12],(unsigned char *)(&number),4);
		}
        memcpy(buf,s,16);
        adr = DEL_BLACK_LIST_ADDRESS;
        memcpy(&buf[16],(unsigned char *)(&adr),4);
        adr = DEL_BLACK_LIST_END;
        memcpy(&buf[20],(unsigned char *)(&adr),4);
        crc = Crc16(buf,24,0);
        memcpy(&buf[24],(unsigned char *)(&crc),2);
        adr = LIST_HEAD_ADDRESS * AT45_PageSize(pAt45) + 2 * sizeof(ListHead);
        memcpy((unsigned char *)(&DelBlackListHead),buf,26);
        if ( WRITE(CONFIG_ID,buf,adr,26) != 26 )
        {
            return 1;
        }
		/*memcpy((unsigned char *)(&length),&s[13],4);
		length = IntEndianConvert(length);
		if(WriteListLength(DELBLACKLIST,length))
		{
			return 1;
		}*/
        break;
    case WHITELIST :
        if ( len != 16 )
        {
            return 3;
        }
		if(number > 200)
		{
			number = 0;
			ListLimit = 1;
			number = IntEndianConvert(number);
			memcpy(&buf[12],(unsigned char *)(&number),4);
		}
        memcpy(buf,s,16);
        adr = WHITE_LIST_ADDRESS;
        memcpy(&buf[16],(unsigned char *)(&adr),4);
        adr = WHITE_LIST_END;
        memcpy(&buf[20],(unsigned char *)(&adr),4);
        crc = Crc16(buf,24,0);
        memcpy(&buf[24],(unsigned char *)(&crc),2);
        adr = LIST_HEAD_ADDRESS * AT45_PageSize(pAt45) + 3 * sizeof(ListHead);
        memcpy((unsigned char *)(&WhiteListHead),buf,26);
        if ( WRITE(CONFIG_ID,buf,adr,26) != 26 )
        {
            return 1;
        }
		/*&memcpy((unsigned char *)(&length),&s[13],4);
		length = IntEndianConvert(length);
		if(WriteListLength(WHITELIST,length))
		{
			return 1;
		}*/
        break;
    default:
        return 2;
    }
    return 0;
}

unsigned char LoadListHead(unsigned char typ)
{
    unsigned short crc,crctmp,len;
    unsigned int adr;
    unsigned char buf[26 << 2];
    unsigned char error = 0;
    At45 *pAt45;
    
    pAt45 = &at45[CONFIG_ID];
    adr = LIST_HEAD_ADDRESS * AT45_PageSize(pAt45);
    if ( READ(CONFIG_ID,buf,adr,26 << 2) != (26 << 2) )
    {
        return 1;
    }
    switch ( typ )
    {
    case BASEBLACKLIST :
        len = sizeof(ListHead);
        crc = Crc16(buf,len - 2,0);
        memcpy((unsigned char *)(&crctmp),&buf[len - 2],2);
        if ( crc != crctmp )
        {
            return 2;
        }
        memcpy((unsigned char *)(&BaseBlackListHead),buf,len);
		memcpy((unsigned char *)(&cr_protected.BaseBlackListLength),BaseBlackListHead.ListAmount,4);
		cr_protected.BaseBlackListLength = IntEndianConvert(cr_protected.BaseBlackListLength);
		cr_protected.BaseBlackListLength *= 10;
		cr_protected.BaseBlackListLength += 16;
		CalculateCrc();
        break;
    case ADDBLACKLIST :
        len = sizeof(ListHead);
        crc = Crc16(&buf[len],len - 2,0);
        memcpy((unsigned char *)(&crctmp),&buf[len * 2  - 2],2);
        if ( crc != crctmp )
        {
            return 2;
        }
        memcpy((unsigned char *)(&AddBlackListHead),&buf[len],len);
		memcpy((unsigned char *)(&cr_protected.AddBlackListLength),AddBlackListHead.ListAmount,4);
		cr_protected.AddBlackListLength = IntEndianConvert(cr_protected.AddBlackListLength);
		cr_protected.AddBlackListLength *= 10;
		cr_protected.AddBlackListLength += 16;
		CalculateCrc();
        break;
    case DELBLACKLIST :
        len = sizeof(ListHead);
        crc = Crc16(&buf[len * 2],len - 2,0);
        memcpy((unsigned char *)(&crctmp),&buf[len * 3 - 2],2);
        if ( crc != crctmp )
        {
            return 2;
        }
        memcpy((unsigned char *)(&DelBlackListHead),&buf[len * 2],len);
		memcpy((unsigned char *)(&cr_protected.DelBlackListLength),DelBlackListHead.ListAmount,4);
		cr_protected.DelBlackListLength = IntEndianConvert(cr_protected.DelBlackListLength);
		cr_protected.DelBlackListLength *= 10;
		cr_protected.DelBlackListLength += 16;
		CalculateCrc();
        break;
    case WHITELIST :
        len = sizeof(ListHead);
        crc = Crc16(&buf[len * 3],len - 2,0);
        memcpy((unsigned char *)(&crctmp),&buf[len * 4 - 2],2);
        if ( crc != crctmp )
        {
            return 5;
        }
        memcpy((unsigned char *)(&WhiteListHead),&buf[len * 3],len);
		memcpy((unsigned char *)(&cr_protected.WhiteListLength),WhiteListHead.ListAmount,4);
		cr_protected.WhiteListLength = IntEndianConvert(cr_protected.WhiteListLength);
		cr_protected.WhiteListLength *= 10;
		cr_protected.WhiteListLength += 16;
		CalculateCrc();
        break;
    default:
        len = sizeof(ListHead);
        crc = Crc16(buf,len - 2,0);
        memcpy((unsigned char *)(&crctmp),&buf[len - 2],2);
        if ( crc != crctmp )
        {
            error = 2;
        }
        memcpy((unsigned char *)(&BaseBlackListHead),buf,len);
		memcpy((unsigned char *)(&cr_protected.BaseBlackListLength),BaseBlackListHead.ListAmount,4);
		cr_protected.BaseBlackListLength = IntEndianConvert(cr_protected.BaseBlackListLength);
		cr_protected.BaseBlackListLength *= 10;
		cr_protected.BaseBlackListLength += 16;
        crc = Crc16(&buf[len],len - 2, 0);
        memcpy((unsigned char *)(&crctmp),&buf[len * 2 -2],2);
        if ( crc != crctmp )
        {
            error = 2;
        }
        memcpy((unsigned char *)(&AddBlackListHead),&buf[len],len);
		memcpy((unsigned char *)(&cr_protected.AddBlackListLength),AddBlackListHead.ListAmount,4);
		cr_protected.AddBlackListLength = IntEndianConvert(cr_protected.AddBlackListLength);
		cr_protected.AddBlackListLength *= 10;
		cr_protected.AddBlackListLength += 16;
        crc = Crc16(&buf[len * 2],len - 2,0);
        memcpy((unsigned char *)(&crctmp),&buf[len * 3 - 2],2);
        if ( crc != crctmp )
        {
            error = 2;
        }
        memcpy((unsigned char *)(&DelBlackListHead),&buf[len * 2],len);
		memcpy((unsigned char *)(&cr_protected.DelBlackListLength),DelBlackListHead.ListAmount,4);
		cr_protected.DelBlackListLength = IntEndianConvert(cr_protected.DelBlackListLength);
		cr_protected.DelBlackListLength *= 10;
		cr_protected.DelBlackListLength  += 16;
        crc = Crc16(&buf[len * 3],len - 2,0);
        memcpy((unsigned char *)(&crctmp),&buf[len * 4 - 2],2);
        if ( crc != crctmp )
        {
            error = 2;
        }
		memcpy((unsigned char *)(&WhiteListHead),&buf[len * 3],len);
		memcpy((unsigned char *)(&cr_protected.WhiteListLength),WhiteListHead.ListAmount,4);
		cr_protected.WhiteListLength = IntEndianConvert(cr_protected.WhiteListLength);
		cr_protected.WhiteListLength *= 10;
		cr_protected.WhiteListLength += 16;
		CalculateCrc();
        break;
    }
    return error;
}

static unsigned char LoadPricelListFlg = 0;

unsigned char LoadGradePriceList(void)
{
	unsigned char buf[482];
    unsigned int adr;
    unsigned short crc,crctmp;
    unsigned char i,j;
    unsigned short tmp;
    unsigned int den;
    At45 *pAt45;
	GDP	gdp;
	GDP	nextgdp;
	GradePriceList GradeTmp;
	
	errno = 0;
	memset((unsigned char *)(&GradeTmp),0,sizeof(GradeTmp));
	pAt45 = &at45[CONFIG_ID];
    adr = LOCAL_PRICE_ADDRESS * AT45_PageSize(pAt45);
    if ( READ(CONFIG_ID,buf,adr,482) != 482 )
    {
        return 1;
    }
    crc = Crc16(buf,480,0);
    memcpy((unsigned char *)(&crctmp),&buf[480],2);
    if ( crc != crctmp )
    {
        return 2;
    }
	memcpy(&GradeTmp.Ver,buf,8);
	adr = 8;
	if((GradeTmp.FieldNum > 8) || (GradeTmp.FieldNum == 0))
	{
		errno = PRICE_ERROR_FLAGS;
		DestroyListIntegrality(GRADEPRICE);
		return -1;
	}
    for ( i = 0; i < GradeTmp.FieldNum; i++ )
    {
        GradeTmp.gdp[i].nzn = buf[adr++];
        memcpy((unsigned char *)(&tmp),&buf[adr],2);
        adr += 2;
        GradeTmp.gdp[i].o_type = ShortEndianConvert(tmp);
        memcpy((unsigned char *)(&den),&buf[adr],4);
        adr += 4;
        GradeTmp.gdp[i].den = IntEndianConvert(den);
        GradeTmp.gdp[i].prc_n = buf[adr++];
		
		if((GradeTmp.gdp[i].prc_n > 8) || (GradeTmp.gdp[i].prc_n == 0))
		{
			errno = PRICE_ERROR_FLAGS;
			DestroyListIntegrality(GRADEPRICE);
			return -1;
		}
        for ( j = 0; j < GradeTmp.gdp[i].prc_n; j++ )
        {
            memcpy((unsigned char *)(&tmp),&buf[adr],2);
            adr += 2;
            GradeTmp.gdp[i].prc[j] = ShortEndianConvert(tmp);
        }
    }
    for ( i = 0; i < GradeTmp.FieldNum; i++ )
    {
        GradeTmp.next_gdp[i].nzn = buf[adr++];
        memcpy((unsigned char *)(&tmp),&buf[adr],2);
        adr += 2;
        GradeTmp.next_gdp[i].o_type = ShortEndianConvert(tmp);
        memcpy((unsigned int *)(&den),&buf[adr],4);
        adr += 4;
        GradeTmp.next_gdp[i].den = IntEndianConvert(den);
        GradeTmp.next_gdp[i].prc_n = buf[adr++];
		
		if((GradeTmp.next_gdp[i].prc_n > 8) || (GradeTmp.next_gdp[i].prc_n == 0))
		{
			errno = PRICE_ERROR_FLAGS;
			DestroyListIntegrality(GRADEPRICE);
			return -1;
		}
        for ( j = 0; j < GradeTmp.next_gdp[i].prc_n; j++ )
        {	
            memcpy((unsigned char *)(&tmp),&buf[adr],2);
            adr += 2;
            GradeTmp.next_gdp[i].prc[j] = ShortEndianConvert(tmp);
		}
    }
	for(i = 0; i < GradeTmp.FieldNum - 1; i++)
	{
		for(j = i + 1; j < GradeTmp.FieldNum; j++)
		{
			if(GradeTmp.next_gdp[i].nzn > GradeTmp.next_gdp[j].nzn)
			{
				memcpy((unsigned char *)(&gdp),(unsigned char *)(&GradeTmp.gdp[i]),sizeof(GDP));
				memcpy((unsigned char *)(&nextgdp),(unsigned char *)(&GradeTmp.next_gdp[i]),sizeof(GDP));
				memcpy((unsigned char *)(&GradeTmp.gdp[i]),(unsigned char *)(&GradeTmp.gdp[j]),sizeof(GDP));
				memcpy((unsigned char *)(&GradeTmp.next_gdp[i]),(unsigned char *)(&GradeTmp.next_gdp[j]),sizeof(GDP));
				memcpy((unsigned char *)(&GradeTmp.gdp[j]),(unsigned char *)(&gdp),sizeof(GDP));
				memcpy((unsigned char *)(&GradeTmp.next_gdp[j]),(unsigned char *)(&nextgdp),sizeof(GDP));
			}
		}
	}
	if(!LoadPricelListFlg)
	{
		LoadPricelListFlg = 1;
		memcpy((unsigned char *)(&GradePrice),(unsigned char *)(&GradeTmp),sizeof(GradePrice)); 
		return 3;
	}
	else if(memcmp((unsigned char *)(&GradePrice.gdp[0]),(unsigned char *)(&GradeTmp.gdp[0]),2 * 8 * sizeof(GDP)))
	{
		memcpy((unsigned char *)(&GradePrice),(unsigned char *)(&GradeTmp),sizeof(GradePrice));
		for(i = 0; i < GradePrice.FieldNum; ++i)
		{
			for(j = 0; j < GradePrice.gdp[i].prc_n; ++j)
			{
				if(GradePrice.gdp[i].prc[j] != GradePrice.next_gdp[i].prc[j])
				{
					return 3;
				}
			}
		}
		return 0;
	}
	else
	{
		memcpy((unsigned char *)(&GradePrice),(unsigned char *)(&GradeTmp),sizeof(GradePrice));
	}
    return 0;
}

unsigned char CombinationPriceList(void)
{
	unsigned char buf[482];
    unsigned int adr;
    unsigned short crc,crctmp;
    unsigned char i,j;
    unsigned short tmp;
    unsigned int den;
    At45 *pAt45;
	unsigned char buffer[482];
	GradePriceList LocalGradeTmp,GradeTmp;
	unsigned char flg;
	
	flg = 0;
	pAt45 = &at45[CONFIG_ID];
 	adr = GRADE_PRICE_ADDRESS * AT45_PageSize(pAt45);
	if ( READ(CONFIG_ID,buffer,adr,482) != 482 )
    {
        return 1;
    }
	crc = Crc16(buffer,480,0);
    memcpy((unsigned char *)(&crctmp),&buffer[480],2);
    if ( crc != crctmp )
    {
        return 2;
    }
	adr = LOCAL_PRICE_ADDRESS * AT45_PageSize(pAt45);
	if ( READ(CONFIG_ID,buf,adr,482) != 482 )
    {
        return 1;
    }
    crc = Crc16(buf,480,0);
    memcpy((unsigned char *)(&crctmp),&buf[480],2);
    if ( crc != crctmp )
    {
       flg = 1;
    }
	if((buffer[7] != buf[7]) || (flg))
	{
		adr = LOCAL_PRICE_ADDRESS * AT45_PageSize(pAt45);
		if(WRITE(CONFIG_ID,buffer,adr,482) != 482)
		{
			return 1;
		}
		return 0;
	}
	adr = 8;
	LocalGradeTmp.FieldNum = buf[7];
	if((LocalGradeTmp.FieldNum > 8) || (LocalGradeTmp.FieldNum == 0))
	{
		DestroyListIntegrality(GRADEPRICE);
		return -1;
	}
	for ( i = 0; i < LocalGradeTmp.FieldNum; i++ )
    {
        LocalGradeTmp.gdp[i].nzn = buf[adr++];
        memcpy((unsigned char *)(&tmp),&buf[adr],2);
        adr += 2;
        LocalGradeTmp.gdp[i].o_type = ShortEndianConvert(tmp);
        memcpy((unsigned char *)(&den),&buf[adr],4);
        adr += 4;
        LocalGradeTmp.gdp[i].den = IntEndianConvert(den);
        LocalGradeTmp.gdp[i].prc_n = buf[adr++];
		if((LocalGradeTmp.gdp[i].prc_n > 8) || (LocalGradeTmp.gdp[i].prc_n == 0))
		{
			DestroyListIntegrality(GRADEPRICE);
			return -1;
		}
        for ( j = 0; j < LocalGradeTmp.gdp[i].prc_n; j++ )
        {
            memcpy((unsigned char *)(&tmp),&buf[adr],2);
            adr += 2;
            LocalGradeTmp.gdp[i].prc[j] = ShortEndianConvert(tmp);
        }
    }
	for ( i = 0; i < LocalGradeTmp.FieldNum; i++ )
    {
        LocalGradeTmp.next_gdp[i].nzn = buf[adr++];
        memcpy((unsigned char *)(&tmp),&buf[adr],2);
        adr += 2;
        LocalGradeTmp.next_gdp[i].o_type = ShortEndianConvert(tmp);
        memcpy((unsigned int *)(&den),&buf[adr],4);
        adr += 4;
        LocalGradeTmp.next_gdp[i].den = IntEndianConvert(den);
        LocalGradeTmp.next_gdp[i].prc_n = buf[adr++];
		if((LocalGradeTmp.next_gdp[i].prc_n > 8) || (LocalGradeTmp.next_gdp[i].prc_n == 0))
		{
			DestroyListIntegrality(GRADEPRICE);
			return -1;
		}
        for ( j = 0; j < LocalGradeTmp.next_gdp[i].prc_n; j++ )
        {
            memcpy((unsigned char *)(&tmp),&buf[adr],2);
            adr += 2;
            LocalGradeTmp.next_gdp[i].prc[j] = ShortEndianConvert(tmp);
       
		}
    }
	adr = 8;
	GradeTmp.FieldNum = buffer[7];
	if((GradeTmp.FieldNum > 8) || (GradeTmp.FieldNum == 0))
	{
		DestroyListIntegrality(GRADEPRICE);
		return -1;
	}
	for ( i = 0; i < GradeTmp.FieldNum; i++ )
    {
        GradeTmp.gdp[i].nzn = buffer[adr++];
        memcpy((unsigned char *)(&tmp),&buffer[adr],2);
        adr += 2;
        GradeTmp.gdp[i].o_type = ShortEndianConvert(tmp);
        memcpy((unsigned char *)(&den),&buffer[adr],4);
        adr += 4;
        GradeTmp.gdp[i].den = IntEndianConvert(den);
        GradeTmp.gdp[i].prc_n = buffer[adr++];
		if((GradeTmp.gdp[i].prc_n > 8) || (GradeTmp.gdp[i].prc_n == 0))
		{
			DestroyListIntegrality(GRADEPRICE);
			return -1;
		}
        for ( j = 0; j < GradeTmp.gdp[i].prc_n; j++ )
        {
            memcpy((unsigned char *)(&tmp),&buffer[adr],2);
            adr += 2;
            GradeTmp.gdp[i].prc[j] = ShortEndianConvert(tmp);
        }
    }
    for ( i = 0; i < GradeTmp.FieldNum; i++ )
    {
        GradeTmp.next_gdp[i].nzn = buffer[adr++];
        memcpy((unsigned char *)(&tmp),&buffer[adr],2);
        adr += 2;
        GradeTmp.next_gdp[i].o_type = ShortEndianConvert(tmp);
        memcpy((unsigned int *)(&den),&buffer[adr],4);
        adr += 4;
        GradeTmp.next_gdp[i].den = IntEndianConvert(den);
        GradeTmp.next_gdp[i].prc_n = buffer[adr++];
		if((GradeTmp.next_gdp[i].prc_n > 8) || (GradeTmp.next_gdp[i].prc_n == 0))
		{
			DestroyListIntegrality(GRADEPRICE);
			return -1;
		}
        for ( j = 0; j < GradeTmp.next_gdp[i].prc_n; j++ )
        {
            memcpy((unsigned char *)(&tmp),&buffer[adr],2);
            adr += 2;
            GradeTmp.next_gdp[i].prc[j] = ShortEndianConvert(tmp);
       
		}
    }
	for(i = 0; i < GradeTmp.FieldNum; i++)
	{
		adr = LOCAL_PRICE_ADDRESS * AT45_PageSize(pAt45);
		if(LocalGradeTmp.gdp[i].prc_n != GradeTmp.gdp[i].prc_n)
		{
			break;
		}
	}
	if(i != GradeTmp.FieldNum)
	{
		adr = LOCAL_PRICE_ADDRESS * AT45_PageSize(pAt45);
		if(WRITE(CONFIG_ID,buffer,adr,482) != 482)
		{
			return 1;
		}
		return 0;
	}
	for(i = 0; i < GradeTmp.FieldNum; i++)
	{
		if(LocalGradeTmp.next_gdp[i].prc_n != GradeTmp.next_gdp[i].prc_n)
		{
			break;
		}
	}
	if(i != GradeTmp.FieldNum)
	{
		if(WRITE(CONFIG_ID,buffer,adr,482) != 482)
		{
			return 1;
		}
		return 0;
	}
	memcpy(buf,buffer,8);
	adr = 8;
	if(cr_protected.PriceUpdateFlg > 9)
	{
		for ( i = 0; i < GradeTmp.FieldNum; i++ )
		{
			buf[adr++] = LocalGradeTmp.next_gdp[i].nzn;
			tmp = ShortEndianConvert(LocalGradeTmp.next_gdp[i].o_type);
			memcpy(&buf[adr],(unsigned char *)(&tmp),2);
			adr += 2;
			den = IntEndianConvert(LocalGradeTmp.next_gdp[i].den);
			memcpy(&buf[adr],(unsigned char *)(&den),4);
			adr += 4;
			buf[adr++] = LocalGradeTmp.next_gdp[i].prc_n;
			for(j = 0; j < LocalGradeTmp.next_gdp[i].prc_n; j++)
			{
				tmp = ShortEndianConvert(LocalGradeTmp.next_gdp[i].prc[j]);
				memcpy(&buf[adr],(unsigned char *)(&tmp),2);
				adr += 2;
			}
		}
	}
	else
	{
		for( i = 0; i < GradeTmp.FieldNum; i++)
		{
			buf[adr++] = LocalGradeTmp.gdp[i].nzn;
			tmp = ShortEndianConvert(LocalGradeTmp.gdp[i].o_type);
			memcpy(&buf[adr],(unsigned char *)(&tmp),2);
			adr += 2;
			den = IntEndianConvert(LocalGradeTmp.gdp[i].den);
			memcpy(&buf[adr],(unsigned char *)(&den),4);
			adr += 4;
			buf[adr++] = LocalGradeTmp.gdp[i].prc_n;
			for(j = 0; j < LocalGradeTmp.gdp[i].prc_n; j++)
			{
				tmp = ShortEndianConvert(LocalGradeTmp.gdp[i].prc[j]);
				memcpy(&buf[adr],(unsigned char *)(&tmp),2);
				adr += 2;
			}
		}
	}
	for(i = 0; i < GradeTmp.FieldNum;i++)
	{
		buf[adr++] = GradeTmp.next_gdp[i].nzn;
		tmp = ShortEndianConvert(GradeTmp.next_gdp[i].o_type);
		memcpy(&buf[adr],(unsigned char *)(&tmp),2);
		adr += 2;
		den = IntEndianConvert(GradeTmp.next_gdp[i].den);
		memcpy(&buf[adr],(unsigned char *)(&den),4);
		adr += 4;
		buf[adr++] = GradeTmp.next_gdp[i].prc_n;
		for(j = 0; j < GradeTmp.next_gdp[i].prc_n; j++)
		{
			tmp = ShortEndianConvert(GradeTmp.next_gdp[i].prc[j]);
			memcpy(&buf[adr],(unsigned char *)(&tmp),2);
			adr += 2;
		}
	}
	crc = Crc16(buf,480,0);
	memcpy(&buf[480],(unsigned char *)(&crc),2);
	adr = LOCAL_PRICE_ADDRESS * AT45_PageSize(pAt45);
	if(WRITE(CONFIG_ID,buf,adr,482) != 482)
	{
		return 1;
	}
	return 0;
}

/*unsigned char LoadGradePriceList(void)
{
    unsigned char buf[482];
    unsigned int adr;
    unsigned short crc,crctmp;
    unsigned char i,j;
    unsigned short tmp;
    unsigned int den;
    At45 *pAt45;
	GDP	gdp;
	GDP	nextgdp;
    
	memset((unsigned char *)(&GradePrice),0,sizeof(GradePrice));
    pAt45 = &at45[CONFIG_ID];
    adr = GRADE_PRICE_ADDRESS * AT45_PageSize(pAt45);
    if ( READ(CONFIG_ID,buf,adr,482) != 482 )
    {
        return 1;
    }
    crc = Crc16(buf,480,0);
    memcpy((unsigned char *)(&crctmp),&buf[480],2);
    if ( crc != crctmp )
    {
        return 2;
    }
    memcpy(&GradePrice.Ver,buf,8);
    adr = 8;
    for ( i = 0; i < GradePrice.FieldNum; i++ )
    {
        GradePrice.gdp[i].nzn = buf[adr++];
        memcpy((unsigned char *)(&tmp),&buf[adr],2);
        adr += 2;
        GradePrice.gdp[i].o_type = ShortEndianConvert(tmp);
        memcpy((unsigned char *)(&den),&buf[adr],4);
        adr += 4;
        GradePrice.gdp[i].den = IntEndianConvert(den);
        GradePrice.gdp[i].prc_n = buf[adr++];
        for ( j = 0; j < GradePrice.gdp[i].prc_n; j++ )
        {
            memcpy((unsigned char *)(&tmp),&buf[adr],2);
            adr += 2;
            GradePrice.gdp[i].prc[j] = ShortEndianConvert(tmp);
        }
    }
    for ( i = 0; i < GradePrice.FieldNum; i++ )
    {
        GradePrice.next_gdp[i].nzn = buf[adr++];
        memcpy((unsigned char *)(&tmp),&buf[adr],2);
        adr += 2;
        GradePrice.next_gdp[i].o_type = ShortEndianConvert(tmp);
        memcpy((unsigned int *)(&den),&buf[adr],4);
        adr += 4;
        GradePrice.next_gdp[i].den = IntEndianConvert(den);
        GradePrice.next_gdp[i].prc_n = buf[adr++];
        for ( j = 0; j < GradePrice.next_gdp[i].prc_n; j++ )
        {
            memcpy((unsigned char *)(&tmp),&buf[adr],2);
            adr += 2;
            GradePrice.next_gdp[i].prc[j] = ShortEndianConvert(tmp);
       
		}
    }
	for(i = 0; i < GradePrice.FieldNum - 1; i++)
	{
		for(j = i + 1; j < GradePrice.FieldNum; j++)
		{
			if(GradePrice.next_gdp[i].nzn > GradePrice.next_gdp[j].nzn)
			{
				memcpy((unsigned char *)(&gdp),(unsigned char *)(&GradePrice.gdp[i]),sizeof(GDP));
				memcpy((unsigned char *)(&nextgdp),(unsigned char *)(&GradePrice.next_gdp[i]),sizeof(GDP));
				memcpy((unsigned char *)(&GradePrice.gdp[i]),(unsigned char *)(&GradePrice.gdp[j]),sizeof(GDP));
				memcpy((unsigned char *)(&GradePrice.next_gdp[i]),(unsigned char *)(&GradePrice.next_gdp[j]),sizeof(GDP));
				memcpy((unsigned char *)(&GradePrice.gdp[j]),(unsigned char *)(&gdp),sizeof(GDP));
				memcpy((unsigned char *)(&GradePrice.next_gdp[j]),(unsigned char *)(&nextgdp),sizeof(GDP));
			}
		}
	}
    return 0;
}*/

unsigned int GetBaseBlackListCount(void)
{
    unsigned int num;
    
    memcpy((unsigned char *)(&num),BaseBlackListHead.ListAmount,4);
    num = IntEndianConvert(num);
    return num;
}

unsigned short GetListVersion(unsigned char typ)
{
    unsigned short ver;
    switch ( typ )
    {
    case BASEBLACKLIST :
        memcpy((unsigned char *)(&ver),BaseBlackListHead.Ver,2);
        break;
    case ADDBLACKLIST :
        memcpy((unsigned char *)(&ver),AddBlackListHead.Ver,2);
        break;
    case DELBLACKLIST :
        memcpy((unsigned char *)(&ver),DelBlackListHead.Ver,2);
        break;
    case WHITELIST :
        memcpy((unsigned char *)(&ver),WhiteListHead.Ver,2);
        break;
    case GRADEPRICE :
        ver = GradePrice.Ver;
        break;
    case OILSTOP :
        ver = OilMessage.ver;
        break;
    default:
        ver = 0;
        break;
    }
    return ver;
}

unsigned char WriteListLength(unsigned char typ,unsigned int len)
{
    unsigned char flg = 1;
	
	WriteListNumber = 0;
    switch ( typ )
    {
    case BASEBLACKLIST :
        cr_protected.BaseBlackListLength = len;
		CalculateCrc();
        flg = 0;
        break;
    case ADDBLACKLIST :
        cr_protected.AddBlackListLength = len;
		CalculateCrc();
        flg = 0;
        break;
    case DELBLACKLIST :
        cr_protected.DelBlackListLength = len;
		CalculateCrc();
        flg = 0;
        break;
    case WHITELIST :
        cr_protected.WhiteListLength = len;
		CalculateCrc();
        flg = 0;
        break;
    case GRADEPRICE :
        cr_protected.GradePriceLength = len;
		CalculateCrc();
        flg = 0;
        break;
    case OILSTOP :
        cr_protected.OilStopLength = len;
		CalculateCrc();
        flg = 0;
        break;
    default:
        break;
    }
    return flg;
}

unsigned short GetPrice(unsigned char gdp,unsigned char prc_n)
{
    unsigned short price;
    if ( gdp > (GradePrice.FieldNum - 1) )
    {
        return 0;
    }
    if ( cr_protected.PriceUpdateFlg < 10 )
    {
        if ( prc_n > (GradePrice.gdp[gdp].prc_n - 1) )
        {
            return 0;
        }
        price = GradePrice.gdp[gdp].prc[prc_n];
    }
    else
    {
        if ( prc_n > (GradePrice.next_gdp[gdp].prc_n - 1) )
        {
            return 0;
        }
        price = GradePrice.next_gdp[gdp].prc[prc_n];
    }
	
    return price;
}

unsigned char GetNZN(unsigned char gdp)
{
    unsigned char nzn;
    if ( gdp > (GradePrice.FieldNum - 1) )
    {
        return 0;
    }
    if ( cr_protected.PriceUpdateFlg < 10 )
    {
        nzn = GradePrice.gdp[gdp].nzn;
    }
    else
    {
        nzn = GradePrice.next_gdp[gdp].nzn;
    }
    return nzn;
}

unsigned short GetO_Type(unsigned char gdp)
{
    unsigned short o_type;
    if ( gdp > (GradePrice.FieldNum - 1) )
    {
        return 0;
    }
    if ( cr_protected.PriceUpdateFlg < 10 )
    {
        o_type = GradePrice.gdp[gdp].o_type;
    }
    else
    {
        o_type = GradePrice.next_gdp[gdp].o_type;
    }
    return o_type;
}

unsigned short GetCurrentPrice(unsigned char gdp,unsigned char prc_n)
{
    unsigned short price;
    if ( gdp > (GradePrice.FieldNum - 1) )
    {
        return 0;
    }
    if ( prc_n > (GradePrice.gdp[gdp].prc_n - 1) )
    {
        return 0;
    }
    price = GradePrice.gdp[gdp].prc[prc_n];
    return price;
}

unsigned char GetCurrentNZN(unsigned char gdp)
{
    unsigned char nzn;
    if ( gdp > (GradePrice.FieldNum - 1) )
    {
        return 0;
    }
    nzn = GradePrice.gdp[gdp].nzn;
    return nzn;
}

unsigned short GetCurrentO_Type(unsigned char gdp)
{
    unsigned short o_type;
    if ( gdp > (GradePrice.FieldNum - 1) )
    {
        return 0;
    }
    o_type = GradePrice.gdp[gdp].o_type;
    return o_type;
}

unsigned short GetNextPrice(unsigned char gdp,unsigned char prc_n)
{
    unsigned short price;
    if ( gdp > (GradePrice.FieldNum - 1) )
    {
        return 0;
    }
    if ( prc_n > (GradePrice.next_gdp[gdp].prc_n - 1) )
    {
        return 0;
    }
    price = GradePrice.next_gdp[gdp].prc[prc_n];
    return price;
}

unsigned char GetNextNZN(unsigned char gdp)
{
    unsigned char nzn;
    if ( gdp > (GradePrice.FieldNum - 1) )
    {
        return 0;
    }
    nzn = GradePrice.next_gdp[gdp].nzn;
    return nzn;
}

unsigned short GetNextO_Type(unsigned char gdp)
{
    unsigned short o_type;
    if ( gdp > (GradePrice.FieldNum - 1) )
    {
        return 0;
    }
    o_type = GradePrice.next_gdp[gdp].o_type;
    return o_type;
}

unsigned char CheckListVersion(unsigned char *pList)
{
    if ( pList[6] != OilMessage.ver )
    {
        pList[0] = 5;
        return 1;
    }
    if ( pList[5] != GradePrice.Ver )
    {
        pList[0] = 4;
        return 1;
    }
    if ( pList[4] != WhiteListHead.Ver[1] )
    {
        pList[0] = 3;
        return 1;
    }
    if ( pList[2] != AddBlackListHead.Ver[1] )
    {
        pList[0] = 1;
        return 1;
    }
    if ( pList[3] != DelBlackListHead.Ver[1] )
    {
        pList[0] = 2;
        return 1;
    }
    if ( memcmp(pList,BaseBlackListHead.Ver,2) )
    {
        pList[0] = 0;
        return 1;
    }
    return 0;
}

unsigned char GetProv(void)
{
    return OilMessage.prov;
}

unsigned char GetCity(void)
{
    return OilMessage.city;
}

unsigned int GetSuperior(void)
{
    unsigned int superior;
    memcpy((unsigned char *)(&superior),OilMessage.superior,4);
    return superior;
}

unsigned int GetS_ID(void)
{
    unsigned int s_id;
    memcpy((unsigned char *)(&s_id),OilMessage.s_id,4);
    return s_id;
}

unsigned char GetPOS_P(void)
{
    return OilMessage.pos_p;
}

unsigned char GetGUN_N(void)
{
    return OilMessage.gun_n > 8 ? 8 : OilMessage.gun_n;
}

unsigned char GetLogicNzn(unsigned char PhysicsNo)
{
    if ( (PhysicsNo + 1) > OilMessage.gun_n )
    {
        return 0;
    }
    else
    {
        return OilMessage.nzn[PhysicsNo];
    }
}

unsigned char GetPhysicsNzn(unsigned char LogicNo)
{
    unsigned char i;
    for ( i = 0; i < OilMessage.gun_n; i++ )
    {
        if ( OilMessage.nzn[i] == LogicNo )
        {
            return i;
        }
    }
    return 0xff;
}

void SetListHead(unsigned char typ)
{
	switch(typ)
	{
	case BASEBLACKLIST:
		memset((unsigned char *)(&BaseBlackListHead),0,sizeof(BaseBlackListHead));
		break;
	case ADDBLACKLIST:
		memset((unsigned char *)(&AddBlackListHead),0,sizeof(AddBlackListHead));
		break;
	case DELBLACKLIST:
		memset((unsigned char *)(&DelBlackListHead),0,sizeof(DelBlackListHead));
		break;
	case WHITELIST:
		memset((unsigned char *)(&WhiteListHead),0,sizeof(WhiteListHead));
		break;
	default:
		break;
	}
}

unsigned char GetBaseListData_Area(unsigned char *s)
{
	memcpy(s,BaseBlackListHead.InuredDate,14);
	return(0);
}

unsigned char GetAddListData_Area(unsigned char *s)
{
	memcpy(s,AddBlackListHead.InuredDate,14);
	return(0);
}

unsigned char GetDelListData_Area(unsigned char *s)
{
	memcpy(s,DelBlackListHead.InuredDate,14);
	return(0);
}

void ListIntegrality(void)
{
	At45 *pAt45;
	unsigned int adr;
	unsigned char buf[8];
	unsigned short crc,crctmp;
	 
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = LIST_INTEGRALITY * AT45_PageSize(pAt45);
	READ(CONFIG_ID,buf,adr,8);
	crc = Crc16(buf,6,0);
	memcpy((unsigned char *)(&crctmp),&buf[6],2);
	if(crc == crctmp)
	{
		if(buf[0] == 0)
		{
			cr_protected.BaseBlackListIntegrality = 0;
		}
		else
		{
			cr_protected.BaseBlackListIntegrality = 0xffff;
		}
		if(buf[1] == 0)
		{
			cr_protected.AddBlackListIntegrality = 0;
		}
		else
		{
			cr_protected.AddBlackListIntegrality = 0xffff;
		}
		if(buf[2] == 0)
		{
			cr_protected.DelBlackListIntegrality = 0;
		}
		else
		{
			cr_protected.DelBlackListIntegrality = 0xffff;
		}
		if(buf[3] == 0)
		{
			cr_protected.WhiteListIntegrality = 0;
		}
		else
		{
			cr_protected.WhiteListIntegrality = 0xffff;
		}
		if(buf[4] == 0)
		{
			cr_protected.GradePriceIntegrality = 0;
		}
		else 
		{
			cr_protected.GradePriceIntegrality = 0xffff;
		}
		if(buf[5] == 0)
		{
			cr_protected.OilStopIntegrality = 0;
		}
		else
		{
			cr_protected.OilStopIntegrality = 0xffff;
		}
	}
	else
	{
		memset((unsigned char *)(&cr_protected.BaseBlackListIntegrality),0xff,12);
	}
	CalculateCrc();
}

unsigned short SetPriceUpdateFlg(unsigned short val)
{
    unsigned short flg;
	
    flg = cr_protected.PriceUpdateFlg;
    cr_protected.PriceUpdateFlg = val;
	WritePriceFlgToDataFlash(val);
	CalculateCrc();
    return flg;
}


void WritePriceFlgToDataFlash(unsigned short PriceFlg)
{
	At45 *pAt45;
	unsigned int adr;
	
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = LIST_INTEGRALITY * AT45_PageSize(pAt45) + 8;
	WRITE(CONFIG_ID,(unsigned char *)(&PriceFlg),adr,2);
}

unsigned short ReadPriceFlgFromDataFlash(void)
{
	At45 *pAt45;
	unsigned int adr;
	unsigned short PriceFlg;
	
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = LIST_INTEGRALITY * AT45_PageSize(pAt45) + 8;
	READ(CONFIG_ID,(unsigned char *)(&PriceFlg),adr,2);
	return(PriceFlg);
}

void SetPriceReceviceFlg(void)
{
	At45 *pAt45;
	unsigned char flg = 1;
	unsigned int adr;
	
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = LIST_INTEGRALITY * AT45_PageSize(pAt45) + 10;
	WRITE(CONFIG_ID,&flg,adr,1);
}

unsigned char ReadPriceReceviceFlg(void)
{
	At45 *pAt45;
	unsigned char flg;
	unsigned int adr;
	
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = LIST_INTEGRALITY * AT45_PageSize(pAt45) + 10;
	READ(CONFIG_ID,&flg,adr,1);
	if(flg != 1)
	{
		flg = 0;
	}
	return(flg);
}

void ClrPriceReceviceFlg(void)
{
	At45 *pAt45;
	unsigned char flg = 0;
	unsigned int adr;
	
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = LIST_INTEGRALITY * AT45_PageSize(pAt45) + 10;
	WRITE(CONFIG_ID,&flg,adr,1);
}

unsigned char SetOverTurnNum(unsigned char *p,unsigned char len)
{
	At45 *pAt45;
	unsigned int adr;
	
	if(len > 228)
	{
		return(1);
	}
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = LIST_INTEGRALITY * AT45_PageSize(pAt45) + 300;
	WRITE(CONFIG_ID,p,adr,len);
	return(0);
}

unsigned char GetOverTurnNum(unsigned char *p,unsigned char len)
{
	At45 *pAt45;
	unsigned int adr;
	
	if(len > 228)
	{
		return(1);
	}
	pAt45 = &at45[CONFIG_ID];
	SPID_ConfigureCS(pAt45->pSpid, CONFIG_ID, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	adr = LIST_INTEGRALITY * AT45_PageSize(pAt45) + 300;
	READ(CONFIG_ID,p,adr,len);
	return(0);
}