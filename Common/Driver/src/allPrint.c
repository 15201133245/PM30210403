#include "global.h"

//Douzhq 20140710 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
#include "CRMemCopy.h"
#endif

static const unsigned char prt0_dt[] = {0x1b,0x40};
static const char Row1[] = "ª∂”≠ π”√÷–π˙ ØªØº””ÕICø®";

static const char gcode89[] = "89#∆˚”Õ";
static const char gcode90[] = "90#∆˚”Õ";
static const char gcode92[] = "92#∆˚”Õ";
static const char gcode93[] = "93#∆˚”Õ";
static const char gcode95[] = "95#∆˚”Õ";
static const char gcode97[] = "97#∆˚”Õ";
static const char gcode120[] = "120#∆˚”Õ";
static const char gcodeOther1[] = "∆‰À˚∆˚”Õ1";
static const char gcode98[] = "98#∆˚”Õ";
static const char gcodeOther2[] = "∆‰À˚∆˚”Õ2";
static const char gcode0[] = "0#≤Ò”Õ";
static const char gcode5[] = "+5#≤Ò”Õ";
static const char gcode10[] = "+10#≤Ò”Õ";
static const char gcode15[] = "+15#≤Ò”Õ";
static const char gcode20[] = "+20#≤Ò”Õ";
static const char gcode_5[] = "-5#≤Ò”Õ";
static const char gcode_10[] = "-10#≤Ò”Õ";
static const char gcode_15[] = "-15#≤Ò”Õ";
static const char gcode_20[] = "-20#≤Ò”Õ";
static const char gcode_30[] = "-30#≤Ò”Õ";
static const char gcode_35[] = "-35#≤Ò”Õ";
static const char gcode_50[] = "-50#≤Ò”Õ";
static const char gcode_Other1[] = "«·≤Ò”Õ";
static const char gcode_Other2[] = "∆‰À˚≤Ò”Õ";

const gcode_t gcode1[] = 
{
	{0x101f,gcode90},
	{0x103f,gcode93},
	{0x104f,gcode95},
	{0x105f,gcode97},
	{0x106f,gcode120},
	{0x108f,gcodeOther1},
	{0x109f,gcode98},
	{0x1fff,gcodeOther2},
	{0x2001,gcode0},
	{0x2002,gcode5},
	{0x2003,gcode10},
	{0x2004,gcode15},
	{0x2005,gcode20},
	{0x2006,gcode_5},
	{0x2007,gcode_10},
	{0x2008,gcode_15},
	{0x2009,gcode_20},
	{0x2010,gcode_30},
	{0x2011,gcode_35},
	{0x2015,gcode_50},
	{0x2016,gcode_Other1},
	{0x2020,gcode_Other1},
	{0x2fff,gcode_Other2}
};

const gcode_t gcode2[] = 
{
	{0x101f,gcode89},
	{0x103f,gcode92},
	{0x104f,gcode95},
	{0x105f,gcode95},
	{0x106f,gcode120},
	{0x108f,gcodeOther1},
	{0x109f,gcode98},
	{0x1fff,gcodeOther2},
	{0x2001,gcode0},
	{0x2002,gcode5},
	{0x2003,gcode10},
	{0x2004,gcode15},
	{0x2005,gcode20},
	{0x2006,gcode_5},
	{0x2007,gcode_10},
	{0x2008,gcode_15},
	{0x2009,gcode_20},
	{0x2010,gcode_30},
	{0x2011,gcode_35},
	{0x2015,gcode_50},
	{0x2016,gcode_Other1},
	{0x2020,gcode_Other1},
	{0x2fff,gcode_Other2}
};

extern	PrintBuf printStruct;

unsigned char BcdToAscii(char *d, char *s,unsigned char len)
{
	unsigned char i,k,tmp;
	
	k = 0;
	for(i = 0; i < len; i++)
	{
		tmp = (*s) >> 4;
		if(tmp > 9)
		{
			tmp = tmp - 0x0a + 'A';
		}
		else
		{
			tmp += '0';
		}
		k++;
		*(d++) = tmp;
		tmp = (*s) & 0x0f;
		if(tmp > 9)
		{
			tmp = tmp - 0x0a + 'A';
		}
		else
		{
			tmp += '0';
		}
		s++;
		k++;
		*(d++) = tmp;
	}
	return(k);
}

unsigned char HexToAscii(char *s,unsigned int val)
{
	unsigned char k,len = 0;
	char buf[16];
	
	k = sprintf(buf,"%lu",val);
	if(k == 1)
	{
		*s++ = '0';
		*s++ = '.';
		*s++ = '0';
		*s = buf[0];
		len = 4;
	}
	else if(k == 2)
	{
		*s++ = '0';
		*s++ = '.';
		memcpy(s,buf,2);
		len = 4;
	}
	else
	{
		memcpy(s,buf,k-2);
		s[k-2] = '.';
		memcpy(s + k -1,&buf[k-2],k);
		len = k + 1;
	}
	return(len);
}

void ReadTicketHead(char *p)
{
	unsigned char len;
	unsigned short crc,crctmp;
	unsigned char buf[64];
	
	READ(CONFIG_ID,buf,TICKET_HEAD_ADRESS *  AT45_PageSize(&at45[0]),64);
	len = strlen((char *)buf);
	crc = Crc16(buf,len + 1,0);
	memcpy((unsigned char *)(&crctmp),&buf[len + 1],2);
//	memcpy((unsigned char *)(&crctmp),&buf[len],2);
	crctmp = ShortEndianConvert(crctmp);
	if(crc == crctmp)
	{
		memcpy(p,buf,len + 1);
	}
	else
	{
		len = strlen(Row1);
		memcpy(p,Row1,len + 1);
	}
}

unsigned char ReadG_Code(char* s, unsigned short g_code)
{
	char buf[32];
	unsigned int adr;
	unsigned short i,len,num,numlen;
	unsigned short gcd;
	At45 *pAt45;
	char tmpbuf[32];
	char flg = 0;
	const gcode_t *pCode;
	
	
	pAt45 = &at45[0];
	adr = G_CODE_TICKET_ADR * AT45_PageSize(pAt45);
	READ(CONFIG_ID,(unsigned char*)buf,adr,16);
	memcpy((unsigned char *)(&num),buf,2);
	num = ShortEndianConvert(num);
	memcpy((unsigned char *)(&numlen),&buf[2],2);
	numlen = ShortEndianConvert(numlen);
	adr += 16;
	if((num == 0xffff) || (numlen > 32))
	{
		if(Area_Station == 1)
		{
			pCode = gcode2;
			num = sizeof(gcode2) / sizeof(gcode_t);
		}
		else
		{
			pCode = gcode1;
			num = sizeof(gcode1) / sizeof(gcode_t);
		}
		for(i = 0; i < num; ++i)
		{
			if(((pCode+i)->g_code & 0x0fff) == 0xfff)
			{
				if(((pCode + i)->g_code & 0xf000) == (g_code & 0xf000))
				{
					len = strlen((pCode + i)->gStr);
					memcpy(tmpbuf,(pCode + i)->gStr,len);
					flg = 1;
					break;
				}
			}
			else if(((pCode + i)->g_code & 0x00ff) == 0x00ff)
			{
				if(((pCode + i)->g_code & 0xff00) == (g_code & 0xff00))
				{
					len = strlen((pCode + i)->gStr);
					memcpy(tmpbuf,(pCode + i)->gStr,len);
					flg = 1;
					break;
				}
			}
			else if(((pCode + i)->g_code & 0x000f) == 0x000f)
			{
				if(((pCode + i)->g_code & 0xfff0) == (g_code & 0xfff0))
				{
					len = strlen((pCode + i)->gStr);
					memcpy(tmpbuf,(pCode + i)->gStr,len);
					flg = 1;
					break;
				}
			}
			else
			{
				if((pCode + i)->g_code == g_code)
				{
					len = strlen((pCode + i)->gStr);
					memcpy(tmpbuf,(pCode + i)->gStr,len);
					flg = 1;
					break;
				}
			}
		}
		if(flg)
		{
			memcpy(s,tmpbuf,len);
		}
		else
		{
			g_code = ShortEndianConvert(g_code);
			memcpy(tmpbuf,(unsigned char*)(&g_code),2);
			len = BcdToAscii(s,tmpbuf,2);
		}
		return(len);
	}
	for(i = 0; i < num; ++i)
	{
		do
		{
			len = READ(CONFIG_ID,(unsigned char*)buf,adr,numlen);
		}
		while(len != numlen);
		memcpy((unsigned char *)(&gcd),buf,2);
		gcd = ShortEndianConvert(gcd);
		if((gcd & 0x0fff) == 0x0fff)
		{
			if((gcd & 0xf000)  == (g_code & 0xf000))
			{
				len = strlen(&buf[2]);
				if(len > 30)
				{
					break;
				}
				memcpy(tmpbuf,&buf[2],len);
				flg = 1;
				break;
			}
		}
		else if((gcd & 0x00ff) == 0x00ff)
		{
			if((gcd & 0xff00) == (g_code & 0xff00))
			{
				len = strlen(&buf[2]);
				if(len > 30)
				{
					break;
				}
				memcpy(tmpbuf,&buf[2],len);
				flg = 1;
				break;
			}
		}
		else if((gcd & 0x000f) == 0x000f)
		{
			if((gcd & 0xfff0) == (g_code & 0xfff0))
			{
				len = strlen(&buf[2]);
				if(len > 30)
				{
					break;
				}
				memcpy(tmpbuf,&buf[2],len);
				flg = 1;
				break;
			}
		}
		else
		{
			if(gcd == g_code)
			{
				len = strlen(&buf[2]);
				if(len > 30)
				{
					break;
				}
				memcpy(tmpbuf,&buf[2],len);
				flg = 1;
				break;
			}
		}
		adr += numlen;
	}
	if(flg == 1)
	{
		memcpy(s,tmpbuf,len);
	}
	else
	{
		memcpy(tmpbuf,(unsigned char *)(&g_code),2);
		len = BcdToAscii(s,tmpbuf,2);
	}
	return(len);
}


void InitPrint(void)
{
	unsigned char *pStr;
	
	pStr = printStruct.buf;
	memcpy(pStr,prt0_dt,2);
	chCtrl[1].sendback = PrintComplete;
	printStruct.len = 2;
	WRITE(USART3_ID,printStruct.buf,0,2);
}

void InitPrintTaskList(void)
{
	unsigned char i;
	sPrintTrans *p;
	sPrintTrans *p1;
	
	memset(PrintTrans,0,PRINT_TASK_MAX * sizeof(sPrintTrans));
	p = &PrintTrans[0];
	p1 = &PrintTrans[1];
	for(i = 0; i < PRINT_TASK_MAX - 1; i++)
	{
		p->pNext = p1;
		p1->pPrev = p;
		p++;
		p1++;
	}
	p->pNext = (sPrintTrans *)0;
	pPrintFreeList = &PrintTrans[0];
	pPrintList = (sPrintTrans *)0;
}

unsigned char AddPrintTask(unsigned char *s)
{
	sPrintTrans *pPrint;
	sPrintTrans *pPrint1;
	sPrintTrans *pPrint2;
	
	if(PrintBusy)
	{
		return(2);
	}
	pPrint = pPrintFreeList;
	if(pPrint == (sPrintTrans *)0)
	{
		return(1);
	}
	pPrintFreeList = pPrint->pNext;
	memcpy(pPrint->s_id,s,100);
	pPrint->pNext = (sPrintTrans *)0;
	pPrint1 = pPrintList;
	while(pPrint1 != (sPrintTrans *)0)
	{
		pPrint2 = pPrint1;
		pPrint1 = pPrint1->pNext;
	}
	if(pPrint1 == pPrintList)
	{
		pPrintList = pPrint;
	}
	else
	{
		pPrint2->pNext = pPrint;
		pPrint->pPrev = pPrint2;
	}
	return(0);
}

unsigned char DelPrintTask(sPrintTrans *pPrint)
{
	sPrintTrans *pPrint1;
	
	if(pPrint == (sPrintTrans *)0)
	{
		return(1);
	}
	if(pPrintList == (sPrintTrans *)0)
	{
		return(2);
	}
	pPrint1 = pPrintList;
	while(pPrint1 != (sPrintTrans *)0)
	{
		if(pPrint1 == pPrint)
		{
			if(pPrint1 == pPrintList)
			{
				pPrintList = pPrintList->pNext;
			}
			else
			{
				pPrint1->pPrev->pNext = pPrint1->pNext;
				pPrint1->pNext->pPrev = pPrint1->pPrev;
			}
			pPrint1->pPrev = (sPrintTrans *)0;
			pPrint1->pNext = pPrintFreeList;
			pPrintFreeList->pPrev = pPrint1;
			pPrintFreeList = pPrint1;
			return(0);
		}
		pPrint1 = pPrint1->pNext;
	}
	return(3);
}

void PrintComplete(unsigned char *s,unsigned char val)
{
	printStruct.len = 0;
}