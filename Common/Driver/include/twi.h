#ifndef	_TWI_H
#define	_TWI_H
#include "AT91SAM7SE512.h"

#ifndef AT91C_TWI_TXRDY
    #define AT91C_TWI_TXRDY   AT91C_TWI_TXRDY_MASTER
#endif
#ifndef AT91C_TWI_TXCOMP
    #define AT91C_TWI_TXCOMP  AT91C_TWI_TXCOMP_MASTER
#endif

typedef struct _Async {

    // Asynchronous transfer status.
    volatile unsigned char status;
    // Callback function to invoke when transfer completes or fails.
    void *callback;
    // Driver storage area; do not use.
    unsigned int pStorage[4];

} Async;

typedef void (*TwiCallback)(Async *);

typedef struct _AsyncTwi {

    /// Asynchronous transfer status.
    volatile unsigned char status;
    // Callback function to invoke when transfer completes or fails.
    TwiCallback callback;
    /// Pointer to the data buffer.
    unsigned char *pData;
    /// Total number of bytes to transfer.
    unsigned int num;
    /// Number of already transferred bytes.
    unsigned int transferred;

} AsyncTwi;

typedef struct _Twid {

    /// Pointer to the underlying TWI peripheral.
    AT91S_TWI *pTwi;
	Async *pTransfer;
	unsigned char volatile semaphore; 
} Twid;	

inline void TWI_Configure(AT91S_TWI *pTwi, unsigned int twck, unsigned int mck)
{
  	unsigned int ckdiv = 0;
    unsigned int cldiv;
	unsigned int pow = 1;
    unsigned char ok = 0;

    //trace_LOG(trace_DEBUG, "-D- TWI_Configure()\n\r");
	// Reset the TWI
    pTwi->TWI_CR = AT91C_TWI_SWRST;

    // Set master mode
    pTwi->TWI_CR = AT91C_TWI_MSEN | AT91C_TWI_SVDIS;
//	pTwi->TWI_CR = AT91C_TWI_SVDIS;
	
	// Configure clock
	while(!ok)
	{
	  	cldiv = ((mck / (2 * twck)) - 3) / pow;
		if(cldiv <= 255)
		{
		  	ok = 1;
		}
		else
		{
		  	ckdiv++;
			pow <<= 1;
		}
	}
	//trace_LOG(trace_INFO, "-D- Using CKDIV = %u and CLDIV/CHDIV = %u\n\r", ckdiv, cldiv);
    pTwi->TWI_CWGR = (ckdiv << 16) | (cldiv << 8) | cldiv;
}

inline unsigned char TWID_IsBusy(Twid *pTwid)
{
    if ( pTwid->semaphore == 0 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

inline unsigned char TWI_GetTXRDY(AT91S_TWI *pTwi)
{
    return ((pTwi->TWI_SR & AT91C_TWI_TXRDY) == AT91C_TWI_TXRDY);
}

inline unsigned char TWI_GetRXRDY(AT91S_TWI *pTwi)
{
  	return ((pTwi->TWI_SR & AT91C_TWI_RXRDY) == AT91C_TWI_RXRDY);
}

inline unsigned char TWI_GetNACK(AT91S_TWI *pTwi)
{
  	return ((pTwi->TWI_SR & AT91C_TWI_NACK_MASTER) == AT91C_TWI_NACK_MASTER);
}

inline unsigned char TWI_TransferComplete(AT91S_TWI *pTwi)
{
  	return ((pTwi->TWI_SR & AT91C_TWI_TXCOMP) == AT91C_TWI_TXCOMP);
}

inline void TWI_WriteByte(AT91S_TWI *pTwi,
				   unsigned char byte)
{
  	pTwi->TWI_THR = byte;
}

inline void TWI_StartWrite(AT91S_TWI *pTwi,
					unsigned char daddr,
					unsigned int iaddr,
					unsigned char isize,
					unsigned char byte)
{
    pTwi->TWI_MMR = (isize << 8) | (daddr << 16);
    pTwi->TWI_IADR = iaddr;
    TWI_WriteByte(pTwi,byte);
}

inline void TWI_StartRead(AT91S_TWI *pTwi,
				   unsigned char address,
				   unsigned int iaddress,
				   unsigned char isize)
{
  	// Set slave address and number of internal address bytes
    pTwi->TWI_MMR = (isize << 8) | AT91C_TWI_MREAD | (address << 16);

    // Set internal address bytes
	if(iaddress != 0)
	{
    	pTwi->TWI_IADR = iaddress;
	}

    // Send START condition
    pTwi->TWI_CR = AT91C_TWI_START;
	
}

inline void TWI_Stop(AT91S_TWI *pTwi)
{
  	pTwi->TWI_CR = AT91C_TWI_STOP;
}

inline unsigned char TWI_ReadByte(AT91S_TWI *pTwi)
{
  	return pTwi->TWI_RHR;
}

void TWID_Initialize(Twid *pTwid, AT91S_TWI *pTwi);


#endif