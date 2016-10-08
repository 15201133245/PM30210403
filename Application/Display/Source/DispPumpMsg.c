
#include "Message.h"
#include "DispPumpMsg.h"



//LCD display  message
//0--100  state message
//100-- > err message
//const char DISPPUMPMSG[][20] = {
//
//	"      ",							// M_PumpMsgStart_Pump,
//	"�ͼ۸�����",						// 1. M_PumpUpdatePrice_Pump,
//	"������Ȩ��",						// 2. M_PumpAuth_Pump,
//	"������,����ο�",					// 3. M_PumpFuelling_Pump,
//
//	"      ",							// M_PumpMsgEnd_Pump
//};
//
//const char DISPPUMPERRMSG[][20] = {
//	"      ",							// M_PumpErrStart_Pump,
//
//	"���ͻ�����",						// 101. M_PumpOffline_Pump,
//	"���ͻ�����",						// 102. M_PumpError_Pump,
//	"��Ч̧ǹ,���ǹ",					// 103. M_PumpNozzleLiftError_Pump,
//	"Ԥ��ֵ����",						// 104. M_PumpPresetExceed_Pump,
//	"˰�ؾܸ��ͼ�",						// 105. M_PumpUpdatePriceError_Pump,
//	"��Ȩʧ��",							// 106. M_PumpAuthError_Pump,
//	"ȡ�ͻ�����ʧ��",					// 107. M_PumpLastTransError_Pump,
//	"ǹ�Ŵ���",							// 108. M_PumpNozzleNumberError_Pump,
//	"ȡ�ѻ�����ʧ��",					// 109. M_PumpOfflineTransError_Pump,
//	"ȡ���ͻ�ʱ��ʧ��",					// 110. M_PumpTimeError_Pump,
//	"���ͻ��ͼ۲���",					// 111. M_PumpPriceError_Pump,
//	"���ͻ����ò���",					// 112. M_PumpConfigureError_Pump,
//	"���ͻ�����״̬����",				// 113. M_PumpStateError_Pump,
//	"CRͨѶ��ַ�ص�",					// 114. M_Pump485AddrError_Pump,
//	"ͨѶ����",							// 115. M_PumpCRCommError_Pump,
//	"�ͼ۱����",						// 116. M_PumpPPUError_Pump,
//	"���ͻ���֤ʧ��",					// 117. M_PumpCertificateError_Pump,
//
//	"      ",							// M_PumpErrEnd_Pump,
//};
//
//const char DISPPUMPPUMPERRMSG[][20] = {
//	"      ",							// M_PumpPumpErrorStart_Pump,
//
//	"δ��⵽���巢����",				// 201. M_PumpErr1_Pump,
//	"EMT������",						// 202. M_PumpErr2_Pump,
//	"���ͻ���Ӳ������λ",				// 203. M_PumpErr3_Pump,
//	"�ϵ��⵽��ǹδ��",				// 204. M_PumpErr4_Pump,
//	"Ԥ�ù���",							// 205. M_PumpErr5_Pump,
//	"Ԥ�ù�С",							// 206. M_PumpErr6_Pump,
//	"���ͽ���δ��ǹ",					// 207. M_PumpErr7_Pump,
//	"������˰��ͨ��Υ��",				// 208. M_PumpErr8_Pump,
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
