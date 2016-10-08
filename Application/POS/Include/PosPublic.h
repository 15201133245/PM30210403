#ifndef _POSPUBLIC_H
#define _POSPUBLIC_H

typedef struct _PcSeg
{
	unsigned char type;
	unsigned char seg;
	unsigned short SegOffset;
	unsigned short SegTotal;
}PcSeg;

void SinopecToResolve(void);

void SetPosOffline(void);

#endif