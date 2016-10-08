
#ifndef _PIT_H
#define _PIT_H

inline void PIT_Configure(unsigned int piv)
{
  	AT91C_BASE_PITC->PITC_PIMR = AT91C_PITC_PITIEN | piv;
}

inline void PIT_Start(void)
{
  	unsigned int tmp;
	
	tmp = AT91C_BASE_PITC->PITC_PIMR;
	tmp |= AT91C_PITC_PITEN;
	AT91C_BASE_PITC->PITC_PIMR = tmp;
}
#endif