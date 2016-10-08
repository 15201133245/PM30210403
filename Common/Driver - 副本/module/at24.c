#include "at24.h"
#include "tc.h"
#include <string.h>

#define	AT24TYPE 	3

#define	AT24_OK			0
#define	LENGTH_ERROR	1
#define	TWI_BUSY		2
#define TWI_ERROR		3
#define TWI_FUNERR		4

const At24Desc at24Device[] = {
  {	16,   8, 1, "AT24C01"},
  {	32,   8, 1, "AT24C02"},
  {	32,  16, 1, "AT24C04"},
  {	64,  16, 1, "AT24C08"},
  {128,  16, 1, "AT24C16"},
  {128,  32, 2, "AT24C32"},
  {256,  32, 2, "AT45C64"},
  {256,  64, 2, "AT24C128"},
  {512,  64, 2, "AT24C256"},
  {512, 128, 2, "AT24C512"},
  {512, 256, 2, "AT24C1024"}
};

extern	At24 at24;

unsigned char AT24_IsBusy(At24 *pAt24)
{
    return TWID_IsBusy(pAt24->pTwid);
}

int WriteToIIC(At24 *pAt24,unsigned char *pBuffer,
			   unsigned int address,unsigned int len)
{
	unsigned int page1,page2;
    unsigned int page;
    unsigned int dfAddress;
    unsigned char i,res1,res2;
    unsigned char DeviceAdr;
    Tmr *pTmr;
    unsigned char error;
	int length;
    Twid *pTwid;
	AsyncTwi *pTransfer = (AsyncTwi *) pAt24->pTwid->pTransfer;
	
	pTwid = pAt24->pTwid;
	pAt24->pDesc = &at24Device[AT24TYPE];
	if((address + len) > AT24_GetNumber(pAt24))
	{
		return (-LENGTH_ERROR);
	}
	
	if(pTransfer)
	{
		return	TWI_BUSY;
	}
	if ( pAt24->RunTime == (run_time)0 )
    {
        return (-TWI_FUNERR);
    }
	dfAddress = address;
#if (AT24TYPE < 2)
	DeviceAdr = pAt24->DeviceAdr;
#elif (AT24TYPE < 5)
	DeviceAdr = pAt24->DeviceAdr | (dfAddress >> 8);
#elif (AT24TYPE < 10)
	DeviceAdr = pAt24->DeviceAdr;
#else 
	DeviceAdr = pAt24->DeviceAdr | (dfAddress >> 16);
#endif 	
	page1 = address / AT24_GetPageSize(pAt24);
	page2 = (address + len) / AT24_GetPageSize(pAt24);
	res1 = address % AT24_GetPageSize(pAt24);
	res2 = (address + len) % AT24_GetPageSize(pAt24);
	if ( page1 == page2 )
    {
		length = len;
        error = AT24_IsBusy(pAt24);
		if(error)
		{
			return(-error);
		}
		pTwid->semaphore--;
		TWI_StartWrite(pTwid->pTwi,DeviceAdr,dfAddress,
					   pAt24->pDesc->adrNumber,*pBuffer++);
		while ( len > 0 )
        {
			len--;
			if(len == 0)
			{
				TWI_Stop(pTwid->pTwi);
			}
			while(!TWI_GetTXRDY(pTwid->pTwi))
			{
			}
			TWI_WriteByte(pTwid->pTwi,*pBuffer++);
			//len--;
		}
        while ( !TWI_TransferComplete(pTwid->pTwi) )
        {
        }
		pTwid->semaphore++;
	}
	else
	{
		length = 0;
        if ( res1 != 0 )
        {
			i = pAt24->pDesc->pageSize - res1;
            length += i;
            error = AT24_IsBusy(pAt24);
            if ( error )
            {
                return (-error);
            }
			pTwid->semaphore--;
			TWI_StartWrite(pTwid->pTwi,DeviceAdr,dfAddress,
            			   pAt24->pDesc->adrNumber,*pBuffer++);
            
			while(i > 0)
			{
				i--;
				if(i == 0)
				{
					TWI_Stop(pTwid->pTwi);
				}
                while ( !TWI_GetTXRDY(pTwid->pTwi) )
				{
				}
				TWI_WriteByte(pTwid->pTwi,*pBuffer++);
			}
			while ( !TWI_TransferComplete(pTwid->pTwi) )
            {
            }
			page1++;
			pTwid->semaphore++;
			pAt24->TimeFlg = 0;
            pTmr = CreateTimer1(TIMER_ONE,PAGE_TIMES,pAt24->RunTime);
            while ( !pAt24->TimeFlg )
            {
                CheckTimer1Go();
            }
			DeleteTimer1(pTmr);
		}
		for ( page = page1; page < page2; page++ )
        {
			i = AT24_GetPageSize(pAt24);
            length += i;
            dfAddress = page * i;
#if (AT24TYPE < 2)
			DeviceAdr = pAt24->DeviceAdr;
#elif (AT24TYPE < 5)
			DeviceAdr = pAt24->DeviceAdr | (dfAddress >> 8);
#elif (AT24TYPE < 10)
			DeviceAdr = pAt24->DeviceAdr;
#else 
			DeviceAdr = PAt24->DeviceAdr | (dfAddress >> 16);
#endif 
			error = AT24_IsBusy	(pAt24);
            if ( error )
            {
                return (- error);
            }
			pTwid->semaphore--;
			TWI_StartWrite(pTwid->pTwi,DeviceAdr,dfAddress,
            			   pAt24->pDesc->adrNumber,*pBuffer++);
			while ( i > 0 )
            {
				i--;
				if(i == 0)
				{
					TWI_Stop(pTwid->pTwi);
				}
                while ( !TWI_GetTXRDY(pTwid->pTwi) )
                {
                }
                TWI_WriteByte(pTwid->pTwi,*pBuffer++);
            }
			while ( !TWI_TransferComplete(pTwid->pTwi) )
            {
            }
            pTwid->semaphore++;
            pAt24->TimeFlg = 0;
            pTmr = CreateTimer1(TIMER_ONE,PAGE_TIMES,pAt24->RunTime);
            while ( !pAt24->TimeFlg )
            {
                CheckTimer1Go();
            }
			DeleteTimer1(pTmr);
		}
		if(res2 != 0)
		{
			dfAddress = page2 * AT24_GetPageSize(pAt24);
            i = res2;
            length += i;
            error = AT24_IsBusy(pAt24);
            if ( error )
            {
                return (-error);
            }
            pTwid->semaphore--;
			TWI_StartWrite(pTwid->pTwi,DeviceAdr,dfAddress,
            			   pAt24->pDesc->adrNumber,*pBuffer++);
            //i--;
			while ( i > 0 )
            {
				i--;
				if(i == 0)
				{
					TWI_Stop(pTwid->pTwi);
				}
                while ( !TWI_GetTXRDY(pTwid->pTwi) )
                {
                }
                TWI_WriteByte(pTwid->pTwi,*pBuffer++);
                //i--;
            }
			while ( !TWI_TransferComplete(pTwid->pTwi) )
            {
            }
            pTwid->semaphore++;
            pAt24->TimeFlg = 0;
            pTmr = CreateTimer1(TIMER_ONE,PAGE_TIMES,pAt24->RunTime);
            while ( !pAt24->TimeFlg )
            {
                CheckTimer1Go();
            }
			DeleteTimer1(pTmr);
		}
	}
	return length;
}

int ReadFromIIC(At24 *pAt24,unsigned char *pBuffer,
						  unsigned int address,unsigned int len)
{
	unsigned char DeviceAdr;
	int length;
    unsigned char error;
    Twid *pTwid;
	AsyncTwi *pTransfer = (AsyncTwi *) pAt24->pTwid->pTransfer;
	
	pTwid = pAt24->pTwid;
    pAt24->pDesc = &at24Device[AT24TYPE];
	if ( (address + len) > AT24_GetNumber(pAt24) )
    {
        return (-LENGTH_ERROR);
    }
    if(pTransfer)
	{
	  	return	TWI_BUSY;	
	}
#if (AT24TYPE < 2)
	DeviceAdr = pAt24->DeviceAdr;
#elif (AT24TYPE < 5)
	DeviceAdr = pAt24->DeviceAdr | (address >> 8);
#elif (AT24TYPE < 10)
	DeviceAdr = pAt24->DeviceAdr;
#else 
	DeviceAdr = pAt24->DeviceAdr | (address >> 16);
#endif 
	length = len;
	error = AT24_IsBusy(pAt24);
	if ( error )
	{
		return (-error);
	}
	pTwid->semaphore--;
	TWI_StartRead(pTwid->pTwi,DeviceAdr,address,pAt24->pDesc->adrNumber);
	while(len > 0)
	{
		if ( len == 1 )
        {
            TWI_Stop(pTwid->pTwi);
        }
		while ( !TWI_GetRXRDY(pTwid->pTwi) )
        {
        }
        *pBuffer++ = TWI_ReadByte(pTwid->pTwi);
        len--;
	}
	while ( !TWI_TransferComplete(pTwid->pTwi) )
    {
    }
    pTwid->semaphore++;
	return(length);
}

void At24TimeOut(void)
{
	 at24.TimeFlg = 1;
}