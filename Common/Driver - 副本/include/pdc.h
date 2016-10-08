
#ifndef _PDC_H
#define _PDC_H
#include "AT91SAM7SE512.h"

//------------------------------------------------------------------------------
/// Set the next receive transfer descriptor
/// \param pdc  pointer to a PDC controller.
/// \param address  address to the next bloc to be received.
/// \param cnt  number of cnt to be received.
//------------------------------------------------------------------------------
inline void PDC_SetNextRx(AT91S_PDC *pdc,
						  char *address,
						  unsigned int cnt)
{
  	pdc->PDC_RNPR = (unsigned int)address;
	pdc->PDC_RNCR = cnt;
}

//------------------------------------------------------------------------------
/// Set the next transmit transfer descriptor
/// \param pdc pointer to PDC controller
/// \param address address to the next bloc to be transmitted
/// \param cnt number of cnt to be transmitted
//------------------------------------------------------------------------------
inline void PDC_SetNextTx(AT91S_PDC *pdc,
						  char *address,
						  unsigned int cnt)
{
  	pdc->PDC_TNPR = (unsigned int)address;
	pdc->PDC_TNCR = cnt;
}

//-------------------------------------------------------------------------------
/// Set the receive transfer descriptor
/// \param pdc pointer to a PDC controller
/// \param address address to the next bloc to be received
/// \param cnt number of cnt to be received
//-------------------------------------------------------------------------------
inline void PDC_SetRx(AT91S_PDC *pdc,
					  char *address,
					  unsigned int cnt)
{
  	pdc->PDC_RPR = (unsigned int)address;
	pdc->PDC_RCR = cnt;
}

//--------------------------------------------------------------------------------
/// Set the transmit transfer descriptor
/// \param pdc pointer to a PDC controller
/// \param address address to the next bloc to be transmitted
/// \param cnt number of cnt to be transmitted
//--------------------------------------------------------------------------------
inline void PDC_SetTx(AT91S_PDC *pdc,
					  char *address,
					  unsigned int cnt)
{
  	pdc->PDC_TPR = (unsigned int)address;
	pdc->PDC_TCR = cnt;
}

//--------------------------------------------------------------------------------
/// \Enable transmit
/// \param pdc pointer to a PDC controller
//--------------------------------------------------------------------------------
inline void PDC_EnableTx(AT91S_PDC *pdc)
{
  	pdc->PDC_PTCR = AT91C_PDC_TXTEN;
}

//--------------------------------------------------------------------------------
/// \Enable receive
/// \param pad pointer to a PDC controller
//--------------------------------------------------------------------------------
inline void PDC_EnableRx(AT91S_PDC *pdc)
{
  	pdc->PDC_PTCR = AT91C_PDC_RXTEN;
}

//--------------------------------------------------------------------------------
/// \Disable transmit
/// \param pdc pointer to a PDC controller
//--------------------------------------------------------------------------------
inline void PDC_DisableTx(AT91S_PDC *pdc)
{
  	pdc->PDC_PTCR = AT91C_PDC_TXTDIS;
}

//--------------------------------------------------------------------------------
/// \Disable receive
/// \param pdc pointer to be a PDC controller
//--------------------------------------------------------------------------------
inline void PDC_DisableRx(AT91S_PDC *pdc)
{
  	pdc->PDC_PTCR = AT91C_PDC_RXTDIS;
}

//--------------------------------------------------------------------------------
/// \Test if the current transfer descriptor has been sent
/// \param pdc pointer  to a PDC controller
//--------------------------------------------------------------------------------
inline int PDC_IsTxEmpty(AT91S_PDC *pdc)
{
  	return !(pdc->PDC_TCR);
}

//--------------------------------------------------------------------------------
/// \Test if the next transfer descriptor has been moved to the current td
/// \param pdc pointer to a PDC controller
//--------------------------------------------------------------------------------
inline int PDC_IsNextTxEmpty(AT91S_PDC *pdc)
{
  	return !(pdc->PDC_TNCR);
}

//--------------------------------------------------------------------------------
/// \Test if the current transfer descriptor has been filled
/// \param pdc pointer to a PDC controller
//--------------------------------------------------------------------------------
inline int PDC_IsRxEmpty(AT91S_PDC *pdc)
{
  	return !(pdc->PDC_RCR);
}

//--------------------------------------------------------------------------------
/// \Test if the next transfer descriptor has been moved to the current td
/// \param pdc pointer to a PDC controller
//--------------------------------------------------------------------------------
inline int PDC_IsNextRxEmpty(AT91S_PDC *pdc)
{
  	return !(pdc->PDC_RNCR);
}

//--------------------------------------------------------------------------------
/// \Open PDC: disable TX and RX reset transfer descriptor,re-enable RX and TX
/// \param pdc pointer to a PDC controller
//--------------------------------------------------------------------------------
inline void PDC_OPEN(AT91S_PDC *pdc)
{
  	//Disable the RX and TX PDC transfer requests
  	PDC_DisableRx(pdc);
	PDC_DisableTx(pdc);
	//Reset all Counter register Next buffer first
	PDC_SetNextTx(pdc, (char *)0, 0);
	PDC_SetNextRx(pdc, (char *)0, 0);
	PDC_SetTx(pdc, (char *)0, 0);
	PDC_SetRx(pdc, (char *)0, 0);
	//Enable the RX and TX PDC transfer requests
	PDC_EnableRx(pdc);
	PDC_EnableTx(pdc);
}

//--------------------------------------------------------------------------------
/// \Close PDC:disable TX and RX reset transfer descriptors
/// \param pdc pointer to a PDC controller
//--------------------------------------------------------------------------------
inline void PDC_Close(AT91S_PDC *pdc)
{
  	//Disable the RX and TX PDC transfer requests
	PDC_DisableRx(pdc);
	PDC_DisableTx(pdc);
	//Reset all counter register Next buffer first
	PDC_SetNextTx(pdc, (char *)0, 0);
	PDC_SetNextRx(pdc, (char *)0, 0);
	PDC_SetTx(pdc, (char *)0, 0);
	PDC_SetRx(pdc, (char *)0, 0);
}

//---------------------------------------------------------------------------------
/// \Set Sent
/// \param pdc pointer to a PDC controller
/// \param pBuffer Send data address
/// \param CntBuffer Number of the data
/// \param pNBuffer Send data Next address
/// \param CntNBuffer Number of the next data
/// \return 1, only one buffer can be initialized;
/// \       2,Buffer and next Buffer can be initialize
/// \       0,All buffer are in use...
//---------------------------------------------------------------------------------
inline unsigned int PDC_SendFrame(AT91S_PDC *pdc,
								  char *pBuffer,
								  unsigned int CntBuffer,
								  char *pNBuffer,
								  unsigned int CntNBuffer)
{
  	if(PDC_IsTxEmpty(pdc))
	{
	  	//buffer and next buffer can be initialized
	  	PDC_SetTx(pdc, pBuffer, CntBuffer);
		PDC_SetNextTx(pdc, pNBuffer, CntNBuffer);
		return 2;
	}
	else if(PDC_IsNextTxEmpty(pdc))
	{
	  	//Only one buffer can be initialized
	  PDC_SetNextTx(pdc, pBuffer, CntBuffer);
	  return 1;
	}
	else
	{
	  	return 0;
	}
}

//---------------------------------------------------------------------------------
/// \Set Received
/// \param pdc pointer to a PDC controller
/// \param pBuffer Received data to be save address
/// \param CntBuffer Number of the data
/// \param pNBuffer Received data to be save next address
/// \param CntNBuffer Number of the next data
/// \return 2,Buffer and next Buffer can be initialized
///         1,Only one buffer can be initialized
///			0,All buffer are in use...
//---------------------------------------------------------------------------------
inline unsigned int PDC_ReceiveFrame(AT91S_PDC *pdc,
									 char *pBuffer,
									 unsigned int CntBuffer,
									 char *pNBuffer,
									 unsigned int CntNBuffer)
{
  	if(PDC_IsRxEmpty(pdc))
	{
	  	PDC_SetRx(pdc, pBuffer, CntBuffer);
		PDC_SetNextRx(pdc, pNBuffer, CntNBuffer);
		return 2;
	}
	else if(PDC_IsNextRxEmpty(pdc))
	{
	  	PDC_SetNextRx(pdc, pBuffer, CntBuffer);
	  	return 1;
	}
	else
	{
	  	return 0;
	}
}

#endif