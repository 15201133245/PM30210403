
#ifndef _CARDREADER_H
#define _CARDREADER_H




extern void ICCardProcess( void );

extern void InitCardReaderProcess( void );
//extern void CR_TestTime( void );
extern unsigned short int CR_PowerDown( void );
extern unsigned char CR_Check_Err( void );





#endif