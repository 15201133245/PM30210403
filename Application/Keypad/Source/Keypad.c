#include "global.h"

#include "define.h"

#include "CRVaribles.h"
#include "CRMemcopy.h"
#include "Keypad.h"
#include "KeyMask.h"
#include "Screen.h"
#include "Display.h"
#include "PdProtect.h"
#include "CRPrint.h"
#include "WorkMode.h"

//Douzhq 20140603 Add , Lock Nozzle
#include "SocialCard.h"
#include "WriteLog.h"
#include "WriteLog.h"


// Define The Display Trans
JOURNAL KeyDispTrans;

// Define The Format ID
// 0: Format Config
// 1: Format Record
unsigned char FormatId = 0; 

const KEYPADFUNC KeypadTable[] =
{
	InputStart,							// Just For Enum Start

	InputCard,							// Check Input While Default Screen, Inserted IC Card Screen
	InputCardBusy,						// Check Input While Defalut Screen, IC Card Busy Screen
	InputCardBalance,					// Check Input While Default Screen, IC Card Ready, Display Balance Screen
										// 3 Default Screens Is According To The ProcStatus[PROCESS_CR]
	InputCardWaiting,					// Check Input While Display Waiting Screen

	InputCardError,						// Check Input While Default Screen Card Reader Error,
										// Include: Card Reader Reset Error And PSAM Error

	InputPriceUpdate,					// Check Input While Display Price Update Screen

	InputSelectPayMode,					// Check Input While Display Select Pay Mode
	InputSelectApp,						// Check Input While Display Select Application
	
//Douzhq 20140523 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	InputSelectAppSaveoil,				// Check Input While Display Select Application ,when Add Saveoil App
#endif

	InputPin,							// Check Input While Display Input Pin
	InputEMPPWD,						// Check Input While Display Input Employee Password
	InputENTERKey,						// Check Input While Display Input ENTER Key
	InputMaintainPWD,					// Check Input While Display Input Maintainance Password
	InputFunctionPWD,					// Check Input While Display Display Input Function Password

//Douzhq 20140523 Add , Lock Nozzle , Unlock Function
	InputUnlockPWD,						// Check Input While Display Display Input Unlock Password 

	InputFormatPWD,						// Check Input While Display Display Input Format Config And Data Password
	
	InputAuthCode,						// Check Input While Display Input Auth Code For Cash Trans

	InputFunction,						// Check Input While Display Function Screen
	InputVersion,						// Check Input While Display Version Screen
	InputInquire,						// Check Input While Display Inquire Screen
	InputStation,						// Check Input While Display Station Information
	InputRecordSpace,					// Check Input While Display Record Space Information
	InputVolumeTotal,					// Check Input While Display Volume Total
	InputVolumeTotalLogOn,				// Check Input While Display Volume Total By This Log On Employee
	InputAmountTotal,					// Check Input While Display Amount Total
	InputAmountTotalLogOn,				// Check Input While Display Amount Total By This Log On Employee

#ifdef MAKE_CASH_CR
	InputCashVolumeTotalLogOn,			// Check Input While Display Cash Volume Total By This Log On Employee
	InputCashAmountTotalLogOn,			// Check Input While Display Cash Amount Total By This Log On Employee
	InputSelectPrint,					// Check Input While Display Cash Trans Print Receipt Or Not
#endif

//Douzhq 20140922 Add , Config Printer Set
	InputChoosePrint,

	InputPriceInquiry,					// Check Input While Display Price Inquiry

	InputTTC,							// Check Input While Display Input TTC Screen


	InputSetupFunction,					// Check Input While Display Setup Function Screen
	InputBacklighttime,					// Check Input While Display Setup Backlight
	InputPrint,							// Check Input While Display Setup Print
	InputVolume,						// Check Input While Display Setup Volume
	InputPSAM,							// Check Input While Display Setup PSAM Validate And Expire Date Check

	InputCustomerPrint,					// Check Input While Display Customer Card Print Setup
	InputEMPPrint,						// Check Input While Display Employee Card Print Setup
	InputTestPrint,						// Check Input While Display Test Card Print Setup
	InputMatnPrint,						// Check Input While Display Maintainance Card Print Setup
	InputPubTransPrint,					// Check Input While Display Trans Without ASN Print Setup
	
	//Douzhq 20140919 Add , Config Printer Set
	InputExpandSetPrint,				// Check Input While Display Expand Set Print Screen
	InputPrinterSwitchSet,				// Check Input While Display Printer Set Print Screen
	InputInquirySwitchSet,				// Check Input While Display Inquiry Set Print Screen
	InputReceiptSwitchSet,				// Check Input While Display Receipt Set Print Screen
	
	InputMatnFunction,					// Check Input While Display Maintainance Screen
	InputUnfinishedTrans,				// Check Input While Display Unfinished Trans Screen
	InputUpload,						// Check Input While Display Upload Management Screen
	InputUploadReady,					// Check Input While Display Upload Management Ready Screen
	InputFormat,						// Check Input While Display Format Flash Screen
	InputPOSP,							// Check Input While Display Setup POSP Screen
	InputLocalPrinter,					// Check Input While Display Setup Local Printer Screen
	InputSoftwareUpgrade,				// Check Input While Display Software Upgrade Screen


	InputFormatConfig,					// Check Input While Display Format Config Flash Screen
	InputFormatRecord,					// Check Input While Display Format Record Flash Screen
	InputDebitPowerDown,				// Check Input While Display The Information For Power Down Debit Or Power On
	InputBlackListSetup,				// Check Input While Display The Information For Check Black List First On POS Or Local

	InputDispTrans,						// Check Input While Display The Trans Record
	
#ifdef MAKE_PSAM_VERIFY

	InputPSAM_V_UnSupport,				// Check Input While Display PSAM Unsupport Verify Function
	InputPSAM_V_UnRegistered,			// Check Input While Display PSAM UnRegistered Pump
	InputPSAM_V_Locked,					// Check Input While Display PSAM Locked For Certificate Pump
	InputPSAM_V_Insert_RID,				// Check Input While Display Insert RID Card Please
	InputPSAM_V_Register_Ok,			// Check Input Display PSAM Register OK Screen

#endif

#ifdef MAKE_CARD_MODE2
	InputWorkModeSet,					// Check Input While Display Work Mode Set Screen
#endif

#ifndef MAKE_CARD_MODE2
    InputWorkManage,                    // Check Input While Display Work Manage Set Screen
#endif

	//Douzhq 20140610 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	InputManageSocial,					// Check Input While Display Manage Social Set Screen
#endif

	InputLockNozzleSet,					// Check Input While Display Lock Nozzle Set Screen

//Douzhq 20141010 Add , Safe Mode
//-------  Begin , Douzhq Add , Safe Mode , 20141010 -------
	InputSafeModeSet,					// Check Input While Dispaly Safe Mode Set Screen
//-------  End   , Douzhq Add , Safe Mode , 20141010 -------

//Douzhq 20140610 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	//IC Card Recognize
	InputCardRecoginze,					// Check Input While Display Ic Card Recoginze Set Screen
	InputICCardSwitch,					// Check Input While Display Ic Card Recognize Switch Set Screen
	InputCardAppSwitch,					// Check Input While Display Ic Card Applaction Sellect Set Screen
	
	// Save Oil
	InputSaveOilAppSet,					// Check Input While Display Save Oil Applaction Set Screen
	
	// Vip Fueling 
	InputVipFuelingSet,					// Check Input While Display Vip Fueling Set Screen
#endif

//Douzhq 20140819 Add , Menu About
//Manage Menu , PASM Verify and No CASH
#if defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	InputManageSocial1,					// Check Input While Display Manage Set Screen
#endif

//Douzhq 20140819 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
	InputManageCarNum,
	InputCarNumberRec,
#endif

    InputEMPNumber,                     // Check Input While Display Input Emp Number Screen
	
    InputOverFueling,
    InputOverFuelingPassword,

	InputEnd,							// Just For Enum END
};


/*
 ** InputStart
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Just For Enum Start
 *
 *  RETURNS: None.
 *
 */
void InputStart( void )
{
}

/*
 ** InputCard
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Default Screen, Inserted IC Card Screen
 *
 *  RETURNS: None.
 *
 */
void InputCard( void )
{
	switch( KeyValue )
	{
#ifdef MAKE_CARD_MODE2
		case '1':

			if( GetWorkMode() == 0x01 )
			{
				// Set Employee Card Payment In Office
				TransAmountPay = 0x01;

				//
				UpdateScreen();
			}
			else
			{
				// Work Offline, Do Not Care, Go On....
			}

		break;

		case AMOUNT:

			if( GetAuthMode() == 0x01 )
			{
				if( ProcStatus[PROCESS_CR] & CARD_LOGON )
				{
					// Input Auth Code Next
					InputLen = 0;

					memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

					memset( InputBuf, 0x00, sizeof( InputBuf ));

					DispListInversedIdx = 0;

					// Non Card Cash Function
					NextScreenID = SCR_INPUT_AUTHCODE;
				}
				else
				{
					// Display " Off Duty "
					FillInfoMsg( M_OffDutyNotPreset, 0, 0 );
				}
			}
			else
			{
				// Display " Must Be Host Auth Mode"
				FillInfoMsg( M_MustBeHostAuth, 0, 0 );
			}

		break;

#endif	// MAKE_CARD_MODE2


		case SETUP:

			// Setup Screen Next
			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;
			
			NextScreenID = SCR_INPUT_FUNCTION_PWD;
		break;

		case SPRING:

			// For Sping Out Key Pressed, Spring Out Card AnyWay Except IC Card Busy
			CardOut();

		break;

//		case SETUP:
//
//		break;
//
		default:

		break;
	}
}

/*
 ** InputCardBusy
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Defalut Screen, IC Card Busy Screen
 *
 *  RETURNS: None.
 *
 */
void InputCardBusy( void )
{
	switch( KeyValue )
	{
		case VOLUME:

			if(( RoundOn == 0 ) || ( RoundOn == 2 ))
			{
				// Volume Round
				// Need To Judge Pump Busy????
				// Inform Pump Volume Round, And....
				ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP] |= KEYPAD_PUMP_VOL_ROUND;

				if( RoundOn == 0 )
				{
					// Display Volume Rounding
					DispRounding( 1 );
				}
				else
				{
					// Volume Already Display, Do Not Display Again
				}
			}
			else
			{
				// Do Not Allow Volume Rounding
			}

		break;

		case '.':

			if(( RoundOn == 0 ) || ( RoundOn == 1 ))
			{
				// Amount Round
				// Need To Judge Pump Busy????
				// Inform Pump Amount Round, And....
				ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP] |= KEYPAD_PUMP_ROUND;

				if( RoundOn == 0 )
				{
					// Display Cash Rounding
					DispRounding( 0 );
				}
				else
				{
					// Amount Already Display, Do Not Display Again
				}
			}
			else
			{
				// Do Not Allow Amount Rounding
			}

		break;
//Douzhq 20140603 Add , Lock Nozzle , Add Lock 
	case SELECT:
	  if (TRINP.TR_CARD_ASN[2] != EMPLOYEE_CARD)
	  {
	  	break;
	  }
	  if (LOCK_NOZZLE.LockFlag != 1 && PowerDown.LockNozzleMode == 0x01)
	  {
		  LOCK_NOZZLE.LockFlag = 1;
		  FillInfoMsg( M_LockedNozzle, 0, 0 );
		  WriteLog(PROCESS_KEYPAD,"Lock Nozzle");
	  }
	  break;
	  
	default:
		break;
	}
}

/*
 ** InputCardBalance
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Default Screen, IC Card Ready, Display Balance Screen
 *
 *  RETURNS: None.
 *
 */
void InputCardBalance( void )
{
#ifdef MAKE_CASH_CR
	if( CRType == CR_TYPE_CASH )
	{
		// Cash Trans
		if( Cash_CR_Buf.CashType == 0x01 )
		{
			// Display Cash Auth POS Trans Balance Screen
			InputCardBalance_Auth();
		}
		else
		{
			// Display Cash Acceptor Trans Balance Screen
			InputCardBalance_Acceptor();
		}
	}
	else
#endif

	{
		InputCardBalance_Card();
	}

}


/*
 ** InputCardBalance_Card
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input For Balance Screen And Inserted IC Card.
 *
 *  RETURNS: None.
 *
 */
void InputCardBalance_Card( void )
{
	switch( KeyValue )
	{
		case SETUP:

			if( InputLen == 0 )
			{
				// Setup Screen Next
				if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
				{
					// Clear Input Buffer
					memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

					memset( InputBuf, 0x00, sizeof( InputBuf ));
					InputLen = 0;

					// Customer Card, Input Function Password
					
					NextScreenID = SCR_INPUT_FUNCTION_PWD;
				}
				else
				{
					// Employee Card, SetUp Function.
					NextScreenID = SCR_FUNCTION;
				}
			}
			else
			{
				// In Preset Display, Do Not Enter Setup
			}

		break;

		case SPRING:

			if( InputLen == 0 )
			{

//Douzhq 20140605 Add , Lock Nozzle , Spring IC Card
			  	if (LOCK_NOZZLE.LockFlag == 1 && TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD)
				{
				  	NextScreenID = SCR_INPUT_UNLOCK_PWD;// Input Password Unlock Window
				  	break;
				}
				else
				{
					ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SPRING_CARD;  
					DispIdle();	
				}
//Before Add Lock Nozzle Feature
//				// Spring Out IC Card
//				ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SPRING_CARD;  
//				DispIdle();

			}
			else
			{
				// In Preset Display, Do Not Enter Spring Card
			}

		break;

		case SELECT:

			if( InputLen == 0 )
			{
				if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
				{
					// Check Wether Support 2 Application
					if(( ProcStatus[PROCESS_CR] & CARD_SINOPEC_APP ) && ( ProcStatus[PROCESS_CR] & CARD_LOYALTY_APP ))
					{
						// For Customer Card And Support 2 Applications, Select Application
					  
//Douzhq 20140529 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
					  	if ( (ProcStatus[PROCESS_CR] & CARD_SAVEOIL_APP) && PowerDown.SaveOilAppMode == 0x01)
						{
							// For Customer Card And Support 3 Applications, Select Application
							NextScreenID = SCR_SELECT_APP_SAVEOIL;
						}
						else
						{
							NextScreenID = SCR_SELECT_APP;
						}
#else
						NextScreenID = SCR_SELECT_APP;
#endif

						DispListInversedIdx = 0;
					}
					else
					{
						// Display:" Not Local Card, Do Not Support Select Application"
 						FillInfoMsg( M_NotLocalCard, 0, 0 );
					}
				}
				else
				{
#ifdef MAKE_CARD_MODE2
		// For Sinopec Mode2, Do Not Select PayMode
#else
					// For Employee Card, Selet Pay Mode
					NextScreenID = SCR_PAYMODE;
					DispListInversedIdx = 0;
#endif	// MAKE_CARD_MODE2
				}
			}
			else
			{
				// In Preset Display, Do Not Enter Select Pay Mode Or Application
			}
		
		break;

		// Begin Input Preset
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
		case AMOUNT:
		case VOLUME:
		case CLEAR:

#ifdef MAKE_CARD_MODE2
			if( PowerDown.PdTrans.PD_TRANS_FLAG & AUTH_RESP )
			{
				// Auth End, Can Not Preset....
				FillInfoMsg( M_AuthedNotPreset, 0, 0 );
			}
			else
			{
				// Get Input Preset
				GetPreset();
			}
#else
			// Get Input Preset
			GetPreset();
#endif

		break;

#ifdef MAKE_CARD_MODE2

		case ENTER:

			if( PowerDown.PdTrans.PD_TRANS_FLAG & AUTH_RESP )
			{
				// Auth End, Can Not Preset....
				FillInfoMsg( M_AuthedNotPreset, 0, 0 );
			}
			else
			{
				InputLen = 0;

				// Preset Before Nozzle On  For Sinopec Mode2
				ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= PRESET_MODE2;
			}

		break;

#endif	// MAKE_CARD_MODE2

		default:

		break;
	}
}

#ifdef MAKE_CASH_CR
/*
 ** InputCardBalance_Auth
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input When Display Balance Screen For Cash POS Auth Trans
 *
 *  RETURNS: None.
 *
 */
void InputCardBalance_Auth( void )
{
	unsigned int value;

	switch( KeyValue )
	{
		case SELECT:
			// For Employee Card, Selet Pay Mode
			NextScreenID = SCR_PAYMODE;
			DispListInversedIdx = 0;

		break;

		// Begin Input Auth Code
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':

			value = CharToIntCopy( Cash_CR_Buf.CashTotal );
			// Only Input Auth Code, Before Scanned
			if( value == 0 )
			{
				// Input Auth Code Next
				InputLen = 0;

				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

				memset( InputBuf, 0x00, sizeof( InputBuf ));

				CustomMsg1[InputLen] = KeyValue;
				InputBuf[InputLen] = KeyValue;

				InputLen++;

				NextScreenID = SCR_INPUT_AUTHCODE;
				DispListInversedIdx = 0;
			}
			else
			{
			}

		break;


		default:
		
		break;
	}
}

/*
 ** InputCardBalance_Acceptor
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input When Display Balance Screen For Cash Acceptor Trans
 *
 *  RETURNS: None.
 *
 */
void InputCardBalance_Acceptor( void )
{
	switch( KeyValue )
	{
		case SELECT:
			// For Employee Card, Selet Pay Mode
			NextScreenID = SCR_PAYMODE;
			DispListInversedIdx = 0;

		break;

		default:
		
		break;
	}
}
#endif

/*
 ** InputCardWaiting
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Waiting Screen
 *
 *  RETURNS: None.
 *
 */
void InputCardWaiting( void )
{
	// Do Nothing, Just Waiting
}

/*
 ** InputCardError
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Default Screen Card Reader Error, Include: Card Reader Reset Error And PSAM Error
 *
 *  RETURNS: None.
 *
 */
void InputCardError( void )
{
	// Check Card Reader Error Times
	if( CommErrorTimes >= CR_COMM_ERR_MAX_TIMES )
	{
		// Clear And Re-Entry The Card Precess
		CommErrorTimes = 0;
	
		// Mark Card Reader Error
		ProcStatus[PROCESS_CR] &= ( ~CARD_ERROR );

		FillInfoMsg( M_DefaultInfo, 0, 0 );

		// Start From CardReader Reset
//		Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
		// Set Next Process Step
		CR_Proc_Step = CR_CARDREADER_PROC;
		// Set Next Command To Be Excuted
		CR_CommandType = CR_CommandStart;
	}
	else if( SpringOutCardTimes > SPRINGOUTTIMES )
	{
		SpringOutCardTimes = 0;
							
		// Mark Card Reader Error
		ProcStatus[PROCESS_CR] &= ( ~CARD_ERROR );

		FillInfoMsg( M_DefaultInfo, 0, 0 );

		// Set Next Process Step
		CR_Proc_Step = CR_CARDREADER_PROC;
		// Set Next Command To Be Excuted
		CR_CommandType = CR_CommandStart;
	}
	else if( GetGMACRetryTimes > GETGMACRETRYTIMES )
	{
        GetGMACRetryTimes = 0;
							
		// Mark Card Reader Error
		ProcStatus[PROCESS_CR] &= ( ~CARD_ERROR );

		FillInfoMsg( M_DefaultInfo, 0, 0 );

		// Set Next Process Step
		CR_Proc_Step = CR_CARDREADER_PROC;
		// Set Next Command To Be Excuted
		CR_CommandType = CR_CommandStart;        
	}
}

/*
 ** InputPriceUpdate
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Price Update Screen
 *
 *  RETURNS: None.
 *
 */
void InputPriceUpdate( void )
{
	// Do Nothing, Just Go On....


	// Check For Function
	switch( KeyValue )
	{
		case SETUP:

			// Do Not Display Price Update Flag AnyMore
			// Clear PRICE_UPDATING Flag
			PriceUpdateFlag = 1;
			ProcStatus[PROCESS_SCHEDULE] &= ( ~PRICE_UPDATING );

			// Setup Screen Next
			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;

			NextScreenID = SCR_INPUT_FORMAT_PWD;

		break;

		default:

		break;
	}
}


/*
 ** InputSelectPayMode
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Select Pay Mode
 *
 *  RETURNS: None.
 *
 */
void InputSelectPayMode( void )
{
	if(( KeyValue >= '0' ) && ( KeyValue <= '5' ))
	{
		// Set Pay Mode
		KEYPAD_CR_Buf.PayMode = ( KeyValue & 0x0F );

		// Next Screen
//		NextScreenID = DEFAULT_SCR_BALANCE;
//		ScreenID = NextScreenID;
//		DrawBalanceSCR();
//		
//		// Clear InfoLine, And Display default
//		FillInfoMsg( M_DefaultInfo, 0, 0 );
		DispIdle();

		// Inform IC Card
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SELECT_PAY_MODE;
		
//		DrawBalanceSCR();
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		// Set Pay Mode
		KEYPAD_CR_Buf.PayMode = ( DispListInversedIdx & 0x0F );

		// Next Screen
//		NextScreenID = DEFAULT_SCR_BALANCE;
//		ScreenID = NextScreenID;
//		DrawBalanceSCR();
//		
//		// Clear InfoLine, And Display default
//		FillInfoMsg( M_DefaultInfo, 0, 0 );

		// Inform IC Card
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SELECT_PAY_MODE;
		
//		DrawBalanceSCR();
		DispIdle();
	}
	else if( KeyValue == CLEAR )
	{
		// Set Pay Mode
		KEYPAD_CR_Buf.PayMode = 0;

		// Next Screen
//		NextScreenID = DEFAULT_SCR_BALANCE;
//		ScreenID = NextScreenID;
//		DrawBalanceSCR();
		
//		// Clear InfoLine, And Display default
//		FillInfoMsg( M_DefaultInfo, 0, 0 );

		DispIdle();
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputSelectApp
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Select Application
 *
 *  RETURNS: None.
 *
 */
void InputSelectApp( void )
{
//	if(( KeyValue >= '0' ) && ( KeyValue <= '1' ))
	if(( KeyValue >= '1' ) && ( KeyValue <= '2' ))
	{
		// Set Pay Mode
		KEYPAD_CR_Buf.APPType = (( KeyValue & 0x0F ) - 1 );

		// Next Screen
//		NextScreenID = DEFAULT_SCR_BALANCE;
//		ScreenID = NextScreenID;
//		DrawBalanceSCR();
//		
//		// Clear InfoLine, And Display default
//		FillInfoMsg( M_DefaultInfo, 0, 0 );
		DispIdle();

		// Inform IC Card
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SELECT_APP;
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		// Set Pay Mode
		KEYPAD_CR_Buf.APPType = ( DispListInversedIdx & 0x0F );

		// Next Screen
//		NextScreenID = DEFAULT_SCR_BALANCE;
//		ScreenID = NextScreenID;
//		DrawBalanceSCR();
//		
//		// Clear InfoLine, And Display default
//		FillInfoMsg( M_DefaultInfo, 0, 0 );
		DispIdle();

		// Inform IC Card
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SELECT_APP;
	}
	else if( KeyValue == CLEAR )
	{
		// Set Pay Mode
		KEYPAD_CR_Buf.APPType = 0;

		// Next Screen
//		NextScreenID = DEFAULT_SCR_BALANCE;
//		ScreenID = NextScreenID;
//		DrawBalanceSCR();
//		
//		// Clear InfoLine, And Display default
//		FillInfoMsg( M_DefaultInfo, 0, 0 );
		DispIdle();
	}
	else
	{
		// Input Wrong, Go On....
	}
}

//Douzhq 20140523 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)

void InputSelectAppSaveoil(void)
{
	if(( KeyValue >= '1' ) && ( KeyValue <= '3' ))
	{
		// Set Pay Mode
		KEYPAD_CR_Buf.APPType = (( KeyValue & 0x0F ) - 1 );

		// Next Screen
//		NextScreenID = DEFAULT_SCR_BALANCE;
//		ScreenID = NextScreenID;
//		DrawBalanceSCR();
//		
//		// Clear InfoLine, And Display default
//		FillInfoMsg( M_DefaultInfo, 0, 0 );
		DispIdle();

		// Inform IC Card
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SELECT_APP;
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		// Set Pay Mode
		KEYPAD_CR_Buf.APPType = ( DispListInversedIdx & 0x0F );

		// Next Screen
//		NextScreenID = DEFAULT_SCR_BALANCE;
//		ScreenID = NextScreenID;
//		DrawBalanceSCR();
//		
//		// Clear InfoLine, And Display default
//		FillInfoMsg( M_DefaultInfo, 0, 0 );
		DispIdle();

		// Inform IC Card
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SELECT_APP;
	}
	else if( KeyValue == CLEAR )
	{
		// Set Pay Mode
		KEYPAD_CR_Buf.APPType = 0;

		// Next Screen
//		NextScreenID = DEFAULT_SCR_BALANCE;
//		ScreenID = NextScreenID;
//		DrawBalanceSCR();
//		
//		// Clear InfoLine, And Display default
//		FillInfoMsg( M_DefaultInfo, 0, 0 );
		DispIdle();
	}
	else
	{
		// Input Wrong, Go On....
	}
}

#endif


/*
 ** InputPin
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Input Pin
 *
 *  RETURNS: None.
 *
 */
void InputPin( void )
{
	if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
	{
		if( InputLen < 12 )
		{
			// Max Pin Length Should < 10
			// For Display
			CustomMsg1[InputLen] = '*';

			InputBuf[InputLen] = KeyValue;
			InputLen++;
		}
	}
	else if( KeyValue == ENTER )
	{
		// Input Ok, Check Input
		if( InputLen > 3 )
		{
			KEYPAD_CR_Buf.ICCardPINLen = ASC2BCD_F( KEYPAD_CR_Buf.ICCardPIN, InputBuf, InputLen );

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;
			
			// Inform IC Card Process
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= ICCARD_PIN;
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( InputLen != 0 )
		{
			// For Display
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			// Clear Input
			InputLen = 0;
		}
		else
		{
			// Input Canceled, Return
			// Clear Input
			KEYPAD_CR_Buf.ICCardPINLen = 0;
			memset( KEYPAD_CR_Buf.ICCardPIN, 0x00, sizeof( KEYPAD_CR_Buf.ICCardPIN ));

			// Clear InfoLine, And Display default
			FillInfoMsg( M_DefaultInfo, 0, 0 );

			// Inform IC Card Process
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= ICCARD_PIN;
		}
	}
	else if(( KeyValue == SCROLL_UP ) || ( KeyValue == SCROLL_DOWN ))
	{
		// BackSpace
		if( InputLen != 0 )
		{
			InputLen--;

			// For Display
			CustomMsg1[InputLen] = 0x00;
		}
		else
		{
			// Remain 0
			InputLen = 0; 
		}
	}
	else
	{
		// Input Error, Return;
	}
}
	
/*
 ** InputEMPPWD
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Input Employee Password
 *
 *  RETURNS: None.
 *
 */
void InputEMPPWD( void )
{
	if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
	{
		if( InputLen < 4 )
		{
			// Max Pin Length Should < 10
			// For Display
			CustomMsg1[InputLen] = '*';

			InputBuf[InputLen] = KeyValue;
			InputLen++;
		}
	}
	else if( KeyValue == ENTER )
	{
		// Input Ok, Check Input
		if( InputLen > 3 )
		{
			KEYPAD_CR_Buf.ICCardEMPPwdLen = ASC2BCD_F( KEYPAD_CR_Buf.ICCardEMPPwd, InputBuf, InputLen );

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;
			
			// Set Next ScreenId
//			NextScreenID = DEFAULT_SCR_BALANCE;
			DispIdle();

			// Inform IC Card Process
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_PASSWORD;
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( InputLen != 0 )
		{
			// For Display
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			// Clear Input
			InputLen = 0;
		}
		else
		{
			// Input Canceled, Return
			// Clear Input
			KEYPAD_CR_Buf.ICCardEMPPwdLen = 0;
			memset( KEYPAD_CR_Buf.ICCardEMPPwd, 0x00, sizeof( KEYPAD_CR_Buf.ICCardEMPPwd ));

//			// Clear InfoLine, And Display default
//			FillInfoMsg( M_DefaultInfo, 0, 0 );
//
//			// Set Next ScreenId
//			NextScreenID = DEFAULT_SCR_BALANCE;

//			// Clear InfoLine, And Display default
//			FillInfoMsg( M_DefaultInfo, 0, 0 );

			// Inform IC Card Process
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_PASSWORD;

			DispIdle();
		}
	}
	else if(( KeyValue == SCROLL_UP ) || ( KeyValue == SCROLL_DOWN ))
	{
		// BackSpace
		if( InputLen != 0 )
		{
			InputLen--;

			// For Display
			CustomMsg1[InputLen] = 0x00;
		}
		else
		{
			// Remain 0
			InputLen = 0; 
		}
	}
	else
	{
		// Input Error, Return;
	}
}


/*
 ** InputENTERKey
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Input ENTER Key
 *
 *  RETURNS: None.
 *
 */
void InputENTERKey( void )
{
	if( KeyValue == ENTER )
	{
		// Inform IC Card Process
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= ICCARD_ENTER_KEY;
	}
	else
	{
		// Input Error, Return;
	}
}


// Define The Maintainance Password: 1309
const char MatnPWD[2] = { 0x13, 0x09 };

// Define The Function Password: 9999
const char FuncPWD[2] = { 0x99, 0x99 };

// Define The Format Password: 4949
const char FormatPWD[2] = { 0x49, 0x49 };

/*
 ** InputMaintainPWD
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS: None.
 *
 *  DESCRIPTION: Check Input While Display Input Maintainance Password
 *
 *  RETURNS: None.
 *
 *  Atten: Maintainance Password: 1309
 *
 */
void InputMaintainPWD( void )
{
//	unsigned char len;
	unsigned char password[4];

	if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
	{
		if( InputLen < 4 )
		{
			if( InputLen == 0 )
			{
				// Clear Info Line
				FillInfoMsg( M_DefaultInfo, 0, 0 );
			}

			// Max Pin Length Should < 10
			// For Display
			CustomMsg1[InputLen] = '*';

			InputBuf[InputLen] = KeyValue;
			InputLen++;
		}
	}
	else if( KeyValue == ENTER )
	{
		// Input Ok, Check Input
		if( InputLen == 4 )
		{
			memset( password, 0x00, sizeof( password ));

//			len = ASC2BCD_F( password, InputBuf, InputLen );
			ASC2BCD_F( password, InputBuf, InputLen );

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			
			if( memcmp( password, MatnPWD, sizeof( MatnPWD )) == 0 )
			{
				// Maintanance Password OK
				// Set Next ScreenId
				NextScreenID = SCR_MATN_FUNCTION;
			}
			else
			{
				// Function Password Error, Retry
				FillInfoMsg( M_MATNPWD_RETRY, 0, 0 );
			}

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( InputLen != 0 )
		{
			// For Display
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			// Clear Input
			InputLen = 0;
		}
		else
		{
//			// Clear InfoLine, And Display default
//			FillInfoMsg( M_DefaultInfo, 0, 0 );
			StopDefaultInfo();

			// Input Canceled, Return
			NextScreenID = SCR_FUNCTION;
		}
	}
	else if(( KeyValue == SCROLL_UP ) || ( KeyValue == SCROLL_DOWN ))
	{
		// BackSpace
		if( InputLen != 0 )
		{
			InputLen--;

			// For Display
			CustomMsg1[InputLen] = 0x00;
		}
		else
		{
			// Remain 0
			InputLen = 0; 
		}
	}
	else
	{
		// Input Error, Return;
	}
}

/*
 ** InputFunctionPWD
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Display Input Function Password
 *
 *  RETURNS: None.
 *
 *  Atten: Function Password: 9999
 *
 */
void InputFunctionPWD( void )
{
//	unsigned char len;
	unsigned char password[4];

	if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
	{
		if( InputLen < 4 )
		{
			if( InputLen == 0 )
			{
				// Clear Info Line
				FillInfoMsg( M_DefaultInfo, 0, 0 );
			}

			// Max Pin Length Should < 10
			// For Display
			CustomMsg1[InputLen] = '*';

			InputBuf[InputLen] = KeyValue;
			InputLen++;
		}
	}
	else if( KeyValue == ENTER )
	{
		// Input Ok, Check Input
		if( InputLen == 4 )
		{
			memset( password, 0x00, sizeof( password ));

//			len = ASC2BCD_F( password, InputBuf, InputLen );
			ASC2BCD_F( password, InputBuf, InputLen );

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			
			if( memcmp( password, FuncPWD, sizeof( FuncPWD )) == 0 )
			{
//				if( PriceUpdateFlag == 1 )
//				{
//					// Enter From Price Update Screen, Only Format Config
//					// Function Password OK
//					// Set Next ScreenId
//					// Format Config Flash
//					// Next Screen
//					NextScreenID = SCR_FORMATCONFIG;
//				}
//				else
//				{
					// Function Password OK
					// Set Next ScreenId
					NextScreenID = SCR_FUNCTION;
//				}
			}
			else
			{
				// Function Password Error, Retry
				FillInfoMsg( M_FUNCPWD_RETRY, 0, 0 );
			}

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( InputLen != 0 )
		{
			// For Display
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			// Clear Input
			InputLen = 0;
		}
		else
		{
//			if( PriceUpdateFlag == 1 )
//			{
//				// Previous Screen Is Price Update Screen Display It Again
//				PriceUpdateFlag = 0;
//				ProcStatus[PROCESS_SCHEDULE] |= PRICE_UPDATING;
//			}
//			else
//			{
				// Input Canceled, Return
				DispIdle();
//			}
		}
	}
	else if(( KeyValue == SCROLL_UP ) || ( KeyValue == SCROLL_DOWN ))
	{
		// BackSpace
		if( InputLen != 0 )
		{
			InputLen--;

			// For Display
			CustomMsg1[InputLen] = 0x00;
		}
		else
		{
			// Remain 0
			InputLen = 0; 
		}
	}
	else
	{
		// Input Error, Return;
	}
}

//Douzhq 20140528 Add , Lock Nozzle , Unlock Input Password Window
void InputUnlockPWD(void)
{
	//	unsigned char len;
	unsigned char password[4];
	
	if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
	{
		if( InputLen < 4 )
		{
			if( InputLen == 0 )
			{
				// Clear Info Line
				FillInfoMsg( M_DefaultInfo, 0, 0 );
			}

			// Max Pin Length Should < 10
			// For Display
			CustomMsg1[InputLen] = '*';

			InputBuf[InputLen] = KeyValue;
			InputLen++;
		}
	}
	else if( KeyValue == ENTER )
	{
		// Input Ok, Check Input
		if( InputLen == 4 )
		{
			memset( password, 0x00, sizeof( password ));

//			len = ASC2BCD_F( password, InputBuf, InputLen );
			ASC2BCD_F( password, InputBuf, InputLen );

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			
			//Password 4949
			if( memcmp( password, FormatPWD, sizeof( FormatPWD )) == 0 )
			{
				if (ProcExchange[PROCESS_KEYPAD][PROCESS_KEYPAD] & UNLOCK_PASSWORD)
				{//Insert IC Card , Unlock Nozzle 
					LOCK_NOZZLE.LockFlag = 0;
					
					ProcExchange[PROCESS_KEYPAD][PROCESS_KEYPAD] &= ~(UNLOCK_PASSWORD);
					ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= UNLOCK_PASSWORD;
					WriteLog(PROCESS_KEYPAD,"Unlock Nozzle");
					
					//ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= (~UNLOCK_PASSWORD);
					//NextScreenID = SCR_INPUT_ENTER_KEY;
				}
				else if (ProcExchange[PROCESS_KEYPAD][PROCESS_KEYPAD] & UNLOCK_PWD_FUEL)
				{//Auth Fuel , Unlock Nozzle 
					LOCK_NOZZLE.LockFlag = 0;
					
					ProcExchange[PROCESS_KEYPAD][PROCESS_KEYPAD] &= ~(UNLOCK_PWD_FUEL);
					ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= UNLOCK_PWD_FUEL;
					WriteLog(PROCESS_KEYPAD,"Unlock Nozzle");

					//ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= (~UNLOCK_PWD_FUEL);
				}
				else
				{//Spring IC Card , Unlock Nozzle 
					LOCK_NOZZLE.LockFlag = 0;
					ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SPRING_CARD;  
					WriteLog(PROCESS_KEYPAD,"Unlock Nozzle");
					DispIdle();
				}
			}
			else
			{
				// Function Password Error, Retry
				FillInfoMsg( M_UNLOCK_RETRY, 0, 0 );
			}

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( InputLen != 0 )
		{
			// For Display
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			// Clear Input
			InputLen = 0;
		}
		/*
		else
		{
				DispIdle();
		}*/
	}
	else if(( KeyValue == SCROLL_UP ) || ( KeyValue == SCROLL_DOWN ))
	{
		// BackSpace
		if( InputLen != 0 )
		{
			InputLen--;

			// For Display
			CustomMsg1[InputLen] = 0x00;
		}
		else
		{
			// Remain 0
			InputLen = 0; 
		}
	}
	else
	{
		// Input Error, Return;
	}
}

/*
 ** InputFormatPWD
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Display Input Format Config And Data Password
 *
 *  RETURNS: None.
 *
 */
void InputFormatPWD( void )
{
//	unsigned char len;
	unsigned char password[4];

	if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
	{
		if( InputLen < 4 )
		{
			if( InputLen == 0 )
			{
				// Clear Info Line
				FillInfoMsg( M_DefaultInfo, 0, 0 );
			}

			// Max Pin Length Should < 10
			// For Display
			CustomMsg1[InputLen] = '*';

			InputBuf[InputLen] = KeyValue;
			InputLen++;
		}
	}
	else if( KeyValue == ENTER )
	{
		// Input Ok, Check Input
		if( InputLen == 4 )
		{
			memset( password, 0x00, sizeof( password ));

//			len = ASC2BCD_F( password, InputBuf, InputLen );
			ASC2BCD_F( password, InputBuf, InputLen );

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			
			if( memcmp( password, FormatPWD, sizeof( FormatPWD )) == 0 )
			{
				if( PriceUpdateFlag == 1 )
				{
					// Enter From Price Update Screen, Only Format Config
					// Function Password OK
					// Set Next ScreenId
					// Format Config Flash
					// Next Screen
					NextScreenID = SCR_FORMATCONFIG;
				}
				else
				{
					// Function Password OK
					// Set Next ScreenId
					if( FormatId == 0 )
					{
						// Format Config Flash
						// Next Screen
						NextScreenID = SCR_FORMATCONFIG;
					}
					else
					{
						// Format Config Flash
						// Next Screen
						NextScreenID = SCR_FORMATRECORD;
					}
//					NextScreenID = SCR_FUNCTION;
				}
			}
			else
			{
				// Function Password Error, Retry
				FillInfoMsg( M_FORMATPWD_RETRY, 0, 0 );
			}

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( InputLen != 0 )
		{
			// For Display
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			// Clear Input
			InputLen = 0;
		}
		else
		{
			if( PriceUpdateFlag == 1 )
			{
				// Previous Screen Is Price Update Screen Display It Again
				PriceUpdateFlag = 0;
				ProcStatus[PROCESS_SCHEDULE] |= PRICE_UPDATING;
			}
			else
			{
//				// Input Canceled, Return
//				DispIdle();

				// Format Config Flash
				// Next Screen
				NextScreenID = SCR_FORMAT;
			}
		}
	}
	else if(( KeyValue == SCROLL_UP ) || ( KeyValue == SCROLL_DOWN ))
	{
		// BackSpace
		if( InputLen != 0 )
		{
			InputLen--;

			// For Display
			CustomMsg1[InputLen] = 0x00;
		}
		else
		{
			// Remain 0
			InputLen = 0; 
		}
	}
	else
	{
		// Input Error, Return;
	}
}

/*
 ** InputAuthCode
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Input Auth Code For Cash Trans
 *
 *  RETURNS: None.
 *
 */
void InputAuthCode( void )
{
	if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
	{
		if( InputLen < 12 )
		{
			// Max Auth Length Should < 12
			// For Display
			CustomMsg1[InputLen] = KeyValue;

			InputBuf[InputLen] = KeyValue;
			InputLen++;
		}
	}
	else if( KeyValue == ENTER )
	{
		// Input Ok, Check Input
		if( InputLen != 0 )
		{
//			Cash_CR_Buf.CashAuthCodeLen	= InputLen;
//			memcpy( Cash_CR_Buf.CashAuthCode, CustomMsg1, InputLen );

			Cash_CR_Buf.CashAuthCode[0]	= InputLen;
			memcpy( &Cash_CR_Buf.CashAuthCode[1], CustomMsg1, InputLen );

			TRINP.TR_AUTH_CODE_LEN = InputLen;
			memcpy( TRINP.TR_AUTH_CODE, CustomMsg1, InputLen );

			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= AUTH_CODE;

			DispIdle();

			FillInfoMsg( M_AuthWaiting, 0, 0 );
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( InputLen != 0 )
		{
			// For Display
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memset( InputBuf, 0x00, sizeof( InputBuf ));

			// Clear Input
			InputLen = 0;
		}
		else
		{
			// Input Canceled, Return
			// Clear Input
			DispIdle();


			// Clear InfoLine, And Display default
			FillInfoMsg( M_DefaultInfo, 0, 0 );
		}
	}
	else if(( KeyValue == SCROLL_UP ) || ( KeyValue == SCROLL_DOWN ))
	{
		// BackSpace
		if( InputLen != 0 )
		{
			InputLen--;

			// For Display
			CustomMsg1[InputLen] = 0x00;
		}
		else
		{
			// Remain 0
			InputLen = 0; 
		}
	}
	else
	{
		// Input Error, Return;
	}
}

/*
 ** InputFunction
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Function Screen When Setup Key Pressed
 *
 *  RETURNS: None.
 *
 */
void InputFunction( void )
{
	unsigned char flag;

	if( KeyValue == '0' )
	{
		// Get The Display Messages
		GetVersionDispMessage();

		// Next Screen
		NextScreenID = SCR_VERSION;
	}
	else if( KeyValue == '1' )
	{
		// Display Inquiry Screen
		// Next Screen
		NextScreenID = SCR_INQUIRE;
	}
	else if( KeyValue == '2' )
	{
		// Display Setup Screen
		// Next Screen
		NextScreenID = SCR_SETUP;
	}
	else if( KeyValue == '3' )
	{
		// Display Maintainance Password Screen
		// Next Screen
		DispClearScr();

		// Display Time
		IdleFlg |= DISP_TIME;

		// Clear Input Buffer
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

		memset( InputBuf, 0x00, sizeof( InputBuf ));
		InputLen = 0;

		// Input Maintainance Password
		FillInfoMsg( M_MAINTAIN_PWD, 0, 0 );

		NextScreenID = SCR_INPUT_MAINTAIN_PWD;
	}
	else if( KeyValue == '4' )
	{
		flag = IssueLog( 0 );
		if( flag == 0 )
		{
			// Employee Card Logon
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_LOG_ON;
		
			// Next Screen
//			NextScreenID = DEFAULT_SCR_BALANCE;
			DispIdle();
		}
		else
		{
			// Inform Error
			// Next Screen
//			if( ProcStatus[PROCESS_CR] & CARD_READY )
//			{
//				NextScreenID = DEFAULT_SCR_BALANCE;
//				ScreenID = NextScreenID;
//				DrawBalanceSCR();
//			}
//			else
//			{
//				NextScreenID = DEFAULT_SCR_CARD;
//				ScreenID = NextScreenID;
//				DrawCardInSCR();
//			}
		
			DispIdle();

			// Display Info Line
			IssueLogDispInfoLine( flag );
		}
	}
	else if( KeyValue == '5' )
	{
		flag = IssueLog( 1 );
		if( flag == 0 )
		{
			// Employee Card Logoff
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_LOG_OFF;
		
			// Next Screen
//			NextScreenID = DEFAULT_SCR_BALANCE;
			DispIdle();
		}
		else
		{
			// Inform Error
			// Next Screen
//			if( ProcStatus[PROCESS_CR] & CARD_READY )
//			{
//				NextScreenID = DEFAULT_SCR_BALANCE;
//				ScreenID = NextScreenID;
//				DrawBalanceSCR();
//			}
//			else
//			{
//				NextScreenID = DEFAULT_SCR_CARD;
//				ScreenID = NextScreenID;
//				DrawCardInSCR();
//			}
			
			DispIdle();
		
			// Display Info Line
			IssueLogDispInfoLine( flag );
		}
	}

#ifdef MAKE_CARD_MODE2

	else if( KeyValue == '6' )
	{
		if( ProcStatus[PROCESS_CR] & CARD_READY )
		{
			if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
			{
				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
				memcpy( CustomMsg1, DISPMSG[M_CurrentWorkMode], strlen( DISPMSG[M_CurrentWorkMode] ));
				flag = strlen(( char * )CustomMsg1 );

				if( PowerDown.WorkMode == 0x01 )
				{
					memcpy( &CustomMsg1[flag], DISPMSG[M_WorkOnline], strlen( DISPMSG[M_WorkOnline] ));
				}
				else
				{
					memcpy( &CustomMsg1[flag], DISPMSG[M_WorkOffline], strlen( DISPMSG[M_WorkOffline] ));
				}

				NextScreenID = SCR_WORK_MODE;
			}		
			else
			{
				// Inform Error
				// Next Screen
				DispIdle();
			
				// Display Info Line
				FillInfoMsg( M_InsertMaintainCard, 0, 0 );
			}
		}
		else
		{
			// Inform Error
			// Next Screen
			DispIdle();
		
			// Display Info Line
			FillInfoMsg( M_InsertMaintainCard, 0, 0 );
		}
	}
#endif	// MAKE_CARD_MODE2

	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Get The Display Messages
			GetVersionDispMessage();

			// Next Screen
			NextScreenID = SCR_VERSION;
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Display Inquiry Screen
			// Next Screen
			NextScreenID = SCR_INQUIRE;
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Display Setup Screen
			// Next Screen
			NextScreenID = SCR_SETUP;
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Display Maintainance Password Screen
			// Next Screen
			DispClearScr();
			
			// Display Time
			IdleFlg |= DISP_TIME;

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;

			// Input Maintainance Password
			FillInfoMsg( M_MAINTAIN_PWD, 0, 0 );

			NextScreenID = SCR_INPUT_MAINTAIN_PWD;
		}
		else if( DispListInversedIdx == 0x04 )
		{
			flag = IssueLog( 0 );
			if( flag == 0 )
			{
				// Employee Card Logon
				ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_LOG_ON;
		
				// Next Screen
//				NextScreenID = DEFAULT_SCR_BALANCE;
				DispIdle();
			}
			else
			{
				// Inform Error
				// Next Screen
//				if( ProcStatus[PROCESS_CR] & CARD_READY )
//				{
//					NextScreenID = DEFAULT_SCR_BALANCE;
//					ScreenID = NextScreenID;
//					DrawBalanceSCR();
//				}
//				else
//				{
//					NextScreenID = DEFAULT_SCR_CARD;
//					ScreenID = NextScreenID;
//					DrawCardInSCR();
//				}
		
				DispIdle();

				// Display Info Line
				IssueLogDispInfoLine( flag );
			}
		}
		else if( DispListInversedIdx == 0x05 )
		{
			flag = IssueLog( 1 );
			if( flag == 0 )
			{
				// Employee Card Logoff
				ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_LOG_OFF;
		
				// Next Screen
//				NextScreenID = DEFAULT_SCR_BALANCE;
				DispIdle();
			}
			else
			{
				// Inform Error
				// Next Screen
//				if( ProcStatus[PROCESS_CR] & CARD_READY )
//				{
//					NextScreenID = DEFAULT_SCR_BALANCE;
//					ScreenID = NextScreenID;
//					DrawBalanceSCR();
//				}
//				else
//				{
//					NextScreenID = DEFAULT_SCR_CARD;
//					ScreenID = NextScreenID;
//					DrawCardInSCR();
//				}
//		
				DispIdle();

				// Display Info Line
				IssueLogDispInfoLine( flag );
			}
		}

#ifdef MAKE_CARD_MODE2
		else if( DispListInversedIdx == 0x06 )
		{
			if( ProcStatus[PROCESS_CR] & CARD_READY )
			{
				if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
				{
					memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
					memcpy( CustomMsg1, DISPMSG[M_CurrentWorkMode], strlen( DISPMSG[M_CurrentWorkMode] ));
					flag = strlen(( char * )CustomMsg1 );

					if( PowerDown.WorkMode == 0x01 )
					{
						memcpy( &CustomMsg1[flag], DISPMSG[M_WorkOnline], strlen( DISPMSG[M_WorkOnline] ));
					}
					else
					{
						memcpy( &CustomMsg1[flag], DISPMSG[M_WorkOffline], strlen( DISPMSG[M_WorkOffline] ));
					}

					NextScreenID = SCR_WORK_MODE;
				}		
				else
				{
					// Inform Error
					// Next Screen
					DispIdle();
				
					// Display Info Line
					FillInfoMsg( M_InsertMaintainCard, 0, 0 );
				}
			}
			else
			{
				// Inform Error
				// Next Screen
				DispIdle();
			
				// Display Info Line
				FillInfoMsg( M_InsertMaintainCard, 0, 0 );
			}
		}
#endif	// MAKE_CARD_MODE2

		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
//		if( ProcStatus[PROCESS_CR] & CARD_READY )
//		{
//			NextScreenID = DEFAULT_SCR_BALANCE;
//			ScreenID = NextScreenID;
//			DrawBalanceSCR();
//		}
//		else
//		{
//			NextScreenID = DEFAULT_SCR_CARD;
//			ScreenID = NextScreenID;
//			DrawCardInSCR();
//		}
//
//		// Clear InfoLine, And Display default
//		FillInfoMsg( M_DefaultInfo, 0, 0 );
		DispIdle();
	}
	else
	{
		// Input Wrong, Go On....
	}
}


/*
 ** InputVersion
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Version Screen, And Check Input
 *
 *  RETURNS: None.
 *
 */
void InputVersion( void )
{
	// Pressed Key Return		
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));

	// Next Screen
	NextScreenID = SCR_FUNCTION;
}


/*
 ** InputInquire
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Inquire Screen
 *
 *  RETURNS: None.
 *
 */
void InputInquire( void )
{
	if( KeyValue == '0' )
	{
		// Display Station Information
		GetStationInfo();
		// Next Screen
		NextScreenID = SCR_STATION;
	}
	else if( KeyValue == '1' )
	{
		// Display Inquiry Record Space Information
		GetDispRecordSpace();
		// Next Screen
		NextScreenID = SCR_RECORDSPACE;
	}
	else if( KeyValue == '2' )
	{
		// Display Setup Screen
		// Next Screen
// For Test
//		TransTTC = GetWriteTTC();
//		TransTTC--;
//		FindRecordTTC( TransTTC, &KeyDispTrans.JOURNALSIGN );
//		
//		DispRecordIdx = 0;
//		GetTrans();
//		LastScreenID = SCR_INQUIRE;
//		NextScreenID = SCR_DISP_TRANS;
// For Test

		LastScreenID = SCR_INQUIRE;
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		memset( InputBuf, 0x00, sizeof( InputBuf ));
		InputLen = 0;
		NextScreenID = SCR_INPUT_TTC;
	}
	else if( KeyValue == '3' )
	{
		// Display Volume Total
		GetVolumeTotal();
		// Next Screen
		LastScreenID = SCR_INQUIRE;
		NextScreenID = SCR_VOLUMETOTAL;
	}
	else if( KeyValue == '4' )
	{
		// Display Price Information
		GetPriceInfo();

		// Next Screen
		NextScreenID = SCR_PRICEINQUIRY;
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Display Station Information
			GetStationInfo();
			// Next Screen
			NextScreenID = SCR_STATION;
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Display Inquiry Record Space Information
			GetDispRecordSpace();
			// Next Screen
			NextScreenID = SCR_RECORDSPACE;
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Display Setup Screen
			// Next Screen
// For Test
//			TransTTC = GetWriteTTC();
//			TransTTC--;
//			FindRecordTTC( TransTTC, &KeyDispTrans.JOURNALSIGN );
//		
//			DispRecordIdx = 0;
//			GetTrans();
//
//			LastScreenID = SCR_INQUIRE;
//			NextScreenID = SCR_DISP_TRANS;
// For Test
			LastScreenID = SCR_INQUIRE;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;
			NextScreenID = SCR_INPUT_TTC;
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Display Volume Total
			GetVolumeTotal();
			// Next Screen
			LastScreenID = SCR_INQUIRE;
			NextScreenID = SCR_VOLUMETOTAL;
		}
		else if( DispListInversedIdx == 0x04 )
		{
			// Display Price Information
			GetPriceInfo();

			// Next Screen
			NextScreenID = SCR_PRICEINQUIRY;
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = SCR_FUNCTION;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputStation
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Station Information
 *
 *  RETURNS: None.
 *
 */
void InputStation( void )
{
	NextScreenID = SCR_INQUIRE;
}



/*
 ** InputRecordSpace
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Record Space Information
 *
 *  RETURNS: None.
 *
 */
void InputRecordSpace( void )
{
	NextScreenID = SCR_INQUIRE;
}


/*
 ** InputVolumeTotal
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Volume Total
 *
 *  RETURNS: None.
 *
 */
void InputVolumeTotal( void )
{
	if( KeyValue == ENTER )
	{
		// Display Log On Volume Total
		GetVolumeTotalLogOn();
		// Next Screen
		NextScreenID = SCR_VOLUMETOTALLOGON;
	}
	else if( KeyValue == AMOUNT )
	{
		// Display Amount Total
		GetAmountTotal();
		// Next Screen
		NextScreenID = SCR_AMOUNTTOTAL;
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = LastScreenID;

		if( NextScreenID != SCR_INQUIRE )
		{
			// Come From Log Off Trans
			DispIdle();
		}

		// Reset Last ScreenID
		LastScreenID = SCREEN_START;

//		if( LastScreenID == DEFAULT_SCR_CARD )
//		{
////			NextScreenID = DEFAULT_SCR_CARD;
////			ScreenID = NextScreenID;
////			DrawCardInSCR();
////
////			// Clear InfoLine, And Display default
////			FillInfoMsg( M_DefaultInfo, 0, 0 );
//
//			DispIdle();
//
//			// Come From Log Off Trans
//			NextScreenID = LastScreenID;
//			LastScreenID = SCREEN_START;
//		}
//		else
//		{
//			// Come From Inquire Screen
//			NextScreenID = SCR_INQUIRE;
//		}
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputVolumeTotalLogOn
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Volume Total By This Log On Employee
 *
 *  RETURNS: None.
 *
 */
void InputVolumeTotalLogOn( void )
{
	if( KeyValue == ENTER )
	{
#ifdef MAKE_CASH_CR
		// Display Log On Cash Volume Total
		GetCashVolumeTotalLogOn();
		// Next Screen
		NextScreenID = SCR_CASHVOLUMETOTALLOGON;
#else
		// Display Volume Total
		GetVolumeTotal();
		// Next Screen
		NextScreenID = SCR_VOLUMETOTAL;
#endif
	}
	else if( KeyValue == AMOUNT )
	{
		// Display Log On Amount Total
		GetAmountTotalLogOn();
		// Next Screen
		NextScreenID = SCR_AMOUNTTOTALLOGON;
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = LastScreenID;

		if( NextScreenID != SCR_INQUIRE )
		{
			// Come From Log Off Trans
			DispIdle();
		}

		// Reset Last ScreenID
		LastScreenID = SCREEN_START;

//		if( LastScreenID == DEFAULT_SCR_CARD )
//		{
////			NextScreenID = DEFAULT_SCR_CARD;
////			ScreenID = NextScreenID;
////			DrawCardInSCR();
////
////			// Clear InfoLine, And Display default
////			FillInfoMsg( M_DefaultInfo, 0, 0 );
//
//			DispIdle();
//
//			// Come From Log Off Trans
//			NextScreenID = LastScreenID;
//			LastScreenID = SCREEN_START;
//		}
//		else
//		{
//			// Come From Inquire Screen
//			NextScreenID = SCR_INQUIRE;
//		}
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputAmountTotal
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Amount Total
 *
 *  RETURNS: None.
 *
 */
void InputAmountTotal( void )
{
	if( KeyValue == ENTER )
	{
		// Display Log On Amount Total
		GetAmountTotalLogOn();
		// Next Screen
		NextScreenID = SCR_AMOUNTTOTALLOGON;
	}
	else if( KeyValue == VOLUME )
	{
		// Display Volume Total
		GetVolumeTotal();
		// Next Screen
		NextScreenID = SCR_VOLUMETOTAL;
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = LastScreenID;

		if( NextScreenID != SCR_INQUIRE )
		{
			// Come From Log Off Trans
			DispIdle();
		}

		// Reset Last ScreenID
		LastScreenID = SCREEN_START;

//		if( LastScreenID == DEFAULT_SCR_CARD )
//		{
////			NextScreenID = DEFAULT_SCR_CARD;
////			ScreenID = NextScreenID;
////			DrawCardInSCR();
////
////			// Clear InfoLine, And Display default
////			FillInfoMsg( M_DefaultInfo, 0, 0 );
//
//			DispIdle();
//
//			// Come From Log Off Trans
//			NextScreenID = LastScreenID;
//			LastScreenID = SCREEN_START;
//		}
//		else
//		{
//			// Come From Inquire Screen
//			NextScreenID = SCR_INQUIRE;
//		}
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputAmountTotalLogOn
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Amount Total By This Log On Employee
 *
 *  RETURNS: None.
 *
 */
void InputAmountTotalLogOn( void )
{
	if( KeyValue == ENTER )
	{
#ifdef MAKE_CASH_CR
		// Display Log On Cash Amount Total
		GetCashAmountTotalLogOn();
		// Next Screen
		NextScreenID = SCR_CASHAMOUNTTOTALLOGON;
#else
		// Display Amount Total
		GetAmountTotal();
		// Next Screen
		NextScreenID = SCR_AMOUNTTOTAL;
#endif
	}
	else if( KeyValue == VOLUME )
	{
		// Display Log On Volume Total
		GetVolumeTotalLogOn();
		// Next Screen
		NextScreenID = SCR_VOLUMETOTALLOGON;
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = LastScreenID;

		if( NextScreenID != SCR_INQUIRE )
		{
			// Come From Log Off Trans
			DispIdle();
		}

		// Reset Last ScreenID
		LastScreenID = SCREEN_START;

//		if( LastScreenID == DEFAULT_SCR_CARD )
//		{
////			NextScreenID = DEFAULT_SCR_CARD;
////			ScreenID = NextScreenID;
////			DrawCardInSCR();
////
////			// Clear InfoLine, And Display default
////			FillInfoMsg( M_DefaultInfo, 0, 0 );
//	
//			DispIdle();
//
//			// Come From Log Off Trans
//			NextScreenID = LastScreenID;
//			LastScreenID = SCREEN_START;
//		}
//		else
//		{
//			// Come From Inquire Screen
//			NextScreenID = SCR_INQUIRE;
//		}
	}
	else
	{
		// Input Wrong, Go On....
	}
}


#ifdef MAKE_CASH_CR
/*
 ** InputCashVolumeTotalLogOn
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Cash Volume Total By This Log On Employee
 *
 *  RETURNS: None.
 *
 */
void InputCashVolumeTotalLogOn( void )
{
	if( KeyValue == ENTER )
	{
		// Display Volume Total
		GetVolumeTotal();
		// Next Screen
		NextScreenID = SCR_VOLUMETOTAL;
	}
	else if( KeyValue == AMOUNT )
	{
		// Display Log On Cash Amount Total
		GetCashAmountTotalLogOn();
		// Next Screen
		NextScreenID = SCR_CASHAMOUNTTOTALLOGON;
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = LastScreenID;

		if( NextScreenID != SCR_INQUIRE )
		{
			// Come From Log Off Trans
			DispIdle();
		}

		// Reset Last ScreenID
		LastScreenID = SCREEN_START;
	}
	else
	{
		// Input Wrong, Go On....
	}
}


/*
 ** InputCashAmountTotalLogOn
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Cash Amount Total By This Log On Employee
 *
 *  RETURNS: None.
 *
 */
void InputCashAmountTotalLogOn( void )
{
	if( KeyValue == ENTER )
	{
		// Display Amount Total
		GetAmountTotal();
		// Next Screen
		NextScreenID = SCR_AMOUNTTOTAL;
	}
	else if( KeyValue == VOLUME )
	{
		// Display Log On Cash Volume Total
		GetCashVolumeTotalLogOn();
		// Next Screen
		NextScreenID = SCR_CASHVOLUMETOTALLOGON;
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = LastScreenID;

		if( NextScreenID != SCR_INQUIRE )
		{
			// Come From Log Off Trans
			DispIdle();
		}

		// Reset Last ScreenID
		LastScreenID = SCREEN_START;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputSelectPrint
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Cash Trans Print Receipt Or Not
 *
 *  RETURNS: None.
 *
 */
void InputSelectPrint( void )
{
	if( KeyValue == ENTER )
	{
		// Print Cash Receipt
		KeypadPrint = KEYPAD_PRINT_IT;

		if( CheckCashType( RECORD.RFU ) == 0 )
		{
			// Save To Print Buffer
			FillPrintBuffer( CASH_ACCEPTOR_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
		}
		else
		{
			// Save To Print Buffer
			FillPrintBuffer( CASH_POS_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
		}
							

		ProcStatus[PROCESS_CR] &= ( ~CARD_WAIT_CASH_PRINT );

		// Do Not Print It Again
		KeypadPrint = KEYPAD_PRINT_OVER; 

		// Display In Card
		NextScreenID = DEFAULT_SCR_CARD;
		
	}
	else if( KeyValue == CLEAR )
	{
		// Do Not Print It, And Return
		KeypadPrint = KEYPAD_PRINT_OVER; 

		ProcStatus[PROCESS_CR] &= ( ~CARD_WAIT_CASH_PRINT );

		// Display In Card
		NextScreenID = DEFAULT_SCR_CARD;
	}
	else
	{
		// Wrong Input
	}
}

//enum KEYPAD_SELECT_PRINT
//{
//	KeypadSelectPrintStart,
//
//	KeypadPrintIt,
//
//	KeypadSelectPrintMax
//};
//extern enum KEYPAD_SELECT_PRINT KeypadPrint;
#endif

//Douzhq 20140922 Add , Config Printer Set
void InputChoosePrint( void )
{
	if( KeyValue == ENTER )
	{
		ProcStatus[PROCESS_CR] &= ( ~CARD_WAIT_PRINT );
		
		KeypadPrintCard = CARD_KEYPAD_PRINT_IT;
		
	}
	else if( KeyValue == CLEAR )
	{
		ProcStatus[PROCESS_CR] &= ( ~CARD_WAIT_PRINT );
		
		KeypadPrintCard = CARD_KEYPAD_PRINT_OVER; 
	}
	else
	{
		
	}
}

/*
 ** InputPriceInquiry
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Price Inquiry
 *
 *  RETURNS: None.
 *
 */
void InputPriceInquiry( void)
{
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	NextScreenID = SCR_INQUIRE;
}


/*
 ** InputTTC
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Input TTC Screen
 *
 *  RETURNS: None.
 *
 */
void InputTTC( void )
{
	unsigned char tmp;
	char ttc[10];

	if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
	{
		// Max TTC Length Should < 10
		if( InputLen < 10 )
		{
			// For Display
//			CustomMsg1[InputLen] = KeyValue;

			InputBuf[InputLen] = KeyValue;
			InputLen++;

			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, "TTC: ", 5 );
			memcpy( &CustomMsg1[5], InputBuf, InputLen );
		}
	}
	else if( KeyValue == ENTER )
	{
		if( InputLen == 0 )
		{
			// Press ENTER Key Directly, Get The Last Trans
			TransTTC = GetWriteTTC();
			TransTTC--;
			if( TransTTC == 0 )
			{
				TransTTC = 0xFFFFFFFF;
			}
		}
		else
		{
			// TTC Input, Get The TTC Trans
			memset( ttc, 0x30, sizeof( ttc ));
			memcpy( &ttc[10-InputLen], InputBuf, InputLen );
			TransTTC = atoi( ttc );
		}

		// Clear Input
		InputLen = 0;
		memset( InputBuf, 0x00, sizeof( InputBuf ));

		// Clear CustomMsg1 For Display
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

		tmp = FindRecordTTC( TransTTC, &KeyDispTrans.JOURNALSIGN );
		
		if( tmp == 0 )
		{
			DispRecordIdx = 0;
			GetTrans();
			
			NextScreenID = SCR_DISP_TRANS;
		}
		else
		{
			// Clear Record Space
			memset( &KeyDispTrans.JOURNALSIGN, 0x00, sizeof( KeyDispTrans ));
			
			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;

			// Not Find
			memcpy( CustomMsg1, "无此记录,重新输入", 19 );
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( InputLen != 0 )
		{
			// For Display
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			// Clear Input
			InputLen = 0;
		}
		else
		{
			// Input Canceled, Return
			NextScreenID = SCR_INQUIRE;
		}
	}
	else if(( KeyValue == SCROLL_UP ) || ( KeyValue == SCROLL_DOWN ))
	{
		// BackSpace
		if( InputLen != 0 )
		{
			InputLen--;

			// For Display
			// "TTC: " 5Bytes
			CustomMsg1[InputLen + 5] = 0x00;
		}
		else
		{
			// Remain 0
			InputLen = 0; 
		}
	}
	else
	{
		// Input Error, Return;
	}
}



/*
 ** InputSetupFunction
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Setup Function Screen
 *
 *  RETURNS: None.
 *
 */
void InputSetupFunction( void )
{
	if(( KeyValue >= '0' ) && ( KeyValue <= '3' ))
	{
		if( KeyValue == '0' )
		{
			GetCurrentBackLightTime();

			NextScreenID = SCR_SETUPBACKLIGHT;
		}
		else if( KeyValue == '1' )
		{
			// Setup Print
			NextScreenID = SCR_SETUPPRINT;
		}
		else if( KeyValue == '2' )
		{
			// Setup Volume
			NextScreenID = SCR_SETUPVOLUME;
		}
		else if( KeyValue == '3' )
		{
			// Get Current Status
			GetPSAMCheck();

			// Setup Volume
			NextScreenID = SCR_SETUPPSAM;
		}
		else
		{
		
		}
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		// Next Screen
		if( DispListInversedIdx == 0 )
		{
			// Setup Backlight Time
			GetCurrentBackLightTime();

			NextScreenID = SCR_SETUPBACKLIGHT;
		}
		else if( DispListInversedIdx == 1 )
		{
			// Setup Print
			NextScreenID = SCR_SETUPPRINT;
		}
		else if( DispListInversedIdx == 2 )
		{
			// Setup Volume
			NextScreenID = SCR_SETUPVOLUME;
		}
		else if( DispListInversedIdx == 3 )
		{
			// Get Current Status
			GetPSAMCheck();

			// Setup Volume
			NextScreenID = SCR_SETUPPSAM;
		}
		else
		{
		}
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = SCR_FUNCTION;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputBacklighttime
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Setup Backlight
 *
 *  RETURNS: None.
 *
 */
void InputBacklighttime( void )
{
	unsigned char len;
	unsigned char opentime, closetime;
	char time[2];
	
	if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
	{
		if( InputLen < 4 )
		{
			memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
			
			// Max Pin Length Should < 4
			len = strlen(( char * )CustomMsg1 );

			// For Display
			CustomMsg1[len] = KeyValue;

			InputBuf[InputLen] = KeyValue;
			InputLen++;
		}
	}
	else if( KeyValue == ENTER )
	{
		// Input Ok, Check Input
		if( InputLen == 4 )
		{
			// Chage The BackLight Time And Save It
			memcpy( time, InputBuf, 2 );
			opentime = atoi( time );
			memcpy( time, &InputBuf[2], 2 );
			closetime = atoi( time );

			if(( opentime < 24 ) && ( closetime < 24 ))
			{
				if( opentime > closetime )
				{
					// Input Ok
					SetLcdLightOpenTime( opentime );
					SetLcdLightCloseTime( closetime );

					// Clear Input Buffer
					memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
					memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

					memset( InputBuf, 0x00, sizeof( InputBuf ));
					InputLen = 0;
			
					// Input Ok, Return
					// Next Screen
					NextScreenID = SCR_SETUP;
				}
				else
				{
					len = strlen( DISPMSG[M_NewBackLightTime] );
					memset( &CustomMsg1[len], 0x00, sizeof( CustomMsg1 ) - len );

					// Clear Input
					InputLen = 0;
				
					// Display InfoLine
					memcpy( CustomMsg2, DISPMSG[M_TimeLate], strlen( DISPMSG[M_TimeLate] ));
				}
			}
			else
			{
				len = strlen( DISPMSG[M_NewBackLightTime] );
				memset( &CustomMsg1[len], 0x00, sizeof( CustomMsg1 ) - len );

				// Clear Input
				InputLen = 0;
				
				// Display InfoLine
				memcpy( CustomMsg2, DISPMSG[M_TimeInputError], strlen( DISPMSG[M_TimeInputError] ));
			}
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( InputLen != 0 )
		{
			// For Display
			len = strlen( DISPMSG[M_NewBackLightTime] );
			memset( &CustomMsg1[len], 0x00, sizeof( CustomMsg1 ) - len );

			// Clear Input
			InputLen = 0;
		}
		else
		{
			// Input Canceled, Return
			// Next Screen
			// Clear Input Buffer
			memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;

			NextScreenID = SCR_SETUP;
		}
	}
	else if(( KeyValue == SCROLL_UP ) || ( KeyValue == SCROLL_DOWN ))
	{
		len = strlen(( char * )CustomMsg1 );

		// BackSpace
		if( InputLen != 0 )
		{
			InputLen--;

			// For Display
			CustomMsg1[len-1] = 0x00;
		}
		else
		{
			// Remain 0
			InputLen = 0; 
		}
	}
	else
	{
		// Input Error, Return;
	}
}

/*
 ** InputPrint
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Setup Print
 *
 *  RETURNS: None.
 *
 */
void InputPrint( void )
{
//	unsigned char len;
//	unsigned char opentime, closetime;
//	char time[2];
	
	if( KeyValue == '0' )
	{
		// Customer Card Print Receipt Setup
		GetPrintSetStatus( 0 );
		NextScreenID = SCR_CUSTOMER_CARD_PRINT;
	}
	else if( KeyValue == '1' )
	{
		// Employee Card Print Receipt Setup
		GetPrintSetStatus( 1 );
		NextScreenID = SCR_EMP_CARD_PRINT;
	}
	else if( KeyValue == '2' )
	{
		// Test Card Print Receipt Setup
		GetPrintSetStatus( 2 );
		NextScreenID = SCR_TEST_CARD_PRINT;
	}
	else if( KeyValue == '3' )
	{
		// Maintainance Card Print Receipt Setup
		GetPrintSetStatus( 3 );
		NextScreenID = SCR_MAINTAIN_CARD_PRINT;
	}
	else if( KeyValue == '4' )
	{
		// No Card ASN Trans Print Receipt Setup
		GetPrintSetStatus( 4 );
		NextScreenID = SCR_PUB_TRANS_PRINT;
	}
	//Douzhq 20140919 Add , Config Printer Set
	else if(KeyValue == '5')
	{
		//GetPrintSetStatus( 5 );
		NextScreenID = SCR_EXPAND_SETUP_PRINT;
	}
	else if( KeyValue == ENTER )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Customer Card Print Receipt Setup
			GetPrintSetStatus( 0 );
			NextScreenID = SCR_CUSTOMER_CARD_PRINT;
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Employee Card Print Receipt Setup
			GetPrintSetStatus( 1 );
			NextScreenID = SCR_EMP_CARD_PRINT;
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Test Card Print Receipt Setup
			GetPrintSetStatus( 2 );
			NextScreenID = SCR_TEST_CARD_PRINT;
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Maintainance Card Print Receipt Setup
			GetPrintSetStatus( 3 );
			NextScreenID = SCR_MAINTAIN_CARD_PRINT;
		}
		else if( DispListInversedIdx == 0x04 )
		{
			// No Card ASN Trans Print Receipt Setup
			GetPrintSetStatus( 4 );
			NextScreenID = SCR_PUB_TRANS_PRINT;
		}
		//Douzhq 20140919 Add , Config Printer Set
		else if (DispListInversedIdx == 0x05)
		{
			NextScreenID = SCR_EXPAND_SETUP_PRINT;
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == CLEAR )
	{
		NextScreenID = SCR_SETUP;

		// Save To DataFlash
		SaveCRConfigFlash();
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else
	{
		// Input Error, Return;
	}
}

/*
 ** InputVolume
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Setup Volume
 *
 *  RETURNS: None.
 *
 */
void InputVolume( void )
{
	if( KeyValue == SCROLL_UP )
	{
		// Set Volume Up
		ProcExchange[PROCESS_KEYPAD][PROCESS_VOICE] |= VOICE_VOLUME_UP;
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Set Volume Down
		ProcExchange[PROCESS_KEYPAD][PROCESS_VOICE] |= VOICE_VOLUME_DOWN;
	}
	else if( KeyValue == CLEAR )
	{
		// Exit The Setup Volume Screen
		NextScreenID = SCR_SETUP;
	}
	else
	{
		// Input Error, Just Go On....
	}
}

/*
 ** InputPSAM
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Setup PSAM Validate And Expire Date Check Screen
 *
 *  RETURNS: None.
 *
 */
void InputPSAM( void )
{
	if( KeyValue == SELECT )
	{
		if( PowerDown.SpecialFunction & CheckPSAMValidate )
		{
			// Clear The Bit
			PowerDown.SpecialFunction &= ( ~CheckPSAMValidate );

			memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));

			// Change To Check PSAM Validate Not,
			memcpy( CustomMsg0, DISPMSG[M_PSAMCheckNot], strlen( DISPMSG[M_PSAMCheckNot] ));
		}
		else
		{
			// Set The Bit
			PowerDown.SpecialFunction |= CheckPSAMValidate;

			memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));

			// Change To Check PSAM Validate
			memcpy( CustomMsg0, DISPMSG[M_PSAMCheck], strlen( DISPMSG[M_PSAMCheck] ));
		}
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		memset( CustomMsg0, 0x00, sizeof( CustomMsg1 ));

		// Save To DataFlash
		SaveCRConfigFlash();

		NextScreenID = SCR_SETUP;
	}
	else
	{
		// Input Wrong, Go On....
	}
}


/*
 ** InputCustomerPrint
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Customer Card Print Setup
 *
 *  RETURNS: None.
 *
 */
//QQQQ
void InputCustomerPrint( void )
{
	if(( KeyValue >= '0' ) &&( KeyValue <= '5' ))
	{
		// Customer Card Print Receipt Setup
		UpdateInversedDisplay( KeyValue & 0x0F );
	}
	else if( KeyValue == ENTER )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Trans Ok Print Set
			CustomMsg0[0] = '*';
			SetPrintIt( CustomTransOkReceipt );
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Trans Grey Print Set
			CustomMsg1[0] = '*';
			SetPrintIt( CustomTransGreyReceipt );
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Trans Unlock Print Set
			CustomMsg2[0] = '*';
			SetPrintIt( CustomGreyUnlockReceipt );
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Trans Clear Grey Flag Print Set
			CustomMsg3[0] = '*';
			SetPrintIt( CustomGreyFlagReceipt );
		}
		else if( DispListInversedIdx == 0x04 )
		{
			// Trans Error Card Print Set
			CustomMsg4[0] = '*';
			SetPrintIt( CustomTransErrorReceipt );
		}
		else if( DispListInversedIdx == 0x05 )
		{
			// Trans Card Error Print Set
			CustomMsg5[0] = '*';
			SetPrintIt( CustomCardErrorReceipt );
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Trans Ok Print Set
			CustomMsg0[0] = ' ';
			ClearPrintIt( CustomTransOkReceipt );
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Trans Grey Print Set
			CustomMsg1[0] = ' ';
			ClearPrintIt( CustomTransGreyReceipt );
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Trans Unlock Print Set
			CustomMsg2[0] = ' ';
			ClearPrintIt( CustomGreyUnlockReceipt );
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Trans Clear Grey Flag Print Set
			CustomMsg3[0] = ' ';
			ClearPrintIt( CustomGreyFlagReceipt );
		}
		else if( DispListInversedIdx == 0x04 )
		{
			// Trans Error Card Print Set
			CustomMsg4[0] = ' ';
			ClearPrintIt( CustomTransErrorReceipt );
		}
		else if( DispListInversedIdx == 0x05 )
		{
			// Trans Card Error Print Set
			CustomMsg5[0] = ' ';
			ClearPrintIt( CustomCardErrorReceipt );
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == SELECT )
	{
		NextScreenID = SCR_SETUPPRINT;
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else
	{
		// Input Error, Return;
	}
}

/*
 ** InputEMPPrint
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Employee Card Print Setup
 *
 *  RETURNS: None.
 *
 */
void InputEMPPrint( void )
{
	if(( KeyValue >= '0' ) &&( KeyValue <= '5' ))
	{
		// Customer Card Print Receipt Setup
		UpdateInversedDisplay( KeyValue & 0x0F );
	}
	else if( KeyValue == ENTER )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Trans Ok Print Set
			CustomMsg0[0] = '*';
			SetPrintIt( EMPTransOkReceipt );
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Trans Grey Print Set
			CustomMsg1[0] = '*';
			SetPrintIt( EMPTransGreyReceipt );
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Trans Unlock Print Set
			CustomMsg2[0] = '*';
			SetPrintIt( EMPGreyUnlockReceipt );
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Trans Clear Grey Flag Print Set
			CustomMsg3[0] = '*';
			SetPrintIt( EMPGreyFlagReceipt );
		}
		else if( DispListInversedIdx == 0x04 )
		{
			// Trans Error Card Print Set
			CustomMsg4[0] = '*';
			SetPrintIt( EMPTransErrorReceipt );
		}
		else if( DispListInversedIdx == 0x05 )
		{
			// Trans Card Error Print Set
			CustomMsg5[0] = '*';
			SetPrintIt( EMPCardErrorReceipt );
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Trans Ok Print Set
			CustomMsg0[0] = ' ';
			ClearPrintIt( EMPTransOkReceipt );
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Trans Grey Print Set
			CustomMsg1[0] = ' ';
			ClearPrintIt( EMPTransGreyReceipt );
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Trans Unlock Print Set
			CustomMsg2[0] = ' ';
			ClearPrintIt( EMPGreyUnlockReceipt );
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Trans Clear Grey Flag Print Set
			CustomMsg3[0] = ' ';
			ClearPrintIt( EMPGreyFlagReceipt );
		}
		else if( DispListInversedIdx == 0x04 )
		{
			// Trans Error Card Print Set
			CustomMsg4[0] = ' ';
			ClearPrintIt( EMPTransErrorReceipt );
		}
		else if( DispListInversedIdx == 0x05 )
		{
			// Trans Card Error Print Set
			CustomMsg5[0] = ' ';
			ClearPrintIt( EMPCardErrorReceipt );
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == SELECT )
	{
		NextScreenID = SCR_SETUPPRINT;
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else
	{
		// Input Error, Return;
	}
}

/*
 ** InputTestPrint
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Test Card Print Setup
 *
 *  RETURNS: None.
 *
 */
void InputTestPrint( void )
{
	if(( KeyValue >= '0' ) &&( KeyValue <= '5' ))
	{
		// Customer Card Print Receipt Setup
		UpdateInversedDisplay( KeyValue & 0x0F );
	}
	else if( KeyValue == ENTER )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Trans Ok Print Set
			CustomMsg0[0] = '*';
			SetPrintIt( TestTransOkReceipt );
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Trans Grey Print Set
			CustomMsg1[0] = '*';
			SetPrintIt( TestTransGreyReceipt );
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Trans Unlock Print Set
			CustomMsg2[0] = '*';
			SetPrintIt( TestGreyUnlockReceipt );
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Trans Clear Grey Flag Print Set
			CustomMsg3[0] = '*';
			SetPrintIt( TestGreyFlagReceipt );
		}
		else if( DispListInversedIdx == 0x04 )
		{
			// Trans Error Card Print Set
			CustomMsg4[0] = '*';
			SetPrintIt( TestTransErrorReceipt );
		}
		else if( DispListInversedIdx == 0x05 )
		{
			// Trans Card Error Print Set
			CustomMsg5[0] = '*';
			SetPrintIt( TestCardErrorReceipt );
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Trans Ok Print Set
			CustomMsg0[0] = ' ';
			ClearPrintIt( TestTransOkReceipt );
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Trans Grey Print Set
			CustomMsg1[0] = ' ';
			ClearPrintIt( TestTransGreyReceipt );
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Trans Unlock Print Set
			CustomMsg2[0] = ' ';
			ClearPrintIt( TestGreyUnlockReceipt );
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Trans Clear Grey Flag Print Set
			CustomMsg3[0] = ' ';
			ClearPrintIt( TestGreyFlagReceipt );
		}
		else if( DispListInversedIdx == 0x04 )
		{
			// Trans Error Card Print Set
			CustomMsg4[0] = ' ';
			ClearPrintIt( TestTransErrorReceipt );
		}
		else if( DispListInversedIdx == 0x05 )
		{
			// Trans Card Error Print Set
			CustomMsg5[0] = ' ';
			ClearPrintIt( TestCardErrorReceipt );
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == SELECT )
	{
		NextScreenID = SCR_SETUPPRINT;
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else
	{
		// Input Error, Return;
	}
}

/*
 ** InputMatnPrint
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Maintainance Card Print Setup
 *
 *  RETURNS: None.
 *
 */
void InputMatnPrint( void )
{
	if(( KeyValue >= '0' ) &&( KeyValue <= '5' ))
	{
		// Customer Card Print Receipt Setup
		UpdateInversedDisplay( KeyValue & 0x0F );
	}
	else if( KeyValue == ENTER )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Trans Ok Print Set
			CustomMsg0[0] = '*';
			SetPrintIt( MatnTransOkReceipt );
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Trans Grey Print Set
			CustomMsg1[0] = '*';
			SetPrintIt( MatnTransGreyReceipt );
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Trans Unlock Print Set
			CustomMsg2[0] = '*';
			SetPrintIt( MatnGreyUnlockReceipt );
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Trans Clear Grey Flag Print Set
			CustomMsg3[0] = '*';
			SetPrintIt( MatnGreyFlagReceipt );
		}
		else if( DispListInversedIdx == 0x04 )
		{
			// Trans Error Card Print Set
			CustomMsg4[0] = '*';
			SetPrintIt( MatnTransErrorReceipt );
		}
		else if( DispListInversedIdx == 0x05 )
		{
			// Trans Card Error Print Set
			CustomMsg5[0] = '*';
			SetPrintIt( MatnCardErrorReceipt );
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Trans Ok Print Set
			CustomMsg0[0] = ' ';
			ClearPrintIt( MatnTransOkReceipt );
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Trans Grey Print Set
			CustomMsg1[0] = ' ';
			ClearPrintIt( MatnTransGreyReceipt );
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Trans Unlock Print Set
			CustomMsg2[0] = ' ';
			ClearPrintIt( MatnGreyUnlockReceipt );
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Trans Clear Grey Flag Print Set
			CustomMsg3[0] = ' ';
			ClearPrintIt( MatnGreyFlagReceipt );
		}
		else if( DispListInversedIdx == 0x04 )
		{
			// Trans Error Card Print Set
			CustomMsg4[0] = ' ';
			ClearPrintIt( MatnTransErrorReceipt );
		}
		else if( DispListInversedIdx == 0x05 )
		{
			// Trans Card Error Print Set
			CustomMsg5[0] = ' ';
			ClearPrintIt( MatnCardErrorReceipt );
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == SELECT )
	{
		NextScreenID = SCR_SETUPPRINT;
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else
	{
		// Input Error, Return;
	}
}

/*
 ** InputPubTransPrint
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Trans Without ASN Print Setup
 *
 *  RETURNS: None.
 *
 */
void InputPubTransPrint( void )
{
	if(( KeyValue >= '0' ) &&( KeyValue <= '3' ))
	{
		// Customer Card Print Receipt Setup
		UpdateInversedDisplay( KeyValue & 0x0F );
	}
	else if( KeyValue == ENTER )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Offline Trans Print Set
			CustomMsg0[0] = '*';
			SetPrintIt( TransNonCardReceipt );
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Log On Print Set
			CustomMsg1[0] = '*';
			SetPrintIt( LogOnReceipt );
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Log Off Print Set
			CustomMsg2[0] = '*';
			SetPrintIt( LogOffReceipt );
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Price Receiving Trans Print Set
			CustomMsg3[0] = '*';
			SetPrintIt( PriceRecvReceipt );
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( DispListInversedIdx == 0x00 )
		{
			// Offline Trans Print Set
			CustomMsg0[0] = ' ';
			ClearPrintIt( TransNonCardReceipt );
		}
		else if( DispListInversedIdx == 0x01 )
		{
			// Log On Print Set
			CustomMsg1[0] = ' ';
			ClearPrintIt( LogOnReceipt );
		}
		else if( DispListInversedIdx == 0x02 )
		{
			// Log Off Print Set
			CustomMsg2[0] = ' ';
			ClearPrintIt( LogOffReceipt );
		}
		else if( DispListInversedIdx == 0x03 )
		{
			// Price Receiving Trans Print Set
			CustomMsg3[0] = ' ';
			ClearPrintIt( PriceRecvReceipt );
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == SELECT )
	{
		NextScreenID = SCR_SETUPPRINT;
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else
	{
		// Input Error, Return;
	}
}

//Douzhq 20140919 Add , Config Printer Set
void InputExpandSetPrint( void )
{
	if (KeyValue == '0')
	{		
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
		if (PowerDown.PrinterSwitchMode != 0x01)
		{
			memcpy( CustomMsg1, DISPMSG[M_PrinterFeatureOff], strlen( DISPMSG[M_PrinterFeatureOff] ));
		}
		else 
		{
			memcpy( CustomMsg1, DISPMSG[M_PrinterFeatureOn], strlen( DISPMSG[M_PrinterFeatureOn] ));
		}
		
		NextScreenID = SCR_PRINTER_SWITCH_SET;
	}
	else if(KeyValue == '1')
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
		if (PowerDown.InquirySwitchMode != 0x01)
		{
			memcpy( CustomMsg1, DISPMSG[M_InquiryOff], strlen( DISPMSG[M_InquiryOff] ));
		}
		else 
		{
			memcpy( CustomMsg1, DISPMSG[M_InquiryOn], strlen( DISPMSG[M_InquiryOn] ));
		}
		
		NextScreenID = SCR_INQUIRY_SWITCH_SET;
	}
	else if (KeyValue == '2')
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
		if (PowerDown.ReceiptSwitchMode != 0x01)
		{
			memcpy( CustomMsg1, DISPMSG[M_ReceiptLong], strlen( DISPMSG[M_ReceiptLong] ));
		}
		else
		{
			memcpy( CustomMsg1, DISPMSG[M_ReceiptShort], strlen( DISPMSG[M_ReceiptShort] ));
		}
		
		NextScreenID = SCR_RECEIPT_SWITCH_SET;
	}
	else if( KeyValue == ENTER )
	{
		if (DispListInversedIdx == 0x00)
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
			if (PowerDown.PrinterSwitchMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_PrinterFeatureOff], strlen( DISPMSG[M_PrinterFeatureOff] ));
			}
			else 
			{
				memcpy( CustomMsg1, DISPMSG[M_PrinterFeatureOn], strlen( DISPMSG[M_PrinterFeatureOn] ));
			}
		
			NextScreenID = SCR_PRINTER_SWITCH_SET;
		}
		else if( DispListInversedIdx == 0x01 )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
			if (PowerDown.InquirySwitchMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_InquiryOff], strlen( DISPMSG[M_InquiryOff] ));
			}
			else 
			{
				memcpy( CustomMsg1, DISPMSG[M_InquiryOn], strlen( DISPMSG[M_InquiryOn] ));
			}
			
			NextScreenID = SCR_INQUIRY_SWITCH_SET;
		}
		else if( DispListInversedIdx == 0x02 )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
			if (PowerDown.ReceiptSwitchMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_ReceiptLong], strlen( DISPMSG[M_ReceiptLong] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_ReceiptShort], strlen( DISPMSG[M_ReceiptShort] ));
			}
			
			NextScreenID = SCR_RECEIPT_SWITCH_SET;
		}
		else
		{
			// Input Wrong, Go on....
		}
	}
	else if( KeyValue == CLEAR )
	{
		NextScreenID = SCR_SETUPPRINT;
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else
	{
		// Input Error, Return;
	}
}

void InputPrinterSwitchSet( void )
{
	if( KeyValue == ENTER )
	{
		if( PowerDown.PrinterSwitchMode != 0x01 )
		{
			// Turn on Work Manage
			PowerDown.PrinterSwitchMode = 0x01;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_PrinterFeatureOn], strlen( DISPMSG[M_PrinterFeatureOn] ));
			WriteLog(PROCESS_KEYPAD,"Printer Feature On");
		}
		else
		{
			// Turn off Work Manage
			PowerDown.PrinterSwitchMode = 0;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_PrinterFeatureOff], strlen( DISPMSG[M_PrinterFeatureOff] ));
			WriteLog(PROCESS_KEYPAD,"Printer Feature Off");
		}

		// Save To DataFlash
		SaveCRConfigFlash();
	}
	else if( KeyValue == CLEAR )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
        // Next Screen
		NextScreenID = SCR_EXPAND_SETUP_PRINT;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

void InputInquirySwitchSet( void )
{
	if( KeyValue == ENTER )
	{
		if( PowerDown.InquirySwitchMode != 0x01 )
		{
			// Turn on Work Manage
			PowerDown.InquirySwitchMode = 0x01;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_InquiryOn], strlen( DISPMSG[M_InquiryOn] ));
			WriteLog(PROCESS_KEYPAD,"Printer Inquiry Feature On");
		}
		else
		{
			// Turn off Work Manage
			PowerDown.InquirySwitchMode = 0;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_InquiryOff], strlen( DISPMSG[M_InquiryOff] ));
			WriteLog(PROCESS_KEYPAD,"Printer Inquiry Feature Off");
		}

		// Save To DataFlash
		SaveCRConfigFlash();
	}
	else if( KeyValue == CLEAR )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
        // Next Screen
		NextScreenID = SCR_EXPAND_SETUP_PRINT;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

void InputReceiptSwitchSet( void )
{
	if( KeyValue == ENTER )
	{
		if( PowerDown.ReceiptSwitchMode != 0x01 )
		{
			// Turn on Work Manage
			PowerDown.ReceiptSwitchMode = 0x01;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_ReceiptShort], strlen( DISPMSG[M_ReceiptShort] ));
			WriteLog(PROCESS_KEYPAD,"Printer Receipt Short");
		}
		else
		{
			// Turn off Work Manage
			PowerDown.ReceiptSwitchMode = 0;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_ReceiptLong], strlen( DISPMSG[M_ReceiptLong] ));
			WriteLog(PROCESS_KEYPAD,"Printer Receipt Long");
		}

		// Save To DataFlash
		SaveCRConfigFlash();
	}
	else if( KeyValue == CLEAR )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
        // Next Screen
		NextScreenID = SCR_EXPAND_SETUP_PRINT;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputMatnFunction
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Maintainance Screen
 *
 *  RETURNS: None.
 *
 */
//OOOO
void InputMatnFunction( void )
{
	if(( KeyValue >= '0' ) && ( KeyValue <= '7' ))
	{
		if( KeyValue == '0' )
		{
			// Unfinished Trans
			GetUnfinishedTrans();
			// Next Screen
			NextScreenID = SCR_UNFINISHED_TRANS;
		}
		else if( KeyValue == '1' )
		{
			// Upload Manage
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			InputLen = 0;
			memset( InputBuf, 0x00, sizeof( InputBuf ));

			// Next Screen
			NextScreenID = SCR_UPLOAD;
		}
		else if( KeyValue == '2' )
		{
			// Setup Local Printer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			if( PowerDown.LocalPrinter == PrintLocal )
			{
				memcpy( CustomMsg1, DISPMSG[M_PrinterCR], strlen( DISPMSG[M_PrinterCR] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_PrinterCR_O], strlen( DISPMSG[M_PrinterCR_O] ));
			}

			// Next Screen
			NextScreenID = SCR_LOCAL_PRINTER;
		}
		else if( KeyValue == '3' )
		{
			// Setup POSP
			// Next Screen
			NextScreenID = SCR_POSP;
		}
		else if( KeyValue == '4' )
		{
			// Format The Flash
			// Next Screen
			NextScreenID = SCR_FORMAT;
		}
		else if( KeyValue == '5' )
		{
			// Save Config
			SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));

			UpdateSoftwareCloseAIC();

			// Software Upgrade
			// Next Screen
			NextScreenID = SCR_SOFTWARE_UPGRADE;
		}
        else if( KeyValue == '6' )
		{
			ForceRebuildProtected();
		}
//#ifndef MAKE_CARD_MODE2
        else if( KeyValue == '7' )
		{
		  
//Douzhq 20140528 Add , Lock Nozzle 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
		  	NextScreenID = SCR_MANAGE_SOCIAL;
#else
	#if defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			NextScreenID = SCR_MANAGE_SOCIAL1;
	#else
			NextScreenID = SCR_MANAGE_CARDNUM; 
            // Setup Work Manage
//			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

//			if( PowerDown.WorkManage == 0x01 )
//			{
//				memcpy( CustomMsg1, DISPMSG[M_WorkManage_On], strlen( DISPMSG[M_WorkManage_On] ));
//			}
//			else
//			{
//				memcpy( CustomMsg1, DISPMSG[M_WorkManage_Off], strlen( DISPMSG[M_WorkManage_Off] ));
//			}
            
//			// Work Manage
//			// Next Screen
//			NextScreenID = SCR_WORK_MANAGE;
	#endif
#endif
		}
//#endif
		else
		{
			// Input Wrong, Go On....
		}
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		// Next Screen
		if( DispListInversedIdx == 0 )
		{
			// Unfinished Trans
			GetUnfinishedTrans();
			// Next Screen
			NextScreenID = SCR_UNFINISHED_TRANS;
		}
		else if( DispListInversedIdx == 1 )
		{
			// Upload Manage
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			InputLen = 0;
			memset( InputBuf, 0x00, sizeof( InputBuf ));

			// Next Screen
			NextScreenID = SCR_UPLOAD;
		}
		else if( DispListInversedIdx == 2 )
		{
			// Setup Local Printer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			if( PowerDown.LocalPrinter == PrintLocal )
			{
				memcpy( CustomMsg1, DISPMSG[M_PrinterCR], strlen( DISPMSG[M_PrinterCR] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_PrinterCR_O], strlen( DISPMSG[M_PrinterCR_O] ));
			}

			// Next Screen
			NextScreenID = SCR_LOCAL_PRINTER;
		}
		else if( DispListInversedIdx == 3 )
		{
			// Setup POSP
			// Next Screen
			NextScreenID = SCR_POSP;
		}
		else if( DispListInversedIdx == 4 )
		{
			// Next Screen
			NextScreenID = SCR_FORMAT;
		}
		else if( DispListInversedIdx == 5 )
		{
			// Save Config
			SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));

			UpdateSoftwareCloseAIC();

			// Software Upgrade
			// Next Screen
			NextScreenID = SCR_SOFTWARE_UPGRADE;
		}
        else if( DispListInversedIdx == 6 )
		{
            ForceRebuildProtected();
		}
//#ifndef MAKE_CARD_MODE2
        else if( DispListInversedIdx == 7 )
		{
            // Setup Work Manage
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			if( PowerDown.WorkManage == 0x01 )
			{
				memcpy( CustomMsg1, DISPMSG[M_WorkManage_On], strlen( DISPMSG[M_WorkManage_On] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_WorkManage_Off], strlen( DISPMSG[M_WorkManage_Off] ));
			}
            
            // Work Manage
			// Next Screen
			//Douzhq 20140528 Add , Lock Nozzle 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
		  	NextScreenID = SCR_MANAGE_SOCIAL;
#else
	#if defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			NextScreenID = SCR_MANAGE_SOCIAL1;
	#else
			NextScreenID = SCR_MANAGE_CARDNUM; 
	#endif
#endif
			
		}
//#endif
		else
		{
			// Input Wrong, Go On....
		}
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = SCR_FUNCTION;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputUnfinishedTrans
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Clear Unfinished Trans Screen
 *
 *  RETURNS: None.
 *
 */
void InputUnfinishedTrans( void )
{
	if( KeyValue == ENTER )
	{
//		if( PowerDown.PdTrans.PD_TRANS_FLAG != TRANS_START )
		// 0xFF: Maybe First Running Or The Protect Data Was Corrupted
		if(( PowerDown.PdTrans.PD_TRANS_FLAG != TRANS_START ) && ( PowerDown.PdTrans.PD_TRANS_FLAG != 0xFF ))
		{
			// For SCR_DISP_TRANS Return To;
			LastScreenID = SCR_MATN_FUNCTION;

			// Unfinished Trans Display, Start From DispRecordIdx = 0
			DispRecordIdx = 0;
			GetTrans();

			// Next Screen
			NextScreenID = SCR_DISP_TRANS;
		}
		else
		{
			// No Unfinished Trans, Exit
			// Next Screen
			NextScreenID = SCR_MATN_FUNCTION;
		}
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = SCR_MATN_FUNCTION;
	}
//	else if( KeyValue == SELECT )
//	{
//		// Next Screen
//		PowerDown.PdTrans.PD_TRANS_FLAG = TRANS_START;
//		NextScreenID = SCR_CLEAR_TRANS_CONFIRM;
//	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputUpload
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Inquire Running Log Screen
 *
 *  RETURNS: None.
 *
 */
// Default Upload Load Password: 64331676
const unsigned char UploadPWD[8] = {

	'6', '4', '3', '3', '1', '6', '7', '6'

};
 
void InputUpload( void )
{
	if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
	{
		// Max Password Length Should < 15
		if( InputLen < 15 )
		{
			if( InputLen == 0 )
			{
				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			}

			InputBuf[InputLen] = KeyValue;
			CustomMsg1[InputLen] = '*';
			InputLen++;
		}
	}
	else if( KeyValue == ENTER )
	{
		// Input OK, Check The Password
		if( InputLen == sizeof( UploadPWD ))
		{
			if( memcmp( UploadPWD, InputBuf, InputLen ) == 0 )
			{
				// Password OK, Entry Upload Management Next
				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

				// Clear Input
				memset( InputBuf, 0x00, sizeof( InputBuf ));
				InputLen = 0;

				NextScreenID = SCR_UPLOAD_READY;
			}
			else
			{
				// Input Error
				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
				memset( InputBuf, 0x00, sizeof( InputBuf ));
				InputLen = 0;

				memcpy( CustomMsg1, DISPMSG[M_UploadPWDError], strlen( DISPMSG[M_UploadPWDError] ));
			}
		}
		else
		{
			// Input Error
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;

			memcpy( CustomMsg1, DISPMSG[M_UploadPWDError], strlen( DISPMSG[M_UploadPWDError] ));
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( InputLen != 0 )
		{
			// For Display
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memset( InputBuf, 0x00, sizeof( InputBuf ));

			// Clear Input
			InputLen = 0;
		}
		else
		{
			// Input Canceled, Return
			NextScreenID = SCR_MATN_FUNCTION;
		}
	}
	else if(( KeyValue == SCROLL_UP ) || ( KeyValue == SCROLL_DOWN ))
	{
		// BackSpace
		if( InputLen != 0 )
		{
			InputLen--;

			// For Display
			CustomMsg1[InputLen] = 0x00;
		}
		else
		{
			// Remain 0
			InputLen = 0; 
		}
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputUploadReady
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Upload Management Ready Screen
 *
 *  RETURNS: None.
 *
 */
void InputUploadReady( void )
{
	if( KeyValue == ENTER )
	{
		// Save Config
		SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));

		// Call Upload Managment, And Waiting Reset
		DbugDispose();
	}
	else if( KeyValue == CLEAR )
	{
		// Input Canceled, Return
		NextScreenID = SCR_MATN_FUNCTION;
	}
	else
	{
		// Input Wrong, Go On....
	}
}


/*
 ** InputFormat
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Format Flash Screen
 *
 *  RETURNS: None.
 *
 */
void InputFormat( void )
{
	if(( KeyValue >= '0' ) && ( KeyValue <= '3' ))
	{
		if( KeyValue == '0' )
		{
			// Set The Power Down Debit Or Power On Debit
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			if( PowerDown.SpecialFunction & DebitOnPowerOn )
			{
				// Form Grey Grey Record While Power Down,
				// And Debit Unlock On Power On
				memcpy( CustomMsg1, DISPMSG[M_GreyPowerDown], strlen( DISPMSG[M_GreyPowerDown] ));
			}
			else
			{
				// Debit Unlock On Power Down,
				// And Form Normal Record
				memcpy( CustomMsg1, DISPMSG[M_DebitPowerDown], strlen( DISPMSG[M_DebitPowerDown] ));
			}

			// Next Screen
			NextScreenID = SCR_DEBITPOWERON;
		}
		else if( KeyValue == '1' )
		{
			// Set The Power Down Debit Or Power On Debit
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			if( PowerDown.SpecialFunction & CheckBlackListPOS )
			{
				// Check Black List ON POS First
				memcpy( CustomMsg1, DISPMSG[M_BlackListPOSFirst], strlen( DISPMSG[M_BlackListPOSFirst] ));
			}
			else
			{
				// Check Black List Local First
				memcpy( CustomMsg1, DISPMSG[M_BlackListLocalFirst], strlen( DISPMSG[M_BlackListLocalFirst] ));
			}

			// Next Screen
			NextScreenID = SCR_POSBLACKLIST;
		}
		else if( KeyValue == '2' )
		{
//			// Format Record Flash
//			// Next Screen
//			NextScreenID = SCR_FORMATRECORD;
			
			// Display Maintainance Password Screen
			// Next Screen
			DispClearScr();

			// Display Time
			IdleFlg |= DISP_TIME;

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;

			// Input Maintainance Password
			FillInfoMsg( M_FORMAT_PWD, 0, 0 );

			// Input Format Password Next
			FormatId = 1;
			// Next Screen
			NextScreenID = SCR_INPUT_FORMAT_PWD;
		}
		else if( KeyValue == '3' )
		{
//			// Format Config Flash
//			// Next Screen
//			NextScreenID = SCR_FORMATCONFIG;
			
			// Display Maintainance Password Screen
			// Next Screen
			DispClearScr();

			// Display Time
			IdleFlg |= DISP_TIME;

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;

			// Input Maintainance Password
			FillInfoMsg( M_FORMAT_PWD, 0, 0 );

			// Input Format Password Next
			FormatId = 0;
			// Next Screen
			NextScreenID = SCR_INPUT_FORMAT_PWD;
		}
		else
		{
			// Input Error
		}
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		if( DispListInversedIdx == 0 )
		{
			// Set The Power Down Debit Or Power On Debit
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			if( PowerDown.SpecialFunction & DebitOnPowerOn )
			{
				// Form Grey Grey Record While Power Down,
				// And Debit Unlock On Power On
				memcpy( CustomMsg1, DISPMSG[M_GreyPowerDown], strlen( DISPMSG[M_GreyPowerDown] ));
			}
			else
			{
				// Debit Unlock On Power Down,
				// And Form Normal Record
				memcpy( CustomMsg1, DISPMSG[M_DebitPowerDown], strlen( DISPMSG[M_DebitPowerDown] ));
			}

			// Next Screen
			NextScreenID = SCR_DEBITPOWERON;
		}
		else if( DispListInversedIdx == 1 )
		{
			// Set The Power Down Debit Or Power On Debit
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			if( PowerDown.SpecialFunction & CheckBlackListPOS )
			{
				// Check Black List ON POS First
				memcpy( CustomMsg1, DISPMSG[M_BlackListPOSFirst], strlen( DISPMSG[M_BlackListPOSFirst] ));
			}
			else
			{
				// Check Black List Local First
				memcpy( CustomMsg1, DISPMSG[M_BlackListLocalFirst], strlen( DISPMSG[M_BlackListLocalFirst] ));
			}

			// Next Screen
			NextScreenID = SCR_POSBLACKLIST;
		}
		else if( DispListInversedIdx == 2 )
		{
//			// Format Record Flash
//			// Next Screen
//			NextScreenID = SCR_FORMATRECORD;
			
			// Display Maintainance Password Screen
			// Next Screen
			DispClearScr();

			// Display Time
			IdleFlg |= DISP_TIME;

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;

			// Input Maintainance Password
			FillInfoMsg( M_FORMAT_PWD, 0, 0 );

			// Input Format Password Next
			FormatId = 1;
			// Next Screen
			NextScreenID = SCR_INPUT_FORMAT_PWD;
		}
		else if( DispListInversedIdx == 3 )
		{
//			// Format Config Flash
//			// Next Screen
//			NextScreenID = SCR_FORMATCONFIG;
			
			// Display Maintainance Password Screen
			// Next Screen
			DispClearScr();

			// Display Time
			IdleFlg |= DISP_TIME;

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;

			// Input Maintainance Password
			FillInfoMsg( M_FORMAT_PWD, 0, 0 );

			// Input Format Password Next
			FormatId = 0;
			// Next Screen
			NextScreenID = SCR_INPUT_FORMAT_PWD;
		}
		else
		{
			// Input Error
		}
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = SCR_MATN_FUNCTION;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputPOSP
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Setup POSP Screen
 *
 *  RETURNS: None.
 *
 */
void InputPOSP( void )
{
	if(( KeyValue >= '0' ) && ( KeyValue <= '4' ))
	{
	}
	else if( KeyValue == ENTER )
	{
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = SCR_MATN_FUNCTION;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputLocalPrinter
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Setup Local Printer Screen
 *
 *  RETURNS: None.
 *
 */
void InputLocalPrinter( void )
{

	if( KeyValue == ENTER )
	{
		if( PowerDown.LocalPrinter == PrintLocal )
		{
			// Change To Printer Remote
			PowerDown.LocalPrinter = PrintRemote;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_PrinterCR_O], strlen( DISPMSG[M_PrinterCR_O] ));
		}
		else
		{
			// Change To Printer Local
			PowerDown.LocalPrinter = PrintLocal;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_PrinterCR], strlen( DISPMSG[M_PrinterCR] ));
		}

		// Save To DataFlash
		SaveCRConfigFlash();
	}
	else if( KeyValue == CLEAR )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		// Next Screen
		NextScreenID = SCR_MATN_FUNCTION;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputSoftwareUpgrade
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Software Upgrade Screen
 *
 *  RETURNS: None.
 *
 */
void InputSoftwareUpgrade( void )
{
	if( KeyValue == ENTER )
	{
		while( 1 )
		{
			// Waiting WatchDog
		}
	}
	else
	{
		// Input Wrong, Go On....
	}
}


/*
 ** InputFormatConfig
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Format Config Flash Screen
 *
 *  RETURNS: None.
 *
 */
void InputFormatConfig( void )
{
	if( KeyValue == SELECT )
	{
		// Display Message
		ClearLcd( DispInfoLeftX, DispInfoTopY, DispInfoBottomX, DispInfoBottomY + 1 );
		// Display The Text
		DisplayString12( ScreenList[ScreenID].DispItem[3].ListTopX, ScreenList[ScreenID].DispItem[3].ListTopY, "请等候...." );
		// Display The Screen
		DisBufToLcd();

		// Format Config Flash
		FormatDataFlash( 0 );
	}
	else if( KeyValue == CLEAR )
	{
		if( PriceUpdateFlag == 1 )
		{
			// Previous Screen Is Price Update Screen Display It Again
			PriceUpdateFlag = 0;
			ProcStatus[PROCESS_SCHEDULE] |= PRICE_UPDATING;
		}
		else
		{
			// Next Screen
			NextScreenID = SCR_FORMAT;
		}
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputFormatRecord
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Format Record Flash Screen
 *
 *  RETURNS: None.
 *
 */
void InputFormatRecord( void )
{
	if( KeyValue == SELECT )
	{
		// Display Message
		ClearLcd( DispInfoLeftX, DispInfoTopY, DispInfoBottomX, DispInfoBottomY + 1 );
		// Display The Text
		DisplayString12( ScreenList[ScreenID].DispItem[3].ListTopX, ScreenList[ScreenID].DispItem[3].ListTopY, "请等候...." );
		// Display The Screen
		DisBufToLcd();

		// Save Config
		SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));

		// Format Record Flash
		FormatDataFlash( 1 );
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = SCR_FORMAT;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputDebitPowerDown
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display The Information For Power Down Debit Or Power On
 *
 *  RETURNS: None.
 *
 */
void InputDebitPowerDown( void )
{
	if( KeyValue == SELECT )
	{
		if( PowerDown.SpecialFunction & DebitOnPowerOn )
		{
			// Clear The Bit
			PowerDown.SpecialFunction &= ( ~DebitOnPowerOn );

			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			// Change To Debit Unlock On Power Down,
			// And Form Normal Record
			memcpy( CustomMsg1, DISPMSG[M_DebitPowerDown], strlen( DISPMSG[M_DebitPowerDown] ));
		}
		else
		{
			// Set The Bit
			PowerDown.SpecialFunction |= DebitOnPowerOn;

			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			// Change To Form Grey Grey Record While Power Down,
			// And Debit Unlock On Power On
			memcpy( CustomMsg1, DISPMSG[M_GreyPowerDown], strlen( DISPMSG[M_GreyPowerDown] ));
		}
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

		// Save To DataFlash
		SaveCRConfigFlash();

		NextScreenID = SCR_FORMAT;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

/*
 ** InputBlackListSetup
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display The Information For Check Black List First On POS Or Local.
 *
 *  RETURNS: None.
 *
 */
void InputBlackListSetup( void )
{
	if( KeyValue == SELECT )
	{
		if( PowerDown.SpecialFunction & CheckBlackListPOS )
		{
			// Clear The Bit
			PowerDown.SpecialFunction &= ( ~CheckBlackListPOS );

			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			// Change To Check Black List Local First,
			memcpy( CustomMsg1, DISPMSG[M_BlackListLocalFirst], strlen( DISPMSG[M_BlackListLocalFirst] ));
		}
		else
		{
			// Set The Bit
			PowerDown.SpecialFunction |= CheckBlackListPOS;

			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			// Change To Check Black List POS First
			memcpy( CustomMsg1, DISPMSG[M_BlackListPOSFirst], strlen( DISPMSG[M_BlackListPOSFirst] ));
		}
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

		// Save To DataFlash
		SaveCRConfigFlash();

		NextScreenID = SCR_FORMAT;
	}
	else
	{
		// Input Wrong, Go On....
	}
}


/*
 ** InputDispTrans
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display The Trans Record
 *
 *  RETURNS: None.
 *
 */
//void InputDispTrans( void )
//{
//	unsigned char tmp;
//	unsigned char buf[4];
//
//	if( KeyValue == SCROLL_UP )
//	{
//		if( DispRecordIdx == 0 )
//		{
//			DispRecordIdx = 6;
//		}
//		else
//		{
//			DispRecordIdx--;
//		}
//	
//		GetTrans();
//	}
//	else if( KeyValue == SCROLL_DOWN )
//	{
//		if( DispRecordIdx == 6 )
//		{
//			DispRecordIdx = 0;
//		}
//		else
//		{
//			DispRecordIdx++;
//		}
//	
//		GetTrans();
//	}
//	else if( KeyValue == ENTER )
//	{
//		// Print The Receipt, If The Come From Unfinished Trans, Do Not Print It
//		if( PowerDown.PdTrans.PD_TRANS_FLAG == TRANS_START )
//		{
//			// Add To Print Buffer
//			memset( buf, 0x00, sizeof( buf ));
//			FillPrintBuffer( INQUIRY_RECEIPT, buf, KeyDispTrans.TRANS_TTC );
//			// Exit For Test
////			// Next Screen
////			NextScreenID = SCR_INQUIRE;
//		}
//		else
//		{
//		}
//	}
//	else if( KeyValue == CLEAR )
//	{
//		// Next Screen
//		NextScreenID = LastScreenID;
//	
//		LastScreenID = SCREEN_START;
//	}
//	else if( KeyValue == SELECT )
//	{
//		if( LastScreenID == SCR_INQUIRE )
//		{
//			// Come From Inquire, Display Trans Next
//			// Last Record
//			TransTTC--;
//			tmp = FindRecordTTC( TransTTC, &KeyDispTrans.JOURNALSIGN );
//		
//			if( tmp == 0 )
//			{
//				DispRecordIdx = 0;
//				GetTrans();
//			}
//			else
//			{
//				// Clear The CustomMsgs
//				memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
//				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
//				memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
//				memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));
//
//				// Clear Record Space
//				memset( &KeyDispTrans.JOURNALSIGN, 0x00, sizeof( KeyDispTrans ));
//			
//				// Not Find
//				memcpy( CustomMsg1, "找不到记录", 10 );
//			}
//		}
//		else
//		{
//			// Come From Unfineshed Trans, Do Not Display Any Other Trans
//		}
//	}
//	else
//	{
//		// Input Wrong, Go On....
//	}
//}

void InputDispTrans( void )
{
	unsigned char tmp;
	unsigned char buf[4];

	if( KeyValue == SELECT )
	{
		if( DispRecordIdx > 6 )
		{
			// Forbidden Display Trans
			// Do Nothing
//			DispRecordIdx = 0;
		}
		else if( DispRecordIdx == 6 )
		{
			DispRecordIdx = 0;
			GetTrans();
		}
		else
		{
			DispRecordIdx++;
			GetTrans();
		}
	}
	else if( KeyValue == ENTER )
	{
		// Print The Receipt, If The Come From Unfinished Trans, Do Not Print It
		if( PowerDown.PdTrans.PD_TRANS_FLAG == TRANS_START )
		{
			// Add To Print Buffer
			memset( buf, 0x00, sizeof( buf ));
			FillPrintBuffer( INQUIRY_RECEIPT, buf, KeyDispTrans.TRANS_TTC );
		}
		else
		{
		}
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = LastScreenID;
	
		LastScreenID = SCREEN_START;
	}
	else if( KeyValue == SCROLL_UP )
	{
		if( LastScreenID == SCR_INQUIRE )
		{
			// Come From Inquire, Display Trans Next
			// Last Record
			if( DispRecordIdx != 8 )
			{
				TransTTC--;
			}
			else
			{
				// Come From Last Record End, Do Not Dec TTC
			}

			tmp = FindRecordTTC( TransTTC, &KeyDispTrans.JOURNALSIGN );
		
			if( tmp == 0 )
			{
				DispRecordIdx = 0;
				GetTrans();
			}
			else
			{
				// Clear The CustomMsgs
				memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
				memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
				memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

				// Restore TTC
				if( DispRecordIdx != 8 )
				{
					TransTTC++;
				}
				else
				{
					// Come From Last Record End, Do Not INC TTC
				}

				// Forbidden Display Trans
				DispRecordIdx = 7;

				// Clear Record Space
				memset( &KeyDispTrans.JOURNALSIGN, 0x00, sizeof( KeyDispTrans ));
			
				// Not Find
				memcpy( CustomMsg1, "已到记录头", 10 );
			}
		}
		else
		{
			// Come From Unfineshed Trans, Do Not Display Any Other Trans
		}
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		if( LastScreenID == SCR_INQUIRE )
		{
			// Come From Inquire, Display Trans Next
			// Last Record
			if( DispRecordIdx != 7 )
			{
				TransTTC++;
			}
			else
			{
				// Come From Last Record Start, Do Not Inc TTC
			}

			tmp = FindRecordTTC( TransTTC, &KeyDispTrans.JOURNALSIGN );
		
			if( tmp == 0 )
			{
				DispRecordIdx = 0;
				GetTrans();
			}
			else
			{
				// Clear The CustomMsgs
				memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
				memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
				memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

				// Restore TTC
				if( DispRecordIdx != 7 )
				{
					TransTTC--;
				}
				else
				{
					// Come From Last Record End, Do Not Dec TTC
				}


				// Forbidden Display Trans
				DispRecordIdx = 8;

				// Clear Record Space
				memset( &KeyDispTrans.JOURNALSIGN, 0x00, sizeof( KeyDispTrans ));
			
				// Not Find
				memcpy( CustomMsg1, "已到记录尾", 10 );
			}
		}
		else
		{
			// Come From Unfineshed Trans, Do Not Display Any Other Trans
		}
	}
	else
	{
		// Input Wrong, Go On....
	}
}







/*
 ** InputEnd
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS: None.
 *
 *  DESCRIPTION: Just For Enum END
 *
 *  RETURNS: None.
 *
 */
void InputEnd( void )
{
}



#ifdef MAKE_PSAM_VERIFY

/*
 ** InputPSAM_V_UnSupport
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display PSAM Unsupport Verify Function
 *
 *  RETURNS: None.
 *
 */
void InputPSAM_V_UnSupport( void )
{
}

/*
 ** InputPSAM_V_UnRegistered
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display PSAM UnRegistered Pump
 *
 *  RETURNS: None.
 *
 */
void InputPSAM_V_UnRegistered( void )
{
}

/*
 ** InputPSAM_V_Locked
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display PSAM Locked For Certificate Pump
 *
 *  RETURNS: None.
 *
 */
void InputPSAM_V_Locked( void )
{
}

/*
 ** InputPSAM_V_Insert_RID
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Insert RID Card Please
 *
 *  RETURNS: None.
 *
 */
void InputPSAM_V_Insert_RID( void )
{
}


/*
 ** InputPSAM_V_Register_Ok
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input Display PSAM Register OK Screen
 *
 *  RETURNS: None.
 *
 */
void InputPSAM_V_Register_Ok( void )
{
//	// Display Insert Card
//	NextScreenID = DEFAULT_SCR_CARD;
}

#endif


#ifdef MAKE_CARD_MODE2
/*
 ** InputWorkModeSet
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION:
 *
 *  RETURNS:
 *
 */
void InputWorkModeSet( void )
{
	unsigned char flag;

	switch( KeyValue )
	{
		case SELECT:

			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_CurrentWorkMode], strlen( DISPMSG[M_CurrentWorkMode] ));
			flag = strlen(( char * )CustomMsg1 );

			if( PowerDown.WorkMode == 0x01 )
			{
				PowerDown.WorkMode = 0x00;
				memcpy( &CustomMsg1[flag], DISPMSG[M_WorkOffline], strlen( DISPMSG[M_WorkOffline] ));
			}
			else
			{
				PowerDown.WorkMode = 0x01;
				memcpy( &CustomMsg1[flag], DISPMSG[M_WorkOnline], strlen( DISPMSG[M_WorkOnline] ));
			}
			
		break;

		case CLEAR:

			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			if( PowerDown.WorkMode != 0x01 )
			{
				// For Work Mode Offline, Auth Mode Must Be Pump Auth
				PowerDown.WorkMode = 0x00;
			}

			// Save To DataFlash
			SaveCRConfigFlash();

			// Display Function
			NextScreenID = SCR_FUNCTION;

		break;

		default:

		break;
	}
}

#endif

//#ifndef MAKE_CARD_MODE2

/*
 ** InputWorkManage
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Work Manage Set Screen
 *
 *  RETURNS: None.
 *
 */
//SSSS
void InputWorkManage( void )
{
	if( KeyValue == ENTER )
	{
		if( PowerDown.WorkManage == 0x01 )
		{
			// Turn Off Work Manage
			PowerDown.WorkManage = 0x00;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_WorkManage_Off], strlen( DISPMSG[M_WorkManage_Off] ));
		}
		else
		{
			// Turn On Work Manage
			PowerDown.WorkManage = 0x01;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_WorkManage_On], strlen( DISPMSG[M_WorkManage_On] ));
			
			//Douzhq 20140826 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (PowerDown.SaveOilAppMode == 0x01)
			{
				PowerDown.SaveOilAppMode = 0;
			}
			if (PowerDown.VipFuelingMode == 0x01)
			{
				PowerDown.VipFuelingMode = 0;
			}
#endif
		}

		// Save To DataFlash
		SaveCRConfigFlash();
	}
	else if( KeyValue == CLEAR )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
//Douzhq 20140610 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
        NextScreenID = SCR_MANAGE_SOCIAL;
#else
		
	#if defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
		NextScreenID = SCR_MANAGE_SOCIAL1;
	#else
		NextScreenID = SCR_MANAGE_CARDNUM;
	#endif
		
#endif
	}
	else
	{
		// Input Wrong, Go On....
	}
}

//Douzhq 20140610 Add , Menu About
//XXXX
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
void InputManageSocial( void )
{
	if(( KeyValue >= '0' ) && ( KeyValue <= '7' ))
	{
		if( KeyValue == '0' )
		{
			// Setup Work Manage
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			if( PowerDown.WorkManage == 0x01 )
			{
				memcpy( CustomMsg1, DISPMSG[M_WorkManage_On], strlen( DISPMSG[M_WorkManage_On] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_WorkManage_Off], strlen( DISPMSG[M_WorkManage_Off] ));
			}
            
			// Work Manage
			// Next Screen
			NextScreenID = SCR_WORK_MANAGE;
		}
		else if( KeyValue == '1' )
		{
		  	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.LockNozzleMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_LockNozzleOff], strlen( DISPMSG[M_LockNozzleOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_LockNozzleOn], strlen( DISPMSG[M_LockNozzleOn] ));
			}
			NextScreenID = SCR_LOCK_NOZZLE;
		}
		else if( KeyValue == '2' )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.CardRecoginMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_CardRecognizeOff], strlen( DISPMSG[M_CardRecognizeOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_CardRecognizeOn], strlen( DISPMSG[M_CardRecognizeOn] ));
			}
			NextScreenID = SCR_ICCARD_RECOGINZE;
		}
		else if( KeyValue == '3' )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.SaveOilAppMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_SaveOilAppOff], strlen( DISPMSG[M_SaveOilAppOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_SaveOilAppOn], strlen( DISPMSG[M_SaveOilAppOn] ));
			}
			NextScreenID = SCR_SAVEOIL_FUN;
		}
		else if ( KeyValue == '4' )		
		{
		  	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.VipFuelingMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_VipFuelingOff], strlen( DISPMSG[M_VipFuelingOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_VipFuelingOn], strlen( DISPMSG[M_VipFuelingOn] ));
			}
			NextScreenID = SCR_VIPFUELING_FUN;
		}
		else if ( KeyValue == '5' )		
		{
		  	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.SafeMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_SafeModeOff], strlen( DISPMSG[M_SafeModeOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_SafeModeOn], strlen( DISPMSG[M_SafeModeOn] ));
			}
			NextScreenID = SCR_SAFE_MODE;
		}		
		else
		{
			// Input Wrong, Go On....
		}
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		// Next Screen
		if( DispListInversedIdx == 0 )
		{
			// Setup Work Manage
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			if( PowerDown.WorkManage == 0x01 )
			{
				memcpy( CustomMsg1, DISPMSG[M_WorkManage_On], strlen( DISPMSG[M_WorkManage_On] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_WorkManage_Off], strlen( DISPMSG[M_WorkManage_Off] ));
			}
            
            // Work Manage
			// Next Screen
			NextScreenID = SCR_WORK_MANAGE;
		}
		else if( DispListInversedIdx == 1 )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.LockNozzleMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_LockNozzleOff], strlen( DISPMSG[M_LockNozzleOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_LockNozzleOn], strlen( DISPMSG[M_LockNozzleOn] ));
			}
			NextScreenID = SCR_LOCK_NOZZLE;
		}
		else if( DispListInversedIdx == 2 )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.CardRecoginMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_CardRecognizeOff], strlen( DISPMSG[M_CardRecognizeOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_CardRecognizeOn], strlen( DISPMSG[M_CardRecognizeOn] ));
			}
			NextScreenID = SCR_ICCARD_RECOGINZE;
		}
		else if ( DispListInversedIdx == 3)
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.SaveOilAppMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_SaveOilAppOff], strlen( DISPMSG[M_SaveOilAppOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_SaveOilAppOn], strlen( DISPMSG[M_SaveOilAppOn] ));
			}
			NextScreenID = SCR_SAVEOIL_FUN;
		}
		else if ( DispListInversedIdx == 4 )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.VipFuelingMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_VipFuelingOff], strlen( DISPMSG[M_VipFuelingOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_VipFuelingOn], strlen( DISPMSG[M_VipFuelingOn] ));
			}
			NextScreenID = SCR_VIPFUELING_FUN;
		}
		else if ( DispListInversedIdx == 5 )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.SafeMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_SafeModeOff], strlen( DISPMSG[M_SafeModeOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_SafeModeOn], strlen( DISPMSG[M_SafeModeOn] ));
			}
			NextScreenID = SCR_SAFE_MODE;
		}
		else
		{
			// Input Wrong, Go On....
		}
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = SCR_MATN_FUNCTION;
	}
	else
	{
		// Input Wrong, Go On....
	}
}
#endif

//BBBB
//Douzhq 20140625 Add , Lock Nozzle
void InputLockNozzleSet( void )
{
	if( KeyValue == ENTER )
	{
		if( PowerDown.LockNozzleMode != 0x01 )
		{
			// Turn Off Work Manage
			PowerDown.LockNozzleMode = 0x01;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_LockNozzleOn], strlen( DISPMSG[M_LockNozzleOn] ));
			WriteLog(PROCESS_KEYPAD,"Lock Nozzle Feature On");
		}
		else
		{
			// Turn On Work Manage
			PowerDown.LockNozzleMode = 0;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_LockNozzleOff], strlen( DISPMSG[M_LockNozzleOff] ));
			WriteLog(PROCESS_KEYPAD,"Lock Nozzle Feature Off");
		}

		// Save To DataFlash
		SaveCRConfigFlash();
	}
	else if( KeyValue == CLEAR )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
        // Next Screen
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
		NextScreenID = SCR_MANAGE_SOCIAL;
#else
		
	#if defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
		NextScreenID = SCR_MANAGE_SOCIAL1;
	#else
		NextScreenID = SCR_MANAGE_CARDNUM;
	#endif
		
#endif
		
	}
	else
	{
		// Input Wrong, Go On....
	}
}

//Douzhq 20141010 Add , Safe Mode
//-------  Begin , Douzhq Add , Safe Mode , 20141010 -------
void InputSafeModeSet( void )
{
	if( KeyValue == ENTER )
	{
		if( PowerDown.SafeMode != 0x01 )
		{
			// Turn Off Work Manage
			PowerDown.SafeMode = 0x01;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_SafeModeOn], strlen( DISPMSG[M_SafeModeOn] ));
			WriteLog(PROCESS_KEYPAD,"Safe Mode Feature On");
		}
		else
		{
			// Turn On Work Manage
			PowerDown.SafeMode = 0;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_SafeModeOff], strlen( DISPMSG[M_SafeModeOff] ));
			WriteLog(PROCESS_KEYPAD,"Safe Mode Feature Off");
		}

		// Save To DataFlash
		SaveCRConfigFlash();
	}
	else if( KeyValue == CLEAR )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
        // Next Screen
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
		NextScreenID = SCR_MANAGE_SOCIAL;
#else
		
	#if defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
		NextScreenID = SCR_MANAGE_SOCIAL1;
	#else
		NextScreenID = SCR_MANAGE_CARDNUM;
	#endif
		
#endif
		
	}
	else
	{
		// Input Wrong, Go On....
	}
}
//-------  End   , Douzhq Add , Safe Mode , 20141010 -------

#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
//PPPP
//Douzhq 20140625 Add , IC Card Recognize
void InputCardRecoginze( void )
{
  	unsigned int TempCardRecognizeMode = CharToIntCopy(PowerDown.IcCardSwitchMode);
	
	if(( KeyValue >= '0' ) && ( KeyValue <= '1' ))
	{
		if( KeyValue == '0' )
		{
		 	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.CardRecoginMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_CardRecognizeOff], strlen( DISPMSG[M_CardRecognizeOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_CardRecognizeOn], strlen( DISPMSG[M_CardRecognizeOn] ));
			}
			NextScreenID = SCR_ICCARD_SELECT;
		}
		else if ( KeyValue == '1' )
		{
		  	InitICCardRecognizeSwitch(TempCardRecognizeMode);
			NextScreenID = SCR_CARDAPP_SELECT;
		}
		else
		{
		  //....
		}
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		// Next Screen
		if( DispListInversedIdx == 0 )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.CardRecoginMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_CardRecognizeOff], strlen( DISPMSG[M_CardRecognizeOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_CardRecognizeOn], strlen( DISPMSG[M_CardRecognizeOn] ));
			}
			NextScreenID = SCR_ICCARD_SELECT;
		}
		else if (DispListInversedIdx == 1)
		{			
		  	InitICCardRecognizeSwitch(TempCardRecognizeMode);
			NextScreenID = SCR_CARDAPP_SELECT;
		}
		else
		{
			//....
		}
	}
	else if (KeyValue == CLEAR)
	{
		NextScreenID = SCR_MANAGE_SOCIAL;
	}
	else
	{
		//....
	}
}

//Douzhq 20140625 Add , IC Card Recognize
void InputICCardSwitch( void )
{
	if( KeyValue == ENTER )
	{
		if( PowerDown.CardRecoginMode != 0x01 )
		{
			// Turn on Work Manage
			PowerDown.CardRecoginMode = 0x01;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_CardRecognizeOn], strlen( DISPMSG[M_CardRecognizeOn] ));
			WriteLog(PROCESS_KEYPAD,"IC Card Recognize Feature on");
		}
		else
		{
			// Turn off Work Manage
			PowerDown.CardRecoginMode = 0;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_CardRecognizeOff], strlen( DISPMSG[M_CardRecognizeOff] ));
			WriteLog(PROCESS_KEYPAD,"IC Card Recognize Feature off");
		}

		// Save To DataFlash
		SaveCRConfigFlash();
	}
	else if( KeyValue == CLEAR )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
        // Next Screen
		NextScreenID = SCR_ICCARD_RECOGINZE;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

void InputCardAppSwitch( void )
{
  	unsigned int TempCardRecognizeMode = CharToIntCopy(PowerDown.IcCardSwitchMode);
	
  	if (KeyValue >= '0' && KeyValue <= '9')
	{
		if (KeyValue == '0')
		{
		  	ClearAllRecognizeFlag(&TempCardRecognizeMode);
			SetRecognizeFlag(SinopecAppFlag,&TempCardRecognizeMode);
			IntToCharCopy(TempCardRecognizeMode,PowerDown.IcCardSwitchMode);
			CustomMsg0[0] = '*';
			WriteLog(PROCESS_KEYPAD,"IC Card Recognize : 中石化");
		}
		else if (KeyValue == '1')
		{
		  	ClearAllRecognizeFlag(&TempCardRecognizeMode);
			SetRecognizeFlag(SocialApp1Flag,&TempCardRecognizeMode);
			IntToCharCopy(TempCardRecognizeMode,PowerDown.IcCardSwitchMode);
			CustomMsg1[0] = '*';
			WriteLog(PROCESS_KEYPAD,"IC Card Recognize : 新华联");
		}
		else if (KeyValue == '2')
		{
		  	ClearAllRecognizeFlag(&TempCardRecognizeMode);
			SetRecognizeFlag(OtherApp1Flag,&TempCardRecognizeMode);
			IntToCharCopy(TempCardRecognizeMode,PowerDown.IcCardSwitchMode);
			CustomMsg2[0] = '*';
		}
		else if (KeyValue == '3')
		{
		  	ClearAllRecognizeFlag(&TempCardRecognizeMode);
			SetRecognizeFlag(OtherApp2Flag,&TempCardRecognizeMode);
			IntToCharCopy(TempCardRecognizeMode,PowerDown.IcCardSwitchMode);
			CustomMsg3[0] = '*';
		}
		else
		{
			//....
		}
		SaveCRConfigFlash();
	}
	else if( KeyValue == CLEAR )
	{
		NextScreenID = SCR_ICCARD_RECOGINZE;
	}
	else if (KeyValue == SCROLL_UP)
	{
		ScrollUpSelect();
	}
	else if (KeyValue == SCROLL_DOWN)
	{
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		// Next Screen
		if( DispListInversedIdx == 0 )
		{
		  	ClearAllRecognizeFlag(&TempCardRecognizeMode);
			SetRecognizeFlag(SinopecAppFlag,&TempCardRecognizeMode);
			IntToCharCopy(TempCardRecognizeMode,PowerDown.IcCardSwitchMode);
			CustomMsg0[0] = '*';	
			WriteLog(PROCESS_KEYPAD,"IC Card Recognize : 中石化");
		}
		else if (DispListInversedIdx == 1)
		{
		  	ClearAllRecognizeFlag(&TempCardRecognizeMode);
			SetRecognizeFlag(SocialApp1Flag,&TempCardRecognizeMode);
			IntToCharCopy(TempCardRecognizeMode,PowerDown.IcCardSwitchMode);
			CustomMsg1[0] = '*';
			WriteLog(PROCESS_KEYPAD,"IC Card Recognize : 新华联");
		}
		else if (DispListInversedIdx == 2)
		{
		  	ClearAllRecognizeFlag(&TempCardRecognizeMode);
			SetRecognizeFlag(OtherApp1Flag,&TempCardRecognizeMode);
			IntToCharCopy(TempCardRecognizeMode,PowerDown.IcCardSwitchMode);
			CustomMsg2[0] = '*';
		}
		else if (DispListInversedIdx == 3)
		{
		  	ClearAllRecognizeFlag(&TempCardRecognizeMode);
			SetRecognizeFlag(OtherApp2Flag,&TempCardRecognizeMode);
			IntToCharCopy(TempCardRecognizeMode,PowerDown.IcCardSwitchMode);
			CustomMsg3[0] = '*';
		}
		else
		{
			//....
		}
		SaveCRConfigFlash();
	}
	else
	{
		//....
	}
}

//NNNN
//Douzhq 20140625 Add , Save Oil App
void InputSaveOilAppSet( void )
{
	if( KeyValue == ENTER )
	{
		if( PowerDown.SaveOilAppMode != 0x01 )
		{
			// Turn on Work Manage
			PowerDown.SaveOilAppMode = 0x01;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_SaveOilAppOn], strlen( DISPMSG[M_SaveOilAppOn] ));
			WriteLog(PROCESS_KEYPAD,"Save Oil Feature On");
			
			//Douzhq 20140826 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (PowerDown.WorkManage == 0x01)
			{
				PowerDown.WorkManage = 0;
			}
#endif
		}
		else
		{
			// Turn off Work Manage
			PowerDown.SaveOilAppMode = 0;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_SaveOilAppOff], strlen( DISPMSG[M_SaveOilAppOff] ));
			WriteLog(PROCESS_KEYPAD,"Save Oil Feature Off");
		}

		// Save To DataFlash
		SaveCRConfigFlash();
	}
	else if( KeyValue == CLEAR )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
        // Next Screen
		NextScreenID = SCR_MANAGE_SOCIAL;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

//Douzhq 20140625 Add , Vip Fueling
void InputVipFuelingSet(void)
{

	if( KeyValue == ENTER )
	{
		if( PowerDown.VipFuelingMode != 0x01 )
		{
			// Turn on Work Manage
			PowerDown.VipFuelingMode = 0x01;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_VipFuelingOn], strlen( DISPMSG[M_VipFuelingOn] ));
			WriteLog(PROCESS_KEYPAD,"Vip Feature On");
			
			//Douzhq 20140826 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (PowerDown.WorkManage == 0x01)
			{
				PowerDown.WorkManage = 0;
			}
#endif
		}
		else
		{
			// Turn off Work Manage
			PowerDown.VipFuelingMode = 0;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_VipFuelingOff], strlen( DISPMSG[M_VipFuelingOff]));
			WriteLog(PROCESS_KEYPAD,"vip Feature Off");
		}

		// Save To DataFlash
		SaveCRConfigFlash();
	}
	else if( KeyValue == CLEAR )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
        // Next Screen
		NextScreenID = SCR_MANAGE_SOCIAL;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

#endif

//Douzhq 20140819 Add , Menu About
//Manage Menu , PASM Verify and No CASH
#if defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
void InputManageSocial1(void)
{
	if(( KeyValue >= '0' ) && ( KeyValue <= '7' ))
	{
		if( KeyValue == '0' )
		{
			// Setup Work Manage
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			if( PowerDown.WorkManage == 0x01 )
			{
				memcpy( CustomMsg1, DISPMSG[M_WorkManage_On], strlen( DISPMSG[M_WorkManage_On] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_WorkManage_Off], strlen( DISPMSG[M_WorkManage_Off] ));
			}
            
			// Work Manage
			// Next Screen
			NextScreenID = SCR_WORK_MANAGE;
		}
		else if( KeyValue == '1' )
		{
		  	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.LockNozzleMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_LockNozzleOff], strlen( DISPMSG[M_LockNozzleOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_LockNozzleOn], strlen( DISPMSG[M_LockNozzleOn] ));
			}
			NextScreenID = SCR_LOCK_NOZZLE;
		}
		else if ( KeyValue == '2' )
		{
			// Setup Safe Mode Manage
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.SafeMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_SafeModeOff], strlen( DISPMSG[M_SafeModeOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_SafeModeOn], strlen( DISPMSG[M_SafeModeOn] ));
			}
			NextScreenID = SCR_SAFE_MODE;
		}
		else
		{
			// Input Wrong, Go On....
		}
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		// Next Screen
		if( DispListInversedIdx == 0 )
		{
			// Setup Work Manage
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			if( PowerDown.WorkManage == 0x01 )
			{
				memcpy( CustomMsg1, DISPMSG[M_WorkManage_On], strlen( DISPMSG[M_WorkManage_On] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_WorkManage_Off], strlen( DISPMSG[M_WorkManage_Off] ));
			}
            
            // Work Manage
			// Next Screen
			NextScreenID = SCR_WORK_MANAGE;
		}
		else if( DispListInversedIdx == 1 )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.LockNozzleMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_LockNozzleOff], strlen( DISPMSG[M_LockNozzleOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_LockNozzleOn], strlen( DISPMSG[M_LockNozzleOn] ));
			}
			NextScreenID = SCR_LOCK_NOZZLE;
		}
		else if ( DispListInversedIdx == 2 )
		{
			// Setup Safe Mode Manage
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.SafeMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_SafeModeOff], strlen( DISPMSG[M_SafeModeOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_SafeModeOn], strlen( DISPMSG[M_SafeModeOn] ));
			}
			NextScreenID = SCR_SAFE_MODE;
		}
		else
		{
			// Input Wrong, Go On....
		}
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = SCR_MATN_FUNCTION;
	}
	else
	{
		// Input Wrong, Go On....
	}
}
#endif

//Douzhq 20140718 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
void InputManageCarNum( void )
{
	if(( KeyValue >= '0' ) && ( KeyValue <= '7' ))
	{
		if( KeyValue == '0' )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.LockNozzleMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_LockNozzleOff], strlen( DISPMSG[M_LockNozzleOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_LockNozzleOn], strlen( DISPMSG[M_LockNozzleOn] ));
			}
			NextScreenID = SCR_LOCK_NOZZLE;
		}
		else if( KeyValue == '1' )
		{
		  	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.CarNumRecognizeModw != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_CarNumberRecognizeOff], strlen( DISPMSG[M_CarNumberRecognizeOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_CarNumberRecognizeOn], strlen( DISPMSG[M_CarNumberRecognizeOn] ));
			}
			NextScreenID = SCR_CARNUMBER_RECOGNIZE;
		}
		else if( KeyValue == '2' )
		{
			// Setup Safe Mode Manage
		  	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.SafeMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_SafeModeOff], strlen( DISPMSG[M_SafeModeOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_SafeModeOn], strlen( DISPMSG[M_SafeModeOn] ));
			}
			NextScreenID = SCR_SAFE_MODE;
		}
		else 
		{
			//....
		}
	}
	else if( KeyValue == SCROLL_UP )
	{
		// Inverse Display The Last Item
		ScrollUpSelect();
	}
	else if( KeyValue == SCROLL_DOWN )
	{
		// Inverse Display The Last Item
		ScrollDownSelect();
	}
	else if( KeyValue == ENTER )
	{
		// Next Screen
		if( DispListInversedIdx == 0 )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.LockNozzleMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_LockNozzleOff], strlen( DISPMSG[M_LockNozzleOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_LockNozzleOn], strlen( DISPMSG[M_LockNozzleOn] ));
			}
			NextScreenID = SCR_LOCK_NOZZLE;
		}
		else if( DispListInversedIdx == 1 )
		{
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.CarNumRecognizeModw != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_CarNumberRecognizeOff], strlen( DISPMSG[M_CarNumberRecognizeOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_CarNumberRecognizeOn], strlen( DISPMSG[M_CarNumberRecognizeOn] ));
			}
			NextScreenID = SCR_CARNUMBER_RECOGNIZE;
		}
		else if( DispListInversedIdx == 2 )
		{
			// Setup Safe Mode Manage
		  	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			if (PowerDown.SafeMode != 0x01)
			{
				memcpy( CustomMsg1, DISPMSG[M_SafeModeOff], strlen( DISPMSG[M_SafeModeOff] ));
			}
			else
			{
				memcpy( CustomMsg1, DISPMSG[M_SafeModeOn], strlen( DISPMSG[M_SafeModeOn] ));
			}
			NextScreenID = SCR_SAFE_MODE;
		}
		else 
		{
			// ......
		}
		
	}
	else if( KeyValue == CLEAR )
	{
		// Next Screen
		NextScreenID = SCR_MATN_FUNCTION;
	}
	else
	{
		// Input Wrong, Go On....
	}
}

//Douzhq 20140718 Add , Car Number Recognize
void InputCarNumberRec(void)
{
	if( KeyValue == ENTER )
	{
		if( PowerDown.CarNumRecognizeModw != 0x01 )
		{
			// Turn on Work Manage
			PowerDown.CarNumRecognizeModw = 0x01;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_CarNumberRecognizeOn], strlen( DISPMSG[M_CarNumberRecognizeOn] ));
			WriteLog(PROCESS_KEYPAD,"Car Number Recognize Feature On");
		}
		else
		{
			// Turn off Work Manage
			PowerDown.CarNumRecognizeModw = 0;
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			memcpy( CustomMsg1, DISPMSG[M_CarNumberRecognizeOff], strlen( DISPMSG[M_CarNumberRecognizeOff]));
			WriteLog(PROCESS_KEYPAD,"Car Number Recognize Feature On");
		}

		// Save To DataFlash
		SaveCRConfigFlash();
	}
	else if( KeyValue == CLEAR )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		
        // Next Screen
		NextScreenID = SCR_MANAGE_CARDNUM;
	}
	else
	{
		// Input Wrong, Go On....
	}
}
#endif

/*
 ** InputEMPNumber
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Input EMP Number Screen
 *
 *  RETURNS: None.
 *
 */
void InputEMPNumber( void )
{    
    unsigned short Temp = 0;
    char EmpNumber[10];
    
	if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
	{
		if( InputLen < 3 )
		{
			// Max Emp Number Length Should < 3
			// For Display
			CustomMsg1[InputLen] = KeyValue;

			InputBuf[InputLen] = KeyValue;
			InputLen++;
		}
	}
	else if( KeyValue == ENTER )
	{
		// Input Ok, Check Input
		if( InputLen > 0 )
		{      
            memset( EmpNumber, 0x00 ,sizeof( EmpNumber ) );
            memcpy( EmpNumber, InputBuf, InputLen );
            
            Temp = atoi( EmpNumber );
            
            if( ( Temp > 0x00 ) && ( Temp < 0xFF ) )
            {    
                KEYPAD_CR_Buf.FuelEmpNumberLen = InputLen;
                PowerDown.FuelEmpNumber = Temp;
                
                // Clear Input Buffer
                memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
    
                memset( InputBuf, 0x00, sizeof( InputBuf ) );
                InputLen = 0;
                
                // Inform IC Card Process
                ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_NUMBER;
            }
            else
            {
                // Display " Out Of Range "
				FillInfoMsg( M_Emp_Number_Out_Of_Range, 0, 0 );
            }
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( InputLen != 0 )
		{
			// For Display
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ) );

			// Clear Input
			InputLen = 0;
		}
		else
		{
			// Input Canceled, Return
			// Clear Input
			KEYPAD_CR_Buf.FuelEmpNumberLen = 0;

			// Clear InfoLine, And Display default
			FillInfoMsg( M_DefaultInfo, 0, 0 );

			// Inform IC Card Process
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_NUMBER;
		}
	}
	else if(( KeyValue == SCROLL_UP ) || ( KeyValue == SCROLL_DOWN ))
	{
		// BackSpace
		if( InputLen != 0 )
		{
			InputLen--;

			// For Display
			CustomMsg1[InputLen] = 0x00;
		}
		else
		{
			// Remain 0
			InputLen = 0; 
		}
	}
	else
	{
		// Input Error, Return;
	}
}

//#endif

/*
 ** InputOverFueling
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: check the spring key
 *
 *  RETURNS: None.
 *
 */
void InputOverFueling(void)       //20110906
{
    if( KeyValue == SPRING )
    {
		ProcStatus[PROCESS_CR] &=(~CARD_OVER_FURLING);
        ProcStatus[PROCESS_CR] |=CARD_OVER_FURLING_PASSWORD;
        NextScreenID = SRC_OVERFUELPASSWORD;
    }
}

/*
 ** InputOverFuelingPassword
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Input While Display Input Employee Password
 *
 *  RETURNS: None.
 *
 */
void InputOverFuelingPassword(void)       //20110906  steven
{
    if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
	{
		if( InputLen < 4 )
		{
			// Max Pin Length Should < 10
			// For Display
			CustomMsg1[InputLen] = '*';

			InputBuf[InputLen] = KeyValue;
			InputLen++;
		}
	}
	else if( KeyValue == ENTER )
	{
		// Input Ok, Check Input
		if( InputLen > 3 )
		{
			KEYPAD_CR_Buf.ICCardEMPPwdLen = ASC2BCD_F( KEYPAD_CR_Buf.ICCardEMPPwd, InputBuf, InputLen );

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;
			
			// Set Next ScreenId
//			NextScreenID = DEFAULT_SCR_BALANCE;
			DispIdle();

			// Inform IC Card Process
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_PASSWORD;
		}
	}
	else if( KeyValue == CLEAR )
	{
		if( InputLen != 0 )
		{
			// For Display
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			// Clear Input
			InputLen = 0;
		}
		else
		{
			// Input Canceled, Return
			// Clear Input
			KEYPAD_CR_Buf.ICCardEMPPwdLen = 0;
			memset( KEYPAD_CR_Buf.ICCardEMPPwd, 0x00, sizeof( KEYPAD_CR_Buf.ICCardEMPPwd ));

//			// Clear InfoLine, And Display default
//			FillInfoMsg( M_DefaultInfo, 0, 0 );
//
//			// Set Next ScreenId
//			NextScreenID = DEFAULT_SCR_BALANCE;

//			// Clear InfoLine, And Display default
//			FillInfoMsg( M_DefaultInfo, 0, 0 );

			// Inform IC Card Process
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_PASSWORD;

			DispIdle();
		}
	}
	else if(( KeyValue == SCROLL_UP ) || ( KeyValue == SCROLL_DOWN ))
	{
		// BackSpace
		if( InputLen != 0 )
		{
			InputLen--;

			// For Display
			CustomMsg1[InputLen] = 0x00;
		}
		else
		{
			// Remain 0
			InputLen = 0; 
		}
	}
	else
	{
		// Input Error, Return;
	}  
  
}