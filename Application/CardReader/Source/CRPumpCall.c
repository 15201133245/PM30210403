#include <string.h>

#include "global.h"

#include "ICCardCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "ICCardProc.h"
#include "Pdprotect.h"
#include "define.h"
#include "CardReady.h"
#include "ExCRProc.h"
#include "CRPumpCall.h"
#include "PSAMCommand.h"
#include "ICCardInProc.h"
#include "Message.h"
#include "Screen.h"
#include "Display.h"
#include "WorkMode.h"
#include "WriteLog.h"
#include "Overfuel.h"

//Douzhq 20140620 Add , Vip Fueling 
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
#include "SocialCard.h"
#endif

//Douzhq 20140805 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
#include "SocialCard.h"
static unsigned char GradeLimNumber = 0;
static unsigned char GradeLimitTmpBuf[40];
#endif

//Douzhq 20141103 Add , Safe Mode
#include "Pump.h"
#include "CRSFunc.h"

/*
 ** ICCard_Pump_Call_Proc
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Pump Call Process, Perform
 *				  Get Tran Date, Get Trans Time, Get TTC
 *				  IC Card Check Limit, IC Card Init Grey
 *				  PSAM Init Grey, IC Card Set Grey
 *				  Auth To Pump, Auth Confirm To Pump
 *
 *  RETURNS: None.
 *
 */
void ICCard_Pump_Call_Proc( void )
{
	unsigned short int retval;
	unsigned int tmp;
//Douzhq 20140822 Add , Save Oil					
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	unsigned short int TmpCardGrade;
#endif
	
	// Only Running When CR_Proc_Step = CR_ICCARD_PUMP_CALL_PROC
	if( CR_Proc_Step == CR_ICCARD_PUMP_CALL_PROC )
	{
		switch( CR_CommandType )
		{
//Douzhq 20141103 Add , Safe Mode
//------------------------------------Down-------------------------------------------
			case CR_WAIT_SAFEMODE_PASSWD:
			
				retval = GetEmpPWD(); 
			
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						if( memcmp( KEYPAD_CR_Buf.ICCardEMPPwd, PowerDown.LogOnCard.EMPPassword, sizeof( PowerDown.LogOnCard.EMPPassword )) == 0 )
						{
							// The Empolyee Password is Right , Then Go To Next Step
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_LIMIT );
						}
						else
						{
							// The Empolyee Password is Not Right , Then Spring IC Card
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
							
							// Send Oil Price , Then Card Log Offed
							if (ProcStatus[PROCESS_CR] & CARD_LOGON)
							{
								// Display " Employee Password Error "
								FillInfoMsg( M_EmpPwdErr, 0, 0 );
							}
							else
							{
								// Display " Log Offed"
								FillInfoMsg( M_LogOffed, 0, 0 );
							}
						}
					}
					else
					{
						
					}
				}
				else
				{
					//Hung Up Nozzle , Then Spring IC Card				
					if (ProcStatus[PROCESS_PUMP] == PUMP_C_IDLE)
					{
						//Hung Up Nozzle , And Pump Return Idle
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						
						if (CR_Task_Flag & ICCARD_EMP_PWD_K)
						{
							CR_Task_Flag &= ( ~ICCARD_EMP_PWD_K );
						}
					}
				}
			break;
//------------------------------------ Up -------------------------------------------
			case CR_ICCARD_CHECK_LIMIT:
				
				// Backlight Open, Closed In Write Record
				OpenLcdLight( 1 );
                                //down added for over fueling process - 20110906
                                if((OverFuelTimes>2)||OverFuelToTValue>=500)
                                {
                                   FillInfoMsg( M_OverFueling, 0, 0 );
                                   Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_SPINGOUT_CARD );
                                   break;
                                }
                                //up added for over fueling process - 20110906
//Douzhq 20140730 Add , Lock Nozzle
					  if (LOCK_NOZZLE.LockFlag == 1 && TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD)	
					  {
					  	  Set_Next_Command(CR_ICCARD_PUMP_CALL_PROC,CR_ICCARD_UNLOCK_NOZZLE);	  	
						  break;
					  }	
//Douzhq 20140822 Add , Save Oil					
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
					  if (TRINP.TR_TRANS_DEBITSOURCE == 0x04 && PowerDown.SaveOilAppMode == 0x01)
					  {
						  TmpCardGrade = CharToShortIntCopy( TRINP.TR_CARD_GRADE_LIMIT );
					  	  if (TmpCardGrade == 0xFFFF)
						  {
							  FillInfoMsg( M_SaveoilError, 0, 0 );
							  
							  Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
							  
							  break;
						  }
					  }
#endif
					  
#ifdef MAKE_CARD_MODE2
				// Mark Auth, Do Not Change Auth Method For Sinopec Mode2
				ProcStatus[PROCESS_CR] |= CARD_AUTHED;
#endif

				// Check Card Ready And Log On Or Not
				if( ProcStatus[PROCESS_CR] & CARD_READY )
				{
					if( !( ProcStatus[PROCESS_CR] & CARD_LOGON ))
					{
						// Display: " Employee Not Log On "
						FillInfoMsg( M_LogOffed, 0, 0 );
				
						Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
					
//						return;
						break;
					}
#ifdef MAKE_CASH_CR
					else if( CRType == CR_TYPE_CASH )
					{
						// For Cash Card, Do Not Issue Pump Call Until Cash accept OK
						if( !( ProcStatus[PROCESS_CR] & CARD_CASH_ACCEPTED ))
						{
							// Display: " Cash Not Ok, Nozzle Off "
							FillInfoMsg( M_NozzleOffCashError, 0, 0 );
					
							Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
						
//							return;
							break;
						}
						else if( Pump_CR_Buf.NozzleNumber != ( Cash_CR_Buf.CashKeyNozzleNumber + 1 ))
						{
							// Display: " Nozzle Number Error, Nozzle Off "
							FillInfoMsg( M_NozzleError, 0, 0 );
							
							//Douzhq 20140916 Add , Voice Add
							if (CRType == CR_TYPE_CASH)
							{
								ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_OILCHOOSE;
							}
							
							Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
						
//							return;
							break;
						}
						else
						{
						}
					}
//					else if( CRType == CR_TYPE_CARD )
//					{
//						if( TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD )
//						{
//							// Display: " Do Not Support Employee Card For IC Card With Cash Function "
//							// Do Not Support Customer Card
//							FillInfoMsg( M_EmployeeCardNotForCash, 0, 0 );
//					
//							// Power Off IC Card And Spring Out
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
//
////							Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
//						
////							return;
//							break;
//						}
//						else
//						{
//						}
//					}
#endif								
					else
					{
						// Go On....
					}
				}
				else
				{
					// Display: " No Card, And Nozzle On Error "
					FillInfoMsg( M_NozzleOnError, 0, 0 );
				
					// Inform Voice Process Nozzle On Error
					ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_NOZZLE_ERR;

					Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );

//					return;
					break;
				}
				
				
				// Need To Get The Nozzle Information First, Into TRINP.TR_TRANS_**** Buffer
				// Include Nozzle Number, Price, Grade Code, Station Information etc.

#ifdef MAKE_CARD_MODE2
				if( PowerDown.PdTrans.PD_TRANS_FLAG == AUTH_RESP )
				{
					// Already Authed, Do Not Get Pump Call Info
					// Just Go On....
				}
				else
				{
					GetPumpCallInfo();
					
					WriteCRPumpCallLog();
				}
#else
				GetPumpCallInfo();
				
				WriteCRPumpCallLog();
#endif	// MAKE_CARD_MODE2				

				// Check Journal Space limit
				tmp = GetRecordSpace();
				if( tmp < ReserveRecordSpace )
				{
//					// Write Log
//					CRWriteLog( M_RecordMax );

					// Display: " Record Max "
					FillInfoMsg( M_RecordMax, 0, 0 );
					// Do Not Clear Untill Display Another
					StopDefaultInfo();
					
					// Auth Error
					FillAuthError();

					// For Write Spring Card Log
					PowerDownFlag = Pd_Flag_8;
                    ErrorFlag = 0xE003;

					// Power Off IC Card And Spring Out
					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );

//					return;
					break;
				}

#ifdef MAKE_CARD_MODE2

				// For Host Auth Mode, Do Not Allow EMP Card
//				if( TRINP.TR_CARD_ASN[2] != CUSTOMER_CARD )
				if( TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD )
				{
					if( TRINP.TR_WORK_MODE == 0x01 )
					{
						// Work Online, Check It
						if( TRINP.TR_AUTH_MODE == 0x01 )
						{
							// Display " Host POS Auth, Can Not Use EMP Card "
							FillInfoMsg( M_EMPCardLimited, 0, 0 );

							// Auth Error
							FillAuthError();

							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_97;

							// Power Off IC Card And Spring Out
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );

							break;
						}
						else
						{
							// Do Nothing, Go On....
						}
					}
					else
					{
						// Work Offline, Do Nothing, Go On....
					}
				}
				else
				{
					// Do Nothing, Go On....
				}

#endif	// MAKE_CARD_MODE2				
				
				// If Employee Card Or Maintainance Card Or Test Card,
				// Do Not Limit Any More, Only Check Limit For Customer Card
				if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
				{
#ifdef MAKE_CARD_MODE2
					if( PowerDown.PdTrans.PD_TRANS_FLAG == AUTH_RESP )
					{
						// Already Authed, Do Not Check Limit Info
						// Just Check Preset For Fuelling....
						// Check Preset Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_PRESET );
					}
					else
					{
						// Display: " Customer Card, Check Limited "
						FillInfoMsg( M_CustomerCardCheckLimited, 0, 0 );
						
						// Check Grade Brand Limit Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_GRADE_BRAND_LIMIT );
					}
#else
					// Display: " Customer Card, Check Limited "
					FillInfoMsg( M_CustomerCardCheckLimited, 0, 0 );
					
					// Check Grade Brand Limit Next
					Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_GRADE_BRAND_LIMIT );
#endif	// MAKE_CARD_MODE2				
				}
				else
				{
#ifdef MAKE_CARD_MODE2
					if(( PowerDown.PdTrans.PD_TRANS_FLAG == AUTH_RESP )
						&& ( TRINP.TR_PAY_X == 0x15 ))
					{
						// Host Authed, Auth Pump Only
						
						// Save PumpCall Information
						SavePumpCallData( 0 );
						
 						// Auth Pump
 						CardAuthPump();

						// Mark Power Down
						PowerDown.PdTrans.PD_TRANS_FLAG = DEBIT_ZERO;
						
						// Start Fuelling, Next
						Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_CARDREADER_CHECK_CARD );
					}
					else if( TRINP.TR_PAY_X == 0x18 )
					{
						// Non IC Card Trans, Auth Pump Only
						
						// Save PumpCall Information
						SavePumpCallData( 0 );
						
 						// Auth Pump
 						CardAuthPump();

						// Mark Power Down
						PowerDown.PdTrans.PD_TRANS_FLAG = DEBIT_ZERO;
						
						// Start Fuelling, Next
						Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_CARDREADER_CHECK_CARD );
					}
					else
					{
						// Get The Max Preset Value
						memcpy( TRINP.TR_MAX_PRESET_AMOUNT, "\xFF\xFF\xFF\xFF", 4 );
						memcpy( TRINP.TR_MAX_PRESET_VOLUME, "\xFF\xFF\xFF\xFF", 4 );

						// Check Preset Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_PRESET );
					}
#else
					// Get The Max Preset Value
					memcpy( TRINP.TR_MAX_PRESET_AMOUNT, "\xFF\xFF\xFF\xFF", 4 );
					memcpy( TRINP.TR_MAX_PRESET_VOLUME, "\xFF\xFF\xFF\xFF", 4 );

					// Check Preset Next
					Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_PRESET );
#endif	// MAKE_CARD_MODE2

				}
			
			break;
//Douzhq 20140603 Add , Lock Nozzle , Auth Fuel Unlock
		case CR_ICCARD_UNLOCK_NOZZLE:
		  if( !( CR_Task_Flag & ICCARD_ENTER_KEY_K ))
		  {
			  CR_Task_Flag |= ICCARD_ENTER_KEY_K;
			  ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= UNLOCK_PWD_FUEL;
		  }
		  else
		  {
			  if (ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= UNLOCK_PWD_FUEL)
			  {
				  ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ~(UNLOCK_PWD_FUEL);
				  CR_Task_Flag &= ( ~ICCARD_ENTER_KEY_K );
				  //CardAuthPump();
				  Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_LIMIT );
			  }
		  }
		  break;
			// Check Grade Brand Limit
			case CR_ICCARD_CHECK_GRADE_BRAND_LIMIT:

				// Check Grade Brand Limit First
				retval = Check_Card_Grade_Brand_Limit();
					
				if( retval != 1 )
				{
					// Check Grade Brand Limit End
					if( retval == 0x9000 )
					{
						// Brand Not Limited, Check Location Limit Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_LOCATION_LIMIT );
					}
					else
					{
//Douzhq 20140820 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
						if (PowerDown.CarNumRecognizeModw == 0x01)
						{
							ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_CAR_ONCE);
							ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_GRADE_lIMIT);
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_GRADE_LIMIT_RECOHNIZE );
							NextScreenID = DEFAULT_GRADELIMIT_REC;
							CarOverFlag = 0;
							CarOverFlag2 = 0;
							if(CarNumberRecWait != (Tmr *)0)
							{
								DeleteTimer(CarNumberRecWait);
							}
						}
						else
						{
							GradeLimitRec();
							Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
						}
#else
						// Brand Limited, Power Off Next
						// Inform POS Brand Limited Card
						memcpy( POS_CR_Buf.ICCARD_STATE, "\x10\x01", sizeof( POS_CR_Buf.ICCARD_STATE ));
						
						// For Record Brand Limited Card
						memcpy( TRINP.TR_CARD_GREY_TAC, "\x10\x01\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
						
						SaveICCardData();

						// Save It For Record
						FillCardErrorData();

						// Auth Error
						FillAuthError();

//						// Write Log
//						CRWriteLog( M_MsgBrandLimitedExceed );

						// Display: "  Grade Brand Limited "
						FillInfoMsg( M_MsgBrandLimitedExceed, 0, 0 );
					
						// For Write The Running Log
						JournalFlag = 1;

						// Form Journal Next
						Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
#endif
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;
			
#ifdef MAKE_CASH_CR
case CR_ICCARD_GRADE_LIMIT_RECOHNIZE:
				if (ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_CAR_ONCE)
				{
					//....
				}
				else
				{
					ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_GRADE_lIMIT;
					
					memset(CarNumGradeBuffer.CardAsn,0,sizeof(CarNumGradeBuffer.CardAsn));
					memcpy(CarNumGradeBuffer.CardAsn,TRINP.TR_CARD_ASN,sizeof(TRINP.TR_CARD_ASN));
				
					memset(CarNumGradeBuffer.LimitGrade,0,sizeof(CarNumGradeBuffer.LimitGrade));
					memcpy(CarNumGradeBuffer.LimitGrade,TRINP.TR_CARD_GRADE_LIMIT,sizeof(CarNumGradeBuffer.LimitGrade));
				
					memset(CarNumGradeBuffer.PumpGrade,0,sizeof(CarNumGradeBuffer.PumpGrade));
					memcpy(CarNumGradeBuffer.PumpGrade,TRINP.TR_TRANS_GRADE_CODE,sizeof(CarNumGradeBuffer.PumpGrade));
					
					//Write Grade Limit Log , Card Limit Grade , Pump Grade 
					memset(GradeLimitTmpBuf,0,sizeof(GradeLimitTmpBuf));
					sprintf((char *)GradeLimitTmpBuf,"Limit Grade : c:%x%x p:%x%x",CarNumGradeBuffer.LimitGrade[0],CarNumGradeBuffer.LimitGrade[1],CarNumGradeBuffer.PumpGrade[0],CarNumGradeBuffer.PumpGrade[1]);
					WriteLog(PROCESS_CR,GradeLimitTmpBuf);
				
					//CarNumGradeBuffer.NozzleNumber = GetLogicNzn(1);
					CarNumGradeBuffer.NozzleNumber = TRINP.TR_TRANS_NOZZLE_NUMBER;
					
					ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_GRADE_lIMIT);
					
					GradeLimNumber = 0;
				}
				if ((ProcExchange[PROCESS_CASH][PROCESS_CR] & CASH_GRADE_lIMIT) || CarOverFlag2 == 1)
				{
					if (CarOverFlag2 == 1)
					{
						CarOverFlag2 = 0;
						CarNumGradeBuffer.CarRecFlag = Cmd_CheckTimeOut;
					}
					
					StopTimer(CarNumberRecWait);
					CarOverFlag = 1;
					
					ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_CAR_ONCE);
					ProcExchange[PROCESS_CASH][PROCESS_CR] &= ~(CASH_GRADE_lIMIT);
					
					if (CarNumGradeBuffer.CarRecFlag == Cmd_CheckMatch)
					{
						if (memcmp(CarNumGradeBuffer.LimitGrade,TRINP.TR_CARD_GRADE_LIMIT,sizeof(TRINP.TR_CARD_GRADE_LIMIT)) == 0 &&
							memcmp(CarNumGradeBuffer.PumpGrade,TRINP.TR_TRANS_GRADE_CODE,sizeof(TRINP.TR_TRANS_GRADE_CODE)) == 0)
						{
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_LOCATION_LIMIT );
							//WriteLog(PROCESS_CR,"Grade Limit Is Matching");
							NextScreenID = DEFAULT_GRADELIMIT_REC;
						}
						else if (GradeLimNumber < 1)
						{
							ResetTimer(CarNumberRecWait);
							
							GradeLimNumber++;
							
							ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_GRADE_lIMIT;
					
							memset(CarNumGradeBuffer.CardAsn,0,sizeof(CarNumGradeBuffer.CardAsn));
							memcpy(CarNumGradeBuffer.CardAsn,TRINP.TR_CARD_ASN,sizeof(TRINP.TR_CARD_ASN));
				
							memset(CarNumGradeBuffer.LimitGrade,0,sizeof(CarNumGradeBuffer.LimitGrade));
							memcpy(CarNumGradeBuffer.LimitGrade,TRINP.TR_CARD_GRADE_LIMIT,sizeof(CarNumGradeBuffer.LimitGrade));
				
							memset(CarNumGradeBuffer.PumpGrade,0,sizeof(CarNumGradeBuffer.PumpGrade));
							memcpy(CarNumGradeBuffer.PumpGrade,TRINP.TR_TRANS_GRADE_CODE,sizeof(CarNumGradeBuffer.PumpGrade));
					
							//CarNumGradeBuffer.NozzleNumber = GetLogicNzn(1);
							CarNumGradeBuffer.NozzleNumber = TRINP.TR_TRANS_NOZZLE_NUMBER;
						
							//ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_GRADE_CONFORM;
							CarNumGradeBuffer.CarRecFlag = Cmd_CheckWrong;
							ProcExchange[PROCESS_CR][PROCESS_CASH] |= CASH_GRADE_lIMIT;
							
							sprintf((char *)GradeLimitTmpBuf,"Limit Grade W: c:%x%x p:%x%x",CarNumGradeBuffer.LimitGrade[0],CarNumGradeBuffer.LimitGrade[1],CarNumGradeBuffer.PumpGrade[0],CarNumGradeBuffer.PumpGrade[1]);
							WriteLog(PROCESS_CR,GradeLimitTmpBuf);
							CarOverFlag = 0;
						}
						else
						{
							GradeLimNumber = 0;
							GradeLimitRec();
							CarNumGradeBuffer.CarRecFlag = Cmd_CheckWrongErr;
							Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
							//WriteLog(PROCESS_CR,"Grade Limit Is Wrong Two");
							NextScreenID = DEFAULT_GRADELIMIT_REC;
						}
					}
					else if (CarNumGradeBuffer.CarRecFlag == Cmd_CheckNotMatch)
					{
						// Not Matching Grade
						
						GradeLimitRec();
						Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
						NextScreenID = DEFAULT_GRADELIMIT_REC;
						//WriteLog(PROCESS_CR,"Grade Limit Is Not Match");
					}
					else if (CarNumGradeBuffer.CarRecFlag == Cmd_CheckTimeOut)
					{
						// Time Out Show EMP Password 
						GradeLimitRec();
						Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
						NextScreenID = DEFAULT_GRADELIMIT_REC;
						//WriteLog(PROCESS_CR,"Limit Grade : Timer Out");
					}
				}
				else
				{
					CarNumGradeBuffer.CarRecFlag = Cmd_Checking;
					FillInfoMsg( M_GradeLimitMatching, 0, 0 );
				}
			break;
#endif
			// Check Locationg Limit
			case CR_ICCARD_CHECK_LOCATION_LIMIT:

				// Check Location Limit
				retval = Check_Card_Location_Limit();
					
				if( retval != 1 )
				{
					// Check Grade Brand Limit End
					if( retval == 0x9000 )
					{
#ifdef MAKE_CARD_MODE2
						// For Sinopec Mode2, Do Not Check Fuelling Limit For Loyalty App
						if( TRINP.TR_TRANS_DEBITSOURCE == 0x01 )
						{
							// Set Max Fuelling Value Is Max
							memcpy( TRINP.TR_MAX_PRESET_AMOUNT, "\xFF\xFF\xFF\xFF", 4 );
							memcpy( TRINP.TR_MAX_PRESET_VOLUME, "\xFF\xFF\xFF\xFF", 4 );

							// Loyalty App, Check Preset Next
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_PRESET );
						}
						else
						{
							// Brand Not Limited, Check Fuelling Limit Next
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_FUELLING_LIMIT );
						}
#else
						// Brand Not Limited, Check Fuelling Limit Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_FUELLING_LIMIT );
#endif
					}
					else
					{
						// Location Limited, Power Off Next
//						// Write Log
//						CRWriteLog( M_MsgLocationLimitedExceed );

						// Display: " Location Limited"
//						FillInfoMsg( M_MsgLocationLimitedExceed, 0, 0 );
					
						// Inform POS Brand Limited Card
						memcpy( POS_CR_Buf.ICCARD_STATE, "\x10\x02", sizeof( POS_CR_Buf.ICCARD_STATE ));
						
						// For Record Brand Limited Card
						memcpy( TRINP.TR_CARD_GREY_TAC, "\x10\x02\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
						
						SaveICCardData();

						// Save It For Record
						FillCardErrorData();

						// Auth Error
						FillAuthError();

						// For Write The Running Log
						JournalFlag = 2;

						// Form Journal Next
						Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_9;
                        ErrorFlag = retval;

//						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			// Check Volume And Amount Per Fuelling Limit
			case CR_ICCARD_CHECK_FUELLING_LIMIT:

				// Check Amount And Volume Limit Every Fuelling
				retval = Check_Card_Fuelling_Limit();
					
				if( retval != 1 )
				{
					// Check Grade Brand Limit End
					if( retval == 0x9000 )
					{
#ifdef MAKE_CARD_MODE2
//						UpdatePresetValue();
#endif
						// Brand Not Limited, Check Fuelling Times Next
						if( TimesAmountTotalLimit() == 1 )
						{
							// Need To Read The IC Card Records To Get The Fuelling Times And Totals
							RecordListNumber = 1;
							// Fuelling Times Of The Transaction Day						
							TRINP.TR_TIMES_DAY = 0;
							// Amount Total Of The Transaction Day
							memset( TRINP.TR_AMOUNT_TOTAL_DAY, 0x00, sizeof( TRINP.TR_AMOUNT_TOTAL_DAY ));

//							// Write Log
//							CRWriteLog( M_FuellingLimited );

							// Display: " Read Card Error"
							FillInfoMsg( M_FuellingLimited, 0, 0 );
					
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_READ_RECORD );
						}
						else
						{
							// No Times And Amount Total Limit, Check Preset Next
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_PRESET );
						}
					}
					else
					{
						// Auth Error
						FillAuthError();

//						// Write Log
//						CRWriteLog( M_CardReadError );

						// Display: " Read Card Error "
						FillInfoMsg( M_CardReadError, 0, 0 );
					
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_10;
                        ErrorFlag = retval;

						// Display: " Location Limited"
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			// IC Card Read All Trans Records In IC Card
			// For Fuelling Times And Amount Total Limit
			case CR_ICCARD_READ_RECORD:

				 retval = CR_ICCARD_Read_Record();
				
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Record List++
						RecordListNumber++;

						// Calc The Fuelling Times And The Fuelling Amount Total
						CalcFuelling();
					}
					else if(  retval == 0x6A83 )
					{
						// No Record In IC Card
						// Check Times Limit Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_TIMES_LIMIT );
					}
					else
					{
						// Auth Error
						FillAuthError();

//						// Write Log
//						CRWriteLog( M_CardReadError );

						// Display: " Read Card Error"
						FillInfoMsg( M_CardReadError, 0, 0 );
					
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_11;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
				 		
			break;

			// Check Fuelling Times Per Day Limit
			case CR_ICCARD_CHECK_TIMES_LIMIT:

				// Check Times Limit
				if( TRINP.TR_TIMES_DAY < TRINP.TR_CARD_TIMES_DAY_LIMIT )
				{
					// Did Not Exceed The Times Limit, Check Amount Total Limit Next
					Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_AMOUNT_TOTAL_LIMIT );
				}
				else
				{
					// Exceed The Times Limit, Stop Process
					// Display: " Exceed Times "
//					// Write Log
//					CRWriteLog( M_MsgTimesLimitedExceed );

					// Display: " Read Card Error"
					FillInfoMsg( M_MsgTimesLimitedExceed, 0, 0 );
					
					// Power Off IC Card And Spring Out
					// Inform POS Brand Limited Card
					memcpy( POS_CR_Buf.ICCARD_STATE, "\x10\x03", sizeof( POS_CR_Buf.ICCARD_STATE ));
						
					// For Record Brand Limited Card
					memcpy( TRINP.TR_CARD_GREY_TAC, "\x10\x03\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
						
					SaveICCardData();
			
					// Save It For Record
					FillCardErrorData();

					// Auth Error
					FillAuthError();

					// For Write The Running Log
					JournalFlag = 3;

					// Form Journal Next
					Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
				}

			break;

			// Check Fuelling Amount Total Per Day Limit
			case CR_ICCARD_CHECK_AMOUNT_TOTAL_LIMIT:

				// Check Amount Total Limit
				if( CheckAmountTotalLimit() == 1 )
				{
					// No Limit, Check Preset Next
					Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_CHECK_PRESET );
				}			
				else
				{
					// Limited, Stop
					// Display: " Exceed Amount Total "
					// Power Off IC Card And Spring Out
					// Inform POS Brand Limited Card
					memcpy( POS_CR_Buf.ICCARD_STATE, "\x10\x04", sizeof( POS_CR_Buf.ICCARD_STATE ));
						
					// For Record Brand Limited Card
					memcpy( TRINP.TR_CARD_GREY_TAC, "\x10\x04\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
						
					SaveICCardData();

					// Save It For Record
					FillCardErrorData();

					// Auth Error
					FillAuthError();

//					// Write Log
//					CRWriteLog( M_MsgAmountLimitedExceed );

					// Display: " Fuelling Amount Limited"
					FillInfoMsg( M_MsgAmountLimitedExceed, 0, 0 );
					
					// For Write The Running Log
					JournalFlag = 4;

					// Form Journal Next
					Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );

					// For Write Spring Card Log
					PowerDownFlag = Pd_Flag_12;
                    ErrorFlag = retval;

//					Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
				}

			break;


			case CR_ICCARD_CHECK_PRESET:
				
#ifdef MAKE_CARD_MODE2				
				// Check Wether Auth Was Get Before Nozzle On
				if( PowerDown.PdTrans.PD_TRANS_FLAG == AUTH_RESP )
				{
#ifdef MAKE_PSAM_VERIFY
					// Initialize Grey Lock Next
					Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_PSAM_GET_RANDOM );
#else
					// Auth Get, Debit Or Initialize Grey Lock Next
					if( TRINP.TR_CARD_ASN[2] != CUSTOMER_CARD )
					{
						if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
						{
							// Maintainance Card, Initialize Grey lock Next
							// Initialize Grey Lock Next
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
						}
						else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
						{
							// Test Card, Initialize Grey lock Next
							// Initialize Grey Lock Next
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
						}
						else
						{
							// Employee Card, Debit 0 Next
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_PURCHASE );
						}
					}
					else
					{
						// Cumstomer Card, Initialize Grey lock Next
						// Initialize Grey Lock Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
					}
#endif	// MAKE_PSAM_VERIFY

					//
					break;
				}
				else
				{
					// Did Not Authed Before, Check Work Mode Here
					TRINP.TR_WORK_MODE = GetWorkMode();
					TRINP.TR_AUTH_MODE = GetAuthMode();

					// For Sinopec Mode2, PayX Byte	For Calc Auth
					if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
					{
						// Customer Card, Default
						// Changed After Auth Response
						if( TRINP.TR_TRANS_DEBITSOURCE == 0x01 )
						{
							TRINP.TR_PAY_X = 0x02;
						}
						else
						{
							TRINP.TR_PAY_X = 0x05;
						}
					}
					else
					{
						// Employee Card, Default
						// Employee Card, Default
						if( TransAmountPay == 0x01 )
						{
							// Changed After Auth Response
							TRINP.TR_PAY_X = 0x17;
						}
						else
						{
							// Changed After Auth Response
							TRINP.TR_PAY_X = 0x07;
						}
					}

					if( TRINP.TR_WORK_MODE != 0x01 )
					{
						// Host POS, Not FS
//						TRINP.TR_PAY_X |= WORKOFFLINE;
//						TRINP.TR_PAY_X |= 0x10;

						TRINP.TR_TRANS_TYPE |= ( WORKOFFLINE | LOCALLIST );
						PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
					}
					else
					{
						TRINP.TR_TRANS_TYPE &= ( ~( LOCALLIST | WORKOFFLINE ));
						PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;
					}
				}

				if( TRINP.TR_WORK_MODE == 0x01 )
				{
					// Send Auth Request Next
					Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_WAITING_MAC );
				}
				else
#endif	// MAKE_CARD_MODE2
				{
					if( SetPresetValue() == 0x01 )
					{
#ifdef MAKE_PSAM_VERIFY
						// Initialize Grey Lock Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_PSAM_GET_RANDOM );
#else	// MAKE_PSAM_VERIFY

#ifdef MAKE_CARD_MODE2
						// Offline Get Trans Time
						// Get Trans Date And Time For Set Grey
						GetTransTime();

						if( TRINP.TR_CARD_ASN[2] != CUSTOMER_CARD )
						{
							if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
							{
								// Maintainance Card, Initialize Grey lock Next
								// Initialize Grey Lock Next
								Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
							}
							else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
							{
								// Test Card, Initialize Grey lock Next
								// Initialize Grey Lock Next
								Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
							}
							else
							{
								// Employee Card, Debit 0 Next
								Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_PURCHASE );
							}
						}
						else
						{
							// Cumstomer Card, Initialize Grey lock Next
							// Initialize Grey Lock Next
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
						}
#else	
						// Initialize Grey Lock Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
#endif	// MAKE_CARD_MODE2

#endif	//  MAKE_PSAM_VERIFY
					}
					else
					{
						// Auth Error
						FillAuthError();

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_13;
                        ErrorFlag = 0xE004;

						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
			break;

#ifdef MAKE_PSAM_VERIFY
			case CR_PSAM_GET_RANDOM:
				
				retval = CR_PSAM_Get_Random( PSAM1 );

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						memcpy( Pump_CR_Buf.PSAMRandom, TRINP.TR_PSAM_ACT_RID_RANDOM, sizeof( Pump_CR_Buf.PSAMRandom ));

						// Calc Crypted Data Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_PUMP_VERIFY );
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_V_PSAM_Random_Error, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_76;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_PUMP_VERIFY:

				// Get Pump Calc Verify
				retval = GetPumpVerify();

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Verify Crypted Data Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_PSAM_ANTI_FRAUD_VERIFY );
					}
					else
					{
						// Display " IC Card Error"
						FillInfoMsg( M_V_VerifyError, 0, 0 );

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_77;
                        ErrorFlag = 0xE005;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_PSAM_ANTI_FRAUD_VERIFY:

				// Get Pump Calc Verify
				retval = CR_PSAM_Anti_Fraud_Verify( PSAM1 );

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
#ifdef MAKE_CARD_MODE2
						if( TRINP.TR_CARD_ASN[2] != CUSTOMER_CARD )
						{
							if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
							{
								// Maintainance Card, Initialize Grey lock Next
								// Initialize Grey Lock Next
								Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
							}
							else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
							{
								// Test Card, Initialize Grey lock Next
								// Initialize Grey Lock Next
								Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
							}
							else
							{
								// Employee Card, Debit 0 Next
								Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_PURCHASE );
							}
						}
						else
						{
							// Cumstomer Card, Initialize Grey lock Next
							// Initialize Grey Lock Next
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
						}
#else
						// Verify Ok, Initialize Grey Lock Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
#endif	// MAKE_CARD_MODE2
					}
					else
					{
						// Inform POS Verify Error
						if( ProcStatus[PROCESS_POS] & POS_ONLINE )
						{
							if(( retval / 0x100 ) == 0x6B )
							{
								retval %= 0x6B;

								// Fill The Buffer
								FillPOSMessage( 1, 0, retval );

//								// Set Bit, To Inform POS Send Message
//								ProcExchange[PROCESS_CR][PROCESS_POS] |= SEND_MESSAGE;

								// Calc POS Message MAC Next
								Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_PSAM_VERIFY_MSG_SINOPEC2 );
							}
							else
							{
								// For Write Spring Card Log
								PowerDownFlag = Pd_Flag_78;
                                ErrorFlag = retval;

								// Run Error
								// Power Off IC Card And Spring Out
								Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
							}
						}
						else
						{
							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_80;
                            ErrorFlag = retval;

							// Run Error
							// Power Off IC Card And Spring Out
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}

						// Display " IC Card Error"
						FillInfoMsg( M_V_VerifyError, 0, 0 );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

			case CR_PSAM_VERIFY_MSG_SINOPEC2:

				retval = CR_PSAM_Select_Sinopec2( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Initialize DES Encrypt In PSAM
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_PSAM_VERIFY_MSG_INIT_MAC );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_81;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			case CR_PSAM_VERIFY_MSG_INIT_MAC:

				retval = CR_PSAM_Init_DES_Encrypt( PSAM1 );

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Prepare The DES Buffer
						// RevcBuffer + 127: Save The Data Length
						// The Data Buffer Starts From RecvBuffer + 128
						// Max Length Is 120
						
						// Clear Buffer
						memset( &RecvBuffer[127], 0x00, 110 );

						// Copy Data To The Buffer: Lead 8 Bytes 0x00 As Initiate Value,
						// Next 36 Bytes Data
						Asc2Hex( &RecvBuffer[128+8], &POS_CR_Buf.InforMessage[8], 36 );

						// Pad 0x80, 0x00, 0x00, 0x00
						memcpy( &RecvBuffer[128+8+36], "\x80\x00\x00\x00", 4 );
						RecvBuffer[127] = 48;
						
						// Initialize DES Encrypt In PSAM
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_PSAM_VERIFY_MSG_MAC );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_82;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On....
				}

			break;

			case CR_PSAM_VERIFY_MSG_MAC:
				
				retval = CR_PSAM_DES_Encrypt0( PSAM1 );

				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Save Message MAC
						Hex2Asc( &POS_CR_Buf.InforMessage[80], &RecvBuffer[9], 4 );

						// Inform POS Register Ok
						if( ProcStatus[PROCESS_POS] & POS_ONLINE )
						{
							// Set Bit, To Inform POS Send Message
							ProcExchange[PROCESS_CR][PROCESS_POS] |= SEND_MESSAGE;
						}

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_83;
                        ErrorFlag = retval;

						// Power Off IC Card And Spring Out, Anyway....
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_84;
                        ErrorFlag = retval;

						// Run Error
						// Power Off IC Card And Spring Out
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;

//			case CR_PSAM_VERIFY_MSG_SINOPEC1:
//				
//				retval = CR_PSAM_Select_Sinopec1( PSAM1 );
//
//				// Check Command Return
//				if( retval != 1 )
//				{
//					if( retval == 0x9000 )
//					{
//						// Power Off IC Card And Spring Out
//						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
//					}
//					else
//					{
//						// For Write Spring Card Log
//						PowerDownFlag = Pd_Flag_85;
//
//						// Run Error
//						// Power Off IC Card And Spring Out
//						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
//					}
//				}
//				else
//				{
//					// Command Running, Go On
//				}
//
//			break;

#endif
				
#ifdef MAKE_CARD_MODE2

			case CR_WAITING_MAC:

				if( TRINP.TR_WORK_MODE != 0x01 )
				{
					// Initialize Purchase, Next
					Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_ICCARD_INIT_PURCHASE );
				}
				else
				{

					if( SetPresetValue() == 0x01 )
					{
						// Preset Ok, Go On....
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_109;

						// Error, Exit
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );

						// Preset Error, Return;
						break;
					}
					
					// Prepare The Calc MAC Elements
					// MAC Process Return
					CR_MAC_CommandType = CR_WAITING_AUTH;
					CR_Proc_MAC_Step = CR_ICCARD_PUMP_CALL_PROC;

					// MAC Type:
					CR_MACType = CR_MAC_TYPE_MAC;

					// Get The TTC, For Sinopec Mode2
					GetTransTTC();

					// Get Trans Date And Time For Set Grey
					GetTransTime();
					
					// Update The Power Down Protection Message
					PowerDown.PdTrans.PD_TRANS_FLAG = AUTH_RESP;

					// Save PumpCall Information
					SavePumpCallData( 0 );

					// Fill The Data
					if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
					{
//						// Customer Card Auth Data
//						FillAuthReqMAC_CST();

						// Customer Card, Calc Pin Block Next
						// Calc PIN Block, Next
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_WAITING_MAC );
					}
					else
					{
						// Employee Card Auth Data
						FillAuthReqMAC_EMP();

						// Calc MAC, Next
						Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_SELECT_SINOPEC2 );
					}
				}
			
			break;

			case CR_WAITING_PIN:

					// Prepare The Calc MAC Elements
					// MAC Process Return
					CR_MAC_CommandType = CR_WAITING_AUTH;
					CR_Proc_MAC_Step = CR_ICCARD_PUMP_CALL_PROC;

					// MAC Type:
					CR_MACType = CR_MAC_TYPE_MAC;

					// Customer Card Auth Data
					FillAuthReqMAC_CST();

					// Calc MAC, Next
					Set_Next_Command( CR_CALC_MAC_PROC, CR_PSAM_SELECT_SINOPEC2 );

			break;

			case CR_WAITING_AUTH:

				// Request POS Auth Data Next
				if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
				{
					// Customer Card Auth POS
					retval = AuthPOSOnline_CST();
				}
				else
				{
					// Employee Card Auth POS
					retval = AuthPOSOnline_EMP();
				}

				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Work Online, Auth Pump Next
						// Check Auth Response
						if( TRINP.TR_AUTH_FEEDBACK == 0x01 )
						{
							// Save PumpCall Information
							SavePumpCallData( 0 );
						
							if( PresetMode2 == 1 )
							{
								// Clear Sign
								PresetMode2 = 0;
								
								// Auth Ok, Update Screen
								UpdateScreen();

								// Auth In Card Ready, Return
								// POS Auth OK, Return To Card Ready Next
								Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
							}
							else
							{
								if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
								{
									// POS Auth OK, Start Initialize Next
									Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
								}
								else
								{
									if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
									{
										// Maintainance Card, Initialize Grey lock Next
										// Initialize Grey Lock Next
										Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
									}
									else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
									{
										// Test Card, Initialize Grey lock Next
										// Initialize Grey Lock Next
										Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
									}
									else
									{
										// Employee Card, Debit 0 Next
										Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_PURCHASE );
									}
								}
							}
						}
						else if( TRINP.TR_AUTH_FEEDBACK == 0x02 )
						{
							// Save PumpCall Information
							SavePumpCallData( 0 );
						
							// Mark Send Record After Trans
							SendCardRecord = 0x01;

							if( PresetMode2 == 1 )
							{
								// Clear Sign
								PresetMode2 = 0;

								
								// Auth Ok, Update Screen
								UpdateScreen();

								// Auth In Card Ready, Return
								// POS Auth OK, Return To Card Ready Next
								Set_Next_Command( CR_ICCARD_READY_PROC, CR_CARDREADER_CHECK_CARD );
							}
							else
							{
								if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
								{
									// POS Auth OK, Start Initialize Next
									Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
								}
								else
								{
									if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
									{
										// Maintainance Card, Initialize Grey lock Next
										// Initialize Grey Lock Next
										Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
									}
									else if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
									{
										// Test Card, Initialize Grey lock Next
										// Initialize Grey Lock Next
										Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_GREY_LOCK );
									}
									else
									{
										// Employee Card, Debit 0 Next
										Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_PURCHASE );
									}
								}
							}
						}
						else if( TRINP.TR_AUTH_FEEDBACK == 0x03 )
						{
							// Black List Card
							// Inform POS Black Card
							memcpy( POS_CR_Buf.ICCARD_STATE, "\x00\x01", sizeof( POS_CR_Buf.ICCARD_STATE ));
							
							// For Record Black Card
							memcpy( TRINP.TR_CARD_GREY_TAC, "\x00\x01\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
							
							SaveICCardData();

			  				// Display " IC Card In Black List "
	  						FillInfoMsg( M_CardBlackList, 0, 0 );

							// IC Card Is Black, Lock PIN, Next
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_ICCARD_PIN_LOCK );
						}
						else if( TRINP.TR_AUTH_FEEDBACK == 0x04 )
						{
			  				// Display " White List Not Fit "
	  						FillInfoMsg( M_NotWhiteList, 0, 0 );

							FillCardErrorData_Mode2();

							// For Record Black Card
							memcpy( TRINP.TR_CARD_GREY_TAC, "\x20\x01\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
							
							// Save To PowerDown.PdTrans, Card Tac, For Limited Card
							memcpy( PowerDown.PdTrans.PD_CARD_GTAC, TRINP.TR_CARD_GREY_TAC, sizeof( PowerDown.PdTrans.PD_CARD_GTAC ));

							// For Write The Running Log
							JournalFlag = 29;

							// Form Journal Next
							Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );

//							// For Write Spring Card Log
//							PowerDownFlag = Pd_Flag_98;
//
//							// Error, Exit
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
						else if( TRINP.TR_AUTH_FEEDBACK == 0x05 )
						{
			  				// Display " Auth Rejected "
//							memset( InfoLine, 0x00, sizeof( InfoLine ));
//							memcpy( InfoLine, TRINP.TR_ADD_IN, TRINP.TR_ADD_IN_LEN );
//							FillInfoMsg( M_InfoLine, 0, 1 );

			  				// Display " Auth Rejected "
							memset( InfoLine, 0x00, sizeof( InfoLine ));
							if( TRINP.TR_ADD_IN_LEN > 19 )
							{
								TRINP.TR_ADD_IN_LEN = 19;
							}

							InfoLine[0]	= '.';
							memcpy( &InfoLine[1], TRINP.TR_ADD_IN, TRINP.TR_ADD_IN_LEN );
							FillInfoMsg( M_InfoLine, 0, 1 );

							FillCardErrorData_Mode2();

							// For Record Black Card
							memcpy( TRINP.TR_CARD_GREY_TAC, "\x20\x02\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
							
							// Save To PowerDown.PdTrans, Card Tac, For Limited Card
							memcpy( PowerDown.PdTrans.PD_CARD_GTAC, TRINP.TR_CARD_GREY_TAC, sizeof( PowerDown.PdTrans.PD_CARD_GTAC ));

							// For Write The Running Log
							JournalFlag = 30;

							// Form Journal Next
							Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );

//							// For Write Spring Card Log
//							PowerDownFlag = Pd_Flag_99;
//
//							// Error, Exit
//							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
						else
						{
							// For Write Spring Card Log
							PowerDownFlag = Pd_Flag_100;
                            ErrorFlag = retval;

							// Error, Exit
							Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
						}
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_101;
                        ErrorFlag = retval;

						// Error, Exit
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}

			break;


			case CR_ICCARD_INIT_PURCHASE:

				retval = CR_ICCARD_Init_Purchase();
						
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_PSAM_INIT_PURCHASE );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_93;
                        ErrorFlag = retval;

						// Display " IC Card Error"
						FillInfoMsg( M_CardReadError, 0, 0 );

						// IC Card Not Exist, Power Off The Card Reader
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
					 		
			break;

			case CR_PSAM_INIT_PURCHASE:

				retval = CR_PSAM_Init_Purchase( PSAM1 );
						
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_DEBIT_PURCHASE );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_94;
                        ErrorFlag = retval;

						// Display " IC Card Error"
						FillInfoMsg( M_CardReadError, 0, 0 );

						// IC Card Not Exist, Power Off The Card Reader
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
					 		
			break;

			case CR_ICCARD_DEBIT_PURCHASE:

				retval = CR_ICCARD_Debit_Purchase();
						
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
 						// Auth Pump
 						CardAuthPump();

//						// Card TAC OK
//						TRINP.TR_TRANS_TYPE |= CARDTACREAD;
						
						// Update The Power Down Protection Message
						PowerDown.PdTrans.PD_TRANS_FLAG = DEBIT_ZERO;

						// Save The Transaction TAC
						memcpy( PowerDown.PdTrans.PD_CARD_MAC3, TRINP.TR_CARD_MAC3, sizeof( PowerDown.PdTrans.PD_CARD_MAC3 ));

						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_PSAM_CREDIT_MAC2 );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_95;
                        ErrorFlag = retval;

						// Display " IC Card Error"
						FillInfoMsg( M_CardReadError, 0, 0 );

						// IC Card Not Exist, Power Off The Card Reader
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
					 		
			break;

			case CR_PSAM_CREDIT_MAC2:

				retval = CR_PSAM_Credit_MAC2( PSAM1 );
						
				// Check Command Return
				if( retval != 1 )
				{
					if( retval == 0x9000 )
					{
						// Start Fuelling, Next
						Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_CARDREADER_CHECK_CARD );
					}
					else
					{
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_96;
                        ErrorFlag = retval;

						// Display " IC Card Error"
						FillInfoMsg( M_CardReadError, 0, 0 );

						// Do Not Power Off Card, Auth Pump Anyway
						// Start Fuelling, Next
						Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_CARDREADER_CHECK_CARD );
						
						// IC Card Not Exist, Power Off The Card Reader
//						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
					 		
			break;

#endif	// MAKE_CARD_MODE2


			case CR_ICCARD_INIT_GREY_LOCK:

#ifdef MAKE_CARD_MODE2
				// For Sinopec Mode2, Customer Card And Host Account Pay, Do Not Issue
				// Initialize Grey Lock, Just Initialize Purchase
				if( TRINP.TR_WORK_MODE == 0x01 )
				{
					if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
					{
						if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
						{
							// Host Account Payment, Do Not Initialize Grey Lock, But Initialize Purchase
							// IC Card Initialize Purchase Next
							Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_INIT_PURCHASE );

							break;
						}
						else
						{
							// Do Nothing.... Initialize Grey Lock Still
						}
					}
					else
					{
						// Do Nothing.... Initialize Grey Lock Still
					}
				}
				else
				{
					// Work Offline, Do Nothing.... Initialize Grey Lock Still
				}
#endif

				// Check PSAM CRC, In Future ????
				// Initialize IC Card Grey Lock
				retval = CR_ICCARD_Init_Grey_Lock();
						
					// Check Command Return
				if( retval != 1 )
				{                    
					if( retval == 0x9000 )
					{
#ifdef MAKE_CARD_MODE2
						// For Sinopec Mode2, Do Not Get Time Here,
						// Get Time At Auth On Line Or Auth Offline
#else
						// Get Trans Date And Time For Set Grey
						GetTransTime();
#endif	// MAKE_CARD_MODE2

						// IC Card Initialize Grey Lock Ok, Initialize PSAM Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_PSAM_INIT_GREY_LOCK );
					}
					else
					{
//						// Write Log
//						CRWriteLog( M_CardInitGreyLockErr );

						// Display: " IC Card Initialize Grey Lock Error"
						FillInfoMsg( M_CardInitGreyLockErr, 0, 0 );
//Douzhq 20140917 Add , Voice Add
#ifdef MAKE_CASH_CR
						if (Cash_CR_Buf.CashType == 0x01 && CRType == CR_TYPE_CASH)
						{
							ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_FAULTERR;
						}
#endif
						// Auth Error
						FillAuthError();

						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_14;
                        ErrorFlag = retval;

						// Error, Exit
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
					 		
			break;

			case CR_PSAM_INIT_GREY_LOCK:
                
				// Initialize PSAM Grey Lock
				retval = CR_PSAM_Init_Grey_Lock( PSAM1 );
						
					// Check Command Return
				if( retval != 1 )
				{ 
					if( retval == 0x9000 )
					{
//						// Write Log
//						CRWriteLog( M_CardSetGreyLock );

//						// Display: " IC Card Set Grey Lock"
//						FillInfoMsg( M_CardSetGreyLock, 0, 0 );
					
						// PSAM Card Initialize Grey Lock Ok, Set IC Card Grey Next
						Set_Next_Command( CR_ICCARD_PUMP_CALL_PROC, CR_ICCARD_SET_GREY_LOCK );
						
#ifdef MAKE_CARD_MODE2
						// Do Nothing....
#else
						// Update The Power Down Protection Message
						PowerDown.PdTrans.PD_TRANS_FLAG = SET_GREY;
#endif	// MAKE_CARD_MODE2

					}
					else
					{
						// Auth Error
						FillAuthError();

//						// Write Log
//						CRWriteLog( M_PSAMInitGreyLockErr );

                        if( retval == 0x9403 )
                        {
                            // Display: " PSAM Key Version Error "
                            FillInfoMsg( M_Test_KeyVer_Err, 0, 0 );
                        }
                        else
                        {
						// Display: " PSAM Initialize Grey Lock "
						FillInfoMsg( M_PSAMInitGreyLockErr, 0, 0 );
                        }
					
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_15;
                        ErrorFlag = retval;

						// Error, Exit
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
					 		
			break;

			case CR_ICCARD_SET_GREY_LOCK:

				// Set IC Card Grey Lock
				retval = CR_ICCARD_Set_Grey_Lock();
						
					// Check Command Return
				if( retval != 1 )
				{                
					if( retval == 0x9000 )
					{
						CardAuthPump();
						// Refresh Grey Lock Message First
						Set_Next_Command( CR_ICCARD_FUELLING_PROC, CR_ICCARD_GET_LOCK_PROOF );
					}
					else
					{
						// Auth Error
						FillAuthError();

//						// Write Log
//						CRWriteLog( M_CardSetGreyLockErr );
                                                                                              
						FillInfoMsg( M_CardSetGreyLockErr, 0, 0 );
					
						// For Write Spring Card Log
						PowerDownFlag = Pd_Flag_16;
                        ErrorFlag = retval;

						// Error, Exit
						Set_Next_Command( CR_ICCARD_INSERT_PROC, CR_CARDREADER_POWEROFF_CARD );
					}
				}
				else
				{
					// Command Running, Go On
				}
					 		
			break;
			
			default:
			break;
		}
	}
}



/*
 ** Check_Card_Grade_Brand_Limit
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check The Card Limit, Include Grade Brand Limit, Region Limit
 *												Times Limit, Amount Total Limit
 *
 *  RETURNS: 1: Running; 0x9000: IC Card Ok, Go To Next Command
 *			 0: IC Card Is Limited, Stop The Process;
 *
 *	Note: The Vehicle Number Limited Should Be Checked In IC Card Insert Proc,
 *			The Black Card Should Be Checked In BlackList Check,
 *			And The Expired Card Should Be Checked In Card Validate Process
 *
 */
unsigned short int Check_Card_Grade_Brand_Limit( void )
{
	unsigned short int retval;
	unsigned short int CardGrade, TransGrade;

	// Default Running
	retval = 1;

	// Get Card Grade
	CardGrade = CharToShortIntCopy( TRINP.TR_CARD_GRADE_LIMIT );
	
	// Get Trans Grade
	TransGrade = CharToShortIntCopy( TRINP.TR_TRANS_GRADE_CODE );

	// Check Grade Brand Limited
	if( CardGrade != 0xFFFF )
	{
		// Need Check Grade Brand Limited
		// The High Nibble Of The High Byte ( HHB )
		// The Low Nibble Of The High Byte ( LHB )
		// The High Nibble Of The Low Byte ( HLB )
		// The Low Nibble Of The Low Byte ( LLB )
		if(( CardGrade & 0x000F ) != 0 )
		{
			// The LLB Is Not '0', All The 4 Nibbles Should Be Same
			if( CardGrade == TransGrade )
			{
				// Grade Same, Go On....
				retval = 0x9000;
			}
			else
			{
				// Grade Limited
				retval = 0;
			}
		}
		else if(( CardGrade & 0x00FF ) != 0 )
		{
			// LLB == 0, Only Check The HHB, LHB And HLB
			if(( CardGrade & 0xFFF0 ) == ( TransGrade & 0xFFF0 ))
			{
				// Grade HHB, LHB And HLB Same, Go On....
				retval = 0x9000;
			}
			else
			{
				// Grade Limited
				retval = 0;
			}
		}
		else if(( CardGrade & 0x0FFF ) != 0 )
		{
			// LLB And HLB == 0, Check The LHB And HHB
			if(( CardGrade & 0xFF00 ) == ( TransGrade & 0xFF00 ))
			{
				// Grade HHB And LHB Same, Go On....
				retval = 0x9000;
			}
			else
			{
				// Grade Limited
				retval = 0;
			}
		}
		else
		{
			// LLB, HLB And LHB == 0, Check The HHB
			if(( CardGrade & 0xF000 ) == ( TransGrade & 0xF000 ))
			{
				// Grade HHB Same, Go On....
				retval = 0x9000;
			}
			else
			{
				// Grade Limited
				retval = 0;
			}
		}
	}
	else
	{
		// No Grade Brand Limit
		retval = 0x9000;
	}


#ifdef MAKE_CARD_MODE2
	if( retval == 0 )
	{
		if(( TRINP.TR_CARD_HOLDER_NEW_FLAG & CARD_MODE2_NON ) == CARD_MODE2_NON )
		{
			// Not Mode2 Card, Go On....
		}
		else if(( TRINP.TR_CARD_HOLDER_NEW_FLAG & CARD_MODE2_NON ) == 0x00 )
		{
			// Mode2 Card, But RFU, Do Not Care, Go On....
		}
		else
		{
			// Grade Limited, Check Extend Limit For Mode 2 Sinopec
			retval = CheckGradeLimitExtend_Mode2();
		}
	}
#endif

	return retval;
}



/*
 ** Check_Card_Location_Limit
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check The Location Limit, Include: Province, City, SuperiorUnit And Station Limit 
 *
 *  RETURNS: 1: Running; 0x9000: IC Card Ok, Go To Next Command
 *			 0: IC Card Is Limited, Stop The Process;
 *
 */
unsigned short int Check_Card_Location_Limit( void )
{
	unsigned short int retval;
	unsigned char flag;
	
	// Default Running
	retval = 1;

	if( TRINP.TR_CARD_LOCATION_LIMIT_MODE != 0xFF )
	{
		if( TRINP.TR_CARD_LOCATION_LIMIT_MODE == 0x01 )
		{
			flag = FindInMem( &TRINP.TR_PROVINCE, 1, TRINP.TR_CARD_LOCATION_LIMIT, sizeof( TRINP.TR_CARD_LOCATION_LIMIT ));

			if( flag == 0 )
			{
				// Display: " Location Limited"
				FillInfoMsg( M_ProvinceLimited, 0, 0 );
			}
		}
		else if( TRINP.TR_CARD_LOCATION_LIMIT_MODE == 0x02 )
		{
			flag = FindInMem( &TRINP.TR_PROVINCE, 2, TRINP.TR_CARD_LOCATION_LIMIT, sizeof( TRINP.TR_CARD_LOCATION_LIMIT ));

			if( flag == 0 )
			{
				// Display: " Location Limited"
//				FillInfoMsg( M_CityLimited, 0, 0 );
				DispLocationLimit( 1 );
			}
		}
		else if( TRINP.TR_CARD_LOCATION_LIMIT_MODE == 0x03 )
		{
			flag = FindInMem( TRINP.TR_SUPERIOR_UNIT, 4, TRINP.TR_CARD_LOCATION_LIMIT, sizeof( TRINP.TR_CARD_LOCATION_LIMIT ));

			if( flag == 0 )
			{
////#ifdef MAKE_CARD_MODE2
////				// For Sinopec Mode2, Do Not Display " Superior Limited"
////				// Display: " Location Limited"
////				FillInfoMsg( M_CityLimited, 0, 0 );
////#else
//				// Display: " Location Limited"
//				FillInfoMsg( M_SuperiorUnitLimited, 0, 0 );
////#endif
				DispLocationLimit( 2 );
			}
		}
		else if( TRINP.TR_CARD_LOCATION_LIMIT_MODE == 0x04 )
		{
			flag = FindInMem( TRINP.TR_STATION_ID, 4, TRINP.TR_CARD_LOCATION_LIMIT, sizeof( TRINP.TR_CARD_LOCATION_LIMIT ));

			if( flag == 0 )
			{
				// Display: " Location Limited"
//				FillInfoMsg( M_OilStationLimited, 0, 0 );
				DispLocationLimit( 3 );
			}
		}
		else
		{
			// Error, Limited
			// Display: " Location Limited Para Error"
			retval = 0;
		}
	
		// Check Include In The Location List Or Not
		if( flag == 0x01 )
		{
			// Found, Go On....
			retval = 0x9000;
		}
		else
		{
			// limited, Stop
			retval = 0;
		}
	}
	else
	{
		retval = 0x9000;
	}

	return retval;
}



/*
 ** Check_Card_Fuelling_Limit
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check The Amout, Volume And Kilogram Limit For Per Fuelling
 *
 *  RETURNS: 1: Running; 0x9000: IC Card Ok, Go To Next Command
 *			 0: IC Card Is Limited, Stop The Process;
 *
 *	Note: The Max Fuelling Is Save In TR_MAX_PRESET_AMOUNT,
 *			Change Both Litre And Kilogram To Amount Limit After Exit	
 *
 */
unsigned short int Check_Card_Fuelling_Limit( void )
{
	unsigned short int retval;
	unsigned short int limits;
	
	unsigned short int Price;
	unsigned int PresetValue;
	
	char tmpbuf[9];


	// Default Running
	retval = 1;

	// Get The Limit
	limits = CharToShortIntCopy( TRINP.TR_CARD_VOLUM_AMOUNT_LIMIT );
	
	if( limits != 0xFFFF )
	{
		// Amount Limit
		if(( limits & 0xF000) == 0x0000 )
		{
			// Card Amount Limited
			TRINP.TR_CARD_PRESET_MODE = 0;
			
			// Change Amount From Yuan To Cent
			memset( tmpbuf, 0x00, sizeof( tmpbuf ));
			sprintf( tmpbuf, "%08x", ( limits & 0x0FFF ));
			PresetValue = atoi( tmpbuf );
			PresetValue *= 100;
			
			
		// Save Max Amount Limit
			
//Limit Amount
	IntToCharCopy( PresetValue, TRINP.TR_MAX_PRESET_AMOUNT );
	
			// Ok
			retval = 0x9000;
		}
		else if(( limits & 0xF000) == 0x1000 )
		{
			// Card Amount Limited
			TRINP.TR_CARD_PRESET_MODE = 1;
			
			// Volume, Litre Limit
			// Get Price
			Price = CharToShortIntCopy( TRINP.TR_TRANS_PRICE );
			
			// Change Litre Volume To Amount
			memset( tmpbuf, 0x00, sizeof( tmpbuf ));
			sprintf( tmpbuf, "%08x", ( limits & 0x0FFF ));
			PresetValue = atoi( tmpbuf );
			PresetValue *= Price;
			
//			PresetValue = (( limits & 0x0FFF ) * Price );
			
			// Save Max Amount Limit
			IntToCharCopy( PresetValue, TRINP.TR_MAX_PRESET_AMOUNT );
		
			// Change Litre Volume To 2 Decimal Point
			memset( tmpbuf, 0x00, sizeof( tmpbuf ));
			sprintf( tmpbuf, "%08x", ( limits & 0x0FFF ));
			PresetValue = atoi( tmpbuf );
			PresetValue *= 100;
			
//			PresetValue = (( limits & 0x0FFF ) * 100 );
			
			// Save Max Volume Limit
			IntToCharCopy( PresetValue, TRINP.TR_MAX_PRESET_VOLUME );
		
			// Ok
			retval = 0x9000;
		}
		else if(( limits & 0xF000) == 0x2000 )
		{
			// Card Amount Limited
			TRINP.TR_CARD_PRESET_MODE = 1;
			
			// Volume, Kilogram Limit
			// Get Price
			Price = CharToShortIntCopy( TRINP.TR_TRANS_PRICE );
			
			// Change Kilogram Volume To Amount
			memset( tmpbuf, 0x00, sizeof( tmpbuf ));
			sprintf( tmpbuf, "%08x", ( limits & 0x0FFF ));
			PresetValue = atoi( tmpbuf );
			PresetValue *= Price;
			
//			PresetValue = (( limits & 0x0FFF ) * Price );
			
			// Save Max Amount Limit
			IntToCharCopy( PresetValue, TRINP.TR_MAX_PRESET_AMOUNT );
		
			// Change Litre Volume To 2 Decimal Point
			memset( tmpbuf, 0x00, sizeof( tmpbuf ));
			sprintf( tmpbuf, "%08x", ( limits & 0x0FFF ));
			PresetValue = atoi( tmpbuf );
			PresetValue *= 100;
			
//			PresetValue = (( limits & 0x0FFF ) * 100 );
			
			// Save Max Volume Limit
			IntToCharCopy( PresetValue, TRINP.TR_MAX_PRESET_VOLUME );
		
			// Ok
			retval = 0x9000;
		}
		else
		{
			// Error
			// Display: " Per Fuelling Limited Para Error"
			retval = 0;
		}
	}
	else
	{
		// Default Amount Limited
		TRINP.TR_CARD_PRESET_MODE = 0;
			
		// No Limit
		// Get The Max Preset Value
		memcpy( TRINP.TR_MAX_PRESET_AMOUNT, "\xFF\xFF\xFF\xFF", 4 );
		memcpy( TRINP.TR_MAX_PRESET_VOLUME, "\xFF\xFF\xFF\xFF", 4 );
		
		retval = 0x9000;
	}

	return retval;
}




/*
 ** TimesAmountTotalLimit
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS: None.
 *
 *  DESCRIPTION: Check Wether Fuelling Times And Amount Total Limit
 *
 *  RETURNS: 0: No Fuelling Times And Amount Total Limit
 *			 1: Either Fuelling Times Or Amount Total Limit Or Both Limit
 *
 */
unsigned char TimesAmountTotalLimit( void )
{
	unsigned char retval;
	unsigned int limit;

	// Default No Limit
	retval = 0;

	limit = CharToIntCopy( TRINP.TR_CARD_AMOUNT_DAY_LIMIT );

	if(( limit != 0xFFFFFFFF ) || ( TRINP.TR_CARD_TIMES_DAY_LIMIT != 0xFF ))
	{
		// Need Limit
		retval = 1;
	}
	else
	{
		// No Limit
		retval = 0;
	}

	return retval;
}



/*
 ** CalcFuelling
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Calculate The Fuelling Times And Amount Total Of The Day
 *
 *  RETURNS: None.
 *
 *	Note: The Transaction Date Was Saved In TR_TRANS_DATE, Before Call The Function
 *			The Fuelling Times Was Saved In TR_TIMES_DAY,
 *			while The Fuelling Amount Total Was Saved in TR_AMOUNT_TOTAL_DAY, After The Function
 *
 */
void CalcFuelling( void )
{
	unsigned char DateFlag;
	unsigned int Amount1, Amount2;
	
	// Check Trans Date
	DateFlag = CompareDate( TRINP.TR_TRANS_DATE, TRINP.TR_CARD_RECORD_DATE );

	if( DateFlag == 0 )
	{
		// Check The Same Day
		// Calc The Total Fuelling Amount And Fuelling Times
		if(( TRINP.TR_CARD_RECORD_TRANS_TYPE == 0x93) ||
			( TRINP.TR_CARD_RECORD_TRANS_TYPE == 0x05) ||
			( TRINP.TR_CARD_RECORD_TRANS_TYPE == 0x95))
		{
			TRINP.TR_TIMES_DAY++;
			
			// Get Amount
			Amount1 = CharToIntCopy( TRINP.TR_AMOUNT_TOTAL_DAY );
			Amount2 = CharToIntCopy( TRINP.TR_CARD_RECORD_AMOUNT);
		
			Amount1 += Amount2;

			// Fuelling Total Amount
			IntToCharCopy( Amount1, TRINP.TR_AMOUNT_TOTAL_DAY );
		}
	}
}



/*
 ** CheckAmountTotalLimit
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS: None.
 *
 *  DESCRIPTION: Check Amount Total Limit
 *
 *  RETURNS: 1: No Limited; 0: Limited
 *
 *	Note: Update The TR_MAX_PRESET_AMOUNT For Preset Process Check
 *
 */
unsigned char CheckAmountTotalLimit( void )
{
	unsigned char retval;

	unsigned short int Price;
	unsigned int PresetAmount, LimitAmount, FuelledAmount;

	// Default Limited
	retval = 0;

	// Get Fuelled Amount, LimitAmount
	FuelledAmount = CharToIntCopy( TRINP.TR_AMOUNT_TOTAL_DAY );
	LimitAmount = CharToIntCopy( TRINP.TR_CARD_AMOUNT_DAY_LIMIT );
	
	if( FuelledAmount < LimitAmount )
	{
		// Not Exceed The Amount Total Limit
		// Get The Max Fuelling Amount
		LimitAmount -= FuelledAmount;

		PresetAmount = CharToIntCopy( TRINP.TR_MAX_PRESET_AMOUNT );

		// Compare With The Max Preset Amount Get By Check_Card_Fuelling_Limit
		if( PresetAmount > LimitAmount )
		{
			// Limit Amount Less The Max Preset Amount, Change The Max Preset Amount
			IntToCharCopy( LimitAmount, TRINP.TR_MAX_PRESET_AMOUNT );
		
			// Check And Update Volume Limited For Card Per Fuelling Volume Limit
			if( TRINP.TR_CARD_PRESET_MODE == 1 )
			{
				// Get Price
				Price = CharToShortIntCopy( TRINP.TR_TRANS_PRICE );
			
				// Change Limit Amount To Volume
                LimitAmount *= 100; // Fix limit volume card bug, 2012-10-08
				LimitAmount /= Price;
			
				// Save Max Volume Limit
				IntToCharCopy( LimitAmount, TRINP.TR_MAX_PRESET_VOLUME );
			}
//
//#ifdef MAKE_CARD_MODE2
//			IntToCharCopy( LimitAmount, TRINP.TR_BALANCE_PRESET );
//#endif			
		}
		else
		{
			// Keep TR_MAX_PRESET_AMOUNT As Max Preset Amount
		}
	
		// No Limit By Amount Total
		retval = 1;
	}
	else
	{
		// Exceed The Amount Total limit
		retval = 0;
	}

	return retval;
}



/*
 ** SetPresetValue
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check The Max Preset Value For Pump Auth
 *
 *  RETURNS: 1: Preset Ok; 0: Preset Error, The Preset Value Is Less Than The Allowed Min Value
 *			 2: Preset Error, Volume: The Preset Value Is More Then The Allowed Max Value
 *			 3: Preset Error, Amount: The Preset Value Is More Then The Allowed Max Value
 *
 *	Note: Update The Preset Value In TR_TRANS_PRESET_MODE And TR_TRANS_PRESET_VALUE[4] For Pump Call Auth
 *
 */
unsigned char SetPresetValue( void )
{
	unsigned char retval;

	unsigned short int Price;
	unsigned int ReserveValue, MaxPresetValue, Balance, PresetValue;
	unsigned int maxfuellingvalue, maxfuellingamount;

	// Default Ok
	retval = 1;

#ifdef MAKE_CASH_CR
	// For Cash Card, Use Authed Or Accepted Amount As Max Preset Value
	if( CRType == CR_TYPE_CASH )
	{
		memcpy( TRINP.TR_MAX_PRESET_AMOUNT, TRINP.TR_CASH_TOTAL, sizeof( TRINP.TR_MAX_PRESET_AMOUNT ));
	}
#endif	// MAKE_CASH_CR

	// Get Max Preset Value
	MaxPresetValue = CharToIntCopy( TRINP.TR_MAX_PRESET_AMOUNT );

	// Get Balance
#ifdef MAKE_CARD_MODE2

	if( TRINP.TR_WORK_MODE == 0x01 )
	{
		// Online, Balance Should Be Max
		Balance = 0xFFFFFFFF - EMP_AMN_PER_TRANS;
	}
	else
	{
		// Offline, Need To Check Customer Card Or Employee Card
		if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
		{
			// Customer Card, Use Balance
			Balance = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );
		}
		else
		{
			// Use Max value
			Balance = 0xFFFFFFFF - EMP_AMN_PER_TRANS;
		}
	}
#else

	Balance = CharToIntCopy( TRINP.TR_CARD_APP_BALANCE );

#endif	// MAKE_CARD_MODE2


	// Get Price
	Price = CharToShortIntCopy( TRINP.TR_TRANS_PRICE );
	
	// Check Balance Should > 1.1L, Otherwise, Do Not Auth The Call
	ReserveValue = CARD_AUTH_V * Price;
	ReserveValue = Round_Mod( ReserveValue, 1000 );
	if( Balance < ReserveValue )
	{
		// Balance Too Small, Return;
		retval = 0;

		// Display Balance Is Too Small
		FillInfoMsg( M_BalanceTooSmall, 0, 0 );

		return retval;
	}
	else
	{
		// Go On....
	}


	// Adjust ReserveValue From mL To L 
	ReserveValue = CARD_RESERVE_V * Price;
	ReserveValue = Round_Mod( ReserveValue, 1000 );

	// Max Value
	if( Balance > ReserveValue )
	{
		Balance -= ReserveValue;
	}
	else
	{
		// Balance Too Small, Return;
		retval = 0;

		// Display Balance Is Too Small
		FillInfoMsg( M_BalanceTooSmall, 0, 0 );

		return retval;
	}

	// Check Balance And Max Allowed Preset Value
	memcpy( &maxfuellingvalue, Pump_CR_Buf.MaxFuellingVolume, sizeof( Pump_CR_Buf.MaxFuellingVolume ));
	if( maxfuellingvalue != 0 )
	{
		// First Define: The Max Preset Value Is Both Volume And Amount,
		// According To The Price >= 1.00 Or Not
		// Use Max Fuelling Number From Pump
//		if( Price < 100 )
//		{
//			// Price < 1.00, Max Fuelling  Value Is Volume
//			// Change L To Amount
//			maxfuellingamount = maxfuellingvalue * Price * 100;
//	
//			maxfuellingamount = Round_Mod( maxfuellingamount, 100 );
//		}
//		else
//		{
//			// Price >= 1.00, Max Fuelling Value Is Amount
//			maxfuellingamount = maxfuellingvalue * 100;
//		}
//
//		if( Balance > maxfuellingamount )
//		{
//			Balance = maxfuellingamount;
//		}
//		else
//		{
//			Balance = Balance;
//		}
		// First Define: End

		// Second Define: The Max Preset Value Is Volume,
		// But The Amount Must Be <= CARD_PRESET_MAX
		// Change L To Amount
		maxfuellingamount = maxfuellingvalue * Price * 100;
	
		maxfuellingamount = Round_Mod( maxfuellingamount, 100 );

		if( maxfuellingamount > CARD_PRESET_MAX )
		{
			maxfuellingamount = CARD_PRESET_MAX;
		}
		else
		{
			maxfuellingamount = maxfuellingamount;
		}

		if( Balance > maxfuellingamount )
		{
			Balance = maxfuellingamount;
		}
		else
		{
			Balance = Balance;
		}
		// Second Define End
	}
	else
	{
		// Use Default Max Value
		// And Get The Less Value
		if( Balance > CARD_PRESET_MAX )
		{
			Balance = CARD_PRESET_MAX;
		}
		else
		{
			Balance = Balance;
		}
	}
	
	
	// Check The TRINP Max Value, And
	// Get The Less Value, Which Can Be Set As Preset
	if( MaxPresetValue > Balance )
	{
		MaxPresetValue = Balance;

		// For Volume Limited Card, Update the Limited Volume
		if( TRINP.TR_CARD_PRESET_MODE == 1 )
		{
			// Change Limit Amount To Volume
            Balance *= 100; // Fix limit volume card bug, 2012-10-08
			Balance /= Price;            
			
			// Save Max Volume Limit
			IntToCharCopy( Balance, TRINP.TR_MAX_PRESET_VOLUME );
		}
	}
	else
	{
		MaxPresetValue = MaxPresetValue;
	}


	// Next, Compare MaxPresetValue With PresetValue, And Get The Less Value As The Final Preset
	
	
	if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & PRESET )
	{
		// Clear Bit
		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~PRESET );

		// Inform Keypad Process Received
		ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= PRESET;
					
		// Check Preset And Prepare Value For Response Pump Auth
		// Get Inputed Preset Value
		PresetValue = CharToIntCopy( KEYPAD_CR_Buf.PresetValue );
		
		if( KEYPAD_CR_Buf.PresetMode == 0x01 )
		{
			// Volume Preset, Preset Should Be Greater Than 1L
			if( PresetValue < 100 )
			{
				// Volume Preset Is Too Small, Quit
				// Display " Preset Too Small ", And Return Fail
				retval = 0;
				// Clear The Keypad - CR Prest Buffer
				KEYPAD_CR_Buf.PresetMode = 0;
				memset( KEYPAD_CR_Buf.PresetValue, 0x00, sizeof( KEYPAD_CR_Buf.PresetValue ));

				// Display Preset Is Too Small By Volume
				FillInfoMsg( M_PresetTooSmall, 0, 0 );

				return retval;
			}
			else
			{
				// Just Go On....
			}

			// Volume Preset, Change To Amount For Compare With The Max Preset Value
			PresetValue *= Price;
			// Change Preset Value To 2 Decimail Pointer
			PresetValue = Round_Mod( PresetValue, 100 );
			
			// Check Preset Value
			if( PresetValue > MaxPresetValue )
			{
				// Volume Preset Is Too More, Quit
				// Display " Preset Too Much ", And Return Fail
				retval = 2;
				// Clear The Keypad - CR Prest Buffer
				KEYPAD_CR_Buf.PresetMode = 0;
				memset( KEYPAD_CR_Buf.PresetValue, 0x00, sizeof( KEYPAD_CR_Buf.PresetValue ));

				// Display Preset Is Too Much By Volume
				FillInfoMsg( M_PerVolumeLimited, 0, 0 );

				return retval;
			}
			else
			{
				// Use Keypad Volume Preset Mode, Do Not Care About Card Preset Mode
				TRINP.TR_TRANS_PRESET_MODE = 1;

				// Set Prest Volume
				memcpy( TRINP.TR_TRANS_PRESET_VALUE, KEYPAD_CR_Buf.PresetValue, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
			
                                
                                //down added for over fueling process - 20110906
                                OverPresetMoney=PresetValue;
                                //OverPresetMoney *= 100;
                                OverPresetValue=CharToIntCopy( KEYPAD_CR_Buf.PresetValue );
                                //OverPresetMode=1;                          //0: money ;1:valume
                                
                                //up added for over fueling process - 20110906
                                
				retval = 1;
			}
		}
		else
		{
			// Amount Preset, Preset Should Be Greater Than Price
			if( PresetValue < Price )
			{
				// Amount Preset Is Too Small, Quit
				// Display " Preset Too Small ", And Return Fail
				retval = 0;
				// Clear The Keypad - CR Prest Buffer
				KEYPAD_CR_Buf.PresetMode = 0;
				memset( KEYPAD_CR_Buf.PresetValue, 0x00, sizeof( KEYPAD_CR_Buf.PresetValue ));

				// Display Preset Is Too Small By Volume
				FillInfoMsg( M_PresetTooSmall, 0, 0 );

				return retval;
			}
			else
			{
				// Just Go On....
			}

			// Amount Preset, Keep MaxPreset Value
			// Check Preset Value
			if( PresetValue <= MaxPresetValue )
			{
				// Keep MaxPresetValue
				MaxPresetValue = PresetValue;
                                
                                //down added for over fueling process - 20110906
                                OverPresetMoney=PresetValue;
                                OverPresetValue=((OverPresetMoney*100)/Price);
                                //OverPresetMode=0;                          //0: money ;1:valume                               
                                //up added for over fueling process - 20110906
			}
			else
			{
				// Maybe Return 0, Is Do Not Allow Fuelling
				// For KeyPad Preset Too Much
				// Or
				// Update MaxPresetValue
//				MaxPresetValue = MaxPresetValue;

				retval = 3;
				// Clear The Keypad - CR Prest Buffer
				KEYPAD_CR_Buf.PresetMode = 0;
				memset( KEYPAD_CR_Buf.PresetValue, 0x00, sizeof( KEYPAD_CR_Buf.PresetValue ));

				// Display Preset Is Too Much By Amount
				MaxPresetValue = CharToIntCopy( TRINP.TR_CARD_AMOUNT_DAY_LIMIT );

				if( MaxPresetValue == 0xFFFFFFFF )
				{
					FillInfoMsg( M_PerAmountLimited, 0, 0 );
				}
				else
				{
					FillInfoMsg( M_TotalAmountLimited, 0, 0 );
				}


				return retval;
			}
			
			// Use Keypad Amount Preset Mode, Do Not Care About Card Preset Mode
			TRINP.TR_TRANS_PRESET_MODE = 0;
			
			// Amount Preset, Use MaxPresetValue
			IntToCharCopy( MaxPresetValue, TRINP.TR_TRANS_PRESET_VALUE );
		
			retval = 1;
		}

		// Clear The Keypad - CR Prest Buffer
		KEYPAD_CR_Buf.PresetMode = 0;
		memset( KEYPAD_CR_Buf.PresetValue, 0x00, sizeof( KEYPAD_CR_Buf.PresetValue ));
	}
	else
	{
		// No Keypad Preset
		// Use Card Preset Mode
		TRINP.TR_TRANS_PRESET_MODE = TRINP.TR_CARD_PRESET_MODE;
                
                //down added for over fueling process - 20110906
                 OverPresetMoney=0;   //no keypad preset,clear it
                //up added for over fueling process - 20110906

		// Set Preset Value To Pump
		if( TRINP.TR_TRANS_PRESET_MODE == 1 )
		{
			// Card Volume Limited, Use TRINP.TR_MAX_PRESET_VOLUME
			memcpy( TRINP.TR_TRANS_PRESET_VALUE, TRINP.TR_MAX_PRESET_VOLUME, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
		}
		else
		{
			// Amount Preset, Use MaxPresetValue
			IntToCharCopy( MaxPresetValue, TRINP.TR_TRANS_PRESET_VALUE );
		}
	
		// Preset Ok
		retval = 1;
	}

	return retval;
}





/*
 ** Round10Mod
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	Value: The Dividend; Mod: Divisor, Should Be 10, 100, 1000, ....
 *
 *  DESCRIPTION: Value / MOD With Decimal Pointer Process
 *
 *  RETURNS: Quotient
 *
 *	Note: If Remainder > 0.5: Quotient + 1; If Remainder < 0.5: Quotient
 *			If Remainder == 0.5: Odd: Quotient +1; Even: Quotient
 *
 */
unsigned int Round_Mod( unsigned int Value, unsigned int Mod )
{
	unsigned int tmpvalue, mode, halfmode;

	halfmode = Mod / 2;
	tmpvalue = Value;
	mode = Mod;
	
	
	if(( tmpvalue % mode ) < halfmode )
	{
		// < 0.5
		tmpvalue /= mode;
	}
	else if(( tmpvalue % mode ) > halfmode )
	{
		// > 0.5 
		tmpvalue /= mode;
		tmpvalue += 1;
	}
	else
	{
		// = 0.5, Odd: + 1;
		tmpvalue /= mode;
		if( tmpvalue % 2 )
		{
			tmpvalue += 1;
		}
	}

	return tmpvalue;
}


/*
 ** SavePumpCallData
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\CRARM\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save The Pump Call Data To PowerDown Buffer
 *
 *  RETURNS: None.
 *
 *	Note: Called After Set IC Card Grey, Only The Pump Call Set Grey Message
 *		   Exclude The Fuelling Data.
 *		  The IC Card Data Was Saved In SaveICCardData()
 *
 */
void SavePumpCallData( unsigned char flag )
{
	unsigned short int cardttc;

//Douzhq 20140627 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	unsigned char vipGrade;
#endif

	// Save The Trans TTC
	memcpy( PowerDown.PdTrans.PD_TRANS_TTC, TRINP.TR_TRANS_TTC, sizeof( PowerDown.PdTrans.PD_TRANS_TTC ));
	
	// Save The Transaction Type
	PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

	// Save The Trans Date And Time
	memcpy( PowerDown.PdTrans.PD_TRANS_DATE, TRINP.TR_TRANS_DATE, sizeof( PowerDown.PdTrans.PD_TRANS_DATE ));
	memcpy( PowerDown.PdTrans.PD_TRANS_TIME, TRINP.TR_TRANS_TIME, sizeof( PowerDown.PdTrans.PD_TRANS_TIME ));

	// Card TTC ++ For Debit Unlock And Grey Record
	cardttc = CharToShortIntCopy( TRINP.TR_CARD_APP_TTC );
	cardttc++;
	ShortIntToCharCopy( cardttc, TRINP.TR_CARD_APP_TTC );

	// Save The IC Card TTC
	memcpy( PowerDown.PdTrans.PD_CARD_APP_TTC, TRINP.TR_CARD_APP_TTC, sizeof(  PowerDown.PdTrans.PD_CARD_APP_TTC ));
	
	// Save The IC Card MAC2
	memcpy( PowerDown.PdTrans.PD_CARD_MAC2, TRINP.TR_CARD_MAC2, sizeof( PowerDown.PdTrans.PD_CARD_MAC2 ));
	
	// Save The TAC Or GTAC, Read From Grey Message
	memcpy( PowerDown.PdTrans.PD_CARD_GTAC, TRINP.TR_CARD_GTAC, sizeof( PowerDown.PdTrans.PD_CARD_GTAC ));
	
	// Save The PSAM TTC
	memcpy( PowerDown.PdTrans.PD_PSAM_TTC, TRINP.TR_PSAM_TTC, sizeof( PowerDown.PdTrans.PD_PSAM_TTC ));

	// Debit Source
	PowerDown.PdTrans.PD_TRANS_DEBITSOURCE = TRINP.TR_TRANS_DEBITSOURCE;
	PowerDown.PdTrans.PD_TRANS_UNIT = TRINP.TR_TRANS_UNIT;

	// Save The IC Card Key Version
	PowerDown.PdTrans.PD_CARD_KEY_VERSION = TRINP.TR_CARD_KEY_VERSION;
	
	// Save The PSAM Key Index, Combine With The PD_CARD_KEY_VERSION,
	// To Generate The CARD_KEY In Journal
	PowerDown.PdTrans.PD_PSAM_KEY_INDEX = TRINP.TR_PSAM_KEY_INDEX;

	// Transaction Nozzle Number
	PowerDown.PdTrans.PD_NOZZLENUMBER = TRINP.TR_TRANS_NOZZLE_NUMBER;
	
	// Grade Code, Product Code	
	memcpy( PowerDown.PdTrans.PD_GRADE_CODE, TRINP.TR_TRANS_GRADE_CODE, sizeof( PowerDown.PdTrans.PD_GRADE_CODE ));
	
	// Transaction Price
	memcpy( PowerDown.PdTrans.PD_TRANS_PRICE, TRINP.TR_TRANS_PRICE, sizeof( PowerDown.PdTrans.PD_TRANS_PRICE ));

#ifdef MAKE_CARD_MODE2
	// Work Mode
	PowerDown.PdTrans.PD_WORK_MODE = TRINP.TR_WORK_MODE;
	
	// Auth Mode
	PowerDown.PdTrans.PD_AUTH_MODE = TRINP.TR_AUTH_MODE;
	
	// PayX
	PowerDown.PdTrans.PD_PAY_X = TRINP.TR_PAY_X;
	
	// Card Holder Flag	( 71st Byte SFI 1C )
	PowerDown.PdTrans.PD_CARD_HOLDER_NEW_FLAG = TRINP.TR_CARD_HOLDER_NEW_FLAG;
	
	// Card Holder Type ( 74th And 75th Bytes SFI 1C )
	memcpy( PowerDown.PdTrans.PD_CARD_HOLDER_TYPE, TRINP.TR_CARD_HOLDER_TYPE, sizeof( TRINP.TR_CARD_HOLDER_TYPE ));

#endif	// MAKE_CARD_MODE2

//Douzhq 20140627 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (PowerDown.VipFuelingMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x00))
	{
		vipGrade = PowerDown.PdTrans.PD_CASH_TOTAL[2];
		
		if (vipGrade > 0 && vipGrade <= 0x07)
		{
			memcpy(PowerDown.PdTrans.PD_CASH_TOTAL,TRINP.TR_CASH_TOTAL,sizeof(PowerDown.PdTrans.PD_CASH_TOTAL));
		}
	}
	//Douzhq 20140813 Add , Save Oil
	else if (PowerDown.SaveOilAppMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x04))
	{
		memcpy(PowerDown.PdTrans.PD_CASH_TOTAL,TRINP.TR_CASH_TOTAL,sizeof(PowerDown.PdTrans.PD_CASH_TOTAL));
	}
#endif

}


/*
 ** GetSFuncData
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Fill The Data To Inform Pump Auth Error For Some Reasons
 *
 *  RETURNS: None.
 *
 */
void FillAuthError( void )
{
	// Preset Vale = 0: Means Can Not Issue The AUth?????????
	
	
	// Inform Pump Auth Data Ready
	// Do Not Inform Pump For Error Auth,
	// Wait Until The Nozzle Off
//	ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_CALL;

	// Clear The Preset Mode
	Pump_CR_Buf.PresetMode = 0;

	// Clear The Prest Value
	memset( Pump_CR_Buf.PresetValue, 0x00, sizeof( Pump_CR_Buf.PresetValue ));

}


/*
 ** GetPumpCallInfo
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION: Get Pump Call Information, Such As Price, Amount, Volume, Nozzle etc.....
 *
 *  RETURNS:
 *
 */
void GetPumpCallInfo( void )
{
	unsigned char nozzle;
	unsigned int tmp4;
	unsigned short int tmp2;

//Douzhq 20140627 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	unsigned short vipPrice;
	unsigned char  vipGrade;
	unsigned char  vipPriceChar[2];
#endif

#ifdef MAKE_CARD_MODE2
	if( Pump_CR_Buf.NozzleNumber == 0x00 )
	{
		// For Preset In Card Ready, Default Nozzle Number 1
		nozzle = 0x00;
	}
	else
	{
		// For Future, MPD
		nozzle = Pump_CR_Buf.NozzleNumber - 1;
	}
#else
	// Phisical Nozzle Start From 0
	nozzle = Pump_CR_Buf.NozzleNumber - 1;
#endif	// MAKE_CARD_MODE2

    // Record 'Pump Call' Physical Nozzle Number To Compare With 'Pump Busy' Physical Nozzle Number While Fueling
    PowerDown.PDPhysicalNozzleNumber = Pump_CR_Buf.NozzleNumber;

	// Get Logical NozzleNumber
	TRINP.TR_TRANS_NOZZLE_NUMBER = GetLogicNzn( nozzle );
	
	// Get Price
//	tmp2 = CharToShortIntCopy( Pump_CR_Buf.Price );
//	ShortIntToCharCopy( tmp2, TRINP.TR_TRANS_PRICE );
//	memcpy( TRINP.TR_TRANS_PRICE, &tmp2, sizeof( TRINP.TR_TRANS_PRICE ));
	
	tmp2 = GetPrice( nozzle, 0 );
	
		
	ShortIntToCharCopy( tmp2, TRINP.TR_TRANS_PRICE );

	// Clear Amount And Volume
	memset( TRINP.TR_TRANS_AMOUNT, 0x00, sizeof( TRINP.TR_TRANS_AMOUNT ));
	memset( TRINP.TR_TRANS_VOLUME, 0x00, sizeof( TRINP.TR_TRANS_VOLUME ));
	
//Douzhq 20140627 Add , Vip Fueling
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (PowerDown.VipFuelingMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x00))
	{
		memset(TRINP.TR_CASH_TOTAL,0x00,sizeof(TRINP.TR_CASH_TOTAL));
		vipGrade = GetVipGradePrice(nozzle,&vipPrice);
		
		if (vipGrade > 0 && vipGrade <= 0x07)
		{
			ShortIntToCharCopy(vipPrice,vipPriceChar);
			memcpy(&TRINP.TR_CASH_TOTAL[0],TRINP.TR_TRANS_PRICE,sizeof(TRINP.TR_TRANS_PRICE));
			memcpy(&TRINP.TR_CASH_TOTAL[2],&vipGrade,sizeof(vipGrade));
		}
	}
	//tmp2 = vipPrice;
	//ShortIntToCharCopy( tmp2, TRINP.TR_TRANS_PRICE );
	//Douzhq 20140813 Add , Save Oil
	else if (PowerDown.SaveOilAppMode == 0x01 && (TRINP.TR_TRANS_DEBITSOURCE == 0x04))
	{
		memset(TRINP.TR_CASH_TOTAL,0x00,sizeof(TRINP.TR_CASH_TOTAL));
		memcpy(&TRINP.TR_CASH_TOTAL[0],TRINP.TR_TRANS_PRICE,sizeof(TRINP.TR_TRANS_PRICE));
	}
#endif

#ifdef MAKE_CARD_MODE2
	memcpy( POS_CR_Buf.TransAmount, TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
	memcpy( POS_CR_Buf.TransVolume, TRINP.TR_TRANS_VOLUME, sizeof( TRINP.TR_TRANS_VOLUME ));
#endif

	// Get Trans Product Grade Code
	tmp2 = GetO_Type( nozzle );
	ShortIntToCharCopy( tmp2, TRINP.TR_TRANS_GRADE_CODE );

	// Get Province
	TRINP.TR_PROVINCE = GetProv();
	// Get City
	TRINP.TR_CITY = GetCity();
	// Get Superior Unit
	tmp4 = GetSuperior();
//	IntToCharCopy( tmp4, TRINP.TR_SUPERIOR_UNIT );
//	Hex2Asc( TRINP.TR_SUPERIOR_UNIT, ( unsigned char * )&tmp4, 4 );
	memcpy( TRINP.TR_SUPERIOR_UNIT, ( unsigned char * )&tmp4, 4 );

	// Get Station ID
	tmp4 = GetS_ID();
//	IntToCharCopy( tmp4, TRINP.TR_STATION_ID );
//	Hex2Asc( TRINP.TR_STATION_ID, ( unsigned char * )&tmp4, 4 );
	memcpy( TRINP.TR_STATION_ID, ( unsigned char * )&tmp4, 4 );

#ifdef MAKE_CARD_MODE2
	// Get Pump Totals, For Auth Cancel Trans
	memcpy( TRINP.TR_TRANS_VOLUME_TOTAL, DISP_CR_Buf.DispTotal[nozzle].VolumeTotal, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));
	memcpy( PowerDown.PdTrans.PD_TRANS_VOLUME_TOTAL, TRINP.TR_TRANS_VOLUME_TOTAL, sizeof( TRINP.TR_TRANS_VOLUME_TOTAL ));
#endif	// MAKE_CARD_MODE2

}


/*
 ** WritePumpCallLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Write The Pump Call Message To Log
 *
 *  RETURNS: None.
 *
 *	Note: Call After GetPumpCallInfo
 *
 */
void WriteCRPumpCallLog( void )
{
	unsigned char len;

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Fill "Nozzle On"
	memcpy( &CRLogTxt[len], "申请授权", 8 );
	len = strlen(( char * )CRLogTxt );

	// Fill "Phisical Nozzle Number"
	sprintf(( char * )&CRLogTxt[len], "%02u ", Pump_CR_Buf.NozzleNumber );
	len = strlen(( char * )CRLogTxt );

	// Fill "Price"
//	sprintf(( char * )&CRLogTxt[len], "%u ", Pump_CR_Buf.Price );
	Hex2Asc(&CRLogTxt[len], TRINP.TR_TRANS_PRICE, sizeof( TRINP.TR_TRANS_PRICE ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill " Oil Type"
	Hex2Asc( &CRLogTxt[len], TRINP.TR_TRANS_GRADE_CODE, sizeof( TRINP.TR_TRANS_GRADE_CODE ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill " Province"
	Hex2Asc( &CRLogTxt[len], &TRINP.TR_PROVINCE, sizeof( TRINP.TR_PROVINCE ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill " City"
	Hex2Asc( &CRLogTxt[len], &TRINP.TR_CITY, sizeof( TRINP.TR_CITY ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill " Superior Unit"
	Hex2Asc( &CRLogTxt[len], TRINP.TR_SUPERIOR_UNIT, sizeof( TRINP.TR_SUPERIOR_UNIT ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill " Station ID"
	Hex2Asc( &CRLogTxt[len], TRINP.TR_STATION_ID, sizeof( TRINP.TR_STATION_ID ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );
}


/*
 ** WriteCRAuthPumpLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Write The Auth Information Log
 *
 *  RETURNS: None.
 *
 */
void WriteCRAuthPumpLog( void )
{
	unsigned char len;

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Fill "Auth Complete"
	memcpy( &CRLogTxt[len], "授权成功", 8 );
	len = strlen(( char * )CRLogTxt );

	// Fill " Auth Mode"
	Hex2Asc( &CRLogTxt[len], &TRINP.TR_TRANS_PRESET_MODE, sizeof( TRINP.TR_TRANS_PRESET_MODE ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill " Auth Value"
	Hex2Asc( &CRLogTxt[len], TRINP.TR_TRANS_PRESET_VALUE, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill " Card ASN"
	Hex2Asc( &CRLogTxt[len], TRINP.TR_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Fill " Card Balance"
	Hex2Asc( &CRLogTxt[len], TRINP.TR_CARD_APP_BALANCE, sizeof( TRINP.TR_CARD_APP_BALANCE ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );
}

/*
 ** WriteCRAuthPumpMAC2Log
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Write The MAC2 Information Log
 *
 *  RETURNS: None.
 *
 */
void WriteCRAuthPumpMAC2Log( void )
{
	unsigned char len;

	// Clear Buffers
	memset( CRLogTxt, 0x00, sizeof( CRLogTxt ));
	len = strlen(( char * )CRLogTxt );

	// Fill "Auth Complete"
	memcpy( &CRLogTxt[len], "保存MAC2: ", 10 );
	len = strlen(( char * )CRLogTxt );

	// Fill " MAC2"
	Hex2Asc( &CRLogTxt[len], TRINP.TR_CARD_MAC2, sizeof( TRINP.TR_CARD_MAC2 ));
	len = strlen(( char * )CRLogTxt );
	CRLogTxt[len] = ' ';
	len += 1;

	// Save To Log
	WriteLog( PROCESS_CR, CRLogTxt );
}

#ifdef MAKE_PSAM_VERIFY
/*
 ** GetPumpVerify
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Get Pump Verify Encrypted Data
 *
 *  RETURNS: 1: Running; 0x9000: Encrypted Data Get And Saved In Pump_CR_Buf.DESRandom
 *			 0: Get Error, eg: Nozzle Off Or Pump Offline While Waiting For The Pump Result
 *
 */
unsigned short int GetPumpVerify( void )
{
	unsigned short int retval;

	// Default Running
	retval = 1;

	// First Set Get Pump Verify Flag In ProcExchange[PROCESS_CR][PROCESS_PUMP];
	if( !( CR_Task_Flag & ICCARD_PUMP_VERIFY ))
	{
		// Set Bit, Local Process Flag
		CR_Task_Flag |= ICCARD_PUMP_VERIFY;

		// Set Bit, To Inform Pump Cala Encrypted Verify Data
		ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_VERIFY;
		// Clear The Bits For Abnormal Set
		ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_VERIFY );
	}
	else
	{				
		// Second Check The Input, By Check ProcExchange[PROCESS_PUMP][PROCESS_CR]
		if( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_VERIFY )
		{
			// Result OK
			// Clear The Bit
			ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_VERIFY );
			
			// Clear Local Process Flag
			CR_Task_Flag &= ( ~ICCARD_PUMP_VERIFY );

			// Calc Ok, Check It In PSAM Next
			retval = 0x9000;
		}
		// Check The Pump Process Error Or Not
		else if( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_VERIFY_ERROR )
		{
			// Result Error
			// Clear The Bit
			ProcExchange[PROCESS_PUMP][PROCESS_CR] &= ( ~PUMP_VERIFY_ERROR );
			
			// Clear Local Process Flag
			CR_Task_Flag &= ( ~ICCARD_PUMP_VERIFY );

			// Calc Ok, Check It In PSAM Next
			retval = 0x00;
		}
		else
		{
			// Wait Input, Go On
		}
	}


	return retval;
}
#endif



/*
 ** CardAuthPump
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Send The Auth Data To Pump And Save The Data For Power Down
 *
 *  RETURNS: None.
 *
 *	ATTN: Called After Set Grey Ok
 *
 */
void CardAuthPump( void )
{
	unsigned int tmp;

#ifdef MAKE_CARD_MODE2
	// For Sinopec Mode2 Online Mode, Do Not Get TTC Here, TTC Get Before Auth POS
	// For Offline Mode, Get TTC Here
	if( TRINP.TR_WORK_MODE != 0x01 )
	{
		// Get The TTC
		GetTransTTC();
	}
	else
	{
		// Online, Do Nothing....
	}
#else

	// Get The TTC
	GetTransTTC();

#endif	// MAKE_CARD_MODE2
						
#ifdef MAKE_CARD_MODE2
	// For Sinopec Mode2
	if( TRINP.TR_CARD_ASN[2] == CUSTOMER_CARD )
	{
		if( TRINP.TR_WORK_MODE == 0x01 )
		{
			// Work Online
			if(( TRINP.TR_PAY_X == 0x03 ) || ( TRINP.TR_PAY_X == 0x13 ))
			{
				// Host Account Payment, Do Not Mark Grey
				// Go On....
			}
			else
			{
				// Not Host Account Payment, Mark Grey
				// Update The Power Down Protection Message
				PowerDown.PdTrans.PD_TRANS_FLAG = MAC2_RECEIVED;

				// Set Grey End, Current Trans Type Is Grey
				TRINP.TR_TRANS_TYPE &= 0xF0;
				TRINP.TR_TRANS_TYPE |= TRANSGREY;
			}
		}
		else
		{
			// Work Offline, Mark Grey
			// Update The Power Down Protection Message
			PowerDown.PdTrans.PD_TRANS_FLAG = MAC2_RECEIVED;

			// Set Grey End, Current Trans Type Is Grey
			TRINP.TR_TRANS_TYPE &= 0xF0;
			TRINP.TR_TRANS_TYPE |= TRANSGREY;
		}
	}
	else
	{
		if( TRINP.TR_CARD_ASN[2] == TEST_CARD )
		{
			// Update The Power Down Protection Message
			PowerDown.PdTrans.PD_TRANS_FLAG = MAC2_RECEIVED;

			// Set Grey End, Current Trans Type Is Grey
			TRINP.TR_TRANS_TYPE &= 0xF0;
			TRINP.TR_TRANS_TYPE |= TRANSGREY;
		}
		else if( TRINP.TR_CARD_ASN[2] == MAINTAINANCE_CARD )
		{
			// Update The Power Down Protection Message
			PowerDown.PdTrans.PD_TRANS_FLAG = MAC2_RECEIVED;

			// Set Grey End, Current Trans Type Is Grey
			TRINP.TR_TRANS_TYPE &= 0xF0;
			TRINP.TR_TRANS_TYPE |= TRANSGREY;
		}
		else
		{
			// Employee Card
			// Do Nothing, Go On....
		}
	}

#else
	// Update The Power Down Protection Message
	PowerDown.PdTrans.PD_TRANS_FLAG = MAC2_RECEIVED;

	// Set Grey End, Current Trans Type Is Grey
	TRINP.TR_TRANS_TYPE &= 0xF0;
	TRINP.TR_TRANS_TYPE |= TRANSGREY;
#endif	// MAKE_CARD_MODE2
						
	// Save PumpCall Information
	SavePumpCallData( 0 );
						
	ProcExchange[PROCESS_CR][PROCESS_PUMP] |= PUMP_AUTH;


	//bbbbbbbbbb
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	if (LOCK_NOZZLE.LockFlag == 1 && TRINP.TR_CARD_ASN[2] == EMPLOYEE_CARD)	
	{
		FillInfoMsg(M_FORMAT_PWD,0,0);
		
		//ProcStatus[PROCESS_CR] |= CARD_BUSY;
	}
	else
	{
		ProcStatus[PROCESS_CR] |= CARD_BUSY;
	}
	// Mark IC Card Busy
#else
	ProcStatus[PROCESS_CR] |= CARD_BUSY;
#endif
	// Inform Pump Preset Value
	Pump_CR_Buf.PresetMode = TRINP.TR_TRANS_PRESET_MODE;

	// Set Prest Value
	tmp = CharToIntCopy( TRINP.TR_TRANS_PRESET_VALUE );

	// Change To LSB
	memcpy( Pump_CR_Buf.PresetValue, &tmp, 4 );

	// Clear Pump_CR_Buf
	memset( Pump_CR_Buf.Volume, 0x00, sizeof( Pump_CR_Buf.Volume ));
	memset( Pump_CR_Buf.Amount, 0x00, sizeof( Pump_CR_Buf.Amount ));
	memset( Pump_CR_Buf.VTotal, 0x00, sizeof( Pump_CR_Buf.VTotal ));
						
	// IC Card Set Grey Lock Ok, Change To Fuelling Steps Next,
	WriteCRAuthPumpLog();

	// IC Card Set Grey Lock Ok, Change To Fuelling Steps Next,
	//WriteCRAuthPumpMAC2Log();

	// Inform Voice Process Nozzle On
	ProcExchange[PROCESS_CR][PROCESS_VOICE] |= VOICE_NOZZLE_ON;

#ifdef MAKE_CASH_CR
	// For Cash Card, Clear Cash Card Ok Flag
	if( CRType == CR_TYPE_CASH )
	{
		// Clear Card Card Ok
		ProcStatus[PROCESS_CR] &= ( ~CARD_CASH_OK );
	}
#endif	// MAKE_CASH_CR
								
#ifdef MAKE_CARD_MODE2
	// Display " Fuelling, Do Not Spring Out Card"
	FillInfoMsg( M_Fuelling, 0, 1 );

	// Do Not Clear The Info
	StopDefaultInfo();

#endif	// MAKE_CARD_MODE2

}

/*
 ** DispLocationLimit
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	flag: 1: Province/City Limited; 2: City/Supiror Limited; 3: Supiror/Station Limited
 *
 *  DESCRIPTION: Display Location Limit Information
 *
 *  RETURNS: None.
 *
 *	ATTN: For Mode2 Sinopec, Limited Information Is Different From Sinopec Standard
 *
 */
void DispLocationLimit( unsigned char flag )
{
	unsigned char location;

	location = flag;

#ifdef MAKE_CARD_MODE2

	if(( TRINP.TR_CARD_HOLDER_NEW_FLAG & CARD_MODE2_NON ) == CARD_MODE2_NON )
	{
		// Not Mode2 Card, Go On....
	}
	else if(( TRINP.TR_CARD_HOLDER_NEW_FLAG & CARD_MODE2_NON ) == 0x00 )
	{
		// Mode2 Card, But RFU, Do Not Care, Go On....
	}
	else
	{
		// Mode2 Card, Adjust Limit Information
		if( location >= 1 )
		{
			location -= 1;
		}
		else
		{
			// Do Not Care, Go On....
		}
	}

#endif
	
	if( location == 0 )
	{
		FillInfoMsg( M_ProvinceLimited, 0, 0 );
	}
	else if( location == 1 )
	{
		FillInfoMsg( M_CityLimited, 0, 0 );
	}
	else if( location == 2 )
	{
		FillInfoMsg( M_SuperiorUnitLimited, 0, 0 );
	}
	else
	{
		FillInfoMsg( M_OilStationLimited, 0, 0 );
	}
}

#ifdef MAKE_CARD_MODE2

/*
 ** AuthPOSOnline_EMP
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Auth POS For The Pump Auth Data
 *
 *  RETURNS: 0x9000: Host Auth Ok; 
 *			 0x01: Running
 *			 0x02: Send Auth TimeOut
 *
 */
unsigned short int AuthPOSOnline_EMP( void )
{
	unsigned short int retval;

	// Default Running
	retval = 1;

	// Check First Running, Or Not
	if( !( CR_Task_Flag & ICCARD_AUTH ))
	{
		// Display Authing....
		FillInfoMsg( M_WaitAuth, 0, 0 );

		// Set Bit, Local Process Flag
		CR_Task_Flag |= ICCARD_AUTH;

		// Save MAC
		memcpy( POS_CR_Buf.Auth_MAC, TRINP.TR_TRANS_MAC, 4 );

//		// Get Trans Date And Time For Set Grey
//		GetTransTime();

		// Update The Power Down Protection Message
		PowerDown.PdTrans.PD_TRANS_FLAG = SEND_AUTH;

		// Save PumpCall Information
		SavePumpCallData( 0 );

		// Clear Flag
		POS_CR_Buf.CheckFlag = 0;

		if( TRINP.TR_PAY_X == 0x18 )
		{
			// Inform POS Process, None Card SS
			ProcExchange[PROCESS_CR][PROCESS_POS] |= AUTH_NCSS;
		}
		else
		{
			// Inform POS Process, Employee
			ProcExchange[PROCESS_CR][PROCESS_POS] |= AUTH_EMP;
		}
	}
	else
	{
		// Check POS Return
		if(( ProcExchange[PROCESS_POS][PROCESS_CR] & AUTH_EMP )
			|| ( ProcExchange[PROCESS_POS][PROCESS_CR] & AUTH_NCSS ))
		{
			// POS Auth Ended
			// Clear The Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~( AUTH_EMP | AUTH_NCSS ));
					
			// Clear Local Process Flag
			CR_Task_Flag &= ( ~ICCARD_AUTH );

			// POS Response, Check The Fit Or Not
			if(( POS_CR_Buf.CheckFlag == 0 ) || ( POS_CR_Buf.CheckFlag == 1 ))
			{
				// Update The Power Down Protection Message
				PowerDown.PdTrans.PD_TRANS_FLAG = AUTH_RESP;

				// Save The Response Data
				AuthPOSOnlineResponse();

				if( CheckOnlinePreset() == 1 )
				{
					// Display Authing OK
					FillInfoMsg( M_AuthEnd, 0, 0 );

					// Fitted, Save Response,
					retval = 0x9000;
				}
				else
				{
					// Display Authing Small
					FillInfoMsg( M_PresetTooSmall, 0, 0 );

					// Fitted, Save Response,
					retval = 2;
				}
			}
			else
			{
				// Display " TimeOut "....
				FillInfoMsg( M_AuthTimeOut, 0, 0 );

//				//Set Offline
//				TRINP.TR_WORK_MODE = 0x00;

				// Error,
				retval = 0x02;
			}
		}
		else
		{
			// Running, Go on....
		}
	}

	return retval;
}


/*
 ** AuthPOSOnline_CST
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Auth POS For The Pump Auth Data
 *
 *  RETURNS: 0x9000: Host Auth Ok; 
 *			 0x01: Running
 *			 0x02: Send Auth TimeOut
 *
 */
unsigned short int AuthPOSOnline_CST( void )
{
	unsigned short int retval;

	// Default Running
	retval = 1;

	// Check First Running, Or Not
	if( !( CR_Task_Flag & ICCARD_AUTH ))
	{
		// Display Authing....
		FillInfoMsg( M_WaitAuth, 0, 0 );

		// Set Bit, Local Process Flag
		CR_Task_Flag |= ICCARD_AUTH;

		// Save MAC
		memcpy( POS_CR_Buf.Auth_MAC, TRINP.TR_TRANS_MAC, 4 );

//		// Get Trans Date And Time For Set Grey
//		GetTransTime();

		// Clear Flag
		POS_CR_Buf.CheckFlag = 0;

		// Update The Power Down Protection Message
		PowerDown.PdTrans.PD_TRANS_FLAG = SEND_AUTH;

		// Inform POS Process
		ProcExchange[PROCESS_CR][PROCESS_POS] |= AUTH_CST;
	}
	else
	{
		// Check POS Return
		if( ProcExchange[PROCESS_POS][PROCESS_CR] & AUTH_CST )
		{
			// POS Auth Ended
			// Clear The Bit
			ProcExchange[PROCESS_POS][PROCESS_CR] &= ( ~AUTH_CST );
					
			// Clear Local Process Flag
			CR_Task_Flag &= ( ~ICCARD_AUTH );

			// POS Response, Check The Fit Or Not
			if(( POS_CR_Buf.CheckFlag == 0 ) || ( POS_CR_Buf.CheckFlag == 1 ))
			{
				// Update The Power Down Protection Message
				PowerDown.PdTrans.PD_TRANS_FLAG = AUTH_RESP;

				// Save The Response Data
				AuthPOSOnlineResponse();

				if( CheckOnlinePreset() == 1 )
				{
					// Display Authing OK
					FillInfoMsg( M_AuthEnd, 0, 0 );

					// Save PumpCall Information
					SavePumpCallData( 0 );

					// Fitted, Save Response,
					retval = 0x9000;
				}
				else
				{
					// Display Authing Small
					FillInfoMsg( M_PresetTooSmall, 0, 0 );

					// Save PumpCall Information
					SavePumpCallData( 0 );

					// Fitted, Save Response,
					retval = 2;
				}
			}
			else
			{
				// Display " TimeOut "....
				FillInfoMsg( M_AuthTimeOut, 0, 0 );

//				//Set Offline
//				TRINP.TR_WORK_MODE = 0x00;

				// Error,
				retval = 0x02;
			}
		}
		else
		{
			// Running, Go on....
		}
	}

	return retval;
}




/*
 ** AuthPOSOnlineResponse
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: POS Reponse The Auth Request, Check And Save
 *
 *  RETURNS: None.
 *
 */
void AuthPOSOnlineResponse( void )
{
	unsigned int val;

	// Save PayX
	TRINP.TR_PAY_X = POS_CR_Buf.Auth_Pay_X;

	// Save The Auth Amont
	TRINP.TR_TRANS_PRESET_MODE = 0;
	memset( TRINP.TR_TRANS_PRESET_VALUE, 0x00, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
//	memcpy( TRINP.TR_TRANS_PRESET_VALUE, POS_CR_Buf.TransAmount, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));

	val = CharToIntCopy( POS_CR_Buf.TransAmount );
	if( val > CARD_PRESET_MAX )
	{
		// Max Preset
		val = CARD_PRESET_MAX;
	}
	IntToCharCopy( val, TRINP.TR_TRANS_PRESET_VALUE );


	// Save As Trans Amount For Auth Cancel
//	memcpy( TRINP.TR_TRANS_AMOUNT, TRINP.TR_TRANS_PRESET_VALUE, sizeof( TRINP.TR_TRANS_AMOUNT ));
//	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT, TRINP.TR_TRANS_AMOUNT, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT ));

	// Clear The Trans Amount
	memset( TRINP.TR_TRANS_AMOUNT, 0x00, sizeof( TRINP.TR_TRANS_AMOUNT ));
	memcpy( PowerDown.PdTrans.PD_TRANS_AMOUNT, TRINP.TR_TRANS_AMOUNT, sizeof( PowerDown.PdTrans.PD_TRANS_AMOUNT ));

	// Save The FeedBack
	TRINP.TR_AUTH_FEEDBACK = POS_CR_Buf.Auth_Resp;

	// Save The Add Ins
	memset( TRINP.TR_ADD_IN, 0x00, sizeof( TRINP.TR_ADD_IN ));
	TRINP.TR_ADD_IN_LEN = POS_CR_Buf.Auth_Add_On[0];
	memcpy( TRINP.TR_ADD_IN, &POS_CR_Buf.Auth_Add_On[1], POS_CR_Buf.Auth_Add_On[0] );
}


/*
 ** FillAuthReqMAC_EMP
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Fill The Data For Emp Card Request Both Calc MAC And POS Buf
 *
 *  RETURNS: None.
 *
 */
void FillAuthReqMAC_EMP( void )
{
	unsigned int value;

	// Clear MAC Buffer
	memset( MACBuffer, 0x00, sizeof( MACBuffer ));
	MACDataLen = 0;

	// Save The Nozzle Number
	POS_CR_Buf.TransNozzleNumber = TRINP.TR_TRANS_NOZZLE_NUMBER;
//	MACBuffer[MACDataLen] = TRINP.TR_TRANS_NOZZLE_NUMBER;
//	MACDataLen += 1;

	// Save The POS TTC
	memcpy( POS_CR_Buf.Auth_TTC, TRINP.TR_TRANS_TTC, sizeof( TRINP.TR_TRANS_TTC ));
//	memcpy( &MACBuffer[MACDataLen], TRINP.TR_TRANS_TTC, sizeof( TRINP.TR_TRANS_TTC ));
//	MACDataLen += sizeof( TRINP.TR_TRANS_TTC );

	// Save The Date
	memcpy( POS_CR_Buf.Auth_Date, TRINP.TR_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));
//	memcpy( &MACBuffer[MACDataLen], TRINP.TR_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));
//	MACDataLen += sizeof( TRINP.TR_TRANS_DATE );

	// Save The Time
	memcpy( POS_CR_Buf.Auth_Time, TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));
//	memcpy( &MACBuffer[MACDataLen], TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));
//	MACDataLen += sizeof( TRINP.TR_TRANS_TIME );

	// Save The PayX
	POS_CR_Buf.Auth_Pay_X = TRINP.TR_PAY_X;
//	MACBuffer[MACDataLen] = TRINP.TR_PAY_X;
//	MACDataLen += 1;

	// Save The Price
	memcpy( POS_CR_Buf.TransPrice, TRINP.TR_TRANS_PRICE, sizeof( TRINP.TR_TRANS_PRICE ));
 //	memcpy( &MACBuffer[MACDataLen], TRINP.TR_TRANS_PRICE, sizeof( TRINP.TR_TRANS_PRICE ));
 //	MACDataLen += sizeof( TRINP.TR_TRANS_PRICE );

//	// Get Preset Value
//	if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & PRESET )
//	{
//		// Clear The Bit
//		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~PRESET );
//		// Set The Bit To Inform Keypad
//		ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= PRESET;
//
//		// Use Keypad Preset
//		TRINP.TR_TRANS_PRESET_MODE = KEYPAD_CR_Buf.PresetMode;
//
//		// Set Prest Value
//		memcpy( TRINP.TR_TRANS_PRESET_VALUE, KEYPAD_CR_Buf.PresetValue, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
//	}
//	else
//	{
//		TRINP.TR_TRANS_PRESET_MODE = 0;
//
//		// Set Prest Value
//		memset( TRINP.TR_TRANS_PRESET_VALUE, 0x00, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
//	}


	// Save The Preset Value
	if( TRINP.TR_TRANS_PRESET_MODE == 0x00 )
	{
		value = CharToIntCopy( TRINP.TR_TRANS_PRESET_VALUE );
		if( value == CARD_PRESET_MAX )
		{
			// No Preset, Full Fuelling
			memset( TRINP.TR_TRANS_PRESET_VALUE, 0x00, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
		}
		else
		{
			// Reserve The Preset Value
		}

		// Preset Amount
		memcpy( POS_CR_Buf.TransAmount, TRINP.TR_TRANS_PRESET_VALUE, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
		memcpy( &MACBuffer[MACDataLen], &TRINP.TR_TRANS_PRESET_VALUE[1], 3 );
		MACDataLen += 3;

		// Skip Preset Volume
		memset( POS_CR_Buf.TransVolume, 0x00, sizeof(POS_CR_Buf.TransVolume ));
		MACDataLen += 3;
	}
	else
	{
		// Skip Preset Amount
		memset( POS_CR_Buf.TransAmount, 0x00, sizeof(POS_CR_Buf.TransAmount ));
		MACDataLen += 3;
		
		// Preset Volume
		memcpy( POS_CR_Buf.TransVolume, TRINP.TR_TRANS_PRESET_VALUE, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
		memcpy( &MACBuffer[MACDataLen], &TRINP.TR_TRANS_PRESET_VALUE[1], 3 );
		MACDataLen += 3;
	}

	// Save The ASN
	memcpy( POS_CR_Buf.ICCARD_ASN, TRINP.TR_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));
	memcpy( &MACBuffer[MACDataLen], TRINP.TR_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));
	MACDataLen += sizeof( TRINP.TR_CARD_ASN );
}

/*
 ** FillAuthReqMAC_CST
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Fill The Data For Customer Card Request Both Calc MAC And POS Buf
 *
 *  RETURNS: None.
 *
 */
void FillAuthReqMAC_CST( void )
{
	unsigned int value;


	// Clear MAC Buffer
	memset( MACBuffer, 0x00, sizeof( MACBuffer ));
	MACDataLen = 0;

	// Save The Nozzle Number
	POS_CR_Buf.TransNozzleNumber = TRINP.TR_TRANS_NOZZLE_NUMBER;
//	MACBuffer[MACDataLen] = TRINP.TR_TRANS_NOZZLE_NUMBER;
//	MACDataLen += 1;

	// Save The POS TTC
	memcpy( POS_CR_Buf.Auth_TTC, TRINP.TR_TRANS_TTC, sizeof( TRINP.TR_TRANS_TTC ));
//	memcpy( &MACBuffer[MACDataLen], TRINP.TR_TRANS_TTC, sizeof( TRINP.TR_TRANS_TTC ));
//	MACDataLen += sizeof( TRINP.TR_TRANS_TTC );

	// Save The Date
	memcpy( POS_CR_Buf.Auth_Date, TRINP.TR_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));
//	memcpy( &MACBuffer[MACDataLen], TRINP.TR_TRANS_DATE, sizeof( TRINP.TR_TRANS_DATE ));
//	MACDataLen += sizeof( TRINP.TR_TRANS_DATE );

	// Save The Time
	memcpy( POS_CR_Buf.Auth_Time, TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));
//	memcpy( &MACBuffer[MACDataLen], TRINP.TR_TRANS_TIME, sizeof( TRINP.TR_TRANS_TIME ));
//	MACDataLen += sizeof( TRINP.TR_TRANS_TIME );

	// Save The PayX
	POS_CR_Buf.Auth_Pay_X = TRINP.TR_PAY_X;
//	MACBuffer[MACDataLen] = TRINP.TR_PAY_X;
//	MACDataLen += 1;

	// Save The Price
	memcpy( POS_CR_Buf.TransPrice, TRINP.TR_TRANS_PRICE, sizeof( TRINP.TR_TRANS_PRICE ));
 //	memcpy( &MACBuffer[MACDataLen], TRINP.TR_TRANS_PRICE, sizeof( TRINP.TR_TRANS_PRICE ));
 //	MACDataLen += sizeof( TRINP.TR_TRANS_PRICE );

	// Save The Balance
	memcpy( POS_CR_Buf.ICCARD_Balance, TRINP.TR_CARD_APP_BALANCE, sizeof( TRINP.TR_CARD_APP_BALANCE ));

	// Save The Add On
	memcpy( POS_CR_Buf.Auth_Add_On, &TRINP.TR_NON_OIL_FLAG, 30 );

	// Save The PSAM ASN
	// Save The PIN Block
	memcpy( POS_CR_Buf.Auth_PIN_Block, &TRINP.TR_PSAM_ASN[2], 8 );
	memcpy( &POS_CR_Buf.Auth_PIN_Block[8], TRINP.TR_PIN_ENCRYPTED, sizeof( TRINP.TR_PIN_ENCRYPTED ));

//	// Get Preset Value
//	if( ProcExchange[PROCESS_KEYPAD][PROCESS_CR] & PRESET )
//	{
//		// Clear The Bit
//		ProcExchange[PROCESS_KEYPAD][PROCESS_CR] &= ( ~PRESET );
//		// Set The Bit To Inform Keypad
//		ProcExchange[PROCESS_CR][PROCESS_KEYPAD] |= PRESET;
//
//		// Use Keypad Preset
//		TRINP.TR_TRANS_PRESET_MODE = KEYPAD_CR_Buf.PresetMode;
//
//		// Set Prest Value
//		memcpy( TRINP.TR_TRANS_PRESET_VALUE, KEYPAD_CR_Buf.PresetValue, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
//
//	}
//	else
//	{
//		TRINP.TR_TRANS_PRESET_MODE = 0;
//
//		// Set Prest Value
//		memset( TRINP.TR_TRANS_PRESET_VALUE, 0x00, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
//
//	}

	// Save The Preset Value
	if( TRINP.TR_TRANS_PRESET_MODE == 0x00 )
	{
		value = CharToIntCopy( TRINP.TR_TRANS_PRESET_VALUE );
		if( value == CARD_PRESET_MAX )
		{
			// No Preset, Full Fuelling
			memset( TRINP.TR_TRANS_PRESET_VALUE, 0x00, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
		}
		else
		{
			// Reserve The Preset Value
		}

		// Preset Amount
		memcpy( POS_CR_Buf.TransAmount, TRINP.TR_TRANS_PRESET_VALUE, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
		memcpy( &MACBuffer[MACDataLen], &TRINP.TR_TRANS_PRESET_VALUE[1], 3 );
		MACDataLen += 3;

		// Skip Preset Volume
		memset( POS_CR_Buf.TransVolume, 0x00, sizeof( POS_CR_Buf.TransVolume ));
		MACDataLen += 3;
	}
	else
	{
		// Skip Preset Amount
		memset( POS_CR_Buf.TransAmount, 0x00, sizeof( POS_CR_Buf.TransAmount ));
		MACDataLen += 3;
		// Preset Volume
		memcpy( POS_CR_Buf.TransVolume, TRINP.TR_TRANS_PRESET_VALUE, sizeof( TRINP.TR_TRANS_PRESET_VALUE ));
		memcpy( &MACBuffer[MACDataLen], &TRINP.TR_TRANS_PRESET_VALUE[1], 3 );
		MACDataLen += 3;
	}

	// Save The ASN
	memcpy( POS_CR_Buf.ICCARD_ASN, TRINP.TR_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));
	memcpy( &MACBuffer[MACDataLen], TRINP.TR_CARD_ASN, sizeof( TRINP.TR_CARD_ASN ));
	MACDataLen += sizeof( TRINP.TR_CARD_ASN );
}



/*
 ** CheckGradeLimitExtend_Mode2
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check The Extend Grade Limited For Sinopec Mode2
 *
 *  RETURNS: 0x9000: No Limited, Go on....
 *			 0x00: Limited, Stop The Trans
 *
 */
unsigned short int CheckGradeLimitExtend_Mode2( void )
{
	unsigned char limittype;
	unsigned short int retval;
	unsigned short int TransGrade, CardGrade;

	// Default Limited
	retval = 0x00;

	if((( TRINP.TR_CARD_HOLDER_NEW_FLAG & CARD_MODE2_1 ) == CARD_MODE2_1 ) ||
		(( TRINP.TR_CARD_HOLDER_NEW_FLAG & CARD_MODE2_2 ) == CARD_MODE2_2 ))
	{
		// Get Card Grade
		CardGrade = CharToShortIntCopy( TRINP.TR_CARD_GRADE_LIMIT );
		
		// Get Trans Grade
		TransGrade = CharToShortIntCopy( TRINP.TR_TRANS_GRADE_CODE );

		// Get Extend Limited Byte
		if(( CardGrade & 0xF000 ) == 0x1000 )
		{
			if(( CardGrade & 0xFF00 ) == 0x1200 )
			{
				// Air Petrol Limited
				limittype = TRINP.TR_GRADE_LIMITED_EXTEND[1];
			}
			else
			{
				// Petrol Limited
				limittype = TRINP.TR_GRADE_LIMITED_EXTEND[0];
			}
		}
		else if(( CardGrade & 0xF000 ) == 0x2000 )
		{
			// Diesel Limited
			limittype = TRINP.TR_GRADE_LIMITED_EXTEND[1];
		}
		else
		{
			// Other Limited
			limittype = TRINP.TR_GRADE_LIMITED_EXTEND[2];
		}

		// Check The Limited Extend
		if(( CardGrade & 0xF000 ) == 0x1000 )
		{
			// Card Limited Petrol
			if(( limittype & PETROL_ALL ) == PETROL_ALL )
			{
				if(( TransGrade & 0xF000 ) == 0x1000 )
				{
					// Trans Grade Is Petrol, Not Limited
					retval = 0x9000;
				}
				else
				{
					// Trans Grade Is Not Petrol, Limited
					retval = 0;
				}
			}
			else if(( limittype & DIESEL_AIR ) == DIESEL_AIR )
			{
				if(( TransGrade & 0xFF00 ) == 0x1200 )
				{
					// Trans Grade Is Air Petrol, Not Limited
					retval = 0x9000;
				}
				else
				{
					// Trans Grade Is Not Air Petrol, Limited
					retval = 0;
				}
			}
			else if(( limittype & PETROL_90 ) == PETROL_90 )
			{
				if(( TransGrade & 0xFFF0 ) == 0x1010 )
				{
					// Trans Grade Is 90# Petrol, Not Limited
					retval = 0x9000;
				}
				else
				{
					// Trans Grade Is 90# Petrol, Limited
					retval = 0;
				}
			}
			else if(( limittype & PETROL_93 ) == PETROL_93 )
			{
				if(( TransGrade & 0xFFF0 ) == 0x1030 )
				{
					// Trans Grade Is 93# Petrol, Not Limited
					retval = 0x9000;
				}
				else
				{
					// Trans Grade Is 93# Petrol, Limited
					retval = 0;
				}
			}
			else if(( limittype & PETROL_95 ) == PETROL_95 )
			{
				if(( TransGrade & 0xFFF0 ) == 0x1040 )
				{
					// Trans Grade Is 95# Petrol, Not Limited
					retval = 0x9000;
				}
				else
				{
					// Trans Grade Is 95# Petrol, Limited
					retval = 0;
				}
			}
			else if(( limittype & PETROL_97 ) == PETROL_97 )
			{
				if(( TransGrade & 0xFFF0 ) == 0x1050 )
				{
					// Trans Grade Is 97# Petrol, Not Limited
					retval = 0x9000;
				}
				else
				{
					// Trans Grade Is 97# Petrol, Limited
					retval = 0;
				}
			}
			else if(( limittype & PETROL_98 ) == PETROL_98 )
			{
				if(( TransGrade & 0xFFF0 ) == 0x1090 )
				{
					// Trans Grade Is 98# Petrol, Not Limited
					retval = 0x9000;
				}
				else
				{
					// Trans Grade Is 98# Petrol, Limited
					retval = 0;
				}
			}
			else
			{
				// Do Not Care, Go On....
			}
		}
		else if(( CardGrade & 0xF000 ) == 0x2000 )
		{
			// Card Limited Diesel
			if(( limittype & DIESEL_NORM ) == DIESEL_NORM )
			{
				if(( TransGrade & 0xFF00 ) == 0x2000 )
				{
					// Trans Grade Is Diesel, Not Limited
					retval = 0x9000;
				}
				else
				{
					// Trans Grade Is Not Diesel, Limited
					retval = 0;
				}
			}
			else if(( limittype & DIESEL_LGT ) == DIESEL_LGT )
			{
				if(( TransGrade & 0xFF00 ) == 0x2100 )
				{
					// Trans Grade Is Light Diesel, Not Limited
					retval = 0x9000;
				}
				else
				{
					// Trans Grade Is Not Light Diesel, Limited
					retval = 0;
				}
			}
			else if(( limittype & DIESEL_HEV ) == DIESEL_HEV )
			{
				if(( TransGrade & 0xFF00 ) == 0x2200 )
				{
					// Trans Grade Is Heavy Diesel, Not Limited
					retval = 0x9000;
				}
				else
				{
					// Trans Grade Is Not Heavy Diesel, Limited
					retval = 0;
				}
			}
			else if(( limittype & DIESEL_MIL ) == DIESEL_MIL )
			{
				if(( TransGrade & 0xFF00 ) == 0x2300 )
				{
					// Trans Grade Is Military Diesel, Not Limited
					retval = 0x9000;
				}
				else
				{
					// Trans Grade Is Not Military Diesel, Limited
					retval = 0;
				}
			}
			else
			{
				// Do Not Card, Go On....
			}
		}
		else
		{
			// Do Not Care
		}
	}
	else
	{
		// Not Sinopec Mode2 Card
		// Do Not Care, Go On....
	}

	return retval;
}


/*
 ** UpdatePresetValue
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Update The Preset Value For Sinopec Mode2
 *
 *				 Only Call For Online Trans After Check Fuelling Limit
 *
 *  RETURNS: None.
 *
 */
void UpdatePresetValue( void )
{
//	unsigned int value;
//
//	if( TRINP.TR_WORK_MODE == 0x01 )
//	{
//		value = CharToIntCopy( TRINP.TR_MAX_PRESET_AMOUNT );
//
//		if( value == 0xFFFFFFFF )
//		{
//			// No Limit, Go On....
//		}
//		else
//		{
//			// Check Limit
//			if( TRINP.TR_CARD_PRESET_MODE == 0 )
//			{
//				// Amount Limit
//				memcpy()
//			}
//			else
//			{
//			}
//		}
//	}
//	else
//	{
//		// Offline, Do Not Update, Go On....
//	}
}


/*
 ** CheckOnlinePreset
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Check Online Preset Should Be > Price
 *
 *  RETURNS: 0: Online Preset Is Too Small; 1: Online Preset Ok
 *
 */
unsigned short int CheckOnlinePreset( void )
{
	unsigned short int retval;
	unsigned short int price;
	unsigned int preset;

	// default Error
	retval = 0;

	price = CharToShortIntCopy( TRINP.TR_TRANS_PRICE );
	preset = CharToIntCopy( TRINP.TR_TRANS_PRESET_VALUE );


	if(( TRINP.TR_AUTH_FEEDBACK == 0x01 ) || ( TRINP.TR_AUTH_FEEDBACK == 0x02 ))
	{
		// Only Check For Authed Ok
		if( preset >= price )
		{
			// Ok
			retval = 1;
		}
		else
		{
			// Preset Error, Too Small
		}
	}
	else
	{
		// Auth Error, Do Not Care About The Preset Value
		retval = 1;
	}

	return retval;
}

/*
 ** AuthErrorTrans
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\CardReader\Source\CRPumpCall.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Save The Auth Error For Card Error Trans Instead Of Auth Cancel Trans For Sinopec Mode2
 *
 *  RETURNS: None.
 *
 */
void AuthErrorTrans( void )
{
	SaveICCardData();

	// Save It For Record

	// Wrong Card, Black Card
	TRINP.TR_TRANS_TYPE &= 0xF0;
	TRINP.TR_TRANS_TYPE |= CARDERROR;
	TRINP.TR_TRANS_TYPE |= TRANSCARDERR;

	PowerDown.PdTrans.PD_TRANS_TYPE = TRINP.TR_TRANS_TYPE;

	// Auth Error
	FillAuthError();

//	// For Write The Running Log
//	JournalFlag = 31;
//
//	// Form Journal Next
//	Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
}

#endif	// MAKE_CARD_MODE2