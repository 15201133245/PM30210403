
#include <string.h>
#include "PSAMCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "Define.h"
#include "Pdprotect.h"
#include "global.h"

//Douzhq 20140610 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
#include "SocialCard.h"
#endif

// For PSAM
// PSE: 1PAY.SYS.DDF01
static const unsigned char PSE[14] = { "1PAY.SYS.DDF01" };
// SINOPEC1 APPLICATION: 0A0H,00H,00H,00H,03H,'SINOPEC1'
static const unsigned char SINOPEC_APP1[13] = { 0xA0, 0x00, 0x00, 0x00, 0x03, 'S', 'I', 'N', 'O', 'P', 'E', 'C', '1' };
// SINOPEC1 APPLICATION: 0A0H,00H,00H,00H,03H,'SINOPEC2'
static const unsigned char SINOPEC_APP2[13] = { 0xA0, 0x00, 0x00, 0x00, 0x03, 'S', 'I', 'N', 'O', 'P', 'E', 'C', '2' };






/*
 ** PSAM1_Select_PSE
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Select PSAM1 PSE
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int PSAM_Select_PSE( unsigned char *pBuffer )
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
	*( pBuffer + len ) = sizeof( PSE );
	len += 1;
	
	memcpy( pBuffer + len, PSE, sizeof( PSE ));
	len += sizeof( PSE );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** PSAM_Select_Sinopec1
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Select PSAM1 SINOPEC1
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int PSAM_Select_Sinopec1( unsigned char *pBuffer )
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
	*( pBuffer + len ) = sizeof( SINOPEC_APP1 );
	len += 1;
	
	memcpy( pBuffer + len, SINOPEC_APP1, sizeof( SINOPEC_APP1 ));
	len += sizeof( SINOPEC_APP1 );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** PSAM_Select_Sinopec2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Select PSAM1 SINOPEC2
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int PSAM_Select_Sinopec2( unsigned char *pBuffer )
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
	*( pBuffer + len ) = sizeof( SINOPEC_APP2 );
	len += 1;
	
	memcpy( pBuffer + len, SINOPEC_APP2, sizeof( SINOPEC_APP2 ));
	len += sizeof( SINOPEC_APP2 );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** PSAM_Init_Grey_Lock
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 PSAM Initiate Grey Lock
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int PSAM_Init_Grey_Lock( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0xE0;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x40;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x24;
	len += 1;
	
	// Data, Card Random, 4
	memcpy( pBuffer + len, TRINP.TR_CARD_RANDOM, sizeof( TRINP.TR_CARD_RANDOM ));
	len += sizeof( TRINP.TR_CARD_RANDOM );

	// Data, Card Application TTC, 2
	memcpy( pBuffer + len, TRINP.TR_CARD_APP_TTC, sizeof( TRINP.TR_CARD_APP_TTC ));
	len += sizeof( TRINP.TR_CARD_APP_TTC );

	// Data, Card Application Balance, 4
	memcpy( pBuffer + len, TRINP.TR_CARD_APP_BALANCE, sizeof( TRINP.TR_CARD_APP_BALANCE ));
	len += sizeof( TRINP.TR_CARD_APP_BALANCE );

	// Data, Algorithm Flag, 1
	*( pBuffer + len ) = 0x91;
	len += 1;
	
	// Data, Transaction Date, 4
	memcpy( pBuffer + len, TRINP.TR_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));
	len += sizeof( TRINP.TR_TRANS_DATE );

	// Data, Transaction Time, 3
	memcpy( pBuffer + len, TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));
	len += sizeof( TRINP.TR_TRANS_TIME );

	// Data, IC Card Key Version, 1
	*( pBuffer + len ) = TRINP.TR_CARD_KEY_VERSION;
	len += 1;
	
	// Data, IC Card Algorithm Flag, 1
	*( pBuffer + len ) = TRINP.TR_CARD_ALGORITHM_FLAG;
	len += 1;
	
	// Data, Right 8 Bytes Of IC Card ASN, 8
	memcpy( pBuffer + len, &TRINP.TR_CARD_ASN[2], ( sizeof( TRINP.TR_CARD_ASN ) - 2 ));
	len += ( sizeof( TRINP.TR_CARD_ASN ) - 2 );

	// Data, IC Card Issuer ID, 8
	memcpy( pBuffer + len, TRINP.TR_CARD_ISSUER_ID, sizeof( TRINP.TR_CARD_ISSUER_ID ));
	len += sizeof( TRINP.TR_CARD_ISSUER_ID );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** PSAM_Certificate_Grey_Lock
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 PSAM Certificate Grey Lock
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 * Note: Attention: Call Get_Lock_Proof() Before This Command
 *
 */
unsigned short int PSAM_Certificate_Grey_Lock( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0xE0;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x42;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x04;
	len += 1;
	
	// Data, Card MAC2, 4
	memcpy( pBuffer + len, TRINP.TR_CARD_MAC2, sizeof( TRINP.TR_CARD_MAC2 ));
	len += sizeof( TRINP.TR_CARD_MAC2 );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** PSAM_Credit_Grey_Debit
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Credit PSAM For Grey Lock Debit
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 * Note: Attention: Call Get_Lock_Proof() Before This Command
 *
 */
unsigned short int PSAM_Credit_Grey_Debit( unsigned char *pBuffer )
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
	*( pBuffer + len ) = 0x44;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x0F;
	len += 1;
	
	// Trans Type;
	*( pBuffer + len ) = 0x93;
	len += 1;
	
	// Data, Right 8 Bytes Of IC Card ASN, 8
	memcpy( pBuffer + len, &TRINP.TR_CARD_ASN[2], ( sizeof( TRINP.TR_CARD_ASN ) - 2 ));
	len += ( sizeof( TRINP.TR_CARD_ASN ) - 2 );

	// Data, Card TTC, 2
	memcpy( pBuffer + len, TRINP.TR_CARD_APP_TTC, sizeof( TRINP.TR_CARD_APP_TTC ));
	len += sizeof( TRINP.TR_CARD_APP_TTC );

#ifdef MAKE_CARD_MODE2
//	// For Sinopec Mode2, Use TransAmount1 Instead of TransAmount
//	if( TRINP.TR_WORK_MODE == 0x01 )
//	{
//		// Online Mode, Uset TransAmount1
//		// Data, Transaction Amount, 4
		memcpy( pBuffer + len, TRINP.TR_TRANS_AMOUNT_T1, sizeof( TRINP.TR_TRANS_AMOUNT_T1 ));
		len += sizeof( TRINP.TR_TRANS_AMOUNT_T1 );
//	}
//	else
//	{
//		// Offline Mode, Uset TransAmount
//		// Data, Transaction Amount, 4
//		memcpy( pBuffer + len, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
//		len += sizeof( TRINP.TR_TRANS_AMOUNT );
//	}
#else
	// Data, Transaction Amount, 4
		
//IC Card Cut Payment
//Douzhq 20140610 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (TRINP.TR_TRANS_DEBITSOURCE == 0x04 && (PowerDown.SaveOilAppMode == 0x01))
	{
		memcpy( pBuffer + len, TRINP.TR_TRANS_VOLUME, sizeof( TRINP.TR_TRANS_VOLUME ));
		len += sizeof( TRINP.TR_TRANS_VOLUME );	
		
		memcpy(TRINP.TR_TRANS_AMOUNT,TRINP.TR_TRANS_VOLUME,sizeof(TRINP.TR_TRANS_VOLUME));
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
	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** PSAM_Get_GMAC
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Get The GMAC From PSAM
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 * Note: Attention: Call Get_Lock_Proof() Before This Command
 *
 */
unsigned short int PSAM_Get_GMAC( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0xE0;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x46;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x04;
	len += 1;
	
	// Data, Transaction Amount, 4
	memcpy( pBuffer + len, TRINP.TR_PSAM_TTC, sizeof( TRINP.TR_PSAM_TTC ));
	len += sizeof( TRINP.TR_PSAM_TTC );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}


/*
 ** PSAM_Credit_MAC2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Credit PSAM MAC2 Of Debit Purchase
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 * Note: Attention: 
 *
 */
unsigned short int PSAM_Credit_MAC2( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x72;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x04;
	len += 1;
	
	// Data, Transaction IC Card MAC2, 4
	memcpy( pBuffer + len, TRINP.TR_CARD_MAC2, sizeof( TRINP.TR_CARD_MAC2 ));
	len += sizeof( TRINP.TR_CARD_MAC2 );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** PSAM_Init_Purchase
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 PSAM Initiate Purchase
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 */
unsigned short int PSAM_Init_Purchase( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x70;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x24;
	len += 1;
	
	// Data, Card Random, 4
	memcpy( pBuffer + len, TRINP.TR_CARD_RANDOM, sizeof( TRINP.TR_CARD_RANDOM ));
	len += sizeof( TRINP.TR_CARD_RANDOM );

	// Data, Card Application TTC, 2
	memcpy( pBuffer + len, TRINP.TR_CARD_APP_TTC, sizeof( TRINP.TR_CARD_APP_TTC ));
	len += sizeof( TRINP.TR_CARD_APP_TTC );

	// Data, Trans Amount, 4
	memcpy( pBuffer + len, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
	len += sizeof( TRINP.TR_TRANS_AMOUNT );

	// Data, Algorithm Flag, 1
	*( pBuffer + len ) = 0x05;
	len += 1;
	
	// Data, Transaction Date, 4
	memcpy( pBuffer + len, TRINP.TR_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));
	len += sizeof( TRINP.TR_TRANS_DATE );

	// Data, Transaction Time, 3
	memcpy( pBuffer + len, TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));
	len += sizeof( TRINP.TR_TRANS_TIME );

	// Data, IC Card Key Version, 1
	*( pBuffer + len ) = TRINP.TR_CARD_KEY_VERSION;
	len += 1;
	
	// Data, IC Card Algorithm Flag, 1
	*( pBuffer + len ) = TRINP.TR_CARD_ALGORITHM_FLAG;
	len += 1;
	
	// Data, Right 8 Bytes Of IC Card ASN, 8
	memcpy( pBuffer + len, &TRINP.TR_CARD_ASN[2], ( sizeof( TRINP.TR_CARD_ASN ) - 2 ));
	len += ( sizeof( TRINP.TR_CARD_ASN ) - 2 );

	// Data, IC Card Issuer ID, 8
	memcpy( pBuffer + len, TRINP.TR_CARD_ISSUER_ID, sizeof( TRINP.TR_CARD_ISSUER_ID ));
	len += sizeof( TRINP.TR_CARD_ISSUER_ID );

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}




/*
 ** PSAM_Init_DES_Encrypt
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Initialize The PSAM DES Encrypt For Calc The Record TAC
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 * Note: Attention: Select Sinopec2 Application Before
 *
 */
unsigned short int PSAM_Init_DES_Encrypt( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x1A;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x08;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x01;
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
 ** PSAM_DES_Encrypt0
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 PSAM DES Encrypt For Calc The Record TAC
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 * Note: All Data Should Be Calc In One Data Packge,
 *		 If Return 0x6700: Means Data Too Long.
 *		 Then, Should Seperate The Data In 2 Buffers
 *		 Call CR_PSAM_DES_Encrypt1 To Calc The 1st Buffer And
 *		 CR_PSAM_DES_Encrypt2, For The 2nd Buffer
 *
 *		 RecvBuffer + 127: Save The Data Length
 *		 The Data Buffer Starts From RecvBuffer + 128
 *		 Max Length Is 120
 *
 */
unsigned short int PSAM_DES_Encrypt0( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xFA;
	len += 1;
	
	// Para1 = 0x04;
	if( CR_MACType == CR_MAC_TYPE_ENC )
	{
		// Encrypt 8Bytes Data
		*( pBuffer + len ) = 0x00;
		len += 1;
	}
	else
	{
		// Calc MAC
		*( pBuffer + len ) = 0x05;
		len += 1;
	}

	// Para2 = 0x00;
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
 ** PSAM_DES_Encrypt1
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 PSAM DES Encrypt For Calc The Record TAC
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 * Note: Combine Used With CR_PSAM_DES_Encrypt2
 *		 This Is The First Buffer In 2 Buffers To Calc The MAC
 *		 Call CR_PSAM_DES_Encrypt1 To Calc The 1st Buffer And
 *		 CR_PSAM_DES_Encrypt2, For The 2nd Buffer
 *
 *		 RecvBuffer + 127: Save The Data Length
 *		 The Data Buffer Starts From RecvBuffer + 128
 *		 The Leading 8 Bytes Is Init Value
 *		 Max Length Is 120
 *
 */
unsigned short int PSAM_DES_Encrypt1( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xFA;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x07;
	len += 1;
	
	// Para2 = 0x00;
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
 ** PSAM_DES_Encrypt2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 PSAM DES Encrypt For Calc The Record TAC
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 * Note: Combine Used With CR_PSAM_DES_Encrypt1
 *		 This Is The Second Buffer In 2 Buffers To Calc The MAC
 *		 Call CR_PSAM_DES_Encrypt1 To Calc The 1st Buffer And
 *		 CR_PSAM_DES_Encrypt2, For The 2nd Buffer
 *
 *		 RecvBuffer + 127: Save The Data Length
 *		 The Data Buffer Starts From RecvBuffer + 128
 *		 Max Length Is 120
 *
 */
unsigned short int PSAM_DES_Encrypt2( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xFA;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x01;
	len += 1;
	
	// Para2 = 0x00;
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




#ifdef MAKE_PSAM_VERIFY


/*
 ** PSAM_Get_Anti_Fraud_Proof
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Get PSAM AntiFraud Proof Command
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *
 */
unsigned short int PSAM_Get_Anti_Fraud_Proof( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xA2;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x01;
	len += 1;
	
	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** PSAM_Get_Random
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Get 4 Bytes PSAM Random Command
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *
 */
unsigned short int PSAM_Get_Random( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x84;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
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
 ** PSAM_Start_Binding
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Start Binding For ACT Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *
 */
unsigned short int PSAM_Start_Binding( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xA6;
	len += 1;
	
	// Para1 = Key Index;
	*( pBuffer + len ) = TRINP.TR_ACT_KEY_INDEX;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x10;
	len += 1;
	
	// Data
	memcpy( pBuffer + len, &TRINP.TR_CARD_ASN[2], 8 );
	len += 8;
	memcpy( pBuffer + len, TRINP.TR_ACT_ENCRYPTED, 8 );
	len += 8;


	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** PSAM_Init_Binding
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Initialize Binding For RID Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *
 */
unsigned short int PSAM_Init_Binding( unsigned char *pBuffer )
{
	unsigned short int len;
	unsigned char buf[8];

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xAC;
	len += 1;
	
	// Para1 = Key Index;
	*( pBuffer + len ) = TRINP.TR_RID_KEY_INDEX;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x18;
	len += 1;
	
	// Data
	memcpy( pBuffer + len, &TRINP.TR_RID_CARD_ASN[2], 8 );
	len += 8;
	
	memset( buf, 0xFF, sizeof( buf ));
	memcpy( buf, GilbarcoID, sizeof( GilbarcoID ));
	memcpy( pBuffer + len, buf, 8 );
	len += 8;
	
	memcpy( pBuffer + len, TRINP.TR_RID_ENCRYPTED, 8 );
	len += 8;


	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

/*
 ** PSAM_Binding
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Binding For RID Card
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 *
 */
unsigned short int PSAM_Binding( unsigned char *pBuffer )
{
	unsigned short int len;
	unsigned char buf[8];

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x84;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xAA;
	len += 1;
	
	// Para1 = Key Index;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x10;
	len += 1;
	
	// Data
	memcpy( pBuffer + len, Pump_CR_Buf.PUMPSerialNumber, 8 );
	len += 8;

	memset( buf, 0xFF, sizeof( buf ));
	memcpy( buf, GilbarcoID, sizeof( GilbarcoID ));
	memcpy( pBuffer + len, buf, 8 );
	len += 8;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}


/*
 ** PSAM_Init_DES_Encrypt_RID_Log
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 Initialize The PSAM DES Encrypt For Calc Append RID Log
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 * Note: Attention: Select Sinopec2 Application Before
 *
 */
unsigned short int PSAM_Init_DES_Encrypt_RID_Log( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0x1A;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x08;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = TRINP.TR_RID_KEY_VERSION;
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
 ** PSAM_Anti_Fraud_Verify
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	pBuffer: Destination Buffer Pointer, Where To Save The Command Data
 *
 *  DESCRIPTION: Fill Destination Buffer With The IC Card Command
 *				 PSAM Verify The Encrypted Data From Pump
 *
 *  RETURNS: Data Length,
 *			 First 2 Bytes Of pBuffer Is Command Data Length, Same As Return Value
 *
 * Note: Attention: Select Sinopec2 Application Before
 *
 */
unsigned short int PSAM_Anti_Fraud_Verify( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x80;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xAE;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// Para3 = Data Length;
	*( pBuffer + len ) = 0x08;
	len += 1;
	
	// Data
	memcpy( pBuffer + len, Pump_CR_Buf.DESRandom, 8 );
	len += 8;

	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}




#endif



/*
 ** PSAM_Get_Response
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\ICCardCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location, PSAM1, PSAM2, PSAM3 Or PSAM4
 *				DataLen: The Length Need To Read
 *
 *  DESCRIPTION: PSAM Get Response
 *
 *  RETURNS: None.
 *
 * Note: Return To The Command Which Need To Get The Response
 *
 */
unsigned short int PSAM_Get_Response( unsigned char PSAM_Location, unsigned char DataLen )
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

	// Card Reader PSAM Command
	memcpy( &SendBuffer[SendLen], PSAM_CAPDU_COMMAND_T0, sizeof( PSAM_CAPDU_COMMAND_T0 ));
	SendLen += sizeof( PSAM_CAPDU_COMMAND_T0 );

	SendBuffer[SendLen] = PSAM_Location;
	SendLen += 1;
		
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
 ** CR_PSAM_ColdReset
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Voltage: PSAM Voltage, 5.0V Or 3.0V
 *				PSAM_Location: PSAM1, PSAM2, PSAM3 Or PSAM4
 *				PSAM_Mode: 7816 Mode Or EMV Mode
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
 *	Note: The Excute Result Is Saved In RecvBuffer[6]
 *
 */
unsigned short int CR_PSAM_ColdReset( unsigned char PSAM_Voltage, unsigned char PSAM_Location, unsigned char PSAM_Mode )
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

		// PSAM Command Leading Charactor
		SendBuffer[SendLen]	= PSAM_COMMAND_LEADING;
		SendLen += 1;

		// PSAM Voltage
		SendBuffer[SendLen]	= PSAM_Voltage;
		SendLen += 1;

		// PSAM Location
		SendBuffer[SendLen]	= PSAM_Location;
		SendLen += 1;

		// PSAM Mode
		SendBuffer[SendLen]	= PSAM_Mode;
		SendLen += 1;

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
		if( RecvBuffer[6] == CR_RESPONSE_YES )
		{
			ATRLen = CharToShortIntCopy( &RecvBuffer[7] );
			memcpy( TRINP.TR_PSAM_ATR, &RecvBuffer[9], ATRLen );

			// Reset The Error Check Times
			CommErrorTimes = 0;

//			retval = 0;
			retval = 0x9000;
		}
		else if( RecvBuffer[6] == CR_RESPONSE_NO )
		{
			retval = 3;
		}
		else
		{
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
 ** CR_PSAM1_ColdReset_7816_50V
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: Cold Reset PSAM In ISO7816 5.0V Mode
 *
// *  RETURNS: 0: Command Excute OK; 1: Command Is Excuting;
// *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
// *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *  RETURNS: 0x9000: Command Excute OK; 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Save The ATR In TRINP.TR_PSAM_ATR
 *
 */
unsigned short int CR_PSAM1_ColdReset_7816_50V( void )
{
	unsigned short int retval;
	
	// Excute The Command
	retval = CR_PSAM_ColdReset( PSAM_50V, PSAM1, MODE_7816 );

	// Return
	return retval;
}






/*
 ** CR_CAPDU_Command_T0_PSAM
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	*PSAM_COMMAND( unsigned char * ): PSAM Command Data Function, Such As PSAM_Select_PSE
 *					PSAM_Select_Sinopec1 etc.
 *				PSAM_Location: The PSAM Locate, PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: Excute The PSAM Command
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFD: Response Check BCC Error;
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 *	Note: The Excute Result Is Saved In RecvBuffer[6]
 *
 */
unsigned short int CR_CAPDU_Command_T0_PSAM( unsigned short int ( *PSAM_COMMAND )( unsigned char * ), unsigned char PSAM_Location )
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

		// Card Reader PSAM Command
		memcpy( &SendBuffer[SendLen], PSAM_CAPDU_COMMAND_T0, sizeof( PSAM_CAPDU_COMMAND_T0 ));
		SendLen += sizeof( PSAM_CAPDU_COMMAND_T0 );

		SendBuffer[SendLen] = PSAM_Location;
		SendLen += 1;
		
		// PSAM Command
		CommandLen = PSAM_COMMAND( &SendBuffer[SendLen] );
		SendLen += CommandLen;

		
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
		if( RecvBuffer[6] == CR_RESPONSE_YES )
		{
			// Save SW1/SW2
			retval = CharToShortIntCopy( &RecvBuffer[RecvBufferLen - 4] );
		
			// Check Need Get Response Or Not
			if(( retval / 0x100 ) == 0x61 )
			{
				// Need Get Response
				SendLen = PSAM_Get_Response( PSAM_Location, ( retval % 0x100 ));
				
				// Continue Excuting
				retval = 1;

				// Send The Command
				CR_CommandStatus = CR_CommandSending;
				CR_Command( SendBuffer, SendLen );
			}
			else
			{
				CR_CommandStatus = CR_CommandSending;
			}
		}
		else if( RecvBuffer[6] == CR_RESPONSE_NO )
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
 ** CR_PSAM_Select_PSE
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: PSAM Select PSE "1PAY.SYS.DDF01"
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_PSAM_Select_PSE( unsigned char PSAM_Location )
{
	unsigned short int retval;
	unsigned short int ResponseLen;
	unsigned short int FCIPosition;

	// Default Excuting
	retval = 1;

	// Select PSE Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Select_PSE, PSAM_Location );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[7] );
		
		// Find FCI 9F0C First
		for( FCIPosition = 9; FCIPosition < ( ResponseLen + 9 ); FCIPosition++ )
		{
			if(( RecvBuffer[FCIPosition] == 0x9F ) && ( RecvBuffer[FCIPosition+1] == 0x0C ))
			{
				break;
			}
		}

		// Found Or Not?
		if( FCIPosition == ( ResponseLen + 9 ))
		{
			// FCI Error
			retval = 3;
		}
		else
		{
			// Found, Save The Contents
			memcpy( TRINP.TR_PSAM_ASN, &RecvBuffer[FCIPosition+3], RecvBuffer[FCIPosition+2] );
		}
	}

	// Return
	return retval;
}


/*
 ** CR_PSAM_Select_Sinopec1
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: PSAM Select Sinopec1 Application "SINOPEC_APP1[13] = { 0xA0, 0x00, 0x00, 0x00, 0x03, 'SINOPEC1' }"
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_PSAM_Select_Sinopec1( unsigned char PSAM_Location )
{
	unsigned short int retval;
	unsigned short int ResponseLen;
	unsigned short int FCIPosition;

	// Default Excuting
	retval = 1;

	// Select Sinopec1 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Select_Sinopec1, PSAM_Location );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[7] );
		
		// Find FCI 9F0C First
		for( FCIPosition = 9; FCIPosition < ( ResponseLen + 9 ); FCIPosition++ )
		{
			if(( RecvBuffer[FCIPosition] == 0x9F ) && ( RecvBuffer[FCIPosition+1] == 0x0C ))
			{
				break;
			}
		}

		// Found Or Not?
		if( FCIPosition == ( ResponseLen + 9 ))
		{
			// FCI Error
			retval = 3;
		}
		else
		{
			// Found, Save The Contents
			memcpy( &TRINP.TR_PSAM_KEY_INDEX, &RecvBuffer[FCIPosition+3], RecvBuffer[FCIPosition+2] );
		}
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_Select_Sinopec2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: PSAM Select Sinopec1 Application "SINOPEC_APP1[13] = { 0xA0, 0x00, 0x00, 0x00, 0x03, 'SINOPEC2' }"
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 */
unsigned short int CR_PSAM_Select_Sinopec2( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;
//	unsigned short int FCIPosition;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Select_Sinopec2, PSAM_Location );

	if( retval == 0x9000 )
	{
//		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[7] );
//		
//		// Find FCI 9F0C First
//		for( FCIPosition = 9; FCIPosition < ( ResponseLen + 9 ); FCIPosition++ )
//		{
//			if(( RecvBuffer[FCIPosition] == 0x9F ) && ( RecvBuffer[FCIPosition+1] == 0x0C ))
//			{
//				break;
//			}
//		}
//
//		// Found Or Not?
//		if( FCIPosition == ( ResponseLen + 9 ))
//		{
//			// FCI Error
//			retval = 3;
//		}
//		else
//		{
//			// Save Nothing For Sinopec2, Only Mark Sinopec2 Ok In Future
//			retval = retval;
//		}
		// Do Not Check FCI, For Sinopec2 Application
		retval = retval;
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_Init_Grey_Lock
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: PSAM Initiate The Grey Lock
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 *	Note:	 The Response Data Was Saved In TRINP, If Success
 */
unsigned short int CR_PSAM_Init_Grey_Lock( unsigned char PSAM_Location )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Init_Grey_Lock, PSAM_Location );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[7] );
	
		// Save PSAM TTC, PSAM RANDOM, And PSAM MAC1
		memcpy( TRINP.TR_PSAM_TTC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_Certificate_Grey_Lock
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: PSAM Certificate The Grey Lock
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 *	Note:	 The Response Data Was Saved In TRINP, If Success
 */
unsigned short int CR_PSAM_Certificate_Grey_Lock( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Certificate_Grey_Lock, PSAM_Location );

	if( retval == 0x9000 )
	{
//		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[7] );
//	
//		// Save PSAM TTC, PSAM RANDOM, And PSAM MAC1
//		memcpy( TRINP.TR_PSAM_TTC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_Credit_Grey_Debit
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: Credit PSAM For The Grey Lock Debit
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 *	Note:	 The Response Data Was Saved In TRINP, If Success
 */
unsigned short int CR_PSAM_Credit_Grey_Debit( unsigned char PSAM_Location )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Credit_Grey_Debit, PSAM_Location );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[7] );
	
		// Save PSAM GMAC, PSAM TAC
		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_Get_GMAC
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: Read The GMAC From PSAM
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 *	Note:	 The Response Data Was Saved In TRINP, If Success
 */
unsigned short int CR_PSAM_Get_GMAC( unsigned char PSAM_Location )
{
	unsigned short int retval;
	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Get_GMAC, PSAM_Location );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
		ResponseLen = CharToShortIntCopy( &RecvBuffer[7] );
	
		// Save PSAM GMAC, PSAM TAC
		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}



/*
 ** CR_PSAM_Credit_MAC2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: Credit PSAM For The Debit Purchase MAC2
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 *	Note:	 The Response Data Was Saved In TRINP, If Success
 */
unsigned short int CR_PSAM_Credit_MAC2( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Credit_MAC2, PSAM_Location );

	if( retval == 0x9000 )
	{
		// No Response
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_Init_Purchase
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: PSAM Initiate Purchase
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 *	Note:	 The Response Data Was Saved In TRINP, If Success
 */
unsigned short int CR_PSAM_Init_Purchase( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Init_Purchase, PSAM_Location );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[7] );
	
		// Save PSAM TTC And PSAM MAC1
		memcpy( TRINP.TR_PSAM_TTC, &RecvBuffer[9], 4 );
		memcpy( TRINP.TR_PSAM_MAC1, &RecvBuffer[9+4], 4 );
	}

	// Return
	return retval;
}



/*
 ** CR_PSAM_Init_DES_Encrypt
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: Initiate PSAM For DES Encrypt Function
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 * Note: Attention: Select Sinopec2 Application Before
 */
unsigned short int CR_PSAM_Init_DES_Encrypt( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Init_DES_Encrypt, PSAM_Location );

	if( retval == 0x9000 )
	{
//		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[7] );
//	
//		// Save PSAM GMAC, PSAM TAC
//		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_DES_Encrypt0
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: PSAM For DES Encrypt Function
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 * Note: All Data Should Be Calc In One Data Packge,
 *		 If Return 0x6700: Means Data Too Long.
 *		 Then, Should Seperate The Data In 2 Buffers
 *		 Call CR_PSAM_DES_Encrypt1 To Calc The 1st Buffer And
 *		 CR_PSAM_DES_Encrypt2, For The 2nd Buffer
 *
 *		 RecvBuffer + 127: Save The Data Length
 *		 The Data Buffer Starts From RecvBuffer + 128
 *		 Max Length Is 120
 *
 */
unsigned short int CR_PSAM_DES_Encrypt0( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_DES_Encrypt0, PSAM_Location );

	if( retval == 0x9000 )
	{
//		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[7] );
//	
//		// Save PSAM GMAC, PSAM TAC
//		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_DES_Encrypt1
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: PSAM For DES Encrypt Function
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 * Note: Combine Used With CR_PSAM_DES_Encrypt2
 *		 This Is The First Buffer In 2 Buffers To Calc The MAC
 *		 Call CR_PSAM_DES_Encrypt1 To Calc The 1st Buffer And
 *		 CR_PSAM_DES_Encrypt2, For The 2nd Buffer
 *
 *		 RecvBuffer + 127: Save The Data Length
 *		 The Data Buffer Starts From RecvBuffer + 128
 *		 The Leading 8 Bytes Is Init Value
 *		 Max Length Is 120
 *
 */
unsigned short int CR_PSAM_DES_Encrypt1( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_DES_Encrypt1, PSAM_Location );

	if( retval == 0x9000 )
	{
//		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[7] );
//	
//		// Save PSAM GMAC, PSAM TAC
//		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_DES_Encrypt2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: PSAM For DES Encrypt Function
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 * Note: Combine Used With CR_PSAM_DES_Encrypt1
 *		 This Is The Second Buffer In 2 Buffers To Calc The MAC
 *		 Call CR_PSAM_DES_Encrypt1 To Calc The 1st Buffer And
 *		 CR_PSAM_DES_Encrypt2, For The 2nd Buffer
 *
 *		 RecvBuffer + 127: Save The Data Length
 *		 The Data Buffer Starts From RecvBuffer + 128
 *		 Max Length Is 120
 *
 */
unsigned short int CR_PSAM_DES_Encrypt2( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_DES_Encrypt2, PSAM_Location );

	if( retval == 0x9000 )
	{
//		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[7] );
//	
//		// Save PSAM GMAC, PSAM TAC
//		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}


#ifdef MAKE_PSAM_VERIFY

/*
 ** CR_PSAM_Get_Anti_Fraud_Proof
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: Get PSAM AntiFraud Proof
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 * Note: Attention: Select Sinopec2 Application Before
 */
unsigned short int CR_PSAM_Get_Anti_Fraud_Proof( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Get_Anti_Fraud_Proof, PSAM_Location );

	if( retval == 0x9000 )
	{
		// Save Anti Fraud Times
		TRINP.TR_PSAM_ANTIFRAUD_TIMES = RecvBuffer[9];

//		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_Get_Random
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: Get PSAM 4 Bytes Random
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 * Note: Attention: 
 */
unsigned short int CR_PSAM_Get_Random( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Get_Random, PSAM_Location );

	if( retval == 0x9000 )
	{
		// Save PSAM Random
		memcpy( TRINP.TR_PSAM_ACT_RID_RANDOM, &RecvBuffer[9], sizeof( TRINP.TR_PSAM_ACT_RID_RANDOM ));
 
//		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_Start_Binding
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: Start PSAM Binding For ACT Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 * Note: Attention: 
 */
unsigned short int CR_PSAM_Start_Binding( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Start_Binding, PSAM_Location );

	if( retval == 0x9000 )
	{
//		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_Init_Binding
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: Iniutialize PSAM Binding For RID Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 * Note: Attention: 
 */
unsigned short int CR_PSAM_Init_Binding( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Init_Binding, PSAM_Location );

	if( retval == 0x9000 )
	{
//		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_Binding
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: PSAM Binding For RID Card
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 * Note: Attention: 
 */
unsigned short int CR_PSAM_Binding( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Binding, PSAM_Location );

	if( retval == 0x9000 )
	{
//		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_Init_DES_RID_Log
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: PSAM Initialize RID Log DES MAC
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 * Note: Attention: 
 */
unsigned short int CR_PSAM_Init_DES_RID_Log( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Init_DES_Encrypt_RID_Log, PSAM_Location );

	if( retval == 0x9000 )
	{
//		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}

/*
 ** CR_PSAM_Anti_Fraud_Verify
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\PSAMCommand.c
 *
 *  PARAMETERS:	PSAM_Location: PSAM Location Number: PSAM1, PSAM2, PSAM3 Or PSAM4
 *
 *  DESCRIPTION: PSAM Verify The Encrypted Data From Pump
 *
 *  RETURNS: 1: Command Is Excuting;
 *			 2: No Card In Card Reader For The Command;	3: Command Excute Error
 *			 0xFE: ACK TimeOut; 0xFF: Response TimeOut
 *			 Otherwise: Return The SW1/SW2 Of The PSAM Command Excuted
 *
 * Note: Attention: 
 */
unsigned short int CR_PSAM_Anti_Fraud_Verify( unsigned char PSAM_Location )
{
	unsigned short int retval;
//	unsigned short int ResponseLen;

	// Default Excuting
	retval = 1;

	// Select Sinopec2 Application Command
	retval = CR_CAPDU_Command_T0_PSAM( PSAM_Anti_Fraud_Verify, PSAM_Location );

	if( retval == 0x9000 )
	{
//		memcpy( TRINP.TR_PSAM_GMAC, &RecvBuffer[9], ResponseLen - 2 );
	}

	// Return
	return retval;
}





#endif