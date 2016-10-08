



#include "global.h"

#include "CardReader.h"
#include "CRMemCopy.h"
#include "CRMemFunc.h"
#include "CRCommand.h"
#include "CRVaribles.h"
#include "PSAMCommand.h"
#include "ICCardCommand.h"
#include "PSAMProc.h"
#include "ICCardProc.h"
#include "ICCardInProc.h"
#include "CardReady.h"
#include "ExCRProc.h"
#include "CRPumpCall.h"
#include "CRPumpEOT.h"
#include "CRPumpBusy.h"
#include "GreyCard.h"
#include "Journal.h"
#include "CRSFunc.h"
#include "Pdprotect.h"
#include "define.h"
#include "CRPowerOn.h"
#include "message.h"
#include "ACTCardProc.h"
#include "RIDCardProc.h"
#include "WorkMode.h"


/*
 ** CardReader
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CardReader.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Entrance for the IC Card Process
 *
 *  RETURNS: None.
 *
 */
void ICCardProcess( void )
{
	// Check PowerDown
	CR_PowerDown();

	// Check Pump Online Or Not
	if( ProcStatus[PROCESS_PUMP] & PUMP_ONLINE )
	{
		IdleFlg |= 0x04;
	}
	else
	{
		IdleFlg &= ( ~0x04 );
	}

	// Inform Power Down Process
	if( ProcExchange[PROCESS_CR][PROCESS_POWERDOWN] & POWERDOWN )
	{
		// IC Card Power Down Already Issued, Just Return
		return;
	}

//	if( ProcStatus[PROCESS_SCHEDULE] & PRICE_UPDATING )
//	{
//		if( CR_CommandStatus == CR_CommandSending )
//		{
//			// Price Updating, Do Not Check Card
//			return;
//		}
//	}

	if( CR_Check_Err() != 0 )
	{
		// There Is Something Error In
		// Card Reader Process, So Return;
		return;
	}

	// Check Power Down Or Not
	switch( CR_Proc_Step )
	{
		case CR_CRPOWERON_PROC:

			// Card Reader Power On Process
			CRPowerOn_Proc();

		break;

		case CR_CARDREADER_PROC:

			// Card Reader Process
			CardReader_Proc();

		break;

		case CR_PSAM_PROC:

			// Process PSAM
			PSAM_Proc();

		break;

		case CR_ICCARD_ABSENCE_PROC:

			// Process Card Reader WithOut IC Card
			ICCard_Absence_Proc();

		break;

		case CR_ICCARD_INSERT_PROC:

			// Process Card Reader With IC Card	Just Inserted
			ICCard_Inserted_Proc();

		break;

		case CR_ICCARD_READY_PROC:

			// Process Card Reader With IC Card	Ready For Fuelling
			ICCard_Ready_Proc();

		break;

		case CR_ICCARD_GREY_CARD_PROC:

			// Process Card Reader With IC Card	Grey, Do Grey Unlock
			// And Form Record
			ICCard_Grey_Card_Proc();

		break;

		case CR_ICCARD_PUMP_CALL_PROC:

			// Process IC Card Pump Call, Set Grey, Check Limit And Preset etc.
			// And Form Record
			ICCard_Pump_Call_Proc();

		break;

		case CR_ICCARD_FUELLING_PROC:

			// Process IC Card Fuelling Is On
			ICCard_Fuelling_Proc();

		break;

		case CR_ICCARD_EOT_PROC:

			// Process IC Card Fuelling Is End, Unlock The Card, And Form Record
			ICCard_EOT_Proc();

		break;

		case CR_TRANS_JOURNAL:

			// Process Form Jounal
			ICCard_Journal_Proc();

		break;

		case CR_ICCARD_EXIST_EXTERNAL_PROC:

			// Process External Request And Response External Process, While IC Card Exist
			ICCard_Exist_External_Proc();

		break;

		case CR_ICCARD_NONEXIST_EXTERNAL_PROC:

			// Process External Request And Response External Process, While IC Card Does Not Exist
			ICCard_NonExist_External_Proc();

		break;

		case CR_ICCARD_BUSY_EXTERNAL_PROC:

			// Process External Request And Response External Process, While IC Card Does Not Exist
			ICCard_Busy_External_Proc();

		break;

		case CR_LOGON_PROC:

			// IC Card Employee LogOn Process
			ICCard_LogOn_Proc();

		break;

		case CR_LOGOFF_PROC:

			// IC Card Employee LogOff Process
			ICCard_LogOff_Proc();

		break;


		case CR_OFFL_TRANS_PROC:

			// IC Card Offline Trans Process
			ICCard_Offl_Trans_Proc();

		break;


		case CR_PRICE_RECV:

			// IC Card Price Receiving Process
			ICCard_Price_Recv_Proc();

		break;

		case CR_CALC_MAC_PROC:

			// IC Card Waiting PSAM Calc MAC Or Encrypt
			PSAM_Calc_MAC();

		break;

#ifdef MAKE_CARD_MODE2

		case CR_SEND_CARD_RECORD_PROC:

			// IC Card Send Log To Host
			Card_Send_Log();

		break;

#endif	// MAKE_CARD_MODE2

#ifdef MAKE_PSAM_VERIFY

		case CR_ACT_CARD_PROC:
			
			// IC Card ACT Card Process
			ICCard_ACT_Proc();

		break;

		case CR_RID_CARD_PROC:
			
			// IC Card RID Card Process
			ICCard_RID_Proc();

		break;

#endif

		default:

			// Display: "IC Card Process Error"
//			while( 1 )
//			{
//			}

		break;
	}
}





/*
 ** InitCardReaderProcess
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CardReader.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Initialize Parameters For Card Reader Process
 *				 Called Before The Main loop
 *
 *  RETURNS: None.
 *
 */
void InitCardReaderProcess( void )
{
#ifdef MAKE_CARD_MODE2
	unsigned short int retval;
#endif

	// Default Start Command: 
	CR_CommandStatus = CR_CommandSending;
	CR_CommandType = CR_CARDREADER_RESET;
//	CR_Proc_Step = CR_CARDREADER_PROC;
	CR_Proc_Step = CR_CRPOWERON_PROC;



//	CreateTimer( TIMER_ONE_DELETE, TIMER_10MS * 74, CR_TestTime );

	// Clear TRINP
	memset( &TRINP, 0x00, sizeof( TRINP ));
        
        
        //20120311
        //Cetificate psam retry times limeted
        if(PowerDown.PDCertifyPSAMRetryTimes>3)
        {
                PowerDown.PDCertifyPSAMRetryTimes=0; 
        }            
        //20120311
        

	// Get The Log On Information
	if(( PowerDown.LogOnCard.EMPNumber != 0xFF ) && ( PowerDown.LogOnCard.EMPNumber != 0x00 ))
	{
		ProcStatus[PROCESS_CR] |= CARD_LOGON;
	}

	if(( PowerDown.PdTrans.PD_TRANS_FLAG > SET_GREY ) && ( PowerDown.PdTrans.PD_TRANS_FLAG < TRANS_END ))
	{
		// A Trans Did Not Completed
		IdleFlg |= ReCallTTC;
	}

	// For Transaction
	TRINP.TR_TRANS_EMP_NUMBER = PowerDown.LogOnCard.EMPNumber;


	// Read Version To PowerDown For Log File
	memcpy( PowerDown.PdVersion, DISPMSG[M_VersionNumber_New], strlen( DISPMSG[M_VersionNumber_New] ));	//20131021 qiyong modify

	// Set The PowerFlag
	CRPowerFlag = 0;

	// For Display
//	DISP_CR_Buf.EMPNumber = PowerDown.LogOnCard.EMPNumber;

#ifdef MAKE_PSAM_VERIFY
	// Set The Power On Flag
	CRPowerFlag |= CRPOWERON;
#endif


#ifdef MAKE_CASH_CR
	CRType = CR_TYPE_CARD;
#endif

#ifdef MAKE_CARD_MODE2
	// Special For Test In SHYK, For S/D
	// Get Logical NozzleNumber
	POS_CR_Buf.TransNozzleNumber = GetLogicNzn( 0 );
								
	// Get Price
	retval = GetPrice( 0, 0 );
	ShortIntToCharCopy( retval, POS_CR_Buf.TransPrice );

	// Check To Set Default Online Mode
	if( PowerDown.WorkMode == 0xFF )
	{
		// Formatted, Default On Line
		SetWorkMode( 0x01 );
	}

#endif

}



// For Test
/*
 ** CR_TestTime
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CardReader.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Only For Test The Running Timer
 *
 *  RETURNS: None.
 *
 */
//void CR_TestTime( void )
//{
//    TestTimeOut = 1;
//}
// For Test End


/*
 ** CR_PowerDown
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CardReader.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check And Set CR Process Can Or Can Not Be PowerDown
 *
 *  RETURNS: 0: Can Not Be PowerDown, Process The Card Reader
 *			 1: Can Be PowerDown, Do Not Issue Any Card Reader Command, And Set The PowerDown Bit
 *
 */
unsigned short int CR_PowerDown( void )
{
	unsigned short retval;

	// Default
	retval = 0;

	if( ProcExchange[PROCESS_POWERDOWN][PROCESS_CR] & POWERDOWN )
	{
		// Check IC Card Ready Or Not
		if( ProcStatus[PROCESS_CR] & CARD_READY )
		{
			if( CR_CommandStatus == CR_CommandSending )
			{
				// Check Power On Debit Or Not
//				if( PowerDown.SpecialFunction & DebitOnPowerOn )
				if(( PowerDown.SpecialFunction & DebitOnPowerOn )
#ifdef MAKE_CASH_CR
					 && ( CRType != CR_TYPE_CASH )
#endif
				)

				{
					// Power On Debit, So The Power Down Can Be Issued Next
					// Clear The Bit
					ProcExchange[PROCESS_POWERDOWN][PROCESS_CR] &= ( ~POWERDOWN );
			
					// Set The Power DOWN Flag
					CRPowerFlag |= CRPOWERDOWN;
				
					// For Write Spring Card Log
					PowerDownFlag = Pd_Flag_1;

					// Next, Spring Out Card
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
				
					retval = 1;
				}
				else
				{
					// Debit On Power Down, So Check The Card Busy Or Not
					// And Issue The Power Down Accordingly
					if( ProcStatus[PROCESS_CR] & CARD_BUSY )
					{
						if( !( CRPowerFlag & CRPUMPSTOP ))
						{
							// Card Busy, Issue Pump Stop
							CRPowerFlag |= CRPUMPSTOP;
							ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_STOP;
						}
						else
						{
							// Do Nothing, Just Waiting Card Not Busy
						}

						// Card Busy, Do Not Issue Power Down
						retval = 0;
					}
					else
					{
#ifdef MAKE_CASH_CR
						if( CRType == CR_TYPE_CASH )
						{
							// For Cash Trans Do Not Issue Power Down Here
							// Issue Until Journal Over
							// To Wait Send Cash Trans Record To Auth POS
							if( CRPowerFlag & CRPUMPSTOP )
							{
								// From IC Card Busy, Issue Power Down After Journal End And Send Record To CR-E
								retval = 0;
							}
							else
							{
								// Authed Or Accepted Cash, But Not Nozzle On, So Just Send Zero Recore To CR-E
								// A Cash Trans Reserved, Maybe Just Power On
								// Or Cash Received But Fail To Nozzle On etc....
								// So, Forbidden A Cash Trans Beginning....
								GetCashAmount();

								retval = 0;
							}
						}
						else
#endif
						{
							// Card Not Busy, 
							// Power On Debit, So The Power Down Can Be Issued Next
							// Clear The Bit
							ProcExchange[PROCESS_POWERDOWN][PROCESS_CR] &= ( ~POWERDOWN );
				
							// Set The Power DOWN Flag
							CRPowerFlag |= CRPOWERDOWN;
					
							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_2;

							// Next, Spring Out Card
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					
							retval = 1;
						}
					}
				}
			}
			else
			{
				// Command Is Running, Do Not Power Down
				retval = 0;
			}
		}
		//Douzhq 20140730 Add , Lock Nozzle
		else if (ProcExchange[PROCESS_CR][PROCESS_CR] & UNLOCK_POWERWD)
		{
			ProcExchange[PROCESS_CR][PROCESS_CR] &= ~(UNLOCK_POWERWD);
			
			if( PowerDown.SpecialFunction & DebitOnPowerOn )
			{
				ProcExchange[PROCESS_POWERDOWN][PROCESS_CR] &= ( ~POWERDOWN );
			
					// Set The Power DOWN Flag
					CRPowerFlag |= CRPOWERDOWN;
				
					// For Write Spring Card Log
					PowerDownFlag = Pd_Flag_1;

					// Next, Spring Out Card
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
				
					retval = 1;
			}
		}
		else
		{
			// Check Wether The POWERDOWN Should Be Issued By PowerOff Or SpringOut	Card
//			if( CRPowerFlag != 0 )
			if( CRPowerFlag & CRPUMPSTOP )
			{
				// Power Down Should Be Issued By PowerOff Or Spring Out Card,
				// The CARD_READY May Be Clear By Power Off Command,
				// So, Do Not Issued Power Down Here
				// Set The Power DOWN Flag
				CRPowerFlag |= CRPOWERDOWN;
				
				retval = 0;
			}
			else if( CRPowerFlag & CRPOWERDOWN )
			{
				retval = 0;
			}
			else
			{
				// No IC Card, Just Power Down
				// Clear The Bit
				ProcExchange[PROCESS_POWERDOWN][PROCESS_CR] &= ( ~POWERDOWN );
			
				// Inform Power Down Process
				ProcExchange[PROCESS_CR][PROCESS_POWERDOWN] |= POWERDOWN; 

				retval = 1;
			}
		}
	}
	else
	{
		// No Power Down Issued
		retval = 1;
	}

	return retval;
}


unsigned char CR_Check_Err( void )
{
	unsigned char retval;

	// Default, No Error
	retval = 0;

	if( CommErrorTimes >= CR_COMM_ERR_MAX_TIMES )
	{
		// Allow Power Off And Spring Out Card
		if(( CR_CommandType == CR_CARDREADER_POWEROFF_CARD ) || ( CR_CommandType == CR_CARDREADER_SPINGOUT_CARD ))
		{
			// Power Off IC Card And Spring Out
			retval = 0;
		}
		else
		{
			// Error Times Exceed, Do Not Enter
			// Mark Card Reader Error
			ProcStatus[PROCESS_CR] |= CARD_ERROR;

			// And Wait Key Press Clear
			retval = 1;
		}
	}
	else if( SpringOutCardTimes > SPRINGOUTTIMES )
	{
		// Mark Card Reader Error
		ProcStatus[PROCESS_CR] |= CARD_ERROR;
		retval = 1;
	}
    else if( GetGMACRetryTimes > GETGMACRETRYTIMES )
    {
        // Mark Card Reader Error
		//ProcStatus[PROCESS_CR] |= CARD_ERROR;
        FillInfoMsg(M_GetGMACErr, 0, 0);
		retval = 1;
    }
    else if( SaveRecordFailFlag > 1 )
    {
        FillInfoMsg(M_WriteRecordError, 0, 0);
		retval = 1;
    }
	else
	{
		retval = 0;
	}

	return retval;
}
