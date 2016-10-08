#include "LockNozzle.h"
#include "define.h"
#include "CRvaribles.h"

//Douzhq 20140528 Add , Lock Nozzle , 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
void LockNozzleFun(void)	//Lock Nozzle
{
	if (TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD && 1)
	{
		LOCK_NOZZLE.LockFlag = 1;
		LOCK_NOZZLE.LockNozzleNumber = Pump_CR_Buf.NozzleNumber;
	}
	else
	{
		LOCK_NOZZLE.LockFlag = 0;
	}
}

void unLockNozzleFun(void)		//unlock Nozzle
{
	if (LOCK_NOZZLE.LockFlag == 1 && TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD)
	{
		
	}
	
	LOCK_NOZZLE.LockFlag = 0;
}
#endif
