#include <string.h>

#include "global.h"

#include "ICCardCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "ICCardProc.h"
#include "ICCardInProc.h"
#include "Pdprotect.h"
#include "define.h"
#include "CardReady.h"
#include "ExCRProc.h"
#include "Screen.h"
#include "Display.h"
#include "CRPumpCall.h"

#include "PSAMCommand.h"


/*
 ** ICCard_Ready_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CardReady.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: IC Card Ready For Fuelling, Perform
 *				  Inquire Card Exit Or Not, Check Nozzle On	From Pump
 *				  Spring Out Card From Keypad etc.
 *
 *  RETURNS: None.
 *
 */
void ICCard_Ready_Proc( void )
{
	unsigned short int retval;
	
	// Only Running When CR_Proc_Step = CR_ICCARD_READY_PROC
	if( CR_Proc_Step == CR_ICCARD_READY_PROC )
	{
		switch( CR_CommandType )
		{
			case CR_CARDREADER_CHECK_CARD:


#ifdef MAKE_CARD_MODE2
				if(( PowerDown.PdTrans.PD_TRANS_FLAG == AUTH_RESP )
					&& ( TRINP.TR_PAY_X == 0x15 ))
				{
					// Host Auth, Return Ok Only
					retval = 0x9000;
				}
				else if( TRINP.TR_PAY_X == 0x18 )
				{
					// Non IC Card Trans, Return Ok Only
					retval = 0x9000;
				}
				else
				{
					// First Check IC Card Exist Or Not
					retval = CR_CardReaderCheckCard();
				}
#else
				// First Check IC Card Exist Or Not
				retval = CR_CardReaderCheckCard();

#endif	// MAKE_CARD_MODE2
						
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// IC Card Exist, Check External Process
						Set_Next_Command( CR_ICCARD_EXIST_EXTERNAL_PROC, CR_CommandStart );
					}
					else
					{
//						// Write Log
//						CRWriteLog( M_CardSpringOut );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_3;
                        ErrorFlag = retval;

						// IC Card Not Exist, Power Off The Card Reader
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
					 		
			break;

#ifdef MAKE_CARD_MODE2
			case CR_WAITING_MAC:

				if( TRINP.TR_WORK_MODE != 0x01 )
				{
					// Can Not Auth OnLine, Check Card Next
					Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
				}
				else
				{
					// Prepare The Calc MAC Elements
					// MAC Process Return
					CR_MAC_CommandType = CR_WAITING_AUTH;
					CR_Proc_MAC_Step = CR_ICCARD_READY_PROC;

					// MAC Type:
					CR_MACType = CR_MAC_TYPE_MAC;

					// Get The TTC, For Sinopec Mode2
					GetTransTTC();

					// Get Trans Date And Time For Set Grey
					GetTransTime();
					
					// Save PumpCall Information
					SavePumpCallData( 0 );

					// Nozzle Number Default The 1st Nozzle, Changed After Select Nozzle Number Screen
					Pump_CR_Buf.NozzleNumber = 1;
					GetPumpCallInfo();

					// For Sinopec Mode2, PayX Byte
					if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
					{
						// Customer Card, Default
						// Changed After Auth Response
						if( TRINP.TR_TRANS_DEBITSOURCE == 0x01 )
						{
							TRINP.TR_PAY_X = 0x02;
						}
						else
						{
							TRINP.TR_PAY_X = 0x05;
						}
					}
					else
					{
						if( TRINP.TR_PAY_X == 0x18 )
						{
							// Non IC Card Trasn By Keypad,
							// Just Go On....
						}
						else
						{
							// Employee Card, Default
							if( TransAmountPay == 0x01 )
							{
								// Changed After Auth Response
								TRINP.TR_PAY_X = 0x17;
							}
							else
							{
								// Changed After Auth Response
								TRINP.TR_PAY_X = 0x07;
							}
						}
					}

					if( TRINP.TR_WORK_MODE != 0x01 )
					{
						// Host POS, Not FS
						TRINP.TR_TRANS_TYPE |= ( WORKOFFLINE | LOCALLIST );
						PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
					}
					else
					{
						TRINP.TR_TRANS_TYPE &= ( ~( LOCALLIST | WORKOFFLINE ));
						PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
					}

					// Fill The Data
					if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
					{
						// Customer Card Auth Data
						FillAuthReqMAC_CST();
					}
					else
					{
						// Employee Card Auth Data
						FillAuthReqMAC_EMP();
					}

					// Calc MAC, Next
					Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_SELECT_SINOPEC2 );
				}
			
			break;

			case CR_WAITING_AUTH:

				// Request POS Auth Data Next
				if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
				{
					// Customer Card Auth POS
					retval = AuthPOSOnline_CST();
				}
				else
				{
					// Employee Card Auth POS
					retval = AuthPOSOnline_EMP();
				}

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Work Online, Auth Pump Next
						// Check Auth Response
						if( TRINP.TR_AUTH_FEEDBACK == 0x01 )
						{
							// Save PumpCall Information
							SavePumpCallData( 0 );
						

							// Auth Ok, Update Screen
							UpdateScreen();

							// POS Auth OK, Check Card Next And Wait Nozzle On
							Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
						}
						else if( TRINP.TR_AUTH_FEEDBACK == 0x02 )
						{
							// Mark Send Record After Trans
							SendCardRecord = 0x01;

							// Auth Ok, Update Screen
							UpdateScreen();

							// Save PumpCall Information
							SavePumpCallData( 0 );
						
							// POS Auth OK, Check Card Next And Wait Nozzle On
							Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
						}
						else if( TRINP.TR_AUTH_FEEDBACK == 0x03 )
						{
							// Black List Card
							// Inform POS Black Card
							memcpy( POS_CR_Buf.ICCARD_STATE, "\x00\x01", sizeof( POS_CR_Buf.ICCARD_STATE ));
							
							// For Record Black Card
							memcpy( TRINP.TR_CARD_GREY_TAC, "\x00\x01\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
							
							SaveICCardData();

			  				// Display " IC Card In Black List "
	  						FillInfoMsg( M_CardBlackList, 0, 0 );

							// IC Card Is Black, Lock PIN, Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_PIN_LOCK );
						}
						else if( TRINP.TR_AUTH_FEEDBACK == 0x04 )
						{
			  				// Display " White List Not Fit "
	  						FillInfoMsg( M_NotWhiteList, 0, 0 );

							FillCardErrorData_Mode2();

							// For Record Black Card
							memcpy( TRINP.TR_CARD_GREY_TAC, "\x20\x01\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
							
							// Save To PowerDown.PdTrans, Card Tac, For Limited Card
							memcpy( PowerDown.PdTrans.PD_CARD_GTAC, TRINP.TR_CARD_GREY_TAC, sizeof( PowerDown.PdTrans.PD_CARD_GTAC ));

							// For Write The Running Log
							JournalFlag = 32;

							// Form Journal Next
							Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );

//							// For Write Spring Card Log
//							PowerDownFlag = Pd_Flag_103;
//
//							// Error, Exit
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
						else if( TRINP.TR_AUTH_FEEDBACK == 0x05 )
						{
			  				// Display " Auth Rejected "
							memset( InfoLine, 0x00, sizeof( InfoLine ));
							if( TRINP.TR_ADD_IN_LEN > 19 )
							{
								TRINP.TR_ADD_IN_LEN = 19;
							}

							InfoLine[0]	= '.';
							memcpy( &InfoLine[1], TRINP.TR_ADD_IN, TRINP.TR_ADD_IN_LEN );
							FillInfoMsg( M_InfoLine, 0, 1 );

							FillCardErrorData_Mode2();

							// For Record Black Card
							memcpy( TRINP.TR_CARD_GREY_TAC, "\x20\x01\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
							
							// Save To PowerDown.PdTrans, Card Tac, For Limited Card
							memcpy( PowerDown.PdTrans.PD_CARD_GTAC, TRINP.TR_CARD_GREY_TAC, sizeof( PowerDown.PdTrans.PD_CARD_GTAC ));

							// For Write The Running Log
							JournalFlag = 33;

							// Form Journal Next
							Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );

//							// For Write Spring Card Log
//							PowerDownFlag = Pd_Flag_104;
//
//							// Error, Exit
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
						else
						{
							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_105;

							// Error, Exit
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_106;
                        ErrorFlag = retval;

						// Error, Exit
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

#endif	// MAKE_CARD_MODE2

			default:
			break;
		}
	}
}
