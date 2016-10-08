/*******************************************************************
 *
 *    DESCRIPTION: For ARM Supports LSB Data Format, 
 *					While Normal Application Uses MSB Data Format
 *					So, Change the Data Format By These Functions
 *
 *    AUTHOR:
 *
 *    HISTORY:    
 *
 *******************************************************************/

/** include files **/

/** local definitions **/

/* default settings */

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/

/** private functions **/

#include <string.h>

#include "global.h"

#include "define.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "Pdprotect.h"
#include "Message.h"
#include "WriteLog.h"
#include "SocialCard.h"





/*
 ** CharToShortIntCopy
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	pBuffer: 1st Char Pointer of the 2 Bytes Char Buffer
 *				
 *
 *  DESCRIPTION: Copy MSB 2Bytes Char To LSB Short Int
 *
 *  RETURNS: The LSB Short Int Corresponding To The 2 Bytes MSB Char.
 *
 */
short int CharToShortIntCopy( unsigned char *pBuffer )
{
    short int TmpVal, IntVal;

    memcpy( &TmpVal, pBuffer, 2 );
    memcpy( &IntVal, pBuffer, 2 );

    TmpVal <<= 8;
	TmpVal &= 0xFF00;

    IntVal >>= 8;
	IntVal &= 0x00FF;

    IntVal |= TmpVal;
    
    return IntVal;
}

/*
 ** CharToIntCopy
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	pBuffer: 1st Char Pointer of the 4 Bytes Char Buffer
 *				
 *
 *  DESCRIPTION: Copy MSB 4Bytes Char To LSB Int
 *
 *  RETURNS: The LSB Int Corresponding To The 4 Bytes MSB Char.
 *
 */
int CharToIntCopy( unsigned char *pBuffer )
{
    int TmpVal, IntVal;

    memcpy( &TmpVal, pBuffer, 4 );
    memcpy( &IntVal, pBuffer, 4 );

    // First Byte Of Int
    IntVal <<= 24;
    IntVal &= 0xFF000000;

    // Second Byte Of Int
    TmpVal <<= 8;
    TmpVal &= 0x00FF0000;
    IntVal |= TmpVal;
    IntVal &= 0xFFFF0000;

    // Third Byte Of Int
    memcpy( &TmpVal, pBuffer, 4 );
    TmpVal >>= 8;
    TmpVal &= 0x0000FF00;
    IntVal |= TmpVal;
    IntVal &= 0xFFFFFF00;

    // Forth Byte Of Int
    memcpy( &TmpVal, pBuffer, 4 );
    TmpVal >>= 24;
    TmpVal &= 0x000000FF;
    IntVal |= TmpVal;

    return IntVal;
}

/*
 ** ShortIntToCharCopy
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	pBuffer: 1st Char Pointer of the 2 Bytes Char Buffer To Be Copied
 *				IntVal: the Short Int Value
 *
 *  DESCRIPTION: Copy LSB Short Int To MSB 2Bytes Char 
 *
 *  RETURNS: None.
 *
 */
void ShortIntToCharCopy( short int IntVal, unsigned char *pBuffer )
{
    char TmpBuffer[2];
    
    memcpy( TmpBuffer, &IntVal, 2 );
    
    *pBuffer = TmpBuffer[1];
    *( pBuffer + 1 ) = TmpBuffer[0];
}

/*
 ** IntToCharCopy
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	pBuffer: 1st Char Pointer of the 4 Bytes Char Buffer To Be Copied
 *				IntVal: the Int Value
 *
 *  DESCRIPTION: Copy LSB Int To MSB 4Bytes Char 
 *
 *  RETURNS: None.
 *
 */
void IntToCharCopy( int IntVal, unsigned char *pBuffer )
{
    char TmpBuffer1[4];
    
    memcpy( TmpBuffer1, &IntVal, 4 );
    
    *pBuffer = TmpBuffer1[3];
    *( pBuffer + 1 ) =  TmpBuffer1[2];
    *( pBuffer + 2 ) =  TmpBuffer1[1];
    *( pBuffer + 3 ) =  TmpBuffer1[0];
}


/*
 ** CalcCRC16
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	pBuffer: Calc CRC Start Adress Pointer
 *				len: Length Of The Buffer For Calculated
 *
 *  DESCRIPTION: Calculate The CRC
 *
 *  RETURNS: CRC Value ( 2 Bytes )
 *
 */
unsigned short int CalcCRC16( unsigned char *pBuffer, unsigned short int len )
{
	unsigned short int CRC16, i;
	unsigned char j;

	CRC16 = 0;

	for( i = 0; i < len; i++ )
	{
		CRC16 ^= *( pBuffer + i );

		for( j = 0; j < 8; j++ )
		{
			if( CRC16 & 0x0001 )
			{
				CRC16 >>= 1;
				CRC16 ^= CRC_POLYNOMIAL;
			}
			else
			{
				 CRC16 >>= 1;
			}
		}
	}

	return CRC16;
}



/*
 ** CompareDate
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	*pDate1: Pointer To Date1 Start Buffer
 *				*pDate2: Pointer To Date2 Start Buffer
 *
 *  DESCRIPTION: Compare Date1 And Date2 
 *
 *  RETURNS: 0: Date1 = Date2; 1: Date1 > Date2 Means Date1 Is Later Than Date2
 *			 2: Date1 < Date2 Means Date1 Is Earlier Than date2
 *
 *	Note: Both Date1 And Date2 Is 4 Bytes, CCYYMMDD, MSB
 *
 */
unsigned char CompareDate( unsigned char *pDate1, unsigned char *pDate2 )
{
	unsigned char retval;
	unsigned int tmpdate1, tmpdate2;

	tmpdate1 = CharToIntCopy( pDate1 );
	tmpdate2 = CharToIntCopy( pDate2 );

	if( tmpdate1 == tmpdate2 )
	{
		retval = 0;
	}
	else if( tmpdate1 > tmpdate2 )
	{
		retval = 1;
	}
	else
	{
		retval = 2;
	}

	return retval;
}



/*
 ** FindInMem
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	unsigned char *pBlock: Start Pointer Of The Data Block,
 *								BlockLen: Lenth Of The Data Block
 *				unsigned char *pMem: Start Pointer Of The Memory Block Will Be Searched
 *								MemLen: Length Of The Memory Block
 *
 *  DESCRIPTION: Find Specific Block In A Certain Memory Block
 *
 *  RETURNS: 0: Did Not Find In The Memory; 1: The Data Block Is Found, Include In The Mem Block
 *
 *	Note: The Interval Of The Mem Block Is Same As The Data Block Length
 *
 */
unsigned char FindInMem( unsigned char *pBlock, unsigned int BlockLen, unsigned char *pMem, unsigned int MemLen )
{
	unsigned char retval;

	unsigned int Blen, Mlen;
	
	// Default Not Found
	retval = 0;

	// Only Check When MemLen >= BlockLen
	if( MemLen > BlockLen )
	{
		for( Mlen = 0; Mlen < MemLen; Mlen += BlockLen )
		{
			for( Blen = 0; Blen < BlockLen; Blen++ )
			{
				if( *( pBlock + Blen ) != ( *( pMem + Mlen + Blen )))
				{
					break;
				}
			}
			
			if( Blen == BlockLen )
			{
				// Found
				retval = 1;
				
				break;
			}
		}	
	}
	else
	{
		retval = 0;
	}

	return retval;
}


/*
 ** GetTransTime
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read The Transaction Time From SysBCDTime To TRINP.TR_TRANS_DATE &	TRINP.TR_TRANS_TIME
 *
 *  RETURNS: None.
 *
 */
void GetTransTime( void )
{
	ShortIntToCharCopy( SysBCDTime.year, TRINP.TR_TRANS_DATE );
	memcpy( &TRINP.TR_TRANS_DATE[2], &SysBCDTime.month, sizeof( TRINP.TR_TRANS_DATE ) - 2 );
	memcpy( TRINP.TR_TRANS_TIME, &SysBCDTime.hour, sizeof( TRINP.TR_TRANS_TIME ));
}
//Douzhq 20141014 Add , For Bug
//----------------------Down----------------------------------------------------
void GetTransTimeZero(unsigned char *Trans_Date,unsigned char *Trans_Time)
{
	unsigned char Trans_Date_Tmp[4];
	unsigned char Trans_Time_Tmp[3];
	
	ShortIntToCharCopy( SysBCDTime.year, Trans_Date_Tmp );
	memcpy( &Trans_Date_Tmp[2], &SysBCDTime.month, sizeof( Trans_Date_Tmp ) - 2 );
	memcpy( Trans_Time_Tmp, &SysBCDTime.hour, sizeof( Trans_Time_Tmp ));
	
	memcpy(Trans_Date,Trans_Date_Tmp,sizeof(Trans_Date_Tmp));
	memcpy(Trans_Time,Trans_Time_Tmp,sizeof(Trans_Time_Tmp));
}
//---------------------- Up ----------------------------------------------------

/*
 ** GetTransTTC
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get Transaction TTC To TRINP.TR_TRANS_TTC, And Inc The TTC
 *
 *  RETURNS: None.
 *
 */
void GetTransTTC( void )
{
	// Get TTC
	TransTTC = GetWriteTTC();
						
	if( TransTTC == 0 )
	{
		// Non Zero TTC
		TransTTC = WriteTTCInc();
	}

	IntToCharCopy( TransTTC, TRINP.TR_TRANS_TTC );
	// Inc TTC
	WriteTTCInc();

	// Write Log
	WriteTTCLog();

	// Update The Power Down Protection Message
	// TTC Must Be A Journal
	PowerDown.PdTrans.PD_TRANS_FLAG = JOURNALIT;

}

/*
 ** WriteTTCLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Write A Running Log That A TTC Is Applied
 *
 *  RETURNS: None.
 *
 */
void WriteTTCLog( void )
{
	unsigned char len;
	unsigned int ttc;

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Fill "TTC Applied"
	memcpy( &CRLogTxt[len], "申请TTC成功 ", 12 );
	len = strlen(( char * )CRLogTxt );

	// Fill " TTC"
	Hex2Asc( &CRLogTxt[len], TRINP.TR_TRANS_TTC, sizeof( TRINP.TR_TRANS_TTC ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	ttc = CharToIntCopy( TRINP.TR_TRANS_TTC );
	sprintf(( char * )&CRLogTxt[len], "%u", ttc );
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );
}

/*
 ** Hex2Asc
 *
 *  FILENAME: D:\WORK\SinoPec09\SinoPec\Software\CR.c
 *
 *  PARAMETERS:	 pDest: Destination Pointer	( ASCII )
 *				 pSource: Source Pointer ( HEX )
 *				 len: HEX Data Length
 *
 *  DESCRIPTION:  Change pSource HEX Data To ASCII
 *
 *  RETURNS: None.
 *
 */
void Hex2Asc( unsigned char *pDest, unsigned char *pSource, unsigned char len )
{
	unsigned char Source_Len;
	unsigned char i, j;

	for( Source_Len = 0; Source_Len < len; Source_Len++ )
	{
		i = *( pSource + Source_Len );
		
		// High Nibble Source Data
		j = i & 0xF0;
		j >>= 4;

		if( j < 0x0A )
		{
			// Numberic
			*( pDest + ( Source_Len * 2 )) = j + 0x30;
		}
		else
		{
			// Alpha
			*( pDest + ( Source_Len * 2 )) = j + 0x37;
		}

		// Low Nibble Source Data
		j = i & 0x0F;

		if( j < 0x0A )
		{
			// Numberic
			*( pDest + ( Source_Len * 2 + 1 )) = j + 0x30;
		}
		else
		{
			// Alpha
			*( pDest + ( Source_Len * 2 + 1 )) = j + 0x37;
		}
	}
}


/*
 ** ASC2Hex
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	pDest: Destination HEX Buffer Pointer
 *				pSource: Source ASCII Buffer Pointer
 *				len: Lenth Of HEX Buffer
 *
 *  DESCRIPTION: Combine Ascii Buffer To Hex Buffer
 *
 *  RETURNS: None.
 *
 */
void Asc2Hex( unsigned char *pDest, unsigned char *pSource, unsigned char len )
{
	unsigned int i;
	unsigned char tmp;

	memset( pDest, 0x00, len );

	for( i = 0; i < len; i++ )
	{
		// The High Nibble
		tmp = *( pSource + ( i * 2 ));
		
		if(( tmp >= 'a' ) && ( tmp <= 'f' ))
		{
//			tmp -= 'a';
			tmp -= 0x57;
		}
		else if(( tmp >= 'A' ) && ( tmp <= 'F' ))
		{
//			tmp -= 'A';
			tmp -= 0x37;
		}
		else
		{
			tmp -= '0';
		}

		tmp &= 0x0F;
		tmp <<= 4;
		tmp &= 0xF0;
		*( pDest + i ) = tmp;

		// The Low Nibble
		tmp = *( pSource + ( i * 2 ) + 1 );
		
		if(( tmp >= 'a' ) && ( tmp <= 'f' ))
		{
//			tmp -= 'a';
			tmp -= 0x57;
		}
		else if(( tmp >= 'A' ) && ( tmp <= 'F' ))
		{
//			tmp -= 'A';
			tmp -= 0x37;
		}
		else
		{
			tmp -= '0';
		}

		tmp &= 0x0F;
		*( pDest + i ) |= tmp;
	}
}


/*
 ** CRWriteLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	mID: List In enum DISPMSGID
 *
 *  DESCRIPTION: Write Log According To enum DISPMSGID
 *
 *  RETURNS: None.
 *
 */
void CRWriteLog( enum DISPMSGID mID )
{
	unsigned char len;

	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));

	len = strlen( DISPMSG[mID] );
	memcpy( CRLogTxt, DISPMSG[mID], len );

	WriteLog( PROCESS_CR, CRLogTxt );
}


/*
 ** CRWriteStepLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Write The CR Running Step Log, Include CR_PROC_STEP And CR_COMMAND_TYPE
 *
 *  RETURNS: None.
 *
 *	Atten: 
 *
 */
void CRWriteStepLog( void )
{
	unsigned char len;

	// Check Write Or Not
	if(( CR_Proc_Step == CR_ICCARD_ABSENCE_PROC ) && ( CR_Proc_Last_Step == CR_ICCARD_NONEXIST_EXTERNAL_PROC ))
	{
		// IC Card Not Exist, Return From External Proc, Check IC Card
		// Do Not Write Log
		return;
	}
	else if(( CR_Proc_Step == CR_ICCARD_NONEXIST_EXTERNAL_PROC ) && ( CR_Proc_Last_Step == CR_ICCARD_ABSENCE_PROC ))
	{
		// Check External Proc While IC Card Not Exist,
		// Do Not Write Log
		return;
	}
	else if(( CR_Proc_Step == CR_ICCARD_READY_PROC ) && ( CR_Proc_Last_Step == CR_ICCARD_EXIST_EXTERNAL_PROC ))
	{
		// IC Card Exist, Return From External Proc, If Check IC Card,
		// Do Not Write Log
		if( CR_CommandType == CR_CARDREADER_CHECK_CARD )
		{
			return;
		}
	}
	else if(( CR_Proc_Step == CR_ICCARD_EXIST_EXTERNAL_PROC ) && ( CR_Proc_Last_Step == CR_ICCARD_READY_PROC ))
	{
		// IC Card Exist, And Check External Proc,
		// Do Not Write Log
		return;
	}
	else if(( CR_Proc_Step == CR_ICCARD_FUELLING_PROC ) && ( CR_Proc_Last_Step == CR_ICCARD_BUSY_EXTERNAL_PROC ))
	{
		// IC Card Busy, Return From External Proc, If Check IC Card,
		// Do Not Write Log
		if( CR_CommandType == CR_CARDREADER_CHECK_CARD )
		{
			return;
		}
	}
	else if(( CR_Proc_Step == CR_ICCARD_BUSY_EXTERNAL_PROC ) && ( CR_Proc_Last_Step == CR_ICCARD_FUELLING_PROC ))
	{
		// IC Card Exist, And Check External Proc,
		// Do Not Write Log
		return;
	}
	else
	{
		// Write Step Log
		len = strlen(( char * )CRStepLog );

		if( len == 0 )
		{
			// New Log Text
			memcpy( CRStepLog, "CRSTEP: ", 8 );
			len = strlen(( char * )CRStepLog );
		}

		// Save CR_PROC_STEP And CR_COMMAND_TYPE
		sprintf(( char * )&CRStepLog[len], "%02X", ( char )CR_Proc_Step );
		len = strlen(( char * )CRStepLog );
		sprintf(( char * )&CRStepLog[len], "%02X ", ( char )CR_CommandType );
		len = strlen(( char * )CRStepLog );

		len = strlen(( char * )CRStepLog );
		// Check Write Or Not
		if(( len + 5 ) > ( sizeof( CRStepLog )))
		{
			// CRStepLog Nearly Full, Write And Clear It
			memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));

			memcpy( CRLogTxt, CRStepLog, len );
		
			WriteLog( PROCESS_CR, CRLogTxt );
	
			// Clear For Next Write
			memset( CRStepLog, 0x00, sizeof( CRStepLog ));
		}
	}
}

/*
 ** CRSpringCardLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Write Log When PowerOff Card And Spring Out Card
 *
 *  RETURNS: None.
 *
 */
void CRSpringCardLog( void )
{
	unsigned char len;

	if( CR_CommandType == CR_CARDREADER_POWEROFF_CARD )
	{
		memset( CRStepLog, 0x00, sizeof( CRStepLog ));
	
		if( HoldCard == 1 )
		{
			memcpy( CRStepLog, "掉电更新: ", 10 );
		}
		else
		{
			memcpy( CRStepLog, "掉电弹卡: ", 10 );
		}

		len	= strlen(( char * )CRStepLog );
		memcpy( &CRStepLog[len], "Proc: ", 6 );
		len	= strlen(( char * )CRStepLog );
		sprintf(( char * )&CRStepLog[len], "%02X", CR_Proc_Last_Step );

		len	= strlen(( char * )CRStepLog );
		memcpy( &CRStepLog[len], "  Step: ", 8 );
		len	= strlen(( char * )CRStepLog );
		sprintf(( char * )&CRStepLog[len], "%02X", CR_Last_CommandType );
		
		len	= strlen(( char * )CRStepLog );
		memcpy( &CRStepLog[len], "  Flag: ", 8 );
		len	= strlen(( char * )CRStepLog );
		sprintf(( char * )&CRStepLog[len], "%u", PowerDownFlag );
        
        len	= strlen(( char * )CRStepLog );
		memcpy( &CRStepLog[len], "  Err: ", 7 );
		len	= strlen(( char * )CRStepLog );
		sprintf(( char * )&CRStepLog[len], "%04X",  ErrorFlag );
		
		// Write To Log
		WriteLog( PROCESS_CR, CRStepLog );
	}
	else if( CR_CommandType == CR_CARDREADER_SPINGOUT_CARD )
	{
		if( CR_Last_CommandType != CR_CARDREADER_POWEROFF_CARD )
		{
			memset( CRStepLog, 0x00, sizeof( CRStepLog ));
		
			memcpy( CRStepLog, "直接弹卡: ", 10 );

			len	= strlen(( char * )CRStepLog );
			memcpy( &CRStepLog[len], "Proc: ", 6 );
			len	= strlen(( char * )CRStepLog );
			sprintf(( char * )&CRStepLog[len], "%02X", CR_Proc_Last_Step );

			len	= strlen(( char * )CRStepLog );
			memcpy( &CRStepLog[len], "  Step: ", 8 );
			len	= strlen(( char * )CRStepLog );
			sprintf(( char * )&CRStepLog[len], "%02X", CR_Last_CommandType );
			
			len	= strlen(( char * )CRStepLog );
			memcpy( &CRStepLog[len], "  Flag: ", 8 );
			len	= strlen(( char * )CRStepLog );
			sprintf(( char * )&CRStepLog[len], "%u", PowerDownFlag );
            
            len	= strlen(( char * )CRStepLog );
            memcpy( &CRStepLog[len], "  Err: ", 7 );
            len	= strlen(( char * )CRStepLog );
            sprintf(( char * )&CRStepLog[len], "%04X",  ErrorFlag);
			
			// Write To Log
			WriteLog( PROCESS_CR, CRStepLog );
		}
		else
		{
			// Log Was Saved In PowerDown, Do Not Save Again
			// Just Go On....
		}
	}
	else
	{
		// Do Not Save, Just Go On....
	}
}

/*
 ** SaveCRConfigFlash
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save The Define Elements In PD_All To DataFlash After Each Updating, Except The PD_TRANS
 *
 *  RETURNS: None.
 *
 *	ATTN: Only Save The Config Elements Except The PD_TRANS
 *
 */
void SaveCRConfigFlash( void )
{
 	// Save To DataFlash
 	WriteCrConfig( &PowerDown.PDStartFlag, (( &PowerDown.PDCRFlag ) - ( &PowerDown.PDStartFlag )));
}

/*
 ** ReadCRConfigFlash
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read The Saved Define Elements In PD_All To DataFlash After Each Updating, Except The PD_TRANS
 *
 *  RETURNS: None.
 *
 *	ATTN: Only Read The Config Elements Except The PD_TRANS
 *
 */
void ReadCRConfigFlash( void )
{
	ReadCrConfig( &PowerDown.PDStartFlag, (( &PowerDown.PDCRFlag ) - ( &PowerDown.PDStartFlag )));
	
//Douzhq 20140611 Add , IC Card Recognize
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	AidType = ReadRecognizeFlag();
#endif
	
}


#ifdef MAKE_PSAM_VERIFY

/*
 ** FillPOSMessage
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	Cmd: Command Type: 0: "#*0011*#"; 1: "#*1122*#"; 2: "#*2233*#";	3: "其他值";
 *				RType: Register Process Flag, Ok Or Failed And Faid Step
 *						0: Register Ok; 1 ---- 40 ACT Fail And Step; 41----90: RID Fail And Step
 *				Times: Allow Verify Times
 *
 *  DESCRIPTION: Send PSAM Register And Verify Message To POS,
 *
 *  RETURNS: Length Of The Message
 *
 */
unsigned short int FillPOSMessage( unsigned char Cmd, unsigned char RType, unsigned short int Times )
{
	unsigned short int len = 0;

	
	// Clear Buffer
	memset( POS_CR_Buf.InforMessage, 0x00, sizeof( POS_CR_Buf.InforMessage ));

	// Fill Command
	if( Cmd == 0x00 )
	{
		memcpy( &POS_CR_Buf.InforMessage[len], "#*0011*#", 8 );
		len += 8; 
	}
	else if( Cmd == 0x01 )
	{
		memcpy( &POS_CR_Buf.InforMessage[len], "#*1122*#", 8 );
		len += 8; 
	}
	else if( Cmd == 0x02 )
	{
		memcpy( &POS_CR_Buf.InforMessage[len], "#*2233*#", 8 );
		len += 8; 
	}
	else
	{
		memcpy( &POS_CR_Buf.InforMessage[len], "#*3344*#", 8 );
		len += 8; 
	}

	// Fill Gilbarco ID
	memcpy( &POS_CR_Buf.InforMessage[len], GilbarcoID, sizeof( GilbarcoID ));
	len += sizeof( GilbarcoID );
	
//	// Fill Pump Software Version
//	Hex2Asc( &POS_CR_Buf.InforMessage[len], &Pump_CR_Buf.PumpVersion[2], 2 );
//	len += 4;
//	// Fill CR Software Version
//	memcpy( &POS_CR_Buf.InforMessage[len], &DISPMSG[M_VersionNumber][4], 2 );
//	len += 2;
//	memcpy( &POS_CR_Buf.InforMessage[len], &DISPMSG[M_VersionNumber][7], 2 );
//	len += 2;

	// Fill Constant Version Number By Steven Xiong Email 2011-07-01 09:47
	memcpy( &POS_CR_Buf.InforMessage[len], "0102", 4 );
	len += 4;



	// Fill Date And Time
	Hex2Asc( &POS_CR_Buf.InforMessage[len], TRINP.TR_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));
	len += ( sizeof( TRINP.TR_TRANS_DATE ) * 2 );
	Hex2Asc( &POS_CR_Buf.InforMessage[len], TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));
	len += ( sizeof( TRINP.TR_TRANS_TIME ) * 2 );

	// Fill Pump Seiral Number
	Hex2Asc( &POS_CR_Buf.InforMessage[len], Pump_CR_Buf.PUMPSerialNumber, sizeof( Pump_CR_Buf.PUMPSerialNumber ));
	len += ( sizeof( Pump_CR_Buf.PUMPSerialNumber ) * 2 );

	// Fill Content
//	memset( &POS_CR_Buf.InforMessage[len], '0', 16 );
	sprintf(( char * )&POS_CR_Buf.InforMessage[len], "%02u", Times );
	len += 2;
//	// Rewrite sprintf end 0x00
//	POS_CR_Buf.InforMessage[len] = '0';
//	len += 1;

	if( Cmd == 0x00 )
	{
		if( RType == 0x00 )
		{
			POS_CR_Buf.InforMessage[len] = '1';
			len += 1;
			memset( &POS_CR_Buf.InforMessage[len], '0', 13 );
			len += 13;
		}
		else if( RType <= 40 )
		{
			POS_CR_Buf.InforMessage[len] = '2';
			len += 1;
			POS_CR_Buf.InforMessage[len] = '1';
			len += 1;
			Hex2Asc( &POS_CR_Buf.InforMessage[len], &RType, 1 );
			len += 2;
			memset( &POS_CR_Buf.InforMessage[len], '0', 10 );
			len += 10;
		}
		else
		{
			POS_CR_Buf.InforMessage[len] = '2';
			len += 1;
			POS_CR_Buf.InforMessage[len] = '2';
			len += 1;
			Hex2Asc( &POS_CR_Buf.InforMessage[len], &RType, 1 );
			len += 2;
			memset( &POS_CR_Buf.InforMessage[len], '0', 10 );
			len += 10;
		}
	}
	else
	{
		memset( &POS_CR_Buf.InforMessage[len], '0', 14 );
		len += 14;
	}

	// Fill PSAM ASN
	Hex2Asc( &POS_CR_Buf.InforMessage[len], TRINP.TR_PSAM_ASN, sizeof( TRINP.TR_PSAM_ASN ));
	len += ( sizeof( TRINP.TR_PSAM_ASN ) * 2 );

//	memcpy( &POS_CR_Buf.InforMessage[len], pData, strlen( pData ));
//	len += strlen( pData );

	return len;
}

#endif


#ifdef MAKE_CASH_CR

/*
 ** CheckCashCard
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ExCRProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Cash Card For Cash Trans, Include Log On, Error etc....
 *
 *  RETURNS: 0: Can Use; 1: Can Not Use
 *
 *	ATTN: Only Check Logon And Card Error, The Actually Can Use Will Be Issued After Get The Cash Card Information In CR_ICCARD_INSERT_PROC
 *
 */
unsigned char CheckCashCard( void )
{
	unsigned char retval;

	// Default Can Not Use
	retval = 1;

	if( !( ProcStatus[PROCESS_CR] & CARD_ERROR ))
	{
		// Check Log On
		if( ProcStatus[PROCESS_CR] & CARD_LOGON )
		{
			if( !( ProcStatus[PROCESS_CR] & CARD_INSERTED ))
			{
				// Can Go To CR_ICCARD_INSERT_PROC, Use Cash Card
				retval = 0;

				// Clear Amount And Datas
				ClearCashInfo();
			}
			else
			{
				//Douzhq 20141023 Add , For Bug
				//-----------------------------Down----------------------------------
				if (ProcExchange[PROCESS_CR][PROCESS_CR] & CASH_CODE_ERR)
				{
					FillInfoMsg(M_InvalidOperation, 0, 0);
				}
				else
				//----------------------------- Up ----------------------------------
				{
					// Card Inserted, Waiting
					FillInfoMsg( M_CardInserted, 0, 0 );
				}
			}
		}
		else
		{
			// Not Log On
			FillInfoMsg( M_LogOffed, 0, 0 );
			
			ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_FAULTERR;
		}
	}
	else
	{
		// Error, Can Not Use
		FillInfoMsg( M_ERROR_STOP, 0, 0 );
	}

	// Check Issue Or Not
	if( retval == 1 )
	{
		// Can not Use
		Cash_CR_Buf.CashCard_Flag = 0x01;
		ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_KEY_PRESSED;
	}


	return retval;
}


/*
 ** SaveCashTotal
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ExCRProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save The Accepted(Authed) Cash And Auth Code etc..
 *
 *  RETURNS: None.
 *
 */
void SaveCashTotal( void )
{
//	unsigned int value;
	unsigned char val;
	unsigned char buf[16];

//	value = CharToIntCopy( Cash_CR_Buf.CashTotal );
//	value *= 100;
//	IntToCharCopy( value, TRINP.TR_CASH_TOTAL );

	// Copy Amount
	memcpy( TRINP.TR_CASH_TOTAL, Cash_CR_Buf.CashTotal, sizeof( TRINP.TR_CASH_TOTAL ));



	memcpy( PowerDown.PdTrans.PD_CASH_TOTAL, TRINP.TR_CASH_TOTAL, sizeof( PowerDown.PdTrans.PD_CASH_TOTAL ));
	// Clear Trans Amount To Avoid Cash Total Ok But Power Off Before Nozzle On
	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT, TRINP.TR_TRANS_AMOUNT, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT ));
	
	//20120719 liuzg start	--for POS to print ticket
	TRINP.TR_TRANS_UNIT &= 0x0f;
	TRINP.TR_TRANS_UNIT |= (CashFromPosUnit & 0xf0);
	PowerDown.PdTrans.PD_TRANS_UNIT = TRINP.TR_TRANS_UNIT;
	//20120719 liuzg end	--for POS to print ticket

	// Save Auth Code
//	TRINP.TR_AUTH_CODE_LEN = Cash_CR_Buf.CashAuthCodeLen;
//	memcpy( TRINP.TR_AUTH_CODE, Cash_CR_Buf.CashAuthCode, sizeof( TRINP.TR_AUTH_CODE ));

//	// Save Auth Code
//	TRINP.TR_AUTH_CODE_LEN = Cash_CR_Buf.CashAuthCode[0];
//	memcpy( TRINP.TR_AUTH_CODE, &Cash_CR_Buf.CashAuthCode[1], sizeof( TRINP.TR_AUTH_CODE ));

	// Save Auth Code
	memset( buf, 0x00, sizeof( buf ));
	Hex2Asc( buf, &Cash_CR_Buf.CashAuthCode[1], Cash_CR_Buf.CashAuthCode[0] );
	
	for( val = 0; val < Cash_CR_Buf.CashAuthCode[0] * 2; val++ )
	{
		if(( buf[val] == 'F' ) || ( buf[val] == 'f' ))
		{
			buf[val] = 0x00;
			break;
		}
	}

	// Include The Leading '6' And Ending Check Byte
	TRINP.TR_AUTH_CODE_LEN = val;
	memcpy( TRINP.TR_AUTH_CODE, buf, val );

	PowerDown.PdTrans.PD_AUTH_CODE_LEN = TRINP.TR_AUTH_CODE_LEN;
	memcpy( PowerDown.PdTrans.PD_AUTH_CODE, TRINP.TR_AUTH_CODE, sizeof( PowerDown.PdTrans.PD_AUTH_CODE ));
}


/*
 ** ClearCashInfo
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\ExCRProc.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Clear The Cash Information
 *
 *  RETURNS: None.
 *
 */
void ClearCashInfo( void )
{
	// Clear The CR Cash Buffer
	memset( Cash_CR_Buf.CashTotal, 0x00, sizeof( Cash_CR_Buf.CashTotal ));
	memset( Cash_CR_Buf.CashInserted, 0x00, sizeof( Cash_CR_Buf.CashInserted ));
	memset( Cash_CR_Buf.CashLists, 0x00, sizeof( Cash_CR_Buf.CashLists ));
	memset( Cash_CR_Buf.CashTotalInserted, 0x00, sizeof( Cash_CR_Buf.CashTotalInserted ));
	memset( Cash_CR_Buf.CashTransRecord, 0x00, sizeof( Cash_CR_Buf.CashTransRecord ));

	// Clear The Power Down Buffer
	PowerDown.PdTrans.PD_AUTH_CODE_LEN = 0;
	memset( PowerDown.PdTrans.PD_AUTH_CODE, 0x00, sizeof( PowerDown.PdTrans.PD_AUTH_CODE ));
	memset( PowerDown.PdTrans.PD_CASH_TOTAL, 0x00, sizeof( PowerDown.PdTrans.PD_CASH_TOTAL ));
}


/*
 ** CheckCashType
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRMemCopy.c
 *
 *  PARAMETERS:	unsigned char *pRecordRFU: Pointer To Record RFU
 *
 *  DESCRIPTION: Check The Trans Record Is Cash Acceptor Or Cash POS Auth
 *
 *  RETURNS: 0: Cash Acceptor Trans; 1: Cash POS Auth; Other: Error
 *
 */
unsigned char CheckCashType( unsigned char *pRecordRFU )
{
	unsigned char retval;
	unsigned char val;
	unsigned int value;

	retval = 0x02;
	// Get Received Cash Amount
	value = CharToIntCopy( pRecordRFU );
	// Get AuthCode Lenght
	val = *( pRecordRFU + 4	);

	if( value != 0 )
	{
		if( val == 0 )
		{
			// Auth Code Length = 0;
			// Cash Acceptor Trans
			retval = 0x00;
		}
		else
		{
			// Auth Code Length != 0;
			// Cash POS Trans
			retval = 0x01;
		}
	}
	else
	{
		// No Received Cash, Neither Cash Acceptor Nor Cash POS
	}

	return retval;
}

#endif
