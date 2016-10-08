#include "tc.h"


#include "global.h"
#include "Display.h"
#include "Screen.h"
#include "Message.h"
#include "DispPumpMsg.h"




// Attention: The Sequence Must According To The
// enum of DISPMSGID
//const char * DISPMSG[] = {
const char DISPMSG[][20] = {
	"    ",											// M_Started, Card Reader Message Begin

	"�汾��Ϣ",										// M_VersionTitle
//	"GPay01.00.TT",									// M_VersionNumber, YY.MM.DD.VV
#ifdef MAKE_CARD_MODE2
		"GPay02.00.00",
		"            ",							// M_VersionNumber_New, YY.MM.DD.VV
#else

#ifdef MAKE_PSAM_VERIFY
	#ifdef MAKE_CASH_CR
		"GPay01.05.12",							// M_VersionNumber, YY.MM.DD.VV
		"PM30.24.04.03",							// M_VersionNumber_New, YY.MM.DD.VV	//20131021 qiyong add
	#else											
// Special For CrProtect Memory Check Test
//		"GPay01.02.04",								// M_VersionNumber, YY.MM.DD.VV
		"GPay01.02.12",							// M_VersionNumber, YY.MM.DD.VV
		"PM30.22.04.03",							// M_VersionNumber_New, YY.MM.DD.VV
	#endif
#else
	#ifdef MAKE_CASH_CR
		"GPay01.04.06",							// M_VersionNumber, YY.MM.DD.VV
		"PM30.23.04.03",							// M_VersionNumber_New, YY.MM.DD.VV
	#else
		"GPay01.00.47",							// M_VersionNumber, YY.MM.DD.VV
		"PM30.21.04.03",							// M_VersionNumber_New, YY.MM.DD.VV
	#endif
#endif

#endif	// MAKE_CARD_MODE2
	"INFOLINE",									// M_InfoLine, Display Info Line For Custom Message
	"CM0",											// M_CustomMsg0, Display Custom Message0
	"CM1",											// M_CustomMsg1, Display Custom Message1
	"CM2",											// M_CustomMsg2, Display Custom Message2
	"CM3",											// M_CustomMsg3, Display Custom Message3
	"CM4",											// M_CustomMsg4, Display Custom Message4
	"CM5",											// M_CustomMsg5, Display Custom Message5
	"CM6",											// M_CustomMsg6, Display Custom Message6
	"CM7",											// M_CustomMsg7, Display Custom Message6
	"CM8",											// M_CustomMsg8, Display Custom Message6
	"CM9",											// M_CustomMsg9, Display Custom Message6


	"��ӭʹ�ó���ϵͳ",								// M_DefaultInfo, Default Info Line Display
	"���Ժ�,���ڴ�����",							// M_Wait
	"�����ͼ�",										// M_PriceUpdate,
	"�������IC��",									// M_CardNotAllowed,


	"û��IC������",									// M_CARDIN, No IC Card In Reader
	"IC��׼����",									// M_CARDOK, IC Card Is Ready For Fuelling
	"������....",									// M_FuellingCash, Fuelling For Cash Trans

	"��������λ",									// M_CardReaderReset, Reset The Card Reader
	"��������λ�ɹ�",								// M_CRResetOk, Card Reader Reset Ok
	"��������λ����",								// M_CRResetErr, Card Reader Reset Error
	
	
	"���������ͻ�",									// M_WaitPumpOnline,
	"��ȡ�ͻ�����",									// M_ReadPumpTotal, Read Pump Total Message
	"��ȡ�ͻ�����ʧ��",								// M_ReadPumpTotalFail, Read The Pump Totals Fail

	"PSAM��λ����",									// M_PSAMResetError, PSAM Reset Error
	"PSAMѡ��PSE����",								// M_PSAMSelectPSEErr, PSAM Select PSE Error
	"PSAMѡ��Ӧ��1����",							// M_PSAMSelectAPP1Err,	PSAM Select Sinopec1 Error
	"PSAMѡ��Ӧ��2����",							// M_PSAMSelectAPP2Err,	PSAM Select Sinopec2 Error
	"PSAM��Ӧ��δ����",								// M_PSAMUnValidate, PSAM Card Application Is Un Validate
	"PSAM��Ӧ�ù���",								// M_PSAMExpired, PSAM Card Expired

	"���ɻҼ�¼",									// M_GreyTrans, Grey Card, Wait Unlock Debit
	" IC���ϵ�ʧ��",								// M_CardPowerOnErr, IC Card Power On Error
	"IC�����ܲ巴��",								// M_CardInverted, IC Card Inverted Maybe
	"IC��ѡ��Ӧ�ô���",								// M_CardSelectAppErr, IC Card Select Application Error
	"�������ܼ���",								// M_ManageCardLimit, Manage Card Do Not Fuelling
	"��δ����",									    // M_CardUnValidate, IC Card Application Is Un Validate
//	"��Ӧ�ù���",									// M_CardExpired, IC Card Expired
	"ʧЧ��",										// M_CardExpired, IC Card Expired
	"��Ӧ������",									// M_CardAppBlocked, IC Card Application Was Blocked
//	"��ؿ�����ѡ��Ӧ��",							// M_NotLocalCard, IC Card Select Application Error
	"����ѡ��Ӧ��",									// M_NotLocalCard, IC Card Select Application Error
	"��ʧ��",										// M_CardBlackList, IC Card In Black List
	"��ѯ����������",								// M_LocalBlackList, Check Local Black List
	"������ѯ������",								// M_POSBlackList, Check POS Black List
	"��������ѯ������",								// M_NeedPOSBlackList, Need Check Black List On POS, But POS Is Offline
	"Ա���������",									// M_EmpPwdErr, Employee Password Error
	"��ǩ��Ա����",									// M_NotSameEMPCard, Employee Card Is Not Same With The LogOn Card Number
	"Ա���Ų�ͬ",									// M_NotSameEMP, Employee Card Number Is Not Same With The LogOn Card Number
	"�Ǳ�վ��������",								// M_NotWhiteList, Not Belong To White List
	"��ѯ����������",								// M_LocalWhiteList, Check Local White List
	"������ѯ������",								// M_POSWhiteList, Check POS White List
	"��������ѯ������",								// M_NeedPOSWhiteList, Need Check White List On POS
//Douzhq 20140610 Add , IC Card Recognisze
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	"�Ǳ�ϵͳ��",									// M_CardError, IC Card Error
#else
	"�Ǽ��Ϳ�",										// M_CardError, IC Card Error
#endif
	"IC����������",									// M_CardPINBlocked, Card PIN Was Blocked
	"IC������У��ʧ��",								// M_CardPINError, Card PIN Error
	"�ҿ�,�����",									// M_CardGreyDebit, Grey Card, Waiting Unlock Debit
	"��Ҫ������Ҽ�¼",								// M_NeedCheckGreyOnPOS
	"�޴˻Ҽ�¼",									// M_NoGreyRecord

	"��������ʱ",									// M_NeedCheckTimeOnPOS, POS IS Offline While 
													// Pump Time Is Earlier Then The Last Transaction Time

	"�����,����*�λ���",							// M_PINRetries, Pin Error, * Times Retry Left
	"Ա�������,��������",							// M_EMPPwdRetries, Employee Password Error, Re-Input Please
    "���ͻ����壡",                                 // M_OverFueling

	"�����ʧ��",									// M_GetBalanceError,
	"��ȡ����״̬ʧ��",								// M_GetLockProofError,

	"������,����ο�",								// M_Fuelling, IC Card Is Fuelling, Do Not Sping Out Card
	"�����аο�,���ǹ",							// M_FuellingCardOut, IC Card Is Fuelling But Spring Out IC Card
	"Ԥ��ֵ����",									// M_PresetTooMuch, The KeyPreset Is Larger Then Permitted.
	"Ԥ��ֵ��С",									// M_PresetTooSmall, The KeyPreset Is Too Small < 1L Or Price.
	"����",										// M_BalanceTooSmall, The Balance Is Too Small

	"���潻��",										// M_WriteRecord,
	"���潻��ʧ��",									// M_WriteRecordError,

	"IC��Ч̧ǹ,���ǹ",							// M_NozzleOnError
	
	"��¼����,�������ϴ�",							// M_RecordMax

	"ϵͳ����....",									// M_SysPowerDown
	"����",											// M_CardSpringOut
	"��������",										// M_CardReadError
	"�ͻ�����",										// M_FuellingPumpOffline
	"��ǹ�Ų�ͬ",									// M_NozzleNumberError
	"��ȡ�ϱʽ���",									// M_ReadLastTrans
	"Certificate PASM",								// M_CertificatePSAM
	"Certific PASM Error",							// M_CertificatePSAMErr
	"ȡGMAC",										// M_GetGMAC
	"ȡGMAC����",									// M_GetGMACErr
	"Credit PSAM",									// M_CreditPSAM
	"Credit PSAM Error",							// M_CreditPSAMErr
	"IC����ҿۿ�",									// M_CardUnlock
	"�޷����",										// M_CardUnlockErr
	"IC����ұ�",									// M_CardClearGreyFlag
	"IC����ұ����",								// M_CardClearGreyFlagErr


	"��������",										// M_MsgBlackListCard, The IC Card Is In Black List
//	"ʧЧ��",										// M_MsgExpired, The Card Either Not Validate Or Expired
	"�û���,����",									// M_CustomerCardCheckLimited
	"����Ʒ��",										// M_MsgBrandLimited, The Card Is Brand Limited
//	"����Ʒ�����޼���",								// M_MsgBrandLimitedExceed, 
	"����Ʒ��",										// M_MsgBrandLimitedExceed, 
	"�޵��п�",										// M_MsgLocationLimited,
	"�޵��п����޼���",								// M_MsgLocationLimitedExceed,
	"��ÿ�ռ��ʹ�����",								// M_MsgTimesLimited,
//	"�޴��������޼���",								// M_MsgTimesLimitedExceed,
	"���ռ��ʹ�����",								// M_MsgTimesLimitedExceed,
	"�޼��ͽ�",									// M_MsgAmountLimited,
	"���ռ��ͽ�",								// M_MsgAmountLimitedExceed,
	"�޳��ſ�",										// M_MsgVehicleNumberLimited,
	"�����Ƽ�����Ϣ",								// M_FuellingLimited
	
//Douzhq 20140805 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
	"���ͳ������",									// M_CarNumMatch
	"���ͳ��Ų���",									// M_CarNumNoMatch
	"����ʶ����...",								// M_CarNumMatchRecing
	"����ʶ��ʱ",									// M_CarNumRecTimeout
	
	"��Ʒ�˶���...",								// M_GradeLimitMatching
	
	"����ʶ���У�",									// M_CarNumberRecing
	"��Ʒ�˶��У�",									// M_GradeLimitRecing
	"���Ժ�...",									// M_Waiting	
#endif
	
	"��ʡ��",										// M_ProvinceLimited,
//#ifdef MAKE_CARD_MODE2
//	"��ʡ��",										// M_CityLimited,
//#else
	"�޵��п�",										// M_CityLimited,
//#endif	// MAKE_CARD_MODE2
	"�޼���վ��",									// M_OilStationLimited,
	"���ϼ���λ",									// M_SuperiorUnitLimited,
	"���μ��ͽ������",								// M_PerAmountLimited,
	"���μ�����������",								// M_PerVolumeLimited,
	"�ܼ��ͽ������",								// M_TotalAmountLimited,
	"�޵��ν��",									// M_PerAmountLimited_D,
	"�޵�������",									// M_PerVolumeLimited_D,
	"�����ܽ��",									// M_TotalAmountLimited_D,


	"IC����ʼ������",								// M_CardInitGreyLock
	"IC����ʼ������ʧ��",							// M_CardInitGreyLockErr
	"PSAM��ʼ������",								// M_PSAMInitGreyLock
	"PSAM��ʼ������ʧ��",							// M_PSAMInitGreyLockErr
	"IC���û�",										// M_CardSetGreyLock
	"IC���û�ʧ��",									// M_CardSetGreyLockErr
	"IC����Ȩ���",									// M_CardAuthPump



	"ѡ��֧����ʽ",									// M_SelectPayMode, Select The Pay Mode
													// 0: Cash ( xian jin ); 1: Oil Ticket ( you piao ); 2: Ticket ( ti you ping zheng );
													// 3: CreditCard ( xin yong ka ); 4: Other1 ( qi ta 1 ); 5: Other2 ( qi ta 2 );
	"0.�ֽ�",										// M_Cash
	"1.��Ʊ",										// M_OilTicket
	"2.����ƾ֤",									// M_Ticket
	"3.���п�",										// M_CreditCard
	"4.����1",										// M_Other1
	"5.����2",										// M_Other2

	"ѡ��IC��Ӧ��",									// M_SelectApp
//	" 0. ��ʯ������Ӧ��",							// M_SinopecApp
//	" 1. ��ʯ������Ӧ��",							// M_LoyaltyApp
//	" 1. ��ʯ������Ӧ��",							// M_SinopecApp
//	" 2. ��ʯ������Ӧ��",							// M_LoyaltyApp
	" 1. ������Ʊ",									// M_SinopecApp
	" 2. ����Ӧ��",									// M_LoyaltyApp
	
//Douzhq 20140523 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	" 3. ����Ӧ��",									// M_SaveoilApp
	"��Ƭ����Ӧ�ô���",								// M_SaveoilError
#endif

	" �����ü�¼",									// M_NeedRecord

	"������IC������",								// M_CARD_PIN
	"������Ա������",								// M_EMP_PWD
	"������ά������",								// M_MAINTAIN_PWD
	"��������������",								// M_FUNCTION_PWD
	
//Douzhq 20140528 Add , Lock Nozzle 
	"�������������",								//M_UNLOCK_PWD
	"�����������,������",							//M_UNLOCK_RETRY

	"�����������,������",							// M_FUNCPWD_RETRY
	"ά���������,������",							// M_MATNPWD_RETRY
	"�������ʽ������",								// M_FORMAT_PWD
	"��ʽ�������,������",							// M_FORMATPWD_RETRY

	"����,��ʱͣ��",								// M_ERROR_STOP
	"����IC������",									// M_CardInserted
	//Douzhq 20141023 Add , For Bug
	"��Ч����",										// M_InvalidOperation
	"���ƿ�����,������",							// M_CashCardRetry
	"���δ���,���ǹ",							// M_NozzleOffCashError
	"��Ʒ����,���ǹ",								// M_NozzleError
	"  ��������Ȩ��",								// M_InputAuthCode
	"����������Ȩ,���Ժ�",							// M_AuthWaiting
	"Ա������ֹ����",								// M_EmployeeCardNotForCash,


	"��    ��",										// M_Function, Display Function Screen
	"0.�汾��Ϣ",									// M_Version
	"1.��ѯ",										// M_Inquire
	"2.����",										// M_Setup
	"3.ά��",										// M_Maintain
	"4.�ϰ�",										// M_Logon
	"5.�°�",										// M_Logoff
	"6.��������",									// M_WorkMode
	"7.�û���",										// M_CustomerCardSpringOutOrNot
	
	"����/�����趨",								// M_WorkModeSet
	"��ǰģʽ: ",									// M_CurrentWorkMode
	"����ģʽ",										// M_WorkOffline
	"����ģʽ",										// M_WorkOnline
	"��ѡ�񡱼�,�޸�",								// M_WorkModeSwitch
	"�����ġ���,�˳�",								// M_WorkModeExit
	"�����ά�޿�",									// M_InsertMaintainCard
	"����MAC����",									// M_CalcMACError
	"��֤MAC����",									// M_VerifyMACError
	"�������,����",								// M_PressKey
	"��̨��Ȩ����Ա����",							// M_EMPCardLimited,
	"����������Ȩ....",								// M_WaitAuth,
	"������Ȩ���",									// M_AuthEnd,
	"�����ύ��������",								// M_WaitFuellingData,
	"��Ȩ��ʱ,������",								// M_AuthTimeOut,
	"ͨѶ��ʱ,���׽���",							// M_TimeOut,
	"�����ϴ�IC����¼",								// M_UploadRecord,
	"�ϴ�IC����¼���",								// M_UploadRecordEnd,
	"��̨��Ȩ",										// M_HostAuth,
	"��̨������Ȩ",									// M_HostAuthCancel,
	"ֹͣ����,���ǹ",								// M_HostStop,
	"��̨ȡ����Ȩ",									// M_HostCancel,
	"��Ȩ���,����Ԥ��",							// M_AuthedNotPreset,
	"���п�����,����ִ��",							// M_CardInNotHostAuth,
	"����ģʽ,����ִ��",							// M_OfflineNotPreset,
	"Ա��δ�ϰ಻��ִ��",							// M_OffDutyNotPreset,
	"�Ǻ�̨��Ȩ����ִ��",							// M_MustBeHostAuth,
	
	"������****������",								// M_RecordToEMPCard,
	"������****������",								// M_RecordToEMPCardTotal,
	"��¼��,��ȥ��̨����",							// M_RecordToEMPCardFull,
	"����,��ȥ��̨����",							// M_RecordToEMPCardError,
	

	"��Ա����,����ִ��",							// M_NotEMPCard
	"Ա�����ϰ�",									// M_LogOned
	"Ա��δ�ϰ�",									// M_LogOffed
	"û�п�����",									// M_InsertCard

	"��ѯ����",										// M_InquireFunc
	"0��վ��Ϣ",									// M_Station
	"1��¼�ռ�",									// M_RecordSpace
	"2���׼�¼",									// M_Record
	"3��������",									// M_Total
	"4��ѯ����",									// M_PriceInq

	"��վ��Ϣ",										// M_StationFunc
	"ʡ: ",											// M_Province
	"��: ",											// M_City
	"�ϼ�: ",										// M_Superior
	"վ��: ",										// M_StationID
	"POSP: ",										// M_POSP

	"��ѯ��¼�ռ�",									// M_RecordSpaceFunc
	"��¼�ռ�: ",									// M_RecordSpaceNum
	"δ���ͼ�¼: ",									// M_RecordNeedUpload

	"�� ���ۼ�",									// M_VolumeTotal
	"�� ���ۼ�",									// M_VolumeTotalLogOn
	"��� ���ۼ�",									// M_AmountTotal
	"��� ���ۼ�",									// M_AmountTotalLogOn

#ifdef MAKE_CASH_CR
	"�ֽ���",										// M_CashAcceptorTrans
	"��Ȩ����",										// M_CashPOSTrans

	"���: ",										// M_CashBalance
	"��Ȩ��:",										// M_CashPOSAuthCode
	"�ճ��ܶ�: ",									// M_CashAcceptorTotal
	"��Ȩ�ܶ�: ",									// M_CashPOSTotal

	"�� ���ۼ� (�ֽ�)",								// M_CashVolumeTotalLogOn
	"��� ���ۼ� (�ֽ�)",							// M_CashAmountTotalLogOn
#endif

	"���۲�ѯ",										// M_PriceInqiry

	"���ù���",										// M_SetUpFunc
	"0. ���⿪/��ʱ��",								// M_BackLightTime
	"1. ��ӡ����",									// M_SetUpPrint
	"2. ��������",									// M_SetUpVolume
	"3. PSAM����Ч��",								// M_SetUpPSAMCheck

	"���ñ��⿪/��ʱ��",							// M_SetBackLightTime
	"��ǰ��/��ʱ��:",								// M_CurrentBackLightTime
	"�¿�/��ʱ��:  ",								// M_NewBackLightTime
	"����,ʱ��ӦС��24",							// M_TimeInputError
	"��ʱ��Ӧ���ڹ�ʱ��",							// M_TimeLate


	"��ӡ��������",									// M_PrintSetUp
	"0�û���",										// M_CustomerCard
	"1Ա����",										// M_EMPCard
	"2��ÿ�",										// M_TestCard
	"3ά�޿�",										// M_MatnCard
	"4��������",									// M_PubTrans
	//Douzhq 20140919 Add , Config Printer Set
	"5��չ����",									// M_ExpandSetUp
	
	"�û�����ӡ����",								// M_CustomerCardPrint
	"Ա������ӡ����",								// M_EMPCardPrint
	"��ÿ���ӡ����",								// M_TestCardPrint
	"ά�޿���ӡ����",								// M_MatnCardPrint
	"�������״�ӡ����",								// M_PubTransPrint
	//Douzhq 20140919 Add , Config Printer Set
	"��չ��ӡ����",									// M_ExpandSetPrint
	
	"0��ӡ����",									// M_PrinterSwitch
	"1ѯ�ʿ���",									// M_InquirySwitch
	"2СƱ����",									// M_ReceiptNumber
	
	"��ӡ��������",									// M_PrinterSwitchSet
	"ѯ�ʿ�������",									// M_InquirySwitchSet
	"СƱ��������",									// M_ReceiptNumberSet
	"��ӡ���ܴ�",									// M_PrinterFeatureOn
	"��ӡ���ܹر�",									// M_PrinterFeatureOff
	"��ӡǰ��ѯ��",									// M_InquiryOff
	"��ӡǰѯ��",									// M_InquiryOn
	"��ǹ��ӡ����",									// M_ReceiptShort
	"��ǹ�����ӡ����",								// M_ReceiptLong
	
	"ȷ�ϼ��л���ӡ����",							// M_PrinterMsg
	"ȷ�ϼ��л�ѯ�ʿ���",							// M_InquiryMsg
	"ȷ�ϼ��л�СƱ����",							// M_ReceiptMsg
	
	" 0�ǿ�",										// M_NonCard
	" 1�ϰ�",										// M_LogOnTrans
	" 2�°�",										// M_LogOffTrans
	" 3�ͼ۽���",									// M_PriceRecv

	" 0����",										// M_TransOk
	" 1�ӿ�",										// M_TransGrey
	" 2����",										// M_TransUnlock
	" 3����",										// M_TransGreyFlag
	" 4��",										// M_ErrCard
	" 5����ܾ�",									// M_CardErrorTrans

	"��������",										// M_VolumeSetup
	"�ϼ�ͷ, ��������",								// M_VolumeUp
	"�¼�ͷ, ������С",								// M_VolumeDown

	"PSAM����Ч��",									// M_PSAMSetUp
	"��֤PSAM��Ч��",								// M_PSAMCheck
	"����֤PSAM��Ч��",								// M_PSAMCheckNot

	"ά������",										// M_MatnFunction
	"0δ�꽻��",									// M_UnfinishedTrans
	"1���ݹ���",									// M_RunningLog
	"2��ӡ��",										// M_LocalPrinter
	"3 POSP",										// M_SetPOSP
//	"4��ʼ��",										// M_FormatFlash
	"4����ά��",									// M_SpecialFunction
	"5�������",									// M_SoftwareUpgrade
    "6�ؽ�",                                        // M_RebuildProtected
#ifndef MAKE_CARD_MODE2    
    "7��Ч����",                                    // M_WorkManage
#endif

//Douzhq 20140609 Add , Menu About
	"7ϵͳ����",									// M_Manage
	"������",										// M_ManageFunction
	"0��Ч����",									// M_WorkManage2
#ifndef MAKE_CASH_CR
	"1��ǹ����",									// M_LockNozzleManage
#else 
	"0��ǹ����",									// M_LockNozzleManage
#endif

//Douzhq 20141010 Add , Safe Mode
//-------  Begin , Douzhq Add , Safe Mode , 20141010 -------
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	"5��ȫģʽ",									// M_SafeMode
#else 
	#ifdef MAKE_CASH_CR
		"2��ȫģʽ",								// M_SafeMode
	#else
		"2��ȫģʽ",								// M_SafeMode
	#endif
#endif

	"��ȫģʽ����",									// M_SetSafeMode
	"��ȫģʽ���ܿ���",								// M_SafeModeOn
	"��ȫģʽ���ܹر�",								// M_SafeModeOff
	"ȷ�ϼ��л���ȫģʽ",							// M_SelSafeMode
//-------  End   , Douzhq Add , Safe Mode , 20141010 -------

//Douzhq 20140819 Add , Car Number Recognize
//Car Number Recognize , Menu About
#ifdef MAKE_CASH_CR
	"1����ʶ��",									// M_CardNumberRecognize
	"����ʶ���ܿ���",								// M_CarNumberRecognizeOn
	"����ʶ���ܹر�",								// M_CarNumberRecognizeOff
	"ȷ�ϼ��л�����ʶ��",							// M_SelCarNumberRecognize
	"����ʶ������",									// M_SetCarNumberRecognize
#endif
	
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	"2��Ƭʶ��",									// M_CardRecognize
	"3���͹���",									// M_SaveOilManage
	"4��������",									// M_DiscountManage
#endif

//Douzhq 20140610 Add , Menu About
	"��ǹ����",										// M_SetLockNozzle
	"��ǹ�������ܿ���",								// M_LockNozzleOn
	"��ǹ�������ܹر�",								// M_LockNozzleOff
	"ȷ�ϼ��л���ǹ����",							// M_SelLockNozzle

//Douzhq 20140610 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	"�����ʶ������",								// M_SetCardRecoginze
	"0�����ʶ�𿪹�����",							// M_ICCardAppSwitch
	"�����ʶ����",								// M_CardRecognizeOn
	"�����ʶ��ر�",								// M_CardRecognizeOff
	"ȷ�ϼ��л�����ʶ��",							// M_SelCardRecognize
	
	"�����ʶ�𿪹�����",							// M_TitleCardRecognize
	"�����ʶ��ѡ��",								// M_TitleCardSwitch
	
	"1�����ʶ��ѡ��",								// M_SwitchCardRecognize
	"0��ʯ��",										// M_SinopecAppRecognize
	"1�»���",										// M_SocialAppRecognize
	"2���վ1",										// M_Other1AppRecognize
	"3���վ2",										// M_Other2AppRecognize
#endif

//Douzhq 20140610 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	"��������",										// M_SetSaveOilApp
	"����Ӧ�ÿ���",									// M_SaveOilAppOn
	"����Ӧ�ùر�",									// M_SaveOilAppOff
	"ȷ�ϼ��л�����Ӧ��",							// M_SelSaveOilApp
#endif

//Douzhq 20140619 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	"VIP�ȼ�����",									// M_SetVipFueling
	"VIP�ȼ�Ӧ�ÿ���",								// M_VipFuelingOn
	"VIP�ȼ�Ӧ�ùر�",								// M_VipFuelingOff
	"ȷ�ϼ��л�VIPӦ��",							// M_SelVipFueling
#endif

	"δ��ɽ���",									// M_UnfinishedTransSetUp
	"��δ��ɽ���",									// M_TransFinished
	"ԭ��:���ϱʽ���ʧ��",							// M_Reason0
	"ԭ��:PSAM������ʧ��",							// M_Reason1
	"ԭ��:д�뽻��ʧ��",							// M_Reason2

	"���ݹ�����",									// M_RunningLogSetup
	"�������ݹ�������:",							// M_UploadPWD,
	"�������",										// M_UploadPWDError,
	"���Ӻù������,",								// M_UploadSoftware,
	"Ȼ�󰴡�ȷ�ϡ���",								// M_UploadReady,
	"�ݲ�֧�ָù���",								// M_FunctionNotSupported

	"���ش�ӡ��",									// M_LocalPrinterSetUp

	"��ӡ���ڵ�ǰCR",								// M_PrinterCR,
	"��ӡ���ڶ���CR",								// M_PrinterCR_O,

//	"��ʼ������",									// M_FormatFlashFunction
	"����ά������",									// M_SpecialFunctionTitle
	"0����ۿ�����",								// M_PowerOnDebit
	"1�����������",								// M_BlackListSetup
	"2��ʽ��������",								// M_FormatTransFlash
	"3��ʽ��������",								// M_FormatConfigFlash

	"��ʽ��������",									// M_FormatConfigFunction
	"��ע��: ",										// M_Warning
	"�����������,��ɺ�",							// M_FormatWarning1
	"������������������",							// M_FormatWarning2
	"��ѡ�񡱼� ��ʽ��",							// M_FormatWarning3

	"��ʽ��������",									// M_FormatTransFunction
//	"�����������,��ɺ�",							// M_FormatWarning4
	"�������������¼",								// M_FormatWarning4
	"TTC��1��ʼ",									// M_FormatWarning5

	"����ۿ�����",									// M_DebitPowerOn
	"����ʱ��ҿۿ�",								// M_DebitPowerDown
	"�����γɻҼ�¼", 								// M_GreyPowerDown

	"�����������",									// M_BlackListPOSSetup
	"�Ȳ�ѯ��̨������",								// M_BlackListPOSFirst
	"�Ȳ�ѯ����������",								// M_BlackListLocalFirst

	"��ǰ����: ",									// M_CurrentSpecialFunc
	"��ѡ�񡱼�,�޸�",								// M_UpdateSpecialFunc

	"����POSP",										// M_SetUpPOSP

	"�������",										// M_UpgradeSW,
	"������ɺ�,",									// M_UpgradeSW0,
	"��ȷ�ϼ�,��������",							// M_UpgradeSW1,

//#ifndef MAKE_CARD_MODE2
    "��Ч����",                                     // M_WorkManageSetup,
    "��Ч����ǰ״̬��",                           // M_WorkManage_On,
    "��Ч����ǰ״̬��",                           // M_WorkManage_Off,
    "������Ա����",                                 // M_Emp_Number,
    "������1-254֮���ֵ",                          // M_Emp_Number_Out_Of_Range
//#endif
	
	"TTC: ",										// M_TTC
	"����: ",										// M_ASN
	"����/ʱ��: ",									// M_Time
	"��������: ",									// M_TransType
	"�����: ",										// M_Balance
	"���׽��: ",									// M_Amount
	"CTC: ",										// M_CTC
	"TAC: ",										// M_TAC
	"GMAC: ",										// M_GMAC
	"SAMTAC: ",										// M_SAM_TAC
	"SAMASN: ",										// M_SAM_ASN
	"PTID",											// M_SAM_TID
	"PTTC: ",										// M_SAM_TTC
	"�ۿ�: ",										// M_DS
	"����: ",										// M_UNIT
	"����: ",										// M_CardType
	"�汾: ",										// M_KeyVersion
	"ǹ��: ",										// M_NozzleNumber
	"��Ʒ����: ",									// M_Grade
	"��������: ",									// M_Volume
	"���׼۸�: ",									// M_Price
	"Ա��: ",										// M_EmployeeNumber
	"���ۼ�: ",										// M_VolumeTotalDisp
	"��֤��: ",										// M_TerminalTAC

	"����",											// M_TransTypeOk
	"�ӿ�",											// M_TransTypeGrey
	"��",											// M_TransTypeErrorCard
	"����",											// M_TransTypeGreyUnlock
	"����",											// M_TransTypeGreyFlag
	"�ϰ�",											// M_TransTypeLogOn
	"�°�",											// M_TransTypeLogOff
	"�ǿ�",											// M_TransTypeNonCard
	"�ͼ۽���",										// M_TransTypePriceRecv
	"����ܾ�",										// M_TransTypeCardErr

	"����",											// M_TransTypeRFU
	"�����쳣",										// M_TransTypeABN
	"�ǿ�����",										// M_TransTypeNonCardSS
	"��̨��Ȩ",										// M_TransTypeHostAuth
	"������Ȩ",										// M_TransTypeAuthCancel
	"���彻��",										// M_TransTypeOverFuel

	"���ϴ�",										// M_TransUploaded
	"δ�ϴ�",										// M_TransUnUploaded
	"�ѵ���",										// M_TransSaveToEMPCard
	"δ����",										// M_TransNotSaveToEMPCard


	"���ײ�ѯ",										// M_TransDisp
	"��ȷ�ϡ��� �鿴����",							// M_ENTER
	"�����ġ��� �˳�",								// M_CANCEL
	"��ѡ�񡱼� �������",							// M_SELECT

	"������TTC",									// M_InputTTC
	"ֱ�Ӱ���ȷ�ϡ���,",							// M_ENTER2
	"�鿴���һ�ʽ���",								// M_LastTrans
	
	"   ȷ��?    ",									// M_ENTER3
	"����ȷ�ϡ���",									// M_ENTER4

#ifdef MAKE_PSAM_VERIFY
	"PSAM��",										// M_V_PSAM
	"��ȫ��������ʧЧ",								// M_V_PSAM_NOT_SUPPORT
	"�����PSAM��",									// M_V_PSAM_CHANGE
	"оƬδע��",									// M_V_PUMP_NOT_REGISTERED
	"�����ACT��ע��",								// M_V_INSERT_ACT
	"��ȫ����",										// M_V_PSAM_LOCK
	"�����°�",									// M_V_RE_BINDING

	"�����ACT��",									// M_V_Insert_ACT
	"�����RID��",									// M_V_Insert_RID
	"��֤ʣ****�λ���",								// M_V_Certificate_Times
	"ȡ�����ʧ��",									// M_V_PSAM_RANDOM_Error
	"PSAM��ʼ��ʧ��",								// M_V_PSAM_Start_Binding_Error

	"ACT���󶨳ɹ�",								// M_V_PSAM_ACT_Ok
	"RID������ʧ��",								// M_V_RID_Error

	"����ע��,��ȴ�",								// M_V_Registering
	"��֤ʧ���ز���RID��",							// M_V_ReInsert_RID
	"��֤ʧ��",										// M_V_VerifyError

	"ACTδ����",									// M_V_ACT_Not_Ready
	"оƬע��ɹ�",									// M_V_RegisterOk
	"оƬע��ʧ��",									// M_V_RegisterError

	"��ȡPSAM��־��",								// M_V_Read_Flag_Error
#endif




	// Define the Error Message	For Approval
	"�Ǳ�վ����,�ܼ���",							// M_Test_Manage_Card0,
	"��վ����,���ܼ���",							// M_Test_Manage_Card1,
	"�Ǳ�վԱ����",									// M_Test_Not_EMP_Card,
	"�Ǳ�վ��ÿ�",									// M_Test_Not_Test_Card,
	"�Ǳ�վά�޿�",									// M_Test_Not_Maintain_Card,

	"��������Ч",								    // M_Test_CardUnValidate, IC Card Application Validate Time Format Error
	"��������Ч",								    // M_Test_CardExpired, IC Card Expired Time Format Error

	"��������ʶ��",									// M_Test_IssuerID_Err
	"Ӧ�����ͱ�ʶ��",								// M_Test_AppID_Err
	"Ӧ�ð汾��",									// M_Test_AppVer_Err
	"Ӧ�����кŴ�",									// M_Test_AppSN_Err
	"����Կ�汾��",									// M_Test_KeyVer_Err
	"���κŴ�",										// M_Test_Batch_Err
	"��ҵ�������",									// M_Test_Type_Err
	"�����ͱ����",									// M_Test_CardType_Err
	"ָ���",										// M_Test_Instruction_Err
	"�������ʹ�",									// M_Test_PINType_Err
	"�Ǳ�վ��",										// M_Test_CardLocation_Err
	// Define the Error Message For Approval End

	// Below Define The Pump Process Message,
	// Be Careful To Change It, According To Pump Message Enumration
	"    ",											// M_PumpMsg

	"      ",										// M_PumpMsgStart,
	"�ͼ۸�����",									// 1. M_PumpUpdatePrice,
	"������Ȩ��",									// 2. M_PumpAuth,
	"������,����ο�",								// 3. M_PumpFuelling,
	
//Douzhq 20140603 Add , Lock Nozzle 
	"��ǹ����",										//M_LockedNozzle,

	"      ",										// M_PumpMsgEnd
	"      ",										// M_PumpErrStart,

	"���ͻ�����",									// 101. M_PumpOffline,
#ifdef MAKE_CARD_MODE2
	"���ǹ",										// 102. M_PumpError,
#else
	"���ͻ�����",									// 102. M_PumpError,
#endif
	"��Ч̧ǹ,���ǹ",								// 103. M_PumpNozzleLiftError,
	"Ԥ��ֵ����",									// 104. M_PumpPresetExceed,
	"˰�ؾܸ��ͼ�",									// 105. M_PumpUpdatePriceError,
	"��Ȩʧ��",										// 106. M_PumpAuthError,
	"ȡ�ͻ�����ʧ��",								// 107. M_PumpLastTransError,
	"ǹ�Ŵ���",										// 108. M_PumpNozzleNumberError,
	"ȡ�ѻ�����ʧ��",								// 109. M_PumpOfflineTransError,
	"ȡ���ͻ�ʱ��ʧ��",								// 110. M_PumpTimeError,
	"���ͻ��ͼ۲���",								// 111. M_PumpPriceError,
	"���ͻ����ò���",								// 112. M_PumpConfigureError,
#ifdef MAKE_CARD_MODE2
	".             ",								// 113. M_PumpStateError,
#else
	"���ͻ�����״̬����",							// 113. M_PumpStateError,
#endif
	"CRͨѶ��ַ�ص�",								// 114. M_Pump485AddrError,
	"ͨѶ����",										// 115. M_PumpCRCommError,
	"�ͼ۱����",									// 116. M_PumpPPUError,
	"���ͻ���֤ʧ��",								// 117. M_PumpCertificateError,

	"      ",										// M_PumpErrEnd,
	"      ",										// M_PumpPumpErrorStart,

	"δ��⵽���巢����",							// 201. M_PumpErr1,
	"EMT������",									// 202. M_PumpErr2,
	"���ͻ���Ӳ������λ",							// 203. M_PumpErr3,
	"�ϵ��⵽��ǹδ��",							// 204. M_PumpErr4,
	"Ԥ�ù���",										// 205. M_PumpErr5,
	"Ԥ�ù�С",										// 206. M_PumpErr6,
	"���ͽ���δ��ǹ",								// 207. M_PumpErr7,
	"������˰��ͨ��Υ��",							// 208. M_PumpErr8,

	"      ",										// M_PumpPumpErrorEnd,

    "�ռ䲻��,�������ϴ�",                           // M_UploadFirst	//20131126 qiyong modify for transspace limit

	"    ",											// M_Max, Card Reader Message End

};


// MsgDelayFlag: 0: No Message Is In Delay Time
// 1: Current Message Is In Delay Time
unsigned char MsgDelayFlag = 0;
// Delay Time Timer
Tmr * MsgTimerDelayIndex;


enum DISPMSGID MsgID;
enum DISPMSGID NextMsgID;


/*
 ** DispInfoDelayTimeOut
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Message.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Info Line Display Time Out CallBack
 *
 *  RETURNS: None.
 *
 */
void DispInfoDelayTimeOut( void )
{
	// Clear Display Message Flag
//	MsgDelayFlag = 0;

	// Do Not Display Pump Info
	NextPumpMsgID = M_DefaultInfo;
	PumpMsgID = M_DefaultInfo;

	// Clear InfoLine, And Display default
//	NextMsgID = M_DefaultInfo;
	FillInfoMsg( M_DefaultInfo, 0, 0 );

	// Display It
//	DispInfoLine();
}

/*
 ** FillInfoMsg
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Message.c
 *
 *  PARAMETERS:	enum DISPMSGID DispMsgID: The Display Message ID
 *				DelayTime: The InfoLine Message Remain On Display Time( In Seconds )
 *				0: Do Not Remain, Replace By The Last Info Message At Any Time
 *				DispFlag: 0: Did Not Display The Message, If Other Info Message Is In Delay Time
 *						  1: Must Display, Even The Other Message Is In Delay Time
 *				
 *
 *  DESCRIPTION: Save The Message To Be Displayed On InfoLine
 *
 *  RETURNS: None.
 *
 */
void FillInfoMsg( enum DISPMSGID DispMsgID, unsigned char DelayTime, unsigned char DispFlag )
{
	// Set MessageId
	NextMsgID = DispMsgID;

	// Display It
	if(( IdleFlg & DISP_CARDLOGO ) || ( IdleFlg & DISP_TIME ))
	{
		// Only Display When Info Line Is Displayed
		// Display Info Line
		DispInfoLine();
	}
	else
	{
		// Do Not Display Info Line
		StopDefaultInfo();
	}
}

///*
// ** DispDefaultInfoLine
// *
// *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Display.c
// *
// *  PARAMETERS:	None.
// *
// *  DESCRIPTION: Display The Default Info Line, While Screen Is Card Insert Or Card Balance
// *
// *  RETURNS: None.
// *
// */
//void DispDefaultInfoLine( void )
//{
//	if( MsgDelayFlag == 0 )
//	{
//		if(( ScreenID == DEFAULT_SCR_CARD ) || ( ScreenID == DEFAULT_SCR_BALANCE ))
//		{
//			// Only Update While Card Insert Or Balance Screen
//			if( MsgID != M_DefaultInfo )
//			{
//				// Display 4 Seconds, And Update To Default Info Line
//				CreateTimer( TIMER_ONE_DELETE, 9 * DispDelayTime1Second, DispInfoDelayTimeOut );
//			}
//			else
//			{
//				// Current Msg Is Default Info Line, Do Not Update, Go On....
//			}
//		}
//		else
//		{
//			// Do Not Update, Go On....
//		}
//	}
//	else
//	{
//		// A Message Is Displayed, Do Not Update Default Info Line
//		// Until TimeOut
//		// Go On....
//	}
//}

/*
 ** SetDispDefaultInfo
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Message.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Creat A Timer To Display The Default Info Line, Call In InitDisplayProcess()
 *
 *  RETURNS: None.
 *
 */
void SetDispDefaultInfo( void )
{
	// Creat Delay Timer For Default Info Line, Delay 9Second To Display The Default Info Line
//	MsgTimerDelayIndex = CreateTimer( TIMER_ONE, 4 * DispDelayTime100ms, DispInfoDelayTimeOut );
	MsgTimerDelayIndex = CreateTimer( TIMER_ONE, 9 * DispDelayTime1Second, DispInfoDelayTimeOut );
}

/*
 ** StopDefaultInfo
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Message.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Stop Display Default Info Line Timer
 *
 *  RETURNS: None.
 *
 */
void StopDefaultInfo( void )
{
	// Stop The Timer
	StopTimer( MsgTimerDelayIndex );
}

/*
 ** ResetDefaultInfo
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Display\Source\Message.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Reset The Display Default Info Line Timer
 *
 *  RETURNS: None.
 *
 */
void ResetDefaultInfo( void )
{
	if(( IdleFlg & DISP_CARDLOGO ) || ( IdleFlg & DISP_TIME ))
	{
		if( NextMsgID != M_DefaultInfo )
		{
			// Reset The Timer
			ResetTimer( MsgTimerDelayIndex );
		}
	}
}