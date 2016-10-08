
#ifndef _POWERDOWN_H
#define _POWERDOWN_H


// Power Down Timeout
// Set The Timer Check After PowerDown Is Issued
// And Inform Driver To Save The Datas After PDTimeOut
// Do Not Care About The Other Process's Response
// Default 2000ms
#define PDTimeOut 2000


extern unsigned char Pdtimeoutflag;
extern unsigned char PdRunning;


extern void PwoerDownProcess( void );

extern void PD_TimeOut_CallBack( void );

extern void WritePdLog( unsigned char PdType );

#endif
