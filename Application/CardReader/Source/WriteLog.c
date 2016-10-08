#include <string.h>

#include "global.h"

#include "define.h"
#include "WriteLog.h"





/*
 ** WriteLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\WriteLog.c
 *
 *  PARAMETERS:	*pSource: Pointer To The String To Be Saved
 *				Process: Process ID
 *
 *  DESCRIPTION: Write Running Log To DataFlash
 *
 *  RETURNS: 0: Ok; 1: DataFlash Error; 2: Write Error
 *
 *	Atten: Length Must Not Exceed MaxLogLen	- TimeLen - 2
 *
 */
unsigned char WriteLog( enum MAX_PROCESS Process, unsigned char *pString )
{
	unsigned char retval;
	unsigned char len, S_len, TxtLen;
	unsigned char buf[MaxLogLen+1];

	// Default Ok
	retval = 0;

	if(( Process > PROCESS_START ) && ( Process < PROCESS_MAX ))
	{
		// Only For Current Process
		memset( buf, 0x00, sizeof( buf ));
		len = 0;

		// Fill Time To Buf
		sprintf(( char * )&buf[len], "%02X", SysBCDTime.month );
		len += 2;
		
		sprintf(( char * )&buf[len], "%02X", SysBCDTime.day );
		len += 2;

		sprintf(( char * )&buf[len], "%02X", SysBCDTime.hour );
		len += 2;

		sprintf(( char * )&buf[len], "%02X", SysBCDTime.minute );
		len += 2;

		sprintf(( char * )&buf[len], "%02X", SysBCDTime.second );
		len += 2;

		buf[len] = ':';
		len += 1;

		buf[len] = ' ';
		len += 1;

		S_len = strlen(( char * )pString );
		
		TxtLen = MaxLogLen - len;
		if( S_len >= TxtLen )
		{
			memcpy( &buf[len], pString, TxtLen );
		}
		else
		{
			memcpy( &buf[len], pString, S_len );
		}
	
		// Save To DataFlash
		retval = AscLog( buf );
	}
	else
	{
		// Return Error
		retval = 2;
	}


	return retval;
}
