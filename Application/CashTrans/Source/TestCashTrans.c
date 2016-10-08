

#include "TestCashTrans.h"
#include "CRMemCopy.h"
#include "define.h"
#include "global.h"
#include "tc.h"


// Delay Time Timer
Tmr * testkeycancel;


unsigned char DelayKeyPressed0 = 0;
unsigned char DelayKeyPressed1 = 0;
void CashTranDelayKeyPressed( void )
{
	DelayKeyPressed1 = 1;
}

unsigned char DelayKeyCancel0 = 0;
unsigned char DelayKeyCancel1 = 0;
void CashTranDelayKeyCancel( void )
{
	DelayKeyCancel1 = 1;
}

unsigned char CashAccept0 = 0;
unsigned char CashAccept1 = 0;
void CashAccept( void )
{
	CashAccept1 = 1;
}

unsigned char AcceptCash = 0;
unsigned char AcceptCashTimes = 0;



void testcash( void )
{
	unsigned short int value, value0;

	if( DelayKeyPressed0 == 0 )
	{
		// Delay For Key Pressed, After Reset
   		CreateTimer( TIMER_ONE_DELETE, 19000, CashTranDelayKeyPressed );
		DelayKeyPressed0 = 1;
	}
	else
	{
		if( DelayKeyPressed1 == 1 )
		{
			// Default
			Cash_CR_Buf.CashType = 0x01;
//			Cash_CR_Buf.CashType = 0x00;

			// Time Out, Set Key Pressed
			// Fill Buffer First
			Cash_CR_Buf.CashKeyNozzleNumber	= 0x00;

			// Set Bit
			ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_KEY_PRESSED;
			
			// Clear It, Do Not Issue Again
			DelayKeyPressed1 = 0;  
		}
		else
		{
		}
	}

	if( ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_KEY_PRESSED )
	{
		// Clear Bit
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= ( ~CASH_KEY_PRESSED );

		// IC Card Return, Check It
		if( Cash_CR_Buf.CashCard_Flag == 0x00 )
		{
			// Can Use Cash Trans, Accept It
			AcceptCash = 1;
		}
		else
		{
			// Can Not Use Cash Trans, Set Another Time Out
			DelayKeyPressed0 = 0;
			DelayKeyPressed1 = 0;
			AcceptCash = 0;

			// Delete Timer For Cancel Key
			DeleteTimer( testkeycancel );
		}
	}

	
	if( AcceptCash == 1 )
	{
		// Accept Cash, Begin
		if( Cash_CR_Buf.CashType == 0x00 )
		{
			if( AcceptCashTimes == 0 )
			{
				if( CashAccept0 == 0 )
				{
					// First Entry, Clear Buffer
					memset( Cash_CR_Buf.CashInserted, 0x00, sizeof( Cash_CR_Buf.CashInserted ));
					memset( Cash_CR_Buf.CashLists, 0x00, sizeof( Cash_CR_Buf.CashLists ));
					memset( Cash_CR_Buf.CashTotalInserted, 0x00, sizeof( Cash_CR_Buf.CashTotalInserted ));
				}
			}

			if( CashAccept0 == 0 )
			{
	   			CreateTimer( TIMER_ONE_DELETE, 4000, CashAccept );
				CashAccept0 = 1;
			}
			else
			{
				if( CashAccept1 == 1 )
				{
					if( AcceptCashTimes <= Lists )
					{
						// Accept 1 Cash
						// Cash Amount
						value0 = CashPerAmount;
						ShortIntToCharCopy( value0, Cash_CR_Buf.CashInserted );

						// Cash Lists
						value = CharToShortIntCopy( Cash_CR_Buf.CashLists );
						value += 1;
						ShortIntToCharCopy( value, Cash_CR_Buf.CashLists );

						// Cash Amount Total
						value = CharToShortIntCopy( Cash_CR_Buf.CashTotalInserted );
						value += value0;
						ShortIntToCharCopy( value, Cash_CR_Buf.CashTotalInserted );

						// Set Bit
						ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_INSERTED;
					
						AcceptCashTimes++;

						// Do Not Entry Until Another Timer Setup
						CashAccept0 = 0;
						CashAccept1 = 0;
					}
				}
			}


			if( AcceptCashTimes > Lists )
			{
				if( CashAccept1 == 0 )
				{
					memcpy( &Cash_CR_Buf.CashTotal[2], Cash_CR_Buf.CashTotalInserted, sizeof( Cash_CR_Buf.CashTotalInserted ));

					// Set Bit
					ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_TOTAL;


					// Do Not Entry
					CashAccept0 = 1;
					CashAccept1 = 1;
				}
			}
		}
		else
		{
			// POS Auth, Only Total
			if( CashAccept0 == 0 )
			{
	   			CreateTimer( TIMER_ONE_DELETE, 40000, CashAccept );
				CashAccept0 = 1;
			}
			else
			{
				if( CashAccept1 == 1 )
				{
					IntToCharCopy( CashPerAmount * Lists, Cash_CR_Buf.CashTotal );

					// Fill Auth Code
//					Cash_CR_Buf.CashAuthCode[0]	= 10;
//					memcpy( &Cash_CR_Buf.CashAuthCode[1], "1234567890", 10 );

					Cash_CR_Buf.CashAuthCode[0]	= 0x05;
					memcpy( &Cash_CR_Buf.CashAuthCode[1], "\x64\x99\x11\x24\xFF", 5 );

					// Set Bit
					ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_TOTAL;

					CashAccept1 = 0;
				}
			}
		}
	}
	
//	// Accept Cash, Begin
//	if( Cash_CR_Buf.CashType == 0x00 )
//	{
//		if( AcceptCash == 1 )
//		{
//			if( AcceptCashTimes == 0 )
//			{
//				if( CashAccept0 == 0 )
//				{
//					// First Entry, Clear Buffer
//					memset( Cash_CR_Buf.CashInserted, 0x00, sizeof( Cash_CR_Buf.CashInserted ));
//					memset( Cash_CR_Buf.CashLists, 0x00, sizeof( Cash_CR_Buf.CashLists ));
//					memset( Cash_CR_Buf.CashTotalInserted, 0x00, sizeof( Cash_CR_Buf.CashTotalInserted ));
//				}
//			}
//
//			if( CashAccept0 == 0 )
//			{
//	   			CreateTimer( TIMER_ONE_DELETE, 4000, CashAccept );
//				CashAccept0 = 1;
//			}
//			else
//			{
//				if( CashAccept1 == 1 )
//				{
//					if( AcceptCashTimes <= Lists )
//					{
//						// Accept 1 Cash
//						// Cash Amount
//						value0 = CashPerAmount;
//						ShortIntToCharCopy( value0, Cash_CR_Buf.CashInserted );
//
//						// Cash Lists
//						value = CharToShortIntCopy( Cash_CR_Buf.CashLists );
//						value += 1;
//						ShortIntToCharCopy( value, Cash_CR_Buf.CashLists );
//
//						// Cash Amount Total
//						value = CharToShortIntCopy( Cash_CR_Buf.CashTotalInserted );
//						value += value0;
//						ShortIntToCharCopy( value, Cash_CR_Buf.CashTotalInserted );
//
//						// Set Bit
//						ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_INSERTED;
//					
//						AcceptCashTimes++;
//
//						// Do Not Entry Until Another Timer Setup
//						CashAccept0 = 0;
//						CashAccept1 = 0;
//					}
//				}
//			}
//
//
//			if( AcceptCashTimes > Lists )
//			{
//				if( CashAccept1 == 0 )
//				{
//					memcpy( &Cash_CR_Buf.CashTotal[2], Cash_CR_Buf.CashTotalInserted, sizeof( Cash_CR_Buf.CashTotalInserted ));
//
//					// Set Bit
//					ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_TOTAL;
//
//
//					// Do Not Entry
//					CashAccept0 = 1;
//					CashAccept1 = 1;
//				}
//			}
//		}
//	}
//	else
//	{
//		if( AcceptCash == 1 )
//		{
//			// POS Auth, Only Total
//			if( CashAccept0 == 0 )
//			{
//	   			CreateTimer( TIMER_ONE_DELETE, 40000, CashAccept );
//				CashAccept0 = 1;
//			}
//			else
//			{
//				if( CashAccept1 == 1 )
//				{
//					IntToCharCopy( CashPerAmount * Lists, Cash_CR_Buf.CashTotal );
//
//					// Set Bit
//					ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_TOTAL;
//
//					CashAccept1 = 0;
//				}
//			}
//		}
//	}
//




	if( ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_TRANS_RECORD )
	{
		// Clear Bit
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= ( ~CASH_TRANS_RECORD );

		// Cash Trans Record Ok, Load And Set Another 
		DelayKeyPressed0 = 0;
		DelayKeyPressed1 = 0;
		AcceptCash = 0;

		CashAccept0 = 0;
		CashAccept1 = 0;
		AcceptCashTimes = 0;
	}

	if( ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_AUTH_CODE )
	{
		// Clear Bit
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= ( ~CASH_AUTH_CODE );
	}




/*
	// Cash Trans Cancel
	if( AcceptCash == 1 )
	{
		if( DelayKeyCancel0 == 0 )
		{
			// Delay For Key Cancel
	   		testkeycancel = CreateTimer( TIMER_ONE_DELETE, 24000, CashTranDelayKeyCancel );
			DelayKeyCancel0 = 1;
		}
	}

	if( DelayKeyCancel0 == 1 )
	{
		// Check TimeOut
		if( DelayKeyCancel1 == 1 )
		{
			ProcExchange[PROCESS_CASH][PROCESS_CR] |= CASH_KEY_CANCELLED;
			// Clear Flag
			DelayKeyCancel0 = 0;
			DelayKeyCancel1 = 0;
		}
	}

	// Cancelled, Issue Next
	if( ProcExchange[PROCESS_CR][PROCESS_CASH] & CASH_KEY_CANCELLED )
	{
		// Clear Bit
		ProcExchange[PROCESS_CR][PROCESS_CASH] &= ( ~CASH_KEY_CANCELLED );

		// Can Not Use Cash Trans, Set Another Time Out
		DelayKeyPressed0 = 0;
		DelayKeyPressed1 = 0;
		AcceptCash = 0;
	}
*/
}

//// Card Reader And Cash Process Exchange
//#define CASH_KEY_PRESSED		0x00000001					// CR-E Pressed The Key To Use Cash Function
//															// The Key Number(Phisical Nozzle) Save In Exchange Buffer
//#define CASH_INSERTED			0x00000002					// CR-E Inserted A New CashNote
//#define CASH_TOTAL				0x00000004					// CR-E Accept All CashNotes Or POS Authed OK
//#define CASH_RFU0				0x00000008					// Reserve For Future
//#define CASH_RFU1				0x00000010					// Reserve For Future
//#define CASH_RFU2				0x00000020					// Reserve For Future
//#define CASH_RFU3				0x00000040					// Reserve For Future
//#define CASH_KEY_CANCELLED		0x00000080					// CR-E Cancelled The Apply For Use CR Cash Trans
//
//#define CASH_CARD_READY			0x00000100					// Cash Card Ready For Cash Trans
//															// Wether Accept Cash(Auth) Saved In Exchange Buffer
//#define CASH_TRANS_RECORD		0x00000200					// Cash Trans End And The Trans Record Need Send To CR-E
//#define CASH_AUTH_CODE			0x00000400					// CR Need Send Input Auth Code To CR-E
//#define CASH_RFU4				0x00000800					// Reserve For Future
//// Card Reader And Cash Process Exchange End


////	结构:		
//typedef struct		
//{		
//	unsigned char CashType;						// 设备类型: 0x00: 现金; 0x01: POS授权
//	unsigned char CashVersion[20];              // CR-E程序版本号
//	unsigned char CashKeyNozzleNumber;			// 按键选择的枪号(物理枪号)
//	unsigned char CashCard_Flag;				// 是否可以开始占用CR: 0x00: 可以占用; 0x01不能占用
//	unsigned char CashNozzleNumber;				// CR选择的逻辑枪号
//	unsigned char CashGradeNumber[2];           // MSB  CR选择的油品编号
//	unsigned char CashTotal[4];					// MSB CR-E总共接受的金额或授权允许的金额
//
//	unsigned char CashInserted[2];				// MSB CR-E 刚接受的一张纸币币值: 元
//	unsigned char CashLists[2];					// MSB CR-E 总共接受纸币张数: 张
//	unsigned char CashTotalInserted[2];			// MSB CR-E 总共接受纸币金额: 元
//	
//	union
//	{
//		unsigned char CashAuthCode[13];			// MSB CR-E接受的授权码或CR输入的授权码
//		unsigned char CashTransRecord[96];		// 交易记录, 送到CR-E
//	};
//}CASH_CR;		
//
//extern CASH_CR Cash_CR_Buf;

