#include "global.h"

const char At83Ver[] = "AT83C26-V1.0";
static unsigned char AppTimeFlg;
static unsigned char isolen;

static unsigned char isoLimitFlg = 0;

static IcCommFormat IcComm;
static unsigned char IcDataBuf[256];


//static const unsigned char IcDI[] = {1,1,2,4,8,16,32,12,20};
//static const unsigned short IcFI[] = {372,372,558,744,1116,1488,1860,512,768,1024,1536,2048};


void IsoGetAtr(unsigned char *p,unsigned int len,unsigned char error)
{
	unsigned int fidi;
	unsigned char di,val;
	Iso7816 *pIso;
	At83Ch *pAt83ch;
	unsigned char length;
	
	pIso = &iso;
	pAt83ch = pIso->pAt83ch;
	if(error)
	{
		At83c26Back('N');
		return;
	}
	while(len--)
	{
		val = *p;
		if(pIso->RevLen > 255)
		{
			if(!isoLimitFlg)
			{
				At83c26Back('N');
				isoLimitFlg = 1;
			}
			return;
		}
		isoLimitFlg = 0;
		if(pIso->RevLen == 0)
		{
			if(val == 0x3b)
			{
				pIso->RevBuf[pIso->RevLen++] = val;
				pAt83ch->mode.msbf = 0;
				pIso->pUsartHw->US_MR = pAt83ch->mode.us_mr;
			}
			else if(val == 0x3f)
			{
				pIso->RevBuf[pIso->RevLen++] = val;
				pAt83ch->mode.msbf = 1;
				pIso->pUsartHw->US_MR = pAt83ch->mode.us_mr;
			}
		}
		else if(pIso->RevLen == 1)
		{
			//T = 0;
			if(pAt83ch->mode.msbf == US_MSBF)
			{//0x3F
				val = ~val;
			}
//			if((val == 0x3b) ||(val == 0x3f))
//			{
//				continue;
//			}
			pIso->RevBuf[pIso->RevLen++] = val;
			isolen = val & 0x0f;
			pAt83ch->head_len = 0;
			if(val & 0x10)
			{
				pAt83ch->head_len++;
				pAt83ch->Flg.ta1 = 1;
			}
			if(val & 0x20)
			{
				pAt83ch->head_len++;
				pAt83ch->Flg.tb1 = 1;
			}
			if(val & 0x40)
			{
				pAt83ch->head_len++;
				pAt83ch->Flg.tc1 = 1;
			}
			if(val & 0x80)
			{
				pAt83ch->head_len++;
				pAt83ch->Flg.td1 = 1;
			}
			isolen += pAt83ch->head_len + 1;
		}
		else if((pIso->RevLen > 1) && (pIso->RevLen < isolen))
		{
			//T = 0;
			if(pAt83ch->mode.msbf == US_MSBF)
			{//0x3F
				val = ~val;
			}
			pIso->RevBuf[pIso->RevLen++] = val;
			if(pAt83ch->Flg.ta1 == 0)
			{
				length = 3;
			}
			else 
			{
				length = 4;
			}
			if((pAt83ch->Flg.tc1 == 1) && (pIso->RevLen == length))
			{
				pAt83ch->us_ttgr = val;
			}
			/*else if((pAt83ch->Flg.td1 == 1) && (pIso->RevLen == (pAt83ch->head_len + 1)))
			{
				if(val & 0x10)
				{
					isolen++;
					pAt83ch->Flg.ta2 = 1;
				}
				if(val & 0x20)
				{
					isolen++;
					pAt83ch->Flg.tb2 = 1;
				}
				if(val & 0x40)
				{
					isolen++;
					pAt83ch->Flg.tc2 = 1;
				}
				if(val & 0x80)
				{
					isolen++;
					pAt83ch->Flg.td2 = 1;
				}
			}*/
		}
		else if(pIso->RevLen == isolen)
		{
			pAt83ch->status = ATR_OK;
			//T = 0
			if(pAt83ch->mode.msbf == US_MSBF)
			{//0x3F
				val = ~val;
			}
			pIso->RevBuf[pIso->RevLen++] = val;
			if(pAt83ch->Flg.ta1 == 1)
			{
				val = pIso->RevBuf[2];
				fidi = val >> 4;
				di = val & 0x0f;
				if(di < 0x04)
				{
					di = 1 << (di - 1);
					fidi = (fidi * 372) / di;
					if(fidi != pAt83ch->fidi)
					{
						pAt83ch->fidi = fidi;
						fidi = pIso->pUsartHw->US_IMR;
						pIso->pUsartHw->US_IDR = (unsigned int)-1;
						pIso->pUsartHw->US_FIDI = pAt83ch->fidi;
						pIso->pUsartHw->US_BRGR =  MAIN_MCK / (pAt83ch->fidi * pAt83ch->baud);
						PDC_DisableRx((AT91PS_PDC)(&AT91C_BASE_US1->US_RPR));
						PDC_DisableTx((AT91PS_PDC)(&AT91C_BASE_US1->US_RPR));
						READ_USART(pIso->pUsartHw,US_CSR);
						val = pIso->pUsartHw->US_RHR;
						pIso->pUsartHw->US_IER = fidi;
					}
				}
			}
			pIso->pUsartHw->US_TTGR = pAt83ch->us_ttgr;
			At83c26Back('Y');
		}
		p++;
	}
}

void RevDataDispose(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char val;			
	Iso7816 *pIso;
	At83Ch *pAt83ch;
	
	pIso = &iso;
	pAt83ch = pIso->pAt83ch;
	if(error)
	{
		At83c26Back('N');
		return;
	}
	while(len--)
	{
		val = *s++;
		if(pIso->RevLen > 255)
		{
			if(!isoLimitFlg)
			{
				At83c26Back('N');
				isoLimitFlg = 1;
			}
			return;
		}
		isoLimitFlg = 0;
		if(pAt83ch->mode.msbf == US_MSBF)
		{//0x3F
			val = ~val;
		}
		pIso->RevBuf[pIso->RevLen++] = val;
		if(pIso->RevLength)
		{
			if(pIso->typ == 0)
			{
				if((val == 0x60) && (pIso->RevLen == 1))
				{
					pIso->RevLen--;
					continue;
				}
				else if(((val & 0xf0) == 0x60) || ((val & 0xf0) == 0x90))
				{
					pIso->typ = 3;
					continue;
				}
				else if((val ^ IcComm.INS) == 0)
				{
					pIso->RevLen--;
					pIso->typ = 2;
				}
				else if((val ^ IcComm.INS) == 1)
				{
					pIso->RevLen--;
					pIso->typ = 2;
				}
				else if((val ^ IcComm.INS) == 0xfe)
				{
					pIso->RevLen--;
					pIso->typ = 1;
				}
				else if((val ^ IcComm.INS) == 0xff)
				{
					pIso->RevLen--;
					pIso->typ = 1;
				}
			}
			else if(pIso->typ == 1)
			{
				pIso->typ = 0;
			}
			else if(pIso->typ == 3)
			{
				if(pIso->RevLen == 2)
				{
					if(((pIso->RevBuf[0] & 0xf0) == 0x60) || ((pIso->RevBuf[0] & 0xf0) == 0x90))
					{
						At83c26Back('Y');
					}
				}
			}
			else if(pIso->typ == 2)
			{
				if(pIso->RevLen == pIso->RevLength)
				{
					pIso->typ = 0;
				}
			}
			if(pIso->RevLen == pIso->RevLength + 2)
			{
				At83c26Back('Y');
			}
			
		}
		else
		{
			if(pIso->typ == 0)
			{
				if((val == 0x60) && (pIso->RevLen == 1))
				{
					pIso->RevLen--;
					continue;
				}
				else if(((val & 0xf0) == 0x60) || (val & 0xf0) == 0x90)
				{
					pIso->typ = 1;
					continue;
				}
				else if((val ^ IcComm.INS) == 0)
				{
					pIso->RevLen--;
					if(pIso->SndLength)
					{
						SndToUsart7816(&IcDataBuf[pIso->SndStartSta],pIso->SndLength,SndComplete,RevDataDispose);
						pIso->SndLength = 0;
						pIso->typ = 2;
					}
				}
				else if((val ^ IcComm.INS) == 1)
				{
					pIso->RevLen--;
					if(pIso->SndLength)
					{
						SndToUsart7816(&IcDataBuf[pIso->SndStartSta],pIso->SndLength,SndComplete,RevDataDispose);
						pIso->SndLength = 0;
						pIso->typ = 2;
					}
				}
				else if((val ^ IcComm.INS) == 0xfe)
				{
					pIso->RevLen--;
					if(pIso->SndLength)
					{
						SndToUsart7816(&IcDataBuf[pIso->SndStartSta],1,SndComplete,RevDataDispose);
						pIso->SndStartSta++;
						pIso->SndLength--;
					}
				}
				else if((val ^ IcComm.INS) == 0xff)
				{
					pIso->RevLen--;
					if(pIso->SndLength)
					{
						SndToUsart7816(&IcDataBuf[pIso->SndStartSta],1,SndComplete,RevDataDispose);
						pIso->SndStartSta++;
						pIso->SndLength--;
					}
				}
			}
			else if(pIso->typ == 2)
			{
				if((val == 0x60) && (pIso->RevLen == 1))
				{
					pIso->RevLen--;
					continue;
				}
				else if(((val & 0xf0) == 0x60) || (val & 0xf0) == 0x90)
				{
					pIso->typ = 1;
					continue;
				}
			}
			else 
			{
				pIso->typ = 0;
				if(pIso->RevLen == 2)
				{
					if(((pIso->RevBuf[0] & 0xf0) == 0x60) || ((pIso->RevBuf[0] & 0xf0) == 0x90))
					{
						At83c26Back('Y');
					}
				}
			}
		}
	}
}

static unsigned char CardFlg;

void At83C26Check(void)
{
	unsigned char val;
	
	val = ReadCardStatus();
	if(CardFlg != val)
	{
		CardFlg = val;
		if(!CardInInt)
		{
			CardInInt++;
		}
	}
}

void AppTimeOut(void)
{
	if(iso.status == CARD_OUT)
	{
		if(AppTimeFlg == 0)
		{
			AppTimeFlg = 1;
		}
	}
	else if(iso.status == CARD_COM)
	{
		if(iso.para == WARM_RESET)
		{
			At83c26Back('N');
		}
	}
}

void Iso7816WarmReset(void)
{
	AT91C_BASE_US1->US_RHR;
    AT91C_BASE_US1->US_CR = AT91C_US_RSTSTA | AT91C_US_RSTIT | AT91C_US_RSTNACK;
	READ_USART(iso.pUsartHw,US_CSR);
}

void AnewInitUsart1(unsigned char channel)
{
	WRITE_USART(iso.pUsartHw,US_CR,AT91C_US_RXDIS);
	iso.pUsartHw->US_MR = at83ch[channel].mode.us_mr;
	iso.pUsartHw->US_FIDI = at83ch[channel].fidi;
	iso.pUsartHw->US_BRGR = MAIN_MCK / (at83ch[channel].fidi * at83ch[channel].baud);
	READ_USART(iso.pUsartHw,US_CSR);
	READ_USART(iso.pUsartHw,US_RHR);
	WRITE_USART(iso.pUsartHw,US_CR,AT91C_US_RXEN);
}

void SndComplete(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned short i;
	
	s = s;
	len = len;
	error = error;
	i = 500;
	while(i--)
	{
	}
	WRITE_USART(iso.pUsartHw,US_CR,AT91C_US_TXDIS);
	READ_USART(iso.pUsartHw,US_CSR);
	READ_USART(iso.pUsartHw,US_RHR);
	WRITE_USART(iso.pUsartHw,US_CR,AT91C_US_RXEN);
	if(error)
	{
		AT91C_BASE_US1->US_CR = AT91C_US_RSTSTA | AT91C_US_RSTIT | AT91C_US_RSTNACK;
		At83c26Back('N');
	}
}

int WriteToCardSam(unsigned char *pBuffer,int len,IsoCallback callback)
{
	short length;
	unsigned char command,status = 0;
	Iso7816 *pIso;
	unsigned char val;
	
	if(pBuffer == (unsigned char*)0)
	{
		return -1;
	}
	if(len > 128)//20130807 code review liuzg modify 256)
	{
		return -5;
	}
	pIso = &iso;
	if((len == 1) && (pBuffer[0] == ENQ))
	{
		status = SendToAt83c26(callback);
		return(status);
	}
	
	if(pBuffer[0] != STX)
	{
		return(-1);
	}
	memcpy((unsigned char *)(&length),&pBuffer[1],2);
	length = ShortEndianConvert(length);
	if(pBuffer[length + 3] != ETX)
	{
		return(-2);
	}
	if(len != (length + 5))
	{
		return(-3);
	}
	if(CrcEor(pBuffer,length + 5))
	{
		if(callback)
		{
			val = NAK;
			callback(&val,1,0);
		}
		return(-4);
	}
	command = pBuffer[3];
	switch(command)
	{
	case RESET:
		pIso->status = RESET;
		pIso->para = pBuffer[4];
		status = 0;
		break;
	case QUERY:
		pIso->status = QUERY;
		pIso->para = pBuffer[4];
		status = 0;
		break;
	case CARD_OUT:
		pIso->status = CARD_OUT;
		pIso->para = pBuffer[4];
		status = 0;
		break;
	case POWER:
		pIso->status = POWER;
		pIso->para = pBuffer[4];
		status = 0;
		break;
	case SET_BAUD:
		pIso->status = SET_BAUD;
		pIso->para = pBuffer[4];
		status = 0;
		break;
	case CARD_COM:
		pIso->status = CARD_COM;
		pIso->para = pBuffer[4];
		pIso->NoTmp = ICCARD;
		if(pIso->para == CARD_PARA)
		{
			memcpy((unsigned char *)(&length),&pBuffer[5],2);
			memcpy((unsigned char *)(&IcComm),&pBuffer[7],5);
			length = ShortEndianConvert(length);
			pIso->typ = 0;
			if(length > 5)
			{
				pIso->RevLength = 0;
				pIso->SndLength = IcComm.P3;
				pIso->SndStartSta = 0;
				memcpy(IcDataBuf,&pBuffer[12],length - 5);
			}
			else
			{
				pIso->RevLength = IcComm.P3;
				pIso->SndLength = 0;
			}
		}
		break;
	case SAM_COM:
		pIso->status = SAM_COM;
		pIso->para = pBuffer[4];
		pIso->NoTmp = pBuffer[5] - '0' + 1;
		if(pIso->para == PSAM_PARA)
		{
			memcpy((unsigned char *)(&length),&pBuffer[6],2);
			memcpy((unsigned char *)(&IcComm),&pBuffer[8],5);
			length = ShortEndianConvert(length);
			pIso->typ = 0;
			if(length > 5)
			{
				pIso->RevLength = 0;
				pIso->SndLength = IcComm.P3;
				pIso->SndStartSta = 0;
				memcpy(IcDataBuf,&pBuffer[13],length - 5);
			}
			else
			{
				pIso->RevLength = IcComm.P3;
				pIso->SndLength = 0;
			}
		}
		break;
	default:
		status = 1;
		break;
	}
	if(!status)
	{
		if(callback)
		{
			val = ACK;
			callback(&val,1,0);
		}
	}
	else
	{
		return(-5);
	}
	return(0);
}

void delay(unsigned int x)
{
	unsigned int i = x;
	
	while(i--)
	{
	}
}

void ResetCardReader(unsigned char no)
{
	unsigned char status;
	unsigned char val;
	
	if(no == 0)
	{
		if(CardStatus & POWERON)
		{
			((SC1_INTERFACE *)&val)->card_rst = CLR;
			c26_write_cmd5_sc1_interface(&at83,val);
			delay(4000);
			((SC1_INTERFACE *)&val)->card_ck = CLR;
			c26_write_cmd5_sc1_interface(&at83,val);
			delay(4000);
			((SC1_INTERFACE *)&val)->io_dis = SET;
			((SC1_INTERFACE *)&val)->card_io = CLR;
			c26_write_cmd5_sc1_interface(&at83,val);
			delay(4000);
			StopC26_dcdcA();
			do
			{
				status = c26_read_cmd1_sc1_status(&at83);
				WatchDog();
			}
			while(((STATUSA *)&status)->vcard_ok == SET);
			CardStatus &= (~POWERON);
			((SC1_INTERFACE *)&val)->card_ck = SET;
			((SC1_INTERFACE *)&val)->io_dis = CLR;
			c26_write_cmd5_sc1_interface(&at83,val);
			//CardOut();         //20120205 lei
		}
	}
	else if(no == 1)
	{
		if(CardStatus & SAM0_POWER_ON)
		{
			((SCX_CFG2 *)&val)->card_rst = CLR;
			c26_write_cmd7_sc2_cfg2(&at83,val);
			delay(4000);
			((SCX_CFG2 *)&val)->card_ck = CLR;
			c26_write_cmd7_sc2_cfg2(&at83,val);
			delay(4000);
			c26_read_cmd2_itdis(&at83);
			val = at83.reg.itdis.itdis;
			((ITDIS *)&val)->io_dis2 = SET;
			c26_write_cmd6_itdis(&at83,val);
			((INTERFACEB *)&val)->card_io2 = CLR;
			c26_write_cmd6_interfaceb(&at83,val);
			delay(4000);
			c26_stop_cvcc2(&at83);
			do
			{
				status = c26_read_cmd3_sc2_cfg0(&at83);
				WatchDog();
			}
			while(((SCX_CFG0 *)&val)->vcard_ok == SET);
			CardStatus &= (~SAM0_POWER_ON);
			((SCX_CFG2 *)&val)->card_ck = SET;
			c26_write_cmd7_sc2_cfg2(&at83,val);
			c26_read_cmd2_itdis(&at83);
			val = at83.reg.itdis.itdis;
			((ITDIS *)&val)->io_dis2 = CLR;
			c26_write_cmd6_itdis(&at83,val);
		}
	}
	else if(no == 2)
	{
		if(CardStatus & SAM1_POWER_ON)
		{
			((SCX_CFG2 *)&val)->card_rst = CLR;
			c26_write_cmd8_sc3_cfg2(&at83,val);
			delay(4000);
			((SCX_CFG2 *)&val)->card_ck = CLR;
			c26_write_cmd8_sc3_cfg2(&at83,val);
			delay(4000);
			c26_read_cmd2_itdis(&at83);
			val = at83.reg.itdis.itdis;
			((ITDIS *)&val)->io_dis3 = SET;
			c26_write_cmd6_itdis(&at83,val);
			((INTERFACEB *)&val)->card_io3_card_c42 = CLR;
			c26_write_cmd6_interfaceb(&at83,val);
			delay(4000);
			c26_stop_cvcc3(&at83);
			do
			{
				status = c26_read_cmd4_sc3_cfg0(&at83);
				WatchDog();
			}
			while(((SCX_CFG0 *)&val)->vcard_ok == SET);
			CardStatus &= (~SAM1_POWER_ON);
			((SCX_CFG2 *)&val)->card_ck = SET;
			c26_write_cmd8_sc3_cfg2(&at83,val);
			c26_read_cmd2_itdis(&at83);
			val = at83.reg.itdis.itdis;
			((ITDIS *)&val)->io_dis3 = CLR;
			c26_write_cmd6_itdis(&at83,val);
		}
	}
	else if(no == 3)
	{
		if(CardStatus & SAM2_POWER_ON)
		{
			((SCX_CFG2 *)&val)->card_rst = CLR;
			c26_write_cmd9_sc4_cfg2(&at83,val);
			delay(4000);
			((SCX_CFG2 *)&val)->card_ck = CLR;
			c26_write_cmd9_sc4_cfg2(&at83,val);
			delay(4000);
			c26_read_cmd2_itdis(&at83);
			val = at83.reg.itdis.itdis;
			((ITDIS *)&val)->io_dis4 = SET;
			c26_write_cmd6_itdis(&at83,val);
			((INTERFACEB *)&val)->card_io4 = CLR;
			c26_write_cmd6_interfaceb(&at83,val);
			delay(4000);
			c26_stop_cvcc4(&at83);
			do
			{
				status = c26_read_cmd5_sc4_cfg0(&at83);
				WatchDog();
			}
			while(((SCX_CFG0 *)&val)->vcard_ok == SET);
			CardStatus &= (~SAM2_POWER_ON);
			((SCX_CFG2 *)&val)->card_ck = SET;
			c26_write_cmd9_sc4_cfg2(&at83,val);
			c26_read_cmd2_itdis(&at83);
			val = at83.reg.itdis.itdis;
			((ITDIS *)&val)->io_dis4 = SET;
			c26_write_cmd6_itdis(&at83,val);
		}
	}
	else if(no == 4)
	{
		if(CardStatus & SAM3_POWER_ON)
		{
			((SCX_CFG2 *)&val)->card_rst = CLR;
			c26_write_cmd10_sc5_cfg2(&at83,val);
			delay(4000);
			((SCX_CFG2 *)&val)->card_ck = CLR;
			c26_write_cmd10_sc5_cfg2(&at83,val);
			delay(4000);
			c26_read_cmd2_itdis(&at83);
			val = at83.reg.itdis.itdis;
			((ITDIS *)&val)->io_dis5 = SET;
			c26_write_cmd6_itdis(&at83,val);
			((INTERFACEB *)&val)->card_io5 = CLR;
			c26_write_cmd6_interfaceb(&at83,val);
			delay(4000);
			c26_stop_cvcc5(&at83);
			do
			{
				status = c26_read_cmd6_sc5_cfg0(&at83);
				WatchDog();
			}
			while(((SCX_CFG0 *)&val)->vcard_ok == SET);
			CardStatus &= (~SAM3_POWER_ON);
			((SCX_CFG2 *)&val)->card_ck = SET;
			c26_write_cmd10_sc5_cfg2(&at83,val);
			c26_read_cmd2_itdis(&at83);
			val = at83.reg.itdis.itdis;
			((ITDIS *)&val)->io_dis5 = SET;
			c26_write_cmd6_itdis(&at83,val);
		}
	}
}

unsigned char SendToAt83c26(IsoCallback callback)
{
	unsigned char status,error = 0;
	unsigned short len,length;
	unsigned char buf[128];
	unsigned int i;
	
	status = iso.status;
	switch(status)
	{
	case RESET:
		buf[0] = STX;
		buf[3] = RESET;
		buf[4] = iso.para;
		len = strlen(At83Ver);
		memcpy(&buf[5],At83Ver,len);
		buf[len + 5] = ETX;
		length = ShortEndianConvert(len + 2);
		memcpy(&buf[1],(unsigned char *)(&length),2);
		buf[len + 6] = CrcEor(buf,len + 6);
		if(iso.para == 0x31)
		{
			CardOut();
		}
		//liuzg 20120203
		else if(iso.para == 0x30)
		{
			ResetCardReader(0);
			ResetCardReader(1);
			ResetCardReader(2);
			ResetCardReader(3);
			ResetCardReader(4);
		}
		len += 7;
		break;
	case QUERY:
		buf[0] = STX;
		buf[1] = 0;
		buf[2] = 0x03;
		buf[3] = QUERY;
		buf[4] = iso.para;
		if(CardStatus & CARDIN)
		{
			buf[5] = 'Y';
		}
		else
		{
			buf[5] = 'N';
		}
		buf[6] = ETX;
		buf[7] = CrcEor(buf,7);
		len = 8;
		break;
	case CARD_OUT:
		if(CardStatus & CARDIN)
		{
			CardOut();
			AppTimeFlg = 0;
			CreateTimer1(TIMER_ONE_DELETE,200,AppTimeOut);
			while(!AppTimeFlg)
			{
				CheckTimer1Go();
			}
			CheckIrq1Int();
			i = 0;
		}
		else
		{
			buf[5] = 'E';
			i = 1;
		}
		if(!i)
		{
			if(CardStatus & CARDIN)
			{
				buf[5] = 'N';
			}
			else
			{
				buf[5] = 'Y';
			}
		}
		buf[0] = STX;
		buf[1] = 0;
		buf[2] = 0x03;
		buf[3] = CARD_OUT;
		buf[4] = iso.para;
		buf[6] = ETX;
		buf[7] = CrcEor(buf,7);
		len = 8;
		break;
	case POWER:
		status = c26_select(iso.no,ICCARD);
		if(!status)
		{
			iso.no = ICCARD;
		}
		buf[0] = STX;
		buf[1] = 0;
		buf[2] = 0x03;
		buf[3] = POWER;
		buf[4] = iso.para;
		if(CardStatus & CARDIN)
		{
			if(iso.para == 0x30)
			{
				i = StartC26_dcdcA();
				if(i)
				{
					i = StartC26_dcdcA();
				}
				CheckIrq1Int();
				if(!i)
				{
					if(CardStatus & POWERON)
					{
						buf[5] = 'Y';
					}
					else
					{
						buf[5] = 'N';
					}
				}
				else
				{
					buf[5] = 'N';
				}
			}
			else 
			{
				i = StopC26_dcdcA();
				if(i)
				{
					i = StopC26_dcdcA();
				}
				CheckIrq1Int();
				if(!i)
				{
					if(CardStatus & POWERON)
					{
						buf[5] = 'N';
					}
					else
					{
						buf[5] = 'Y';
					}
				}
				else
				{
					buf[5] = 'N';
				}
			}
		}
		else
		{
			buf[5] = 'E';
		}
		buf[6] = ETX;
		buf[7] = CrcEor(buf,7);
		len = 8;
		break;
	case SET_BAUD:
		buf[0] = STX;
		buf[1] = 0;
		buf[2] = 0x03;
		buf[3] = SET_BAUD;
		buf[4] = iso.para;
		buf[5] = 'Y';
		buf[6] = ETX;
		buf[7] = CrcEor(buf,7);
		len = 8;
		break;
	case CARD_COM:
		//if(CardStatus & CARDIN)
		{
			error = IcCardComDispose();
			if(!error)
			{
				error = 2;
				iso.returnback = callback;
			}
		}
		/*else
		{
			
		}*/
		break;
	case SAM_COM:
		error = SamComDispose();
		if(!error)
		{
			error = 2;
			iso.returnback = callback;
		}
		break;
	default:
		error = 1;
		break;
	}
	if(!error)
	{
		if(callback)
		{
			callback(buf,len,0);
		}
		else
		{
			error = 1;
		}
	}
	if(error == 2)
	{
		error = 0;
	}
	return(error);
}

void At83c26Back(unsigned char type)
{
	unsigned short len;
	unsigned char buf[128];
	
	switch(type)
	{
	case 'Y':
		buf[0] = STX;
		if(iso.no == 0)
		{
			len = iso.RevLen + 6;
			len = ShortEndianConvert(len);
			memcpy(&buf[1],(unsigned char *)(&len),2);
			buf[3] = iso.status;
			buf[4] = iso.para;
			buf[5] = type;
			len = iso.RevLen;
			len = ShortEndianConvert(len);
			memcpy(&buf[6],(unsigned char *)(&len),2);
			memcpy(&buf[8],iso.RevBuf,iso.RevLen);
			buf[8 + iso.RevLen] = ETX;
			buf[9 + iso.RevLen] = CrcEor(buf,9 + iso.RevLen);
			len = iso.RevLen + 10;
		}
		else
		{
			len = iso.RevLen + 6;
			len = ShortEndianConvert(len);
			memcpy(&buf[1],(unsigned char *)(&len),2);
			buf[3] = iso.status;
			buf[4] = iso.para;
			buf[5] = iso.no - 1 + '0';
			buf[6] = type;
			len = iso.RevLen;
			len = ShortEndianConvert(len);
			memcpy(&buf[7],(unsigned char *)(&len),2);
			memcpy(&buf[9],iso.RevBuf,iso.RevLen);
			buf[9 + iso.RevLen] = ETX;
			buf[10 + iso.RevLen] = CrcEor(buf,10 + iso.RevLen);
			len = iso.RevLen + 11;
		}
		break;
	case 'N':
	case 'E':
	case 'W':
	default:
		buf[0] = STX;
		buf[1] = 0;
		if(iso.no == 0)
		{
			buf[2] = 0x03;
			buf[3] = iso.status;
			buf[4] = iso.para;
			buf[5] = type;
			buf[6] = ETX;
			buf[7] = CrcEor(buf,7);
			len = 8;
		}
		else
		{
			buf[2] = 0x04;
			buf[3] = iso.status;
			buf[4] = iso.para;
			buf[5] = iso.no;
			buf[6] = type;
			buf[7] = ETX;
			buf[8] = CrcEor(buf,8);
			len = 9;
		}
		break;
	}
	if(iso.returnback)
	{
		iso.returnback(buf,len,0);
	}
}

unsigned char IcCardComDispose(void)
{
	unsigned char para;
	unsigned char i,status;
	unsigned short len;
	unsigned char val;
	At83Reg *pReg;
	IsoCallback pRevBack;
	
	para = iso.para;
	pReg = &at83.reg;
	switch(para)
	{
	case COLD_RESET:
		if(!(CardStatus & CARDIN))
		{
			At83c26Back('E');
			status = 0;
			break;
		}
		status = c26_select(iso.no,ICCARD);
		if(status)
		{
			At83c26Back('N');
			status = 0;
			break;
		}
		iso.no = ICCARD;
		val = 0;
		((SC1_INTERFACE *)&val)->io_dis = CLR;
		((SC1_INTERFACE *)&val)->ck_stop = CLR;
		((SC1_INTERFACE *)&val)->card_rst = CLR;
		for(unsigned int j = 0; j < 2; j++)
		{
			status = c26_write_cmd5_sc1_interface(&at83,val);
			if(!status)
			{
				break;
			}
		}
		if(status)
		{
			break;
		}
		for(unsigned int j = 0; j < 2; j++)
		{
			status = c26_read_cmd1_sc1_cfg1(&at83);
			if(!status)
			{
				break;
			}
		}
		if(status)
		{
			break;
		}
		val = pReg->sc1_cfg1.cfg1;
		if(pReg->sc1_cfg1.art == SET)
		{
			((SC1_CFG1 *)&val)->art = CLR;
			for(unsigned int j = 0; j < 2; j++)
			{
				status = c26_write_cmd3_sc1_cfg1(&at83,val);
				if(!status)
				{
					break;
				}
			}
			if(status)
			{
				break;
			}
		}
		for(unsigned int j = 0; j < 2; j++)
		{
			status = c26_read_cmd1_sc1_cfg1(&at83);
			if(!status)
			{
				break;
			}
		}
		if(status)
		{
			break;
		}
		val = pReg->sc1_cfg1.cfg1;
		if(pReg->sc1_cfg1.art == CLR)
		{
			((SC1_CFG1 *)&val)->art = SET;
			for(unsigned int j = 0; j < 2; j++)
			{
				status = c26_write_cmd3_sc1_cfg1(&at83,val);
				if(!status)
				{
					break;
				}
			}
			if(status)
			{
				break;
			}
		}
		WRITE_USART(iso.pUsartHw,US_CR,AT91C_US_RXDIS);
		at83ch[0].fidi = 372;
		at83ch[0].baud = USART1_BAUD;
		at83ch[0].mode.us_mode = AT91C_US_USMODE_ISO7816_0;
		at83ch[0].mode.us_clks = CLKS_CLOCK;
		at83ch[0].mode.nb_stop = NBSTOP_1_BIT;
		at83ch[0].mode.par = PAR_EVEN;
		at83ch[0].mode.chrl = CHRL_8_BITS;
		at83ch[0].mode.clko = US_CKLO;
		at83ch[0].mode.max_iteration = MAX_ITER(3);
		iso.no = 0;
		iso.pAt83ch = &at83ch[0];
		iso.RevLen = 0;
		iso.pAt83ch->status = GET_ATR;
		iso.pAt83ch->Flg.flg = 0;
		iso.receviceback = IsoGetAtr;
		AnewInitUsart1(0);
		at83.TimeFlg = 0;
		CreateTimer1(TIMER_ONE_DELETE,20,at83.RunTime);
		while(!at83.TimeFlg)
		{
			CheckTimer1Go();
		}
		status = StartC26_dcdcA();
		if(status)
		{
			At83c26Back('N');
			break;
		}
		at83.TimeFlg = 0;
		CreateTimer1(TIMER_ONE_DELETE,20,at83.RunTime);
		while(!at83.TimeFlg)
		{
			CheckTimer1Go();
		}
		CheckIrq1Int();
		if(!(CardStatus & POWERON))
		{
			At83c26Back('N');
			status = 0;
			break;
		}
		at83.TimeFlg = 0;
		CreateTimer1(TIMER_ONE_DELETE,20,at83.RunTime);
		while(!at83.TimeFlg)
		{
			CheckTimer1Go();
		}
		val = 0;
		((SC1_INTERFACE *)&val)->io_dis = CLR;
		((SC1_INTERFACE *)&val)->ck_stop = CLR;
		((SC1_INTERFACE *)&val)->card_rst = SET;
		for(unsigned int j = 0; j < 2; j++)
		{
			status = c26_write_cmd5_sc1_interface(&at83,val);
			if(!status)
			{
				break;
			}
		}
		break;
	case WARM_RESET:
		if(!(CardStatus & CARDIN))
		{
			At83c26Back('E');
			status = 0;
			break;
		}
		for(i = 0; i < 3; i++)
		{
			iso.receviceback = IsoGetAtr;
			//pAt83Tmr = CreateTimer(TIMER_ONE_DELETE,1000,AppTimeOut);
			status = CardSamWarmReset(0);
			if(!status)
			{
				break;
			}
		}
		if(status)
		{
			//DeleteTimer(pAt83Tmr);
			At83c26Back('N');
			status = 0;
			break;
		}
		break;
	case CARD_PARA:
		status = c26_select(iso.no,iso.NoTmp);
		if(status)
		{
			At83c26Back('N');
			status = 0;
			break;
		}
		iso.no = iso.NoTmp;
		len = 5;
		/*if(iso.RevLength)
		{
			pRevBack = RevIso7816;
		}
		else
		{
			pRevBack = SndIso7816;
		}*/
		pRevBack = RevDataDispose;
		SndToUsart7816((unsigned char *)(&IcComm),len,SndComplete,pRevBack);
		status = 0;
		break;
	default:
		status = 1;
		break;
	}
	return(status);
}

unsigned char SamComDispose(void)
{
	unsigned char i,para;
	unsigned char status;
	unsigned short len;
	IsoCallback pRevBack;
	
	para = iso.para;
	switch(para)
	{
	case SAM_RESET_3:
		At83c26Back('E');
		break;
	case SAM_RESET_5:
		for(i = 0; i < 3; i++)
		{
			status = StartCardSamPower(iso.NoTmp);
			if(!status)
			{
				break;
			}
		}
		if(status)
		{
			At83c26Back('N');
			status = 0;
			break;
		}
		for(i = 0; i < 3; i++)
		{
			iso.receviceback = IsoGetAtr;
			status = CardSamWarmReset(iso.NoTmp);
			if(!status)
			{
				break;
			}
		}
		if(status)
		{
			At83c26Back('N');
			status = 0;
			break;
		}
		break;
	case PSAM_PARA:
		status = c26_select(iso.no,iso.NoTmp);
		if(status)
		{
			At83c26Back('N');
			status = 0;
			break;
		}
		iso.no = iso.NoTmp;
		len = 5;
		/*if(iso.RevLength)
		{
			pRevBack = RevIso7816;
		}
		else
		{
			pRevBack = SndIso7816;
		}*/
		pRevBack = RevDataDispose;
		SndToUsart7816((unsigned char *)(&IcComm),len,SndComplete,pRevBack);
		status = 0;
		break;
	default:
		status = 1;
	}
	return(status);
}

void SndToUsart7816(unsigned char *p,unsigned int len,IsoCallback pSndCallBack,IsoCallback pRevCallBack)
{
	AT91S_USART *pUsart;
	Iso7816 *pIso;
	At83Ch *pAt83ch;
	unsigned int i;
    
	i = 500;
	while(i--)
	{
	}
	pIso = &iso;
	pAt83ch = pIso->pAt83ch;
    pUsart = pIso->pUsartHw;
	pIso->RevLen = 0;
	memset(pIso->RevBuf,0,128);
	pIso->SndLen = len;
	if(pAt83ch->mode.msbf == US_MSBF)
	{
		for(i = 0; i < len; i++)
		{
			p[i] = ~p[i];
		}
	}
	memcpy(pIso->SndBuf,p,len);
	Iso7816WarmReset();
	WRITE_USART(pUsart,US_PTCR,AT91C_PDC_TXTDIS | AT91C_PDC_RXTDIS);
	pIso->sendback = pSndCallBack;
	pIso->receviceback = pRevCallBack;
	WRITE_USART(pUsart,US_TPR,((unsigned int)(&pIso->SndBuf)));
    WRITE_USART(pUsart,US_TCR,pIso->SndLen);
	WRITE_USART(pUsart,US_IER,AT91C_US_TXBUFE | AT91C_US_ITERATION);
	WRITE_USART(pUsart,US_CR,AT91C_US_TXEN | AT91C_US_RXDIS);
	WRITE_USART(pUsart,US_PTCR,AT91C_PDC_TXTEN);
}

void CheckIrq1Int(void)
{
	At83Reg *pReg;
	
	pReg = &at83.reg;
	if((CardInInt) && (PowerDownFlg < 2))
	{
		//SC1_STATUS + SC0_CFG0
		c26_read_cmd1(&at83);
		//DCDCB
		c26_read_cmd7(&at83);
		//SC2_CFG0
		c26_read_cmd3(&at83);
		//SC3_CFG0
		c26_read_cmd4(&at83);
		//SC4_CFG0
		c26_read_cmd5(&at83);
		//SC5_CFG0
		c26_read_cmd6(&at83);
		if(pReg->statusa.vcard_ok == SET)
		{
			if(!(CardStatus & POWERON))
			{
				CardStatus |= POWERON;
			}
		}
		else
		{
			if(CardStatus & POWERON)
			{
				CardStatus &= (~POWERON);
			}
		}
		if(pReg->statusa.card_in == SET)
		{
			if(!(CardStatus & CARDIN))
			{
				if(ReadCardStatus())
				{
					CardStatus |= CARDIN;
				}
			}
		}
		else
		{
			if(CardStatus & CARDIN)
			{
				if(!ReadCardStatus())
				{
					CardStatus &= (~(CARDIN | RESET_OK));
				}
			}
		}
		if(pReg->sc1_cfg0.atr_err == SET)
		{
			if(CardStatus & POWERON)
			{
				if(!(CardStatus & CARD_ATR_ERR))
				{
					CardStatus |= CARD_ATR_ERR;
				}
				else
				{
					CardStatus &= (~CARD_ATR_ERR);
				}
			}
			else
			{
				CardStatus &= (~CARD_ATR_ERR);
			}
		}
		if(pReg->dcdcb.vdc_ok == SET)
		{
			if(!(CardStatus & DCDCB_POWERON))
			{
				CardStatus |= DCDCB_POWERON;
			}
		}
		else
		{
			if(CardStatus & DCDCB_POWERON)
			{
				CardStatus &= (~DCDCB_POWERON);
			}
		}
		if(pReg->sc2_cfg0.vcard_ok == SET)
		{
			if(!(CardStatus & SAM0_POWER_ON))
			{
				CardStatus |= SAM0_POWER_ON;
			}
		}
		else
		{
			if(CardStatus & SAM0_POWER_ON)
			{
				CardStatus &= (~SAM0_POWER_ON);
			}
		}
		if(pReg->sc2_cfg0.atr_err == SET)
		{
			if(CardStatus & SAM0_POWER_ON)
			{
				if(!(CardStatus & SAM0_ATR_ERR))
				{
					CardStatus |= SAM0_ATR_ERR;
				}
				else
				{
					CardStatus &= (~SAM0_ATR_ERR);
				}
			}
			else
			{
				CardStatus &= (~SAM0_ATR_ERR);
			}
		}
		if(pReg->sc3_cfg0.vcard_ok == SET)
		{
			if(!(CardStatus & SAM1_POWER_ON))
			{
				CardStatus |= SAM1_POWER_ON;
			}
		}
		else
		{
			if(CardStatus & SAM1_POWER_ON)
			{
				CardStatus &= (~SAM1_POWER_ON);
			}
		}
		if(pReg->sc3_cfg0.atr_err == SET)
		{
			if(CardStatus & SAM1_POWER_ON)
			{
				if(!(CardStatus & SAM1_ATR_ERR))
				{
					CardStatus |= SAM1_ATR_ERR;
				}
				else
				{
					CardStatus &= (~SAM1_ATR_ERR);
				}
			}
			else
			{
				CardStatus &= (~SAM1_ATR_ERR);
			}
		}
		if(pReg->sc4_cfg0.vcard_ok == SET)
		{
			if(!(CardStatus & SAM2_POWER_ON))
			{
				CardStatus |= SAM2_POWER_ON;
			}
		}
		else
		{
			if(CardStatus & SAM2_POWER_ON)
			{
				CardStatus &= (~SAM2_POWER_ON);
			}
		}
		if(pReg->sc4_cfg0.atr_err == SET)
		{
			if(CardStatus & SAM2_POWER_ON)
			{
				if(!(CardStatus & SAM2_ATR_ERR))
				{
					CardStatus |= SAM2_ATR_ERR;
				}
				else
				{
					CardStatus &= (~SAM2_ATR_ERR);
				}
			}
			else
			{
				CardStatus &= (~SAM2_ATR_ERR);
			}
		}
		if(pReg->sc5_cfg0.vcard_ok == SET)
		{
			if(!(CardStatus & SAM3_POWER_ON))
			{
				CardStatus |= SAM3_POWER_ON;
			}
		}
		else
		{
			if(CardStatus & SAM3_POWER_ON)
			{
				CardStatus &= (~SAM3_POWER_ON);
			}
		}
		if(pReg->sc5_cfg0.atr_err == SET)
		{
			if(CardStatus & SAM3_POWER_ON)
			{
				if(!(CardStatus & SAM3_ATR_ERR))
				{
					CardStatus |= SAM3_ATR_ERR;
				}
				else
				{
					CardStatus &= (~SAM3_ATR_ERR);
				}
			}
			else
			{
				CardStatus &= (~SAM3_ATR_ERR);
			}
		}
		CardInInt--;
	}
	if(CardStatus & CARDIN)
	{
		if(IdleFlg & CARD_IN_NOT)
		{
			CardOut();
		}
	}
}
