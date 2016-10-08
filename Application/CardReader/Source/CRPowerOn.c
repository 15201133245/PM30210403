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
#include "Display.h"
#include "CRSFunc.h"
#include "CRPowerOn.h"
#include "message.h"




/*
 ** CRPowerOn_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPowerOn.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Process For Card Reader First Power On, Running CardReader Reset, Get Pump Totals,
 *				 Spring Out IC Card etc.....
 *
 *  RETURNS: None.
 *
 */
void CRPowerOn_Proc( void )
{
	unsigned short int retval;
	
	// Only Running When CR_Proc_Step = CR_CRPOWERON_PROC
	if( CR_Proc_Step == CR_CRPOWERON_PROC )
	{
		switch( CR_CommandType )
		{
			case CR_CARDREADER_RESET:

				// Card Reader Reset
				retval = CR_CardReaderReset();
		
				if( retval != 1 )
				{
					// Check Response
					if( retval == 0x9000 )
					{
						if( IcTypeFlg == 0 )
						{
							// For Intelligence Card Reader, Get Card Type Next
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_GET_CARD_TYPE );

							// Delay A Certain Time For Test Approval
							Set_Next_Command( CR_CRPOWERON_PROC, CR_ICCARD_DELAY );
						}
						else
						{
							// Card Reader Reset Ok, Chek IC Card Next
							Set_Next_Command( CR_CRPOWERON_PROC, CR_CARDREADER_CHECK_CARD );
						}
					}
					else
					{
//						// Write Log
//						CRWriteLog( M_CRResetErr );

						CommErrorTimes++;
						// Display Message
						FillInfoMsg( M_CRResetErr, 0, 0 );

						// Response Error, Retry The Same Reset Command
						// Card Reader Power On Proc Next
						Set_Next_Command( CR_CRPOWERON_PROC, CR_CARDREADER_RESET );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			case CR_ICCARD_DELAY:

				// Delay A Certain Time For Test Approval
		   		if( CardDelaySet == 0 )
				{
		   			CreateTimer( TIMER_ONE_DELETE, CardDelayTimeOut * 4, CardDelay );
					CardDelaySet = 1;
				}
				else
				{
					if( CardDelayed == 1 )
					{
						CardDelaySet = 0;
						CardDelayed = 0;

						// Delay Time Out, Go On....
						Set_Next_Command( CR_CRPOWERON_PROC, CR_CARDREADER_CHECK_CARD );
					}
					else
					{
						// Loop
						Set_Next_Command( CR_CRPOWERON_PROC, CR_ICCARD_DELAY );
					}
				}

			break;

			case CR_CARDREADER_CHECK_CARD:

				// Check IC Card Inserted Or Not
				retval = CR_CardReaderCheckCard();
			
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_195;
                        ErrorFlag = retval;

						// Spring Out IC Card, For First Power On
						Set_Next_Command( CR_CRPOWERON_PROC, CR_CARDREADER_SPINGOUT_CARD );
					}
					else
					{
						// Check Last Trans In PUMP Next
						Set_Next_Command( CR_CRPOWERON_PROC, CR_GET_LAST_TRANS_POWERON );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;

			case CR_CARDREADER_SPINGOUT_CARD:

				// Spring Out IC Card
				retval = CR_CardReaderSpringOutCard();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Check Last Trans In PUMP Next
						Set_Next_Command( CR_CRPOWERON_PROC, CR_GET_LAST_TRANS_POWERON );
					}
					else
					{
						// Spring Out Error
						// Display: "Spring Out IC Card Error"
						
						// Check Last Trans In PUMP Next
						Set_Next_Command( CR_CRPOWERON_PROC, CR_GET_LAST_TRANS_POWERON );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;

			case CR_GET_LAST_TRANS_POWERON:
				
				// Check Wether A Trans Is Reserved In PUMP
				// If A Trans Is Reserved In Pump, But Not In CR, For Example: Change The CR PCB
				// The PUMP Will Remain In EOT, And Can Not Go On....
				// So, Read The Last Trans To Make Sure The PUMP Go On....
				// This Bit Was Set By PUMP Process At Power On
				if( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_LASTTRANS )
				{
					// Clear The Bit
					ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_LASTTRANS );

					// Yes, There Is A Trans In PUMP
					// Check Wether A Trans Is Reserved In CR
					// Do Not Include Journal It
					if(( PowerDown.PdTrans.PD_TRANS_FLAG == SET_GREY ) ||
						( PowerDown.PdTrans.PD_TRANS_FLAG == MAC2_RECEIVED ))
					{
						// There Is A Trans In CR, Need To Read From Pump
						// Go On, Wait Grey Record To Read It Again
//						// Write Log
//						CRWriteLog( M_ReadPumpTotal );

						// Display Message
//						FillInfoMsg( M_WaitPumpOnline, 0, 0 );

						// Get Pump Totals Next
						Set_Next_Command( CR_CRPOWERON_PROC, CR_PUMP_TOTALS );
					}
					else
					{
						// There Is No Trans In CR, Write To Error Log, In Future
//						// Write Log
//						CRWriteLog( M_ReadPumpTotal );

						// Display Message
//						FillInfoMsg( M_ReadPumpTotal, 0, 0 );
//						FillInfoMsg( M_WaitPumpOnline, 0, 0 );

						// Get Pump Totals Next
						Set_Next_Command( CR_CRPOWERON_PROC, CR_PUMP_TOTALS );
					}

					// Display " Wait Pump Online "
//					FillInfoMsg( M_WaitPumpOnline, 0, 0 );
				}
				else
				{
//					// Write Log
//					CRWriteLog( M_ReadPumpTotal );

					// Display " Wait Pump Online "
//					FillInfoMsg( M_WaitPumpOnline, 0, 0 );

					// No Trans, Go On....
					// Get Pump Totals Next
					Set_Next_Command( CR_CRPOWERON_PROC, CR_PUMP_TOTALS );
				}
			
			break; 

			case CR_PUMP_TOTALS:

				// Get Pump Totals
				retval = GetPumpTotals();

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
//#ifdef MAKE_CARD_MODE2
//						// Save Totals
//						SaveTotals();
//#endif

						// Update Log On Card Totals,.....
						UpdateDispTotal_PowerOn();						

						// Write Log
						WriteTotalLog( 0 );

						// Normal Proc Next
//						// Card Reader Proc Next
//						Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
						// Check Time Next
						Set_Next_Command( CR_CRPOWERON_PROC, CR_CHECK_TIME );
					}
					else
					{
						// Retry Reading Totals
//						Set_Next_Command( CR_CRPOWERON_PROC, CR_PUMP_TOTALS );
						// Retry Check Card, And Spring Out If Exist
						Set_Next_Command( CR_CRPOWERON_PROC, CR_CARDREADER_CHECK_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			// Loop For Check Time: Check Card And Spring Card
			case CR_CARDREADER_CHECK_CARD_POWERON:

				// Check IC Card Inserted Or Not
				retval = CR_CardReaderCheckCard();
			
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Spring Out IC Card, For First Power On
						Set_Next_Command( CR_CRPOWERON_PROC, CR_CARDREADER_SPINGOUT_CARD_POWERON );
					}
					else
					{
						// Check Time Next
						Set_Next_Command( CR_CRPOWERON_PROC, CR_CHECK_TIME );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;
			
			case CR_CARDREADER_SPINGOUT_CARD_POWERON:

				// Spring Out IC Card
				retval = CR_CardReaderSpringOutCard();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Check Time Next
						Set_Next_Command( CR_CRPOWERON_PROC, CR_CHECK_TIME );
					}
					else
					{
						// Spring Out Error
						// Display: "Spring Out IC Card Error"
						
						// Check Time PUMP Next
						Set_Next_Command( CR_CRPOWERON_PROC, CR_CHECK_TIME );
					}
				}
				else
				{
					// Command Running, Go On
				}
			
			break;

			case CR_CHECK_TIME:

				// Check Pump Time With The Last Transaction, While Pos Is Offline
				// To Avoid The Small TTC But Late Time
				if( ProcStatus[PROCESS_POS] & POS_ONLINE )
				{

#ifdef MAKE_CARD_MODE2

					// Work Mode May Be Set To Offline, Update The Work Mode Before Enter IC Card Process
					UpdateScreen();

#endif	// MAKE_CARD_MODE2

					// Display " Default Info Line "
					FillInfoMsg( M_DefaultInfo, 0, 0 );

					// Pos Online, Do Not Need Check Time
					// Normal Proc Next
					// Card Reader Proc Next
					Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
				}
				else
				{
					// POS Offline, Check The Time
					retval = CheckTime();

					if( retval != 1 )
					{
						if( retval == 0x9000 )
						{
							// Time Ok, Card Reader Proc Next
							// Display " Default Info Line "
							FillInfoMsg( M_DefaultInfo, 0, 0 );

							Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
						}
						else
						{
							// Check Time Error, Inform POS Online, And Check Again
							// Display " Need POS Online "
							FillInfoMsg( M_NeedCheckTimeOnPOS, 0, 0 );

							Set_Next_Command( CR_CRPOWERON_PROC, CR_CARDREADER_CHECK_CARD_POWERON );
						}
					}
					else
					{
						// Go On....
					}
				}
			
			break;
			
			default:

			break;
		}
	}
}


/*
 ** UpdateDispTotal_PowerOn
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPowerOn.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Update The Display Totals And Duty Totals On Power On Proc
 *
 *  RETURNS: None.
 *
 */
void UpdateDispTotal_PowerOn( void )
{
	unsigned char PhiscalNozzle, Nozzles;
	unsigned int value, sum;

	for( PhiscalNozzle = 0; PhiscalNozzle < 4; PhiscalNozzle++)
	{
		// Default: 0xFF Means No Nozzle Number
		DISP_CR_Buf.DispTotal[PhiscalNozzle].PhiscalNozzle = 0xFF;
		DISP_CR_Buf.DispTotal[PhiscalNozzle].LogicalNozzle = 0xFF;
	}
	
	// Get Nozzle Numbers
	Nozzles = GetGUN_N();
	
	for( PhiscalNozzle = 0; PhiscalNozzle < Nozzles; PhiscalNozzle++ )
	{
		DISP_CR_Buf.DispTotal[PhiscalNozzle].PhiscalNozzle = PhiscalNozzle;
		DISP_CR_Buf.DispTotal[PhiscalNozzle].LogicalNozzle = GetLogicNzn( PhiscalNozzle );
	
		// Get Volume Total
		memcpy( &value, &Pump_CR_Buf.VolumeTotal[4*PhiscalNozzle], 4 );
		IntToCharCopy( value, DISP_CR_Buf.DispTotal[PhiscalNozzle].VolumeTotal );

		memcpy( &value, &Pump_CR_Buf.AmountTotal[4*PhiscalNozzle], 4 );
		IntToCharCopy( value, DISP_CR_Buf.DispTotal[PhiscalNozzle].AmountTotal );

		// Logged On, Update The Duty Totals
		if( ProcStatus[PROCESS_CR] & CARD_LOGON )
		{
			// Update Duty Volume
			sum = CharToIntCopy( DISP_CR_Buf.DispTotal[PhiscalNozzle].VolumeTotal );
			value = CharToIntCopy( &PowerDown.LogOnCard.LogOnVolumeTotals[PhiscalNozzle*4] );

			if( sum > value )
			{
				sum -= value;
				IntToCharCopy( sum, DISP_CR_Buf.DispTotal[PhiscalNozzle].DutyVolumeTotal );
			}

			// Update Duty Amount
			sum = CharToIntCopy( DISP_CR_Buf.DispTotal[PhiscalNozzle].AmountTotal );
			value = CharToIntCopy( &PowerDown.LogOnCard.LogOnAmountTotals[PhiscalNozzle*4] );

			if( sum > value )
			{
				sum -= value;
				IntToCharCopy( sum, DISP_CR_Buf.DispTotal[PhiscalNozzle].DutyAmountTotal );
			}

#ifdef MAKE_CASH_CR
			// Update Duty Cash Volume
			sum = CharToIntCopy( &PowerDown.LogOnCard.LogOnVolumeTotals[PhiscalNozzle*4] );
			value = CharToIntCopy( &PowerDown.PDDutyCashVolumeTotal[PhiscalNozzle*4] );
			if( sum > value )
			{
				IntToCharCopy( value, DISP_CR_Buf.DispTotal[PhiscalNozzle].DutyCashVolumeTotal );
			}

			// Update Duty Cash Amount
			sum = CharToIntCopy( &PowerDown.LogOnCard.LogOnAmountTotals[PhiscalNozzle*4] );
			value = CharToIntCopy( &PowerDown.PDDutyCashAmountTotal[PhiscalNozzle*4] );
			if( sum > value )
			{
				IntToCharCopy( value, DISP_CR_Buf.DispTotal[PhiscalNozzle].DutyCashAmountTotal );
			}
#endif

		}
	}	
}						


/*
 ** CheckTime
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPowerOn.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check The Pump Time With The Last Transaction Time
 *
 *  RETURNS: 1: Running, 0x9000: Time Is Ok, Go Next
 *			 0: Need Check Time On POS
 *
 */
unsigned short int CheckTime( void )
{
	unsigned char tmp;
	unsigned short int retval;

	unsigned char CurrentTime[7];
	
	JOURNAL tempbuf;

	// Default Running
	retval = 1;

	// Get The Current Time
	ShortIntToCharCopy( SysBCDTime.year, CurrentTime );
	memcpy( &CurrentTime[2], &SysBCDTime.month, 2 );
	memcpy( &CurrentTime[4], &SysBCDTime.hour, 3 );

//	if( PowerDown.PdTrans.PD_TRANS_FLAG != TRANS_START )
	// 0xFF: Maybe First Running Or The Protect Data Was Corrupted
	if(( PowerDown.PdTrans.PD_TRANS_FLAG != TRANS_START ) && ( PowerDown.PdTrans.PD_TRANS_FLAG != 0xFF ))
	{
		// Last Transaction Is In PowerDown Protect
		// Get The Trans Date And Time
		memcpy( tempbuf.TRANS_DATE, PowerDown.PdTrans.PD_TRANS_DATE, sizeof( tempbuf.TRANS_DATE ));
		memcpy( tempbuf.TRANS_TIME, PowerDown.PdTrans.PD_TRANS_TIME, sizeof( tempbuf.TRANS_TIME ));
	
	}
	else
	{
		// Check The Last Transaction In Journal
		// Get TTC, First
		TransTTC = GetWriteTTC();
		TransTTC--;
		if( TransTTC == 0 )
		{
			TransTTC = 0xFFFFFFFF;
		}

		tmp = FindRecordTTC( TransTTC, &tempbuf.JOURNALSIGN );
		
		if( tmp != 0 )
		{
			// No Transaction, Need Not Check
			retval = 0x9000;

			return retval;
		}
		else
		{
			// Check The Time
		}
	}

	if( memcmp( CurrentTime, tempbuf.TRANS_DATE, 7 ) >= 0 )
	{
		// Currnet Time Is Later Than Last Transaction Time, Go On....
		retval = 0x9000;
	}
	else
	{
		if( tempbuf.TRANS_DATE[0] == 0xFF )
		{
			// Maybe No Transaction, Do Not Check Time
			retval = 0x9000;
		}
		else
		{
			// Currnet Time Is Earlier Than Last Transactino, Need Check On POS
			retval = 0;
		}
	}

	return retval;
}
