#ifndef _RIDCARDPROC_H
#define _RIDCARDPROC_H


#ifdef MAKE_PSAM_VERIFY

#define RIDCardDelayTimeOut 400

extern unsigned char RIDCardDelaySet;
extern unsigned char RIDCardDelayed;

extern void RIDCardDelay( void );


extern void ICCard_RID_Proc( void );

extern unsigned short int Check_RID_Log_Para( void );
extern void FillRIDLogMACBuffer( void );



#endif



#endif