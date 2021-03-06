#include <string.h>
#include "global.h"

#include "ICCardCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "Journal.h"
#include "Pdprotect.h"
#include "define.h"
#include "PSAMCommand.h"
#include "CRPrint.h"
#include "CRSFunc.h"
#include "Message.h"
#include "WriteLog.h"
#include "Overfuel.h"

//Douzhq 20140620 Add , Vip Fueling 
#include "SocialCard.h"

/*
 ** ICCard_Journal_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\Journal.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Record The Transaction Journal According To Sinopec's Specification
 *
 *  RETURNS: None.
 *
 */
void ICCard_Journal_Proc( void )
{
	unsigned short int retval;
	
	//Douzhq 20140929 Add , Config Printer Set
	unsigned char cardtype, transtype;
	unsigned char *pRecord;
	
	// Only Running When CR_Proc_Step = CR_TRANS_JOURNAL
	if( CR_Proc_Step == CR_TRANS_JOURNAL )
	{
		switch( CR_CommandType )
		{
			// Fill Journal
			case CR_FILL_JOURNAL:
				
				// Get PowerDown Buffer
				ReadPDBuffer();
				
				Fill_Journal();

				// Update The Power Down Protection Message
				PowerDown.PdTrans.PD_TRANS_FLAG = JOURNALIT;
				
				//Douzhq 20140929 Add , Config Printer Set
				TRINP.TR_TRANS_TYPE &= 0x0F;
				if( TRINP.TR_TRANS_TYPE != TRANSLOGON  &&
				    TRINP.TR_TRANS_TYPE != TRANSLOGOFF &&
					TRINP.TR_TRANS_TYPE != TRANPRICERECV)
				{
					PrintFlag = 0;
				}
				
				// Calculate Transaction TAC Next
				// First, PSAM Select Sinopec2
				Set_Next_Command( CR_TRANS_JOURNAL, CR_PSAM_SELECT_SINOPEC2 );

			break;

			// Select Sinopec2 In PSAM
			case CR_PSAM_SELECT_SINOPEC2:

				retval = CR_PSAM_Select_Sinopec2( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Initialize DES Encrypt In PSAM
						Set_Next_Command( CR_TRANS_JOURNAL, CR_PSAM_INIT_DES_ENCRYPT );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_60;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			// Initialize Calculate Trans TAC
			case CR_PSAM_INIT_DES_ENCRYPT:

				retval = CR_PSAM_Init_DES_Encrypt( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Prepare The DES Buffer
						// RevcBuffer + 127: Save The Data Length
						// The Data Buffer Starts From RecvBuffer + 128
						// Max Length Is 120
						
#ifdef MAKE_CARD_MODE2
						// MAC Type:
						CR_MACType = CR_MAC_TYPE_MAC;
#endif	// MAKE_CARD_MODE2

						// Clear Buffer
						memset( &RecvBuffer[127], 0x00, 110 );
						// Copy Data To The Buffer: Lead 8 Bytes 0x00 As Initiate Value,
						// Next 91 Bytes Data
						memcpy( &RecvBuffer[128+8], RECORD.TRANS_TTC, 91 );
						// Pad 0x80, 0x00, 0x00, 0x00, 0x00
						memcpy( &RecvBuffer[128+8+91], "\x80\x00\x00\x00\x00", 5 );
						RecvBuffer[127] = 104;
						
						// Initialize DES Encrypt In PSAM
						Set_Next_Command( CR_TRANS_JOURNAL, CR_PSAM_DES_ENCRYPT0 );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_61;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			// Calculate Trans TAC In 1 Package
			case CR_PSAM_DES_ENCRYPT0:

				retval = CR_PSAM_DES_Encrypt0( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Save Transaction TAC
						memcpy( RECORD.TRANS_TAC, &RecvBuffer[9], 4 );
						

						// Save The Record
						if( SaveRecord() != 0 )
						{
							// Save Error
                            SaveRecordFailFlag++;
							break;
						}
                        
                        SaveRecordFailFlag = 0;

//						if( WriteRecord( &RECORD.JOURNALSIGN ) != 0 )
//						{
//							// Retry 4 Times
//							if( WriteRecord( &RECORD.JOURNALSIGN ) != 0 )
//							{
//								if( WriteRecord( &RECORD.JOURNALSIGN ) != 0 )
//								{
//									if( WriteRecord( &RECORD.JOURNALSIGN ) != 0 )
//									{
//										// Display " Write Record Error "
//										FillInfoMsg( M_WriteRecordError, 0, 0 );
//										
//										// Mard Error, Do Not Entry.
//										CommErrorTimes = CR_COMM_ERR_MAX_TIMES;
//
//										// For Write Spring Card Log
//										PowerDownFlag = Pd_Flag_62;
//
//										// Run Error
//										// Power Off IC Card And Spring Out
//										Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
//
//										break;
//									}
//								}
//							}
//						}
						
#ifdef MAKE_CASH_CR
						// For Cash Card, Print Cash Trans Receipt
						if( CRType == CR_TYPE_CASH )
						{
							// Maybe Print It
							KeypadPrint = KEYPAD_SELECT_PRINT_BEGIN; 

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
							
							// Do Not Entry Again
							CRType = CR_TYPE_CARD;

							// Inform Cash Process
							memcpy( Cash_CR_Buf.CashTransRecord, &RECORD.JOURNALSIGN, sizeof( JOURNAL ));
							ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_TRANS_RECORD;
						}
						else
#endif

						{
							//Douzhq 20140926 Add , Config Printer Set
							if (PowerDown.PrinterSwitchMode == 0x01 && 
								PowerDown.InquirySwitchMode != 0x01 &&
								PowerDown.ReceiptSwitchMode == 0x01)
							{
								FillPrintBuffer( SHORT_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
							}
							else if (PowerDown.PrinterSwitchMode == 0x01 && 
									 PowerDown.InquirySwitchMode != 0x01 &&
									 PowerDown.ReceiptSwitchMode != 0x01)
							{
								FillPrintBuffer( TRANS_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
							}
							else if (PowerDown.PrinterSwitchMode == 0x01 &&
									 PowerDown.InquirySwitchMode == 0x01 &&
									 PowerDown.ReceiptSwitchMode != 0x01 )
							{
								pRecord = RECORD.TRANS_TTC;
								cardtype = *( pRecord + 12 + 2 );
								transtype = *( pRecord + 4 );
								
								if ((CheckPrintIt( cardtype, transtype ) == 0) || (PrintFlag != 0))
								{
									//FillPrintBuffer( TRANS_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
									
									if (PrintFlag == 0x01 && (CheckPrintIt( cardtype, transtype ) != 0))
									{
										//Print It
										FillPrintBuffer( TRANS_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
									}
									if (PrintFlag == 0x02 && (CheckPrintIt( cardtype, transtype ) != 0))
									{
										//Not Print It
										//....
									}
									else
									{
										//....
									}
								}
								else
								{
									KeypadPrintCard = CARD_KEYPAD_SELECT_PRINT_BEGIN;
									Set_Next_Command(CR_TRANS_JOURNAL,CR_WAIT_PRINT_CHOOSE);
									break;
								}
							}
							else if (PowerDown.PrinterSwitchMode == 0x01 &&
									 PowerDown.InquirySwitchMode == 0x01 &&
									 PowerDown.ReceiptSwitchMode == 0x01)
							{
								pRecord = RECORD.TRANS_TTC;
								cardtype = *( pRecord + 12 + 2 );
								transtype = *( pRecord + 4 );
								
								if ((CheckPrintIt( cardtype, transtype ) == 0) || (PrintFlag != 0))
								{
									//FillPrintBuffer( SHORT_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
									
									if (PrintFlag == 0x01 && (CheckPrintIt( cardtype, transtype ) != 0))
									{
										//Print It
										FillPrintBuffer( SHORT_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
									}
									else if (PrintFlag == 0x02 && (CheckPrintIt( cardtype, transtype ) != 0))
									{
										//Not Print It
										//....
									}
									else
									{
										//....
									}
								}
								else
								{
									KeypadPrintCard = CARD_KEYPAD_SELECT_PRINT_BEGIN;
									Set_Next_Command(CR_TRANS_JOURNAL,CR_WAIT_PRINT_CHOOSE);
									break;
								}
							}
							else
							{
								//....
							}
						}

						// Save To DataFlash
						// Clear PowerDown.PdTrans.PD_TRANS_FLAG
						PowerDown.PdTrans.PD_TRANS_FLAG = TRANS_START;
						
						// For Test
//						memset( &RECORD, 0x00, sizeof( RECORD ));
//						FindRecordTactic( TransTTC, &RECORD.JOURNALSIGN );
//						if( SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All )) != 0 )
//						{
//							while( 1 )
//							{
//							};
//						}
						// For Test

						// Restore To Select Sinopec1 Application
						Set_Next_Command( CR_TRANS_JOURNAL, CR_PSAM_SELECT_SINOPEC1 );
					}
					else if( retval == 0x6700 )
					{
						// Initialize DES Encrypt In PSAM
						Set_Next_Command( CR_TRANS_JOURNAL, CR_PSAM_INIT_DES_ENCRYPT1 );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_63;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;


			// Initialize Calculate Trans TAC
			case CR_PSAM_INIT_DES_ENCRYPT1:

				retval = CR_PSAM_Init_DES_Encrypt( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Prepare The 1st DES Buffer
						// RevcBuffer + 127: Save The Data Length
						// The Data Buffer Starts From RecvBuffer + 128
						// Max Length Is 120
						
						// Clear Buffer
						memset( &RecvBuffer[127], 0x00, 80 );
						// Copy Data To The Buffer: Lead 8 Bytes 0x00 As Initiate Value,
						// Next 91 Bytes Data
						memcpy( &RecvBuffer[128+8], RECORD.TRANS_TTC, 48 );
//						// Pad 0x80, 0x00, 0x00, 0x00, 0x00
//						memcpy( &RecvBuffer[128+8+91], "\x80\x00\x00\x00\x00", 5 );
						RecvBuffer[127] = 56;
						
						// DES Encrypt In PSAM The 1st Half
						Set_Next_Command( CR_TRANS_JOURNAL, CR_PSAM_DES_ENCRYPT1 );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_64;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			// Calculate Trans TAC In 2 Packages, The 1st Half
			case CR_PSAM_DES_ENCRYPT1:

				retval = CR_PSAM_DES_Encrypt1( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Prepare 2nd The DES Buffer
						// RevcBuffer + 127: Save The Data Length
						// The Data Buffer Starts From RecvBuffer + 128
						// Max Length Is 120
						
						// Clear Buffer
						memset( &RecvBuffer[127], 0x00, 80 );
						// Copy Data To The Buffer: Lead 8 Bytes 0x00 As Initiate Value,
						// Next 91 Bytes Data
						memcpy( &RecvBuffer[128], &RECORD.TRANS_TTC[0] + 48, 43 );
						// Pad 0x80, 0x00, 0x00, 0x00, 0x00
						memcpy( &RecvBuffer[128+43], "\x80\x00\x00\x00\x00", 5 );
						RecvBuffer[127] = 48;
						
						// DES Encrypt In PSAM The 2nd Half
						Set_Next_Command( CR_TRANS_JOURNAL, CR_PSAM_DES_ENCRYPT2 );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_65;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			// Calculate Trans TAC In 2 Packages, The 2nd Half
			case CR_PSAM_DES_ENCRYPT2:

				retval = CR_PSAM_DES_Encrypt2( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// 2nd Half DES Encrypt In PSAM	Ok
						// Save Transaction TAC
						memcpy( RECORD.TRANS_TAC, &RecvBuffer[9], 4 );
						
						// Save The Record
						if( SaveRecord() != 0 )
						{
                            // Save Error
                            SaveRecordFailFlag++;
							break;
						}
                        
                        SaveRecordFailFlag = 0;

//						if( WriteRecord( &RECORD.JOURNALSIGN ) != 0 )
//						{
//							// Retry 4 Times
//							if( WriteRecord( &RECORD.JOURNALSIGN ) != 0 )
//							{
//								if( WriteRecord( &RECORD.JOURNALSIGN ) != 0 )
//								{
//									if( WriteRecord( &RECORD.JOURNALSIGN ) != 0 )
//									{
//										// Display " Write Record Error "
//										FillInfoMsg( M_WriteRecordError, 0, 0 );
//										
//										// Mard Error, Do Not Entry.
//										CommErrorTimes = CR_COMM_ERR_MAX_TIMES;
//
//										// For Write Spring Card Log
//										PowerDownFlag = Pd_Flag_66;
//
//										// Run Error
//										// Power Off IC Card And Spring Out
//										Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
//
//										break;
//									}
//								}
//							}
//						}

#ifdef MAKE_CASH_CR
						// For Cash Card, Print Cash Trans Receipt
						if( CRType == CR_TYPE_CASH )
						{
							// Maybe Print It
							KeypadPrint = KEYPAD_SELECT_PRINT_BEGIN; 

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
							
							// Do Not Entry Again
							CRType = CR_TYPE_CARD;

							// Inform Cash Process
							memcpy( Cash_CR_Buf.CashTransRecord, &RECORD.JOURNALSIGN, sizeof( JOURNAL ));
							ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_TRANS_RECORD;
						}
						else
#endif

						{		
							//Douzhq 20140926 Add , Config Printer Set
							if (PowerDown.PrinterSwitchMode == 0x01 && 
								PowerDown.InquirySwitchMode != 0x01 &&
								PowerDown.ReceiptSwitchMode == 0x01)
							{
								// Print Short Receipt
								FillPrintBuffer( SHORT_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
							}
							else if (PowerDown.PrinterSwitchMode == 0x01 && 
									 PowerDown.InquirySwitchMode != 0x01 &&
									 PowerDown.ReceiptSwitchMode != 0x01)
							{
								// Printer Trans Double Receipt
								FillPrintBuffer( TRANS_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
							}
							else if (PowerDown.PrinterSwitchMode == 0x01 &&
									 PowerDown.InquirySwitchMode == 0x01 &&
									 PowerDown.ReceiptSwitchMode != 0x01 )
							{
								pRecord = RECORD.TRANS_TTC;
								cardtype = *( pRecord + 12 + 2 );
								transtype = *( pRecord + 4 );
								
								// Judeg Can Print It Or Not
								if ((CheckPrintIt( cardtype, transtype ) == 0) || (PrintFlag != 0))
								{
									//FillPrintBuffer( TRANS_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
									
									if (PrintFlag == 0x01 && (CheckPrintIt( cardtype, transtype ) != 0))
									{
										//Print It
										FillPrintBuffer( TRANS_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
									}
									if (PrintFlag == 0x02 && (CheckPrintIt( cardtype, transtype ) != 0))
									{
										//Not Print It
										//....
									}
									else
									{
										//....
									}
								}
								else
								{
									// Choose Print It Or Not
									KeypadPrintCard = CARD_KEYPAD_SELECT_PRINT_BEGIN;
									Set_Next_Command(CR_TRANS_JOURNAL,CR_WAIT_PRINT_CHOOSE);
									break;
								}
							}
							else if (PowerDown.PrinterSwitchMode == 0x01 &&
									 PowerDown.InquirySwitchMode == 0x01 &&
									 PowerDown.ReceiptSwitchMode == 0x01)
							{
								pRecord = RECORD.TRANS_TTC;
								cardtype = *( pRecord + 12 + 2 );
								transtype = *( pRecord + 4 );
								
								// Judeg Can Print It Or Not
								if ((CheckPrintIt( cardtype, transtype ) == 0) || (PrintFlag != 0))
								{
									//FillPrintBuffer( SHORT_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
									
									if (PrintFlag == 0x01 && (CheckPrintIt( cardtype, transtype ) != 0))
									{
										//Print It 
										FillPrintBuffer( SHORT_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
									}
									else if (PrintFlag == 0x01 && (CheckPrintIt( cardtype, transtype ) != 0))
									{
										//Not Print It
										//....
									}
									else
									{
										//....
									}
								}
								else
								{
									// Choose Print It Or Not
									KeypadPrintCard = CARD_KEYPAD_SELECT_PRINT_BEGIN;
									Set_Next_Command(CR_TRANS_JOURNAL,CR_WAIT_PRINT_CHOOSE);
									break;
								}
							}
							else
							{
								//....
							}
						}
						
						// Save To DataFlash
						// Clear PowerDown.PdTrans.PD_TRANS_FLAG
						PowerDown.PdTrans.PD_TRANS_FLAG = TRANS_START;
						
						// For Test
//						memset( &RECORD, 0x00, sizeof( RECORD ));
//						FindRecordTactic( TransTTC, &RECORD.JOURNALSIGN );
//						if( SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All )) != 0 )
//						{
//							while( 1 )
//							{
//							};
//						}
						// For Test


						// Restore To Select Sinopec1 Application
						Set_Next_Command( CR_TRANS_JOURNAL, CR_PSAM_SELECT_SINOPEC1 );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_67;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;
			
			//Douzhq 20140926 Add , Config Printer Set
			// This Cmd is Enter In Choose Print It Screen
			case CR_WAIT_PRINT_CHOOSE:
				if (KeypadPrintCard == CARD_KEYPAD_SELECT_PRINT_BEGIN)
				{
					// Set This Flag , Choose Print Screen
					ProcStatus[PROCESS_CR] |= CARD_WAIT_PRINT;
				}
				else if (KeypadPrintCard == CARD_KEYPAD_PRINT_IT)
				{
					ProcStatus[PROCESS_CR] &= ~(CARD_WAIT_PRINT);
					
					PrintFlag = 0x01;
					
					if (PowerDown.ReceiptSwitchMode != 0x01)
					{
						// Print Trans Double Receipt 
						FillPrintBuffer( TRANS_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
					}
					else
					{	
						// Print Short Receipt
						FillPrintBuffer( SHORT_RECEIPT, TRINP.TR_STATION_ID, RECORD.TRANS_TTC );
					}
					
					// Save To DataFlash
					// Clear PowerDown.PdTrans.PD_TRANS_FLAG
					PowerDown.PdTrans.PD_TRANS_FLAG = TRANS_START;
					
					Set_Next_Command( CR_TRANS_JOURNAL, CR_PSAM_SELECT_SINOPEC1 );
				}
				else if (KeypadPrintCard == CARD_KEYPAD_PRINT_OVER)
				{
					ProcStatus[PROCESS_CR] &= ~(CARD_WAIT_PRINT);
					
					PrintFlag = 0x02;
					
					// Save To DataFlash
					// Clear PowerDown.PdTrans.PD_TRANS_FLAG
					PowerDown.PdTrans.PD_TRANS_FLAG = TRANS_START;
					
					Set_Next_Command( CR_TRANS_JOURNAL, CR_PSAM_SELECT_SINOPEC1 );
				}
				else
				{
					//....
				}
			break;
			
			// Select Sinopec1 In PSAM
			case CR_PSAM_SELECT_SINOPEC1:

				retval = CR_PSAM_Select_Sinopec1( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						TRINP.TR_TRANS_TYPE &= 0x0F;
						
						if( TRINP.TR_TRANS_TYPE == TRANSLOGON )
						{
							TRINP.TR_TRANS_TYPE = PowerDown.PdTrans.PD_TRANS_TYPE;
						
							// Next, Return To Log On Process
							Set_Next_Command( CR_LOGON_PROC, CR_CHECK_NOZZLES );
							
							//Douzhq 20140929 Add , Config Printer Set
							if (PowerDown.CurrentNozzleNumber < PowerDown.NozzleNumbers)
							{
								if (PrintFlag == 0x02)
								{
									//....
								}
								else
								{
									PrintFlag = 0x01;
								}
							}
							else 
							{
								PrintFlag = 0;
							}
						}
						else if( TRINP.TR_TRANS_TYPE == TRANSLOGOFF )
						{
							TRINP.TR_TRANS_TYPE = PowerDown.PdTrans.PD_TRANS_TYPE;
						
							// Next, Return To Log Off Process
							Set_Next_Command( CR_LOGOFF_PROC, CR_CHECK_NOZZLES );
							
							//Douzhq 20140929 Add , Config Printer Set
							if (PowerDown.CurrentNozzleNumber < PowerDown.NozzleNumbers)
							{
								if (PrintFlag == 0x02)
								{
									//....
								}
								else
								{
									PrintFlag = 0x01;
								}
							}
							else 
							{
								PrintFlag = 0;
							}
							
						}
						else if( TRINP.TR_TRANS_TYPE == TRANSNONCARD )
						{
							TRINP.TR_TRANS_TYPE = PowerDown.PdTrans.PD_TRANS_TYPE;
						
							// Next, Return To Offline Trans Process
							Set_Next_Command( CR_OFFL_TRANS_PROC, CR_CHECK_NOZZLES );
							
							//Douzhq 20140929 Add , Config Printer Set
							if (PowerDown.CurrentNozzleNumber < PowerDown.NozzleNumbers)
							{
								if (PrintFlag == 0x02)
								{
									//....
								}
								else
								{
									PrintFlag = 0x01;
								}
							}
							else 
							{
								PrintFlag = 0;
							}
						}
						else if( TRINP.TR_TRANS_TYPE == TRANPRICERECV )
						{
							TRINP.TR_TRANS_TYPE = PowerDown.PdTrans.PD_TRANS_TYPE;
						
							// Next, Return To Price Receiving Process
							Set_Next_Command( CR_PRICE_RECV, CR_CHECK_NOZZLES );
							
							//Douzhq 20140929 Add , Config Printer Set
							if (PowerDown.CurrentNozzleNumber < PowerDown.NozzleNumbers)
							{
								if (PrintFlag == 0x02)
								{
									//....
								}
								else
								{
									PrintFlag = 0x01;
								}
							}
							else 
							{
								PrintFlag = 0;
							}
						}
						else
						{
							
							//Douzhq 20140930 Add , Config Printer Set
							PrintFlag = 0;
							
							WriteRecordLog();

							// For Write The Running Log
							JournalFlag = 0;

							// Clear TRINP For Next Transaction
							memset( &TRINP, 0x00, sizeof( TRINP ));

							// Retrive Card Balance For Display
							memcpy( TRINP.TR_CARD_APP_BALANCE, PowerDown.PdTrans.PD_CARD_APP_BALANCE, sizeof( TRINP.TR_CARD_APP_BALANCE ));

#ifdef MAKE_CARD_MODE2
							// 1: Sinopec Mode2: Card Is Yellow, Need Send Card Trans Record After Fuelling
							// Other: Don't Care
							if( SendCardRecord == 0x01 )
							{
								// Send IC Card Record To Host
								// Next, Send Card Log
								Set_Next_Command( CR_SEND_CARD_RECORD_PROC, CR_CARD_LOG_START );

								break;
							}
#endif	// MAKE_CARD_MODE2

							// Check Divided Trans Or Not
							if( PowerDown.OverFuelling_OfflineTrans == 1 )
							{
								// Set The Offline Trans Flag
								PowerDown.PdTrans.PD_MULTI_TRANS_FLAG =	NON_ICCARD_JOURNAL;

								// Get Offline Totals
								ReadTotals();
							}

							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_68;
                            ErrorFlag = 0x9000;

							// Next, Spring Card
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_69;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			default:
			break;
		}
	}
}




/*
 ** Fill_Journal
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\Journal.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Copy The Journal Record Elements From TRINP To RECORD
 *
 *  RETURNS: None.
 *
 */
void Fill_Journal( void )
{
//	unsigned char type;
	
	// Clear Record Space

//Douzhq 20140627 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	unsigned char PhysicsNozzle;
	unsigned short vipPrice;
	unsigned int   vipGrade;
	unsigned char  vipPriceChar[2];
	
// Vip Fueling Amount
	unsigned int   vipAmount;
	unsigned char  vipAmountChar[4];
	
// Save Oil
	unsigned char tempValueChar[2];
	
	if (PowerDown.VipFuelingMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x00))	
	{
		vipGrade = PowerDown.PdTrans.PD_CASH_TOTAL[2];
		
		if (vipGrade > 0 && vipGrade <= 0x07)
		{
			//Get Physics Nozzle
			PhysicsNozzle = GetPhysicsNzn(TRINP.TR_TRANS_NOZZLE_NUMBER);
			
			//Get Vip Price
			vipPrice = GetPrice(PhysicsNozzle , vipGrade);
			ShortIntToCharCopy(vipPrice,vipPriceChar);
			
			vipAmount = vipPrice * CharToIntCopy(TRINP.TR_TRANS_VOLUME);
			if ((vipAmount % 100 / 10) > 4)
			{
				vipAmount /=  100;
				vipAmount++;
			}
			else
			{
				vipAmount /=  100;
			}
			IntToCharCopy(vipAmount , vipAmountChar);
		}
	}
	
#endif
	
	memset( &RECORD.JOURNALSIGN, 0x00, sizeof( RECORD ));
	
	// Fill The Journal Buffer
	// Journal Start Flag
	RECORD.JOURNALSIGN = 0xAA;

	// Transaction TTC
	memcpy( RECORD.TRANS_TTC, TRINP.TR_TRANS_TTC, sizeof( RECORD.TRANS_TTC ));
		
	// Transaction Type
	RECORD.TRANS_TYPE = TRINP.TR_TRANS_TYPE;
		
	// Transaction Date	And Time
	memcpy( RECORD.TRANS_DATE, TRINP.TR_TRANS_DATE, sizeof( RECORD.TRANS_DATE ));
	memcpy( RECORD.TRANS_TIME, TRINP.TR_TRANS_TIME, sizeof( RECORD.TRANS_TIME ));

	// IC Card ASN
	memcpy( RECORD.CARD_ASN, TRINP.TR_CARD_ASN, sizeof( RECORD.CARD_ASN ));

	// IC Card Application Balance
	memcpy( RECORD.CARD_APP_BALANCE, TRINP.TR_CARD_APP_BALANCE, sizeof( RECORD.CARD_APP_BALANCE ));

#ifdef MAKE_CARD_MODE2
//	if( TRINP.TR_WORK_MODE == 0x01 )
//	{
		// Online, Use TransAmount1
		// Transaction Amount
		memcpy( RECORD.TRANS_AMOUNT, &TRINP.TR_TRANS_AMOUNT_T1[1], sizeof( RECORD.TRANS_AMOUNT ));
//	}
//	else
//	{
//		// Offline, Use TransAmount
//		// Transaction Amount
//		memcpy( RECORD.TRANS_AMOUNT, &TRINP.TR_TRANS_AMOUNT[1], sizeof( RECORD.TRANS_AMOUNT ));
//	}
#else

//Douzhq 20140709 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (PowerDown.VipFuelingMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x00))	
	{
		if (vipGrade > 0 && vipGrade <= 0x07)
		{
			memcpy( RECORD.TRANS_AMOUNT, &vipAmountChar[1], sizeof( RECORD.TRANS_AMOUNT ));
		}
		else
		{
			memcpy( RECORD.TRANS_AMOUNT, &TRINP.TR_TRANS_AMOUNT[1], sizeof( RECORD.TRANS_AMOUNT ));
		}
	}
	else
	{
		memcpy( RECORD.TRANS_AMOUNT, &TRINP.TR_TRANS_AMOUNT[1], sizeof( RECORD.TRANS_AMOUNT ));
	}
#else
	// Transaction Amount	
	memcpy( RECORD.TRANS_AMOUNT, &TRINP.TR_TRANS_AMOUNT[1], sizeof( RECORD.TRANS_AMOUNT ));
#endif
	
#endif	// MAKE_CARD_MODE2

//Douzhq 20140714 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (TRINP.TR_TRANS_DEBITSOURCE == 0x04 && (PowerDown.SaveOilAppMode == 0x01))
	{
		if ((RECORD.TRANS_TYPE & 0x07) == 0x03)
		{
			//....
		}
		else
		{
			memcpy( RECORD.TRANS_AMOUNT, &TRINP.TR_TRANS_VOLUME[1], sizeof( RECORD.TRANS_AMOUNT ));
		}
	}
#endif
	
	// IC Card Application TTC
	memcpy( RECORD.CARD_TTC, TRINP.TR_CARD_APP_TTC, sizeof( RECORD.CARD_TTC ));

	// IC Card Application TAC
//	if( TRINP.TR_TRANS_TYPE & TRANSGREY )
	if(( TRINP.TR_TRANS_TYPE & 0x0F ) == TRANSGREY )
	{
		// For Grey Trans Record, Get GTAC
		memcpy( RECORD.CARD_TAC, TRINP.TR_CARD_GTAC, sizeof( RECORD.CARD_TAC ));
	}
	else if(( TRINP.TR_TRANS_TYPE & 0x0F ) == TRANSCARDERR )
	{
		// For Card Error Trans, Get CARD TAC
		memcpy( RECORD.CARD_TAC, TRINP.TR_CARD_GREY_TAC, sizeof( RECORD.CARD_TAC ));
	}
	else
	{
		// Others, Get CARD TAC
//		memcpy( RECORD.CARD_TAC, TRINP.TR_CARD_GREY_TAC, sizeof( RECORD.CARD_TAC ));
		memcpy( RECORD.CARD_TAC, TRINP.TR_CARD_MAC3, sizeof( RECORD.CARD_TAC ));
	}

	// PSAM GMAC
	memcpy( RECORD.PSAM_GMAC, TRINP.TR_PSAM_GMAC, sizeof( RECORD.PSAM_GMAC ));

	// PSAM TTC
	memcpy( RECORD.PSAM_TAC, TRINP.TR_PSAM_SAMTAC, sizeof( RECORD.PSAM_TAC ));

	// PSAM ASN
	memcpy( RECORD.PSAM_ASN, TRINP.TR_PSAM_ASN, sizeof( RECORD.PSAM_ASN ));

	// PSAM TID
	memcpy( RECORD.PSAM_TID, TRINP.TR_PSAM_TID, sizeof( RECORD.PSAM_TID ));

	// PSAM TTC
	memcpy( RECORD.PSAM_TTC, TRINP.TR_PSAM_TTC, sizeof( RECORD.PSAM_TTC ));

	// Debit Source
	RECORD.TRANS_DEBITSOURCE = TRINP.TR_TRANS_DEBITSOURCE;
		
	// Debit Source
	RECORD.TRANS_UNIT = TRINP.TR_TRANS_UNIT;

	// IC Card Type
	RECORD.CARD_TYPE = TRINP.TR_CARD_TYPE;

	// Transaction Key Information
	// bit7 - bit4: Key Index; bit3 - bit0: Key Version
	RECORD.CARD_KEY = ((( TRINP.TR_PSAM_KEY_INDEX << 4 ) & 0xF0 ) | ( TRINP.TR_CARD_KEY_VERSION & 0x0F ));

	// Transaction Nozzle Number
	RECORD.NOZZLENUMBER = TRINP.TR_TRANS_NOZZLE_NUMBER;

	// Grade Code
	memcpy( RECORD.GRADE_CODE, TRINP.TR_TRANS_GRADE_CODE, sizeof( RECORD.GRADE_CODE ));

	// Transaction Volume
	memcpy( RECORD.TRANS_VOLUME, &TRINP.TR_TRANS_VOLUME[1], sizeof( RECORD.TRANS_VOLUME ));

	// Transaction Price
	
//Douzhq 20140623 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (PowerDown.VipFuelingMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x00))	
	{
		if (vipGrade > 0 && vipGrade <= 0x07)
		{
			memcpy(RECORD.TRANS_PRICE,vipPriceChar,sizeof(RECORD.TRANS_PRICE));
		}
		else
		{
			memcpy( RECORD.TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( RECORD.TRANS_PRICE ));
		}
	}
	else
	{
		memcpy( RECORD.TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( RECORD.TRANS_PRICE ));
	}
#else
	memcpy( RECORD.TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( RECORD.TRANS_PRICE ));
#endif

//Douzhq 20140714 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (TRINP.TR_TRANS_DEBITSOURCE == 0x04 && (PowerDown.SaveOilAppMode == 0x01))
	{
		ShortIntToCharCopy(100,tempValueChar);
		memcpy( RECORD.TRANS_PRICE, tempValueChar, sizeof( RECORD.TRANS_PRICE ));
	}
#endif
	
	// Transaction Employee Number
	RECORD.EMPLOYEE_NUMBER = TRINP.TR_TRANS_EMP_NUMBER;

	// Transaction Volume Total
	memcpy( RECORD.VOLUME_TOTAL, TRINP.TR_TRANS_VOLUME_TOTAL, sizeof( RECORD.VOLUME_TOTAL ));
	// Fill The Journal Buffer End

		
#ifdef MAKE_CARD_MODE2
	// Clear RFU
	memset( RECORD.RFU, 0x00, sizeof( RECORD.RFU ));

	memcpy( &RECORD.RFU[0], &TRINP.TR_TRANS_AMOUNT[1], 3 );
	RECORD.RFU[3] = TRINP.TR_PAY_X;
	if( TRINP.TR_WORK_MODE == 0x01 )
	{
		if(( TRINP.TR_TRANS_TYPE & 0x0F ) == TRANSOK )
		{
			// Online, And Normal End, Fill Host Response Data
			memcpy( &RECORD.RFU[4], TRINP.TR_DISCOUNT_NUMBER, 2 );
		}
		else
		{
			// Online, But Abnormal End, Fill Card Data
			memcpy( &RECORD.RFU[4], TRINP.TR_CARD_HOLDER_TYPE, 2 );
		}
	}
	else
	{
		// Offline, Fill Card Data
		memcpy( &RECORD.RFU[4], TRINP.TR_CARD_HOLDER_TYPE, 2 );
	}
	memcpy( &RECORD.RFU[6], &TRINP.TR_TRANS_AMOUNT_OVER_F[1], 3 );
	RECORD.RFU[9] = TRINP.TR_CARD_HOLDER_NEW_FLAG;

#else

#ifdef MAKE_CASH_CR
	// For Cash Trans; Save The Cash Message To RFU
	// Cash Total: 4Bytes; Auth Code Lenght: 1Byte; Auth Code: High 6 Bytes If Exceed 6;
	memcpy( &RECORD.RFU[0], TRINP.TR_CASH_TOTAL, sizeof( TRINP.TR_CASH_TOTAL ));

	RECORD.RFU[4] = TRINP.TR_AUTH_CODE_LEN;

//	memcpy( &RECORD.RFU[5], TRINP.TR_AUTH_CODE, 6 );
	if( TRINP.TR_AUTH_CODE_LEN % 2 )
	{
		TRINP.TR_AUTH_CODE_LEN++;
	}
	Asc2Hex( &RECORD.RFU[5], TRINP.TR_AUTH_CODE, TRINP.TR_AUTH_CODE_LEN / 2 );

	// Check Whther CashTrans, Maybe Just Power On Grey Record
	if( CRType != CR_TYPE_CASH )
	{
		if( memcmp( TRINP.TR_CASH_TOTAL, "\x00\x00\x00\x00", sizeof( TRINP.TR_CASH_TOTAL )) != 0x00 )
		{
			if( memcmp( TRINP.TR_CASH_TOTAL, "\xFF\xFF\xFF\xFF", sizeof( TRINP.TR_CASH_TOTAL )) != 0x00 )
			{
				CRType = CR_TYPE_CASH;
			}
			else
			{
				// Do Nothing, Just Go On....
			}
		}
		else
		{
			// Do Nothing, Just Go On....
		}
	}
	else
	{
		// Do Nothing, Just Go On....
	}
	// For Cash Trans End
#else
    if( PowerDown.WorkManage == 0x01 )
    {
        if( (( TRINP.TR_TRANS_TYPE & 0x0F ) == TRANSOK) || (( TRINP.TR_TRANS_TYPE & 0x0F ) == TRANSGREY) )
        {
            RECORD.RFU[0] = PowerDown.FuelEmpNumber;
        }
        else
        {
            RECORD.RFU[0] = 0x00;
        }
    }
#endif	// MAKE_CASH_CR

#endif	// MAKE_CARD_MODE2
	
//Douzhq 20140620 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (PowerDown.VipFuelingMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x00))
			{
				vipGrade = PowerDown.PdTrans.PD_CASH_TOTAL[2];
				
				if (vipGrade > 0 && vipGrade <= 0x07)
				{
					memcpy(&RECORD.RFU[0],&PowerDown.PdTrans.PD_CASH_TOTAL[0],2);
					memcpy(&RECORD.RFU[2],&PowerDown.PdTrans.PD_CASH_TOTAL[2],1);
				}
				else
				{
					//.......
					memset(&RECORD.RFU[0],0x00,3);
				}
			}
			//Douzhq 20140813 Add , Save Oil
			else if (PowerDown.SaveOilAppMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x04))
			{
				memcpy(&RECORD.RFU[0],&PowerDown.PdTrans.PD_CASH_TOTAL[0],2);
			}
#endif
	
	switch( TRINP.TR_TRANS_TYPE & 0x0F )
	{
		// Normal Transaction Journal
		case TRANSOK:

			// Clear The GMAC And PSAM TTC
			memset( RECORD.PSAM_GMAC, 0x00, sizeof( RECORD.PSAM_GMAC ));
			memset( RECORD.PSAM_TAC, 0x00, sizeof( RECORD.PSAM_TAC ));
		break;

		// Grey Transaction Journal, Same as TRANSERROR
		case TRANSGREY:

		// Error Transaction Journal
		case TRANSERROR:

			// All The Elements

		break;

		// Grey Unlock Transaction Journal, Same As TRANSGREYFLAG
		case TRANGREYUNLOCK:
		
		// Clear Grey Flag Transaction Journal
		case TRANSGREYFLAG:

//			// Change Balance
//			Balance0 = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
//			Balance1 = CharToIntCopy( TRINP.TR_TRANS_AMOUNT );
//			Balance0 -= Balance1;
//			IntToCharCopy( Balance0, RECORD.CARD_APP_BALANCE );
			
			// Clear The GMAC And PSAM TAC
			memset( RECORD.PSAM_GMAC, 0x00, sizeof( RECORD.PSAM_GMAC ));
			memset( RECORD.PSAM_TAC, 0x00, sizeof( RECORD.PSAM_TAC ));
		
			// Debit Source
			RECORD.TRANS_UNIT = 0;

			// IC Card Type
			RECORD.CARD_TYPE = 0;

			// Transaction Nozzle Number
			RECORD.NOZZLENUMBER = 0;

			// Grade Code
			memset( RECORD.GRADE_CODE, 0x00, sizeof( RECORD.GRADE_CODE ));

			// Transaction Volume
			memset( RECORD.TRANS_VOLUME, 0x00, sizeof( RECORD.TRANS_VOLUME ));

			// Transaction Price
			memset( RECORD.TRANS_PRICE, 0x00, sizeof( RECORD.TRANS_PRICE ));

			// Transaction Volume Total
			memset( RECORD.VOLUME_TOTAL, 0x00, sizeof( RECORD.VOLUME_TOTAL ));
		
		break;

		// Employee Log On Transaction Journal, Same As	TRANSLOGOFF
		case TRANSLOGON:
		
		// Employee Log Off Transaction Journal
		case TRANSLOGOFF:
		
			// Clear Elements From TRANS_AMOUNT To CARD_KEY
			memset( RECORD.TRANS_AMOUNT, 0x00, sizeof( RECORD.TRANS_AMOUNT )
											   + sizeof( RECORD.CARD_TTC )
											   + sizeof( RECORD.CARD_TAC )
											   + sizeof( RECORD.PSAM_GMAC )
											   + sizeof( RECORD.PSAM_TAC )
											   + sizeof( RECORD.PSAM_ASN )
											   + sizeof( RECORD.PSAM_TID )
											   + sizeof( RECORD.PSAM_TTC )
											   + sizeof( RECORD.TRANS_DEBITSOURCE )
											   + sizeof( RECORD.TRANS_UNIT )
											   + sizeof( RECORD.CARD_TYPE )
											   + sizeof( RECORD.CARD_KEY ));

			// Clear Volume And Price
			memset( RECORD.TRANS_VOLUME, 0x00, sizeof( RECORD.TRANS_VOLUME ));
			memset( RECORD.TRANS_PRICE, 0x00, sizeof( RECORD.TRANS_PRICE ));

			// Clear RFU
			memset( RECORD.RFU, 0x00, sizeof( RECORD.RFU ));

#ifdef MAKE_CARD_MODE2
			// For Sinopec Mode2
			// Transaction Price
			memcpy( RECORD.TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( RECORD.TRANS_PRICE ));
#endif

		break;

		// Non IC Card Transaction Journal
		case TRANSNONCARD:
			
			// Clear Elements From CARD_ASN To CARD_KEY
			memset( RECORD.CARD_ASN, 0x00, sizeof( RECORD.CARD_ASN )
										   + sizeof( RECORD.CARD_APP_BALANCE )
										   + sizeof( RECORD.TRANS_AMOUNT )
										   + sizeof( RECORD.CARD_TTC )
										   + sizeof( RECORD.CARD_TAC )
										   + sizeof( RECORD.PSAM_GMAC )
										   + sizeof( RECORD.PSAM_TAC )
										   + sizeof( RECORD.PSAM_ASN )
										   + sizeof( RECORD.PSAM_TID )
										   + sizeof( RECORD.PSAM_TTC )
										   + sizeof( RECORD.TRANS_DEBITSOURCE )
										   + sizeof( RECORD.TRANS_UNIT )
										   + sizeof( RECORD.CARD_TYPE )
										   + sizeof( RECORD.CARD_KEY ));
			
		
                        
                //down added for over fueling process - 20110906
                    // Transaction Amount
                if(OverFuelTransFlg==1)            //over fueling non-transaction needed  20110906
                {
                      OverFuelTransFlg=0; 
                      memcpy( RECORD.TRANS_AMOUNT, &TRINP.TR_TRANS_AMOUNT[1], sizeof( RECORD.TRANS_AMOUNT ));                                           
                }
                //up added for over fueling process - 20110906
                break;

		// Price Receiving Transaction Journal
		case TRANPRICERECV:
			
			// Clear Elements From CARD_ASN To RFU
			memset( RECORD.CARD_ASN, 0x00, RECORD.TRANS_TAC - RECORD.CARD_ASN );

#ifdef MAKE_CARD_MODE2
			// For Sinopec Mode2, Reserve Nozzle Number And Grade Code
			// Transaction Nozzle Number
			RECORD.NOZZLENUMBER = TRINP.TR_TRANS_NOZZLE_NUMBER;

			// Grade Code
			memcpy( RECORD.GRADE_CODE, TRINP.TR_TRANS_GRADE_CODE, sizeof( RECORD.GRADE_CODE ));

			// Transaction Price
			memcpy( RECORD.TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( RECORD.TRANS_PRICE ));
#endif

		break;

		// Card Error Transaction Journal
		case TRANSCARDERR:
		
			// Clear IC Card Balance, Amount And Application TTC
			memset( RECORD.CARD_APP_BALANCE, 0x00, sizeof( RECORD.CARD_APP_BALANCE )
										   			+ sizeof( RECORD.TRANS_AMOUNT )
										   			+ sizeof( RECORD.CARD_TTC ));

			// Clear From GMAC To Price
			memset( RECORD.PSAM_GMAC, 0x00, &RECORD.EMPLOYEE_NUMBER - RECORD.PSAM_GMAC );
		
			// Clear Volume Total
			memset( RECORD.VOLUME_TOTAL, 0x00, sizeof( RECORD.VOLUME_TOTAL ));

#ifdef MAKE_CARD_MODE2
			// For Sinopec Mode2, Reserve Nozzle Number And Grade Code
			// Transaction Nozzle Number
			RECORD.NOZZLENUMBER = TRINP.TR_TRANS_NOZZLE_NUMBER;

			// Grade Code
			memcpy( RECORD.GRADE_CODE, TRINP.TR_TRANS_GRADE_CODE, sizeof( RECORD.GRADE_CODE ));

			// Transaction Volume Total
			memcpy( RECORD.VOLUME_TOTAL, TRINP.TR_TRANS_VOLUME_TOTAL, sizeof( RECORD.VOLUME_TOTAL ));
#endif

		break;

#ifdef MAKE_CARD_MODE2

		case TRANSTYPERFU:
		break;

		case TRANSONLINEABN:
			
			// Clear The GMAC And PSAM TTC
			memset( RECORD.PSAM_GMAC, 0x00, sizeof( RECORD.PSAM_GMAC ));
			memset( RECORD.PSAM_TAC, 0x00, sizeof( RECORD.PSAM_TAC ));
		
		break;

		case TRANSNONCARDSS:

			// Clear Balance
			memset( RECORD.CARD_APP_BALANCE, 0x00, sizeof( RECORD.CARD_APP_BALANCE ));

			memset( RECORD.CARD_TTC, 0x00, sizeof( RECORD.CARD_TTC )
										   + sizeof( RECORD.CARD_TAC )
										   + sizeof( RECORD.PSAM_GMAC )
										   + sizeof( RECORD.PSAM_TAC ));

			memset( RECORD.PSAM_TTC, 0x00, sizeof( RECORD.PSAM_TTC ));
			
			// IC Card Type
			RECORD.CARD_TYPE = 0;

			// IC Card Key Ver
			RECORD.CARD_KEY = 0;


		break;

		case TRANSHOSTAUTH:

			// Clear Balance
			memset( RECORD.CARD_APP_BALANCE, 0x00, sizeof( RECORD.CARD_APP_BALANCE ));

			memset( RECORD.CARD_TTC, 0x00, sizeof( RECORD.CARD_TTC )
										   + sizeof( RECORD.CARD_TAC )
										   + sizeof( RECORD.PSAM_GMAC )
										   + sizeof( RECORD.PSAM_TAC ));

			memset( RECORD.PSAM_TTC, 0x00, sizeof( RECORD.PSAM_TTC ));
			
			// IC Card DS
			RECORD.TRANS_DEBITSOURCE = 0;

			// IC Card Type
			RECORD.CARD_TYPE = 0;

			// IC Card Key Ver
			RECORD.CARD_KEY = 0;

		break;

		case TRANSCANCELAUTH:

			// Clear Balance
			memset( RECORD.CARD_APP_BALANCE, 0x00, sizeof( RECORD.CARD_APP_BALANCE ));

			memset( RECORD.CARD_TTC, 0x00, sizeof( RECORD.CARD_TTC )
										   + sizeof( RECORD.CARD_TAC )
										   + sizeof( RECORD.PSAM_GMAC )
										   + sizeof( RECORD.PSAM_TAC ));

			memset( RECORD.PSAM_TTC, 0x00, sizeof( RECORD.PSAM_TTC ));
			
			// IC Card DS
			RECORD.TRANS_DEBITSOURCE = 0;

			// IC Card UNIT
			RECORD.TRANS_UNIT = 0;
			
			// IC Card Type
			RECORD.CARD_TYPE = 0;

			// IC Card Key Ver
			RECORD.CARD_KEY = 0;

			// Trans Volume
			memset( RECORD.TRANS_VOLUME, 0x00, sizeof( RECORD.TRANS_VOLUME ));

		break;

		case TRANSOVERFUEL:
			
			// Clear The GMAC And PSAM TTC
			memset( RECORD.PSAM_GMAC, 0x00, sizeof( RECORD.PSAM_GMAC ));
			memset( RECORD.PSAM_TAC, 0x00, sizeof( RECORD.PSAM_TAC ));
		
		break;

#endif

		default:
		break;
	}
}



/*
 ** WriteToPDBuffer
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\Journal.c
 *
 *  PARAMETERS:	unsigned char *pDes: Start Pointer For Destination Save;
 *				unsgined char *pSource: The Source Pointer Of The Buffer
 *				unsigned char len: Length Of The Buffer To Be Saved
 *
 *  DESCRIPTION: Save To PowerDown Buffer, For PowerDown Proction
 *
 *  RETURNS: None.
 *
 */
void WriteToPDBuffer( unsigned char *pDes, unsigned char *pSource, unsigned char len )
{
	memcpy( pDes, pSource, len );
}


/*
 ** ReadPDBuffer
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\Journal.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read PowerDown Protectiong Buffer To TRINP, Except	PD_TRANS_FLAG
 *
 *  RETURNS: None.
 *
 */
void ReadPDBuffer( void )
{
	// TTC
	memcpy( TRINP.TR_TRANS_TTC, PowerDown.PdTrans.PD_TRANS_TTC, sizeof( TRINP.TR_TRANS_TTC ));

	// Transaction Type
	TRINP.TR_TRANS_TYPE = PowerDown.PdTrans.PD_TRANS_TYPE;

	// Transaction Date And Time
	memcpy( TRINP.TR_TRANS_DATE, PowerDown.PdTrans.PD_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));
	memcpy( TRINP.TR_TRANS_TIME, PowerDown.PdTrans.PD_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));

	// ASN
	memcpy( TRINP.TR_CARD_ASN, PowerDown.PdTrans.PD_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));

	// Application Balance
	memcpy( TRINP.TR_CARD_APP_BALANCE, PowerDown.PdTrans.PD_CARD_APP_BALANCE, sizeof( TRINP.TR_CARD_APP_BALANCE ));

#ifdef MAKE_CARD_MODE2
	// Work Mode
	TRINP.TR_WORK_MODE = PowerDown.PdTrans.PD_WORK_MODE;
	
	// Auth Mode
	TRINP.TR_AUTH_MODE = PowerDown.PdTrans.PD_AUTH_MODE;
	
	// PayX
	TRINP.TR_PAY_X = PowerDown.PdTrans.PD_PAY_X;
	
	// Amount & Amount1
//	if( TRINP.TR_WORK_MODE == 0x01 )
//	{
//		// Work Online, Exchange The Trans Amount And The TransAmount1
////		if(( TRINP.TR_TRANS_TYPE & 0x0F ) == TRANSCANCELAUTH )
////		{
////			// For Auth Cancel, Do Not Exchange Amoun And Amount1
////			memcpy( TRINP.TR_TRANS_AMOUNT, PowerDown.PdTrans.PD_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
////			memcpy( TRINP.TR_TRANS_AMOUNT_T1, PowerDown.PdTrans.PD_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));
////		}
////		else
//		{
//			memcpy( TRINP.TR_TRANS_AMOUNT, PowerDown.PdTrans.PD_TRANS_AMOUNT_T1, sizeof( TRINP.TR_TRANS_AMOUNT ));
//			memcpy( TRINP.TR_TRANS_AMOUNT_T1, PowerDown.PdTrans.PD_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));
//		}
//	}
//	else
//	{
//		// Work Offline, Do Not Exchange, Use TransAmount As TransAmount1
//		memcpy( TRINP.TR_TRANS_AMOUNT, PowerDown.PdTrans.PD_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
//		memcpy( TRINP.TR_TRANS_AMOUNT_T1, PowerDown.PdTrans.PD_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));
//	}

	// Amount & 
	memcpy( TRINP.TR_TRANS_AMOUNT, PowerDown.PdTrans.PD_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));

	// Amount1
	memcpy( TRINP.TR_TRANS_AMOUNT_T1, PowerDown.PdTrans.PD_TRANS_AMOUNT_T1, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));

	// Amount2
	memcpy( TRINP.TR_TRANS_AMOUNT_T2, PowerDown.PdTrans.PD_TRANS_AMOUNT_T2, sizeof( TRINP.TR_TRANS_AMOUNT_T2 ));

	// Amount OverFuelling
	memcpy( TRINP.TR_TRANS_AMOUNT_OVER_F, PowerDown.PdTrans.PD_TRANS_AMOUNT_OVER_F, sizeof( TRINP.TR_TRANS_AMOUNT_OVER_F ));

	// Discount Number
	memcpy( TRINP.TR_DISCOUNT_NUMBER, PowerDown.PdTrans.PD_DISCOUNT_NUMBER, sizeof( TRINP.TR_DISCOUNT_NUMBER ));

	// Card Holder Flag	( 71st Byte SFI 1C )
	TRINP.TR_CARD_HOLDER_NEW_FLAG = PowerDown.PdTrans.PD_CARD_HOLDER_NEW_FLAG;
	
	// Card Holder Type ( 74th And 75th Bytes SFI 1C )
	memcpy( TRINP.TR_CARD_HOLDER_TYPE, PowerDown.PdTrans.PD_CARD_HOLDER_TYPE, sizeof( TRINP.TR_CARD_HOLDER_TYPE ));
#else
	// Transaction Amount
	memcpy( TRINP.TR_TRANS_AMOUNT, PowerDown.PdTrans.PD_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
#endif	// MAKE_CARD_MODE2

	// IC Card TTC
	memcpy( TRINP.TR_CARD_APP_TTC, PowerDown.PdTrans.PD_CARD_APP_TTC, sizeof( TRINP.TR_CARD_APP_TTC ));

	// IC Card Set Grey MAC2
	memcpy( TRINP.TR_CARD_MAC2, PowerDown.PdTrans.PD_CARD_MAC2, sizeof( TRINP.TR_CARD_MAC2 ));

	// IC Card Set Grey TAC
	memcpy( TRINP.TR_CARD_GTAC, PowerDown.PdTrans.PD_CARD_GTAC, sizeof( TRINP.TR_CARD_GTAC ));
		
	// IC Card MAC3 Also TAC
	memcpy( TRINP.TR_CARD_MAC3, PowerDown.PdTrans.PD_CARD_MAC3, sizeof( TRINP.TR_CARD_MAC3 ));
		
	// PSAM GMAC
	memcpy( TRINP.TR_PSAM_GMAC, PowerDown.PdTrans.PD_PSAM_GMAC, sizeof( TRINP.TR_PSAM_GMAC ));

	// PSAM TAC
	memcpy( TRINP.TR_PSAM_SAMTAC, PowerDown.PdTrans.PD_PSAM_TAC, sizeof( TRINP.TR_PSAM_SAMTAC ));

	// PSAM ASN
	memcpy( TRINP.TR_PSAM_ASN, PowerDown.PdTrans.PD_PSAM_ASN, sizeof( TRINP.TR_PSAM_ASN ));

	// PSAM TID
	memcpy( TRINP.TR_PSAM_TID, PowerDown.PdTrans.PD_PSAM_TID, sizeof( TRINP.TR_PSAM_TID ));
		
	// PSAM TTC
	memcpy( TRINP.TR_PSAM_TTC, PowerDown.PdTrans.PD_PSAM_TTC, sizeof( TRINP.TR_PSAM_TTC ));

	// Transaction Debit Source
	TRINP.TR_TRANS_DEBITSOURCE = PowerDown.PdTrans.PD_TRANS_DEBITSOURCE;

	// Debit Unit
	TRINP.TR_TRANS_UNIT = PowerDown.PdTrans.PD_TRANS_UNIT;

	// Transaction IC Card Type
	TRINP.TR_CARD_TYPE = PowerDown.PdTrans.PD_CARD_TYPE;

	// Save The IC Card Key Version
	TRINP.TR_CARD_KEY_VERSION = PowerDown.PdTrans.PD_CARD_KEY_VERSION;

	// Save The PSAM Key Index, Combine With The PD_CARD_KEY_VERSION,  To Generate The CARD_KEY In Journal
	TRINP.TR_PSAM_KEY_INDEX = PowerDown.PdTrans.PD_PSAM_KEY_INDEX;

	// Transaction Nozzle Number
	TRINP.TR_TRANS_NOZZLE_NUMBER = PowerDown.PdTrans.PD_NOZZLENUMBER;

	// Grade Code, Product Code
	memcpy( TRINP.TR_TRANS_GRADE_CODE, PowerDown.PdTrans.PD_GRADE_CODE, sizeof( TRINP.TR_TRANS_GRADE_CODE ));

	// Transaction Volume
	memcpy( TRINP.TR_TRANS_VOLUME, PowerDown.PdTrans.PD_TRANS_VOLUME, sizeof( TRINP.TR_TRANS_VOLUME ) );

	// Transaction Price
	memcpy( TRINP.TR_TRANS_PRICE, PowerDown.PdTrans.PD_TRANS_PRICE, sizeof( TRINP.TR_TRANS_PRICE ));

	// Transaction Employee Number
	TRINP.TR_TRANS_EMP_NUMBER = PowerDown.PdTrans.PD_EMPLOYEE_NUMBER;

	// Total Volume	For This Transaction
	memcpy( TRINP.TR_TRANS_VOLUME_TOTAL, PowerDown.PdTrans.PD_TRANS_VOLUME_TOTAL, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));

#ifdef MAKE_CARD_MODE2
	// For Sinopec Mode2, Do Not Process Auth Code Here, Should Be In ASN Process
#else
	// For Cash Trans
	// Cash Total Accepted Or Authed
	memcpy( TRINP.TR_CASH_TOTAL, PowerDown.PdTrans.PD_CASH_TOTAL, sizeof( TRINP.TR_CASH_TOTAL ));

	TRINP.TR_AUTH_CODE_LEN = PowerDown.PdTrans.PD_AUTH_CODE_LEN;
	memcpy( TRINP.TR_AUTH_CODE, PowerDown.PdTrans.PD_AUTH_CODE, sizeof( TRINP.TR_AUTH_CODE ));
	// For Cash Trans End
#endif	// MAKE_CARD_MODE2
}


/*
 ** SaveRecord
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\Journal.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save The Record In Memory To DataFlash, And Check The Save Result, If Error, Send To POS
 *
 *  RETURNS: 0: Save OK; else: Save Error.
 *
 */
unsigned char SaveRecord( void )
{
	unsigned char i, j;
	unsigned char retval;
	unsigned char buf[100];
	unsigned int ttc;
	unsigned int tmp;
	unsigned char len;

	// Default Error
	retval = 1;

	//20131104 qiyong add for check record space limit begin
	// Check Journal Space limit
	tmp = GetRecordSpace();
	if(( tmp < ( ReserveRecordSpace + 1 )) && ( tmp > ReserveRecordSpaceMin ))
	{
		// Display: " Record Max "
		FillInfoMsg( M_RecordMax, 0, 0 );
		// Do Not Clear Untill Display Another
		StopDefaultInfo();

//		return retval;
	}
	else if( tmp < ReserveRecordSpaceMin )
	{
		// Display " space limit, upload "
		FillInfoMsg( M_UploadFirst, 0, 0 );	//20131126
/*20131126
		// For Write Spring Card Log
		PowerDownFlag = Pd_Flag_203;
        ErrorFlag = RECORD.TRANS_TYPE;		//20131126

        HoldCard = 0;

		// Run Error
		// Power Off IC Card And Spring Out
		Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );

		//20131126 return retval;
*/		retval = 0;

		//20131126
		if(!WriteTTCDec())
		{
			WriteTTCDec();
		}

		// Clear Buffers
		memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
		len = strlen(( char * )CRLogTxt );

		memcpy( &CRLogTxt[len], "未上传记录超限  ", 16 );
		len = strlen(( char * )CRLogTxt );

		// Save Trans Type
		sprintf(( char * )&CRLogTxt[len], "transtype:%02X", RECORD.TRANS_TYPE );
		len = strlen(( char * )CRLogTxt );
		CRLogTxt[len] = ' ';
		len += 1;

		// Save To Log
		WriteLog( PROCESS_CR, CRLogTxt );

	}
	//20131104 qiyong add for check record space limit end

	if( retval != 0 )
	{	//20131126
	// Retry 4 Times
	for( i = 0; i < 4; i++ )
	{
		retval = WriteRecord( &RECORD.JOURNALSIGN );

		if( retval == 0 )
		{
            retval = 1;
            
			// Save Ok, Check It
			ttc = CharToIntCopy( RECORD.TRANS_TTC );
	
			// Read The TTC
			memset( buf, 0x00, sizeof( buf ));
			j = FindRecordTTC( ttc, buf );
			if( j == 0 )
			{
				// Read Ok, Compare It
				if( memcmp( &RECORD, buf, sizeof( RECORD )) == 0 )
				{
					// Compare Ok, Return
					retval = 0;
					
#ifdef MAKE_CARD_MODE2
					if(( TRINP.TR_PAY_X == 0x15 ) || ( TRINP.TR_PAY_X == 0x18 ))
					{
						// Maybe, Affect The Check Card Next
						TRINP.TR_PAY_X = 0x00;
						PowerDown.PdTrans.PD_PAY_X = TRINP.TR_PAY_X;
					}
#endif

					break;
				}
				else
				{
					// Compare Error, Continue
					continue;
				}
			}
			else
			{
				// Read Error, Continue
				continue;
			}
		}
	}
	}

	if( retval == 0 )
	{
		// Save Ok, Return
		// For Cash Trans, Clear The Cash Info
#ifdef MAKE_CASH_CR
		if( CRType == CR_TYPE_CASH )
		{
			ClearCashInfo();
		}
#endif
	}
	else
	{
		// Need To Upload To POS
		if( ProcStatus[PROCESS_POS] & POS_ONLINE )
		{
			memcpy( POSUploadTrans.UploadTrans, &RECORD, sizeof( POSUploadTrans.UploadTrans ));

			// Set Bit, To Inform POS Upload The Trans
			ProcExchange[PROCESS_CR][PROCESS_POS] |= TRANS_UPLOAD;
		}
		else
		{
			// Do Nothing, Go On....
		}
		
		// Save Error
		// Display " Write Record Error "
		FillInfoMsg( M_WriteRecordError, 0, 0 );
										
		// Mark Error, Do Not Entry.
		//CommErrorTimes = CR_COMM_ERR_MAX_TIMES;

		// For Write Spring Card Log
		PowerDownFlag = Pd_Flag_70;
        ErrorFlag = 0xE00E;

        HoldCard = 0;
        
		// Run Error
		// Power Off IC Card And Spring Out
		Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
	}

	return retval;
}


/*
 ** WriteRecordLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\Journal.c
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION: Write Trans Ok log
 *
 *  RETURNS:
 *
 */
void WriteRecordLog( void )
{
	unsigned char len;

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Fill "Normal EOT"
	memcpy( &CRLogTxt[len], "保存交易正常: ", 14 );
	len = strlen(( char * )CRLogTxt );

	// Fill "Trans Type"
	sprintf(( char * )&CRLogTxt[len], "%02X ", RECORD.TRANS_TYPE );
	len = strlen(( char * )CRLogTxt );

	// Fill "Trans TTC"
	Hex2Asc( &CRLogTxt[len], TRINP.TR_TRANS_TTC, sizeof( TRINP.TR_TRANS_TTC ));
	len = strlen(( char * )CRLogTxt );

	// Fill "Journal Flag"
	sprintf(( char * )&CRLogTxt[len], " %02X ", JournalFlag );
	len = strlen(( char * )CRLogTxt );

	if( PowerDown.OverFuelling_OfflineTrans == 1 )
	{
		// Fill "Divided Trans"
		memcpy( &CRLogTxt[len], " 需拆分: ", 9 );
		len = strlen(( char * )CRLogTxt );
	}

	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );
}

