#include "global.h"
//Douzhq 20141023 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
#include "CRMemCopy.h"
#endif

#if POS_TYPE==1
static const unsigned char Ctrl1[] = {0x1b,0x21,0x10};
static const unsigned char Ctrl2[] = {0x1b,0x21,0x00};

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

static const char VOL_C[] = "升";
static const char YUAN[] = "元";
static const char POINT[] = "点";
static const char UNITS[] = "元/升";

static const char TRANS_YEAR[] = "年";
static const char TRANS_MONTH[] = "月";
static const char TRANS_DAY[] = "日";

static const unsigned char Ctrl4[] = {0x1b,0x21,0x30};
static const char CRow1[] = " 欢迎自助加油";
static const char CRow4[] = "余额:";
static const char CRow5[] = "找零余额:";
static const char CRow6[] = "如有余额,请凭此小票立即到营业室找零，并索取发票，小票自交易时间24小时内有效，该小票不作为报销凭证.";
static const char CRow7[] = "    该小票不作为报销凭证";
static const char P_CashAuthCode[] = "验证码:";
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

PrintBuf printStruct;

unsigned char CashAuthCodeConvert(char* d,char* s)
{
	unsigned char i;
	unsigned char len = 0;
	unsigned char tmp;
	
	for(i = 0; i < 5; ++i)
	{
		tmp = (*s) >> 4;
		if(tmp == 0x0f)
		{
			break;
		}
		if(tmp > 9)
		{
			tmp = tmp - 0x0a + 'A';
		}
		else
		{
			tmp += '0';
		}
		*(d++) = tmp;
		++len;
		tmp = (*s) & 0x0f;
		if(tmp == 0x0f)
		{
			break;
		}
		if(tmp > 9)
		{
			tmp = tmp - 0x0a + 'A';
		}
		else
		{
			tmp += '0';
		}
		*(d++) = tmp;
		++s;
		++len;
	}
	return(len);
}

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
	unsigned char print_typ = 1;
	
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
	if(((pPrint->typ & 0x0f) < 2) || ((pPrint->typ & 0x0f) == 6))
	{					//IC
		if((pPrint->typ & 0x0f) == 1)
		{
			j = 2;
		}
		else
		{
			j = 1;
		}
		if((pPrint->typ & 0x0f) == 6) {
			print_typ = 0;
		}
		i = 2;
		memcpy(pStr + len,Ctrl1,sizeof(Ctrl1));
		len += sizeof(Ctrl1);
		
		if(print_typ) {
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
		
			memset(pStr + len, '-',29);
			len += 29;
			*(pStr + len++) = '\n';
		
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
		}
		
		while(j)
		{
			if(!print_typ) {
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
			
				memset(pStr + len, '-',29);
				len += 29;
				*(pStr + len++) = '\n';
			}
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
			
			k = BcdToAscii(buf,pPrint->g_code,2);
			memcpy(pStr + len,buf,k);
			len += k;
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
			
			memcpy((unsigned char *)(&itmp),pPrint->bal,4);
			itmp = IntEndianConvert(itmp);
			k = HexToAscii(buf,itmp);
			memcpy(pStr + len,buf,k);
			len += k;
			if(pPrint->ds == 1)
			{
				k = strlen(POINT);
				memcpy(pStr + len,POINT,k);
			}
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
			
			if(print_typ) {
				memset(pStr + len,' ', 4);
				len += 4;
				k = strlen(Row30e[2 - i]);
				memcpy(pStr + len,Row30e[2 - i],k);
				len += k;
			}
			
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
				memcpy( pStr + len, &buf[1], pPrint->rfu[4] - 2 );
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
	
			memset(pStr + len,'\n',5);
			len += 5;
			
			*(pStr + len++) = 0x1b;
			*(pStr + len++) = 0x69;
			memset(pStr + len,'\n',5);
			len += 5;

		}
	}
	
	DelPrintTask(pPrint);
	chCtrl[1].sendback = PrintComplete;
	printStruct.len = len;
	WRITE(USART3_ID,printStruct.buf,0,len);
}

#endif