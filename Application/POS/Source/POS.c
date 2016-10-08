/****************************************************************************
**文件名称：Arm70_POS_main.c
**时间：    2009.04.23
**编写人：  李立国
**Verson：  00.00.01
**更改原因：存在隐患
**更改时间：2010-1-7
**修改人：liuzg
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
		case ARM70_PC_QUE:				//PC普通查询
			PosMessage_Polling();
			break;
		case ARM70_PC_STRIKE_ACK:		//PC应答成就数据
			PosMessage_ReadPOSTrade();
			break;
		case ARM70_PC_DATA_LENG:		//PC回应数据长度
			PosMessage_DataLeng();
			break;
		case ARM70_PC_APPLY_DATACON:	//PC发送数据
			PosMessage_Data();
			break;
		case ARM70_PC_QUE_ASHCARD:		//PC回应灰记录查询结果
			PosMessage_GrayRecord();
			break;
		case ARM70_PC_QUE_BWCARD:		//PC回应黑白名单查询结果
			PosMessage_BWL();		
			break;
		case ARM70_PC_READ_WMCOIL:		//PC读取加油机总累
			if(IdleFlg & 0x04)
			{
				PosMessage_AskTotal();
			}
			break;
		case ARM70_PC_READ_WMCMESS:
			PosMessage_AskInfo();
			break;
		case ARM70_POS_ERROR:			//POS应答加油机错误
			break;
		case ARM70_PC_ERROR_ACK:		//通讯接受确认
			break;
		case ARM70_PC_READ_STRIKE:		//PC主动读取纪录
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
 