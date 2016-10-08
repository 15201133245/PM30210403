#include <string.h>

#include "global.h"

#include "ICCardCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "ICCardProc.h"
#include "Pdprotect.h"
#include "define.h"
#include "CRPrint.h"
#include "Journal.h"
#include "Message.h"
#include "DispPumpMsg.h"
#include "WorkMode.h"


//extern void ClearInterval( void );

/*
 ** ICCard_Absence_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardProc.c
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION:
 *
 *  RETURNS:
 *
 */
void ICCard_Absence_Proc( void )
{
	unsigned short int retval;

	// Only Running When CR_Proc_Step = CR_ICCARD_ABSENCE_PROC
	if( CR_Proc_Step == CR_ICCARD_ABSENCE_PROC )
	{
		if( PowerDown.PdTrans.PD_TRANS_FLAG != TRANS_START )
		{
//			// Display " Grey Recording "
//			CRFillMsg( MsgPSAMSelectPSEOk );

//			// Do Not Insert Card
//			IdleFlg |= CARD_IN_NOT;

			// A Trans Did Not Completed
			IdleFlg |= ReCallTTC;

			// Need Do Something
			ICCard_Absence();
		}
		else if( PowerDown.PdTrans.PD_MULTI_TRANS_FLAG != MULTI_TRANS_START )
		{
			// Pre - Trans Is Multi Journal Trans, Resumed
			if( PowerDown.PdTrans.PD_MULTI_TRANS_FLAG == LOGON_JOURNAL )
			{
				// Resume Log On Next
				Set_Next_Command( CR_LOGON_PROC, CR_CHECK_NOZZLES );
			}
			else if( PowerDown.PdTrans.PD_MULTI_TRANS_FLAG == LOGOFF_JOURNAL )
			{
				// Resume Log Off Next
				Set_Next_Command( CR_LOGOFF_PROC, CR_CHECK_NOZZLES );
			}
			else if( PowerDown.PdTrans.PD_MULTI_TRANS_FLAG == NON_ICCARD_JOURNAL )
			{
				if( PowerDown.OverFuelling_OfflineTrans == 1 )
				{
					// Need Divided Trans
					PowerDown.OverFuelling_OfflineTrans = 0;

					// Offline Trans Start
					Set_Next_Command( CR_OFFL_TRANS_PROC, CR_OFFLINE_TRANS );
				}
				else
				{
					// Resume Offline Trans Next
					Set_Next_Command( CR_OFFL_TRANS_PROC, CR_CHECK_NOZZLES );
				}
			}
			else if( PowerDown.PdTrans.PD_MULTI_TRANS_FLAG == PRICE_RECV_JOURNAL )
			{
				// Resume Price Recv Trans Next
				Set_Next_Command( CR_PRICE_RECV, CR_CHECK_NOZZLES );
			}
			else
			{
				// Restore To No Multi - Trans
				PowerDown.PdTrans.PD_MULTI_TRANS_FLAG = MULTI_TRANS_START;
			}
		}
		else
		{
			// All Trans Completed
			if( IdleFlg & ReCallTTC )
			{
				// Clear It
				IdleFlg &= ( ~ReCallTTC );
			}

#ifdef MAKE_CASH_CR
// 2011-07-15 Do Not Print Zero Trans Receipt Here
// Sinopec Require Send Record To Auth POS, 
// So Marked Out The Print Zreo Trans Receipt
// 20140818 release for Guizhou
// 2014-8-17 Email from liu mike
// group meeting determined
			if( CheckCashAmount() == 0 )
			{
				// A Cash Trans Reserved, Maybe Just Power On
				// Or Cash Received But Fail To Nozzle On etc....
				// So, Forbidden A Cash Trans Beginning....
				GetCashAmount();
				// Clear the cashtotal for next trans' scan, 20140818
				memset( TRINP.TR_CASH_TOTAL, 0x00, sizeof( TRINP.TR_CASH_TOTAL ));
			}

#endif


			// Check IC Card Inserted Or Not
			retval = CR_CardReaderCheckCard();
			
			if( retval != 1 )
			{
				if( retval == 0x9000 )
				{
					if( ProcStatus[PROCESS_SCHEDULE] & PRICE_UPDATING )
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_196;

						// Price Updating While IC Card Inserted, Spring Out It
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_SPINGOUT_CARD );
					
						return;
					}
					else
					{
						// Just Go On....
					}

                    if( CRType == CR_TYPE_CARD )
                    {
                        if( ProcStatus[PROCESS_PUMP] == PUMP_CALL_E )
                        {
                            // For Write Spring Card Log
                            PowerDownFlag = Pd_Flag_194;
    
                            // Pump Called Before IC Card Inserted, Spring Out It
                            Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_SPINGOUT_CARD );
                        
                            return;
                        }
                        else
                        {
                            // Just Go On....
                        }
                    }
                    
                    if( !(ProcStatus[PROCESS_PUMP] & PUMP_ONLINE) )
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_199;
                        ErrorFlag = 0xE00D;

						// Pump not online, Spring Out It
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_SPINGOUT_CARD );
					
						return;
					}
					else
					{
						// Just Go On....
					}

#ifdef MAKE_CASH_CR
					// Clear Print It For Cash Trans Receipt
					if( ProcStatus[PROCESS_CR] & CARD_WAIT_CASH_PRINT )
					{
						ProcStatus[PROCESS_CR] &= ( ~CARD_WAIT_CASH_PRINT );
						KeypadPrint = KEYPAD_PRINT_OVER;
					} 
#endif

#ifdef MAKE_CARD_MODE2

					// For Sinopec Mode2, Do Not Clear EOT Information, While Card Insert
					// Clear The EOT Information Until Keypressed Or TimeOut
					if( KeepBalScreen == 1 )
					{
						Set_Next_Command( CR_ICCARD_NONEXIST_EXTERNAL_PROC, CR_CommandStart );

						return;
					}

//					// Clear Send Record After Trans, For New Card Insert
//					PowerDown.SendCardRecord = 0x00;

//					if(( PowerDown.PdTrans.PD_TRANS_FLAG == AUTH_RESP )
//						&& ( TRINP.TR_PAY_X == 0x15 ))
//					{
//						// Host Authed, Do Not Clear The TRINP And Get Mode
//						ProcStatus[PROCESS_CR] |= CARD_INSERTED;
//
//						// Backlight Open, For 30 Seconds
//						OpenLcdLight( 0 );
//
//						// Display Default Info Line
//						ResetDefaultInfo();
//
//						// IC Card Just Inserted, Check Last Balance Keep Timer
//						if( KeepBalScreen == 1 )
//						{
//							// Delete The Timer
//							DeleteTimer( BalScreenKeepTimer );
//							KeepBalScreen = 0;
//						}
//
//						// IC Card Just Inserted, Maybe Left The Unsuccessful PRESET Last Trans, Just Clear It
//						if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & PRESET )
//						{
//							// Clear The Bit
//							ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= ( ~PRESET );
//						}
//
//						// Default For IC Card Type
//						CardType = CardContact;
//
//						// Card Ready Next
//						Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
//
//						return;
//					}
//					else
//					{
						// Clear The Trans Buffers
						memset( TRINP.TR_CARD_ATR, 0x00, ( &TRINP.TR_RFU - TRINP.TR_CARD_ATR ));

						// Get Work Mode And Auth Mode
						TRINP.TR_WORK_MODE = GetWorkMode();
						TRINP.TR_AUTH_MODE = GetAuthMode();
//					}
#endif	// MAKE_CARD_MODE2

					// New Card Inserted, Clear The Power Down Trans Saved
//					memset( PowerDown.PdTrans.PD_TRANS_TTC, 0x00, sizeof( PD_TRANS ) - 2 );
					memset( PowerDown.PdTrans.PD_TRANS_TTC, 0x00, ( &PowerDown.PdTrans.PD_CLEAR_FLAG - PowerDown.PdTrans.PD_TRANS_TTC ));

					ProcStatus[PROCESS_CR] |= CARD_INSERTED;

					// Backlight Open, For 30 Seconds
					OpenLcdLight( 0 );

					// Display Default Info Line
					ResetDefaultInfo();

					GetTransTime();

					// IC Card Just Inserted, Check Last Balance Keep Timer
					if( KeepBalScreen == 1 )
					{
						// Delete The Timer
						DeleteTimer( BalScreenKeepTimer );
						KeepBalScreen = 0;
					}

					// IC Card Just Inserted, Maybe Left The Unsuccessful PRESET Last Trans, Just Clear It
					if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & PRESET )
					{
						// Clear The Bit
						ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= ( ~PRESET );
					}

//					ClearInterval();

					// Default For IC Card Type
					CardType = CardTypeStart;

					if( PowerDown.SpecialFunction & CheckPSAMValidate )
					{
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_PSAM_CHECK_VALIDATE );
					}
					else
					{
						if( IcTypeFlg == 0 )
						{
							// For Intelligence Card Reader, Get Card Type Next
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_GET_CARD_TYPE );

							// Delay A Certain Time For Test Approval
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_DELAY );
						}
						else
						{
#ifdef MAKE_CASH_CR
							if( CRType == CR_TYPE_CASH )
							{
								// For Cash Card, Do Not PowerOn, Just Reset
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_RESET );
							}
							else
#endif
							{
								// IC Card Inserted, Run IC Card Inserted Proc Next
								// First, Excute IC Card Power On Command
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWERON_CARD );
							}

//							// IC Card Inserted, Run IC Card Inserted Proc Next
//							// First, Excute IC Card Power On Command
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWERON_CARD );
//
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_PSAM_CHECK_VALIDATE );
						}
					}
				}
				else if( retval == 0xFE )
				{
					// Card Reader Is Error
					CommErrorTimes++;
					// No IC Card Inserted, Check External Proc
					Set_Next_Command( CR_ICCARD_NONEXIST_EXTERNAL_PROC, CR_CommandStart );
				}
				else
				{
					// No IC Card Inserted, Clear The Status Bits
					// Clear The Status Bits
					if( ProcStatus[PROCESS_CR] & CARD_LOGON )
					{
						ProcStatus[PROCESS_CR] = 0;
					
						ProcStatus[PROCESS_CR] |= CARD_LOGON;
					
						HoldCard = 0;

#ifdef MAKE_CASH_CR
						// Maybe Print It
						if( KeypadPrint == KEYPAD_SELECT_PRINT_BEGIN )
						{
							// Display Keypad Selsect Print Screen Next
							ProcStatus[PROCESS_CR] |= CARD_WAIT_CASH_PRINT;
						} 
#endif
					}
					else
					{
						ProcStatus[PROCESS_CR] = 0;
					}

					// No IC Card Inserted, Check External Proc
					Set_Next_Command( CR_ICCARD_NONEXIST_EXTERNAL_PROC, CR_CommandStart );
				}
			}
			else
			{
				// Command Running, Go On
			}
		}
	}
}



/*
 ** ICCard_Absence
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Process For IC Card Absence And Need To Excute Somethings
 *			Include: Form Grey Record, Save Journal etc..
 *
 *  RETURNS: None.
 *
 */
void ICCard_Absence( void )
{
	unsigned int ttc;
//	unsigned short int retval;

//	// Get PSAM TTC
//	memcpy( TRINP.TR_PSAM_TTC, PowerDown.PdTrans.PD_PSAM_TTC, sizeof( TRINP.TR_PSAM_TTC ));
		
//	// Get IC Card MAC2
//	memcpy( TRINP.TR_CARD_MAC2, PowerDown.PdTrans.PD_CARD_MAC2, sizeof( TRINP.TR_CARD_MAC2 ));

	// First Check Write TTC, For Write TTC Maybe Reconstructed By PowerOn, But Fail To Save In PowerDown Process
	ttc = CharToIntCopy( PowerDown.PdTrans.PD_TRANS_TTC );
	// Get TTC
	TransTTC = GetWriteTTC();
						
	if( TransTTC == ttc )
	{
		// Inc TTC
		WriteTTCInc();
	}

#ifdef MAKE_CARD_MODE2
	// IC Card Absence For Sinopec Mode2
	ICCardAbsence_Mode2();
#else
	// Normal Sinopec IC Card Absence Process
	ICCardAbsence();
#endif	// MAKE_CARD_MODE2
	
}

#ifdef MAKE_CASH_CR

/*
 ** CheckCashAmount
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Wether A Cash Trans Reserved in PowerDown Or Not
 *
 *  RETURNS: 0: A Cash Trans Need To Be Go On....; 1: No Cash Trans
 *
 */
unsigned char CheckCashAmount( void )
{
	unsigned int value;
	unsigned char retval;

	retval = 1;

	if( PowerDown.PdTrans.PD_AUTH_CODE_LEN != 0x00 )
	{
		if( PowerDown.PdTrans.PD_AUTH_CODE_LEN == 0xFF )
		{
			// New Running
			PowerDown.PdTrans.PD_AUTH_CODE_LEN = 0x00;
		}
		else
		{
			// A Auth Transaction
//			Cash_CR_Buf.CashType = 0x01;

			// 
			retval = 0;
		}
	}
	else
	{
		// Check A Cash Acceptor Trans
		value = CharToIntCopy( PowerDown.PdTrans.PD_CASH_TOTAL );
		if( value != 0 )
		{
			if( value == 0xFFFFFFFF )
			{
				// New Running
				memset( PowerDown.PdTrans.PD_CASH_TOTAL, 0x00, sizeof( PowerDown.PdTrans.PD_CASH_TOTAL ));
			}
			else
			{
				// A Cash Acceptor Tran
//				Cash_CR_Buf.CashType = 0x00;

				// 
				retval = 0;
			}
		}
	}

	return retval;
}


/*
 ** GetCashAmount
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: IC Card Process Form A Cash Accept To Go On The Unfinished Cash Trans
 *
 *  RETURNS: None.
 *
 */
void GetCashAmount( void )
{
	JOURNAL ZeroRecord; 

	// Retrive The Auth Code And Amount
	Cash_CR_Buf.CashAuthCode[0]	= PowerDown.PdTrans.PD_AUTH_CODE_LEN;
	memcpy( &Cash_CR_Buf.CashAuthCode[1], PowerDown.PdTrans.PD_AUTH_CODE, sizeof( PowerDown.PdTrans.PD_AUTH_CODE ));
	memcpy( Cash_CR_Buf.CashTotal, PowerDown.PdTrans.PD_CASH_TOTAL, sizeof( Cash_CR_Buf.CashTotal ));

	// Set Bit To Check Amount In CR-E, If Yes, CR-E Form A Key Press To Go On The Cash Trans
	ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_CHECK_AMOUNT;



	// Did Not Nozzle On, Just 0 Trans
	// Get Items For Print
	// Zero Trans Amount Record For CR-E End
	memset( &ZeroRecord.JOURNALSIGN, 0x00, sizeof( JOURNAL ));

	// Begin Zero Trans Record
	ZeroRecord.JOURNALSIGN = 0xAA;

	// Get Logic Nozzle Number
	ZeroRecord.NOZZLENUMBER = PowerDown.PdTrans.PD_NOZZLENUMBER;
	
	// Get Oil Type
	memcpy( ZeroRecord.GRADE_CODE, PowerDown.PdTrans.PD_GRADE_CODE, sizeof( PowerDown.PdTrans.PD_GRADE_CODE ));

	// Get Price
	memcpy( ZeroRecord.TRANS_PRICE, PowerDown.PdTrans.PD_TRANS_PRICE, sizeof( PowerDown.PdTrans.PD_TRANS_PRICE ));

	// Fill Time
	memcpy( ZeroRecord.TRANS_DATE, PowerDown.PdTrans.PD_TRANS_DATE, sizeof( PowerDown.PdTrans.PD_TRANS_DATE ));
	memcpy( ZeroRecord.TRANS_TIME, PowerDown.PdTrans.PD_TRANS_TIME, sizeof( PowerDown.PdTrans.PD_TRANS_TIME ));

	// Fill RFU
	// Retrive The Auth Code And Amount
	memcpy( ZeroRecord.RFU, PowerDown.PdTrans.PD_CASH_TOTAL, sizeof( PowerDown.PdTrans.PD_CASH_TOTAL ));
	ZeroRecord.RFU[4] = Cash_CR_Buf.CashAuthCode[0];
	
	Asc2Hex( &ZeroRecord.RFU[5], &Cash_CR_Buf.CashAuthCode[1], ( ZeroRecord.RFU[4] + 1 ) / 2 );

	// Print Zero Trans Receipt Only The PowerDown Not Issued
	if( !( ProcExchange[PROCESS_POWERDOWN][PROCESS_CR] & POWERDOWN ))
	{
		// Do Not Go On The Cash Trans, Just Print The Receipt For Change
		if( PowerDown.PdTrans.PD_AUTH_CODE_LEN != 0x00 )
		{
			// Do Not Print It Again
			KeypadPrint = KEYPAD_PRINT_OVER; 

			// Save To Print Buffer
			FillPrintBuffer( CASH_POS_RECEIPT, TRINP.TR_STATION_ID, ZeroRecord.TRANS_TTC );
		}
		else
		{
			// Do Not Print It Again
			KeypadPrint = KEYPAD_PRINT_OVER; 

			// Save To Print Buffer
			FillPrintBuffer( CASH_ACCEPTOR_RECEIPT, TRINP.TR_STATION_ID, ZeroRecord.TRANS_TTC );
		}
	}
	else
	{
		// Do Nothing, Just Go On....
	}
	
	memset( PowerDown.PdTrans.PD_CASH_TOTAL, 0x00, sizeof( PowerDown.PdTrans.PD_CASH_TOTAL ));
	PowerDown.PdTrans.PD_AUTH_CODE_LEN = 0;
	memset( PowerDown.PdTrans.PD_AUTH_CODE, 0x00, sizeof( PowerDown.PdTrans.PD_AUTH_CODE ));

	// Do Not Entry Again
	CRType = CR_TYPE_CARD;

	// Inform Cash Process
	memcpy( Cash_CR_Buf.CashTransRecord, &ZeroRecord.JOURNALSIGN, sizeof( JOURNAL ));

	// Only Send Record For Authed Len != 0
	if( ZeroRecord.RFU[4] != 0 )
	{
		ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_TRANS_RECORD;
	}
}

//Douzhq 20141013 Add , For Bug
//While CRE Send A Auth , But CR Is Not On Wait Auth Status
//CR Send A Zero Trans , To Cash POS
//------------------Down-----------------------------------------------

struct ZEROCASHTRANS
{
	unsigned char ZERO_AUTH_CODE_LEN;
	unsigned char ZERO_AUTH_CODE[12];
	unsigned char ZERO_NOZZLENUMBER;
	unsigned char ZERO_GRADE_CODE[2];
	unsigned char ZERO_TRANS_PRICE[2];
	unsigned char ZERO_CASH_TOTAL[4];
	unsigned char ZERO_TRANS_DATE[4];
	unsigned char ZERO_TRANS_TIME[3];
};

void GetCashAmountZero( void )
{
	JOURNAL ZeroRecord; 
	unsigned char buf[12];
	unsigned char val,num;
	unsigned short int TmpGrade;
	struct ZEROCASHTRANS ZeroCashTrans;
	
	ZeroCashTrans.ZERO_AUTH_CODE_LEN = (Cash_CR_Buf.CashAuthCode[1] >> 4) + 2;
	//TRINP.TR_AUTH_CODE_LEN = PowerDown.PdTrans.PD_AUTH_CODE_LEN;
	
	num = 0;
	memset( buf, 0x00, sizeof( buf ));
	for (val = 1;val < (sizeof(Cash_CR_Buf.CashAuthCode) - 1);val++)
	{
		if (Cash_CR_Buf.CashAuthCode[val] == 0xff)
		{
			break;
		}
		else
		{
			num++;
		}
	}
	
	BcdToAscii((char*)buf,(char*)(&Cash_CR_Buf.CashAuthCode[1]),num);
	
	memset(ZeroCashTrans.ZERO_AUTH_CODE,0,sizeof(ZeroCashTrans.ZERO_AUTH_CODE));
	memcpy(ZeroCashTrans.ZERO_AUTH_CODE,buf,sizeof(ZeroCashTrans.ZERO_AUTH_CODE));
	//memcpy(TRINP.TR_AUTH_CODE,PowerDown.PdTrans.PD_AUTH_CODE,sizeof(TRINP.TR_AUTH_CODE));
	
	//BcdToAscii(buf,&Cash_CR_Buf.CashAuthCode[1],);
	/*
	memset( buf, 0x00, sizeof( buf ));
	Hex2Asc( buf, &Cash_CR_Buf.CashAuthCode[1], Cash_CR_Buf.CashAuthCode[0] );
	
	for( val = 0; val < Cash_CR_Buf.CashAuthCode[0] * 2; val++ )
	{
		if(( buf[val] == 'F' ) || ( buf[val] == 'f' ))
		{
			buf[val] = 0x00;
			break;
		}
	}
	
	memset(PowerDown.PdTrans.PD_AUTH_CODE,0,sizeof(PowerDown.PdTrans.PD_AUTH_CODE));
	memcpy(PowerDown.PdTrans.PD_AUTH_CODE,buf,sizeof(PowerDown.PdTrans.PD_AUTH_CODE));
	*/
	
	memset(ZeroCashTrans.ZERO_CASH_TOTAL,0,sizeof(ZeroCashTrans.ZERO_CASH_TOTAL));
	memcpy(ZeroCashTrans.ZERO_CASH_TOTAL,Cash_CR_Buf.CashTotal,sizeof(ZeroCashTrans.ZERO_CASH_TOTAL));
	
	// Retrive The Auth Code And Amount
	Cash_CR_Buf.CashAuthCode[0]	= ZeroCashTrans.ZERO_AUTH_CODE_LEN;
	memcpy( &Cash_CR_Buf.CashAuthCode[1], ZeroCashTrans.ZERO_AUTH_CODE, sizeof( ZeroCashTrans.ZERO_AUTH_CODE ));
	memcpy( Cash_CR_Buf.CashTotal, ZeroCashTrans.ZERO_CASH_TOTAL, sizeof( Cash_CR_Buf.CashTotal ));
	
	// Set Bit To Check Amount In CR-E, If Yes, CR-E Form A Key Press To Go On The Cash Trans
	ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_CHECK_AMOUNT;



	// Did Not Nozzle On, Just 0 Trans
	// Get Items For Print
	// Zero Trans Amount Record For CR-E End
	memset( &ZeroRecord.JOURNALSIGN, 0x00, sizeof( JOURNAL ));

	// Begin Zero Trans Record
	ZeroRecord.JOURNALSIGN = 0xAA;

	// Get Logic Nozzle Number
	ZeroCashTrans.ZERO_NOZZLENUMBER = GetLogicNzn(0);
	ZeroRecord.NOZZLENUMBER = ZeroCashTrans.ZERO_NOZZLENUMBER;
	//TRINP.TR_TRANS_NOZZLE_NUMBER = PowerDown.PdTrans.PD_NOZZLENUMBER;
	
	// Get Oil Type
	TmpGrade = GetO_Type( 0 );
	ShortIntToCharCopy( TmpGrade, ZeroCashTrans.ZERO_GRADE_CODE );
	memcpy( ZeroRecord.GRADE_CODE, ZeroCashTrans.ZERO_GRADE_CODE, sizeof( ZeroCashTrans.ZERO_GRADE_CODE ));
	
	
	// Get Price
	TmpGrade = GetPrice( 0, 0 );
	ShortIntToCharCopy( TmpGrade, ZeroCashTrans.ZERO_TRANS_PRICE );
	memcpy( ZeroRecord.TRANS_PRICE, ZeroCashTrans.ZERO_TRANS_PRICE, sizeof( ZeroCashTrans.ZERO_TRANS_PRICE ));

	// Fill Time
	GetTransTimeZero(ZeroCashTrans.ZERO_TRANS_DATE,ZeroCashTrans.ZERO_TRANS_TIME);
	memcpy( ZeroRecord.TRANS_DATE, ZeroCashTrans.ZERO_TRANS_DATE, sizeof( ZeroCashTrans.ZERO_TRANS_DATE ));
	memcpy( ZeroRecord.TRANS_TIME, ZeroCashTrans.ZERO_TRANS_TIME, sizeof( ZeroCashTrans.ZERO_TRANS_TIME ));

	// Fill RFU
	// Retrive The Auth Code And Amount
	memcpy( ZeroRecord.RFU, ZeroCashTrans.ZERO_CASH_TOTAL, sizeof( ZeroCashTrans.ZERO_CASH_TOTAL ));
	ZeroRecord.RFU[4] = Cash_CR_Buf.CashAuthCode[0];
	Asc2Hex( &ZeroRecord.RFU[5], &Cash_CR_Buf.CashAuthCode[1], ( ZeroRecord.RFU[4] + 1 ) / 2 );

	// Print Zero Trans Receipt Only The PowerDown Not Issued
	if( !( ProcExchange[PROCESS_POWERDOWN][PROCESS_CR] & POWERDOWN ))
	{
		// Do Not Go On The Cash Trans, Just Print The Receipt For Change
		if( ZeroCashTrans.ZERO_AUTH_CODE_LEN != 0x00 )
		{
			// Do Not Print It Again
			KeypadPrint = KEYPAD_PRINT_OVER; 

			// Save To Print Buffer
			FillPrintBuffer( CASH_POS_RECEIPT, TRINP.TR_STATION_ID, ZeroRecord.TRANS_TTC );
		}
		else
		{
			// Do Not Print It Again
			KeypadPrint = KEYPAD_PRINT_OVER; 

			// Save To Print Buffer
			FillPrintBuffer( CASH_ACCEPTOR_RECEIPT, TRINP.TR_STATION_ID, ZeroRecord.TRANS_TTC );
		}
	}
	else
	{
		// Do Nothing, Just Go On....
	}
						
	//memset( PowerDown.PdTrans.PD_CASH_TOTAL, 0x00, sizeof( PowerDown.PdTrans.PD_CASH_TOTAL ));
	//PowerDown.PdTrans.PD_AUTH_CODE_LEN = 0;
	//memset( PowerDown.PdTrans.PD_AUTH_CODE, 0x00, sizeof( PowerDown.PdTrans.PD_AUTH_CODE ));

	// Do Not Entry Again
	CRType = CR_TYPE_CARD;

	// Inform Cash Process
	memcpy( Cash_CR_Buf.CashTransRecord, &ZeroRecord.JOURNALSIGN, sizeof( JOURNAL ));

	// Only Send Record For Authed Len != 0
	if( ZeroRecord.RFU[4] != 0 )
	{
		ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_TRANS_RECORD;
	}
}
//------------------ Up -----------------------------------------------

#endif



/*
 ** ICCardAbsence
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Sinopec IC Card Absence Process For Grey Card, Journal etc
 *
 *  RETURNS: None.
 *
 */
void ICCardAbsence( void )
{
	if( PowerDown.PdTrans.PD_TRANS_FLAG == JOURNALIT )
	{
		// For Write The Running Log
		JournalFlag = 13;

		// The Last Record Was Not Saved, Save It
		Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
	}
	else if( PowerDown.PdTrans.PD_TRANS_FLAG == SET_GREY )
	{
		// The Last Transaction Was Just Sent The Set Grey Command
		// And Uncertain The Current Status,
		// Need Process Future, Do Nothing Currently

		PowerDown.PdTrans.PD_TRANS_FLAG = TRANS_START;
	}
	else if( PowerDown.PdTrans.PD_TRANS_FLAG == MAC2_RECEIVED )
	{
//		// Display: " IC Card Not Exist, While Fuelling"
//		FillInfoMsg( M_ReadLastTrans, 0, 0 );

		ReadPDBuffer();

		// Get Phiscal Nozzle Number
		//Pump_CR_Buf.NozzleNumber = ( GetPhysicsNzn( PowerDown.PdTrans.PD_NOZZLENUMBER ) + 1 );
        
        // Does't Get Physical Nozzle Number From Logical Nozzle Number, Cause Logical Nozzle Number Maybe Change While Fueling
        Pump_CR_Buf.NozzleNumber = PowerDown.PDPhysicalNozzleNumber;

		// The Last Transaction Was Received MAC2,
		// Then Get Last Trans Next
		Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_GET_LAST_TRANS );
	}
	else if	( PowerDown.PdTrans.PD_TRANS_FLAG == GET_LAST_TRANS )
	{
		ReadPDBuffer();
		
		// Get Last Trans, IC Card Certificate PSAM Next
		Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
	}
	else if( PowerDown.PdTrans.PD_TRANS_FLAG == GMAC_RECEIVED )
	{
		// For Write The Running Log
		JournalFlag = 14;

		// GAMC Received, Form Grey Record Next
		Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
	}
	else if( PowerDown.PdTrans.PD_TRANS_FLAG == UNLOCK_GREY )
	{
		// For Write The Running Log
		JournalFlag = 15;

		// Form Grey Record Next
		Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
	}
	else if( PowerDown.PdTrans.PD_TRANS_FLAG == CLEAR_GREY_FLAG )
	{
		// Clear Grey Flag OK, Form Normal Record Next
		TRINP.TR_TRANS_TYPE &= 0xF0;
		TRINP.TR_TRANS_TYPE |= TRANSOK;

		// For Write The Running Log
		JournalFlag = 16;

		Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
	}
	else
	{
		// Wrong Status, Don't Care
		PowerDown.PdTrans.PD_TRANS_FLAG = TRANS_START;
	}
}


#ifdef MAKE_CARD_MODE2

/*
 ** ICCardAbsence_Mode2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: IC Card Absence For Sinopec Mode2 For Auth Cancel, Grey Card etc.... Special For Mode2
 *
 *  RETURNS: None.
 *
 */
void ICCardAbsence_Mode2( void )
{
	if( SendCardRecord == 0x01 )
	{
		// Do Not Send Card Log
		// Clear Flag
		SendCardRecord = 0x00;
	}
	else
	{
		// Do Nothing, Go On....
	}

	if( PowerDown.PdTrans.PD_WORK_MODE == 0x01 )
	{
		// Clear The Work Offline Bits In TransType, Must Use Host Info
		if( PowerDown.PdTrans.PD_TRANS_TYPE & CARDERROR )
		{
			// Card Error Trans, Do Not Change
		}
		else
		{
			PowerDown.PdTrans.PD_TRANS_TYPE &= ( ~( LOCALLIST | WORKOFFLINE ));
		}

		// Work Online
		if( PowerDown.PdTrans.PD_TRANS_FLAG == SEND_AUTH )
		{
			// After Send Auth, Should Be Auth Cancel Record Next
			TRINP.TR_TRANS_TYPE &= 0x00;
			TRINP.TR_TRANS_TYPE |= TRANSCANCELAUTH;
			PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

			// For Write The Running Log
			JournalFlag = 25;

			// The Last Record Was Not Saved, Save It
			Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == AUTH_RESP )
		{
			// After Auth Received, Should Be Auth Cancel Record Next
			TRINP.TR_TRANS_TYPE &= 0x00;
			TRINP.TR_TRANS_TYPE |= TRANSCANCELAUTH;
			PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

			// For Write The Running Log
			JournalFlag = 26;

			// The Last Record Was Not Saved, Save It
			Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == SEND_FUELLING )
		{
			ReadPDBuffer();

			// After Fuelling And Send Data To Host, Should Be Online Abnormal Or Grey Next, Record Next
			if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
			{
				if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
				{
					// Host Account Payment
					// Online Abormal Record
					TRINP.TR_TRANS_TYPE &= 0x00;
					TRINP.TR_TRANS_TYPE |= TRANSONLINEABN;
					PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
					
					// Online Normal Record
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
				}
				else
				{
					// Grey Record, Remain It, Go On....
					// Get Last Trans, IC Card Certificate PSAM Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
				}
			}
			else
			{
				if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
				{
					// Grey Record, Remain It, Go On....
					// Get Last Trans, IC Card Certificate PSAM Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
				}
				else if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
				{
					// Grey Record, Remain It, Go On....
					// Get Last Trans, IC Card Certificate PSAM Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
				}
				else
				{
					// Employee Card
					// Online Abormal Record
					TRINP.TR_TRANS_TYPE &= 0x00;
					TRINP.TR_TRANS_TYPE |= TRANSONLINEABN;
					PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
					
					// Online Normal Record
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
				}
			}

//			// For Write The Running Log
//			JournalFlag = 27;
//
//			// The Last Record Was Not Saved, Save It
//			Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == FUELLING_RESP )
		{
			ReadPDBuffer();

			// After Fuelling And Send Data To Host, Should Be Normal Or Grey Next, Record Next
			if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
			{
				if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
				{
					// Host Account Payment
					// Online Normal Record
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
				}
				else
				{
					// Grey Record, Remain It, Go On....
					// Get Last Trans, IC Card Certificate PSAM Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
				}
			}
			else
			{
				if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
				{
					// Grey Record, Remain It, Go On....
					// Get Last Trans, IC Card Certificate PSAM Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
				}
				else if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
				{
					// Grey Record, Remain It, Go On....
					// Get Last Trans, IC Card Certificate PSAM Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
				}
				else
				{
					// Employee Card
					// Online Normal Record
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
				}
			}

//			// For Write The Running Log
//			JournalFlag = 28;
//
//			// The Last Record Was Not Saved, Save It
//			Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == JOURNALIT )
		{
			// For Write The Running Log
			JournalFlag = 21;

			// The Last Record Was Not Saved, Save It
			Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == SET_GREY )
		{
			// The Last Transaction Was Just Sent The Set Grey Command
			// And Uncertain The Current Status,
			// Need Process Future, Do Nothing Currently

			PowerDown.PdTrans.PD_TRANS_FLAG = TRANS_START;
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == MAC2_RECEIVED )
		{
			ReadPDBuffer();

			// Get Phiscal Nozzle Number
			Pump_CR_Buf.NozzleNumber = ( GetPhysicsNzn( PowerDown.PdTrans.PD_NOZZLENUMBER ) + 1 );

			// The Last Transaction Was Received MAC2,
			// Then Get Last Trans Next
			Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_GET_LAST_TRANS );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == AUTH_PUMP )
		{
			ReadPDBuffer();

			// Get Phiscal Nozzle Number
			Pump_CR_Buf.NozzleNumber = ( GetPhysicsNzn( PowerDown.PdTrans.PD_NOZZLENUMBER ) + 1 );

			// The Last Transaction Was Received MAC2,
			// Then Get Last Trans Next
			Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_GET_LAST_TRANS );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == DEBIT_ZERO )
		{
			ReadPDBuffer();

			// After Debit Zero, Should Be Online Abnormal Next Record Next
			TRINP.TR_TRANS_TYPE &= 0x00;
			TRINP.TR_TRANS_TYPE |= TRANSONLINEABN;
			PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

			// Get Phiscal Nozzle Number
			Pump_CR_Buf.NozzleNumber = ( GetPhysicsNzn( PowerDown.PdTrans.PD_NOZZLENUMBER ) + 1 );

			// The Last Transaction Was Received MAC2,
			// Then Get Last Trans Next
			Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_GET_LAST_TRANS );
		}
		else if	( PowerDown.PdTrans.PD_TRANS_FLAG == GET_LAST_TRANS )
		{
			ReadPDBuffer();
			
			if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
			{
				// Customer Card, Certificate Grey Or Just Journal
				if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
				{
					// Get Last Trans, IC Card Journal Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
				}
				else
				{
					// Get Last Trans, IC Card Certificate PSAM Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
				}
			}
			else
			{
				if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
				{
					// Grey Record, Remain It, Go On....
					// Get Last Trans, IC Card Certificate PSAM Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
				}
				else if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
				{
					// Grey Record, Remain It, Go On....
					// Get Last Trans, IC Card Certificate PSAM Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
				}
				else
				{
					// Employee Card
					// Get Last Trans, Employee Card, Journal Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
				}
			}
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == GMAC_RECEIVED )
		{
			// For Write The Running Log
			JournalFlag = 22;

			// GAMC Received, Form Grey Record Next
			Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == UNLOCK_GREY )
		{
			// For Write The Running Log
			JournalFlag = 23;

			// Form Grey Record Next
			Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == CLEAR_GREY_FLAG )
		{
			// Clear Grey Flag OK, Form Normal Record Next
			TRINP.TR_TRANS_TYPE &= 0xF0;
			TRINP.TR_TRANS_TYPE |= TRANSOK;

			// For Write The Running Log
			JournalFlag = 24;

			Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
		}
		else
		{
			// Wrong Status, Don't Care
			PowerDown.PdTrans.PD_TRANS_FLAG = TRANS_START;
		}
	}
	else
	{
		// Work Offline
		if( PowerDown.PdTrans.PD_TRANS_FLAG == JOURNALIT )
		{
			// For Write The Running Log
			JournalFlag = 17;

			// The Last Record Was Not Saved, Save It
			Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == SET_GREY )
		{
			// The Last Transaction Was Just Sent The Set Grey Command
			// And Uncertain The Current Status,
			// Need Process Future, Do Nothing Currently

			PowerDown.PdTrans.PD_TRANS_FLAG = TRANS_START;
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == MAC2_RECEIVED )
		{
			ReadPDBuffer();

			// Get Phiscal Nozzle Number
			Pump_CR_Buf.NozzleNumber = ( GetPhysicsNzn( PowerDown.PdTrans.PD_NOZZLENUMBER ) + 1 );

			// The Last Transaction Was Received MAC2,
			// Then Get Last Trans Next
			Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_GET_LAST_TRANS );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == AUTH_PUMP )
		{
			ReadPDBuffer();

			// Get Phiscal Nozzle Number
			Pump_CR_Buf.NozzleNumber = ( GetPhysicsNzn( PowerDown.PdTrans.PD_NOZZLENUMBER ) + 1 );

			// The Last Transaction Was Received MAC2,
			// Then Get Last Trans Next
			Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_GET_LAST_TRANS );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == DEBIT_ZERO )
		{
			ReadPDBuffer();

			// Get Phiscal Nozzle Number
			Pump_CR_Buf.NozzleNumber = ( GetPhysicsNzn( PowerDown.PdTrans.PD_NOZZLENUMBER ) + 1 );

			// The Last Transaction Was Received MAC2,
			// Then Get Last Trans Next
			Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_GET_LAST_TRANS );
		}
		else if	( PowerDown.PdTrans.PD_TRANS_FLAG == GET_LAST_TRANS )
		{
			ReadPDBuffer();
			
			if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
			{
				// Customer Card, Certificate Grey Or Just Journal
				if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
				{
					// Get Last Trans, IC Card Journal Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
				}
				else
				{
					// Get Last Trans, IC Card Certificate PSAM Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
				}
			}
			else
			{
				if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
				{
					// Grey Record, Remain It, Go On....
					// Get Last Trans, IC Card Certificate PSAM Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
				}
				else if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
				{
					// Grey Record, Remain It, Go On....
					// Get Last Trans, IC Card Certificate PSAM Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
				}
				else
				{
					// Employee Card
					// Get Last Trans, Employee Card, Journal Next
					Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
				}
			}
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == GMAC_RECEIVED )
		{
			// For Write The Running Log
			JournalFlag = 18;

			// GAMC Received, Form Grey Record Next
			Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == UNLOCK_GREY )
		{
			// For Write The Running Log
			JournalFlag = 19;

			// Form Grey Record Next
			Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == CLEAR_GREY_FLAG )
		{
			// Clear Grey Flag OK, Form Normal Record Next
			TRINP.TR_TRANS_TYPE &= 0xF0;
			TRINP.TR_TRANS_TYPE |= TRANSOK;

			// For Write The Running Log
			JournalFlag = 20;

			Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
		}
		else
		{
			// Wrong Status, Don't Care
			PowerDown.PdTrans.PD_TRANS_FLAG = TRANS_START;
		}
	}
}








#endif	// MAKE_CARD_MODE2
