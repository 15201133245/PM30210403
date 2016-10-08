
#include <string.h>

#include "global.h"

#include "CRMemFunc.h"
#include "define.h"
#include "CRMemCopy.h"
#include "CRvaribles.h"
#include "Pdprotect.h"
#include "Message.h"
#include "PSAMCommand.h"
#include "ICCardCommand.h"
#include "CRCommand.h"
#include "WriteLog.h"





/*
 ** PSAM_Calc_MAC
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: PSAM Sinopec2 Application Cala Mac Or Encrypted 8Bytes Data
 *
 *  RETURNS: None.
 *
 *	ATTN: 
 *		  CR_MACType: Save The Calc MAC Or Encrypt Data Flag
 *        CR_MAC_CommandType: Save The Command After Cala MAC
 *        CR_Proc_MAC_Step: Save The Process After Cala MAC
 *		  CR_MACType: Save The Flag For Calc MAC Or Encrypt 8Bytes Data
 *
 *		  MACDataLen: Save The Data Length; For Encrypt, Always 8
 *		  MACBuffer: Save The Data To Calc MAC Or Encrypted; For Encrypt, Only 8Bytes Data
 *		  Max Length Is 110
 *
 */


// Save Return
//CR_Proc_MAC_Step = CR_ICCARD_NONEXIST_EXTERNAL_PROC;
//CR_MAC_CommandType = CR_CommandStart;
//CR_MACType = CR_MAC_TYPE_ENC;

void PSAM_Calc_MAC( void )
{
	unsigned short int retval;

	// Only Running When CR_Proc_Step = CR_CALC_MAC_PROC
	if( CR_Proc_Step == CR_CALC_MAC_PROC )
	{
		switch( CR_CommandType )
		{
			// Select Sinopec2 In PSAM
			case CR_PSAM_SELECT_SINOPEC2:

				retval = CR_PSAM_Select_Sinopec2( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Initialize DES Encrypt In PSAM
						Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_INIT_DES_ENCRYPT );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_86;
                        ErrorFlag = retval;

						// Display " MAC Error"
						FillInfoMsg( M_CalcMACError, 0, 0 );

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

			// Initialize Calculate MAC
			case CR_PSAM_INIT_DES_ENCRYPT:

				retval = CR_PSAM_Init_DES_Encrypt( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Trim MAC Data
						if( CR_MACType != CR_MAC_TYPE_ENC )
						{
							TrimMACData();

							// Clear Buffer
							memset( &RecvBuffer[127], 0x00, 128 );
							// Copy Data To The Buffer
							memcpy( &RecvBuffer[128], MACBuffer, MACDataLen );
							RecvBuffer[127] = MACDataLen;
						}
						else
						{
							// Encrypt 8Bytes Data, Do Not Trim Data, Just Limit The Lenght
							// Clear Buffer
							memset( &RecvBuffer[127], 0x00, 128 );
							// Copy Data To The Buffer
							memcpy( &RecvBuffer[128], MACBuffer, 8 );
							RecvBuffer[127] = 8;
						}

						// Initialize DES Encrypt In PSAM
						Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_DES_ENCRYPT0 );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_87;
                        ErrorFlag = retval;

						// Display " MAC Error"
						FillInfoMsg( M_CalcMACError, 0, 0 );

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
						// Save The MAC
						memcpy( TRINP.TR_TRANS_MAC, &RecvBuffer[9], 8 );
						
						// Restore To Select Sinopec1 Application
						Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_SELECT_SINOPEC1 );
					}
					else if( retval == 0x6700 )
					{
						// Initialize DES Encrypt In PSAM
						Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_INIT_DES_ENCRYPT1 );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_88;
                        ErrorFlag = retval;

						// Display " MAC Error"
						FillInfoMsg( M_CalcMACError, 0, 0 );

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


			// Initialize Calculate MAC	In 2 Parts
			case CR_PSAM_INIT_DES_ENCRYPT1:

				retval = CR_PSAM_Init_DES_Encrypt( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Clear Buffer
						memset( &RecvBuffer[127], 0x00, 128 );
						// Copy Data To The Buffer
						memcpy( &RecvBuffer[128], MACBuffer, 56 );
						RecvBuffer[127] = 56;

						// DES Encrypt In PSAM The 1st Half
						Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_DES_ENCRYPT1 );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_89;
                        ErrorFlag = retval;

						// Display " MAC Error"
						FillInfoMsg( M_CalcMACError, 0, 0 );

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
						// Clear Buffer
						memset( &RecvBuffer[127], 0x00, 128 );
						// Copy Data To The Buffer
						memcpy( &RecvBuffer[128], &MACBuffer[56], MACDataLen - 56 );
						RecvBuffer[127] = MACDataLen - 56;

						// DES Encrypt In PSAM The 2nd Half
						Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_DES_ENCRYPT2 );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_90;
                        ErrorFlag = retval;

						// Display " MAC Error"
						FillInfoMsg( M_CalcMACError, 0, 0 );

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
						// Save The MAC
						memcpy( TRINP.TR_TRANS_MAC, &RecvBuffer[9], 8 );

						// Restore To Select Sinopec1 Application
						Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_SELECT_SINOPEC1 );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_91;
                        ErrorFlag = retval;

						// Display " MAC Error"
						FillInfoMsg( M_CalcMACError, 0, 0 );

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

			// Select Sinopec1 In PSAM
			case CR_PSAM_SELECT_SINOPEC1:

				retval = CR_PSAM_Select_Sinopec1( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Return To Save Process And Command
						Set_Next_Command( CR_Proc_MAC_Step, CR_MAC_CommandType );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_92;
                        ErrorFlag = retval;

						// Display " MAC Error"
						FillInfoMsg( M_CalcMACError, 0, 0 );

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
 ** TrimMACData
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Trim MAC Data In MACBuffer As Standard Format For DES
 *
 *  RETURNS: None.
 *
 *	ATTN: MACDataLen Max Is 110
 *		  Save The Trimed Data Length To RecvBuffer[127], Data To RecvBuffer[128].... For Calc
 *		  Called From Init DES Command
 *
 */

const unsigned char PatchData[8] = { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void TrimMACData( void )
{
	unsigned char buf[128];
	unsigned char len, val;


	// Max Length
	if( MACDataLen > 110 )
	{
		MACDataLen = 110;
	}

	// Clear Buffer
	len = 0;
	memset( buf, 0x00, sizeof( buf ));

	// Copy Data
	memcpy( &buf[8], MACBuffer, MACDataLen );
	
	// Total Length
	len = MACDataLen + 8;
	
	// Patch Suffix
	val = len % 8;
	val = 8 - val;
	memcpy( &buf[len], PatchData, val );
	len += val;

	// Fill Trimed Data To MACDataLen And MACBuffer
	MACDataLen = len;
	memset( MACBuffer, 0x00, sizeof( MACBuffer ));
	memcpy( MACBuffer, buf, len );
}


#ifdef MAKE_CARD_MODE2

/*
 ** Card_Send_Log
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Send IC Card Log To Host, While IC Card ASN Is Yellow Listed, For Sinopec Mode2
 *
 *  RETURNS: None.
 *
 */
void Card_Send_Log( void )
{
	unsigned short int retval;

	// Only Running When CR_Proc_Step = CR_SEND_CARD_RECORD_PROC
	if( CR_Proc_Step == CR_SEND_CARD_RECORD_PROC )
	{
		switch( CR_CommandType )
		{
			// IC Card Read All Trans Records In IC Card
			case CR_CARD_LOG_START:

				// Clear Flag
				SendCardRecord = 0x00;
				
				// Need To Read The IC Card Records To Get The Fuelling Times And Totals
				RecordListNumber = 1;

				// The Counters
				CurrentLogs = 0;
				LogItems = 0;
				
				// Clear Bit, Maybe Reserved By Last Upload
				ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~CARD_RECORD );

				// Clear Buffer
				memset( POS_CR_Buf.InforMessage, 0x00, sizeof( POS_CR_Buf.InforMessage ));

				// Display: " Upload Logs"
				FillInfoMsg( M_UploadRecord, 0, 0 );
						
				// Read All The Logs
				Set_Next_Command( CR_SEND_CARD_RECORD_PROC, CR_ICCARD_READ_RECORD );

			break;

			// For Send To Host, Sinopec Mode2
			case CR_ICCARD_READ_RECORD:

				 retval = CR_ICCARD_Read_Record();
					
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Record List++
						RecordListNumber++;

						CurrentLogs++;

						// Copy The Read Log From TRINP TO POC_CR_BUF
						CopyLog();

						// Check The Total Logs
						if( CurrentLogs >= CARD_LOG_ITEM )
						{
							// Package Counter Inc
							LogItems++;
							
							// Update The Package Counter
							POS_CR_Buf.InforMessage[0] &= 0x0F;
							POS_CR_Buf.InforMessage[0] |= (( LogItems << 4 ) & 0xF0 );
							
							// Clear Flag
							POS_CR_Buf.CheckFlag = 0;

							// Clear For Next Counter
							CurrentLogs = 0;

							// Inform POS Send
							ProcExchange[PROCESS_CR][PROCESS_POS] |= CARD_RECORD;
							
							// Waiting End Or Go On To Send.... 
							Set_Next_Command( CR_SEND_CARD_RECORD_PROC, CR_CARD_LOG_END );
						}
						else
						{
							// Re-Reading.... 
							Set_Next_Command( CR_SEND_CARD_RECORD_PROC, CR_ICCARD_READ_RECORD );
						}
					}
					else if(  retval == 0x6A83 )
					{
						if( CurrentLogs != 0 )
						{
							// Still Reserve Log, Inform And Send It
							// Package Counter Inc
							LogItems++;
							
							// Update The Package Counter
							POS_CR_Buf.InforMessage[0] &= 0x0F;
							POS_CR_Buf.InforMessage[0] |= (( LogItems << 4 ) & 0xF0 );
							
							// Clear For Next Counter
							CurrentLogs = 0;

							// Clear Flag
							POS_CR_Buf.CheckFlag = 0;

							// Inform POS Send
							ProcExchange[PROCESS_CR][PROCESS_POS] |= CARD_RECORD;
						}
						else
						{
							// Send All Logs End, Do Nothing....
						}

						// Display: " Upload Logs"
						FillInfoMsg( M_UploadRecordEnd, 0, 0 );
						
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_107;
                        ErrorFlag = retval;

						// Read All The Logs
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_108;
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

			case CR_CARD_LOG_END:
				
				// Check Send Return Or Not
				if( ProcExchange[PROCESS_POS][PROCESS_CR] & CARD_RECORD )
				{
					// Send Over, Go On....
					// Clear Bit
					ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~CARD_RECORD );

					// Clear Buffer
					memset( POS_CR_Buf.InforMessage, 0x00, sizeof( POS_CR_Buf.InforMessage ));
					
					if(( POS_CR_Buf.CheckFlag == 0 ) || ( POS_CR_Buf.CheckFlag == 1 ))
					{
						// Read All The Logs
						Set_Next_Command( CR_SEND_CARD_RECORD_PROC, CR_ICCARD_READ_RECORD );
					}
					else
					{
						// Error, Return;
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
			
			break;

			default:
			break;
		}
	}
}


/*
 ** CopyLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemFunc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Copy The Log To POS_CR_BUF, For Sending To Host, Sinopec Mode2
 *
 *  RETURNS: None.
 *
 */
void CopyLog( void )
{
	unsigned int i;

	POS_CR_Buf.InforMessage[0] = CurrentLogs;

	i = (( CurrentLogs - 1 ) * CARD_LOG_LEN );

	memcpy( &POS_CR_Buf.InforMessage[i+1], TRINP.TR_CARD_RECORD_TTC, CARD_LOG_LEN );
}


#endif	// MAKE_CARD_MODE2



//
//
//
//
//
//
//
//
//
//
//