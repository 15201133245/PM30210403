
#ifndef _CRPUMPEOT_H
#define _CRPUMPEOT_H


extern void ICCard_EOT_Proc( void );

extern void SavePumpEOTData( void );

extern void Card_Balance_Update( void );

extern void ClearBalDispFlag( void );

extern void WriteDebitLog( void );


#ifdef MAKE_CARD_MODE2

extern void CheckTransType( void );
extern void CR_OverFuelling_Or_Not( unsigned char type );

#endif	// MAKE_CARD_MODE2





#endif
