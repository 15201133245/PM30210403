#include "global.h"
#if POS_TYPE==2
static const unsigned char Ctrl1[] = {0x1b,0x21,0x10};
static const unsigned char Ctrl2[] = {0x1b,0x21,0x00};

static const char Row2[] = "����ƾ��";
static const char Row3[] = "��վ����:";
static const char Row4[] = "ǹ��:";
static const char Row5[] = "����:";
static const char Row6[] = "��ˮ:";
static const char Row7[] = "��ǰ���:";
static const char Row8[] = "��Ʒ:";
static const char Row9[] = "����:";
static const char Row10[] = "����:";
static const char Row11[] = "Ӧ��:";
static const char Row12[] = "�ۿ�";
static const char Row13[] = "����:";
static const char Row14[] = "�Ż�:";
static const char Row15[] = "�Żݹ���:";
static const char Row16[] = "�ۺ����:";
static const char Row17[] = "��������:";
static const char Row17e[][9] = 
{
	"����",
	"�ӿ�",
	"��",
	"����",
	"����",
	"�ϰ�",
	"�°�",
	"�ǿ�����",
	"�ͼ۽���",
	"����ܾ�",
	"����",
	"�����쳣",
	"�ǿ�����",
	"��̨��Ȩ",
	"������Ȩ",
	"���彻��"
};
static const char Row18[] = "������:";
static const char Row19[] = "����ʱ��:";
static const char Row20[][32]=
{
	"��ƾ����������ƾ֤����1����",
	"��ƾ����������ƾ֤����2����"
};

static const char VOL_C[] = "��";
static const char YUAN[] = "Ԫ";
static const char POINT[] = "��";
static const char UNITS[] = "Ԫ/��";

static const char TRANS_YEAR[] = "��";
static const char TRANS_MONTH[] = "��";
static const char TRANS_DAY[] = "��";

PrintBuf printStruct;

void print_dispose(void)
{
	sPrintTrans *pPrint;
	unsigned char *pStr;
	unsigned short len;
	char HeadBuf[64];
	char buf[64];
	unsigned char i,j,k,m,typ;
	unsigned int itmp,itmp1;
	unsigned short stmp;
	
	if(printStruct.len)
	{
		return;
	}
	pPrint = pPrintList;
	if(pPrint == (sPrintTrans *)0)
	{
		return;
	}
	
	pStr = printStruct.buf;
	len = 0;
	pPrint = pPrintList;
	typ = pPrint->t_type & 0x0f;
	j = 1;
	if(pPrint->typ & 0x01)
	{
		j = 2;
	}
	
	for(i = 0; i < j; i++)
	{
		memcpy(pStr + len,Ctrl1,sizeof(Ctrl1));
		len += sizeof(Ctrl1);
		ReadTicketHead(HeadBuf);
		k = strlen(HeadBuf);
		m = (32 - k) >> 1;
		memset(pStr + len,' ', m);
		len += m;
		memcpy(pStr + len,HeadBuf,k);
		len += k;
		memcpy(pStr + len,Ctrl2,sizeof(Ctrl2));
		len += sizeof(Ctrl2);
		memset(pStr + len,'\n',2);
		len += 2;
		
		k = strlen(Row2);
		m = (32 - k) >> 1;
		memset(pStr + len,' ',m);
		len += m;
		memcpy(pStr + len,Row2,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row3);
		memcpy(pStr + len,Row3,k);
		len += k;
		k = BcdToAscii(buf,pPrint->s_id,4);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row4);
		memcpy(pStr + len,Row4,k);
		len += k;
		k = sprintf(buf,"%lu",pPrint->nzn);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row5);
		memcpy(pStr + len,Row5,k);
		len += k;
		k = BcdToAscii(buf,pPrint->asn,10);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		k = strlen(Row6);
		memcpy(pStr + len,Row6,k);
		len += k;
		memcpy((unsigned char *)(&itmp),pPrint->pos_ttc,4);
		itmp = IntEndianConvert(itmp);
		k = sprintf(buf,"%lu",itmp);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row7);
		memcpy(pStr + len,Row7,k);
		len += k;
		if(pPrint->asn[2] == 0x04)
		{
			itmp = 0;
		}
		else
		{
			memcpy((unsigned char *)(&itmp),pPrint->bal,4);
			itmp = IntEndianConvert(itmp);
			if(typ != 0x01)
			{
				itmp1 = 0;
				memcpy((unsigned char *)(&itmp1),pPrint->amn,3);
				itmp1 = IntEndianConvert(itmp1);
				itmp1 >>= 8;
				itmp += itmp1;
			}
		}
		k = HexToAscii(buf,itmp);
		memcpy(pStr + len,buf,k);
		len += k;
		if(pPrint->ds == 1)
		{
			k = strlen(POINT);
			memcpy(pStr + len,POINT,k);
		}
		else
		{
			k = strlen(YUAN);
			memcpy(pStr + len,YUAN,k);
		}
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row8);
		memcpy(pStr + len,Row8,k);
		len += k;
		memcpy((unsigned char *)(&stmp),pPrint->g_code,2);
		stmp = ShortEndianConvert(stmp);
		k = ReadG_Code(buf,stmp);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row9);
		memcpy(pStr + len,Row9,k);
		len += k;
		memcpy((unsigned char *)(&stmp),pPrint->prc,2);
		stmp = ShortEndianConvert(stmp);
		k = HexToAscii(buf,stmp);
		memcpy(pStr + len,buf,k);
		len += k;
		k = strlen(UNITS);
		memcpy(pStr + len,UNITS,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row10);
		memcpy(pStr + len,Row10,k);
		len += k;
		itmp = 0;
		memcpy((unsigned char *)(&itmp),pPrint->vol,3);
		itmp = IntEndianConvert(itmp);
		itmp >>= 8;
		k = HexToAscii(buf,itmp);
		memcpy(pStr + len,buf,k);
		len += k;
		k = strlen(VOL_C);
		memcpy(pStr + len,VOL_C,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row11);
		memcpy(pStr + len,Row11,k);
		len += k;
		itmp = 0;
		memcpy((unsigned char *)(&itmp),pPrint->rfu,3);
		itmp = IntEndianConvert(itmp);
		itmp >>= 8;
		k = HexToAscii(buf,itmp);
		memcpy(pStr + len,buf,k);
		len += k;
		k = strlen(YUAN);
		memcpy(pStr + len,YUAN,k);
		*(pStr + len++) = '\n';
		
		k = strlen(Row12);
		memcpy(pStr + len,Row12,k);
		len += k;
		if(pPrint->asn[2] == 0x04)
		{
			itmp = 0;
			k = HexToAscii(buf,itmp);
			memcpy(pStr + len,buf,k);
			len += k;
			if(pPrint->ds == 1)
			{
				k = strlen(POINT);
				memcpy(pStr + len,POINT,k);
			}
			else
			{
				k = strlen(YUAN);
				memcpy(pStr + len,YUAN,k);
			}
			len += k;
		}
		else
		{
			if((pPrint->rfu[3] & 0x0f) < 3)
			{
				itmp = 0;
				memcpy((unsigned char *)(&itmp),pPrint->rfu,3);
				itmp = IntEndianConvert(itmp);
				itmp >>= 8;
				k = HexToAscii(buf,itmp);
				memcpy(pStr + len,buf,k);
				len += k;
				if(pPrint->ds == 1)
				{
					k = strlen(POINT);
					memcpy(pStr + len,POINT,k);
				}
				else
				{
					k = strlen(YUAN);
					memcpy(pStr + len,YUAN,k);
				}
				len += k;
			}
			else
			{
				itmp = 0;
				k = HexToAscii(buf,itmp);
				memcpy(pStr + len,buf,k);
				len += k;
				k = strlen(YUAN);
				memcpy(pStr + len,YUAN,k);
			}
		}
		*(pStr + len++) = '\n';
		
		k = strlen(Row13);
		memcpy(pStr + len,Row13,k);
		len += k;
		if((pPrint->rfu[3] & 0x0f) == 3)
		{
			itmp = 0;
			memcpy((unsigned char *)(&itmp),pPrint->rfu,3);
			itmp = IntEndianConvert(itmp);
			itmp >>= 8;
			k = HexToAscii(buf,itmp);
			memcpy(pStr + len,buf,k);
			len += k;
			k = strlen(YUAN);
			memcpy(pStr + len,YUAN,k);
			len += k;
		}
		else
		{
			itmp = 0;
			k = HexToAscii(buf,itmp);
			memcpy(pStr + len,buf,k);
			len += k;
			k = strlen(YUAN);
			memcpy(pStr + len,YUAN,k);
			len += k;
		}
		*(pStr + len++) = '\n';
		
		k = strlen(Row14);
		memcpy(pStr + len,Row14,k);
		len += k;
		itmp = 0;
		memcpy((unsigned char *)(&itmp),pPrint->amn,3);
		itmp = IntEndianConvert(itmp);
		itmp >>= 8;
		memcpy((unsigned char *)(&itmp1),pPrint->rfu,3);
		itmp1 = IntEndianConvert(itmp1);
		itmp1 >>= 8;
		itmp -= itmp1;
		k = HexToAscii(buf,itmp);
		memcpy(pStr + len,buf,k);
		len += k;
		k = strlen(YUAN);
		memcpy(pStr + len,YUAN,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row15);
		memcpy(pStr + len,Row15,k);
		len += k;
		memcpy((unsigned char *)(&stmp),&pPrint->rfu[4],2);
		stmp = ShortEndianConvert(stmp);
		k = sprintf(buf,"%.04x",stmp);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row16);
		memcpy(pStr + len,Row16,k);
		len += k;
		memcpy((unsigned char *)(&itmp),pPrint->bal,4);
		itmp = IntEndianConvert(itmp);
		k = HexToAscii(buf,itmp);
		memcpy(pStr + len,buf,k);
		len += k;
		k = strlen(YUAN);
		memcpy(pStr + len,YUAN,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row17);
		memcpy(pStr + len,Row17,k);
		len += k;
		k = strlen(Row17e[typ]);
		memcpy(pStr + len,Row17e[typ],k);
		len += k;
		*(pStr + len++) = '\n';
		
		if(typ == 0x0f)
		{
			k = strlen(Row18);
			memcpy(pStr + len,Row18,k);
			len += k;
			itmp = 0;
			memcpy((unsigned char *)(&itmp),&pPrint->rfu[6],4);
			itmp = IntEndianConvert(itmp);
			itmp >>= 8;
			k = HexToAscii(buf,itmp);
			memcpy(pStr + len,buf,k);
			len += k;
			k = strlen(YUAN);
			memcpy(pStr + len,YUAN,k);
			len += k;
			*(pStr + len++) = '\n';
		}
		
		k = strlen(Row19);
		memcpy(pStr + len,Row19,k);
		len += k;
		*(pStr + len++) = (pPrint->time[0] >> 4) + '0';
		*(pStr + len++) = (pPrint->time[0] & 0x0f) + '0';
		*(pStr + len++) = (pPrint->time[1] >> 4) + '0';
		*(pStr + len++) = (pPrint->time[1] & 0x0f) + '0';
		k = strlen(TRANS_YEAR);
		memcpy(pStr + len,TRANS_YEAR,k);
		len += k;
		*(pStr + len++) = (pPrint->time[2] >> 4) + '0';
		*(pStr + len++) = (pPrint->time[2] & 0x0f) + '0';
		k = strlen(TRANS_MONTH);
		memcpy(pStr + len,TRANS_MONTH,k);
		len += k;
		*(pStr + len++) = (pPrint->time[3] >> 4) + '0';
		*(pStr + len++) = (pPrint->time[3] & 0x0f) + '0';
		k = strlen(TRANS_DAY);
		memcpy(pStr + len,TRANS_DAY,k);
		len += k;
		*(pStr + len++) = (pPrint->time[4] >> 4) + '0';
		*(pStr + len++) = (pPrint->time[4] & 0x0f) + '0';
		*(pStr + len++) = ':';
		*(pStr + len++) = (pPrint->time[5] >> 4) + '0';
		*(pStr + len++) = (pPrint->time[5] & 0x0f) + '0';
		*(pStr + len++) = ':';
		*(pStr + len++) = (pPrint->time[6] >> 4) + '0';
		*(pStr + len++) = (pPrint->time[6] & 0x0f) + '0';
		*(pStr + len++) = '\n';
		
		k = strlen(Row20[i]);
		memcpy(pStr + len,Row20[i],k);
		len += k;
		
		memset(pStr + len,'\n',5);
		len += 5;
		*(pStr + len++) = 0x1b;
		*(pStr + len++) = 0x69;
		memset(pStr + len,'\n',5);
		len += 5;
	}
	
	DelPrintTask(pPrint);
	chCtrl[1].sendback = PrintComplete;
	printStruct.len = len;
	WRITE(USART3_ID,printStruct.buf,0,len);
}

#endif