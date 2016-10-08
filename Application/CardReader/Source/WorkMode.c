
#include <string.h>


#include "define.h"
#include "CRVaribles.h"
#include "CRMemcopy.h"
#include "global.h"
#include "Pdprotect.h"
#include "Display.h"

#include "WorkMode.h"




//#ifdef MAKE_CARD_MODE2

/*
 ** GetWorkMode
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\WorkMode.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check The Current Work Mode
 *
 *  RETURNS: 0: Work Mode Is Offline; 1: Work Mode Is Online.
 *
 *	ATTN: Before The Offline Line Record Is Uploaded To Host, The Work Mode Remains Offline
 *
 */
unsigned char GetWorkMode( void )
{
	unsigned char retval;
	unsigned int val;
	
	// Check The Mode
	if( PowerDown.WorkMode == 0x01 )
	{
		if( ProcStatus[PROCESS_POS] & POS_ONLINE )
		{
			// Check Journal Space limit
			val = GetRecordSpace();

			// RecordMax - 1: Maybe The Last Trans Did Not Uploaded.
			if( val >= ( RecordMax - 1 ))
			{
				// Work Online
				retval = 0x01;
			}
			else
			{
				// Work Offline
				retval = 0x00;
			}
		}
		else
		{
			// Work Offline
			retval = 0x00;
		}
	}
	else
	{
		// Work Offline
		retval = 0x00;
	}

	//
	return retval;
}

/*
 ** SetWorkMode
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\WorkMode.c
 *
 *  PARAMETERS:	mode: 0: Set The Work Mode To Offline; 1: Set The Work Mode To Online; Other: Error
 *
 *  DESCRIPTION: Set The Work Mode
 *
 *  RETURNS: None.
 *
 */
void SetWorkMode( unsigned char mode )
{
	unsigned char val;
	
	val = mode;
	
	if( val == 0x00 )	
	{
		PowerDown.WorkMode = 0x00;

		// For Work Mode Offline, Auth Mode Must Be Pump
//		PowerDown.WorkMode = 0x00;
	}
	else if( val == 0x01 )
	{
		PowerDown.WorkMode = 0x01;
	}
	else
	{
		// Error, Do Not Change
	}

	// Save To DataFlash
	SaveCRConfigFlash();

	// Update Display
	UpdateScreen();
}




/*
 ** GetAuthMode
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\WorkMode.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check The Current Auth Mode
 *
 *  RETURNS: 0: Auth Mode Is FS Or Pump; 1: Auth Mode Is Host POS.
 *
 */
unsigned char GetAuthMode( void )
{
	unsigned char retval;
	
	// Check The Mode
	if( PowerDown.WorkMode == 0x00 )
	{
		// Work Offline, Auth Mode Must Be FS Or Pump
		retval = 0x00;
	}
	else if( PowerDown.AuthMode == 0x01 )
	{
		// Host Pos Auth
		retval = 0x01;
	}
	else
	{
		// FS Or Pump Auth
		retval = 0x00;
	}

	//
	return retval;
}

/*
 ** SetAuthMode
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\WorkMode.c
 *
 *  PARAMETERS:	mode: 0: Set The Auth Mode To Front Pump Auth; 1: Set The Auth Mode To Host POS; Other: Error
 *
 *  DESCRIPTION: Set The Auth Mode
 *
 *  RETURNS: None.
 *
 */
void SetAuthMode( unsigned char mode )
{
	unsigned char val;
	
	val = mode;
	
	if( val == 0x00 )	
	{
		PowerDown.AuthMode = 0x00;
	}
	else if( val == 0x01 )
	{
		PowerDown.AuthMode = 0x01;
	}
	else
	{
		// Error, Do Not Change
	}

	// Save To DataFlash
	SaveCRConfigFlash();

	// Update Display
	UpdateScreen();
}




//#endif
