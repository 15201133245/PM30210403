#include "global.h"

#include "define.h"

#include "CRVaribles.h"
#include "CRMemcopy.h"
#include "Keypad.h"
#include "KeyMask.h"
#include "Screen.h"
#include "Display.h"
#include "PdProtect.h"
#include "CRPrint.h"



/*
 ** KeypadProcess
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Main Entrance For KeyPad Process
 *
 *  RETURNS: None.
 *
 */
void KeypadProcess( void )
{
	unsigned char KeyCol, KeyRow;
	
	// KeyMask Was Set In IC Card Proc
	// KeyMask = KEYMASK_FUELLING After Pump Call
	// KeyMask = KEYMASK_DEFAULT; After Power Off Card
	// Check KeyMask
	if( ProcStatus[PROCESS_CR] & CARD_BUSY )
	{
		KeyMask = KEYMASK_FUELLING;
	}
	else
	{
		KeyMask = KEYMASK_DEFAULT;
	}
	// KeyMask Was Set In IC Card Proc

	KeyValue = GetKey();

	if( KeyValue != NOKEY )
	{
		// Key Pressed, Check Key Colume And Row
		if( KeyValue == 0x6C )
		{
			KeyRow = 0;
			KeyCol = 0;
		}
		else if( KeyValue == 0x02 )
		{
			KeyRow = 0;
			KeyCol = 1;
		}
		else if( KeyValue == 0x6B )
		{
			KeyRow = 0;
			KeyCol = 2;
		}
		else if( KeyValue == 0x6A )
		{
			KeyRow = 0;
			KeyCol = 3;
		}
//		else if( KeyValue == 0x )
//		{
//			KeyRow = 0;
//			KeyCol = 4;
//		}
		else if( KeyValue == 0x13 )
		{
			KeyRow = 1;
			KeyCol = 0;
		}
		else if( KeyValue == 0x14 )
		{
			KeyRow = 1;
			KeyCol = 1;
		}
		else if( KeyValue == 0x12 )
		{
			KeyRow = 1;
			KeyCol = 2;
		}
		else if( KeyValue == 0x11 )
		{
			KeyRow = 1;
			KeyCol = 3;
		}
//		else if( KeyValue == 0x )
//		{
//			KeyRow = 1;
//			KeyCol = 4;
//		}
		else if( KeyValue == 0x15 )
		{
			KeyRow = 2;
			KeyCol = 0;
		}
		else if( KeyValue == 0x16 )
		{
			KeyRow = 2;
			KeyCol = 1;
		}
		else if( KeyValue == 0x09 )
		{
			KeyRow = 2;
			KeyCol = 2;
		}
		else if( KeyValue == 0x06 )
		{
			KeyRow = 2;
			KeyCol = 3;
		}
//		else if( KeyValue == 0x )
//		{
//			KeyRow = 2;
//			KeyCol = 4;
//		}
		else if( KeyValue == 0x19 )
		{
			KeyRow = 3;
			KeyCol = 0;
		}
		else if( KeyValue == 0x18 )
		{
			KeyRow = 3;
			KeyCol = 1;
		}
		else if( KeyValue == 0x07 )
		{
			KeyRow = 3;
			KeyCol = 2;
		}
		else if( KeyValue == 0x04 )
		{
			KeyRow = 3;
			KeyCol = 3;
		}
//		else if( KeyValue == 0x )
//		{
//			KeyRow = 3;
//			KeyCol = 4;
//		}
		else if( KeyValue == 0x00 )
		{
			KeyRow = 4;
			KeyCol = 0;
		}
		else if( KeyValue == 0x17 )
		{
			KeyRow = 4;
			KeyCol = 1;
		}
		else if( KeyValue == 0x08 )
		{
			KeyRow = 4;
			KeyCol = 2;
		}
		else if( KeyValue == 0x05 )
		{
			KeyRow = 4;
			KeyCol = 3;
		}
//		else if( KeyValue == 0x )
//		{
//		}
		else
		{
			// Default: DISABLE
			KeyRow = 4;
			KeyCol = 4;
		}

		// Key Pressed, Return Key
		switch( KeyMask )
		{
			case KEYMASK_DEFAULT:
				
				KeyValue = KeyMask_Default[KeyRow][KeyCol];

			break;

			case KEYMASK_FUELLING:
				KeyValue = KeyMask_Fuelling[KeyRow][KeyCol];
			break;

			default:
			
				KeyValue = NOKEY;
			break;
		}

		// Check The Input Key
		KeyPressed();
	}
	else
	{
		// No Key Pressed, Check Other Process Need Input Or Not
		if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & ICCARD_PIN )
		{
			// Clear Bit
			ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= ( ~ICCARD_PIN );

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;

			// Next Display Screen: Input Pin
			NextScreenID = SCR_INPUT_PIN;
		}
		else if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & EMP_PASSWORD )
		{
			// Clear Bit
			ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= ( ~EMP_PASSWORD );

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;

			// Next Display Screen: Input Employee Password
			NextScreenID = SCR_INPUT_EMP_PWD;
		}
		else if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & ICCARD_ENTER_KEY )
		{
			// IC Card Need Press ENTER Key
			// Clear Bit
			ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= ( ~ICCARD_ENTER_KEY );

//			// Clear Input Buffer
//			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
//
//			memset( InputBuf, 0x00, sizeof( InputBuf ));
//			InputLen = 0;
//
//			// Display The Message
//			memcpy( CustomMsg1, DISPMSG[M_ENTER4], strlen( DISPMSG[M_ENTER4] ));

			// Next Display Screen: Press ENTER Key
			NextScreenID = SCR_INPUT_ENTER_KEY;
		}
		
//Douzhq 20140529 Add , Lock Nozzle 
		else if (ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & UNLOCK_PASSWORD)
		{
		  	ProcExchange[PROCESS_KEYPAD][PROCESS_KEYPAD] |= UNLOCK_PASSWORD;
			ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= (~UNLOCK_PASSWORD);
			NextScreenID = SCR_INPUT_UNLOCK_PWD;
			FillInfoMsg( M_UNLOCK_PWD, 0, 0 );
		}
		else if (ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & UNLOCK_PWD_FUEL)
		{
		  	ProcExchange[PROCESS_KEYPAD][PROCESS_KEYPAD] |= UNLOCK_PWD_FUEL;
			ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= (~UNLOCK_PWD_FUEL);
			NextScreenID = SCR_INPUT_UNLOCK_PWD;
			FillInfoMsg( M_UNLOCK_PWD, 0, 0 );
		}

//#ifndef MAKE_CARD_MODE2
        else if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & EMP_NUMBER )
		{
			// Clear Bit
			ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= ( ~EMP_NUMBER );

			// Clear Input Buffer
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			memset( InputBuf, 0x00, sizeof( InputBuf ));
			InputLen = 0;

			// Next Display Screen: Input Emp Number
			NextScreenID = SCR_INPUT_EMP_NUMBER;
		}
//#endif        
		else
		{
		}
	}
}



/*
 ** InitKeypadProcess
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Initialize For KeyPad Process
 *
 *  RETURNS: None.
 *
 */
void InitKeypadProcess( void )
{
	KeyMask = KEYMASK_DEFAULT;
	KeyValue = NOKEY;
}


/*
 ** KeyPressed
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Process The Pressed Key
 *
 *  RETURNS: None.
 *
 */
void KeyPressed( void )
{
	// Check The Pressed Key According To Different Function
	if(( KeyValue != NOKEY ) && ( KeyValue != DISABLE ))
	{
//		// Display Default Info Line
//		ResetDefaultInfo();

		// Key Pressed, Check Last Balance Keep Timer
		if( KeepBalScreen == 1 )
		{
			// Delete The Timer
			DeleteTimer( BalScreenKeepTimer );
			KeepBalScreen = 0;

			// Just Clear The Customer Card Balance Screen
			// Do Not Running Any Function
			KeyValue = NOKEY;

#ifdef MAKE_CARD_MODE2
			
			ResetDefaultInfo();

			// Display: " OK"
			FillInfoMsg( M_DefaultInfo, 0, 0 );
					
#endif	// MAKE_CARD_MODE2

			return;
		}

		
		KeypadTable[ScreenID].pKeypadFunc();
		
		KeyValue = NOKEY;
	}
}


/*
 ** DestroyInput
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: For Any Input Request, Must Be Destroyed While The Default Screen Changed
 *
 *  RETURNS: None.
 *
 *
 *
 */
void DestroyInput( void )
{
	InputLen = 0;
	memset( InputBuf, 0x00, sizeof( InputBuf ));

	// Check The Input Requests
	if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & ICCARD_PIN )
	{
		// Need Input IC Card PIN, Destroy It
		KEYPAD_CR_Buf.ICCardPINLen = 0;
		memset( KEYPAD_CR_Buf.ICCardPIN, 0x00, sizeof( KEYPAD_CR_Buf.ICCardPIN ));
		
		// Inform IC Card Process
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= ICCARD_PIN;
	}
	else if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & EMP_PASSWORD )
	{
		// IC Card Need Employee Password, Destroy It
		KEYPAD_CR_Buf.ICCardEMPPwdLen = 0;
		memset( KEYPAD_CR_Buf.ICCardEMPPwd, 0x00, sizeof( KEYPAD_CR_Buf.ICCardEMPPwd ));
		
		// Inform IC Card Process
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_PASSWORD;
	}
	else if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & ICCARD_ENTER_KEY )
	{
		// Inform IC Card Process
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= ICCARD_ENTER_KEY;
	}
	else if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & PRESET )
	{
		// Keypad Has Preset For IC Card
		KEYPAD_CR_Buf.PresetMode = 0;
		memset( KEYPAD_CR_Buf.PresetValue, 0x00, sizeof( KEYPAD_CR_Buf.PresetValue ));
	
		// Clear Preset Flag
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~PRESET );
	}
//#ifndef MAKE_CARD_MODE2    
    else if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & EMP_NUMBER )
	{
		// Need Input IC Card Emp Number, Destroy It
        KEYPAD_CR_Buf.FuelEmpNumberLen = 0;

		// Inform IC Card Process
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_NUMBER;
	}
//#endif    
}


