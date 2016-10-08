#ifndef _CRCOMMAND_H
#define _CRCOMMAND_H

#include "CRVaribles.h"


#define CR_STX 0x02
#define CR_ETX 0x03
#define CR_ENQ 0x05
#define CR_ACK 0x06
#define CR_NAK 0x15
#define CR_EOT 0x04


#define CR_RESPONSE_YES 'Y'
#define CR_RESPONSE_NO 'N'
#define CR_RESPONSE_ERROR 'E'

// default ACK TimeOut Is 200ms
#define CR_WAITINGACKTIMEOUT 200
//#define CR_WAITINGACKTIMEOUT 900

// default Response TimeOut Is 900ms
//#define CR_WAITINGRESPONSEIMEOUT 900
#define CR_WAITINGRESPONSEIMEOUT 2000





extern int SendCommandCardReader( unsigned char *pBuffer, int len );
extern int RecvResponseCardReader( unsigned char *pBuffer, int len );

extern void Uart1_SendCallBack( unsigned char *pBuffer, unsigned int len, unsigned char offset );
extern void Uart1_RecvCallBack( unsigned char *pBuffer, unsigned int len, unsigned char offset );
extern void TWI_RecvCallBack( unsigned char *pBuffer, unsigned int len, unsigned char offset );

extern unsigned char CR_Calc_BCC( unsigned char *pBuffer, int len );

extern void CR_Command( unsigned char *pSendBuffer, int SendLen );
extern void CR_Command_232( unsigned char *pSendBuffer, int SendLen );
extern void CR_Command_TWI( unsigned char *pSendBuffer, int SendLen );
extern void CR_ACKTimeOut( void );
extern void CR_ResponseTimeOut( void );

extern unsigned short int CR_Command_CR( const unsigned char *pCommand, unsigned char CR_CommandLen );

extern unsigned short int CR_CardReaderReset( void );
extern unsigned short int CR_CardReaderCheckCard( void );
extern unsigned short int CR_CardReaderSpringOutCard( void );
extern unsigned short int CR_CardReaderPowerOnCard( void );
extern unsigned short int CR_CardReaderPowerOffCard( void );
extern unsigned short int CR_CardReaderPowerOffPSAM( void );

extern void CardReader_Proc( void );
//extern void Set_Next_Command( unsigned int Step, unsigned Command );
extern void Set_Next_Command( enum CR_PROC_STEP Step, enum CR_COMMAND_TYPE Command );

#endif