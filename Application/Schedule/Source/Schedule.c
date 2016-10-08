
#include "global.h"
#include "define.h"
#include "CRVaribles.h"
#include "PdProtect.h"

#include "Schedule.h"



//unsigned char SchPriceUpdataFlag;

/*
 ** ScheduleProcess
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Schedule\Source\Schedule.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION:  Main Entrance For Schedule Process, Currently, Only For Price Update
 *
 *  RETURNS: None.
 *
 */
void ScheduleProcess( void )
{
	unsigned short retval;

	retval = GetPriceUpdateFlg();

	if( retval == 2 )
	{
		// Need Update Price
		// Check IC Card
		if( !( ProcStatus[PROCESS_CR] & CARD_BUSY ))
		{
//			if( !( ProcExchange[PROCESS_POS][PROCESS_CR] & PRICE_RECV ))
//			{
//				if( PowerDown.PdTrans.PD_MULTI_TRANS_FLAG == MULTI_TRANS_START)
//				{
					if( PowerDown.PdTrans.PD_TRANS_FLAG == TRANS_START )
					{
						// IC Card Not Busy, Log Off Next
						if( ProcStatus[PROCESS_CR] & CARD_LOGON )
						{
							// IC Card Logg On, Inform IC Card For Price Update Log Off
							ProcExchange[PROCESS_SCHEDULE][PROCESS_CR] |= PRICEUPDATE_LOGOFF;
						}
						else
						{
							// IC Card Logg Off, Inform Pump For Price Update
							// Inform Pump Process Update The Prices
							SCH_GetNextPrice();

							ProcExchange[PROCESS_SCHEDULE][PROCESS_PUMP] |= PRICE_UPDATE;
						
							// Important, After Pump Updated, Should Clear The PriceUpdateFlg
							// Use SetPriceUpdateFlg()
							// Informed Pump, Avoid Re-Enter To Setup PRICE_UPDATE
							SetPriceUpdateFlg( 4 );

							// Mark Price Update
							ProcStatus[PROCESS_SCHEDULE] |= PRICE_UPDATING;
						}
					}
					else
					{
						// There Is A Transaction Need To Be Processed, Waiting....
					}
//				}
//				else
//				{
//					// There Is Transactions Need To Be Processed, Waiting....
//				}
//			}
//			else
//			{
//				// Need To Produce Price Update Record, Just Waiting
//			}
		}
		else
		{
			// IC Card Busy, Waiting
		}
	}
	else if( retval == 10 )
	{
		//
		if( ProcStatus[PROCESS_SCHEDULE] & PRICE_UPDATING )
		{
			// Price Update Just End
			// Clear The Bit
			ProcStatus[PROCESS_SCHEDULE] &= ( ~PRICE_UPDATING );
		
#ifdef MAKE_CARD_MODE2
			// For Sinopec Mode2, Record Price Updated
			ProcExchange[PROCESS_SCHEDULE][PROCESS_CR] |= PRICE_UPDATED;
#endif
		
		}
		else
		{
			// No Price Update, Go On....
		}
	}
	else
	{
		// No Price update, Go On....
	}
}

/*
 ** SCH_GetNextPrice
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Schedule\Source\Schedule.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get The Next Price To PumpPriceBuf For Pump Price Update
 *
 *  RETURNS: None.
 *
 */
void SCH_GetNextPrice( void )
{
//	unsigned short int price;
	unsigned char nozzle, nozzle_num;

	// Clear The Buffer
	memset( PumpPriceBuf, 0x00, 4 * sizeof( PumpPriceBuf[0].NextPrice ));

	// Get Nozzle Numbers
	nozzle_num = GetGUN_N();
	
	for( nozzle = 0; nozzle < nozzle_num; nozzle++ )
	{
		// Get The Next Price
		PumpPriceBuf[nozzle].NextPrice = GetNextPrice( nozzle, 0 );
	}
}
