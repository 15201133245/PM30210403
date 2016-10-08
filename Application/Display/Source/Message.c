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

	"版本信息",										// M_VersionTitle
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


	"欢迎使用长吉系统",								// M_DefaultInfo, Default Info Line Display
	"请稍候,正在处理中",							// M_Wait
	"更新油价",										// M_PriceUpdate,
	"请勿插入IC卡",									// M_CardNotAllowed,


	"没有IC卡插入",									// M_CARDIN, No IC Card In Reader
	"IC卡准备好",									// M_CARDOK, IC Card Is Ready For Fuelling
	"加油中....",									// M_FuellingCash, Fuelling For Cash Trans

	"读卡器复位",									// M_CardReaderReset, Reset The Card Reader
	"读卡器复位成功",								// M_CRResetOk, Card Reader Reset Ok
	"读卡器复位错误",								// M_CRResetErr, Card Reader Reset Error
	
	
	"正在连接油机",									// M_WaitPumpOnline,
	"读取油机总累",									// M_ReadPumpTotal, Read Pump Total Message
	"读取油机总累失败",								// M_ReadPumpTotalFail, Read The Pump Totals Fail

	"PSAM复位错误",									// M_PSAMResetError, PSAM Reset Error
	"PSAM选择PSE错误",								// M_PSAMSelectPSEErr, PSAM Select PSE Error
	"PSAM选择应用1错误",							// M_PSAMSelectAPP1Err,	PSAM Select Sinopec1 Error
	"PSAM选择应用2错误",							// M_PSAMSelectAPP2Err,	PSAM Select Sinopec2 Error
	"PSAM卡应用未启用",								// M_PSAMUnValidate, PSAM Card Application Is Un Validate
	"PSAM卡应用过期",								// M_PSAMExpired, PSAM Card Expired

	"生成灰记录",									// M_GreyTrans, Grey Card, Wait Unlock Debit
	" IC卡上电失败",								// M_CardPowerOnErr, IC Card Power On Error
	"IC卡可能插反了",								// M_CardInverted, IC Card Inverted Maybe
	"IC卡选择应用错误",								// M_CardSelectAppErr, IC Card Select Application Error
	"管理卡不能加油",								// M_ManageCardLimit, Manage Card Do Not Fuelling
	"卡未启用",									    // M_CardUnValidate, IC Card Application Is Un Validate
//	"卡应用过期",									// M_CardExpired, IC Card Expired
	"失效卡",										// M_CardExpired, IC Card Expired
	"卡应用锁定",									// M_CardAppBlocked, IC Card Application Was Blocked
//	"异地卡不能选择应用",							// M_NotLocalCard, IC Card Select Application Error
	"不能选择应用",									// M_NotLocalCard, IC Card Select Application Error
	"挂失卡",										// M_CardBlackList, IC Card In Black List
	"查询本机黑名单",								// M_LocalBlackList, Check Local Black List
	"联机查询黑名单",								// M_POSBlackList, Check POS Black List
	"需联机查询黑名单",								// M_NeedPOSBlackList, Need Check Black List On POS, But POS Is Offline
	"员工密码错误",									// M_EmpPwdErr, Employee Password Error
	"非签到员工卡",									// M_NotSameEMPCard, Employee Card Is Not Same With The LogOn Card Number
	"员工号不同",									// M_NotSameEMP, Employee Card Number Is Not Same With The LogOn Card Number
	"非本站白名单卡",								// M_NotWhiteList, Not Belong To White List
	"查询本机白名单",								// M_LocalWhiteList, Check Local White List
	"联机查询白名单",								// M_POSWhiteList, Check POS White List
	"需联机查询白名单",								// M_NeedPOSWhiteList, Need Check White List On POS
//Douzhq 20140610 Add , IC Card Recognisze
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	"非本系统卡",									// M_CardError, IC Card Error
#else
	"非加油卡",										// M_CardError, IC Card Error
#endif
	"IC卡密码锁定",									// M_CardPINBlocked, Card PIN Was Blocked
	"IC卡密码校验失败",								// M_CardPINError, Card PIN Error
	"灰卡,解灰中",									// M_CardGreyDebit, Grey Card, Waiting Unlock Debit
	"需要联机查灰记录",								// M_NeedCheckGreyOnPOS
	"无此灰纪录",									// M_NoGreyRecord

	"需联机对时",									// M_NeedCheckTimeOnPOS, POS IS Offline While 
													// Pump Time Is Earlier Then The Last Transaction Time

	"密码错,还有*次机会",							// M_PINRetries, Pin Error, * Times Retry Left
	"员工密码错,重新输入",							// M_EMPPwdRetries, Employee Password Error, Re-Input Please
    "加油机过冲！",                                 // M_OverFueling

	"读余额失败",									// M_GetBalanceError,
	"读取灰锁状态失败",								// M_GetLockProofError,

	"加油中,请勿拔卡",								// M_Fuelling, IC Card Is Fuelling, Do Not Sping Out Card
	"加油中拔卡,请挂枪",							// M_FuellingCardOut, IC Card Is Fuelling But Spring Out IC Card
	"预置值过大",									// M_PresetTooMuch, The KeyPreset Is Larger Then Permitted.
	"预置值过小",									// M_PresetTooSmall, The KeyPreset Is Too Small < 1L Or Price.
	"余额不足",										// M_BalanceTooSmall, The Balance Is Too Small

	"保存交易",										// M_WriteRecord,
	"保存交易失败",									// M_WriteRecordError,

	"IC无效抬枪,请挂枪",							// M_NozzleOnError
	
	"记录将满,请联机上传",							// M_RecordMax

	"系统掉电....",									// M_SysPowerDown
	"弹卡",											// M_CardSpringOut
	"读卡出错",										// M_CardReadError
	"油机离线",										// M_FuellingPumpOffline
	"油枪号不同",									// M_NozzleNumberError
	"读取上笔交易",									// M_ReadLastTrans
	"Certificate PASM",								// M_CertificatePSAM
	"Certific PASM Error",							// M_CertificatePSAMErr
	"取GMAC",										// M_GetGMAC
	"取GMAC出错",									// M_GetGMACErr
	"Credit PSAM",									// M_CreditPSAM
	"Credit PSAM Error",							// M_CreditPSAMErr
	"IC卡解灰扣款",									// M_CardUnlock
	"无法解扣",										// M_CardUnlockErr
	"IC卡清灰标",									// M_CardClearGreyFlag
	"IC卡清灰标出错",								// M_CardClearGreyFlagErr


	"黑名单卡",										// M_MsgBlackListCard, The IC Card Is In Black List
//	"失效卡",										// M_MsgExpired, The Card Either Not Validate Or Expired
	"用户卡,查限",									// M_CustomerCardCheckLimited
	"限油品卡",										// M_MsgBrandLimited, The Card Is Brand Limited
//	"限油品卡超限加油",								// M_MsgBrandLimitedExceed, 
	"限油品卡",										// M_MsgBrandLimitedExceed, 
	"限地市卡",										// M_MsgLocationLimited,
	"限地市卡超限加油",								// M_MsgLocationLimitedExceed,
	"限每日加油次数卡",								// M_MsgTimesLimited,
//	"限次数卡超限加油",								// M_MsgTimesLimitedExceed,
	"限日加油次数卡",								// M_MsgTimesLimitedExceed,
	"限加油金额卡",									// M_MsgAmountLimited,
	"限日加油金额卡",								// M_MsgAmountLimitedExceed,
	"限车号卡",										// M_MsgVehicleNumberLimited,
	"查限制加油信息",								// M_FuellingLimited
	
//Douzhq 20140805 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
	"加油车号相符",									// M_CarNumMatch
	"加油车号不符",									// M_CarNumNoMatch
	"车号识别中...",								// M_CarNumMatchRecing
	"车号识别超时",									// M_CarNumRecTimeout
	
	"油品核对中...",								// M_GradeLimitMatching
	
	"车号识别中，",									// M_CarNumberRecing
	"油品核对中，",									// M_GradeLimitRecing
	"请稍候...",									// M_Waiting	
#endif
	
	"限省卡",										// M_ProvinceLimited,
//#ifdef MAKE_CARD_MODE2
//	"限省卡",										// M_CityLimited,
//#else
	"限地市卡",										// M_CityLimited,
//#endif	// MAKE_CARD_MODE2
	"限加油站卡",									// M_OilStationLimited,
	"限上级单位",									// M_SuperiorUnitLimited,
	"单次加油金额限制",								// M_PerAmountLimited,
	"单次加油升数限制",								// M_PerVolumeLimited,
	"总加油金额限制",								// M_TotalAmountLimited,
	"限单次金额",									// M_PerAmountLimited_D,
	"限单次升数",									// M_PerVolumeLimited_D,
	"限日总金额",									// M_TotalAmountLimited_D,


	"IC卡初始化灰锁",								// M_CardInitGreyLock
	"IC卡初始化灰锁失败",							// M_CardInitGreyLockErr
	"PSAM初始化灰锁",								// M_PSAMInitGreyLock
	"PSAM初始化灰锁失败",							// M_PSAMInitGreyLockErr
	"IC卡置灰",										// M_CardSetGreyLock
	"IC卡置灰失败",									// M_CardSetGreyLockErr
	"IC卡授权完成",									// M_CardAuthPump



	"选择支付方式",									// M_SelectPayMode, Select The Pay Mode
													// 0: Cash ( xian jin ); 1: Oil Ticket ( you piao ); 2: Ticket ( ti you ping zheng );
													// 3: CreditCard ( xin yong ka ); 4: Other1 ( qi ta 1 ); 5: Other2 ( qi ta 2 );
	"0.现金",										// M_Cash
	"1.油票",										// M_OilTicket
	"2.提油凭证",									// M_Ticket
	"3.银行卡",										// M_CreditCard
	"4.其它1",										// M_Other1
	"5.其它2",										// M_Other2

	"选择IC卡应用",									// M_SelectApp
//	" 0. 中石化加油应用",							// M_SinopecApp
//	" 1. 中石化积分应用",							// M_LoyaltyApp
//	" 1. 中石化加油应用",							// M_SinopecApp
//	" 2. 中石化积分应用",							// M_LoyaltyApp
	" 1. 电子油票",									// M_SinopecApp
	" 2. 积分应用",									// M_LoyaltyApp
	
//Douzhq 20140523 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	" 3. 存油应用",									// M_SaveoilApp
	"卡片存油应用错误",								// M_SaveoilError
#endif

	" 无启用记录",									// M_NeedRecord

	"请输入IC卡密码",								// M_CARD_PIN
	"请输入员工密码",								// M_EMP_PWD
	"请输入维护密码",								// M_MAINTAIN_PWD
	"请输入设置密码",								// M_FUNCTION_PWD
	
//Douzhq 20140528 Add , Lock Nozzle 
	"请输入解锁密码",								//M_UNLOCK_PWD
	"解锁密码错误,请重试",							//M_UNLOCK_RETRY

	"设置密码错误,请重试",							// M_FUNCPWD_RETRY
	"维护密码错误,请重试",							// M_MATNPWD_RETRY
	"请输入格式化密码",								// M_FORMAT_PWD
	"格式化密码错,请重试",							// M_FORMATPWD_RETRY

	"错误,暂时停用",								// M_ERROR_STOP
	"已有IC卡插入",									// M_CardInserted
	//Douzhq 20141023 Add , For Bug
	"无效操作",										// M_InvalidOperation
	"控制卡错误,请重试",							// M_CashCardRetry
	"金额未完成,请挂枪",							// M_NozzleOffCashError
	"油品错误,请挂枪",								// M_NozzleError
	"  请输入授权码",								// M_InputAuthCode
	"正在申请授权,请稍候",							// M_AuthWaiting
	"员工卡禁止加油",								// M_EmployeeCardNotForCash,


	"功    能",										// M_Function, Display Function Screen
	"0.版本信息",									// M_Version
	"1.查询",										// M_Inquire
	"2.设置",										// M_Setup
	"3.维护",										// M_Maintain
	"4.上班",										// M_Logon
	"5.下班",										// M_Logoff
	"6.在线离线",									// M_WorkMode
	"7.用户卡",										// M_CustomerCardSpringOutOrNot
	
	"在线/离线设定",								// M_WorkModeSet
	"当前模式: ",									// M_CurrentWorkMode
	"离线模式",										// M_WorkOffline
	"在线模式",										// M_WorkOnline
	"“选择”键,修改",								// M_WorkModeSwitch
	"“更改”键,退出",								// M_WorkModeExit
	"请插入维修卡",									// M_InsertMaintainCard
	"计算MAC出错",									// M_CalcMACError
	"验证MAC出错",									// M_VerifyMACError
	"按任意键,继续",								// M_PressKey
	"后台授权禁用员工卡",							// M_EMPCardLimited,
	"正在申请授权....",								// M_WaitAuth,
	"申请授权完成",									// M_AuthEnd,
	"正在提交加油数据",								// M_WaitFuellingData,
	"授权超时,请重试",								// M_AuthTimeOut,
	"通讯超时,交易结束",							// M_TimeOut,
	"正在上传IC卡记录",								// M_UploadRecord,
	"上传IC卡记录完成",								// M_UploadRecordEnd,
	"后台授权",										// M_HostAuth,
	"后台撤销授权",									// M_HostAuthCancel,
	"停止加油,请挂枪",								// M_HostStop,
	"后台取消授权",									// M_HostCancel,
	"授权完成,不能预置",							// M_AuthedNotPreset,
	"已有卡插入,不能执行",							// M_CardInNotHostAuth,
	"离线模式,不能执行",							// M_OfflineNotPreset,
	"员工未上班不能执行",							// M_OffDutyNotPreset,
	"非后台授权不能执行",							// M_MustBeHostAuth,
	
	"导出第****条交易",								// M_RecordToEMPCard,
	"共导出****条交易",								// M_RecordToEMPCardTotal,
	"记录满,请去后台处理",							// M_RecordToEMPCardFull,
	"出错,请去后台处理",							// M_RecordToEMPCardError,
	

	"非员工卡,不能执行",							// M_NotEMPCard
	"员工已上班",									// M_LogOned
	"员工未上班",									// M_LogOffed
	"没有卡插入",									// M_InsertCard

	"查询功能",										// M_InquireFunc
	"0油站信息",									// M_Station
	"1记录空间",									// M_RecordSpace
	"2交易记录",									// M_Record
	"3交易总累",									// M_Total
	"4查询单价",									// M_PriceInq

	"油站信息",										// M_StationFunc
	"省: ",											// M_Province
	"市: ",											// M_City
	"上级: ",										// M_Superior
	"站号: ",										// M_StationID
	"POSP: ",										// M_POSP

	"查询记录空间",									// M_RecordSpaceFunc
	"记录空间: ",									// M_RecordSpaceNum
	"未上送记录: ",									// M_RecordNeedUpload

	"升 总累计",									// M_VolumeTotal
	"升 班累计",									// M_VolumeTotalLogOn
	"金额 总累计",									// M_AmountTotal
	"金额 班累计",									// M_AmountTotalLogOn

#ifdef MAKE_CASH_CR
	"现金交易",										// M_CashAcceptorTrans
	"授权交易",										// M_CashPOSTrans

	"余额: ",										// M_CashBalance
	"授权码:",										// M_CashPOSAuthCode
	"收钞总额: ",									// M_CashAcceptorTotal
	"授权总额: ",									// M_CashPOSTotal

	"升 班累计 (现金)",								// M_CashVolumeTotalLogOn
	"金额 班累计 (现金)",							// M_CashAmountTotalLogOn
#endif

	"单价查询",										// M_PriceInqiry

	"设置功能",										// M_SetUpFunc
	"0. 背光开/关时间",								// M_BackLightTime
	"1. 打印设置",									// M_SetUpPrint
	"2. 音量设置",									// M_SetUpVolume
	"3. PSAM卡有效期",								// M_SetUpPSAMCheck

	"设置背光开/关时间",							// M_SetBackLightTime
	"当前开/关时间:",								// M_CurrentBackLightTime
	"新开/关时间:  ",								// M_NewBackLightTime
	"错误,时间应小于24",							// M_TimeInputError
	"开时间应晚于关时间",							// M_TimeLate


	"打印功能设置",									// M_PrintSetUp
	"0用户卡",										// M_CustomerCard
	"1员工卡",										// M_EMPCard
	"2验泵卡",										// M_TestCard
	"3维修卡",										// M_MatnCard
	"4其它交易",									// M_PubTrans
	//Douzhq 20140919 Add , Config Printer Set
	"5扩展设置",									// M_ExpandSetUp
	
	"用户卡打印设置",								// M_CustomerCardPrint
	"员工卡打印设置",								// M_EMPCardPrint
	"验泵卡打印设置",								// M_TestCardPrint
	"维修卡打印设置",								// M_MatnCardPrint
	"其他交易打印设置",								// M_PubTransPrint
	//Douzhq 20140919 Add , Config Printer Set
	"扩展打印设置",									// M_ExpandSetPrint
	
	"0打印开关",									// M_PrinterSwitch
	"1询问开关",									// M_InquirySwitch
	"2小票联数",									// M_ReceiptNumber
	
	"打印开关设置",									// M_PrinterSwitchSet
	"询问开关设置",									// M_InquirySwitchSet
	"小票联数设置",									// M_ReceiptNumberSet
	"打印功能打开",									// M_PrinterFeatureOn
	"打印功能关闭",									// M_PrinterFeatureOff
	"打印前不询问",									// M_InquiryOff
	"打印前询问",									// M_InquiryOn
	"挂枪打印短联",									// M_ReceiptShort
	"挂枪常规打印两联",								// M_ReceiptLong
	
	"确认键切换打印开关",							// M_PrinterMsg
	"确认键切换询问开关",							// M_InquiryMsg
	"确认键切换小票联数",							// M_ReceiptMsg
	
	" 0非卡",										// M_NonCard
	" 1上班",										// M_LogOnTrans
	" 2下班",										// M_LogOffTrans
	" 3油价接受",									// M_PriceRecv

	" 0正常",										// M_TransOk
	" 1逃卡",										// M_TransGrey
	" 2补扣",										// M_TransUnlock
	" 3补充",										// M_TransGreyFlag
	" 4错卡",										// M_ErrCard
	" 5卡错拒绝",									// M_CardErrorTrans

	"音量设置",										// M_VolumeSetup
	"上箭头, 音量增加",								// M_VolumeUp
	"下箭头, 音量减小",								// M_VolumeDown

	"PSAM卡有效期",									// M_PSAMSetUp
	"验证PSAM有效期",								// M_PSAMCheck
	"不验证PSAM有效期",								// M_PSAMCheckNot

	"维护功能",										// M_MatnFunction
	"0未完交易",									// M_UnfinishedTrans
	"1数据管理",									// M_RunningLog
	"2打印机",										// M_LocalPrinter
	"3 POSP",										// M_SetPOSP
//	"4初始化",										// M_FormatFlash
	"4设置维护",									// M_SpecialFunction
	"5软件升级",									// M_SoftwareUpgrade
    "6重建",                                        // M_RebuildProtected
#ifndef MAKE_CARD_MODE2    
    "7绩效管理",                                    // M_WorkManage
#endif

//Douzhq 20140609 Add , Menu About
	"7系统管理",									// M_Manage
	"管理功能",										// M_ManageFunction
	"0绩效管理",									// M_WorkManage2
#ifndef MAKE_CASH_CR
	"1锁枪管理",									// M_LockNozzleManage
#else 
	"0锁枪管理",									// M_LockNozzleManage
#endif

//Douzhq 20141010 Add , Safe Mode
//-------  Begin , Douzhq Add , Safe Mode , 20141010 -------
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	"5安全模式",									// M_SafeMode
#else 
	#ifdef MAKE_CASH_CR
		"2安全模式",								// M_SafeMode
	#else
		"2安全模式",								// M_SafeMode
	#endif
#endif

	"安全模式设置",									// M_SetSafeMode
	"安全模式功能开启",								// M_SafeModeOn
	"安全模式功能关闭",								// M_SafeModeOff
	"确认键切换安全模式",							// M_SelSafeMode
//-------  End   , Douzhq Add , Safe Mode , 20141010 -------

//Douzhq 20140819 Add , Car Number Recognize
//Car Number Recognize , Menu About
#ifdef MAKE_CASH_CR
	"1车号识别",									// M_CardNumberRecognize
	"车号识别功能开启",								// M_CarNumberRecognizeOn
	"车号识别功能关闭",								// M_CarNumberRecognizeOff
	"确认键切换车号识别",							// M_SelCarNumberRecognize
	"车号识别设置",									// M_SetCarNumberRecognize
#endif
	
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	"2卡片识别",									// M_CardRecognize
	"3存油管理",									// M_SaveOilManage
	"4明扣明折",									// M_DiscountManage
#endif

//Douzhq 20140610 Add , Menu About
	"锁枪设置",										// M_SetLockNozzle
	"油枪锁定功能开启",								// M_LockNozzleOn
	"油枪锁定功能关闭",								// M_LockNozzleOff
	"确认键切换锁枪功能",							// M_SelLockNozzle

//Douzhq 20140610 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	"卡类别识别设置",								// M_SetCardRecoginze
	"0卡类别识别开关设置",							// M_ICCardAppSwitch
	"卡类别识别开启",								// M_CardRecognizeOn
	"卡类别识别关闭",								// M_CardRecognizeOff
	"确认键切换卡类识别",							// M_SelCardRecognize
	
	"卡类别识别开关设置",							// M_TitleCardRecognize
	"卡类别识别选项",								// M_TitleCardSwitch
	
	"1卡类别识别选项",								// M_SwitchCardRecognize
	"0中石化",										// M_SinopecAppRecognize
	"1新华联",										// M_SocialAppRecognize
	"2社会站1",										// M_Other1AppRecognize
	"3社会站2",										// M_Other2AppRecognize
#endif

//Douzhq 20140610 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	"存油设置",										// M_SetSaveOilApp
	"存油应用开启",									// M_SaveOilAppOn
	"存油应用关闭",									// M_SaveOilAppOff
	"确认键切换存油应用",							// M_SelSaveOilApp
#endif

//Douzhq 20140619 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	"VIP等级设置",									// M_SetVipFueling
	"VIP等级应用开启",								// M_VipFuelingOn
	"VIP等级应用关闭",								// M_VipFuelingOff
	"确认键切换VIP应用",							// M_SelVipFueling
#endif

	"未完成交易",									// M_UnfinishedTransSetUp
	"无未完成交易",									// M_TransFinished
	"原因:读上笔交易失败",							// M_Reason0
	"原因:PSAM卡操作失败",							// M_Reason1
	"原因:写入交易失败",							// M_Reason2

	"数据管理功能",									// M_RunningLogSetup
	"输入数据管理密码:",							// M_UploadPWD,
	"密码错误",										// M_UploadPWDError,
	"连接好管理软件,",								// M_UploadSoftware,
	"然后按“确认”键",								// M_UploadReady,
	"暂不支持该功能",								// M_FunctionNotSupported

	"本地打印机",									// M_LocalPrinterSetUp

	"打印机在当前CR",								// M_PrinterCR,
	"打印机在对面CR",								// M_PrinterCR_O,

//	"初始化功能",									// M_FormatFlashFunction
	"设置维护功能",									// M_SpecialFunctionTitle
	"0掉电扣款设置",								// M_PowerOnDebit
	"1查黑名单设置",								// M_BlackListSetup
	"2格式化交易区",								// M_FormatTransFlash
	"3格式化配置区",								// M_FormatConfigFlash

	"格式化配置区",									// M_FormatConfigFunction
	"请注意: ",										// M_Warning
	"将清除配置区,完成后",							// M_FormatWarning1
	"需联机重新下载配置",							// M_FormatWarning2
	"“选择”键 格式化",							// M_FormatWarning3

	"格式化交易区",									// M_FormatTransFunction
//	"将清除交易区,完成后",							// M_FormatWarning4
	"将清除交易区记录",								// M_FormatWarning4
	"TTC从1开始",									// M_FormatWarning5

	"掉电扣款设置",									// M_DebitPowerOn
	"掉电时解灰扣款",								// M_DebitPowerDown
	"掉电形成灰记录", 								// M_GreyPowerDown

	"查黑名单设置",									// M_BlackListPOSSetup
	"先查询后台黑名单",								// M_BlackListPOSFirst
	"先查询本机黑名单",								// M_BlackListLocalFirst

	"当前设置: ",									// M_CurrentSpecialFunc
	"“选择”键,修改",								// M_UpdateSpecialFunc

	"设置POSP",										// M_SetUpPOSP

	"软件升级",										// M_UpgradeSW,
	"升级完成后,",									// M_UpgradeSW0,
	"按确认键,重新启动",							// M_UpgradeSW1,

//#ifndef MAKE_CARD_MODE2
    "绩效管理",                                     // M_WorkManageSetup,
    "绩效管理当前状态开",                           // M_WorkManage_On,
    "绩效管理当前状态关",                           // M_WorkManage_Off,
    "请输入员工号",                                 // M_Emp_Number,
    "请输入1-254之间的值",                          // M_Emp_Number_Out_Of_Range
//#endif
	
	"TTC: ",										// M_TTC
	"卡号: ",										// M_ASN
	"日期/时间: ",									// M_Time
	"交易类型: ",									// M_TransType
	"卡余额: ",										// M_Balance
	"交易金额: ",									// M_Amount
	"CTC: ",										// M_CTC
	"TAC: ",										// M_TAC
	"GMAC: ",										// M_GMAC
	"SAMTAC: ",										// M_SAM_TAC
	"SAMASN: ",										// M_SAM_ASN
	"PTID",											// M_SAM_TID
	"PTTC: ",										// M_SAM_TTC
	"扣款: ",										// M_DS
	"结算: ",										// M_UNIT
	"卡类: ",										// M_CardType
	"版本: ",										// M_KeyVersion
	"枪号: ",										// M_NozzleNumber
	"油品代码: ",									// M_Grade
	"交易升数: ",									// M_Volume
	"交易价格: ",									// M_Price
	"员工: ",										// M_EmployeeNumber
	"升累计: ",										// M_VolumeTotalDisp
	"认证码: ",										// M_TerminalTAC

	"正常",											// M_TransTypeOk
	"逃卡",											// M_TransTypeGrey
	"错卡",											// M_TransTypeErrorCard
	"补扣",											// M_TransTypeGreyUnlock
	"补充",											// M_TransTypeGreyFlag
	"上班",											// M_TransTypeLogOn
	"下班",											// M_TransTypeLogOff
	"非卡",											// M_TransTypeNonCard
	"油价接受",										// M_TransTypePriceRecv
	"卡错拒绝",										// M_TransTypeCardErr

	"备用",											// M_TransTypeRFU
	"在线异常",										// M_TransTypeABN
	"非卡自助",										// M_TransTypeNonCardSS
	"后台授权",										// M_TransTypeHostAuth
	"撤销授权",										// M_TransTypeAuthCancel
	"过冲交易",										// M_TransTypeOverFuel

	"已上传",										// M_TransUploaded
	"未上传",										// M_TransUnUploaded
	"已导出",										// M_TransSaveToEMPCard
	"未导出",										// M_TransNotSaveToEMPCard


	"交易查询",										// M_TransDisp
	"“确认”键 查看交易",							// M_ENTER
	"“更改”键 退出",								// M_CANCEL
	"“选择”键 清除交易",							// M_SELECT

	"请输入TTC",									// M_InputTTC
	"直接按“确认”键,",							// M_ENTER2
	"查看最后一笔交易",								// M_LastTrans
	
	"   确认?    ",									// M_ENTER3
	"按“确认”键",									// M_ENTER4

#ifdef MAKE_PSAM_VERIFY
	"PSAM卡",										// M_V_PSAM
	"安全提升功能失效",								// M_V_PSAM_NOT_SUPPORT
	"请更换PSAM卡",									// M_V_PSAM_CHANGE
	"芯片未注册",									// M_V_PUMP_NOT_REGISTERED
	"请插入ACT卡注册",								// M_V_INSERT_ACT
	"安全锁定",										// M_V_PSAM_LOCK
	"或重新绑定",									// M_V_RE_BINDING

	"请插入ACT卡",									// M_V_Insert_ACT
	"请插入RID卡",									// M_V_Insert_RID
	"验证剩****次机会",								// M_V_Certificate_Times
	"取随机数失败",									// M_V_PSAM_RANDOM_Error
	"PSAM开始绑定失败",								// M_V_PSAM_Start_Binding_Error

	"ACT卡绑定成功",								// M_V_PSAM_ACT_Ok
	"RID卡操作失败",								// M_V_RID_Error

	"正在注册,请等待",								// M_V_Registering
	"验证失败重插入RID卡",							// M_V_ReInsert_RID
	"验证失败",										// M_V_VerifyError

	"ACT未启动",									// M_V_ACT_Not_Ready
	"芯片注册成功",									// M_V_RegisterOk
	"芯片注册失败",									// M_V_RegisterError

	"读取PSAM标志错",								// M_V_Read_Flag_Error
#endif




	// Define the Error Message	For Approval
	"非本站管理卡,拒加油",							// M_Test_Manage_Card0,
	"本站管理卡,不能加油",							// M_Test_Manage_Card1,
	"非本站员工卡",									// M_Test_Not_EMP_Card,
	"非本站验泵卡",									// M_Test_Not_Test_Card,
	"非本站维修卡",									// M_Test_Not_Maintain_Card,

	"卡日期无效",								    // M_Test_CardUnValidate, IC Card Application Validate Time Format Error
	"卡日期无效",								    // M_Test_CardExpired, IC Card Expired Time Format Error

	"发卡方标识错",									// M_Test_IssuerID_Err
	"应用类型标识错",								// M_Test_AppID_Err
	"应用版本错",									// M_Test_AppVer_Err
	"应用序列号错",									// M_Test_AppSN_Err
	"卡密钥版本错",									// M_Test_KeyVer_Err
	"批次号错",										// M_Test_Batch_Err
	"行业类型码错",									// M_Test_Type_Err
	"卡类型编码错",									// M_Test_CardType_Err
	"指令集错",										// M_Test_Instruction_Err
	"密码类型错",									// M_Test_PINType_Err
	"非本站卡",										// M_Test_CardLocation_Err
	// Define the Error Message For Approval End

	// Below Define The Pump Process Message,
	// Be Careful To Change It, According To Pump Message Enumration
	"    ",											// M_PumpMsg

	"      ",										// M_PumpMsgStart,
	"油价更改中",									// 1. M_PumpUpdatePrice,
	"加油授权中",									// 2. M_PumpAuth,
	"加油中,请勿拔卡",								// 3. M_PumpFuelling,
	
//Douzhq 20140603 Add , Lock Nozzle 
	"油枪已锁",										//M_LockedNozzle,

	"      ",										// M_PumpMsgEnd
	"      ",										// M_PumpErrStart,

	"加油机离线",									// 101. M_PumpOffline,
#ifdef MAKE_CARD_MODE2
	"请挂枪",										// 102. M_PumpError,
#else
	"加油机故障",									// 102. M_PumpError,
#endif
	"无效抬枪,请挂枪",								// 103. M_PumpNozzleLiftError,
	"预置值超限",									// 104. M_PumpPresetExceed,
	"税控拒改油价",									// 105. M_PumpUpdatePriceError,
	"授权失败",										// 106. M_PumpAuthError,
	"取油机交易失败",								// 107. M_PumpLastTransError,
	"枪号错误",										// 108. M_PumpNozzleNumberError,
	"取脱机交易失败",								// 109. M_PumpOfflineTransError,
	"取加油机时间失败",								// 110. M_PumpTimeError,
	"加油机油价不符",								// 111. M_PumpPriceError,
	"加油机配置不符",								// 112. M_PumpConfigureError,
#ifdef MAKE_CARD_MODE2
	".             ",								// 113. M_PumpStateError,
#else
	"加油机命令状态不符",							// 113. M_PumpStateError,
#endif
	"CR通讯地址重叠",								// 114. M_Pump485AddrError,
	"通讯错误",										// 115. M_PumpCRCommError,
	"油价表错误",									// 116. M_PumpPPUError,
	"加油机认证失败",								// 117. M_PumpCertificateError,

	"      ",										// M_PumpErrEnd,
	"      ",										// M_PumpPumpErrorStart,

	"未检测到脉冲发生器",							// 201. M_PumpErr1,
	"EMT检测错误",									// 202. M_PumpErr2,
	"加油机做硬件主复位",							// 203. M_PumpErr3,
	"上电检测到有枪未挂",							// 204. M_PumpErr4,
	"预置过大",										// 205. M_PumpErr5,
	"预置过小",										// 206. M_PumpErr6,
	"加油结束未挂枪",								// 207. M_PumpErr7,
	"计量与税控通信违法",							// 208. M_PumpErr8,

	"      ",										// M_PumpPumpErrorEnd,

    "空间不足,请联机上传",                           // M_UploadFirst	//20131126 qiyong modify for transspace limit

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