
#ifndef _CRPUMPCALL_H
#define _CRPUMPCALL_H


#ifdef MAKE_CARD_MODE2

// For Sinopec Mode2, The Record Trans Amount For Every Fuelling
#define EMP_AMN_PER_TRANS 0

#endif



extern void ICCard_Pump_Call_Proc( void );

extern unsigned short int Check_Card_Grade_Brand_Limit( void );
extern unsigned short int Check_Card_Location_Limit( void );
extern unsigned short int Check_Card_Fuelling_Limit( void );
extern unsigned char TimesAmountTotalLimit( void );
extern void CalcFuelling( void );
extern unsigned char CheckAmountTotalLimit( void );
extern unsigned char SetPresetValue( void );
extern unsigned int Round_Mod( unsigned int Value, unsigned int Mod );

extern void SavePumpCallData( unsigned char flag );

extern void FillAuthError( void );
extern void GetPumpCallInfo( void );
extern void WriteCRPumpCallLog( void );
extern void WriteCRAuthPumpLog( void );


#ifdef MAKE_PSAM_VERIFY
extern unsigned short int GetPumpVerify( void );
#endif


extern void CardAuthPump( void );

extern void DispLocationLimit( unsigned char flag );



#ifdef MAKE_CARD_MODE2

extern unsigned short int AuthPOSOnline_EMP( void );
extern unsigned short int AuthPOSOnline_CST( void );
extern void AuthPOSOnlineResponse( void );
extern void FillAuthReqMAC_EMP( void );
extern void FillAuthReqMAC_CST( void );
extern unsigned short int CheckGradeLimitExtend_Mode2( void );
extern void UpdatePresetValue( void );
extern unsigned short int CheckOnlinePreset( void );
extern void AuthErrorTrans( void );


#endif	// MAKE_CARD_MODE2

#endif
