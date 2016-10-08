

#ifndef _SMC_H
#define _SMC_H
#include "arm_bits.h"
#include "AT91SAM7SE512.h"

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

#endif