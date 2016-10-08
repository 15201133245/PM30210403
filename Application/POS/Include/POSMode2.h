
#ifndef _POSMODE2_H
#define _POSMODE2_H

#include "global.h"
#include "PosPublic.h"
#include "WorkMode.h"
#define SEG_STEP 10

#define	POLL					0x10
#define	VER_INFO				0x11
#define	NOZZLE_ST_DATA			0x12
#define	AUTH_REQ1				0x13
#define	AUTH_REQ2				0x14
#define	AUTH_FB					0x15
#define	AUTH_MODE				0x16		
#define	STOP					0x17		
#define	NOZZLE_DOWN_DATA		0x18
#define	PAYMENT_DATA			0x19
#define	PRICE_CHANGE			0x1a		
#define	CB_RECORD				0x1b		
#define	FB_CARD_RECORD			0x1d		
#define	DEMAND_GRAY_RECORD		0x1e		
#define	GRAY_RECORD				0x1f		
#define	BWLIST_CHECK			0x20
#define	BWLIST_RESULT			0x21
#define REQ_V_TOT				0x22		
#define V_TOT					0x23		
#define REQ_DATA				0x24
#define DATA_BYTES				0x25
#define REQ_CONT				0x26
#define DAT_CONT				0x27
#define REQ_DISPENSER_INFO		0x28		
#define	DISPENSER_INFO			0x29		
#define ERROR_INFO				0x2a
#define AUTH_BG					0x2b		
#define TRAN_DATA				0x2c
#define AUTH_REQ3				0x2d		
#define	RECORD_TRAN				0x2f
#define	CMD_ACK					0x55

#define POS_RESPONSE_TIME		2 * 500
#define COMMNAD_MAX_COUNT		3
#define	POS_COMMAND_DELAY		100
#define	POS_SNDCOM_TIMEOUT		1000

#define	ASK_POS_DATALENG		(1 << 0)
#define ASK_POS_DATA			(1 << 1)
#define POS_ASK_V_TOT			(1 << 2)
#define POS_ASK_INFO			(1 << 3)   
#define POS_ASK_TRAN			(1 << 4)

#define POS_ERROR				(1 << 16)

#define POS_PRICE_ADR			0x5c9

enum POS_COMMAND_STATUS
{
	PosSending,
	PosWaitSnded,
	PosWaitResponse,
	PosReceived,
	PosWaitResponseTimeOut,
	PosWaitDelay,
	PosWaitDelayCom,
	PosReceivedDelay,
	PosReceivedDelayCom,
};



typedef struct _SndPosStruct
{
	unsigned char POS_P;
	unsigned char CurAdr;
	
	//= 0:可以下载数据长度，
	//= 1:下载数据
	//= 2:可以发送POLL 
	unsigned char Download;
	unsigned char MessNum;
	unsigned char PosFlgSnded;
	unsigned char PreState;
	unsigned char CurState;
	unsigned char SndCounts;
	unsigned char TimeOutCounts;
	unsigned int SndPosErrCounts;
	unsigned int SndPosNumber;
	unsigned int SndComTimeOutCount;
}SndPosStruct_t;

#ifdef TEST_POS_MODE2
typedef struct _TestPosMode2
{
	unsigned char AckFlg;
	unsigned char ResultFlg;
	unsigned char AuthBgFlg;
	unsigned char AuthModeFlg;
	unsigned char PosStopFlg;
	unsigned char PollMax;
	unsigned char PollCount;
	unsigned char AuthMode;
	unsigned char NDD_PollMax;
	unsigned char NDD_PollCount;
	unsigned char Pay_X;
	unsigned int Stop_PollMax;
	unsigned int Stop_PollCount;
}TestPosMode2Fun;
extern	TestPosMode2Fun testPos2Var;
#endif


extern	SndPosStruct_t SndPos;
extern	Tmr *PosTmr;
extern	Tmr *DownLoadTmr;
extern	Tmr *PosAckTmr;
extern	Tmr *PosErrTmr;
extern	Tmr *PosSndTimeOut;
extern	enum POS_COMMAND_STATUS PosState;
extern	unsigned char PosSndBuf[];
extern	unsigned char Arm_g_posbuff[];
extern	unsigned char DownLoadStatus;
extern	unsigned char PosToPosBuf[];
extern	unsigned char BaseBlackVer[];
extern	PcSeg PosSeg;
extern	unsigned char SendCountToPos; 
extern	unsigned int PosReadTTC;   
extern	unsigned int ProcExchangePos;
extern  unsigned char limit_flg;


extern	const unsigned char WriteDataFlashErr[];


extern 	void InitPosProcess2(void);

extern 	void Arm70_POS_Mode2_main( void );

extern 	unsigned char Pos_Polling(void);

extern	unsigned char Pos_AuthReq1(void);

extern	unsigned char Pos_AuthReq2(void);

extern	unsigned char Pos_NozzleDownData(void);

extern	unsigned char Pos_FbCardRecord(void);

extern	unsigned char Pos_GrayRecord(void);

extern	unsigned char Pos_BWListCheck(void);

extern	unsigned char Pos_VTot(void);

extern	unsigned char Pos_DataLeng(void);

extern	unsigned char Pos_Data(void);

extern	unsigned char Pos_DispenserInfo(void);

extern 	unsigned char Pos_TranData(void);

extern	unsigned char Pos_AuthReq3(void);  

extern	unsigned char Pos_RecordTran(void);

extern 	unsigned char Pos_ErrorSnd(void);



extern	void WritePosPrice(unsigned char *p,unsigned short len);

extern	unsigned short GetPosPriceNew(unsigned char nzn);

extern	unsigned short GetPosPriceOld(unsigned char nzn);

extern	unsigned int QueExportRecord(void);

extern	unsigned int ReadExportRecord(unsigned char *s);

extern	unsigned char MakeExportRecord(void);

#endif