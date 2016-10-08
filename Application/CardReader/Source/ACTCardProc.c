#include "global.h"

#include "Define.h"
#include "CRVaribles.h"
#include "ICCardCommand.h"
#include "PSAMCommand.h"
#include "CRMemCopy.h"
#include "CRCommand.h"
#include "ICCardInProc.h"
#include "Screen.h"

#include "ACTCardProc.h"

//Douzhq 20140610 Add , IC Card Recognize
//#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
//#include "SocialCard.h"
//#endif

#ifdef MAKE_PSAM_VERIFY	


unsigned char ACTCardDelaySet = 0;
unsigned char ACTCardDelayed = 0;

void ACTCardDelay( void )
{
	ACTCardDelayed = 1;
}

//static unsigned char ACTProcFlag = 0;



void ICCard_ACT_Proc( void )
{
	unsigned short int retval;

	// Only Running When CR_Proc_Step = CR_ACT_CARD_PROC
	if( CR_Proc_Step == CR_ACT_CARD_PROC )
	{
		switch( CR_CommandType )
		{
			case CR_CARDREADER_CHECK_CARD:

				// Check IC Card Inserted Or Not
				retval = CR_CardReaderCheckCard();
				
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Clear ACT Process Flag
//						ACTProcFlag = 0;

						// Backlight Open, For 30 Seconds
						OpenLcdLight( 0 );

						if( IcTypeFlg == 0 )
						{
							// For Intelligence Card Reader, Get Card Type Next
							// Delay A Certain Time For Test Approval
							Set_Next_Command( CR_ACT_CARD_PROC, CR_ICCARD_DELAY );
						}
						else
						{
							// First, Excute IC Card Power On Command
							Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWERON_CARD );
						}
					}
					else
					{
						// Just Check Card Insert And Waiting
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_CHECK_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
				
			break;

			// IC Card Power Off
			case CR_CARDREADER_POWEROFF_CARD:
				
				// Power Off
				retval = CR_CardReaderPowerOffCard();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Spring Out IC Card Next
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_SPINGOUT_CARD );
					}
					else if( retval == 0x0002 )
					{
						// Inform POS Register Ok
						if( ProcStatus[PROCESS_POS] & POS_ONLINE )
						{
							if( RegisterStep != 0 )
							{
								// Fill The Buffer
								FillPOSMessage( 0, RegisterStep, 0 );

								RegisterStep = 0;

								// Calc MAC Next
								Set_Next_Command( CR_RID_CARD_PROC, CR_PSAM_VERIFY_MSG_SINOPEC2 );
							}
						}
						else
						{
							// PSAM Register Next
							// Display Insert ACT Card
							NextScreenID = SCR_PSAM_V_UNREGISTERED;

							// No Card In The Reader, Do Not Spring Out
							// Card Reader Proc Next
							Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_CHECK_CARD );
						}
					}
					else
					{
						// Spring Out IC Card Anyway
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_SPINGOUT_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;
			
			// IC Card Spring Out
			case CR_CARDREADER_SPINGOUT_CARD:
				// Spring Out IC Card
				retval = CR_CardReaderSpringOutCard();

				// Check Command Return
				if( retval != 1 )
				{
					// Inform POS Register Ok
					if( ProcStatus[PROCESS_POS] & POS_ONLINE )
					{
						if( RegisterStep != 0 )
						{
							// Fill The Buffer
							FillPOSMessage( 0, RegisterStep, 0 );

							RegisterStep = 0;

							// Calc MAC Next
							Set_Next_Command( CR_RID_CARD_PROC, CR_PSAM_VERIFY_MSG_SINOPEC2 );
						}
					}
					else
					{
						if( retval == 0x9000 )
						{
							// Card Reader Proc Next
							Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
						}
						else
						{
							// Spring Out Error
							// Display: "Spring Out IC Card Error"
							// Card Reader Proc Next
							Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
						}
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;
			

			case CR_ICCARD_DELAY:

				// Delay A Certain Time For Intelligent Card Reader
		   		if( ACTCardDelaySet == 0 )
				{
		   			CreateTimer( TIMER_ONE_DELETE, ACTCardDelayTimeOut, ACTCardDelay );
					ACTCardDelaySet = 1;
				}
				else
				{
					if( ACTCardDelayed == 1 )
					{
						ACTCardDelaySet = 0;
						ACTCardDelayed = 0;

						// Delay Time Out, Go On....
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWERON_CARD );
					}
					else
					{
						// Loop
						Set_Next_Command( CR_ACT_CARD_PROC, CR_ICCARD_DELAY );
					}
				}

			break;

			// IC Card Power On
			case CR_CARDREADER_POWERON_CARD:
				// Power On
				retval = CR_CardReaderPowerOnCard();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Reset The IC Card Next
						Set_Next_Command( CR_ACT_CARD_PROC, CR_ICCARD_RESET );
					}
					else
					{
		  				// Display " IC Card Power On Error "
  						FillInfoMsg( M_CardPowerOnErr, 0, 0 );

						// Step For Message Field;
						RegisterStep = 1;

						// Power On Error
						// Spring Out IC Card
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_SPINGOUT_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;
			
			// IC Card Reset
			case CR_ICCARD_RESET:
				
				// Reset The IC Card
				retval = CR_IC_ColdReset_7816();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Select Sinopec Application Next
						Set_Next_Command( CR_ACT_CARD_PROC, CR_ICCARD_SELECT_SINOPEC_APP );
					}
					else
					{
		  				// Display " IC Card Maybe Inverted "
  						FillInfoMsg( M_CardInverted, 0, 0 );

						// Step For Message Field;
						RegisterStep = 2;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;
			
			// IC Card Select Sinopec Application
			case CR_ICCARD_SELECT_SINOPEC_APP:
				
//Douzhq 20140610 Add , IC Card Recognize
//IC Card Recognize
//#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
//			  if (PowerDown.CardRecoginMode)
//			  {
//				if (AidType == M_SINOPEC)
//				{
//					retval = CR_ICCARD_Select_Sinopec_APP();
//				}
//				else if (AidType == M_SOCIAL)
//				{
//					retval = CR_ICCARD_Select_Social_APP();	
//				}
//				else
//				{
//					//....
//					retval = CR_ICCARD_Select_Sinopec_APP();
//				}			  	
//			  }
//			  else
//			  {
//				retval = CR_ICCARD_Select_Sinopec_APP();			  	
//			  }
//				
//#else
				retval = CR_ICCARD_Select_Sinopec_APP();
//#endif

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Read SFI 0x15 = 21
						Set_Next_Command( CR_ACT_CARD_PROC, CR_ICCARD_READ_SFI_0X15 );
					}
					else
					{
		  				// Display " IC Card Select App Error "
  						FillInfoMsg( M_CardSelectAppErr, 0, 0 );

						// Step For Message Field;
						RegisterStep = 3;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;
			
			// IC Card Read SFI 0x15
			case CR_ICCARD_READ_SFI_0X15:
				
				retval = CR_ICCARD_Read_SFI_0x15();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Check ACT Card Or Not
						if( TRINP.TR_CARD_ASN[2] == ACT_CARD )
						{
							// Check PSAM Acquirer With ACT Card Issuer
							if( memcmp( TRINP.TR_PSAM_ACQUIRE_ID, TRINP.TR_CARD_ISSUER_ID, sizeof( TRINP.TR_CARD_ISSUER_ID )) == 0 )
							{
								// Get Date And Time
								GetTransTime();

								// Check ACT Card Validate And Expire Date
								retval = Card_Validate_Check();
								if( retval == 0 )
								{
									// Read SFI 0x1A = 26 Next
									Set_Next_Command( CR_ACT_CARD_PROC, CR_ICCARD_READ_SFI_0X1A );
								}
								else
								{
									// Step For Message Field;
									RegisterStep = 4;

									// Power Off IC Card And Spring Out
									Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
								}
							}
							else
							{
								// Display " IC Card Error"
								FillInfoMsg( M_CardError, 0, 0 );

								// Step For Message Field;
								RegisterStep = 5;

								// Run Error
								// Power Off IC Card And Spring Out
								Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
							}
						}
						else
						{
							// Display " Please Insert ACT Card"
							FillInfoMsg( M_V_Insert_ACT, 0, 0 );

							// Step For Message Field;
							RegisterStep = 6;

							// Power Off IC Card And Spring Out
							Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_CardError, 0, 0 );

						// Step For Message Field;
						RegisterStep = 7;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;
			
			// IC Card Read SFI 0x1A
			case CR_ICCARD_READ_SFI_0X1A:
				
				// Attention: For The ACT SFI0x1A Is Same As IC Card SFI0x16,
				// So Use Same RAM With SFI0x16
				retval = CR_ICCARD_ACT_Read_SFI_0x1A();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Wait Input PIN Next
						Set_Next_Command( CR_ACT_CARD_PROC, CR_WAITING_PIN );
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_CardError, 0, 0 );

						// Step For Message Field;
						RegisterStep = 8;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			// IC Card Wait PIN Input
			case CR_WAITING_PIN:

				// Use Input PIN
				// First Set Get PIN Flag In ProcExchange[PROCESS_CR][PROCESS_KEYPAD];
				if( !( CR_Task_Flag & ICCARD_PIN_K ))
				{
					// Check PIN Input Or Not
					// Set Bit0, Local Process Flag
					CR_Task_Flag |= ICCARD_PIN_K;

					// Set Bit, To Inform Keypad Input IC Card PIN
					ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= ICCARD_PIN;
				}
				else
				{
					// Second Check The Input, By Check ProcExchange[PROCESS_KEYPAD][PROCESS_CR]
					if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & ICCARD_PIN )
					{
						// PIN Inputed By Keypad
						// Clear The Bit
						ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~ICCARD_PIN );
				
						// Clear Local Process Flag
						CR_Task_Flag &= ( ~ICCARD_PIN_K );

						// Check Input Ok Or Cancel
						if( KEYPAD_CR_Buf.ICCardPINLen != 0 )
						{
							// Input OK
							TRINP.TR_INPUT_PIN_LEN = KEYPAD_CR_Buf.ICCardPINLen;
							memcpy( TRINP.TR_INPUT_PIN, KEYPAD_CR_Buf.ICCardPIN, TRINP.TR_INPUT_PIN_LEN );
				
							// Clear Input
							KEYPAD_CR_Buf.ICCardPINLen = 0;
							memset( KEYPAD_CR_Buf.ICCardPIN, 0x00, sizeof( KEYPAD_CR_Buf.ICCardPIN ));

							// Verfiy PIN Next
							Set_Next_Command( CR_ACT_CARD_PROC, CR_ICCARD_VERIFY_PIN );
						}
						else
						{
							// Step For Message Field;
							RegisterStep = 9;

							// Input Canceled
							// Power Off IC Card And Spring Out
							Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
					}
					else
					{
						// Wait Input, Go On
					}
				}
						
			break;
			
			// IC Card Verify PIN
			case CR_ICCARD_VERIFY_PIN:

				 retval = CR_ICCARD_Verify_PIN();
				
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Get Balance Next
						Set_Next_Command( CR_ACT_CARD_PROC, CR_PSAM_GET_RANDOM );

						memset( InfoLine, 0x00, sizeof( InfoLine ));
						// Display "    "
						FillInfoMsg( M_InfoLine, 0, 0 );
					}
					else if(( retval == 0x63C0 ) || ( retval == 0x6983 ))
					{
						// Display: "PIN Blocked"
						// Display " PIN Block"
						FillInfoMsg( M_CardPINBlocked, 0, 0 );
						
						// Step For Message Field;
						RegisterStep = 10;

						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
					else if(( retval / 0x100 ) == 0x63 )
					{
						// Display: "PIN Error, x Times Retry Left"
						retval %= 0x10;
						
						memset( InfoLine, 0x00, sizeof( InfoLine ));
						
						retval |= 0x30;
						memcpy( InfoLine, DISPMSG[M_PINRetries], strlen( DISPMSG[M_PINRetries] ));
						InfoLine[11] = retval;

						// Display " PIN Retries, * Times Left"
						FillInfoMsg( M_InfoLine, 0, 0 );

						// Get Pin Next
						Set_Next_Command( CR_ACT_CARD_PROC, CR_WAITING_PIN );
					}
					else
					{
						// Display " PIN Block"
						FillInfoMsg( M_CardPINError, 0, 0 );

						// Step For Message Field;
						RegisterStep = 11;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
				 		
			break;
			
			// PSAM Get Random
			case CR_PSAM_GET_RANDOM:
				
				retval = CR_PSAM_Get_Random( PSAM1 );

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						memset( &RecvBuffer[127], 0x00, 64 );
						RecvBuffer[127] = 0x08;
						memcpy( &RecvBuffer[128], TRINP.TR_PSAM_ACT_RID_RANDOM, sizeof( TRINP.TR_PSAM_ACT_RID_RANDOM ));

//						// Power Off IC Card And Spring Out
//						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
						// Calc Crypted Data Next
						Set_Next_Command( CR_ACT_CARD_PROC, CR_ACT_RID_ENCRYPTED );
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_V_PSAM_Random_Error, 0, 0 );

						// Step For Message Field;
						RegisterStep = 12;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			// IC Card Calc Crypted Data
			case CR_ACT_RID_ENCRYPTED:
				
				retval = CR_ICCard_ACT_Encrypted();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Start Binding Next
						Set_Next_Command( CR_ACT_CARD_PROC, CR_PSAM_START_BINDING );
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_CardReadError, 0, 0 );

						// Step For Message Field;
						RegisterStep = 13;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			// PSAM Start Binding
			case CR_PSAM_START_BINDING:
				
				retval = CR_PSAM_Start_Binding( PSAM1 );

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						NextScreenID = SCR_PSAM_V_INSERT_RID;

						// Set ACT Process Flag
						ACTRegisterStep = 1;

						// Start RID Next
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
					else
					{
						// Display " PSAM Start Binding Error"
						FillInfoMsg( M_V_PSAM_Start_Binding_Error, 0, 0 );

						// Step For Message Field;
						RegisterStep = 14;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
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
	}
}





#endif