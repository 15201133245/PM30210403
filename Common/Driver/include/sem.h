
#ifndef _SEM_H
#define _SEM_H

#define	BUSY	1
#define IDLE	0

unsigned char P(unsigned char volatile *sem);

void V(unsigned char volatile *sem);

#endif