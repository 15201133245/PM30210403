#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "tc.h"



// Attention: The DISPMSGID Must Corresponding To
// The Sequence Of DISPMSG
enum DISPMSGID
{
	M_Started,

	M_VersionTitle,
	M_VersionNumber,
	M_VersionNumber_New,	//20131021 qiyong add
	
	M_InfoLine,
	M_CustomMsg0,
	M_CustomMsg1,
	M_CustomMsg2,
	M_CustomMsg3,
	M_CustomMsg4,
	M_CustomMsg5,
	M_CustomMsg6,
	M_CustomMsg7,
	M_CustomMsg8,
	M_CustomMsg9,

	M_DefaultInfo,
	M_Wait,
	M_PriceUpdate,
	M_CardNotAllowed,

	M_CARDIN,
	M_CARDOK,
	M_FuellingCash,

	M_CardReaderReset,
	M_CRResetOk,
	M_CRResetErr,

	M_WaitPumpOnline,
	M_ReadPumpTotal,
	M_ReadPumpTotalFail,

	M_PSAMResetError,
	M_PSAMSelectPSEErr,
	M_PSAMSelectAPP1Err,
	M_PSAMSelectAPP2Err,
	M_PSAMUnValidate,
	M_PSAMExpired,

	M_GreyTrans,
	M_CardPowerOnErr,
	M_CardInverted,
	M_CardSelectAppErr,
	M_ManageCardLimit,
	M_CardUnValidate,
	M_CardExpired,
	M_CardAppBlocked,
	M_NotLocalCard,
	M_CardBlackList,
	M_LocalBlackList,
	M_POSBlackList,
	M_NeedPOSBlackList,
	M_EmpPwdErr,
	M_NotSameEMPCard,
	M_NotSameEMP,
	M_NotWhiteList,
	M_LocalWhiteList,
	M_POSWhiteList,
	M_NeedPOSWhiteList,
	M_CardError,
	M_CardPINBlocked,
	M_CardPINError,
	M_CardGreyDebit,
	M_NeedCheckGreyOnPOS,
	M_NoGreyRecord,

	M_NeedCheckTimeOnPOS,

	M_PINRetries,
	M_EMPPwdRetries,
    M_OverFueling,

	M_GetBalanceError,
	M_GetLockProofError,

	M_Fuelling,
	M_FuellingCardOut,
	M_PresetTooMuch,
	M_PresetTooSmall,
	M_BalanceTooSmall,

	M_WriteRecord,
	M_WriteRecordError,
	
	M_NozzleOnError,
	
	M_RecordMax,

	M_SysPowerDown,
	M_CardSpringOut,
	M_CardReadError,
	M_FuellingPumpOffline,
	M_NozzleNumberError,
	M_ReadLastTrans,
	M_CertificatePSAM,
	M_CertificatePSAMErr,
	M_GetGMAC,
	M_GetGMACErr,
	M_CreditPSAM,
	M_CreditPSAMErr,
	M_CardUnlock,
	M_CardUnlockErr,
	M_CardClearGreyFlag,
	M_CardClearGreyFlagErr,

	M_MsgBlackListCard,
//	M_MsgExpired,
	M_CustomerCardCheckLimited,
	M_MsgBrandLimited,
	M_MsgBrandLimitedExceed, 
	M_MsgLocationLimited,
	M_MsgLocationLimitedExceed,
	M_MsgTimesLimited,
	M_MsgTimesLimitedExceed,
	M_MsgAmountLimited,
	M_MsgAmountLimitedExceed,
	M_MsgVehicleNumberLimited,
	M_FuellingLimited,

//Douzhq 20140805 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
	M_CarNumMatch,
	M_CarNumNoMatch,
	M_CarNumMatchRecing,
	M_CarNumRecTimeout,
	
	M_GradeLimitMatching,
	
	M_CarNumberRecing,
	M_GradeLimitRecing,
	M_Waiting,	
#endif

	M_ProvinceLimited,
	M_CityLimited,
	M_OilStationLimited,
	M_SuperiorUnitLimited,
	M_PerAmountLimited,
	M_PerVolumeLimited,
	M_TotalAmountLimited,
	M_PerAmountLimited_D,
	M_PerVolumeLimited_D,
	M_TotalAmountLimited_D,

	M_CardInitGreyLock,
	M_CardInitGreyLockErr,
	M_PSAMInitGreyLock,
	M_PSAMInitGreyLockErr,
	M_CardSetGreyLock,
	M_CardSetGreyLockErr,
	M_CardAuthPump,


	
	M_SelectPayMode,
	M_Cash,
	M_OilTicket,
	M_Ticket,
	M_CreditCard,
	M_Other1,
	M_Other2,

	M_SelectApp,
	M_SinopecApp,
	M_LoyaltyApp,
//Douzhq 20140523 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	M_SaveoilApp,			
	M_SaveoilError,
#endif
	M_NeedRecord,

	M_CARD_PIN,
	M_EMP_PWD,
	M_MAINTAIN_PWD,
	M_FUNCTION_PWD,
	
//Douzhq 20140528 Add , Lock Nozzle 
	M_UNLOCK_PWD,
	M_UNLOCK_RETRY,

	M_FUNCPWD_RETRY,
	M_MATNPWD_RETRY,
	M_FORMAT_PWD,
	M_FORMATPWD_RETRY,

	M_ERROR_STOP,
	M_CardInserted,
	M_InvalidOperation,		//Douzhq 20141023 Add , For Bug
	M_CashCardRetry,
	M_NozzleOffCashError,
	M_NozzleError,
	M_InputAuthCode,
	M_AuthWaiting,
	M_EmployeeCardNotForCash,


	M_Function,
	M_Version,
	M_Inquire,
	M_Setup,
	M_Maintain,
	M_Logon,
	M_Logoff,

	M_WorkMode,
	M_CustomerCardSpringOutOrNot,
	M_WorkModeSet,
	M_CurrentWorkMode,
	M_WorkOffline,
	M_WorkOnline,
	M_WorkModeSwitch,
	M_WorkModeExit,
	M_InsertMaintainCard,
	M_CalcMACError,
	M_VerifyMACError,
	M_PressKey,
	M_EMPCardLimited,
	M_WaitAuth,
	M_AuthEnd,
	M_WaitFuellingData,
	M_AuthTimeOut,
	M_TimeOut,
	M_UploadRecord,
	M_UploadRecordEnd,
	M_HostAuth,
	M_HostAuthCancel,
	M_HostStop,
	M_HostCancel,
	M_AuthedNotPreset,
	M_CardInNotHostAuth,
	M_OfflineNotPreset,
	M_OffDutyNotPreset,
	M_MustBeHostAuth,

	M_RecordToEMPCard,
	M_RecordToEMPCardTotal,
	M_RecordToEMPCardFull,
	M_RecordToEMPCardError,


	M_NotEMPCard,
	M_LogOned,
	M_LogOffed,
	M_InsertCard,

	M_InquireFunc,
	M_Station,
	M_RecordSpace,
	M_Record,
	M_Total,
	M_PriceInq,

	M_StationFunc,
	M_Province,
	M_City,
	M_Superior,
	M_StationID,
	M_POSP,

	M_RecordSpaceFunc,
	M_RecordSpaceNum,
	M_RecordNeedUpload,

	M_VolumeTotal,
	M_VolumeTotalLogOn,
	M_AmountTotal,
	M_AmountTotalLogOn,
	
#ifdef MAKE_CASH_CR
	M_CashAcceptorTrans,
	M_CashPOSTrans,

	M_CashBalance,
	M_CashPOSAuthCode,
	M_CashAcceptorTotal,
	M_CashPOSTotal,

	M_CashVolumeTotalLogOn,
	M_CashAmountTotalLogOn,
#endif

	M_PriceInqiry,


	M_SetUpFunction,
	M_BackLightTime,
	M_SetUpPrint,
	M_SetUpVolume,
	M_SetUpPSAMCheck,

	M_SetBackLightTime,
	M_CurrentBackLightTime,
	M_NewBackLightTime,
	M_TimeInputError,
	M_TimeLate,

	M_PrintSetUp,
	M_CustomerCard,
	M_EMPCard,
	M_TestCard,
	M_MatnCard,
	M_PubTrans,
	//Douzhq 20140919 Add , Config Printer Set
	M_ExpandSetUp,
	
	M_CustomerCardPrint,
	M_EMPCardPrint,
	M_TestCardPrint,
	M_MatnCardPrint,
	M_PubTransPrint,
	//Douzhq 20140919 Add , Config Printer Set
	M_ExpandSetPrint,
	
	M_PrinterSwitch,
	M_InquirySwitch,
	M_ReceiptNumber,
	
	M_PrinterSwitchSet,
	M_InquirySwitchSet,
	M_ReceiptNumberSet,
	M_PrinterFeatureOn,
	M_PrinterFeatureOff,
	M_InquiryOff,
	M_InquiryOn,
	M_ReceiptShort,
	M_ReceiptLong,
	
	M_PrinterMsg,
	M_InquiryMsg,
	M_ReceiptMsg,
	
	M_NonCard,
	M_LogOnTrans,
	M_LogOffTrans,
	M_PriceRecv,

	M_TransOk,
	M_TransGrey,
	M_TransUnlock,
	M_TransGreyFlag,
	M_ErrCard,
	M_CardErrorTrans,

	M_VolumeSetup,
	M_VolumeUp,
	M_VolumeDown,

	M_PSAMSetUp,
	M_PSAMCheck,
	M_PSAMCheckNot,


	M_MatnFunction,
	M_UnfinishedTrans,
	M_RunningLog,
	M_LocalPrinter,
	M_SetPOSP,
//	M_FormatFlash,
	M_SpecialFunction,
	M_SoftwareUpgrade,
    M_RebuildProtected,
//#ifndef MAKE_CARD_MODE2    
    M_WorkManage,
//#endif

//Douzhq 20140609 Add , Menu About
	M_Manage,
	M_ManageFunction,
	M_WorkManage2,
	M_LockNozzleManage,

//Douzhq 20141010 Add , Safe Mode
//-------  Begin , Douzhq Add , Safe Mode , 20141010 -------
	M_SafeMode,
	M_SetSafeMode,
	M_SafeModeOn,
	M_SafeModeOff,
	M_SelSafeMode,
//-------  End   , Douzhq Add , Safe Mode , 20141010 -------

//Douzhq 20140819 Add , Car Number Recognize
//Car Number Recognize , Menu About
#ifdef MAKE_CASH_CR
	M_CardNumberRecognize,
	M_CarNumberRecognizeOn,
	M_CarNumberRecognizeOff,
	M_SelCarNumberRecognize,
	M_SetCarNumberRecognize,
#endif

#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	M_CardRecognize,
	M_SaveOilManage,
	M_DiscountManage,
#endif

//Douzhq 20140610 Add , Menu About
	M_SetLockNozzle,
	M_LockNozzleOn,
	M_LockNozzleOff,
	M_SelLockNozzle,

//Douzhq 20140610 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	M_SetCardRecoginze,
	M_ICCardAppSwitch,
	M_CardRecognizeOn,
	M_CardRecognizeOff,
	M_SelCardRecognize,
	
	M_TitleCardRecognize,
	M_TitleCardSwitch,
	  
	M_SwitchCardRecognize,
	M_SinopecAppRecognize,
	M_SocialAppRecognize,
	M_Other1AppRecognize,
	M_Other2AppRecognize,
#endif

//Douzhq 20140610 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	M_SetSaveOilApp,
	M_SaveOilAppOn,
	M_SaveOilAppOff,
	M_SelSaveOilApp,
#endif

//Douzhq 20140610 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	 M_SetVipFueling,
	 M_VipFuelingOn,
	 M_VipFuelingOff,
	 M_SelVipFueling,
#endif

	M_UnfinishedTransSetUp,
	M_TransFinished,
	M_Reason0,
	M_Reason1,
	M_Reason2,

	M_RunningLogSetup,
	M_UploadPWD,
	M_UploadPWDError,
	M_UploadSoftware,
	M_UploadReady,
	M_FunctionNotSupported,

	M_LocalPrinterSetUp,

	M_PrinterCR,
	M_PrinterCR_O,


//	M_FormatFlashFunction,
	M_SpecialFunctionTitle,
	M_PowerOnDebit,
	M_BlackListSetup,
	M_FormatTransFlash,
	M_FormatConfigFlash,

	M_FormatConfigFunction,
	M_Warning,
	M_FormatWarning1,
	M_FormatWarning2,
	M_FormatWarning3,

	M_FormatTransFunction,
	M_FormatWarning4,
	M_FormatWarning5,

	M_DebitPowerOn,
	M_DebitPowerDown,
	M_GreyPowerDown,

	M_BlackListPOSSetup,
	M_BlackListPOSFirst,
	M_BlackListLocalFirst,
	
	M_CurrentSpecialFunc,
	M_UpdateSpecialFunc,

	M_SetUpPOSP,

	M_UpgradeSW,
	M_UpgradeSW0,
	M_UpgradeSW1,

//#ifndef MAKE_CARD_MODE2
    M_WorkManageSetup,
    M_WorkManage_On,
    M_WorkManage_Off,
    M_Emp_Number,
    M_Emp_Number_Out_Of_Range,
//#endif

	M_TTC,
	M_ASN,
	M_Time,
	M_TransType,
	M_Balance,
	M_Amount,
	M_CTC,
	M_TAC,
	M_GMAC,
	M_SAM_TAC,
	M_SAM_ASN,
	M_SAM_TID,
	M_SAM_TTC,
	M_DS,
	M_UNIT,
	M_CardType,
	M_KeyVersion,
	M_NozzleNumber,
	M_Grade,
	M_Volume,
	M_Price,
	M_EmployeeNumber,
	M_VolumeTotalDisp,
	M_TerminalTAC,

	M_TransTypeOk,
	M_TransTypeGrey,
	M_TransTypeErrorCard,
	M_TransTypeGreyUnlock,
	M_TransTypeGreyFlag,
	M_TransTypeLogOn,
	M_TransTypeLogOff,
	M_TransTypeNonCard,
	M_TransTypePriceRecv,
	M_TransTypeCardErr,

	M_TransTypeRFU,
	M_TransTypeABN,
	M_TransTypeNonCardSS,
	M_TransTypeHostAuth,
	M_TransTypeAuthCancel,
	M_TransTypeOverFuel,


	M_TransUploaded,
	M_TransUnUploaded,
	M_TransSaveToEMPCard,
	M_TransNotSaveToEMPCard,


	M_TransDisp,
	M_ENTER,
	M_CANCEL,
	M_SELECT,

	M_InputTTC,
	M_ENTER2,
	M_LastTrans,
	
	M_ENTER3,
	M_ENTER4,


#ifdef MAKE_PSAM_VERIFY
	M_V_PSAM,
	M_V_PSAM_NOT_SUPPORT,
	M_V_PSAM_CHANGE,
	M_V_PUMP_NOT_REGISTERED,
	M_V_INSERT_ACT,
	M_V_PSAM_LOCK,
	M_V_RE_BINDING,

	M_V_Insert_ACT,
	M_V_Insert_RID,
	M_V_Certificate_Times,
	M_V_PSAM_Random_Error,
	M_V_PSAM_Start_Binding_Error,

	M_V_PSAM_ACT_Ok,
	M_V_RID_Error,

	M_V_Registering,
	M_V_ReInsert_RID,

	M_V_VerifyError,

	M_V_ACT_Not_Ready,
	M_V_RegisterOk,
	M_V_RegisterError,
	M_V_Read_Flag_Error,

#endif




	M_Test_Manage_Card0,
	M_Test_Manage_Card1,
	M_Test_Not_EMP_Card,
	M_Test_Not_Test_Card,
	M_Test_Not_Maintain_Card,

	M_Test_CardUnValidate,
	M_Test_CardExpired,

	M_Test_IssuerID_Err,
	M_Test_AppID_Err,
	M_Test_AppVer_Err,
	M_Test_AppSN_Err,
	M_Test_KeyVer_Err,
	M_Test_Batch_Err,
	M_Test_Type_Err,
	M_Test_CardType_Err,
	M_Test_Instruction_Err,
	M_Test_PINType_Err,
	M_Test_CardLocation_Err,


	M_PumpMsg,
	
	M_PumpMsgStart,
	M_PumpUpdatePrice,
	M_PumpAuth,
	M_PumpFuelling,
	
	M_LockedNozzle,
	
	M_PumpMsgEnd,
	M_PumpErrStart,

	M_PumpOffline,
	M_PumpError,
	M_PumpNozzleLiftError,
	M_PumpPresetExceed,
	M_PumpUpdatePriceError,
	M_PumpAuthError,
	M_PumpLastTransError,
	M_PumpNozzleNumberError,
	M_PumpOfflineTransError,
	M_PumpTimeError,
	M_PumpPriceError,
	M_PumpConfigureError,
	M_PumpStateError,
	M_Pump485AddrError,
	M_PumpCRCommError,
	M_PumpPPUError,
	M_PumpCertificateError,

	M_PumpErrEnd,
	M_PumpPumpErrorStart,

	M_PumpErr1,
	M_PumpErr2,
	M_PumpErr3,
	M_PumpErr4,
	M_PumpErr5,
	M_PumpErr6,
	M_PumpErr7,
	M_PumpErr8,

	M_PumpPumpErrorEnd,

	M_UploadFirst,	//20131126 qiyong modify for transspace limit

	M_Max
};

//extern const char * DISPMSG[];
extern const char DISPMSG[][20];

extern unsigned char MsgDelayFlag;
extern Tmr * MsgTimerDelayIndex;

extern enum DISPMSGID MsgID;
extern enum DISPMSGID NextMsgID;

extern void DispInfoDelayTimeOut( void );
extern void FillInfoMsg( enum DISPMSGID DispMsgID, unsigned char DelayTime, unsigned char DispFlag );
//extern void DispDefaultInfoLine( void );
extern void SetDispDefaultInfo( void );
extern void StopDefaultInfo( void );
extern void ResetDefaultInfo( void );




#endif
