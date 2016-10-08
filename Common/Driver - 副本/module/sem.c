
#include "sem.h"

unsigned char P(unsigned char volatile *sem)
{
  	if(*sem == 0)
	{
	  	return	BUSY;
	}
	(*sem)--;
	return IDLE;
}

void V(unsigned char volatile *sem)
{
  	if(*sem != 0)
	{
	  	return;
	}
	(*sem)++;
}