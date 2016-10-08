
#ifndef _AIC_H
#define _AIC_H
#include "AT91SAM7SE512.h"

//------------------------------------------------------------------------------
/// Configures the interrupt associated with the given source, using the
/// specified mode and interrupt handler.
/// \param source  Interrupt source to configure.
/// \param mode  Triggering mode of the interrupt.
/// \param handler  Interrupt handler function.
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
/// Enables interrupts coming from the given (unique) source.
/// \param source  Interrupt source to enable.
//------------------------------------------------------------------------------
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

inline void AIC_TriggerIt(unsigned int source)
{
  	AT91C_BASE_AIC->AIC_ISCR = 1 << source;
}
#endif