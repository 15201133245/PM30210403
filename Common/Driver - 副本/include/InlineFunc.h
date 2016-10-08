#ifndef _INLINE_FUNC_H
#define _INLINE_FUNC_H
#include "AT91SAM7SE512.h"
#include "lib_AT91SAM7SE512.h"

#define READ_FROM(peripheral, register)          (peripheral->register)

#define WRITE_TO(peripheral, register, value)  (peripheral->register = value)

#define READ_DATA8(dataAddress)	 \
  	(*((volatile unsigned char *) dataAddress))
#define READ_DATA16(dataAddress) \
    (*((volatile unsigned short *) dataAddress))
#define READ_DATA32(dataAddress) \
    (*((volatile unsigned int *) dataAddress))

#define WRITE_DATA8(dataAddress, data) \
    {(*((volatile unsigned char *) dataAddress)) = (unsigned char) data;}
#define WRITE_DATA16(dataAddress, data) \
    {(*((volatile unsigned short *) dataAddress)) = (unsigned short) data;}
#define WRITE_DATA32(dataAddress, data) \
    {(*((volatile unsigned int *) dataAddress)) = (unsigned int) data;}

typedef union
{
    unsigned int   smc_csr;
    struct
    {
        unsigned int   NWS : 7;            //Number of Wait States
        unsigned int   WSEN : 1;           //Wait State Enable
        unsigned int   TDF : 4;            //Data Float Time
        unsigned int   BAT : 1;            //Byte Access Type
        unsigned int   DBW : 2;            //Data Bus Width
        unsigned int   DRP : 1;            //Data Read Protocol
        unsigned int   ACSS : 2;           //Address to Chip Select Setup
        unsigned int   Reserved0 : 6;
        unsigned int   RWSETUP : 3;        //Read and Write Signal Setup Time
        unsigned int   Reserved1 : 1;
        unsigned int   RWHOLD : 3;         //Read and Write Signal Hold Time
        unsigned int   Reserved2 : 1;
    };
}SMC_CSR,*pSMC_CSR;                         //SMC Chip Select 0~7 Register

inline void PIO_SetInput(AT91S_PIO *pio,
                         unsigned int mask,
                         unsigned char enablePullUp,
                         unsigned char enableFilter)
{
    // Disable interrupts
    WRITE_TO(pio, PIO_IDR, mask);

    // Enable pull-up(s) if necessary
    if (enablePullUp) {
    
        WRITE_TO(pio, PIO_PPUER, mask);
    }
    else {
    
        WRITE_TO(pio, PIO_PPUDR, mask);
    }

    // Enable filter(s) if necessary
    if (enableFilter) {
    
        WRITE_TO(pio, PIO_IFER, mask);
    }
    else {
    
        WRITE_TO(pio, PIO_IFDR, mask);
    }

    // Configure pin as input
    WRITE_TO(pio, PIO_ODR, mask);
    WRITE_TO(pio, PIO_PER, mask);
}

inline void PIO_SetOutput(AT91S_PIO *pio,
                          unsigned int mask,
                          unsigned char defaultValue,
                          unsigned char enableMultiDrive,
                          unsigned char enablePullUp)
{
    // Disable interrupts
    WRITE_TO(pio, PIO_IDR, mask);

    // Enable pull-up(s) if necessary
    if (enablePullUp) {
    
        WRITE_TO(pio, PIO_PPUER, mask);
    }
    else {
    
        WRITE_TO(pio, PIO_PPUDR, mask);
    }

    // Enable multi-drive if necessary
    if (enableMultiDrive) {
    
        WRITE_TO(pio, PIO_MDER, mask);
    }
    else {
    
        WRITE_TO(pio, PIO_MDDR, mask);
    }

    // Set default value
    if (defaultValue) {

        WRITE_TO(pio, PIO_SODR, mask);
    }
    else {

        WRITE_TO(pio, PIO_CODR, mask);
    }

    // Configure pin(s) as output(s)
    WRITE_TO(pio, PIO_OER, mask);
    WRITE_TO(pio, PIO_PER, mask);
}

inline void PIO_SetPeripheralA(AT91S_PIO *pio,
							   unsigned int mask,
							   unsigned char enablePullUp)
{
  	// Disable interrupts on the pin(s)
    WRITE_TO(pio, PIO_IDR, mask);
	
	// Enable the pull-up(s) if necessary
    if (enablePullUp) {

        WRITE_TO(pio, PIO_PPUER, mask);
    }
    else {

        WRITE_TO(pio, PIO_PPUDR, mask);
    }

    // Configure pin
    WRITE_TO(pio, PIO_ASR, mask);
    WRITE_TO(pio, PIO_PDR, mask);
}

inline void PIO_SetPeripheralB(AT91S_PIO *pio,
							   unsigned int mask,
							   unsigned char enablePullUp)
{
    // Disable interrupts on the pin(s)
    WRITE_TO(pio, PIO_IDR, mask);

    // Enable the pull-up(s) if necessary
    if (enablePullUp) {

        WRITE_TO(pio, PIO_PPUER, mask);
    }
    else {

        WRITE_TO(pio, PIO_PPUDR, mask);
    }

    // Configure pin
    WRITE_TO(pio, PIO_BSR, mask);
    WRITE_TO(pio, PIO_PDR, mask);
}

inline void PMC_EnablePeripheral(unsigned int id)
{
  	if((AT91C_BASE_PMC->PMC_PCSR & (1 << id)) == (1 << id))
  	{
		return;
  	}
  	else
  	{
	  	AT91C_BASE_PMC->PMC_PCER = 1 << id;
  	}
}

inline void AIC_ConfigureIT(unsigned int source,
							unsigned int mode,
							void (*handler)(void))
{
  	//Disable the interrupt first
  	AT91C_BASE_AIC->AIC_IDCR = 1 << source;
	
	//Configure mode and handler
	AT91C_BASE_AIC->AIC_SMR[source] = mode;
	AT91C_BASE_AIC->AIC_SVR[source] = (unsigned int)handler;
	
	//Clear interrupt
	AT91C_BASE_AIC->AIC_ICCR = 1 << source;
}

inline void AIC_EnableIT(unsigned int source)
{
    AT91C_BASE_AIC->AIC_IECR = 1 << source;
}

inline void AIC_DisableIT(unsigned int source)
{
  	AT91C_BASE_AIC->AIC_IDCR = 1 << source;
}

inline void AIC_ClearIT(unsigned int source)
{
  	AT91C_BASE_AIC->AIC_ICCR = 1 << source;
}

inline void PIT_Configure(unsigned int piv)
{
  	AT91C_BASE_PITC->PITC_PIMR = AT91C_PITC_PITIEN | piv;
}

inline void PDC_DisableRx(AT91S_PDC *pdc)
{
  	pdc->PDC_PTCR = AT91C_PDC_RXTDIS;
}

inline void PDC_DisableTx(AT91S_PDC *pdc)
{
  	pdc->PDC_PTCR = AT91C_PDC_TXTDIS;
}

inline void SMC_SetCsr(AT91S_SMC2 *smc,
					   unsigned char no,
					   unsigned int nws,
					   unsigned int wsen,
					   unsigned int tdf,
					   unsigned int bat,
					   unsigned int dbw,
					   unsigned int drp,
					   unsigned int acss,
					   unsigned int rw_setup,
					   unsigned int rw_hold)
{
  	SMC_CSR csr;
	csr.NWS = nws;
	csr.WSEN = wsen;
	csr.TDF = tdf;
	csr.BAT = bat;
	csr.DBW = dbw;
	csr.DRP = drp;
	csr.ACSS = acss;
	csr.RWSETUP = rw_setup;
	csr.RWHOLD = rw_hold;
	smc->SMC2_CSR[no] = csr.smc_csr;
}

inline void PIO_Set(const Pin *pin)
{
    WRITE_TO(pin->pio, PIO_SODR, pin->mask);
}

inline void PIO_Clear(const Pin *pin)
{
    WRITE_TO(pin->pio, PIO_CODR, pin->mask);
}

inline void ConfigureWDTC(void)
{
	//AT91F_WDTRestart(AT91C_BASE_WDTC);
	AT91F_WDTSetMode(AT91C_BASE_WDTC,0x2fff7fff);
}

inline void PIT_Start(void)
{
  	unsigned int tmp;
	
	tmp = AT91C_BASE_PITC->PITC_PIMR;
	tmp |= AT91C_PITC_PITEN;
	AT91C_BASE_PITC->PITC_PIMR = tmp;
}

extern	DataProtected cr_protected;

inline unsigned int WriteTTCInc(void)
{
    cr_protected.RecordWriteTTC++;
	CalculateCrc();
    return cr_protected.RecordWriteTTC;
}

inline unsigned int WriteTTCDec(void)
{
    cr_protected.RecordWriteTTC--;
	CalculateCrc();
    return cr_protected.RecordWriteTTC;
}

inline unsigned int GetWriteTTC(void)
{
    return cr_protected.RecordWriteTTC;
}

inline unsigned int GetRecordSpace(void)
{
	unsigned int space;
	
	if(cr_protected.RecordWriteTTC >= cr_protected.RecordReadTTC)
	{
		space = cr_protected.RecordWriteTTC - cr_protected.RecordReadTTC;
		space = RECORD_MAX - space;
	}
	else
	{
		space = 0xffffffff-cr_protected.RecordReadTTC;
		space += (cr_protected.RecordWriteTTC - 1);
		space = RECORD_MAX - space;
	}
	return(space);
}

inline unsigned int ReadTTCInc(void)
{
    cr_protected.RecordReadTTC++;
    if ( cr_protected.RecordReadTTC == 0 )
    {
        cr_protected.RecordReadTTC++;
    }
	CalculateCrc();
    return cr_protected.RecordReadTTC;
}

inline unsigned short GetPriceUpdateFlg(void)
{
    return cr_protected.PriceUpdateFlg;
}


inline unsigned short GetBaseBlackListIntegrality(void)
{
	return cr_protected.BaseBlackListIntegrality;
}

inline unsigned short GetAddBlackListIntegrality(void)
{
	return cr_protected.AddBlackListIntegrality;
}

inline unsigned short GetDelBlackListIntegrality(void)
{
	return cr_protected.DelBlackListIntegrality;
}

inline unsigned short GetWhiteListIntegrality(void)
{
	return cr_protected.WhiteListIntegrality;
}

inline unsigned short GetGradePriceIntegrality(void)
{
	return cr_protected.GradePriceIntegrality;
}

inline unsigned char GetLcdLightOpenTime( void )
{
	return cr_protected.OpenTime;
}

inline unsigned char GetLcdLightCloseTime( void )
{
	return cr_protected.CloseTime;
}
inline void SetLcdLightOpenTime(unsigned char time)
{
	cr_protected.OpenTime = time;
	WriteOpenOrCloseLcdLightTime(0,time);
	CalculateCrc();
}

inline void SetLcdLightCloseTime(unsigned char time)
{
	cr_protected.CloseTime = time;
	WriteOpenOrCloseLcdLightTime(1,time);
	CalculateCrc();
}


#endif