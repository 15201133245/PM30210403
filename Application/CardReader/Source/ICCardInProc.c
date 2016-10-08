#include <string.h>

#include "global.h"

#include "ICCardCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "ICCardINProc.h"
#include "define.h"
//#include "CRMessage.h"
#include "CRSFunc.h"
#include "Message.h"
#include "Screen.h"
#include "Display.h"
#include "Overfuel.h"
#include "PdProtect.h"

//Douzhq 20140610 Add , IC Card Recognize
#ifndef MAKE_CASH_CR
	#include "SocialCard.h"
	#include "CRVaribles.h"
#else
	#include "SocialCard.h"
//	#include "WriteLog.h"
#endif

#ifdef MAKE_CARD_MODE2

#include "POSMode2.h"

#endif

	#include "WriteLog.h"




// For Test Approval Delay
// Define Delay 400ms Before Issue Poll Card Type
unsigned char CardDelaySet = 0;
unsigned char CardDelayed = 0;

//Douzhq 20140812 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
unsigned char CarTryNumber = 0;
unsigned char CarNumberTmpBuf[30];
#endif

// 20151221 for zhonghangyou req begin
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
void card_sinopec_app_deal( void );
void card_loyalty_app_deal( void );
void card_saveoil_app_deal( void );
#endif

// 20160108
void WriteBlackCardLog( void );

void CardDelay( void )
{
	CardDelayed = 1;
}

// For Test Approval Delay End

//extern void ClearInterval( void );



/*
 ** ICCard_Inserted_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardProc.c
 *
 *  PARAMETERS: None.
 *
 *  DESCRIPTION: IC Card Was Inserted, Perform Power On IC Card
 *				 IC Card Reset, Select IC Card Applications, Read IC Card Information
 *				 Input PIN, Read Grey Message, Read Application Balance
 *				 Power Off IC Card And Spring Out IC Card
 *
 *  RETURNS: None.
 *
 */
void ICCard_Inserted_Proc( void )
{
	unsigned short int retval;

	// Only Running When CR_Proc_Step = CR_ICCARD_INSERT_PROC
	if( CR_Proc_Step == CR_ICCARD_INSERT_PROC )
	{
		switch( CR_CommandType )
		{
			// Check PSAM Application Validate Or Not First
			case CR_PSAM_CHECK_VALIDATE:

				// PSAM Validate Date And Expired Date Was Read Before In PSAM Porcess
				// Get TransTime For Card Validate Check
//				GetTransTime();

				if( PSAM_Check_Validate() == 0 )
				{
					if( IcTypeFlg == 0 )
					{
						// For Intelligence Card Reader, Get Card Type Next
//						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_GET_CARD_TYPE );

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
					}

//					// PSAM Validate, Go On....
//					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWERON_CARD );
				}
				else
				{
					// For Write Spring Card Log
					PowerDownFlag = Pd_Flag_190;

					// Spring Out IC Card
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_SPINGOUT_CARD );
				}
	
			break;


			case CR_ICCARD_DELAY:

				// Delay A Certain Time For Test Approval
		   		if( CardDelaySet == 0 )
				{
		   			CreateTimer( TIMER_ONE_DELETE, CardDelayTimeOut, CardDelay );
					CardDelaySet = 1;
				}
				else
				{
					if( CardDelayed == 1 )
					{
						CardDelaySet = 0;
						CardDelayed = 0;


#ifdef MAKE_CASH_CR
						// For Cash Card, Reset As Contact Card
						// Instead Of Get Card Type
						if( CRType == CR_TYPE_CASH )
						{
							CardType = CardContact;

							// Reset The IC Card Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_RESET );
						}
						else
#endif

						{
							// Delay Time Out, Go On....
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_GET_CARD_TYPE );
						}
					}
					else
					{
						// Loop
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_DELAY );
					}
				}

			break;

			case CR_ICCARD_GET_CARD_TYPE:

				// Power On
				retval = CR_Poll_CardType();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						if( CardType == CardContact )
						{
//							// Reset The IC Card Next
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_RESET );

// 20151217 qiyong add for zhonghangyou requirement
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
                            if (PowerDown.SaveOilAppMode == 0x01)
                            {
                                // Read SFI 15 Next
                                Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_FID_0X15 );
                            }
                            else
                            {
                                // Select Loyalty Application Next
                                Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_LOYALTY_APP );
                            }
#else
							// Select Loyalty Application Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_LOYALTY_APP );
#endif
						}
						else if( CardType == CardTypeA )
						{
//							// Reset The IC Card Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_POLL_TYPEA );

							// Select Loyalty Application Next
							//Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_LOYALTY_APP );
						}
						else if( CardType == CardTypeB )
						{
//							// Reset The IC Card Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_POLL_TYPEB );

							// Select Loyalty Application Next
							//Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_LOYALTY_APP );
						}
						else
						{
			  				// Display " Not Sinopec IC Card "
	  						FillInfoMsg( M_CardError, 0, 0 );

							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_198;

							// Power On Error
							// Spring Out IC Card
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_SPINGOUT_CARD );
						}
					}
					else
					{
		  				// Display " Not Sinopec IC Card "
  						FillInfoMsg( M_CardError, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_197;
                        ErrorFlag = retval;

						// Power On Error
						// Spring Out IC Card
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_SPINGOUT_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;

// 20151222 for zhonghangyou req
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			case CR_ICCARD_READ_FID_0X15:

				retval = CR_ICCARD_Read_FID_0x15();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
                    {
                        if(( TRINP.TR_CARD_FID15 != 0x01 )
                            && ( TRINP.TR_CARD_FID15 != 0x02 )
                            && ( TRINP.TR_CARD_FID15 != 0x03 ))
                        {
                            TRINP.TR_CARD_FID15 = 0xFF;
                        }
                    }
                    else
                    {
                        // FID not exist
                        TRINP.TR_CARD_FID15 = 0xFF;
                    }

                    // Select Loyalty Application Next
                    Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_LOYALTY_APP );
                }
                else
                {
					// Command Running, Go On
                }

                break;
#endif
			// IC Card Power On
			case CR_CARDREADER_POWERON_CARD:
				// Power On
				retval = CR_CardReaderPowerOnCard();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Get TransTime For Card Validate Check
//						GetTransTime();

						// Reset The IC Card Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_RESET );
					}
					else
					{
		  				// Display " IC Card Power On Error "
  						FillInfoMsg( M_CardPowerOnErr, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_191;
                        ErrorFlag = 0xE009;

						// Power On Error
						// Spring Out IC Card
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_SPINGOUT_CARD );
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

#ifdef MAKE_CASH_CR
					// For Cash Card, Clear It, Use Default IC Card
					if( CRType == CR_TYPE_CASH )
					{
						CRType = CR_TYPE_CARD;

						// Inform Cash Process
						if( !( ProcStatus[PROCESS_CR] & CARD_CASH_OK ))
						{
							if( HoldCard == 1 )
							{
								// Normal Or Grey EOT, Do Not Dispaly Error, Just Clear It
								HoldCard = 0;
							}
							else
							{	//Inform Error
				  				// Display " Cash Card Error, Retry "
		  						FillInfoMsg( M_CashCardRetry, 0, 0 );
								//Douzhq 20140917 Add , Voice Add
								ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_FAULTERR;
								
								
								// Can not Use
								Cash_CR_Buf.CashCard_Flag = 0x01;
									
								ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_KEY_PRESSED;
							}
						}
					}
#endif
					
					// Clear The Status Bits
					if( ProcStatus[PROCESS_CR] & CARD_LOGON )
					{
						ProcStatus[PROCESS_CR] = 0;
					
						ProcStatus[PROCESS_CR] |= CARD_LOGON;
					
						if( HoldCard == 1 )
						{
//							// Keep Balance Screen
//							ProcStatus[PROCESS_CR] |= CARD_READY;
							// Display Waiting Screen Next
							ProcStatus[PROCESS_CR] |= CARD_WAITING;
						}

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

					// Maybe Reserved Sping Out Card Flag
					if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & SPRING_CARD )
					{
						// Reserved Sping Out Card Flag
						// Clear Bit, And Do Not Response It
						ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~SPRING_CARD );
					}

#ifdef MAKE_CARD_MODE2
					// Clear Send Record After Trans, For New Card Insert
					if( SendCardRecord == 0x01 )
					{
						SendCardRecord = 0x00;
					}

					// Check Payment Location
					if( TransAmountPay == 0x01 )
					{
						// Clear Payment Location Set
						TransAmountPay = 0x00;
					}
#endif	// MAKE_CARD_MODE2

					if( retval == 0x9000 )
					{
                        //down added for over fueling process - 20110906
                        if(OverFuelFlg==1)
                        {
                            ProcStatus[PROCESS_CR]|=CARD_OVER_FURLING;
                            //Display: "over fuel,"
                            FillInfoMsg( M_OverFueling, 0, 0 );

                            Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_OVER_FUEL );
                        }
                        //up added for over fueling process - 20110906
                        else
                        {                        
                            // For Write Spring Card Log
                            PowerDownFlag = Pd_Flag_192;
                            ErrorFlag = retval;
    
                            // Spring Out IC Card Next
                            Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_SPINGOUT_CARD );
                        
    //						ClearInterval();
                        }
					}
					else if( retval == 0x0002 )
					{
						// Check PowerDown Or Not
						if( CRPowerFlag & CRPOWERDOWN )
						{
							// Clear The Bit
							CRPowerFlag &= ( ~CRPOWERDOWN );

							// Inform Power Down Process
							ProcExchange[PROCESS_CR][PROCESS_POWERDOWN] |= POWERDOWN; 
						}
						
						// No IC Card, Clear The Reserved PIN Info
						// Clear The Reserved PIN Information
						memset( PINInfo.CardASN, 0x00, sizeof( PININFO ));

						// No Card In The Reader, Do Not Spring Out
						// Card Reader Proc Next
						Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_193;
                        ErrorFlag = retval;

						// Power OFF Error
						// Spring Out IC Card Anyway
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_SPINGOUT_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;
            
            // down added for over fueling process - 20110906
            case CR_OVER_FUEL:                        
                    
                if(ProcStatus[PROCESS_CR]&CARD_OVER_FURLING_PASSWORD)
                {
                      // ProcStatus[PROCESS_CR]|=CARD_OVER_FURLING_PASSWORD;
                      // Display: "over fueling,"
                      FillInfoMsg( M_OverFueling, 0, 0 );

                      Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_OVER_FUEL_PASSWORD );   
                }
                             
			break;
            
            case CR_OVER_FUEL_PASSWORD:
                // Get Employee Password,spring card
                ProcStatus[PROCESS_CR] &= ( ~CARD_OVER_FURLING_PASSWORD );
			    
                retval = GetEmpPWD();
				
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						if( memcmp( KEYPAD_CR_Buf.ICCardEMPPwd, PowerDown.LogOnCard.EMPPassword, sizeof( PowerDown.LogOnCard.EMPPassword )) == 0 )
						{
							// Clear Input,
							KEYPAD_CR_Buf.ICCardEMPPwdLen = 0;
							memset( KEYPAD_CR_Buf.ICCardEMPPwd, 0x00, sizeof( KEYPAD_CR_Buf.ICCardEMPPwd ));
                                                        
                            OverFuelFlg = 0; 
                            OverFuelValue = 0;

                            // Employee Password Input Ok, Sping Out Card Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_SPINGOUT_CARD );
						}
						else
						{
//							//retry
							//Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_SPINGOUT_CARD );
						}
					}
					else
					{
						//retry
					}                    
				}
				else
				{
					// Command Running, Go On....
				}           
                          
			break;
            
            //up added for over fueling process - 20110906
            // IC Card Spring Out
			case CR_CARDREADER_SPINGOUT_CARD:
			  

		  	// Spring Out IC Card
		 	retval = CR_CardReaderSpringOutCard();

                
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						SpringOutCardTimes = 0;
						// Card Reader Proc Next
						Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
					}
					else
					{
						// Spring Out Error
						if( retval == 3 )
						{
							SpringOutCardTimes++;
						}
						else
						{
							SpringOutCardTimes = 0;
						}

						// Card Reader Proc Next
						Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
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
						// Contact IC Card
						CardType = CardContact;

// 20151217 qiyong add for zhonghangyou requirement
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
                        if (PowerDown.SaveOilAppMode == 0x01)
                        {
                            // Read FID 0x15 Next
                            Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_FID_0X15 );
                        }
                        else
                        {
                            // Select Loyalty Application Next
                            Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_LOYALTY_APP );
                        }
#else
                        // Select Loyalty Application Next
                        Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_LOYALTY_APP );
#endif
					}
//					else if( retval == 0x0003 )
//					{
//						// Try Poll Type A Contactless Card Next
//						// Select Loyalty Application Next
//						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_POLL_TYPEA );
//					}
					else
					{
		  				// Display " IC Card Maybe Inverted "
  						FillInfoMsg( M_CardInverted, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_36;
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
			
			// Poll Type A Contactless Card
			case CR_ICCARD_POLL_TYPEA:

				// Poll Type A 
				retval = CR_Poll_TypeA();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Contact IC Card
						CardType = CardTypeA;

						// Select Loyalty Application Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_LOYALTY_APP );
					}
//					else if( retval == 0x0003 )
//					{
//						// Try Poll Type A Contactless Card Next
//						// Select Loyalty Application Next
//						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_POLL_TYPEB );
//					}
					else
					{
		  				// Display " IC Card Maybe Inverted "
  						FillInfoMsg( M_CardError, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_71;
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

			// Poll Type A Contactless Card
			case CR_ICCARD_POLL_TYPEB:

				// Poll Type A 
				retval = CR_Poll_TypeB();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Contact IC Card
						CardType = CardTypeB;

						// Select Loyalty Application Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_LOYALTY_APP );
					}
					else
					{
		  				// Display " IC Card Maybe Inverted "
  						FillInfoMsg( M_CardError, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_72;
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

			// IC Card Select Loyalty Application
			case CR_ICCARD_SELECT_LOYALTY_APP:
				
				retval = CR_ICCARD_Select_Loyalty_APP();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// There Is Loyalty Application
						ProcStatus[PROCESS_CR] |= CARD_LOYALTY_APP;

						// Select Sinopec Application Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_SINOPEC_APP );
					}
					else if(( retval == 0x6A81 ) || ( retval == 0x9303 ))
					{
						// There Is Loyalty Application, But Was Blocked
						ProcStatus[PROCESS_CR] |= CARD_LOYALTY_APP;
						ProcStatus[PROCESS_CR] |= CARD_LOYALTY_APP_BLOCK;

						// Select Sinopec Application Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_SINOPEC_APP );
					}
					else if( retval == 0x6A82 )
					{
						// There Is Not Loyalty Application
						// Reset The IC Card Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_SINOPEC_APP );
					}
					else
					{
						if(( CardType == CardTypeA ) || ( CardType == CardTypeB ))
						{
							// For Contactless Card, Select SINOPEC Application Anyway
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_SINOPEC_APP );
						}
						else
						{
							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_37;
                            ErrorFlag = retval;

							// Display " Not Sinopec IC Card "
							FillInfoMsg( M_CardError, 0, 0 );

							// Run Error
							// Power Off IC Card And Spring Out
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
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

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// There Is Sinopce Application
						ProcStatus[PROCESS_CR] |= CARD_SINOPEC_APP;

						// Determine The Used Application Next
						
//Douzhq 20140529 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
						if (PowerDown.SaveOilAppMode == 0x01)
						{
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_SAVEOIL_APP );						
						}
						else
						{
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_USED_APP );
						}
#else
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_USED_APP );
#endif
					}
					else if(( retval == 0x6A81 ) || ( retval == 0x9303 ))
					{
						// There Is Sinopec Application, But Was Blocked
						ProcStatus[PROCESS_CR] |= CARD_SINOPEC_APP;
						ProcStatus[PROCESS_CR] |= CARD_SINOPEC_APP_BLOCK;
//Douzhq 20140711 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
						if (PowerDown.SaveOilAppMode == 0x01)
						{
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_SAVEOIL_APP );						
						}
						else
						{
							// Determine The Used Application Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_USED_APP );
						}
#else
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_USED_APP );
#endif		
					}
					else if( retval == 0x6A82 )
					{
//Douzhq 20140711 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
						if (PowerDown.SaveOilAppMode == 0x01)
						{
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_SELECT_SAVEOIL_APP );						
						}
						else
						{
							// Determine The Used Application Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_USED_APP );
						}
#else
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_USED_APP );
#endif		
					}
					else
					{
		  				// Display " IC Card Select App Error "
  						FillInfoMsg( M_CardSelectAppErr, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_38;
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
			
//Douzhq 20140612 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			case CR_ICCARD_SELECT_SAVEOIL_APP:
				
				retval = CR_ICCARD_Select_Saveoil_APP();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// There Is Saveoil Application
						ProcStatus[PROCESS_CR] |= CARD_SAVEOIL_APP;

						// Select Saveoil Application Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_USED_APP );
					}
					else if(( retval == 0x6A81 ) || ( retval == 0x9303 ))
					{
						// There Is Saveoil Application, But Was Blocked
						ProcStatus[PROCESS_CR] |= CARD_SAVEOIL_APP;
						ProcStatus[PROCESS_CR] |= CARD_SAVEOIL_APP_BLOCK;

						// Select Saveoil Application Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_USED_APP );
					}
					else if( retval == 0x6A82 )
					{
						// There Is Not Saveoil Application
						// Reset The IC Card Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_USED_APP );
					}
					else
					{
						if(( CardType == CardTypeA ) || ( CardType == CardTypeB ))
						{
							// For Contactless Card, Select SINOPEC Application Anyway
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_USED_APP );
						}
						else
						{
							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_37;
                            ErrorFlag = retval;

							// Display " Not Sinopec IC Card "
							FillInfoMsg( M_CardError, 0, 0 );

							// Run Error
							// Power Off IC Card And Spring Out
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;
#endif
			case CR_ICCARD_USED_APP:

//----------------------------------------------------------------//
//Douzhq 20140715 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
                // 20151221 for zhonghangyou req begin
                if(( PowerDown.SaveOilAppMode == 0x01 ) 
                   && ( TRINP.TR_CARD_FID15 != 0xFF ))
                {
                    if( TRINP.TR_CARD_FID15 == 0x01 )
                    {
						if( ProcStatus[PROCESS_CR] & CARD_SINOPEC_APP )
						{
							if( !( ProcStatus[PROCESS_CR] & CARD_SINOPEC_APP_BLOCK ))
							{
								retval = CR_ICCARD_Select_Sinopec_APP();
								
								if( retval != 1 )
								{
									if( retval == 0x9000 )
									{
										Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X15 );
                                        break;
									}
								}
                                else
                                {
                                    // do nothing
                                    break;
                                }
                            }
                        }
                    }
                    else if( TRINP.TR_CARD_FID15 == 0x02 )
                    {
						if( ProcStatus[PROCESS_CR] & CARD_LOYALTY_APP )
						{
							if( !( ProcStatus[PROCESS_CR] & CARD_LOYALTY_APP_BLOCK ))
							{
								retval = CR_ICCARD_Select_Loyalty_APP();
								
								if( retval != 1 )
								{
									if( retval == 0x9000 )
									{
										Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X15 );
                                        break;
									}
								}
                                else
                                {
                                    // do nothing
                                    break;
                                }
                            }
                        }
                    }
                    else if( TRINP.TR_CARD_FID15 == 0x03 )
                    {
						if( ProcStatus[PROCESS_CR] & CARD_SAVEOIL_APP )
						{
							if( !( ProcStatus[PROCESS_CR] & CARD_SAVEOIL_APP_BLOCK ))
							{
                                Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X15 );
                                break;
                            }
                        }
                    }
                }
                // 20151221 for zhonghangyou req end
                
				if ( ProcStatus[PROCESS_CR] & CARD_SAVEOIL_APP )
				{
                    if( ProcStatus[PROCESS_CR] & CARD_SINOPEC_APP )
                    {
                        card_sinopec_app_deal();
                    }
                    else if( ProcStatus[PROCESS_CR] & CARD_LOYALTY_APP )
                    {
                        card_loyalty_app_deal();
                    }
                    else
                    {
                        card_saveoil_app_deal();
                    }
                }
				else
#endif
				{
					
				
				
				
//---------------------------------------------------------------------
				
				// Default Application Is Sinopec Application, And Must Have SINOPEC Application
				if( ProcStatus[PROCESS_CR] & CARD_SINOPEC_APP )
				{
					if( !( ProcStatus[PROCESS_CR] & CARD_SINOPEC_APP_BLOCK ))
					{
						// Does Not Blocked, Use Sinopce Application And Go On
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X15 );
					}
					else
					{
						if( ProcStatus[PROCESS_CR] & CARD_LOYALTY_APP )
						{
							// There Is Loyalty Application
							if( !( ProcStatus[PROCESS_CR] & CARD_LOYALTY_APP_BLOCK ))
							{
								// Does Not Blocked, Use Loyalty Application And Go On
								// Re-Select Loyalty Application
								retval = CR_ICCARD_Select_Loyalty_APP();
							
								// Check Response
								if( retval != 1 )
								{
									if( retval == 0x9000 )
									{
										// Select Loyalty Application, Go On
										Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X15 );
									}
									else
									{
										// For Write Spring Card Log
										PowerDownFlag = Pd_Flag_39;
                                        ErrorFlag = retval;

										// "Display: Card Error", And Spring Out Next
										Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
									}
								}
								else
								{
									// Running, Go On
								}
							}
							else
							{
								// Both 2 Application Were Blocked, Spring Out Next
								// "Display: Card Blocked"
 		 						FillInfoMsg( M_CardAppBlocked, 0, 0 );

								// For Write Spring Card Log
								PowerDownFlag = Pd_Flag_40;

								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
							}
						}
						else
						{
							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_41;

							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
					}
				}
				else
				{
					// Display " IC Card Error"
					FillInfoMsg( M_CardError, 0, 0 );

					// For Write Spring Card Log
					PowerDownFlag = Pd_Flag_42;

					// Run Error
					// Power Off IC Card And Spring Out
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
				}
				
				
				
				
//--------------------------------------------------------------	
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

#ifdef MAKE_PSAM_VERIFY
						// Check ACT Inserted Here For PSAM Verify Register
						if( TRINP.TR_CARD_ASN[2] == ACT_CARD )
						{
							// PSAM Register Next
							// Display Insert ACT Card
							NextScreenID = SCR_PSAM_V_UNREGISTERED;
							Set_Next_Command( CR_ACT_CARD_PROC, CR_CARDREADER_CHECK_CARD );

							break;
						}
						else if( TRINP.TR_CARD_ASN[2] == RID_CARD )
						{
 							// Display ACT Not Ready
 							FillInfoMsg( M_V_ACT_Not_Ready, 0, 0 );

							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );

							break;
						}
						else
						{
						}
#endif

#ifdef MAKE_CASH_CR
						// For Cash Card, Do Not Support Customer Card
						if( CRType == CR_TYPE_CASH )
						{
							if( TRINP.TR_CARD_ASN[2] != EMPLOYEE_CARD )
							{
								// Display: " Control Card Error "
								// Do Not Support Customer Card
								FillInfoMsg( M_ERROR_STOP, 0, 0 );
						
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
							
								break;
							}
						}
#endif

						// Check Local Card Or Not
						if( TRINP.TR_PSAM_TID[1] != ((( TRINP.TR_CARD_ASN[3] & 0x0F ) * 0x10 ) + (( TRINP.TR_CARD_ASN[4] & 0xF0 ) >> 4 )))
						{
							// Not Local Card, Do Not Support Loyalty Application
							ProcStatus[PROCESS_CR] &= ( ~CARD_LOYALTY_APP );
							ProcStatus[PROCESS_CR] &= ( ~CARD_LOYALTY_APP_BLOCK );
						
							// Check Wether Sinopec Application Blocked
							if( ProcStatus[PROCESS_CR] & CARD_SINOPEC_APP_BLOCK )
							{
								// Card Blocked, Return
								// "Display: Card Blocked"
 	 							FillInfoMsg( M_CardAppBlocked, 0, 0 );

								// For Write Spring Card Log
								PowerDownFlag = Pd_Flag_43;
                                ErrorFlag = retval;

								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
		
								break;
							}
							else
							{
								// Go On.... Do Nothing
							}
						}
						else
						{
							// Go On.... Do Nothing
						}

						// Set Card In
						// Set Bit, To Inform POS IC Card Inserted
						ProcExchange[PROCESS_CR][PROCESS_POS] |= SENDCARDIN;
						
//						// Check Manage Card Or Not
//						if( TRINP.TR_CARD_ASN[2] == MANAGE_CARD )
//						{
//							// Display " IC Card Error"
//							FillInfoMsg( M_ManageCardLimit, 0, 0 );
//
//							// For Write Spring Card Log
//							PowerDownFlag = Pd_Flag_44;
//
//							// Display: " Manage Card Can Not Refuel "
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
//						
//							// Exit,
//							break;
//						}
//						
						// Fill The POS-CardReader Buffer, IC Card ASN
						memcpy( POS_CR_Buf.ICCARD_ASN, TRINP.TR_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));

#ifdef MAKE_CARD_MODE2
						// Special For Test In SHYK, For S/D
						// Get Logical NozzleNumber
						POS_CR_Buf.TransNozzleNumber = GetLogicNzn( 0 );
								
						// Get Price
						retval = GetPrice( 0, 0 );
						ShortIntToCharCopy( retval, POS_CR_Buf.TransPrice );

#endif

						// Check IC Card Validation
//						if( Card_Validate_Check() == 0 )
						retval = Card_Validate_Check();
						if( retval == 0 )
						{
							// Inform POS Validate Card, Default
							memcpy( POS_CR_Buf.ICCARD_STATE, "\x00\x00", sizeof( POS_CR_Buf.ICCARD_STATE ));
						

							// Check White List Next, For Non Customer Card
							if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
							{
								// For Customer Card, Do Not Check Black List
								// For The Check Black List Need PIN Verification
								// So, Check Black List After The PIN Verify
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X16 );
							}
							else
							{
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CHECK_WHITE_LIST );
							}

#ifdef MAKE_CARD_MODE2
							if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
							{
								if( TransAmountPay == 0x01 )
								{
									// For Customer Card, Do Not Change Payment Location
									TransAmountPay = 0x00;
								}
								else
								{
									// Do Nothing, Go On....
								}
							}
							else
							{
								// Employee Card
								// Do Nothing, Go On.... 
							}
#endif

						}
						else if( retval == 1 )
						{
							// Inform POS Validate Card, Default
							memcpy( POS_CR_Buf.ICCARD_STATE, "\x00\x00", sizeof( POS_CR_Buf.ICCARD_STATE ));

							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_74;

							// Card Error
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
						else
						{
							// Inform POS Not Validate Card
							memcpy( POS_CR_Buf.ICCARD_STATE, "\x00\x02", sizeof( POS_CR_Buf.ICCARD_STATE ));
						
							// For Record Not Validate Card
							memcpy( TRINP.TR_CARD_GREY_TAC, "\x00\x02\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
						
							SaveICCardData();

							// Save It For Record
//							FillCardErrorData();

		  					// Display " IC Card Expired "
// 							FillInfoMsg( M_CardExpired, 0, 0 );

//							// Form Journal Next
//							Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );

							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_45;
                            ErrorFlag = retval;

							// Card Error
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_CardError, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_46;
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
			
			case CR_CHECK_BLACK_LIST:

#ifdef MAKE_CARD_MODE2
//				if( GetWorkMode() == 0x01 )
				if( TRINP.TR_WORK_MODE == 0x01 )
				{
					// For Work Online, Do Not Check Black List
					retval = 0x9000;
				}
				else
				{
					// For Work Offline, Check Black List
					retval = CR_Check_Black_List();
				}
#else
				// For Not Sinopec Mode2, Check Black List
				retval = CR_Check_Black_List();
#endif
				
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Fill The POS-CardReader Buffer, IC Card ASN
						memcpy( POS_CR_Buf.ICCARD_ASN, TRINP.TR_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));
						// If Vehicle Number Limited The Need Employee Password
						if( TRINP.TR_VEHICLE_NUMBER_LIMITED_FLAG == 1 )
						{
							//Douzhq 20140917 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
							if (ProcExchange[PROCESS_CR][PROCESS_CR] & CAR_GREY_FLAG)
							{
								ProcExchange[PROCESS_CR][PROCESS_CR] &= ~(CAR_GREY_FLAG);
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_GET_BALANCE );								
								break;
							}
#endif
//Douzhq 20140820 Add , Car Number Recognize 
#ifdef MAKE_CASH_CR
							if (PowerDown.CarNumRecognizeModw == 0x01)
							{
								if (!(ProcStatus[PROCESS_CR] & CARD_LOGON))
								{
									Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
									FillInfoMsg( M_OffDutyNotPreset, 0, 0 );
									break;
								}
								ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_CAR_ONCE);
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_CARNUM_RECOGNIZE );
								NextScreenID = DEFAULT_CARNUM_REC;
								CarOverFlag = 0;
								CarOverFlag2 = 0;
								if(CarNumberRecWait != (Tmr *)0)
								{
									DeleteTimer(CarNumberRecWait);
								}
							}
							else
							{
								// For Display
								memset( InfoLine, 0x00, sizeof( InfoLine ));
								memcpy( InfoLine, "限车号: ", 8 );
								memcpy( &InfoLine[8], TRINP.TR_CARD_VEHICLE_NUMBER, strlen(( char const * )TRINP.TR_CARD_VEHICLE_NUMBER ));
								// Display " Vehicle Number Limited"
								FillInfoMsg( M_InfoLine, 0, 1 );

								// Do Not Clear Untill Display Another
								StopDefaultInfo();

					
								// Get Employee Number Next
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_EMP_PASSWORD );
							}
#else
							// For Display
							memset( InfoLine, 0x00, sizeof( InfoLine ));
							memcpy( InfoLine, "限车号: ", 8 );
							memcpy( &InfoLine[8], TRINP.TR_CARD_VEHICLE_NUMBER, strlen(( char const * )TRINP.TR_CARD_VEHICLE_NUMBER ));
							// Display " Vehicle Number Limited"
							FillInfoMsg( M_InfoLine, 0, 1 );

							// Do Not Clear Untill Display Another
							StopDefaultInfo();

					
							// Get Employee Number Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_EMP_PASSWORD );
#endif
						}
						else
						{
							{
								// Get Balance Next, For The Check Black List
								// Should Be Processed After The PIN Verify
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_GET_BALANCE );
							}
						}
					}
					else if( retval == 0 )
					{
						// Inform POS Black Card
						memcpy( POS_CR_Buf.ICCARD_STATE, "\x00\x01", sizeof( POS_CR_Buf.ICCARD_STATE ));
						
						// For Record Black Card
						memcpy( TRINP.TR_CARD_GREY_TAC, "\x00\x01\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
						
						SaveICCardData();

		  				// Display " IC Card In Black List "
  						FillInfoMsg( M_CardBlackList, 0, 0 );
                        
                        // 20160108 for some card can not PIN-Lock use ffff
                        // Save log " IC Card In Black List "
                        WriteBlackCardLog();

						// IC Card Is Black, Lock PIN, Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_PIN_LOCK );
					}
					else
					{
		  				// Display " IC Card Need Check Black List OnLine "
  						FillInfoMsg( M_NeedPOSBlackList, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_47;
                        ErrorFlag = 0xE00A;

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
#ifdef MAKE_CASH_CR
		case CR_ICCARD_CARNUM_RECOGNIZE:	
			memset( InfoLine, 0x00, sizeof( InfoLine ));
			memcpy( InfoLine, "限车号: ", 8 );
			memcpy( &InfoLine[8], TRINP.TR_CARD_VEHICLE_NUMBER, strlen(( char const * )TRINP.TR_CARD_VEHICLE_NUMBER ));
			
			if (ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_CAR_ONCE)
			{
				//....
			}
			else
			{
				ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_CARE_RECOGNIZE;
				
				memset(CarNumGradeBuffer.CardAsn,0,sizeof(CarNumGradeBuffer.CardAsn));
				memcpy(CarNumGradeBuffer.CardAsn,TRINP.TR_CARD_ASN,sizeof(TRINP.TR_CARD_ASN));
				
				memset(CarNumGradeBuffer.CarNumber,0,sizeof(CarNumGradeBuffer.CarNumber));
				//memcpy(CarNumGradeBuffer.CarNumber,"京",strlen("京"));
				memcpy(CarNumGradeBuffer.CarNumber,TRINP.TR_CARD_VEHICLE_NUMBER,strlen(( char const * )TRINP.TR_CARD_VEHICLE_NUMBER ));
				
				//Write Car Number Log
				memset(CarNumberTmpBuf,0,sizeof(CarNumberTmpBuf));
				sprintf((char *)CarNumberTmpBuf,"CarNumberRec:");
				memcpy(&CarNumberTmpBuf[sizeof("CarNumberRec:")-1],CarNumGradeBuffer.CarNumber,strlen((char const *)CarNumGradeBuffer.CarNumber));
				WriteLog( PROCESS_CR,CarNumberTmpBuf );

				CarNumGradeBuffer.NozzleNumber = GetLogicNzn(0);
				
				ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_CARE_RECOGNIZE);
				
				CarTryNumber = 0;
			}
			
			//Douzhq 20140825 Add , Car Number Recognize
			if ((ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_CARE_RECOGNIZE) || CarOverFlag2 == 1)
			{
				if (CarOverFlag2 == 1)
				{
					CarOverFlag2 = 0;
					CarNumGradeBuffer.CarRecFlag = Cmd_CheckTimeOut;
				}
				
				StopTimer(CarNumberRecWait);
				CarOverFlag = 1;
				
				ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_CAR_ONCE);
				ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_CARE_RECOGNIZE);
				
				if (CarNumGradeBuffer.CarRecFlag == Cmd_CheckMatch)
				{
					if (memcmp(CarNumGradeBuffer.CarNumber,TRINP.TR_CARD_VEHICLE_NUMBER,strlen(( char const * )TRINP.TR_CARD_VEHICLE_NUMBER)+1) == 0)
					{
						// Car Number Matching
						FillInfoMsg( M_CarNumMatch, 0, 1 );
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_GET_BALANCE );	
						NextScreenID = DEFAULT_CARNUM_REC;
						//WriteLog(PROCESS_CR,"Car Number Is Matching");
					}
					else if(CarTryNumber < 1)
					{
						ResetTimer(CarNumberRecWait);
							
						CarTryNumber++;
					
						memset(CarNumGradeBuffer.CardAsn,0,sizeof(CarNumGradeBuffer.CardAsn));
						memcpy(CarNumGradeBuffer.CardAsn,TRINP.TR_CARD_ASN,sizeof(TRINP.TR_CARD_ASN));
				
						memset(CarNumGradeBuffer.CarNumber,0,sizeof(CarNumGradeBuffer.CarNumber));
						memcpy(CarNumGradeBuffer.CarNumber,TRINP.TR_CARD_VEHICLE_NUMBER,strlen(( char const * )TRINP.TR_CARD_VEHICLE_NUMBER ));

						CarNumGradeBuffer.NozzleNumber = GetLogicNzn(1);
					
						FillInfoMsg( M_CarNumMatchRecing, 0, 1 );
						
						CarNumGradeBuffer.CarRecFlag = Cmd_CheckWrong;
						ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_CARE_RECOGNIZE;
						
						CarOverFlag = 0;
						
						sprintf((char *)CarNumberTmpBuf,"CarNumberRec:");
						memcpy(&CarNumberTmpBuf[sizeof("CarNumberRec:")-1],CarNumGradeBuffer.CarNumber,strlen((char const *)CarNumGradeBuffer.CarNumber));
						WriteLog(PROCESS_CR,CarNumberTmpBuf);
					}
					else
					{
						CarTryNumber = 0;
						
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_EMP_PASSWORD );
						
						FillInfoMsg( M_InfoLine, 0, 1 );
						StopDefaultInfo();
						
						CarNumGradeBuffer.CarRecFlag = Cmd_CheckWrongErr;
						
						NextScreenID = DEFAULT_CARNUM_REC;
						//WriteLog(PROCESS_CR,"Car Number Is Wrong Two");
					}
				}
				else if (CarNumGradeBuffer.CarRecFlag == Cmd_CheckNotMatch)
				{
					// Not Exist This Car Number
					FillInfoMsg( M_CarNumNoMatch, 0, 1 );
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_EMP_PASSWORD );
				
					FillInfoMsg( M_InfoLine, 0, 1 );
					StopDefaultInfo();
					
					NextScreenID = DEFAULT_CARNUM_REC;
					//WriteLog(PROCESS_CR,"Car Number Is Not Match");
				}
				else if (CarNumGradeBuffer.CarRecFlag == Cmd_CheckTimeOut)
				{
					// Time Out Show EMP Password 
					//FillInfoMsg( M_CarNumRecTimeout, 0, 1 );
					FillInfoMsg( M_InfoLine, 0, 1 );
					StopDefaultInfo();
					
					//ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_CAR_ONCE);
					//ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_MSG_ERROR);
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_EMP_PASSWORD );
					
					NextScreenID = DEFAULT_CARNUM_REC;
					//WriteLog(PROCESS_CR,"Car Number : Time Out");
				}
			}
			else
			{
				CarNumGradeBuffer.CarRecFlag = Cmd_Checking;
				FillInfoMsg( M_CarNumMatchRecing, 0, 0 );
			}
			break;
#endif
			case CR_ICCARD_PIN_LOCK:

				 retval = CR_ICCARD_Verify_PIN();
				
				// Check Command Return
				if( retval != 1 )
				{
					if(( retval == 0x63C0 ) || ( retval == 0x6983 ))
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_48;
                        ErrorFlag = retval;

#ifdef MAKE_CARD_MODE2
						// Save It For Record
						FillCardErrorData_Mode2();
#else
						// Save It For Record
						FillCardErrorData();
#endif	// MAKE_CARD_MODE2

						// For Write The Running Log
						JournalFlag = 11;

						// Form Journal Next
						Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );

//						// Power Off IC Card And Spring Out
// 						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
					else if(( retval == 0x9000 ) || (( retval / 0x10 ) == 0x63C ))
                    {
						// Retry, Until Lock PIN
						TRINP.TR_INPUT_PIN_LEN = 0x02;
						TRINP.TR_INPUT_PIN[0] ++;
						TRINP.TR_INPUT_PIN[1] ++;
                    }
                    else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_48;
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
			
			case CR_WAITING_EMP_PASSWORD:

				// Get Employee Password
				retval = GetEmpPWD();
				
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						if( memcmp( KEYPAD_CR_Buf.ICCardEMPPwd, PowerDown.LogOnCard.EMPPassword, sizeof( PowerDown.LogOnCard.EMPPassword )) == 0 )
						{
							// Clear Input,
							KEYPAD_CR_Buf.ICCardEMPPwdLen = 0;
							memset( KEYPAD_CR_Buf.ICCardEMPPwd, 0x00, sizeof( KEYPAD_CR_Buf.ICCardEMPPwd ));

							// Employee Password Input Ok, Get Balance Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_GET_BALANCE );
						}
						else
						{
//							// Display: "EMP Password Error, Retry....."
//							FillInfoMsg( M_EMPPwdRetries, 0, 0 );
//
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_EMP_PASSWORD );

							// Employee PWD Error, Just Record And Spring Out Card According To Sinopec Specification
							// Inform POS Employee Password Error
							memcpy( POS_CR_Buf.ICCARD_STATE, "\x10\x06", sizeof( POS_CR_Buf.ICCARD_STATE ));
							
							// For Record Black Card
							memcpy( TRINP.TR_CARD_GREY_TAC, "\x10\x06\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
							
							SaveICCardData();


							// Save It For Record
							FillCardErrorData();

			  				// Display " Employee Password Error "
	  						FillInfoMsg( M_EmpPwdErr, 0, 0 );

							// For Write The Running Log
							JournalFlag = 12;

							// Form Journal Next
							Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
						}
					}
					else
					{
						// Input Cancelled, Do Not Form Record
//						// Inform POS Employee Password Error
//						memcpy( POS_CR_Buf.ICCARD_STATE, "\x10\x06", sizeof( POS_CR_Buf.ICCARD_STATE ));
//						
//						// For Record Black Card
//						memcpy( TRINP.TR_CARD_GREY_TAC, "\x10\x06\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
//						
//						SaveICCardData();
//
//						// Save It For Record
//						FillCardErrorData();
//
//		  				// Display " Employee Password Error "
//  						FillInfoMsg( M_EmpPwdErr, 0, 0 );
//
//						// Form Journal Next
//						Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_79;
                        ErrorFlag = retval;

						// Input Error, Spring Out Card
 						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			case CR_CHECK_WHITE_LIST:

				retval = CR_Check_White_List();
				
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Check Manage Card Or Not
						if( TRINP.TR_CARD_ASN[2] == MANAGE_CARD )
						{
							// Display " Manage Card Limited"
							FillInfoMsg( M_Test_Manage_Card1, 0, 0 );

							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_44;
                            ErrorFlag = 0xE00B;

							// Display: " Manage Card Can Not Refuel "
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						
//							// Exit,
//							break;
						}
						else
						{
							// Read SFI 0x16 Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X16 );
						}
					}
					else
					{
//		  				// Display " Employee Card Not In White List"
// 						FillInfoMsg( M_NotWhiteList, 0, 0 );

						if( TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD )
						{
							// Display " EMP Card But Not Belong To This Station"
							FillInfoMsg( M_Test_Not_EMP_Card, 0, 0 );
						}
						else if( TRINP.TR_CARD_ASN[2] == MANAGE_CARD )
						{
							// Display " Manage Card But Not Belong To This Station"
							FillInfoMsg( M_Test_Manage_Card0, 0, 0 );
						}
						else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
						{
							// Display " Test Card But Not Belong To This Station"
							FillInfoMsg( M_Test_Not_Test_Card, 0, 0 );
						}
						else if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
						{
							// Display " Maintain Card But Not Belong To This Station"
							FillInfoMsg( M_Test_Not_Maintain_Card, 0, 0 );
						}
						else
						{
							// Display " Not Belong To This Station"
							FillInfoMsg( M_Test_CardType_Err, 0, 0 );
						}

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_49;
                        ErrorFlag = 0xE00C;

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
			
			// IC Card Read SFI 0x16
			case CR_ICCARD_READ_SFI_0X16:
				
				retval = CR_ICCARD_Read_SFI_0x16();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Read SFI 0x1B Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X1B );
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_CardError, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_50;
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
			
			// IC Card Read SFI 0x1B
			case CR_ICCARD_READ_SFI_0X1B:
				
				retval = CR_ICCARD_Read_SFI_0x1B();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Check IC Card Data For Approval Test
						if( Card_Data_Check() != 0 )
						{
							// Error, Display Message Is Set In Function Card_Data_Check()

							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_73;

							// Display: " Manage Card Can Not Refuel "
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						
							// Exit,
							break;
						}

						// Check IC Card Data For Approval Test	End
						// Read SFI 0x1C Next

						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X1C );
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_CardError, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_51;
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
			// IC Card Read SFI 0x1C
			case CR_ICCARD_READ_SFI_0X1C:
				
#ifdef MAKE_CARD_MODE2
				// For Sinopec Mode2, Do Not Read Limit Infos For Loyalty App
				// Use Sinopec App's Limit Infos
				if( TRINP.TR_TRANS_DEBITSOURCE == 0x01 )
				{
					retval = 0x9000;
				}
				else
				{
					retval = CR_ICCARD_Read_SFI_0x1C();
				}
#else
				retval = CR_ICCARD_Read_SFI_0x1C();
#endif

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Save IC Card State For POS And Record
						Save_ICCard_State();

						if( TRINP.TR_CARD_PIN_DEFAULT == 0 )
						{
							// Display " IC Card PIN Please"
							FillInfoMsg( M_ENTER2, 0, 0 );

							// For Default Pin, Need Press ENTER Key Next
//Douzhq 20140529 Add , Lock Nozzle 
							if (LOCK_NOZZLE.LockFlag == 1 && TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD)	
							{
								Set_Next_Command( CR_ICCARD_INSERT_PROC,CR_WAITING_UNLOCK_PASSWORD );
							}
							else
							{
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_ENTER_KEY );
							}
							//Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_ENTER_KEY );
						}
						else
						{
							// Display " IC Card PIN Please"
							FillInfoMsg( M_CARD_PIN, 0, 0 );
							
							// Get PIN Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_PIN );
						}
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_CardError, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_52;
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
//Douzhq 20140529 Add , Lock Nozzle 
			case CR_WAITING_UNLOCK_PASSWORD:
				if( !( CR_Task_Flag & ICCARD_ENTER_KEY_K ))
				{
				  	CR_Task_Flag |= ICCARD_ENTER_KEY_K;
					ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= UNLOCK_PASSWORD;
				}
				else
				{
					if (ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & UNLOCK_PASSWORD)
					{
						ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ~(UNLOCK_PASSWORD);
						CR_Task_Flag &= ( ~ICCARD_ENTER_KEY_K );
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_ENTER_KEY );
						if (ProcExchange[PROCESS_CR][PROCESS_CR] & UNLOCK_POWERWD)
						{
							ProcExchange[PROCESS_CR][PROCESS_CR] &= ~(UNLOCK_POWERWD);
						}
					}
					else
					{
						ProcExchange[PROCESS_CR][PROCESS_CR] |= UNLOCK_POWERWD;
						//....
					}
				}
				break;
			case CR_WAITING_ENTER_KEY:
				
				// Need Press ENTER Key
				if( !( CR_Task_Flag & ICCARD_ENTER_KEY_K ))
				{
					// Update Card Message, Do Not Press ENTER Key
					if( ProcStatus[PROCESS_CR] & CARD_WAITING )
					{
						// Get PIN Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_PIN );
					}
#ifdef MAKE_CASH_CR
					// Cash Trans, Do Not Press ENTER Key
					else if( CRType == CR_TYPE_CASH )
					{
						// Get PIN Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_PIN );
					}
#endif
					else
					{
						// Set Bit11, Local Process Flag
						CR_Task_Flag |= ICCARD_ENTER_KEY_K;

						// Set Bit, To Inform Keypad Press ENTER Key
						
						ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= ICCARD_ENTER_KEY;
					}
				}
				else
				{
					// Second Check The Input, By Check ProcExchange[PROCESS_KEYPAD][PROCESS_CR]
					if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & ICCARD_ENTER_KEY )
					{
						// PIN Inputed By Keypad
						// Clear The Bit
						ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~ICCARD_ENTER_KEY );
					
						// Clear Local Process Flag
						CR_Task_Flag &= ( ~ICCARD_ENTER_KEY_K );

						// Get PIN Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_PIN );
					}
					else
					{
						// Wait Input, Go On
					}
				}

			break;

			// IC Card Get PIN
			case CR_WAITING_PIN:

				if( TRINP.TR_CARD_PIN_DEFAULT == 0 )
				{
					// Use Default PIN
					TRINP.TR_INPUT_PIN_LEN = ICCard_Default_PIN_Len;
					TRINP.TR_INPUT_PIN[0] = ICCard_Default_PIN[0];
					TRINP.TR_INPUT_PIN[1] = ICCard_Default_PIN[1];

					// Verfiy PIN Next
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_VERIFY_PIN );
				}
				else
				{
					// Use Input PIN
					// First Set Get PIN Flag In ProcExchange[PROCESS_CR][PROCESS_KEYPAD];
					if( !( CR_Task_Flag & ICCARD_PIN_K ))
					{
						// Check PIN Input Or Not
						if( memcmp( PINInfo.CardASN, TRINP.TR_CARD_ASN, sizeof( PINInfo.CardASN )) == 0 )
						{
							// Same Card, Do Not Input PIN
							TRINP.TR_INPUT_PIN_LEN = PINInfo.CardPINLen;
							memcpy( TRINP.TR_INPUT_PIN, PINInfo.CardPIN, TRINP.TR_INPUT_PIN_LEN );
				
							// Verfiy PIN Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_VERIFY_PIN );
						}
						else
						{
							// Check Loged On Emp Card Or Not
							if( ProcStatus[PROCESS_CR] & CARD_LOGON )
							{
								// Loged On, Check Same EMP Number Or Not
								if( TRINP.TR_CARD_EMP_NUMBER == PowerDown.LogOnCard.EMPNumber )
								{
									// Same EMP Number, Check Same Card Or Not
									if( memcmp( PowerDown.LogOnCard.EMPCardASN, TRINP.TR_CARD_ASN, sizeof( PowerDown.LogOnCard.EMPCardASN )) == 0 )
									{
										// Same EMP Card, Use Same PIN
										TRINP.TR_INPUT_PIN_LEN = PowerDown.LogOnCard.EMPPinLen;
										memcpy( TRINP.TR_INPUT_PIN, PowerDown.LogOnCard.EMPPin, TRINP.TR_INPUT_PIN_LEN );
				
										// Verfiy PIN Next
										Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_VERIFY_PIN );
									}
									else
									{
										// Set Bit0, Local Process Flag
										CR_Task_Flag |= ICCARD_PIN_K;

										// Set Bit, To Inform Keypad Input IC Card PIN
										ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= ICCARD_PIN;
										
										ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_ICPWD; //Douzhq 20141008 Add , Voice Add
									}
								}
								else
								{
									// Set Bit0, Local Process Flag
									CR_Task_Flag |= ICCARD_PIN_K;

									// Set Bit, To Inform Keypad Input IC Card PIN
									ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= ICCARD_PIN;
									
									// Loged On , And Is Not This Card
									ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_ICPWD; //Douzhq 20141008 Add , Voice Add
								}
							}
							else
							{
								// Set Bit0, Local Process Flag
								CR_Task_Flag |= ICCARD_PIN_K;

								// Set Bit, To Inform Keypad Input IC Card PIN
								ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= ICCARD_PIN;
								
								// Not Loged On 
								ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_ICPWD; //Douzhq 20141008 Add , Voice Add
							}
						}
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
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_VERIFY_PIN );
							}
							else
							{
								// For Write Spring Card Log
								PowerDownFlag = Pd_Flag_53;

								// Input Canceled
								// Power Off IC Card And Spring Out
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
							}
						}
						else
						{
							// Wait Input, Go On
						}
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
						// Clear The Reserved PIN Information
//						memset( PINInfo.CardASN, 0x00, sizeof( PININFO ));

						// Save PIN Information To Avoid Re-Input After Grey Unlock
						// Or Non Logoned Employee Card Re-Input
						PINInfo.CardPINLen = TRINP.TR_INPUT_PIN_LEN;
						memcpy( PINInfo.CardPIN, TRINP.TR_INPUT_PIN, TRINP.TR_INPUT_PIN_LEN );
						memcpy( PINInfo.CardASN, TRINP.TR_CARD_ASN, sizeof( PINInfo.CardASN ));

						if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
						{
							// For Customer Card, Read The Last IC Card Record Next
							// For Check Black List
							RecordListNumber = 1;
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_RECORD );
						}
						else
						{
							// Get Balance Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_GET_BALANCE );
						}

						memset( InfoLine, 0x00, sizeof( InfoLine ));
						// Display "    "
						FillInfoMsg( M_InfoLine, 0, 0 );
					}
					else if(( retval == 0x63C0 ) || ( retval == 0x6983 ))
					{
						// Display: "PIN Blocked"
						// Display " PIN Block"
						FillInfoMsg( M_CardPINBlocked, 0, 0 );

						
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_54;
                        ErrorFlag = retval;

						// Get Pin Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
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
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_PIN );
					}
					else
					{
						// Display " PIN Block"
						FillInfoMsg( M_CardPINError, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_55;
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
			
			// IC Card Read Application Balance
			case CR_ICCARD_GET_BALANCE:
				 
				 retval = CR_ICCARD_Get_Balance();
				
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Fill The POS-CardReader Buffer, IC Card Balance
						memcpy( POS_CR_Buf.ICCARD_Balance, TRINP.TR_CARD_APP_BALANCE, sizeof( TRINP.TR_CARD_APP_BALANCE ));

#ifdef MAKE_CASH_CR
						// For Cash Card, Inform Cash Process OK
						if( CRType == CR_TYPE_CASH )
						{
							// Check IC Card Balance
							if( CheckBalance() == 0 )
							{
								// Get Lock Proof Next
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_GET_LOCK_PROOF );
							}
							else
							{
								// Balance Not Enough
								// Power Off IC Card And Spring Out
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
							}
						}
						else
#endif
						{
							// Get Lock Proof Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_GET_LOCK_PROOF );
						}
					}
					else
					{
						// Run Error
						FillInfoMsg( M_GetBalanceError, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_56;
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
			
			// IC Card Read Lock Proof
			case CR_ICCARD_GET_LOCK_PROOF:

				 retval = CR_ICCARD_Get_Lock_Proof();
				
				// Check Command Return
				if( retval != 1 )
				{
					if(( retval == 0x9000 ) || ( retval == 0x6985 ))
					{
						// 0x6985 Means No Lock Trans Ocurred Before
						if( retval == 0x6985 )
						{
							// So, Clear TR_CARD_GREY_FLAG
							TRINP.TR_CARD_GREY_FLAG = 0;
						}
						
						// Check Grey Card Or Not
						if(( TRINP.TR_CARD_GREY_FLAG == 0x01 ) || ( TRINP.TR_CARD_GREY_FLAG == 0x10 ))
						{
//							// Save PIN Information To Avoid Re-Input After Grey Unlock
//							PINInfo.CardPINLen = TRINP.TR_INPUT_PIN_LEN;
//							memcpy( PINInfo.CardPIN, TRINP.TR_INPUT_PIN, TRINP.TR_INPUT_PIN_LEN );
//							memcpy( PINInfo.CardASN, TRINP.TR_CARD_ASN, sizeof( PINInfo.CardASN ));

							SaveICCardData();

							// Display " Grey Card, Waiting Unlock Debit"
							FillInfoMsg( M_CardGreyDebit, 0, 0 );
							
							//Douzhq 20140917 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
							ProcExchange[PROCESS_CR][PROCESS_CR] |= CAR_GREY_FLAG;
#endif
							
							// Check Grey Record, First
							Set_Next_Command( CR_ICCARD_GREY_CARD_PROC, CR_CHECK_GREY_RECORD );
						}
						else if( TRINP.TR_CARD_GREY_FLAG == 0x00 )
						{
							// For Select Application, Do Not Read The Balance First Time
							// Read The Balance Until The Select Application Was Over
							if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & SELECT_APP )
							{
								// Need Select Application Next,
								// Do Not Set Card Ready Flag Now.
							}
							else
							{
								// Set IC Card Ready
								ProcStatus[PROCESS_CR] |= CARD_READY;

#ifdef MAKE_CASH_CR
								// For Cash Card, Inform Cash Process OK
								if( CRType == CR_TYPE_CASH )
								{
									// Fill Message For CR-E
									FillCashCardOk();
								}
#endif
								
								// Inform Voice Process Only No CARD_WAITING
								if( !( ProcStatus[PROCESS_CR] & CARD_WAITING ))
								{
                                    if( CRType == CR_TYPE_CARD )
                                    {
                                        // 20160113 for daqing require
                                        if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
                                        {
                                            // IC Card Just Inserted, Inform Voice Process
                                            ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_CARD;
                                        }
                                    }
								}
								else
								{
                                    // 20160113 for daqing require
                                    if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
                                    {
                                        // Remained IC Card, Do Not Inform Voice Process
                                        //Douzhq 20140918 Add , Voice Add
                                        ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_CARD;
                                    }
								}

								// Clear Card Waiting
								ProcStatus[PROCESS_CR] &= ( ~CARD_WAITING );


								SaveICCardData();
								
								// Inform POS The TRINP.TR_TRANS_UNIT
								POS_CR_Buf.P_UNIT = TRINP.TR_TRANS_UNIT;

#ifdef MAKE_CARD_MODE2
								// Display Card Limited, Do Not Change
								CardLimitInfo();
#else
								// Display " Default Info Line"
								FillInfoMsg( M_DefaultInfo, 0, 0 );
#endif	// MAKE_CARD_MODE2

								// Update Display Balance
								NextScreenID = DEFAULT_SCR_BALANCE;
								ScreenID = NextScreenID;
								DrawBalanceSCR();
								//aaaaaaaaaa
								/*
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
								if ()
#endif*/

							}

#ifdef MAKE_CARD_MODE2
							// For Sinopec Mode2, Calc And Save Customer Card PIN Block First
							if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
							{
//								// Calc PIN Block Next
//								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_MAC );

								// Check Card Ready
								Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
							}
							else if( TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD )
							{
								// For Employee Card, Remark The TR_TRANS_DEBITSOURCE
								TRINP.TR_TRANS_DEBITSOURCE = 0x08;

								if( ProcStatus[PROCESS_POS] & POS_ONLINE )
								{
									// Check Card Ready
									Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
								}
								else
								{
									TransTTC = 1;

									// Append Offline Record Next
									Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_OFFLINE_RECORD_START );
//									// Check Card Ready
//									Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
								}
							}
							else
							{
								// Check Card Ready
								Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
							}
#else

#ifndef MAKE_CASH_CR
                            if( PowerDown.WorkManage == 0x01 )
                            {
                                if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
                                {
                                    // Clear IC Card Ready, Set It Again Until Input Emp Number
								    ProcStatus[PROCESS_CR] &= ( ~CARD_READY );
                                
                                    Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_EMP_NUMBER );
                                }
                                else
                                {
                                    PowerDown.FuelEmpNumber = TRINP.TR_CARD_EMP_NUMBER;
                                    
                                    Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
                                }
                            }
                            else
                            {
                                // First Inquire Card Exist Or Not
                                Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
                            }
#else
							//Douzhq 20140916 Add , Voice Add
							if (CRType == CR_TYPE_CASH)
							{
								ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_SCANCODE;
							}
							
                            // First Inquire Card Exist Or Not
                            Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
#endif // MAKE_CASH_CR
                            
#endif	// MAKE_CARD_MODE2
						}
						else
						{
							// Run Error
							FillInfoMsg( M_GetLockProofError, 0, 0 );

							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_75;

							// Power Off IC Card And Spring Out
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
					}
					else
					{
						// Run Error
						FillInfoMsg( M_GetLockProofError, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_57;
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
			
			// IC Card Read Trans Record In IC Card
			case CR_ICCARD_READ_RECORD:

				 retval = CR_ICCARD_Read_Record();
				
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Get Lock Proof Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CHECK_BLACK_LIST );
					}
					else if(  retval == 0x6A83 )
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_58;
                        ErrorFlag = retval;

						// No Record In IC Card
						FillInfoMsg( M_NeedRecord, 0, 0 );
						
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_59;
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
			
#ifdef MAKE_CARD_MODE2
			case CR_WAITING_MAC:

				// Prepare The Calc MAC Elements
				// MAC Process Return
				CR_MAC_CommandType = CR_WAITING_MAC_END;
				CR_Proc_MAC_Step = CR_ICCARD_INSERT_PROC;

				// MAC Type:
				CR_MACType = CR_MAC_TYPE_ENC;

				// Fill The Buffer
				MACDataLen = 8;
				memset( MACBuffer, 0x00, sizeof( MACBuffer ));
				GetPINData();

				// Calc Crypted, Next
				Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_SELECT_SINOPEC2 );
			
			break;

			case CR_WAITING_MAC_END: 

				// Save The Encrypted PIN, And Check Card Next
				memcpy( TRINP.TR_PIN_ENCRYPTED, TRINP.TR_TRANS_MAC, sizeof( TRINP.TR_PIN_ENCRYPTED ));

//				// First Inquire Card Exist Or Not
//				Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );

				// Return To Pump Call, Next
				Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_WAITING_PIN );

			break;


			case CR_OFFLINE_RECORD_START:
				
				// Check Append Or Not
				if( CardAppendOrNot() == 0 )
				{
					// No Record Need To Be Appended
					// Append End Next
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_OFFLINE_RECORD_END );
				}
				else
				{
					if( ReadRecordAppend() == 1 )
					{
						// Prepare The Calc MAC Data
						// MAC Process Return
						CR_MAC_CommandType = CR_OFFLINE_MAC;
						CR_Proc_MAC_Step = CR_ICCARD_INSERT_PROC;

						// MAC Type:
						CR_MACType = CR_MAC_TYPE_MAC;

						// Display " Append Record Number, "
						memset( InfoLine, 0x00, sizeof( InfoLine ));
						memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
						memcpy( InfoLine, DISPMSG[M_RecordToEMPCard], strlen( DISPMSG[M_RecordToEMPCard] ));
						sprintf(( char * )CustomMsg1, "%4u", TransTTC );
						memcpy( &InfoLine[6], CustomMsg1, strlen(( char * )CustomMsg1 ));

						FillInfoMsg( M_InfoLine, 0, 0 );

						// Calc Crypted, Next
						Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_SELECT_SINOPEC2 );
					}
					else
					{
						// No Record Need To Be Appended
						// Append End Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_OFFLINE_RECORD_END );
					}
				}

			break;

			case CR_OFFLINE_MAC:
			
				// Get Journal
				memcpy( &RecvBuffer[128], &MACBuffer[8], sizeof( JOURNAL ) - 1 );
				RecvBuffer[127]	= sizeof( JOURNAL ) - 1;
//				memcpy( &RecvBuffer[128+95], TRINP.TR_TRANS_MAC, 4 );
//				RecvBuffer[127]	= sizeof( JOURNAL ) - 1 + 4;

 				// Append End Next
 				Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_OFFLINE_RECORD_APPEND );
			
			break;

			case CR_OFFLINE_RECORD_APPEND:

				retval = CR_ICCard_Append_Log();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						MakeExportRecord();

						TransTTC++;

						// Append Next Record Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_OFFLINE_RECORD_START );
					}
					else if( retval == 0x9407 )
					{
						// Display " IC Card Full, "
						FillInfoMsg( M_RecordToEMPCardFull, 0, 0 );

						// Append Next Record Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
					else
					{
						// Display " IC Card Error, "
						FillInfoMsg( M_RecordToEMPCardError, 0, 0 );

						// Append Next Record Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}

			break;

			case CR_OFFLINE_RECORD_END:

				// Display " Card Ready, "
				FillInfoMsg( M_DefaultInfo, 0, 0 );

				// First Inquire Card Exist Or Not
				Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );

			break;


#endif	// MAKE_CARD_MODE2
                      
            // IC Card Need Input Emp Number
            case CR_WAITING_EMP_NUMBER:
                
                // Use Input PIN
                // First Set Get PIN Flag In ProcExchange[PROCESS_CR][PROCESS_KEYPAD];
                if( !( CR_Task_Flag & ICCARD_EMP_NUMBER_K ))
                {
                    // Set Bit0, Local Process Flag
                    CR_Task_Flag |= ICCARD_EMP_NUMBER_K;

                    // Set Bit, To Inform Keypad Input IC Card PIN
                    ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= EMP_NUMBER;
                }
                else
                {
                    // Second Check The Input, By Check ProcExchange[PROCESS_KEYPAD][PROCESS_CR]
                    if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & EMP_NUMBER )
                    {
                        // PIN Inputed By Keypad
                        // Clear The Bit
                        ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~EMP_NUMBER );
                
                        // Clear Local Process Flag
                        CR_Task_Flag &= ( ~ICCARD_EMP_NUMBER_K );

                        // Check Input Ok Or Cancel
                        if( KEYPAD_CR_Buf.FuelEmpNumberLen != 0 )
                        {            
                            // Set IC Card Ready
							ProcStatus[PROCESS_CR] |= CARD_READY;
                                
                            // Clear Input
                            KEYPAD_CR_Buf.FuelEmpNumberLen = 0;
                            
                            // Update Display Balance
                            NextScreenID = DEFAULT_SCR_BALANCE;
                            ScreenID = NextScreenID;
                            DrawBalanceSCR();

                            // Check Card Next
                            Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
                        }
                        else
                        {
                            // For Write Spring Card Log
                            PowerDownFlag = Pd_Flag_200;

                            // Input Canceled
                            // Power Off IC Card And Spring Out
                            Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
                        }
                    }
                    else
                    {
                        // Wait Input, Go On
                    }
                }
            break;

			default:
			break;
		}
	}
}

// 20151221 for zhonghangyou req begin
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
void card_app_select_poweroff( unsigned short int retval )
{
    // For Write Spring Card Log
    PowerDownFlag = Pd_Flag_201;
    ErrorFlag = retval;

    // "Display: Card Error", And Spring Out Next
    Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
}

void card_sinopec_app_deal( void )
{
	unsigned short int retval;

    if( !( ProcStatus[PROCESS_CR] & CARD_SINOPEC_APP_BLOCK ))
    {
        retval = CR_ICCARD_Select_Sinopec_APP();
        
        if( retval != 1 )
        {
            if( retval == 0x9000 )
            {
                Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X15 );
            }
            else
            {
                card_app_select_poweroff( retval );
            }
        }
    }
    else if (ProcStatus[PROCESS_CR] & CARD_LOYALTY_APP)
    {
        card_loyalty_app_deal();
    }
    else
    {
        card_saveoil_app_deal();
    }
}

void card_loyalty_app_deal( void )
{
	unsigned short int retval;

    if( !( ProcStatus[PROCESS_CR] & CARD_LOYALTY_APP_BLOCK ))
    {
        retval = CR_ICCARD_Select_Loyalty_APP();

        if( retval != 1 )
        {
            if( retval == 0x9000 )
            {
                Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X15 );
            }
            else
            {
                card_app_select_poweroff( retval );
            }
        }
    }
    else
    {
        card_saveoil_app_deal();
    }
}

void card_saveoil_app_deal( void )
{
    if (!( ProcStatus[PROCESS_CR] & CARD_SAVEOIL_APP_BLOCK ))				
    {
        Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_READ_SFI_0X15 );
    }
    else
    {
        card_app_select_poweroff( 0x9001 );
    }
}
#endif

// 20160108
void WriteBlackCardLog( void )
{
	unsigned char len;

    // Clear Buffers
    memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
    len = strlen(( char * )CRLogTxt );

    memcpy( &CRLogTxt[len], "黑名单卡", 8 );
    len = strlen(( char * )CRLogTxt );

	// Fill " Card ASN"
	Hex2Asc( &CRLogTxt[len], TRINP.TR_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;
    
    // Save To Log
    WriteLog( PROCESS_CR, CRLogTxt );
}
