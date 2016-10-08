#include "global.h"
#include "cardreader.h"
#include "CRVaribles.h"
#include "CRMemcopy.h"
#include "Pdprotect.h"
#include "Pump.h"
#include "Keypad.h"
#include "KeyMask.h"


#include "CRPrint.h"
#include "PowerDown.h"
#include "Schedule.h"
#include "Display.h"
#include "Voice.h"
#include "CashTrans.h"



#ifdef MAKE_CARD_MODE2

//#include "POS.h"
#include "POSMode2.h"

#else

#include "POS.h"

#endif	// MAKE_CARD_MODE2





// For Test
//#include "testkeypad.h"
//#include "testpos.h"
#include "testpos2.h"
#include "testpump.h"
#include "testcashtrans.h"
// For Test End


// Special For CrProtect Memory Check Test
unsigned char NoRunningFlag = 0xFF;
unsigned char CRFlag = 0x00;

int main()
{

	InitSys();

//      For Test
//	Rs485Adr = 0x00;

	
	LoadConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));

//	cr_protected.OpenTime = 0x03;
//	cr_protected.CloseTime = 0x02;
//	OpenLcdLight();
	LcdBackLightControl();

//	SaveConfig( &PowerDown.PDStartFlag, sizeof( PD_All ));

//	// Format Config
//	FormatDataFlash(0);
//	// Format Record
//	FormatDataFlash(1);

	ReadCRConfigFlash();

//	IdleFlg = 0x03;

	InitM1172( 0 );

#ifdef TEST_PUMP
#else
	InitPumpProcess();
#endif 
    
#ifdef MAKE_CARD_MODE2
	InitPosProcess2();
#else
	InitPosProcess();
#endif	// MAKE_CARD_MODE2

	InitCardReaderProcess();
	
	InitKeypadProcess();
	InitDisplayProcess();

	InitPrintProcess();

	InitVoiceProcess();

#ifdef MAKE_CASH_CR
	InitCashProcess();
#endif

	while( 1 )
	{
	  	CheckTimerGo();
   	        
        SysTimer();

#ifdef TEST_PUMP
#else
	    PumpProcess();
#endif
		
		SpiUsart();

#ifdef MAKE_CARD_MODE2
		Arm70_POS_Mode2_main();
#else
		Arm70_POS_main();
#endif

		ICCardProcess();

		VoiceProcess();

		ProcTimes++;

		KeypadProcess();

		DisplayProcess();

		PrintProcess();

		PwoerDownProcess();

		ScheduleProcess();

#ifdef MAKE_CASH_CR
		CashProcess();
#endif

		if( IcTypeFlg )
		{
			CheckIrq1Int();
		}

//		CheckInterval();

		WatchDog();

#ifdef TEST_PUMP
        testpump();
#endif
	}
}