
#ifndef _PIO_H
#define _PIO_H
#include "AT91SAM7SE512.h"

//------------------------------------------------------------------------------
//         Internal definitions
//------------------------------------------------------------------------------
/// \internal Returns the current value of a register.
#define READ_FROM(peripheral, register)          (peripheral->register)
/// \internal Modifies the current value of a register.
#define WRITE_TO(peripheral, register, value)  (peripheral->register = value)

//------------------------------------------------------------------------------
/// Configures one or more pin(s) of a PIO controller as being controlled by
/// peripheral A. Optionally, the corresponding internal pull-up(s) can be
/// enabled.
/// \param pio  Pointer to a PIO controller.
/// \param mask  Bitmask of one or more pin(s) to configure.
/// \param enablePullUp  Indicates if the pin(s) internal pull-up shall be
///                      configured.
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
/// Configures one or more pin(s) of a PIO controller as being controlled by
/// peripheral A. Optionally, the corresponding internal pull-up(s) can be
/// enabled.
/// \param pio  Pointer to a PIO controller.
/// \param mask  Bitmask of one or more pin(s) to configure.
/// \param enablePullUp  Indicates if the pin(s) internal pull-up shall be
///                      configured.
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
/// Configures one or more pin(s) or a PIO controller as inputs. Optionally,
/// the corresponding internal pull-up(s) and glitch filter(s) can be
/// enabled.
/// \param pio  Pointer to a PIO controller.
/// \param mask  Bitmask indicating which pin(s) to configure as input(s).
/// \param enablePullUp  Indicates if the internal pull-up(s) must be enabled.
/// \param enableFilter  Indicates if the glitch filter(s) must be enabled.
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
/// Configures one or more pin(s) of a PIO controller as outputs, with the
/// given default value. Optionally, the multi-drive feature can be enabled
/// on the pin(s).
/// \param pio  Pointer to a PIO controller.
/// \param mask  Bitmask indicating which pin(s) to configure.
/// \param defaultValue  Default level on the pin(s).
/// \param enableMultiDrive  Indicates if the pin(s) shall be configured as
///                          open-drain.
/// \param enablePullUp  Indicates if the pin shall have its pull-up activated.
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
/// Sets a high output level on one or more pin(s) (if configured as output(s)).
/// \param pin  Pointer to a Pin instance describing one or more pins.
//------------------------------------------------------------------------------
inline void PIO_Set(const Pin *pin)
{
    WRITE_TO(pin->pio, PIO_SODR, pin->mask);
}

//------------------------------------------------------------------------------
/// Sets a low output level on one or more pin(s) (if configured as output(s)).
/// \param pin  Pointer to a Pin instance describing one or more pins.
//------------------------------------------------------------------------------
inline void PIO_Clear(const Pin *pin)
{
    WRITE_TO(pin->pio, PIO_CODR, pin->mask);
}

#endif