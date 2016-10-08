#include "POSMode2.h"
#include "define.h"
#include "message.h"

void PosCommandDelay(void)
{
	if(PosState == PosWaitDelay)
	{
		PosState = PosWaitDelayCom;
	}
	else if(PosState == PosReceivedDelay)
	{
		PosState = PosReceivedDelayCom;
	}
}

void PosTimerOut(void)
{
	if(PosState == PosWaitResponse)
	{
		PosState = PosWaitResponseTimeOut;
	}
}

void PosSndCommandTimeOut(void)
{
	if(PosState == PosWaitSnded)
	{
		SndPos.PosFlgSnded = 2;
	}
}

extern	void PosSndComplete(unsigned char *s,unsigned char val);

static unsigned char LastCommand;

void SndCommandToPos(unsigned char *p,unsigned short len)
{
	if(PosState == PosSending)
	{
		if(len != 0)
		{
			memcpy(PosSndBuf,p,len);
			SndPos.PosFlgSnded = 0;
			chCtrl[0].sendback = PosSndComplete;
			WRITE(USART2_ID,PosSndBuf,0,len);
			PosState = PosWaitSnded;   
			SndPos.SndPosNumber++;
			if(LastCommand != SndPos.CurState)
			{
				LastCommand = SndPos.CurState;
				SndPos.SndComTimeOutCount = 0;
			}
			if(PosSndTimeOut == (Tmr *)0)
			{
				PosSndTimeOut = CreateTimer(TIMER_ONE,POS_SNDCOM_TIMEOUT,PosSndCommandTimeOut);
			}
			else
			{
				ResetTimer(PosSndTimeOut);
			}
		}
	}
	else if(PosState == PosWaitSnded)
	{
		if(SndPos.PosFlgSnded == 1)
		{
			if(SndPos.CurState == RECORD_TRAN)
			{
				PosState = PosWaitDelay;
				if(DownLoadTmr == (Tmr *)0)
				{
					DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
				}
				else
				{
					ResetTimer(DownLoadTmr);
				}
			}
			else
			{
				PosState = PosWaitResponse;
				if(PosTmr == (Tmr *)0)
				{
					PosTmr = CreateTimer(TIMER_ONE,POS_RESPONSE_TIME,PosTimerOut);
				}
				else
				{
					ResetTimer(PosTmr);
				}
			}
		}
		else if(SndPos.PosFlgSnded == 2)
		{
			PosState = PosSending;
			SndPos.SndComTimeOutCount++;
			if(SndPos.SndComTimeOutCount >= 10)
			{
				InitM1172(1);
			}
		}
	}
	else if(PosState == PosWaitResponse)
	{
		if(PosFlg)
		{
			StopTimer(PosTmr);
			memcpy(Arm_g_posbuff,SinopecRevBuf.buf,SinopecRevBuf.len);
			PosFlg = 0;
			PosState = PosReceived;
			SndPos.SndCounts = 0;
		}
	}
	else if(PosState == PosWaitResponseTimeOut)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts++;
		SndPos.SndPosErrCounts++;
		if(SndPos.TimeOutCounts >= 10)
		{
											//liuzg_____ 自动从在线切换到离线模式
			//SetWorkMode(0);
			//liuzg ////20111020
			if(PosOnline != 0)
			{
				SetPosOffline();
			}
		}
	}
}

static unsigned  char CardOutFlg = 0;

unsigned char Pos_Polling(void)
{
	unsigned char val;
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	
	if(PosState == PosSending)
	{
		if((ProcExchange[PROCESS_CR][PROCESS_POS] & SENDCARDIN) && (ProcStatus[PROCESS_CR] & CARD_READY))
		{
			CardOutFlg = 0;
			if(SendCountToPos < 3)
			{
				buf[0] = 0xfa;
				buf[1] = SndPos.POS_P;
				buf[2] = SndPos.CurAdr;
				buf[3] = SndPos.MessNum;
				buf[4] = 0;
				buf[5] = 0x23;
				buf[6] = NOZZLE_ST_DATA;
				buf[7] = POS_CR_Buf.TransNozzleNumber;
				if(ProcStatus[PROCESS_CR] & CARD_BUSY)
				{
					buf[8] = 2;
				}
				else
				{
					buf[8] = 1;
				}
				memcpy(&buf[9],POS_CR_Buf.ICCARD_ASN,10);
				buf[19] = POS_CR_Buf.P_UNIT;              
				memset(&buf[20],0,3);
				memset(&buf[23],0,3);
				memcpy(&buf[26],&POS_CR_Buf.TransPrice,2);
				buf[28] = GetAuthMode();							//liuzg|| 授权模式，整合是添加
				crc = Crc16(&buf[1],28,0);
				crc = ShortEndianConvert(crc);
				memcpy(&buf[29],(unsigned char *)(&crc),2);
				SndLen = AddFA(buf,31);
				SendCountToPos++;
				SndPos.CurState = NOZZLE_ST_DATA;
			}
			else
			{
				SendCountToPos = 0;
				ProcExchange[PROCESS_CR][PROCESS_POS] &= (~SENDCARDIN);
				SndPos.CurState = POLL;
			}
		}
		else if(ProcExchange[PROCESS_CR][PROCESS_POS] & SENDEOT)
		{
			if(SendCountToPos < 3)
			{
				buf[0] = 0xfa;
				buf[1] = SndPos.POS_P;
				buf[2] = SndPos.CurAdr;
				buf[3] = SndPos.MessNum;
				buf[4] = 0;
				buf[5] = 0x23;
				buf[6] = NOZZLE_ST_DATA;
				buf[7] = POS_CR_Buf.TransNozzleNumber;
				buf[8] = 2;
				memcpy(&buf[9],POS_CR_Buf.ICCARD_ASN,10);
				buf[19] = POS_CR_Buf.P_UNIT;              
				memcpy(&buf[20],&POS_CR_Buf.TransAmount[1],3);
				memcpy(&buf[23],&POS_CR_Buf.TransVolume[1],3);
				memcpy(&buf[26],&POS_CR_Buf.TransPrice,2);
				buf[28] = GetAuthMode();							//liuzg|| 授权模式，整合是添加
				crc = Crc16(&buf[1],28,0);
				crc = ShortEndianConvert(crc);
				memcpy(&buf[29],(unsigned char *)(&crc),2);
				SndLen = AddFA(buf,31);
				SendCountToPos++;
				SndPos.CurState = NOZZLE_ST_DATA;
			}
			else
			{
				SendCountToPos = 0;
				ProcExchange[PROCESS_CR][PROCESS_POS] &= (~SENDEOT);
				SndPos.CurState = POLL;
				SndPos.CurState = NOZZLE_ST_DATA;
			}
		}
		else if((ProcStatus[PROCESS_CR] & CARD_BUSY) && (SndPos.PreState == POLL))
		{
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 0x23;
			buf[6] = NOZZLE_ST_DATA;
			buf[7] = POS_CR_Buf.TransNozzleNumber;
			buf[8] = 2;
			memcpy(&buf[9],POS_CR_Buf.ICCARD_ASN,10);
			buf[19] = POS_CR_Buf.P_UNIT;              
			memcpy(&buf[20],&POS_CR_Buf.TransAmount[1],3);
			memcpy(&buf[23],&POS_CR_Buf.TransVolume[1],3);
			memcpy(&buf[26],&POS_CR_Buf.TransPrice,2);
			buf[28] = GetAuthMode();							//liuzg|| 授权模式，整合是添加
			crc = Crc16(&buf[1],28,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[29],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,31);
			SendCountToPos++;
			SndPos.CurState = NOZZLE_ST_DATA;
		}
		else if((!CardOutFlg) && (!(ProcStatus[PROCESS_CR] & CARD_READY)))
		{
			CardOutFlg = 1;
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 0x23;
			buf[6] = NOZZLE_ST_DATA;
			buf[7] = POS_CR_Buf.TransNozzleNumber;
			buf[8] = 1;
			memset(&buf[9],0,10);
			buf[19] = POS_CR_Buf.P_UNIT;              
			memset(&buf[20],0,3);
			memset(&buf[23],0,3);
			memcpy(&buf[26],&POS_CR_Buf.TransPrice,2);
			buf[28] = GetAuthMode();							//liuzg|| 授权模式，整合是添加
			crc = Crc16(&buf[1],28,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[29],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,31);
			//SendCountToPos++;
//=====================================================; Bellow LLLLC
//			SndPos.CurState = NOZZLE_ST_DATA;
			SndPos.CurState = POLL;
//=====================================================; Upper LLLLC
		}
		else
		{
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 2;
			buf[6] = POLL;
			val = 0;
			if(ProcStatus[PROCESS_CR] & CARD_LOGON)
			{
				val |= 0x02;
				if(GetAuthMode())
				{
//====================================; Bellow LLLLO
//					val |= 0x01;
//====================================; Upper LLLLO
//====================================; Bellow LLLLA
					if( GetWorkMode() == 0x01 )
					{
						val |= 0x01;
					}
					else
					{
						// For Offline, b0 Of val Must Be 0
					}
//====================================; Upper LLLLA
				}
			}
			else
			{
				val |= 0x01;
			}
			
			buf[7] = val;					//liuzg||  支付点工作模式，需要整合是填写
			crc = Crc16(&buf[1],7,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[8],(unsigned char *)(&crc),2);
			SndLen = 10;
			SndLen = AddFA(buf,SndLen);
			if(SndPos.Download == 2)
			{
				SndPos.Download = 1;
			}
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_AuthReq1(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	
	if(PosState == PosSending)
	{
		if(SndPos.SndCounts < COMMNAD_MAX_COUNT)
		{
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 0x86;
			buf[6] = AUTH_REQ1;
			//liuzg||          需要整合是填写
			//NZN 枪号 －－－						1
			buf[7] = POS_CR_Buf.TransNozzleNumber;
			//POS-TTC 交易流水号－－－				4
			memcpy(&buf[8],POS_CR_Buf.Auth_TTC,4);
			//Dispenser_TIME 加油机时间－－－		7
			memcpy(&buf[12],POS_CR_Buf.Auth_Date,7);
			//Pay_X 付款方式/地点－－－				1
			buf[19] = POS_CR_Buf.Auth_Pay_X;
			//PRC 价格－－－						2
			memcpy(&buf[20],POS_CR_Buf.TransPrice,2);
			//BAL 卡钱包余额－－－					4
			memcpy(&buf[22],POS_CR_Buf.ICCARD_Balance,4);
			//Add-ons 卡附加信息－－－				30
			memcpy(&buf[26],POS_CR_Buf.Auth_Add_On,30);
			//PIN_block 联机PIN数据－－－			16
			memcpy(&buf[56],POS_CR_Buf.Auth_PIN_Block,16);
			//AMN 请求授权金额－－－				3
			memcpy(&buf[72],&POS_CR_Buf.TransAmount[1],3);
			//VOL 请求授权升数－－－				3
			memcpy(&buf[75],&POS_CR_Buf.TransVolume[1],3);
			//ASN 卡号－－－						10
			memcpy(&buf[78],POS_CR_Buf.ICCARD_ASN,10);
			//MAC 消息鉴别码－－－					4
			memcpy(&buf[88],POS_CR_Buf.Auth_MAC,4);
			
			crc = Crc16(&buf[1],91,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[92],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,94);
			SndPos.SndCounts++;
		}
		else
		{										//liuzg|| 请求授权时，3次未得到响应。
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_CST); 	
			ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_CST;	 	
			POS_CR_Buf.CheckFlag = 0x02;   
			SndPos.CurState = POLL;
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_AuthReq2(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	
	if(PosState == PosSending)
	{
		if(SndPos.SndCounts < COMMNAD_MAX_COUNT)
		{
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 0x36;
			buf[6] = AUTH_REQ2;
			//liuzg|| 需要整合时填写
			//NZN 枪号－－－							1
			buf[7] = POS_CR_Buf.TransNozzleNumber;
			//POS-TTC 交易流水号－－－					4
			memcpy(&buf[8],POS_CR_Buf.Auth_TTC,4);
			//Dispenser_TIME 加油机时间－－－			7
			memcpy(&buf[12],POS_CR_Buf.Auth_Date,7);
			//Pay_X 付款方式－－－						1
			buf[19] = POS_CR_Buf.Auth_Pay_X;
			//PRC 价格－－－							2
			memcpy(&buf[20],POS_CR_Buf.TransPrice,2);
			//AMN 请求授权金额－－－					3
			memcpy(&buf[22],&POS_CR_Buf.TransAmount[1],3);
			//VOL 请求授权升数－－－					3
			memcpy(&buf[25],&POS_CR_Buf.TransVolume[1],3);
			//ASN 卡号－－－							10
			memcpy(&buf[28],POS_CR_Buf.ICCARD_ASN,10);
			//MAC 消息鉴别码－－－						4
			memcpy(&buf[38],POS_CR_Buf.Auth_MAC,4);
			
			crc = Crc16(&buf[1],41,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[42],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,44);
			SndPos.SndCounts++;
		}
		else
		{									//liuzg|| 请求授权时，3次未得到响应。
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_EMP); 	
			ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_EMP;	 	
			POS_CR_Buf.CheckFlag = 0x02;  
			SndPos.CurState = POLL;
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_NozzleDownData(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	
	if(PosState == PosSending)
	{
		if(SndPos.SndCounts < COMMNAD_MAX_COUNT)
		{
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 0x44;
			buf[6] = NOZZLE_DOWN_DATA;
			
			buf[7] = POS_CR_Buf.TransNozzleNumber;
			memcpy(&buf[8],POS_CR_Buf.Auth_TTC,4);
			memcpy(&buf[12],POS_CR_Buf.Auth_Date,7);
			memcpy(&buf[19],POS_CR_Buf.Grade_Number,2);
			buf[21] = POS_CR_Buf.Auth_Pay_X;
			memcpy(&buf[22],POS_CR_Buf.TransPrice,2);
			memcpy(&buf[24],POS_CR_Buf.Card_CTC,2);
			memcpy(&buf[26],POS_CR_Buf.VolumeTotal,4);
			memcpy(&buf[30],&POS_CR_Buf.TransAmount[1],3);
			memcpy(&buf[33],&POS_CR_Buf.TransVolume[1],3);
			memcpy(&buf[36],POS_CR_Buf.ICCARD_ASN,10);
			memcpy(&buf[46],POS_CR_Buf.Auth_MAC,4);
			
			crc = Crc16(&buf[1],49,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[50],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,52);
			SndPos.SndCounts++;
		}
		else
		{
			POS_CR_Buf.CheckFlag = 2;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~FUELLING_DATA);
			ProcExchange[PROCESS_POS][PROCESS_CR] |= FUELLING_DATA;  
			SndPos.CurState = POLL;
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_FbCardRecord(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	unsigned char num;
	unsigned short len;
	
	if(PosState == PosSending)
	{
		if(SndPos.SndCounts < COMMNAD_MAX_COUNT)
		{
			num = POS_CR_Buf.InforMessage[0];
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			len = (num & 0x0f)* 23 + 1;
			HexToComBcd(len + 11,&buf[4]);
			buf[6] = FB_CARD_RECORD;
			memcpy(&buf[7],POS_CR_Buf.ICCARD_ASN,10);
			memcpy(&buf[17],POS_CR_Buf.InforMessage,len);
			crc = Crc16(&buf[1],len + 16,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[len + 17],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,len + 19);
			SndPos.SndCounts++;
		}
		else
		{
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~CARD_RECORD);
			ProcExchange[PROCESS_POS][PROCESS_CR] |= CARD_RECORD;
			POS_CR_Buf.CheckFlag = 0x02;  
			SndPos.CurState = POLL;
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_GrayRecord(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	
	if(PosState == PosSending)
	{
		if(SndPos.SndCounts < COMMNAD_MAX_COUNT)
		{
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 0x25;
			buf[6] = DEMAND_GRAY_RECORD;
			memcpy(&buf[7],GreyInquiry.GreyCardASN,24);
			crc = Crc16(&buf[1],30,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[31],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,33);
			SndPos.SndCounts++;
		}
		else
		{
			GreyResponse.CheckFlag = 0x02;
			ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_GREY;
			ProcExchangePos &= (~POS_CHECK_GREY);  
			SndPos.CurState = POLL;
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_BWListCheck(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	
	if(PosState == PosSending)
	{
		if(SndPos.SndCounts < COMMNAD_MAX_COUNT)
		{
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 0x11;
			buf[6] = BWLIST_CHECK;
			memcpy(&buf[7],POS_CR_Buf.ICCARD_ASN,10);
			crc = Crc16(&buf[1],16,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[17],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,19);
		}
		else
		{
			POS_CR_Buf.CheckFlag = 0x02; 
			SndPos.CurState = POLL;
			if(ProcExchange[PROCESS_POS][PROCESS_CR] & POS_CHECK_BLACK)
			{
				ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_BLACK;
				ProcExchangePos &= (~POS_CHECK_BLACK);
			}
			else if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_WHITE)
			{
				ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_WHITE;
				ProcExchangePos &= (~POS_CHECK_WHITE);
			}
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_VTot(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	unsigned char i,count;
	
	if(PosState == PosSending)
	{
		if(SndPos.SndCounts < COMMNAD_MAX_COUNT)
		{
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			count = GetGUN_N();
			buf[6] = V_TOT;
			buf[7] = count;
			for(i = 0; i < count; i++)
			{
				buf[8 + i * 5] = GetLogicNzn(i);
				memcpy(&buf[9 + i * 5],DISP_CR_Buf.DispTotal[i].VolumeTotal,4);
			}
			SndLen = 5 * count + 2;
			HexToComBcd(SndLen,&buf[4]);
			SndLen += 5;
			crc = Crc16(&buf[1],SndLen,0);
			SndLen ++;
			crc = ShortEndianConvert(crc);
			memcpy(&buf[SndLen],(unsigned char *)(&crc),2);
			SndLen += 2;
			SndLen = AddFA(buf,SndLen);
		}
		else
		{  
			SndPos.CurState = POLL;
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_DataLeng(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	
	if(PosState == PosSending)
	{
		if(SndPos.SndCounts < COMMNAD_MAX_COUNT)
		{
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 0x04;
			buf[6] = REQ_DATA;
			buf[7] = PosToPosBuf[0];
			memcpy(&buf[8],BaseBlackVer,2);
			crc = Crc16(&buf[1],9,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[10],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,12);
			SndPos.SndCounts++;
		}
		else
		{
			SndPos.CurState = POLL;
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_Data(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	unsigned int PosLeng;
	
	if(PosState == PosSending)
	{
		if(SndPos.SndCounts < COMMNAD_MAX_COUNT)
		{
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 0x05;
			buf[6] = REQ_CONT;
			PosLeng = PosSeg.SegTotal - PosSeg.SegOffset;
			if(PosSeg.SegOffset == 0)
			{
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
					SndPos.CurState = POLL;
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
			SndLen = AddFA(buf,13);
			SndPos.SndCounts++;
		}
		else
		{
			SndPos.Download = 2;
			SndPos.CurState = POLL;
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_DispenserInfo(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	unsigned char j,count;
	int tmp;
	
	if(PosState == PosSending)
	{
		buf[0] = 0xfa;
		buf[1] = SndPos.POS_P;
		buf[2] = SndPos.CurAdr;
		buf[3] = SndPos.MessNum;
		buf[6] = DISPENSER_INFO;
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
		SndLen = 31 + count;
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
		SndLen += 7;
		//厂商自定义信息－－暂时无
		HexToComBcd(SndLen,&buf[4]);
		SndLen += 5;
		crc = Crc16(&buf[1],SndLen,0);
		crc = ShortEndianConvert(crc);
		memcpy(&buf[44 + count],(unsigned char *)(&crc),2);
		SndLen += 3;
		SndLen = AddFA(buf,SndLen);
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_AuthReq3(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	
	if(PosState == PosSending)
	{
		if(SndPos.SndCounts < COMMNAD_MAX_COUNT)
		{
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 0x36;
			buf[6] = AUTH_REQ3;
			//liuzg|| 需要整合时填写
			//NZN 枪号－－－							1
			buf[7] = POS_CR_Buf.TransNozzleNumber;
			//POS-TTC 交易流水号－－－					4
			memcpy(&buf[8],POS_CR_Buf.Auth_TTC,4);
			//Dispenser_TIME 加油机时间－－－			7
			memcpy(&buf[12],POS_CR_Buf.Auth_Date,7);
			//Pay_X 付款方式－－－						1
			buf[19] = POS_CR_Buf.Auth_Pay_X;
			//PRC 价格－－－							2
			memcpy(&buf[20],POS_CR_Buf.TransPrice,2);
			//AMN 请求授权金额－－－					3
			memcpy(&buf[22],&POS_CR_Buf.TransAmount[1],3);
			//VOL 请求授权升数－－－					3
			memcpy(&buf[25],&POS_CR_Buf.TransVolume[1],3);
			//ASN 卡号－－－							10
			memcpy(&buf[28],POS_CR_Buf.ICCARD_ASN,10);
			//MAC 消息鉴别码－－－						4
			memcpy(&buf[38],POS_CR_Buf.Auth_MAC,4);
			
			crc = Crc16(&buf[1],41,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[42],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,44);
			SndPos.SndCounts++;
		}
		else
		{										//liuzg|| 请求授权时，3次未得到响应。
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_NCSS); 	
			ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_NCSS;	 	
			POS_CR_Buf.CheckFlag = 0x02;     
			SndPos.CurState = POLL;
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_TranData(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc;
	
	if(PosState == PosSending)
	{
		buf[0] = 0xfa;
		buf[1] = SndPos.POS_P;
		buf[2] = SndPos.CurAdr;
		buf[3] = SndPos.MessNum;
		buf[4] = 0;
		buf[5] = 0x96;     
		ReadRecord(&buf[6]);
		buf[6] = TRAN_DATA;
		crc = Crc16(&buf[1],101,0);
		crc = ShortEndianConvert(crc);
		memcpy(&buf[102],(unsigned char *)(&crc),2);
		SndLen = AddFA(buf,104);
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

unsigned char Pos_RecordTran(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc; 
	unsigned char flg;
	
	if(PosState == PosSending)
	{
		flg = FindRecordTTC(PosReadTTC,&buf[6]); 
		if(flg)
		{   
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 0x04;
			buf[6] = CMD_ACK;
			buf[7] = CB_RECORD;
			buf[8] = 0;
			buf[9] = 3;
			crc = Crc16(&buf[1],9,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[10],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,12);
		}
		else
		{ 
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			buf[5] = 0x96;
			FindRecordTTC(PosReadTTC,&buf[6]);  
			buf[6] = RECORD_TRAN; 
			crc = Crc16(&buf[1],101,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[102],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,104);  
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}

char const Pos_Err[] = "FS在后台授权时,加油机提交加油数据,FS未回应支付数据!!!";

unsigned char Pos_ErrorSnd(void)
{
	unsigned char retval = 1;
	unsigned char buf[256];
	unsigned short SndLen = 0;
	unsigned short crc; 
	unsigned char j;
	
	if(PosState == PosSending)
	{
		if(SndPos.SndCounts < COMMNAD_MAX_COUNT)
		{
			buf[0] = 0xfa;
			buf[1] = SndPos.POS_P;
			buf[2] = SndPos.CurAdr;
			buf[3] = SndPos.MessNum;
			buf[4] = 0;
			j = strlen(Pos_Err) + 1;
			buf[5] = j + 2;
			buf[6] = ERROR_INFO;
			buf[7] = 0x01;
			memcpy(&buf[8],Pos_Err,j);
			crc = Crc16(&buf[1],j + 7,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[j + 8],(unsigned char *)(&crc),2);
			SndLen = AddFA(buf,j + 10);  
		}
		else
		{
			ProcExchange[PROCESS_POS][PROCESS_POS] &= (~POS_ERROR);
			SndPos.CurState = POLL;
		}
	}
	SndCommandToPos(buf,SndLen);
	if(PosState == PosReceived)
	{
		PosState = PosReceivedDelay;
		if(DownLoadTmr == (Tmr *)0)
		{
			DownLoadTmr = CreateTimer(TIMER_ONE,POS_COMMAND_DELAY,PosCommandDelay);
		}
		else
		{
			ResetTimer(DownLoadTmr);
		}
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
		retval = 0;
	}
	else if(PosState == PosReceivedDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
	}
	else if(PosState == PosWaitDelayCom)
	{
		PosState = PosSending;
		SndPos.CurState = POLL;
		SndPos.TimeOutCounts = 0;
		SndPos.MessNum++;
		SndPos.MessNum &= 0x3f;
		SndPos.MessNum |= 0x40;
	}
	return(retval);
}