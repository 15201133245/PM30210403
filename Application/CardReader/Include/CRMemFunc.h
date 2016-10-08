#ifndef _CRMEMFUNC_H
#define _CRMEMFUNC_H


extern void PSAM_Calc_MAC( void );
extern void TrimMACData( void );

#ifdef MAKE_CARD_MODE2

extern void Card_Send_Log( void );
extern void CopyLog( void );

#endif	// MAKE_CARD_MODE2

#endif