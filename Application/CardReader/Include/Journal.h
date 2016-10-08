#ifndef _JOURNAL_H
#define _JOURNAL_H

extern void ICCard_Journal_Proc( void );

extern void Fill_Journal( void );

extern void WriteToPDBuffer( unsigned char *pDes, unsigned char * pSource, unsigned char len );

extern void ReadPDBuffer( void );

extern unsigned char SaveRecord( void );

extern void WriteRecordLog( void );

#endif
