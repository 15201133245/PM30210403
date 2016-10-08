#ifndef	_AT24_H
#define	_AT24_H
#include "twi.h"

#define AT24_TIMES		10
#define PAGE_TIMES		5

#define AT24_GetPageNumber(pAt24)	(pAt24->pDesc->pageNumber)
#define AT24_GetPageSize(pAt24)		(pAt24->pDesc->pageSize)
#define AT24_GetNumber(pAt24)		((pAt24->pDesc->pageNumber) * (pAt24->pDesc->pageSize))

typedef	void (*run_time)(void);

typedef struct _At24Desc
{
  	unsigned int	pageNumber;
	unsigned int	pageSize;
	unsigned int	adrNumber;
	const char 		*name;
}At24Desc;

typedef struct _At24
{
  	Twid 	*pTwid;
	unsigned char DeviceAdr;
	const At24Desc *pDesc;
	unsigned char TimeFlg;
	run_time RunTime;
}At24;

int WriteToIIC(At24 *pAt24,unsigned char *pBuffer,
			   unsigned int address,unsigned int len);

int ReadFromIIC(At24 *pAt24,unsigned char *pBuffer,
						  unsigned int address,unsigned int len);

void At24TimeOut(void);

#endif