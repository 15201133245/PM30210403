#include <string.h>

#include "global.h"

#include "ICCardCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "ICCardINProc.h"
#include "Pdprotect.h"
#include "define.h"
//#include "CRMessage.h"
#include "CRSFunc.h"
#include "Message.h"
#include "Screen.h"
#include "Display.h"

#ifdef MAKE_CARD_MODE2

#include "POSMode2.h"

#endif


/*
 ** Card_Validate_Check
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check IC Card Validation, Including: Validate Date Check
 *														Expire Date Check etc.
 *
 *  RETURNS: 0: IC Card Validation; 1: Loyalty App But Not Validated yeat; 2: IC Card Not Validate
 *
 */
unsigned short int Card_Validate_Check()
{
	unsigned short int retval;
	unsigned char DateFlag;
//	unsigned int CardDate, CurrentDate;
	
	// Defautl OK
	retval = 0;

	// Check Loyalty App But Not Used Yeat
	if( TRINP.TR_TRANS_DEBITSOURCE == 0x01 )
	{
		DateFlag = CompareDate( TRINP.TR_CARD_APP_VALIDATE_DATE, "\x00\x00\x00\x00" );
		if( DateFlag == 0 )
		{
			// Display: Card Not Validate
			FillInfoMsg( M_CardUnValidate, 0, 0 );

			retval = 1;

			return retval;
		}
	}

	// Check Validate Time Format
	retval = CheckCardTime( TRINP.TR_CARD_APP_VALIDATE_DATE );
	if( retval != 0x9000 )
	{
		// Display: Card Validate Time Format Error
		FillInfoMsg( M_Test_CardUnValidate, 0, 0 );

		retval = 2;

		return retval;
	}
	
	// Check Expire Date Time Format
	retval = CheckCardTime( TRINP.TR_CARD_APP_EXPIRY_DATE );
	if( retval != 0x9000 )
	{
		// Display: Card Validate Time Format Error
		FillInfoMsg( M_Test_CardExpired, 0, 0 );

		retval = 3;

		return retval;
	}
	
	// Check Validate Date
	DateFlag = CompareDate( TRINP.TR_TRANS_DATE, TRINP.TR_CARD_APP_VALIDATE_DATE );
	if( DateFlag == 2 )
	{
		// Display: Card Not Validate
		// Card Not Validate
		FillInfoMsg( M_CardUnValidate, 0, 0 );

		retval = 4;
	}
	else
	{
		// Check Expiry Date
		DateFlag = CompareDate( TRINP.TR_TRANS_DATE, TRINP.TR_CARD_APP_EXPIRY_DATE );
		if( DateFlag == 1 )
		{
			// Display: Card Expired
			// Card Expired
			FillInfoMsg( M_CardExpired, 0, 0 );

			retval = 5;
			
			//Douzhq 20140916 Add , Voice Add
			ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_CARDERR;
			
			// It's For Test , Douzhq Add , 20140919
			//retval = 0;
		}
		else
		{
			// Card Ok
			retval = 0;
		}
	}

    if( TRINP.TR_CARD_ASN[0] != 0x01 )
	{
		// IC Card Type Error
		FillInfoMsg( M_Test_Type_Err, 0, 0 );

		retval = 6;

		return retval;
	}

//	// default Error
//	retval = 1;
//
//	// Check Validate Date And Expire Date
//	DateFlag = CompareDate( TRINP.TR_CARD_APP_VALIDATE_DATE, "\x00\x00\x00\x00" );
//	if( DateFlag == 0 )
//	{
//		// Display: Card Not Validate
//		FillInfoMsg( M_CardUnValidate, 0, 0 );
//
//		retval = 1;
//	}
//	else
//	{
//		DateFlag = CompareDate( TRINP.TR_TRANS_DATE, TRINP.TR_CARD_APP_VALIDATE_DATE );
//		if( DateFlag == 2 )
//		{
//			// Display: Card Not Validate
//			// Card Not Validate
//			FillInfoMsg( M_CardUnValidate, 0, 0 );
//
//			retval = 1;
//		}
//		else
//		{
//			DateFlag = CompareDate( TRINP.TR_TRANS_DATE, TRINP.TR_CARD_APP_EXPIRY_DATE );
//			if( DateFlag == 1 )
//			{
//				// Display: Card Expired
//				// Card Expired
//				FillInfoMsg( M_CardExpired, 0, 0 );
//
//				retval = 1;
//			}
//			else
//			{
//				// Card Ok
//				retval = 0;
//			}
//		}
//	}

	return retval;
}

/*
 ** PSAM_Check_Validate
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check PSAM Validation, Including: Validate Date Check
 *														Expire Date Check etc.
 *
 *  RETURNS: 0: PSAM Validation; 1: PSAM Error
 *
 */
unsigned short int PSAM_Check_Validate()
{
	unsigned short int retval;
	unsigned char DateFlag;
	
	// default Error
	retval = 1;

	// Check Validate Date And Expire Date
	DateFlag = CompareDate( TRINP.TR_TRANS_DATE, TRINP.TR_PSAM_VALIDATE_DATE );
	if( DateFlag == 2 )
	{
		// Display: PSAM Not Validate
		// PSAM Not Validate
		FillInfoMsg( M_PSAMUnValidate, 0, 0 );

		retval = 1;
	}
	else
	{
		DateFlag = CompareDate( TRINP.TR_TRANS_DATE, TRINP.TR_PSAM_EXPIRE_DATE );
		if( DateFlag == 1 )
		{
			// Display: PSAM Expired
			// PSAM Expired
			FillInfoMsg( M_PSAMExpired, 0, 0 );

			retval = 1;
		}
		else
		{
			// PSAM Ok
			retval = 0;
		}
	}

	return retval;
}




///*
// ** CR_Check_Black_List
// *
// *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardProc.c
// *
// *  PARAMETERS:	None.
// *
// *  DESCRIPTION: Check Black List
// *
// *  RETURNS: 1: Running Checking List; 0x9000: IC Card Ok, Go To Next Command
// *			 0: IC Card Is In Blacklist, Stop The Process; 2: Can not Check Blacklist On POS,
// *																Maybe POS Comm Timeout, POS Offline....
// *
// */
//
//unsigned short int CR_Check_Black_List( void )
//{
//	unsigned short int retval;
//
//	// Default Running
//	retval = 1;
//
//	// Check First Running, Or Not
//	if( !( CR_Task_Flag & ICCARD_POS_BLACK_LIST ))
//	{
//		// Default Use Local List
//		TRINP.TR_TRANS_TYPE |= LOCALLIST;
//
//		// Display " IC Card Need Check Black List Local "
//		FillInfoMsg( M_LocalBlackList, 0, 0 );
//
//		// Check Local Black List
//		retval = CR_BlackLists_Local();
//		
//		if( retval == 0 )
//		{
//			// Not In Black List, Go On ....
//			retval = 0x9000;
//		}
//		else if( retval == 1 )
//		{
//			// In The Black List, Stop ....
//			retval = 0;
//		}
//		else
//		{
//			// Need Check On POS
//			retval = 1;
//
//			// Check Black List On POS
//			// Check POS Online Or Not
//			if( ProcStatus[PROCESS_POS] & POS_ONLINE )
//			{
//				// Display " IC Card Need Check Black List On POS "
//				FillInfoMsg( M_POSBlackList, 0, 0 );
//
//				// Set Bit, Local Process Flag
//				CR_Task_Flag |= ICCARD_POS_BLACK_LIST;
//
//				// Clear Local List Flag, Check POS List
//				TRINP.TR_TRANS_TYPE &= ( ~LOCALLIST );
//
//				// Display: " Check White List In POS "
//				// Set Bit, To Inform POS Check White List
//				ProcExchange[PROCESS_CR][PROCESS_POS] |= POS_CHECK_BLACK;
//			}
//			else
//			{
//				// Display: " Can Not Check Black List In POS "
//				// Pos Offline, Mark Error
//				retval = 2;
//			}
//		}
//	}
//	else
//	{
//		// Check POS Response
//		if( ProcExchange[PROCESS_POS][PROCESS_CR] & POS_CHECK_BLACK )
//		{
//			// POS Check Black Ended
//			// Clear The Bit
//			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~POS_CHECK_BLACK );
//					
//			// Clear Local Process Flag
//			CR_Task_Flag &= ( ~ICCARD_POS_BLACK_LIST );
//
//			// POS Response, Check The Fit Or Not
//			if( POS_CR_Buf.CheckFlag == 0 )
//			{
//				// Display: " Black Card "
//				// Fitted, Black Card,
//				retval = 0x00;
//			}
//			else if( POS_CR_Buf.CheckFlag == 1 )
//			{
//				// UnFitted, Not Black Card,
//				retval = 0x9000;
//			}
//			else
//			{
//				// Error,
//				// Display: " Can Not Check Black List In POS "
//				retval = 0x02;
//			}
//		}
//		else
//		{
//			// Running, Go on....
//		}
//	}
//
//	return retval;
//}

/*
 ** CR_Check_White_List
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Black List
 *
 *  RETURNS: 1: Checking List; 0x9000: IC Card Ok, Go To Next Command
 *			 0: IC Card Is Not In White List, Stop The Process; 2: Can not Check White list On POS,
 *																Maybe POS Comm Timeout, POS Offline....
 *
 *
 */
unsigned short int CR_Check_White_List( void )
{
	unsigned short int retval;

	// Default Running
	retval = 1;

	// Check First Running, Or Not
	if( !( CR_Task_Flag & ICCARD_POS_WHITE_LIST ))
	{
		// Check The Whitelist Status OK Or Not
		if( GetWhiteListIntegrality() == 0 )
		{
			// Display: " Check White List Local "
			// Display " Local White List"
			FillInfoMsg( M_LocalWhiteList, 0, 0 );


			// Check WhiteList In Buffer
			// cr_protected.WhiteListIntegrality == 0
			// unsigned char FindWhiteList(unsigned char *s)
			
			if( FindWhiteList( TRINP.TR_CARD_ASN ) == 0 )
			{
				TRINP.TR_TRANS_TYPE |= LOCALLIST;
				// IC Card In White List
				retval = 0x9000;
			}
			else
			{
				// IC Card Not In White List
				retval = 0;
			}
		}
		else
		{
			// White Not Ok, Check WhiteList On POS
			// Check White List On POS
//			if( POSStatus == 0 )
			// Check POS Online Or Not
			if( ProcStatus[PROCESS_POS] & POS_ONLINE )
			{
				// Set Bit, Local Process Flag
				CR_Task_Flag |= ICCARD_POS_WHITE_LIST;

				// Clear Local List Flag, Check POS List
				TRINP.TR_TRANS_TYPE &= ( ~LOCALLIST );

				// Display: " Check White List In POS "
				// Display " POS White List"
				FillInfoMsg( M_POSWhiteList, 0, 0 );

				// Set Bit, To Inform POS Check White List
				ProcExchange[PROCESS_CR][PROCESS_POS] |= POS_CHECK_WHITE;
			}
			else
			{
				// Display " Need Check White List On POS"
				FillInfoMsg( M_NeedPOSWhiteList, 0, 0 );

				// Pos Offline, Mark Error
				retval = 2;
			}
		}
	}
	else
	{
		// Check POS Return
		if( ProcExchange[PROCESS_POS][PROCESS_CR] & POS_CHECK_WHITE )
		{
			// POS Check White Ended
			// Clear The Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~POS_CHECK_WHITE );
					
			// Clear Local Process Flag
			CR_Task_Flag &= ( ~ICCARD_POS_WHITE_LIST );

			// POS Response, Check The Fit Or Not
			if( POS_CR_Buf.CheckFlag == 0 )
			{
				// Display: " White Card "
				// Fitted, White Card,
				retval = 0x9000;
			}
			else if( POS_CR_Buf.CheckFlag == 1 )
			{
				// Display: " Not White Card "
				// UnFitted, Not White Card,
				retval = 0x00;
			}
			else
			{
				// Error,
				// Display: " Can Not Check White List In POS "
				retval = 0x02;
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
 ** CR_BlackLists_Local
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Find The IC Card ASN In Local BlackLists Include: Base BlackList
 *				  New Added BlackList And New Deleted Blacklist
 *
 *  RETURNS: 0: Not In The Blacklist, And Go On For The Next Step
 *			 1: In The Blacklist, And Stop The Fuelling
 *			 2: Can Not Check Local Black List, Need Check POS
 *
 */
unsigned short int CR_BlackLists_Local( void )
{
	unsigned short int retval;
	unsigned int tmplist;
	unsigned char BlackListHead[15];
	
	//20120820 start liuzg for ShiHuaYinKe Black find
	unsigned char AddListHead[15];
	unsigned char DelListHead[15];
	unsigned char buf[7];
	//20120820 end liuzg
	
	// Default In The Black List
	retval = 1;

	// Use Local Black List
	TRINP.TR_TRANS_TYPE |= LOCALLIST;

	// Display " IC Card Need Check Black List Local "
	FillInfoMsg( M_LocalBlackList, 0, 0 );

	if(( GetBaseBlackListIntegrality() == 0 ) &&
		( GetAddBlackListIntegrality() == 0 ) &&
		( GetDelBlackListIntegrality() == 0 ))
	{
		// All 3 BlackLists Are Integrity
		if( TRINP.TR_PSAM_TID[1] == ((( TRINP.TR_CARD_ASN[3] & 0x0F ) * 0x10 ) + (( TRINP.TR_CARD_ASN[4] & 0xF0 ) >> 4 )))
//		if( TRINP.TR_PSAM_ASN[1] == TRINP.TR_CARD_ISSUER_ID[0] )
		{
			// Local IC Card
			// Get The Head Of The Black Lists Head
			GetBaseListData_Area( BlackListHead );
			
			//20120820 start liuzg for ShiHuaYinKe Black find
			GetAddListData_Area(AddListHead);
			GetDelListData_Area(DelListHead);
			ReadSystimer(buf);
			//20120820 end liuzg
			
			// Get Current Date
			GetTransTime();

			// Check Blacklist Validate Date First
			
			//20120820 start liuzg for ShiHuaYinKe Black find
			if(CompareDate(buf,&BlackListHead[0]) == 2)
			{
				return 2;
			}
			if(CompareDate(buf,&AddListHead[0]) == 2)
			{
				return 2;
			}
			if(CompareDate(buf,&DelListHead[0]) == 2)
			{
				return 2;
			}
			if(CompareDate(buf,&BlackListHead[4]) == 1)
			{
				return 2;
			}
			if(CompareDate(buf,&AddListHead[4]) == 1)
			{
				return 2;
			}
			if(CompareDate(buf,&DelListHead[4]) == 1)
			{
				return 2;
			}
			//20120820 end liuzg
			if( CompareDate( TRINP.TR_TRANS_DATE, &BlackListHead[0] ) != 2 )
			{
				// Check Blacklist Expire Date First
				
				//20120820 start liuzg for ShiHuaYinKe Black find
				//if( CompareDate( TRINP.TR_TRANS_DATE, &BlackListHead[4] ) == 2 )
				//20120820 end liuzg
				{
					// Black List Not Expire
					// Check Card Record Date And Black List Validate Date
					
					if( CompareDate( TRINP.TR_CARD_RECORD_DATE, &BlackListHead[0] ) == 1 )
					{
						// Record Date Is Later Than Validate Date, Check Local
						retval = FindDelBlackList( TRINP.TR_CARD_ASN );
						if( retval == 1 )
						{
							// Not In New Delete Blacklist, Check New Add Black List
							retval = FindAddBlackList( TRINP.TR_CARD_ASN );
							if( retval == 1 )
							{
								// Not In The New Add Blacklist, Check Base Black List
								retval = FindBaseBlackList( TRINP.TR_CARD_ASN );
								if( retval == 1 )
								{
									// Not In The Base Blacklist, Check Black List Items
									tmplist = CharToIntCopy( &BlackListHead[10] );
									if( tmplist > BlackListMax )
									{
										retval = 2;
									}
									else
									{
										// Not In Black List, Go On ....
										retval = 0;
									}
								}
								else if( retval == 0 )
								{
									// In The Base Blacklist, Stop Fuelling ....
									retval = 1;
								}
								else
								{
									// Check The Base Add BlackList Error, Check On POS ....
									retval = 2;
								}
							}
							else if( retval == 0 )
							{
								// In The New Add BlackList, Stop Fuelling ....
								retval = 1;
							}
							else
							{
								// Check The New Add BlackList Error, Check On POS ....
								retval = 2;
							}
						}
						else if( retval == 0 )
						{
							// In The New Delete Black List, Start Fuelling ....
							retval = 0;
						}
						else
						{
							// Check New Delete Black List Error, Check On POS
							retval = 2;
						}
					}
					else
					{
						// Record Date Is Earlier Than Validate Date, Check On POS
						retval = 2;
					}
				}
				//20120820 liuzg
				/*else
				{
					// Current Date Is Later Then Expire Date, Check On POS
					retval = 2;
				}*/
			}
			else
			{
				// Current Date Is Earlier Then Validate Date, Check On POS
				retval = 2;
			}
		}
		else
		{
			// Not Local IC Card, Check On POS
			retval = 2;
		}
	}
	else
	{
		// Not Integrity, Check On POS
		retval = 2;
	}

	return retval;
}

/*
 ** CR_BlackLists_POS
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check IC Card ASN On POS For Blac kList
 *
 *  RETURNS: 1: Running Checking List; 0x9000: IC Card Ok, Go To Next Command
 *			 0: IC Card Is In Blacklist, Stop The Process; 2: Can not Check Blacklist On POS,
 *																Maybe POS Comm Timeout, POS Offline....
 *
 */
unsigned short int CR_BlackLists_POS( void )
{
	unsigned short int retval;

	// Default Running
	retval = 1;

	// Check First Running, Or Not
	if( !( CR_Task_Flag & ICCARD_POS_BLACK_LIST ))
	{
		// Check Black List On POS
		// Check POS Online Or Not
		if( ProcStatus[PROCESS_POS] & POS_ONLINE )
		{
			// Use POS Black List
			TRINP.TR_TRANS_TYPE &= ( ~LOCALLIST );

			// Display " IC Card Need Check Black List On POS "
			FillInfoMsg( M_POSBlackList, 0, 0 );

			// Set Bit, Local Process Flag
			CR_Task_Flag |= ICCARD_POS_BLACK_LIST;

			// Display: " Check Black List In POS "
			// Set Bit, To Inform POS Check White List
			ProcExchange[PROCESS_CR][PROCESS_POS] |= POS_CHECK_BLACK;
		}
		else
		{
			// Display: " Can Not Check Black List In POS "
			// Pos Offline, Mark Error
			retval = 2;
		}
	}
	else
	{
		// Check POS Response
		if( ProcExchange[PROCESS_POS][PROCESS_CR] & POS_CHECK_BLACK )
		{
			// POS Check Black Ended
			// Clear The Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~POS_CHECK_BLACK );
					
			// Clear Local Process Flag
			CR_Task_Flag &= ( ~ICCARD_POS_BLACK_LIST );

			// POS Response, Check The Fit Or Not
			if( POS_CR_Buf.CheckFlag == 0 )
			{
				// Display: " Black Card "
				// Fitted, Black Card,
				retval = 0x00;
			}
			else if( POS_CR_Buf.CheckFlag == 1 )
			{
				// UnFitted, Not Black Card,
				retval = 0x9000;
			}
			else
			{
				// Error,
				// Display: " Can Not Check Black List In POS "
				retval = 0x02;
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
 ** CR_Check_Black_List
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Black List
 *
 *  RETURNS: 1: Running Checking List; 0x9000: IC Card Ok, Go To Next Command
 *			 0: IC Card Is In Blacklist, Stop The Process; 2: Can not Check Blacklist On POS,
 *																Maybe POS Comm Timeout, POS Offline....
 *
 */

unsigned short int CR_Check_Black_List( void )
{
	unsigned short int retval;

	// Default Running
	retval = 1;

#ifdef MAKE_CARD_MODE2
	// For Sinopec Mode2, Only Check Local, The POS Black List Need Check With Auth
	retval = CR_BlackLists_Local();

	if( retval == 0 )
	{
		// Not In The Black List, Go On Fuelling
		retval = 0x9000;
	}
	else if( retval == 1 )
	{
		// In The Black List, Stop Fuelling
		retval = 0;
	}
	else
	{
		// Check Black List On POS, Start
		retval = 2;
	}
#else
	// Check On POS First Or Local First
	if( PowerDown.SpecialFunction & CheckBlackListPOS )
	{
		// Check On POS First
		retval = CR_BlackLists_POS();

		if( retval != 1 )
		{
			if( retval == 0x9000 )
			{
				// Check Ok, And IC Card OK
				retval = retval;
			}
			else if( retval == 0 )
			{
				// Check OK, And IC Card Is Black List
				retval = retval;
			}
			else
			{
				// Check Error, Need Check Local
				retval = CR_BlackLists_Local();

				if( retval == 0 )
				{
					// Not In The Black List, Go On Fuelling
					retval = 0x9000;
				}
				else if( retval == 1 )
				{
					// In The Black List, Stop Fuelling
					retval = 0;
				}
				else
				{
					// Check Black List On POS, Start
					retval = 2;
				}
			}
		}
		else
		{
			// Running, Just Go On....
		}
	}
	else
	{
		// Check Local First
		// Local Black List Checked Or Not
		if( !( CR_Task_Flag & ICCARD_POS_BLACK_LIST ))
		{
			// Check Local First
			retval = CR_BlackLists_Local();

			// *  RETURNS: 0: Not In The Blacklist, And Go On For The Next Step
			// *			 1: In The Blacklist, And Stop The Fuelling
			// *			 2: Can Not Check Local Black List, Need Check POS
			if( retval == 0 )
			{
				// Not In The Black List, Go On Fuelling
				retval = 0x9000;
			}
			else if( retval == 1 )
			{
				// In The Black List, Stop Fuelling
				retval = 0;
			}
			else
			{
				// Check Black List On POS, Start
				retval = CR_BlackLists_POS();
			}
		}
		else
		{
			// Local Checked, Check POS Return Only
			retval = CR_BlackLists_POS();
		}
	}

#endif	// MAKE_CARD_MODE2

	return retval;
}


/*
 ** SaveICCardData
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardInProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save The IC Card Data To PowerDown Buffer
 *
 *  RETURNS: None.
 *
 *	Note: Called After Set CARD_READY, Only The IC Card Data
 *			Exclude The Set Grey Data And Fuelling Data etc.
 *
 */
void SaveICCardData( void )
{
	// Save The IC Card ASN
	memcpy( PowerDown.PdTrans.PD_CARD_ASN, TRINP.TR_CARD_ASN, sizeof( PowerDown.PdTrans.PD_CARD_ASN ));

	// Save The IC Card	Application Balance
	memcpy( PowerDown.PdTrans.PD_CARD_APP_BALANCE, TRINP.TR_CARD_APP_BALANCE, sizeof( PowerDown.PdTrans.PD_CARD_APP_BALANCE ));
	
	// PSAM ASN
	memcpy( PowerDown.PdTrans.PD_PSAM_ASN, TRINP.TR_PSAM_ASN, sizeof( PowerDown.PdTrans.PD_PSAM_ASN ));

	// PSAM TID
	memcpy( PowerDown.PdTrans.PD_PSAM_TID, TRINP.TR_PSAM_TID, sizeof( PowerDown.PdTrans.PD_PSAM_TID ));

	// Transaction IC Card Debit Source
	PowerDown.PdTrans.PD_TRANS_DEBITSOURCE = TRINP.TR_TRANS_DEBITSOURCE;
	PowerDown.PdTrans.PD_TRANS_UNIT = TRINP.TR_TRANS_UNIT;
	
	// Transaction IC Card Type
	PowerDown.PdTrans.PD_CARD_TYPE = TRINP.TR_CARD_TYPE;
	
	// For Transaction
	TRINP.TR_TRANS_EMP_NUMBER = PowerDown.LogOnCard.EMPNumber;
	// Transaction Employee Number	
	PowerDown.PdTrans.PD_EMPLOYEE_NUMBER = TRINP.TR_TRANS_EMP_NUMBER;
						
	// Save To PowerDown.PdTrans, Card Tac, For Limited Card
	memcpy( PowerDown.PdTrans.PD_CARD_GTAC, TRINP.TR_CARD_GREY_TAC, sizeof( PowerDown.PdTrans.PD_CARD_GTAC ));

#ifdef MAKE_CARD_MODE2

	// Transaction Nozzle Number
	PowerDown.PdTrans.PD_NOZZLENUMBER = TRINP.TR_TRANS_NOZZLE_NUMBER;
	
	// Grade Code, Product Code	
	memcpy( PowerDown.PdTrans.PD_GRADE_CODE, TRINP.TR_TRANS_GRADE_CODE, sizeof( PowerDown.PdTrans.PD_GRADE_CODE ));
	
#endif
}




/*
 ** Save_ICCard_State
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save IC Card State For POS, And Record
 *
 *  RETURNS: None.
 *
 */
void Save_ICCard_State( void )
{
	unsigned short int limitflag;
	unsigned int limitamount;

	// Check Brand Limit
	limitflag = CharToShortIntCopy( TRINP.TR_CARD_GRADE_LIMIT );
	
	if( limitflag != 0xFFFF )
	{
		// Display "BrandLimited"
		FillInfoMsg( M_MsgBrandLimited, 0, 0 );

		// Inform POS Brand Limited Card
		memcpy( POS_CR_Buf.ICCARD_STATE, "\x10\x01", sizeof( POS_CR_Buf.ICCARD_STATE ));
						
		// For Record Brand Limited Card
		memcpy( TRINP.TR_CARD_GREY_TAC, "\x10\x01\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
						
		SaveICCardData();
	}

	// Check Location Limit
	if( TRINP.TR_CARD_LOCATION_LIMIT_MODE != 0xFF )
	{
		// Display "LocationLimited"
		FillInfoMsg( M_MsgLocationLimited, 0, 0 );
//		CRFillMsg( MsgLocationLimited );

		// Inform POS Brand Limited Card
		memcpy( POS_CR_Buf.ICCARD_STATE, "\x10\x02", sizeof( POS_CR_Buf.ICCARD_STATE ));
						
		// For Record Brand Limited Card
		memcpy( TRINP.TR_CARD_GREY_TAC, "\x10\x02\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
						
		SaveICCardData();
	}

	// Check Fuelling Times Limit
	if( TRINP.TR_CARD_TIMES_DAY_LIMIT != 0xFF )
	{
		// Display "TimesLimited"
		FillInfoMsg( M_MsgTimesLimited, 0, 0 );
//		CRFillMsg( MsgTimesLimited );

		// Inform POS Brand Limited Card
		memcpy( POS_CR_Buf.ICCARD_STATE, "\x10\x03", sizeof( POS_CR_Buf.ICCARD_STATE ));
						
		// For Record Brand Limited Card
		memcpy( TRINP.TR_CARD_GREY_TAC, "\x10\x03\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
						
		SaveICCardData();
	}

	// Check Fuelling Amount Total Limit
	limitamount = CharToIntCopy( TRINP.TR_CARD_AMOUNT_DAY_LIMIT );
	if( limitamount != 0xFFFFFFFF )
	{
		// Display "AmountLimited"
		FillInfoMsg( M_MsgAmountLimited, 0, 0 );
//		CRFillMsg( MsgAmountLimited );

		// Inform POS Brand Limited Card
		memcpy( POS_CR_Buf.ICCARD_STATE, "\x10\x04", sizeof( POS_CR_Buf.ICCARD_STATE ));
						
		// For Record Brand Limited Card
		memcpy( TRINP.TR_CARD_GREY_TAC, "\x10\x04\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
						
		SaveICCardData();
	}

	// Check Vehicle Number Limited
//	memcpy( DISP_CR_Buf.VehicleNumber, TRINP.TR_CARD_VEHICLE_NUMBER, sizeof( DISP_CR_Buf.VehicleNumber ));
						
	for( limitflag = 0; limitflag < 16; limitflag++ )
	{
		if( TRINP.TR_CARD_VEHICLE_NUMBER[limitflag] != 0xFF )
		{
			break;
		}
	}

	if( limitflag < 16 )
	{
//		// Copy Vehicle Number To Display
//		memcpy( DISP_CR_Buf.VehicleNumber, TRINP.TR_CARD_VEHICLE_NUMBER, sizeof( DISP_CR_Buf.VehicleNumber ));

		// Find The Last Charater Of Vehicle Number
		for( limitflag = 0; limitflag < 16; limitflag++ )
		{
			if( TRINP.TR_CARD_VEHICLE_NUMBER[limitflag] != 0xFF )
			{
				continue;
			}
			else
			{
				break;
			}
		}

		// Add The String Ending Flag
		TRINP.TR_CARD_VEHICLE_NUMBER[limitflag]	= 0;

		//Vehicle Number Limited	
		memcpy( POS_CR_Buf.ICCARD_STATE, "\x10\x05", sizeof( POS_CR_Buf.ICCARD_STATE ));
						
		// For Record Vehicle Number Limited Card
		memcpy( TRINP.TR_CARD_GREY_TAC, "\x10\x05\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
						
		SaveICCardData();
	
		// For Input EMP Password
		TRINP.TR_VEHICLE_NUMBER_LIMITED_FLAG = 1;
	}
        else
        {
                TRINP.TR_VEHICLE_NUMBER_LIMITED_FLAG = 0;
          
        }
}


/*
 ** FillCardErrorData
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Fill The Card Error Paras, For The Card Error Record
 *				 Include: Get TTC, Get Trans Date & Time etc..... And Save To PowerDown.PdTrans
 *
 *  RETURNS: None.
 *
 */
void FillCardErrorData( void )
{
	// Get TTC
	GetTransTTC();
	// Save To PowerDown.PdTrans
	memcpy( PowerDown.PdTrans.PD_TRANS_TTC, TRINP.TR_TRANS_TTC, sizeof( PowerDown.PdTrans.PD_TRANS_TTC ));

	// Get Trans Time
	GetTransTime();
	// Save To PowerDown.PdTrans
	memcpy( PowerDown.PdTrans.PD_TRANS_DATE, TRINP.TR_TRANS_DATE, sizeof( PowerDown.PdTrans.PD_TRANS_DATE ));
	memcpy( PowerDown.PdTrans.PD_TRANS_TIME, TRINP.TR_TRANS_TIME, sizeof( PowerDown.PdTrans.PD_TRANS_TIME ));

	// Wrong Card, Black Card
	TRINP.TR_TRANS_TYPE &= 0xF0;
	TRINP.TR_TRANS_TYPE |= CARDERROR;
	TRINP.TR_TRANS_TYPE |= TRANSCARDERR;

	PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
}


/*
 ** Card_Data_Check
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check IC Card Data In SFI = 0x15 And .... For Test Approval
 *
 *  RETURNS: 0x00: IC Card Data Ok, And Go On....
 *			 Other: Error And Set The Display Message
 *
 */
unsigned short int Card_Data_Check( void )
{
	unsigned int i;
	unsigned short int retval;

	// Default OK
	retval = 0;

	// Check Issuer ID
	if((( TRINP.TR_CARD_ISSUER_ID[0] & 0xF0 ) >= 0xA0 ) || (( TRINP.TR_CARD_ISSUER_ID[0] & 0x0F ) >= 0x0A ))
	{
		// Issuer ID Error
		FillInfoMsg( M_Test_IssuerID_Err, 0, 0 );

		retval = 1;

		return retval;
	}
	else
	{
		for( i = 1; i < 8; i++ )
		{
			if( TRINP.TR_CARD_ISSUER_ID[i] != 0xFF )
			{
				// Issuer ID Error
				FillInfoMsg( M_Test_IssuerID_Err, 0, 0 );

				retval = 2;

				return retval;
			}
			else
			{
				// OK, Go On....
			}
		}
	}

	if( TRINP.TR_CARD_APP_TYPE != 0x11 )
	{
		// Application Type Error
		FillInfoMsg( M_Test_AppID_Err, 0, 0 );

		retval = 3;

		return retval;
	}

	if( TRINP.TR_CARD_APP_VERSION != 0x01 )
	{
		// Application Version Error
		FillInfoMsg( M_Test_AppVer_Err, 0, 0 );

		retval = 4;

		return retval;
	}

	if( TRINP.TR_CARD_INSTRUCTION_VERSION != 0x01 )
	{
		// Instruction Version Error
		FillInfoMsg( M_Test_Instruction_Err, 0, 0 );

		retval = 5;

		return retval;
	}

	if( TRINP.TR_CARD_ASN[0] != 0x01 )
	{
		// IC Card Type Error
		FillInfoMsg( M_Test_Type_Err, 0, 0 );

		retval = 6;

		return retval;
	}

	if(( TRINP.TR_CARD_ASN[3] / 16 ) != 0x01 )
	{
		// IC Card Batch Error
		FillInfoMsg( M_Test_Batch_Err, 0, 0 );

		retval = 7;

		return retval;
	}

	if( TRINP.TR_CARD_ASN[2] > 0x06 )
	{
		// IC Card Location Error
		FillInfoMsg( M_Test_CardType_Err, 0, 0 );

		retval = 8;

		return retval;
	}

	if( TRINP.TR_CARD_PIN_DEFAULT > 0x01 )
	{
		// IC Card Default PIN Format Error
		FillInfoMsg( M_Test_PINType_Err, 0, 0 );

		retval = 9;

		return retval;
	}


	return retval;
}


#ifdef MAKE_CASH_CR

/*
 ** FillCashCardOk
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Fill Cash Card Ok, To Inform Cash Process
 *
 *  RETURNS: None.
 *
 */
void FillCashCardOk( void )
{
	unsigned short int tmp2;

	// Can Use
	Cash_CR_Buf.CashCard_Flag = 0x00;
	ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_KEY_PRESSED;

	// Fill Nozzle Number
	Cash_CR_Buf.CashNozzleNumber = GetLogicNzn( Cash_CR_Buf.CashKeyNozzleNumber );
	TRINP.TR_TRANS_NOZZLE_NUMBER = Cash_CR_Buf.CashNozzleNumber;
	PowerDown.PdTrans.PD_NOZZLENUMBER = TRINP.TR_TRANS_NOZZLE_NUMBER;

	// Fill Price
	// Get Price
	tmp2 = GetPrice( Cash_CR_Buf.CashKeyNozzleNumber, 0 );
	ShortIntToCharCopy( tmp2, TRINP.TR_TRANS_PRICE );
	memcpy( PowerDown.PdTrans.PD_TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( PowerDown.PdTrans.PD_TRANS_PRICE ));

	// Fill Grade Code
	tmp2 = GetO_Type( Cash_CR_Buf.CashKeyNozzleNumber );
	ShortIntToCharCopy( tmp2, Cash_CR_Buf.CashGradeNumber );
	ShortIntToCharCopy( tmp2, TRINP.TR_TRANS_GRADE_CODE );
	memcpy( PowerDown.PdTrans.PD_GRADE_CODE, TRINP.TR_TRANS_GRADE_CODE, sizeof( PowerDown.PdTrans.PD_GRADE_CODE ));

	// Get Trans Time
	GetTransTime();
	// Save To PowerDown.PdTrans
	memcpy( PowerDown.PdTrans.PD_TRANS_DATE, TRINP.TR_TRANS_DATE, sizeof( PowerDown.PdTrans.PD_TRANS_DATE ));
	memcpy( PowerDown.PdTrans.PD_TRANS_TIME, TRINP.TR_TRANS_TIME, sizeof( PowerDown.PdTrans.PD_TRANS_TIME ));

	// Set Card Card Ok
	ProcStatus[PROCESS_CR] |= CARD_CASH_OK;
}


/*
 ** CheckBalance
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: For Cash Trans, Balance Should > 9900.00
 *
 *  RETURNS: 0: Balance Ok; 1: Balance Not Enough, Limited
 *
 */
unsigned char CheckBalance( void )
{
	unsigned int value;
	unsigned char retval;

	retval = 1;

	value = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );

	if( value >= CASH_TRANS_BALANCE_LIMITED )
	{
		retval = 0;
	}

	return retval;
}

#endif



#ifdef MAKE_CARD_MODE2
/*
 ** GetPINData
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc2.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get The Pin Data For Calc The Encrypted PIN Block And Saved In MACBuffer
 *
 *  RETURNS: None.
 *
 */
void GetPINData( void )
{
	unsigned char i, j;
//	unsigned char len;
	unsigned char buf[8], buf0[16], buf1[16];

	// First Get PIN
	memset( buf, 0x00, sizeof( buf ));
	memset( buf0, 'F', sizeof( buf0 ));
	memset( buf1, 0x00, sizeof( buf1 ));

	i = TRINP.TR_INPUT_PIN_LEN;
	i *= 2;
	// First Change To Ascii
	Hex2Asc( buf0, TRINP.TR_INPUT_PIN, TRINP.TR_INPUT_PIN_LEN );
	for( j = 0; j < i; j++ )
	{
		if(( buf0[j] == 'F' ) || ( buf0[j] == 'f' ))
		{
			break;
		}
	}

	if( j != i )
	{
		i -= 1;
	}


	// Save Data Length
	buf[0] = i;

	// Save Data

	// Change To Hex
	Asc2Hex( &buf[1], buf0, 7 );

	// Xor ASN
	for( i = 0; i < 8; i++ )
	{
		buf[i] ^= TRINP.TR_CARD_ASN[2+i];
	}

	// Xor Date And Time ( MMDDHHMMSS )
	memset( buf1, 0xFF, sizeof( buf1 ));
	memcpy( buf1, &TRINP.TR_TRANS_DATE[2], 2 );
	memcpy( &buf1[2], TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));

	for( i = 0; i < 8; i++ )
	{
		buf[i] ^= buf1[i];
	}

	// Save To MAC Buffer
	memcpy( MACBuffer, buf, 8 );
}


/*
 ** CardAppendOrNot
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc2.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Wether Offline Record Need To Be Appended Or Nor
 *
 *  RETURNS: 0: No Offline Record Need To Be Appended; 1: At Least 1 Record Need To Be Appended
 *
 */
unsigned short int CardAppendOrNot( void )
{
	unsigned short int retval;

	retval = 0;

	if( QueExportRecord() == ( RecordMax - 1 ))
	{
		if( ProcStatus[PROCESS_POS] & POS_ONLINE )
		{
			retval = 0;
		}
		else
		{
			retval = 1;
		}
	}
	else if( QueExportRecord() == RecordMax )
	{
		retval = 0;
	}
	else
	{
		retval = 1;
	}

	return retval;
}


/*
 ** ReadRecordAppend
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc2.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read And Save The UnAppended Record To Mac Buffer For Cala MAC
 *
 *  RETURNS: 0: Do Not Append; 1: Append Actual.
 *
 */
unsigned short int ReadRecordAppend( void )
{
	unsigned char type;
	unsigned short int retval, val;

	JOURNAL AppendRecord;

	retval = 0;

	// Clear The Buffer
	MACDataLen = 0;
	memset( MACBuffer, 0x00, sizeof( MACBuffer ));

	do
	{
		retval = ReadExportRecord( &AppendRecord.JOURNALSIGN );
		
		if( retval == 0 )
		{
			// No Append, Go On....
			val = CardAppendOrNot();
		}
		else
		{
			type = AppendRecord.TRANS_TYPE & 0x0F;
			if(( type == 0x08 ) || ( type == 0x0A ) || ( type == 0x0B )
				||( type == 0x0C ) || ( type == 0x0D ) || ( type == 0x0E ))
			{
				// Only INC TTC, Do Not Append
				val = 0;
				MakeExportRecord();
			}
			else
			{
				// Need To Be Appended
				memcpy( MACBuffer, AppendRecord.TRANS_TTC, sizeof( JOURNAL ) - 1 );
				MACDataLen = sizeof( JOURNAL ) - 1;
				
				retval = 1;
				
				break;
			}
		}

		WatchDog();

	}while( val == 1 );

	return retval;
}





/*
 ** FillCardErrorData
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Fill The Card Error Paras, Special For BlackList Card In Sinopec Mode2
 *				 Do Not Get TTC And Time For Online Mode, It Was Read In Auth Process Already
 *				 
 *
 *  RETURNS: None.
 *
 */
void FillCardErrorData_Mode2( void )
{
	if( TRINP.TR_WORK_MODE == 0x01 )
	{
		// Work Online, Do Not Get TTC And Time
		// Just Go On....
	}
	else
	{
		// Work Offline
		// Get TTC
		GetTransTTC();

		// Get Trans Time
		GetTransTime();
	}

	// Save To PowerDown.PdTrans
	memcpy( PowerDown.PdTrans.PD_TRANS_TTC, TRINP.TR_TRANS_TTC, sizeof( PowerDown.PdTrans.PD_TRANS_TTC ));

	// Save To PowerDown.PdTrans
	memcpy( PowerDown.PdTrans.PD_TRANS_DATE, TRINP.TR_TRANS_DATE, sizeof( PowerDown.PdTrans.PD_TRANS_DATE ));
	memcpy( PowerDown.PdTrans.PD_TRANS_TIME, TRINP.TR_TRANS_TIME, sizeof( PowerDown.PdTrans.PD_TRANS_TIME ));

	// Wrong Card, Black Card
	TRINP.TR_TRANS_TYPE &= 0xF0;
	TRINP.TR_TRANS_TYPE |= CARDERROR;
	TRINP.TR_TRANS_TYPE |= TRANSCARDERR;

	PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
}


/*
 ** CardLimitInfo
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ICCardInProc2.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Just Display The Card Limit Information For Mode2 Test
 *
 *  RETURNS: None.
 *
 */
void CardLimitInfo( void )
{
	unsigned short int limitflag;
	unsigned int limitamount;
	unsigned char location;

	unsigned char flag = 0;

	// Check Brand Limit
	limitflag = CharToShortIntCopy( TRINP.TR_CARD_GRADE_LIMIT );
	
	if( limitflag != 0xFFFF )
	{
		flag = 1;

		// Display "BrandLimited"
		FillInfoMsg( M_MsgBrandLimited, 0, 0 );
	}

	// Check Location Limit
	if( TRINP.TR_CARD_LOCATION_LIMIT_MODE != 0xFF )
	{
		flag = 1;
		
		location = TRINP.TR_CARD_LOCATION_LIMIT_MODE;

#ifdef MAKE_CARD_MODE2

		if(( TRINP.TR_CARD_HOLDER_NEW_FLAG & CARD_MODE2_NON ) == CARD_MODE2_NON )
		{
			// Not Mode2 Card, Go On....
		}
		else if(( TRINP.TR_CARD_HOLDER_NEW_FLAG & CARD_MODE2_NON ) == 0x00 )
		{
			// Mode2 Card, But RFU, Do Not Care, Go On....
		}
		else
		{
			// Mode2 Card, Adjust Limit Information
			if( TRINP.TR_CARD_LOCATION_LIMIT_MODE >= 2 )
			{
				location -= 1;
			}
			else
			{
				// Do Not Care, Go On....
			}
		}

#endif


//		if( TRINP.TR_CARD_LOCATION_LIMIT_MODE == 0x01 )
		if( location == 0x01 )
		{
			// Display: " Location Limited"
			FillInfoMsg( M_ProvinceLimited, 0, 0 );
		}
//		else if( TRINP.TR_CARD_LOCATION_LIMIT_MODE == 0x02 )
		else if( location == 0x02 )
		{
			// Display: " Location Limited"
			FillInfoMsg( M_CityLimited, 0, 0 );
		}
//		else if( TRINP.TR_CARD_LOCATION_LIMIT_MODE == 0x03 )
		else if( location == 0x03 )
		{
//#ifdef MAKE_CARD_MODE2
//			// For Sinopec Mode2, Do Not Display " Superior Limited"
//			// Display: " Location Limited"
//			FillInfoMsg( M_CityLimited, 0, 0 );
//#else
			// Display: " Location Limited"
			FillInfoMsg( M_SuperiorUnitLimited, 0, 0 );
//#endif
		}
//		else if( TRINP.TR_CARD_LOCATION_LIMIT_MODE == 0x04 )
		else if( location == 0x04 )
		{
			// Display: " Location Limited"
			FillInfoMsg( M_OilStationLimited, 0, 0 );
		}
		else
		{
			// Do Nothing....
		}
	
	}

#ifdef MAKE_CARD_MODE2
	// For Sinopec Mode2, Do Not Check Fuelling Limit For Loyalty App
	if( TRINP.TR_TRANS_DEBITSOURCE == 0x01 )
	{
		// Loyalty App, Do Not Check Fuelling Limit
	}
	else
	{
		// Check Fuelling Times Limit
		if( TRINP.TR_CARD_TIMES_DAY_LIMIT != 0xFF )
		{
			flag = 1;

			// Display "TimesLimited"
			FillInfoMsg( M_MsgTimesLimited, 0, 0 );
		}

		// Check Fuelling Amount Total Limit
		limitamount = CharToIntCopy( TRINP.TR_CARD_AMOUNT_DAY_LIMIT );
		if( limitamount != 0xFFFFFFFF )
		{
			flag = 1;

			// Display "AmountLimited"
			FillInfoMsg( M_TotalAmountLimited_D, 0, 0 );
		}


		// Check Every Fuelling Limit
		// Get The Limit
		limitamount = CharToShortIntCopy( TRINP.TR_CARD_VOLUM_AMOUNT_LIMIT );
		if( limitamount != 0xFFFF )
		{
			// Amount Limit
			if(( limitamount & 0xF000) == 0x0000 )
			{
				flag = 1;

				// Display "AmountLimited"
				FillInfoMsg( M_PerAmountLimited_D, 0, 0 );
			}
			else if(( limitamount & 0xF000) == 0x1000 )
			{
				flag = 1;

				// Display "VolumeLimited"
				FillInfoMsg( M_PerVolumeLimited_D, 0, 0 );
			}
			else
			{
			}
		}
		else
		{
			// Do Nothing....
		}
	}

#else
	
	// Check Fuelling Times Limit
	if( TRINP.TR_CARD_TIMES_DAY_LIMIT != 0xFF )
	{
		flag = 1;

		// Display "TimesLimited"
		FillInfoMsg( M_MsgTimesLimited, 0, 0 );
	}

	// Check Fuelling Amount Total Limit
	limitamount = CharToIntCopy( TRINP.TR_CARD_AMOUNT_DAY_LIMIT );
	if( limitamount != 0xFFFFFFFF )
	{
		flag = 1;

		// Display "AmountLimited"
		FillInfoMsg( M_TotalAmountLimited_D, 0, 0 );
	}


	// Check Every Fuelling Limit
	// Get The Limit
	limitamount = CharToShortIntCopy( TRINP.TR_CARD_VOLUM_AMOUNT_LIMIT );
	if( limitamount != 0xFFFF )
	{
		// Amount Limit
		if(( limitamount & 0xF000) == 0x0000 )
		{
			flag = 1;

			// Display "AmountLimited"
			FillInfoMsg( M_PerAmountLimited_D, 0, 0 );
		}
		else if(( limitamount & 0xF000) == 0x1000 )
		{
			flag = 1;

			// Display "VolumeLimited"
			FillInfoMsg( M_PerVolumeLimited_D, 0, 0 );
		}
		else
		{
		}
	}
	else
	{
		// Do Nothing....
	}
#endif

	if( flag == 0 )
	{
		// Display " Default Info Line"
		FillInfoMsg( M_DefaultInfo, 0, 0 );
	}
}
#endif	// MAKE_CARD_MODE2

