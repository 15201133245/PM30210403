/****************************************************************************
**�ļ����ƣ�Arm70_POS_main.c
**ʱ�䣺    2009.04.23
**��д�ˣ�  ������
**Verson��  00.00.01
**����ԭ�򣺴�������
**����ʱ�䣺2010-1-7
**�޸��ˣ�liuzg
*******************************************************************************
*/
//head area
#include "POS.h"
#include "define.h"
#include <string.h>
#include "board.h"


void  Arm70_POS_main()
{
	unsigned int old_time,current_time;
  //test       
//  if( !( ProcStatus[PROCESS_PUMP] & PUMP_ONLINE ))
// {
//	return;
//  }

  	SinopecToResolve(); 
	if(PosOnline)
	{
		ProcStatus[PROCESS_POS] |= POS_ONLINE;
	}
	else
	{
		ProcStatus[PROCESS_POS] &= (~POS_ONLINE);
	}
  	if(!PosOnline)
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
		//QueStatus = 0;
	}
  	if(PosFlg ==0)
  	{
   		return; 
  	}
	if(PosFlg)
	{
		old_time = timer_val.current_val;
		memcpy(Arm_g_posbuff,SinopecRevBuf.buf,SinopecRevBuf.len);
		PosFlg = 0;
		switch(Arm_g_posbuff[6])
		{
		case ARM70_PC_QUE:				//PC��ͨ��ѯ
			PosMessage_Polling();
			break;
		case ARM70_PC_STRIKE_ACK:		//PCӦ��ɾ�����
			PosMessage_ReadPOSTrade();
			break;
		case ARM70_PC_DATA_LENG:		//PC��Ӧ���ݳ���
			PosMessage_DataLeng();
			break;
		case ARM70_PC_APPLY_DATACON:	//PC��������
			PosMessage_Data();
			break;
		case ARM70_PC_QUE_ASHCARD:		//PC��Ӧ�Ҽ�¼��ѯ���
			PosMessage_GrayRecord();
			break;
		case ARM70_PC_QUE_BWCARD:		//PC��Ӧ�ڰ�������ѯ���
			PosMessage_BWL();		
			break;
		case ARM70_PC_READ_WMCOIL:		//PC��ȡ���ͻ�����
			if(IdleFlg & 0x04)
			{
				PosMessage_AskTotal();
			}
			break;
		case ARM70_PC_READ_WMCMESS:
			PosMessage_AskInfo();
			break;
		case ARM70_POS_ERROR:			//POSӦ����ͻ�����
			break;
		case ARM70_PC_ERROR_ACK:		//ͨѶ����ȷ��
			break;
		case ARM70_PC_READ_STRIKE:		//PC������ȡ��¼
			PosMessage_Trade_TTC();
			break;		
		default:
			break;
		}
	}
	if(PosCastLeng)
	{
		current_time = timer_val.current_val;
		if((current_time - old_time) <= 20)
		{
			WRITE(USART2_ID,PosSndBuf,0,PosCastLeng);
		}
		else if(PosReadRecordFlg & POS_READ_RECORD)
		{
			PosReadRecordFlg &= (~POS_READ_RECORD);
		}
		else if(PosReadRecordFlg & LAST_TRANS_RECORD)
		{
			PosReadRecordFlg &= (~LAST_TRANS_RECORD);
		}
		PosCastLeng = 0;
	}
}

void InitPosProcess(void)
{
	if(ReadPriceReceviceFlg())
	{
		ProcExchange[PROCESS_POS][PROCESS_CR] |= PRICE_RECV;
	}
}
 