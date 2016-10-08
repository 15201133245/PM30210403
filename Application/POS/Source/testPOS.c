#ifdef TEST_POS
#include "testPOS.h"
#include "define.h"
#include "global.h"



unsigned char POSdelayed = 0;
void POSdelaytimer( void )
{
	POSdelayed = 1;
}

unsigned char POSdelayed1 = 0;
void POSdelaytimer1( void )
{
	POSdelayed1 = 1;
}

unsigned char POSdelayed2 = 0;
void POSdelaytimer2( void )
{
	POSdelayed2 = 1;
}





void testpos( void )
{
//	unsigned int i;

	// Check Need POS Check Black List Or Not
	if( ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_BLACK )
	{
		// Need Check Black List
		// Clear The Bit First
		ProcExchange[PROCESS_CR][PROCESS_POS] &= ( ~POS_CHECK_BLACK );
	
		// Start Counter
	   	CreateTimer( TIMER_ONE_DELETE, 1990, POSdelaytimer );
	}

	if( POSdelayed == 1 )
	{
		// Clear delay
		POSdelayed = 0;
		
//		BlackFit = 0;			// 0: Fitted; 1: Unfitted; 2: Error
		BlackFit = 1;			// 0: Fitted; 1: Unfitted; 2: Error
//		BlackFit = 2;			// 0: Fitted; 1: Unfitted; 2: Error


		// Inform IC Card, Check End
		ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_BLACK;
	}

	// Check Need POS Check White List Or Not
	if( ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_WHITE )
	{
		// Need Check Black List
		// Clear The Bit First
		ProcExchange[PROCESS_CR][PROCESS_POS] &= ( ~POS_CHECK_WHITE );
	
		// Start Counter
	   	CreateTimer( TIMER_ONE_DELETE, 1990, POSdelaytimer1 );
	}

	if( POSdelayed1 == 1 )
	{
		// Clear delay
		POSdelayed1 = 0;
		
		WhiteFit = 0;			// 0: Fitted; 1: Unfitted; 2: Error
//		WhiteFit = 1;			// 0: Fitted; 1: Unfitted; 2: Error
//		WhiteFit = 2;			// 0: Fitted; 1: Unfitted; 2: Error


		// Inform IC Card, Check End
		ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_WHITE;
	}

	// Check Need POS Check Grey Or Not
	if( ProcExchange[PROCESS_CR][PROCESS_POS] & POS_CHECK_GREY )
	{
		// Need Check Black List
		// Clear The Bit First
		ProcExchange[PROCESS_CR][PROCESS_POS] &= ( ~POS_CHECK_GREY );
	
		// Start Counter
	   	CreateTimer( TIMER_ONE_DELETE, 1990, POSdelaytimer2 );
	}

	if( POSdelayed2 == 1 )
	{
		// Clear delay
		POSdelayed2 = 0;
		
		GreyFit = 0;			// 0: Fitted; 1: Unfitted; 2: Error
//		GreyFit = 1;			// 0: Fitted; 1: Unfitted; 2: Error
//		GreyFit = 2;			// 0: Fitted; 1: Unfitted; 2: Error


		// Inform IC Card, Check End
		ProcExchange[PROCESS_POS][PROCESS_CR] |= POS_CHECK_GREY;
	}

}

#endif
