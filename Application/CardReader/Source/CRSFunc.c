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
#include "CardReady.h"
#include "ExCRProc.h"
#include "CRPumpEOT.h"
#include "CRSFunc.h"
#include "Message.h"
#include "Screen.h"
#include "WorkMode.h"
#include "WriteLog.h"

//Douzhq 20140930 Add , Config Printer Set
#include "SocialCard.h"


// IC Card Employee LogOn Process
/*
 ** ICCard_LogOn_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Process For IC Card Employee Log On
 *
 *  RETURNS: None.
 *
 */
void ICCard_LogOn_Proc( void )
{
	unsigned short int retval;
	
	// Only Running When CR_Proc_Step = CR_LOGON_PROC
	if( CR_Proc_Step == CR_LOGON_PROC )
	{
		switch( CR_CommandType )
		{
			case CR_WAITING_EMP_PASSWORD:

				// Get Employee Password
				retval = GetEmpPWD();
				
				if( retval != 1 )
				{
					//
					if( retval == 0x9000 )
					{
						if( memcmp( KEYPAD_CR_Buf.ICCardEMPPwd, TRINP.TR_CARD_EMP_PASSWORD, sizeof( TRINP.TR_CARD_EMP_PASSWORD )) == 0 )
						{
							//Douzhq 20140930 Add , Config Printer Set
							// Clear PrintFlag
							PrintFlag = 0;
							
							// Display " Please Waiting "
							FillInfoMsg( M_Wait, 0, 0 );

							// Clear Input,
							KEYPAD_CR_Buf.ICCardEMPPwdLen = 0;
							memset( KEYPAD_CR_Buf.ICCardEMPPwd, 0x00, sizeof( KEYPAD_CR_Buf.ICCardEMPPwd ));

							Set_Next_Command( CR_LOGON_PROC, CR_PUMP_TOTALS );
						}
						else
						{
							// Display: "EMP Password Error, Retry....."
							FillInfoMsg( M_EMPPwdRetries, 0, 0 );

							Set_Next_Command( CR_LOGON_PROC, CR_WAITING_EMP_PASSWORD );
						}
					}
					else
					{
						// Input Error, Check Card In Card Ready Proc
						Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );

//						// Next, Check IC Card
//						Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;
			
			case CR_PUMP_TOTALS:

				// Get Pump Totals
				retval = GetPumpTotals();

				if( retval != 1 )
				{
					//
					if( retval == 0x9000 )
					{
						// Save The Volume Totals
						SaveTotals();

#ifdef MAKE_CARD_MODE2
						if( GetWorkMode() == 0x01 )
						{
							TRINP.TR_TRANS_TYPE &= ( ~( LOCALLIST | WORKOFFLINE ));
							PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
						}
#endif	// MAKE_CARD_MODE2

						SaveLogOnTotals();

						PowerDown.LogOnCard.LogOnFlag = 0;
						PowerDown.LogOnCard.EMPNumber = TRINP.TR_CARD_EMP_NUMBER;
						memcpy( PowerDown.LogOnCard.EMPPassword, TRINP.TR_CARD_EMP_PASSWORD, sizeof( PowerDown.LogOnCard.EMPPassword ));
				
						PowerDown.LogOnCard.EMPPinLen = TRINP.TR_INPUT_PIN_LEN;
						memcpy( PowerDown.LogOnCard.EMPPin, TRINP.TR_INPUT_PIN, sizeof( PowerDown.LogOnCard.EMPPin ));
						memcpy( PowerDown.LogOnCard.EMPCardASN, TRINP.TR_CARD_ASN, sizeof( PowerDown.LogOnCard.EMPCardASN ));
						memcpy( PowerDown.LogOnCard.EMPCardBalance, TRINP.TR_CARD_APP_BALANCE, sizeof( PowerDown.LogOnCard.EMPCardBalance ));
					
						// Begin Check Nozzle Next
						// Get Total Nozzle Numbers First
						PowerDown.NozzleNumbers = GetGUN_N();
						PowerDown.CurrentNozzleNumber = 0;

						// Set The Log On Trans Begin
						PowerDown.PdTrans.PD_MULTI_TRANS_FLAG =	LOGON_JOURNAL;
						
						// Write Log
						WriteTotalLog( 1 );

						// Save To DataFlash
						SaveCRConfigFlash();

						// Need Employee Password Next
						Set_Next_Command( CR_LOGON_PROC, CR_CHECK_NOZZLES );
					}
					else
					{
						// Display: "Get Total Fail, Can Not Go On...."
						FillInfoMsg( M_ReadPumpTotalFail, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_22;
                        ErrorFlag = 0xE006;

						// Error, Exit
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			
			case CR_CHECK_NOZZLES:

				// Fill The Elements For Journal
				if( PowerDown.CurrentNozzleNumber < PowerDown.NozzleNumbers )
				{
					// Save Totals For Display
					SaveDispTotal( PowerDown.CurrentNozzleNumber );

					GetSFuncData( 0 );

					// For Test
					// Save Log On Information
//					if( SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All )) != 0 )
//					{
//						while( 1 )
//						{
//						};
//					}

					// For Test
					// Logon Trans
					TRINP.TR_TRANS_TYPE &= 0xF0;
					TRINP.TR_TRANS_TYPE |= TRANSLOGON;

					PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

					// Save To DataFlash
					SaveCRConfigFlash();

					// For Write The Running Log
					JournalFlag = 6;

					// Form Journal Next
					Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
					
					//
				}
				else
				{
					// Journal End, Save The Logon Information
					PowerDown.LogOnCard.LogOnFlag = 1;
//					PowerDown.LogOnCard.EMPNumber = PowerDown.PdTrans.PD_EMPLOYEE_NUMBER;
//					memcpy( PowerDown.LogOnCard.EMPPassword, TRINP.TR_CARD_EMP_PASSWORD, sizeof( PowerDown.LogOnCard.EMPPassword ));
					
					
//					PowerDown.LogOnCard.EMPPinLen = TRINP.TR_INPUT_PIN_LEN;
//					memcpy( PowerDown.LogOnCard.EMPPin, TRINP.TR_INPUT_PIN, sizeof( PowerDown.LogOnCard.EMPPin ));
//					memcpy( PowerDown.LogOnCard.EMPCardASN, TRINP.TR_CARD_ASN, sizeof( PowerDown.LogOnCard.EMPCardASN ));
					
					ProcStatus[PROCESS_CR] |= CARD_LOGON;

					// For Display
//					DISP_CR_Buf.EMPNumber = PowerDown.LogOnCard.EMPNumber;
					
					// Clear The Log On Trans Flag
					PowerDown.PdTrans.PD_MULTI_TRANS_FLAG =	MULTI_TRANS_START;

					// Save To DataFlash
					SaveCRConfigFlash();
						

					// Clear InfoLine, And Display default
					FillInfoMsg( M_DefaultInfo, 0, 0 );
					
#ifdef MAKE_CARD_MODE2
					// Did Not Authed Before, Check Work Mode Here
					TRINP.TR_WORK_MODE = GetWorkMode();
					TRINP.TR_AUTH_MODE = GetAuthMode();

#endif

					// Journal End, Check Card In Card Ready Proc
					Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );

//					// Next, Check IC Card
//					Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );

//					// Next, Check Card Reader
//					Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );


					// For Test
					// Save Log On Information
//					if( SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All )) != 0 )
//					{
//						while( 1 )
//						{
//						};
//					}

					// For Test

				}
							
			break;

			default:

			break;
		}
	}
}


// IC Card Employee LogOff Process
/*
 ** ICCard_LogOff_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Process For IC Card Employee Log Off
 *
 *  RETURNS: None.
 *
 */
void ICCard_LogOff_Proc( void )
{
	unsigned short int retval;
	
	// Only Running When CR_Proc_Step = CR_LOGOFF_PROC
	if( CR_Proc_Step == CR_LOGOFF_PROC )
	{
		switch( CR_CommandType )
		{
			case CR_WAITING_EMP_PASSWORD:

				if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
				{
					// Check Test Card
					// Test Card May Log Off

				}
				else
				{
//					// Check Same Emoloyee Card Or Not
//					if( memcmp( PowerDown.LogOnCard.EMPCardASN, TRINP.TR_CARD_ASN, sizeof( PowerDown.LogOnCard.EMPCardASN )) != 0 )
//					{
//						// Display " Not Same Employee Card"
//						FillInfoMsg( M_NotSameEMPCard, 0, 0 );
//
//						// For Write Spring Card Log
//						PowerDownFlag = Pd_Flag_23;
//
//						// Error, Exit
//						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
//				
//						// Return;
//						break;
//					}

					// Check Same Emoloyee Number Or Not
					if( PowerDown.LogOnCard.EMPNumber != TRINP.TR_CARD_EMP_NUMBER )
					{
						// Display " Not Same Employee Card"
						FillInfoMsg( M_NotSameEMP, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_23;
                        ErrorFlag = 0xE007;

						// Error, Exit
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
				
						// Return;
						break;
					}
				}

				// Same Employee Card
				// Get Employee Password
				retval = GetEmpPWD();
				
				if( retval != 1 )
				{
					//
					if( retval == 0x9000 )
					{
						if( memcmp( KEYPAD_CR_Buf.ICCardEMPPwd, TRINP.TR_CARD_EMP_PASSWORD, sizeof( TRINP.TR_CARD_EMP_PASSWORD )) == 0 )
						{
							//Douzhq 20140930 Add , Config Printer Set
							// Clear PrintFlag
							PrintFlag = 0;
							
							// Display " Please Waiting "
							FillInfoMsg( M_Wait, 0, 0 );

							// Clear Input,
							KEYPAD_CR_Buf.ICCardEMPPwdLen = 0;
							memset( KEYPAD_CR_Buf.ICCardEMPPwd, 0x00, sizeof( KEYPAD_CR_Buf.ICCardEMPPwd ));

							Set_Next_Command( CR_LOGOFF_PROC, CR_PUMP_TOTALS );
						}
						else
						{
							// Display: "EMP Password Error, Retry....."
							FillInfoMsg( M_EMPPwdRetries, 0, 0 );

							Set_Next_Command( CR_LOGOFF_PROC, CR_WAITING_EMP_PASSWORD );
						}
					}
					else
					{
						// Input Error, Check Card In Card Ready Proc
						Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;
			
			case CR_PUMP_TOTALS:

				// Get Pump Totals
				retval = GetPumpTotals();

				if( retval != 1 )
				{
					//
					if( retval == 0x9000 )
					{
						
						// Save The Volume Totals
						SaveTotals();

#ifdef MAKE_CARD_MODE2
						if( GetWorkMode() == 0x01 )
						{
							TRINP.TR_TRANS_TYPE &= ( ~( LOCALLIST | WORKOFFLINE ));
							PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
						}
#endif	// MAKE_CARD_MODE2

						// Begin Check Nozzle Next
						// Get Total Nozzle Numbers First
						PowerDown.NozzleNumbers = GetGUN_N();
						PowerDown.CurrentNozzleNumber = 0;

						// Set The Log Off Trans Flag
						PowerDown.PdTrans.PD_MULTI_TRANS_FLAG =	LOGOFF_JOURNAL;

						// Save To DataFlash
						SaveCRConfigFlash();
						
						// Write Log
						WriteTotalLog( 2 );

						// Need Employee Password Next
						Set_Next_Command( CR_LOGOFF_PROC, CR_CHECK_NOZZLES );
					}
					else
					{
						// Display: "Get Total Fail, Can Not Go On...."
						FillInfoMsg( M_ReadPumpTotalFail, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_24;
                        ErrorFlag = 0xE008;

						// Error, Exit
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			
			case CR_CHECK_NOZZLES:

				// Fill The Elements For Journal
				if( PowerDown.CurrentNozzleNumber < PowerDown.NozzleNumbers )
				{
					GetSFuncData( 1 );

					// Logon Trans
					TRINP.TR_TRANS_TYPE &= 0xF0;
					TRINP.TR_TRANS_TYPE |= TRANSLOGOFF;

					PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

					// Save To DataFlash
					SaveCRConfigFlash();

					// For Write The Running Log
					JournalFlag = 7;

					// Form Journal Next
					Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
				}
				else
				{
					// Journal End, Save The Logoff Information
					memset( &PowerDown.LogOnCard, 0xFF, sizeof( LOG_ON_CARD ));
					
					ProcStatus[PROCESS_CR] &= ( ~CARD_LOGON );

					
					// For Display
//					DISP_CR_Buf.EMPNumber = PowerDown.LogOnCard.EMPNumber;
					
					// Clear The Log Off Trans Flag
					PowerDown.PdTrans.PD_MULTI_TRANS_FLAG =	MULTI_TRANS_START;

					// Save To DataFlash
					SaveCRConfigFlash();
						
					// Next Display Totals
					NextScreenID = SCR_VOLUMETOTAL;
					LastScreenID = DEFAULT_SCR_CARD;

					// Clear InfoLine, And Display default
					FillInfoMsg( M_DefaultInfo, 0, 0 );

					// Journal End, Spring Out Card
					HoldCard = 0;

					// For Write Spring Card Log
					PowerDownFlag = Pd_Flag_25;
                    ErrorFlag = 0x9000;

					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );

					//
					// For Test
					// Save Log On Information
//					if( SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All )) != 0 )
//					{
//						while( 1 )
//						{
//						};
//					}

					// For Test

				}
							
			break;

			default:

			break;
		}
	}
}

// IC Card Offline Trans Process
/*
 ** ICCard_Offl_Trans_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Process For IC Card Offline Transactions
 *
 *  RETURNS: None.
 *
 */
void ICCard_Offl_Trans_Proc( void )
{
//	unsigned short int retval;
	
	// Only Running When CR_Proc_Step = CR_OFFL_TRANS_PROC
	if( CR_Proc_Step == CR_OFFL_TRANS_PROC )
	{
		switch( CR_CommandType )
		{
			case CR_OFFLINE_TRANS:

				// Save Offline Trans Datas To Power Down Protect
				SaveTotals();

				// Save Log
				WriteOfflineLog();
				
				// Copy The Log On Emp Card
				TRINP.TR_CARD_EMP_NUMBER = PowerDown.LogOnCard.EMPNumber;
				memcpy( TRINP.TR_CARD_ASN, PowerDown.LogOnCard.EMPCardASN, sizeof( TRINP.TR_CARD_ASN ));

				// Begin Check Nozzle Next
				// Get Total Nozzle Numbers First
				PowerDown.NozzleNumbers = GetGUN_N();
				PowerDown.CurrentNozzleNumber = 0;

				// Set The Offline Trans Flag
				PowerDown.PdTrans.PD_MULTI_TRANS_FLAG =	NON_ICCARD_JOURNAL;

				// Save To DataFlash
				SaveCRConfigFlash();
						
				Set_Next_Command( CR_OFFL_TRANS_PROC, CR_CHECK_NOZZLES );
	
			break;

			case CR_CHECK_NOZZLES:

				// Get The Offline Trans
				if( GetOfflineData() == 1 )
				{
					// Form Offline Trans Journal
					// Fill The Elements For Journal
					if( PowerDown.CurrentNozzleNumber < PowerDown.NozzleNumbers )
					{
						GetSFuncData( 2 );

						// Offline Trans
						TRINP.TR_TRANS_TYPE &= 0x00;
						TRINP.TR_TRANS_TYPE |= TRANSNONCARD;

#ifdef MAKE_CARD_MODE2
						TRINP.TR_TRANS_TYPE |= ( WORKOFFLINE | LOCALLIST );
#endif

						PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

						// Save To DataFlash
						SaveCRConfigFlash();

						// For Write The Running Log
						JournalFlag = 8;

						// Form Journal Next
						Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
					}
					else
					{
						// Clear The Offline Trans Flag
						PowerDown.PdTrans.PD_MULTI_TRANS_FLAG =	MULTI_TRANS_START;

						// Clear InfoLine, And Display default
						FillInfoMsg( M_DefaultInfo, 0, 0 );

						// Save To DataFlash
						SaveCRConfigFlash();
						
						// Journal End, Check IC Card
						Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
					}
				}
				else
				{
					PowerDown.CurrentNozzleNumber++;

					// Check The Next NozzleNumber
					if( PowerDown.CurrentNozzleNumber < PowerDown.NozzleNumbers )
					{
						Set_Next_Command( CR_OFFL_TRANS_PROC, CR_CHECK_NOZZLES );
					}
					else
					{
						// Clear The Offline Trans Flag
						PowerDown.PdTrans.PD_MULTI_TRANS_FLAG =	MULTI_TRANS_START;

						// Clear InfoLine, And Display default
						FillInfoMsg( M_DefaultInfo, 0, 0 );
						
						// Journal End, Check IC Card
						Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
					}

					// Save To DataFlash
					SaveCRConfigFlash();
				}

							
			break;

			default:
			break;
		}
	}
}

// IC Card Price Receiving Process
/*
 ** ICCard_Price_Recv_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Process For IC Card Price Receiving
 *
 *  RETURNS: None.
 *
 */
void ICCard_Price_Recv_Proc( void )
{
//	unsigned short int retval;
	
	// Only Running When CR_Proc_Step = CR_PRICE_RECV
	if( CR_Proc_Step == CR_PRICE_RECV )
	{
		switch( CR_CommandType )
		{
			case CR_PRICE_RECORD:
				
				//Douzhq 20140930 Add , Config Printer Set
				// Clear PrintFlag
				PrintFlag = 0;
							
				// Begin Check Nozzle Next
				// Get Total Nozzle Numbers First
				PowerDown.NozzleNumbers = GetGUN_N();
				PowerDown.CurrentNozzleNumber = 0;

				// Set The Price Recv Trans Flag
				PowerDown.PdTrans.PD_MULTI_TRANS_FLAG =	PRICE_RECV_JOURNAL;
						
				// Clear The Flag In Protected
				ClrPriceReceviceFlg();

				// Save To DataFlash
				SaveCRConfigFlash();

				Set_Next_Command( CR_PRICE_RECV, CR_CHECK_NOZZLES );

			break;
			
			
			case CR_CHECK_NOZZLES:

				// Fill The Elements For Journal
				if( PowerDown.CurrentNozzleNumber < PowerDown.NozzleNumbers )
				{
					GetSFuncData( 3 );

					// Price Recv Trans
//					TRINP.TR_TRANS_TYPE &= 0xF0;
//					TRINP.TR_TRANS_TYPE |= TRANPRICERECV;
					TRINP.TR_TRANS_TYPE = TRANPRICERECV;

					PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

					// Save To DataFlash
					SaveCRConfigFlash();

					// For Write The Running Log
					JournalFlag = 9;

					// Form Journal Next
					Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
				}
				else
				{
					// Clear The Price Recv Trans Flag
					PowerDown.PdTrans.PD_MULTI_TRANS_FLAG =	MULTI_TRANS_START;

					// Save To DataFlash
					SaveCRConfigFlash();
						
//					// Journal End, Check IC Card
//					Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );

					// Journal End, Check Card In Card Ready Proc
					Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
				}
							
			break;

			default:

			break;
		}
	}
}


/*
 ** GetPumpTotals
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get Pump Total ( All Grades, 4 At Most )
 *
 *  RETURNS: 1: Running, 0x9000: Total Get And Saved In Pump_CR_Buf.VolumeTotal And Pump_CR_Buf.AmountTotal
 *			 0: Get Total Error
 *
 */
unsigned short int GetPumpTotals( void )
{
	unsigned short int retval;

	// Default Running
	retval = 1;

	if( !( CR_Task_Flag & ICCARD_PUMP_TOTALS ))
	{
		// Check Pump Online Or Not
		if( ProcStatus[PROCESS_PUMP] & PUMP_ONLINE )
		{
			// Set Bit, Local Process Flag
			CR_Task_Flag |= ICCARD_PUMP_TOTALS;

			// Display: " Inform PUMP Need  Totals "
			// Set Bit, To Inform Pump Totals
			ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_TOTALS;
		}
		else
		{
			retval = 0;
		}
	}
	else
	{
		// Wait For Last Trans
		if( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_TOTALS )
		{
			// PUMP Load Last Trans Ended
			// Clear The Bit
			ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_TOTALS );
					
			// Clear Local Process Flag
			CR_Task_Flag &= ( ~ICCARD_PUMP_TOTALS );
			
			retval = 0x9000;
		}
		else
		{
			// Command Running, Go On ....
		}
	}

	return retval;
}


/*
 ** GetEmpPWD
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Input Employee Password
 *
 *  RETURNS: 1: Running, 0x9000: Total Get And Saved In KEYPAD_CR_Buf.ICCardEMPPwd
 *			 0: Get Total Error
 *
 */
unsigned short int GetEmpPWD( void )
{
	unsigned short int retval;

	// Default Running
	retval = 1;

	// First Set Get Employee Password Flag In ProcExchange[PROCESS_CR][PROCESS_KEYPAD];
	if( !( CR_Task_Flag & ICCARD_EMP_PWD_K ))
	{
		// Set Bit, Local Process Flag
		CR_Task_Flag |= ICCARD_EMP_PWD_K;

		// Set Bit, To Inform Keypad Input Employee Password
		ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= EMP_PASSWORD;
	}
	else
	{				
		// Second Check The Input, By Check ProcExchange[PROCESS_KEYPAD][PROCESS_CR]
		if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & EMP_PASSWORD )
		{
			// Password Inputed By Keypad
			// Clear The Bit
			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~EMP_PASSWORD );
			
			// Clear Local Process Flag
			CR_Task_Flag &= ( ~ICCARD_EMP_PWD_K );

			// Check Input Ok Or Cancel
			if( KEYPAD_CR_Buf.ICCardEMPPwdLen != 0 )
			{
				retval = 0x9000;
			}
			else
			{
				// Input Canceled
				retval = 0;
			}
		}
		else
		{
			// Wait Input, Go On
		}
	}

	return retval;
}



/*
 ** GetSFuncData
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	type: 0: Log On; 1: Log Off; 2 Offline Trans; 3 Price Update
 *
 *  DESCRIPTION: Get The Special Function Datas For Journal, And Save To PowerDown.PdTrans
 *
 *  RETURNS: None.
 *
 */
void GetSFuncData( unsigned char type )
{
	unsigned short int gradecode;

#ifdef MAKE_CARD_MODE2
	unsigned short int price;
#endif

	// Get Logical Nozzle Number
	TRINP.TR_TRANS_NOZZLE_NUMBER = GetLogicNzn( PowerDown.CurrentNozzleNumber );
	PowerDown.PdTrans.PD_NOZZLENUMBER = TRINP.TR_TRANS_NOZZLE_NUMBER;
					
	if(( type == 0 ) || ( type == 1 ) || ( type == 2 ))
	{
		// Maybe Resume From Power Down, Resume LogOn Information
		TRINP.TR_CARD_EMP_NUMBER = PowerDown.LogOnCard.EMPNumber;
		memcpy( TRINP.TR_CARD_EMP_PASSWORD, PowerDown.LogOnCard.EMPPassword, sizeof( PowerDown.LogOnCard.EMPPassword ));
		TRINP.TR_INPUT_PIN_LEN = PowerDown.LogOnCard.EMPPinLen;
		memcpy( TRINP.TR_INPUT_PIN, PowerDown.LogOnCard.EMPPin, sizeof( PowerDown.LogOnCard.EMPPin ));
		memcpy( TRINP.TR_CARD_ASN, PowerDown.LogOnCard.EMPCardASN, sizeof( PowerDown.LogOnCard.EMPCardASN ));
		memcpy( PowerDown.PdTrans.PD_CARD_ASN, TRINP.TR_CARD_ASN, sizeof( PowerDown.PdTrans.PD_CARD_ASN ));
	}
	else
	{
		// type == 3
		// Price Update Transaction, Do Not Resume The Datas
		// For All The Data Will Be Delete In Journal Proc
		// Just Go On....
        
        // Get CARD_EMP_NUMBER From LogOnCard While ' Price Recv '
        // Otherwise, CARD_EMP_NUMBER Will Change To Current IC Card's CARD_EMP_NUMBER
        // Instead Of Logged On IC Card's CARD_EMP_NUMBER
        TRINP.TR_CARD_EMP_NUMBER = PowerDown.LogOnCard.EMPNumber;
	}
					
	// Get Employee Number
	TRINP.TR_TRANS_EMP_NUMBER = TRINP.TR_CARD_EMP_NUMBER;
	PowerDown.PdTrans.PD_EMPLOYEE_NUMBER = TRINP.TR_TRANS_EMP_NUMBER;
					
	// Get Grade Code
	gradecode = GetO_Type( PowerDown.CurrentNozzleNumber );
	ShortIntToCharCopy( gradecode, TRINP.TR_TRANS_GRADE_CODE );
	memcpy( PowerDown.PdTrans.PD_GRADE_CODE, TRINP.TR_TRANS_GRADE_CODE, sizeof( PowerDown.PdTrans.PD_GRADE_CODE ));
					
	if( type == 0 )
	{
		// For Log On Trans, Save The Grade Code
		memcpy( PowerDown.LogOnCard.EMPGradeCode[PowerDown.CurrentNozzleNumber], TRINP.TR_TRANS_GRADE_CODE, sizeof( TRINP.TR_TRANS_GRADE_CODE ));
	}
	else if( type == 1 )
	{
		// For Log Off Trans, Use The Log On Grade Code Instead Of The Current Grade Code
		// Grade Code Be Changed Maybe
		memcpy( TRINP.TR_TRANS_GRADE_CODE, PowerDown.LogOnCard.EMPGradeCode[PowerDown.CurrentNozzleNumber], sizeof( TRINP.TR_TRANS_GRADE_CODE ));
		memcpy( PowerDown.PdTrans.PD_GRADE_CODE, TRINP.TR_TRANS_GRADE_CODE, sizeof( PowerDown.PdTrans.PD_GRADE_CODE ));
	}
	else
	{
		// Do Nothing
	}

#ifdef MAKE_CARD_MODE2
	// Get Price
	price = GetPrice( PowerDown.CurrentNozzleNumber, 0 );
	ShortIntToCharCopy( price, TRINP.TR_TRANS_PRICE );
	memcpy( PowerDown.PdTrans.PD_TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( PowerDown.PdTrans.PD_TRANS_PRICE ));
#endif

	// Get Pump Totals
	memcpy( TRINP.TR_TRANS_VOLUME_TOTAL, &PowerDown.PdTrans.PD_VOLUME_TOTALS[4*PowerDown.CurrentNozzleNumber], sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));
	memcpy( PowerDown.PdTrans.PD_TRANS_VOLUME_TOTAL, TRINP.TR_TRANS_VOLUME_TOTAL, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));
					
//	// Save Totals To Display Buffer: NozzleNumber + Volume Totals
//	DISP_CR_Buf.VolumeTotals[PowerDown.CurrentNozzleNumber * 5] = TRINP.TR_TRANS_NOZZLE_NUMBER;
//	memcpy( &DISP_CR_Buf.VolumeTotals[( PowerDown.CurrentNozzleNumber * 5 ) + 1], TRINP.TR_TRANS_VOLUME_TOTAL, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));

	// Get TTC
	GetTransTTC();
	// Save To PowerDown.PdTrans
	memcpy( PowerDown.PdTrans.PD_TRANS_TTC, TRINP.TR_TRANS_TTC, sizeof( PowerDown.PdTrans.PD_TRANS_TTC ));

	// Point To Next Nozzle
	PowerDown.CurrentNozzleNumber++;

	// Get Trans Time
	GetTransTime();
	// Save To PowerDown.PdTrans
	memcpy( PowerDown.PdTrans.PD_TRANS_DATE, TRINP.TR_TRANS_DATE, sizeof( PowerDown.PdTrans.PD_TRANS_DATE ));
	memcpy( PowerDown.PdTrans.PD_TRANS_TIME, TRINP.TR_TRANS_TIME, sizeof( PowerDown.PdTrans.PD_TRANS_TIME ));
}



/*
 ** GetOfflineData
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get Offline Data From PowerDown.PdTrans, And Check Wether A Offline Journal Needed
 *
 *  RETURNS: 0: Do Not Form Offline Trans Journal; 1: Form A Offline Trans Journal.
 *
 */
unsigned char GetOfflineData( void )
{
	unsigned char retval;
	unsigned int volume, amount;
	unsigned short int price;

	// Default Do Not Form Offline Trans Journal
	retval = 0;

	if( PowerDown.CurrentNozzleNumber < PowerDown.NozzleNumbers )
	{
		// Get Offline Trans
		volume = CharToIntCopy( &PowerDown.PdTrans.PD_OFFL_TRANS_VOLUME[4*PowerDown.CurrentNozzleNumber] );
	}
	else
	{
		volume = 0;
	}

	if( volume != 0 )
	{
		// Need A Offline Trans Journal
		memcpy( TRINP.TR_TRANS_VOLUME, &PowerDown.PdTrans.PD_OFFL_TRANS_VOLUME[4*PowerDown.CurrentNozzleNumber], sizeof( TRINP.TR_TRANS_VOLUME ));
		memcpy( TRINP.TR_TRANS_AMOUNT, &PowerDown.PdTrans.PD_OFFL_TRANS_AMOUNT[4*PowerDown.CurrentNozzleNumber], sizeof( TRINP.TR_TRANS_AMOUNT ));

		// Calc The Price
		amount = CharToIntCopy( &PowerDown.PdTrans.PD_OFFL_TRANS_AMOUNT[4*PowerDown.CurrentNozzleNumber] );
	
		// Calc Price
		price = (( amount * 100 ) / volume );
//		if((( amount * 100 ) / 2 ) >= volume )
		if((( amount * 100 ) % volume ) >= ( volume >> 1 ))
		{
			price++;
		}
		
		// Save Price
		ShortIntToCharCopy( price, TRINP.TR_TRANS_PRICE );
	
		// Save To Power Down
		memcpy( PowerDown.PdTrans.PD_TRANS_VOLUME, TRINP.TR_TRANS_VOLUME, sizeof( PowerDown.PdTrans.PD_TRANS_VOLUME ));
		memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT, TRINP.TR_TRANS_AMOUNT, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT ));
		memcpy( PowerDown.PdTrans.PD_TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( PowerDown.PdTrans.PD_TRANS_PRICE ));
	
		retval = 1;
	}
	
	return retval;
}


/*
 ** SaveTotals
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save The Multi-Nozzles Volume And Amount Totals To PowerDown
 *				 For Log On, Log Off, Offline Trans, And Price Recv
 *
 *  RETURNS: None.
 *
 */
void SaveTotals( void )
{
	unsigned char i;
	unsigned int value;

	// Save The Volume And Amount Totals
//	memcpy( PowerDown.PdTrans.PD_VOLUME_TOTALS, Pump_CR_Buf.VolumeTotal, sizeof( PowerDown.PdTrans.PD_VOLUME_TOTALS ));
//	memcpy( PowerDown.PdTrans.PD_AMOUNT_TOTALS, Pump_CR_Buf.AmountTotal, sizeof( PowerDown.PdTrans.PD_AMOUNT_TOTALS ));
	for( i = 0; i < 4; i++ )
	{
		// Save Volume Total As MSB
		memcpy( &value, &Pump_CR_Buf.VolumeTotal[4*i], 4 );
		IntToCharCopy( value, &PowerDown.PdTrans.PD_VOLUME_TOTALS[4*i] );
	
		// Save Amount Total As MSB
		memcpy( &value, &Pump_CR_Buf.AmountTotal[4*i], 4 );
		IntToCharCopy( value, &PowerDown.PdTrans.PD_AMOUNT_TOTALS[4*i] );
	}


//	// Save Offline Amount
//	memcpy( PowerDown.PdTrans.PD_OFFL_TRANS_AMOUNT, Pump_CR_Buf.OfflineAmount, sizeof( PowerDown.PdTrans.PD_OFFL_TRANS_AMOUNT ));
//	// Save Offline Volume
//	memcpy( PowerDown.PdTrans.PD_OFFL_TRANS_VOLUME, Pump_CR_Buf.OfflineVolume, sizeof( PowerDown.PdTrans.PD_OFFL_TRANS_VOLUME ));

	for( i = 0; i < 4; i++ )
	{
		// Save Offline Trans Volume Total As MSB
		memcpy( &value, &Pump_CR_Buf.OfflineVolume[4*i], 4 );
		IntToCharCopy( value, &PowerDown.PdTrans.PD_OFFL_TRANS_VOLUME[4*i] );

		// Save Offline Trans Amount Total As MSB
		memcpy( &value, &Pump_CR_Buf.OfflineAmount[4*i], 4 );
		IntToCharCopy( value, &PowerDown.PdTrans.PD_OFFL_TRANS_AMOUNT[4*i] );
	}

}

/*
 ** ReadTotals
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read The Totals From The PowerDown Buffer To Pump_CR_Buf
 *
 *  RETURNS: None.
 *
 */
void ReadTotals( void )
{
	unsigned char i;
	unsigned int value;

	// Read The Volume And Amount Totals
	for( i = 0; i < 4; i++ )
	{
		// Read Volume Total As MSB
		value = CharToIntCopy( &PowerDown.PdTrans.PD_VOLUME_TOTALS[4*i] );
		memcpy( &Pump_CR_Buf.VolumeTotal[4*i], &value, 4 );
	
		// Read Amount Total As MSB
		value = CharToIntCopy( &PowerDown.PdTrans.PD_AMOUNT_TOTALS[4*i] );
		memcpy( &Pump_CR_Buf.AmountTotal[4*i], &value, 4 );
	}


	// Save Offline Amount
	for( i = 0; i < 4; i++ )
	{
		// Save Offline Trans Volume Total As MSB
		value = CharToIntCopy( &PowerDown.PdTrans.PD_OFFL_TRANS_VOLUME[4*i] );
		memcpy( &Pump_CR_Buf.OfflineVolume[4*i], &value, 4 );

		// Save Offline Trans Amount Total As MSB
		value = CharToIntCopy( &PowerDown.PdTrans.PD_OFFL_TRANS_AMOUNT[4*i] );
		memcpy( &Pump_CR_Buf.OfflineAmount[4*i], &value, 4 );
	}
}

/*
 ** SaveLogOnTotals
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save The Log On Totals For Display
 *
 *  RETURNS: None.
 *
 */
void SaveLogOnTotals( void )
{
	// Save The Log On Totals
	memcpy( PowerDown.LogOnCard.LogOnVolumeTotals, PowerDown.PdTrans.PD_VOLUME_TOTALS, sizeof( PowerDown.LogOnCard.LogOnVolumeTotals ));
	memcpy( PowerDown.LogOnCard.LogOnAmountTotals, PowerDown.PdTrans.PD_AMOUNT_TOTALS, sizeof( PowerDown.LogOnCard.LogOnAmountTotals ));
}

/*
 ** SaveDispTotal
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	Phiscal Nozzle Number.
 *
 *  DESCRIPTION: Save The Log On Total To Display Total Buffer
 *
 *  RETURNS: None.
 *
 */
void SaveDispTotal( unsigned char PhNozzle )
{
	DISP_CR_Buf.DispTotal[PhNozzle].PhiscalNozzle = PhNozzle;
	DISP_CR_Buf.DispTotal[PhNozzle].LogicalNozzle = GetLogicNzn( PhNozzle );

	memcpy( DISP_CR_Buf.DispTotal[PhNozzle].VolumeTotal, &PowerDown.LogOnCard.LogOnVolumeTotals[PhNozzle*4], sizeof( DISP_CR_Buf.DispTotal[0].VolumeTotal ));
	memcpy( DISP_CR_Buf.DispTotal[PhNozzle].AmountTotal, &PowerDown.LogOnCard.LogOnAmountTotals[PhNozzle*4], sizeof( DISP_CR_Buf.DispTotal[0].AmountTotal ));

	// Clear The Duty Totals
	memset( DISP_CR_Buf.DispTotal[PhNozzle].DutyVolumeTotal, 0x00, sizeof( DISP_CR_Buf.DispTotal[0].DutyVolumeTotal ));
	memset( DISP_CR_Buf.DispTotal[PhNozzle].DutyAmountTotal, 0x00, sizeof( DISP_CR_Buf.DispTotal[0].DutyAmountTotal ));

#ifdef MAKE_CASH_CR
	// Clear The Duty Cash Totals
	memset( DISP_CR_Buf.DispTotal[PhNozzle].DutyCashVolumeTotal, 0x00, sizeof( DISP_CR_Buf.DispTotal[0].DutyCashVolumeTotal ));
	memset( DISP_CR_Buf.DispTotal[PhNozzle].DutyCashAmountTotal, 0x00, sizeof( DISP_CR_Buf.DispTotal[0].DutyCashVolumeTotal ));

	// Save To PdProtect
	memcpy( &PowerDown.PDDutyCashVolumeTotal[PhNozzle*4], DISP_CR_Buf.DispTotal[PhNozzle].DutyCashVolumeTotal, sizeof( DISP_CR_Buf.DispTotal[0].DutyCashVolumeTotal ));
	memcpy( &PowerDown.PDDutyCashAmountTotal[PhNozzle*4], DISP_CR_Buf.DispTotal[PhNozzle].DutyCashAmountTotal, sizeof( DISP_CR_Buf.DispTotal[0].DutyCashAmountTotal ));
#endif

}


/*
 ** WriteTotalLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	type: 0: PowerOn Totals; 1: LogOn Totals; 2: LogOff Totals
 *
 *  DESCRIPTION: Write The Volume And Amount Total From Pump toLog
 *
 *  RETURNS: None.
 *
 */
void WriteTotalLog( unsigned char type )
{
	unsigned char len;
	unsigned int value;

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Fill "Totals Flag"
	if( type == 0 )
	{
		memcpy( &CRLogTxt[len], "上电读取油机总累", 16 );
	}
	else if( type == 1 )
	{
		memcpy( &CRLogTxt[len], "签到读取油机总累", 16 );
	}
	else
	{
		memcpy( &CRLogTxt[len], "签退读取油机总累", 16 );
	}
	len = strlen(( char * )CRLogTxt );

	// Save Volume Total 0
	memcpy( &value, &Pump_CR_Buf.VolumeTotal[0], 4 );
	sprintf(( char * )&CRLogTxt[len], "%08X", value );
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save Amount Total 0
	memcpy( &value, &Pump_CR_Buf.AmountTotal[0], 4 );
	sprintf(( char * )&CRLogTxt[len], "%08X", value );
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save Volume Total 1
	memcpy( &value, &Pump_CR_Buf.VolumeTotal[4], 4 );
	sprintf(( char * )&CRLogTxt[len], "%08X", value );
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save Amount Total 1
	memcpy( &value, &Pump_CR_Buf.AmountTotal[4], 4 );
	sprintf(( char * )&CRLogTxt[len], "%08X", value );
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Save Volume Total 2
	memcpy( &value, &Pump_CR_Buf.VolumeTotal[8], 4 );
	sprintf(( char * )&CRLogTxt[len], "%08X", value );
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save Amount Total 2
	memcpy( &value, &Pump_CR_Buf.AmountTotal[8], 4 );
	sprintf(( char * )&CRLogTxt[len], "%08X", value );
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save Volume Total 3
	memcpy( &value, &Pump_CR_Buf.VolumeTotal[12], 4 );
	sprintf(( char * )&CRLogTxt[len], "%08X", value );
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save Amount Total 3
	memcpy( &value, &Pump_CR_Buf.AmountTotal[12], 4 );
	sprintf(( char * )&CRLogTxt[len], "%08X", value );
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );
}


/*
 ** WriteOfflineLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRSFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Write The Offline Totals Log
 *
 *  RETURNS: None.
 *
 */
void WriteOfflineLog( void )
{
	unsigned char i;
	unsigned char len;
	unsigned int value;

	for( i = 0; i < 4; i++ )
	{
		// Clear Buffers
		memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
		len = strlen(( char * )CRLogTxt );

		memcpy( &CRLogTxt[len], "非卡累计", 8 );
		len = strlen(( char * )CRLogTxt );
		
		// Save Index
		sprintf(( char * )&CRLogTxt[len], "%02X ", i );
		len = strlen(( char * )CRLogTxt );
		
		// Save Volume Total
		memcpy( &value, &Pump_CR_Buf.VolumeTotal[4*i], 4 );
		sprintf(( char * )&CRLogTxt[len], "%08X", value );
		len = strlen(( char * )CRLogTxt );
		CRLogTxt[len] = ' ';
		len += 1;

		// Save Amount Total
		memcpy( &value, &Pump_CR_Buf.AmountTotal[4*i], 4 );
		sprintf(( char * )&CRLogTxt[len], "%08X", value );
		len = strlen(( char * )CRLogTxt );
		CRLogTxt[len] = ' ';
		len += 1;

		// Save Offline Volume
		memcpy( &value, &Pump_CR_Buf.OfflineVolume[4*i], 4 );
		sprintf(( char * )&CRLogTxt[len], "%08X", value );
		len = strlen(( char * )CRLogTxt );
		CRLogTxt[len] = ' ';
		len += 1;

		// Save Offline Amount
		memcpy( &value, &Pump_CR_Buf.OfflineAmount[4*i], 4 );
		sprintf(( char * )&CRLogTxt[len], "%08X", value );
		len = strlen(( char * )CRLogTxt );
		CRLogTxt[len] = ' ';
		len += 1;

		// Save To Log
		WriteLog( PROCESS_CR, CRLogTxt );
	}
}