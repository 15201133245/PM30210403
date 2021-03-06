#ifndef	_GLOBAL_H
#define	_GLOBAL_H

#include <string.h>
#include <intrinsics.h>

#include "at24.h"
#include "at83.h"
#include "tc.h"
#include "usart.h"
#include "at45.h"
#include "m1172.h"
#include "DbugUsart.h"
#include "at83.h"

#include "board.h"
#include "func.h"
#include "InlineFunc.h"


extern	const Pin run_led;
extern	const Pin beep;
extern	const Pin lcd_bl;
extern	const Pin twi_adr;
extern	const Pin usbcnx;
extern	const Pin prn_cts;
extern	const Pin irq1;
extern	const Pin jump;
extern	const Pin fiq;
extern	const Pin irq0;
extern	const Pin card_open;
extern	const Pin card_in;
extern	const Pin pin_twi;
extern	const Pin usart0;  
extern  const Pin usart1;
extern	const Pin iso7816;
extern	const Pin dbug_usart;
extern	const Pin spi;
extern	const Pin spi_ncs1;
extern	const Pin spi_ncs2;
extern	const Pin data_bus;
extern	const Pin addr_bus;
extern	const Pin ctrl_bus;

extern	const unsigned char YEAR[2][13];

extern	const unsigned int month[];

extern	const char PosError[][20];

extern	volatile unsigned char spi_status;
extern	unsigned char Rs485Adr;
extern	unsigned char DisBuf[];
extern	Tmr *pBeepTmr;
extern	unsigned char BeepFlg;
extern	unsigned char POS_P;
extern	unsigned char PC_ADDR;
extern	unsigned char PosFlg;
extern	unsigned char PosOnline;
extern	unsigned char IdleFlg;
extern	unsigned char LcdStatus;
extern	unsigned char PowerDownFlg;
extern	unsigned char LcdLightFlg;
extern	Tmr *pLcdTmr;
extern	unsigned int PowerDownSaveReadTTC;
extern	unsigned short PriceUpdateFlg;

extern	Async 	TwiSync;
extern	Twid	twid;	
extern	At24 	at24;
extern	At83	at83;
extern	unsigned int TwiErrCount;
extern	unsigned int CardStatus;
extern	volatile unsigned char CardInInt;

extern	Timer_Val	timer_val;
extern	Timer_Val	timer1_val;
extern	Timer_Val	pit_val;

extern	pTimeFunc pTimer1;

extern	Serial_t	Serial0;  
extern  Serial_t	Serial1;
extern	DbugSerial_t DbugSerial;

extern	Iso7816		iso;
extern	At83Ch		at83ch[];

extern	Spid 	spid;	
extern	At45	at45[];
extern	M1172	m1172;
extern	M1172_ChCtrl chCtrl[];

extern	Time 	SysTime;
extern	Time 	SysBCDTime;
extern	Time	PriceUpdateTime;

extern	Key_t	Key;
extern	Button_t Button;

extern	Queue 	queue;

extern	DataProtected cr_protected;

extern	ListWriteStation ListFlg;
extern	BaseBlackListLink BBLLink;
extern	ListLink ABLLink;
extern	ListLink DBLLink;
extern	ListLink WLLink;

extern	SinopecRev	SinopecRevBuf;

extern	sPrintTrans *pPrintFreeList;
extern	sPrintTrans *pPrintList;

extern	sPrintTrans PrintTrans[];

extern	unsigned char PrintBusy;

extern	unsigned char IcTypeFlg;

extern	unsigned char CardFlg;

extern	GradePriceList GradePrice;

extern	OilStationMessage OilMessage;


extern	unsigned char Pos2MessageTime;

extern  unsigned char reset_typ;

extern  unsigned int Reset_Adr;
extern	unsigned int HandlerAbort;

extern	int errno;

#ifdef MAKE_CASH_CR
extern 	unsigned char CashFromPosUnit;
//Douzhq 20140916 Add , Voice Add
extern unsigned short CashErrorNumber;
#endif

extern	unsigned char Area_Station;

extern	Output_t output[];
extern	unsigned char lampValue;
extern	Tmr* pTmrButton;
extern	Tmr* pTmrInfrared;
extern	unsigned char infraredFlg;
extern	unsigned char infraredTmrFlg;

// 20140220 qiyong
//20140325
extern	Tmr* pTmrPrint;

#endif
