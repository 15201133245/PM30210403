#ifndef _CRMEMCOPY_H
#define _CRMEMCOPY_H

#include "message.h"

// PolyNomial For CRC16
//#define		CRC_POLYNOMIAL		0x8408
#define		CRC_POLYNOMIAL		0xA001


extern short int CharToShortIntCopy( unsigned char *pBuffer );
extern int CharToIntCopy( unsigned char *pBuffer );
extern void ShortIntToCharCopy( short int IntVal, unsigned char *pBuffer );
extern void IntToCharCopy( int IntVal, unsigned char *pBuffer );
extern unsigned short int CalcCRC16( unsigned char *pBuffer, unsigned short int len );

extern unsigned char CompareDate( unsigned char *pDate1, unsigned char *pDate2 );
extern unsigned char FindInMem( unsigned char *pBlock, unsigned int BlockLen, unsigned char *pMem, unsigned int MemLen );


extern void GetTransTime( void );
//Douzhq 20141014 Add , For Bug
//---------------Down------------------------------------------------------------
extern void GetTransTimeZero(unsigned char *Trans_Date,unsigned char *Trans_Time);
//--------------- Up ------------------------------------------------------------
extern void GetTransTTC( void );
extern void WriteTTCLog( void );

extern void Hex2Asc( unsigned char *pDest, unsigned char *pSource, unsigned char len );
extern void Asc2Hex( unsigned char *pDest, unsigned char *pSource, unsigned char len );

extern void CRWriteLog( enum DISPMSGID mID );
extern void CRWriteStepLog( void );
extern void CRSpringCardLog( void );

extern void SaveCRConfigFlash( void );
extern void ReadCRConfigFlash( void );

#ifdef MAKE_PSAM_VERIFY

extern unsigned short int  FillPOSMessage( unsigned char Cmd, unsigned char RType, unsigned short int Times );

#endif

#ifdef MAKE_CASH_CR

extern unsigned char CheckCashCard( void );

extern void SaveCashTotal( void );
extern void ClearCashInfo( void );
extern unsigned char CheckCashType( unsigned char *pRecordRFU );

#endif



#endif