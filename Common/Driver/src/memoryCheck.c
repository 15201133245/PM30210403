#include "global.h"

void SetCrcError(unsigned char val)
{
	At45 *pAt45;
	unsigned int adr;

	pAt45 = &at45[CONFIG_ID];
	adr = CRC_PROTECTED_FLG_ADR;
	adr *= AT45_PageSize(pAt45);
	WRITE(CONFIG_ID,&val,adr,1);
}

unsigned char GetCrcError(void)
{
	At45 *pAt45;
	unsigned int adr;
	unsigned char val;

	pAt45 = &at45[CONFIG_ID];
	adr = CRC_PROTECTED_FLG_ADR;
	adr *= AT45_PageSize(pAt45);
	READ(CONFIG_ID,&val,adr,1);
	return(val);
}

void CalculateCrc(void)
{
	unsigned short crc;
	unsigned int len;

	len = (unsigned int )(&((DataProtected *)0)->crc);
	crc = Crc16((unsigned char *)(&cr_protected),len,0);
	cr_protected.crc = crc;
}

unsigned char CheckProtectedCrc(void)
{
	unsigned short crc;
	unsigned int len;

	len = (unsigned int )(&((DataProtected *)0)->crc);
	crc = Crc16((unsigned char *)(&cr_protected),len,0);
	if(crc == cr_protected.crc)
	{
		return 0;
	}
	return 1;
}