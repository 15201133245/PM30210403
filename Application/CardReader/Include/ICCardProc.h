#ifndef _ICCARDPROC_H
#define _ICCARDPROC_H

extern void ICCard_Absence_Proc( void );

extern void ICCard_Absence( void );

#ifdef MAKE_CASH_CR
extern unsigned char CheckCashAmount( void );
extern void GetCashAmount( void );
//Douzhq 20141013 Add , For Bug
//----------------------------------------------------------
extern void GetCashAmountZero( void );
//----------------------------------------------------------
#endif

extern void ICCardAbsence( void );

#ifdef MAKE_CARD_MODE2

extern void ICCardAbsence_Mode2( void );

#endif	// MAKE_CARD_MODE2

#endif