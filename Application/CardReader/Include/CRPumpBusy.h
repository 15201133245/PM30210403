
#ifndef _CRPUMPBUSY_H
#define _CRPUMPBUSY_H


// Check Card Retry Times While Card Busy And Check Card Failure
#define CHECK_CARD_RETRY_TIMES 4

extern void ICCard_Fuelling_Proc( void );

extern void CR_GetLastTrans( void );
extern void Disp_Totals_Update( void );
extern void GetFuellingInfo( void );

extern unsigned short int GetZeroLastTran( void );
extern void WriteCRFuellingErrorLog( unsigned char type );
extern void WriteCRPumpEOTLog( void );
extern void WriteCRPumpLastTransLog( void );
extern void WritePumpLastTransLog( void );
extern void WriteCRCertiMAC2Log( unsigned char type );

extern void CR_OfflineTrans_Or_Not( void );

#ifdef MAKE_CARD_MODE2
extern unsigned short int FuellingPOSOnline( void );
extern void FillFuellingReqMAC( void );
extern void GetFuellingRespOnline( void );

#endif	// MAKE_CARD_MODE2


#endif
