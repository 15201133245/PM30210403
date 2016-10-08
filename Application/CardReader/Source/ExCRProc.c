#include <string.h>
#include "global.h"
#include "ICCardCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "ICCardProc.h"
#include "Pdprotect.h"
#include "define.h"
#include "ExCRProc.h"
#include "Message.h"
#include "Display.h"
#include "CRPumpBusy.h"
#include "WorkMode.h"
#include "Pump.h"

//Douzhq 20140610 Add , IC Card Recognize
#ifndef MAKE_CASH_CR
#include "SocialCard.h"
#endif

/*
 ** ICCard_Exist_External_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ExCRProc.c
 *
 *  PARAMETERS: None.
 *
 *  DESCRIPTION: Check Wether Other Process Need IC Card Process Fullfill Tasks
 *				 And, Perform The Task, If Needed, While IC Card Exist
 *
 *  RETURNS: None.
 *
 *	Note: Task Will Be:
 *			Keypad: SPRING_CARD, EMP_LOG_ON, EMP_LOG_OFF, SELECT_APP, SELECT_PAY_MODE
 *			POS: PRICE_RECV
 *			PUMP: PUMP_CALL
 *			Schedule: PRICEUPDATE_LOGOFF
 *			
 *
 */
void ICCard_Exist_External_Proc( void )
{
	unsigned short int retval;
	unsigned int keypadflag = 0;
	unsigned int POSflag = 0;
	unsigned int PUMPflag = 0;
	unsigned int Schflag = 0;

#ifdef MAKE_CASH_CR
	unsigned int Cashflag = 0;
#endif

	// Get flags
#ifdef MAKE_CARD_MODE2
	keypadflag = SPRING_CARD | EMP_LOG_ON |	EMP_LOG_OFF | SELECT_APP | SELECT_PAY_MODE | AUTH_CODE | PRESET_MODE2;
#else
	keypadflag = SPRING_CARD | EMP_LOG_ON |	EMP_LOG_OFF | SELECT_APP | SELECT_PAY_MODE | AUTH_CODE;
#endif	// MAKE_CARD_MODE2

#ifdef MAKE_CARD_MODE2
	POSflag = PRICE_RECV | HOST_STOP | AUTH_HOST;
#else
	POSflag = PRICE_RECV;
#endif	// MAKE_CARD_MODE2

	PUMPflag = PUMP_CALL | PUMP_OFFLINE_TRANS;

#ifdef MAKE_CARD_MODE2
	Schflag = PRICEUPDATE_LOGOFF | PRICE_UPDATED;
#else
	Schflag = PRICEUPDATE_LOGOFF;
#endif

#ifdef MAKE_CASH_CR
	Cashflag = CASH_KEY_PRESSED | CASH_KEY_CANCELLED | CASH_INSERTED | CASH_TOTAL;
#endif


	// Check KeyPad Process Task
	if(( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & keypadflag ) != 0 )
	{
		// Keypad Has Task
		if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & SPRING_CARD )
		{
			// Keypad Need Spring Card
			// Clear Bit
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~SPRING_CARD );

			// Inform Keypad Process
			ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= SPRING_CARD;

			// Power Off And Spring Out
			HoldCard = 0;

			// For Write Spring Card Log
			PowerDownFlag = Pd_Flag_26;
            ErrorFlag = 0x9000;

			Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
		}
//		else if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & PRESET )
//		{
//			// Keypad Has Preset
//			// Do Nothing, Untill The Nozzle On
//		}
#ifdef MAKE_CARD_MODE2
		else if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & PRESET_MODE2 )
		{
			// Clear Bit
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~PRESET_MODE2 );

			// Keypad Has Preset For Sinopec Mode2
			if( ProcStatus[PROCESS_CR] & CARD_LOGON )
			{
				TRINP.TR_WORK_MODE = GetWorkMode();

				if( TRINP.TR_WORK_MODE == 0x01 )
				{
					// Mark Preset In CardReady
					PresetMode2 = 1;

					// Check Limit And Send Auth To POS Next
					Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_LIMIT );


//					// Send Auth To POS Next
//					Set_Next_Command( CR_ICCARD_READY_PROC, CR_WAITING_MAC );
				}
				else
				{
					// Inform Offline, And Clear The Message
					// Display " Work Offline"
					FillInfoMsg( M_OfflineNotPreset, 0, 0 );
				}
			}
			else
			{
				// Inform Offduty, And Clear The Message
				// Display " Not Log On"
				FillInfoMsg( M_OffDutyNotPreset, 0, 0 );
			}
		}
#endif	// MAKE_CARD_MODE2
		else if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & EMP_LOG_ON )
		{
			// Keypad Request Employee Log On
			// Clear The Bit
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~EMP_LOG_ON );
			
			if( !( ProcStatus[PROCESS_CR] & CARD_LOGON ))
			{
				if( TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD )
				{
					// Display " Employee Password Please"
					FillInfoMsg( M_EMP_PWD, 0, 0 );

					// Not Log On, Begin To Log On IC Card Next
					Set_Next_Command( CR_LOGON_PROC, CR_WAITING_EMP_PASSWORD );
				}
				else
				{
					// Display " Not Employee Card"
					// Return To Card Ready
					// And Check For Card
					Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
				}
			}
			else
			{
				// Display " Already Logged On"
				// Return To Card Ready
				// And Check For Card
				Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
			}
		}
		else if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & EMP_LOG_OFF )
		{
			// Keypad Request Employee Log Off
			// Clear The Bit
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~EMP_LOG_OFF );
			
			if( ProcStatus[PROCESS_CR] & CARD_LOGON )
			{
				if(( TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD ) || ( TRINP.TR_CARD_ASN[2] == TEST_CARD ))
				{
					// Display " Employee Password Please"
					FillInfoMsg( M_EMP_PWD, 0, 0 );

					// Log On, Begin To Log Off IC Card Next
					Set_Next_Command( CR_LOGOFF_PROC, CR_WAITING_EMP_PASSWORD );
				}
				else
				{
					// For Write Spring Card Log
					PowerDownFlag = Pd_Flag_27;

					// Display " Not Employee Card"
					// Return To Spring Out Card
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
				}
			}
			else
			{
				// For Write Spring Card Log
				PowerDownFlag = Pd_Flag_28;
                ErrorFlag = 0x9000;

				// Display " Already Logged Off"
				// Return To Spring Out Card
				Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
			}
		}
		else if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & SELECT_APP )
		{
			// Display " Please Waiting "
			FillInfoMsg( M_Wait, 0, 0 );

			// Clear The Card Ready Status Bits????
			// Or Do Not Clear, Keep The Balance Display????
//			ProcStatus[PROCESS_CR] &= ( ~CARD_READY );

			// Keypad Select Application
			if( KEYPAD_CR_Buf.APPType == 1 )
			{
				// Select Loyalty Application
				retval = CR_ICCARD_Select_Loyalty_APP();
			}
			//Douzhq 20140717 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			else if (KEYPAD_CR_Buf.APPType == 2)
			{
				retval = CR_ICCARD_Select_Saveoil_APP();
			}
#endif
			else
			{
				// Select Sinopec Application
//Douzhq 20140610 Add , IC Card Recognize
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			  if (PowerDown.CardRecoginMode)
			  {
				if (AidType == M_SINOPEC)
				{
					retval = CR_ICCARD_Select_Sinopec_APP();
				}
				else if (AidType == M_SOCIAL)
				{
					retval = CR_ICCARD_Select_Social_APP();	
				}
				else
				{
					//....
					retval = CR_ICCARD_Select_Sinopec_APP();
				}	
			  }
			  else
			  {
				retval = CR_ICCARD_Select_Sinopec_APP();			  	
			  }
				
#else
				retval = CR_ICCARD_Select_Sinopec_APP();
#endif
			}

			// Check Command Return
			if( retval != 1 )
			{
				// Clear Input
				KEYPAD_CR_Buf.APPType = 0;

				// Clear Bit
				ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~SELECT_APP );

				// Inform Keypad Process Received
				ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= SELECT_APP;

				if( retval == 0x9000 )
				{
					// Application Select OK, Read The SFIs Next
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X15 );
				}
				else if(( retval == 0x6A81 ) || ( retval == 0x9303 ))
				{
					// For Write Spring Card Log
					PowerDownFlag = Pd_Flag_29;
                    ErrorFlag = retval;

					// Application Was Blocked, Spring Out Next
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					
					// Display: " Application Was Blocked"
				}
				else if( retval == 0x6A82 )
				{
					// For Write Spring Card Log
					PowerDownFlag = Pd_Flag_30;
                    ErrorFlag = retval;

					// Do Not Support The Application
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					
					// Display: " Application Do Not Supported"
				}
				else
				{
					// For Write Spring Card Log
					PowerDownFlag = Pd_Flag_31;
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
		}
		else if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & SELECT_PAY_MODE )
		{
			// Keypad Select The Pay Mode
			// Clear The Bit
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~SELECT_PAY_MODE );
			
			// Get PayMode
			TRINP.TR_TRANS_UNIT = ((( KEYPAD_CR_Buf.PayMode << 4 ) & 0xF0 ) | TRINP.TR_TRANS_DEBITSOURCE );
			PowerDown.PdTrans.PD_TRANS_UNIT = TRINP.TR_TRANS_UNIT;
			KEYPAD_CR_Buf.PayMode = 0;

			// Inform POS The TRINP.TR_TRANS_UNIT
			POS_CR_Buf.P_UNIT = TRINP.TR_TRANS_UNIT;

			Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
		}
#ifdef MAKE_CARD_MODE2
		// For Sinopec Mode2, Do Not Input Auth Code Here, Input Auth In Card Not Inserted Process
#else
		else if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & AUTH_CODE )
		{
			// Keypad Input Auth Code For Cash Trans
			// Clear The Bit
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~AUTH_CODE );
			
			// Inform Keypad Process Receive Ok
			// Set The Bit
			ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= AUTH_CODE;

			// Inform Cash Process Input Auth Code
			// Set The Bit
			ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_AUTH_CODE;

			//Douzhq 20140923 Add , For Bug
			//CRE Engross OK , Then CR - CRE Break , Clear Flag CASH_AUTH_CODE
#ifdef MAKE_CASH_CR
			ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_CODE_TIMER;
#endif
			
			Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
		}
#endif
		else
		{
			// Maybe Responding For The IC Card Process
			// Do Nothing, Untill IC Card Process
			// Return To Card Ready
			// And Check For Card
			Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
		}
	}
	// Check POS Process Task
	else if(( ProcExchange[PROCESS_POS][PROCESS_CR] & POSflag ) != 0 )
	{
		// POS Has Task
		if( ProcExchange[PROCESS_POS][PROCESS_CR] & PRICE_RECV )
		{
			// Clear Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~PRICE_RECV );

			// Form Price Receiving Record
			Set_Next_Command( CR_PRICE_RECV, CR_PRICE_RECORD );
		}
#ifdef MAKE_CARD_MODE2
		else if( ProcExchange[PROCESS_POS][PROCESS_CR] & HOST_STOP )
		{
			// Clear Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~HOST_STOP );

			// Inform Keypad Process
			ProcExchange[PROCESS_CR][PROCESS_POS] |= HOST_STOP;
			
			// Beep It
			BeepControl( 2400 );
								
			// Display: " Host Stop Pump "
			FillInfoMsg( M_HostCancel, 0, 0 );

			// Host Issue The Pump Stop, Spring Out Card, And....
			Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
		}
		else if( ProcExchange[PROCESS_POS][PROCESS_CR] & AUTH_HOST )
		{
			// Clear Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~AUTH_HOST );

			// Inform Keypad Process
			ProcExchange[PROCESS_CR][PROCESS_POS] |= AUTH_HOST;
			
			// Display: " Card Insert, Can Not Be Host Auth "
			FillInfoMsg( M_CardInNotHostAuth, 0, 0 );

			// Host Issue The Pump Stop, Spring Out Card, And....
			Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
		}
#endif	// MAKE_CARD_MODE2
		else
		{
			// Maybe Responding For The IC Card Process
			// Do Nothing, Untill IC Card Process
			// Return To Card Ready
			// And Check For Card
			Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
		}
	}
	// Check Pump Process Task
	else if(( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMPflag ) != 0 )
	{
		// Check Nozzle On Or Not
		if( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_CALL )
		{
			// Clear Bit, Begin Auth Next
			ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_CALL );

			// Inform Received Pump Call, Untill Auth Data Ready Next, In CRPumpCall.c
//			ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_CALL;
			
			// Pump Call
			//Douzhq 20141104 Add , Safe Mode
			//--------------------------Down---------------------------------------------
			if (PowerDown.SafeMode == 0x01 && TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD)
			{
				//Safe Mode , Display Input Empolyee Password
				Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_WAIT_SAFEMODE_PASSWD );
			}
			else
			{
				// Pump Call, IC Card Check Limited Next
				Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_LIMIT );
			}
			//-------------------------- Up ---------------------------------------------
		}
		// Check Non IC Card Trans
		else if( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_OFFLINE_TRANS )
		{
			// Clear Bit
			ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_OFFLINE_TRANS );
			
			if( IssueOfflineTrans() == 1 )
			{
				// Next Offline Trans
				Set_Next_Command( CR_OFFL_TRANS_PROC, CR_OFFLINE_TRANS );
			}
			else
			{
				Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
			}
		}
		else
		{
			// Not Log On, Return To IC Card Absence Check Card
			Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
		}
	}
	// Check Schedule Process Task
	else if(( ProcExchange[PROCESS_SCHEDULE][PROCESS_CR] & Schflag ) != 0 )
	{
		if( ProcExchange[PROCESS_SCHEDULE][PROCESS_CR] & PRICEUPDATE_LOGOFF )
		{
			// Price Updating Need Log Off
			// Clear Bit
			ProcExchange[PROCESS_SCHEDULE][PROCESS_CR] &= ( ~PRICEUPDATE_LOGOFF );

			if( ProcStatus[PROCESS_CR] & CARD_LOGON )
			{
				// Check Wether The Card Is Same As The Log On Card
				if( memcmp( TRINP.TR_CARD_ASN, PowerDown.LogOnCard.EMPCardASN, sizeof( TRINP.TR_CARD_ASN )) != 0 )
				{
					// Not The Save Card, Clear The Balance
					memset( PowerDown.LogOnCard.EMPCardBalance, 0x00, sizeof( PowerDown.LogOnCard.EMPCardBalance ));
				}
				else
				{
					// Do Nothing, Just Go On....
				}
				
				// Copy The Log On Emp Card	Balance
				memcpy( TRINP.TR_CARD_APP_BALANCE, PowerDown.LogOnCard.EMPCardBalance, sizeof( TRINP.TR_CARD_APP_BALANCE ));
				memcpy( PowerDown.PdTrans.PD_CARD_APP_BALANCE, TRINP.TR_CARD_APP_BALANCE, sizeof( PowerDown.PdTrans.PD_CARD_APP_BALANCE ));
	
				// Do Not Spring Card
				HoldCard = 1;

				// Log On, Begin To Log Off IC Card Next
				Set_Next_Command( CR_LOGOFF_PROC, CR_PUMP_TOTALS );
			}
			else
			{
				// Not Log On, Return To IC Card Absence Check Card
				Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
			}
		}

#ifdef MAKE_CARD_MODE2
		else if( ProcExchange[PROCESS_SCHEDULE][PROCESS_CR] & PRICE_UPDATED )
		{
			// Price Updated
			// Clear Bit
			ProcExchange[PROCESS_SCHEDULE][PROCESS_CR] &= ( ~PRICE_UPDATED );
			
			// Form Price Receiving Record
			Set_Next_Command( CR_PRICE_RECV, CR_PRICE_RECORD );
		}
#endif

		else
		{
			// Return To Card Ready
			// And Check For Card
			Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
		}
	}

#ifdef MAKE_CASH_CR
	// Check Cash Process Task
	else if(( ProcExchange[PROCESS_CASH][PROCESS_CR] & Cashflag ) != 0 )
	{
		// Backlight Open, For 30 Seconds
		OpenLcdLight( 0 );
		
		if( ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_KEY_PRESSED )
		{
			// Cash Apply For Use CR
			// Clear Bit
			ProcExchange[PROCESS_CASH][PROCESS_CR] &= ( ~CASH_KEY_PRESSED );

			if( CheckCashCard() == 0 )
			{
				// Set Use Cash Card
				CRType = CR_TYPE_CASH;
			}

			// Return To IC Card Ready Check Card, Anyway
			Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
		}
		else if( ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_KEY_CANCELLED )
		{
			// Cancel Cash Apply For Use CR
			// Clear Bit
			ProcExchange[PROCESS_CASH][PROCESS_CR] &= ( ~CASH_KEY_CANCELLED );
			
			if (!(ProcExchange[PROCESS_CR][PROCESS_CR] & CASH_CODE_ERR))//Douzhq 20141011 Add , For Bug
			{
				if( CRType == CR_TYPE_CASH )
				{
					//Douzhq 20140916 Add , Voice Add
					if (CashErrorNumber == 3)
					{
						CashErrorNumber = 0;
						ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_VERERR;
					}
					else if (CashErrorNumber == 2)
					{
						CashErrorNumber = 0;
						ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_VEROVER;
					}
					else if (CashErrorNumber == 1)
					{
						CashErrorNumber = 0;
						ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_OILCHOOSE;
					}
					else
					{
						//....
					}
					
					//Douzhq 20140928 Add , For Bug
					/*
					if( CheckCashAmount() == 0 && (ProcExchange[PROCESS_CR][PROCESS_CR] & CASH_CODE_ERR)) 
					{
						// A Cash Trans Reserved, Maybe Just Power On
						// Or Cash Received But Fail To Nozzle On etc....
						// So, Forbidden A Cash Trans Beginning....
						GetCashAmount();
						// Clear the cashtotal for next trans' scan, 20140818
						memset( TRINP.TR_CASH_TOTAL, 0x00, sizeof( TRINP.TR_CASH_TOTAL ));
					}*/
					
					// Set Use IC Card
					CRType = CR_TYPE_CARD;
					
					// Return To IC Card Inserted Check Card, Anyway
					//Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
				}
	
				// Inform Cancelled
				ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_KEY_CANCELLED;
			}
		}
		else if( ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_INSERTED )
		{
			// Cancel Cash Apply For Use CR
			// Clear Bit
			ProcExchange[PROCESS_CASH][PROCESS_CR] &= ( ~CASH_INSERTED );

			memcpy( &Cash_CR_Buf.CashTotal[2], Cash_CR_Buf.CashTotalInserted, sizeof( Cash_CR_Buf.CashTotalInserted ));

			// Save The Cash Trans
			SaveCashTotal();

			// Display It
			UpdateScreen();

			// Return To IC Card Inserted Check Card, Anyway
			Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
		}
		else if( ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_TOTAL )
		{
			// Cancel Cash Apply For Use CR
			// Clear Bit
			ProcExchange[PROCESS_CASH][PROCESS_CR] &= ( ~CASH_TOTAL );
			
			// Save The Cash Trans
			SaveCashTotal();

			// Set Cash Accept Ok
			ProcStatus[PROCESS_CR] |= CARD_CASH_ACCEPTED;
			
			// Display It
			UpdateScreen();
            
            // 20160113 for daqing require
            if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
            {
                ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_CARD;
            }
            
            if( ProcStatus[PROCESS_PUMP] == PUMP_C_CALL )
            {
                ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_CALL;
            }
            else
            {
                ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_CALL );
            }

			// Return To IC Card Inserted Check Card, Anyway
			Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
		}
		else
		{
			// Return To Card Absence
			// And Check For Card
			Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
		}
	}
#endif

	// No Task
	else
	{
		// Return To Card Ready
		// And Check For Card
		Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
	}
}


/*
 ** ICCard_NonExist_External_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ExCRProc.c
 *
 *  PARAMETERS: None.
 *
 *  DESCRIPTION: Check Wether Other Process Need IC Card Process Fullfill Tasks
 *				 And, Perform The Task If Needed, While IC Card Does Not Exist
 *
 *  RETURNS: None.
 *
 *	Note: When IC Card Does Not Exist, Only Check PRICE_RECV And Schedule Price Updating
 *			All The Other External Tasks Should Be Forbiddened By The Other Processes, Either Keypad Or Pump
 *
 *
 *	Note: Task Will Be:
 *			Keypad:
 *			POS: PRICE_RECV
 *			PUMP:
 *			Schedule: PRICEUPDATE_LOGOFF
 *			
 *
 */
void ICCard_NonExist_External_Proc( void )
{
//	unsigned short int retval;
#ifdef MAKE_CARD_MODE2
	unsigned int keypadflag = 0;
#else
//	unsigned int keypadflag = 0;
#endif	// MAKE_CARD_MODE2

	unsigned int POSflag = 0;
	unsigned int PUMPflag = 0;
	unsigned int Schflag = 0;
#ifdef MAKE_CASH_CR
	unsigned int Cashflag = 0;
#endif

	// Get flags
#ifdef MAKE_CARD_MODE2
	keypadflag = AUTH_CODE;
#else
//	keypadflag = 0;
#endif	// MAKE_CARD_MODE2

#ifdef MAKE_CARD_MODE2
	POSflag = PRICE_RECV | AUTH_HOST | HOST_STOP;
#else
	POSflag = PRICE_RECV;
#endif	// MAKE_CARD_MODE2

	PUMPflag = PUMP_OFFLINE_TRANS;

#ifdef MAKE_CARD_MODE2
	Schflag = PRICEUPDATE_LOGOFF | PRICE_UPDATED;
#else
	Schflag = PRICEUPDATE_LOGOFF;
#endif

#ifdef MAKE_CASH_CR
	Cashflag = CASH_KEY_PRESSED | CASH_KEY_CANCELLED;
#endif


	// Check POS Process Task
	if(( ProcExchange[PROCESS_POS][PROCESS_CR] & POSflag ) != 0 )
	{
		// POS Has Task
		if( ProcExchange[PROCESS_POS][PROCESS_CR] & PRICE_RECV )
		{
			// Clear Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~PRICE_RECV );

//			// Inform POS Process
//			ProcExchange[PROCESS_CR][PROCESS_POS] |= PRICE_RECV;
			
			// Form Price Receiving Record
			Set_Next_Command( CR_PRICE_RECV, CR_PRICE_RECORD );
		}
#ifdef MAKE_CARD_MODE2
		else if( ProcExchange[PROCESS_POS][PROCESS_CR] & AUTH_HOST )
		{
			// Clear Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~AUTH_HOST );

			// Inform POS Process
			ProcExchange[PROCESS_CR][PROCESS_POS] |= AUTH_HOST;
			
			// Clear The Trans Buffers
			memset( TRINP.TR_CARD_ATR, 0x00, ( &TRINP.TR_RFU - TRINP.TR_CARD_ATR ));

			// Clear The Power Down Trans Saved
//			memset( PowerDown.PdTrans.PD_TRANS_TTC, 0x00, sizeof( PD_TRANS ) - 2 );
			memset( PowerDown.PdTrans.PD_TRANS_TTC, 0x00, ( &PowerDown.PdTrans.PD_CLEAR_FLAG - PowerDown.PdTrans.PD_TRANS_TTC ));

			// Save The Host Auth Data
			SaveHostAuth();

			// Backlight Open, For 30 Seconds
			OpenLcdLight( 0 );

			// For Display EOT Screen, Clear It
			if( KeepBalScreen == 1 )
			{
				// Display: " OK"
				FillInfoMsg( M_DefaultInfo, 0, 0 );
					
				// Delete The Timer
				DeleteTimer( BalScreenKeepTimer );
				KeepBalScreen = 0;
			}

			// Mark Auth, Do Not Change Auth Method For Sinopec Mode2
			ProcStatus[PROCESS_CR] |= CARD_AUTHED;

			// Return To Check Card
			// Check Card Ready Next
			Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
		}
		else if( ProcExchange[PROCESS_POS][PROCESS_CR] & HOST_STOP )
		{
			// Clear Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~HOST_STOP );

			// Inform Keypad Process
			ProcExchange[PROCESS_CR][PROCESS_POS] |= HOST_STOP;
			
			// And Check For Card
			Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
		}
			
#endif
		else
		{
			// Return To Card Ready
			// And Check For Card
			Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
		}
	}
	// Check Pump Process Task
	else if(( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMPflag ) != 0 )
	{
		// Check Non IC Card Trans
		if( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_OFFLINE_TRANS )
		{
			// Clear Bit
			ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_OFFLINE_TRANS );
			
			if( IssueOfflineTrans() == 1 )
			{
				// Next Offline Trans
				Set_Next_Command( CR_OFFL_TRANS_PROC, CR_OFFLINE_TRANS );
			}
			else
			{
				Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
			}
		}
		else
		{
			Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
		}
	}
#ifdef MAKE_CARD_MODE2
	else if(( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & keypadflag ) != 0 )
	{
		if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & AUTH_CODE )
		{
			// Keypad Input Auth Code For Cash Trans
			// Clear The Bit
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~AUTH_CODE );
			
			// Inform Keypad Process Receive Ok
			// Set The Bit
			ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= AUTH_CODE;

			// Clear The Trans Buffers
			memset( TRINP.TR_CARD_ATR, 0x00, ( &TRINP.TR_RFU - TRINP.TR_CARD_ATR ));

			// Clear The Power Down Trans Saved
//			memset( PowerDown.PdTrans.PD_TRANS_TTC, 0x00, sizeof( PD_TRANS ) - 2 );
			memset( PowerDown.PdTrans.PD_TRANS_TTC, 0x00, ( &PowerDown.PdTrans.PD_CLEAR_FLAG - PowerDown.PdTrans.PD_TRANS_TTC ));

			// Save The NonCard Auth Data
			SaveNonCardAuth_Mode2();

//			// Check Card Ready Next
//			Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );

			// Mark Auth, Do Not Change Auth Method For Sinopec Mode2
			ProcStatus[PROCESS_CR] |= CARD_AUTHED;

			// Send Auth To POS Next
			Set_Next_Command( CR_ICCARD_READY_PROC, CR_WAITING_MAC );
		}
		else
		{
			Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
		}
	}
#endif

	// Check Schedule Process Task
	else if(( ProcExchange[PROCESS_SCHEDULE][PROCESS_CR] & Schflag ) != 0 )
	{
		if( ProcExchange[PROCESS_SCHEDULE][PROCESS_CR] & PRICEUPDATE_LOGOFF )
		{
			// Price Updating Need Log Off
			// Clear Bit
			ProcExchange[PROCESS_SCHEDULE][PROCESS_CR] &= ( ~PRICEUPDATE_LOGOFF );

			if( ProcStatus[PROCESS_CR] & CARD_LOGON )
			{
				// For IC Card Not Inserted, Fill 00000000 In Balance For The Record
				memset( PowerDown.LogOnCard.EMPCardBalance, 0x00, sizeof( PowerDown.LogOnCard.EMPCardBalance ));

				// Copy The Log On Emp Card	Balance
				memcpy( TRINP.TR_CARD_APP_BALANCE, PowerDown.LogOnCard.EMPCardBalance, sizeof( TRINP.TR_CARD_APP_BALANCE ));
				memcpy( PowerDown.PdTrans.PD_CARD_APP_BALANCE, TRINP.TR_CARD_APP_BALANCE, sizeof( PowerDown.PdTrans.PD_CARD_APP_BALANCE ));

				// Log On, Begin To Log Off IC Card Next
				Set_Next_Command( CR_LOGOFF_PROC, CR_PUMP_TOTALS );
			}
			else
			{
				// Not Log On, Return To IC Card Absence Check Card
				Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
			}
		}

#ifdef MAKE_CARD_MODE2
		else if( ProcExchange[PROCESS_SCHEDULE][PROCESS_CR] & PRICE_UPDATED )
		{
			// Price Updated
			// Clear Bit
			ProcExchange[PROCESS_SCHEDULE][PROCESS_CR] &= ( ~PRICE_UPDATED );
			
			// Form Price Receiving Record
			Set_Next_Command( CR_PRICE_RECV, CR_PRICE_RECORD );
		}
#endif

		else
		{
			// Return To Card Ready
			// And Check For Card
			Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
		}
	}

#ifdef MAKE_CASH_CR
	// Check Cash Process Task
	else if(( ProcExchange[PROCESS_CASH][PROCESS_CR] & Cashflag ) != 0 )
	{
		// Backlight Open, For 30 Seconds
		OpenLcdLight( 0 );
		
		if( ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_KEY_PRESSED )
		{
			// Cash Apply For Use CR
			// Clear Bit
			ProcExchange[PROCESS_CASH][PROCESS_CR] &= ( ~CASH_KEY_PRESSED );

			if( CheckCashCard() == 0 )
			{
				// Set Use Cash Card
				CRType = CR_TYPE_CASH;
			}

			// Return To IC Card Absence Check Card, Anyway
			Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
		}
		else if( ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_KEY_CANCELLED )
		{
			// Cancel Cash Apply For Use CR
			// Clear Bit
			ProcExchange[PROCESS_CASH][PROCESS_CR] &= ( ~CASH_KEY_CANCELLED );

			if( CRType == CR_TYPE_CASH )
			{
				// Set Use IC Card
				CRType = CR_TYPE_CARD;
			}

			// Inform Cancelled
			ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_KEY_CANCELLED;

			// Return To IC Card Absence Check Card, Anyway
			//Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
            Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
		}
		else
		{
			// Return To Card Absence
			// And Check For Card
			Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
		}
	}
#endif

	// No Task
	else
	{
		// Return To IC Card Absence
		// And Check For Card
		Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
	}
}


/*
 ** ICCard_Busy_External_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ExCRProc.c
 *
 *  PARAMETERS: None.
 *
 *  DESCRIPTION: Check Wether Other Process Need IC Card Process Fullfill Tasks
 *				 And, Perform The Task, If Needed, While IC Card IS Busy In Fuelling
 *
 *  RETURNS: None.
 *
 *	Note: When IC Card Is Busy In Fuelling, Only Check PRICE_RECV, Rounding, PumpEOT
 *			All The Other External Tasks Should Be Forbiddened By The Other Processes, Either Keypad Or Pump Or POS
 *
 *
 *	Note: Task Will Be:
 *			Keypad:	ROUNDING
 *			POS: PRICE_RECV
 *			PUMP: PUMP_EOT
 *			Schedule:
 *			
 *
 */
void ICCard_Busy_External_Proc( void )
{
//	unsigned short int retval;
	unsigned int keypadflag = 0;
	unsigned int POSflag = 0;
	unsigned int PUMPflag = 0;
//	unsigned int Schflag = 0;

	// Get flags
	keypadflag = 0;
#ifdef MAKE_CARD_MODE2
	POSflag = HOST_STOP;
#else
//	POSflag = PRICE_RECV;
#endif	// MAKE_CARD_MODE2
	PUMPflag = PUMP_EOT | PUMP_ZERO;
//	Schflag = 0;

//Douzhq 20141022 Add , For Bug
//---------------------------Down----------------------------------------------------
#ifdef MAKE_CASH_CR
	if( CRType == CR_TYPE_CASH )
	{
		if (ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_KEY_PRESSED)
		{
			ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_KEY_PRESSED);
			
			Cash_CR_Buf.CashCard_Flag = 0x01;
			
			ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_KEY_PRESSED;
		}
	}
#endif
//--------------------------- Up ----------------------------------------------------
	// Check KeyPad Process Task
	if(( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & keypadflag ) != 0 )
	{
//		if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & ROUNDING )
//		{
//			// Clear Bit
//			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~ROUNDING );
//
//			// Inform Keypad Process
//			ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= ROUNDING;
//
//			// Send Rounding To Pump
//			ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_AUTH;
//			Pump_CR_Buf.PresetMode = KEYPAD_CR_Buf.PresetMode;
//			KEYPAD_CR_Buf.PresetMode = 0;
//		}
//		else
//		{
//			// Return To Card Ready
//			// And Check For Card
//			Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_CARDREADER_CHECK_CARD );
//		}
	}
#ifdef MAKE_CARD_MODE2
	// Check POS Process Task
	else if(( ProcExchange[PROCESS_POS][PROCESS_CR] & POSflag ) != 0 )
	{
		// POS Has Task
		if( ProcExchange[PROCESS_POS][PROCESS_CR] & HOST_STOP )
		{
			// Clear Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~HOST_STOP );

			// Inform Keypad Process
			ProcExchange[PROCESS_CR][PROCESS_POS] |= HOST_STOP;
			
			// Beep It
			BeepControl( 2400 );
								
			// Display: " Host Stop Pump "
			FillInfoMsg( M_HostStop, 0, 0 );

			// Issue Pump Stop			
			// Host Issue The Pump Stop, Inform Pump Stop, And....
			ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_STOP;
		}
		else
		{
			// Return To Card Ready
			// And Check For Card
			Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_CARDREADER_CHECK_CARD );
		}
	}
#else
	// Check POS Process Task
	else if(( ProcExchange[PROCESS_POS][PROCESS_CR] & POSflag ) != 0 )
	{
		// POS Has Task
		if( ProcExchange[PROCESS_POS][PROCESS_CR] & PRICE_RECV )
		{
			// Clear Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~PRICE_RECV );

			// Inform Keypad Process
			ProcExchange[PROCESS_CR][PROCESS_POS] |= PRICE_RECV;
			
			
			// Form Price Receiving Record
		}
		else
		{
			// Return To Card Ready
			// And Check For Card
			Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_CARDREADER_CHECK_CARD );
		}
	}
#endif	// MAKE_CARD_MODE2

	// Check Pump Process Task
	else if(( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMPflag ) != 0 )
	{
		if( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_ZERO )
		{
			// Clear Bit In Get Last Trans, Begin Get Zero Last Trans Next
			ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_ZERO );

			// Pump EOT, IC Card Load Last Trans,
			WriteCRPumpEOTLog();
			// The CR EOT Process Should Start After Getting The Last Trans
			Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_GET_LAST_TRANS_ZERO );
		}
		else if( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_EOT )
		{
			// Clear Bit In Get Last Trans, Begin EOT Next
			
//			ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_BUSY );
//			ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_EOT );

			// Pump EOT, IC Card Load Last Trans,
			WriteCRPumpEOTLog();

			// The CR EOT Process Should Start After Getting The Last Trans
			Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_GET_LAST_TRANS );
		}
		else
		{
			// Return To Card Busy
			// And Check For Card
			Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_CARDREADER_CHECK_CARD );
		}
	}
	// No Task
	else
	{
		// Return To Card Ready
		// And Check For Card
		Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_CARDREADER_CHECK_CARD );
	}
}



/*
 ** IssueOfflineTrans
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ExCRProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Issue Offline Trans Or Not
 *				 Because The PUMP Process Will Inform Offline Trans Every Power On Without Any Analysis
 *				 So, This Step Determine Wether The Offline Trans Will Be Issue Really
 *
 *  RETURNS: 0: Do Not Issue The Offline Trans, The PUMP Process Only A Fraud
 *			 1: Issue The Offline Trans
 *
 */
unsigned char IssueOfflineTrans( void )
{
	unsigned char retval;
	unsigned int i;
	unsigned int value;

	// Default: Do Not Issue
	retval = 0;

	for( i = 0; i < 4; i++ )
	{
		// Check Offline Volume, Must Not Be 0
		memcpy( &value, &Pump_CR_Buf.OfflineVolume[4*i], 4 );
		if( value != 0 )
		{
			// Need Issue The Offline Trans
			retval = 1;

			break;
		}
		else
		{
			// Do Nothing, Just Go On....
		}

//		// Check Offline Amount
//		memcpy( &value, &Pump_CR_Buf.OfflineAmount[4*i], 4 );
//		if( value != 0 )
//		{
//			// Need Issue The Offline Trans
//			retval = 1;
//
//			break;
//		}
//		else
//		{
//			// Do Nothing, Just Go On....
//		}
	}

	return retval;
}




#ifdef MAKE_CARD_MODE2

/*
 ** SaveHostAuth
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ExCRProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save The Host Auth Data For Sinopec Mode2
 *
 *  RETURNS: None.
 *
 */
void SaveHostAuth( void )
{
	unsigned char nozzle;
	unsigned short int val;

	// Save Nozzle Number
	TRINP.TR_TRANS_NOZZLE_NUMBER = POS_CR_Buf.TransNozzleNumber;
	PowerDown.PdTrans.PD_NOZZLENUMBER = TRINP.TR_TRANS_NOZZLE_NUMBER;

	// Get Phiscal Nozzle Number
	nozzle = GetPhysicsNzn( TRINP.TR_TRANS_NOZZLE_NUMBER );
	
	// Get Price
	val = GetPrice( nozzle, 0 );
	ShortIntToCharCopy( val, TRINP.TR_TRANS_PRICE );
	memcpy( PowerDown.PdTrans.PD_TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( PowerDown.PdTrans.PD_TRANS_PRICE ));

	// Get Trans Product Grade Code
	val = GetO_Type( nozzle );
	ShortIntToCharCopy( val, TRINP.TR_TRANS_GRADE_CODE );
	memcpy( PowerDown.PdTrans.PD_GRADE_CODE, TRINP.TR_TRANS_GRADE_CODE, sizeof( PowerDown.PdTrans.PD_GRADE_CODE ));

//	// Save Employee Number
//	TRINP.TR_TRANS_EMP_NUMBER = PowerDown.LogOnCard.EMPNumber;
//	// Transaction Employee Number	
//	PowerDown.PdTrans.PD_EMPLOYEE_NUMBER = TRINP.TR_TRANS_EMP_NUMBER;
	
	// Save TTC
	GetTransTTC();
	memcpy( PowerDown.PdTrans.PD_TRANS_TTC, TRINP.TR_TRANS_TTC, sizeof( PowerDown.PdTrans.PD_TRANS_TTC ));

	// Save Data And Time
	memcpy( TRINP.TR_TRANS_DATE, POS_CR_Buf.Auth_Date, sizeof( TRINP.TR_TRANS_DATE ));
	memcpy( TRINP.TR_TRANS_TIME, POS_CR_Buf.Auth_Time, sizeof( TRINP.TR_TRANS_TIME ));
	memcpy( PowerDown.PdTrans.PD_TRANS_DATE, TRINP.TR_TRANS_DATE, sizeof( PowerDown.PdTrans.PD_TRANS_DATE ));
	memcpy( PowerDown.PdTrans.PD_TRANS_TIME, TRINP.TR_TRANS_TIME, sizeof( PowerDown.PdTrans.PD_TRANS_TIME ));

	// Save PayX
	TRINP.TR_PAY_X = POS_CR_Buf.Auth_Pay_X;
	PowerDown.PdTrans.PD_PAY_X = TRINP.TR_PAY_X;

	// Save Card ASN
	memset( TRINP.TR_CARD_ASN, 0x00, sizeof( TRINP.TR_CARD_ASN ));
	memcpy( &TRINP.TR_CARD_ASN[8], &POS_CR_Buf.ICCARD_ASN[0], 2 );
	memcpy( PowerDown.PdTrans.PD_CARD_ASN, TRINP.TR_CARD_ASN, sizeof( PowerDown.PdTrans.PD_CARD_ASN ));
	memcpy( POS_CR_Buf.ICCARD_ASN, TRINP.TR_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));


	// Save Amount
	memcpy( TRINP.TR_TRANS_AMOUNT, POS_CR_Buf.TransAmount, sizeof( TRINP.TR_TRANS_AMOUNT ));
	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT, TRINP.TR_TRANS_AMOUNT, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT ));
	
	TRINP.TR_TRANS_PRESET_MODE = 0;
	memcpy( TRINP.TR_TRANS_PRESET_VALUE, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));

	// Work Mode And Auth Mode
	TRINP.TR_WORK_MODE = GetWorkMode();
	TRINP.TR_AUTH_MODE = GetAuthMode();
	PowerDown.PdTrans.PD_WORK_MODE = TRINP.TR_WORK_MODE;
	PowerDown.PdTrans.PD_AUTH_MODE = TRINP.TR_AUTH_MODE;


	// Save PD_TRANS_FLAG
	// Update The Power Down Protection Message
	PowerDown.PdTrans.PD_TRANS_FLAG = AUTH_RESP;

	// Get Pump Totals, For Auth Cancel Trans
	memcpy( TRINP.TR_TRANS_VOLUME_TOTAL, DISP_CR_Buf.DispTotal[nozzle].VolumeTotal, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));
	memcpy( PowerDown.PdTrans.PD_TRANS_VOLUME_TOTAL, TRINP.TR_TRANS_VOLUME_TOTAL, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));

	// Mark Card Ready
	ProcStatus[PROCESS_CR] |= ( CARD_READY | CARD_INSERTED );
}

/*
 ** SaveNonCardAuth_Mode2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ExCRProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save Non IC Card Auth Trans Data, eg, Barcode etc....
 *
 *  RETURNS: None.
 *
 */
void SaveNonCardAuth_Mode2( void )
{
	unsigned char buf[12];

	// Save PayX
	TRINP.TR_PAY_X = 0x18;
	PowerDown.PdTrans.PD_PAY_X = TRINP.TR_PAY_X;

	// For Non Card Trans, Remark The TR_TRANS_DEBITSOURCE
	TRINP.TR_TRANS_DEBITSOURCE = 0x09;
	PowerDown.PdTrans.PD_TRANS_DEBITSOURCE = TRINP.TR_TRANS_DEBITSOURCE;

	// Save Card ASN
	memset( buf, '0', sizeof( buf ));

	// Get Auth Code
	TRINP.TR_AUTH_CODE_LEN = Cash_CR_Buf.CashAuthCode[0];
	if( TRINP.TR_AUTH_CODE_LEN > 12 )
	{
		TRINP.TR_AUTH_CODE_LEN = 12;
	}

	memcpy( TRINP.TR_AUTH_CODE, &Cash_CR_Buf.CashAuthCode[1], TRINP.TR_AUTH_CODE_LEN );

	memcpy( &buf[12-TRINP.TR_AUTH_CODE_LEN], TRINP.TR_AUTH_CODE, TRINP.TR_AUTH_CODE_LEN );

	memset( TRINP.TR_CARD_ASN, 0x00, sizeof( TRINP.TR_CARD_ASN ));
	Asc2Hex( &TRINP.TR_CARD_ASN[4], buf, 6 );
	memcpy( PowerDown.PdTrans.PD_CARD_ASN, TRINP.TR_CARD_ASN, sizeof( PowerDown.PdTrans.PD_CARD_ASN ));

	// Save Amount
	memset( TRINP.TR_TRANS_AMOUNT, 0x00, sizeof( TRINP.TR_TRANS_AMOUNT ));
	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT, TRINP.TR_TRANS_AMOUNT, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT ));
	
	TRINP.TR_TRANS_PRESET_MODE = 0;
	memcpy( TRINP.TR_TRANS_PRESET_VALUE, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));

	// Work Mode And Auth Mode
	TRINP.TR_WORK_MODE = GetWorkMode();
	TRINP.TR_AUTH_MODE = GetAuthMode();
	PowerDown.PdTrans.PD_WORK_MODE = TRINP.TR_WORK_MODE;
	PowerDown.PdTrans.PD_AUTH_MODE = TRINP.TR_AUTH_MODE;

	// Get Pump Totals, For Auth Cancel Trans
	memcpy( TRINP.TR_TRANS_VOLUME_TOTAL, DISP_CR_Buf.DispTotal[0].VolumeTotal, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));
	memcpy( PowerDown.PdTrans.PD_TRANS_VOLUME_TOTAL, TRINP.TR_TRANS_VOLUME_TOTAL, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));

	// Mark Card Ready
	ProcStatus[PROCESS_CR] |= ( CARD_READY | CARD_INSERTED );
}




#endif
