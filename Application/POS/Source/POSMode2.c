#include "POSMODE2.h"
#include "define.h"
#include <string.h>
#include "board.h"
#include "PdProtect.h"

#define BASE_BLACK_LIST_LIMIT		20000 * 10 + 16
#define LIST_LIMIT					200 * 10 + 16

SndPosStruct_t SndPos;
Tmr *PosTmr;
enum POS_COMMAND_STATUS PosState;
unsigned char AckFlg;

void PosErrCmdTimeOut(void)
{
	if(ProcExchangePos & FUELLING_DATA_HOST)
	{
		ProcExchangePos &= (~FUELLING_DATA_HOST);
		ProcExchange[PROCESS_POS][PROCESS_POS] |= POS_ERROR;
		PosErrTmr = (Tmr *)0;
	}
}

void PosAckCmdTimeOut(void)
{
	PosAckTmr = (Tmr *)0;
	if(SndPos.PreState == AUTH_REQ1)
	{
		//DeleteTimer(PosAckTmr);
		//liuzg|| 通知ICCARD进程授权超时
		POS_CR_Buf.CheckFlag = 2;
		ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_CST;
		
		SndPos.PreState = POLL;
	}
	else if(SndPos.PreState == AUTH_REQ2)
	{
		//PosAckTmr = (Tmr *)0;
		//DeleteTimer(PosAckTmr);
		//liuzg|| 通知ICCARD进程授权超时
		POS_CR_Buf.CheckFlag = 2;
		ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_EMP;
		
		SndPos.PreState = POLL;
	} 
	else if(SndPos.PreState == AUTH_REQ3)
	{
		//PosAckTmr = (Tmr *)0;
		//DeleteTimer(PosAckTmr);
		//liuzg|| 通知ICCARD进程授权超时
		POS_CR_Buf.CheckFlag = 2;
		ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_NCSS;
		
		SndPos.PreState = POLL;
	}
	else if(SndPos.PreState == NOZZLE_DOWN_DATA)
	{
		//PosAckTmr = (Tmr *)0;
		//DeleteTimer(PosAckTmr);
		POS_CR_Buf.CheckFlag = 2;
		ProcExchange[PROCESS_POS][PROCESS_CR] |= FUELLING_DATA;
		
		SndPos.PreState = POLL;
	}  
	else if(SndPos.PreState == DEMAND_GRAY_RECORD)
	{   
		//PosAckTmr = (Tmr *)0;
		//DeleteTimer(PosAckTmr);
		POS_CR_Buf.CheckFlag = 2;
		ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_GREY;
		ProcExchangePos &= (~POS_CHECK_GREY);
		SndPos.PreState = POLL;
	} 
	else if(SndPos.PreState == BWLIST_CHECK)
	{   
		//PosAckTmr = (Tmr *)0;
		//DeleteTimer(PosAckTmr);
		POS_CR_Buf.CheckFlag = 2; 
		if(ProcExchangePos & POS_CHECK_BLACK)
		{       
			ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_BLACK;  
			ProcExchangePos &= (~POS_CHECK_BLACK);
		}
		if(ProcExchangePos & POS_CHECK_WHITE)
		{   
			ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_WHITE; 
			ProcExchangePos &= (~POS_CHECK_WHITE);
		}
		
		SndPos.PreState = POLL;
	}
}

void PosSndComplete(unsigned char *s,unsigned char val)
{
	SndPos.PosFlgSnded = 1;
}

void AckAnsComplete(unsigned char *s,unsigned char val)
{
	AckFlg = 1;
}

void SndCmdAck(unsigned char command,unsigned char nzn,unsigned char val)
{
	unsigned char buf[64];
	unsigned short SndLen;
	unsigned short crc;
	
	AckFlg = 0;
	chCtrl[0].sendback = AckAnsComplete;
	
	buf[0] = 0xfa;
	buf[1] = SndPos.POS_P;
	buf[2] = SndPos.CurAdr;
	buf[3] = SndPos.MessNum;
	buf[4] = 0;
	buf[5] = 0x04;
	buf[6] = CMD_ACK;
	buf[7] = command;
	buf[8] = nzn;
	buf[9] = val;
	crc = Crc16(&buf[1],9,0);
	crc = ShortEndianConvert(crc);
	memcpy(&buf[10],(unsigned char *)(&crc),2);
	SndLen = AddFA(buf,12);
	memcpy(PosSndBuf,buf,SndLen);
	WRITE(USART2_ID,PosSndBuf,0,SndLen);
	while(!AckFlg)
	{
		SpiUsart();
	}
	chCtrl[0].sendback = PosSndComplete;
}

void PosMessage_VerInfo(void)
{
	unsigned char listtype[7];
	unsigned char i;
	
	AdjustSystime(&Arm_g_posbuff[7],0);
	if((SndPos.PreState == POLL) && (GetWorkMode()))
	{
		if(SndPos.Download == 0)
		{
			memcpy(listtype,&Arm_g_posbuff[14],7);
			i = CheckListVersion(listtype);
			if(i)
			{
				memcpy(BaseBlackVer,&Arm_g_posbuff[14],2);
				PosToPosBuf[0] = listtype[0];
				ProcExchange[PROCESS_POS][PROCESS_POS] |= ASK_POS_DATALENG;
			}
		}
		else if(SndPos.Download == 1)
		{
			ProcExchange[PROCESS_POS][PROCESS_POS] |= ASK_POS_DATA;
		}
	}
}

void PosMessage_AuthFB(void)
{
	unsigned char len;
	
	if((SndPos.PreState == AUTH_REQ1) || (SndPos.PreState == AUTH_REQ2) || (SndPos.PreState == AUTH_REQ3))
	{
		DeleteTimer(PosAckTmr); 
		PosAckTmr = (Tmr *)0;
		SndCmdAck(AUTH_FB,Arm_g_posbuff[7],1);
		//liuzg|| 通知ICCARD进程后台授权结果，整合时填写
		POS_CR_Buf.CheckFlag = 0;
		
		POS_CR_Buf.TransNozzleNumber = Arm_g_posbuff[7];
		
		memcpy(POS_CR_Buf.Auth_TTC,&Arm_g_posbuff[8],4);
		
		POS_CR_Buf.Auth_Pay_X = Arm_g_posbuff[12];
		
		memcpy(POS_CR_Buf.TransPrice,&Arm_g_posbuff[13],2);
		
		len = Arm_g_posbuff[15];
		memcpy(POS_CR_Buf.Auth_Add_On,&Arm_g_posbuff[15],len + 1);
		
		memcpy(&POS_CR_Buf.TransAmount[1],&Arm_g_posbuff[len + 16],3);
		
		memcpy(POS_CR_Buf.ICCARD_ASN,&Arm_g_posbuff[len + 19],10);
		
		POS_CR_Buf.Auth_Resp = Arm_g_posbuff[len + 29];
		
		memcpy(POS_CR_Buf.Auth_MAC,&Arm_g_posbuff[len + 30],4);
		if(SndPos.PreState == AUTH_REQ1)
		{
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_CST);
			ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_CST;
		}
		else if(SndPos.PreState == AUTH_REQ2)
		{
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_EMP);
			ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_EMP;
		}
		else if(SndPos.PreState == AUTH_REQ3)
		{
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_NCSS);
			ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_NCSS;
		}
		SndPos.PreState = POLL;
	}
	else
	{    
		SndCmdAck(AUTH_FB,Arm_g_posbuff[7],4);
	}
}

void PosMessage_AuthMode(void)
{
	unsigned char nzn;
	
	nzn = Arm_g_posbuff[7];    

	//liuzg--
    if(Arm_g_posbuff[8] == 0xff)
	{
		Arm_g_posbuff[8] = 0;
	}
	//liuzg--
	if((SndPos.PreState == POLL) && (SndPos.CurState == POLL))
	{
		if(GetWorkMode())
		{    
			if(GetAuthMode() == Arm_g_posbuff[8])
			{
				SndCmdAck(AUTH_MODE,nzn,4);
			}
			else
			{
				if(ProcStatus[PROCESS_CR] & CARD_AUTHED)
				{
					SndCmdAck(AUTH_MODE,nzn,4);
				}
				else
				{
					//liuzg|| 授权模式切换，整合时填写
					SndCmdAck(AUTH_MODE,nzn,1);
					SetAuthMode(Arm_g_posbuff[8]);
				}
			}
		}
		else
		{
			SndCmdAck(AUTH_MODE,nzn,4);
		}
	}
	else
	{
		SndCmdAck(AUTH_MODE,nzn,4);
	}
}

void PosMessage_Stop(void)
{
	unsigned char nzn;
	
	nzn = Arm_g_posbuff[7];
	if((SndPos.CurState == POLL) || (SndPos.CurState == NOZZLE_ST_DATA)
	   || (SndPos.CurState == AUTH_REQ1) || (SndPos.CurState == AUTH_REQ2)
		   ||(SndPos.CurState == AUTH_REQ3))
	{
		if(GetWorkMode())
		{
			if(ProcStatus[PROCESS_CR] & CARD_READY)
			{    
				SndCmdAck(STOP,nzn,1);
				POS_CR_Buf.CheckFlag = 0;
				POS_CR_Buf.TransNozzleNumber = nzn;
				memcpy(POS_CR_Buf.Auth_TTC,&Arm_g_posbuff[8],4);
				memcpy(POS_CR_Buf.Auth_Date,&Arm_g_posbuff[12],7);
				if(SndPos.CurState == AUTH_REQ1)
				{
					ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_CST);
				}
				else if(SndPos.CurState == AUTH_REQ2)
				{
					ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_EMP);
				}
				else if(SndPos.CurState == AUTH_REQ3)
				{ 
					ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_NCSS);
				}
				ProcExchange[PROCESS_POS][PROCESS_CR] |= HOST_STOP;
				SndPos.CurState = POLL;
			}
//=========================================; Bellow LLLLA 2011-1102
			else
			{
				SndCmdAck(STOP,nzn,4);
			}
//=========================================; Upper LLLLA 2011-1102
		}
		else
		{
			SndCmdAck(STOP,nzn,4);
		}
	}
	else
	{
		SndCmdAck(STOP,nzn,4);
	}
}

void PosMessage_PaymentData(void)
{
	unsigned char nzn;
	
	nzn = Arm_g_posbuff[7];
	if(SndPos.PreState == NOZZLE_DOWN_DATA)
	{
		SndCmdAck(PAYMENT_DATA,nzn,1);
		
		POS_CR_Buf.CheckFlag = 0;
		POS_CR_Buf.TransNozzleNumber = Arm_g_posbuff[7];
		memcpy(POS_CR_Buf.Auth_TTC,&Arm_g_posbuff[8],4);
		POS_CR_Buf.Auth_Pay_X = Arm_g_posbuff[12];
		memcpy(&POS_CR_Buf.TransAmount[1],&Arm_g_posbuff[13],3);
		memcpy(&POS_CR_Buf.TransAmount1[1],&Arm_g_posbuff[16],3);
		memcpy(&POS_CR_Buf.TransAmount2[1],&Arm_g_posbuff[19],3);
		memcpy(POS_CR_Buf.DiscountNumber,&Arm_g_posbuff[22],2);
		memcpy(POS_CR_Buf.ICCARD_ASN,&Arm_g_posbuff[24],10);
		memcpy(POS_CR_Buf.Auth_MAC,&Arm_g_posbuff[34],4);
		ProcExchange[PROCESS_CR][PROCESS_POS] &= (~FUELLING_DATA);
		ProcExchange[PROCESS_POS][PROCESS_CR] |= FUELLING_DATA; 
		SndPos.PreState = POLL;  
		SndPos.CurState = POLL;
	}
    else
	{
		if((SndPos.CurState == POLL) && (ProcExchangePos & FUELLING_DATA_HOST))
		{
			ProcExchangePos &= (~FUELLING_DATA_HOST);
			DeleteTimer(PosAckTmr);
			PosAckTmr = (Tmr *)0;
		}
		SndCmdAck(PAYMENT_DATA,nzn,4);
	}
}

void PosMessage_PriceChange(void)
{
	unsigned short len;
	unsigned char buf[64];
	
	len = Arm_g_posbuff[7];
	len *= 5;
	len += 12;
	memcpy(buf,&Arm_g_posbuff[7],len);
	if(SndPos.CurState == POLL)
	{
		if(GetWorkMode())
		{
			if(ProcStatus[PROCESS_CR] & CARD_BUSY)
			{
				SndCmdAck(PRICE_CHANGE,0,4);
			}
			else
			{
				WritePosPrice(buf,len);
//==========================================================; Bellow LLLLO
//				SndCmdAck(PRICE_CHANGE,0,1);
//==========================================================; Upper LLLLO
//==========================================================; Bellow LLLLA
				if( Arm_g_posbuff[7] == 0x01 )
				{
					// Only 1 Nozzle
					SndCmdAck(PRICE_CHANGE,Arm_g_posbuff[15],4);
				}
				else
				{
					SndCmdAck(PRICE_CHANGE,0,4);
				}
//==========================================================; Upper LLLLA
				//ProcExchange[PROCESS_POS][PROCESS_CR] |= 0x4000;   //liuzg-- 待定
			}
		}
		else
		{
			SndCmdAck(PRICE_CHANGE,0,4);
		}
//==========================================================; Bellow LLLLA
// Download Price Next
		GradePrice.Ver--;
//==========================================================; Upper LLLLA

	}
}

void PosMessage_CbRecord(void)
{
	if(SndPos.PreState == POLL)
	{   
		memcpy((unsigned char *)(&PosReadTTC),&Arm_g_posbuff[7],4);    
		PosReadTTC = IntEndianConvert(PosReadTTC); 
		ProcExchange[PROCESS_POS][PROCESS_POS] |= POS_ASK_TRAN;
	}
}

void Pos2Message_GrayRecord(void)
{
	if((SndPos.CurState == DEMAND_GRAY_RECORD) || (SndPos.PreState == DEMAND_GRAY_RECORD))
	{
		if(ProcExchangePos & POS_CHECK_GREY  )
		{     
			SndPos.PreState = POLL;
			SndPos.CurState = POLL;  
			GreyResponse.CheckFlag = Arm_g_posbuff[7] & 0x01;
			if(GreyResponse.CheckFlag == 0)
			{
				//
				memcpy(GreyResponse.GreyCardASN,&Arm_g_posbuff[8],17);
				//CTC 2 +DS 1+ TIME 7+ GMC 4+ PSAM-TID 6 +PSMA 4
				memcpy(GreyResponse.GreyCardCTC,&Arm_g_posbuff[25],24);
			}
			ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_GREY;
			ProcExchangePos &= (~POS_CHECK_GREY); 
		}
	}
}

void PosMessage_BWListResult(void)
{
	if(SndPos.CurState == BWLIST_CHECK)
	{         
		SndPos.CurState = POLL;
		if(ProcExchangePos & (POS_CHECK_WHITE | POS_CHECK_BLACK))
		{
			POS_CR_Buf.CheckFlag = Arm_g_posbuff[7] & 0x01;
			if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_BLACK)
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
}

void PosMessage_ReqVTot(void)
{
	if(SndPos.CurState == POLL)
	{
		if(IdleFlg & 0x04)
		{
			StopTimer(PosTmr);
			//liuzg---
//			PosState = PosSending;
			ProcExchange[PROCESS_POS][PROCESS_POS] |= POS_ASK_V_TOT;
		}
	}
}

void PosMessage_DataBytes(void)
{
	unsigned int len;
	
	if(SndPos.CurState == REQ_DATA)
	{
		memcpy((unsigned char *)(&len),&Arm_g_posbuff[7],4);
		len = IntEndianConvert(len);
		if(Arm_g_posbuff[11] == 0)
		{
			if(len > BASE_BLACK_LIST_LIMIT)
			{
                if(!limit_flg)
                {
                    limit_flg = 1;
                    len = 16;
                    WriteDriveLog(0,0);
                    SndPos.CurState = POLL;
                }
				 return;
			}
		}
		else if(Arm_g_posbuff[11] < 4)
		{
			if(len > LIST_LIMIT)
			{
                if(!limit_flg)
                {
                    limit_flg = 1;
                    len = 16;
                    WriteDriveLog(Arm_g_posbuff[11],0);
                    SndPos.CurState = POLL;
                }
				return;
			}
		}
		else if(Arm_g_posbuff[11] == 4)
		{
			if(len > sizeof(GradePrice))
			{
				if(!limit_flg)
				{
					limit_flg = 1;
					len = sizeof(GradePrice);
					WriteDriveLog(4,0);
					SndPos.CurState = POLL;
				}
				return;
			}
		}
		else if(Arm_g_posbuff[11] == 5)
		{
			if(len > sizeof(OilMessage))
			{
				if(!limit_flg)
				{
					limit_flg = 1;
					len = sizeof(OilMessage);
					WriteDriveLog(5,0);
					SndPos.CurState = POLL;
				}
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
		ProcExchange[PROCESS_POS][PROCESS_POS] |= ASK_POS_DATA;
		SndPos.Download = 1;
	}
}

extern	PD_All PowerDown;

void PosMessage_DatCont(void)
{
	unsigned char i;
	unsigned short PosLeng;
	unsigned char WriteListBuf[256];
	
	if(SndPos.Download == 1)
	{
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
							SndPos.Download = 0;
							SndPos.CurState = POLL;
						}
						else
						{
							SndPos.Download = 2;
							SndPos.CurState = POLL;
							PosSeg.SegOffset++;							
						}
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
						SndPos.Download = 0;
						SndPos.CurState = POLL;
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
						SndPos.Download = 0;
						SndPos.CurState = POLL;
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
						SndPos.Download = 2;
						SndPos.CurState = POLL;
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
	}
}

void PosMessage_DispenserInfo(void)
{
	if(SndPos.CurState == POLL)
	{
		SndCmdAck(REQ_DISPENSER_INFO,0,1);
		StopTimer(PosTmr);
		//liuzg---
//		PosState = PosSending;
		ProcExchange[PROCESS_POS][PROCESS_POS] |= POS_ASK_INFO;
	}
}

void PosMessage_AuthBg(void)
{
	unsigned char nzn;
	
	nzn = Arm_g_posbuff[7];
	if(SndPos.CurState == POLL)
	{
		if(GetWorkMode())
		{
			if(GetAuthMode())
			{
				if(ProcStatus[PROCESS_CR] & CARD_LOGON)
				{
					if(ProcStatus[PROCESS_CR] & CARD_READY)
					{
						SndCmdAck(AUTH_BG,nzn,4);
					}
				  	else
				  	{
						POS_CR_Buf.CheckFlag = 0;
					  	POS_CR_Buf.TransNozzleNumber = Arm_g_posbuff[7];
					  	memcpy(POS_CR_Buf.Auth_TTC,&Arm_g_posbuff[8],4);
					  	memcpy(POS_CR_Buf.Auth_Date,&Arm_g_posbuff[12],7);
					  	POS_CR_Buf.Auth_Pay_X = Arm_g_posbuff[19];
					  	memcpy(POS_CR_Buf.ICCARD_ASN,&Arm_g_posbuff[20],2);
					  	memcpy(&POS_CR_Buf.TransAmount[1],&Arm_g_posbuff[22],3);
					  	ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_HOST;
					  	SndCmdAck(AUTH_BG,nzn,1);
				  	}
				}
				else
				{
					SndCmdAck(AUTH_BG,nzn,4);
				}
			}
			else
			{
				SndCmdAck(AUTH_BG,nzn,4);
			}
		}
		else
		{
			SndCmdAck(AUTH_BG,nzn,4);
		}
	}
}

void PosCmdAck(void)
{
	unsigned int time;
	
	if(SndPos.CurState == Arm_g_posbuff[7])
	{
		if(SndPos.CurState == NOZZLE_ST_DATA)
		{
			SndPos.CurState = POLL;
		}
		else if(SndPos.CurState == AUTH_REQ1)
		{
			if((Arm_g_posbuff[9] == 1) || (Arm_g_posbuff[9] == 2))
			{
				SndPos.PreState = SndPos.CurState;
				SndPos.CurState = POLL;
				if((Pos2MessageTime > 0) && (Pos2MessageTime < 201))
				{
					time = Pos2MessageTime * 1000;
				}
				else
				{
					time = 6 * 1000;
				}
				if(PosAckTmr != (Tmr *)0)
				{
					DeleteTimer(PosAckTmr);
				}
				PosAckTmr = CreateTimer(TIMER_ONE_DELETE,time,PosAckCmdTimeOut);
				ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_CST);
			}
			else
			{
				SndPos.PreState = POLL;
				SndPos.CurState = POLL;
				ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_CST);
				ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_CST;
				POS_CR_Buf.CheckFlag = 2;
			}
		}
		else if(SndPos.CurState == AUTH_REQ2)
		{
			if((Arm_g_posbuff[9] == 1) || (Arm_g_posbuff[9] == 2))
			{
				SndPos.PreState = SndPos.CurState;
				SndPos.CurState = POLL;
				if((Pos2MessageTime > 0) && (Pos2MessageTime < 201))
				{
					time = Pos2MessageTime * 1000;
				}
				else
				{
					time = 6 * 1000;
				}
				if(PosAckTmr != (Tmr *)0)
				{
					DeleteTimer(PosAckTmr);
				}
				PosAckTmr = CreateTimer(TIMER_ONE_DELETE,time,PosAckCmdTimeOut);
				ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_EMP);
			}
			else
			{
				SndPos.PreState = POLL;
				SndPos.CurState = POLL;
				ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_EMP);
				ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_EMP;
				POS_CR_Buf.CheckFlag = 2;
			}
		}
		else if(SndPos.CurState == AUTH_REQ3)
		{
			if((Arm_g_posbuff[9] == 1) || (Arm_g_posbuff[9] == 2))
			{
				SndPos.PreState = SndPos.CurState;
				SndPos.CurState = POLL;
				if((Pos2MessageTime > 0) && (Pos2MessageTime < 201))
				{
					time = Pos2MessageTime * 1000;
				}
				else
				{
					time = 6 * 1000;
				}
				if(PosAckTmr != (Tmr *)0)
				{
					DeleteTimer(PosAckTmr);
				}
				PosAckTmr = CreateTimer(TIMER_ONE_DELETE,time,PosAckCmdTimeOut);
				ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_NCSS);
			}
			else
			{
				SndPos.PreState = POLL;
				SndPos.CurState = POLL;
				ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_NCSS);
				ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_NCSS;
				POS_CR_Buf.CheckFlag = 2;
			}
		}
		else if(SndPos.CurState == DEMAND_GRAY_RECORD)
		{
			if((Arm_g_posbuff[9] == 1) || (Arm_g_posbuff[9] == 2))
			{
				SndPos.PreState = SndPos.CurState;
				SndPos.CurState = POLL;
				if((Pos2MessageTime > 0) && (Pos2MessageTime < 201))
				{
					time = Pos2MessageTime * 1000;
				}
				else
				{
					time = 6 * 1000;
				}
				if(PosAckTmr != (Tmr *)0)
				{
					DeleteTimer(PosAckTmr);
				}
				PosAckTmr = CreateTimer(TIMER_ONE_DELETE,time,PosAckCmdTimeOut);
				//ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_GREY);
			}
			else
			{
				SndPos.PreState = POLL;
				SndPos.CurState = POLL;
				ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_GREY; 
				ProcExchangePos &= (~POS_CHECK_GREY);
				POS_CR_Buf.CheckFlag = 2;
			}
		}
		else if(SndPos.CurState == BWLIST_CHECK)
		{
			if((Arm_g_posbuff[9] == 1) || (Arm_g_posbuff[9] == 2))
			{
				SndPos.PreState = SndPos.CurState;
				SndPos.CurState = POLL;
				if((Pos2MessageTime > 0) && (Pos2MessageTime < 201))
				{
					time = Pos2MessageTime * 1000;
				}
				else
				{
					time = 6 * 1000;
				}
				if(PosAckTmr != (Tmr *)0)
				{
					DeleteTimer(PosAckTmr);
				}
				PosAckTmr = CreateTimer(TIMER_ONE_DELETE,time,PosAckCmdTimeOut);
			}
			else
			{
				SndPos.PreState = POLL;
				SndPos.CurState = POLL;
				if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_BLACK)
				{
					ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_BLACK;  
					ProcExchangePos &= (~POS_CHECK_BLACK);
				}
				else
				{
					ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_WHITE; 
					ProcExchangePos &= (~POS_CHECK_WHITE);
				}
				POS_CR_Buf.CheckFlag = 2;
			}
		}
		else if(SndPos.CurState == NOZZLE_DOWN_DATA)
		{
			if((Arm_g_posbuff[9] == 1) || (Arm_g_posbuff[9] == 2))
			{
				if(ProcExchange[PROCESS_CR][PROCESS_POS] & FUELLING_DATA_HOST)
				{
					ProcExchangePos = ProcExchange[PROCESS_CR][PROCESS_POS];
					ProcExchange[PROCESS_CR][PROCESS_POS] &= (~FUELLING_DATA_HOST);
					ProcExchange[PROCESS_CR][PROCESS_POS] &= (~FUELLING_DATA);
					ProcExchange[PROCESS_POS][PROCESS_CR] |= FUELLING_DATA; 
					SndPos.PreState = POLL;  
					SndPos.CurState = POLL;
					POS_CR_Buf.CheckFlag = 0;
					/*if((Pos2MessageTime > 0) && (Pos2MessageTime < 201))
					{
						time = Pos2MessageTime * 1000;
					}
					else
					{
						time = 6 * 1000;
					}
					if(PosAckTmr != (Tmr *)0)
					{
						DeleteTimer(PosAckTmr);
					}
					PosAckTmr = CreateTimer(TIMER_ONE_DELETE,time,PosAckCmdTimeOut);*/
//================================================; Bellow LLLLO 20111102
//					if(PosErrTmr != (Tmr *)0)
//					{
//						ProcExchange[PROCESS_POS][PROCESS_POS] |= POS_ERROR;
//						DeleteTimer(PosErrTmr);
//					}
//					PosErrTmr = CreateTimer(TIMER_ONE_DELETE,time,PosErrCmdTimeOut);
//================================================; Upper LLLLO 20111102

					//ProcExchange[PROCESS_POS][PROCESS_POS] |= POS_ERROR;
				}
				else
				{
					SndPos.PreState = SndPos.CurState;
					SndPos.CurState = POLL;
					if((Pos2MessageTime > 0) && (Pos2MessageTime < 201))
					{
						time = Pos2MessageTime * 1000;
					}
					else
					{
						time = 6 * 1000;
					}
					if(PosAckTmr != (Tmr *)0)
					{
						DeleteTimer(PosAckTmr);
					}
					PosAckTmr = CreateTimer(TIMER_ONE_DELETE,time,PosAckCmdTimeOut);
					ProcExchange[PROCESS_CR][PROCESS_POS] &= (~FUELLING_DATA);
				}
			}
			else
			{
				SndPos.PreState = POLL;
				SndPos.CurState = POLL;
				ProcExchange[PROCESS_CR][PROCESS_POS] &= (~FUELLING_DATA);
				ProcExchange[PROCESS_POS][PROCESS_CR] |= FUELLING_DATA;
				POS_CR_Buf.CheckFlag = 2;
			}
		}
		else if(SndPos.CurState == V_TOT)
		{
			SndPos.CurState = POLL;
			StopTimer(PosTmr);
			PosState = PosSending;;
		}
		else if(SndPos.CurState == TRAN_DATA)
		{    
			SndPos.CurState = POLL;
			if(Arm_g_posbuff[9] == 5)
			{
				if(!MakeOverRecord())
				{
					ReadTTCInc();
				}
			}
		}
		else if(SndPos.CurState == FB_CARD_RECORD)
		{
			SndPos.CurState = POLL;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~CARD_RECORD);
			ProcExchange[PROCESS_POS][PROCESS_CR] |= CARD_RECORD;
		} 
		else if(SndPos.CurState == DISPENSER_INFO)
		{ 
			SndPos.CurState = POLL;
		}
		else if(SndPos.CurState == ERROR_INFO)
		{
			SndPos.CurState = POLL;
			ProcExchange[PROCESS_POS][PROCESS_POS] &= (~POS_ERROR);
		}
	}
}

void PosDataDispose(void)
{
	unsigned char command;
	
	command = Arm_g_posbuff[6];
	if(SndPos.POS_P == 0xff)
	{
		SndPos.POS_P = Arm_g_posbuff[2];
	}
	switch(command)
	{
	case VER_INFO:
		PosMessage_VerInfo();
		break;
	case AUTH_FB:
		PosMessage_AuthFB();
		break;
	case AUTH_MODE:
		PosMessage_AuthMode();
		break;
	case STOP:
		PosMessage_Stop();
		break;
	case PAYMENT_DATA:
		PosMessage_PaymentData();
		break;
	case PRICE_CHANGE:
		PosMessage_PriceChange();
		break;
	case CB_RECORD:
		PosMessage_CbRecord();
		break;
	case GRAY_RECORD:
		Pos2Message_GrayRecord();
		break;
	case BWLIST_RESULT:
		PosMessage_BWListResult();
		break;
	case REQ_V_TOT:
		PosMessage_ReqVTot();
		break;
	case DATA_BYTES:
		PosMessage_DataBytes();
		break;
	case DAT_CONT:
		PosMessage_DatCont();
		SndPos.CurAdr = OilMessage.pos_p;
		break;
	case REQ_DISPENSER_INFO:
		PosMessage_DispenserInfo();
		break;
	case AUTH_BG:
		PosMessage_AuthBg();
		break;
	case CMD_ACK:
		PosCmdAck();
		break;
	default:
		break;
	}
}

void PosStatus_Dispose(void)
{
	unsigned char buf[128];
	
	if(PosOnline)
	{
		if(PosState == PosSending)
		{
			if((SndPos.PreState == POLL) && (SndPos.CurState == POLL))
			{
				SndPos.SndCounts = 0;
				if(ProcExchange[PROCESS_POS][PROCESS_POS] & POS_ERROR)
				{
					SndPos.CurState = ERROR_INFO;
				}
				if((GetRecordSpace() < RECORD_MAX) && (ReadRecord(buf)))
				{
					SndPos.CurState = TRAN_DATA;
				}
				else if(ProcExchange[PROCESS_CR][PROCESS_POS] & FUELLING_DATA)
				{
					SndPos.CurState = NOZZLE_DOWN_DATA;
				}
				else if(ProcExchange[PROCESS_CR][PROCESS_POS] & AUTH_CST)
				{
					SndPos.CurState = AUTH_REQ1;
				}
				else if(ProcExchange[PROCESS_CR][PROCESS_POS] & AUTH_EMP)
				{
					SndPos.CurState = AUTH_REQ2;
				}
				else if(ProcExchange[PROCESS_CR][PROCESS_POS] & AUTH_NCSS)
				{
					SndPos.CurState = AUTH_REQ3;
				}
				else if(ProcExchange[PROCESS_CR][PROCESS_POS] & CARD_RECORD)
				{
					SndPos.CurState = FB_CARD_RECORD;
				}
				else if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_GREY)
				{
					SndPos.CurState = DEMAND_GRAY_RECORD; 
					ProcExchangePos = ProcExchange[PROCESS_CR][PROCESS_POS];
					ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_GREY);
				}
				else if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_BLACK)
				{
					SndPos.CurState = BWLIST_CHECK;  
					ProcExchangePos = ProcExchange[PROCESS_CR][PROCESS_POS]; 
					ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_BLACK);
				}
				else if(ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_WHITE)
				{
					SndPos.CurState = BWLIST_CHECK;   
					ProcExchangePos = ProcExchange[PROCESS_CR][PROCESS_POS];  
					ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_WHITE);
				}
				else if(ProcExchange[PROCESS_POS][PROCESS_POS] & POS_ASK_V_TOT)
				{
					SndPos.CurState = V_TOT;
					ProcExchange[PROCESS_POS][PROCESS_POS] &= (~POS_ASK_V_TOT);
				}
				else if(ProcExchange[PROCESS_POS][PROCESS_POS] & POS_ASK_INFO)
				{
					SndPos.CurState = DISPENSER_INFO;
					ProcExchange[PROCESS_POS][PROCESS_POS] &= (~POS_ASK_INFO);
				} 
				else if(ProcExchange[PROCESS_POS][PROCESS_POS] & POS_ASK_TRAN)
				{     
					SndPos.CurState = RECORD_TRAN;
					ProcExchange[PROCESS_POS][PROCESS_POS] &= (~POS_ASK_TRAN);
				}
				/*else if(SndPos.Download == 1)
				{
					SndPos.CurState = REQ_DATA;
				}*/
				else if(ProcExchange[PROCESS_POS][PROCESS_POS] & ASK_POS_DATALENG)
				{
					SndPos.CurState = REQ_DATA;
					ProcExchange[PROCESS_POS][PROCESS_POS] &= (~ASK_POS_DATALENG);
				}
				else if(ProcExchange[PROCESS_POS][PROCESS_POS] & ASK_POS_DATA)
				{
					SndPos.CurState = REQ_CONT;
					ProcExchange[PROCESS_POS][PROCESS_POS] &= (~ASK_POS_DATA);
				}
			}
		}
	}
	else
	{
		SndPos.PreState = POLL;
		SndPos.CurState = POLL;
	}
}

void Arm70_POS_Mode2_main( void )
{
	unsigned char retval;
	
//FOR TEST liuzg
#ifndef TEST_POS_MODE2
	SinopecToResolve(); 
#endif
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
		if((ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_BLACK) || (ProcExchangePos & POS_CHECK_BLACK))
		{
			POS_CR_Buf.CheckFlag = 0x02;
			ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_BLACK;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_BLACK); 
			ProcExchangePos &= (~POS_CHECK_BLACK);
		}
		if((ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_WHITE) || (ProcExchangePos & POS_CHECK_WHITE))
		{
			POS_CR_Buf.CheckFlag = 0x02;
			ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_WHITE;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_WHITE);  
			ProcExchangePos &= (~POS_CHECK_WHITE);
		}
		if((ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_GREY) || (ProcExchangePos & POS_CHECK_GREY))
		{
			GreyResponse.CheckFlag = 0x02;
			ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_GREY;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~POS_CHECK_GREY); 
			ProcExchangePos &= (~POS_CHECK_GREY);
		}
		if((ProcExchange[PROCESS_CR][PROCESS_POS] & AUTH_EMP) || (SndPos.PreState ==  AUTH_REQ2))
		{
			POS_CR_Buf.CheckFlag = 0x02;
			ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_EMP;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_EMP);
		}
		if((ProcExchange[PROCESS_CR][PROCESS_POS] & AUTH_CST) || (SndPos.PreState ==  AUTH_REQ1))
		{
			POS_CR_Buf.CheckFlag = 0x02;
			ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_CST;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_CST);
		} 
		if((ProcExchange[PROCESS_CR][PROCESS_POS] & AUTH_NCSS) || (SndPos.PreState ==  AUTH_REQ3))
		{
			POS_CR_Buf.CheckFlag = 0x02;
			ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_NCSS;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~AUTH_NCSS);
		}
		if((ProcExchange[PROCESS_CR][PROCESS_POS] & FUELLING_DATA) || (SndPos.PreState == NOZZLE_DOWN_DATA))
		{
			POS_CR_Buf.CheckFlag = 0x02;
			ProcExchange[PROCESS_POS][PROCESS_CR] |= FUELLING_DATA;
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~FUELLING_DATA);
		}
		if((ProcExchange[PROCESS_CR][PROCESS_POS] & CARD_RECORD) || (SndPos.PreState == FB_CARD_RECORD))
		{
			ProcExchange[PROCESS_CR][PROCESS_POS] &= (~CARD_RECORD);
			ProcExchange[PROCESS_POS][PROCESS_CR] |= CARD_RECORD;
			POS_CR_Buf.CheckFlag = 0x02; 
		}
	}
	PosStatus_Dispose();
	switch(SndPos.CurState)
	{
	case POLL:   
	case NOZZLE_ST_DATA:
		retval = Pos_Polling();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;
	case AUTH_REQ1:
		retval = Pos_AuthReq1();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;
	case AUTH_REQ2:
		retval = Pos_AuthReq2();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;
	case NOZZLE_DOWN_DATA:
		retval = Pos_NozzleDownData();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;
	case FB_CARD_RECORD:
		retval = Pos_FbCardRecord();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;
	case DEMAND_GRAY_RECORD:
		retval = Pos_GrayRecord();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;
	case BWLIST_CHECK:
		retval = Pos_BWListCheck();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;
	case V_TOT:
		retval = Pos_VTot();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;
	case REQ_DATA:
		retval = Pos_DataLeng();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;
	case REQ_CONT:
		retval = Pos_Data();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;
	case DISPENSER_INFO:
		retval = Pos_DispenserInfo();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;
	case ERROR_INFO:
		retval = Pos_ErrorSnd();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;	
	case AUTH_REQ3:
		retval = Pos_AuthReq3();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break;
	case TRAN_DATA:
		retval = Pos_TranData();
		if(retval == 0)
		{
			PosDataDispose();
		}
		break; 
	case RECORD_TRAN:
		retval = Pos_RecordTran();
		if(retval == 0)
		{ 
			PosDataDispose();
		}   
		break;
	default:
		break;
	}
}

void InitPosProcess2(void)
{
	chCtrl[0].sendback = PosSndComplete;
	SndPos.POS_P = 0xff;
	SndPos.CurAdr = OilMessage.pos_p;
	SndPos.MessNum = 0x40;
	SndPos.PosFlgSnded = 0;
	SndPos.PreState = POLL;
	SndPos.CurState = POLL;
	SndPos.SndCounts = 0;
	SndPos.TimeOutCounts = 0;
	//DownLoadTmr = CreateTimer(TIMER_ONE,3 * 1000,ResetDownLoad);
	//StopTimer(DownLoadTmr);
	if(ReadPriceReceviceFlg())
	{
		ProcExchange[PROCESS_POS][PROCESS_CR] |= PRICE_RECV;
	}
}