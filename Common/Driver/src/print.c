#include "global.h"

//Douzhq 20140710 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
#include "CRMemCopy.h"
#endif

static const unsigned char prt0_dt[] = {0x1b,0x40};
static const unsigned char Ctrl1[] = {0x1b,0x21,0x10};
static const unsigned char Ctrl2[] = {0x1b,0x21,0x00};

static const char Row1[] = "欢迎使用中国石化加油IC卡";
//liuzg 20120201 start
#ifdef MAKE_CARD_MODE2
static const char Row2[] = "交易凭条";
static const char Row3[] = "油站编码:";
static const char Row4[] = "枪号:";
static const char Row5[] = "卡号:";
static const char Row6[] = "流水:";
static const char Row7[] = "扣前余额:";
static const char Row8[] = "油品:";
static const char Row9[] = "单价:";
static const char Row10[] = "升数:";
static const char Row11[] = "应付:";
static const char Row12[] = "扣卡";
static const char Row13[] = "联机:";
static const char Row14[] = "优惠:";
static const char Row15[] = "优惠规则:";
static const char Row16[] = "扣后余额:";
static const char Row17[] = "交易类型:";
static const char Row17e[][9] = 
{
	"正常",
	"逃卡",
	"错卡",
	"补扣",
	"补充",
	"上班",
	"下班",
	"非卡联动",
	"油价接收",
	"出错拒绝",
	"不祥",
	"在线异常",
	"非卡自助",
	"后台授权",
	"撤销授权",
	"过冲交易"
};
static const char Row18[] = "过冲金额:";
static const char Row19[] = "交易时间:";
static const char Row20[][32]=
{
	"本凭条不做报销凭证（第1联）",
	"本凭条不做报销凭证（第2联）"
};

static const char gcode1[] = "90#汽油";
static const char gcode2[] = "93#汽油";
static const char gcode3[] = "95#汽油";
static const char gcode4[] = "97#汽油";
static const char gcode5[] = "120#汽油";
static const char gcode6[] = "其他汽油1";
static const char gcode7[] = "98#汽油";
static const char gcode8[] = "其他汽油2";
static const char gcode9[] = "0#柴油";
static const char gcode10[] = "+5#柴油";
static const char gcode11[] = "+10#柴油";
static const char gcode12[] = "+15#柴油";
static const char gcode13[] = "+20#柴油";
static const char gcode14[] = "-5#柴油";
static const char gcode15[] = "-10#柴油";
static const char gcode16[] = "-15#柴油";
static const char gcode17[] = "-20#柴油";
static const char gcode18[] = "-30#柴油";
static const char gcode19[] = "-35#柴油";
static const char gcode20[] = "-50#柴油";
static const char gcode21[] = "轻柴油";
static const char gcode22[] = "其他柴油";

gcode_t gcode[] = 
{
	{0x101f,gcode1},
	{0x103f,gcode2},
	{0x104f,gcode3},
	{0x105f,gcode4},
	{0x106f,gcode5},
	{0x108f,gcode6},
	{0x109f,gcode7},
	{0x1fff,gcode8},
	{0x2001,gcode9},
	{0x2002,gcode10},
	{0x2003,gcode11},
	{0x2004,gcode12},
	{0x2005,gcode13},
	{0x2006,gcode14},
	{0x2007,gcode15},
	{0x2008,gcode16},
	{0x2009,gcode17},
	{0x2010,gcode18},
	{0x2011,gcode19},
	{0x2015,gcode20},
	{0x2016,gcode21},
	{0x2020,gcode21},
	{0x2fff,gcode22}
};
#else
static const char Row2[] = "卡的CTC:";
static const char Row3[] = "电子签名:";
static const char Row4[] = "解灰认证码:";
static const char Row5[] = "灰锁签名:";
static const char Row6[] = "PSAM应用号:";
static const char Row7[] = "PSAM编号:";
static const char Row8[] = "PSAM-TTC:";
static const char Row9[] = "扣款来源:";
static const char Row10[] = "支付类型:";
static const char Row11[] = "结算单位:";
static const char Row12[] = "卡类:";
static const char Row13[] = "卡密钥版本号:";
static const char Row14[] = "卡密钥索引号:";
static const char Row15[] = "员工号:";
static const char Row16[] = "升累计:";
static const char Row17[] = "终端数据认证码:";
static const char Row18[] = "加油站编码:";
static const char Row19[] = "枪号:";
static const char Row20[] = "卡号:";
static const char Row21[] = "流水号(POS-TTC):";
static const char Row22[] = "扣前金额:";
static const char Row23[] = "油品代码:";
static const char Row24[] = "单价:";
static const char Row25[] = "加油升数:";
static const char Row26[] = "加油金额:";
static const char Row27[] = "扣后余额:";
static const char Row28[] = "交易类型:";
static const char Row29[] = "交易时间:";


//Douzhq 20140707 Add , Save Oil 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
static const char Row9e[][9] = {"电子油票",
								"积分应用",
								"电子钱包",
								"电子存折",
								"存油应用"};
static const char Row10e[][9] = {	"现金",
									"油票",
									"提油凭证",
									"银行卡",
									"油",
									"其他二"};
static const char Row11e[][3] = {	"分",
									"点",
									"升"};
#else
static const char Row9e[][9] = {"电子油票",
								"积分应用",
								"电子钱包",
								"电子存折"};
static const char Row10e[][9] = {	"现金",
									"油票",
									"提油凭证",
									"银行卡",
									"其他一",
									"其他二"};
static const char Row11e[][3] = {	"分",
									"点"};
#endif

static const char Row28e[][9] = {	"正常",
									"逃卡",
									"错卡",
									"补扣",
									"补充",
									"上班",
									"下班",
									"非卡",
									"油价接收",
									"卡错拒绝"};

static const char Row30e[][20] = {	"不作报销凭证(第1联)",
									"不作报销凭证(第2联)"};



//=======================================; Bellow LLLLLO
//static const char CRow1[] = "  欢迎使用中石化现金支付系统";
//static const char CRow2[] = "交易类型:投币加油";
//static const char CRow3[] = "预付金额:";
//static const char CRow4[] = "余额:";
//static const char CRow5[] = "找零金额:";
//static const char CRow6[] = "小票当日有效,如有余额请立即到营业厅找零,过期本站概不负责.";
//=======================================; Upper LLLLLO

//static const char CrNumber[] = "CR终端号:";

//=======================================; Bellow LLLLLA
static const unsigned char Ctrl4[] = {0x1b,0x21,0x30};
static const char CRow1[] = " 欢迎自助加油";
static const char CRow4[] = "余额:";
static const char CRow5[] = "找零余额:";
//static const char CRow6[] = "小票当日有效,如有余额请立即到营业房找零,如有过期,本站概不负责";
//static const char CRow6[] = "如有余额请立即到营业室找零,并索取发票,该小票自交易时间24小时内有效,不作为报销凭证";
static const char CRow6[] = "如有余额,请凭此小票立即到营业室找零，并索取发票，小票自交易时间24小时内有效，该小票不作为报销凭证。";
static const char CRow7[] = "    该小票不作为报销凭证";
static const char P_CashAuthCode[] = "授权验证码:";
static const char P_CTTC[] = "交易序号:";
static const char P_CashAcceptorAmount[] = "投币金额:";
static const char P_CashPOSAmount[] = "缴费金额:";
static const char P_Reprint[] = "          重打凭证";
static const char C_GCODE[] = "加油油品:";
static const char GCODE_90[] = "90#汽油";
static const char GCODE_93[] = "93#汽油";
static const char GCODE_95[] = "95#汽油";
static const char GCODE_97[] = "97#汽油";
static const char GCODE_98[] = "98#汽油";
static const char GCODE_100[] = "100#汽油";
static const char GCODE_Chai[] = "柴油";
static const char GCODE_Qi[] = "汽油";
//=======================================; Upper LLLLLA
#endif

static const char VOL_C[] = "升";
static const char YUAN[] = "元";
static const char POINT[] = "点";
static const char UNITS[] = "元/升";

static const char TRANS_YEAR[] = "年";
static const char TRANS_MONTH[] = "月";
static const char TRANS_DAY[] = "日";

//liuzg 20120201 end


PrintBuf printStruct;

/* EasyCODE ] */
/* EasyCODE [ 
BcdToAscii 
function describe:bcd to ascii
param:d:dest pointer;s:source pointer,len:length of source data
return:real length
call fucntion:none
global variable:none
author:liuzg
date:2009-9-10
version:.01*/
/* EasyCODE F */
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

/* EasyCODE ] */
/* EasyCODE [ 
HexToAscii 
function describe:hex to ascii(add point)
param:s:pointer of data save,val:data
return:real length
call fucntion:sprintf
global variable:none
author:liuzg
date:2009-9-10
version:.01*/
/* EasyCODE F */
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

void InitPrint(void)
{
	unsigned char *pStr;
	
	pStr = printStruct.buf;
	memcpy(pStr,prt0_dt,2);
	chCtrl[1].sendback = PrintComplete;
	printStruct.len = 2;
	WRITE(USART3_ID,printStruct.buf,0,2);
}

/* EasyCODE ] */
/* EasyCODE [ 
print_dispose 
function describe:print data to dispose
param:none
return:none
call fucntion:BcdToAscii,HexToAscii,sprintf,memcpy,strlen
global variable:PrintTrans,printStruct
author:liuzg
date:2009-10-10
version:.01*/
/* EasyCODE F */
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

#ifdef MAKE_CARD_MODE2
unsigned char ReadG_Code(char *s,unsigned short g_code)
{
	char buf[32];
	unsigned int adr;
	unsigned short i,len,num,numlen;
	unsigned short gcd;
	At45 *pAt45;
	char tmpbuf[32];
	char flg = 0;
	
	pAt45 = &at45[0];
	adr = G_CODE_TICKET_ADR * AT45_PageSize(pAt45);
	READ(CONFIG_ID,(unsigned char*)buf,adr,16);
	memcpy((unsigned char *)(&num),buf,2);
	memcpy((unsigned char *)(&numlen),&buf[2],2);
	adr += 16;
	if((num == 0xffff) || (numlen > 32))
	{
		num = sizeof(gcode) / sizeof(gcode_t);
		for(i = 0; i < num; i++)
		{
			if((gcode[i].g_code & 0x0fff) == 0x0fff)
			{
				if((gcode[i].g_code & 0xf000) == (g_code & 0xf000))
				{
					len = strlen(gcode[i].gStr);
					memcpy(tmpbuf,gcode[i].gStr,len);
					flg = 1;
					break;
				}
			}
			else if((gcode[i].g_code & 0x00ff) == 0x00ff)
			{
				if((gcode[i].g_code & 0xff00) == (g_code & 0xff00))
				{
					len = strlen(gcode[i].gStr);
					memcpy(tmpbuf,gcode[i].gStr,len);
					flg = 1;
					break;
				}
			}
			else if((gcode[i].g_code & 0x000f) == 0x000f)
			{
				if((gcode[i].g_code & 0xfff0) == (g_code & 0xfff0))
				{
					len = strlen(gcode[i].gStr);
					memcpy(tmpbuf,gcode[i].gStr,len);
					flg = 1;
					break;
				}
			}
			else
			{
				if(gcode[i].g_code == g_code)
				{
					len = strlen(gcode[i].gStr);
					memcpy(tmpbuf,gcode[i].gStr,len);
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
			memcpy(tmpbuf,(unsigned char *)(&g_code),2);
			len = BcdToAscii(s,tmpbuf,2);
		}
		return(len);
	}
	for(i = 0; i < num; i++)
	{
		do
		{
			len = READ(CONFIG_ID,(unsigned char*)buf,adr,numlen);
		}
		while(len != numlen);
		memcpy((unsigned char *)(&gcd),buf,2);
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
//Printer
//Douzhq 20140612 Add , Save Oil 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
		else if (pPrint->ds == 0x04)
		{
			k = strlen(VOL_C);
			memcpy(pStr + len,VOL_C,k);			
		}
#endif
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
//Printer
//Douzhq 20140612 Add , Save Oil 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
		else if (pPrint->ds == 0x04)
		{
			k = strlen(VOL_C);
			memcpy(pStr + len,VOL_C,k);			
		}
#endif
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
				//Printer
//Douzhq 20140612 Add , Save Oil 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
		else if (pPrint->ds == 0x04)
		{
			k = strlen(VOL_C);
			memcpy(pStr + len,VOL_C,k);			
		}
#endif
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
#else
void print_dispose(void)
{
	unsigned char i,j,k,typ;
	unsigned short len;
	unsigned char *pStr;
	unsigned int itmp,itmp1;
	unsigned short stmp; 
	unsigned char ctmp;
	sPrintTrans *pPrint;
	char buf[20];
	char HeadBuf[64];
	unsigned char m;

	unsigned short int gcode;

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
	typ = pPrint->t_type & 0x0f;
	//memcpy(pStr,prt0_dt,2);
	len = 0;
	if((pPrint->typ & 0x0f) < 2)			//IC
	{
		if((pPrint->typ & 0x0f) == 1)
		{
			j = 2;
		}
		else
		{
			j = 1;
		}
		i = 2;
		memcpy(pStr + len,Ctrl1,sizeof(Ctrl1));
		len += sizeof(Ctrl1);
		
		ReadTicketHead(HeadBuf);
		k = strlen(HeadBuf);
		m = (32 - k) >> 1;
		memset(pStr + len,' ', m);
		len += m;
		memcpy(pStr + len,HeadBuf,k);
		len += k;
		/*k = strlen(Row1);
		memcpy(pStr + len,Row1,k);
		len += k; */
		memcpy(pStr + len,Ctrl2,sizeof(Ctrl2));
		len += sizeof(Ctrl2);
		memset(pStr + len,'\n',2);
		len += 2;
		
		memset(pStr + len, '-',29);
		len += 29;
		*(pStr + len++) = '\n';
		
		/*k = strlen(CrNumber);
		memcpy(pStr + len,CrNumber,k);
		len += k;
		if((pPrint->typ >> 4) < 0x0a)
		{
			*(pStr + len++) = (pPrint->typ >> 4) + '0';
		}
		else
		{
			*(pStr + len++) = (pPrint->typ >> 4) + 'A';
		}
		*(pStr + len++) = '\n';*/
		
		k = strlen(Row2);
		memcpy(pStr + len,Row2,k);
		len += k;
		memcpy((unsigned char *)(&stmp),pPrint->ctc,2);
		stmp = ShortEndianConvert(stmp);
		k = sprintf(buf,"%lu",stmp);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row3);
		memcpy(pStr + len,Row3,k);
		len += k;
		memcpy((unsigned char *)(&itmp),pPrint->tac,4);
		k = BcdToAscii(buf,(char *)(&itmp),4);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row4);
		memcpy(pStr + len,Row4,k);
		len += k;
		memcpy((unsigned char *)(&itmp),pPrint->gmac,4);
		k = BcdToAscii(buf,(char *)(&itmp),4);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row5);
		memcpy(pStr + len,Row5,k);
		len += k;
		memcpy((unsigned char *)(&itmp),pPrint->psam_tac,4);
		k = BcdToAscii(buf,(char *)(&itmp),4);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row6);
		memcpy(pStr + len,Row6,k);
		len += k;
		k = BcdToAscii(buf,pPrint->psam_asn,10);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row7);
		memcpy(pStr + len,Row7,k);
		len += k;
		k = BcdToAscii(buf,pPrint->psam_tid,6);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row8);
		memcpy(pStr + len,Row8,k);
		len += k;
		memcpy((unsigned char *)(&itmp),pPrint->psam_ttc,4);
		itmp = IntEndianConvert(itmp);
		k = sprintf(buf,"%lu",itmp);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row9);
		memcpy(pStr + len,Row9,k);
		len += k;
		if(typ < 5)
		{
			k = strlen(Row9e[pPrint->ds]);
			memcpy(pStr + len,Row9e[pPrint->ds],k);
			len += k;
		}
		*(pStr + len++) = '\n';
		
		k = strlen(Row10);
		memcpy(pStr + len,Row10,k);
		len += k;
		if(typ < 3)
		{
			k = strlen(Row10e[pPrint->unit >> 4]);
			memcpy(pStr + len,Row10e[pPrint->unit >> 4],k);
//Douzhq 20140707 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (pPrint->ds == 0x04)
			{
				k = strlen(Row10e[pPrint->ds]);
				memcpy(pStr + len,Row10e[pPrint->ds],k);
			}
#endif
			len += k;
		}
		*(pStr + len++) = '\n';
		
		k = strlen(Row11);
		memcpy(pStr + len,Row11,k);
		len += k;
		if(typ < 3)
		{
			k = strlen(Row11e[pPrint->unit & 0x01]);
			memcpy(pStr + len,Row11e[pPrint->unit & 0x01],k);
//Douzhq 20140707 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (pPrint->ds == 0x04)
			{
				k = strlen(Row11e[2]);
				memcpy(pStr + len,Row11e[2],k);
			}
#endif
			len += k;
		}
		*(pStr + len++) = '\n';
		
		k = strlen(Row12);
		memcpy(pStr + len,Row12,k);
		len += k;
		k = BcdToAscii(buf,&pPrint->c_type,1);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row13);
		memcpy(pStr + len,Row13,k);
		len += k;
		ctmp = pPrint->ver & 0x0f;
		k = sprintf(buf,"%lu",ctmp);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row14);
		memcpy(pStr + len,Row14,k);
		len += k;
		ctmp = pPrint->ver >> 4;
		k = sprintf(buf,"%lu",ctmp);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row15);
		memcpy(pStr + len,Row15,k);
		len += k;
		if(typ != 8)
		{
			ctmp = pPrint->emp;
			k = sprintf(buf,"%lu",ctmp);
			memcpy(pStr + len,buf,k);
			len += k;
		}
		*(pStr + len++) = '\n';
		
		k = strlen(Row16);
		memcpy(pStr + len,Row16,k);
		len += k;
		memcpy((unsigned char *)(&itmp),pPrint->v_tot,4);
		itmp = IntEndianConvert(itmp);
		k = HexToAscii(buf,itmp);
		memcpy(pStr + len,buf,k);
		len += k;
		k = strlen(VOL_C);
		memcpy(pStr + len,VOL_C,k);
		len += k;
		*(pStr + len++) = '\n';
		
		k = strlen(Row17);
		memcpy(pStr + len,Row17,k);
		len += k;
		k = BcdToAscii(buf,pPrint->t_mac,4);
		memcpy(pStr + len,buf,k);
		len += k;
		*(pStr + len++) = '\n';
		
		memset(pStr + len,'-',29);
		len += 29;
		*(pStr + len++) = '\n';
		
		while(j)
		{
			/*if(j == 2)
			{
				memcpy(pStr + len,Ctrl1,sizeof(Ctrl1));
				len += sizeof(Ctrl1);
				k = strlen(Row1);
				memcpy(pStr + len,Row1,k);
				len += k;
				memcpy(pStr + len,Ctrl2,sizeof(Ctrl2));
				len += sizeof(Ctrl2);
				memset(pStr + len,'\n',2);
				len += 2;
				
				memset(pStr + len, '-',29);
				len += 29;
				*(pStr + len++) = '\n';
			}*/
			
//Douzhq 20140710 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (pPrint->rfu[2] > 0 && pPrint->rfu[2] < 8)
			{
				//Do Any Thing
			}
			else
			{
				k = strlen(Row18);
				memcpy(pStr + len,Row18,k);
				len += k;
				k = BcdToAscii(buf,pPrint->s_id,4);
				memcpy(pStr + len,buf,k);
				len += k;
				*(pStr + len++) = '\n';
			}
#else
			k = strlen(Row18);
			memcpy(pStr + len,Row18,k);
			len += k;
			k = BcdToAscii(buf,pPrint->s_id,4);
			memcpy(pStr + len,buf,k);
			len += k;
			*(pStr + len++) = '\n';
#endif
			
			k = strlen(Row19);
			memcpy(pStr + len,Row19,k);
			len += k;
			k = sprintf(buf,"%lu",pPrint->nzn);
			memcpy(pStr + len,buf,k);
			len += k;
			*(pStr + len++) = '\n';
			
			k = strlen(Row20);
			memcpy(pStr + len,Row20,k);
			len += k;
			k = BcdToAscii(buf,pPrint->asn,10);
			memcpy(pStr + len,buf,k);
			len += k;
			*(pStr + len++) = '\n';
			
//Douzhq 20140710 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (pPrint->rfu[2] > 0 && pPrint->rfu[2] < 8)
			{
				// Do Any Thing				
			}
			else
			{
				k = strlen(Row21);
				memcpy(pStr + len,Row21,k);
				len += k;
				memcpy((unsigned char *)(&itmp),pPrint->pos_ttc,4);
				itmp = IntEndianConvert(itmp);
				k = sprintf(buf,"%lu",itmp);
				memcpy(pStr + len,buf,k);
				len += k;
				*(pStr + len++) = '\n';
			}
#else
			k = strlen(Row21);
			memcpy(pStr + len,Row21,k);
			len += k;
			memcpy((unsigned char *)(&itmp),pPrint->pos_ttc,4);
			itmp = IntEndianConvert(itmp);
			k = sprintf(buf,"%lu",itmp);
			memcpy(pStr + len,buf,k);
			len += k;
			*(pStr + len++) = '\n';
#endif
			
			k = strlen(Row22);
			memcpy(pStr + len,Row22,k);
//Douzhq 20140707 Add , Save Oil 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (pPrint->ds == 0x04)
			{
				k = strlen("扣前油额:");
				memcpy(pStr + len,"扣前油额:",k);
			}
#endif
			len += k;
			if(typ < 5)
			{
				memcpy((unsigned char *)(&itmp),pPrint->bal,4);
				itmp = IntEndianConvert(itmp);
				if(typ != 1)
				{
					itmp1 = 0;
					memcpy((unsigned char *)(&itmp1),pPrint->amn,3);
					
//Douzhq 20140707 Add , Save Oil 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
				if (pPrint->ds == 0x04)
				{
					memcpy((unsigned char *)(&itmp1),pPrint->amn,3);
				}
#endif
					itmp1 = IntEndianConvert(itmp1);
					itmp1 >>= 8;
					itmp += itmp1;
				}
				k = HexToAscii(buf,itmp);
				memcpy(pStr + len,buf,k);
				len += k;
				//if(pPrint->unit & 0x01)
				if(pPrint->ds == 1)
				{
					k = strlen(POINT);
					memcpy(pStr + len,POINT,k);
				}
				//Printer
//Douzhq 20140612 Add , Save Oil 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
				else if (pPrint->ds == 0x04)
				{
					k = strlen(VOL_C);
					memcpy(pStr + len,VOL_C,k);			
				}
#endif
				else
				{
					k = strlen(YUAN);
					memcpy(pStr + len,YUAN,k);
				}
				len += k;
			}
			*(pStr + len++) = '\n';
			
			k = strlen(Row23);
			memcpy(pStr + len,Row23,k);
			len += k;
			/*if((typ < 3) || ((typ > 4) && (j < 8)))
			{
				k = BcdToAscii(buf,pPrint->g_code,2);
				memcpy(pStr + len,buf,k);
				len += k;
			}*/
			k = BcdToAscii(buf,pPrint->g_code,2);
			memcpy(pStr + len,buf,k);
			len += k;
			*(pStr + len++) = '\n';
			
			k = strlen(Row24);
			memcpy(pStr + len,Row24,k);
			len += k;
			/*if((typ < 3) || (typ == 7))
			{
				memcpy((unsigned char *)(&stmp),pPrint->prc,2);
				stmp = ShortEndianConvert(stmp);
				k = HexToAscii(buf,stmp);
				memcpy(pStr + len,buf,k);
				len += k;
				k = strlen(UNITS);
				memcpy(pStr + len,UNITS,k);
				len += k;
			}*/
			memcpy((unsigned char *)(&stmp),pPrint->prc,2);
			stmp = ShortEndianConvert(stmp);
			k = HexToAscii(buf,stmp);
			memcpy(pStr + len,buf,k);
			len += k;
			k = strlen(UNITS);
			memcpy(pStr + len,UNITS,k);
			len += k;
			*(pStr + len++) = '\n';
			
			k = strlen(Row25);
			memcpy(pStr + len,Row25,k);
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
			/*if((typ < 3) || (typ == 7))
			{
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
			}*/
			*(pStr + len++) = '\n';

//Douzhq 20140710 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (pPrint->rfu[2] > 0 && pPrint->rfu[2] < 8)
			{
				k = strlen("加油金额:");
				memcpy(pStr + len,"加油金额:",k);
				len += k;
				
				memcpy((unsigned char *)(&itmp1),pPrint->vol,3);
				itmp1 = IntEndianConvert(itmp1);
				itmp1 >>= 8;
				
				stmp = CharToShortIntCopy((unsigned char *)(pPrint->rfu));
				itmp = itmp * stmp;
				
				if ((itmp % 100 / 10) > 4)
				{
					itmp /= 100;
					itmp++;
				}
				else
				{
					itmp /= 100;
				}
				
				k = HexToAscii(buf,itmp);
				memcpy(pStr + len,buf,k);
				len += k;
				
				*(pStr + len++) = '\n';
			}
#endif
			k = strlen(Row26);
			memcpy(pStr + len,Row26,k);
			
			//Douzhq 20140710 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (pPrint->rfu[2] > 0 && pPrint->rfu[2] < 8)
			{
				k = strlen("实扣金额:");				
				memcpy(pStr + len,"实扣金额:",k);
			}
#endif
			
			len += k;
			

			/*if(typ < 5)
			{
				itmp = 0;
				memcpy((unsigned char *)(&itmp),pPrint->amn,3);
				itmp = IntEndianConvert(itmp);
				itmp >>= 8;
				k = HexToAscii(buf,itmp);
				memcpy(pStr + len,buf,k);
				len += k;
				if(pPrint->unit & 0x01)
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
			}*/
			itmp = 0;
			memcpy((unsigned char *)(&itmp),pPrint->amn,3);
			itmp = IntEndianConvert(itmp);
			itmp >>= 8;
			k = HexToAscii(buf,itmp);
			memcpy(pStr + len,buf,k);
			len += k;
			//if(pPrint->unit & 0x01)
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

//Douzhq 20140710 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (pPrint->rfu[2] > 0 && pPrint->rfu[2] < 8)
			{
				k = strlen("优惠金额:");				
				memcpy(pStr + len,"优惠金额:",k);
				len += k;
				
				memcpy((unsigned char *)(&itmp1),pPrint->vol,3);
				itmp1 = IntEndianConvert(itmp1);
				itmp1 >>= 8;
				
				stmp = CharToShortIntCopy((unsigned char *)(pPrint->rfu));
				itmp = itmp1 * stmp;
				if ((itmp % 100 / 10) > 4)
				{
					itmp /= 100;
					itmp++;
				}
				else
				{
					itmp /= 100;
				}
				
				memcpy((unsigned char *)(&itmp1),pPrint->amn,3);
				itmp1 = IntEndianConvert(itmp1);
				itmp1 >>= 8;
				if (itmp >= itmp1)
				{
					itmp -= itmp1;
				}
				else
				{
					itmp = 0;		
				}
				
				k = HexToAscii(buf,itmp);
				memcpy(pStr + len,buf,k);
				len += k;
				
				*(pStr + len++) = '\n';
			}
#endif
			
			k = strlen(Row27);
			memcpy(pStr + len,Row27,k);
			
//Douzhq 20140707 Add , Save Oil 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (pPrint->ds == 0x04)
			{
				k = strlen("扣后油额:");
				memcpy(pStr + len,"扣后油额:",k);
			}
#endif
			len += k;
			/*if(typ < 7)
			{
				memcpy((unsigned char *)(&itmp),pPrint->bal,4);
				itmp = IntEndianConvert(itmp);
				k = HexToAscii(buf,itmp);
				memcpy(pStr + len,buf,k);
				len += k;
				if(pPrint->unit & 0x01)
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
			}*/
			memcpy((unsigned char *)(&itmp),pPrint->bal,4);
			itmp = IntEndianConvert(itmp);
			k = HexToAscii(buf,itmp);
			memcpy(pStr + len,buf,k);
			len += k;
			//if(pPrint->unit & 0x01)
			if(pPrint->ds == 1)
			{
				k = strlen(POINT);
				memcpy(pStr + len,POINT,k);
			}
			//Printer
//Douzhq 20140612 Add , Save Oil 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			else if (pPrint->ds == 0x04)
			{
				k = strlen(VOL_C);
				memcpy(pStr + len,VOL_C,k);			
			}
#endif
			else
			{
				k = strlen(YUAN);
				memcpy(pStr + len,YUAN,k);
			}
			len += k;
			*(pStr + len++) = '\n';
			
			k = strlen(Row28);
			memcpy(pStr + len,Row28,k);
			len += k;
			k = strlen(Row28e[typ]);
			memcpy(pStr + len,Row28e[typ],k);
			len += k;
			*(pStr + len++) = '\n';
			
			k = strlen(Row29);
			memcpy(pStr + len,Row29,k);
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
			
			memset(pStr + len,'-',29);
			len += 29;
			*(pStr + len++) = '\n';
			
			memset(pStr + len,' ', 4);
			len += 4;
			k = strlen(Row30e[2 - i]);
			memcpy(pStr + len,Row30e[2 - i],k);
			len += k;
			
			memset(pStr + len,'\n',5);
			len += 5;
			*(pStr + len++) = 0x1b;
			*(pStr + len++) = 0x69;
			memset(pStr + len,'\n',5);
			len += 5;
			j--;
			i--;
			if(j)
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
				/*k = strlen(Row1);
				memcpy(pStr + len,Row1,k);
				len += k;*/
				memcpy(pStr + len,Ctrl2,sizeof(Ctrl2));
				len += sizeof(Ctrl2);
				memset(pStr + len,'\n',2);
				len += 2;
				
				memset(pStr + len, '-',29);
				len += 29;
				*(pStr + len++) = '\n';
			}
		}
	}
	else			//CASH
	{
//=======================================; Bellow LLLLLO
//		if((pPrint->typ & 0x0f) == 4)
//		{
//			j = 2;
//		}
//		else
//		{
//			j = 1;
//		}
//		i = 2;
//		memcpy(pStr + len,Ctrl1,sizeof(Ctrl1));
//		len += sizeof(Ctrl1);
//		
//		k = strlen(CRow1);
//		memcpy(pStr + len,CRow1,k);
//		len += k;
//		
//		memcpy(pStr + len,Ctrl2,sizeof(Ctrl2));
//		len += sizeof(Ctrl2);
//		memset(pStr + len,'\n',2);
//		len += 2;
//		memset(pStr + len,'-',29);
//		len += 29;
//		*(pStr + len++) = '\n';
//		
//		k = strlen(Row18);
//		memcpy(pStr + len,Row18,k);
//		len += k;
//		k = BcdToAscii(buf,pPrint->s_id,4);
//		memcpy(pStr + len,buf,k);
//		len += k;
//		*(pStr + len++) = '\n';
//		
//		k = strlen(Row21);
//		memcpy(pStr + len,Row21,k);
//		len += k;
//		memcpy((unsigned char *)(&itmp),pPrint->pos_ttc,4);
//		itmp = IntEndianConvert(itmp);
//		k = sprintf(buf,"%lu",itmp);
//		memcpy(pStr + len,buf,k);
//		len += k;
//		*(pStr + len++) = '\n';
//		
//		k = strlen(Row19);
//		memcpy(pStr + len,Row19,k);
//		len += k;
//		k = sprintf(buf,"%lu",pPrint->nzn);
//		memcpy(pStr + len,buf,k);
//		len += k;
//		*(pStr + len++) = '\n';
//		
//		k = strlen(CRow2);
//		memcpy(pStr + len,CRow2,k);
//		len += k;
//		*(pStr + len++) = '\n';
//		
//		k = strlen(Row23);
//		memcpy(pStr + len,Row23,k);
//		len += k;
//		/*if((typ < 3) || ((typ > 4) && (j < 8)))
//		{
//			k = BcdToAscii(buf,pPrint->g_code,2);
//			memcpy(pStr + len,buf,k);
//			len += k;
//		}*/
//		k = BcdToAscii(buf,pPrint->g_code,2);
//		memcpy(pStr + len,buf,k);
//		len += k;
//		*(pStr + len++) = '\n';
//		
//		k = strlen(Row24);
//		memcpy(pStr + len,Row24,k);
//		len += k;
//		/*if((typ < 3) || (typ == 7))
//		{
//			memcpy((unsigned char *)(&stmp),pPrint->prc,2);
//			stmp = ShortEndianConvert(stmp);
//			k = HexToAscii(buf,stmp);
//			memcpy(pStr + len,buf,k);
//			len += k;
//			k = strlen(UNITS);
//			memcpy(pStr + len,UNITS,k);
//			len += k;
//		}*/
//		memcpy((unsigned char *)(&stmp),pPrint->prc,2);
//		stmp = ShortEndianConvert(stmp);
//		k = HexToAscii(buf,stmp);
//		memcpy(pStr + len,buf,k);
//		len += k;
//		k = strlen(UNITS);
//		memcpy(pStr + len,UNITS,k);
//		len += k;
//		*(pStr + len++) = '\n';
//		
//		k = strlen(CRow3);
//		memcpy(pStr + len,CRow3,k);
//		len += k;
//		memcpy((unsigned char *)(&itmp),pPrint->rfu,4);
//		itmp = IntEndianConvert(itmp);
//		k = HexToAscii(buf,itmp);
//		memcpy(pStr + len,buf,k);
//		len += k;
//		k = strlen(YUAN);
//		memcpy(pStr + len,YUAN,k);
//		len += k;
//		*(pStr + len++) = '\n';
//		
//		k = strlen(Row25);
//		memcpy(pStr + len,Row25,k);
//		len += k;
//		/*if((typ < 3) || (typ == 7))
//		{
//			itmp = 0;
//			memcpy((unsigned char *)(&itmp),pPrint->vol,3);
//			itmp = IntEndianConvert(itmp);
//			itmp >>= 8;
//			k = HexToAscii(buf,itmp);
//			memcpy(pStr + len,buf,k);
//			len += k;
//			k = strlen(VOL_C);
//			memcpy(pStr + len,VOL_C,k);
//			len += k;
//		}*/
//		itmp = 0;
//		memcpy((unsigned char *)(&itmp),pPrint->vol,3);
//		itmp = IntEndianConvert(itmp);
//		itmp >>= 8;
//		k = HexToAscii(buf,itmp);
//		memcpy(pStr + len,buf,k);
//		len += k;
//		k = strlen(VOL_C);
//		memcpy(pStr + len,VOL_C,k);
//		len += k;
//		*(pStr + len++) = '\n';
//		
//		k = strlen(Row26);
//		memcpy(pStr + len,Row26,k);
//		len += k;
//		/*if(typ < 5)
//		{
//			itmp = 0;
//			memcpy((unsigned char *)(&itmp),pPrint->amn,3);
//			itmp = IntEndianConvert(itmp);
//			itmp >>= 8;
//			k = HexToAscii(buf,itmp);
//			memcpy(pStr + len,buf,k);
//			len += k;
//			if(pPrint->unit & 0x01)
//			{
//				k = strlen(POINT);
//				memcpy(pStr + len,POINT,k);
//			}
//			else
//			{
//				k = strlen(YUAN);
//				memcpy(pStr + len,YUAN,k);
//			}
//			len += k;
//		}*/
//		itmp = 0;
//		memcpy((unsigned char *)(&itmp),pPrint->amn,3);
//		itmp = IntEndianConvert(itmp);
//		itmp >>= 8;
//		k = HexToAscii(buf,itmp);
//		memcpy(pStr + len,buf,k);
//		len += k;
//		//if(pPrint->unit & 0x01)
//		if(pPrint->ds == 1)
//		{
//			k = strlen(POINT);
//			memcpy(pStr + len,POINT,k);
//		}
//		else
//		{
//			k = strlen(YUAN);
//			memcpy(pStr + len,YUAN,k);
//		}
//		len += k;
//		*(pStr + len++) = '\n';
//		
//		k = strlen(CRow4);
//		memcpy(pStr + len,CRow4,k);
//		len += k;
//		if(typ < 7)
//		{
//			memcpy((unsigned char *)(&itmp),pPrint->bal,4);
//			itmp = IntEndianConvert(itmp);
//			k = HexToAscii(buf,itmp);
//			memcpy(pStr + len,buf,k);
//			len += k;
//			//if(pPrint->unit & 0x01)
//			if(pPrint->ds == 1)
//			{
//				k = strlen(POINT);
//				memcpy(pStr + len,POINT,k);
//			}
//			else
//			{
//				k = strlen(YUAN);
//				memcpy(pStr + len,YUAN,k);
//			}
//			len += k;
//		}
//		*(pStr + len++) = '\n';
//		
//		k = strlen(CRow5);
//		memcpy(pStr + len,CRow5,k);
//		len += k;
//		memcpy((unsigned char *)(&itmp),&pPrint->rfu[4],4);
//		itmp = IntEndianConvert(itmp);
//		k = HexToAscii(buf,itmp);
//		memcpy(pStr + len,buf,k);
//		len += k;
//		k = strlen(YUAN);
//		memcpy(pStr + len,YUAN,k);
//		len += k;
//		*(pStr + len++) = '\n';
//		
//		k = strlen(Row29);
//		memcpy(pStr + len,Row29,k);
//		len += k;
//		*(pStr + len++) = (pPrint->time[0] >> 4) + '0';
//		*(pStr + len++) = (pPrint->time[0] & 0x0f) + '0';
//		*(pStr + len++) = (pPrint->time[1] >> 4) + '0';
//		*(pStr + len++) = (pPrint->time[1] & 0x0f) + '0';
//		k = strlen(TRANS_YEAR);
//		memcpy(pStr + len,TRANS_YEAR,k);
//		len += k;
//		*(pStr + len++) = (pPrint->time[2] >> 4) + '0';
//		*(pStr + len++) = (pPrint->time[2] & 0x0f) + '0';
//		k = strlen(TRANS_MONTH);
//		memcpy(pStr + len,TRANS_MONTH,k);
//		len += k;
//		*(pStr + len++) = (pPrint->time[3] >> 4) + '0';
//		*(pStr + len++) = (pPrint->time[3] & 0x0f) + '0';
//		k = strlen(TRANS_DAY);
//		memcpy(pStr + len,TRANS_DAY,k);
//		len += k;
//		*(pStr + len++) = (pPrint->time[4] >> 4) + '0';
//		*(pStr + len++) = (pPrint->time[4] & 0x0f) + '0';
//		*(pStr + len++) = ':';
//		*(pStr + len++) = (pPrint->time[5] >> 4) + '0';
//		*(pStr + len++) = (pPrint->time[5] & 0x0f) + '0';
//		*(pStr + len++) = ':';
//		*(pStr + len++) = (pPrint->time[6] >> 4) + '0';
//		*(pStr + len++) = (pPrint->time[6] & 0x0f) + '0';
//		*(pStr + len++) = '\n';
//		
//		memset(pStr + len,'-',29);
//		len += 29;
//		*(pStr + len++) = '\n';
//		
//		k = strlen(CRow6);
//		memcpy(pStr + len,CRow6,k);
//		len += k;
//		*(pStr + len++) = '\n';
//		memset(pStr + len,'-',29);
//		len += 29;
//		*(pStr + len++) = '\n';
//		memset(pStr + len,' ',3);
//		len += 3;
//		
//		k = strlen(Row30e[0]);
//		memcpy(pStr + len,Row30e,k);
//		len += k;
//		memset(pStr + len,'\n',5);
//		len += 5;
//		*(pStr + len++) = 0x1b;
//		*(pStr + len++) = 0x69;
//		memset(pStr + len,'\n',5);
//		len += 5;
//=======================================; Upper LLLLLO

//=======================================; Bellow LLLLLA
		if((pPrint->typ & 0x0f) <= 5)
		{
			j = ( pPrint->typ & 0x0f );

			i = 2;
			memcpy(pStr + len,Ctrl4,sizeof(Ctrl4));
			len += sizeof(Ctrl4);
			
			memset(pStr + len,'\n',2);
			len += 2;
			
			k = strlen(CRow1);
			memcpy(pStr + len,CRow1,k);
			len += k;

			memcpy(pStr + len,Ctrl2,sizeof(Ctrl2));
			len += sizeof(Ctrl2);
			memset(pStr + len,'\n',2);
			len += 2;

			memset(pStr + len,'-',29);
			len += 29;
			*(pStr + len++) = '\n';
			
			if(( j == 4 ) || ( j == 5 ))
			{
				k = strlen( P_CashAuthCode );
				memcpy( pStr + len, P_CashAuthCode, k );
				len += k;
//				if( pPrint->rfu[4] > 6 )
//				{
//					// Max Is 6
//					pPrint->rfu[4] = 6;
//				}
//				memcpy( pStr + len, pPrint->rfu + 5, pPrint->rfu[4] );
//				len += pPrint->rfu[4];
//				*(pStr + len++) = '\n';

				k = BcdToAscii( buf, pPrint->rfu + 5, 6 );
				// Do Not Print The Leading And Ending Bytes
				memcpy( pStr + len, &buf[1], (unsigned char)(pPrint->rfu[4] - 2) );
				len += pPrint->rfu[4] - 2;
				*(pStr + len++) = '\n';
			}

			k = strlen(Row18);
			memcpy(pStr + len,Row18,k);
			len += k;
			k = BcdToAscii(buf,pPrint->s_id,4);
			memcpy(pStr + len,buf,k);
			len += k;
			*(pStr + len++) = '\n';
			
			k = strlen(Row19);
			memcpy(pStr + len,Row19,k);
			len += k;
			k = sprintf(buf,"%lu",pPrint->nzn);
			memcpy(pStr + len,buf,k);
			len += k;
			*(pStr + len++) = '\n';
			
			k = strlen(P_CTTC);
			memcpy(pStr + len,P_CTTC,k);
			len += k;
			memcpy((unsigned char *)(&itmp),pPrint->pos_ttc,4);
			itmp = IntEndianConvert(itmp);
			k = sprintf(buf,"%lu",itmp);
			memcpy(pStr + len,buf,k);
			len += k;
			*(pStr + len++) = '\n';
			
//			k = strlen(CRow2);
//			memcpy(pStr + len,CRow2,k);
//			len += k;
//			*(pStr + len++) = '\n';
//			
			k = strlen( C_GCODE );
			memcpy( pStr + len, C_GCODE, k );
			len += k;

			memcpy(( unsigned char * )( &stmp ), pPrint->g_code, 2 );
			stmp = ShortEndianConvert( stmp );
			gcode = stmp;
			stmp &= 0xFFF0;
			
			if( stmp == 0x1010 )
			{
				k = strlen( GCODE_90 );
				memcpy( pStr + len, GCODE_90, k );
				len += k;
			}
			else if( stmp == 0x1030 )
			{
				k = strlen( GCODE_93 );
				memcpy( pStr + len, GCODE_93, k );
				len += k;
			}
			else if( stmp == 0x1040 )
			{
				k = strlen( GCODE_95 );
				memcpy( pStr + len, GCODE_95, k );
				len += k;
			}
			else if( stmp == 0x1050 )
			{
				k = strlen( GCODE_97 );
				memcpy( pStr + len, GCODE_97, k );
				len += k;
			}
			else if( stmp == 0x1090 )
			{
				k = strlen( GCODE_98 );
				memcpy( pStr + len, GCODE_98, k );
				len += k;
			}
			else if( gcode == 0x1203 )
			{
				k = strlen( GCODE_100 );
				memcpy( pStr + len, GCODE_100, k );
				len += k;
			}
			else if( gcode == 0x2000 )
			{
				k = strlen( GCODE_90 );
				memcpy( pStr + len, GCODE_Chai, k );
				len += k;
			}
			else if( gcode == 0x1000 )
			{
				k = strlen( GCODE_Qi );
				memcpy( pStr + len, GCODE_Qi, k );
				len += k;
			}
			else
			{
				k = BcdToAscii(buf,pPrint->g_code,2);
				memcpy(pStr + len,buf,k);
				len += k;
			}

			*(pStr + len++) = '\n';



			k = strlen(Row24);
			memcpy(pStr + len,Row24,k);
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
			
//			k = strlen(CRow3);
//			memcpy(pStr + len,CRow3,k);
//			len += k;
//			memcpy((unsigned char *)(&itmp),pPrint->rfu,4);
//			itmp = IntEndianConvert(itmp);
//			k = HexToAscii(buf,itmp);
//			memcpy(pStr + len,buf,k);
//			len += k;
//			k = strlen(YUAN);
//			memcpy(pStr + len,YUAN,k);
//			len += k;
//			*(pStr + len++) = '\n';
			
			k = strlen(Row25);
			memcpy(pStr + len,Row25,k);
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
			
			k = strlen(Row26);
			memcpy(pStr + len,Row26,k);
			len += k;

			itmp = 0;
			memcpy((unsigned char *)(&itmp),pPrint->amn,3);
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
//Printer
//Douzhq 20140612 Add , Save Oil 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			else if (pPrint->ds == 0x04)
			{
				k = strlen(VOL_C);
				memcpy(pStr + len,VOL_C,k);			
			}
#endif
			else
			{
				k = strlen(YUAN);
				memcpy(pStr + len,YUAN,k);
			}
			len += k;
			*(pStr + len++) = '\n';
			
//			k = strlen(CRow4);
//			memcpy(pStr + len,CRow4,k);
//			len += k;
//			if(typ < 7)
//			{
//				memcpy((unsigned char *)(&itmp),pPrint->bal,4);
//				itmp = IntEndianConvert(itmp);
//				k = HexToAscii(buf,itmp);
//				memcpy(pStr + len,buf,k);
//				len += k;
//				//if(pPrint->unit & 0x01)
//				if(pPrint->ds == 1)
//				{
//					k = strlen(POINT);
//					memcpy(pStr + len,POINT,k);
//				}
//				else
//				{
//					k = strlen(YUAN);
//					memcpy(pStr + len,YUAN,k);
//				}
//				len += k;
//			}
//			*(pStr + len++) = '\n';
			
			if(( j == 4 ) || ( j == 5 ))
			{
				k = strlen( P_CashPOSAmount );
				memcpy( pStr + len, P_CashPOSAmount, k );
			}
			else
			{
				k = strlen( P_CashAcceptorAmount );
				memcpy( pStr + len, P_CashAcceptorAmount, k );
			}

			len += k;

			memcpy(( unsigned char * )( &itmp ), pPrint->rfu, 4 );
			itmp = IntEndianConvert( itmp );
			k = HexToAscii( buf, itmp );
			memcpy( pStr + len, buf, k );
			len += k;
			k = strlen( YUAN );
			memcpy( pStr + len, YUAN, k );
			len += k;
			*(pStr + len++) = '\n';

			if(( j == 4 ) || ( j == 5 ))
			{
				k = strlen( CRow4 );
				memcpy( pStr + len, CRow4, k );
			}
			else
			{
				k = strlen( CRow5 );
				memcpy( pStr + len, CRow5, k );
			}
			len += k;

			itmp = 0;
			itmp1 = 0;
			memcpy(( unsigned char * )( &itmp ), pPrint->rfu, 4 );
			memcpy(( unsigned char * )( &itmp1 ), pPrint->amn, 3 );
			itmp = IntEndianConvert( itmp );
			itmp1 = IntEndianConvert( itmp1 );
			itmp1 >>= 8;
			if( itmp >= itmp1 )
			{
				itmp -= itmp1;
			}
			else
			{
				itmp = 0;
			}
			k = HexToAscii( buf, itmp );
			memcpy( pStr + len, buf, k );
			len += k;
			k = strlen( YUAN );
			memcpy( pStr + len, YUAN, k );
			len += k;
			*(pStr + len++) = '\n';
			


			k = strlen(Row29);
			memcpy(pStr + len,Row29,k);
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
			
			memset(pStr + len,'-',29);
			len += 29;
			*(pStr + len++) = '\n';
			
			if(( j == 2 ) || ( j == 4 ))
			{
				*(pStr + len++) = '\n';
				k = strlen( CRow7 );
				memcpy( pStr + len, CRow7, k );
				len += k;
				*(pStr + len++) = '\n';

				*(pStr + len++) = '\n';
				k = strlen( P_Reprint );
				memcpy( pStr + len, P_Reprint, k );
				len += k;
				*(pStr + len++) = '\n';
			}
			else
			{
				k = strlen(CRow6);
				memcpy(pStr + len,CRow6,k);
				len += k;
				*(pStr + len++) = '\n';
			}
			
			memset(pStr + len,'-',29);
			len += 29;
			*(pStr + len++) = '\n';
			memset(pStr + len,' ',3);
			len += 3;
			


//			k = strlen(Row30e[0]);
//			memcpy(pStr + len,Row30e,k);
//			len += k;
			memset(pStr + len,'\n',5);
			len += 5;
			
			
			
			
			
			*(pStr + len++) = 0x1b;
			*(pStr + len++) = 0x69;
			memset(pStr + len,'\n',5);
			len += 5;

		}

//=======================================; Upper LLLLLA

	}
	
	DelPrintTask(pPrint);
	chCtrl[1].sendback = PrintComplete;
	printStruct.len = len;
	WRITE(USART3_ID,printStruct.buf,0,len);
}
#endif

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