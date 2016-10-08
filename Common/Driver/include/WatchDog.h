#ifndef _WatchDog_h
#define _WatchDog_h
#include "AT91SAM7SE512.h"
#include "lib_AT91SAM7SE512.h"

inline void ConfigureWDTC(void)
{
	//AT91F_WDTRestart(AT91C_BASE_WDTC);
	AT91F_WDTSetMode(AT91C_BASE_WDTC,0x2fff7fff);
}

#endif