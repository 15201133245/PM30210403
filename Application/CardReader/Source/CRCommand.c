

#include "global.h"

#include "define.h"
#include "CRCommand.h"
#include "CRVaribles.h"
#include "CRMemCopy.h"
#include "ICCardCommand.h"
#include "ICCardInProc.h"
#include "PDProtect.h"
#include "Message.h"
#include "WriteLog.h"


unsigned char flag = 0;

unsigned char CanCheckCard = 1;
void CanCheckCardDelay( void )
{
	CanCheckCard = 1;
}

unsigned char CardCanSpring = 1;
void CardCanSpringDelay( void )
{
	CardCanSpring = 1;
}

/*
 ** SendCommandCardReader
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CardReader.c
 *
 *  PARAMETERS:	*pBuffer: Send Buffer Start Address Pointer; len: Length of the Bytes to Be Sent
 *
 *  DESCRIPTION: Send Command To Card Reader
 *
 *  RETURNS: < 0: Error In Sending Process; > 0: Sent Bytes
 *
 */
int SendCommandCardReader( unsigned char *pBuffer, int len )
{
    int retval;

    Usart_InitSendBack( &Serial1, Uart1_SendCallBack );

    retval = WriteToUsart( &Serial1, pBuffer, len );
	
    return retval;
}

/*
 ** RecvResponseCardReader
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CardReader.c
 *
 *  PARAMETERS:	*pBuffer: Receive Buffer Start Address Pointer; len: Length of the Bytes to Be Received
 *
 *  DESCRIPTION:
 *
 *  RETURNS:
 *
 */
int RecvResponseCardReader( unsigned char *pBuffer, int len )
{
    int retval = 0;

    Serial1.ReceiveLen = len;
    Serial1.pReceiver = pBuffer;
    
    Usart_InitReceviceBack( &Serial1, Uart1_RecvCallBack );

    WriteToUsart( &Serial1, 0, 0 );

    return retval;
}





void Uart1_SendCallBack( unsigned char *pBuffer, unsigned int len, unsigned char offset )
{
	SentFlag = 1;
}


void Uart1_RecvCallBack( unsigned char *pBuffer, unsigned int len, unsigned char offset )
{
//	unsigned char tmplength[4];
	int length;
	unsigned short tmp;

	if( CR_CommandStatus == CR_CommandWaitingACK )
	{
		if( RecvBuffer[0] == CR_ACK )
		{
			// Delete The ACK Timer
			DeleteTimer( CR_Command_ACK_TimeOut );
		}
	}
	// Check Receive Only When  CR_CommandStatus == CR_CommandWaitingResponse 
    else if( CR_CommandStatus == CR_CommandWaitingResponse )
	{
		// The First 3 Bytes Was Received, And Received The Left Response
		if( ReceivedFlag == 0 )
		{
			// Get The Response Length
//			tmplength[0] = 0;
//			tmplength[1] = 0;
//			tmplength[2] = RecvBuffer[1];
// 			tmplength[3] = RecvBuffer[2];
//			length = CharToIntCopy( tmplength );

			memcpy( &tmp, &RecvBuffer[1], 2 );
			length = ShortEndianConvert( tmp );

			// length: Append The EOT And BCC Bytes
			length += 2;

			// Receive The Left Datas
			RecvResponseCardReader( &RecvBuffer[3], length );
		    RecvBufferLen = length + 3;
		
			//
			ReceivedFlag = 1;
		}
		else
		{
			// Delete The ACK Timer
			DeleteTimer( CR_Command_Response_TimeOut );

		    ReceivedFlag = 2;
		}
	}
	else
	{
	}
}

void TWI_RecvCallBack( unsigned char *pBuffer, unsigned int len, unsigned char offset )
{
	memcpy( RecvBuffer, pBuffer, len );
	RecvBufferLen = len;
	
	if( CR_CommandStatus == CR_CommandWaitingACK )
	{
		if( RecvBuffer[0] == CR_ACK )
		{
			// Delete The ACK Timer
			DeleteTimer( CR_Command_ACK_TimeOut );
		}
	}
	// Check Receive Only When  CR_CommandStatus == CR_CommandWaitingResponse 
    else if( CR_CommandStatus == CR_CommandWaitingResponse )
	{
		if( len > 1 )
		{
			// Delete The ACK Timer
			DeleteTimer( CR_Command_Response_TimeOut );

		    ReceivedFlag = 2;
		}
	}
	else
	{
	}
}

/*
 ** CR_ACKTimeOut
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Used To Mark The Waiting ACK Time Out
 *
 *  RETURNS: None.
 *
 *	Notes: Call By The CreatTimer CallBack, And Excute Automacticly
 *
 */
void CR_ACKTimeOut( void )
{
    CR_CommandStatus = CR_CommandACKTimeOut;
}

/*
 ** CR_ResponseTimeOut
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Used To Mark The Waiting Response Time Out
 *
 *  RETURNS: None.
 *
 *	Notes: Call By The CreatTimer CallBack, And Excute Automacticly
 *
 */
void CR_ResponseTimeOut( void )
{
    CR_CommandStatus = CR_CommandResponseTimeOut;
}



/*
 ** CR_Calc_BCC
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	*pBuffer: Start Pointer Of The Calculation; len: Length Of Fhe Calculation
 *
 *  DESCRIPTION: Calculate BCC(XOR Result) For Card Reader Command
 *
 *  RETURNS: BCC Result
 *
 */
unsigned char CR_Calc_BCC( unsigned char *pBuffer, int len )
{
	unsigned char retval;
	int i;

	// intialize the retval 
	retval = 0;

	for( i = 0; i < len; i++ )
	{
		retval ^= *( pBuffer + i );
	}

	return retval;
}





/*
 ** CR_Command
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	pSendBuffer: The Start Pointer Of The Command Send Buffer
 *				pSendLen: The Length Of The Send Data
 *
 *  DESCRIPTION: Communication With Card Reader	Via RS232 Or TWI
 *
 *  RETURNS: CR_CommandStatus
 *
 *	Notes: 
 *
 *	a.		This Function Diverse The Different IC Card Routine,
 *			For Intelligence Card Reader, Call CR_Command_232()
 *			For TWI Reader, Call CR_Command_Twi()
 *
 *	b.		The Data Should Be Saved In Send Buffer, And The CR_CommandStatus
 *			Be the CR_CommandSending In Order To Send The Command
 *			The Other Enums For The CR_CommandStatus Means The Different
 *			Process Steps
 */
void CR_Command( unsigned char *pSendBuffer, int SendLen )
{
	if( IcTypeFlg == 0 )
	{
        // For Intelligence Card Reader
		CR_Command_232( pSendBuffer, SendLen );
	}
	else
	{
        // For Non-Intelligence Card Reader
		CR_Command_TWI( pSendBuffer, SendLen );
	}
}


/*
 ** CR_Command_TWI
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	pSendBuffer: The Start Pointer Of The Command Send Buffer
 *				pSendLen: The Length Of The Send Data
 *
 *  DESCRIPTION: Communication With Card Reader	Via TWI
 *
 *  RETURNS: CR_CommandStatus
 *
 *	Notes: The Data Should Be Saved In Send Buffer, And The CR_CommandStatus
 *			Be the CR_CommandSending In Order To Send The Command
 *			The Other Enums For The CR_CommandStatus Means The Different
 *			Process Steps
 */
void CR_Command_TWI( unsigned char *pSendBuffer, int SendLen )
{
	// First Entry, Send The Command
	if( CR_CommandStatus == CR_CommandSending )
	{
		// Clear Receive Buffer
		memset( RecvBuffer, 0x00, sizeof( RecvBuffer ));
  		RecvBufferLen = 0;
  		ReceivedFlag = 0;

//		// Waiting For ACK, First
//		RecvResponseCardReader( &RecvBuffer[RecvBufferLen], 1 );
//			
//		// Send The Command
//		SendCommandCardReader( pSendBuffer, SendLen );

		// Start TimeOut Check
	   	CR_Command_ACK_TimeOut = CreateTimer( TIMER_ONE_DELETE, CR_WAITINGACKTIMEOUT, CR_ACKTimeOut );

		// Mark The Command Is Sending, And Wait For ACK
		CR_CommandStatus = CR_CommandWaitingACK;
		
		// Send Command Via TWI
		WriteToCardSam( pSendBuffer, SendLen, TWI_RecvCallBack );
	}
	// Command Sent And Waiting For ACK
	else if( CR_CommandStatus == CR_CommandWaitingACK )
	{
		// ACK?
		if( RecvBuffer[0] == CR_ACK )
		{
			// Clear Receive Buffer
			memset( RecvBuffer, 0x00, sizeof( RecvBuffer ));
			RecvBufferLen = 0;
			ReceivedFlag = 0;
			
//			// Begin Receiving
//			// First Receiving: 3 Bytes STX + Length
//			// The Next Response Data Will Be Received In Uart1_RecvCallBack
//			RecvResponseCardReader( &RecvBuffer[RecvBufferLen], 3 );
//			
//			// Begin To Send ENQ
//			// Waiting Certain Time
//			for( i = 0; i < 1000; i++ )
//			{
//				i = i;
//			}

			// Send ENQ
			SendBuffer[0] = CR_ENQ;
			SendLen = 1;

//			// Send The Command
//			SendCommandCardReader( SendBuffer, 1 );

			// Start Time Out
			// Start TimeOut Check
		   	CR_Command_Response_TimeOut = CreateTimer( TIMER_ONE_DELETE, CR_WAITINGRESPONSEIMEOUT, CR_ResponseTimeOut );
			
			// Waiting The Response
			CR_CommandStatus = CR_CommandWaitingResponse;

			// Send Command Via TWI
			WriteToCardSam( pSendBuffer, SendLen, TWI_RecvCallBack );
		}
	}
	// Command Sent And Waiting For Response
	else if( CR_CommandStatus == CR_CommandWaitingResponse )
	{
		// Response Received, Check BCC
		if( ReceivedFlag == 2 )
		{
			if( CR_Calc_BCC( RecvBuffer, RecvBufferLen ) == 0 )
			{
				// Command Response Received
				// Saved In RecvBuffer
				// RecvBufferLen = length
				CR_CommandStatus = CR_CommandResponseReceived;
			}
			else
			{
				// Command Response Error,
				// Mark As Response Check BCC Error
				CR_CommandStatus = CR_CommandResponseError;
			}
		}
	}
}




/*
 ** CR_Command_232
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	pSendBuffer: The Start Pointer Of The Command Send Buffer
 *				pSendLen: The Length Of The Send Data
 *
 *  DESCRIPTION: Communication With Card Reader	Via RS232
 *
 *  RETURNS: CR_CommandStatus
 *
 *	Notes: The Data Should Be Saved In Send Buffer, And The CR_CommandStatus
 *			Be the CR_CommandSending In Order To Send The Command
 *			The Other Enums For The CR_CommandStatus Means The Different
 *			Process Steps
 */
void CR_Command_232( unsigned char *pSendBuffer, int SendLen )
{
//	unsigned char TmpBCC;

	int i;

	// First Entry, Send The Command
	if( CR_CommandStatus == CR_CommandSending )
	{
		// Clear Receive Buffer
		memset( RecvBuffer, 0x00, sizeof( RecvBuffer ));
  		RecvBufferLen = 0;
  		ReceivedFlag = 0;

		// Waiting For ACK, First
		RecvResponseCardReader( &RecvBuffer[RecvBufferLen], 1 );
			
		// Send The Command
		SendCommandCardReader( pSendBuffer, SendLen );
		
		// Start TimeOut Check
//	   	CR_Command_ACK_TimeOut = CreateTimer( TIMER_ONE_DELETE, TIMER_10MS * CR_WAITINGACKTIMEOUT, CR_ACKTimeOut );
	   	CR_Command_ACK_TimeOut = CreateTimer( TIMER_ONE_DELETE, CR_WAITINGACKTIMEOUT, CR_ACKTimeOut );

		// Mark The Command Is Sending, And Wait For ACK
		CR_CommandStatus = CR_CommandWaitingACK;
	}
	// Command Sent And Waiting For ACK
	else if( CR_CommandStatus == CR_CommandWaitingACK )
	{
		// ACK?
		if( RecvBuffer[0] == CR_ACK )
		{
                  
            //20120603
            CRCardCommErrTimes = 0;
                        
			// Clear Receive Buffer
			memset( RecvBuffer, 0x00, sizeof( RecvBuffer ));
			RecvBufferLen = 0;
			ReceivedFlag = 0;
			
			// Begin Receiving
			// First Receiving: 3 Bytes STX + Length
			// The Next Response Data Will Be Received In Uart1_RecvCallBack
			RecvResponseCardReader( &RecvBuffer[RecvBufferLen], 3 );
			
			// Begin To Send ENQ
			// Waiting Certain Time
			for( i = 0; i < 1000; i++ )
			{
				i = i;
			}

			// Send ENQ
			SendBuffer[0] = CR_ENQ;
			// Send The Command
			SendCommandCardReader( SendBuffer, 1 );

			// Start Time Out
			// Start TimeOut Check
//		   	CR_Command_Response_TimeOut = CreateTimer( TIMER_ONE_DELETE, TIMER_10MS * CR_WAITINGRESPONSEIMEOUT, CR_ResponseTimeOut );
		   	CR_Command_Response_TimeOut = CreateTimer( TIMER_ONE_DELETE, CR_WAITINGRESPONSEIMEOUT, CR_ResponseTimeOut );
			
			// Waiting The Response
			CR_CommandStatus = CR_CommandWaitingResponse;
		}
	}
	// Command Sent And Waiting For Response
	else if( CR_CommandStatus == CR_CommandWaitingResponse )
	{
        // Response Received, Check BCC
		if( ReceivedFlag == 2 )                 
		{
            //20120603  
            CRCardCommErrTimes = 0;
            
			if( CR_Calc_BCC( RecvBuffer, RecvBufferLen ) == 0 )
			{
				// Command Response Received
				// Saved In RecvBuffer
				// RecvBufferLen = length
				CR_CommandStatus = CR_CommandResponseReceived;
			}
			else
			{
				// Command Response Error,
				// Mark As Response Check BCC Error
				CR_CommandStatus = CR_CommandResponseError;
			}
		}
	}
    else//20120603  
    {
        //20120603         
        if( CR_CommandStatus == CR_CommandACKTimeOut )
        {
            //added card reader communication init if  some errs  20120603
            CRCardCommErrTimes++;
            
            if( CRCardCommErrTimes > CRCOMMERRTIMES )
            {
                 InitUSART1();
                 CRCardCommErrTimes = 0;
            } 
        }
        else if( CR_CommandStatus == CR_CommandResponseTimeOut )
        {
            //added card reader communication init if  some errs  20120603
            CRCardCommErrTimes++;
            
            if( CRCardCommErrTimes > CRCOMMERRTIMES )
            {
                 InitUSART1();
                 CRCardCommErrTimes = 0;
            }               
        }
    }
}



/*
 ** CR_CardReaderReset
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Reset The Card Reader
 *
// *  RETURNS: 0: Command Excute OK; 1: Command Is Excuting;
// *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *  RETURNS: 0x9000: Command Excute OK; 1: Command Is Excuting;
 *			 0xFD: Response Check BCC Error;
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *
 */
unsigned short int CR_CardReaderReset( void )
{
	unsigned short int retval;
	short int SendLen;

	// Default Excuting
	retval = 1;

	if( CR_CommandStatus == CR_CommandSending )
	{
		// Clear SendBuffer
		memset( SendBuffer, 0x00, sizeof( SendBuffer ));

		// Begin To Fill The Send Buffer
		SendLen = 0;

		// STX
		SendBuffer[SendLen]	= CR_STX;
		SendLen += 1;

		// Spare For Command Length
		SendLen += 2;

		// Card Reader Reset
		memcpy( &SendBuffer[SendLen], CR_RESET_CR, sizeof( CR_RESET_CR ));
		SendLen += sizeof( CR_RESET_CR );
		
		// Fill Command Length
		ShortIntToCharCopy( SendLen - 3, &SendBuffer[1] );

		// ETX
		SendBuffer[SendLen]	= CR_ETX;
		SendLen += 1;

		// BCC
		SendBuffer[SendLen]	= CR_Calc_BCC( SendBuffer, SendLen );
		SendLen += 1;
	}
	
	// Send The Command
	CR_Command( SendBuffer, SendLen );

	if( CR_CommandStatus == CR_CommandResponseReceived )
	{
		retval = CharToShortIntCopy( &RecvBuffer[1] );
		
		memcpy( TRINP.TR_CR_VERSION, &RecvBuffer[5], retval - 2 );
	
		CR_CommandStatus = CR_CommandSending;

		retval = 0x9000;
	}
	else if( CR_CommandStatus == CR_CommandResponseError )
	{
		CR_CommandStatus = CR_CommandSending;

		retval = 0xFD;
	}
	else if( CR_CommandStatus == CR_CommandACKTimeOut )
	{
		CR_CommandStatus = CR_CommandSending;

		retval = 0xFE;
	}
	else if( CR_CommandStatus == CR_CommandResponseTimeOut )
	{
		CR_CommandStatus = CR_CommandSending;

		retval = 0xFF;
	}
	else
	{
		retval = 1;
	}
	
	// Return
	return retval;
}



/*
 ** CR_Command_CR
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	*pCommand: Command String For The Card Reader; CR_CommandLen: Length Of The Command
 *
 *  DESCRIPTION: Commands For Card Reader, Including
 *					Card Reader State, Card Reader Spring Out Card
 *					Card Reader Power On Card, Card Reader Power Off Card
 *					PSAM Power Off
 *					
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFD: Response Check BCC Error;
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return 0x9000 For Excute Ok
 *
 *	Note: The Excute Result Is Saved In RecvBuffer[5]
 *
 */
unsigned short int CR_Command_CR( const unsigned char *pCommand, unsigned char CR_CommandLen )
{
	unsigned short int retval;
	short int SendLen;

	// Default Excuting
	retval = 1;

	if( CR_CommandStatus == CR_CommandSending )
	{
		// Clear SendBuffer
		memset( SendBuffer, 0x00, sizeof( SendBuffer ));

		// Begin To Fill The Send Buffer
		SendLen = 0;

		// STX
		SendBuffer[SendLen]	= CR_STX;
		SendLen += 1;

		// Spare For Command Length
		SendLen += 2;

		// Card Reader Check Card Command 
		memcpy( &SendBuffer[SendLen], pCommand, CR_CommandLen );
		SendLen += CR_CommandLen;

		// Fill Command Length
		ShortIntToCharCopy( SendLen - 3, &SendBuffer[1] );

		// ETX
		SendBuffer[SendLen]	= CR_ETX;
		SendLen += 1;

		// BCC
		SendBuffer[SendLen]	= CR_Calc_BCC( SendBuffer, SendLen );
		SendLen += 1;
	}
	
	// Send The Command
	CR_Command( SendBuffer, SendLen );

	if( CR_CommandStatus == CR_CommandResponseReceived )
	{
		if( RecvBuffer[5] == CR_RESPONSE_YES )
		{
			// Reset The Error Check Times
			CommErrorTimes = 0;

			// Excute OK
			retval = 0x9000;
		}
		else if( RecvBuffer[5] == CR_RESPONSE_NO )
		{
			// Excute Error
			retval = 3;
		}
		else
		{
			// No Card For Excute
			retval = 2;
                        
		}
	
		// For Next Sending
		CR_CommandStatus = CR_CommandSending;
                //modified for COMM err 20120522
                //=0
                
               //modified for COMM err 20120522
	}
	else if( CR_CommandStatus == CR_CommandResponseError )
	{
		CR_CommandStatus = CR_CommandSending;

		retval = 0xFD;
                //modified for COMM err 20120522
                
                //+1
               //modified for COMM err 20120522
	}
	else if( CR_CommandStatus == CR_CommandACKTimeOut )
	{
		CR_CommandStatus = CR_CommandSending;

		retval = 0xFE;
                //
                //modified for COMM err 20120522
                
                //+1
               //modified for COMM err 20120522
                
	}
	else if( CR_CommandStatus == CR_CommandResponseTimeOut )
	{
		CR_CommandStatus = CR_CommandSending;

		retval = 0xFF;
                //modified for COMM err 20120522
                
                //+1
               //modified for COMM err 20120522
                
                //
	}
	else
	{
		retval = 1;
	}
	
	// Return
	return retval;
}

/*
 ** CR_CardReaderCheckCard
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Wether Card In, Or Not
 *
 *  RETURNS: 0: Command Excute OK; 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *
 */
static Tmr* pTmrCheckCrad;
unsigned short int CR_CardReaderCheckCard( void )
{
	unsigned short int retval;

#ifdef MAKE_CASH_CR
	// For Cash Card, Use PSAM4 As IC Card2
	if( CRType == CR_TYPE_CASH )
	{
		// Always Card In
		retval = 0x9000;
	}
	else
#endif
	{
        if( CanCheckCard )
        {
		    // Check Card State Command
		    retval = CR_Command_CR( CR_CARD_STATE, sizeof( CR_CARD_STATE ));
        }
        else
        {
            retval = 1;
        }
        
        if( retval != 1 )
        {
            CanCheckCard = 0;
			if(pTmrCheckCrad == (Tmr*)0)
			{
            	pTmrCheckCrad = CreateTimer( TIMER_ONE, 500, CanCheckCardDelay );
			}
			else
			{
				ResetTimer(pTmrCheckCrad);
			}
        }
	}

	// Return
	return retval;
}


/*
 ** CR_CardReaderSpringOutCard
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Spring Out The Card In Card Reader
 *
 *  RETURNS: 0: Command Excute OK; 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *
 */
static Tmr* pTmrCardSpring;
unsigned short int CR_CardReaderSpringOutCard( void )
{
	unsigned short int retval;
	
//	// Check First Power On Spring Out Card
//	if( CRPowerFlag & CRPOWERON )
//	{
//		// First Power On, Just Spring Out Card
//		retval = CR_Command_CR( CR_SPRING_CARD, sizeof( CR_SPRING_CARD ));
//	
//		// Clear The Bit
//		CRPowerFlag &= ( ~CRPOWERON );
//	}
//	// Check Power Down Flag Or Not
//	else if( CRPowerFlag & CRPOWERDOWN )
	if( CRPowerFlag & CRPOWERDOWN )
	{
		// Power Down, Just Spring Out Card
		retval = CR_Command_CR( CR_SPRING_CARD, sizeof( CR_SPRING_CARD ));
	
		if( retval != 1 )
		{
			// Clear The Bit
			CRPowerFlag &= ( ~CRPOWERDOWN );

			// Inform Power Down Process
			ProcExchange[PROCESS_CR][PROCESS_POWERDOWN] |= POWERDOWN; 
		}
	}
	else
	{
		// Normal Trans Spring Out Card
		if( HoldCard == 0 )
		{
            if( CardCanSpring )
            {
                // Spring Out Card Command
                retval = CR_Command_CR( CR_SPRING_CARD, sizeof( CR_SPRING_CARD ));
            }
            else
            {
                retval = 1;
            }
            
			if( retval != 1 )
			{      
                CardCanSpring = 0;
				if(pTmrCardSpring == (Tmr*)0)
				{
                	pTmrCardSpring = CreateTimer( TIMER_ONE, 1000, CardCanSpringDelay );
				}
				else
				{
					ResetTimer(pTmrCardSpring);
				}
                
				// A Real Spring Out Card, Clear The Reserved PIN Info
				// Clear The Reserved PIN Information
				memset( PINInfo.CardASN, 0x00, sizeof( PININFO ));
			}
		}
		else
		{
			retval = 0x9000;
			HoldCard = 0;
		}
	}

	// Return
	return retval;
}


/*
 ** CR_CardReaderPowerOnCard
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Power On The Card In The Card Reader
 *
 *  RETURNS: 0: Command Excute OK; 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *
 */
unsigned short int CR_CardReaderPowerOnCard( void )
{
	unsigned short int retval;
	
	// Power On Card Command
	retval = CR_Command_CR( CR_CARDPOWERON, sizeof( CR_CARDPOWERON ));

	// Return
	return retval;
}


/*
 ** CR_CardReaderPowerOffCard
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Power OFF The Card In The Card Reader
 *
 *  RETURNS: 0: Command Excute OK; 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *
 */
unsigned short int CR_CardReaderPowerOffCard( void )
{
	unsigned short int retval;

	// Check PowerDown Or Not
	if( CRPowerFlag & CRPOWERDOWN )
	{
		// Set The Power Down Flag To 2
		PowerDownFlg = 0x02;
	}
						
	if(( CardType == CardTypeA ) || ( CardType == CardTypeB ))
	{
		// Deselect Card
		retval = CR_Command_CR( CR_DESELECT, sizeof( CR_DESELECT ));
	}
	else
	{
		// Power Off Card Command
		retval = CR_Command_CR( CR_CARDPOWEROFF, sizeof( CR_CARDPOWEROFF ));
	}

	// Return
	return retval;
}

/*
 ** CR_CardReaderPowerOffPSAM
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Power OFF The PSAM In The Card Reader
 *
 *  RETURNS: 0: Command Excute OK; 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *
 */
unsigned short int CR_CardReaderPowerOffPSAM( void )
{
	unsigned short int retval;
	
	// Power Off The PSAMs
	retval = CR_Command_CR( CR_PSAMPOWEROFF, sizeof( CR_PSAMPOWEROFF ));

	// Return
	return retval;
}


/*
 ** CardReader_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Card Reader Process, Perform
 *				  Card Reader Reset
 *
 *  RETURNS: None.
 *
 */
void CardReader_Proc( void )
{
	unsigned short int retval;

//	if( CR_CommandStatus == CR_CommandSending )
//	{
//		// First Entry, Display: " Reset The Card Reader "
//		CRFillMsg( MsgCardReaderReset );
//	}
		
	// Only Running When CR_Proc_Step = CR_CARDREADER_PROC
	if( CR_Proc_Step == CR_CARDREADER_PROC )
	{
		switch( CR_CommandType )
		{
			case CR_ICCARD_DELAY:

				// Delay A Certain Time For Test Approval
		   		if( CardDelaySet == 0 )
				{
		   			CreateTimer( TIMER_ONE_DELETE, CardDelayTimeOut * 4, CardDelay );
					CardDelaySet = 1;
				}
				else
				{
					if( CardDelayed == 1 )
					{
						CardDelaySet = 0;
						CardDelayed = 0;

						if( flag == 1 )
						{
							flag = 0;

							// Card Reader Reset Error, Reset Card Reader Next
							Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );
						}
						else
						{
							// Card Reader Reset Ok, Chek PSAM Next
							Set_Next_Command( CR_PSAM_PROC, CR_PSAM_COLD_RESET );
						}
					}
					else
					{
						// Loop
						Set_Next_Command( CR_CARDREADER_PROC, CR_ICCARD_DELAY );
					}
				}

			break;
		
			
			
			
			default:

				retval = CR_CardReaderReset();
					
				if( retval != 1 )
				{
					// Check Response
					if( retval == 0x9000 )
					{
						if( IcTypeFlg == 0 )
						{
							// For Intelligence Card Reader, Get Card Type Next
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_GET_CARD_TYPE );
//							Set_Next_Command( CR_CARDREADER_PROC, CR_CommandStart );

							// Delay A Certain Time For Test Approval
							Set_Next_Command( CR_CARDREADER_PROC, CR_ICCARD_DELAY );
						}
						else
						{
							// Card Reader Reset Ok, Chek PSAM Next
							Set_Next_Command( CR_PSAM_PROC, CR_PSAM_COLD_RESET );
						}
					}
					else
					{
						// Card Reset Error, Inform Error
						CommErrorTimes++;

						// Display " Card Reset Error "
						FillInfoMsg( M_CRResetErr, 0, 0 );

						if( CommErrorTimes > CR_COMM_ERR_MAX_TIMES )
						{
							// Next Retry, Same Proc And Same Command
							CR_CommandStatus = CR_CommandSending;

						}
						else
						{
							flag = 1;
							
							// Delay A Certain Time For Test Approval
							Set_Next_Command( CR_CARDREADER_PROC, CR_ICCARD_DELAY );

							// Next Retry, Same Proc And Same Command
							CR_CommandStatus = CR_CommandSending;
						}
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;
		}
	}
}


/*
 ** Set_Next_Command
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	unsigned int Step: Next Process Step Which Will Be Saved In CR_Proc_Step;
 *				unsigned int Command: Next Command Which Will Be Saved In CR_CommandType
 *
 *  DESCRIPTION: Set The Next Process Command
 *
 *  RETURNS: None.
 *
 */
void Set_Next_Command( enum CR_PROC_STEP Step, enum CR_COMMAND_TYPE Command )
{
	// Save Last Step
	CR_Proc_Last_Step = CR_Proc_Step;
	CR_Last_CommandType = CR_CommandType;

	// Set Next Process Step
	CR_Proc_Step = Step;
	// Set Next Command To Be Excuted
	CR_CommandType = Command;

//	// Write Spring Out Card Log
	CRSpringCardLog();

//	// Write Step Log
//	CRWriteStepLog();
}



