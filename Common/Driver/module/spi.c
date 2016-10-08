#include "spi.h"

void SPI_Handler(Spid *pSpid)
{
    SpidCmd *pSpidCmd = pSpid->pCurrentCommand;
    AT91S_SPI *pSpiHw = pSpid->pSpiHw;
    volatile unsigned int spiSr;
    // Read the status register
    spiSr = READ_SPI(pSpiHw, SPI_SR);
    if ( spiSr & AT91C_SPI_RXBUFF )
    {
        // Disable transmitter and receiver
        WRITE_SPI(pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
        
        // Disable the SPI clock
        WRITE_PMC(AT91C_BASE_PMC, PMC_PCDR, (1 << pSpid->spiId));
        
        // Disable buffer complete interrupt
        WRITE_SPI(pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);
        
        // Release the dataflash semaphore
        pSpid->semaphore++;
        // Invoke the callback associated with the current command
        if ( pSpidCmd && pSpidCmd->callback )
        {
            pSpidCmd->callback(0, pSpidCmd->pArgument);
        }
    }
}

unsigned char SPID_Configure(Spid *pSpid, AT91S_SPI *pSpiHw, unsigned char spiId)
{
    // Initialize the SPI structure
    pSpid->pSpiHw = pSpiHw;
    pSpid->spiId  = spiId;
    pSpid->semaphore = 1;
    pSpid->pCurrentCommand = 0;
    
    // Enable the SPI clock
    WRITE_PMC(AT91C_BASE_PMC, PMC_PCER, (1 << pSpid->spiId));
    
    // Execute a software reset of the SPI
    WRITE_SPI(pSpiHw, SPI_CR, AT91C_SPI_SWRST);
    
    // Configure SPI in Master Mode with No CS selected !!!
    WRITE_SPI(pSpiHw, SPI_MR, AT91C_SPI_MSTR | AT91C_SPI_MODFDIS | AT91C_SPI_PCS);
    
    // Disable the PDC transfer	
    WRITE_SPI(pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
    
    // Enable the SPI
    WRITE_SPI(pSpiHw, SPI_CR, AT91C_SPI_SPIEN);
    
    // Enable the SPI clock
    WRITE_PMC(AT91C_BASE_PMC, PMC_PCDR, (1 << pSpid->spiId));
    return 0;
}

void SPID_ConfigureCS(Spid *pSpid, unsigned char cs, unsigned int csr)
{
    AT91S_SPI *pSpiHw = pSpid->pSpiHw;
    WRITE_SPI(pSpiHw, SPI_CSR[cs], csr);
}

unsigned char SPID_IsBusy(const Spid *pSpid)
{
    if ( pSpid->semaphore == 0 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char SPID_SendCommand(Spid *pSpid, SpidCmd *pCommand)
{
    AT91S_SPI *pSpiHw = pSpid->pSpiHw;
    unsigned int spiMr;
    
    // Try to get the dataflash semaphore
    if ( pSpid->semaphore == 0 )
    {
        return SPID_ERROR_LOCK;
    }
    pSpid->semaphore--;
    
    // Enable the SPI clock
    WRITE_PMC(AT91C_BASE_PMC, PMC_PCER, (1 << pSpid->spiId));
    
    // Disable transmitter and receiver
    WRITE_SPI(pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
    
    // Write to the MR register
    spiMr = READ_SPI(pSpiHw, SPI_MR);
    spiMr |= AT91C_SPI_PCS;
    spiMr &= ~((1 << pCommand->spiCs) << 16);
    WRITE_SPI(pSpiHw, SPI_MR, spiMr);
    
    // Initialize the two SPI PDC buffer
    WRITE_SPI(pSpiHw, SPI_RPR, (int) pCommand->pCmd);
    WRITE_SPI(pSpiHw, SPI_RCR, pCommand->cmdSize);
    WRITE_SPI(pSpiHw, SPI_TPR, (int) pCommand->pCmd);
    WRITE_SPI(pSpiHw, SPI_TCR, pCommand->cmdSize);
    
    WRITE_SPI(pSpiHw, SPI_RNPR, (int) pCommand->pData);
    WRITE_SPI(pSpiHw, SPI_RNCR, pCommand->dataSize);
    WRITE_SPI(pSpiHw, SPI_TNPR, (int) pCommand->pData);
    WRITE_SPI(pSpiHw, SPI_TNCR, pCommand->dataSize);
    
    // Initialize the callback
    pSpid->pCurrentCommand = pCommand;
    
    // Enable transmitter and receiver
    WRITE_SPI(pSpiHw, SPI_PTCR, AT91C_PDC_RXTEN | AT91C_PDC_TXTEN);
    
    // Enable buffer complete interrupt
    WRITE_SPI(pSpiHw, SPI_IER, AT91C_SPI_RXBUFF);
    return 0;
}
