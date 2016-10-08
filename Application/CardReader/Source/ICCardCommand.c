
#include <string.h>
#include "ICCardCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "define.h"
#include "Pdprotect.h"
#include "global.h"

//Douzhq 20140620 Add , Vip Fueling
#ifndef MAKE_CASH_CR
#include "SocialCard.h"
#endif

// For IC Card
// SINOPEC APPLICATION: 0A0H,00H,00H,00H,03H,'SINOPEC'
static const unsigned char SINOPEC_APP_AID[12] = { 0xA0, 0x00, 0x00, 0x00, 0x03, 'S', 'I', 'N', 'O', 'P', 'E', 'C' };

// LOYALTY APPLICATION: 0A0H,00H,00H,00H,03H,'LOYALTY'
static const unsigned char LOYALTY_APP_AID[12] = { 0xA0, 0x00, 0x00, 0x00, 0x03, 'L', 'O', 'Y', 'A', 'L', 'T', 'Y' };

//Douzhq 20140613 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
#if 1
//static const unsigned char VOLAPP_APP_AID[11] = { 0xA0, 0x00, 0x00, 0x00, 0x03, 'V', 'O', 'L', 'A', 'P', 'P' };
static const unsigned char VOLAPP_APP_AID[6] = { 'V', 'O', 'L', 'A', 'P', 'P' };
#else
static const unsigned char VOLAPP_APP_AID[12] = { 0xA0, 0x00, 0x00, 0x00, 0x03, 'S', 'I', 'N', 'O', 'P', 'E', 'C' };
#endif
#endif

// Save The Record List Number,
// Special Use In Read IC Card Record
unsigned char RecordListNumber = 0;



/*
 ** CR_ICCard_ColdReset
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Reset The PSAMs
 *
// *  RETURNS: 0: Command Excute OK; 1: Command Is Excuting;
// *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
// *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *  RETURNS: 0x9000: Command Excute OK; 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFD: Response Check BCC Error;
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *
 *	Note: The Excute Result Is Saved In RecvBuffer[RespDATOffset]
 *
 */
unsigned short int CR_IC_ColdReset_7816( void )
{
	unsigned short int retval;
	unsigned short int ATRLen;
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

#ifdef MAKE_CASH_CR
		// For Cash Card, Use PSAM4 As IC Card2
		if( CRType == CR_TYPE_CASH )
		{
			// IC Card2 Command Leading Charactor
			SendBuffer[SendLen]	= PSAM_COMMAND_LEADING;
			SendLen += 1;

			// IC Card2 Voltage
			SendBuffer[SendLen]	= PSAM_50V;
			SendLen += 1;

			// IC Card2 Location
			SendBuffer[SendLen]	= ICCARD2;
			SendLen += 1;

			// IC Card2 Mode
			SendBuffer[SendLen]	= MODE_7816;
			SendLen += 1;
		}
		else
#endif

		{
			// IC Card Cold Reset Command
			memcpy( &SendBuffer[SendLen], ICCARD_COLD_RESET_7816, sizeof( ICCARD_COLD_RESET_7816 ));
			SendLen += sizeof( ICCARD_COLD_RESET_7816 );
		}

		// Fill Command Length
		ShortIntToCharCopy( SendLen - 3, &SendBuffer[1] );

		// ETX
		SendBuffer[SendLen]	= CR_ETX;
		SendLen += 1;

		// BCC
		SendBuffer[SendLen]	= CR_Calc_BCC( SendBuffer, SendLen );
		SendLen += 1;

#ifdef MAKE_CASH_CR
		// For Cash Card, Use PSAM4 As IC Card2
		if( CRType == CR_TYPE_CASH )
		{
			// Define The Response Running Flag Byte Offset
			RespCMDOffset = 0x06;

			// Define The Response Data Byte Offset
			RespDATOffset = 0x07;
		}
		else
#endif

		{
			// Define The Response Running Flag Byte Offset
			RespCMDOffset = 0x05;

			// Define The Response Data Byte Offset
			RespDATOffset = 0x06;
		}
	}
	
	// Send The Command
	CR_Command( SendBuffer, SendLen );

	if( CR_CommandStatus == CR_CommandResponseReceived )
	{
		if( RecvBuffer[RespCMDOffset] == CR_RESPONSE_YES )
		{
			ATRLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
			memcpy( TRINP.TR_CARD_ATR, &RecvBuffer[RespDATOffset+2], ATRLen );

//			// Reset The Error Check Times
//			CommErrorTimes = 0;

			// Reset Ok
//			retval = 0;
			retval = 0x9000;
		}
		else if( RecvBuffer[RespCMDOffset] == CR_RESPONSE_NO )
		{
			// Reset Error
			retval = 3;
		}
		else
		{
			// No IC Card
			retval = 2;
		}
		
		CR_CommandStatus = CR_CommandSending;
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
 ** ICCard_Select_Sinopec_APP
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Select SINOPEC_APP Application
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int ICCard_Select_Sinopec_APP( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xA4;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x04;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = sizeof( SINOPEC_APP_AID );
	len += 1;
	
	memcpy( pBuffer + len, SINOPEC_APP_AID, sizeof( SINOPEC_APP_AID ));
	len += sizeof( SINOPEC_APP_AID );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}


/*
 ** ICCard_Select_Loyalty_APP
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Select LOYALTE_APP Application
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int ICCard_Select_Loyalty_APP( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xA4;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x04;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = sizeof( LOYALTY_APP_AID );
	len += 1;
	
	memcpy( pBuffer + len, LOYALTY_APP_AID, sizeof( LOYALTY_APP_AID ));
	len += sizeof( LOYALTY_APP_AID );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

//Douzhq 20140612 Add , Save Oil 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
unsigned short int ICCard_Select_Saveoil_APP( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xA4;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x04;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = sizeof( VOLAPP_APP_AID );
	len += 1;
	
	memcpy( pBuffer + len, VOLAPP_APP_AID, sizeof( VOLAPP_APP_AID ));
	len += sizeof( VOLAPP_APP_AID );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}
#endif

// 20151217 qiyong add for zhonghangyou requirement
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
unsigned short int ICCard_Read_FID_0x15( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xB0;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = ( 0x15 | 0x80 );
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// LE;
	*( pBuffer + len ) = 0x01;
	len += 1;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}
#endif

/*
 ** ICCard_Read_SFI_0x15
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Read Binary File SFI = 0x15
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int ICCard_Read_SFI_0x15( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xB0;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = ( 0x15 | 0x80 );
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x1E;
	len += 1;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** ICCard_Read_SFI_0x16
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Read Binary File SFI = 0x16
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int ICCard_Read_SFI_0x16( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xB0;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = ( 0x16 | 0x80 );
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x29;
	len += 1;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** ICCard_Read_SFI_0x1B
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Read Binary File SFI = 0x1B
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int ICCard_Read_SFI_0x1B( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xB0;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = ( 0x1B | 0x80 );
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x20;
	len += 1;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** ICCard_Read_SFI_0x1C
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Read Binary File SFI = 0x1C
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int ICCard_Read_SFI_0x1C( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xB0;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = ( 0x1C | 0x80 );
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x60;
	len += 1;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** ICCard_Read_Record
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Read IC Card Records SFI = 0x18
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *	Note: The Record Length Is 23(0x17)
 *			The SW1/SW2 == 0x6A83, Means The Records Is End
 *			Otherwise, The RecordListNumber Save The Record To Be Read
 *
 */
unsigned short int ICCard_Read_Record( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xB2;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = RecordListNumber;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0xC4;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x17;
	len += 1;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** ICCard_Verify_Pin
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Verify The PIN
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *	Note: Pin Was Saved In TR_INPUT_PIN[]
 */
unsigned short int ICCard_Verify_Pin( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x20;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = TRINP.TR_INPUT_PIN_LEN;
	len += 1;

	memcpy( pBuffer + len, TRINP.TR_INPUT_PIN, TRINP.TR_INPUT_PIN_LEN );
	len += TRINP.TR_INPUT_PIN_LEN;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** ICCard_Get_Lock_Proof
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Read Grey Lock Status
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *	
 */
unsigned short int ICCard_Get_Lock_Proof( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0xE0;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xCA;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x1E;
	len += 1;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** ICCard_Clear_Lock_Proof
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Clear The Grey Lock Status, After The Debit.
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *	
 */
unsigned short int ICCard_Clear_Lock_Proof( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0xE0;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xCA;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x01;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x00;
	len += 1;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** ICCard_Get_Balance
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Get The Application Balance
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *	
 */
unsigned short int ICCard_Get_Balance( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x5C;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x01;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x04;
	len += 1;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** ICCard_Init_Grey_Lock
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Initiate The Grey Lock For IC Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int ICCard_Init_Grey_Lock( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0xE0;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x7A;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x08;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x01;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x07;
	len += 1;

	// PSAM Key Index
	*( pBuffer + len ) = TRINP.TR_PSAM_KEY_INDEX;
	len += 1;

	memcpy( pBuffer + len, TRINP.TR_PSAM_TID, sizeof( TRINP.TR_PSAM_TID ));
	len += sizeof( TRINP.TR_PSAM_TID );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** ICCard_Set_Grey_Lock
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Set The Grey Lock For IC Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int ICCard_Set_Grey_Lock( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0xE0;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x7C;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x08;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x13;
	len += 1;

	// PSAM TTC
	memcpy( pBuffer + len, TRINP.TR_PSAM_TTC, sizeof( TRINP.TR_PSAM_TTC ));
	len += sizeof( TRINP.TR_PSAM_TTC );

	// PSAM RANDOM
	memcpy( pBuffer + len, TRINP.TR_PSAM_RANDOM, sizeof( TRINP.TR_PSAM_RANDOM ));
	len += sizeof( TRINP.TR_PSAM_RANDOM );

	// Data, Transaction Date, 4
	memcpy( pBuffer + len, TRINP.TR_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));
	len += sizeof( TRINP.TR_TRANS_DATE );

	// Data, Transaction Time, 3
	memcpy( pBuffer + len, TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));
	len += sizeof( TRINP.TR_TRANS_TIME );

	// PSAM MAC1
	memcpy( pBuffer + len, TRINP.TR_PSAM_MAC1, sizeof( TRINP.TR_PSAM_MAC1 ));
	len += sizeof( TRINP.TR_PSAM_MAC1 );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** ICCard_Debit_Grey_Unlock
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Debit From The IC Card, And Change The Grey Flag To 0x10
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int ICCard_Debit_Grey_Unlock( unsigned char *pBuffer )
{
	unsigned short int len;
	
//Douzhq 20140626 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	unsigned char PhysicsNozzle;
	
	unsigned short vipPrice;
	unsigned int   vipAmount;
	unsigned int   vipGrade;
	unsigned char  vipAmountChar[4];
	
	if (PowerDown.VipFuelingMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x00))
	{
		vipGrade = PowerDown.PdTrans.PD_CASH_TOTAL[2];
		
		if (vipGrade > 0 && vipGrade <= 0x07)
		{
			PhysicsNozzle = GetPhysicsNzn(TRINP.TR_TRANS_NOZZLE_NUMBER);
			
			vipPrice = GetPrice(PhysicsNozzle , vipGrade);
			vipAmount = vipPrice * CharToIntCopy(TRINP.TR_TRANS_VOLUME);
			if ((vipAmount % 100 / 10) > 4)
			{
				vipAmount /=  100;
				vipAmount++;
			}
			else
			{
				vipAmount /=  100;
			}
			IntToCharCopy(vipAmount , vipAmountChar);
			memcpy(TRINP.TR_TRANS_AMOUNT,vipAmountChar,sizeof(TRINP.TR_TRANS_AMOUNT));
		}
	}
#endif
	
	
	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0xE0;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x7E;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x08;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x01;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x1B;
	len += 1;

#ifdef MAKE_CARD_MODE2
//	// For Sinopec Mode2, Use TransAmount1 Instead of TransAmount
//	if( TRINP.TR_WORK_MODE == 0x01 )
//	{
//		// Online, Use TransAmount1
//		// Data, Transaction Amount, 4
		memcpy( pBuffer + len, TRINP.TR_TRANS_AMOUNT_T1, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));
		len += sizeof( TRINP.TR_TRANS_AMOUNT_T1 );
//	}
//	else
//	{
//		// Offline, Use TransAmount
//		// Data, Transaction Amount, 4
//		memcpy( pBuffer + len, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
//		len += sizeof( TRINP.TR_TRANS_AMOUNT );
//	}
#else
	// Transaction Amount
		
//IC Card Cut Payment
//Douzhq 20140610 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (TRINP.TR_TRANS_DEBITSOURCE == 0x04 && PowerDown.SaveOilAppMode == 0x01)
	{
		memcpy( pBuffer + len, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
		len += sizeof( TRINP.TR_TRANS_AMOUNT );
	}
	else
	{
		memcpy( pBuffer + len, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
		len += sizeof( TRINP.TR_TRANS_AMOUNT );
	}
#else
	memcpy( pBuffer + len, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
	len += sizeof( TRINP.TR_TRANS_AMOUNT );
#endif
	
	
#endif	// MAKE_CARD_MODE2

	// Card TTC
	memcpy( pBuffer + len, TRINP.TR_CARD_APP_TTC, sizeof( TRINP.TR_CARD_APP_TTC ));
	len += sizeof( TRINP.TR_CARD_APP_TTC );

	// PSAM Terminal ID
	memcpy( pBuffer + len, TRINP.TR_PSAM_TID, sizeof( TRINP.TR_PSAM_TID ));
	len += sizeof( TRINP.TR_PSAM_TID );
	
	// PSAM TTC
	memcpy( pBuffer + len, TRINP.TR_PSAM_TTC, sizeof( TRINP.TR_PSAM_TTC ));
	len += sizeof( TRINP.TR_PSAM_TTC );

	// Data, Transaction Date, 4
	memcpy( pBuffer + len, TRINP.TR_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));
	len += sizeof( TRINP.TR_TRANS_DATE );

	// Data, Transaction Time, 3
	memcpy( pBuffer + len, TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));
	len += sizeof( TRINP.TR_TRANS_TIME );

	// PSAM GMAC
	memcpy( pBuffer + len, TRINP.TR_PSAM_GMAC, sizeof( TRINP.TR_PSAM_GMAC ));
	len += sizeof( TRINP.TR_PSAM_GMAC );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}


/*
 ** ICCard_Debit_Purchase
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Debit From The IC Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int ICCard_Debit_Purchase( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x54;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x01;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x0F;
	len += 1;

	// PSAM TTC
	memcpy( pBuffer + len, TRINP.TR_PSAM_TTC, sizeof( TRINP.TR_PSAM_TTC ));
	len += sizeof( TRINP.TR_PSAM_TTC );

	// Data, Transaction Date, 4
	memcpy( pBuffer + len, TRINP.TR_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));
	len += sizeof( TRINP.TR_TRANS_DATE );

	// Data, Transaction Time, 3
	memcpy( pBuffer + len, TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));
	len += sizeof( TRINP.TR_TRANS_TIME );

	// PSAM MAC1
	memcpy( pBuffer + len, TRINP.TR_PSAM_MAC1, sizeof( TRINP.TR_PSAM_MAC1 ));
	len += sizeof( TRINP.TR_PSAM_MAC1 );

//	// Para3 = Data Length;
//	*( pBuffer + len ) = 0x08;
//	len += 1;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** ICCard_Init_Purchase
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Initialize Purchase For IC Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int ICCard_Init_Purchase( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x50;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x01;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x01;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x0B;
	len += 1;

	// Key Index
	*( pBuffer + len ) = TRINP.TR_PSAM_KEY_INDEX;
	len += 1;

	// TRANS Amount
	memcpy( pBuffer + len, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
	len += sizeof( TRINP.TR_TRANS_AMOUNT );

	// PSAM TID
	memcpy( pBuffer + len, TRINP.TR_PSAM_TID, sizeof( TRINP.TR_PSAM_TID ));
	len += sizeof( TRINP.TR_PSAM_TID );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}


/*
 ** ICCard_Get_Response
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	DataLen: The Length Need To Read
 *
 *  DESCRIPTION: IC Card Get Response
 *
 *  RETURNS: None.
 *
 * Note: Return To The Command Which Need To Get The Response
 *
 */
unsigned short int ICCard_Get_Response( unsigned char DataLen )
{
	unsigned short int SendLen;

	// Clear SendBuffer
	memset( SendBuffer, 0x00, sizeof( SendBuffer ));

	// Begin To Fill The Send Buffer
	SendLen = 0;

	// STX
	SendBuffer[SendLen]	= CR_STX;
	SendLen += 1;

	// Spare For Command Length
	SendLen += 2;

#ifdef MAKE_CASH_CR
	// For Cash Card, Use PSAM4 As IC Card2
	if( CRType == CR_TYPE_CASH )
	{
		// Card Reader IC Card2 Command
		memcpy( &SendBuffer[SendLen], PSAM_CAPDU_COMMAND_T0, sizeof( PSAM_CAPDU_COMMAND_T0 ));
		SendLen += sizeof( PSAM_CAPDU_COMMAND_T0 );

		SendBuffer[SendLen] = ICCARD2;
		SendLen += 1;
	}
	else
#endif

	{
		// Card Reader IC Card Command
		if(( CardType == CardTypeA ) || ( CardType == CardTypeB ))
		{
			memcpy( &SendBuffer[SendLen], ICCARD_CAPDU_COMMAND_CL, sizeof( ICCARD_CAPDU_COMMAND_CL ));
			SendLen += sizeof( ICCARD_CAPDU_COMMAND_CL );
		}
		else
		{
			memcpy( &SendBuffer[SendLen], ICCARD_CAPDU_COMMAND_T0, sizeof( ICCARD_CAPDU_COMMAND_T0 ));
			SendLen += sizeof( ICCARD_CAPDU_COMMAND_T0 );
		}
	}

	// Command Length
	SendBuffer[SendLen] = 0x00;
	SendLen += 1;
		
	SendBuffer[SendLen] = 0x05;
	SendLen += 1;
		
	// PSAM Command Get Response
	memcpy( &SendBuffer[SendLen], "\x00\xC0\x00\x00", 4 );
	SendLen += 4;

	// Response Data Length
	SendBuffer[SendLen] = DataLen;
	SendLen += 1;
		
	// Fill Command Length
	ShortIntToCharCopy( SendLen - 3, &SendBuffer[1] );

	// ETX
	SendBuffer[SendLen]	= CR_ETX;
	SendLen += 1;

	// BCC
	SendBuffer[SendLen]	= CR_Calc_BCC( SendBuffer, SendLen );
	SendLen += 1;

	// Return
	return SendLen;
}











/*
 ** CR_CAPDU_Command_T0_ICCard
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	*ICCARD_COMMAND( unsigned char * ): IC Card Command Data Function, Such As IC Card Select Application
 *					IC Card Grey Lock etc.
 *
 *  DESCRIPTION: Excute The IC Card Command
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFD: Response Check BCC Error;
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The IC Card Command Excuted
 *
 *	Note: The Excute Result Is Saved In RecvBuffer[5]
 *
 */
unsigned short int CR_CAPDU_Command_T0_ICCard( unsigned short int ( *ICCARD_COMMAND )( unsigned char * ))
{
	unsigned short int retval;
	unsigned short int CommandLen;
	unsigned short int SendLen;

	
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


#ifdef MAKE_CASH_CR
		// For Cash Card, Use PSAM4 As IC Card2
		if( CRType == CR_TYPE_CASH )
		{
			// Card Reader IC Card2 Command
			memcpy( &SendBuffer[SendLen], PSAM_CAPDU_COMMAND_T0, sizeof( PSAM_CAPDU_COMMAND_T0 ));
			SendLen += sizeof( PSAM_CAPDU_COMMAND_T0 );

			SendBuffer[SendLen] = ICCARD2;
			SendLen += 1;
		}
		else
#endif

		{
			// Card Reader PSAM Command
			if(( CardType == CardTypeA ) || ( CardType == CardTypeB ))
			{
				memcpy( &SendBuffer[SendLen], ICCARD_CAPDU_COMMAND_CL, sizeof( ICCARD_CAPDU_COMMAND_CL ));
				SendLen += sizeof( ICCARD_CAPDU_COMMAND_CL );
			}
			else
			{
				memcpy( &SendBuffer[SendLen], ICCARD_CAPDU_COMMAND_T0, sizeof( ICCARD_CAPDU_COMMAND_T0 ));
				SendLen += sizeof( ICCARD_CAPDU_COMMAND_T0 );
			}
		}

		// IC Card Command
		CommandLen = ICCARD_COMMAND( &SendBuffer[SendLen] );
		SendLen += CommandLen;

		
		// Fill Command Length
		ShortIntToCharCopy( SendLen - 3, &SendBuffer[1] );

		// ETX
		SendBuffer[SendLen]	= CR_ETX;
		SendLen += 1;

		// BCC
		SendBuffer[SendLen]	= CR_Calc_BCC( SendBuffer, SendLen );
		SendLen += 1;

#ifdef MAKE_CASH_CR
		// For Cash Card, Use PSAM4 As IC Card2
		if( CRType == CR_TYPE_CASH )
		{
			// Define The Response Running Flag Byte Offset
			RespCMDOffset = 0x06;

			// Define The Response Data Byte Offset
			RespDATOffset = 0x07;
		}
		else
#endif

		{
			// Define The Response Running Flag Byte Offset
			RespCMDOffset = 0x05;

			// Define The Response Data Byte Offset
			RespDATOffset = 0x06;
		}
	}
	
	// Send The Command
	CR_Command( SendBuffer, SendLen );

	if( CR_CommandStatus == CR_CommandResponseReceived )
	{
		if( RecvBuffer[RespCMDOffset] == CR_RESPONSE_YES )
		{
			// Save SW1/SW2
			retval = CharToShortIntCopy( &RecvBuffer[RecvBufferLen - 4] );
		
			// Check Need Get Response Or Not
			if(( retval / 0x100 ) == 0x61 )
			{
				// Need Get Response
				SendLen = ICCard_Get_Response( retval % 0x100 );
				
				// Continue Excuting
				retval = 1;

				// Send The Command
				CR_CommandStatus = CR_CommandSending;
				CR_Command( SendBuffer, SendLen );
			}
			// Check Need Re-Send Or Not
			else if(( retval / 0x100 ) == 0x6C )
			{

#ifdef MAKE_CASH_CR
				// For Cash Card, Use PSAM4 As IC Card2
				if( CRType == CR_TYPE_CASH )
				{
					// Re-Send Only For Read Binary And Read Record
					if( SendBuffer[8] == 0x00 )
					{
						if(( SendBuffer[9] == 0xB0 ) || ( SendBuffer[9] == 0xB2 ))
						{
							// Need Re-Send
							// Update SendBuffer[0] And SendBuffer[11]
							SendBuffer[0] = CR_STX;
							SendBuffer[12] = retval % 0x100;
							SendLen = 15;

							// Continue Excuting
							retval = 1;

							// Send The Command
							CR_CommandStatus = CR_CommandSending;
							CR_Command( SendBuffer, SendLen );
						}
						else
						{
							// Do Not Re-Send, Go On....
							CR_CommandStatus = CR_CommandSending;
						}
					}
					else
					{
						// Do Not Re-Send, Go On....
						CR_CommandStatus = CR_CommandSending;
					}
				}
				else
#endif

				{
					// Re-Send Only For Read Binary And Read Record
					if( SendBuffer[7] == 0x00 )
					{
						if(( SendBuffer[8] == 0xB0 ) || ( SendBuffer[8] == 0xB2 ))
						{
							// Need Re-Send
							// Update SendBuffer[0] And SendBuffer[11]
							SendBuffer[0] = CR_STX;
							SendBuffer[11] = retval % 0x100;
							SendLen = 14;

							// Continue Excuting
							retval = 1;

							// Send The Command
							CR_CommandStatus = CR_CommandSending;
							CR_Command( SendBuffer, SendLen );
						}
						else
						{
							// Do Not Re-Send, Go On....
							CR_CommandStatus = CR_CommandSending;
						}
					}
					else
					{
						// Do Not Re-Send, Go On....
						CR_CommandStatus = CR_CommandSending;
					}
				}
			}
			else
			{
				CR_CommandStatus = CR_CommandSending;
			}
		}
		else if( RecvBuffer[RespCMDOffset] == CR_RESPONSE_NO )
		{
			CR_CommandStatus = CR_CommandSending;
			// Excute Error
			retval = 3;
		}
		else
		{
			CR_CommandStatus = CR_CommandSending;
			// No Card For Excute
			retval = 2;
		}
	}
	else if( CR_CommandStatus == CR_CommandResponseError )
	{
		CR_CommandStatus = CR_CommandSending;

		retval = 0xFD;
	}
	else if( CR_CommandStatus == CR_CommandACKTimeOut )
	{
		CR_CommandStatus = CR_CommandSending;

		// ACK Time Out
		retval = 0xFE;
	}
	else if( CR_CommandStatus == CR_CommandResponseTimeOut )
	{
		CR_CommandStatus = CR_CommandSending;

		// Response Time Out
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
 ** CR_ICCARD_Select_Sinopec_APP
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: IC Card Select Sinopec_App Application
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Select_Sinopec_APP( void )
{
	unsigned short int retval;

	// Default Excuting
	retval = 1;

	// Select Sinopec_APP Application Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Select_Sinopec_APP );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
				
		// Debit Source
		TRINP.TR_TRANS_DEBITSOURCE = 0x00;

		// Trans Unit
		TRINP.TR_TRANS_UNIT = TRINP.TR_TRANS_DEBITSOURCE;

		// Trans Card Type
		TRINP.TR_CARD_TYPE = 0;

		// Inform Display
//		DISP_CR_Buf.AppType = TRINP.TR_TRANS_DEBITSOURCE;


		// Mark The Sinopec Application
//		TRINP.TR_TRANS_APP = 0x00;
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Select_Loyalty_APP
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: IC Card Select Loyalty_App Application
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Select_Loyalty_APP( void )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Loyalty_APP Application Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Select_Loyalty_APP );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
				
		// Debit Source
		TRINP.TR_TRANS_DEBITSOURCE = 0x01;

		// Trans Unit
		TRINP.TR_TRANS_UNIT = TRINP.TR_TRANS_DEBITSOURCE;

		// Trans Card Type
		TRINP.TR_CARD_TYPE = 0;

		// Inform Display
//		DISP_CR_Buf.AppType = TRINP.TR_TRANS_DEBITSOURCE;


		// Mark The Loyalty Application
//		TRINP.TR_TRANS_APP = 0x01;
	}

	// Return
	return retval;
}

//Douzhq 20140612 Add , Save Oil 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
unsigned short int CR_ICCARD_Select_Saveoil_APP( void )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Loyalty_APP Application Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Select_Saveoil_APP );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
				
		// Debit Source
		TRINP.TR_TRANS_DEBITSOURCE = 0x04;

		// Trans Unit
		TRINP.TR_TRANS_UNIT = 0x62;

		// Trans Card Type
		TRINP.TR_CARD_TYPE = 0;

		// Inform Display
//		DISP_CR_Buf.AppType = TRINP.TR_TRANS_DEBITSOURCE;


		// Mark The Loyalty Application
//		TRINP.TR_TRANS_APP = 0x01;
	}

	// Return

	return retval;
}
#endif

// 20151217 qiyong add for zhonghangyou requirement
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
unsigned short int CR_ICCARD_Read_FID_0x15( void )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 15 Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Read_FID_0x15 );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
	    TRINP.TR_CARD_FID15 = 0xFF;
        TRINP.TR_CARD_FID15 = RecvBuffer[RespDATOffset+2];
	}

	// Return
	return retval;
}
#endif

/*
 ** CR_ICCARD_Read_SFI_0x15
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read Binary File SFI = 0x15 In IC Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Read_SFI_0x15( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x15 Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Read_SFI_0x15 );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( TRINP.TR_CARD_ISSUER_ID, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Read_SFI_0x16
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read Binary File SFI = 0x16 In IC Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Read_SFI_0x16( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x16 Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Read_SFI_0x16 );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( &TRINP.TR_CARD_HOLDER_FLAG, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Read_SFI_0x1B
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read Binary File SFI = 0x1B In IC Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Read_SFI_0x1B( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x1B Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Read_SFI_0x1B );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( &TRINP.TR_CARD_PIN_DEFAULT, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Read_SFI_0x1C
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read Binary File SFI = 0x1C In IC Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Read_SFI_0x1C( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x1C Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Read_SFI_0x1C );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( TRINP.TR_CARD_GRADE_LIMIT, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Read_Record
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read Binary File SFI = 0x1C In IC Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 *	Note: The Record Length Is 23(0x17)
 *			The SW1/SW2 == 0x6A83, Means The Records Is End
 *			Otherwise, The RecordListNumber Save The Record To Be Read
 *
 */
unsigned short int CR_ICCARD_Read_Record( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x1C Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Read_Record );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( TRINP.TR_CARD_RECORD_TTC, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Verify_PIN
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Verify The PIN Of The IC Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Verify_PIN( void )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Verify IC Card PIN Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Verify_Pin );

	if( retval == 0x9000 )
	{
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Get_Lock_Proof
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read The Grey Lock Status And Messages
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Get_Lock_Proof( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read IC Card Grey Lock Proof Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Get_Lock_Proof );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( &TRINP.TR_CARD_GREY_FLAG, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Clear_Lock_Proof
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Clear The Grey Lock, After Debit
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Clear_Lock_Proof( void )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Clear IC Card Grey Lock Flag Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Clear_Lock_Proof );

	if( retval == 0x9000 )
	{
//		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
//	
//		memcpy( &TRINP.TR_CARD_GREY_FLAG, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Get_Balance
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get The Application Balance
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Get_Balance( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read IC Card Application Balance Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Get_Balance );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( &TRINP.TR_CARD_APP_BALANCE, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );


//#ifdef MAKE_CARD_MODE2
//		// Save For Preset, Special For Mode2 Amount Total Limited Check
//		memcpy( TRINP.TR_BALANCE_PRESET, TRINP.TR_CARD_APP_BALANCE, sizeof( TRINP.TR_BALANCE_PRESET ));
//#endif
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Init_Grey_Lock
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Initiate The Grey Lock Command To IC Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Init_Grey_Lock( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Initiate IC Card Grey Lock Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Init_Grey_Lock );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( TRINP.TR_CARD_APP_BALANCE0, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Set_Grey_Lock
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Set The Grey Lock Command To IC Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Set_Grey_Lock( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Set IC Card Grey Lock Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Set_Grey_Lock );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( TRINP.TR_CARD_GTAC, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Debit_Grey_Unlock
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Debit The Amount From IC Card And Unlock The IC Card( Grey Flag = 0x10, Waiting For Clear )
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Debit_Grey_Unlock( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Set IC Card Grey Lock Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Debit_Grey_Unlock );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( TRINP.TR_CARD_MAC3, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}


/*
 ** CR_ICCARD_Debit_Purchase
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Debit The Amount From IC Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Debit_Purchase( void )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Set IC Card Grey Lock Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Debit_Purchase );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		// Save TAC
		memcpy( TRINP.TR_CARD_MAC3, &RecvBuffer[RespDATOffset+2], 4 );
		memcpy( TRINP.TR_CARD_MAC2, &RecvBuffer[RespDATOffset+2+4], 4 );
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_Init_Purchase
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Initiate The Purchase Command To IC Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_ICCARD_Init_Purchase( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Initiate IC Card Purchase Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Init_Purchase );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		// Use The Same Response RAM With CR_ICCARD_Init_Grey_Lock In TRINP
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( TRINP.TR_CARD_APP_BALANCE0, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}







#ifdef MAKE_PSAM_VERIFY

/*
 ** ICCard_Read_SFI_0x1A
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Read Binary File SFI = 0x1A
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int ICCard_Read_SFI_0x1A( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xB0;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = ( 0x1A | 0x80 );
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x29;
	len += 1;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** CR_ICCARD_ACT_Read_SFI_0x1A
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read Binary File SFI = 0x1A In ACT Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The ACT Command Excuted
 *
 */
unsigned short int CR_ICCARD_ACT_Read_SFI_0x1A( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x16 Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Read_SFI_0x1A );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( &TRINP.TR_ACT_KEY_INDEX, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_RID_Read_SFI_0x1A
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read Binary File SFI = 0x1A In RID Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The ACT Command Excuted
 *
 */
unsigned short int CR_ICCARD_RID_Read_SFI_0x1A( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x16 Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Read_SFI_0x1A );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( &TRINP.TR_RID_KEY_INDEX, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_ICCARD_RID_Read_SFI_0x15
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read Binary File SFI = 0x15 In RID Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The ACT Command Excuted
 *
 */
unsigned short int CR_ICCARD_RID_Read_SFI_0x15( void )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x16 Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Read_SFI_0x15 );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( &TRINP.TR_RID_CARD_ISSUER_ID, &RecvBuffer[RespDATOffset+2], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** ICCard_ACT_Encrypted
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Get The Encrypted Data For ACT Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *  Atten: The Plain Data Is Saved In RecvBuffer; RecvBuffer[127] = Data Length
 *												  RecvBuffer[128].... = Data
 *
 */
unsigned short int ICCard_ACT_Encrypted( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xA8;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = TRINP.TR_ACT_KEY_INDEX;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = RecvBuffer[127];
	len += 1;

	// Para4.... = Data....;
	memcpy( pBuffer + len, &RecvBuffer[128], RecvBuffer[127] );
	len += RecvBuffer[127];

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** CR_ICCard_ACT_Encrypted
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get ACT Encrypted Data
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The ACT Command Excuted
 *
 *  Atten: The Plain Data Is Saved In RecvBuffer; RecvBuffer[127] = Data Length
 *												  RecvBuffer[128].... = Data
 *
 */
unsigned short int CR_ICCard_ACT_Encrypted( void )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x16 Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_ACT_Encrypted );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( TRINP.TR_ACT_ENCRYPTED, &RecvBuffer[RespDATOffset+2], sizeof( TRINP.TR_ACT_ENCRYPTED ));
	}

	// Return
	return retval;
}

/*
 ** ICCard_RID_Encrypted
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Get The Encrypted Data For RID Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *  Atten: The Plain Data Is Saved In RecvBuffer; RecvBuffer[127] = Data Length
 *												  RecvBuffer[128].... = Data
 *
 */
unsigned short int ICCard_RID_Encrypted( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xA8;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = TRINP.TR_RID_KEY_INDEX;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = RecvBuffer[127];
	len += 1;

	// Para4.... = Data....;
	memcpy( pBuffer + len, &RecvBuffer[128], RecvBuffer[127] );
	len += RecvBuffer[127];

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** CR_ICCard_RID_Encrypted
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get RID Crypted Data
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The ACT Command Excuted
 *
 *  Atten: The Plain Data Is Saved In RecvBuffer; RecvBuffer[127] = Data Length
 *												  RecvBuffer[128].... = Data
 *
 */
unsigned short int CR_ICCard_RID_Encrypted( void )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x16 Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_RID_Encrypted );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( TRINP.TR_RID_ENCRYPTED, &RecvBuffer[RespDATOffset+2], sizeof( TRINP.TR_RID_ENCRYPTED ));
	}

	// Return
	return retval;
}

/*
 ** ICCard_RID_Get_Log_Para_Total
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Get The Log Para Total For RID Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *
 */
unsigned short int ICCard_RID_Get_Log_Para_Total( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xB8;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x02;
	len += 1;

	// Para4.... = Data....;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** CR_ICCard_RID_Get_Log_Para_Total
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get RID Log Para
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The ACT Command Excuted
 *
 *
 */
unsigned short int CR_ICCard_RID_Get_Log_Para_Total( void )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x16 Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_RID_Get_Log_Para_Total );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( TRINP.TR_RID_LOG_PARA_TOTAL, &RecvBuffer[RespDATOffset+2], sizeof( TRINP.TR_RID_LOG_PARA_TOTAL ));
	}

	// Return
	return retval;
}

/*
 ** ICCard_RID_Get_Log_Para_Added
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Get The Log Para Added For RID Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *
 */
unsigned short int ICCard_RID_Get_Log_Para_Added( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xB8;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x01;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x02;
	len += 1;

	// Para4.... = Data....;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** CR_ICCard_RID_Get_Log_Para_Added
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get RID Log Para Added Currently
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The ACT Command Excuted
 *
 *
 */
unsigned short int CR_ICCard_RID_Get_Log_Para_Added( void )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x16 Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_RID_Get_Log_Para_Added );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
		memcpy( TRINP.TR_RID_LOG_PARA_ADDED, &RecvBuffer[RespDATOffset+2], sizeof( TRINP.TR_RID_LOG_PARA_ADDED ));
	}

	// Return
	return retval;
}

/*
 ** ICCard_RID_Append_Log
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 To Append Log For RID Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *
 */
unsigned short int ICCard_RID_Append_Log( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xE6;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = RecvBuffer[127];
	len += 1;
	
	memcpy( pBuffer + len, &RecvBuffer[128], RecvBuffer[127] );
	len += RecvBuffer[127];
	

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** CR_ICCard_RID_Append_Log
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Append Log To RID Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The ACT Command Excuted
 *
 *
 */
unsigned short int CR_ICCard_RID_Append_Log( void )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x16 Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_RID_Append_Log );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
//		memcpy( TRINP.TR_RID_LOG_PARA, &RecvBuffer[RespDATOffset+2], sizeof( TRINP.TR_RID_LOG_PARA ));
	}

	// Return
	return retval;
}






#endif


/*
 ** ICCard_Append_Log
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 To Append Log To Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *
 */
unsigned short int ICCard_Append_Log( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xE6;
	len += 1;
	
	// Para1;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = RecvBuffer[127];
	len += 1;
	
	memcpy( pBuffer + len, &RecvBuffer[128], RecvBuffer[127] );
	len += RecvBuffer[127];
	

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** CR_ICCard_Append_Log
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Append Log To Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The ACT Command Excuted
 *
 *
 */
unsigned short int CR_ICCard_Append_Log( void )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Read SFI 0x16 Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Append_Log );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
	
//		memcpy( TRINP.TR_RID_LOG_PARA, &RecvBuffer[RespDATOffset+2], sizeof( TRINP.TR_RID_LOG_PARA ));
	}

	// Return
	return retval;
}




/*
 ** CR_Poll_TypeA
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Poll Type A Contactless IC Card
 *
 *  RETURNS: 0x9000: Command Excute OK; 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFD: Response Check BCC Error;
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *
 *	Note: The Excute Result Is Saved In RecvBuffer[RespDATOffset]
 *
 */
unsigned short int CR_Poll_TypeA( void )
{
	unsigned short int retval;
	unsigned short int ATRLen;
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

		// IC Card Cold Reset Command
		memcpy( &SendBuffer[SendLen], CR_POLL_TYPE_A, sizeof( CR_POLL_TYPE_A ));
		SendLen += sizeof( CR_POLL_TYPE_A );

		// Fill Command Length
		ShortIntToCharCopy( SendLen - 3, &SendBuffer[1] );

		// ETX
		SendBuffer[SendLen]	= CR_ETX;
		SendLen += 1;

		// BCC
		SendBuffer[SendLen]	= CR_Calc_BCC( SendBuffer, SendLen );
		SendLen += 1;

#ifdef MAKE_CASH_CR
		// For Cash Card, Use PSAM4 As IC Card2
		if( CRType == CR_TYPE_CASH )
		{
			// Define The Response Running Flag Byte Offset
			RespCMDOffset = 0x06;

			// Define The Response Data Byte Offset
			RespDATOffset = 0x07;
		}
		else
#endif

		{
			// Define The Response Running Flag Byte Offset
			RespCMDOffset = 0x05;

			// Define The Response Data Byte Offset
			RespDATOffset = 0x06;
		}
	}
	
	// Send The Command
	CR_Command( SendBuffer, SendLen );

	if( CR_CommandStatus == CR_CommandResponseReceived )
	{
		if( RecvBuffer[RespCMDOffset] == CR_RESPONSE_YES )
		{
			ATRLen = CharToShortIntCopy( &RecvBuffer[1] );
			memcpy( TRINP.TR_CARD_ATR, &RecvBuffer[RespDATOffset], ATRLen - 3 );

			// Reset Ok
			retval = 0x9000;
		}
		else if( RecvBuffer[RespCMDOffset] == CR_RESPONSE_NO )
		{
			// Reset Error
			retval = 3;
		}
		else
		{
			// No IC Card
			retval = 2;
		}
		
		CR_CommandStatus = CR_CommandSending;
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
 ** CR_Poll_TypeB
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Poll Type B Contactless IC Card
 *
 *  RETURNS: 0x9000: Command Excute OK; 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFD: Response Check BCC Error;
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *
 *	Note: The Excute Result Is Saved In RecvBuffer[RespDATOffset]
 *
 */
unsigned short int CR_Poll_TypeB( void )
{
	unsigned short int retval;
	unsigned short int ATRLen;
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

		// IC Card Cold Reset Command
		memcpy( &SendBuffer[SendLen], CR_POLL_TYPE_B, sizeof( CR_POLL_TYPE_B ));
		SendLen += sizeof( CR_POLL_TYPE_B );

		// Fill Command Length
		ShortIntToCharCopy( SendLen - 3, &SendBuffer[1] );

		// ETX
		SendBuffer[SendLen]	= CR_ETX;
		SendLen += 1;

		// BCC
		SendBuffer[SendLen]	= CR_Calc_BCC( SendBuffer, SendLen );
		SendLen += 1;

#ifdef MAKE_CASH_CR
		// For Cash Card, Use PSAM4 As IC Card2
		if( CRType == CR_TYPE_CASH )
		{
			// Define The Response Running Flag Byte Offset
			RespCMDOffset = 0x06;

			// Define The Response Data Byte Offset
			RespDATOffset = 0x07;
		}
		else
#endif

		{
			// Define The Response Running Flag Byte Offset
			RespCMDOffset = 0x05;

			// Define The Response Data Byte Offset
			RespDATOffset = 0x06;
		}
	}
	
	// Send The Command
	CR_Command( SendBuffer, SendLen );

	if( CR_CommandStatus == CR_CommandResponseReceived )
	{
		if( RecvBuffer[RespCMDOffset] == CR_RESPONSE_YES )
		{
			ATRLen = CharToShortIntCopy( &RecvBuffer[1] );
			memcpy( TRINP.TR_CARD_ATR, &RecvBuffer[RespDATOffset], ATRLen - 3 );

			// Reset Ok
			retval = 0x9000;
		}
		else if( RecvBuffer[RespCMDOffset] == CR_RESPONSE_NO )
		{
			// Reset Error
			retval = 3;
		}
		else
		{
			// No IC Card
			retval = 2;
		}
		
		CR_CommandStatus = CR_CommandSending;
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
 ** CR_Poll_CardType
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get Card Type, Specia For Intelligence Card Reader
 *
 *  RETURNS: 0x9000: Command Excute OK; 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFD: Response Check BCC Error;
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *
 *	Note: The Card Type Was Set In CardType
 *
 */
unsigned short int CR_Poll_CardType( void )
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

		// IC Card Cold Reset Command
		memcpy( &SendBuffer[SendLen], CR_GET_CARDTYPE, sizeof( CR_GET_CARDTYPE ));
		SendLen += sizeof( CR_GET_CARDTYPE );

		// Fill Command Length
		ShortIntToCharCopy( SendLen - 3, &SendBuffer[1] );

		// ETX
		SendBuffer[SendLen]	= CR_ETX;
		SendLen += 1;

		// BCC
		SendBuffer[SendLen]	= CR_Calc_BCC( SendBuffer, SendLen );
		SendLen += 1;

#ifdef MAKE_CASH_CR
		// For Cash Card, Use PSAM4 As IC Card2
		if( CRType == CR_TYPE_CASH )
		{
			// Define The Response Running Flag Byte Offset
			RespCMDOffset = 0x06;

			// Define The Response Data Byte Offset
			RespDATOffset = 0x07;
		}
		else
#endif

		{
			// Define The Response Running Flag Byte Offset
			RespCMDOffset = 0x05;

			// Define The Response Data Byte Offset
			RespDATOffset = 0x06;
		}
	}
	
	// Send The Command
	CR_Command( SendBuffer, SendLen );

	if( CR_CommandStatus == CR_CommandResponseReceived )
	{
		if(( RecvBuffer[RespCMDOffset] >= '0' ) && ( RecvBuffer[RespCMDOffset] <= '4' ))
		{
			// Save Card Type
			if( RecvBuffer[RespCMDOffset] == '1' )
			{
				// Contact CPU IC Card
				CardType = CardContact;

				// Reset Ok
				retval = 0x9000;
			}
			else if( RecvBuffer[RespCMDOffset] == '0' )
			{
				if( RecvBuffer[RespDATOffset] == '4' )
				{
					// Contactless Type A IC Card
					CardType = CardTypeA;

					// Reset Ok
					retval = 0x9000;
				}
				else if( RecvBuffer[RespDATOffset] == '5' )
				{
					// Contactless Type B IC Card
					CardType = CardTypeB;

					// Reset Ok
					retval = 0x9000;
				}
				else
				{
					// Error IC Card
					CardType = CardTypeStart;

					// Card Error
					retval = 3;
				}
			}
			else
			{
				// Error IC Card
				CardType = CardTypeStart;

				// Card Error
				retval = 3;
			}
		}
		else if( RecvBuffer[RespCMDOffset] == CR_RESPONSE_NO )
		{
			// Reset Error
			retval = 3;
		}
		else
		{
			// No IC Card
			retval = 2;
		}
		
		CR_CommandStatus = CR_CommandSending;
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

