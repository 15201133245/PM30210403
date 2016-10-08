#include "CashTrans.h"
#include "global.h"
#include "define.h"
#include "Screen.h"
#include "CRvaribles.h"

#include "ICCardProc.h"		//Douzhq 20141011 Add , For Bug

#ifdef MAKE_CASH_CR

unsigned short RevCreLen;
unsigned short RevCreDataLen;
unsigned char RevCreFlg;
unsigned char RevCreBuf[128];
unsigned char SndCreBuf[128];

unsigned char CreCommand;
unsigned char CrState;
unsigned char CrePreCommand;

//Douzhq 20140728 Add , Car Number Recognize
unsigned char CarNumErr = 0;

// Bellow LLLLA
// For Cash Trans Process PowerDown Or Not
// Only PowerDown After Send The Record To POS
unsigned char CashSendRecordStart = 0;
unsigned char CRESendRecord = 0;
unsigned char CrEscOrNoEsc = 0;
// Upper LLLLA

enum CRE_COMMAND_STATUS CreStatus;

Tmr *pCreTmr;
Tmr *pCrWait;
Tmr *pCrEscTime;

//Douzhq 20140923 Add , For Bug
//Tmr *pAuthTimer;

//Douzhq 20140801 Add , Car Number Recognize
Tmr *pCarTimer;
Tmr *pCarCmdTimer;
unsigned char TmpFlag = 0;
unsigned char AckNumber = 0;
#define ACKNUMBER 3

static void CrCarTimeOut(void);

void ComCrRev(unsigned char *s,unsigned int len,unsigned char err)
{
	unsigned char tmp;
	
	if(!err)
	{
		while(len--)
		{
			tmp = *s;
			if(!RevCreFlg)
			{
				if(RevCreLen == 0)
				{
					if(tmp == STX_C)
					{
						RevCreBuf[RevCreLen++] = tmp;
					}
					else
					{
						RevCreDataLen = 0;
						RevCreLen = 0;
					}
				}
				else if(RevCreLen == 1)
				{
					if(((tmp & 0xf0) < 0xa0) && ((tmp & 0x0f) < 0x0a))
					{
						RevCreDataLen = tmp >> 4;
						RevCreDataLen *= 10;
						RevCreDataLen += (tmp & 0x0f);
						RevCreBuf[RevCreLen++] = tmp;
					}
					else
					{
						RevCreLen = 0;
						RevCreDataLen = 0;
					}
				}
				else if((RevCreLen < (RevCreDataLen + 2)) && (RevCreDataLen != 0))
				{
					RevCreBuf[RevCreLen++] = tmp;
				}
				else if((RevCreLen == (RevCreDataLen + 2)) && (RevCreDataLen != 0))
				{
					if(tmp == ETX_C)
					{
						RevCreBuf[RevCreLen++] = tmp;
					}
					else
					{
						RevCreLen = 0;
						RevCreDataLen = 0;
					}
				}
				else if((RevCreLen == (RevCreDataLen + 3)) && (RevCreDataLen != 0))
				{
					RevCreBuf[RevCreLen++] = tmp;
					RevCreFlg = 1;
					RevCreDataLen = 0;
				}
				else
				{
					RevCreLen = 0;
					RevCreDataLen = 0;
				}
			}
			s++;
		}
	}
}

void ResposneTimeOut(void)
{
	CreStatus = CRE_Command_ResposeTimeOut;
}

//Douzhq 20140923 Add , For Bug
//CRE Engross OK , Then CE - CRE Break , Clear Flag CASH_AUTH_CODE
void AuthCodeTimeOut(void)
{
	if (ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_AUTH_CODE)
	{
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_AUTH_CODE);
	}
}

void ComCrSnded(unsigned char *s,unsigned int len,unsigned char err)
{
	DbugSerial.pUsartHw->DBGU_IER = AT91C_US_RXRDY;
	if(CreCommand == SND_RECORD)
	{
		CreStatus = CRE_Command_WaitRespose;
		pCreTmr = CreateTimer(TIMER_ONE_DELETE,WAIT_RESPONSE_TIME,ResposneTimeOut);

		// Bellow LLLLA
		if( CashSendRecordStart == 1 )
		{
			CashSendRecordStart = 0;
			CRESendRecord = 0;
		}

	}
	else if(CreCommand == CR_INPUT)
	{
		CreStatus = CRE_Command_WaitRespose;
		pCreTmr = CreateTimer(TIMER_ONE_DELETE,WAIT_RESPONSE_TIME,ResposneTimeOut);
	}
//Douzhq 20140731 Add , Car Number Recognize
	else if (CreCommand == CAR_NUMBER_REC)
	{
		CreStatus = CRE_Command_WaitRespose;
		pCreTmr = CreateTimer(TIMER_ONE_DELETE,WAIT_RESPONSE_CAR,ResposneTimeOut);
	}
	else if (CreCommand == GRADE_LIMIT)
	{
		CreStatus = CRE_Command_WaitRespose;
		pCreTmr = CreateTimer(TIMER_ONE_DELETE,WAIT_RESPONSE_CAR,ResposneTimeOut);
	}
	else
	{
		CreStatus = CRE_Command_Rev;
	}

}


void InitCashProcess( void )
{
	unsigned int mode;
    
    PIO_SetPeripheralA(dbug_usart.pio,dbug_usart.mask,ENABLE_PULLUP);
    mode = AT91C_US_PAR_NONE | AT91C_US_CHMODE_NORMAL;
    DbugUsart_Configure(AT91C_BASE_DBGU,mode,DBUG_USART_BAUD_INT,MAIN_MCK);
    PDC_DisableRx((AT91PS_PDC)(&AT91C_BASE_DBGU->DBGU_RPR));
    PDC_DisableTx((AT91PS_PDC)(&AT91C_BASE_DBGU->DBGU_TPR));
    //INT
    DBUG_US_EnableIt(AT91C_BASE_DBGU, 
    		AT91C_US_RXRDY);
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN | AT91C_US_TXEN;
	DbugSerial.pUsartHw = AT91C_BASE_DBGU;
	DbugSerial.receviceback = ComCrRev;
	DbugSerial.sendback = ComCrSnded;
}

// Bellow LLLLA
//
//void DbugCashSentOver(unsigned char *s,unsigned int len,unsigned char error)
//{
//	// While Power Down Issued, Wait Until Record Sent Over
//	if( CashSendRecordStart == 1 )
//	{
//    	CashSendRecordStart = 0;
//		CRESendRecord = 0;
//	}
//}
// Upper LLLLA


void CrePolling_Dispose(void)
{
	unsigned char len;
	unsigned short year;
	
	CrState = 0;
	CrePreCommand = 0;
	Cash_CR_Buf.CashType = RevCreBuf[3];
	len = strlen((char *)(&RevCreBuf[4]));
	
//Douzhq 20140801 Add , Car Number Recognize
	unsigned char CarNumRecLen;
	unsigned char TempCardAsn[20];
//Douzhq End
	memcpy(Cash_CR_Buf.CashVersion,&RevCreBuf[4],len + 1);
	if(ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_TRANS_RECORD)
	{
		//Douzhq 20140928 Add , For Bug
		if (ProcExchange[PROCESS_CR][PROCESS_CR] & CASH_CODE_ERR)
		{
			ProcExchange[PROCESS_CR][PROCESS_CR] &= ~(CASH_CODE_ERR);
		}
		
		SndCreBuf[0] = STX_C;
		SndCreBuf[1] = 0x97;
		SndCreBuf[2] = SND_RECORD;
		memcpy(&SndCreBuf[3],Cash_CR_Buf.CashTransRecord,96);
		SndCreBuf[99] = ETX_C;
		SndCreBuf[100] = CrcEor(SndCreBuf,100);
		CreCommand = SND_RECORD;
		CreStatus = CRE_Command_Sending;
		WriteToDBUG_USART(SndCreBuf,101);

		// Start Send Record, LLLLA
		CashSendRecordStart = 1;
	}
	else if(ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_AUTH_CODE)
	{
		//Douzhq 20140923 Add , For Bug
		//CRE Engross OK , Then CR - CRE Break , Clear Flag CASH_AUTH_CODE
		//StopTimer(pAuthTimer);
		
		len = Cash_CR_Buf.CashAuthCode[0];
		SndCreBuf[0] = STX_C; 
		SndCreBuf[1] = len + 2;		
		SndCreBuf[2] = CR_INPUT;
		memcpy(&SndCreBuf[3],Cash_CR_Buf.CashAuthCode,len + 1);
		SndCreBuf[len + 4] = ETX_C;
		SndCreBuf[len + 5] = CrcEor(SndCreBuf,len + 5);
		CreCommand =CR_INPUT ;
		CreStatus = CRE_Command_Sending;
		WriteToDBUG_USART(SndCreBuf,len + 6);
		
		//Douzhq 20140923 Add , For Bug
		if (ProcExchange[PROCESS_CR][PROCESS_CR] & CASH_CODE_ERR)
		{
			ProcExchange[PROCESS_CR][PROCESS_CR] &= ~(CASH_CODE_ERR);
		}
	}
//Douzhq 20140728 Add , Car Number Recognize
	else if (TmpFlag != 0 && CarNumErr != 1)
	{	
		if(pCarTimer != (Tmr *)0)
		{
			DeleteTimer(pCarTimer);
		}
		CreStatus = CRE_Command_Rev;
		//CarNumberSend = 0;
		if (TmpFlag == 1)
		{
			//Send Car Number cmd 0x40
			SndCreBuf[0] = STX_C;
			SndCreBuf[1] = 0x40;
			SndCreBuf[2] = CAR_NUMBER_REC;
			
			BcdToAscii((char *)TempCardAsn,(char *)CarNumGradeBuffer.CardAsn,sizeof(CarNumGradeBuffer.CardAsn));
			
			memcpy(&SndCreBuf[3],TempCardAsn,sizeof(TempCardAsn));
			CarNumRecLen = 3 + sizeof(TempCardAsn);
			
			memset(&SndCreBuf[CarNumRecLen],0,1);
			CarNumRecLen++;
			
			memcpy(&SndCreBuf[CarNumRecLen],CarNumGradeBuffer.CarNumber,sizeof(CarNumGradeBuffer.CarNumber));
			CarNumRecLen += sizeof(CarNumGradeBuffer.CarNumber);
			
			memset(&SndCreBuf[CarNumRecLen],0,1);
			CarNumRecLen++;
			
			SndCreBuf[CarNumRecLen] = CarNumGradeBuffer.NozzleNumber;
			CarNumRecLen++;
			
			SndCreBuf[CarNumRecLen] = ETX_C;
			CarNumRecLen++;
			
			SndCreBuf[CarNumRecLen] = CrcEor(SndCreBuf,CarNumRecLen);
			CarNumRecLen++;
			
			CreCommand = CAR_NUMBER_REC;
			CreStatus = CRE_Command_Sending;
			WriteToDBUG_USART(SndCreBuf,CarNumRecLen);
			
			CarNumRecLen = 0;
		}
		else if (TmpFlag == 2)
		{
			//ProcExchange[PROCESS_CASH][PROCESS_CASH] &= ~(CASH_GRADE_lIMIT);
			
			//CarNumberSend = 0;
			
			SndCreBuf[0] = STX_C;
			SndCreBuf[1] = 0x27;
			SndCreBuf[2] = GRADE_LIMIT;
			
			BcdToAscii((char *)TempCardAsn,(char *)CarNumGradeBuffer.CardAsn,sizeof(CarNumGradeBuffer.CardAsn));
			
			memcpy(&SndCreBuf[3],TempCardAsn,sizeof(TempCardAsn));
			CarNumRecLen = 3 + sizeof(TempCardAsn);
			
			memset(&SndCreBuf[CarNumRecLen],0,1);
			CarNumRecLen++;
			
			memcpy(&SndCreBuf[CarNumRecLen],CarNumGradeBuffer.LimitGrade,sizeof(CarNumGradeBuffer.LimitGrade));
			CarNumRecLen += sizeof(CarNumGradeBuffer.LimitGrade);
			
			memcpy(&SndCreBuf[CarNumRecLen],CarNumGradeBuffer.PumpGrade,sizeof(CarNumGradeBuffer.PumpGrade));
			CarNumRecLen += sizeof(CarNumGradeBuffer.PumpGrade);
			
			SndCreBuf[CarNumRecLen] = CarNumGradeBuffer.NozzleNumber;
			CarNumRecLen++;
			
			SndCreBuf[CarNumRecLen] = ETX_C;
			CarNumRecLen++;
			
			SndCreBuf[CarNumRecLen] = CrcEor(SndCreBuf,CarNumRecLen);
			CarNumRecLen++;
			
			CreCommand = GRADE_LIMIT;
			CreStatus = CRE_Command_Sending;
			WriteToDBUG_USART(SndCreBuf,CarNumRecLen);
			
			CarNumRecLen = 0;
		}
		if (AckNumber == 0)
		{
			if(pCarCmdTimer != (Tmr *)0)
			{
				DeleteTimer(pCarCmdTimer);
				pCarCmdTimer = (Tmr *)0;
			}
			pCarCmdTimer = CreateTimer(TIMER_ONE_STOP,WAIT_CMD_REQ,CrCarTimeOut);
		}
		
		AckNumber++;
		
		if (AckNumber > ACKNUMBER)
		{
			if (pCarCmdTimer != (Tmr *)0)
			{
				DeleteTimer(pCarCmdTimer);
			}
			CrCarTimeOut();
			AckNumber = 0;
			TmpFlag = 0;
		}
	}
//Douzhq End
	else
	{
		SndCreBuf[0] = STX_C;
		SndCreBuf[1] = 8;
		SndCreBuf[2] = POLLING;
		year = ShortEndianConvert(SysBCDTime.year);
		memcpy(&SndCreBuf[3],(unsigned char *)(&year),2);
		memcpy(&SndCreBuf[5],&SysBCDTime.month,5);
		SndCreBuf[10] = ETX_C;
		SndCreBuf[11] = CrcEor(SndCreBuf,11);
		CreCommand = POLLING;
		CreStatus = CRE_Command_Sending;
		WriteToDBUG_USART(SndCreBuf,12);
	}
		
}

void WaitCrTimeOut(void)
{
	CreStatus = CRE_Command_Rev;
	
	//Douzhq 20141022 Add , For Bug
	//-----------------------------Down--------------------------------------
	if (ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_KEY_PRESSED)
	{
		ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_KEY_PRESSED);
	}
	//----------------------------- Up --------------------------------------	
	
}

void CrEscTimeOut(void)
{
	CrEscOrNoEsc = 1;
	ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_KEY_CANCELLED;
	
	//Douzhq 20140925 Add , For Bug
	if(ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_KEY_PRESSED)
	{
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_KEY_PRESSED);
	}
	if (ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_AUTH_CODE)
	{
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_AUTH_CODE);
	}
}

//Douzhq 20140728 Add , Car Number Recognize
void CrCarTimeOut(void)
{
	CarNumErr = 1;
	
	//ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_MSG_ERROR;
	//ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_GRADE_CONFORM);
	
	ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_CARE_RECOGNIZE;
	ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_GRADE_lIMIT;
	CarNumGradeBuffer.CarRecFlag = Cmd_CheckTimeOut;
}
//Douzhq End

void CreEngross_Dispose(void)
{
	Cash_CR_Buf.CashKeyNozzleNumber = RevCreBuf[3];
	if(!CrePreCommand)
	{
		if((ProcStatus[PROCESS_CR] & CARD_INSERTED) && (CRType != CR_TYPE_CASH)) {
			if(ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_KEY_PRESSED) {
	
			} else {
				if(ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_KEY_PRESSED) {
					ProcExchange[PROCESS_CASH][PROCESS_CR] &= (~CASH_KEY_PRESSED);
				}
				Cash_CR_Buf.CashCard_Flag = 1;
				ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_KEY_PRESSED;
			}
		} else {
			//Douzhq 20141011 Add , For Bug
			//------------------Down---------------------------------------------------
			if (ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_KEY_PRESSED)
			{
				ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_KEY_PRESSED);
			}
			//Douzhq 20141107 Add , For Bug
			if (ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_KEY_CANCELLED)
			{
				ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_KEY_CANCELLED);
			}
			//------------------ Up ---------------------------------------------------
			ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_KEY_PRESSED;
			CrePreCommand = 1;
		}
	}
	CreStatus = CRE_Command_WaitCr;
	if(ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_KEY_PRESSED)
	{
		if(pCrWait != (Tmr *)0)
		{
			DeleteTimer(pCrWait);
			pCrWait = (Tmr *)0;
		}
		SndCreBuf[0] = STX_C;
		SndCreBuf[1] = 6;
		SndCreBuf[2] = ENGROSS;
		SndCreBuf[3] = Cash_CR_Buf.CashCard_Flag;
		SndCreBuf[4] = Cash_CR_Buf.CashKeyNozzleNumber;
		SndCreBuf[5] = Cash_CR_Buf.CashNozzleNumber;
		memcpy(&SndCreBuf[6],Cash_CR_Buf.CashGradeNumber,2);
		SndCreBuf[8] = ETX_C;
		SndCreBuf[9] = CrcEor(SndCreBuf,9);
		CreCommand = ENGROSS;
		CreStatus = CRE_Command_Sending;
		WriteToDBUG_USART(SndCreBuf,10);
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= (~CASH_KEY_PRESSED);
		CrState = 1;
		CrePreCommand = 0;
		CrEscOrNoEsc = 0;
		if(!Cash_CR_Buf.CashCard_Flag)
		{
			if(pCrEscTime != (Tmr *)0)
			{
				DeleteTimer(pCrEscTime);
				pCrEscTime = (Tmr *)0;
			}
			pCrEscTime = CreateTimer(TIMER_ONE,60 * 1000,CrEscTimeOut);
		}
		else
		{
			//Douzhq 20140918 Add , Car Number Recognize
			//For Bug
			//CrEscOrNoEsc = 1;
			//ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_KEY_CANCELLED;
		}
	}
	else if(CrState == 1)
	{
		if(pCrWait != (Tmr *)0)
		{
			DeleteTimer(pCrWait);
			pCrWait = (Tmr *)0;
		}
		SndCreBuf[0] = STX_C;
		SndCreBuf[1] = 6;
		SndCreBuf[2] = ENGROSS;
		SndCreBuf[3] = Cash_CR_Buf.CashCard_Flag;
		SndCreBuf[4] = Cash_CR_Buf.CashKeyNozzleNumber;
		SndCreBuf[5] = Cash_CR_Buf.CashNozzleNumber;
		memcpy(&SndCreBuf[6],Cash_CR_Buf.CashGradeNumber,2);
		SndCreBuf[8] = ETX_C;
		SndCreBuf[9] = CrcEor(SndCreBuf,9);
		CreCommand = ENGROSS;
		CreStatus = CRE_Command_Sending;
		WriteToDBUG_USART(SndCreBuf,10);
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= (~CASH_KEY_PRESSED);
		CrePreCommand = 0;
		CrEscOrNoEsc = 0;
		if(!Cash_CR_Buf.CashCard_Flag)
		{
			if(pCrEscTime != (Tmr *)0)
			{
				DeleteTimer(pCrEscTime);
				pCrEscTime = (Tmr *)0;
			}
			pCrEscTime = CreateTimer(TIMER_ONE,60 * 1000,CrEscTimeOut);
		}
	}
	else
	{
		pCrWait = CreateTimer(TIMER_ONE_DELETE,50 * WAIT_RESPONSE_TIME,WaitCrTimeOut);
	}
}

void CreSndRecord_Dispose(void)
{
	CreStatus = CRE_Command_Rev;
	DeleteTimer(pCreTmr);
	ProcExchange[PROCESS_CR][PROCESS_CASH] &= (~CASH_TRANS_RECORD);
}

void CreWarning_Dispose(void)
{
	unsigned short WarningCode;
	unsigned char len;
	
	memcpy((unsigned char *)(&WarningCode),&RevCreBuf[3],2);
	WarningCode = ShortEndianConvert(WarningCode);
	
	CashErrorNumber = WarningCode;
	// liuzg
	// LLLLO
	len = strlen((char *)(&RevCreBuf[5]));
	if(len < RevCreBuf[1])
	{
		//memcpy(CustomMsg0,&RevCreBuf[5],len + 1);
		memcpy(InfoLine,&RevCreBuf[5],len + 1);
		FillInfoMsg(M_InfoLine,0,0);
	}
	// LLLLO End
	//liuzg End
	
	/*
	// LLLLA
	memset( InfoLine, 0x00, sizeof( InfoLine ));
	len = strlen((char *)(&RevCreBuf[5]));
	
	if( len > 2 )
	{
		// Not Include 0xFB Anc XorC
    	len -= 2;

		if( len > 18 )
		{
			len = 18;
		}

		memcpy( InfoLine, &RevCreBuf[5], len );
		FillInfoMsg( M_InfoLine, 0, 0 );
	}
	// LLLLA End
	*/

	if(WarningCode < 0x8000)
	{
		ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_KEY_CANCELLED;
		
		//Douzhq 20140925 Add , For Bug
		if(ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_KEY_PRESSED)
		{
			ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_KEY_PRESSED);
		}
		if (ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_AUTH_CODE)
		{
			ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_AUTH_CODE);
		}
	}
	SndCreBuf[0] = STX_C;
	SndCreBuf[1] = 1;
	SndCreBuf[2] = WARNING;
	SndCreBuf[3] = ETX_C;
	SndCreBuf[4] = CrcEor(SndCreBuf,4);
	CreCommand = WARNING;
	CreStatus = CRE_Command_Sending;
	WriteToDBUG_USART(SndCreBuf,5);
}

void CreCrInput_Dispose(void)
{
	CreStatus = CRE_Command_Rev;
	DeleteTimer(pCreTmr);
	ProcExchange[PROCESS_CR][PROCESS_CASH] &= (~CASH_AUTH_CODE);
}

void CreAuthCr_Dispose(void)
{
	//20120719 liuzg start	--for POS to print ticket
	unsigned char len;
	
	len = RevCreBuf[1];
	//20120719 liuzg end
	Cash_CR_Buf.CashAuthCode[0] = 5;
	memcpy(&Cash_CR_Buf.CashAuthCode[1],&RevCreBuf[6],5);
	memcpy(Cash_CR_Buf.CashTotal,&RevCreBuf[11],4);
	//20120719 liuzg start	--for POS to print ticket
	CashFromPosUnit = 0;
	if(len == 0x14)
	{
		CashFromPosUnit = RevCreBuf[15];
	}
	
	//Douzhq 20141011 Add , For Buf
	//-------------------------------------------------------
	if (CRType != CR_TYPE_CASH)
	{
		CrEscOrNoEsc = 3;		//CrEscOrNoEsc = 0 , CRE Occupy CR
								//CrEscOrNoEsc = 1 , Cash Trans Time Out , CR Return Idle
								//CrEscOrNoEsc = 2 , Cash Trans Auth Ok
								//CrEscOrNoEsc = 3 , CR Current Status Not Cash Trans
	}
	
	//20120719 liuzg end
	if(!CrEscOrNoEsc)
	{
		ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_TOTAL;
		
		//Douzhq 20140928 Add , For Bug
		ProcExchange[PROCESS_CR][PROCESS_CR] |= CASH_CODE_ERR;
		CrEscOrNoEsc = 2;
		
		SndCreBuf[3] = 0;
		StopTimer(pCrEscTime);
	}
	else
	{
		SndCreBuf[3] = 1;
		
		//Douzhq 20141011 Add , For Bug
		//---------------------------------Down-----------------------------------------------
		if (CrEscOrNoEsc == 2)
		{
			SndCreBuf[3] = 0;
		}
		else
		{
			if (CrEscOrNoEsc == 3 )
			{
				SndCreBuf[3] = 0;
				//Zero Trans
				GetCashAmountZero();
				memset(TRINP.TR_CASH_TOTAL, 0x00, sizeof( TRINP.TR_CASH_TOTAL ));
			}
			else
			{
				if( CheckCashAmount() == 0 )
				{
					// A Cash Trans Reserved, Maybe Just Power On
					// Or Cash Received But Fail To Nozzle On etc....
					// So, Forbidden A Cash Trans Beginning....
					GetCashAmount();
					// Clear the cashtotal for next trans' scan, 20140818
					memset( TRINP.TR_CASH_TOTAL, 0x00, sizeof( TRINP.TR_CASH_TOTAL ));
				}
			}
		}
		//--------------------------------- Up -----------------------------------------------
	}
	SndCreBuf[0] = STX_C;
	SndCreBuf[1] = 2;
	SndCreBuf[2] = AUTH_CR;
	SndCreBuf[4] = ETX_C;
	SndCreBuf[5] = CrcEor(SndCreBuf,5);
	CreCommand = AUTH_CR;
	CreStatus = CRE_Command_Sending;
	WriteToDBUG_USART(SndCreBuf,6);

	// A Record Need To Be Sent, LLLLLA
	CRESendRecord = 1;
}

void CreAuthEsc_Dispose(void)
{
// LLLLA
	unsigned char len;

	memset( InfoLine, 0x00, sizeof( InfoLine ));

	len = RevCreBuf[1];
	len -= 1;
//	len = strlen((char *)(&RevCreBuf[3]));
//	if( len > 2 )
//	{
//		// Not Include 0xFB And XroC
//		len -= 2;
//
//		if( len > 18 )
//		{
//			len = 18;
//		}

		memcpy( InfoLine, &RevCreBuf[3], len );
		FillInfoMsg( M_InfoLine, 0, 0 );
//	}
// LLLLA End

	ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_KEY_CANCELLED;

	SndCreBuf[0] = STX_C;
	SndCreBuf[1] = 1;
	SndCreBuf[2] = LOCK_ASK_ESC;
	SndCreBuf[3] = ETX_C;
	SndCreBuf[4] = CrcEor(SndCreBuf,4);
	CreCommand = AUTH_CR;
	CreStatus = CRE_Command_Sending;
	WriteToDBUG_USART(SndCreBuf,5);
	
	//Douzhq 20141107 Add , For Bug
	if(pCrEscTime != (Tmr *)0)
    {
	    DeleteTimer(pCrEscTime);
		pCrEscTime = (Tmr *)0;
	}
}

//Douzhq 20140721 Add , Car Number Recognize
void CreCarNumberRec_Dispose(void)
{
	unsigned char TempFlag = RevCreBuf[20];
	
	CreStatus = CRE_Command_Rev;
	//StopTimer(pCarTimer);
	
	if(pCarCmdTimer != (Tmr *)0)
	{
		StopTimer(pCarCmdTimer);
	}
	//ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_GRADE_CONFORM);
	
	SndCreBuf[0] = STX_C;
	SndCreBuf[1] = 1;
	SndCreBuf[2] = CAR_NUMBER_CAL;
	SndCreBuf[3] = ETX_C;
	SndCreBuf[4] = CrcEor(SndCreBuf,4);
	CreCommand = CAR_NUMBER_CAL;
	CreStatus = CRE_Command_Sending;
	WriteToDBUG_USART(SndCreBuf,5);
	
	if (CarNumErr == 1)
	{
		//CarNumErr = 0;
		//ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_GRADE_CONFORM);
		return ;
	}
	
	if (TempFlag == 0)
	{
		memcpy(CarNumGradeBuffer.CarNumber,&RevCreBuf[3],sizeof(CarNumGradeBuffer.CarNumber));
		
		ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_CARE_RECOGNIZE;
		CarNumGradeBuffer.CarRecFlag = Cmd_CheckMatch;
	}
	else
	{
		
		//CreStatus = CRE_Command_Rev;
		ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_CARE_RECOGNIZE;
		CarNumGradeBuffer.CarRecFlag = Cmd_CheckNotMatch;
		
		//Douzhq 20141021 Add , Add Car Number Recognize Message
		FillInfoMsg( M_CarNumNoMatch, 0, 0 );
	}
}

//Douzhq 20140721 Add , Grade Limit
void CreGradeLimit_Dispose(void)
{
	unsigned char TempFlag = RevCreBuf[28];
	unsigned char TempLen = 0;
	
	//StopTimer(pCarTimer);
	if(pCarCmdTimer != (Tmr *)0)
	{
		StopTimer(pCarCmdTimer);
	}
	CreStatus = CRE_Command_Rev;
	//ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_GRADE_CONFORM);
	
	SndCreBuf[0] = STX_C;
	SndCreBuf[1] = 1;
	SndCreBuf[2] = GRADE_LIM_RES;
	SndCreBuf[3] = ETX_C;
	SndCreBuf[4] = CrcEor(SndCreBuf,4);
	CreCommand = GRADE_LIM_RES;
	CreStatus = CRE_Command_Sending;
	WriteToDBUG_USART(SndCreBuf,5);
	
	if (CarNumErr == 1)
	{
		//CarNumErr = 0;
		//ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_GRADE_CONFORM);
		return ;
	}
	
	if (TempFlag == 0)
	{
		//memcpy(CarNumGradeBuffer.CardAsn,&RevCreBuf[3],sizeof(CarNumGradeBuffer.CardAsn));
		TempLen = 3 + 21;
			
		memcpy(CarNumGradeBuffer.LimitGrade,&RevCreBuf[TempLen],sizeof(CarNumGradeBuffer.LimitGrade));
		TempLen += 2;
		
		memcpy(CarNumGradeBuffer.PumpGrade,&RevCreBuf[TempLen],sizeof(CarNumGradeBuffer.PumpGrade));
		TempLen = 0;
			
		ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_GRADE_lIMIT;
		CarNumGradeBuffer.CarRecFlag = Cmd_CheckMatch;
	}
	else
	{
		//CreStatus = CRE_Command_Rev;
		ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_GRADE_lIMIT;
		CarNumGradeBuffer.CarRecFlag = Cmd_CheckNotMatch;
	}
}

void InitCarFeatureTimer(void)
{
	if(pCarTimer != (Tmr *)0)
	{
		DeleteTimer(pCarTimer);
	}
	if(pCarCmdTimer != (Tmr *)0)
	{
		DeleteTimer(pCarCmdTimer);
	}
}

//Douzhq 20140825 Add , Car Number Recognize
//Car Number & Limit Grade Set TmpFlag and Set Timer
void CarNumberRecProc(void)
{
	if (ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_CARE_RECOGNIZE)
	{
		InitCarFeatureTimer();
		AckNumber = 0;
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_CARE_RECOGNIZE);
		ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_CAR_ONCE;
		TmpFlag = 1;
		
		if(pCarTimer != (Tmr *)0)
		{
			DeleteTimer(pCarTimer);
			pCarTimer = (Tmr *)0;
		}
		pCarTimer = CreateTimer(TIMER_ONE_STOP,WAIT_CAR_RECOGNIZE,CrCarTimeOut);
		CarNumErr = 0;
	}
	else if (ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_GRADE_lIMIT)
	{
		InitCarFeatureTimer();
		AckNumber = 0;
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_GRADE_lIMIT);
		ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_CAR_ONCE;
		TmpFlag = 2;
		
		if(pCarTimer != (Tmr *)0)
		{
			DeleteTimer(pCarTimer);
			pCarTimer = (Tmr *)0;
		}
		pCarTimer = CreateTimer(TIMER_ONE_STOP,WAIT_CAR_RECOGNIZE,CrCarTimeOut);
		CarNumErr = 0;
	}
}

void CashProcess( void )
{
	unsigned char crc;
	unsigned char command;
	unsigned char len;
	
	//Douzhq 20140825 Add , Car Number Recognize
	CarNumberRecProc();
	
	//Douzhq 20140923 Add , For Bug
	//CRE Engross OK , Then CE - CRE Break , Clear Flag CASH_AUTH_CODE
	if (ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_CODE_TIMER)
	{
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_CODE_TIMER);
		/*if(pAuthTimer != (Tmr *)0)
		{
			DeleteTimer(pAuthTimer);
			pAuthTimer = (Tmr *)0;
		}
		pAuthTimer = CreateTimer(TIMER_ONE_STOP,WAIT_RESPONSE_CAR,AuthCodeTimeOut);*/
	}
	
	if(RevCreFlg == 1)
	{
		len = RevCreLen;
		RevCreLen = 0;
		if((CreStatus == CRE_Command_Rev) || (CreStatus == CRE_Command_WaitRespose))
		{
			crc = CrcEor(RevCreBuf,len);
			if(crc == 0)
			{
				command = RevCreBuf[2];
				switch(command)
				{
				case POLLING:
					CrePolling_Dispose();
					break;
				case ENGROSS:
					CreEngross_Dispose();
					break;
				case SND_RECORD:
					CreSndRecord_Dispose();
					break;
				case CARD_CPDU:
					break;
				case WARNING:
					CreWarning_Dispose();
					break;
				case CASH_AMN:
					break;
				case CPDU_END:
					break;
				case CR_INPUT:
					CreCrInput_Dispose();
					break;
				case AUTH_CR:
					CreAuthCr_Dispose();
					break;
				case LOCK_ASK_ESC:
					CreAuthEsc_Dispose();
					break;
//Douzhq 20140721 Add , Car Number Recognize
				case CAR_NUMBER_REC:
					TmpFlag = 0;
					AckNumber = 0;
					CreStatus = CRE_Command_Rev;
					if (pCreTmr != (Tmr *)0)
					{
						DeleteTimer(pCreTmr);
					}
					break;
				case CAR_NUMBER_CAL:
					CreCarNumberRec_Dispose();
					break;
//Douzhq 20140721 Add , Grade Limit
				case GRADE_LIMIT:
					TmpFlag = 0;
					AckNumber = 0;
					CreStatus = CRE_Command_Rev;
					if (pCreTmr != (Tmr *)0)
					{
						DeleteTimer(pCreTmr);
					}
					break;
				case GRADE_LIM_RES:
					CreGradeLimit_Dispose();
					break;
				default:break;
				}
			}
		}
		else if(CreStatus == CRE_Command_ResposeTimeOut)
		{
			CreStatus = CRE_Command_Rev;
		}
		RevCreFlg = 0;
	}
	if(CreStatus == CRE_Command_WaitCr)
	{
		if(ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_KEY_PRESSED)
		{
			if(pCrWait != (Tmr *)0)
			{
				DeleteTimer(pCrWait);
				pCrWait = (Tmr *)0;
			}
			SndCreBuf[0] = STX_C;
			SndCreBuf[1] = 6;
			SndCreBuf[2] = ENGROSS;
			SndCreBuf[3] = Cash_CR_Buf.CashCard_Flag;
			SndCreBuf[4] = Cash_CR_Buf.CashKeyNozzleNumber;
			SndCreBuf[5] = Cash_CR_Buf.CashNozzleNumber;
			memcpy(&SndCreBuf[6],Cash_CR_Buf.CashGradeNumber,2);
			SndCreBuf[8] = ETX_C;
			SndCreBuf[9] = CrcEor(SndCreBuf,9);
			CreCommand = ENGROSS;
			CreStatus = CRE_Command_Sending;
			WriteToDBUG_USART(SndCreBuf,10);
			ProcExchange[PROCESS_CR][PROCESS_CASH] &= (~CASH_KEY_PRESSED);
            
            CrePreCommand = 0;
     		CrEscOrNoEsc = 0;
            if(!Cash_CR_Buf.CashCard_Flag)
            {
                if(pCrEscTime != (Tmr *)0)
                {
                    DeleteTimer(pCrEscTime);
                    pCrEscTime = (Tmr *)0;
                }
                pCrEscTime = CreateTimer(TIMER_ONE,60 * 1000,CrEscTimeOut);
            }
		}
		else if(CrState == 1)
		{
			if(pCrWait != (Tmr *)0)
			{
				DeleteTimer(pCrWait);
				pCrWait = (Tmr *)0;
			}
			SndCreBuf[0] = STX_C;
			SndCreBuf[1] = 6;
			SndCreBuf[2] = ENGROSS;
			SndCreBuf[3] = Cash_CR_Buf.CashCard_Flag;
			SndCreBuf[4] = Cash_CR_Buf.CashKeyNozzleNumber;
			SndCreBuf[5] = Cash_CR_Buf.CashNozzleNumber;
			memcpy(&SndCreBuf[6],Cash_CR_Buf.CashGradeNumber,2);
			SndCreBuf[8] = ETX_C;
			SndCreBuf[9] = CrcEor(SndCreBuf,9);
			CreCommand = ENGROSS;
			CreStatus = CRE_Command_Sending;
			WriteToDBUG_USART(SndCreBuf,10);
			//ProcExchange[PROCESS_CR][PROCESS_CASH] &= (~CASH_KEY_PRESSED);
			CrePreCommand = 0;
		} else {
			if(CrePreCommand) {
				if((ProcStatus[PROCESS_CR] & CARD_INSERTED) && (CRType != CR_TYPE_CASH)) {
					if(ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_KEY_PRESSED) {
						ProcExchange[PROCESS_CASH][PROCESS_CR] &= (~CASH_KEY_PRESSED);
					}
					Cash_CR_Buf.CashCard_Flag = 1;
					if(pCrWait != (Tmr *)0)
					{
						DeleteTimer(pCrWait);
						pCrWait = (Tmr *)0;
					}
					SndCreBuf[0] = STX_C;
					SndCreBuf[1] = 6;
					SndCreBuf[2] = ENGROSS;
					SndCreBuf[3] = Cash_CR_Buf.CashCard_Flag;
					SndCreBuf[4] = Cash_CR_Buf.CashKeyNozzleNumber;
					SndCreBuf[5] = Cash_CR_Buf.CashNozzleNumber;
					memcpy(&SndCreBuf[6],Cash_CR_Buf.CashGradeNumber,2);
					SndCreBuf[8] = ETX_C;
					SndCreBuf[9] = CrcEor(SndCreBuf,9);
					CreCommand = ENGROSS;
					CreStatus = CRE_Command_Sending;
					WriteToDBUG_USART(SndCreBuf,10);
					CrePreCommand = 0;
				}
			}
		}
	}

	// Bellow LLLLA
	if( ProcExchange[PROCESS_POWERDOWN][PROCESS_CASH] & POWERDOWN )
	{
		if( CRESendRecord == 1 )
		{
			// Do Not Issue Power Down, Wait Send Record End
		}
        else
		{
        	// Issuer Power Down
			// Clear Bit
			ProcExchange[PROCESS_POWERDOWN][PROCESS_CASH] &= ( ~POWERDOWN );

			ProcExchange[PROCESS_CASH][PROCESS_POWERDOWN] |= POWERDOWN;
		}
	}
	// Upper LLLLA
	/*if (ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_CARE_RECOGNIZE)
	{
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= ~(CASH_CARE_RECOGNIZE);
		ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_MSG_ERROR;
	}*/
}


#endif
