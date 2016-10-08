
#include "global.h"

#include "define.h"

#include "CRVaribles.h"
#include "CRMemcopy.h"
#include "Keypad.h"
#include "KeyMask.h"
#include "Screen.h"
#include "Display.h"
#include "PdProtect.h"
#include "CRPrint.h"






/*
 ** IssueLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	flag: 0: Issue Log on; 1: Issue Log Off
 *
 *  DESCRIPTION: Wether To Inform CR Process Excute The Log On/Off
 *
 *  RETURNS: 0: Inform CR To Excute Log On/Off;
 *			 1: Do Not Issue The Log Command, And Display: Already Log On
 *			 2: Do Not Issue The Log Command, And Display: Already Log Off
 *			 3: Do Not Issue The Log Command, And Display: Not Employee Card
 *			 4: No Card In The Card Slot
 *
 */
unsigned char IssueLog( unsigned char flag )
{
	unsigned char retval;

	// Default, 
	retval = 1;
		
	if( ProcStatus[PROCESS_CR] & CARD_READY )
	{
		if( TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD )
		{
			if( flag == 0 )
			{
				// Check Issue Log On
				if( ProcStatus[PROCESS_CR] & CARD_LOGON )
				{
					// Display "Already Log On"
					retval = 1;
				}
				else
				{
					// Issue Log On
					retval = 0;
				}
			}
			else
			{
				// Check Issue Log Off
				if( ProcStatus[PROCESS_CR] & CARD_LOGON )
				{
					// Issue Log Off
					retval = 0;
				}
				else
				{
					// Display "Already Log Off"
					retval = 2;
				}
			}
		}
		else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
		{
			// Test Card May Log Off, Buf Not Log On
			if( flag == 0 )
			{
				// Display "Not Employee Card"
				retval = 3;
			}
			else
			{
				// Check Issue Log Off
				if( ProcStatus[PROCESS_CR] & CARD_LOGON )
				{
					// Issue Log Off
					retval = 0;
				}
				else
				{
					// Display "Already Log Off"
					retval = 2;
				}
			}
		}
		else
		{
			// Display "Not Employee Card"
			retval = 3;
		}
	}
	else
	{
		// Display " No Card In Slot"
		retval = 4;
	
	}

	return retval;
}

/*
 ** IssueLogDispInfoLine
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	flag:
 *			 1: Do Not Issue The Log Command, And Display: Already Log On
 *			 2: Do Not Issue The Log Command, And Display: Already Log Off
 *			 3: Do Not Issue The Log Command, And Display: Not Employee Card
 *			 4: No Card In The Card Slot
 *
 *  DESCRIPTION: Display The Info Line After Log On/Off Command Can Not Issue
 *
 *  RETURNS:
 *
 */
void IssueLogDispInfoLine( unsigned char flag )
{
	if( flag == 1 )
	{
		FillInfoMsg( M_LogOned, 0, 0 );
	}
	else if( flag == 2 )
	{
		FillInfoMsg( M_LogOffed, 0, 0 );
	}
	else if( flag == 3 )
	{
		FillInfoMsg( M_NotEMPCard, 0, 0 );
	}
	else
	{
		FillInfoMsg( M_InsertCard, 0, 0 );
	}
}



/*
 ** ASC2BCD_F
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	*pASC: Pointer To Ascii Buffer
 *				*pBCD: Pointer To BCD Buffer
 *				len: Ascii Buffer Length
 *
 *  DESCRIPTION: Combine Ascii Buffer To BCD Buffer, And Attached 0x0F, While len%2
 *
 *  RETURNS: BCD Buffer Length
 *
 */
unsigned char ASC2BCD_F( unsigned char *pBCD, unsigned char *pASC, unsigned char len )
{
	unsigned char i;
	unsigned char BCDLen;
	unsigned char temp;

	// Clear Buffer
//	memset( pBCD, 0x00, sizeof( pBCD ));
	memset( pBCD, 0x00, len );
	BCDLen = 0;

	for( i = 0; i < len; i++ )
	{
		temp = *( pASC + i );
		temp &= 0x0F;
		temp <<= 4;
		temp &= 0xF0;
		*( pBCD + BCDLen ) = temp;

		i++;
		temp = *( pASC + i );
		temp &= 0x0F;
		*( pBCD + BCDLen ) |= temp;
		
		BCDLen++;
	}

	if( len % 2 )
	{
		*( pBCD + BCDLen - 1 ) |= 0x0F;	
//
//		BCDLen *= 2;
//		BCDLen -= 1;
	}
//	else
//	{
//		BCDLen *= 2;
//	}


	return BCDLen;
}


/*
 ** GetPreset
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get And Display The Preset Value And Mode
 *
 *  RETURNS: None.
 *
 */
void GetPreset( void )
{
	unsigned char len;
	unsigned char numbers;
	unsigned int presetvalue;
	char buf[11];
	
	// Check Max Input Len
	if( PresetDotFlag == 0 )
	{
		// No Decimal Point, Max 4 Digits
		numbers = 4;
	}
	else
	{
		numbers = 6;
	}

	// Input Preset Value
	if( InputLen == 0 )
	{
		if(( KeyValue >= '1' ) && ( KeyValue <= '9' ))
		{
		  
//Preset Save Oil Applaction
//Douzhq 20140610 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
			if (TRINP.TR_TRANS_DEBITSOURCE == 0x04 && PowerDown.SaveOilAppMode == 0x01)
			{
				KEYPAD_CR_Buf.PresetMode = 1;	
			}
#endif
			// No Decimal Point
			PresetDotFlag = 0;

			// Leading Charater Must Be 1 To 9
			InputBuf[InputLen] = KeyValue;
			InputLen++;
		}
		else
		{
			// Do Not Care About, Go On....
		}
	}
	else
	{
		if(( KeyValue >= '0' ) && ( KeyValue <= '9' ))
		{
			// Input Number
			if( InputLen < numbers )
			{
				if( PresetDotFlag == 0 )
				{
					InputBuf[InputLen] = KeyValue;
					InputLen++;
				}
				else if( PresetDotFlag == 1 )
				{
					// The First Decimal Point Number
					InputBuf[InputLen-2] = KeyValue;
					PresetDotFlag = 2;
				}
				else if( PresetDotFlag == 2 )
				{
					// The Second Decimal Point Number
					InputBuf[InputLen-1] = KeyValue;
					PresetDotFlag = 4;
				}
				else
				{
					// Input Decimal Point Error, Go On....
				}
			}
			else
			{
				// Input Too Long, Only Check The Decimal Point Numbers
				if( PresetDotFlag == 1 )
				{
					// The First Decimal Point Number
					InputBuf[InputLen-2] = KeyValue;
					PresetDotFlag = 2;
				}
				else if( PresetDotFlag == 2 )
				{
					// The Second Decimal Point Number
					InputBuf[InputLen-1] = KeyValue;
					PresetDotFlag = 4;
				}
				else
				{
					// Input Decimal Point Error, Go On....
				}
			}
		}
		else
		{
			// Input AMOUNT Or VOLUME Or Clear
			if( KeyValue == CLEAR )
			{
				// Clear The Preset
				KEYPAD_CR_Buf.PresetMode = 0;
				memset( KEYPAD_CR_Buf.PresetValue, 0x00, sizeof( KEYPAD_CR_Buf.PresetValue ));

				memset( InputBuf, 0x00, sizeof( InputBuf ));
				InputLen = 0;
				
				ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~PRESET );
			
				// Next Screen
//				NextScreenID = DEFAULT_SCR_BALANCE;
//				DrawBalanceSCR();
				DispIdle();
			}
			else if(  KeyValue == '.' )
			{
				if( PresetDotFlag == 0 )
				{
					// Preset Decimal Point Was Pressed
					PresetDotFlag = 1;
			
					// Attached 2 '0'
					len = strlen(( char *)InputBuf );
					InputBuf[len] = '0';
					InputLen++;
					InputBuf[len+1]	= '0';
					InputLen++;
				}
			}
			else if(  KeyValue == AMOUNT )
			{
				// Amount Preset

//Preset Save Oil Applaction
//Douzhq 20140610 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (TRINP.TR_TRANS_DEBITSOURCE == 0x04 && PowerDown.SaveOilAppMode == 0x01)
	{
		KEYPAD_CR_Buf.PresetMode = 1;
	}
	else
	{
		KEYPAD_CR_Buf.PresetMode = 0;
	}
#else
		KEYPAD_CR_Buf.PresetMode = 0;
#endif
	
			}
			else
			{
				// Must Be Volume Preset
				KEYPAD_CR_Buf.PresetMode = 1;
			}
		}
	}

	// Cala Input Value, And Display It
	if( InputLen != 0 )
	{
		memset( buf, 0x30, sizeof( buf ));
		if( PresetDotFlag == 0 )
		{
			memcpy( &buf[11-InputLen-2], InputBuf, InputLen );
		}
		else
		{
			memcpy( &buf[11-InputLen], InputBuf, InputLen );
		}
//		presetvalue = CharToIntCopy( KEYPAD_CR_Buf.PresetValue );
		presetvalue = atoi( buf );
		IntToCharCopy( presetvalue, KEYPAD_CR_Buf.PresetValue );
		
		// Inform CR Preset
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] |= PRESET;

		// For Display
		memset( buf, 0x00, sizeof( buf ));
		sprintf( buf, "%u", presetvalue );

		if( PresetDotFlag == 0 )
		{
			// No Preset Decimal Point, Clear The Attached 2 Digits Decimal
			len = strlen( buf );
			buf[len-1] = 0x00;
			buf[len-2] = 0x00;
		
			// Display It
			PresetDisplay(( unsigned char *)buf, PresetDotFlag, KEYPAD_CR_Buf.PresetMode );
		}
		else
		{
			// Display It
			PresetDisplay(( unsigned char *)buf, 1, KEYPAD_CR_Buf.PresetMode );
		}
	}
}


/*
 ** GetCurrentBackLightTime
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Format Current BackLight And Save To CustomMsg0 For Display
 *
 *  RETURNS: None.
 *
 */
void GetCurrentBackLightTime( void )
{
	unsigned char len;
	unsigned char time;

	// Clear Buffer
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));

	// Setup Backlight Time
	len = strlen( DISPMSG[M_CurrentBackLightTime] );
	memcpy( CustomMsg0, DISPMSG[M_CurrentBackLightTime], len );

	time = GetLcdLightOpenTime();
	sprintf(( char * )&CustomMsg0[len], "%02d", time );
	len = strlen(( char * )CustomMsg0 );
	time = GetLcdLightCloseTime();
	sprintf(( char * )&CustomMsg0[len], "%02d", time );

	len = strlen( DISPMSG[M_NewBackLightTime] );
	memcpy( CustomMsg1, DISPMSG[M_NewBackLightTime], len );
}


/*
 ** ScrollUpSelect
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Scroll_Up Key Pressed, Display Inversed The Next List Item
 *
 *  RETURNS: None.
 *
 */
void ScrollUpSelect( void )
{
	// Inverse Display The Last Item
	DisplayContra( ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopX,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopY,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomX,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomY );
	
	if( DispListInversedIdx == 0 )
	{
		DispListInversedIdx = ScreenList[ScreenID].ListCnt - 1;
	}
	else
	{
		DispListInversedIdx--;
	}

	// Inverse Display The Next Item
	DisplayContra( ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopX,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopY,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomX,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomY );
	
	// Display The Screen
	DisBufToLcd();
}

/*
 ** ScrollDownSelect
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Scroll_Down Key Pressed, Display Inversed The Next Last Item
 *
 *  RETURNS: None.
 *
 */
void ScrollDownSelect( void )
{
	// Inverse Display The Last Item
	DisplayContra( ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopX,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopY,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomX,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomY );
	
	if( DispListInversedIdx >= ( ScreenList[ScreenID].ListCnt - 1 ))
	{
		DispListInversedIdx = 0;
	}
	else
	{
		DispListInversedIdx++;
	}

	// Inverse Display The Next Item
	DisplayContra( ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopX,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopY,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomX,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomY );
	
		
	// Display The Screen
	DisBufToLcd();
}


/*
 ** GetPrintSetStatus
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	type: 0: Customer Card; 1: Employee Card; 2 Test Card; 3 Maintainance Card
 *						4: Trans Without Card ASN, Such As Offline Trans, Pice Receiving And Log On Log Off
 *
 *  DESCRIPTION: Get Current Print Setup Status And Save To CustomMsg0 ----CustomMsg5
 *
 *  RETURNS: None.
 *
 */
void GetPrintSetStatus( unsigned char type )
{
	unsigned int status;

	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));
	memset( CustomMsg4, 0x00, sizeof( CustomMsg4 ));
	memset( CustomMsg5, 0x00, sizeof( CustomMsg5 ));

	status = CharToIntCopy( PowerDown.PrintIt );

	if( type == 0 )
	{
		// Check Customer Card Print Setup Bits
		// Copy The Messages
		memcpy( CustomMsg0, DISPMSG[M_TransOk], strlen( DISPMSG[M_TransOk] ));
		memcpy( CustomMsg1, DISPMSG[M_TransGrey], strlen( DISPMSG[M_TransGrey] ));
		memcpy( CustomMsg2, DISPMSG[M_TransUnlock], strlen( DISPMSG[M_TransUnlock] ));
		memcpy( CustomMsg3, DISPMSG[M_TransGreyFlag], strlen( DISPMSG[M_TransGreyFlag] ));
		memcpy( CustomMsg4, DISPMSG[M_ErrCard], strlen( DISPMSG[M_ErrCard] ));
		memcpy( CustomMsg5, DISPMSG[M_CardErrorTrans], strlen( DISPMSG[M_CardErrorTrans] ));
	
		// Fill Current Status
		if( status & CustomTransOkReceipt )
		{
			CustomMsg0[0] = '*';
		}
		else
		{
			CustomMsg0[0] = ' ';
		}

		if( status & CustomTransGreyReceipt )
		{
			CustomMsg1[0] = '*';
		}
		else
		{
			CustomMsg1[0] = ' ';
		}

		if( status & CustomGreyUnlockReceipt )
		{
			CustomMsg2[0] = '*';
		}
		else
		{
			CustomMsg2[0] = ' ';
		}

		if( status & CustomGreyFlagReceipt )
		{
			CustomMsg3[0] = '*';
		}
		else
		{
			CustomMsg3[0] = ' ';
		}

		if( status & CustomTransErrorReceipt )
		{
			CustomMsg4[0] = '*';
		}
		else
		{
			CustomMsg4[0] = ' ';
		}

		if( status & CustomCardErrorReceipt )
		{
			CustomMsg5[0] = '*';
		}
		else
		{
			CustomMsg5[0] = ' ';
		}
	}
	else if( type == 1 )
	{
		// Check Employee Card Print Setup Bits
		// Copy The Messages
		memcpy( CustomMsg0, DISPMSG[M_TransOk], strlen( DISPMSG[M_TransOk] ));
		memcpy( CustomMsg1, DISPMSG[M_TransGrey], strlen( DISPMSG[M_TransGrey] ));
		memcpy( CustomMsg2, DISPMSG[M_TransUnlock], strlen( DISPMSG[M_TransUnlock] ));
		memcpy( CustomMsg3, DISPMSG[M_TransGreyFlag], strlen( DISPMSG[M_TransGreyFlag] ));
		memcpy( CustomMsg4, DISPMSG[M_ErrCard], strlen( DISPMSG[M_ErrCard] ));
		memcpy( CustomMsg5, DISPMSG[M_CardErrorTrans], strlen( DISPMSG[M_CardErrorTrans] ));
	
		// Fill Current Status
		if( status & EMPTransOkReceipt )
		{
			CustomMsg0[0] = '*';
		}
		else
		{
			CustomMsg0[0] = ' ';
		}

		if( status & EMPTransGreyReceipt )
		{
			CustomMsg1[0] = '*';
		}
		else
		{
			CustomMsg1[0] = ' ';
		}

		if( status & EMPGreyUnlockReceipt )
		{
			CustomMsg2[0] = '*';
		}
		else
		{
			CustomMsg2[0] = ' ';
		}

		if( status & EMPGreyFlagReceipt )
		{
			CustomMsg3[0] = '*';
		}
		else
		{
			CustomMsg3[0] = ' ';
		}

		if( status & EMPTransErrorReceipt )
		{
			CustomMsg4[0] = '*';
		}
		else
		{
			CustomMsg4[0] = ' ';
		}

		if( status & EMPCardErrorReceipt )
		{
			CustomMsg5[0] = '*';
		}
		else
		{
			CustomMsg5[0] = ' ';
		}
	}
	else if( type == 2 )
	{
		// Check Test Card Print Setup Bits
		// Copy The Messages
		memcpy( CustomMsg0, DISPMSG[M_TransOk], strlen( DISPMSG[M_TransOk] ));
		memcpy( CustomMsg1, DISPMSG[M_TransGrey], strlen( DISPMSG[M_TransGrey] ));
		memcpy( CustomMsg2, DISPMSG[M_TransUnlock], strlen( DISPMSG[M_TransUnlock] ));
		memcpy( CustomMsg3, DISPMSG[M_TransGreyFlag], strlen( DISPMSG[M_TransGreyFlag] ));
		memcpy( CustomMsg4, DISPMSG[M_ErrCard], strlen( DISPMSG[M_ErrCard] ));
		memcpy( CustomMsg5, DISPMSG[M_CardErrorTrans], strlen( DISPMSG[M_CardErrorTrans] ));
	
		// Fill Current Status
		if( status & TestTransOkReceipt )
		{
			CustomMsg0[0] = '*';
		}
		else
		{
			CustomMsg0[0] = ' ';
		}

		if( status & TestTransGreyReceipt )
		{
			CustomMsg1[0] = '*';
		}
		else
		{
			CustomMsg1[0] = ' ';
		}

		if( status & TestGreyUnlockReceipt )
		{
			CustomMsg2[0] = '*';
		}
		else
		{
			CustomMsg2[0] = ' ';
		}

		if( status & TestGreyFlagReceipt )
		{
			CustomMsg3[0] = '*';
		}
		else
		{
			CustomMsg3[0] = ' ';
		}

		if( status & TestTransErrorReceipt )
		{
			CustomMsg4[0] = '*';
		}
		else
		{
			CustomMsg4[0] = ' ';
		}

		if( status & TestCardErrorReceipt )
		{
			CustomMsg5[0] = '*';
		}
		else
		{
			CustomMsg5[0] = ' ';
		}
	}
	else if( type == 3 )
	{
		// Check Maintainance Card Print Setup Bits
		// Copy The Messages
		memcpy( CustomMsg0, DISPMSG[M_TransOk], strlen( DISPMSG[M_TransOk] ));
		memcpy( CustomMsg1, DISPMSG[M_TransGrey], strlen( DISPMSG[M_TransGrey] ));
		memcpy( CustomMsg2, DISPMSG[M_TransUnlock], strlen( DISPMSG[M_TransUnlock] ));
		memcpy( CustomMsg3, DISPMSG[M_TransGreyFlag], strlen( DISPMSG[M_TransGreyFlag] ));
		memcpy( CustomMsg4, DISPMSG[M_ErrCard], strlen( DISPMSG[M_ErrCard] ));
		memcpy( CustomMsg5, DISPMSG[M_CardErrorTrans], strlen( DISPMSG[M_CardErrorTrans] ));
	
		// Fill Current Status
		if( status & MatnTransOkReceipt )
		{
			CustomMsg0[0] = '*';
		}
		else
		{
			CustomMsg0[0] = ' ';
		}

		if( status & MatnTransGreyReceipt )
		{
			CustomMsg1[0] = '*';
		}
		else
		{
			CustomMsg1[0] = ' ';
		}

		if( status & MatnGreyUnlockReceipt )
		{
			CustomMsg2[0] = '*';
		}
		else
		{
			CustomMsg2[0] = ' ';
		}

		if( status & MatnGreyFlagReceipt )
		{
			CustomMsg3[0] = '*';
		}
		else
		{
			CustomMsg3[0] = ' ';
		}

		if( status & MatnTransErrorReceipt )
		{
			CustomMsg4[0] = '*';
		}
		else
		{
			CustomMsg4[0] = ' ';
		}

		if( status & MatnCardErrorReceipt )
		{
			CustomMsg5[0] = '*';
		}
		else
		{
			CustomMsg5[0] = ' ';
		}
	}
	else
	{
		// Check Public Trans Print Setup Bits
		// Attention: Pub Trans Used 4 CustomMsgs Only,
		// For Offline Trans, Price Receiving, Log On And Log Off
		// Copy The Messages
		memcpy( CustomMsg0, DISPMSG[M_NonCard], strlen( DISPMSG[M_NonCard] ));
		memcpy( CustomMsg1, DISPMSG[M_LogOnTrans], strlen( DISPMSG[M_LogOnTrans] ));
		memcpy( CustomMsg2, DISPMSG[M_LogOffTrans], strlen( DISPMSG[M_LogOffTrans] ));
		memcpy( CustomMsg3, DISPMSG[M_PriceRecv], strlen( DISPMSG[M_PriceRecv] ));
	
		// Fill Current Status
		if( status & TransNonCardReceipt )
		{
			CustomMsg0[0] = '*';
		}
		else
		{
			CustomMsg0[0] = ' ';
		}

		if( status & LogOnReceipt )
		{
			CustomMsg1[0] = '*';
		}
		else
		{
			CustomMsg1[0] = ' ';
		}

		if( status & LogOffReceipt )
		{
			CustomMsg2[0] = '*';
		}
		else
		{
			CustomMsg2[0] = ' ';
		}

		if( status & PriceRecvReceipt )
		{
			CustomMsg3[0] = '*';
		}
		else
		{
			CustomMsg3[0] = ' ';
		}
	}
}

/*
 ** SetPrintIt
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	PrintFlag: Print Flag Define In CRPrint.h
 *
 *  DESCRIPTION: Set The Print Bit And Save To PdProtect
 *
 *  RETURNS:
 *
 */
void SetPrintIt( unsigned int PrintFlag )
{
	unsigned int flags;

	flags = CharToIntCopy( PowerDown.PrintIt );

	flags |= PrintFlag;

	// Mark The Bits Was Set, Do Not Change Only Re-set It
	flags &= ( ~PrintItSet );
		
	IntToCharCopy( flags, PowerDown.PrintIt );
}

/*
 ** ClearPrintIt
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	PrintFlag: Print Flag Define In CRPrint.h
 *
 *  DESCRIPTION: Clear The Print Bit And Save To PdProtect
 *
 *  RETURNS:
 *
 */
void ClearPrintIt( unsigned int PrintFlag )
{
	unsigned int flags;

	flags = CharToIntCopy( PowerDown.PrintIt );

	flags &= ( ~PrintFlag );

	// Mark The Bits Was Set, Do Not Change Only Re-set It
	flags &= ( ~PrintItSet );
		
	IntToCharCopy( flags, PowerDown.PrintIt );
}


/*
 ** UpdateInversedDisplay
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	index: Next Display List Item, And Change The DispListInversedIdx
 *
 *  DESCRIPTION: Inversed Display The List Item
 *
 *  RETURNS: None.
 *
 */
void UpdateInversedDisplay( unsigned char index )
{
	// Inverse Display The Last Item
	DisplayContra( ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopX,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopY,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomX,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomY );
	
	if( index < 8 )
	{
		DispListInversedIdx = index;
	}

	// Inverse Display The Next Item
	DisplayContra( ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopX,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopY,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomX,
					ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomY );
	
		
	// Display The Screen
	DisBufToLcd();
}

/*
 ** GetUnfinishedTrans
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Read The Unfinished Trans And Fill The List CustomMsgs
 *
 *  RETURNS: None.
 *
 */
void GetUnfinishedTrans( void )
{
	unsigned char len;

	// Clear Record Space
	memset( &KeyDispTrans.JOURNALSIGN, 0x00, sizeof( KeyDispTrans ));
	
	// Fill The Buffer
	if( PowerDown.PdTrans.PD_TRANS_FLAG != TRANS_START )
	{
		// Journal Start Flag
		KeyDispTrans.JOURNALSIGN = 0xAA;

		// Transaction TTC
		memcpy( KeyDispTrans.TRANS_TTC, PowerDown.PdTrans.PD_TRANS_TTC, sizeof( KeyDispTrans.TRANS_TTC ));
			
		// Transaction Type
		KeyDispTrans.TRANS_TYPE = PowerDown.PdTrans.PD_TRANS_TYPE;
			
		// Transaction Date	And Time
		memcpy( KeyDispTrans.TRANS_DATE, PowerDown.PdTrans.PD_TRANS_DATE, sizeof( KeyDispTrans.TRANS_DATE ));
		memcpy( KeyDispTrans.TRANS_TIME, PowerDown.PdTrans.PD_TRANS_TIME, sizeof( KeyDispTrans.TRANS_TIME ));

		// IC Card ASN
		memcpy( KeyDispTrans.CARD_ASN, PowerDown.PdTrans.PD_CARD_ASN, sizeof( KeyDispTrans.CARD_ASN ));

		// IC Card Application Balance
		memcpy( KeyDispTrans.CARD_APP_BALANCE, PowerDown.PdTrans.PD_CARD_APP_BALANCE, sizeof( KeyDispTrans.CARD_APP_BALANCE ));

		// Transaction Amount
		memcpy( KeyDispTrans.TRANS_AMOUNT, &PowerDown.PdTrans.PD_TRANS_AMOUNT[1], sizeof( KeyDispTrans.TRANS_AMOUNT ));

		// IC Card Application TTC
		memcpy( KeyDispTrans.CARD_TTC, PowerDown.PdTrans.PD_CARD_APP_TTC, sizeof( KeyDispTrans.CARD_TTC ));

		// IC Card Application TAC
		if( KeyDispTrans.TRANS_TYPE & TRANSGREY )
		{
			// For Grey Trans Record, Get GTAC
			memcpy( KeyDispTrans.CARD_TAC, PowerDown.PdTrans.PD_CARD_GTAC, sizeof( KeyDispTrans.CARD_TAC ));
		}
		else
		{
			// For Not Grey Trans, Get CARD TAC
			memcpy( KeyDispTrans.CARD_TAC, PowerDown.PdTrans.PD_CARD_MAC2, sizeof( KeyDispTrans.CARD_TAC ));
		}

		// PSAM GMAC
		memcpy( KeyDispTrans.PSAM_GMAC, PowerDown.PdTrans.PD_PSAM_GMAC, sizeof( KeyDispTrans.PSAM_GMAC ));

		// PSAM TTC
		memcpy( KeyDispTrans.PSAM_TAC, PowerDown.PdTrans.PD_PSAM_TAC, sizeof( KeyDispTrans.PSAM_TAC ));

		// PSAM ASN
		memcpy( KeyDispTrans.PSAM_ASN, PowerDown.PdTrans.PD_PSAM_ASN, sizeof( KeyDispTrans.PSAM_ASN ));

		// PSAM TID
		memcpy( KeyDispTrans.PSAM_TID, PowerDown.PdTrans.PD_PSAM_TID, sizeof( KeyDispTrans.PSAM_TID ));

		// PSAM TTC
		memcpy( KeyDispTrans.PSAM_TTC, PowerDown.PdTrans.PD_PSAM_TTC, sizeof( KeyDispTrans.PSAM_TTC ));

		// Debit Source
		KeyDispTrans.TRANS_DEBITSOURCE = PowerDown.PdTrans.PD_TRANS_DEBITSOURCE;
			
		// Debit Source
		KeyDispTrans.TRANS_UNIT = PowerDown.PdTrans.PD_TRANS_UNIT;

		// IC Card Type
		KeyDispTrans.CARD_TYPE = PowerDown.PdTrans.PD_CARD_TYPE;

		// Transaction Key Information
		// bit7 - bit4: Key Index; bit3 - bit0: Key Version
		KeyDispTrans.CARD_KEY = ((( PowerDown.PdTrans.PD_PSAM_KEY_INDEX << 4 ) & 0xF0 ) | ( PowerDown.PdTrans.PD_CARD_KEY_VERSION & 0x0F ));

		// Transaction Nozzle Number
		KeyDispTrans.NOZZLENUMBER = PowerDown.PdTrans.PD_NOZZLENUMBER;

		// Grade Code
		memcpy( KeyDispTrans.GRADE_CODE, PowerDown.PdTrans.PD_GRADE_CODE, sizeof( KeyDispTrans.GRADE_CODE ));

		// Transaction Volume
		memcpy( KeyDispTrans.TRANS_VOLUME, &PowerDown.PdTrans.PD_TRANS_VOLUME[1], sizeof( KeyDispTrans.TRANS_VOLUME ));

		// Transaction Price
		memcpy( KeyDispTrans.TRANS_PRICE, PowerDown.PdTrans.PD_TRANS_PRICE, sizeof( KeyDispTrans.TRANS_PRICE ));

		// Transaction Employee Number
		KeyDispTrans.EMPLOYEE_NUMBER = PowerDown.PdTrans.PD_EMPLOYEE_NUMBER;

		// Transaction Volume Total
		memcpy( KeyDispTrans.VOLUME_TOTAL, PowerDown.PdTrans.PD_TRANS_VOLUME_TOTAL, sizeof( KeyDispTrans.VOLUME_TOTAL ));
		// Fill The Journal Buffer End
	}

	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Fill The Start Message For Unfinished Trans
	// 0xFF: Maybe First Running Or The Protect Data Was Corrupted
	if(( PowerDown.PdTrans.PD_TRANS_FLAG == TRANS_START ) || ( PowerDown.PdTrans.PD_TRANS_FLAG == 0xFF ))
	{
		// No Unfinished Trans
		// Save Reasons, No Unfinished Trans
		len = strlen( DISPMSG[M_TransFinished] );
		memcpy( CustomMsg0, DISPMSG[M_TransFinished], len );
	}
	else
	{
		// There Is A Unfinished Trans
		if( PowerDown.PdTrans.PD_TRANS_FLAG == MAC2_RECEIVED )
		{
			// Save Reasons, Can Not Read The Last Trans
			len = strlen( DISPMSG[M_Reason0] );
			memcpy( CustomMsg0, DISPMSG[M_Reason0], len );
		}
		else if( PowerDown.PdTrans.PD_TRANS_FLAG == GET_LAST_TRANS )
		{
			// Save Reasons, PSAM Error
			len = strlen( DISPMSG[M_Reason1] );
			memcpy( CustomMsg0, DISPMSG[M_Reason1], len );
		}
		else
		{
			// Save Reasons, Write Record Error
			len = strlen( DISPMSG[M_Reason2] );
			memcpy( CustomMsg0, DISPMSG[M_Reason2], len );
		}
	
		len = strlen( DISPMSG[M_ENTER] );
		memcpy( CustomMsg1, DISPMSG[M_ENTER], len );
		
		len = strlen( DISPMSG[M_CANCEL] );
		memcpy( CustomMsg2, DISPMSG[M_CANCEL], len );
		
//		len = strlen( DISPMSG[M_SELECT] );
//		memcpy( CustomMsg3, DISPMSG[M_SELECT], len );
	}
}

/*
 ** GetTrans
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Call Functions GetTrans0 To GetTrans6 According To DispListInversedIdx
 *
 *  RETURNS: None.
 *
 */
void GetTrans( void )
{
	if( DispRecordIdx == 0 )
	{
		GetTrans0();
	}
	else if(  DispRecordIdx == 1 )
	{
		GetTrans1();
	}	
	else if(  DispRecordIdx == 2 )
	{
		GetTrans2();
	}	
	else if(  DispRecordIdx == 3 )
	{
		GetTrans3();
	}	
	else if(  DispRecordIdx == 4 )
	{
		GetTrans4();
	}	
	else if(  DispRecordIdx == 5 )
	{
		GetTrans5();
	}	
	else if(  DispRecordIdx == 6 )
	{
		GetTrans6();
	}	
	else
	{
	}
}

/*
 ** GetTrans0
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Copy Trans In KeyDispTrans To CustomMsg0 To CustomMsg4 For Display
 *
 *  RETURNS: None.
 *
 *  Atten: CARD_ASN[10], TRANS_DATE[4], TRANS_TIME[3]
 *
 */
void GetTrans0( void )
{
	unsigned char len;

	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Save Card ASN
	len = strlen( DISPMSG[M_ASN] );
	memcpy( CustomMsg0, DISPMSG[M_ASN], len );
	Hex2Asc( &CustomMsg0[len], KeyDispTrans.CARD_ASN, 5 );
	Hex2Asc( &CustomMsg1[0], &KeyDispTrans.CARD_ASN[5], 5 );
	
	// Save Trans Date And Time
	len = strlen( DISPMSG[M_Time] );
	memcpy( CustomMsg2, DISPMSG[M_Time], len );
	Hex2Asc( &CustomMsg3[0], KeyDispTrans.TRANS_DATE, 7 );
}

/*
 ** GetTrans1
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Copy Trans In KeyDispTrans To CustomMsg0 To CustomMsg4 For Display
 *
 *  RETURNS: None.
 *
 *  Atten: TRANS_TYPE, TRANS_TTC[4], CARD_APP_BALANCE[4] 
 *
 */
void GetTrans1( void )
{
	unsigned char len;
	unsigned int value;

#ifdef MAKE_CASH_CR
	unsigned char type;
	unsigned int bal;
	unsigned char buf[4];
#endif

	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

#ifdef MAKE_CASH_CR
	// Get Trans Type
	type = CheckCashType( KeyDispTrans.RFU );

	if( type == 0x00 )
	{
		// Cash Acceptor Trans
		// Save Tran Type
		len = strlen( DISPMSG[M_TransType] );
		memcpy( CustomMsg0, DISPMSG[M_TransType], len );
		memcpy( &CustomMsg0[len], DISPMSG[M_CashAcceptorTrans], strlen( DISPMSG[M_CashAcceptorTrans] ));

		// Save Cash Acceptor Total
		len = strlen( DISPMSG[M_CashAcceptorTotal] );
		memcpy( CustomMsg1, DISPMSG[M_CashAcceptorTotal], len );
		value = CharToIntCopy( KeyDispTrans.RFU );
		sprintf(( char * )&CustomMsg1[len], "%03u", value );
		// Trim Decimal And Attach "元"
		TrimDecimal( &CustomMsg1[0], 1 );

		// Balance
		memset( buf, 0x00, sizeof( buf ));
		memcpy( &buf[1], KeyDispTrans.TRANS_AMOUNT, sizeof( KeyDispTrans.TRANS_AMOUNT ));
		bal = CharToIntCopy( buf );
		value = CharToIntCopy( KeyDispTrans.RFU );
		if( value > bal )
		{
			value -= bal;
		}
		else
		{
			value = 0;
		}

		len = strlen( DISPMSG[M_CashBalance] );
		memcpy( CustomMsg3, DISPMSG[M_CashBalance], len );
		sprintf(( char * )&CustomMsg3[len], "%03u", value );
		// Trim Decimal And Attach "元"
		TrimDecimal( &CustomMsg3[0], 1 );
	}
	else if( type == 0x01 )
	{
		// Cash POS Trans
		// Save Tran Type
		len = strlen( DISPMSG[M_TransType] );
		memcpy( CustomMsg0, DISPMSG[M_TransType], len );
		memcpy( &CustomMsg0[len], DISPMSG[M_CashPOSTrans], strlen( DISPMSG[M_CashPOSTrans] ));

		// Save Cash POS Total
		len = strlen( DISPMSG[M_CashPOSTotal] );
		memcpy( CustomMsg1, DISPMSG[M_CashPOSTotal], len );
		value = CharToIntCopy( KeyDispTrans.RFU );
		sprintf(( char * )&CustomMsg1[len], "%03u", value );
		// Trim Decimal And Attach "元"
		TrimDecimal( &CustomMsg1[0], 1 );

		// Auth Code
		len = strlen( DISPMSG[M_CashPOSAuthCode] );
		memcpy( CustomMsg2, DISPMSG[M_CashPOSAuthCode], len );
		type = KeyDispTrans.RFU[4];
		
		if( type > 10 )
		{
			// Max: 10
			type = 10;
		}

		if( type % 2 )
		{
			Hex2Asc( &CustomMsg2[len], &KeyDispTrans.RFU[5], ( type / 2 ) + 1 );
			// Replace Last Byte With ' '
			len = strlen(( char * )&CustomMsg2[0] );
			CustomMsg2[len - 1] = ' ';
		}
		else
		{
			Hex2Asc( &CustomMsg2[len], &KeyDispTrans.RFU[5], type / 2 );
		}

		// Balance
		memset( buf, 0x00, sizeof( buf ));
		memcpy( &buf[1], KeyDispTrans.TRANS_AMOUNT, sizeof( KeyDispTrans.TRANS_AMOUNT ));
		bal = CharToIntCopy( buf );
		value = CharToIntCopy( KeyDispTrans.RFU );
		if( value > bal )
		{
			value -= bal;
		}
		else
		{
			value = 0;
		}

		len = strlen( DISPMSG[M_CashBalance] );
		memcpy( CustomMsg3, DISPMSG[M_CashBalance], len );
		sprintf(( char * )&CustomMsg3[len], "%03u", value );
		// Trim Decimal And Attach "元"
		TrimDecimal( &CustomMsg3[0], 1 );
	}
	else
	{
		// IC Card Trans, Displaly Card Message
		// Save Tran Type
		len = strlen( DISPMSG[M_TransType] );
		memcpy( CustomMsg0, DISPMSG[M_TransType], len );
		GetTransType( &CustomMsg0[len], KeyDispTrans.TRANS_TYPE );

		// Save TTC
		len = strlen( DISPMSG[M_TTC] );
		memcpy( CustomMsg1, DISPMSG[M_TTC], len );
		value = CharToIntCopy( KeyDispTrans.TRANS_TTC );
		sprintf(( char * )&CustomMsg1[len], "%u", value );

		// Save Card Balance
		len = strlen( DISPMSG[M_Balance] );
		memcpy( CustomMsg2, DISPMSG[M_Balance], len );
		value = CharToIntCopy( KeyDispTrans.CARD_APP_BALANCE );
		sprintf(( char * )&CustomMsg3[0], "%03u", value );
		if( KeyDispTrans.TRANS_UNIT & 0x01 )
		{
			// Trim Decimal And Attach "点"
			TrimDecimal( &CustomMsg3[0], 2 );
		}
		else
		{
			// Trim Decimal And Attach "元"
			TrimDecimal( &CustomMsg3[0], 1 );
		}
	}
#else
	// Save Tran Type
	len = strlen( DISPMSG[M_TransType] );
	memcpy( CustomMsg0, DISPMSG[M_TransType], len );
	GetTransType( &CustomMsg0[len], KeyDispTrans.TRANS_TYPE );

	// Save TTC
	len = strlen( DISPMSG[M_TTC] );
	memcpy( CustomMsg1, DISPMSG[M_TTC], len );
	value = CharToIntCopy( KeyDispTrans.TRANS_TTC );
	sprintf(( char * )&CustomMsg1[len], "%u", value );

	// Save Card Balance
	len = strlen( DISPMSG[M_Balance] );
	memcpy( CustomMsg2, DISPMSG[M_Balance], len );
	value = CharToIntCopy( KeyDispTrans.CARD_APP_BALANCE );
	sprintf(( char * )&CustomMsg3[0], "%03u", value );
	if( KeyDispTrans.TRANS_UNIT & 0x01 )
	{
		// Trim Decimal And Attach "点"
		TrimDecimal( &CustomMsg3[0], 2 );
	}
	//Douzhq 20140610 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	else if (KeyDispTrans.TRANS_DEBITSOURCE == 0x04 && PowerDown.SaveOilAppMode == 0x01)
	{
	  	// Trim Decimal And Attach "升"
		TrimDecimal( &CustomMsg3[0], 3 );
	}
#endif
	else
	{
		// Trim Decimal And Attach "元"
		TrimDecimal( &CustomMsg3[0], 1 );
	}
#endif
}

/*
 ** GetTrans2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Copy Trans In KeyDispTrans To CustomMsg0 To CustomMsg4 For Display
 *
 *  RETURNS: None.
 *
 *  Atten: TRANS_AMOUNT[3], TRANS_VOLUME[3], TRANS_PRICE[2], 
 *
 */
void GetTrans2( void )
{
	unsigned char len;
	unsigned char buf[4];
	unsigned int value;

	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Save Tran Amount
	len = strlen( DISPMSG[M_Amount] );
	memcpy( CustomMsg0, DISPMSG[M_Amount], len );
	memset( buf, 0x00, sizeof( buf ));
	memcpy( &buf[1], KeyDispTrans.TRANS_AMOUNT, 3 );
	value = CharToIntCopy( buf );
	sprintf(( char * )&CustomMsg0[len], "%03u", value );
	if( KeyDispTrans.TRANS_UNIT & 0x01 )
	{
		TrimDecimal( &CustomMsg0[0], 2 );
	}
	else
	{
		TrimDecimal( &CustomMsg0[0], 1 );
	}

	// Save Trans Volume
	len = strlen( DISPMSG[M_Volume] );
	memcpy( CustomMsg1, DISPMSG[M_Volume], len );
	memset( buf, 0x00, sizeof( buf ));
	memcpy( &buf[1], KeyDispTrans.TRANS_VOLUME, 3 );
	value = CharToIntCopy( buf );
	sprintf(( char * )&CustomMsg1[len], "%03u", value );
	TrimDecimal( &CustomMsg1[len], 3 );

	// Save Trans Price
	len = strlen( DISPMSG[M_Price] );
	memcpy( CustomMsg2, DISPMSG[M_Price], len );
	memset( buf, 0x00, sizeof( buf ));
	memcpy( &buf[2], KeyDispTrans.TRANS_PRICE, 2 );
	value = CharToIntCopy( buf );
	sprintf(( char * )&CustomMsg2[len], "%03u", value );
	TrimDecimal( &CustomMsg2[len], 1 );
}

/*
 ** GetTrans3
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Copy Trans In KeyDispTrans To CustomMsg0 To CustomMsg4 For Display
 *
 *  RETURNS: None.
 *
 *  Atten: VOLUME_TOTAL[4], NOZZLENUMBER, EMPLOYEE_NUMBER, GRADE_CODE[2] 
 *
 */
void GetTrans3( void )
{
	unsigned char len;
	unsigned int value;

	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Save Volume Total
	len = strlen( DISPMSG[M_VolumeTotalDisp] );
	memcpy( CustomMsg0, DISPMSG[M_VolumeTotalDisp], len );
	value = CharToIntCopy( KeyDispTrans.VOLUME_TOTAL );
	sprintf(( char * )&CustomMsg1[0], "%03u", value );
	// Trim Decimal And Attach "升"
	TrimDecimal( &CustomMsg1[0], 3 );

	// Save Tran Nozzle Number And Employee Number
	len = strlen( DISPMSG[M_NozzleNumber] );
	memcpy( CustomMsg2, DISPMSG[M_NozzleNumber], len );
	sprintf(( char * )&CustomMsg2[len], "%u", KeyDispTrans.NOZZLENUMBER );
	
	// Employee Number In Same Line
	len = strlen( ( char * )CustomMsg2 );
	CustomMsg2[len] = ' ';
	len = strlen(( char * )CustomMsg2 );
	memcpy( &CustomMsg2[len], DISPMSG[M_EmployeeNumber], strlen( DISPMSG[M_EmployeeNumber] ));
	len = strlen( ( char * )CustomMsg2 );
	sprintf(( char * )&CustomMsg2[len], "%u", KeyDispTrans.EMPLOYEE_NUMBER );

	// Save Trans Product Code
	len = strlen( DISPMSG[M_Grade] );
	memcpy( CustomMsg3, DISPMSG[M_Grade], len );
	Hex2Asc( &CustomMsg3[len], KeyDispTrans.GRADE_CODE, 2 );
}

/*
 ** GetTrans4
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Copy Trans In KeyDispTrans To CustomMsg0 To CustomMsg4 For Display
 *
 *  RETURNS: None.
 *
 *  Atten: CARD_TTC[2], CARD_TAC[4], TRANS_DEBITSOURCE, TRANS_UNIT, CARD_TYPE, CARD_KEY
 *
 */
void GetTrans4( void )
{
	unsigned char len;
	unsigned short int value;

	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Save Card TTC
	len = strlen( DISPMSG[M_CTC] );
	memcpy( CustomMsg0, DISPMSG[M_CTC], len );
	value = CharToShortIntCopy( KeyDispTrans.CARD_TTC );
	sprintf(( char * )&CustomMsg0[len], "%u", value );

	// Save Card TAC
	len = strlen( DISPMSG[M_TAC] );
	memcpy( CustomMsg1, DISPMSG[M_TAC], len );
	Hex2Asc( &CustomMsg1[len], KeyDispTrans.CARD_TAC, 4 );

	// Save Tran Debit Source
	len = strlen( DISPMSG[M_DS] );
	memcpy( CustomMsg2, DISPMSG[M_DS], len );
	sprintf(( char * )&CustomMsg2[len], "%X", KeyDispTrans.TRANS_DEBITSOURCE );

	// Card Trans Unit Is In Same Line
	len = strlen(( char * )CustomMsg2 );
	CustomMsg2[len] = ' ';
	len = strlen(( char * )CustomMsg2 );
	memcpy( &CustomMsg2[len], DISPMSG[M_UNIT], strlen( DISPMSG[M_UNIT] ));
	len = strlen(( char * )CustomMsg2 );
	sprintf(( char * )&CustomMsg2[len], "%X", KeyDispTrans.TRANS_UNIT );

	// Save Trans Key Version & CardType
	len = strlen( DISPMSG[M_KeyVersion] );
	memcpy( CustomMsg3, DISPMSG[M_KeyVersion], len );
	sprintf(( char * )&CustomMsg3[len], "%X", KeyDispTrans.CARD_KEY );

	// Card Type Is In Same Line
	len = strlen(( char * )CustomMsg3 );
	CustomMsg3[len] = ' ';
	len = strlen(( char * )CustomMsg3 );
	memcpy( &CustomMsg3[len], DISPMSG[M_CardType], strlen( DISPMSG[M_CardType] ));
	len = strlen(( char * )CustomMsg3 );
	sprintf(( char * )&CustomMsg3[len], "%X", KeyDispTrans.CARD_TYPE );
}

/*
 ** GetTrans5
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Copy Trans In KeyDispTrans To CustomMsg0 To CustomMsg4 For Display
 *
 *  RETURNS: None.
 *
 *  Atten: PSAM_GMAC[4], PSAM_TAC[4], PSAM_ASN[10]
 *
 */
void GetTrans5( void )
{
	unsigned char len;

	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Save Tran PSAM GAMC
	len = strlen( DISPMSG[M_GMAC] );
	memcpy( CustomMsg0, DISPMSG[M_GMAC], len );
	Hex2Asc( &CustomMsg0[len], KeyDispTrans.PSAM_GMAC, 4 );

	// Save Trans PSAM TAC
	len = strlen( DISPMSG[M_SAM_TAC] );
	memcpy( CustomMsg1, DISPMSG[M_SAM_TAC], len );
	Hex2Asc( &CustomMsg1[len], KeyDispTrans.PSAM_TAC, 4 );

	// Save Trans PSAM ASN
	len = strlen( DISPMSG[M_SAM_ASN] );
	memcpy( CustomMsg2, DISPMSG[M_SAM_ASN], len );
	Hex2Asc( &CustomMsg2[len], KeyDispTrans.PSAM_ASN, 4 );
	Hex2Asc( &CustomMsg3[0], &KeyDispTrans.PSAM_ASN[4], 6 );
}

/*
 ** GetTrans6
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Copy Trans In KeyDispTrans To CustomMsg0 To CustomMsg4 For Display
 *
 *  RETURNS: None.
 *
 *  Atten: PSAM_TID[6], PSAM_TTC[4], TRANS_TAC[4]
 *
 */
void GetTrans6( void )
{
	unsigned char len;
	unsigned int value;

#ifdef MAKE_CARD_MODE2
	unsigned char len0;
#endif

	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Save PSAM TID
	len = strlen( DISPMSG[M_SAM_TID] );
	memcpy( CustomMsg0, DISPMSG[M_SAM_TID], len );
	Hex2Asc( &CustomMsg0[len], KeyDispTrans.PSAM_TID, 6 );

	// Save Tran PSAM TTC
	len = strlen( DISPMSG[M_SAM_TTC] );
	memcpy( CustomMsg1, DISPMSG[M_SAM_TTC], len );
	value = CharToIntCopy( KeyDispTrans.PSAM_TTC );
	sprintf(( char * )&CustomMsg1[len], "%u", value );

	// Save Trans Terminal TAC
	len = strlen( DISPMSG[M_TerminalTAC] );
	memcpy( CustomMsg2, DISPMSG[M_TerminalTAC], len );
	Hex2Asc( &CustomMsg2[len], KeyDispTrans.TRANS_TAC, 4 );

#ifdef MAKE_CARD_MODE2
	if( KeyDispTrans.JOURNALSIGN == 0x55 )
	{
		len = strlen( DISPMSG[M_TransUploaded] );
		memcpy( CustomMsg3, DISPMSG[M_TransUploaded], len );
	}
	else if( KeyDispTrans.JOURNALSIGN == 0x5A )
	{
		len = strlen( DISPMSG[M_TransSaveToEMPCard] );
		memcpy( CustomMsg3, DISPMSG[M_TransSaveToEMPCard], len );
		CustomMsg3[len]	= ' ';
		len += 1;
		len0 = strlen( DISPMSG[M_TransUnUploaded] );
		memcpy( &CustomMsg3[len], DISPMSG[M_TransUnUploaded], len0 );
	}
	else
	{
		len = strlen( DISPMSG[M_TransNotSaveToEMPCard] );
		memcpy( CustomMsg3, DISPMSG[M_TransNotSaveToEMPCard], len );
		CustomMsg3[len]	= ' ';
		len += 1;
		len0 = strlen( DISPMSG[M_TransUnUploaded] );
		memcpy( &CustomMsg3[len], DISPMSG[M_TransUnUploaded], len0 );
	}
#endif

}


/*
 ** GetTransType
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	pDest: Dest Pointer To Save The Trans Type; TransType: Trans Type Byte
 *
 *  DESCRIPTION: Change The TranType Byte To Chinese For Display
 *
 *  RETURNS: None.
 *
 */
void GetTransType( unsigned char *pDest, unsigned char TransType )
{
	unsigned char type;

	type = ( TransType & 0x0F );

	if( type == TRANSOK )
	{
		memcpy( pDest, DISPMSG[M_TransTypeOk], strlen( DISPMSG[M_TransTypeOk] ));
	}
	else if( type == TRANSGREY )
	{
		memcpy( pDest, DISPMSG[M_TransTypeGrey], strlen( DISPMSG[M_TransTypeGrey] ));
	}
	else if( type == TRANGREYUNLOCK )
	{
		memcpy( pDest, DISPMSG[M_TransTypeGreyUnlock], strlen( DISPMSG[M_TransTypeGreyUnlock] ));
	}
	else if( type == TRANSGREYFLAG )
	{
		memcpy( pDest, DISPMSG[M_TransTypeGreyFlag], strlen( DISPMSG[M_TransTypeGreyFlag] ));
	}
	else if( type == TRANSLOGON )
	{
		memcpy( pDest, DISPMSG[M_TransTypeLogOn], strlen( DISPMSG[M_TransTypeLogOn] ));
	}
	else if( type == TRANSLOGOFF )
	{
		memcpy( pDest, DISPMSG[M_TransTypeLogOff], strlen( DISPMSG[M_TransTypeLogOff] ));
	}
	else if( type == TRANSNONCARD )
	{
		memcpy( pDest, DISPMSG[M_TransTypeNonCard], strlen( DISPMSG[M_TransTypeNonCard] ));
	}
	else if( type == TRANPRICERECV )
	{
		memcpy( pDest, DISPMSG[M_TransTypePriceRecv], strlen( DISPMSG[M_TransTypePriceRecv] ));
	}
	else if( type == TRANSCARDERR )
	{
		memcpy( pDest, DISPMSG[M_TransTypeCardErr], strlen( DISPMSG[M_TransTypeCardErr] ));
	}
#ifdef MAKE_CARD_MODE2
	else if( type == TRANSTYPERFU )
	{
		memcpy( pDest, DISPMSG[M_TransTypeRFU], strlen( DISPMSG[M_TransTypeRFU] ));
	}
	else if( type == TRANSONLINEABN )
	{
		memcpy( pDest, DISPMSG[M_TransTypeABN], strlen( DISPMSG[M_TransTypeABN] ));
	}
	else if( type == TRANSNONCARDSS )
	{
		memcpy( pDest, DISPMSG[M_TransTypeNonCardSS], strlen( DISPMSG[M_TransTypeNonCardSS] ));
	}
	else if( type == TRANSHOSTAUTH )
	{
		memcpy( pDest, DISPMSG[M_TransTypeHostAuth], strlen( DISPMSG[M_TransTypeHostAuth] ));
	}
	else if( type == TRANSCANCELAUTH )
	{
		memcpy( pDest, DISPMSG[M_TransTypeAuthCancel], strlen( DISPMSG[M_TransTypeAuthCancel] ));
	}
	else if( type == TRANSOVERFUEL )
	{
		memcpy( pDest, DISPMSG[M_TransTypeOverFuel], strlen( DISPMSG[M_TransTypeOverFuel] ));
	}
#endif	// MAKE_CARD_MODE2
	else
	{
		// type == 2; Error Card
		memcpy( pDest, DISPMSG[M_TransTypeErrorCard], strlen( DISPMSG[M_TransTypeErrorCard] ));
	}
}



/*
 ** TrimDecimal
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	pBuffer: Pointer To The Ascii Buffer To Be Trimed
 *				type: 0: No Attached;
 *					  1: Attach "元" At The End;
 *					  2: Attach "点" At The End;
 *					  3: Attach "升" At The End;
 *
 *
 *  DESCRIPTION: Trim 2 Decimal Point To The Ascii Buffer
 *
 *  RETURNS: None.
 *
 */
void TrimDecimal( unsigned char *pBuffer, unsigned char type )
{
	unsigned char len;

	len = strlen(( char * )pBuffer );

	*( pBuffer + len ) = *( pBuffer + len - 1 );
	*( pBuffer + len - 1 ) = *( pBuffer + len - 2 );
	*( pBuffer + len - 2 ) = '.';

	// Replace ' ' With '0'
	if( *( pBuffer + len - 3 ) == ' ' )
	{
		*( pBuffer + len - 3 ) = '0';
	}

	if( *( pBuffer + len - 1 ) == ' ' )
	{
		*( pBuffer + len - 1 ) = '0';
	}

	len = strlen(( char * )pBuffer );
	pBuffer += len;
	// Attached
	if( type == 1 )
	{
		memcpy( pBuffer, "元", 2 );
	}
	else if( type == 2 )
	{
		memcpy( pBuffer, "点", 2 );
	}
	else if( type == 3 )
	{
		memcpy( pBuffer, "升", 2 );
	}
	else
	{
		// Do Not Attached
	}
}


/*
 ** GetStationInfo
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get Station Information And Arrange To CustomMsg0 To 3 For Display
 *
 *  RETURNS: None.
 *
 */
void GetStationInfo( void )
{
	unsigned char len;
	unsigned char tmp;
	unsigned int value;

	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Save Province And City
	len = strlen( DISPMSG[M_Province] );
	memcpy( CustomMsg0, DISPMSG[M_Province], len );
	tmp = GetProv();
	Hex2Asc( &CustomMsg0[len], &tmp, 1 );
	len = strlen(( char * )CustomMsg0 );
	memcpy( &CustomMsg0[len], "  ", 2 );
	len = strlen(( char * )CustomMsg0 );
	memcpy( &CustomMsg0[len], DISPMSG[M_City], strlen( DISPMSG[M_City] ));
	len = strlen(( char * )CustomMsg0 );
	tmp = GetCity();
	Hex2Asc( &CustomMsg0[len], &tmp, 1 );
	
	// Save Suprior
	len = strlen( DISPMSG[M_Superior] );
	memcpy( CustomMsg1, DISPMSG[M_Superior], len );
	value = GetSuperior();
	Hex2Asc( &CustomMsg1[len], ( unsigned char * )&value, 4 );

	// Save Station ID
	len = strlen( DISPMSG[M_StationID] );
	memcpy( CustomMsg2, DISPMSG[M_StationID], len );
	value = GetS_ID();
	Hex2Asc( &CustomMsg2[len], ( unsigned char * )&value, 4 );

	// Save Station ID
	len = strlen( DISPMSG[M_POSP] );
	memcpy( CustomMsg3, DISPMSG[M_POSP], len );
	tmp = GetPOS_P();
	sprintf(( char * )&CustomMsg3[len], "%u", tmp );
}


/*
 ** GetDispRecordSpace
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get The Record Space Inform And Arrange To CustomMsg0 And CustomMsg1 For Display
 *
 *  RETURNS: None.
 *
 */
void GetDispRecordSpace( void )
{
	unsigned char len;
	unsigned int value;

	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

	// Save Record Space
	len = strlen( DISPMSG[M_RecordSpaceNum] );
	memcpy( CustomMsg0, DISPMSG[M_RecordSpaceNum], len );
	value = GetRecordSpace();
	sprintf(( char * )&CustomMsg0[len], "%u", value );

	// Save Records Need To Upload To POS
	len = strlen( DISPMSG[M_RecordNeedUpload] );
	memcpy( CustomMsg1, DISPMSG[M_RecordNeedUpload], len );
	value = ( RecordMax - value );
	sprintf(( char * )&CustomMsg1[len], "%01u", value );
}


/*
 ** GetVolumeTotal
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get And Arrange Volume Total To CustomMsg0 To CustomMsg3 For Display
 *
 *  RETURNS: None.
 *
 */
void GetVolumeTotal( void )
{
	unsigned char len;
	unsigned char PhNozzle;
	unsigned char NozzleNum;
	unsigned int value;
	unsigned char *pCmsg;
	
	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Get Total Nozzle Number
	NozzleNum = GetGUN_N();

	for( PhNozzle = 0; PhNozzle < NozzleNum; PhNozzle++ )
	{
		if( PhNozzle == 0 )
		{
			pCmsg = &CustomMsg0[0];
		}
		else if( PhNozzle == 1 )
		{
			pCmsg = &CustomMsg1[0];
		}
		else if( PhNozzle == 2 )
		{
			pCmsg = &CustomMsg2[0];
		}
		else
		{
			pCmsg = &CustomMsg3[0];
		}
		
		// Fill The Nozzles
		sprintf(( char * )pCmsg, "%02u: ", DISP_CR_Buf.DispTotal[PhNozzle].LogicalNozzle );
		len = strlen(( char * )pCmsg );
		
		value = CharToIntCopy( DISP_CR_Buf.DispTotal[PhNozzle].VolumeTotal );
		sprintf(( char * )pCmsg + len, "%03u", value );

		TrimDecimal( pCmsg, 0 );
	}
}

/*
 ** GetAmountTotal
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get And Arrange Amount Total To CustomMsg0 To CustomMsg3 For Display
 *
 *  RETURNS: None.
 *
 */
void GetAmountTotal( void )
{
	unsigned char len;
	unsigned char PhNozzle;
	unsigned char NozzleNum;
	unsigned int value;
	unsigned char *pCmsg;
	
	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Get Total Nozzle Number
	NozzleNum = GetGUN_N();

	for( PhNozzle = 0; PhNozzle < NozzleNum; PhNozzle++ )
	{
		if( PhNozzle == 0 )
		{
			pCmsg = &CustomMsg0[0];
		}
		else if( PhNozzle == 1 )
		{
			pCmsg = &CustomMsg1[0];
		}
		else if( PhNozzle == 2 )
		{
			pCmsg = &CustomMsg2[0];
		}
		else
		{
			pCmsg = &CustomMsg3[0];
		}
		
		// Fill The Nozzles
		sprintf(( char * )pCmsg, "%02u: ", DISP_CR_Buf.DispTotal[PhNozzle].LogicalNozzle );
		len = strlen(( char * )pCmsg );
		
		value = CharToIntCopy( DISP_CR_Buf.DispTotal[PhNozzle].AmountTotal );
		sprintf(( char * )pCmsg + len, "%03u", value );

		TrimDecimal( pCmsg, 0 );
	}
}

/*
 ** GetVolumeTotalLogOn
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get And Arrange Log On Volume Total To CustomMsg0 To CustomMsg3 For Display
 *
 *  RETURNS: None.
 *
 */
void GetVolumeTotalLogOn( void )
{
	unsigned char len;
	unsigned char PhNozzle;
	unsigned char NozzleNum;
	unsigned int value;
	unsigned char *pCmsg;
	
	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Get Total Nozzle Number
	NozzleNum = GetGUN_N();

	for( PhNozzle = 0; PhNozzle < NozzleNum; PhNozzle++ )
	{
		if( PhNozzle == 0 )
		{
			pCmsg = &CustomMsg0[0];
		}
		else if( PhNozzle == 1 )
		{
			pCmsg = &CustomMsg1[0];
		}
		else if( PhNozzle == 2 )
		{
			pCmsg = &CustomMsg2[0];
		}
		else
		{
			pCmsg = &CustomMsg3[0];
		}
		
		// Fill The Nozzles
		sprintf(( char * )pCmsg, "%02u: ", DISP_CR_Buf.DispTotal[PhNozzle].LogicalNozzle );
		len = strlen(( char * )pCmsg );
		
		value = CharToIntCopy( DISP_CR_Buf.DispTotal[PhNozzle].DutyVolumeTotal );
		sprintf(( char * )pCmsg + len, "%03u", value );

		TrimDecimal( pCmsg, 0 );
	}
}

/*
 ** GetAmountTotalLogOn
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get And Arrange Log On Amount Total To CustomMsg0 To CustomMsg3 For Display
 *
 *  RETURNS: None.
 *
 */
void GetAmountTotalLogOn( void )
{
	unsigned char len;
	unsigned char PhNozzle;
	unsigned char NozzleNum;
	unsigned int value;
	unsigned char *pCmsg;
	
	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Get Total Nozzle Number
	NozzleNum = GetGUN_N();

	for( PhNozzle = 0; PhNozzle < NozzleNum; PhNozzle++ )
	{
		if( PhNozzle == 0 )
		{
			pCmsg = &CustomMsg0[0];
		}
		else if( PhNozzle == 1 )
		{
			pCmsg = &CustomMsg1[0];
		}
		else if( PhNozzle == 2 )
		{
			pCmsg = &CustomMsg2[0];
		}
		else
		{
			pCmsg = &CustomMsg3[0];
		}
		
		// Fill The Nozzles
		sprintf(( char * )pCmsg, "%02u: ", DISP_CR_Buf.DispTotal[PhNozzle].LogicalNozzle );
		len = strlen(( char * )pCmsg );
		
		value = CharToIntCopy( DISP_CR_Buf.DispTotal[PhNozzle].DutyAmountTotal );
		sprintf(( char * )pCmsg + len, "%03u", value );

		TrimDecimal( pCmsg, 0 );
	}
}

#ifdef MAKE_CASH_CR
/*
 ** GetCashVolumeTotalLogOn
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get And Arrange Log On Cash Volume Total To CustomMsg0 To CustomMsg3 For Display
 *
 *  RETURNS: None.
 *
 */
void GetCashVolumeTotalLogOn( void )
{
	unsigned char len;
	unsigned char PhNozzle;
	unsigned char NozzleNum;
	unsigned int value;
	unsigned char *pCmsg;
	
	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Get Total Nozzle Number
	NozzleNum = GetGUN_N();

	for( PhNozzle = 0; PhNozzle < NozzleNum; PhNozzle++ )
	{
		if( PhNozzle == 0 )
		{
			pCmsg = &CustomMsg0[0];
		}
		else if( PhNozzle == 1 )
		{
			pCmsg = &CustomMsg1[0];
		}
		else if( PhNozzle == 2 )
		{
			pCmsg = &CustomMsg2[0];
		}
		else
		{
			pCmsg = &CustomMsg3[0];
		}
		
		// Fill The Nozzles
		sprintf(( char * )pCmsg, "%02u: ", DISP_CR_Buf.DispTotal[PhNozzle].LogicalNozzle );
		len = strlen(( char * )pCmsg );
		
		value = CharToIntCopy( DISP_CR_Buf.DispTotal[PhNozzle].DutyCashVolumeTotal );
		sprintf(( char * )pCmsg + len, "%03u", value );

		TrimDecimal( pCmsg, 0 );
	}
}

/*
 ** GetCashAmountTotalLogOn
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get And Arrange Log On Cash Amount Total To CustomMsg0 To CustomMsg3 For Display
 *
 *  RETURNS: None.
 *
 */
void GetCashAmountTotalLogOn( void )
{
	unsigned char len;
	unsigned char PhNozzle;
	unsigned char NozzleNum;
	unsigned int value;
	unsigned char *pCmsg;
	
	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Get Total Nozzle Number
	NozzleNum = GetGUN_N();

	for( PhNozzle = 0; PhNozzle < NozzleNum; PhNozzle++ )
	{
		if( PhNozzle == 0 )
		{
			pCmsg = &CustomMsg0[0];
		}
		else if( PhNozzle == 1 )
		{
			pCmsg = &CustomMsg1[0];
		}
		else if( PhNozzle == 2 )
		{
			pCmsg = &CustomMsg2[0];
		}
		else
		{
			pCmsg = &CustomMsg3[0];
		}
		
		// Fill The Nozzles
		sprintf(( char * )pCmsg, "%02u: ", DISP_CR_Buf.DispTotal[PhNozzle].LogicalNozzle );
		len = strlen(( char * )pCmsg );
		
		value = CharToIntCopy( DISP_CR_Buf.DispTotal[PhNozzle].DutyCashAmountTotal );
		sprintf(( char * )pCmsg + len, "%03u", value );

		TrimDecimal( pCmsg, 0 );
	}
}

#endif

/*
 ** GetPriceInfo
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get And Arrange Price To CustomMsg0 To CustomMsg3 For Display
 *
 *  RETURNS: None.
 *
 */
void GetPriceInfo( void )
{
	unsigned char len;
	unsigned char PhNozzle;
	unsigned char NozzleNum;
	unsigned char LoNozzle;
	unsigned char *pCmsg;
	unsigned short int tmp2;
	
	// Clear The CustomMsgs
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));
	memset( CustomMsg3, 0x00, sizeof( CustomMsg3 ));

	// Get Total Nozzle Number
	NozzleNum = GetGUN_N();

	for( PhNozzle = 0; PhNozzle < NozzleNum; PhNozzle++ )
	{
		if( PhNozzle == 0 )
		{
			pCmsg = &CustomMsg0[0];
		}
		else if( PhNozzle == 1 )
		{
			pCmsg = &CustomMsg1[0];
		}
		else if( PhNozzle == 2 )
		{
			pCmsg = &CustomMsg2[0];
		}
		else
		{
			pCmsg = &CustomMsg3[0];
		}
		
		// Get Price
		tmp2 = GetPrice( PhNozzle, 0 );
		// Get Logical Nozzle Number
		LoNozzle = GetLogicNzn( PhNozzle );

		// Fill The Nozzles
		sprintf(( char * )pCmsg, "%02u: ", LoNozzle );
		len = strlen(( char * )pCmsg );
		
		// Fill The Price
		sprintf(( char * )pCmsg + len, "%03u", tmp2 );

		TrimDecimal( pCmsg, 0 );
	}
}

/*
 ** GetPSAMCheck
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Fill PSAM Check Status To CustomMsg0 And CustomMsg1 For Display
 *
 *  RETURNS: None.
 *
 */
void GetPSAMCheck( void )
{
	// Clear CustomMsg0
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));

	// Copy Display Items To CustomMsg0	According To PowerDown.SpecialFunction
	if( PowerDown.SpecialFunction & CheckPSAMValidate )
	{
		memcpy( CustomMsg0, DISPMSG[M_PSAMCheck], strlen( DISPMSG[M_PSAMCheck] ));
	}
	else
	{
		memcpy( CustomMsg0, DISPMSG[M_PSAMCheckNot], strlen( DISPMSG[M_PSAMCheckNot] ));
	}
}





/*
 ** GetVersionDispMessage
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get The Display Message For Display On Version Screen
 *
 *  RETURNS: None.
 *
 *	ATTN: Include: Card Reader Type; Receipt Title etc.
 *
 */
void GetVersionDispMessage( void )
{
	unsigned char i, j;
	unsigned char buf[40];
/*20131021 qiyong modify begin
	// Display Version Number
	memset( CustomMsg0, 0x00, sizeof( CustomMsg0 ));

#ifdef MAKE_CASH_CR
	// For Cash Functions, Display The CR-E Version
	memcpy( CustomMsg0, Cash_CR_Buf.CashVersion, strlen(( char * )Cash_CR_Buf.CashVersion ));
#else
	memcpy( CustomMsg0, TRINP.TR_CR_VERSION, strlen(( char * )TRINP.TR_CR_VERSION ));
#endif
20131021 qiyong modify end*/
	// Display Receipt Title
	memset( buf, 0x00, sizeof( buf ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memset( CustomMsg2, 0x00, sizeof( CustomMsg2 ));

	if( PowerDown.LocalPrinter == PrintLocal )
	{
		// Only Display The Ticket Head For Local Printer
		// First Get Title To buf
		ReadTicketHead((char *)buf );

		// Check The ASCII Character( Not Chinese ) In First 16 Bytes
		j = 0;
		for( i = 0; i < 16; i++ )
		{
			if( buf[i] <= 0xA0 )
			{
				j ^= 0x01;
			}
		}

		// Copy buf to CustomMsgs For Display
		memcpy( CustomMsg1, buf, 16 - j );
		memcpy( CustomMsg2, &buf[16-j], 16 );
	}
}


/*
 ** DispIdle
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Keypad\Source\Keypad.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Return To Display The Idle Screen According To "ProcStatus[PROCESS_CR] & CARD_READY"
 *				 And Display The Default Info Line
 *
 *  RETURNS: None.
 *
 */
void DispIdle( void )
{
	// Display Insert Card Or Balance Screen
	if( ProcStatus[PROCESS_CR] & CARD_READY )
	{
		NextScreenID = DEFAULT_SCR_BALANCE;
		ScreenID = NextScreenID;
		DrawBalanceSCR();
	}
	else                                                           
	{
		NextScreenID = DEFAULT_SCR_CARD;
		ScreenID = NextScreenID;
		DrawCardInSCR();
	}

	// Clear InfoLine, And Display default
	FillInfoMsg( M_DefaultInfo, 0, 0 );
}
