#include "POSMode2.h"

void WritePosPrice(unsigned char *p,unsigned short len)
{
	At45 *pAt45;
	unsigned char buf[64];
	unsigned short crc;
	unsigned int adr;
	
	pAt45 = &at45[CONFIG_ID];
	memcpy(buf,p,len);
	crc = Crc16(buf,len,0);
	memcpy(&buf[len],(unsigned char *)(&crc),2);
	adr = POS_PRICE_ADR * AT45_PageSize(pAt45);
	WRITE(CONFIG_ID,buf,adr,len + 2);
}

unsigned short GetPosPriceNew(unsigned char nzn)
{
	At45 *pAt45;
	unsigned char buf[528];
	unsigned short crc,crctmp;
	unsigned int adr;
	unsigned char i,len;
	unsigned short price;
	
	pAt45 = &at45[CONFIG_ID];
	adr = POS_PRICE_ADR * AT45_PageSize(pAt45);
	READ(CONFIG_ID,buf,adr,482);
	len = buf[0];
	crc = Crc16(buf,len * 5 + 12,0);
	memcpy((unsigned char *)(&crctmp),&buf[len * 5 + 12],2);
	if(crc != crctmp)
	{
		return((unsigned short)(-1));
	}
	if((len > 8) || (len == 0))
	{
		return((unsigned short)(-1));
	}
	for(i = 0; i < len; i++)
	{
		if(buf[8 + 5 * i] == nzn)
		{
			break;
		}
	}
	if(i == len)
	{
		return((unsigned short)(-1));
	}
	memcpy((unsigned char *)(&price),&buf[9 + 5 * i],2);
	price = ShortEndianConvert(price);
	return(price);
}

unsigned short GetPosPriceOld(unsigned char nzn)
{
	At45 *pAt45;
	unsigned char buf[528];
	unsigned short crc,crctmp;
	unsigned int adr;
	unsigned char i,len;
	unsigned short price;
	
	pAt45 = &at45[CONFIG_ID];
	adr = POS_PRICE_ADR * AT45_PageSize(pAt45);
	READ(CONFIG_ID,buf,adr,482);
	len = buf[0];
	crc = Crc16(buf,len * 5 + 12,0);
	memcpy((unsigned char *)(&crctmp),&buf[len * 5 + 12],2);
	if(crc != crctmp)
	{
		return((unsigned short)(-1));
	}
	if((len > 8) || (len == 0))
	{
		return((unsigned short)(-1));
	}
	for(i = 0; i < len; i++)
	{
		if(buf[8 + 5 * i] == nzn)
		{
			break;
		}
	}
	if(i == len)
	{
		return((unsigned short)(-1));
	}
	memcpy((unsigned char *)(&price),&buf[11 + 5 * i],2);
	price = ShortEndianConvert(price);
	return(price);
}

static unsigned int ExportTtc = 0;

unsigned int QueExportRecord(void)
{
	unsigned int space;
	
	if(ExportTtc == 0)
	{
		ExportTtc = cr_protected.RecordReadTTC;
	}
	if(cr_protected.RecordWriteTTC >= ExportTtc)
	{
		space = cr_protected.RecordWriteTTC - ExportTtc;
		space = RECORD_MAX - space;
	}
	else
	{
		space = 0xffffffff - ExportTtc;
		space += (cr_protected.RecordWriteTTC - 1);
		space = RECORD_MAX - space;
	}
	return(space);
}

unsigned int ExportTtcInc(void)
{
	ExportTtc++;
    if ( ExportTtc == 0 )
    {
        ExportTtc++;
    }
    return ExportTtc;
}

unsigned int ReadExportRecord(unsigned char *s)
{
	unsigned char buf[128];
    unsigned char flg;
    //unsigned int ttc;
	unsigned int space;
	//unsigned char gun_n;
	
    space = QueExportRecord();
	if(space == RECORD_MAX)
	{
		return 0;
	}
	flg = FindRecordTTC(ExportTtc,buf);
	//liuzg 2010-6-18
	if(flg)
	{
		return 0;
	}
	if((buf[0] & 0xf0) == 0x50)
	{
		ExportTtcInc();
		return 0;
	}
	memcpy(s,buf,96);
    return ExportTtc;
}

unsigned char MakeExportRecord(void)
{
	unsigned char buf[98];
    unsigned short page,crc;
    unsigned int adr,tmp,ttc;
    At45 *pAt45;
    
    pAt45 = &at45[TRANS_ID];
	ttc = ExportTtc;
    if ( ttc >= cr_protected.RecordWriteTTC )
    {
        return 1;
    }
    else
    {
        if ( (ttc + RECORD_MAX) < (cr_protected.RecordWriteTTC - 1) )
        {
            return 2;
        }
    }
    page = ttc >> 2;
    page %= (cr_protected.RecordEndPageAddr - cr_protected.RecordStartPageAddr + 1);
	page += cr_protected.RecordStartPageAddr;
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
    buf[0] &= 0x0f;
	buf[0] |= 0x50;
	crc = Crc16(buf,96,0);
	memcpy(&buf[96],(unsigned char *)(&crc),2);
	if(WRITE(TRANS_ID,buf,adr,98) != 98)
	{
		return 5;
	}
	ExportTtcInc();
	return 0;
}