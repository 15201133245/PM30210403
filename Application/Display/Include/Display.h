
#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "Message.h"
#include "Screen.h"


// Used In IdleFlg
#define	DISP_CARDLOGO		0x01
#define	DISP_TIME			0x02


extern unsigned char RoundOn;

extern void DisplayProcess( void );

extern void InitDisplayProcess( void );

extern void DispClearScr( void );
extern void DispClearTransScr( void );
extern void DispClearInfoScr( void );

extern void DrawScr( void );

extern void DrawCardInSCR( void );


#ifdef MAKE_CARD_MODE2
extern void DrawCardInSCR_Mode2( void );
extern void DrawBalanceSCR_Card_Mode2( void );
extern void DrawCardBusySCR_Card_Mode2( void );
extern void DrawCardEOT_Mode2( void );
extern void Fill_Disp_Line1_Mode2( void );
extern void Fill_Disp_Line3_Mode2( unsigned int value );
#endif	// MAKE_CARD_MODE2


extern void DrawBalanceSCR( void );
extern void DrawBalanceSCR_Card( void );

#ifdef MAKE_CASH_CR
extern void DrawBalanceSCR_Cash_Acceptor( void );
extern void DrawBalanceSCR_Cash_Auth( void );
#endif	// MAKE_CASH_CR



extern void DrawErrorSCR( void );
extern void DrawWaitingSCR( void );
extern void DrawPriceUpdateSCR( void );
extern void DrawCardBusySCR( void );

extern void DrawCardBusySCR_Card( void );

#ifdef MAKE_CASH_CR
extern void DrawCardBusySCR_Cash_Auth( void );
extern void DrawCardBusySCR_Cash_Acceptor( void );
#endif	// MAKE_CASH_CR



extern void DrawCardBusy( void );
extern void DrawFunctionSCR( void );
extern void DrawPopUpScr( void );
extern void DrawNormalScr( void );
extern void DispInfoLine( void );
//extern void DispPumpMsg( void );
extern void DispAppType( void );
extern void DispRounding( unsigned char RoundType );

extern void UpdateScreen( void );

extern void DrawOverFuelScreen( void );
extern void DrawOverFuelPasswordScreen(void);

#ifdef MAKE_CASH_CR
extern void CarNumberRecScreen(void);
extern void GradeLimitRecScreen(void);
#endif

#ifdef MAKE_PSAM_VERIFY
extern void DrawPSAMUnSupportSCR( void );
extern void DrawPSAMUnRegisteredSCR( void );
extern void DrawPSAMLockSCR( void );
extern void DrawPSAMInsertRIDSCR( void );
extern void DrawPSAMRegisterOkSCR( void );
#endif

#ifdef MAKE_CASH_CR		
extern void DrawKeypadSelectPrintSCR( void );
#endif

//Douzhq 20140922 Add , Config Printer Set
extern void DrawKeypadChoosePrintSCR( void );

//Douzhq 20140613 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
extern void DrawCardBusySaveOil( void );
#endif

#endif
