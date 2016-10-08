#include "global.h"

#include "Define.h"
#include "CRVaribles.h"
#include "ICCardCommand.h"
#include "PSAMCommand.h"
#include "CRMemCopy.h"
#include "CRCommand.h"
#include "ICCardInProc.h"
#include "Screen.h"

#include "RIDCardProc.h"

//Douzhq 20140610 Add , IC Card Recognize
//#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
//#include "SocialCard.h"
//#endif

#ifdef MAKE_PSAM_VERIFY



unsigned char RIDCardDelaySet = 0;
unsigned char RIDCardDelayed = 0;

void RIDCardDelay( void )
{
	RIDCardDelayed = 1;
}


static unsigned char RIDProcFlag = 0;



void ICCard_RID_Proc( void )
{
	unsigned short int retval;

	// Only Running When CR_Proc_Step = CR_RID_CARD_PROC
	if( CR_Proc_Step == CR_RID_CARD_PROC )
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
						// Enter RID Process
						ACTRegisterStep = 0;

						if( IcTypeFlg == 0 )
						{
							// For Intelligence Card Reader, Get Card Type Next
							// Delay A Certain Time For Test Approval
							Set_Next_Command( CR_RID_CARD_PROC, CR_ICCARD_DELAY );
						}
						else
						{
							// First, Excute IC Card Power On Command
							Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWERON_CARD );
						}
					}
					else
					{
						// Just Check Card Insert And Waiting
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_CHECK_CARD );
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
//					NextScreenID = SCR_PSAM_V_INSERT_RID;

					if( retval == 0x9000 )
					{
						// Spring Out IC Card Next
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_SPINGOUT_CARD );
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
							else
							{
								if( ACTRegisterStep == 1 )
								{
									// ACT Normal End
									ACTRegisterStep = 0;
									NextScreenID = SCR_PSAM_V_INSERT_RID;

									// Card Reader Proc Next
									Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_CHECK_CARD );
								}
								else
								{
									NextScreenID = DEFAULT_SCR_CARD;

									// Card Reader Proc Next
									Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
								}
							}
						}
						else
						{
							NextScreenID = SCR_PSAM_V_INSERT_RID;

							// No Card In The Reader, Do Not Spring Out
							// Card Reader Proc Next
							Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_CHECK_CARD );
						}
					}
					else
					{
						// Spring Out IC Card Anyway
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_SPINGOUT_CARD );
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
					if( retval == 0x9000 )
					{
						if( RIDProcFlag == 1 )
						{
							RIDProcFlag = 0;

							// Card Reader Proc Next
							Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
						}
						else
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
								else
								{
									if( ACTRegisterStep == 1 )
									{
										// ACT Normal End
										ACTRegisterStep = 0;
										NextScreenID = SCR_PSAM_V_INSERT_RID;

										// Card Reader Proc Next
										Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_CHECK_CARD );
									}
									else
									{
										NextScreenID = DEFAULT_SCR_CARD;

										// Card Reader Proc Next
										Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
									}
								}
							}
							else
							{
								NextScreenID = SCR_PSAM_V_INSERT_RID;

								// Card Reader Proc Next
								Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_CHECK_CARD );
							}
						}
					}
					else
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
							// Spring Out Error
							NextScreenID = SCR_PSAM_V_INSERT_RID;

							// Card Reader Proc Next
							Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_CHECK_CARD );
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
		   		if( RIDCardDelaySet == 0 )
				{
		   			CreateTimer( TIMER_ONE_DELETE, RIDCardDelayTimeOut, RIDCardDelay );
					RIDCardDelaySet = 1;
				}
				else
				{
					if( RIDCardDelayed == 1 )
					{
						RIDCardDelaySet = 0;
						RIDCardDelayed = 0;

						// Delay Time Out, Go On....
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWERON_CARD );
					}
					else
					{
						// Loop
						Set_Next_Command( CR_RID_CARD_PROC, CR_ICCARD_DELAY );
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
						Set_Next_Command( CR_RID_CARD_PROC, CR_ICCARD_RESET );
					}
					else
					{
		  				// Display " IC Card Power On Error "
  						FillInfoMsg( M_CardPowerOnErr, 0, 0 );

						// Step For Message Field, RID Step Start From 41;
						RegisterStep = 41;

						// Power On Error
						// Spring Out IC Card
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_SPINGOUT_CARD );
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
						Set_Next_Command( CR_RID_CARD_PROC, CR_ICCARD_SELECT_SINOPEC_APP );
					}
					else
					{
		  				// Display " IC Card Maybe Inverted "
  						FillInfoMsg( M_CardInverted, 0, 0 );

						// Step For Message Field;
						RegisterStep = 42;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
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
//#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
//			  if (PowerDown.CardRecoginMode != 0)
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
						Set_Next_Command( CR_RID_CARD_PROC, CR_ICCARD_READ_SFI_0X15 );
					}
					else
					{
		  				// Display " IC Card Select App Error "
  						FillInfoMsg( M_CardSelectAppErr, 0, 0 );

						// Step For Message Field;
						RegisterStep = 43;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;
			
			// IC Card Read SFI 0x15
			case CR_ICCARD_READ_SFI_0X15:
				
				retval = CR_ICCARD_RID_Read_SFI_0x15();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Check RID Card Or Not
						if( TRINP.TR_RID_CARD_ASN[2] == RID_CARD )
						{
			  				// Display " Registering.... "
	  						FillInfoMsg( M_V_Registering, 0, 0 );

							// Check PSAM Acquirer With RID Card Issuer
							if( memcmp( GilbarcoID, TRINP.TR_RID_CARD_ISSUER_ID, sizeof( GilbarcoID )) == 0 )
							{
								// Get Date And Time
								GetTransTime();

								// Temporary Save
								memcpy( TRINP.TR_VOLUME_TOTAL_DAY, TRINP.TR_CARD_APP_VALIDATE_DATE, sizeof( TRINP.TR_VOLUME_TOTAL_DAY ));
								memcpy( TRINP.TR_AMOUNT_TOTAL_DAY, TRINP.TR_CARD_APP_EXPIRY_DATE, sizeof( TRINP.TR_AMOUNT_TOTAL_DAY ));

								memcpy( TRINP.TR_CARD_APP_VALIDATE_DATE, TRINP.TR_RID_CARD_APP_VALIDATE_DATE, sizeof( TRINP.TR_VOLUME_TOTAL_DAY ));
								memcpy( TRINP.TR_CARD_APP_EXPIRY_DATE, TRINP.TR_RID_CARD_APP_EXPIRY_DATE, sizeof( TRINP.TR_AMOUNT_TOTAL_DAY ));
								// Temporary Save End

								// Check RID Card Validate And Expire Date
								retval = Card_Validate_Check();

								// Restore Temporary Save
								memcpy( TRINP.TR_RID_CARD_APP_VALIDATE_DATE, TRINP.TR_CARD_APP_VALIDATE_DATE, sizeof( TRINP.TR_VOLUME_TOTAL_DAY ));
								memcpy( TRINP.TR_RID_CARD_APP_EXPIRY_DATE, TRINP.TR_CARD_APP_EXPIRY_DATE, sizeof( TRINP.TR_AMOUNT_TOTAL_DAY ));

								memcpy( TRINP.TR_CARD_APP_VALIDATE_DATE, TRINP.TR_VOLUME_TOTAL_DAY, sizeof( TRINP.TR_VOLUME_TOTAL_DAY ));
								memcpy( TRINP.TR_CARD_APP_EXPIRY_DATE, TRINP.TR_AMOUNT_TOTAL_DAY, sizeof( TRINP.TR_AMOUNT_TOTAL_DAY ));
								
								memset( TRINP.TR_VOLUME_TOTAL_DAY, 0x00, sizeof( TRINP.TR_VOLUME_TOTAL_DAY ));
								memset( TRINP.TR_AMOUNT_TOTAL_DAY, 0x00, sizeof( TRINP.TR_AMOUNT_TOTAL_DAY ));
								// RestoreTemporary Save End

								if( retval == 0 )
								{
									// Read SFI 0x1A = 26 Next
									Set_Next_Command( CR_RID_CARD_PROC, CR_ICCARD_READ_SFI_0X1A );
								}
								else
								{
									// Step For Message Field;
									RegisterStep = 44;

									// Power Off IC Card And Spring Out
									Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
								}
							}
							else
							{
								// Display " IC Card Error"
								FillInfoMsg( M_CardError, 0, 0 );

								// Step For Message Field;
								RegisterStep = 45;

								// Run Error
								// Power Off IC Card And Spring Out
								Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
							}
						}
//						// Check RID Card Or Not
//						else if( TRINP.TR_CARD_ASN[2] == ACT_CARD )
//						{
//							// Re-Inserted ACT Card, The Re-Start Bind
//							Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_CHECK_CARD );
//						}
						else
						{
							// Display " Please Insert RID Card"
							FillInfoMsg( M_V_Insert_RID, 0, 0 );

							// Step For Message Field;
							RegisterStep = 46;

							// Power Off IC Card And Spring Out
							Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_CardError, 0, 0 );

						// Step For Message Field;
						RegisterStep = 47;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;
			
			// IC Card Read SFI 0x1A
			case CR_ICCARD_READ_SFI_0X1A:
				
				// Read RID SFI0x1A
				retval = CR_ICCARD_RID_Read_SFI_0x1A();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Wait Input PIN Next
						Set_Next_Command( CR_RID_CARD_PROC, CR_WAITING_PIN );
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_CardError, 0, 0 );

						// Step For Message Field;
						RegisterStep = 48;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
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
							Set_Next_Command( CR_RID_CARD_PROC, CR_ICCARD_VERIFY_PIN );
						}
						else
						{
							// Step For Message Field;
							RegisterStep = 49;

							// Input Canceled
							// Power Off IC Card And Spring Out
							Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
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
						Set_Next_Command( CR_RID_CARD_PROC, CR_PSAM_GET_RANDOM );

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
						RegisterStep = 50;

						// Get Pin Next
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
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
						Set_Next_Command( CR_RID_CARD_PROC, CR_WAITING_PIN );
					}
					else
					{
						// Display " PIN Block"
						FillInfoMsg( M_CardPINError, 0, 0 );

						// Step For Message Field;
						RegisterStep = 51;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
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

						// Calc Crypted Data Next
						Set_Next_Command( CR_RID_CARD_PROC, CR_ACT_RID_ENCRYPTED );
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_V_PSAM_Random_Error, 0, 0 );

						// Step For Message Field;
						RegisterStep = 52;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			// IC Card Calc Crypted Data
			case CR_ACT_RID_ENCRYPTED:
				
				retval = CR_ICCard_RID_Encrypted();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Start Binding Next
						Set_Next_Command( CR_RID_CARD_PROC, CR_PSAM_INIT_BINDING );
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_CardReadError, 0, 0 );

						// Step For Message Field;
						RegisterStep = 53;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			// PSAM Start Binding
			case CR_PSAM_INIT_BINDING:
				
				retval = CR_PSAM_Init_Binding( PSAM1 );

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Start RID Next
						Set_Next_Command( CR_RID_CARD_PROC, CR_PSAM_BINDING );
					}
					else
					{
						// Display " PSAM Initialize Binding Error"
						FillInfoMsg( M_V_ReInsert_RID, 0, 0 );

						// Step For Message Field;
						RegisterStep = 54;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			// PSAM Binding
			case CR_PSAM_BINDING:
				
				retval = CR_PSAM_Binding( PSAM1 );

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Check IC Card Next
//						Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
						
						RIDProcFlag = 1;

						// Display Register Ok
						NextScreenID = SCR_PSAM_V_REGISTER_OK;

						// Inform POS Register Ok
						if( ProcStatus[PROCESS_POS] & POS_ONLINE )
						{
							// Get PSAM Anti Fraud Times Next
							Set_Next_Command( CR_RID_CARD_PROC, CR_PSAM_GET_ANTI_FRAUD_PROOF );
						}
						else
						{
							// Check Log Para Next
							Set_Next_Command( CR_RID_CARD_PROC, CR_RID_GET_LOG_PARA_TOTAL );
						}
					}
					else
					{
						// Display " PSAM Initialize Binding Error"
						FillInfoMsg( M_V_ReInsert_RID, 0, 0 );

						// Step For Message Field;
						RegisterStep = 55;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_PSAM_GET_ANTI_FRAUD_PROOF:
				
				retval = CR_PSAM_Get_Anti_Fraud_Proof( PSAM1 );

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Inform POS Register Ok
						if( ProcStatus[PROCESS_POS] & POS_ONLINE )
						{
							// Fill The Buffer
							FillPOSMessage( 0, 0, TRINP.TR_PSAM_ANTIFRAUD_TIMES );

							// Calc MAC Next
							Set_Next_Command( CR_RID_CARD_PROC, CR_PSAM_VERIFY_MSG_SINOPEC2 );
						}
						else
						{
							// Check Log Para Next
							Set_Next_Command( CR_RID_CARD_PROC, CR_RID_GET_LOG_PARA_TOTAL );
						}
					}
					else
					{
						// Display " PSAM Initialize Binding Error"
						FillInfoMsg( M_V_ReInsert_RID, 0, 0 );

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_PSAM_VERIFY_MSG_SINOPEC2:

				retval = CR_PSAM_Select_Sinopec2( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Initialize DES Encrypt In PSAM
						Set_Next_Command( CR_RID_CARD_PROC, CR_PSAM_VERIFY_MSG_INIT_MAC );
					}
					else
					{
						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			case CR_PSAM_VERIFY_MSG_INIT_MAC:

				retval = CR_PSAM_Init_DES_Encrypt( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Prepare The DES Buffer
						// RevcBuffer + 127: Save The Data Length
						// The Data Buffer Starts From RecvBuffer + 128
						// Max Length Is 120
						
						// Clear Buffer
						memset( &RecvBuffer[127], 0x00, 110 );

						// Copy Data To The Buffer: Lead 8 Bytes 0x00 As Initiate Value,
						// Next 36 Bytes Data
						Asc2Hex( &RecvBuffer[128+8], &POS_CR_Buf.InforMessage[8], 36 );

						// Pad 0x80, 0x00, 0x00, 0x00
						memcpy( &RecvBuffer[128+8+36], "\x80\x00\x00\x00", 4 );
						RecvBuffer[127] = 48;
						
						// Initialize DES Encrypt In PSAM
						Set_Next_Command( CR_RID_CARD_PROC, CR_PSAM_VERIFY_MSG_MAC );
					}
					else
					{
						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			case CR_PSAM_VERIFY_MSG_MAC:
				
				retval = CR_PSAM_DES_Encrypt0( PSAM1 );

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Save Message MAC
						Hex2Asc( &POS_CR_Buf.InforMessage[80], &RecvBuffer[9], 4 );

						// Inform POS Register Ok
						if( ProcStatus[PROCESS_POS] & POS_ONLINE )
						{
							// Set Bit, To Inform POS Send Message
							ProcExchange[PROCESS_CR][PROCESS_POS] |= SEND_MESSAGE;
						}

						// Check Log Para Next
						Set_Next_Command( CR_RID_CARD_PROC, CR_PSAM_VERIFY_MSG_SINOPEC1 );
					}
					else
					{
						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_PSAM_VERIFY_MSG_SINOPEC1:
				
				retval = CR_PSAM_Select_Sinopec1( PSAM1 );

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Check Log Para Next
						Set_Next_Command( CR_RID_CARD_PROC, CR_RID_GET_LOG_PARA_TOTAL );
					}
					else
					{
						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_RID_GET_LOG_PARA_TOTAL:
				
				retval = CR_ICCard_RID_Get_Log_Para_Total();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Check Added Log Para
						Set_Next_Command( CR_RID_CARD_PROC, CR_RID_GET_LOG_PARA_ADDED );
					}
					else
					{
//						// Display " PSAM Initialize Binding Error"
//						FillInfoMsg( M_V_ReInsert_RID, 0, 0 );
//
						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
//
//						// Spring Out Card, And Normal IC Card Function Next, Anyway
//						Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_RID_GET_LOG_PARA_ADDED:
				
				retval = CR_ICCard_RID_Get_Log_Para_Added();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						if( Check_RID_Log_Para() == 0 )
						{
							// Can Append Log, Calc Init PSAM MAC Next
							Set_Next_Command( CR_RID_CARD_PROC, CR_PSAM_INIT_DES_ENCRYPT );
						}
						else
						{
							// Can Not Append Log
							// Power Off IC Card And Spring Out
							Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );

//							// Spring Out Card, And Normal IC Card Function Next
//							Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
						}
					}
					else
					{
//						// Display " PSAM Initialize Binding Error"
//						FillInfoMsg( M_V_ReInsert_RID, 0, 0 );
//
						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
//
//						// Spring Out Card, And Normal IC Card Function Next, Anyway
//						Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_PSAM_INIT_DES_ENCRYPT:
				
				retval = CR_PSAM_Init_DES_RID_Log( PSAM1 );

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Fill Buffer
						FillRIDLogMACBuffer();

						// Can Append Log, Calc PSAM MAC Next
						Set_Next_Command( CR_RID_CARD_PROC, CR_PSAM_DES_ENCRYPT0 );
					}
					else
					{
//						// Display " PSAM Initialize Binding Error"
//						FillInfoMsg( M_V_ReInsert_RID, 0, 0 );
//
						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
//
//						// Spring Out Card, And Normal IC Card Function Next, Anyway
//						Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_PSAM_DES_ENCRYPT0:
				
				retval = CR_PSAM_DES_Encrypt0( PSAM1 );

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Save RID Append Log MAC
						memcpy( TRINP.TR_RID_ENCRYPTED, &RecvBuffer[9], 8 );

						// Fill Buffer For Log File
						FillRIDLogMACBuffer();
						// Add MAC
						memcpy( &RecvBuffer[128+RecvBuffer[127]], TRINP.TR_RID_ENCRYPTED, 4 );
						RecvBuffer[127]	+= 4;

						
						// Can Append Log, Calc PSAM MAC Next
						Set_Next_Command( CR_RID_CARD_PROC, CR_RID_APPEND_LOG );
					}
					else
					{
//						// Display " PSAM Initialize Binding Error"
//						FillInfoMsg( M_V_ReInsert_RID, 0, 0 );
//
						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
//
//						// Spring Out Card, And Normal IC Card Function Next, Anyway
//						Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_RID_APPEND_LOG:
				
				retval = CR_ICCard_RID_Append_Log();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
//						// Append Log OK, And Normal IC Card Function Next
//						Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );

						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
					else
					{
//						// Display " PSAM Initialize Binding Error"
//						FillInfoMsg( M_V_ReInsert_RID, 0, 0 );
//
						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_RID_CARD_PROC, CR_CARDREADER_POWEROFF_CARD );
//
//						// Spring Out Card, And Normal IC Card Function Next, Anyway
//						Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
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




/*
 ** Check_RID_Log_Para
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\RIDCardProc.c
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION: Check Log Space In RID Card
 *
 *  RETURNS: 0: At Least 1 Space For The RID Card, Can Append Log
 *			 1: No Space For Log Any More, Just Go On....
 *
 */
unsigned short int Check_RID_Log_Para( void )
{
	unsigned short int retval, ParaTotal, ParaAdded;

	// Default Error
	retval = 1;

	ParaTotal = CharToShortIntCopy( TRINP.TR_RID_LOG_PARA_TOTAL );

	ParaAdded = CharToShortIntCopy( TRINP.TR_RID_LOG_PARA_ADDED );

	if( ParaTotal != ParaAdded )
	{
		retval = 0;
	}

	return retval;
}


/*
 ** FillRIDLogMACBuffer
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\RIDCardProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Fill RID Card Log MAC To RecvBuffer For Calc MAC
 *
 *  RETURNS: None.
 *
 *	Atten: RecvBuffer[127]: Data Length; RecvBuffer[128].... Data;
 *
 */
void FillRIDLogMACBuffer( void )
{
	unsigned char len = 0;

	// Clear Buffer
	memset( &RecvBuffer[127], 0x00, 80 );

	// Copy Data To The Buffer
	// Date
	memcpy( &RecvBuffer[128+len], TRINP.TR_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));
	len += sizeof( TRINP.TR_TRANS_DATE );

	// Time
	memcpy( &RecvBuffer[128+len], TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));
	len += sizeof( TRINP.TR_TRANS_TIME );

	// ACT Card ASN
	memcpy( &RecvBuffer[128+len], TRINP.TR_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));
	len += sizeof( TRINP.TR_CARD_ASN );

	// ACT Key Index
	RecvBuffer[128+len] = TRINP.TR_ACT_KEY_INDEX;
	len += 1;

	// RID Card ASN
	memcpy( &RecvBuffer[128+len], TRINP.TR_RID_CARD_ASN, sizeof( TRINP.TR_RID_CARD_ASN ));
	len += sizeof( TRINP.TR_RID_CARD_ASN );

	// RID Key Index
	RecvBuffer[128+len] = TRINP.TR_RID_KEY_INDEX;
	len += 1;

	// RID Key Version
	RecvBuffer[128+len] = TRINP.TR_RID_KEY_VERSION;
	len += 1;

	// PSAM Card ASN
	memcpy( &RecvBuffer[128+len], TRINP.TR_PSAM_ASN, sizeof( TRINP.TR_PSAM_ASN ));
	len += sizeof( TRINP.TR_PSAM_ASN );

	// Pump ID
	memcpy( &RecvBuffer[128+len], Pump_CR_Buf.PUMPSerialNumber, sizeof( Pump_CR_Buf.PUMPSerialNumber ));
	len += sizeof( Pump_CR_Buf.PUMPSerialNumber );

	// Fill Lenght
	RecvBuffer[127] = len;
}

#endif