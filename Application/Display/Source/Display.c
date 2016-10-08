
#include "global.h"

#include "define.h"

#include "CRVaribles.h"
#include "Pdprotect.h"
#include "CRMemCopy.h"
#include "Display.h"
#include "Screen.h"
#include "Message.h"
#include "DispPumpMsg.h"
#include "WorkMode.h"

#include "KeyMask.h"
#include "Keypad.h"
#include "Overfuel.h"

//Douzhq 20140620 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
#include "SocialCard.h"
#endif

unsigned int LastDispAmount = 0;
//Douzhq 20140613 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
unsigned int LastDispVolume = 0;
#endif

//Douzhq 20140826 Add , Car Number recognize
#ifdef MAKE_CASH_CR
#include "SocialCard.h"
unsigned char CarNumberTmpNum;
#endif

#ifdef MAKE_CASH_CR
void CarNumberWaitTimeout(void)
{
	//static unsigned char num = 0;
	CarNumberTmpNum++;
	
	ClearLcd(DispTransLeftX + 2,DispTransLine3 + 1,DispTransLeftX + 123,DispTransLine3 + 11);
	
	if (CarNumberTmpNum == CarWaitBlack)
	{
		CarNumberTmpNum = 0;
		DisplayContra(DispTransLeftX + 2,DispTransLine3 + 1,DispTransLeftX + 123,DispTransLine3 + 11);
		//StopTimer(CarNumberRecWait);
		CarOverFlag2 = 1;
	}
	else
	{
		DisplayContra(DispTransLeftX + 2,DispTransLine3 + 1,DispTransLeftX + 2 + CarNumberTmpNum * (123/CarWaitBlack),DispTransLine3 + 11);
	}
	
	DisBufToLcd();
}
#endif
/*
 ** DisplayProcess
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Main Entrance For Display Process
 *
 *  RETURNS: None.
 *
 */
void DisplayProcess( void )
{
//	unsigned int retval;

	// First Check Display Price Update Or Not
	if( ProcStatus[PROCESS_SCHEDULE] & PRICE_UPDATING )
	{
		if( NextScreenID != SCR_PRICE_UPDATE )
		{
			// Display It
			NextScreenID = SCR_PRICE_UPDATE;
			ScreenID = NextScreenID;
			DrawPriceUpdateSCR();
		}
	
		// For Any Change, Destroy The Input Requests
		DestroyInput();

		LastDefaultSCRStatus = 0xFFFFFFFF;
		// Do Not Display Other Screen, Just Return
		return;
	}

	// Check Display Balance Screen, If Set, Do Not Change Display
	if( KeepBalScreen == 1 )
	{
		// For Update Screen
		LastDefaultSCRStatus = 0xFFFFFFFF;

		return;
	}

	// Check IC Card State, Only Check IC Card Ready And Busy
	if( LastDefaultSCRStatus != ( ProcStatus[PROCESS_CR] & ( CARD_ERROR | CARD_LOGON | 
															 CARD_INSERTED | CARD_READY | 
															 CARD_BUSY | CARD_WAITING | 
															 CARD_CASH_ACCEPTED |
															 CARD_WAIT_CASH_PRINT |CARD_OVER_FURLING 
															 //Douzhq 20140926 Add , Config Printer Set
															 | CARD_WAIT_PRINT
																 )))
	
	{
		// IC Card Status Changed, Dispaly It
		// Save IC Card Status
		LastDefaultSCRStatus = ( ProcStatus[PROCESS_CR] & ( CARD_ERROR | CARD_LOGON | 
															CARD_INSERTED | CARD_READY | 
															CARD_BUSY | CARD_WAITING | 
															CARD_CASH_ACCEPTED |
															CARD_WAIT_CASH_PRINT|CARD_OVER_FURLING 
															//Douzhq 20140926 Add , Config Printer Set
															| CARD_WAIT_PRINT	
																));

		// For Any Change, Destroy The Input Requests
		DestroyInput();

		if( !( ProcStatus[PROCESS_CR] & CARD_ERROR ))
		{
			//down added for over fueling process - 20110906
                        if(ProcStatus[PROCESS_CR] & CARD_OVER_FURLING)    //added for over fueling process - 20110906
                        {
                                // Display Keypad Select Print
			        NextScreenID = SRC_OVERFUELING;
				ScreenID = NextScreenID;
				DrawOverFuelScreen();
                        }
						//Douzhq 20140926 Add , Config Printer Set
			else if(ProcStatus[PROCESS_CR] & CARD_WAIT_PRINT)
			{
				// Display Keypad Select Print
				NextScreenID = SCR_CHOOSEPRINT;
				ScreenID = NextScreenID;
				DrawKeypadChoosePrintSCR();
				
				//ProcStatus[PROCESS_CR] &= ~(CARD_WAIT_PRINT);
				KeypadPrintCard = CARD_KEYPAD_SELECT_PRINT_START;
			}
                        //else if(ProcStatus[PROCESS_CR] & CARD_OVER_FURLING_PASSWORD)
                        //{
                                  // Display Keypad Select Print
			//      NextScreenID = SRC_OVERFUELPASSWORD;
			//	ScreenID = NextScreenID;
			//	DrawOverFuelPasswordScreen();
                        // }
                        //up added for over fueling process - 20110906
                        else if( ProcStatus[PROCESS_CR] & CARD_READY )
			{
				// Display Balance Or Busy
				if( ProcStatus[PROCESS_CR] & CARD_BUSY )
				{
					// Display IC Card Busy
					NextScreenID = DEFAULT_SCR_BUSY;
					ScreenID = NextScreenID;
					DrawCardBusySCR();
				}
				else
				{
					// Display IC Card Balance
					if( NextScreenID != SCR_VOLUMETOTAL )
					{
						// Not For Display Totals
						NextScreenID = DEFAULT_SCR_BALANCE;
						ScreenID = NextScreenID;
						DrawBalanceSCR();
					}
					else
					{
						// Just Log Off, Display Totals
						// Keep NextScreenID = SCR_VOLUMETOTAL
						// To Display Totals In Display In Card
					}
				}
			}
			else if( ProcStatus[PROCESS_CR] & CARD_WAITING )
			{
				// Display IC Card Waiting
				NextScreenID = DEFAULT_SCR_WAITING;
				ScreenID = NextScreenID;
				DrawWaitingSCR();
			}
			else if( ProcStatus[PROCESS_CR] & CARD_INSERTED )
			{
				// Just Inserted Card, Display In Card????
				NextScreenID = DEFAULT_SCR_CARD;
				ScreenID = NextScreenID;
				DrawCardInSCR();
			}

#ifdef MAKE_CASH_CR		
			else if( ProcStatus[PROCESS_CR] & CARD_WAIT_CASH_PRINT )
			{
				// Display Keypad Select Print
				NextScreenID = SCR_SELECTPRINT;
				ScreenID = NextScreenID;
				DrawKeypadSelectPrintSCR();
			}
#endif
			else
			{
				// Display Card Insert
				if( NextScreenID == SCR_VOLUMETOTAL )
				{
					// Just LogOff, Display The Volume Totals
					// NextScreenID Was Set In Log Off Porcess
					// Display Volume Total
					GetVolumeTotal();
					// Next Screen
					NextScreenID = SCR_VOLUMETOTAL;
				}
				else
				{
					if( PriceUpdateFlag == 1 )
					{
						// Enter From Price Update Screen, Draw Screen
						// Do Not Display Card Insert Screen
						DrawScr();
					}
					else
					{
						NextScreenID = DEFAULT_SCR_CARD;
						ScreenID = NextScreenID;
						DrawCardInSCR();
					}
				}
			}
		}
		else
		{
			// Card Reader Error Or PSAM Error,
			// Do Not Display " Card In "
			NextScreenID = DEFAULT_SCR_ERROR;
			ScreenID = NextScreenID;
			DrawErrorSCR();
		}
	}
	else
	{
		// Display Former Screen
		// Do Not Change Screen ID
//		DrawFunctionSCR();
//		NextScreenID = DEFAULT_SCR_CARD;
	}

	// Display The Screen
	DrawScr();
}


/*
 ** InitDisplayProcess
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Initialize For Display Process
 *
 *  RETURNS: None.
 *
 */
void InitDisplayProcess( void )
{
	// For Default Screen
	LastDefaultSCRStatus = 0xFFFFFFFF;

	// For Pump Mssage
	NextPumpMsgID = M_PumpMsgEnd;

	// Creat Default Info Line Display Timer
	SetDispDefaultInfo();

	// Stop The Default Info Line
	StopDefaultInfo();
}


/*
 ** DispClearScr
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Clear The Whole Display Area
 *
 *  RETURNS: None.
 *
 */
void DispClearScr( void )
{
//	if( MsgDelayFlag != 0 )
//	{
//		// 1 Info Line Is In Delay, Clear It
//		// Current Message Is In Delay Time, Delet The Timer
//		DeleteTimer( MsgTimerDelayIndex );
//			
//		MsgDelayFlag = 0;
//	}

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Do Not Display Time
	IdleFlg &= ( ~DISP_TIME );

	// Set Message ID
	MsgID = M_Started;

	// Clear Whole Display Screen	
	ClearLcd( DispScrLeftX, DispScrTopY, DispScrBottomX, DispScrBottomY );
}

/*
 ** DispClearTranScr
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Clear The Transaction Data Display Area
 *
 *  RETURNS: None.
 *
 */
void DispClearTransScr( void )
{
	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );

	// Clear Transaction Display Screen	
	ClearLcd( DispTransLeftX, DispTransTopY, DispTransBottomX, DispTransBottomY );

	// ReDraw The Time Rectangle
	DrawRectangle( DispScrLeftX, DispScrTopY, DispScrBottomX, DispTransTopY - 1 );

	// ReDraw The Trans Rectangle
	DrawRectangle( DispTransLeftX - 1, DispTransTopY - 1, DispTransBottomX + 1, DispTransBottomY + 1 );

	// ReDraw The info Rectangle
	DrawRectangle( DispInfoLeftX - 1, DispInfoTopY - 1, DispInfoBottomX + 1, DispInfoBottomY + 1 );
}

/*
 ** DispClearInfoScr
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION: Clear The Info Line( Bottom Line ) Display Area
 *
 *  RETURNS:
 *
 */
void DispClearInfoScr( void )
{
	// Clear Info Line Display Screen	
	ClearLcd( DispInfoLeftX, DispInfoTopY, DispInfoBottomX, DispInfoBottomY );

	// ReDraw The Trans Rectangle
	DrawRectangle( DispTransLeftX - 1, DispTransTopY - 1, DispTransBottomX + 1, DispTransBottomY + 1 );

	// ReDraw The info Rectangle
	DrawRectangle( DispInfoLeftX - 1, DispInfoTopY - 1, DispInfoBottomX + 1, DispInfoBottomY + 1 );
}





/*
 ** DrawScr
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Draw The Screen According To The enum SCREEN NextScreenID And enum SCREEN ScreenID
 *
 *  RETURNS: None.
 *
 */
void DrawScr( void )
{
	unsigned char i;

	if( NextScreenID != ScreenID )
	{
		// Screen Changed, Display It
		// Clear Screen And Redraw The Rectangles
//		DispClearScr();

		// ReDraw The Screen Rectangle
		DrawRectangle( 0, 0, 127, 127 );

		// ReDraw The Trans Rectangle
		DrawRectangle( DispTransLeftX - 1, DispTransTopY - 1, DispTransBottomX + 1, DispTransBottomY + 1 );

		// ReDraw The info Rectangle
		DrawRectangle( DispInfoLeftX - 1, DispInfoTopY - 1, DispInfoBottomX + 1, DispInfoBottomY + 1 );

		// Save Screen ID
		ScreenID = NextScreenID;

		// Check Screen ID
		if( ScreenID == DEFAULT_SCR_CARD )
		{
			// IC Card Not Insert
			DrawCardInSCR();
		}
		else if( ScreenID == DEFAULT_SCR_BUSY )
		{
			// IC Card Busy
			DrawCardBusySCR();
		}
		else if( ScreenID == DEFAULT_SCR_BALANCE )
		{
			// Dispaly IC Card Balance
			DrawBalanceSCR();
		}
//Douzhq 20140826 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
		else if (ScreenID == DEFAULT_CARNUM_REC)
		{
			CarNumberRecScreen();
		}
		else if (ScreenID == DEFAULT_GRADELIMIT_REC)
		{
			GradeLimitRecScreen();
		}
#endif
		
#ifdef MAKE_PSAM_VERIFY
		else if( ScreenID == SCR_PSAM_V_UNSUPPORT )
		{
			// Dispaly PSAM Unsupport Verify
			DrawPSAMUnSupportSCR();
		}
		else if( ScreenID == SCR_PSAM_V_UNREGISTERED )
		{
			// Dispaly PSAM Need Register And Insert ACT Card
			DrawPSAMUnRegisteredSCR();
		}
		else if( ScreenID == SCR_PSAM_V_PSAM_LOCK )
		{
			// Dispaly PSAM Locked
			DrawPSAMLockSCR();
		}
		else if( ScreenID == SCR_PSAM_V_INSERT_RID )
		{
			// Dispaly Insert RID Card Please
			DrawPSAMInsertRIDSCR();
		}
		else if( ScreenID == SCR_PSAM_V_REGISTER_OK )
		{
			// Dispaly Register Ok
			DrawPSAMRegisterOkSCR();
		}
#endif
		else
		{
			DrawFunctionSCR();
		}
	}
	else
	{
		// Check CustomMsg
		if( ScreenID > DEFAULT_SCR_BALANCE )
		{
			// Not Default Screen, Check CustomMsg
			if( ScreenList[ScreenID].ScrType == POPUP_SCR )
			{
				// POP Up Screen, Check CustomMsg0 And CustomMsg1
				if(( memcmp( CurrentCustomMsg0, CustomMsg0, sizeof( CurrentCustomMsg0 )) != 0 ) || 
					( memcmp( CurrentCustomMsg1, CustomMsg1, sizeof( CurrentCustomMsg1 )) != 0 ))
				{
					// CustomMsg Changed
					DrawFunctionSCR();
				}

				// Display Info Line, Only Display Info Line
				// When Default Screen Displayed And PopUp Screen
//				DispInfoLine();
			}
			else
			{
				// Not POPUP Screen, Check All The CustomMsgs
				for( i = 0; i < ScreenList[ScreenID].ListCnt; i++ )
				{
					// Check Use CustmoMsgs Or Not
					if(( ScreenList[ScreenID].DispItem[i].ListItemTxtID >=  M_CustomMsg0 ) &&
						( ScreenList[ScreenID].DispItem[i].ListItemTxtID <=  M_CustomMsg9 ))
					{
						// There Is A CustomMsg, Check It
						if(( memcmp( CurrentCustomMsg0, CustomMsg0, sizeof( CurrentCustomMsg0 )) != 0 ) ||
							( memcmp( CurrentCustomMsg1, CustomMsg1, sizeof( CurrentCustomMsg1 )) != 0 ) ||
							( memcmp( CurrentCustomMsg2, CustomMsg2, sizeof( CurrentCustomMsg2 )) != 0 ) ||
							( memcmp( CurrentCustomMsg3, CustomMsg3, sizeof( CurrentCustomMsg3 )) != 0 ) ||
							( memcmp( CurrentCustomMsg4, CustomMsg4, sizeof( CurrentCustomMsg4 )) != 0 ) ||
							( memcmp( CurrentCustomMsg5, CustomMsg5, sizeof( CurrentCustomMsg5 )) != 0 ) ||
							( memcmp( CurrentCustomMsg6, CustomMsg6, sizeof( CurrentCustomMsg6 )) != 0 ) ||
							( memcmp( CurrentCustomMsg7, CustomMsg7, sizeof( CurrentCustomMsg7 )) != 0 ) ||
							( memcmp( CurrentCustomMsg8, CustomMsg8, sizeof( CurrentCustomMsg8 )) != 0 ) ||
							( memcmp( CurrentCustomMsg9, CustomMsg9, sizeof( CurrentCustomMsg9 )) != 0 ))
						{
							// CustomMsg Changed
							DrawFunctionSCR();
						}
						else
						{
							// CustomMsgs Not Changed, Go On....
						}
					}
					else
					{
						// No CustomMsgs, Go On....
					}
				}
			}
		}
		else
		{
			// Default Screen, Do Not Check CustomMsg, But Check Busy Screen
			if( ScreenID == DEFAULT_SCR_BUSY )
			{
			  //Douzhq 20140616 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
				if (TRINP.TR_TRANS_DEBITSOURCE == 0x04 && PowerDown.SaveOilAppMode == 0x01)
				{
				    DrawCardBusySaveOil();
				}
				else 
				{
					DrawCardBusy();
				}
#else 
				DrawCardBusy();
#endif
			}
			else
			{
				// Do Not Change
			}
		}
	}
}




/*
 ** DrawCardInSCR
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Please Insert Card Screen
 *
 *  RETURNS: None.
 *
 */
void DrawCardInSCR( void )
{
	// Clear Screen
//	InitLcd();

	// Clear Trans Data Area	
	DispClearTransScr();

#ifdef MAKE_CARD_MODE2

	// Display Mode2 No IC Card Screen
	DrawCardInSCR_Mode2();

#else

	// Display IC Card Insert, Please
	IdleFlg |= DISP_CARDLOGO;

#endif	// MAKE_CARD_MODE2

	// Display Time
	IdleFlg |= DISP_TIME;

	// Display Info Line
//	DispInfoLine();

	// Check Update Info Line Or Not
//	DispDefaultInfoLine();

	// Set Display Default Info	Line
//	ResetDefaultInfo();
}


#ifdef MAKE_CARD_MODE2

/*
 ** DrawCardInSCR_Mode2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Mode2 No IC Card Screen
 *
 *  RETURNS: None.
 *
 */
void DrawCardInSCR_Mode2( void )
{
	unsigned char buf[20];
//	unsigned int val;

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	// Display Line 1
	Fill_Disp_Line1_Mode2();

	// Display Line 2
	if( TransAmountPay == 0x01 )
	{
		memset( buf, 0x00, sizeof( buf ));
		memcpy( buf, "员工卡转室内收银", 16 );
		DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );
	}
	else
	{
		memset( buf, 0x00, sizeof( buf ));
		memcpy( buf, "         ", 9 );
		DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );
	}

	// Display Line 3
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, "请插卡(刷卡)加油.. ", 19 );
	DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );
	
	DisBufToLcd();
}


/*
 ** DrawBalanceSCR_Card_Mode2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Sinopec Mode2 IC Card Balance
 *
 *  RETURNS: None.
 *
 */
void DrawBalanceSCR_Card_Mode2( void )
{
	unsigned char buf[20];
	unsigned short int number;
	unsigned int val;

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	if( RoundOn != 0 )
	{
		// Allow Runnding Again
		RoundOn = 0;
	}

	// Display Line 1
	Fill_Disp_Line1_Mode2();

	// Display Line 2
	memset( buf, 0x00, sizeof( buf ));
	val = strlen(( char * )buf );

	if( TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD )
	{
		// Employee Card
		sprintf(( char * )&buf[val], "员工卡:%3u", TRINP.TR_CARD_EMP_NUMBER );
	}
	else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
	{
		// Test Card
		sprintf(( char * )&buf[val], "验泵卡:%3u", TRINP.TR_CARD_EMP_NUMBER );
	}
	else if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
	{
		// Maintainance Card
		sprintf(( char * )&buf[val], "维修卡:%3u", TRINP.TR_CARD_EMP_NUMBER );
	}
	else if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
	{
		// Display Card Holder Class
		if( TRINP.TR_CRM_CLASS == 0x00 )
		{
			memcpy( &buf[val], "用户卡:  ", 9 );
		}
		else if( TRINP.TR_CRM_CLASS == 0x01 )
		{
			memcpy( &buf[val], "普通会员 ", 9 );
		}
		else if( TRINP.TR_CRM_CLASS == 0x02 )
		{
			memcpy( &buf[val], "银质会员 ", 9 );
		}
		else if( TRINP.TR_CRM_CLASS == 0x03 )
		{
			memcpy( &buf[val], "黄金会员 ", 9 );
		}
		else if( TRINP.TR_CRM_CLASS == 0x04 )
		{
			memcpy( &buf[val], "白金会员 ", 9 );
		}
		else
		{
			memcpy( &buf[val], "用户卡:  ", 9 );
		}

		val = strlen(( char * )buf );
		// Customer Card
		if(( TRINP.TR_TRANS_DATE[2] == TRINP.TR_CARD_HOLDER_BIRTHDAY[0] )
			&& ( TRINP.TR_TRANS_DATE[3] == TRINP.TR_CARD_HOLDER_BIRTHDAY[1] ))
		{
			// Display " Happy Birthday "
			memcpy( &buf[val], "生日快乐 ", 9 );
		}

//		// Do Not Display Card Holder Name
//		for( number = 1; number < 8; number++ )
//		{
//			if( TRINP.TR_CARD_CUSTOMER_NAME[number] == 0xFF )
//			{
//				TRINP.TR_CARD_CUSTOMER_NAME[number] = 0x00;
//
//				break;
//			}
//		}
//
//		// Display Card Holder Name
//		val = strlen(( char * )buf );
//		if(( TRINP.TR_CARD_CUSTOMER_NAME[0] != 0xFF ) && ( TRINP.TR_CARD_CUSTOMER_NAME[0] != 0x11 ))
//		{
//			memcpy( &buf[val], TRINP.TR_CARD_CUSTOMER_NAME, 8 );
//		}
	}
	else
	{
		if( TRINP.TR_PAY_X == 0x15 )
		{
			number = CharToShortIntCopy( &TRINP.TR_CARD_ASN[8] );
			
			// Host Auth
			sprintf(( char * )&buf[val], "后台授权 %X", number );
		}
		else
		{
			// Host Auth
			memcpy(( char * )&buf[val], "其它授权方式", 12 );
		}
	}

	if( TRINP.TR_CARD_ASN[2] != CUSTOMER_CARD )
	{
		if( TransAmountPay == 0x01 )
		{
			// Display Payment Location
			val = strlen(( char * )buf );
			memcpy( &buf[val], " 室内收银", 9 );
		}
		else
		{
			// Do Nothing, Go On....
		}
	}
	else
	{
		// Do Nothing, Go On....
	}

	DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );

	// Display Line 3
	// Line3: Display Balance
	val = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
	Fill_Disp_Line3_Mode2( val );

	DisBufToLcd();
}


/*
 ** DrawCardBusySCR_Card_Mode2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Sinopec Mode2 IC Card Balance
 *
 *  RETURNS: None.
 *
 */
void DrawCardBusySCR_Card_Mode2( void )
{
	unsigned char buf[20];
	unsigned int val;

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	// Display Line 1
	Fill_Disp_Line1_Mode2();

	// Display Line 2
	memset( buf, 0x00, sizeof( buf ));
	val = strlen(( char * )buf );

	if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & PRESET )
	{
		// Clear The Bit
		ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= ( ~PRESET );
		
		// Get Preset Value
		val = CharToIntCopy( TRINP.TR_TRANS_PRESET_VALUE );
		
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

		if( TRINP.TR_TRANS_PRESET_MODE == 0 )
		{
			// Amount Preset
			if( TRINP.TR_TRANS_UNIT & 0x01 )
			{
				// Loyalty Application
				sprintf(( char * )CustomMsg1, "定额: %03u", val );
				TrimDecimal( CustomMsg1, 2 );
			}
			else
			{
				// Sinopec Application
				sprintf(( char * )CustomMsg1, "定额: %03u", val );
				TrimDecimal( CustomMsg1, 1 );
			}
		}
		else
		{
			// Volume Preset
			sprintf(( char * )CustomMsg1, "定量: %03u", val );
			TrimDecimal( CustomMsg1, 3 );
		}

		val = strlen(( char * )buf );
		memcpy( &buf[val], CustomMsg1, strlen(( char * )CustomMsg1 ));
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	}
	else
	{
		// For Host Auth, Display Amount Preset
		if( TRINP.TR_PAY_X == 0x15 )
		{
			val = CharToIntCopy( TRINP.TR_TRANS_PRESET_VALUE );
			
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			sprintf(( char * )CustomMsg1, "后台定额: %03u", val );
			TrimDecimal( CustomMsg1, 1 );
			
			val = strlen(( char * )buf );
			memcpy( &buf[val], CustomMsg1, strlen(( char * )CustomMsg1 ));
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		}
		else if( TRINP.TR_PAY_X == 0x18 )
		{
			val = CharToIntCopy( TRINP.TR_TRANS_PRESET_VALUE );
			
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			sprintf(( char * )CustomMsg1, "非卡自助: %03u", val );
			TrimDecimal( CustomMsg1, 1 );
			
			val = strlen(( char * )buf );
			memcpy( &buf[val], CustomMsg1, strlen(( char * )CustomMsg1 ));
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		}
		else
		{
			memcpy( buf, "随意加油", 8 );
		}
	}

	DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );

//	// Display Line 3
//	if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
//	{
//		val = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
//	}
//	else
//	{
//		// Employee Card, Display Trans Amount
//		val = CharToIntCopy( TRINP.TR_TRANS_AMOUNT );
//	}

	// Display Balance
	val = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
	Fill_Disp_Line3_Mode2( val );

	DisBufToLcd();
}

/*
 ** DrawCardEOT_Mode2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display The IC Card EOT Information
 *
 *  RETURNS: None.
 *
 */
void DrawCardEOT_Mode2( void )
{
	unsigned char buf[20];
	unsigned int val;

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	// Line 1: Trans Amount
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, "应付金额 ", 9 );

	val = CharToIntCopy( TRINP.TR_TRANS_AMOUNT );
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	sprintf(( char * )CustomMsg1, "%03u", val );
	TrimDecimal( CustomMsg1, 1 );
				
	val = strlen(( char * )buf );
	memcpy( &buf[val], CustomMsg1, strlen(( char * )CustomMsg1 ));
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

	DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );

	// Line 2
	if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
	{
		// Customer Card
		if( TRINP.TR_TRANS_UNIT & 0x01 )
		{
			memset( buf, 0x00, sizeof( buf ));
			memcpy( buf, "积分扣分 ", 9 );

			val = CharToIntCopy( TRINP.TR_TRANS_AMOUNT_T1 );
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			sprintf(( char * )CustomMsg1, "%03u", val );
			TrimDecimal( CustomMsg1, 2 );
			
			val = strlen(( char * )buf );
			memcpy( &buf[val], CustomMsg1, strlen(( char * )CustomMsg1 ));
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		}
		else
		{
			memset( buf, 0x00, sizeof( buf ));
			memcpy( buf, "油票扣款 ", 9 );

			val = CharToIntCopy( TRINP.TR_TRANS_AMOUNT_T1 );
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			sprintf(( char * )CustomMsg1, "%03u", val );
			TrimDecimal( CustomMsg1, 1 );
			
			val = strlen(( char * )buf );
			memcpy( &buf[val], CustomMsg1, strlen(( char * )CustomMsg1 ));
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		}

		DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );
		
		// Line 3
		memset( buf, 0x00, sizeof( buf ));
		memcpy( buf, "帐户扣款 ", 9 );

		val = CharToIntCopy( TRINP.TR_TRANS_AMOUNT_T2 );
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		sprintf(( char * )CustomMsg1, "%03u", val );
		TrimDecimal( CustomMsg1, 1 );
					
		val = strlen(( char * )buf );
		memcpy( &buf[val], CustomMsg1, strlen(( char * )CustomMsg1 ));
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

		DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );
	}
	else
	{
		// Employee Card
		memset( buf, 0x00, sizeof( buf ));
		memcpy( buf, "实付金额 ", 9 );

		val = CharToIntCopy( TRINP.TR_TRANS_AMOUNT_T1 );
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		sprintf(( char * )CustomMsg1, "%03u", val );
		TrimDecimal( CustomMsg1, 1 );
					
		val = strlen(( char * )buf );
		memcpy( &buf[val], CustomMsg1, strlen(( char * )CustomMsg1 ));
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

		DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );
		
		// Line 3
		memset( buf, 0x00, sizeof( buf ));
		if( TRINP.TR_PAY_X == 0x07 )
		{
			memcpy( buf, "请收现金 ", 9 );
		}
		else
		{
			memcpy( buf, "后台支付 ", 9 );
		}

		DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );
	}

	DisBufToLcd();

	// Update The Screen
//	UpdateScreen();
	
	// Display " Press Key To Exit"
	FillInfoMsg( M_PressKey, 0, 0 );

	// Do Not Clear The Info Line
	StopDefaultInfo();
}

/*
 ** Fill_Disp_Line1_Mode2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Fill The Content Of Display Line1 For Sinopec Mode2
 *
 *  RETURNS: None.
 *
 *  ATTN:
 *
 */
void Fill_Disp_Line1_Mode2( void )
{
	unsigned char buf[20];
	unsigned int val;

	memset( buf, 0x00, sizeof( buf ));
	val = strlen(( char * )buf );
	if( ProcStatus[PROCESS_CR] & CARD_LOGON )
	{
		// Logged On, Display Log on Employee Number
		sprintf(( char * )&buf[val], "员工:%u  ", PowerDown.LogOnCard.EMPNumber );
	}
	else
	{
		// Not Logged On
		memcpy( &buf[val], "员工:无   ", 10 );
	}

	val = strlen(( char * )buf );
	if( GetWorkMode() != 0x01 )
	{
		// Work Offline
		memcpy( &buf[val], "离线模式", 8 );
	}
	else
	{
		if( GetAuthMode() != 0x01 )
		{
			// FS Auth Mode
			memcpy( &buf[val], "前庭授权", 8 );
		}
		else
		{
			// Host POS Auth Mode
			memcpy( &buf[val], "后台授权", 8 );
		}
	}

	DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );
}


/*
 ** Fill_Disp_Line3_Mode2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Fill The Content Of Display Line3( Amount, Balance etc.... ) For Sinopec Mode2
 *
 *  RETURNS: None.
 *
 */
void Fill_Disp_Line3_Mode2( unsigned int value )
{
	unsigned char buf[20];
	unsigned int val;

	memset( buf, 0x00, sizeof( buf ));

//	if( PowerDown.PdTrans.PD_TRANS_FLAG == AUTH_RESP )
	if(( PowerDown.PdTrans.PD_TRANS_FLAG == AUTH_RESP )
		|| ( PowerDown.PdTrans.PD_TRANS_FLAG == DEBIT_ZERO )
		|| ( PowerDown.PdTrans.PD_TRANS_FLAG == AUTH_PUMP )
		|| ( PowerDown.PdTrans.PD_TRANS_FLAG == MAC2_RECEIVED ))
	{
		// Authed, Dispaly Auth Amount
		memset( buf, 0x00, sizeof( buf ));

		if( TRINP.TR_WORK_MODE == 0x01 )
		{
			memcpy( buf, "授权金额:", 9 );
			val = CharToIntCopy( TRINP.TR_TRANS_PRESET_VALUE );
		}
		else
		{
			memcpy( buf, "油票", 4 );
			val = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
		}

		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		sprintf(( char * )CustomMsg1, "%6u", val );
		TrimDecimal( CustomMsg1, 1 );
			
		val = strlen(( char * )buf );
		memcpy( &buf[val], CustomMsg1, strlen(( char * )CustomMsg1 ));
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	}
	else
	{
		// UnAuthed, Display Balance
		if( TRINP.TR_TRANS_UNIT & 0x01 )
		{
			memset( buf, 0x00, sizeof( buf ));
			memcpy( buf, "积分", 4 );

			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			sprintf(( char * )CustomMsg1, "%10u", value );
			TrimDecimal( CustomMsg1, 2 );
			
			val = strlen(( char * )buf );
			memcpy( &buf[val], CustomMsg1, strlen(( char * )CustomMsg1 ));
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		}
		else
		{
			memset( buf, 0x00, sizeof( buf ));
			memcpy( buf, "油票", 4 );

			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			sprintf(( char * )CustomMsg1, "%10u", value );
			TrimDecimal( CustomMsg1, 1 );
			
			val = strlen(( char * )buf );
			memcpy( &buf[val], CustomMsg1, strlen(( char * )CustomMsg1 ));
			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		}
	}	

	DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );
}


#endif	// MAKE_CARD_MODE2



/*
 ** DrawBalanceSCR
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display IC Card( Cash ) Balance Screen
 *
 *  RETURNS: None.
 *
 */
void DrawBalanceSCR( void )
{
#ifdef MAKE_CASH_CR
	if( CRType == CR_TYPE_CASH )
	{
		// Cash Trans
		if( Cash_CR_Buf.CashType == 0x01 )
		{
			// Display Cash Auth POS Trans Balance Screen
			DrawBalanceSCR_Cash_Auth();
		}
		else
		{
			// Display Cash Acceptor Trans Balance Screen
			DrawBalanceSCR_Cash_Acceptor();
		}
	}
	else
#endif	// MAKE_CASH_CR

	{

#ifdef MAKE_CARD_MODE2

		// Display IC Card Balance
		DrawBalanceSCR_Card_Mode2();

#else

		// Display IC Card Balance
		DrawBalanceSCR_Card();

#endif	// MAKE_CARD_MODE2

	}
}

/*
 ** DrawBalanceSCR_Card
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display IC Card Screen
 *
 *  RETURNS: None.
 *
 */
void DrawBalanceSCR_Card( void )
{
//	unsigned char buf[20];
	unsigned int value;

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	if( RoundOn != 0 )
	{
		// Allow Runnding Again
		RoundOn = 0;
	}

	// Display Emp Number
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	if( ProcStatus[PROCESS_CR] & CARD_LOGON )
	{
		// Logged On, Display Log on Employee Number
		sprintf(( char * )CustomMsg1, "员工:%u", PowerDown.LogOnCard.EMPNumber );
	}
	else
	{
		// Not Logged On
		memcpy(( char * )CustomMsg1, "员工:无", 7 );
	}

	DisplayString12( DispTransLeftX + 1, DispTransLine1, CustomMsg1 );

	// Display Card App Or Card Holder Name
	DispAppType();

	// Display Balance
	value = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
	if( TRINP.TR_TRANS_UNIT & 0x01 )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		memcpy( CustomMsg1, "积分点数", 8 );
		DisplayString12( 77, 24, CustomMsg1 );

//		memset( buf, 0x00, sizeof( buf ));
//		sprintf(( char * )&buf[0], "%10u点", value );
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		sprintf(( char * )CustomMsg1, "%10u", value );
		TrimDecimal( CustomMsg1, 2 );
		DisplayString12( 26, 37, CustomMsg1 );
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	}
//Douzhq 20140613 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	else if( TRINP.TR_TRANS_DEBITSOURCE == 0x04 && PowerDown.SaveOilAppMode == 0x01 )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		memcpy( CustomMsg1, "存油升数", 8 );
		DisplayString12( 77, 24, CustomMsg1 );

//		memset( buf, 0x00, sizeof( buf ));
//		sprintf(( char * )&buf[0], "%10u点", value );
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		sprintf(( char * )CustomMsg1, "%10u", value );
		TrimDecimal( CustomMsg1, 3 );
		DisplayString12( 26, 37, CustomMsg1 );
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	}
#endif
	else
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		memcpy( CustomMsg1, "油票余额", 8 );
		DisplayString12( 77, 24, CustomMsg1 );

//		memset( buf, 0x00, sizeof( buf ));
//		sprintf(( char * )&buf[0], "%10u元", value );
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		sprintf(( char * )CustomMsg1, "%10u", value );
		TrimDecimal( CustomMsg1, 1 );
		DisplayString12( 26, 37, CustomMsg1 );
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	}

	DisBufToLcd();
}

#ifdef MAKE_CASH_CR
/*
 ** DrawBalanceSCR_Cash_Acceptor
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Cash Acceptor Balance Scren
 *
 *  RETURNS: None.
 *
 */
void DrawBalanceSCR_Cash_Acceptor( void )
{
	unsigned char buf[20];
	unsigned int value;
	unsigned short int val0;

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	if( RoundOn != 0 )
	{
		// Allow Runnding Again
		RoundOn = 0;
	}

	// Get Input Amount
//	value = CharToIntCopy( Cash_CR_Buf.CashTotal );
	value = CharToIntCopy( TRINP.TR_CASH_TOTAL );

	if( value == 0x00 )
	{
		// Just Begin, Inform Message
		// Display Line 1
		memset( buf, 0x00, sizeof( buf ));
		memcpy( buf, "现金交易: 请1分钟内", 19 );
		DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );

		// Display Line 2
		memset( buf, 0x00, sizeof( buf ));
		memcpy( buf, "开始插入纸币,接受5,", 19 );
		DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );

		// Display Line 3
		memset( buf, 0x00, sizeof( buf ));
		memcpy( buf, "10,20,50,100面额", 16 );
		DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );

		memset( buf, 0x00, sizeof( buf ));
	}
	else
	{
		// Cash Inserted, Display It
		// Display Line 1
		memset( buf, 0x00, sizeof( buf ));
		memcpy( buf, "现金交易: ", 10 );
		DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );

		// Display Line 2
		val0 = CharToShortIntCopy( Cash_CR_Buf.CashLists );
		memset( buf, 0x00, sizeof( buf ));
		sprintf(( char * )&buf[0], "已接受纸币: %u张", val0 );
		DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );

		// Display Line 3
		memset( buf, 0x00, sizeof( buf ));
//		sprintf(( char * )&buf[0], "已接受金额: %u元", value );
		sprintf(( char * )&buf[0], "已接受金额: %u", value );
		TrimDecimal( buf, 1 );
		DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );

		memset( buf, 0x00, sizeof( buf ));
	}
	
	DisBufToLcd();
}

/*
 ** DrawBalanceSCR_Cash_Auth
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Cash Auth POS Balance Scren
 *
 *  RETURNS: None.
 *
 */
void DrawBalanceSCR_Cash_Auth( void )
{
	unsigned char buf[20];
	unsigned int value;

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	if( RoundOn != 0 )
	{
		// Allow Runnding Again
		RoundOn = 0;
	}

	// Get Input Amount
//	value = CharToIntCopy( Cash_CR_Buf.CashTotal );
	value = CharToIntCopy( TRINP.TR_CASH_TOTAL );

	if( value == 0x00 )
	{
		// Display Line 1
		memset( buf, 0x00, sizeof( buf ));
		memcpy( buf, "授权交易:   1分钟内", 19 );
		DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );

		// Display Line 2
		memset( buf, 0x00, sizeof( buf ));
		memcpy( buf, "请扫描授权码       ", 19 );
		DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );

		// Display Line 3
		memset( buf, 0x00, sizeof( buf ));
		memcpy( buf, "或键盘输入授权码   ", 19 );
		DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );

		memset( buf, 0x00, sizeof( buf ));
	}
	else
	{
		// Display Line 1
		memset( buf, 0x00, sizeof( buf ));
		memcpy( buf, "授权交易: ", 10 );
		DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );

		// Display Line 2
//		memset( buf, 0x00, sizeof( buf ));
//		sprintf(( char * )&buf[0], "已接受纸币: %u张", Cash_CR_Buf.CashLists );
//		DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );

		// Display Line 3
		memset( buf, 0x00, sizeof( buf ));
//		sprintf(( char * )&buf[0], "授权金额: %u元", value );
		sprintf(( char * )&buf[0], "授权金额: %u", value );
		TrimDecimal( buf, 1 );
		DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );

		memset( buf, 0x00, sizeof( buf ));
	}
	
	DisBufToLcd();
}

#endif



/*
 ** DrawErrorSCR
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Card Reader Or PSAM Error Screen
 *
 *  RETURNS: None.
 *
 */
void DrawErrorSCR( void )
{
//	unsigned char buf[20];
//	unsigned int value;

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	DisplayString12( 36, 19, "错    误" );
	DisplayString12( 28, 34, "按 键 重 试" );
	

	DisBufToLcd();

	// Display Info Line
//	DispInfoLine();

	// Check Update Info Line Or Not
//	DispDefaultInfoLine();
}

/*
 ** DrawWaitingSCR
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display While IC Card Do Not Spring Out Card, Instead of Disply Please Insert Card
 *
 *  RETURNS: None.
 *
 */
void DrawWaitingSCR( void )
{
	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	DisplayString12( 34, 19, "请  等  候" );
	DisplayString12( 14, 34, "正在更新IC卡数据" );
	
	DisBufToLcd();
}

/*
 ** DrawPriceUpdateSCR
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Draw Price Update Information Screen
 *
 *  RETURNS: None.
 *
 */
void DrawPriceUpdateSCR( void )
{
	unsigned char buf[20];

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_PriceUpdate], strlen( DISPMSG[M_PriceUpdate] ));
	DisplayString12( 1, 10, buf );

	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_CardNotAllowed], strlen( DISPMSG[M_CardNotAllowed] ));
	DisplayString12( 1, 22, buf );

	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_Wait], strlen( DISPMSG[M_Wait] ));
	DisplayString12( 1, 35, buf );
	
	// Display " Default Info Line "
	FillInfoMsg( M_DefaultInfo, 0, 0 );

	DisBufToLcd();

}

/*
 ** DrawCardBusySCR
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display IC Card Busy Screen
 *
 *  RETURNS: None.
 *
 */
void DrawCardBusySCR( void )
{
#ifdef MAKE_CASH_CR
	if( CRType == CR_TYPE_CASH )
	{
		// Cash Trans
		if( Cash_CR_Buf.CashType == 0x01 )
		{
			// Display Cash Auth POS Trans Busy Screen
			DrawCardBusySCR_Cash_Auth();
		}
		else
		{
			// Display Cash Acceptor Trans Busy Screen
			DrawCardBusySCR_Cash_Acceptor();
		}
	}
	else
#endif

	{

#ifdef MAKE_CARD_MODE2

		// Display IC Card Busy
		DrawCardBusySCR_Card_Mode2();

#else

		// Display IC Card Busy
		DrawCardBusySCR_Card();

#endif	// MAKE_CARD_MODE2
	}
}


/*
 ** DrawCardBusySCR_Card
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display IC Card Busy
 *
 *  RETURNS: None.
 *
 */
void DrawCardBusySCR_Card( void )
{
//	unsigned char buf[20];
//	unsigned char len;
	unsigned int value;

	// Clear Screen
//	InitLcd();

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	
	// Display Time
	IdleFlg |= DISP_TIME;

	// Card Busy Is From Card Balance,
	// So Do Not Clear Trans Screen
	// And Just Change The Busy Messages
//	// Clear Trans Data Area	
	DispClearTransScr();

	// Add Preset Mode
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	if( ProcExchange[PROCESS_CR][PROCESS_KEYPAD] & PRESET )
	{
		// Clear The Bit
		ProcExchange[PROCESS_CR][PROCESS_KEYPAD] &= ( ~PRESET );
		
		// Get Preset Value
		value = CharToIntCopy( TRINP.TR_TRANS_PRESET_VALUE );
		
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

		if( TRINP.TR_TRANS_PRESET_MODE == 0 )
		{
			// Amount Preset
			if( TRINP.TR_TRANS_UNIT & 0x01 )
			{
				// Loyalty Application
				sprintf(( char * )CustomMsg1, "定额:%03u", value );
				TrimDecimal( CustomMsg1, 2 );
			}
			else
			{
				// Sinopec Application
				sprintf(( char * )CustomMsg1, "定额:%03u", value );
				TrimDecimal( CustomMsg1, 1 );
			}
		}
		else
		{
			// Volume Preset
			sprintf(( char * )CustomMsg1, "定量:%03u", value );
			TrimDecimal( CustomMsg1, 3 );
		}
	}
	else
	{
		memcpy( CustomMsg1, "随意加油", 8 );
	}

	DisplayString12( DispTransLeftX, DispTransLine1, CustomMsg1 );

	// Display Pay Mode
	DispAppType();

	// Display Balance
	value = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
	if( TRINP.TR_TRANS_UNIT & 0x01 )
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		memcpy( CustomMsg1, "积分点数", 8 );
		DisplayString12( 77, 24, CustomMsg1 );
	}
	//Douzhq 20140613 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	else if (TRINP.TR_TRANS_DEBITSOURCE == 0x04 && PowerDown.SaveOilAppMode == 0x01)
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		memcpy( CustomMsg1, "剩余油量", 8 );
		DisplayString12( 77, 24, CustomMsg1 );
	}
#endif
	else
	{
		memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
		memcpy( CustomMsg1, "油票余额", 8 );
		DisplayString12( 77, 24, CustomMsg1 );
	}


	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	sprintf(( char * )CustomMsg1, "%10u", value );
	TrimDecimal( CustomMsg1, 1 );

	// Clear Disp Amount
	LastDispAmount = 0;
	
//Douzhq 20140613 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	LastDispVolume = 0;
#endif

	DisplayString12( 26, 37, CustomMsg1 );
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	
	DisBufToLcd();

	// Display Info Line
//	DispInfoLine();
}

#ifdef MAKE_CASH_CR
/*
 ** DrawCardBusySCR_Cash_Auth
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Cash Auth POS Trans Busy Screen
 *
 *  RETURNS: None.
 *
 */
void DrawCardBusySCR_Cash_Auth( void )
{
//	unsigned char buf[20];
//	unsigned char len;
	unsigned int value;

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	
	// Display Time
	IdleFlg |= DISP_TIME;

//	// Clear Trans Data Area	
	DispClearTransScr();

	// Add Display Message

	// Get Input Amount
//	value = CharToIntCopy( Cash_CR_Buf.CashTotal );
//	// Change To Cent
//	value *= 100;

	value = CharToIntCopy( TRINP.TR_CASH_TOTAL );


	// Display Line 1
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memcpy( CustomMsg1, "授权交易: ", 10 );
	DisplayString12( DispTransLeftX + 1, DispTransLine1, CustomMsg1 );

	// Display Line 2
//	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
//	sprintf(( char * )CustomMsg1, "已接受纸币: %u张", Cash_CR_Buf.CashLists );
//	DisplayString12( DispTransLeftX + 1, DispTransLine2, CustomMsg1 );

	// Display Line 3
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	sprintf(( char * )CustomMsg1, "余额:   %6u", value );
//	sprintf(( char * )CustomMsg1, "余额:   %u", value );
	TrimDecimal( CustomMsg1, 1 );

	DisplayString12( DispTransLeftX + 1, DispTransLine3, CustomMsg1 );
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	
	DisBufToLcd();
}

/*
 ** DrawCardBusySCR_Cash_Acceptor
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Cash Acceptor Trans Busy Screen
 *
 *  RETURNS: None.
 *
 */
void DrawCardBusySCR_Cash_Acceptor( void )
{
//	unsigned char buf[20];
//	unsigned char len;
	unsigned int value;

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	
	// Display Time
	IdleFlg |= DISP_TIME;

//	// Clear Trans Data Area	
	DispClearTransScr();

	// Add Display Message

	// Get Input Amount
//	value = CharToIntCopy( Cash_CR_Buf.CashTotal );
//	// Change To Cent
//	value *= 100;

	value = CharToIntCopy( TRINP.TR_CASH_TOTAL );


	// Display Line 1
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	memcpy( CustomMsg1, "现金交易: ", 10 );
	DisplayString12( DispTransLeftX + 1, DispTransLine1, CustomMsg1 );

	// Display Line 2
//	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
//	sprintf(( char * )CustomMsg1, "已接受纸币: %u张", Cash_CR_Buf.CashLists );
//	DisplayString12( DispTransLeftX + 1, DispTransLine2, CustomMsg1 );

	// Display Line 3
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	sprintf(( char * )CustomMsg1, "余额:   %6u", value );
//	sprintf(( char * )CustomMsg1, "余额:   %u", value );
	TrimDecimal( CustomMsg1, 1 );

	DisplayString12( DispTransLeftX + 1, DispTransLine3, CustomMsg1 );
	memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
	
	DisBufToLcd();
}

#endif









/*
 ** DrawCardBusy
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Update The Balance While IC Card Is Busy
 *
 *  RETURNS: None.
 *
 */
//DDDDDDDDDDZZZZZZZZZZ
void DrawCardBusy( void )
{
//	unsigned char buf[20];
	unsigned int balance;
	unsigned PumpAmount = 0;
	
	memcpy( &PumpAmount, Pump_CR_Buf.Amount, 4 );
	
	if( LastDispAmount != PumpAmount )
	{
		// Do Not Entry Again, Except PumpAmount Changed
	  
		LastDispAmount = PumpAmount;

#ifdef MAKE_CASH_CR
		if( CRType == CR_TYPE_CASH )
		{
			// Amount Changed
			balance = CharToIntCopy( TRINP.TR_CASH_TOTAL );
		}
		else
#endif
		{
			// Amount Changed

#ifdef MAKE_CARD_MODE2
//			// For Sinopce Mode2, Maybe Balance Or TransAmount
//			if( TRINP.TR_CARD_ASN[2] != CUSTOMER_CARD )
//			{
//				// Employee Card, Display Trans Amount
//				balance = PumpAmount;
//			}
//			else
//			{
//				// Customer Card, Display Balance
//				balance = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
//			}

			if( TRINP.TR_WORK_MODE != 0x01 )
			{
				// Sinopec Mode2, Offline, Display Balance
				balance = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
			}
			else
			{
				// Sinopec Mode2, Display Authed Balance
				balance = CharToIntCopy( TRINP.TR_TRANS_PRESET_VALUE );
			}
#else
			
			balance = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
#endif	// MAKE_CARD_MODE2
		}
		

		if( balance >= LastDispAmount )
		{
			balance -= LastDispAmount;
		}
		else
		{
			balance = 0x00;
		}

		if( 1 )
		{

			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			//
#ifdef MAKE_CASH_CR
			if( CRType == CR_TYPE_CASH )
			{
				sprintf(( char * )CustomMsg1, "余额:   %6u", balance );
				TrimDecimal( CustomMsg1, 1 );

				DisplayString12( DispTransLeftX + 1, DispTransLine3, CustomMsg1 );
				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			}
			else
#endif
			{

#ifdef MAKE_CARD_MODE2
				if( TRINP.TR_WORK_MODE == 0x01 )
				{
					sprintf(( char * )CustomMsg1, "授权余额:%6u", balance );
				}
				else
				{
					sprintf(( char * )CustomMsg1, "余额%6u", balance );
				}

				TrimDecimal( CustomMsg1, 1 );

				DisplayString12( DispTransLeftX + 1, DispTransLine3, CustomMsg1 );
				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
#else
				if( TRINP.TR_TRANS_UNIT & 0x01 )
				{
					sprintf(( char * )CustomMsg1, "%10u", balance );
					TrimDecimal( CustomMsg1, 2 );
				} 
//Douzhq 20140616 Add , Save Oil				
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
				else if (TRINP.TR_TRANS_DEBITSOURCE == 0x04)
				{
					sprintf(( char * )CustomMsg1, "%10u", balance );
					TrimDecimal( CustomMsg1, 3 );
				}
#endif
				else
				{
					sprintf(( char * )CustomMsg1, "%10u", balance );
					TrimDecimal( CustomMsg1, 1 );
				}

				DisplayString12( 26, 37, CustomMsg1 );
				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
#endif	// MAKE_CARD_MODE2
			}

			DisBufToLcd();
		}
	}
}

//Douzhq 20140613 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
void DrawCardBusySaveOil( void )
{
//	unsigned char buf[20];
	unsigned int balance;
	
	unsigned PumpVolume = 0;
	memcpy(&PumpVolume,Pump_CR_Buf.Volume, 4);
	
	if (LastDispVolume != PumpVolume)
	{
		// Do Not Entry Again, Except PumpAmount Changed
	  
	  	LastDispVolume = PumpVolume;

#ifdef MAKE_CASH_CR
		if( CRType == CR_TYPE_CASH )
		{
			// Amount Changed
			balance = CharToIntCopy( TRINP.TR_CASH_TOTAL );
		}
		else
#endif
		{
			// Amount Changed

#ifdef MAKE_CARD_MODE2
//			// For Sinopce Mode2, Maybe Balance Or TransAmount
//			if( TRINP.TR_CARD_ASN[2] != CUSTOMER_CARD )
//			{
//				// Employee Card, Display Trans Amount
//				balance = PumpAmount;
//			}
//			else
//			{
//				// Customer Card, Display Balance
//				balance = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
//			}

			if( TRINP.TR_WORK_MODE != 0x01 )
			{
				// Sinopec Mode2, Offline, Display Balance
				balance = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
			}
			else
			{
				// Sinopec Mode2, Display Authed Balance
				balance = CharToIntCopy( TRINP.TR_TRANS_PRESET_VALUE );
			}
#else
			
			balance = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
#endif	// MAKE_CARD_MODE2
		}
		

		if( balance >= LastDispVolume )
		{
			balance -= LastDispVolume;
		}
		else
		{
			balance = 0x00;
		}

		if( 1 )
		{

			memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));

			//
#ifdef MAKE_CASH_CR
			if( CRType == CR_TYPE_CASH )
			{
				sprintf(( char * )CustomMsg1, "余额:   %6u", balance );
				TrimDecimal( CustomMsg1, 1 );

				DisplayString12( DispTransLeftX + 1, DispTransLine3, CustomMsg1 );
				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
			}
			else
#endif
			{

#ifdef MAKE_CARD_MODE2
				if( TRINP.TR_WORK_MODE == 0x01 )
				{
					sprintf(( char * )CustomMsg1, "授权余额:%6u", balance );
				}
				else
				{
					sprintf(( char * )CustomMsg1, "余额%6u", balance );
				}

				TrimDecimal( CustomMsg1, 1 );

				DisplayString12( DispTransLeftX + 1, DispTransLine3, CustomMsg1 );
				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
#else
				if( TRINP.TR_TRANS_UNIT & 0x01 )
				{
					sprintf(( char * )CustomMsg1, "%10u", balance );
					TrimDecimal( CustomMsg1, 2 );
				} 
//Douzhq 20140616 Add , Save Oil				
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
				else if (TRINP.TR_TRANS_DEBITSOURCE == 0x04)
				{
					sprintf(( char * )CustomMsg1, "%10u", balance );
					TrimDecimal( CustomMsg1, 3 );
				}
#endif
				else
				{
					sprintf(( char * )CustomMsg1, "%10u", balance );
					TrimDecimal( CustomMsg1, 1 );
				}

				DisplayString12( 26, 37, CustomMsg1 );
				memset( CustomMsg1, 0x00, sizeof( CustomMsg1 ));
#endif	// MAKE_CARD_MODE2
			}

			DisBufToLcd();
		}
	}
}
#endif

/*
 ** DrawFunctionSCR
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Screens Except For The Default 3 Screen
 *
 *  RETURNS: None.
 *
 */
void DrawFunctionSCR( void )
{
	if( ScreenList[ScreenID].ScrType == POPUP_SCR )
	{
		// Clear Previous Screen
		DispClearTransScr();

		// For PopUp Screen
		// Do Not Clear Previous Screen
		DrawPopUpScr();
	}
	else
	{
		// Clear Previous Screen
		DispClearScr();

		// Draw Normal List Item Screen
		DrawNormalScr();
	}
}


/*
 ** DrawPopUpScr
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Draw PopUp Screen According To ScreenID
 *
 *  RETURNS: None.
 *
 *	ATTENTION: POP UP Screen Custom Message Only Use CustomMsg0 And CustomMsg1
 *
 */
void DrawPopUpScr( void )
{
	unsigned char buffer[24];
	unsigned char listlen;
	unsigned char items;
	unsigned char topX;
	unsigned char topY;
	unsigned char i;

	if( ScreenList[ScreenID].ListCnt != 0 )
	{
		// Display Screen Title
		// For PopUp Screen
		// Do Not Care About The Defined TopX, TopY And BottomX, BottomY
		// Calc The Paras
		memset( buffer, 0x00, sizeof( buffer ));
		
		// Get String Length, Use Title Length To Calc The POPUP Screen Width
		// And Use ListCnt To Calc The POPUP Screen Height
		listlen = strlen(( char const * )DISPMSG[ScreenList[ScreenID].ScrTitle.ListItemTxtID] );
		memcpy( buffer, DISPMSG[ScreenList[ScreenID].ScrTitle.ListItemTxtID], listlen );
		
		items = ScreenList[ScreenID].ListCnt;
		
//		topX = ( 64 - (( listlen / 2 ) * 6 ) - 1 );
		topX = (( 128 - ( listlen * 6 )) / 2 );
		topY = (( 64 - (( items + 1 ) * 12 )) / 2 );
		
		// Add 2 Blanks For Width
		listlen += 2;

		ClearLcd( topX - 1, topY - 1, topX + listlen * 6 + 1 + 1, topY + ( items + 1 ) * 12 + 1 + 1 + 2 );
		DrawRectangle( topX - 1, topY - 1, topX + listlen * 6 + 1, topY + ( items + 1 ) * 12 + 1 + 2 );
		
		DrawHorizontalLine( topX, topX + listlen * 6 + 1 + 1 + 1, topY + ( items + 1 ) * 12 + 1 + 1 + 2 );
		DrawVerticalline( topX + listlen * 6 + 1 + 1, topY,	topY + ( items + 1 ) * 12 + 1 + 1 + 2 );

		// Fill The Title
		DisplayString12( topX, topY, buffer );
		
		// Inverse Display The Title
		DisplayContra( topX, topY, topX + listlen * 6, topY + 12 );



		for( i = 0; i < ScreenList[ScreenID].ListCnt; i++ )
		{
			memset( buffer, 0x00, sizeof( buffer ));

			if( ScreenList[ScreenID].DispItem[i].ListItemTxtID == M_CustomMsg0 )
			{
				memcpy( CurrentCustomMsg0, CustomMsg0, sizeof( CurrentCustomMsg0 ));
				memcpy( buffer, CustomMsg0, sizeof( CustomMsg0 ));
			}
			else if( ScreenList[ScreenID].DispItem[i].ListItemTxtID == M_CustomMsg1 )
			{
				memcpy( CurrentCustomMsg1, CustomMsg1, sizeof( CurrentCustomMsg1 ));
				memcpy( buffer, CustomMsg1, sizeof( CustomMsg1 ));
			}
			else
			{
				listlen = strlen(( char const * )DISPMSG[ScreenList[ScreenID].DispItem[i].ListItemTxtID] );
				memcpy( buffer, DISPMSG[ScreenList[ScreenID].DispItem[i].ListItemTxtID], listlen );
			}
			
			// Display The Text
			DisplayString12( topX + 2, topY + ( 12 * ( i + 1 )) + 2, buffer );
		}
		
		
		// Display The Screen
		DisBufToLcd();
	}
}

/*
 ** DrawNormalScr
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Draw Normal List Items Screen According To ScreenID
 *
 *  RETURNS: None.
 *
 */
void DrawNormalScr( void )
{
	unsigned char buffer[24];
	unsigned char listlen;
	unsigned char titletopx;
	unsigned char i;

	if( ScreenList[ScreenID].ListCnt != 0 )
	{
		// Display Screen Title
		memset( buffer, 0x00, sizeof( buffer ));
		
		// Get String Length
		listlen = strlen(( char const * )DISPMSG[ScreenList[ScreenID].ScrTitle.ListItemTxtID] );
		memcpy( buffer, DISPMSG[ScreenList[ScreenID].ScrTitle.ListItemTxtID], listlen );
		
		titletopx = ( 64 - (( listlen / 2 ) * 6 ));
		DisplayString12( titletopx, ScreenList[ScreenID].ScrTitle.ListTopY, buffer );
	
		// Inverse Display Title Line
		DisplayContra( ScreenList[ScreenID].ScrTitle.ListTopX, ScreenList[ScreenID].ScrTitle.ListTopY,
					   ScreenList[ScreenID].ScrTitle.ListBottomX, ScreenList[ScreenID].ScrTitle.ListBottomY );
		// Display Title End

		// Display List Items
		for( i = 0; i < ScreenList[ScreenID].ListCnt; i++ )
		{
			memset( buffer, 0x00, sizeof( buffer ));
			
			if( ScreenList[ScreenID].DispItem[i].ListItemTxtID == M_CustomMsg0 )
			{
				memcpy( CurrentCustomMsg0, CustomMsg0, sizeof( CurrentCustomMsg0 ));
				memcpy( buffer, CustomMsg0, sizeof( CustomMsg0 ));
			}
			else if( ScreenList[ScreenID].DispItem[i].ListItemTxtID == M_CustomMsg1 )
			{
				memcpy( CurrentCustomMsg1, CustomMsg1, sizeof( CurrentCustomMsg1 ));
				memcpy( buffer, CustomMsg1, sizeof( CustomMsg1 ));
			}
			else if( ScreenList[ScreenID].DispItem[i].ListItemTxtID == M_CustomMsg2 )
			{
				memcpy( CurrentCustomMsg2, CustomMsg2, sizeof( CurrentCustomMsg2 ));
				memcpy( buffer, CustomMsg2, sizeof( CustomMsg2 ));
			}
			else if( ScreenList[ScreenID].DispItem[i].ListItemTxtID == M_CustomMsg3 )
			{
				memcpy( CurrentCustomMsg3, CustomMsg3, sizeof( CurrentCustomMsg3 ));
				memcpy( buffer, CustomMsg3, sizeof( CustomMsg3 ));
			}
			else if( ScreenList[ScreenID].DispItem[i].ListItemTxtID == M_CustomMsg4 )
			{
				memcpy( CurrentCustomMsg4, CustomMsg4, sizeof( CurrentCustomMsg4 ));
				memcpy( buffer, CustomMsg4, sizeof( CustomMsg4 ));
			}
			else if( ScreenList[ScreenID].DispItem[i].ListItemTxtID == M_CustomMsg5 )
			{
				memcpy( CurrentCustomMsg5, CustomMsg5, sizeof( CurrentCustomMsg5 ));
				memcpy( buffer, CustomMsg5, sizeof( CustomMsg5 ));
			}
			else if( ScreenList[ScreenID].DispItem[i].ListItemTxtID == M_CustomMsg6 )
			{
				memcpy( CurrentCustomMsg6, CustomMsg6, sizeof( CurrentCustomMsg6 ));
				memcpy( buffer, CustomMsg6, sizeof( CustomMsg6 ));
			}
			else if( ScreenList[ScreenID].DispItem[i].ListItemTxtID == M_CustomMsg7 )
			{
				memcpy( CurrentCustomMsg7, CustomMsg7, sizeof( CurrentCustomMsg7 ));
				memcpy( buffer, CustomMsg7, sizeof( CustomMsg7 ));
			}
			else if( ScreenList[ScreenID].DispItem[i].ListItemTxtID == M_CustomMsg8 )
			{
				memcpy( CurrentCustomMsg8, CustomMsg8, sizeof( CurrentCustomMsg8 ));
				memcpy( buffer, CustomMsg8, sizeof( CustomMsg8 ));
			}
			else if( ScreenList[ScreenID].DispItem[i].ListItemTxtID == M_CustomMsg9 )
			{
				memcpy( CurrentCustomMsg9, CustomMsg9, sizeof( CurrentCustomMsg9 ));
				memcpy( buffer, CustomMsg9, sizeof( CustomMsg9 ));
			}
			else
			{
				// No CustomMsgs
				listlen = strlen(( char const * )DISPMSG[ScreenList[ScreenID].DispItem[i].ListItemTxtID] );
				memcpy( buffer, DISPMSG[ScreenList[ScreenID].DispItem[i].ListItemTxtID], listlen );
			}
			// Display The Text
			DisplayString12( ScreenList[ScreenID].DispItem[i].ListTopX, ScreenList[ScreenID].DispItem[i].ListTopY, buffer );
		}

		// Clear Inversed Display List Item Index
		if( DispListInversedIdx >= ScreenList[ScreenID].ListCnt )
		{
			DispListInversedIdx = 0;
		}

		if( ScreenList[ScreenID].ScrType == NORMAL_SCR )
		{
			// Inverse Display The Indexed List Item
			DisplayContra( ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopX,
							ScreenList[ScreenID].DispItem[DispListInversedIdx].ListTopY,
							ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomX,
							ScreenList[ScreenID].DispItem[DispListInversedIdx].ListBottomY );
		}
		else
		{
			// For Information Screen, Do Not Inversed It, Go On....
		}

		// Display The Screen
		DisBufToLcd();
	}
}


/*
 ** DispInfoLine
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display The Message In Information Line
 *
 *  RETURNS: None.
 *
 */
void DispInfoLine( void )
{
	unsigned char DispBuffer[22];

	if( NextMsgID == M_InfoLine )
	{
		// Check CustomMsg0 And CurrentCustomMsg0
		if( memcmp( InfoLine, CurrentInfoLine, sizeof( InfoLine )) != 0 )
		{
			MsgID = NextMsgID;

			// Clear Info Line
			DispClearInfoScr();
			
			// InfoLine Changed, Display It
			memcpy( CurrentInfoLine, InfoLine, sizeof( InfoLine ));

			// Display It On Info Line
			DisplayString12( DispInfoLeftX, DispInfoTopY, InfoLine );
			DisBufToLcd();

			// Display Default Info Line After Seconds
			ResetDefaultInfo();
		}
	}
	else
	{
		if( NextMsgID != MsgID )
		{
			// Clear Info Line
			DispClearInfoScr();
			
			MsgID = NextMsgID;

			// Current Display Is Different, Display It
			memset( DispBuffer, 0x00, sizeof( DispBuffer ));
			memcpy( DispBuffer, DISPMSG[MsgID], strlen( DISPMSG[MsgID] ));

			// Display It On Info Line
			DisplayString12( DispInfoLeftX, DispInfoTopY, DispBuffer );
			DisBufToLcd();
			
			// Display Default Info Line After Seconds
			ResetDefaultInfo();
		}
		else
		{
			// Current Display Is Same, Go On....
		}
	}
}


//void DispPumpMsg( void )
//{
//	unsigned char DispBuffer[22];
//	unsigned int tmpId;
//
//	// Clear Info Line
//	DispClearInfoScr();
//		
//	// Current Display Is Different, Display It
//	memset( DispBuffer, 0x00, sizeof( DispBuffer ));
//	
//	if(( PumpMsgID > M_PumpMsgStart ) &&( PumpMsgID < M_PumpMsgEnd ))
//	{
//		tmpId = NextPumpMsgID;
//		memcpy( DispBuffer, DISPPUMPMSG[tmpId], strlen( DISPPUMPMSG[tmpId] ));
//	}
//	else if(( PumpMsgID > M_PumpErrStart ) &&( PumpMsgID < M_PumpErrEnd ))
//	{
//		tmpId = ( NextPumpMsgID - M_PumpErrStart );
//		memcpy( DispBuffer, DISPPUMPERRMSG[tmpId], strlen( DISPPUMPERRMSG[tmpId] ));
//	}
//	else if(( PumpMsgID > M_PumpPumpError ) &&( PumpMsgID < M_PumpPumpEnd ))
//	{
//		tmpId = ( NextPumpMsgID - M_PumpPumpError );
//		memcpy( DispBuffer, DISPPUMPPUMPERRMSG[tmpId], strlen( DISPPUMPPUMPERRMSG[tmpId] ));
//	}
//	else
//	{
//		return;
//	}
//
//	// Display It On Info Line
//	DisplayString12( DispInfoLeftX, DispInfoTopY, DispBuffer );
//	DisBufToLcd();
//
//	// Set Display Default Info	Line
//	ResetDefaultInfo();
//}

/*
 ** DispAppType
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	void
 *
 *  DESCRIPTION: Display App Type Or Card Holder Name( For Customer Card ) To Specific Area Of Screen
 *
 *  RETURNS: void
 *
 */
void DispAppType( void )
{
	unsigned char buf[16];
	unsigned char AppByte;
	unsigned char number;

	if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
	{
		// Display Card Holder Name
		memset( buf, 0x00, sizeof( buf ));
		if(( TRINP.TR_CARD_CUSTOMER_NAME[0] != 0xFF ) && ( TRINP.TR_CARD_CUSTOMER_NAME[0] != 0x11 ))
		{
			for( number = 1; number < 10; number++ )
			{
				if( TRINP.TR_CARD_CUSTOMER_NAME[number] == 0xFF )
				{
					TRINP.TR_CARD_CUSTOMER_NAME[number] = 0x00;
					
					break;
				}
			}

			memcpy( buf, TRINP.TR_CARD_CUSTOMER_NAME, 10 );
		}
	}
	else
	{
		if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & SELECT_PAY_MODE )
		{
			// Keypad Just Issued The Select App Mode Command
			AppByte = KEYPAD_CR_Buf.PayMode;
			AppByte <<= 4;
		}
		else
		{
			AppByte = ( TRINP.TR_TRANS_UNIT & 0xF0 );
		}

		memset( buf, 0x00, sizeof( buf ));
		if( AppByte == 0x10 )
		{
			memcpy( buf, "油票", 4 );
		}
		else if( AppByte == 0x20 )
		{
			memcpy( buf, "提油凭证", 8 );
		}
		else if( AppByte == 0x30 )
		{
			memcpy( buf, "银行卡", 6 );
		}
		else if( AppByte == 0x40 )
		{
			memcpy( buf, "其他1", 5 );
		}
		else if( AppByte == 0x50 )
		{
			memcpy( buf, "其他2", 5 );
		}
		else
		{
			memcpy( buf, "现金", 4 );
		}
	}

	DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );
}



/*
 ** DispRounding
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	RoundType: 0: Display Cash Rounding On Screen; 1: Display Volume Rounding On Screen
 *
 *  DESCRIPTION: Display The Rounding Message On Screen When Rounding Keys Pressed
 *
 *  RETURNS: None.
 *
 *	Attn: Call Only In IC Card Busy And Rounding Keys Pressed
 *
 */
unsigned char RoundOn = 0;

void DispRounding( unsigned char RoundType )
{
	unsigned char buf[20];

	if( RoundOn == 0 )
	{

#ifdef MAKE_CARD_MODE2

		// Clear Transaction Display Screen	
		ClearLcd( DispTransLeftX, DispTransTopY + 1 + 12, DispTransBottomX, DispTransTopY + 1 + 12 + 12 );

#else

		// Clear Transaction Display Screen	
		ClearLcd( DispTransLeftX, DispTransTopY + 1, DispTransBottomX, DispTransTopY + 1 + 12 );

#endif	// MAKE_CARD_MODE2

		// Add Preset Mode
		memset( buf, 0x00, sizeof( buf ));

		if( RoundType == 0 )
		{
			// Do Not Runnding Again
			RoundOn = 1;

#ifdef MAKE_CASH_CR
			if( CRType == CR_TYPE_CASH )
			{
				// Cash Trans
				if( Cash_CR_Buf.CashType == 0x01 )
				{
					memcpy( buf, "授权交易   金额圆整", 19 );
				}
				else
				{
					memcpy( buf, "现金交易   金额圆整", 19 );
				}
			}
			else
#endif	// MAKE_CASH_CR

			{

#ifdef MAKE_CARD_MODE2

				memcpy( buf, "金额圆整           ", 19 );

#else

				memcpy( buf, "金额圆整", 8 );

#endif	// MAKE_CARD_MODE2

			}
		}
		else if( RoundType == 1 )
		{
			// Do Not Runnding Again
			RoundOn = 2;

#ifdef MAKE_CASH_CR
			if( CRType == CR_TYPE_CASH )
			{
				// Cash Trans
				if( Cash_CR_Buf.CashType == 0x01 )
				{
					memcpy( buf, "授权交易   升数圆整", 19 );
				}
				else
				{
					memcpy( buf, "现金交易   升数圆整", 19 );
				}
			}
			else
#endif	// MAKE_CASH_CR

			{

#ifdef MAKE_CARD_MODE2

				memcpy( buf, "升数圆整           ", 19 );

#else

				memcpy( buf, "升数圆整", 8 );

#endif	// MAKE_CARD_MODE2

			}
		}
		else
		{
			// Error, Don't Care About
		}

#ifdef MAKE_CARD_MODE2

		DisplayString12( DispTransLeftX, DispTransLine2, buf );

#else

		DisplayString12( DispTransLeftX, DispTransLine1, buf );

#endif	// MAKE_CARD_MODE2

		DisBufToLcd();
	}
	else
	{
		// Already Rounded, Do Not Care
	}
}


/*
 ** UpdateScreen
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Update Balance Screen Special For Cash Trans
 *
 *  RETURNS: None.
 *
 */
void UpdateScreen( void )
{
	ScreenID = SCREEN_START;
}

//Douzhq 20140826 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
void CarNumberRecScreen(void)
{
	unsigned char buf[20];
	
	CarNumberTmpNum = 0;
	
	if (CarOverFlag == 0)
	{
		if(CarNumberRecWait != (Tmr *)0)
		{
			DeleteTimer(CarNumberRecWait);
			CarNumberRecWait = (Tmr *)0;
		}
		CarNumberRecWait = CreateTimer(TIMER_AUTO,CARWAITTiMEOUT,CarNumberWaitTimeout);
	}
	
	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_CarNumberRecing], strlen( DISPMSG[M_CarNumberRecing] ));
	DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );
	
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_Waiting], strlen( DISPMSG[M_Waiting] ));
	DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );
	if (CarOverFlag == 0)
	{
		DrawRectangle(DispTransLeftX + 1,DispTransLine3,DispTransLeftX + 124,DispTransLine3 + 12);
	}
	else
	{
		CarOverFlag = 0;
		DrawRectangle(DispTransLeftX + 1,DispTransLine3,DispTransLeftX + 124,DispTransLine3 + 12);
		ClearLcd(DispTransLeftX + 2,DispTransLine3 + 1,DispTransLeftX + 123,DispTransLine3 + 11);
		DisplayContra(DispTransLeftX + 2,DispTransLine3 + 1,DispTransLeftX + 123,DispTransLine3 + 11);
	}
	
	DisBufToLcd();
}

void GradeLimitRecScreen(void)
{
	unsigned char buf[20];
	
	CarNumberTmpNum = 0;
	
	if (CarOverFlag == 0)
	{
		if(CarNumberRecWait != (Tmr *)0)
		{
			DeleteTimer(CarNumberRecWait);
			CarNumberRecWait = (Tmr *)0;
		}
		CarNumberRecWait = CreateTimer(TIMER_AUTO,CARWAITTiMEOUT,CarNumberWaitTimeout);
	}
	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_GradeLimitRecing], strlen( DISPMSG[M_GradeLimitRecing] ));
	DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );
	
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_Waiting], strlen( DISPMSG[M_Waiting] ));
	DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );
	
	if (CarOverFlag == 0)
	{
		DrawRectangle(DispTransLeftX + 1,DispTransLine3,DispTransLeftX + 124,DispTransLine3 + 12);
	}
	else
	{
		CarOverFlag = 0;
		DrawRectangle(DispTransLeftX + 1,DispTransLine3,DispTransLeftX + 124,DispTransLine3 + 12);
		ClearLcd(DispTransLeftX + 2,DispTransLine3 + 1,DispTransLeftX + 123,DispTransLine3 + 11);
		DisplayContra(DispTransLeftX + 2,DispTransLine3 + 1,DispTransLeftX + 123,DispTransLine3 + 11);
	}
	
	DisBufToLcd();
}
#endif


#ifdef MAKE_PSAM_VERIFY
/*
 ** DrawPSAMUnSupportSCR
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Just Display The PSAM Did Not Support The Certificate Function
 *
 *  RETURNS: None.
 *
 */
void DrawPSAMUnSupportSCR( void )
{
	unsigned char buf[20];

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_V_PSAM], strlen( DISPMSG[M_V_PSAM] ));
	DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );
	
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_V_PSAM_NOT_SUPPORT], strlen( DISPMSG[M_V_PSAM_NOT_SUPPORT] ));
	DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );

	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_V_PSAM_CHANGE], strlen( DISPMSG[M_V_PSAM_CHANGE] ));
	DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );

	DisBufToLcd();
}

/*
 ** DrawPSAMUnRegisteredSCR
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display PSAM Need Register And Insert ACT Card Please
 *
 *  RETURNS: None.
 *
 */
void DrawPSAMUnRegisteredSCR( void )
{
	unsigned char buf[20];

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_V_PUMP_NOT_REGISTERED], strlen( DISPMSG[M_V_PUMP_NOT_REGISTERED] ));
	DisplayString12( DispTransLeftX + 1, DispTransTopY + 1 + 2, buf );
	
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_V_INSERT_ACT], strlen( DISPMSG[M_V_INSERT_ACT] ));
	DisplayString12( DispTransLeftX + 1, DispTransTopY + 1 + 12 + 6, buf );
	

	DisBufToLcd();
}

/*
 ** DrawPSAMLockSCR
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display PSAM Locked
 *
 *  RETURNS: None.
 *
 */
void DrawPSAMLockSCR( void )
{
	unsigned char buf[20];

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_V_PSAM_LOCK], strlen( DISPMSG[M_V_PSAM_LOCK] ));
	DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );
	
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_V_PSAM_CHANGE], strlen( DISPMSG[M_V_PSAM_CHANGE] ));
	DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );

	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_V_RE_BINDING], strlen( DISPMSG[M_V_RE_BINDING] ));
	DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );

	DisBufToLcd();
}


void DrawPSAMInsertRIDSCR( void )
{
	unsigned char buf[20];

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_V_PSAM_ACT_Ok], strlen( DISPMSG[M_V_PSAM_ACT_Ok] ));
	DisplayString12( DispTransLeftX + 1, DispTransTopY + 1 + 2, buf );
	
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_V_Insert_RID], strlen( DISPMSG[M_V_Insert_RID] ));
	DisplayString12( DispTransLeftX + 1, DispTransTopY + 1 + 12 + 6, buf );
	

	DisBufToLcd();
}


void DrawPSAMRegisterOkSCR( void )
{
	unsigned char buf[20];

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, DISPMSG[M_V_RegisterOk], strlen( DISPMSG[M_V_RegisterOk] ));
	DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );
	
//	memset( buf, 0x00, sizeof( buf ));
//	memcpy( buf, "按任意键继续", 12 );
//	DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );
	

	DisBufToLcd();
}



#endif



#ifdef MAKE_CASH_CR		

/*
 ** DrawKeypadSelectPrintSCR
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display Keypad Select Print Cash Receipt Or Not
 *
 *  RETURNS: None.
 *
 */
void DrawKeypadSelectPrintSCR( void )
{
	unsigned char buf[20];

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	// Just Begin, Inform Message
	// Display Line 1
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, "   是否打印小票?   ", 19 );
	DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );

	// Display Line 2
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, "按'确认'键,打印小票", 19 );
	DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );

	// Display Line 3
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, "按'更改'键, 退出   ", 19 );
	DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );

	memset( buf, 0x00, sizeof( buf ));
	
	DisBufToLcd();
}

#endif

//Douzhq 20140922 Add , Config Printer Set
void DrawKeypadChoosePrintSCR( void )
{
	unsigned char buf[20];

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	// Just Begin, Inform Message
	// Display Line 1
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, "   是否打印小票?   ", 19 );
	DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );

	// Display Line 2
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, "按'确认'键,打印小票", 19 );
	DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );

	// Display Line 3
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, "按'更改'键, 退出   ", 19 );
	DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );

	memset( buf, 0x00, sizeof( buf ));
	
	DisBufToLcd();
}

//down added for over fueling process - 20110906
/*
 ** DrawOverFuelScreen
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display over fueling data
 *
 *  RETURNS: None.
 *
 */
void DrawOverFuelScreen( void )
{
        unsigned char buf[20];
        //unsigned char i;

	// Do Not Display IC Card Insert, Please
	IdleFlg &= ( ~DISP_CARDLOGO );
	// Display Time
	IdleFlg |= DISP_TIME;

	// Clear Trans Data Area	
	DispClearTransScr();

	// Just Begin, Inform Message
	// Display Line 1
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, "加油量超过IC卡余额!", 19 );
	DisplayString12( DispTransLeftX + 1, DispTransLine1, buf );

	// Display Line 2
	memset( buf, 0x00, sizeof( buf ));
	memcpy( buf, "过冲金额:", 9 );
	DisplayString12( DispTransLeftX + 1, DispTransLine2, buf );

	// Display Line 3
	memset( buf, 0x00, sizeof( buf ));           
    //sprintf(( char * )&buf[0], "%u元", OverFuelMoney );
    sprintf(( char * )&buf[0], "%10u", OverFuelMoney );
	//TrimDecimal( &buf[0], 1 );       
	if( KeyDispTrans.TRANS_UNIT & 0x01 )
	{
		TrimDecimal( &buf[0], 2 );
	}
	else
	{
		TrimDecimal( &buf[0], 1 );
	}
	DisplayString12( DispTransLeftX + 1, DispTransLine3, buf );
	

	memset( buf, 0x00, sizeof( buf ));
	
	DisBufToLcd();
  
}
/*
 ** DrawOverFuelPasswordScreen
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Display over fueling password input
 *
 *  RETURNS: None.
 *
 */
void DrawOverFuelPasswordScreen( void )
{
  
  
}

//down added for over fueling process - 20110906
