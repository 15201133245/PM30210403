
#ifndef _PMC_H
#define _PMC_H
#include "AT91SAM7SE512.h"

//------------------------------------------------------------------------------
/// Enables the clock of a peripheral. The peripheral ID (AT91C_ID_xxx) is used
/// to identify which peripheral is targetted.
/// Note that the ID must NOT be shifted (i.e. 1 << AT91C_ID_xxx).
/// \param id  Peripheral ID (AT91C_ID_xxx).
//------------------------------------------------------------------------------
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

#endif