#ifndef _DISPPUMPMSG_H
#define _DISPPUMPMSG_H


enum DISPPUMPMSGID
{
	M_PumpMsgStart_Pump,						// ,
	
	M_PumpUpdatePrice_Pump,						// 1. ,
	M_PumpAuth_Pump,							// 2. ,
	M_PumpFuelling_Pump,						// 3. ,

	M_PumpMsgEnd_Pump,
};

enum DISPPUMPERRMSGID
{
	M_PumpErrStart_Pump = 100,					// ,

	M_PumpOffline_Pump,							// 101. ,
	M_PumpError_Pump,							// 102. ,
	M_PumpNozzleLiftError_Pump,					// 103. ,
	M_PumpPresetExceed_Pump,					// 104. ,
	M_PumpUpdatePriceError_Pump,				// 105. ,
	M_PumpAuthError_Pump,						// 106. ,
	M_PumpLastTransError_Pump,					// 107. ,
	M_PumpNozzleNumberError_Pump,				// 108. ,
	M_PumpOfflineTransError_Pump,				// 109. ,
	M_PumpTimeError_Pump,						// 110. ,
	M_PumpPriceError_Pump,						// 111. ,
	M_PumpConfigureError_Pump,					// 112. ,
	M_PumpStateError_Pump,						// 113. ,
	M_Pump485AddrError_Pump,					// 114. ,
	M_PumpCRCommError_Pump,						// 115. ,
	M_PumpPPUError_Pump,						// 116. ,
	M_PumpCertificateError_Pump,				// 117. ,
	
	M_PumpErrEnd_Pump,
};

enum DISPPUMPPUMPERRMSGID
{
	M_PumpPumpError_Pump = 200,					// ,

	M_PumpErr1_Pump,							// 201. ,
	M_PumpErr2_Pump,							// 202. ,
	M_PumpErr3_Pump,							// 203. ,
	M_PumpErr4_Pump,							// 204. ,
	M_PumpErr5_Pump,							// 205. ,
	M_PumpErr6_Pump,							// 206. ,
	M_PumpErr7_Pump,							// 207. ,
	M_PumpErr8_Pump,							// 208. ,

	M_PumpPumpEnd_Pump,							// 
};


extern const char DISPPUMPMSG[][20];
extern const char DISPPUMPERRMSG[][20];
extern const char DISPPUMPPUMPERRMSG[][20];

extern unsigned int PumpMsgID;
extern unsigned int NextPumpMsgID;

extern void SetDispPumpMsg( unsigned int DispPumpMsgID );




#endif