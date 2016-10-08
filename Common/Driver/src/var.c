#include "global.h"

//pin
const Pin run_led = RUN_LED;
const Pin beep = BEEP;
const Pin lcd_bl = LCD_BL;
const Pin twi_adr = TWI_ADR;
const Pin usbcnx = USBCNX;
const Pin prn_cts = PRINT_CTS;
const Pin irq1 = IRQ1;
const Pin jump = JUMP;
const Pin fiq = FIQ;
const Pin irq0 = IRQ0;
const Pin card_open = CARD_OPEN;
const Pin card_in = CARD_IN;
const Pin pin_twi = PINS_TWI;
const Pin usart0 = USART0;  
const Pin usart1 = USART1;
const Pin iso7816 = ISO7816;
const Pin dbug_usart = DBUG_USART;
const Pin spi = SPI;
const Pin spi_ncs1 = SPI_NCS1;
const Pin spi_ncs2 = SPI_NCS2;
const Pin data_bus = DATA_BUS;
const Pin addr_bus = ADDR_BUS;
const Pin ctrl_bus = CTRL_BUS;

const unsigned char YEAR[2][13] = 
{
  {0,31,28,31,30,31,30,31,31,30,31,30,31},
  {0,31,29,31,30,31,30,31,31,30,31,30,31}
};

const char PosError[][20] =
{
	"油品油价表错误",
	"基础黑名单长度错误",
	"基础黑名单数量超长",
	"新增黑名单长度错误",
	"新增黑名单数量超长",
	"新删黑名单长度错误",
	"新删黑名单数量超长",
	"白名单长度错误",
	"白名单数量超长",
	"油品油价表长度为0",
	"油品油价表超长",
	"油站通用信息长度为0",
	"油站通用信息超长"
};

volatile unsigned char spi_status;
unsigned char Rs485Adr;
unsigned char DisBuf[1024 + 128];
Tmr *pBeepTmr;
unsigned char BeepFlg;
unsigned char POS_P;
unsigned char PC_ADDR;
unsigned char PosFlg;
unsigned char PosOnline;
unsigned char PowerDownFlg;
unsigned char LcdLightFlg;
Tmr *pLcdTmr;
unsigned int PowerDownSaveReadTTC;
unsigned short PriceUpdateFlg = 0xffff;

//bit0 = 1:显示请插卡,否则不显示
//bit1 = 1:显示系统时间，否则不显示
unsigned char IdleFlg;
unsigned char LcdStatus;

Async 	TwiSync;
Twid	twid;	
At24 	at24;
At83	at83;
unsigned int TwiErrCount;
unsigned int CardStatus;
volatile unsigned char CardInInt;

Timer_Val	timer_val;
Timer_Val	timer1_val;
Timer_Val	pit_val;

pTimeFunc pTimer1;

Serial_t	Serial0; 
Serial_t	Serial1;
DbugSerial_t DbugSerial;


Iso7816		iso;
At83Ch		at83ch[5];

Spid 	spid;	
At45	at45[2];
M1172	m1172;
M1172_ChCtrl chCtrl[2];

Time 	SysTime;
Time 	SysBCDTime;
Time	PriceUpdateTime;

Key_t	Key;

Queue 	queue;

DataProtected cr_protected;

ListWriteStation ListFlg;
BaseBlackListLink BBLLink;
ListLink ABLLink;
ListLink DBLLink;
ListLink WLLink;

SinopecRev	SinopecRevBuf;

sPrintTrans *pPrintFreeList;
sPrintTrans *pPrintList;

sPrintTrans PrintTrans[PRINT_TASK_MAX];

unsigned char PrintBusy;

unsigned char IcTypeFlg;

unsigned char CardFlg;

GradePriceList GradePrice;

OilStationMessage OilMessage;


unsigned char Pos2MessageTime;

unsigned char reset_typ;
__no_init unsigned int Reset_Adr;
__no_init unsigned int HandlerAbort;

int errno;

unsigned char Area_Station;

#ifdef MAKE_CASH_CR
unsigned char CashFromPosUnit;
unsigned short CashErrorNumber;
#endif
