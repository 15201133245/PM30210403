#include <string.h>

#include "global.h"
#include "ICCardCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "ICCardProc.h"
#include "Pdprotect.h"
#include "define.h"
#include "CardReady.h"
#include "ExCRProc.h"
#include "CRPumpBusy.h"
#include "Message.h"
#include "CRSFunc.h"
#include "CRPumpCall.h"
#include "WriteLog.h"
#include "Overfuel.h"
#include "Pump.h"


/*
 ** ICCard_Fuelling_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION: In Fuelling Process, Perform
 *				  Check IC Card Exist Or Not,
 *				  External Rounding Or Not
 *				  Real-time Message
 *
 *  RETURNS:
 *
 */
unsigned char CheckCardRetryTimes = 0;

void ICCard_Fuelling_Proc( void )
{
	unsigned short int retval;
	
	// Only Running When CR_Proc_Step = CR_ICCARD_FUELLING_PROC
	if( CR_Proc_Step == CR_ICCARD_FUELLING_PROC )
	{
		switch( CR_CommandType )
		{
			// IC Card Read Lock Proof
			case CR_ICCARD_GET_LOCK_PROOF:
				 retval = CR_ICCARD_Get_Lock_Proof();
				
				// Check Command Return
				if( retval != 1 )
				{
					if(( retval == 0x9000 ) || ( retval == 0x6985 ))
					{
						CheckCardRetryTimes = 0;
						// 0x6985 Means No Lock Trans Ocurred Before
						if( retval == 0x6985 )
						{
							// So, Clear TR_CARD_GREY_FLAG
							TRINP.TR_CARD_GREY_FLAG = 0;
						}
						
						// Replace The IC Card TTC, For The Debit Unlock Next
						memcpy( TRINP.TR_CARD_APP_TTC, TRINP.TR_CARD_GREY_TTC, sizeof( TRINP.TR_CARD_APP_TTC ));
						memcpy( PowerDown.PdTrans.PD_CARD_APP_TTC, TRINP.TR_CARD_GREY_TTC, sizeof( PowerDown.PdTrans.PD_CARD_APP_TTC ));
						
//						// Write Log
//						CRWriteLog( M_Fuelling );

#ifdef MAKE_CASH_CR
						if( CRType == CR_TYPE_CASH )
						{
							// For Cash Trans Display " Fuelling...."
							FillInfoMsg( M_FuellingCash, 0, 1 );
						}
						else
#endif
						{
							// Display " Fuelling, Do Not Spring Out Card"
							FillInfoMsg( M_Fuelling, 0, 1 );
						}

						// Do Not Clear The Info
						StopDefaultInfo();

//						// Inform Voice Process BUSY
//						ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_BUSY;

						// Refresh The Grey Information Ok, Check Card Next
						Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_CARDREADER_CHECK_CARD );
					}
					else
					{
						// Run Error
						// Inform Pump Stop, And....
						ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_STOP;

//						// Write Log
//						CRWriteLog( M_CardReadError );
						WriteCRFuellingErrorLog( 0 );

						// Display Message
						FillInfoMsg( M_CardReadError, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_4;
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

			case CR_CARDREADER_CHECK_CARD:
				
#ifdef MAKE_CARD_MODE2
				if(( PowerDown.PdTrans.PD_TRANS_FLAG == DEBIT_ZERO )
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
				// Check IC Card Inserted Or Not
				retval = CR_CardReaderCheckCard();

#endif	// MAKE_CARD_MODE2
			
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Get Trans Data
						CheckCardRetryTimes = 0;

						// Check Same Nozzle Or Not
						//if( TRINP.TR_TRANS_NOZZLE_NUMBER == GetLogicNzn( Pump_CR_Buf.NozzleNumber - 1 )) 
                        
                        // Does't Compare Logical Nozzle Number Anymore, Cause Logical Nozzle Number Maybe Change While Fueling
                        if( PowerDown.PDPhysicalNozzleNumber > 4 ) // > 4 means 'Memory Change'
                        {
                            PowerDown.PDPhysicalNozzleNumber = Pump_CR_Buf.NozzleNumber;
                        }
                        
                        if( Pump_CR_Buf.NozzleNumber == PowerDown.PDPhysicalNozzleNumber )
						{
							// Get The Fuelling Information
							GetFuellingInfo();
							
							if( ProcStatus[PROCESS_PUMP] & PUMP_ONLINE )
							{
								// IC Card Exist, Check Rounding Next
								Set_Next_Command( CR_ICCARD_BUSY_EXTERNAL_PROC, CR_CommandStart );
							}
							else
							{
//								// Write Log
//								CRWriteLog( M_FuellingPumpOffline );
								WriteCRFuellingErrorLog( 1 );

								// Display Message
								FillInfoMsg( M_FuellingPumpOffline, 0, 0 );

								// Beep It
								BeepControl( 900 );
								
								// For Write Spring Card Log
								PowerDownFlag = Pd_Flag_5;
                                ErrorFlag = 0xE001;

								// Pump Offline, Spring Out Card Next
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
							}
						}
						else
						{
//							// Write Log
//							CRWriteLog( M_NozzleNumberError );
							WriteCRFuellingErrorLog( 2 );

							// Inform Pump Stop, And....
							ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_STOP;

							// Display Message
							FillInfoMsg( M_NozzleNumberError, 0, 0 );

							// Beep It
							BeepControl( 900 );
								
							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_6;
                            ErrorFlag = 0xE002;

							// Different Nozzle, Spring Out Card
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
					}
					else
					{
						// Check Retry Times
						CheckCardRetryTimes++;
						if( CheckCardRetryTimes <= CHECK_CARD_RETRY_TIMES )
						{
							// Retry
	//						// Write Log
							WriteCRFuellingErrorLog( 4 );

							// IC Card Exist, Check Rounding Next
							Set_Next_Command( CR_ICCARD_BUSY_EXTERNAL_PROC, CR_CommandStart );
						}
						else
						{
							// Exceed Retry Times, Stop And Grey Record
						// IC Card Not Exist Or Cardreader Error, Inform Pump Stop, And....
						ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_STOP;

						// Clear The Status Bits
						if( ProcStatus[PROCESS_CR] & CARD_LOGON )
						{
							ProcStatus[PROCESS_CR] &= CARD_LOGON;
						}
						else
						{
							ProcStatus[PROCESS_CR] = 0;
						}

//						// Write Log
//						CRWriteLog( M_FuellingCardOut );
						WriteCRFuellingErrorLog( 3 );

						// Display: " IC Card Not Exist, While Fuelling"
						FillInfoMsg( M_FuellingCardOut, 0, 1 );

							// Do Not Clear The Info
							StopDefaultInfo();
						// Beep It
						BeepControl( 2400 );
								
						Set_Next_Command( CR_ICCARD_ABSENCE_PROC, CR_CommandStart );
						}
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_GET_LAST_TRANS:
				// Pump EOT, IC Card Load Last Trans,
				// The CR EOT Process Should Start After Getting The Last Trans

				// Check First Running, Or Not
				if( !( CR_Task_Flag & ICCARD_PUMP_LAST_TRAN ))
				{
					// Nee Pump For Last Transaction
					// Check Pump Online Or Not
					if( ProcStatus[PROCESS_PUMP] & PUMP_ONLINE )
					{
						// Get Last Trans, Waiting Pump Idle Or Pump EOT
						if( ProcStatus[PROCESS_PUMP] == PUMP_IDLE )
						{
							// While A Grey Trans Is Needed
							// PUMP EOT Must Be Issued
							ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_BUSY );
							ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_EOT );

//							// Write Log
//							CRWriteLog( M_ReadLastTrans );

//							// Display: " IC Card Not Exist, While Fuelling"
//							FillInfoMsg( M_ReadLastTrans, 0, 0 );

							// Set Bit, Local Process Flag
							CR_Task_Flag |= ICCARD_PUMP_LAST_TRAN;
	
							// Set Bit, To Inform PUMP Get Last Transaction
							ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_LASTTRANS;
							
							// Clear To Avoid Duplicated Trans
							ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_LASTTRANS );
						}
						else
						{
							// Maybe Spring Out Card While Fuelling, So
							// Clear Bit, Begin EOT Next
							if( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_EOT )
							{
								// Normal EOT Or Spring Out Card While Fuelling,
								// PUMP EOT Must Be Issued
								ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_BUSY );
								ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_EOT );
								
//								// Write Log
//								CRWriteLog( M_ReadLastTrans );

//								// Display: " Read Last Trans"
//								FillInfoMsg( M_ReadLastTrans, 0, 0 );

								// Set Bit, Local Process Flag
								CR_Task_Flag |= ICCARD_PUMP_LAST_TRAN;
	
								// Set Bit, To Inform PUMP Get Last Transaction
								ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_LASTTRANS;

							
								// Clear To Avoid Duplicated Trans
								ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_LASTTRANS );
							}
							else
							{
								// Waiting PUMP EOT, Go On....
							}
						}
					}
					else
					{
						// Pump Offline ....
					}
				}
				else
				{
					// Wait For Last Trans
					if( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_LASTTRANS )
					{
						// PUMP Load Last Trans End
						// Clear The Bit
						ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_LASTTRANS );
                                                
                                                PowerDown.PDCRTrsFlg=TRANSOFF;     //clear trans flag , modified by xiong 2012.5.31
                                                
                                                
					
						// Clear Local Process Flag
						CR_Task_Flag &= ( ~ICCARD_PUMP_LAST_TRAN );

#ifdef MAKE_CARD_MODE2
#else
						// Check Over Fuelling Or Not And Need Divided The Trans Or Not 
                        WritePumpLastTransLog(); // 20120824 - Recor pump fuel information
						CR_OfflineTrans_Or_Not();
#endif	// MAKE_CARD_MODE2

						// Get Last Trans To Buffers
						CR_GetLastTrans();
						
						// Update Display Totals
						Disp_Totals_Update();

						// Save Datas.......
						PowerDown.PdTrans.PD_TRANS_FLAG = GET_LAST_TRANS;

						// Set PUMP EOT
						// Set Bit, To Inform POS PUMP EOT
						ProcExchange[PROCESS_CR][PROCESS_POS] |= SENDEOT;
						
//						// Write Log
//						CRWriteLog( M_CertificatePSAM );
                        if( OverFuelTransFlg )
                        {
						    WriteCRPumpLastTransLog();
                        }

					      CertiMAC2LogType = 1;
                                              //WriteCRCertiMAC2Log( 1 );
#ifdef MAKE_CARD_MODE2

// Do Not Check Card In Or Not, Get Fuelling Data Anyway,

						// Start Fuelling Data, Next
						Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_WAITING_MAC );


//						if( ProcStatus[PROCESS_CR] & CARD_READY )
//						{
//							// Start Fuelling, Next
//							Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_WAITING_MAC );
//						}
//						else
//						{
//							if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
//							{
//								// Customer Card, Certificate Grey Or Just Journal
//								if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
//								{
//									// Get Last Trans, IC Card Journal Next
//									Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
//								}
//								else
//								{
//									// Get Last Trans, IC Card Certificate PSAM Next
//									Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
//								}
//							}
//							else
//							{
//								if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
//								{
//									// Maintainance Card, IC Card Certificate PSAM Next
//									// IC Card Certificate PSAM Next
//									Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
//								}
//								else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
//								{
//									// Test Card, IC Card Certificate PSAM Next
//									// IC Card Certificate PSAM Next
//									Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
//								}
//								else
//								{
//									// Get Last Trans, Employee Card, Journal Next
//									Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
//								}
//							}
//						}
// Do Not Check Card In Or Not, Get Fuelling Data Anyway, End
#else
						// Get Last Trans, IC Card Certificate PSAM Next
						Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
			
#endif	// MAKE_CARD_MODE2

					}
					else
					{
						// Command Running, Go On ....
					}
				}

			break;

			case CR_GET_LAST_TRANS_ZERO:

				// IC Card Issued Auth Info, But PUMP Failed To Fuelling
				// So Excute ZERO Undebit Unlock
				retval = GetZeroLastTran();
				
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
//						// Write Log
//						CRWriteLog( M_CertificatePSAM );
						WriteCRPumpLastTransLog();

						//WriteCRCertiMAC2Log( 0 );
                        CertiMAC2LogType = 0;

#ifdef MAKE_CARD_MODE2

// Do Not Check Card In Or Not, Get Fuelling Data Anyway,

						// Start Fuelling Data, Next
						Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_WAITING_MAC );


//						if( ProcStatus[PROCESS_CR] & CARD_READY )
//						{
//							// Start Fuelling, Next
//							Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_WAITING_MAC );
//						}
//						else
//						{
//							if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
//							{
//								// Customer Card, Certificate Grey Or Just Journal
//								if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
//								{
//									// Get Last Trans, IC Card Journal Next
//									Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
//								}
//								else
//								{
//									// Get Last Trans, IC Card Certificate PSAM Next
//									Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
//								}
//							}
//							else
//							{
//								if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
//								{
//									// Maintainance Card, IC Card Certificate PSAM Next
//									// IC Card Certificate PSAM Next
//									Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
//								}
//								else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
//								{
//									// Test Card, IC Card Certificate PSAM Next
//									// IC Card Certificate PSAM Next
//									Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
//								}
//								else
//								{
//									// Get Last Trans, Employee Card, Journal Next
//									Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
//								}
//							}
//						}
// Do Not Check Card In Or Not, Get Fuelling Data Anyway, End
#else
						// Get Last Zero Trans, IC Card Certificate PSAM Next
						Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
			
#endif	// MAKE_CARD_MODE2

					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_7;
                        ErrorFlag = retval;

						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}
				
			break;

#ifdef MAKE_CARD_MODE2

			case CR_WAITING_MAC:

				if( TRINP.TR_WORK_MODE != 0x01 )
				{
					// Work Offline, Trans EOT Next
					// EOT Next
					if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
					{
						// For Customer Card, Certificate PSAM Next
						Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
					}
					else
					{
						if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
						{
							// Maintainance Card, IC Card Certificate PSAM Next
							// IC Card Certificate PSAM Next
							Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
						}
						else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
						{
							// Test Card, IC Card Certificate PSAM Next
							// IC Card Certificate PSAM Next
							Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
						}
						else
						{
							// Get Last Trans, Employee Card, Journal Next
							Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
						}
					}
				}
				else
				{
					// Prepare The Calc MAC Elements
					// MAC Process Return
					CR_MAC_CommandType = CR_WAITING_FUELLING_END;
					CR_Proc_MAC_Step = CR_ICCARD_FUELLING_PROC;

					// MAC Type:
					CR_MACType = CR_MAC_TYPE_MAC;

					// Fill The Data
					FillFuellingReqMAC();

					// Calc MAC, Next
					Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_SELECT_SINOPEC2 );
				}
			
			break;

			case CR_WAITING_FUELLING_END:

				// Request POS Fuelling Data Next
				retval = FuellingPOSOnline();

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// OK, Return Back
						// EOT Next
						if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
						{
							if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
							{
								// For Customer Card And Not Host Account Payment, Certificate PSAM Next
								Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
							}
							else
							{
								// For Customer Card And Not Host Account Payment, Certificate PSAM Next
								Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
							}
						}
						else
						{
							if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
							{
								// Maintainance Card, IC Card Certificate PSAM Next
								// IC Card Certificate PSAM Next
								Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
							}
							else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
							{
								// Test Card, IC Card Certificate PSAM Next
								// IC Card Certificate PSAM Next
								Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
							}
							else
							{
								// Get Last Trans, Employee Card, Journal Next
								Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
							}
						}
					}
					else
					{
						// Should Be Online Abnormal Or Grey Record
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_110;
                        ErrorFlag = retval;

						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );

//						// Anyway, EOT Must Be, Only The Amount Different
//						// EOT Next
//						if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
//						{
//							if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
//							{
//								// For Customer Card And Not Host Account Payment, Certificate PSAM Next
//								Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
//							}
//							else
//							{
//								// For Customer Card And Not Host Account Payment, Certificate PSAM Next
//								Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
//							}
//						}
//						else
//						{
//							if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
//							{
//								// Maintainance Card, IC Card Certificate PSAM Next
//								// IC Card Certificate PSAM Next
//								Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
//							}
//							else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
//							{
//								// Test Card, IC Card Certificate PSAM Next
//								// IC Card Certificate PSAM Next
//								Set_Next_Command( CR_ICCARD_EOT_PROC, CR_PSAM_CERTIFICATE_GREY_LOCK );
//							}
//							else
//							{
//								// Get Last Trans, Employee Card, Journal Next
//								Set_Next_Command( CR_ICCARD_EOT_PROC, CR_ICCARD_CLEAR_LOCK_PROOF );
//							}
//						}
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


/*
 ** CR_GetLastTrans
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get Last Trans Response From Pump To TRINP, And Save To PowerDown.PdTrans
 *
 *  RETURNS: None.
 *
 */
void CR_GetLastTrans( void )
{
	unsigned short int price;
	unsigned int value;

	unsigned int amountzero, volumezero;

	// Save To TRINP
    // When 'Get Last Trans', Does't Get Logical Nozzle Number Anymore, Cause Logical Nozzle Number Maybe Change While Fueling
    // So The Logical Nozzle Number In TRINP Is The Old One
    // Note: Get Logical Nozzle Number Again If Want To Use The New One 
	//TRINP.TR_TRANS_NOZZLE_NUMBER = GetLogicNzn( Pump_CR_Buf.NozzleNumber - 1 );

	// Save Volume
	memcpy( &value, Pump_CR_Buf.Volume, sizeof( Pump_CR_Buf.Volume ));
	IntToCharCopy( value, TRINP.TR_TRANS_VOLUME );

	// Save Amount
	memcpy( &value, Pump_CR_Buf.Amount, sizeof( Pump_CR_Buf.Amount ));
	IntToCharCopy( value, TRINP.TR_TRANS_AMOUNT );

	// Save Volume Total
	memcpy( &value, Pump_CR_Buf.VTotal, sizeof( Pump_CR_Buf.VTotal ));
	IntToCharCopy( value, TRINP.TR_TRANS_VOLUME_TOTAL );

	// Save Price
	memcpy( &price, Pump_CR_Buf.Price, sizeof( Pump_CR_Buf.Price ));
	ShortIntToCharCopy( price, TRINP.TR_TRANS_PRICE );


	// Bellow To Correct The PUMP Bugs
	// May Be Deleted Directly After Pump Software Upgrading
	// For Some Special( Zero Amount And Volume ) Operations,
	// Pump Will Send Wrong Price In The Get Trans Procedure
	memcpy( &volumezero, Pump_CR_Buf.Volume, sizeof( Pump_CR_Buf.Volume ));
	memcpy( &amountzero, Pump_CR_Buf.Amount, sizeof( Pump_CR_Buf.Amount ));
	if(( volumezero == 0 ) && ( amountzero == 0 ))
	{
		// Zero Trans, Get Price
		price = GetPrice( Pump_CR_Buf.NozzleNumber - 1, 0 );
		ShortIntToCharCopy( price, TRINP.TR_TRANS_PRICE );
	}
	// Upper To Correct The PUMP Software Bugs

	// Save To PD Buffer
	PowerDown.PdTrans.PD_NOZZLENUMBER = TRINP.TR_TRANS_NOZZLE_NUMBER;

	memcpy( PowerDown.PdTrans.PD_TRANS_VOLUME, TRINP.TR_TRANS_VOLUME, sizeof( PowerDown.PdTrans.PD_TRANS_VOLUME ));
	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT, TRINP.TR_TRANS_AMOUNT, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT ));
	memcpy( PowerDown.PdTrans.PD_TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( PowerDown.PdTrans.PD_TRANS_PRICE ));

	memcpy( PowerDown.PdTrans.PD_TRANS_VOLUME_TOTAL, TRINP.TR_TRANS_VOLUME_TOTAL, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));

//Douzhq 20140708 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (PowerDown.VipFuelingMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x00))
	{
		memcpy(PowerDown.PdTrans.PD_CASH_TOTAL,TRINP.TR_CASH_TOTAL,sizeof(PowerDown.PdTrans.PD_CASH_TOTAL));
	}
	//Douzhq 20140813 Add , Save Oil
	else if (PowerDown.SaveOilAppMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x04))
	{
		memcpy(PowerDown.PdTrans.PD_CASH_TOTAL,TRINP.TR_CASH_TOTAL,sizeof(PowerDown.PdTrans.PD_CASH_TOTAL));;
	}
#endif
	
#ifdef MAKE_CARD_MODE2
	// For Sinopec Mode2, First Save TransAmount1 As TransAmont, For Grey Record And Unlock Grey
	// Replace The TransAmount1 After Fuelling Data Response
	memcpy( TRINP.TR_TRANS_AMOUNT_T1, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));
	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT_T1, TRINP.TR_TRANS_AMOUNT_T1, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT_T1 ));
#endif	// MAKE_CARD_MODE2

}


/*
 ** Disp_Totals_Update
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpEOT.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Update The Totals And Duty Totals For Display.
 *
 *  RETURNS: None.
 *
 */
void Disp_Totals_Update( void )
{
	unsigned char nozzle;
	unsigned int value, sum;

	// Phisical Nozzle Number Start From 0
	nozzle = Pump_CR_Buf.NozzleNumber - 1;

	// Check Duplicated Trans Or Not, Accord To TR_TRANS_VOLUME_TOTAL
	value = CharToIntCopy( TRINP.TR_TRANS_VOLUME_TOTAL );
	sum = CharToIntCopy( DISP_CR_Buf.DispTotal[nozzle].VolumeTotal );

	if( value > sum )
	{
		// New Trans, Update The Totals
		// Update Volume Total
		// Do Not Calc With The TR_TRANS_VOLUME, But Replace With The TR_TRANS_VOLUME_TOTAL 
//		value = CharToIntCopy( TRINP.TR_TRANS_VOLUME );
//		sum = CharToIntCopy( DISP_CR_Buf.DispTotal[nozzle].VolumeTotal );
//		sum += value;
//		IntToCharCopy( sum, DISP_CR_Buf.DispTotal[nozzle].VolumeTotal );
		memcpy( DISP_CR_Buf.DispTotal[nozzle].VolumeTotal, TRINP.TR_TRANS_VOLUME_TOTAL, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));



		// Update Duty Volume Total
		value = CharToIntCopy( TRINP.TR_TRANS_VOLUME );
		sum = CharToIntCopy( DISP_CR_Buf.DispTotal[nozzle].DutyVolumeTotal );
		sum += value;
		IntToCharCopy( sum, DISP_CR_Buf.DispTotal[nozzle].DutyVolumeTotal );

		// Update Amount Total 
		value = CharToIntCopy( TRINP.TR_TRANS_AMOUNT );
		sum = CharToIntCopy( DISP_CR_Buf.DispTotal[nozzle].AmountTotal );
		sum += value;
		IntToCharCopy( sum, DISP_CR_Buf.DispTotal[nozzle].AmountTotal );
		
//TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
		// Update Duty Amount Total
		value = CharToIntCopy( TRINP.TR_TRANS_AMOUNT );
		sum = CharToIntCopy( DISP_CR_Buf.DispTotal[nozzle].DutyAmountTotal );
		sum += value;
		IntToCharCopy( sum, DISP_CR_Buf.DispTotal[nozzle].DutyAmountTotal );

#ifdef MAKE_CASH_CR
		value = CharToIntCopy( PowerDown.PdTrans.PD_CASH_TOTAL );
		// value != 0: Maybe Grey Record....
		if(( CRType == CR_TYPE_CASH ) || ( value != 0x00 ))
		{
			// Update Duty Cash Volume
			sum = CharToIntCopy( DISP_CR_Buf.DispTotal[nozzle].DutyCashVolumeTotal );
			value = CharToIntCopy( TRINP.TR_TRANS_VOLUME );
			sum += value;
			IntToCharCopy( sum, DISP_CR_Buf.DispTotal[nozzle].DutyCashVolumeTotal );
			IntToCharCopy( sum, &PowerDown.PDDutyCashVolumeTotal[nozzle*4] );

			// Update Duty Cash Amount
			sum = CharToIntCopy( DISP_CR_Buf.DispTotal[nozzle].DutyCashAmountTotal );
			value = CharToIntCopy( TRINP.TR_TRANS_AMOUNT );
			sum += value;
			IntToCharCopy( sum, DISP_CR_Buf.DispTotal[nozzle].DutyCashAmountTotal );
			IntToCharCopy( sum, &PowerDown.PDDutyCashAmountTotal[nozzle*4] );
		}
#endif
	}
	else
	{
		// Maybe Duplicated Trans Or Some Other Errors, Do Not Change The Totals
	}
}


/*
 ** GetFuellingInfo
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get The Fuelling Information While IC Card Busy, Include Price, Amount, Volume
 *
 *  RETURNS: None.
 *
 */
void GetFuellingInfo( void )
{
	unsigned short int price;
	unsigned int value;

//	memcpy( TRINP.TR_TRANS_AMOUNT, Pump_CR_Buf.Amount, sizeof( TRINP.TR_TRANS_AMOUNT ));
//	memcpy( TRINP.TR_TRANS_VOLUME, Pump_CR_Buf.Volume, sizeof( TRINP.TR_TRANS_VOLUME ));
//	memcpy( TRINP.TR_TRANS_PRICE, Pump_CR_Buf.Price, sizeof( TRINP.TR_TRANS_PRICE ));

	// Get Amount
	memcpy( &value, Pump_CR_Buf.Amount, 4 );
	IntToCharCopy( value, TRINP.TR_TRANS_AMOUNT );

	// Get Volume
	memcpy( &value, Pump_CR_Buf.Volume, 4 );
	IntToCharCopy( value, TRINP.TR_TRANS_VOLUME );

	if( CRVoiceBusyFlag == 0 )
	{
		if( value != 0 )
		{
			// Fuelling Just Started, Issued Voice Busy
			// Inform Voice Process BUSY
			ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_BUSY;

			// Do Not Issue Any More, Except The CRVoiceBusyFlag Is Cleared
			CRVoiceBusyFlag = 1;
		}	
		else
		{
			// Fuelling Not Started, Go On....
		}
	}
	else
	{
		// Voice Busy Is Issued, Go On....
	}

	// Get Price
	memcpy( &price, Pump_CR_Buf.Price, 2 );
	ShortIntToCharCopy( price, TRINP.TR_TRANS_PRICE );
	
	// Save To PdProtect
	memcpy( PowerDown.PdTrans.PD_TRANS_VOLUME, TRINP.TR_TRANS_VOLUME, sizeof( PowerDown.PdTrans.PD_TRANS_VOLUME ));
	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT, TRINP.TR_TRANS_AMOUNT, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT ));
	memcpy( PowerDown.PdTrans.PD_TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( PowerDown.PdTrans.PD_TRANS_PRICE ));

	
//Douzhq 20140708 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (PowerDown.VipFuelingMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x00))
	{
		memcpy(PowerDown.PdTrans.PD_CASH_TOTAL,TRINP.TR_CASH_TOTAL,sizeof(PowerDown.PdTrans.PD_CASH_TOTAL));
	}
	//Douzhq 20140813 Add , Save Oil
	else if (PowerDown.SaveOilAppMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x04))
	{
		memcpy(PowerDown.PdTrans.PD_CASH_TOTAL,TRINP.TR_CASH_TOTAL,sizeof(PowerDown.PdTrans.PD_CASH_TOTAL));
	}
#endif
	
	// Copy The Trans Data For Display & POS
	POS_CR_Buf.TransNozzleNumber = TRINP.TR_TRANS_NOZZLE_NUMBER;
	memcpy( POS_CR_Buf.TransPrice, TRINP.TR_TRANS_PRICE, sizeof( TRINP.TR_TRANS_PRICE ));
	memcpy( POS_CR_Buf.TransAmount, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
	memcpy( POS_CR_Buf.TransVolume, TRINP.TR_TRANS_VOLUME, sizeof( TRINP.TR_TRANS_VOLUME ));
}


/*
 ** GetZeroLastTran
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get The Last Zero Trans For Zero Debit Unlock
 *
 *  RETURNS: 1: Running; 0x9000: Zero Data Get And Saved
 *			 0: Get Error
 *
 */
unsigned short int GetZeroLastTran( void )
{
	unsigned short int retval, val;
	unsigned int value;

	// Default Running
	retval = 1;

	// Clear Volume
	memset( Pump_CR_Buf.Volume, 0x00, sizeof( Pump_CR_Buf.Volume ));

	// Clear Amount
	memset( Pump_CR_Buf.Amount, 0x00, sizeof( Pump_CR_Buf.Amount ));

	// Get Price For Current Zero Trans
	val = GetPrice( Pump_CR_Buf.NozzleNumber - 1, 0 );
	memcpy( Pump_CR_Buf.Price, &val, 2 );
	
	// Check Trans Total
	memcpy( &value, Pump_CR_Buf.VTotal, sizeof( Pump_CR_Buf.VTotal ));
	if( value == 0 )
	{
		// Maybe From PowerOn, Pump_CR_Buf.VTotal = 0, Fill It
		value = CharToIntCopy( DISP_CR_Buf.DispTotal[Pump_CR_Buf.NozzleNumber - 1].VolumeTotal );
		memcpy( Pump_CR_Buf.VTotal, &value, sizeof( Pump_CR_Buf.VTotal ));
	}
	else
	{
		// Remain Pump_CR_Buf.VTotal
	}
			
	// Save The Data
	CR_GetLastTrans();

	retval = 0x9000;

	return retval;
}


/*
 ** WriteCRFuellingErrorLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:	type: 0: Just Start Fuelling, But Fail To Read The Lock Proof
 *					  1: Fuelling But The Pump Offline;
 *					  2: Fuelling But The Current Nozzle Number Is Different From The Last
 *					  3: Fuelling But The IC Card Spring Out
 *
 *  DESCRIPTION: Write The CR Fuelling, But Fail To Wait Pump EOT Log
 *
 *  RETURNS: None.
 *
 */
void WriteCRFuellingErrorLog( unsigned char type )
{
	unsigned char len;

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Fill "Fuelling Error"
	if( type == 0 )
	{
		memcpy( &CRLogTxt[len], "加油中读灰标错", 14 );
	}
	else if( type == 1 )
	{
		memcpy( &CRLogTxt[len], "油机离线", 8 );
	}
	else if( type == 2 )
	{
		memcpy( &CRLogTxt[len], "枪号不同", 8 );
	}
	else if( type == 3 )
	{
		memcpy( &CRLogTxt[len], "加油中弹卡", 10 );
	}
	else
	{
		memcpy( &CRLogTxt[len], "加油中卡状态错", 14 );
		
		len = strlen(( char * )CRLogTxt );

		// Fill "Card Recv Buffer From 5;"
		// Note: For IC Card: 5 Is 'Y' Or 'N'
		// For IC Card2: 6 Is 'Y' Or 'N', Cause IC Card2 Is PSAM Format
		Hex2Asc(&CRLogTxt[len], &RecvBuffer[5], 5 );
		len = strlen(( char * )CRLogTxt );
		CRLogTxt[len] = ' ';
		len += 1;
	}
	len = strlen(( char * )CRLogTxt );

	// Fill "Phisical Nozzle Number"
	//sprintf(( char * )&CRLogTxt[len], "%02u ", Pump_CR_Buf.NozzleNumber );
    sprintf(( char * )&CRLogTxt[len], "%02u ", PowerDown.PDPhysicalNozzleNumber );
	len = strlen(( char * )CRLogTxt );
    
    // Fill "Logical Nozzle Number"
	//sprintf(( char * )&CRLogTxt[len], "%02u ", GetLogicNzn( Pump_CR_Buf.NozzleNumber - 1 ) );
	//len = strlen(( char * )CRLogTxt );
    
    // Fill "Transaction Nozzle Number"
	//sprintf(( char * )&CRLogTxt[len], "%02u ", TRINP.TR_TRANS_NOZZLE_NUMBER );
	//len = strlen(( char * )CRLogTxt );

	// Fill "Amount"
	Hex2Asc(&CRLogTxt[len], TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill "Volume"
	Hex2Asc(&CRLogTxt[len], TRINP.TR_TRANS_VOLUME, sizeof( TRINP.TR_TRANS_VOLUME ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill "Price"
	Hex2Asc(&CRLogTxt[len], TRINP.TR_TRANS_PRICE, sizeof( TRINP.TR_TRANS_PRICE ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );
}


/*
 ** WriteCRPumpEOTLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION: Receive The Pump Normal EOT, And Ask For The Last Trans
 *
 *  RETURNS:
 *
 */
void WriteCRPumpEOTLog( void )
{
	unsigned char len;

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Fill "Normal EOT"
	memcpy( &CRLogTxt[len], "加油结束,读取上笔交易数据", 25 );
	len = strlen(( char * )CRLogTxt );

	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );
}

/*
 ** WritePumpLastTransLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read The Last Trans OK, And Undebit Or Grey Card Next
 *
 *  RETURNS: None.
 *
 */
void WritePumpLastTransLog( void )
{
	unsigned char len;
    unsigned int intValue;
    unsigned short int shortValue;
    unsigned char buf[4];

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Fill "Normal EOT"
	memcpy( &CRLogTxt[len], "读油机交易成功", 14 );
	len = strlen(( char * )CRLogTxt );

	// Fill "Phisical Nozzle Number"
	sprintf(( char * )&CRLogTxt[len], "%02u ", Pump_CR_Buf.NozzleNumber );
	len = strlen(( char * )CRLogTxt );

	// Fill "Amount"
    memcpy( &intValue, Pump_CR_Buf.Amount, sizeof(intValue) );
	IntToCharCopy( intValue, buf );
    
	Hex2Asc(&CRLogTxt[len], buf, sizeof( Pump_CR_Buf.Amount ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill "Volume"
    memcpy( &intValue, Pump_CR_Buf.Volume, sizeof(intValue) );
	IntToCharCopy( intValue, buf );
    
	Hex2Asc(&CRLogTxt[len], buf, sizeof( Pump_CR_Buf.Volume ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill "Price"
    memcpy( &shortValue, Pump_CR_Buf.Price, sizeof(shortValue) );
	ShortIntToCharCopy( shortValue, buf );
    
	Hex2Asc(&CRLogTxt[len], buf, sizeof( Pump_CR_Buf.Price ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill "Volume Total"
    memcpy( &intValue, Pump_CR_Buf.VTotal, sizeof(intValue) );
	IntToCharCopy( intValue, buf );
        
	Hex2Asc(&CRLogTxt[len], buf, sizeof( Pump_CR_Buf.VTotal ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );
}

/*
 ** WriteCRPumpLastTransLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read The Last Trans OK, And Undebit Or Grey Card Next
 *
 *  RETURNS: None.
 *
 */
void WriteCRPumpLastTransLog( void )
{
	unsigned char len;

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Fill "Normal EOT"
	memcpy( &CRLogTxt[len], "读上笔交易成功", 14 );
	len = strlen(( char * )CRLogTxt );

	// Fill "Phisical Nozzle Number"
	sprintf(( char * )&CRLogTxt[len], "%02u ", Pump_CR_Buf.NozzleNumber );
	len = strlen(( char * )CRLogTxt );

	// Fill "Amount"
	Hex2Asc(&CRLogTxt[len], TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill "Volume"
	Hex2Asc(&CRLogTxt[len], TRINP.TR_TRANS_VOLUME, sizeof( TRINP.TR_TRANS_VOLUME ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill "Price"
	Hex2Asc(&CRLogTxt[len], TRINP.TR_TRANS_PRICE, sizeof( TRINP.TR_TRANS_PRICE ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill "Volume Total"
	Hex2Asc(&CRLogTxt[len], TRINP.TR_TRANS_VOLUME_TOTAL, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );
}

/*
 ** WriteCRCertiMAC2Log
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:	type: 0: Get Zero Trans; 1: Normal Trans.
 *
 *  DESCRIPTION: Certificate the MAC2 Log
 *
 *  RETURNS: None.
 *
 */
void WriteCRCertiMAC2Log( unsigned char type )
{
	unsigned char len;

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Fill "Normal EOT"
	if( type == 0 )
	{
		memcpy( &CRLogTxt[len], "验证MAC2: 0 ", 12 );
		len = strlen(( char * )CRLogTxt );
	}
	else
	{
		memcpy( &CRLogTxt[len], "验证MAC2: 1 ", 12 );
		len = strlen(( char * )CRLogTxt );
	}

	// Fill " MAC2"
	Hex2Asc( &CRLogTxt[len], TRINP.TR_CARD_MAC2, sizeof( TRINP.TR_CARD_MAC2 ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

    // Fill CR_PSAM_Certificate_Grey_Lock returned SW
    len	= strlen(( char * )CRLogTxt );
    memcpy( &CRLogTxt[len], "  CerRet: ", 10 );
    len	= strlen(( char * )CRLogTxt );
    sprintf(( char * )&CRLogTxt[len], "%04X", PSAMCertificateGreyLockRetval );
        
	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );
}



/*
 ** CR_OfflineTrans_Or_Not
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Over Fuelling Or Not And Need Divided The Trans Or Not
 *
 *  RETURNS: None.
 *
 *	ATTN: For Sinopec New Requirement: A Over Fuelling Trans And The Card Balance < Trans_Amount
 *		  Then, Divided The Trans To 1 Normal Trans That The Trans_Amount Is Card Balance
 *				And 1 Non Card Offline Trans For The Remainder
 *
 */
void CR_OfflineTrans_Or_Not( void )
{
	unsigned char NozzleNumber;
	unsigned short int price;
	unsigned int value, balance, volume, amount, volumetotal;

	// Get Nozzle Number
	NozzleNumber = Pump_CR_Buf.NozzleNumber - 1;

	// Get Amount
	memcpy( &amount, Pump_CR_Buf.Amount, 4 );
	
	// Get Volume
	memcpy( &volume, Pump_CR_Buf.Volume, 4 );

	// Get Price
	memcpy( &price, Pump_CR_Buf.Price, 2 );

	// Get Volume Total
	memcpy( &volumetotal, Pump_CR_Buf.VTotal, sizeof( Pump_CR_Buf.VTotal ));

	// Get Balance
	balance = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );

	if( amount > balance )
	{
		// Need Divided
		// Save The Balance As Amount
		memcpy( Pump_CR_Buf.Amount, &balance, sizeof( Pump_CR_Buf.Amount ));
		
		// Save The Trans Volume
		value = Round_Mod(( balance * 100 ), price );
		memcpy( Pump_CR_Buf.Volume, &value, sizeof( Pump_CR_Buf.Volume ));
		
		// Save The Trans Volume Total
		value += volumetotal - volume;
		memcpy( Pump_CR_Buf.VTotal, &value, sizeof( Pump_CR_Buf.VTotal ));

		
		// Save The Difference For Non Card Offline Trans
		// Save Offline Trans Volume Total As MSB
		value = volumetotal - value;
                
                //down added for over fueling process - 20110906
                OverFuelFlg=1;  
                OverFuelTransFlg=1;
                OverFuelTimes+=1;     
                OverFuelValue=value;
                OverFuelToTValue+=value;;
                OverFuelMoney=amount - balance;
                //up added for over fueling process - 20110906
                
		memcpy( &Pump_CR_Buf.OfflineVolume[4*NozzleNumber], &value, 4 );
//		IntToCharCopy( value, &PowerDown.PdTrans.PD_OFFL_TRANS_VOLUME[4*NozzleNumber] );

		// Save Offline Trans Amount Total As MSB
		value = amount - balance;
		memcpy( &Pump_CR_Buf.OfflineAmount[4*NozzleNumber], &value, 4 );
//		IntToCharCopy( value, &PowerDown.PdTrans.PD_OFFL_TRANS_AMOUNT[4*NozzleNumber] );

		// Save Volume Total Difference
		memcpy( &Pump_CR_Buf.VolumeTotal[4*NozzleNumber], &volumetotal, 4 );
		
		// Save Totals
		SaveTotals();		
		
		// Mark Divided Trans For Offline Non Card Trans
		PowerDown.OverFuelling_OfflineTrans = 1;
	}
        //down added for over fueling process - 20110906
        else if((OverPresetMoney>0)&&(amount > OverPresetMoney))
        {       
              //down added for over fueling process - 20110906
                OverFuelFlg=1;  
                OverFuelTimes+=1; 
                OverFuelMoney=amount - OverPresetMoney;
                OverFuelValue=((OverFuelMoney*100)/price);
                OverFuelToTValue+=OverFuelValue;;
              //up added for over fueling process - 20110906
          
        }
        //up added for over fueling process - 20110906
	else
	{
		// Balance >= Amount
             //down added for over fueling process - 20110906
                OverFuelFlg=0;  
                OverFuelTimes=0;     
                OverFuelValue=0;
                OverFuelToTValue=0;;
                OverFuelMoney=0;
            //up added for over fueling process - 20110906
		// Do Nothing, And Just Go....
	}
}



#ifdef MAKE_CARD_MODE2

/*
 ** FuellingPOSOnline
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Request POS For The Fuelling Data End
 *
 *  RETURNS: 0x9000: Host Auth Ok; 
 *			 0x01: Running
 *
 */
unsigned short int FuellingPOSOnline( void )
{
	unsigned short int retval;

	// Default Running
	retval = 1;

	// Check First Running, Or Not
	if( !( CR_Task_Flag & ICCARD_FUELLING_DATA ))
	{
		// Display Fuelling Data....
		FillInfoMsg( M_WaitFuellingData, 0, 0 );

		// Set Bit, Local Process Flag
		CR_Task_Flag |= ICCARD_FUELLING_DATA;

		// Save MAC
		memcpy( POS_CR_Buf.Auth_MAC, TRINP.TR_TRANS_MAC, 4 );

		// Update The Power Down Protection Message
		PowerDown.PdTrans.PD_TRANS_FLAG = SEND_FUELLING;

		// Clear Flag
		POS_CR_Buf.CheckFlag = 0;

		// Inform POS Process
		ProcExchange[PROCESS_CR][PROCESS_POS] |= FUELLING_DATA;

		// Clear The Bit To Avoid The Re-set
		ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~FUELLING_DATA );

		// Check Host Authed Record
		if(( TRINP.TR_PAY_X == 0x15 ) || ( TRINP.TR_PAY_X == 0x18 ))
		{
			// Mark Host Auth Trans, No Response From Host Except ACK
			ProcExchange[PROCESS_CR][PROCESS_POS] |= FUELLING_DATA_HOST;
		}
		else
		{
			// Do Nothing, Go On....
		}
	}
	else
	{
		// Check POS Return
		if( ProcExchange[PROCESS_POS][PROCESS_CR] & FUELLING_DATA )
		{
			// POS Auth Ended
			// Clear The Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~FUELLING_DATA );
					
			// Clear Local Process Flag
			CR_Task_Flag &= ( ~ICCARD_FUELLING_DATA );

			// POS Response, Check The Fit Or Not
			if(( POS_CR_Buf.CheckFlag == 0 ) || ( POS_CR_Buf.CheckFlag == 1 ))
			{
				// Update The Power Down Protection Message
				PowerDown.PdTrans.PD_TRANS_FLAG = FUELLING_RESP;

				// Get The Response
				GetFuellingRespOnline();

				// Fitted, Save Response,
				retval = 0x9000;
			}
			else
			{
//				// Set Work Mode Offline
//				TRINP.TR_WORK_MODE = 0x00;
				
				if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
				{
					// For Host Account Trans, Clear Trans Amount, Anyway
					memset( TRINP.TR_TRANS_AMOUNT_T1, 0x00, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));
					memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT_T1, TRINP.TR_TRANS_AMOUNT_T1, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));
				}

				// Display " TimeOut "....
				FillInfoMsg( M_TimeOut, 0, 0 );
				
				// Error, Use Trans Data As Fuelling Data
				// Update The Power Down Protection Message
				PowerDown.PdTrans.PD_TRANS_FLAG = FUELLING_RESP;
				OnlineFlag = 1;
				retval = 0x9000;

//				// Error,
//				retval = 0x02;
			}
		}
		else
		{
			// Running, Go on....
		}
	}

	return retval;
}

/*
 ** FillFuellingReqMAC
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Fill The Fuelling Data To MAC Buffer For Calc MAC
 *
 *  RETURNS: None.
 *
 */
void FillFuellingReqMAC( void )
{

	// Clear MAC Buffer
	memset( MACBuffer, 0x00, sizeof( MACBuffer ));
	MACDataLen = 0;

	// Save The Nozzle Number
	POS_CR_Buf.TransNozzleNumber = TRINP.TR_TRANS_NOZZLE_NUMBER;

	// Save The POS TTC
	memcpy( POS_CR_Buf.Auth_TTC, TRINP.TR_TRANS_TTC, sizeof( TRINP.TR_TRANS_TTC ));

	// Save The Date
	memcpy( POS_CR_Buf.Auth_Date, TRINP.TR_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));

	// Save The Time
	memcpy( POS_CR_Buf.Auth_Time, TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));

	// Save The Grade Number
	memcpy( POS_CR_Buf.Grade_Number, TRINP.TR_TRANS_GRADE_CODE, sizeof( TRINP.TR_TRANS_GRADE_CODE ));

	// Save The PayX
	POS_CR_Buf.Auth_Pay_X = TRINP.TR_PAY_X;

	// Save The Price
	memcpy( POS_CR_Buf.TransPrice, TRINP.TR_TRANS_PRICE, sizeof( TRINP.TR_TRANS_PRICE ));

	// Save The Card CTC
	memcpy( POS_CR_Buf.Card_CTC, TRINP.TR_CARD_APP_TTC, sizeof( TRINP.TR_CARD_APP_TTC ));

	// Save The Volume Total
	memcpy( POS_CR_Buf.VolumeTotal, TRINP.TR_TRANS_VOLUME_TOTAL, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));

	// Save The Amount
	memcpy( POS_CR_Buf.TransAmount, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
	memcpy( &MACBuffer[MACDataLen], &TRINP.TR_TRANS_AMOUNT[1], 3 );
	MACDataLen += 3;

	// Save The Volume
	memcpy( POS_CR_Buf.TransVolume, TRINP.TR_TRANS_VOLUME, sizeof( TRINP.TR_TRANS_VOLUME ));
	memcpy( &MACBuffer[MACDataLen], &TRINP.TR_TRANS_VOLUME[1], 3 );
	MACDataLen += 3;

	// Save The ASN
	memcpy( POS_CR_Buf.ICCARD_ASN, TRINP.TR_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));
	memcpy( &MACBuffer[MACDataLen], TRINP.TR_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));
	MACDataLen += sizeof( TRINP.TR_CARD_ASN );
}

/*
 ** GetFuellingRespOnline
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpBusy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save The Fuelling Data Response By FS
 *
 *  RETURNS: None.
 *
 */
void GetFuellingRespOnline( void )
{
	// Save The Amount_T1
	memcpy( TRINP.TR_TRANS_AMOUNT_T1, POS_CR_Buf.TransAmount1, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));
	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT_T1, TRINP.TR_TRANS_AMOUNT_T1, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));

	// Save The Amount_T2
	memcpy( TRINP.TR_TRANS_AMOUNT_T2, POS_CR_Buf.TransAmount2, sizeof( TRINP.TR_TRANS_AMOUNT_T2 ));
	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT_T2, TRINP.TR_TRANS_AMOUNT_T2, sizeof( TRINP.TR_TRANS_AMOUNT_T2 ));

	// Save The Discount Number
	memcpy( TRINP.TR_DISCOUNT_NUMBER, POS_CR_Buf.DiscountNumber, sizeof( TRINP.TR_DISCOUNT_NUMBER ));
	memcpy( PowerDown.PdTrans.PD_DISCOUNT_NUMBER, TRINP.TR_DISCOUNT_NUMBER, sizeof( TRINP.TR_DISCOUNT_NUMBER ));
}





#endif	// MAKE_CARD_MODE2

