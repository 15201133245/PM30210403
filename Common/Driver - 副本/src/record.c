#include "global.h"

//liuzg start 20121105
unsigned char NotUpLoadling(unsigned int ttc)
{
	unsigned int space;
	if(ttc < cr_protected.RecordReadTTC)
	{
		if(ttc < cr_protected.RecordWriteTTC)
		{
			space = cr_protected.RecordWriteTTC - ttc - 1;
			if(space <= RECORD_MAX)
			{
				cr_protected.RecordReadTTC = ttc;
				return 1;
			}
		}
		else
		{
			space = 0xffffffff - ttc;
			space += (cr_protected.RecordWriteTTC - 1);
			if(space <= RECORD_MAX)
			{
				cr_protected.RecordReadTTC = ttc;
				return 1;
			}
		}
	}
	return 0;	
}

unsigned char WriteRecord(unsigned char *s)
{
    unsigned int adr;
    unsigned char buf[98];
    unsigned short crc,crctmp;
	unsigned int ttc,ttctmp;
	unsigned short page,pageoffset;
    At45 *pAt45;
    
	if(s == (unsigned char*)0)
	{
		return -1;
	}
	CloseLcdLightOil();
    pAt45 = &at45[TRANS_ID];
	memcpy((unsigned char *)(&ttc),&s[1],4);
	ttc = IntEndianConvert(ttc);
	page = ttc / 4;
	page %= (RECORD_END_PAGE - RECORD_START_PAGE + 1);
	page += RECORD_START_PAGE;
	pageoffset = ttc & 0x03;
    adr = page * AT45_PageSize(pAt45);
	adr += (pageoffset * RECORD_SIZE);
	READ(TRANS_ID,buf,adr,98);
	crc = Crc16(buf,96,0);
	memcpy((char *)(&crctmp),&buf[96],2);
	if(crc == crctmp)
	{
		if((buf[0] & 0x0f) == 0x0a)
		{
			memcpy((char *)(&ttctmp),&buf[1],4);
			ttctmp = IntEndianConvert(ttc);
			if((ttc & 0x7ff) == (ttctmp & 0x7ff))
			{
				if(NotUpLoadling(ttctmp))
				{
					return 2;
				}
			}
		}
	}
    memcpy(buf,s,96);
    crc = Crc16(buf,96,0);
    memcpy(&buf[96],(unsigned char *)(&crc),2);
    if ( WRITE(TRANS_ID,buf,adr,98) != 98 )
    {
        return 1;
    }
    return 0;
}
//liuzg end 20121105

unsigned char FindRecordTTC(unsigned int ttc,unsigned char *s)
{
    unsigned char buf[98];
    unsigned short page;//,crc,crctmp;
    unsigned int adr,tmp;
    At45 *pAt45;
    
	if(s == (unsigned char*)0)
	{
		return -1;
	}
    pAt45 = &at45[TRANS_ID];
    /*if ( ttc >= cr_protected.RecordWriteTTC )
    {
        return 2;
    }
    else
    {
        if ( (ttc + RECORD_MAX) < (cr_protected.RecordWriteTTC - 1) )
        {
            return 2;
        }
    }*/
    page = ttc >> 2;
    page %= (RECORD_END_PAGE - RECORD_START_PAGE + 1);
	page += RECORD_START_PAGE;
    adr = page * AT45_PageSize(pAt45);
    adr +=  (ttc & 0x03) * RECORD_SIZE;
    if ( READ(TRANS_ID,buf,adr,98)!= 98 )
    {
        return 3;
    }
	//record crc 
    /*crc = Crc16(buf,96,0);
    memcpy((unsigned char *)(&crctmp),&buf[96],2);
    if ( crc != crctmp )
    {
        return 1;
    }*/
    memcpy((unsigned char *)(&tmp),&buf[1],4);
    tmp = IntEndianConvert(tmp);
    if ( tmp != ttc )
    {
        return 1;
    }
    memcpy(s,buf,96);
    return 0;
}

unsigned char FindRecordTactic(unsigned int ttc,unsigned char *s)
{
    unsigned short i,crc,crctmp;
    unsigned char j;
    unsigned char buf[RECORD_SIZE * 4];
    At45 *pAt45;
    unsigned int adr,tmp;
    
	if(s == (unsigned char*)0)
	{
		return -1;
	}
    pAt45 = &at45[TRANS_ID];
    for ( i = RECORD_START_PAGE; i <= RECORD_END_PAGE; i++ )
    {
        WatchDog();
        adr = i * AT45_PageSize(pAt45);
        if ( READ(TRANS_ID,buf,adr,RECORD_SIZE * 4) != RECORD_SIZE * 4 )
        {
            return 3;
        }
        for ( j = 0; j < 4; j++ )
        {
            memcpy((unsigned char *)(&tmp),&buf[j * RECORD_SIZE + 1],4);
            tmp = IntEndianConvert(tmp);
            if ( tmp == ttc )
            {
                crc = Crc16(&buf[j * RECORD_SIZE],96,0);
                memcpy((unsigned char *)(&crctmp),&buf[j * RECORD_SIZE + 96],2);
                if ( crc != crctmp )
                {
                    return 2;
                }
                memcpy(s,&buf[j * RECORD_SIZE],96);
                return 0;
            }
        }
    }
    return 1;
}

unsigned char FindGrayRecord(unsigned char *dest,unsigned char *source)
{
    unsigned short i;//,crc,crctmp;
    unsigned char j,flg;
    unsigned char buf[RECORD_SIZE * 4];
    At45 *pAt45;
    unsigned int adr;
    
	if((dest == (unsigned char*)0) || (source == (unsigned char*)0))
	{
		return -1;
	}
    pAt45 = &at45[TRANS_ID];
    for ( i = RECORD_START_PAGE; i <= RECORD_END_PAGE; i++ )
    {
        WatchDog();
        adr = i * AT45_PageSize(pAt45);
        if ( READ(TRANS_ID,buf,adr,RECORD_SIZE * 4) != RECORD_SIZE * 4 )
        {
            return 3;
        }
        for ( j = 0; j < 4; j++ )
        {
			/*crc = Crc16(&buf[j * RECORD_SIZE],96,0);
            memcpy((unsigned char *)(&crctmp),&buf[j * RECORD_SIZE + 96],2);
			if(crc != crctmp)
			{
				continue;
			}*/
            if ( (buf[j * RECORD_SIZE + 5] & 0x0f) != 1 )
            {
                continue;
            }
            flg = memcmp(&buf[j * RECORD_SIZE + 13],source,10);
            if ( flg )
            {
                continue;
            }
            flg = memcmp(&buf[j * RECORD_SIZE + 23],&source[10],4);
            if ( flg )
            {
                continue;
            }
            flg = memcmp(&buf[j * RECORD_SIZE + 30],&source[14],2);
            if ( flg )
            {
                continue;
            }
            if ( buf[j * RECORD_SIZE + 64] != source[16] )
            {
                continue;
            }
            flg = memcmp(&buf[j * RECORD_SIZE + 6],&source[17],7);
            if ( flg )
            {
                continue;
            }
            else
            {
                //M-FLAG
                *dest = 0;
                //ASN,BAL,AMN,CTC
                memcpy(&dest[1],&buf[j * RECORD_SIZE + 13],19);
                //DS
                *(dest + 20) = buf[j * RECORD_SIZE + 64];
                //TIME
                memcpy(&dest[21],&buf[j * RECORD_SIZE + 6],7);
                //GMAC
                memcpy(&dest[28],&buf[j * RECORD_SIZE + 36],4);
                //PSAM-TID
                memcpy(&dest[32],&buf[j * RECORD_SIZE + 54],6);
                //PSAM-TTC
                memcpy(&dest[38],&buf[j * RECORD_SIZE + 60],4);
                return 0;
            }
        }
    }
    return 1;
}

unsigned char ReBuildRecordProtected(void)
{
	int ReadTtc,WriteTtc,ttc;
	unsigned int TtcTmp1,TtcTmp2;
    unsigned int i,adr;
    unsigned char j,flg,buf[4 * RECORD_SIZE],buffer[RECORD_SIZE];
    unsigned short crc,crctmp;
	unsigned char TransFlg;
	unsigned char flags;
    At45 *pAt45;
	unsigned char crcErrFlg = 0;
	
    
    pAt45 = &at45[TRANS_ID];
    ReadTtc = 0;
    WriteTtc = 0;
    flg = 0;
	flags = 0;
    memset(buffer,0xff,RECORD_SIZE);
    for ( i = RECORD_START_PAGE; i < RECORD_END_PAGE + 1; i++ )
    {
        WatchDog();
        adr = i * AT45_PageSize(pAt45);
        for ( j = 0; j < 3; j++ )
        {
            if ( READ(TRANS_ID,buf,adr,4 * RECORD_SIZE) == 4 * RECORD_SIZE )
            {
                break;
            }
        }
        if ( j == 3 )
        {
            flg = 1;
            break;
        }
        for ( j = 0; j < 4; j++ )
        {
            if ( memcmp(buffer,&buf[j * RECORD_SIZE],RECORD_SIZE) == 0 )
            {
                continue;
            }
            crc = Crc16(&buf[j * RECORD_SIZE],96,0);
            memcpy((unsigned char *)(&crctmp),&buf[j * RECORD_SIZE + 96],2);
            if ( (crc != crctmp ) && (crcErrFlg == 0))
            {
				WriteDriveLog(13,0);
				crcErrFlg = 1;
                continue;
            }
            memcpy((unsigned char *)(&ttc),&buf[j * RECORD_SIZE + 1],4);
            ttc = IntEndianConvert(ttc);
			TransFlg = buf[j * RECORD_SIZE];
			if(TransFlg == 0xaa)
			{
				if(ReadTtc == 0)
				{
					ReadTtc = ttc;
					WriteTtc = ttc;
				}
				else
				{
					if(ttc < ReadTtc)
					{
						ReadTtc = ttc;
					}
					if(ttc > WriteTtc)
					{
						WriteTtc = ttc;
					}
				}
			}
        }
    }
	if((ReadTtc == 0) && (WriteTtc == 0))
	{
		TtcTmp1 = 0;
		TtcTmp2 = 0;
		flags = 0;
		for ( i = RECORD_START_PAGE; i < RECORD_END_PAGE + 1; i++ )
		{
			WatchDog();
			adr = i * AT45_PageSize(pAt45);
			for ( j = 0; j < 3; j++ )
			{
				if ( READ(TRANS_ID,buf,adr,4 * RECORD_SIZE) == 4 * RECORD_SIZE )
				{
					break;
				}
			}
			if ( j == 3 )
			{
				flg = 1;
				break;
			}
			for ( j = 0; j < 4; j++ )
			{
				if ( memcmp(buffer,&buf[j * RECORD_SIZE],RECORD_SIZE) == 0 )
				{
					continue;
				}
				crc = Crc16(&buf[j * RECORD_SIZE],96,0);
				memcpy((unsigned char *)(&crctmp),&buf[j * RECORD_SIZE + 96],2);
				if ( (crc != crctmp ) && (crcErrFlg == 0))
				{
					WriteDriveLog(13,0);
					crcErrFlg = 1;
					continue;
				}
				memcpy((unsigned char *)(&ttc),&buf[j * RECORD_SIZE + 1],4);
				TtcTmp1 = IntEndianConvert(ttc);
				if(TtcTmp2 == 0)
				{
					flags = 1;
					TtcTmp2 = TtcTmp1;
				}
				else
				{
					if(TtcTmp1 > TtcTmp2)
					{
						flags = 1;
						TtcTmp2 = TtcTmp1;
					}
				}
			}
		}
		ReadTtc = TtcTmp2;
		WriteTtc = ReadTtc;
	}
	if(ReadTtc > WriteTtc)
	{
		unsigned int space;
		space = 0xffffffff - ReadTtc;
		space += WriteTtc;
		if(space > RECORD_MAX)
		{
			flg = 2;
		}
	}
	if ( ReadTtc == 0)//0xffffffff )
	{
		ReadTtc = 1;
	}
    if ( flg )
    {
#if LCD_TYPE==0		//OK
        ClearLcd(2,9,126,49);
#else
		ClearLcd(2,21,317,218);
#endif
        while ( 1 )
        {
            WatchDog();
            if ( flg != 3 )
            {
                LcdErr(RECORD_ERROR,0);
                flg = 3;
            }
            if ( BeepFlg ==0 )
            {
                pBeepTmr = BeepControl2(100);
            }
            else
            {
                if ( BeepFlg == 2 )
                {
                    BeepDelay(100);
                }
            }
            CheckTimerGo();
        }
    }
    if ( flg == 0 )
    {
		if(WriteTtc == (unsigned int)-1)
		{
			WriteTtc++;
		}
		cr_protected.RecordWriteTTC = WriteTtc + 1;
		cr_protected.RecordReadTTC = ReadTtc;
		if(flags)
		{
			cr_protected.RecordReadTTC++;
		}
    }
    return flg;
}

unsigned int ReadRecord(unsigned char *s)
{
    unsigned char buf[96];
    unsigned char flg;
    //unsigned int ttc;
	unsigned int space;
	//unsigned char gun_n; 
	
	if(s == (unsigned char*)0)
	{
		buf[0] = 0;
		return -1;
	}
    space = GetRecordSpace();
	//gun_n = GetGUN_N();
	if(space == RECORD_MAX)
	{
		return 0;
	}
    flg = FindRecordTTC(cr_protected.RecordReadTTC,buf);
	//liuzg 2010-6-18
	if(flg)
	{
		return 0;
	}
	if((buf[0] == 0x55) && (GetRecordSpace() < (RECORD_MAX - 1)))
	{
		ReadTTCInc();
		return 0;
	}
    /*if ( flg > 1)
    {
        return 0;
    }
	else if(flg == 1)
	{
		if((cr_protected.RecordReadTTC + gun_n) < (cr_protected.RecordWriteTTC - 1))
		{
			cr_protected.RecordReadTTC++;
		}
		return 0;
	}
    memcpy((unsigned char *)(&ttc),&buf[1],4);
    ttc = IntEndianConvert(ttc);
    if ( ttc != cr_protected.RecordReadTTC )
    {
		cr_protected.RecordReadTTC = ttc;
    }*/
    memcpy(s,buf,96);
    return cr_protected.RecordReadTTC;
}

unsigned char MakeOverRecord(void)
{
	unsigned char buf[98];
    unsigned short page,crc;
    unsigned int adr,tmp,ttc;
    At45 *pAt45;
    
    pAt45 = &at45[TRANS_ID];
	ttc = cr_protected.RecordReadTTC;
    /*if ( ttc >= cr_protected.RecordWriteTTC )
    {
        return 1;
    }
    else
    {
        if ( (ttc + RECORD_MAX) < (cr_protected.RecordWriteTTC - 1) )
        {
            return 2;
        }
    }*/
    page = ttc >> 2;
    page %= (RECORD_END_PAGE - RECORD_START_PAGE + 1);
	page += RECORD_START_PAGE;
    adr = page * AT45_PageSize(pAt45);
    adr +=  (ttc & 0x03) * RECORD_SIZE;
	if ( READ(TRANS_ID,buf,adr,98)!= 98 )
    {
        return 3;
    }
	//record crc 
    /*crc = Crc16(buf,96,0);
    memcpy((unsigned char *)(&crctmp),&buf[96],2);
    if ( crc != crctmp )
    {
        return 1;
    }*/
    memcpy((unsigned char *)(&tmp),&buf[1],4);
    tmp = IntEndianConvert(tmp);
    if ( tmp != ttc )
    {
        return 4;
    }
    buf[0] = 0x55;
	crc = Crc16(buf,96,0);
	memcpy(&buf[96],(unsigned char *)(&crc),2);
	if(WRITE(TRANS_ID,buf,adr,98) != 98)
	{
		return 5;
	}
	return 0;
}
