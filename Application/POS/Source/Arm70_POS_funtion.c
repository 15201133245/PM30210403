
#include <stdlib.h>
#include <string.h>
#include "POS.h"
#include "define.h"
#include "PdProtect.h"
#include "message.h"
#include "Screen.h"
//=======================================; Bellow LLLLLA 2011-06-20
#include "CRMemCopy.h"
//=======================================; Upper LLLLLA 2011-06-20

#define _POS_Debug 1



//0:no task;1:wait List ack;2:wait Grey ack
//unsigned char QueStatus;
//0:no task;1:wait ack list length;2:may send download data seg;3:wait ack list data

#define	POS_REQUE_TIMEOUT		8000


/*void ResetQueStatus(void)
{
	if(QueStatus)
	{
		if(PosFindListCount < 3)
		{
			PosFindListCount++;
		}
		else
		{
			if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_BLACK)
			{
				POS_CR_Buf.CheckFlag = 0x02;
				ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_BLACK;
				ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_BLACK);
			}
			if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_WHITE)
			{
				POS_CR_Buf.CheckFlag = 0x02;
				ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_WHITE;
				ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_WHITE);
			}
			if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_GREY)
			{
				GreyResponse.CheckFlag = 0x02;
				ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_GREY;
				ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_GREY);
			}
			PosFindListCount = 0;
		}
		QueStatus = 0;
	}
}*/

void ResetDownloadStatus(void)
{
	if(DownLoadStatus == 1)
	{
		DownLoadStatus = 0;
	}
	else if(DownLoadStatus == 3)
	{
		DownLoadStatus = 2;
	}
	else if(DownLoadStatus == 4)
	{
		DownLoadStatus = 0;
	}
}

void ResetTradeFlg(void)
{
	if(PosReadRecordFlg & POS_READ_RECORD)
	{
		PosReadRecordFlg &= (~POS_READ_RECORD);
	}
	if(PosReadRecordFlg & LAST_TRANS_RECORD)
	{
		PosReadRecordFlg &= (~LAST_TRANS_RECORD);
	}
	if(PosReadRecordFlg & TRANS_UPLOADING)
	{
		PosReadRecordFlg &= (~TRANS_UPLOADING);
	}
}
//end liuzg20120706



/**************************************************************************** 
**函数名称：SndPosRealMessage(void)
** 说明: 上送加油实时信息
** 输入: 无
** 输出:
** 返回值: 
** 作者：BILL li
** 时间：2009-04-24
** 变更原因：存在隐患
** 修改者：liuzg
** 修改时间：2010-1-7
** Version :00.00.01
*****************************************************************************
**/


void SndPosRealMessage(void)
{
  	unsigned short len,crc;
  	unsigned char N_Count;
  	unsigned short i;
	unsigned char buf[256];
	
  	i=0;
  	buf[0] = 0xfa;
	buf[1] = Arm_g_posbuff[2];
	buf[2] = Arm_g_posbuff[1];
	buf[3] = Arm_g_posbuff[3];
	if(ProcStatus[PROCESS_CR] & CARD_BUSY)
	{
		buf[6] = ARM70_POS_RTOS;
		buf[7] = 1;
		buf[8] = 2;
		buf[9] = POS_CR_Buf.TransNozzleNumber;// 枪号
		buf[10] = POS_CR_Buf.P_UNIT;              //结算单位，元  积分
		memcpy(&buf[11],&POS_CR_Buf.TransAmount[1],3);//数额
		memcpy(&buf[14],&POS_CR_Buf.TransVolume[1],3);//升数
		memcpy(&buf[17],&POS_CR_Buf.TransPrice,2);//价格
		buf[4] = 0;
		buf[5] = 0x13;
		crc = Crc16(&buf[1],18,0);
		crc = ShortEndianConvert(crc);
		memcpy(&buf[19],(unsigned char *)(&crc),2);
		len = AddFA(buf,21);
		memcpy(PosSndBuf,buf,len);
	}
	else if(ProcStatus[PROCESS_CR] & CARD_READY)
	{
		// count 1 + st 1+ NZN 1+LEN 1 + ASN 10+ CardSt 2 +BAL 4 +Cardinfor 0 
		buf[6] = ARM70_POS_RTOS;
		buf[7] = GetGUN_N();//count
		for(N_Count = 0; N_Count < GetGUN_N();N_Count++)
		{
			buf[8 + N_Count * 19] = 1;
			buf[9 + N_Count * 19] = GetLogicNzn(N_Count);// 枪号
			buf[10 + N_Count * 19] = 16; //卡信息长度
			memcpy(&buf[11 + N_Count * 19],POS_CR_Buf.ICCARD_ASN,10);//卡号
			memcpy(&buf[21 + N_Count * 19],POS_CR_Buf.ICCARD_STATE,2);//卡状态
			memcpy(&buf[23 + N_Count * 19],POS_CR_Buf.ICCARD_Balance,4);//余额
		}
		i = N_Count * 19 + 2;
		HexToComBcd(i,&buf[4]);
		crc = Crc16(&buf[1],i + 5,0);
		crc = ShortEndianConvert(crc);
		memcpy(&buf[i + 6],(unsigned char *)(&crc),2);
		len = AddFA(buf,i + 8);
		memcpy(PosSndBuf,buf,len);
	}
	else
	{
		buf[4] = 0;
		buf[5] = 0x02;
		buf[6] = ARM70_POS_RTOS;
		buf[7] = 0;
		crc = Crc16(&buf[1],7,0);
		crc = ShortEndianConvert(crc);
		memcpy(&buf[8],(unsigned char *)(&crc),2);
		len = AddFA(buf,10);
		memcpy(PosSndBuf,buf,len);
	}
	PosCastLeng = len;
}

/**************************************************************************** 
**函数名称：PosMessage_Polling(void)
** 说明: POLL 命令解析
** 输入: 无
** 输出:
** 返回值: 
** 作者：BILL li
** 时间：2009-04-24
** 变更原因：存在隐患
** 修改者：liuzg
** 修改时间：2010-1-7
** Version :00.00.01
*****************************************************************************
**/
//_CrQue			xdata CrQue  全局变量

#define ALLOW_QUE_INSERT_POLL_NUM	8
#define ALLOW_QUE_LIST_COUNT 9

void SndPosVerifyMessage(void)
{
	unsigned short crc,len;
	unsigned char buf[256];
//=======================================; Bellow LLLLLA 2011-06-20
	unsigned char buf0[128];
//=======================================; Upper LLLLLA 2011-06-20

	buf[0] = 0xfa;
	buf[1] = Arm_g_posbuff[2];
	buf[2] = Arm_g_posbuff[1];
	buf[3] = Arm_g_posbuff[3];
	buf[4] = 0;
	buf[5] = 0x94;
	buf[6] = ARM70_POS_ERROR;
	buf[7] = 0;
//=======================================; Bellow LLLLLA 2011-06-20
	Asc2Hex( buf0, &POS_CR_Buf.InforMessage[8], 40 );
//=======================================; Upper LLLLLA 2011-06-20
	memcpy(&buf[8],POS_CR_Buf.InforMessage,56+32);
//=======================================; Bellow LLLLLC 2011-06-20
//	crc = Crc16(&buf[8],56+32,0);
	crc = Crc16( buf0, 40, 0 );
//=======================================; Upper LLLLLC 2011-06-20
	//crc = ShortEndianConvert(crc);
	sprintf((char *)(&buf[64+32]),"%4X",crc);
	crc = Crc16(&buf[1],67+32,0);
	crc = ShortEndianConvert(crc);
	memcpy(&buf[68+32],(unsigned char *)(&crc),2);
	len = AddFA(buf,70+32);
	memcpy(PosSndBuf,buf,len);
	PosCastLeng = len;
}

void PosMessage_Polling(void)
{       
	unsigned int i;
	unsigned char listtype[7];
	
	AdjustSystime(&Arm_g_posbuff[7],0);//YYYYMMDDHHMM
	memcpy(listtype,&Arm_g_posbuff[14],7);
  	if(ProcExchange[PROCESS_CR][PROCESS_POS] & SEND_MESSAGE)
	{
		ProcExchange[PROCESS_CR][PROCESS_POS] &= (~SEND_MESSAGE);
		SndPosVerifyMessage();
		return;
	}
  	if(ProcExchange[PROCESS_CR][PROCESS_POS] & SENDCARDIN)
	{
		if(SendCountToPos < 3)
		{
			SndPosRealMessage();
			SendCountToPos++;
			return;
		}
		else
		{
			SendCountToPos = 0;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~SENDCARDIN);
		}
	}
	if(ProcStatus[PROCESS_CR] & CARD_BUSY)
	{
		if(SendCountToPos < 1)
		{
			SndPosRealMessage();
			SendCountToPos++;
			return;
		}
		else
		{
			SendCountToPos = 0;
		}
	}
	   
	if(ProcExchange[PROCESS_CR][PROCESS_POS] & SENDEOT)
    {
        if(SendCountToPos < 3)
        {
            SndPosRealMessage();
            SendCountToPos++;
            return;
        }
		else
		{
			SendCountToPos = 0;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~SENDEOT);
		}
    }
	if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_BLACK)
	{
		if(PosQueListCount < ALLOW_QUE_LIST_COUNT)
		{
			PosQueListCount++;
			ReqPosQueBWL();
			return;
		}
		else
		{
			PosQueListCount = 0;
			POS_CR_Buf.CheckFlag = 0x02;
			ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_BLACK;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_BLACK);
		}
		/*if(!QueStatus)
		{
			PosQueInsertPollCount = 0;
			ReqPosQueBWL();
			return;
		}*/
	}
	if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_WHITE)
	{
		if(PosQueListCount < ALLOW_QUE_LIST_COUNT)
		{
			PosQueListCount++;
			ReqPosQueBWL();
			return;
		}
		else
		{
			PosQueListCount = 0;
			POS_CR_Buf.CheckFlag = 0x02;
			ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_WHITE;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_WHITE);
		}
		/*if(!QueStatus)
		{
			PosQueInsertPollCount = 0;
			ReqPosQueBWL();
			return;
		}*/
	}
	if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_GREY)
	{
		if(PosQueListCount < ALLOW_QUE_LIST_COUNT)
		{
			PosQueListCount++;
			ReqPosQueGreyList();
			return;
		}
		else
		{
			PosQueListCount = 0;
			GreyResponse.CheckFlag = 0x02;
			ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_GREY;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_GREY);
		}
		/*if(!QueStatus)
		{
			PosQueInsertPollCount = 0;
			ReqPosQueGreyList();
			return;
		}*/
	}
	if(DownLoadStatus == 0)
	{
		//memcpy(listtype,&Arm_g_posbuff[14],7);
		i = CheckListVersion(listtype);
		if(i)
		{
			if((listtype[0] == 4) || (listtype[0] == 5))
			{
				DownLoadPriceFlg = listtype[0];
				ReqDownloadList(listtype[0]);
				DownLoadStatus = 1;
				return;
			}
		}
	}
	else if(DownLoadStatus == 2)
	{
		if(DownLoadPriceFlg)
		{
			DownLoadPriceFlg = 0;
			ReqPosData();
			DownLoadStatus = 3;
			return;
		}
	}
	if(ProcExchange[PROCESS_CR][PROCESS_POS] & TRANS_UPLOAD)
	{
		if(!(PosReadRecordFlg & LAST_TRANS_RECORD))
		{
			SndPosLastTrade();
			PosReadRecordFlg |= LAST_TRANS_RECORD;
			return;
		}
	}
	if(ReadRecord(&Arm_g_posbuff[7]))
	{
		//SndPosTrade();
		//return;
		//start liuzg20120706
		if(!(PosReadRecordFlg & TRANS_UPLOADING))
		{
			PosReadRecordFlg |= TRANS_UPLOADING;
			SndPosTrade();
			return;
		}
		//end liuzg20120706
	}
	if(DownLoadStatus == 0)
	{
		//memcpy(listtype,&Arm_g_posbuff[14],7);
		i = CheckListVersion(listtype);
		if(i)
		{
			ReqDownloadList(listtype[0]);
			DownLoadStatus = 1;
			return;
		}
	}
	else if(DownLoadStatus == 2)
	{
		ReqPosData();
		DownLoadStatus = 3;
		return;
	}
	SndPosRealMessage();
}

/*void PosMessage_Polling(void)
{       
	unsigned int i;
	unsigned char listtype[7];
	
	AdjustSystime(&Arm_g_posbuff[7],0);//YYYYMMDDHHMM
  
  	if(ProcExchange[PROCESS_CR][PROCESS_POS] & SENDCARDIN)
	{
		if(SendCountToPos < 3)
		{
			SndPosRealMessage();
			SendCountToPos++;
			return;
		}
		else
		{
			SendCountToPos = 0;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~SENDCARDIN);
		}
	}
	if(ProcExchange[PROCESS_CR][PROCESS_POS] & SENDEOT)
    {
        if(SendCountToPos < 3)
        {
            SndPosRealMessage();
            SendCountToPos++;
            return;
        }
		else
		{
			SendCountToPos = 0;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~SENDEOT);
		}
    }
	if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_BLACK)
	{
		if(!QueStatus)
		{
			ReqPosQueBWL();
			return;
		}
	}
	if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_WHITE)
	{
		if(!QueStatus)
		{
			ReqPosQueBWL();
			return;
		}
	}
	if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_GREY)
	{
		if(!QueStatus)
		{
			ReqPosQueGreyList();
			return;
		}
	}
	if(DownLoadStatus == 0)
	{
		memcpy(listtype,&Arm_g_posbuff[14],7);
		i = CheckListVersion(listtype);
		if(i)
		{
			if((listtype[0] == 4) || (listtype[0] == 5))
			{
				DownLoadPriceFlg = listtype[0];
				ReqDownloadList(listtype[0]);
				DownLoadStatus = 1;
				return;
			}
		}
	}
	else if(DownLoadStatus == 2)
	{
		if(DownLoadPriceFlg)
		{
			DownLoadPriceFlg = 0;
			ReqPosData();
			DownLoadStatus = 3;
			return;
		}
	}
	if(ProcExchange[PROCESS_CR][PROCESS_POS] & TRANS_UPLOAD)
	{
		if(!(PosReadRecordFlg & LAST_TRANS_RECORD))
		{
			SndPosLastTrade();
			PosReadRecordFlg |= LAST_TRANS_RECORD;
			return;
		}
	}
	if(ReadRecord(&Arm_g_posbuff[7]))
	{
		SndPosTrade();
		return;
	}
	if(DownLoadStatus == 0)
	{
		memcpy(listtype,&Arm_g_posbuff[14],7);
		i = CheckListVersion(listtype);
		if(i)
		{
			ReqDownloadList(listtype[0]);
			DownLoadStatus = 1;
			return;
		}
	}
	else if(DownLoadStatus == 2)
	{
		ReqPosData();
		DownLoadStatus = 3;
		return;
	}
	SndPosRealMessage();
}*/

#define BASE_BLACK_LIST_LIMIT		20000 * 10 + 16
#define LIST_LIMIT					200 * 10 + 16

void PosMessage_DataLeng(void)
{
	unsigned int len;
	unsigned char sta;
	if(DownLoadStatus == 1)
	{
		StopTimer(DownLoadTmr);
		memcpy((unsigned char *)(&len),&Arm_g_posbuff[7],4);
		len = IntEndianConvert(len);
		if(Arm_g_posbuff[11] == 0)
		{
			if(len == 0)
			{
				DownLoadStatus = 4;
				memset(CurrentInfoLine,0,sizeof(CurrentInfoLine));
				memcpy(InfoLine,PosError[1],strlen(PosError[1]) + 1);
				FillInfoMsg(M_InfoLine,0,0);
				return;
			}
			else if(len > BASE_BLACK_LIST_LIMIT)
			{
				if(!limit_flg)
				{
					limit_flg = 1;
					len = 16;
					WriteDriveLog(0,0);
				}
				DownLoadStatus = 4;
				memset(CurrentInfoLine,0,sizeof(CurrentInfoLine));
				memcpy(InfoLine,PosError[2],strlen(PosError[2]) + 1);
				FillInfoMsg(M_InfoLine,0,0);
				return;
			}
		}
		else if(Arm_g_posbuff[11] < 4)
		{
			if(len == 0)
			{
				DownLoadStatus = 4;
				sta = 3 + (Arm_g_posbuff[11] - 1) * 2;
				memset(CurrentInfoLine,0,sizeof(CurrentInfoLine));
				memcpy(InfoLine,PosError[sta],strlen(PosError[sta]) + 1);
				FillInfoMsg(M_InfoLine,0,0);
				return;
			}
			else if(len > LIST_LIMIT)
			{
				if(!limit_flg)
				{
					limit_flg = 1;
					len = 16;
					WriteDriveLog(Arm_g_posbuff[11],0);
				}
				DownLoadStatus = 4;
				sta = 4 + (Arm_g_posbuff[11] - 1) * 2;
				memset(CurrentInfoLine,0,sizeof(CurrentInfoLine));
				memcpy(InfoLine,PosError[sta],strlen(PosError[sta]) + 1);
				FillInfoMsg(M_InfoLine,0,0);
				return;
			}
		}
		else if(Arm_g_posbuff[11] == 4)
		{
			if(len == 0)
			{
				DownLoadStatus = 4;
				memset(CurrentInfoLine,0,sizeof(CurrentInfoLine));
				memcpy(InfoLine,PosError[9],strlen(PosError[9]) + 1);
				FillInfoMsg(M_InfoLine,0,0);
				return;
			}
			else if(len > sizeof(GradePrice))
			{
				if(!limit_flg)
				{
					limit_flg = 1;
					len = sizeof(GradePrice);
					WriteDriveLog(4,0);
				}
				DownLoadStatus = 4;
				memset(CurrentInfoLine,0,sizeof(CurrentInfoLine));
				memcpy(InfoLine,PosError[10],strlen(PosError[10]) + 1);
				FillInfoMsg(M_InfoLine,0,0);
				return;
			}
		}
		else if(Arm_g_posbuff[11] == 5)
		{
			if(len == 0)
			{
				DownLoadStatus = 4;
				memset(CurrentInfoLine,0,sizeof(CurrentInfoLine));
				memcpy(InfoLine,PosError[11],strlen(PosError[11]) + 1);
				FillInfoMsg(M_InfoLine,0,0);
				return;
			}
			else if(len > sizeof(OilMessage))
			{
				if(!limit_flg)
				{
					limit_flg = 1;
					len = sizeof(OilMessage);
					WriteDriveLog(5,0);
				}
				DownLoadStatus = 4;
				memset(CurrentInfoLine,0,sizeof(CurrentInfoLine));
				memcpy(InfoLine,PosError[12],strlen(PosError[12]) + 1);
				FillInfoMsg(M_InfoLine,0,0);
				return;
			}
		}
		limit_flg = 0;
		WriteListLength(Arm_g_posbuff[11],len);
		PosSeg.type = Arm_g_posbuff[11];
		PosSeg.seg = 0;
		PosSeg.SegOffset = 0;
		PosSeg.SegTotal = len >> 4;
		if(len & 0x0f)
		{
			PosSeg.SegTotal++;
		}
		DownLoadStatus = 2;
	}
}


          
	

void PosMessage_GrayRecord(void)
{
  	if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_GREY)
	{
		PosQueListCount = 0;
		GreyResponse.CheckFlag = Arm_g_posbuff[7] & 0x01;
		if(GreyResponse.CheckFlag == 0)
		{
		  	//
		  	memcpy(GreyResponse.GreyCardASN,&Arm_g_posbuff[8],17);
		  	//CTC 2 +DS 1+ TIME 7+ GMC 4+ PSAM-TID 6 +PSMA 4
		  	memcpy(GreyResponse.GreyCardCTC,&Arm_g_posbuff[25],24);
		}
		ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_GREY;
		ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_GREY);
	}
}

void PosMessage_AskTotal(void)
{
	unsigned char buf[256];
	unsigned short len,crc;
	unsigned char i,count;
	
	buf[0] = 0xfa;
	buf[1] = Arm_g_posbuff[2];
	buf[2] = Arm_g_posbuff[1];
	buf[3] = Arm_g_posbuff[3];
	count = GetGUN_N();
	buf[6]= ARM70_POS_OIL_ACK;
	buf[7] = count;
	for(i = 0; i < count; i++)
	{
		buf[8 + i * 5] = GetLogicNzn(i);
		memcpy(&buf[9 + i * 5],DISP_CR_Buf.DispTotal[i].VolumeTotal,4);
	}
	len = 5 * count + 2;
	HexToComBcd(len,&buf[4]);
	len += 5;
	crc = Crc16(&buf[1],len,0);
	len ++;
	crc = ShortEndianConvert(crc);
	memcpy(&buf[len],(unsigned char *)(&crc),2);
	len += 2;
	len = AddFA(buf,len);
	memcpy(PosSndBuf,buf,len);
	PosCastLeng = len;
}

//static const char m_info[] = "gilbarco";

void PosMessage_AskInfo(void)
{
	unsigned char buf[256];
	unsigned short len,crc;
	unsigned char j,count;
	int tmp;
	
	buf[0] = 0xfa;
	buf[1] = Arm_g_posbuff[2];
	buf[2] = Arm_g_posbuff[1];
	buf[3] = Arm_g_posbuff[3];
	buf[6] = ARM70_PC_READ_WMCMESS;
	j = strlen(DISPMSG[2]);
	if(j > 12)
	{
		j = 12;
	}
	memcpy(&buf[7],DISPMSG[2],j);
	memset(&buf[7 + j],' ' ,12 - j);
	buf[19] = GetProv();
	buf[20] = GetCity();
	tmp = GetSuperior();
	memcpy(&buf[21],(unsigned char *)(&tmp),4);
	tmp = GetS_ID();
	memcpy(&buf[25],(unsigned char *)(&tmp),4);
	crc = ShortEndianConvert(SysBCDTime.year);
	memcpy(&buf[29],(unsigned char *)(&crc),2);
	memcpy(&buf[31],&SysBCDTime.month,5);
	count = GetGUN_N();
	buf[36] = count;
	for(j = 0; j < count; j++)
	{
		buf[37 + j] = GetLogicNzn(j);
	}
	len = 31 + count;
	crc = GetListVersion(BASEBLACKLIST);
	memcpy(&buf[37 + count],(unsigned char *)(&crc),2);
	crc = GetListVersion(ADDBLACKLIST);
	buf[39 + count] = *((unsigned char *)(&crc) + 1);
	crc = GetListVersion(DELBLACKLIST);
	buf[40 + count] = *((unsigned char *)(&crc) + 1);
	crc = GetListVersion(WHITELIST);
	buf[41 + count] = *((unsigned char *)(&crc) + 1);
	crc = GetListVersion(GRADEPRICE);
	buf[42 + count] = *((unsigned char *)(&crc));
	crc = GetListVersion(OILSTOP);
	buf[43 + count] = *((unsigned char *)(&crc));
	len += 7;
	//厂商自定义信息－－暂时无
	HexToComBcd(len,&buf[4]);
	len += 5;
	crc = Crc16(&buf[1],len,0);
	crc = ShortEndianConvert(crc);
	memcpy(&buf[44 + count],(unsigned char *)(&crc),2);
	len += 3;
	len = AddFA(buf,len);
	memcpy(PosSndBuf,buf,len);
	PosCastLeng = len;
}
/**************************************************************************** 
**函数名称：ReqPosQueBWL(unsigned char no)
** 说明: 向PC机查询白、黑名单
** 输入: no: 1 白 2黑
** 输出:
** 返回值: 
** 作者：BILL li
** 时间：2009-05-24
** 变更原因：存在隐患
** 修改者：liuzg
** 修改时间：2010-1-7
** Version :00.00.01
*****************************************************************************
**/
//请求查询黑/白名单
void ReqPosQueBWL(void)
{
	unsigned short crc,len;
 	unsigned char buf[256];

  	buf[0] = 0xfa;
	buf[1] = Arm_g_posbuff[2];
	buf[2] = Arm_g_posbuff[1];
	buf[3] = Arm_g_posbuff[3];
	buf[4] = 0;
	buf[5] = 0x11;
	buf[6] = ARM70_POS_QUE_BWCARD;
	memcpy(&buf[7],POS_CR_Buf.ICCARD_ASN,10);
	crc = Crc16(&buf[1],16,0);
	crc = ShortEndianConvert(crc);
	memcpy(&buf[17],(unsigned char *)(&crc),2);
	len = AddFA(buf,19);
	memcpy(PosSndBuf,buf,len);
	PosCastLeng = len;
}


/**************************************************************************** 
**函数名称：ReqPosQueGreyList(void)
** 说明: 向后台查灰名单
** 输入: 
** 输出:
** 返回值: 
** 作者：BILL li
** 时间：2009-06-24
** 变更原因：存在隐患
** 修改者：liuzg
** 修改时间：2010-1-7
** Version :00.00.01
*****************************************************************************
**/

void ReqPosQueGreyList()
{
  	unsigned short crc,len;
  	unsigned char buf[256];
	
  	buf[0] = 0xfa;
	buf[1] = Arm_g_posbuff[2];
	buf[2] = Arm_g_posbuff[1];
	buf[3] = Arm_g_posbuff[3];
	buf[4] = 0;
	buf[5] = 0x25;
	buf[6] = ARM70_POS_RELEASE_ASH;
	memcpy(&buf[7],GreyInquiry.GreyCardASN,24);
	crc = Crc16(&buf[1],30,0);
	crc = ShortEndianConvert(crc);
	memcpy(&buf[31],(unsigned char *)(&crc),2);
	len = AddFA(buf,33);
	memcpy(PosSndBuf,buf,len);
	PosCastLeng = len;
}
/**************************************************************************** 
**函数名称：PosMessage_BWL(void)
** 说明: PC机查询白、黑名单名单结果
** 输入: no: 1 白 2黑 其他为无效
** 输出:
** 返回值: 
** 作者：BILL li
** 时间：2009-05-28
** 变更原因：存在隐患
** 修改者：liuzg
** 修改时间：2010-1-7
** Version :00.00.01
*****************************************************************************
**/
void PosMessage_BWL(void)
{
   	//黑白名单查询结果
	if(ProcExchange[PROCESS_CR][PROCESS_POS] & (POS_CHECK_WHITE | POS_CHECK_BLACK))
	{
		PosQueListCount = 0;
		POS_CR_Buf.CheckFlag = Arm_g_posbuff[7] & 0x01;
		if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_BLACK)
		{
			ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_BLACK;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_BLACK);
		}
		else if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_WHITE)
		{
			ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_WHITE;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_WHITE);
		}
	}
}

void SndPosLastTrade(void)
{
	unsigned char buf[256];
  	unsigned short crc;
  	unsigned short len;
	
	buf[0] = 0xfa;
	buf[1] = Arm_g_posbuff[2];
	buf[2] = Arm_g_posbuff[1];
	buf[3] = Arm_g_posbuff[3];
	buf[4] = 0x00;
	buf[5] = 0x96;
	buf[6] = ARM70_POS_STRIKE;
	memcpy(&buf[7],&POSUploadTrans.UploadTrans[1],95);
	crc = Crc16(&buf[1],101,0);
	crc = ShortEndianConvert(crc);
	memcpy(&buf[102],(unsigned char *)(&crc),2);
	len = AddFA(buf,104);
	memcpy(PosSndBuf,buf,len);
	PosCastLeng = len;
	if(SndTransTmr == (Tmr *)0)
	{
		//SndTransTmr = CreateTimer(TIMER_ONE,POS_REQUE_TIMEOUT,ResetLastTradeFlg);
		//start liuzg20120706
		SndTransTmr = CreateTimer(TIMER_ONE,POS_REQUE_TIMEOUT,ResetTradeFlg);
		//end liuzg20120706
	}
	ResetTimer(SndTransTmr);
}

/**************************************************************************** 
**函数名称：SndPosTrade(void)
** 说明: 上送交易记录
** 输入:
** 输出:
** 返回值: 
** 作者：BILL li
** 时间：2009-05-29
** 变更原因：存在隐患
** 修改者：liuzg
** 修改时间：2010-1-7
** Version :00.00.01
*****************************************************************************
**/

void SndPosTrade(void)
{
  	unsigned char buf[256];
  	unsigned short crc;
  	unsigned short len;
  
  	buf[0] = 0xfa;
	buf[1] = Arm_g_posbuff[2];
	buf[2] = Arm_g_posbuff[1];
	buf[3] = Arm_g_posbuff[3];
	buf[4] = 0x00;
	buf[5] = 0x96;
	buf[6] = ARM70_POS_STRIKE;
	memcpy(&buf[7],&Arm_g_posbuff[8],95);
	crc = Crc16(&buf[1],101,0);
	crc = ShortEndianConvert(crc);
	memcpy(&buf[102],(unsigned char *)(&crc),2);
	len = AddFA(buf,104);
	memcpy(PosSndBuf,buf,len);
	PosCastLeng = len;
	//liuzg 20120704 Reset Timer
	if(SndTransTmr == (Tmr *)0)
	{
		//SndTransTmr = CreateTimer(TIMER_ONE,POS_REQUE_TIMEOUT,ResetLastTradeFlg);
		//start liuzg20120706
		SndTransTmr = CreateTimer(TIMER_ONE,POS_REQUE_TIMEOUT,ResetTradeFlg);
		//end liuzg20120706
	}
	ResetTimer(SndTransTmr);
}

/**************************************************************************** 
**函数名称：void ReqDownloadList(unsigned char type)
** 说明: POS 向 PC 申请下载名单 
** 输入: 
** 输出:
** 返回值: 
** 作者：BILL li
** 时间：2009-06-05
** 变更原因：存在隐患
** 修改者：liuzg
** 修改时间：2010-1-7
** Version :00.00.01
*****************************************************************************
**/
void ReqDownloadList(unsigned char type)
{
  	unsigned short crc;
  	unsigned char buf[256];
  	unsigned short len;
  
	buf[0] = 0xfa;
	buf[1] = Arm_g_posbuff[2];
	buf[2] = Arm_g_posbuff[1];
	buf[3] = Arm_g_posbuff[3];
	buf[4] = 0;
	buf[5] = 0x04;
	buf[6] = ARM70_POS_APPLY_DATA;
	buf[7] = type;
	memcpy(&buf[8],&Arm_g_posbuff[14],2);
	crc = Crc16(&buf[1],9,0);
	crc = ShortEndianConvert(crc);
	memcpy(&buf[10],(unsigned char *)(&crc),2);
	len = AddFA(buf,12);
	memcpy(PosSndBuf,buf,len);
	PosCastLeng = len;
	if(DownLoadTmr == (Tmr *)0)
	{
		DownLoadTmr = CreateTimer(TIMER_ONE,POS_REQUE_TIMEOUT,ResetDownloadStatus);
	}
	ResetTimer(DownLoadTmr);
}

void ReqPosData(void)
{
  	unsigned short crc;
  	unsigned char buf[256];
  	unsigned int PosLeng,len;
  
	buf[0] = 0xfa;
	buf[1] = Arm_g_posbuff[2];
	buf[2] = Arm_g_posbuff[1];
	buf[3] = Arm_g_posbuff[3];
	buf[4] = 0;
	buf[5] = 0x05;
	buf[6] = ARM70_POS_APPLY_DATACON;
	PosLeng = PosSeg.SegTotal - PosSeg.SegOffset;
	if(PosSeg.SegOffset == 0)
	{
		//list head
		if(PosSeg.type < 4)
		{
			PosSeg.seg = 1;
		}
		else
		{
			PosSeg.seg = PosSeg.SegTotal;
		}
		buf[7] = PosSeg.type;
		crc = ShortEndianConvert(PosSeg.SegOffset);
		memcpy(&buf[8],(unsigned char *)(&crc),2);
		buf[10] = PosSeg.seg;
	}
	else
	{
		if(PosSeg.type > 3)
		{
			return;
		}
		if(PosLeng < SEG_STEP)
		{
			PosSeg.seg = PosLeng;
		}
		else
		{
			PosSeg.seg = SEG_STEP;
		}
		buf[7] = PosSeg.type;
		crc = ShortEndianConvert(PosSeg.SegOffset);
		memcpy(&buf[8],(unsigned char *)(&crc),2);
		buf[10] = PosSeg.seg;
	}
	crc = Crc16(&buf[1],10,0);
	crc = ShortEndianConvert(crc);
	memcpy(&buf[11],(unsigned char *)(&crc),2);
	len = AddFA(buf,13);
	memcpy(PosSndBuf,buf,len);
	PosCastLeng = len;
	if(DownLoadTmr == (Tmr *)0)
	{
		DownLoadTmr = CreateTimer(TIMER_ONE,POS_REQUE_TIMEOUT,ResetDownloadStatus);
	}
	ResetTimer(DownLoadTmr);
}


extern	PD_All PowerDown;

void PosMessage_Data(void)
{
  	unsigned char i;
  	unsigned short PosLeng;
  	unsigned char WriteListBuf[256];
 
  	if(DownLoadStatus == 3)
	{
		StopTimer(DownLoadTmr);
		if((PosSeg.type == Arm_g_posbuff[7]) && (PosSeg.seg == Arm_g_posbuff[10]))
		{
			if(PosSeg.SegOffset == 0)
			{
				//list head
				DestroyListIntegrality(PosSeg.type);
				if(PosSeg.type < 4)
				{
					memcpy(WriteListBuf,&Arm_g_posbuff[11],16);
					i = WriteListHead(PosSeg.type,WriteListBuf,16);
					if(!i)
					{
						if(PosSeg.SegTotal == 1)
						{
							DownLoadStatus = 0;
							i = WriteList(PosSeg.type,WriteListBuf,0,1);
							if(i)
							{
								ClearLcd(2,51,126,62);
								DisPlayHelp(WriteDataFlashErr);
								DisBufToLcd();
								SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));
								//写失败写日志
								//liuzg10
								while(1)
								{
									WatchDog();
								}
							}
							return;
						}
						DownLoadStatus = 2;
						PosSeg.SegOffset++;
						return;
					}
					else
					{
						ClearLcd(2,51,126,62);
						DisPlayHelp(WriteDataFlashErr);
						DisBufToLcd();
						SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));
						//写失败写日志
						//liuzg10
						while(1)
						{
							WatchDog();
						}
					}
				}
				else
				{
					PosLeng = Arm_g_posbuff[10];
					PosLeng <<= 4;
					memcpy(WriteListBuf,&Arm_g_posbuff[11],PosLeng);
					i = WriteList(PosSeg.type,WriteListBuf,PosLeng,1);
					if(!i)
					{
						PosSeg.SegOffset = PosSeg.SegTotal;
						if(PosSeg.type == 4)
						{
							ProcExchange[PROCESS_POS][PROCESS_CR] |= PRICE_RECV;
							SetPriceReceviceFlg();
						}
						DownLoadStatus = 0;
						return;
					}
					else if(i == (unsigned char)-1)
					{
						DownLoadStatus = 0;
						memset(CurrentInfoLine,0,sizeof(CurrentInfoLine));
						memcpy(InfoLine,PosError[0],strlen(PosError[0]) + 1);
						FillInfoMsg(M_InfoLine,0,0);
					}
					else
					{
						ClearLcd(2,51,126,62);
						DisPlayHelp(WriteDataFlashErr);
						DisBufToLcd();
						SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));
						//写失败写日志
						//liuzg10
						while(1)
						{
							WatchDog();
						}
					}
				}
			}
			else
			{
				PosLeng = Arm_g_posbuff[10];
				PosLeng <<= 4;	
				memcpy(WriteListBuf,&Arm_g_posbuff[11],PosLeng);
				if((PosSeg.SegOffset + Arm_g_posbuff[10]) >= PosSeg.SegTotal)
				{
					i = WriteList(PosSeg.type,WriteListBuf,PosLeng,1);
					if(!i)
					{
						DownLoadStatus = 0;
						return;
					}
					else
					{
						ClearLcd(2,51,126,62);
						DisPlayHelp(WriteDataFlashErr);
						DisBufToLcd();
						SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));
						//写失败写日志
						//liuzg10
						while(1)
						{
							WatchDog();
						}
					}
				}
				else
				{
					i = WriteList(PosSeg.type,WriteListBuf,PosLeng,0);
					if(!i)
					{
						PosSeg.SegOffset += Arm_g_posbuff[10];
						DownLoadStatus = 2;
						return;
					}
					else
					{
						ClearLcd(2,51,126,62);
						DisPlayHelp(WriteDataFlashErr);
						DisBufToLcd();
						SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));
						//写失败写日志
						//liuzg10
						while(1)
						{
							WatchDog();
						}
					}
				}
			}
		}
		else
		{
			DownLoadStatus = 2;
		}
	}
}



void PosMessage_Trade_TTC(void)
{
	unsigned int ttc;
	unsigned char buf[256];
	unsigned short len,crc;
	unsigned char i;
	
	buf[0] = 0xfa;
	buf[1] = Arm_g_posbuff[2];
	buf[2] = Arm_g_posbuff[1];
	buf[3] = Arm_g_posbuff[3];
	memcpy((unsigned char *)(&ttc),&Arm_g_posbuff[7],4);
	i = memcmp((unsigned char *)(&ttc),&RecordLast[1],4);
	if(!i)
	{
		buf[4] = 0x00;
		buf[5] = 0x96;
		buf[6] = ARM70_POS_STRIKE;
		memcpy(&buf[7],&RecordLast[1],95);
		crc = Crc16(&buf[1],101,0);
		crc = ShortEndianConvert(crc);
		memcpy(&buf[102],(unsigned char *)(&crc),2);
		len = AddFA(buf,104);
		memcpy(PosSndBuf,buf,len);
		PosCastLeng = len;
		PosReadRecordFlg |= POS_READ_RECORD;
		StopTimer(SndTransTmr);
		if(SndTransTmr == (Tmr *)0)
		{
			//SndTransTmr = CreateTimer(TIMER_ONE,POS_REQUE_TIMEOUT,ResetPosReadRecordFlg);
			//start liuzg20120706
			SndTransTmr = CreateTimer(TIMER_ONE,POS_REQUE_TIMEOUT,ResetTradeFlg);
			//end liuzg20120706
		}
		ResetTimer(SndTransTmr);
		return;
	}
	ttc = IntEndianConvert(ttc);
	i = FindRecordTTC(ttc,RecordLast);
	if(!i)
	{
		buf[6] = ARM70_POS_STRIKE;
		memcpy(&buf[7],&RecordLast[1],95);
		len = 96;
	}
	else
	{
		buf[6] = ARM70_POS_NO_STRICKE;
		ttc = IntEndianConvert(ttc);
		memcpy(&buf[7],(unsigned char *)(&ttc),4);
		len = 5;
	}
	HexToComBcd(len,&buf[4]);
	len += 5;
	crc = Crc16(&buf[1],len,0);
	crc = ShortEndianConvert(crc);
	len++;
	memcpy(&buf[len],(unsigned char *)(&crc),2);
	len += 2;
	len = AddFA(buf,len);
	memcpy(PosSndBuf,buf,len);
	PosCastLeng = len;
	if(!i)
	{
		PosReadRecordFlg |= POS_READ_RECORD;
		StopTimer(SndTransTmr);
		if(SndTransTmr == (Tmr *)0)
		{
			//SndTransTmr = CreateTimer(TIMER_ONE,POS_REQUE_TIMEOUT,ResetPosReadRecordFlg);
			//start liuzg20120706
			SndTransTmr = CreateTimer(TIMER_ONE,POS_REQUE_TIMEOUT,ResetTradeFlg);
			//end liuzg20120706
		}
		ResetTimer(SndTransTmr);
	}
}

void PosMessage_ReadPOSTrade()
{ 
	if(Arm_g_posbuff[7] == 0)
	{
		if(PosReadRecordFlg & POS_READ_RECORD)
		{
			PosReadRecordFlg &= (~POS_READ_RECORD);
			StopTimer(SndTransTmr);
		}
		else if(PosReadRecordFlg & LAST_TRANS_RECORD)
		{
			PosReadRecordFlg &= (~LAST_TRANS_RECORD);
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~TRANS_UPLOAD);
			StopTimer(SndTransTmr);
		}
		else if(PosReadRecordFlg & TRANS_UPLOADING)
		{
			PosReadRecordFlg &= (~TRANS_UPLOADING);
			StopTimer(SndTransTmr);
			if(!MakeOverRecord())
			{
				ReadTTCInc();
			}
		}
	}
	else
	{
		if(PosReadRecordFlg & POS_READ_RECORD)
		{
			PosReadRecordFlg &= (~POS_READ_RECORD);
			StopTimer(SndTransTmr);
		}
		else if(PosReadRecordFlg & LAST_TRANS_RECORD)
		{
			PosReadRecordFlg &= (~LAST_TRANS_RECORD);
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~TRANS_UPLOAD);
			StopTimer(SndTransTmr);
		}
		//T_MAC error
	}
}

