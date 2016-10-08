#include <string.h>

#include "global.h"

#include "ICCardCommand.h"
#include "PSAMCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "ICCardProc.h"
#include "Pdprotect.h"
#include "define.h"
#include "GreyCard.h"
#include "CRPumpEOT.h"
#include "WorkMode.h"





/*
 ** ICCard_Grey_Card_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\GreyCard.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Process For The Grey Card, Perform
 *				  Local Inquire Grey Record, Inquire Grey Record In POS
 *				  Debit Grey Unlock, Get TAC, Clear Grey Lock Proof
 *				  Form Grey Unlock Record Or Form Read TAC Record
 *
 *  RETURNS: None.
 *
 */
void ICCard_Grey_Card_Proc( void )
{
	unsigned short int retval;

	// Only Running When CR_Proc_Step = CR_ICCARD_GREY_CARD_PROC
	if( CR_Proc_Step == CR_ICCARD_GREY_CARD_PROC )
	{
		switch( CR_CommandType )
		{
			case CR_CHECK_GREY_RECORD:
				
//				if( TRINP.TR_CARD_GREY_FLAG == 0x10 )
//				{
//					// For 0x10, Do Not Check Grey Record, Just Clear Grey Flag
//					retval = 0x9000;
//				}
//				else
//				{
//					retval = CR_Check_Grey_Record();
//				}

				// Check Grey Record
				retval = CR_Check_Grey_Record();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						CR_GetGreyResponse();

#ifdef MAKE_CARD_MODE2
						if( TRINP.TR_WORK_MODE != 0x01 )
						{
							TRINP.TR_TRANS_TYPE |= ( WORKOFFLINE | LOCALLIST );
							PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
						}
						else
						{
							TRINP.TR_TRANS_TYPE &= ( ~( LOCALLIST | WORKOFFLINE ));
							PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
						}
#endif	// MAKE_CARD_MODE2

						if( TRINP.TR_CARD_GREY_FLAG == 0x10 )
						{
							// TAC Not Read, Read The TAC And Form Read TAC Record
							// Display: "Grey Unlock"
		
							// Clear Grey Flag
							TRINP.TR_TRANS_TYPE &= 0xF0;
							TRINP.TR_TRANS_TYPE |= TRANSGREYFLAG;
							TRINP.TR_TRANS_TYPE |= CARDTACREAD;
	
							// Save The Transaction Type
							PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

//							// Get The Amount And Balance
//							memcpy( TRINP.TR_CARD_APP_BALANCE, TRINP.TR_CARD_GREY_BALANCE, sizeof( TRINP.TR_CARD_GREY_BALANCE ));
//							memcpy( TRINP.TR_TRANS_AMOUNT, TRINP.TR_CARD_GREY_AMOUNT, sizeof( TRINP.TR_CARD_GREY_AMOUNT ));
//							memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT, TRINP.TR_TRANS_AMOUNT, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT ));
//							
//							SaveICCardData();

							// Do Not Check Grey Record, But Get Data Local
							CR_GetGreyResponse_Local();
							
							// Save The Transaction TAC For Grey Flag 0x10
							memcpy( TRINP.TR_CARD_MAC3, TRINP.TR_CARD_GREY_TAC, sizeof( TRINP.TR_CARD_MAC3 ));
							memcpy( PowerDown.PdTrans.PD_CARD_MAC3, TRINP.TR_CARD_MAC3, sizeof( PowerDown.PdTrans.PD_CARD_MAC3 ));

							// Clear Lock Proof Next
							Set_Next_Command( CR_ICCARD_GREY_CARD_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
						}
						else
						{
							// IC Card Debit Unlock Next
							Set_Next_Command( CR_ICCARD_GREY_CARD_PROC, CR_ICCARD_DEBIT_GREY_UNLOCK );
						}
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_32;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_ICCARD_DEBIT_GREY_UNLOCK:
			
				 retval = CR_ICCARD_Debit_Grey_Unlock();
				
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Call: FormRecord( Grey Debit Unlock );

						// Update Balance
						Card_Balance_Update();

						// Unlock Grey
						TRINP.TR_TRANS_TYPE &= 0xF0;
						TRINP.TR_TRANS_TYPE |= TRANGREYUNLOCK;

#ifdef MAKE_CARD_MODE2
#else
						TRINP.TR_TRANS_TYPE |= CARDTACREAD;
#endif	// MAKE_CARD_MODE2
	
						// Save The Transaction Type
						PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

						// Save The Transaction TAC
						memcpy( PowerDown.PdTrans.PD_CARD_MAC3, TRINP.TR_CARD_MAC3, sizeof( PowerDown.PdTrans.PD_CARD_MAC3 ));

						// Clear Lock Proof Next
						Set_Next_Command( CR_ICCARD_GREY_CARD_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
					}
					else
					{
						// Run Error
						// Display: " IC Card Clear Grey Flag Error"
						FillInfoMsg( M_CardUnlockErr, 0, 0 );
					
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_33;
                        ErrorFlag = retval;

						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;

			case CR_ICCARD_CLEAR_LOCK_PROOF:
			
				 retval = CR_ICCARD_Clear_Lock_Proof();
				
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Call: FormRecord( Grey Debit Unlock );

						// Get TTC
						GetTransTTC();

						// Save To PowerDown.PdTrans
						memcpy( PowerDown.PdTrans.PD_TRANS_TTC, TRINP.TR_TRANS_TTC, sizeof( PowerDown.PdTrans.PD_TRANS_TTC ));

						// Get Trans Time
//						GetTransTime();
//						// Save To PowerDown.PdTrans
//						memcpy( PowerDown.PdTrans.PD_TRANS_DATE, TRINP.TR_TRANS_DATE, sizeof( PowerDown.PdTrans.PD_TRANS_DATE ));
//						memcpy( PowerDown.PdTrans.PD_TRANS_TIME, TRINP.TR_TRANS_TIME, sizeof( PowerDown.PdTrans.PD_TRANS_TIME ));

						// Unlock Grey, Do Not Spring IC Card
						HoldCard = 1;

						if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
						{
							// Check Last Undebit Is Loyalty Or Not
							if( TRINP.TR_TRANS_DEBITSOURCE == 0x01 )
							{
								// Forbidden A Select Loyalty Application
								KEYPAD_CR_Buf.APPType = 0x01;

								// Inform IC Card
								ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SELECT_APP;
							}
//Douzhq 20140523 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
							else if (TRINP.TR_TRANS_DEBITSOURCE == 0x00)
							{
								KEYPAD_CR_Buf.APPType = 0x00;
								ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SELECT_APP;
							}
#endif
							else
							{
								// Go On.... Do Nothing
							}
						}
						else
						{
							// Go On.... Do Nothing
						}
						
                        if( CRType == CR_TYPE_CARD )
                        {
                            // Grey Card						
                            // IC Card Just Inserted, Inform Voice Process
							//Douzhq 20140918 Add , Voice Add
							//Douzhq Note
                            //ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_CARD;
                        }

#ifdef MAKE_CASH_CR
						// For Cash Card, Display Message For Retry
						if( CRType == CR_TYPE_CASH )
						{
			  				// Do Not Send Record To CR-E
							CRType = CR_TYPE_CARD;
							HoldCard = 0;
			  				
			  				// Display " Cash Card Error, Retry "
	  						FillInfoMsg( M_CashCardRetry, 0, 0 );
							
							// Can not Use
							Cash_CR_Buf.CashCard_Flag = 0x01;
							ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_KEY_PRESSED;
						}
#endif

						// For Write The Running Log
						JournalFlag = 10;

						// Clear Lock Proof Ok, And Journal It
						Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_34;

//						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_35;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;


			default:
			break;
		}
		
//		// First Check Read TAC Or Not
	}
}




/*
 ** CR_Check_Grey_Record
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\GreyCard.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Grey Record
 *
 *  RETURNS: 1: Checking; 0x9000: Grey Record Found, Go To Next Command
 *			 0: Did Not Find The Grey Record, Stop The Process; 2: Can not Check Grey Record On POS,
 *																Maybe POS Comm Timeout, POS Offline....
 *
 *
 */
unsigned short int CR_Check_Grey_Record( void )
{
	unsigned char GreyFlag;
	unsigned short int retval;

	// Default Running
	retval = 1;

	// Check First Running, Or Not
	if( !( CR_Task_Flag & ICCARD_POS_GREY ))
	{
//		unsigned char FindGrayRecord(unsigned char *dest,unsigned char *source)

		// Check The Grey Recor Local
		// Display: " Check White List Local "
		// Fill The Grey Card Data
		memcpy( GreyInquiry.GreyCardASN, TRINP.TR_CARD_ASN, sizeof( GreyInquiry.GreyCardASN ));
		memcpy( GreyInquiry.GreyCardBalance, TRINP.TR_CARD_GREY_BALANCE, sizeof( GreyInquiry.GreyCardBalance ));
		memcpy( GreyInquiry.GreyCardCTC, TRINP.TR_CARD_GREY_TTC, sizeof( GreyInquiry.GreyCardCTC ));
		GreyInquiry.GreyCardDebitSource = TRINP.TR_TRANS_DEBITSOURCE;
		memcpy( GreyInquiry.GreyCardDate, TRINP.TR_CARD_GREY_DATE, sizeof( GreyInquiry.GreyCardDate ));
		memcpy( GreyInquiry.GreyCardTime, TRINP.TR_CARD_GREY_TIME, sizeof( GreyInquiry.GreyCardTime ));
		
		
		GreyFlag = FindGrayRecord( &GreyResponse.CheckFlag, &GreyInquiry.GreyCardASN[0] );
		if( GreyFlag == 0 )
		{
			// Check Grey Record Local
			if( GreyResponse.CheckFlag == 0 )
			{
				// Grey Record Found
				retval = 0x9000;
			}
			else
			{
				// Check Local Error, Check On POS
//				if( POSStatus == 0 )
				if( ProcStatus[PROCESS_POS] & POS_ONLINE )
				{
					// Set Bit, Local Process Flag
					CR_Task_Flag |= ICCARD_POS_GREY;

					// Clear Pre-Reserved, Maybe
					ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~POS_CHECK_GREY );

					// Display: " Check Grey Record In POS "
					// Set Bit, To Inform POS Check Grey Record
					ProcExchange[PROCESS_CR][PROCESS_POS] |= POS_CHECK_GREY;
				}
				else
				{
					// Display: " Can Not Check Grey Record In POS "
					FillInfoMsg( M_NeedCheckGreyOnPOS, 0, 0 );

					// Pos Offline, Mark Error
					retval = 2;
				}
			}
		}
		else
		{
			// Check Grey Record On POS
			// Check POS Online Or Not
//			if( POSStatus == 0 )
			if( ProcStatus[PROCESS_POS] & POS_ONLINE )
			{
				if( TRINP.TR_CARD_GREY_FLAG == 0x10 )
				{
					// For Grey Flag = 0x10, Clear Grey Flag, Although No Grey Record
					CR_GetGreyResponse_Local();
					
					retval = 0x9000;
				}
				else
				{
					// Set Bit, Local Process Flag
					CR_Task_Flag |= ICCARD_POS_GREY;

					// Clear Pre-Reserved, Maybe
					ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~POS_CHECK_GREY );

					// Display: " Check Grey Record In POS "
					// Set Bit, To Inform POS Check Grey Record
					ProcExchange[PROCESS_CR][PROCESS_POS] |= POS_CHECK_GREY;
				}
			}
			else
			{
				if( TRINP.TR_CARD_GREY_FLAG == 0x10 )
				{
					// For Grey Flag = 0x10, Clear Grey Flag, Although No Grey Record
					CR_GetGreyResponse_Local();
					
					retval = 0x9000;
				}
				else
				{
					// Display: " Can Not Check Grey Record In POS "
					FillInfoMsg( M_NeedCheckGreyOnPOS, 0, 0 );
					// Pos Offline, Mark Error
					retval = 2;
				}
			}
		}
	}
	else
	{
		// Check POS Return
		if( ProcExchange[PROCESS_POS][PROCESS_CR] & POS_CHECK_GREY )
		{
			// POS Check Grey Record Ended
			// Clear The Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~POS_CHECK_GREY );
					
			// Clear Local Process Flag
			CR_Task_Flag &= ( ~ICCARD_POS_GREY );

			// POS Response, Check The Fit Or Not
			if( GreyResponse.CheckFlag == 0 )
			{
				// Display: " Grey Record "
				// Fitted, Found The Grey Record,
				retval = 0x9000;
			}
			else if( GreyResponse.CheckFlag == 1 )
			{
				if( TRINP.TR_CARD_GREY_FLAG == 0x10 )
				{
					// For Grey Flag = 0x10, Clear Grey Flag, Although No Grey Record
					CR_GetGreyResponse_Local();
					
					retval = 0x9000;
				}
				else
				{
					// Display: " No Grey Record "
					FillInfoMsg( M_NoGreyRecord, 0, 0 );
					// UnFitted, Can Not Find The Grey Record,
					retval = 0x00;
				}
			}
			else
			{
				// Error,
				// Display: " Can Not Found Grey In POS "
				FillInfoMsg( M_NeedCheckGreyOnPOS, 0, 0 );
				retval = 0x02;
			}
		}
		else
		{
			// Running, Go on....
		}
	}

	return retval;
}



/*
 ** CR_GetGreyResponse
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\GreyCard.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get Grey Response To TRINP, And Save To PowerDown.PdTrans
 *
 *  RETURNS: None.
 *
 */
void CR_GetGreyResponse( void )
{
	// Get The Grey Message For Debit Unlock
	memcpy( TRINP.TR_CARD_ASN, GreyResponse.GreyCardASN, sizeof( TRINP.TR_CARD_ASN ));
	memcpy( TRINP.TR_CARD_APP_BALANCE, GreyResponse.GreyCardBalance, sizeof( TRINP.TR_CARD_APP_BALANCE ));
	TRINP.TR_TRANS_AMOUNT[0] = 0;
	memcpy( &TRINP.TR_TRANS_AMOUNT[1], GreyResponse.GreyTransAmount, sizeof( TRINP.TR_TRANS_AMOUNT ) - 1);
	memcpy( TRINP.TR_CARD_APP_TTC, GreyResponse.GreyCardCTC, sizeof( TRINP.TR_CARD_APP_TTC ));
	TRINP.TR_TRANS_DEBITSOURCE = GreyResponse.GreyCardDebitSource;
	memcpy( TRINP.TR_TRANS_DATE, GreyResponse.GreyCardDate, sizeof( TRINP.TR_TRANS_DATE ));
	memcpy( TRINP.TR_TRANS_TIME, GreyResponse.GreyCardTime, sizeof( TRINP.TR_TRANS_TIME ));
	memcpy( TRINP.TR_PSAM_GMAC, GreyResponse.GreyTransGMAC, sizeof( TRINP.TR_PSAM_GMAC ));
	memcpy( TRINP.TR_PSAM_TID, GreyResponse.GreyPSAMTID, sizeof( TRINP.TR_PSAM_TID ));
	memcpy( TRINP.TR_PSAM_TTC, GreyResponse.GreyPSAMTTC, sizeof( TRINP.TR_PSAM_TTC ));
							
	// Save To PowerDown.PdTrans
	memcpy( PowerDown.PdTrans.PD_CARD_ASN, TRINP.TR_CARD_ASN, sizeof( PowerDown.PdTrans.PD_CARD_ASN ));
	memcpy( PowerDown.PdTrans.PD_CARD_APP_BALANCE, TRINP.TR_CARD_APP_BALANCE, sizeof( PowerDown.PdTrans.PD_CARD_APP_BALANCE ));
	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT, TRINP.TR_TRANS_AMOUNT, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT ));
	memcpy( PowerDown.PdTrans.PD_CARD_APP_TTC, TRINP.TR_CARD_APP_TTC, sizeof( PowerDown.PdTrans.PD_CARD_APP_TTC ));
	PowerDown.PdTrans.PD_TRANS_DEBITSOURCE = TRINP.TR_TRANS_DEBITSOURCE;
	memcpy( PowerDown.PdTrans.PD_TRANS_DATE, TRINP.TR_TRANS_DATE, sizeof( PowerDown.PdTrans.PD_TRANS_DATE ));
	memcpy( PowerDown.PdTrans.PD_TRANS_TIME, TRINP.TR_TRANS_TIME, sizeof( PowerDown.PdTrans.PD_TRANS_TIME ));
	memcpy( PowerDown.PdTrans.PD_PSAM_GMAC, TRINP.TR_PSAM_GMAC, sizeof( PowerDown.PdTrans.PD_PSAM_GMAC ));
	memcpy( PowerDown.PdTrans.PD_PSAM_TID, TRINP.TR_PSAM_TID, sizeof( PowerDown.PdTrans.PD_PSAM_TID ));
	memcpy( PowerDown.PdTrans.PD_PSAM_TTC, TRINP.TR_PSAM_TTC, sizeof( PowerDown.PdTrans.PD_PSAM_TTC ));

	// Save The PSAM Key Index, Combine With The PD_CARD_KEY_VERSION,
	// To Generate The CARD_KEY In Journal
	PowerDown.PdTrans.PD_PSAM_KEY_INDEX = TRINP.TR_PSAM_KEY_INDEX;

//	// Save The IC Card Key Version
//	TRINP.TR_CARD_KEY_VERSION = TRINP.TR_CARD_ASN[3] & 0x0F;

	// Save The IC Card Key Version
	TRINP.TR_CARD_KEY_VERSION = (( TRINP.TR_CARD_ASN[3] & 0xF0 ) >> 4 );
	TRINP.TR_CARD_KEY_VERSION &= 0x0F;

	// Save The IC Card Key Version
	PowerDown.PdTrans.PD_CARD_KEY_VERSION = TRINP.TR_CARD_KEY_VERSION;

#ifdef MAKE_CARD_MODE2
	// Use TransAmount1 As TransAmount For Debit Unlock
	memcpy( TRINP.TR_TRANS_AMOUNT_T1, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));
	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT_T1, TRINP.TR_TRANS_AMOUNT, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT_T1 ));

	if( GetWorkMode() != 0x01 )
	{
		// Mark TranType
		TRINP.TR_TRANS_TYPE |= WORKOFFLINE;
		PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
	}

#endif	// MAKE_CARD_MODE2

}


/*
 ** CR_GetGreyResponse_Local
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\GreyCard.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: For Grey Flag = 0x10, Just Get And Save The Record Data Local Instead Of Check Grey Record
 *
 *  RETURNS: None.
 *
 */
void CR_GetGreyResponse_Local( void )
{
	// Get The Grey Message Local
	memcpy( GreyResponse.GreyCardASN, TRINP.TR_CARD_ASN, sizeof( GreyResponse.GreyCardASN ));
	memcpy( GreyResponse.GreyCardBalance, TRINP.TR_CARD_GREY_BALANCE, sizeof( GreyResponse.GreyCardBalance ));
	memcpy( GreyResponse.GreyTransAmount, &TRINP.TR_CARD_GREY_AMOUNT[1], sizeof( GreyResponse.GreyTransAmount ));
		
	memcpy( GreyResponse.GreyCardCTC, TRINP.TR_CARD_GREY_TTC, sizeof( GreyResponse.GreyCardCTC ));

	GreyResponse.GreyCardDebitSource = GreyInquiry.GreyCardDebitSource;

	memcpy( GreyResponse.GreyCardDate, TRINP.TR_CARD_GREY_DATE, sizeof( GreyResponse.GreyCardDate ));
	memcpy( GreyResponse.GreyCardTime, TRINP.TR_CARD_GREY_TIME, sizeof( GreyResponse.GreyCardTime ));

	memcpy( GreyResponse.GreyPSAMTID, TRINP.TR_CARD_GREY_PSAM_TID, sizeof( GreyResponse.GreyPSAMTID ));

	// Where Is PSAM TTC From????
//	memcpy( GreyResponse.GreyPSAMTTC, TRINP.TR_PSAM_TTC, sizeof( GreyResponse.GreyPSAMTTC ));
}
