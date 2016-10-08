
#include "Message.h"
#include "DispPumpMsg.h"



//LCD display  message
//0--100  state message
//100-- > err message
//const char DISPPUMPMSG[][20] = {
//
//	"      ",							// M_PumpMsgStart_Pump,
//	"油价更改中",						// 1. M_PumpUpdatePrice_Pump,
//	"加油授权中",						// 2. M_PumpAuth_Pump,
//	"加油中,请勿拔卡",					// 3. M_PumpFuelling_Pump,
//
//	"      ",							// M_PumpMsgEnd_Pump
//};
//
//const char DISPPUMPERRMSG[][20] = {
//	"      ",							// M_PumpErrStart_Pump,
//
//	"加油机离线",						// 101. M_PumpOffline_Pump,
//	"加油机故障",						// 102. M_PumpError_Pump,
//	"无效抬枪,请挂枪",					// 103. M_PumpNozzleLiftError_Pump,
//	"预置值超限",						// 104. M_PumpPresetExceed_Pump,
//	"税控拒改油价",						// 105. M_PumpUpdatePriceError_Pump,
//	"授权失败",							// 106. M_PumpAuthError_Pump,
//	"取油机交易失败",					// 107. M_PumpLastTransError_Pump,
//	"枪号错误",							// 108. M_PumpNozzleNumberError_Pump,
//	"取脱机交易失败",					// 109. M_PumpOfflineTransError_Pump,
//	"取加油机时间失败",					// 110. M_PumpTimeError_Pump,
//	"加油机油价不符",					// 111. M_PumpPriceError_Pump,
//	"加油机配置不符",					// 112. M_PumpConfigureError_Pump,
//	"加油机命令状态不符",				// 113. M_PumpStateError_Pump,
//	"CR通讯地址重叠",					// 114. M_Pump485AddrError_Pump,
//	"通讯错误",							// 115. M_PumpCRCommError_Pump,
//	"油价表错误",						// 116. M_PumpPPUError_Pump,
//	"加油机认证失败",					// 117. M_PumpCertificateError_Pump,
//
//	"      ",							// M_PumpErrEnd_Pump,
//};
//
//const char DISPPUMPPUMPERRMSG[][20] = {
//	"      ",							// M_PumpPumpErrorStart_Pump,
//
//	"未检测到脉冲发生器",				// 201. M_PumpErr1_Pump,
//	"EMT检测错误",						// 202. M_PumpErr2_Pump,
//	"加油机做硬件主复位",				// 203. M_PumpErr3_Pump,
//	"上电检测到有枪未挂",				// 204. M_PumpErr4_Pump,
//	"预置过大",							// 205. M_PumpErr5_Pump,
//	"预置过小",							// 206. M_PumpErr6_Pump,
//	"加油结束未挂枪",					// 207. M_PumpErr7_Pump,
//	"计量与税控通信违法",				// 208. M_PumpErr8_Pump,
//
//	"      ",							// M_PumpPumpErrorEnd_Pump,
//
//};




unsigned int PumpMsgID;
unsigned int NextPumpMsgID;

void SetDispPumpMsg( unsigned int DispPumpMsgID )
{
	if( DispPumpMsgID != NextPumpMsgID )
	{
		NextPumpMsgID = DispPumpMsgID;

		// Change Pump Message ID To Display MessageID
		switch( NextPumpMsgID )
		{
			case M_PumpUpdatePrice_Pump:
				FillInfoMsg( M_PumpUpdatePrice, 0, 0 );
			break;

			case M_PumpAuth_Pump:
				FillInfoMsg( M_PumpAuth, 0, 0 );
			break;

			case M_PumpFuelling_Pump:
				FillInfoMsg( M_PumpFuelling, 0, 0 );
			break;

			case M_PumpOffline_Pump:
				FillInfoMsg( M_PumpOffline, 0, 0 );
			break;

			case M_PumpError_Pump:
				FillInfoMsg( M_PumpError, 0, 0 );
			break;

			case M_PumpNozzleLiftError_Pump:
				FillInfoMsg( M_PumpNozzleLiftError, 0, 0 );
			break;

			case M_PumpPresetExceed_Pump:
				FillInfoMsg( M_PumpPresetExceed, 0, 0 );
			break;

			case M_PumpUpdatePriceError_Pump:
				FillInfoMsg( M_PumpUpdatePriceError, 0, 0 );
			break;

			case M_PumpAuthError_Pump:
				FillInfoMsg( M_PumpAuthError, 0, 0 );
			break;

			case M_PumpLastTransError_Pump:
				FillInfoMsg( M_PumpLastTransError, 0, 0 );
			break;

			case M_PumpNozzleNumberError_Pump:
				FillInfoMsg( M_PumpNozzleNumberError, 0, 0 );
			break;

			case M_PumpOfflineTransError_Pump:
				FillInfoMsg( M_PumpOfflineTransError, 0, 0 );
			break;

			case M_PumpTimeError_Pump:
				FillInfoMsg( M_PumpTimeError, 0, 0 );
			break;

			case M_PumpPriceError_Pump:
				FillInfoMsg( M_PumpPriceError, 0, 0 );
			break;

			case M_PumpConfigureError_Pump:
				FillInfoMsg( M_PumpConfigureError, 0, 0 );
			break;

			case M_PumpStateError_Pump:
				FillInfoMsg( M_PumpStateError, 0, 0 );
			break;

			case M_Pump485AddrError_Pump:
				FillInfoMsg( M_Pump485AddrError, 0, 0 );
			break;

			case M_PumpCRCommError_Pump:
				FillInfoMsg( M_PumpCRCommError, 0, 0 );
			break;

			case M_PumpPPUError_Pump:
				FillInfoMsg( M_PumpPPUError, 0, 0 );
			break;

			case M_PumpCertificateError_Pump:
				FillInfoMsg( M_PumpCertificateError, 0, 0 );
			break;

			case M_PumpErr1_Pump:
				FillInfoMsg( M_PumpErr1, 0, 0 );
			break;

			case M_PumpErr2_Pump:
				FillInfoMsg( M_PumpErr2, 0, 0 );
			break;

			case M_PumpErr3_Pump:
				FillInfoMsg( M_PumpErr3, 0, 0 );
			break;

			case M_PumpErr4_Pump:
				FillInfoMsg( M_PumpErr4, 0, 0 );
			break;

			case M_PumpErr5_Pump:
				FillInfoMsg( M_PumpErr5, 0, 0 );
			break;

			case M_PumpErr6_Pump:
				FillInfoMsg( M_PumpErr6, 0, 0 );
			break;

			case M_PumpErr7_Pump:
				FillInfoMsg( M_PumpErr7, 0, 0 );
			break;

			case M_PumpErr8_Pump:
				FillInfoMsg( M_PumpErr8, 0, 0 );
			break;

			default:
			break;
		}
	}
}
