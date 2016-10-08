#ifndef _ACTCARDPROC_H
#define _ACTCARDPROC_H


#ifdef MAKE_PSAM_VERIFY

#define ACTCardDelayTimeOut 400

extern unsigned char ACTCardDelaySet;
extern unsigned char ACTCardDelayed;

extern void ACTCardDelay( void );




extern void ICCard_ACT_Proc( void );

#endif


#endif
