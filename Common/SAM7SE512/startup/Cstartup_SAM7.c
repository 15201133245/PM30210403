//-----------------------------------------------------------------------------
//         ATMEL Microcontroller Software Support  -  ROUSSET  -
//-----------------------------------------------------------------------------
// DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
// DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
// OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// File Name           : Cstartup_SAM7.c
// Object              : Low level initialisations written in C for Tools
//                       For AT91SAM7SE512 with 2 flash plane
// Creation            : JPP  09-May-2006
//-----------------------------------------------------------------------------

#include "project.h"


//  The following functions must be write in ARM mode this function called
// directly by exception vector
extern void AT91F_Spurious_handler(void);
extern void AT91F_Default_IRQ_handler(void);
extern void AT91F_Default_FIQ_handler(void);

//*----------------------------------------------------------------------------
//* \fn    AT91F_LowLevelInit
//* \brief This function performs very low level HW initialization
//*        this function can use a Stack, depending the compilation
//*        optimization mode
//*----------------------------------------------------------------------------
void AT91F_LowLevelInit(void)
{
    unsigned char i;
    ///////////////////////////////////////////////////////////////////////////
    // EFC Init
    ///////////////////////////////////////////////////////////////////////////
    AT91C_BASE_MC->MC0_FMR = AT91C_MC_FWS_2FWS;// 2 Wait State to work at 48MHz
    AT91C_BASE_MC->MC1_FMR = AT91C_MC_FWS_2FWS;// 2 Wait State to work at 48MHz

    ///////////////////////////////////////////////////////////////////////////
    // Init PMC Step 1. Enable Main Oscillator
    // Main Oscillator startup time is board specific:
    // Main Oscillator Startup Time worst case (3MHz) corresponds to 15ms
    // (0x40 for AT91C_CKGR_OSCOUNT field)
    ///////////////////////////////////////////////////////////////////////////
    AT91C_BASE_PMC->PMC_MOR = (( AT91C_CKGR_OSCOUNT & (0x40 <<8) | AT91C_CKGR_MOSCEN ));
    // Wait Main Oscillator stabilization
    while(!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS));

    ///////////////////////////////////////////////////////////////////////////
    // Init PMC Step 2.
    // Set PLL to 96MHz (96,109MHz) and UDP Clock to 48MHz
    // PLL Startup time depends on PLL RC filter: worst case is choosen
    // UDP Clock (48,058MHz) is compliant with the Universal Serial Bus
    // Specification (+/- 0.25% for full speed)
    ///////////////////////////////////////////////////////////////////////////
    /*AT91C_BASE_PMC->PMC_PLLR = AT91C_CKGR_USBDIV_1           |
                               AT91C_CKGR_OUT_0              |
                               (16 << 8)                     |
                               (AT91C_CKGR_MUL & (72 << 16)) |
                               (AT91C_CKGR_DIV & 14);*/
	AT91C_BASE_PMC->PMC_PLLR = AT91C_CKGR_USBDIV_1           |
                               AT91C_CKGR_OUT_0              |
                               (16 << 8)                     |
                               (AT91C_CKGR_MUL & (374 << 16)) |
                               (AT91C_CKGR_DIV & 72);
	/*AT91C_BASE_PMC->PMC_PLLR = AT91C_CKGR_USBDIV_1           |
                               AT91C_CKGR_OUT_0              |
                               (16 << 8)                     |
                               (AT91C_CKGR_MUL & (124 << 16)) |
                               (AT91C_CKGR_DIV & 24);*/
    // Wait for PLL stabilization
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK) );
    // Wait until the master clock is established for the case we already
    // turn on the PLL
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) );

    ///////////////////////////////////////////////////////////////////////////
    // Init PMC Step 3.
    // Selection of Master Clock MCK equal to (Processor Clock PCK) PLL/2=48MHz
    // The PMC_MCKR register must not be programmed in a single write operation
    // (see. Product Errata Sheet)
    ///////////////////////////////////////////////////////////////////////////
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2;
    // Wait until the master clock is established
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) );

    AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK;
    // Wait until the master clock is established
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) );

    ///////////////////////////////////////////////////////////////////////////
    //  Disable Watchdog (write once register)
    ///////////////////////////////////////////////////////////////////////////
//    AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;

    ///////////////////////////////////////////////////////////////////////////
    //  Enable Watchdog (write once register) WatchDog Guard Timer: 2 Second
    ///////////////////////////////////////////////////////////////////////////
    AT91C_BASE_WDTC->WDTC_WDMR = ( AT91C_WDTC_WDDBGHLT |
									AT91C_WDTC_WDIDLEHLT |
									AT91C_WDTC_WDFIEN |
									AT91C_WDTC_WDD		|
									AT91C_WDTC_WDRSTEN	|
								 	( AT91C_WDTC_WDV & 0x200 ));

    ///////////////////////////////////////////////////////////////////////////
    //  Init AIC: assign corresponding handler for each interrupt source
    ///////////////////////////////////////////////////////////////////////////
    AT91C_BASE_AIC->AIC_SVR[0] = (int) AT91F_Default_FIQ_handler ;
    for (i = 1; i < 31; i++) {
        AT91C_BASE_AIC->AIC_SVR[i] = (int) AT91F_Default_IRQ_handler ;
    }
    AT91C_BASE_AIC->AIC_SPU = (unsigned int) AT91F_Spurious_handler;
}
