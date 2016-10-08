#ifndef _ICCARDINPROC_H
#define _ICCARDINPROC_H


//#define CardMsgDelay			0x02		   // Disp 2 Seconds At Lease
//#define CardMsgDisp				0x01		   // Must Display

#define CASH_TRANS_BALANCE_LIMITED		999900

// For Test Approval Delay
// Define Delay 400ms Before Issue Poll Card Type
#define CardDelayTimeOut 400

extern unsigned char CardDelaySet;
extern unsigned char CardDelayed;

extern void CardDelay( void );

// For Test Approval Delay End

extern void ICCard_Inserted_Proc( void );

extern unsigned short int Card_Validate_Check();
extern unsigned short int PSAM_Check_Validate();
extern unsigned short int CR_Check_Black_List( void );
extern unsigned short int CR_Check_White_List( void );
extern unsigned short int CR_BlackLists_Local( void );
extern unsigned short int CR_BlackLists_POS( void );
extern void SaveICCardData( void );
extern void Save_ICCard_State( void );
extern void FillCardErrorData( void );
extern unsigned short int Card_Data_Check( void );

#ifdef MAKE_CASH_CR
extern void FillCashCardOk( void );

extern unsigned char CheckBalance( void );
#endif

#ifdef MAKE_CARD_MODE2

extern void GetPINData( void );
extern unsigned short int CardAppendOrNot( void );
extern unsigned short int ReadRecordAppend( void );
extern void FillCardErrorData_Mode2( void );
extern void CardLimitInfo( void );


#endif	// MAKE_CARD_MODE2


#endif
