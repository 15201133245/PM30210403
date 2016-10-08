#include "twi.h"

void TWID_Initialize(Twid *pTwid, AT91S_TWI *pTwi)
{
    // Initialize driver
    pTwid->pTwi = pTwi;
    pTwid->pTransfer = 0;
    pTwid->semaphore = 1;
}
