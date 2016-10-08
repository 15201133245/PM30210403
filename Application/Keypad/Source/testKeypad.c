
#include "define.h"
#include "global.h"

#include "CRVaribles.h"



unsigned char delayed = 0;
void delaytimer( void )
{
	delayed = 1;
}

unsigned int delayed0 = 0;
unsigned int delayed1 = 0;
unsigned char delayed0sign = 0;
void delaytimer0( void )
{
	delayed0 = 1;
	delayed1++;
}

unsigned int delayed2 = 0;


unsigned keypaddelay = 0;
unsigned keypaddelay0 = 0;
void keypaddelaytimer( void )
{
	keypaddelay = 1;
}

unsigned keypaddelay1 = 0;
void keypaddelaytimer1( void )
{
	keypaddelay1 = 1;
}


void testkeypad( void )
{
//	unsigned int i;

	// Check Need Input PIN Or Not
//	if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & ICCARD_PIN )
//	{
//		// Need Input PIN
//		// Clear The Bit First
//		ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= ( ~ICCARD_PIN );
//	
//		// Start Counter
//	   	CreateTimer( TIMER_ONE_DELETE, 1990, delaytimer );
//	}
//	
//	// Input
//	if( delayed == 1 )
//	{
//		// Clear delay
//		delayed = 0;
//		
//		KEYPAD_CR_Buf.ICCardPINLen = 0x02;
//		KEYPAD_CR_Buf.ICCardPIN[0] = 0x99;
//		KEYPAD_CR_Buf.ICCardPIN[1] = 0x99;
//
//		// Inform IC Card, Input Over
//		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= ICCARD_PIN;
//	}
	// Input PIN Over

//	// Check Need Input Emp Password Or Not
//	if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & EMP_PASSWORD )
//	{
//		// Need Input Emp Password
//		// Clear The Bit First
//		ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= ( ~EMP_PASSWORD );
//	
//		// Start Counter
//	   	CreateTimer( TIMER_ONE_DELETE, 1990, keypaddelaytimer1 );
//	}
//	
//	// Input
//	if( keypaddelay1 == 1 )
//	{
//		// Clear delay
//		keypaddelay1 = 0;
//		
//		KEYPAD_CR_Buf.ICCardPINLen = 0x02;
//		KEYPAD_CR_Buf.ICCardPIN[0] = 0x12;
//		KEYPAD_CR_Buf.ICCardPIN[1] = 0x34;
//
//		// Inform IC Card, Input Over
//		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_PASSWORD;
//	}
//	// Input PIN Over


//	// Spring Out Card
//	if( delayed0sign == 0 )
//	{
//		// Start Counter
//	   	CreateTimer( TIMER_ONE_DELETE, 19000, delaytimer0 );
//		
//		delayed0sign = 1;
//	}
//
//	if( delayed0 == 1 )
//	{
//		if( delayed0sign == 1 )
//		{
//			// Set Spring Card
//			ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SPRING_CARD;
//			delayed0sign = 2;
//		}
//	}
//
//	// Spring Out Card Over

//	// Select Application
//	if( delayed0sign == 0 )
//	{
//		CreateTimer( TIMER_ONE_DELETE, 19900, delaytimer0 );
//		delayed0sign = 1;
//	}
//	
//	if( delayed0 == 1 )
//	{
//		delayed0 = 0;
//
//		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= SELECT_APP;
//
//		if( delayed1 % 2 )
//		{
//			KEYPAD_CR_Buf.APPType = 1;
//		}
//		else
//		{
//			KEYPAD_CR_Buf.APPType = 0;
//		}
//	}
//	// Select Application End

// Preset
	if( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_CALL )
	{
//		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= PRESET;

		KEYPAD_CR_Buf.PresetMode = 0;
		memcpy( KEYPAD_CR_Buf.PresetValue, "\x00\x00\x11\x11", 4 );

		delayed2 = 1;
	}



// Preset End


// Logon Begin
//	if( keypaddelay0 == 0 )
//	{
//		if( ProcStatus[PROCESS_CR] & CARD_READY )
//		{
//			if( !( ProcStatus[PROCESS_CR] & CARD_LOGON ))
//			{
//				if( TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD )
//				{
//					// Start Counter
//				   	CreateTimer( TIMER_ONE_DELETE, 1990, keypaddelaytimer );
//					keypaddelay0 = 1;
//				}
//			}
//		}
//	}
//
//	if( keypaddelay == 1 )
//	{
//		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= EMP_LOG_ON;
//		keypaddelay = 2;
//	}
//
//
// Logon End




}