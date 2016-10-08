
#include <string.h>
#include "global.h"

#include "PSAMCommand.h"
#include "ICCardInProc.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "PSAMProc.h"
#include "Screen.h"
#include "Message.h"




/*
 ** PSAM_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Process PSAM, Include Reset PSAM, Select PSE, Select Sinopec1 ADF
 *				 Select Sinopec2 ADF And Set PSAM Check CRC
 *
 *  RETURNS:
 *
 */
void PSAM_Proc( void )
{
	unsigned short int retval;

	// Only Running When CR_Proc_Step = CR_PSAM_PROC
	if( CR_Proc_Step == CR_PSAM_PROC )
	{
		// Check Command Running
		if( CR_CommandType == CR_PSAM_COLD_RESET )
		{
			// PSAM Reset
			retval = CR_PSAM1_ColdReset_7816_50V();
			
			if( retval != 1 )
			{
				if( retval == 0x9000 )
				{
					// PSAM Reset Ok, Select PSE Next In Same CR_Proc_Step
//					Set_Next_Command( CR_PSAM_PROC, CR_PSAM_SELECT_PSE );
					Set_Next_Command( CR_PSAM_PROC, CR_ICCARD_DELAY );
//
//					// Display " PSAM Reset Ok "
//					CRFillMsg( MsgPSAMResetOk );
				}
				else
				{
					// Card Reset Error, Inform Error
					CommErrorTimes++;
					
					// PSAM Error, Check Card Reader Next
					Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );

					// Display " PSAM Reset Error "
					FillInfoMsg( M_PSAMResetError, 0, 0 );
				}
			}
			else
			{
				// Command Running, Go On
			}
		}
		else if( CR_CommandType == CR_ICCARD_DELAY )
		{
			// Delay A Certain Time For Test Approval
	   		if( CardDelaySet == 0 )
			{
	   			CreateTimer( TIMER_ONE_DELETE, CardDelayTimeOut * 2, CardDelay );
				CardDelaySet = 1;
			}
			else
			{
				if( CardDelayed == 1 )
				{
					CardDelaySet = 0;
					CardDelayed = 0;

					// PSAM Reset Ok, Chek PSAM PSE Next
					Set_Next_Command( CR_PSAM_PROC, CR_PSAM_SELECT_PSE );
				}
				else
				{
					// Loop
					Set_Next_Command( CR_PSAM_PROC, CR_ICCARD_DELAY );
				}
			}
		}
		else if( CR_CommandType == CR_PSAM_SELECT_PSE )
		{
			// PSAM Select PSE
			retval = CR_PSAM_Select_PSE( PSAM1 );
			
			if( retval != 1 )
			{
				if( retval == 0x9000 )
				{
					// PSAM Select PSE Ok, Select Sinopec2 Application
					// Next In Same CR_Proc_Step
					Set_Next_Command( CR_PSAM_PROC, CR_PSAM_SELECT_SINOPEC2 );
//
//					// Display " PSAM Select PSE OK "
//					CRFillMsg( MsgPSAMSelectPSEOk );
				}
				else
				{
					// Card Reset Error, Inform Error
					CommErrorTimes++;
					
					// PSAM Error, Check Card Reader Next
					Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );

					// Display " PSAM Select PSE Error "
					FillInfoMsg( M_PSAMSelectPSEErr, 0, 0 );
				}
			}
			else
			{
				// Command Running, Go On
			}
		}
		else if( CR_CommandType == CR_PSAM_SELECT_SINOPEC2 )
		{
			// PSAM Select PSE
			retval = CR_PSAM_Select_Sinopec2( PSAM1 );
			
			if( retval != 1 )
			{
				if( retval == 0x9000 )
				{
					// PSAM Select Sinopec2 Ok, Select Sinopec1 Application
					// Next In Same CR_Proc_Step
					Set_Next_Command( CR_PSAM_PROC, CR_PSAM_SELECT_SINOPEC1 );
//
//					// Display " PSAM Select Sinopec2 Ok "
//					CRFillMsg( MsgPSAMSelectSino2Ok );
				}
				else
				{
					// Card Reset Error, Inform Error
					CommErrorTimes++;
					
					// PSAM Error, Check Card Reader Next
					Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );

					// Display " PSAM Select Sinopec2 Error "
					FillInfoMsg( M_PSAMSelectAPP2Err, 0, 0 );
				}
			}
			else
			{
				// Command Running, Go On
			}
		}
		else if( CR_CommandType == CR_PSAM_SELECT_SINOPEC1 )
		{
			// PSAM Select PSE
			retval = CR_PSAM_Select_Sinopec1( PSAM1 );
			
			if( retval != 1 )
			{
				if( retval == 0x9000 )
				{
					// Calculate CRC For PSAM Information
					retval = CalcCRC16( TRINP.TR_PSAM_ATR, TRINP.TR_PSAM_CHECK_CRC - TRINP.TR_PSAM_ATR );
					ShortIntToCharCopy( retval, TRINP.TR_PSAM_CHECK_CRC );
					
#ifdef MAKE_PSAM_VERIFY
					// PSAM Get Anti Fraud Proof Next
					Set_Next_Command( CR_PSAM_PROC, CR_PSAM_GET_ANTI_FRAUD_PROOF );
#else
					// PSAM Select Sinopec1 Ok, Check IC Card Absence Next
					Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
#endif
				}
				else
				{
					// Card Reset Error, Inform Error
					CommErrorTimes++;
					
					// PSAM Error, Check Card Reader Next
					Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );

					// Display " PSAM Select Sinopec1 Error "
					FillInfoMsg( M_PSAMSelectAPP1Err, 0, 0 );
				}
			}
			else
			{
				// Command Running, Go On
			}
		}
#ifdef MAKE_PSAM_VERIFY

		else if( CR_CommandType == CR_PSAM_GET_ANTI_FRAUD_PROOF )
		{
			// PSAM Select PSE
			retval = CR_PSAM_Get_Anti_Fraud_Proof( PSAM1 );
			
			if( retval != 1 )
			{
				if( retval == 0x9000 )
				{
					if( !( CRPowerFlag & CRPOWERON ))
					{
						// For First Power On, Do  Not Display The Message
						// Display: "Certificate, x Times Left"
						memset( InfoLine, 0x00, sizeof( InfoLine ));
//						memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
						memcpy( InfoLine, DISPMSG[M_V_Certificate_Times], strlen( DISPMSG[M_V_Certificate_Times] ));
//						sprintf(( char * )&CustomMsg0[0], "% 3u", TRINP.TR_PSAM_ANTIFRAUD_TIMES );
//						memcpy( &InfoLine[6], CustomMsg0, 3 );
						sprintf(( char * )&InfoLine[6], "% 3u", TRINP.TR_PSAM_ANTIFRAUD_TIMES );
						InfoLine[9]	= ' ';

						// Display " Certificate, * Times Left"
						FillInfoMsg( M_InfoLine, 0, 0 );
					}
					else
					{
						// Clear Power On Flag
						CRPowerFlag &= ( ~CRPOWERON );
					}
					// PSAM Check AntiFraud Ok, Check IC Card Absence Next
					Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
				}
				else if(( retval == 0x6D00 ) || ( retval == 0x9B01 ))
				{
					// PSAM Do Not Support Certificate Function, Just Waiting Next, And Spring Out Card If Inserted
					// Display Not Support
					NextScreenID = SCR_PSAM_V_UNSUPPORT;
					Set_Next_Command( CR_PSAM_PROC, CR_CARDREADER_CHECK_CARD_PSAM );
				}
				else if( retval == 0x9B02 )
				{
					// PSAM Need Register, Go To Check ACT Card Inserted
					// Display Insert ACT Card
					NextScreenID = SCR_PSAM_V_UNREGISTERED;
					Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_CHECK_CARD );
				}
				else if( retval == 0x9B03 )
				{
					// PSAM Lock Certificate Function, Just Waiting Next, And Spring Out Card If Inserted
					// Display Locked
					NextScreenID = SCR_PSAM_V_PSAM_LOCK;
					Set_Next_Command( CR_PSAM_PROC, CR_CARDREADER_CHECK_CARD_PSAM );
				}
				else
				{
					// Card Reset Error, Inform Error
					CommErrorTimes++;
					
					// PSAM Error, Check Card Reader Next
					Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );

					// Display " PSAM Read Proof Error "
					FillInfoMsg( M_V_Read_Flag_Error, 0, 0 );
				}
			}
			else
			{
				// Command Running, Go On
			}
		}
		else if( CR_CommandType == CR_CARDREADER_CHECK_CARD_PSAM )
		{

			// Check IC Card Inserted Or Not
			retval = CR_CardReaderCheckCard();
			
			if( retval != 1 )
			{
				if( retval == 0x9000 )
				{
					// Spring Out IC Card, For PSAM Do Not Support Verify Or Locked
					Set_Next_Command( CR_PSAM_PROC, CR_CARDREADER_SPINGOUT_CARD_PSAM );
				}
				else
				{
					// Just Check Card Insert And Waiting
					Set_Next_Command( CR_PSAM_PROC, CR_CARDREADER_CHECK_CARD_PSAM );
				}
			}
			else
			{
				// Command Running, Go On
			}
		}
		else if( CR_CommandType == CR_CARDREADER_SPINGOUT_CARD_PSAM )
		{

			// Support Out Card
			retval = CR_CardReaderSpringOutCard();
			
			if( retval != 1 )
			{
				if( retval == 0x9000 )
				{
					// Just Check Card Insert And Waiting
					Set_Next_Command( CR_PSAM_PROC, CR_CARDREADER_CHECK_CARD_PSAM );
				}
				else
				{
					// Just Check Card Insert And Waiting
					Set_Next_Command( CR_PSAM_PROC, CR_CARDREADER_CHECK_CARD_PSAM );
				}
			}
			else
			{
				// Command Running, Go On
			}
		}

#endif
	}
	else
	{
		// Go On
	}
}