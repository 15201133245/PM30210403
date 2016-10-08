#include <string.h>
#include "ICCardCommand.h"
#include "PSAMCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "ICCardProc.h"
#include "Pdprotect.h"
#include "define.h"
#include "Display.h"
#include "CardReady.h"
#include "ExCRProc.h"
#include "CRPumpEOT.h"
#include "WriteLog.h"
#include "Global.h"

//Douzhq 20160620 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
#include "SocialCard.h"
#endif

/*
 ** ICCard_EOT_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpEOT.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Pump EOT Process, Perform
 *				  Get Trans Amount, Volume, Price, Total
 *				  Certificate PSAM MAC2, Credit PSAM
 *				  Debit Unlock, Clear Lock Proof, Form Record
 *
 *  RETURNS: None.
 *
 */
void ICCard_EOT_Proc( void )
{
	unsigned short int retval;
    unsigned char LogBuf[52];
	
	// Only Running When CR_Proc_Step = CR_ICCARD_EOT_PROC
	if( CR_Proc_Step == CR_ICCARD_EOT_PROC )
	{
		switch( CR_CommandType )
		{
			case CR_PSAM_CERTIFICATE_GREY_LOCK:
				//Douzhq 20141011 Add , For Bug
#ifdef MAKE_CASH_CR
				//-----------Down-----------------------------------------------------
				if (ProcExchange[PROCESS_CR][PROCESS_CR] & CASH_CODE_ERR)
				{
					ProcExchange[PROCESS_CR][PROCESS_CR] &= ~(CASH_CODE_ERR);
				}
				//----------- Up -----------------------------------------------------
#endif
                //20120311
                //Cetificate psam retry times limeted,3 times Err MAC2 retry will lock the PSAM 
                if( PowerDown.PDCertifyPSAMRetryTimes > 3 )
                {
                    memset( LogBuf, 0x00, sizeof( LogBuf ));
                    sprintf(( char * )LogBuf, "PSAMRetryTimesError: %02X", PowerDown.PDCertifyPSAMRetryTimes );
                    
                    // Write To Log
                    WriteLog( PROCESS_CR, LogBuf );
                    
                    PowerDownFlg = 1;
                }
                else if( PowerDown.PDCertifyPSAMRetryTimes > 1 )
                {
                    Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_GET_GMAC );
                }
                else
                {
                    retval = CR_PSAM_Certificate_Grey_Lock( PSAM1 );
            
                    // Check Command Return
                    if( retval != 1 )
                    {                    
                        PSAMCertificateGreyLockRetval = retval;
                        //WriteCRCertiMAC2Log( CertiMAC2LogType );
                        
                        memset( LogBuf, 0x00, sizeof( LogBuf ));
                        sprintf(( char * )LogBuf, "PSAMRetryTimes: %02X", PowerDown.PDCertifyPSAMRetryTimes );
                        
                        // Write To Log
                        WriteLog( PROCESS_CR, LogBuf );
                        
                        if( retval == 0x9000 )
                        {
                            PowerDown.PDCertifyPSAMRetryTimes=0;
                            // Write Log
//						    CRWriteLog( M_CreditPSAM );

//						    // Display: " OK"
//						    FillInfoMsg( M_DefaultInfo, 0, 0 );
                    
#ifdef MAKE_CARD_MODE2
                            // Check Over Fuelling For Sinopec Mode2
                            // Last Trans From Pump 
                            CR_OverFuelling_Or_Not( 0 );
#endif

                    
                            // Credit PSAM Debit Next
                            Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CREDIT_GREY_DEBIT );

//						    // Update The Power Down Protection Message
                            // Do Not Save The Credit PSAM, If Failed After
                            // The Credit PSAM And Before The GMAC Getted, Restart It.
//						    PowerDown.PdTrans.PD_TRANS_FLAG = CREDIT_PSAM;
                        }
                        else if( retval == 0x6901 )
                        {
                            // Get PSAM TTC
                            TransTTC = CharToIntCopy( TRINP.TR_PSAM_TTC );
                            TransTTC++;
                            IntToCharCopy( TransTTC, TRINP.TR_PSAM_TTC );

//						    // Write Log
//						    CRWriteLog( M_GetGMAC );

//						    // Display: " OK"
//						    FillInfoMsg( M_DefaultInfo, 0, 0 );
                    
#ifdef MAKE_CARD_MODE2
                            // Check Over Fuelling For Sinopec Mode2
                            // Last Trans From Pump 
                            CR_OverFuelling_Or_Not( 0 );
#endif

                            //down SAVE config  20120626
                            SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));
                            //up SAVE config  20120626
                            
                            // Get GMAC Next
                            Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_GET_GMAC );
                        }
                        else
                        {
                            //LiuQi-------------------------------------------------------------
                            //memcpy( TRINP.TR_PSAM_GMAC, "\x00\x00\x00\x00", sizeof(TRINP.TR_PSAM_GMAC) ); 
                            //memcpy( TRINP.TR_PSAM_SAMTAC, "\x00\x00\x00\x00", sizeof(TRINP.TR_PSAM_SAMTAC) ); 

                            // Save PSAMGMAC And PSAM TAC To PowerDown.PdTrans
                            //memcpy( PowerDown.PdTrans.PD_PSAM_GMAC, TRINP.TR_PSAM_GMAC, sizeof( PowerDown.PdTrans.PD_PSAM_GMAC ));
                            //memcpy( PowerDown.PdTrans.PD_PSAM_TAC, TRINP.TR_PSAM_SAMTAC, sizeof( PowerDown.PdTrans.PD_PSAM_TAC ));
    
                            // Update The Power Down Protection Message
                            //PowerDown.PdTrans.PD_TRANS_FLAG = GMAC_RECEIVED;
                            //LiuQi-------------------------------------------------------------
    
                            PowerDown.PDCertifyPSAMRetryTimes++;
//						
                            //down SAVE config  20120626
                            SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));
                            //up SAVE config  20120626
                            
                            // Write Log
//						    CRWriteLog( M_CertificatePSAMErr );

                            // Display: " Certificate PSAM Error"
                            FillInfoMsg( M_CertificatePSAMErr, 0, 0 );
                    
                            // For Write Spring Card Log
                            PowerDownFlag = Pd_Flag_17;
                            ErrorFlag = retval;

                            // Error, Exit
                            Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
                        }
                    }
                    else
                    {
                        // Command Running, Go On
                    }
                }
			break;

			case CR_PSAM_CREDIT_GREY_DEBIT:
			
				retval = CR_PSAM_Credit_Grey_Debit( PSAM1 );
				
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Save PSAMGMAC And PSAM TAC To PowerDown.PdTrans
						memcpy( PowerDown.PdTrans.PD_PSAM_GMAC, TRINP.TR_PSAM_GMAC, sizeof( PowerDown.PdTrans.PD_PSAM_GMAC ));
						memcpy( PowerDown.PdTrans.PD_PSAM_TAC, TRINP.TR_PSAM_SAMTAC, sizeof( PowerDown.PdTrans.PD_PSAM_TAC ));

						// Update The Power Down Protection Message
						PowerDown.PdTrans.PD_TRANS_FLAG = GMAC_RECEIVED;

						if( ProcStatus[PROCESS_CR] & CARD_READY )
						{
//							// Write Log
//							CRWriteLog( M_CardUnlock );

//							// Display: " IC Card Debit Unlock"
//							FillInfoMsg( M_CardUnlock, 0, 0 );
					
							// Debit IC Card Unlock Next
							Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_DEBIT_GREY_UNLOCK );
						}
						else
						{
//							// Write Log
//							CRWriteLog( M_GreyTrans );

//							// Display: " IC Card Debit Unlock"
//							FillInfoMsg( M_GreyTrans, 0, 0 );

							// Display: " OK"
							FillInfoMsg( M_DefaultInfo, 0, 0 );
					
							// Inform Voice Process Exception EOT
							ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_EXC;
							// Clear CRVoiceBusyFlag
							CRVoiceBusyFlag = 0;

							// Maybe Come From Grey Record, Return To IC Card Absence Procedure
							Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
						}
					}
					else
					{
//						// Write Log
//						CRWriteLog( M_CreditPSAMErr );

						// Display: " Credit PSAM"
						FillInfoMsg( M_CreditPSAMErr, 0, 0 );
					
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_18;
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

			case CR_PSAM_GET_GMAC:
			
				 retval = CR_PSAM_Get_GMAC( PSAM1 );
				
				// Check Command Return
				if( retval != 1 )
				{                    
					if( retval == 0x9000 )
					{
                        GetGMACRetryTimes = 0;
                        
						// Save PSAMGMAC And PSAM TAC To PowerDown.PdTrans
						memcpy( PowerDown.PdTrans.PD_PSAM_GMAC, TRINP.TR_PSAM_GMAC, sizeof( PowerDown.PdTrans.PD_PSAM_GMAC ));
						memcpy( PowerDown.PdTrans.PD_PSAM_TAC, TRINP.TR_PSAM_SAMTAC, sizeof( PowerDown.PdTrans.PD_PSAM_TAC ));

						// Update The Power Down Protection Message
						PowerDown.PdTrans.PD_TRANS_FLAG = GMAC_RECEIVED;

						if( ProcStatus[PROCESS_CR] & CARD_READY )
						{
//							// Write Log
//							CRWriteLog( M_CardUnlock );

//							// Display: " IC Card Debit Unlock"
//							FillInfoMsg( M_CardUnlock, 0, 0 );
					
							// Debit IC Card Unlock Next
							Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_DEBIT_GREY_UNLOCK );
						}
						else
						{
//							// Write Log
//							CRWriteLog( M_GreyTrans );

//							// Display: " IC Card Debit Unlock"
//							FillInfoMsg( M_GreyTrans, 0, 0 );

							// Display: " OK"
							FillInfoMsg( M_DefaultInfo, 0, 0 );
					
							// Maybe Come From Grey Record, Return To IC Card Absence Procedure
							Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
						}
					}
					else
					{    
                        //LiuQi-------------------------------------------------------------
                        //memcpy( TRINP.TR_PSAM_GMAC, "\x00\x00\x00\x00", sizeof(TRINP.TR_PSAM_GMAC) ); 
                        //memcpy( TRINP.TR_PSAM_SAMTAC, "\x00\x00\x00\x00", sizeof(TRINP.TR_PSAM_SAMTAC) ); 

                        // Save PSAMGMAC And PSAM TAC To PowerDown.PdTrans
                        //memcpy( PowerDown.PdTrans.PD_PSAM_GMAC, TRINP.TR_PSAM_GMAC, sizeof( PowerDown.PdTrans.PD_PSAM_GMAC ));
                        //memcpy( PowerDown.PdTrans.PD_PSAM_TAC, TRINP.TR_PSAM_SAMTAC, sizeof( PowerDown.PdTrans.PD_PSAM_TAC ));

                        // Update The Power Down Protection Message
                        //PowerDown.PdTrans.PD_TRANS_FLAG = GMAC_RECEIVED;
                        //LiuQi-------------------------------------------------------------
                                                
                        GetGMACRetryTimes++;//可以去掉？？
                        
                        if( GetGMACRetryTimes <= GET_GMAC_RETRY_TIMES )
                        {
                        }
                        else
                        {
                        }
                        
						// Run Error
//						// Write Log
//						CRWriteLog( M_GetGMACErr );

						// Display: " Get GMAC"
						FillInfoMsg( M_GetGMACErr, 0, 0 );
					
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_19;
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

#ifdef MAKE_CARD_MODE2
						// For Sinopec Mode2, Do Not Mard CARDTACREAD
						// Only Mark In Grey Card And Clear Grey Flag Trans
#else
						// Update The Power Down Protection Message
						TRINP.TR_TRANS_TYPE |= CARDTACREAD;
						PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
#endif	// MAKE_CARD_MODE2

						PowerDown.PdTrans.PD_TRANS_FLAG = UNLOCK_GREY;

						// Save The Transaction TAC
						memcpy( PowerDown.PdTrans.PD_CARD_MAC3, TRINP.TR_CARD_MAC3, sizeof( PowerDown.PdTrans.PD_CARD_MAC3 ));

//						// Write Log
//						CRWriteLog( M_CardClearGreyFlag );

//						// Display: " IC Card Clear Grey Flag"
//						FillInfoMsg( M_CardClearGreyFlag, 0, 0 );
					
						// Inform Voice Process EOT
						ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_EOT;
						// Clear CRVoiceBusyFlag
						CRVoiceBusyFlag = 0;

						// Clear Lock Proof Next
						Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_20;
                        ErrorFlag = retval;

						// Error, Exit
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}

					// Display: " OK"
					FillInfoMsg( M_DefaultInfo, 0, 0 );
				}
				else
				{
					// Command Running, Go On
				}
			
			break;

			case CR_ICCARD_CLEAR_LOCK_PROOF:
			
#ifdef MAKE_CARD_MODE2
				if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
				{
					if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
					{
						// Host Account Payment, Do Not Excute
						retval = 0x9000;
					}
					else
					{
						// Excute The Command
						retval = CR_ICCARD_Clear_Lock_Proof();
					}
				}
				else
				{
					if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
					{
						// Maintainance Card
						// Excute The Command
						retval = CR_ICCARD_Clear_Lock_Proof();
					}
					else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
					{
						// Test Card
						// Excute The Command
						retval = CR_ICCARD_Clear_Lock_Proof();
					}
					else
					{
						// For Employee Card, Do Not Excute, Go To Journal Next
						retval = 0x9000;
					}
				}
#else
				retval = CR_ICCARD_Clear_Lock_Proof();
#endif	// MAKE_CARD_MODE2
				
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Call: FormRecord( Normal Debit Unlock );

						// Update The Power Down Protection Message
						PowerDown.PdTrans.PD_TRANS_FLAG = CLEAR_GREY_FLAG;
						
#ifdef MAKE_CARD_MODE2
						CheckTransType();
#else
						// Clear Lock Proof End
						TRINP.TR_TRANS_TYPE &= 0xF0;
						TRINP.TR_TRANS_TYPE |= TRANSOK;

						PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
#endif	// MAKE_CARD_MODE2
						
						// Save EOT Data
//						SavePumpEOTData();

						// Mark IC Card Not Busy
						ProcStatus[PROCESS_CR] &= ( ~CARD_BUSY );

#ifdef MAKE_CARD_MODE2

						DrawCardEOT_Mode2();

						// For Sinopec Mode2, Create EOT Information Timer
						// Delay For 60 * 4 * 1000ms = 4 Minutes
						// Delay For 10 * 1 * 1000ms = 20 Seconds
	   					BalScreenKeepTimer = CreateTimer( TIMER_ONE_DELETE, 1 * 10 * 1000, ClearBalDispFlag );
						KeepBalScreen = 1;

						// For Customer Card, Spring IC Card
						if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
						{
// Special For SHYK Test, Do Not Spring Out Customer Card
// Should Spring Out For Actual Use.
//							HoldCard = 1;
							HoldCard = 0;
						}
						else
						{
							// For Not Employee Card, Do Not Spring Out Card
							HoldCard = 1;
						}

#else

						// For Customer Card, Spring IC Card
						if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
						{
							HoldCard = 0;

							// Create Update Balance Display Timer, Delay For 4 * 1000ms = 4 Seconds
	   						BalScreenKeepTimer = CreateTimer( TIMER_ONE_DELETE, 4 * 1000, ClearBalDispFlag );
							KeepBalScreen = 1;
						}
						else
						{
#ifdef MAKE_CASH_CR
							// For Cash Card, Do Not Display Message For Retry
							if( CRType == CR_TYPE_CASH )
							{
								HoldCard = 0;
							}
							else
#endif
							{
								// For Not Employee Card, Do Not Spring Out Card
								HoldCard = 1;
							}
						}

#endif	// MAKE_CARD_MODE2

//						// Display: " OK"
//						FillInfoMsg( M_DefaultInfo, 0, 0 );
					
						WriteDebitLog();

						// For Write The Running Log
						JournalFlag = 5;

						// Form Journal Next
						Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
					}
					else
					{
//						// Write Log
//						CRWriteLog( M_CardClearGreyFlagErr );

						// Display: " IC Card Clear Grey Flag"
						FillInfoMsg( M_CardClearGreyFlagErr, 0, 0 );
					
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_21;
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
				
			default:
			break;
		}
	}
}




/*
 ** SavePumpEOTData
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpEOT.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save The EOT Data For PowerDown
 *
 *  RETURNS: None.
 *
 *	Note: Called After Clear Grey Proof, Only The Pump EOT Message
 *		  The IC Card Data And Pump Call Data Was Saved In SaveICCardData()	And SavePumpCallData()
 *
 *
 */
void SavePumpEOTData( void )
{
	// Save The Transaction Type
	PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

//	// Save The IC Card TTC
//	memcpy( PowerDown.PdTrans.PD_CARD_APP_TTC, TRINP.TR_CARD_APP_TTC, sizeof(  PowerDown.PdTrans.PD_CARD_APP_TTC ));
	
//	// Save The PSAM TAC And GMAC
//	memcpy( PowerDown.PdTrans.PD_PSAM_GMAC, TRINP.TR_PSAM_GMAC, sizeof(  PowerDown.PdTrans.PD_PSAM_GMAC ));
//	memcpy( PowerDown.PdTrans.PD_PSAM_TAC, TRINP.TR_PSAM_SAMTAC, sizeof(  PowerDown.PdTrans.PD_PSAM_TAC ));
	
//	// Save The Trans Amount
//	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT, TRINP.TR_TRANS_AMOUNT, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT ));
	
//	// Transaction Volume
//	memcpy( PowerDown.PdTrans.PD_TRANS_VOLUME, TRINP.TR_TRANS_VOLUME, sizeof( PowerDown.PdTrans.PD_TRANS_VOLUME ));

//	// Save The Trans Price
//	memcpy( PowerDown.PdTrans.PD_TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( PowerDown.PdTrans.PD_TRANS_PRICE ));
	
//	// Total Volume	For This Transaction
//	memcpy( PowerDown.PdTrans.PD_TRANS_VOLUME_TOTAL, TRINP.TR_TRANS_VOLUME_TOTAL, sizeof( PowerDown.PdTrans.PD_TRANS_VOLUME_TOTAL ));
}


/*
 ** Card_Balance_Update
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpEOT.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Update The IC Card Balance, For Record,
 *				 Call After IC Card Debit Unlock
 *
 *  RETURNS: None.
 *
 */
void Card_Balance_Update( void )
{
	unsigned int balance;
	unsigned int amount;
	
	balance = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
	
#ifdef MAKE_CARD_MODE2
	if( TRINP.TR_WORK_MODE == 0x01 )
	{
		// Online, Uset TransAmount1
		amount = CharToIntCopy( TRINP.TR_TRANS_AMOUNT_T1 );
	}
	else
	{
		// Offline, Uset TransAmount
		amount = CharToIntCopy( TRINP.TR_TRANS_AMOUNT );
	}
#else

	
//IC Card Cut Payment
//Douzhq 20140610 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (TRINP.TR_TRANS_DEBITSOURCE == 0x04 && PowerDown.SaveOilAppMode == 0x01)
	{
		amount = CharToIntCopy( TRINP.TR_TRANS_AMOUNT );
	}
	  	//Douzhq 20140620 Add , Vip Fueling 
	else
	{
		amount = CharToIntCopy( TRINP.TR_TRANS_AMOUNT );
	}
#else
	amount = CharToIntCopy( TRINP.TR_TRANS_AMOUNT );
#endif
	
	
#endif	// MAKE_CARD_MODE2
	
		balance -= amount;	


	IntToCharCopy( balance, TRINP.TR_CARD_APP_BALANCE );

	// Save To PowerDown.PdTrans
	memcpy( PowerDown.PdTrans.PD_CARD_APP_BALANCE, TRINP.TR_CARD_APP_BALANCE, sizeof( PowerDown.PdTrans.PD_CARD_APP_BALANCE ));

	// Save To POS Buffer
	memcpy( POS_CR_Buf.ICCARD_Balance, TRINP.TR_CARD_APP_BALANCE, sizeof( POS_CR_Buf.ICCARD_Balance ));

	// For Logged On Employee Card, Update Balance In PowerDown.LogOnCard.EMPCardBalance
	if( memcmp( TRINP.TR_CARD_ASN, PowerDown.LogOnCard.EMPCardASN, sizeof( TRINP.TR_CARD_ASN )) == 0 )
	{
		memcpy( PowerDown.LogOnCard.EMPCardBalance, TRINP.TR_CARD_APP_BALANCE, sizeof( PowerDown.LogOnCard.EMPCardBalance ));
	}

	// Clear The Amount In Pump CR Buffer
	// Avoid The Buf That Display Error Balance
	
//Douzhq 20140617 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (TRINP.TR_TRANS_DEBITSOURCE == 0x04 && PowerDown.SaveOilAppMode == 0x01)
	{
		//memset( Pump_CR_Buf.Volume, 0x00, sizeof( Pump_CR_Buf.Volume ));
		memset( Pump_CR_Buf.Amount, 0x00, sizeof( Pump_CR_Buf.Amount ));
	}
	else
	{
		memset( Pump_CR_Buf.Amount, 0x00, sizeof( Pump_CR_Buf.Amount ));
	}
#else
	memset( Pump_CR_Buf.Amount, 0x00, sizeof( Pump_CR_Buf.Amount ));
#endif
}



/*
 ** ClearBalDispFlag
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Call Back Function For Check Balance Display Screen For Customer Card,
 *					Creat The Timer In CRPumpEOT
 *
 *  RETURNS: None.
 *
 */
void ClearBalDispFlag( void )
{
	KeepBalScreen = 0;

#ifdef MAKE_CARD_MODE2
			
	ResetDefaultInfo();

	// Display: " OK"
	FillInfoMsg( M_DefaultInfo, 0, 0 );
					
#endif	// MAKE_CARD_MODE2

}


/*
 ** WriteDebitLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpEOT.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Write Debit Ok Log
 *
 *  RETURNS: None.
 *
 */
void WriteDebitLog( void )
{
	unsigned char len;

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Fill "Normal EOT"
	memcpy( &CRLogTxt[len], "扣款解灰结束, 开始保存记录", 26 );
	len = strlen(( char * )CRLogTxt );

	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );
}


#ifdef MAKE_CARD_MODE2
/*
 ** CheckTransType
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpEOT.c
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION:
 *
 *  RETURNS:
 *
 */
void CheckTransType( void )
{
	unsigned char type;
	unsigned int val;


	// Check Host Authed Record
	if( TRINP.TR_PAY_X == 0x15 )
	{
		TRINP.TR_TRANS_TYPE &= 0x00;
		TRINP.TR_TRANS_TYPE |= TRANSHOSTAUTH;
		PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
	}
	else if( TRINP.TR_PAY_X == 0x18 )
	{
		TRINP.TR_TRANS_TYPE &= 0x00;
		TRINP.TR_TRANS_TYPE |= TRANSNONCARDSS;
		PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
	}
	else
	{
		// Do Nothing....
	}

	type = TRINP.TR_TRANS_TYPE & 0x0F;


	// Check OverFueeling Record Or Not
	val = CharToIntCopy( TRINP.TR_TRANS_AMOUNT_OVER_F );
	if( type == TRANSGREY )
	{
		// Only Save OverFuelling Record, While Normal End
		if( val != 0 )
		{
			TRINP.TR_TRANS_TYPE &= 0x00;
			TRINP.TR_TRANS_TYPE |= TRANSOVERFUEL;
			
			if( TRINP.TR_WORK_MODE != 0x01 )
			{
				TRINP.TR_TRANS_TYPE |= ( WORKOFFLINE | LOCALLIST );
			}

			PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

		}
		else
		{
			// Check Online Abnormal Trans Or Not
			if( TRINP.TR_WORK_MODE == 0x01 )
			{
				if( OnlineFlag == 0x01 )
				{
					// Online Abnormal Trans
					TRINP.TR_TRANS_TYPE &= 0x00;
					TRINP.TR_TRANS_TYPE |= TRANSONLINEABN;
					PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
				}
				else
				{
					// Go On....
				}
			}
			else
			{
				// Offline, Go On....
			}
		}
	}
	else
	{
		// Check Online Abnormal Trans Or Not
		if( TRINP.TR_WORK_MODE == 0x01 )
		{
			if( OnlineFlag == 0x01 )
			{
				// Online Abnormal Trans
				TRINP.TR_TRANS_TYPE &= 0x00;
				TRINP.TR_TRANS_TYPE |= TRANSONLINEABN;
				PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
			}
			else
			{
				// Go On....
			}
		}
		else
		{
			// Offline, Go On....
		}
	}

	// Clear Online Trans Flag
	OnlineFlag = 0;

	type = TRINP.TR_TRANS_TYPE & 0x0F;

	if(( type == TRANSTYPERFU ) ||
		( type == TRANSONLINEABN ) ||
		( type == TRANSNONCARDSS ) ||
		( type == TRANSHOSTAUTH ) ||
		( type == TRANSCANCELAUTH ) ||
		( type == TRANSOVERFUEL ))
	{
		// Do Nothing, Do Not Change TransType, Go On....
	}
	else
	{
		// Clear Lock Proof End
		TRINP.TR_TRANS_TYPE &= 0xF0;
		TRINP.TR_TRANS_TYPE |= TRANSOK;

		// Clear The Card TAC Flag
		TRINP.TR_TRANS_TYPE &= ( ~CARDTACREAD );
		PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

		PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
	}
}

//  
/*
 ** CR_OverFuelling_Or_Not
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:	type: RFU
 *
 *  DESCRIPTION: Check Over Fuelling And Save OverFuelling Amount For Sinopec Mode2
 *
 *  RETURNS: None.
 *
 *	ATTN: Only Check OverFuelling For Grey Locked Customer Card, Do Not Check Host Payment Trans
 *
 */
void CR_OverFuelling_Or_Not( unsigned char type )
{
	unsigned int balance, amount;

	if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
	{
		// Customer Card
		if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
		{
			// Host Payment, Do Not Check OverFuelling, Go On....
		}
		else
		{
			// Need Check OverFuelling
			balance = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );

			// Use TramsAmount1 Instead Of TransAmount
			amount = CharToIntCopy( TRINP.TR_TRANS_AMOUNT_T1 );
			
			if( amount > balance )
			{
				// OverFuelling, Divide The TransAmount1
				// Save Balance As TransAmount1
				IntToCharCopy( balance, TRINP.TR_TRANS_AMOUNT_T1 );
				// Save The Amount To PowerDown
				memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT_T1, TRINP.TR_TRANS_AMOUNT_T1, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));

				amount -= balance;
				// Save The OverFuelling Amount
				//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
				IntToCharCopy( amount, TRINP.TR_TRANS_AMOUNT_OVER_F );
				// Save The Amount To PowerDown
				memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT_OVER_F, TRINP.TR_TRANS_AMOUNT_OVER_F, sizeof( TRINP.TR_TRANS_AMOUNT_OVER_F ));
			}
			else
			{
				// Not Fuelling, Go On....
			}
		}
	}
	else
	{
		// Do Not Check OverFuelling, Go On....
	}
}




#endif	// MAKE_CARD_MODE2