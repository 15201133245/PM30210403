#ifndef	_SPI_H
#define	_SPI_H
#include "AT91SAM7SE512.h"

#define SPID_ERROR_LOCK     2

#define SPID_CSR_SCBR(mck, spck)    ((((mck) / (spck)) << 8) & AT91C_SPI_SCBR)

/// Calculates the value of the DLYBS field of the Chip Select Register given
/// the delay in ns and MCK.
#define SPID_CSR_DLYBS(mck, delay) \
    ((((((delay) * ((mck) / 1000000)) / 1000) + 1)  << 16) & AT91C_SPI_DLYBS)

/// Calculates the value of the DLYBCT field of the Chip Select Register given
/// the delay in ns and MCK.
#define SPID_CSR_DLYBCT(mck, delay) \
    ((((((delay) / 32 * ((mck) / 1000000)) / 1000) + 1) << 24) & AT91C_SPI_DLYBCT)

#define SPI_CSR(mck, spck) \
        (AT91C_SPI_NCPHA | SPID_CSR_DLYBCT(mck, 250) \
         | SPID_CSR_DLYBS(mck, 250) | SPID_CSR_SCBR(mck, spck))

#define WRITE_PMC(pPmc, regName, value) pPmc->regName = (value)
#define WRITE_SPI(pSpi, regName, value) ((pSpi->regName) = (value))
#define READ_SPI(pSpi, regName) (pSpi->regName)

typedef void (*SpidCallback )();

typedef struct _SpidCmd {

    /// Pointer to the command data.
	unsigned char *pCmd;
    /// Command size in bytes.
	unsigned char cmdSize;
    /// Pointer to the data to be sent.
	unsigned char *pData;
    /// Data size in bytes.
	unsigned short dataSize;
    /// SPI chip select.
	unsigned char spiCs;
	SpidCallback callback;
	void *pArgument;

} SpidCmd;

typedef struct {

    /// Pointer to SPI Hardware registers
	AT91S_SPI *pSpiHw;
    /// SPI Id as defined in the product datasheet
	char spiId;
    /// Current SpiCommand being processed
	SpidCmd *pCurrentCommand;
    /// Mutual exclusion semaphore.
	volatile char semaphore;
} Spid;

void SPI_Handler(Spid *pSpid);

unsigned char SPID_Configure(Spid *pSpid, AT91S_SPI *pSpiHw, unsigned char spiId);

void SPID_ConfigureCS(Spid *pSpid, unsigned char cs, unsigned int csr);

unsigned char SPID_IsBusy(const Spid *pSpid);

unsigned char SPID_SendCommand(Spid *pSpid, SpidCmd *pCommand);

#endif