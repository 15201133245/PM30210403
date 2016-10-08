#ifndef	_DEF_H
#define	_DEF_H
#include "AT91SAM7SE512.H"
#include "iso7816.h"

//Lcd light control
#define LCD_LIGHT_CLOSE		0
#define	LCD_LIGHT_30S		(1 << 0)
#define LCD_LIGHT_OPEN		(1 << 1)
#define LCD_LIGHT_OPEN_TIME (1 << 7)

#define CARD_IN_NOT			(1 << 3)

#define RECLAIM_WRITETTC	(1 << 7)

#define	LIGHT_TIME		30 * 1000

#define	PRINT_TASK_MAX	8

#define	STX					0x02
#define	ETX					0x03
#define	EOT					0x04
#define	ENQ					0x05
#define ACK					0x06
#define	NAK					0x15

//ic command param
#define	WARM_RESET			0x2f
#define	COLD_RESET			'0'
#define CARD_PARA			'1'
#define SAM_RESET_3			0x2f
#define SAM_RESET_5			'0'
#define PSAM_PARA			'1'

//command
#define	RESET				'0'
#define	QUERY				'1'
#define	CARD_OUT			'2'
#define	POWER				'3'
#define	SET_BAUD			'4'

#define	CARD_COM			'7'
#define SAM_COM				0x3d

//IC_CARD
#define	CARDIN				(1 << 0)
#define	POWERON				(1 << 1)
#define	RESET_OK			(1 << 2)
#define	CARD_ATR_ERR		(1 << 3)
#define DCDCB_POWERON		(1 << 4)
#define	SAM0_POWER_ON		(1 << 5)
#define	SAM1_POWER_ON		(1 << 6)
#define	SAM2_POWER_ON		(1 << 7)
#define	SAM3_POWER_ON		(1 << 8)
#define	SAM0_ATR_ERR		(1 << 9)
#define	SAM1_ATR_ERR		(1 << 10)
#define	SAM2_ATR_ERR		(1 << 11)
#define	SAM3_ATR_ERR		(1 << 12)

#define	GET_ATR				0
#define	ATR_OK				1
#define	SEND_STATUS			2
#define	WAIT_REV			3
#define REV_OK				4
#define	RDY_GET_RESPONSE	5

//usart-iso7816
#define	NBSTOP_1_BIT	0
#define	NBSTOP_15_BIT	1
#define	NBSTOP_2_BIT	2
#define	CLKS_CLOCK		0
#define	CLKS_FDIV1		1
#define	CLKS_SLOW		2
#define	CLKS_EXT		3
#define	PAR_EVEN		0
#define	PAR_ODD			1
#define	PAR_SPACE		2
#define	PAR_MARK		3
#define	PAR_NONE		4
#define	PAR_MULTI_DROP	6
#define	CHRL_5_BITS		0
#define	CHRL_6_BITS		1
#define	CHRL_7_BITS		2
#define	CHRL_8_BITS		3
#define	US_MSBF			1
#define	US_MODE9		1
#define	US_CKLO			1
#define	US_OVER			1
#define	US_INACK		1
#define	US_DSNACK		1
#define	MAX_ITER(x)		((x) & 0x07)
#define	US_FILTER		1

//card
#define		ICCARD		0
#define		SAM0		1
#define		SAM1		2
#define		SAM2		3
#define		SAM3		4

#define		ALL			0xff

#define		VCC_5		3

//ChipId
//0x0~0x0f SPI chip ID
#define ZIMO_ID				0
#define CONFIG_ID			0
#define TRANS_ID			1
#define LOG_ID			1
//0x10~01f TWI chip ID
#define	EEPROM_ID			0x10
//0x20~0x2f USART chip ID
#define DUSART0_ID			0x20
#define USART0_ID			0x21
#define USART1_ID			0x22
#define USART2_ID			0x23
#define USART3_ID			0x24

#define	ID_ERROR			(-0xfe)

//PIN function
#define DISABLE_PULLUP		0
#define ENABLE_PULLUP		1
#define DISABLE_MULTIDRIVE	0
#define ENABLE_MULTIDRIVE	1
#define DISABLE_FILTER		0
#define	ENABLE_FILETER		1
#define CLOSE				1
#define	OPEN				0

#define TC_CLKS_MCK1024    		 	0x4
#define	CYC_1MS						((unsigned int)((1e-3)*(MAIN_MCK)/16))
#define CYC_MS(x)					((unsigned int )(CYC_1MS*x))

//interrupt priority
#define USART0_INTERRUPT_PRIORITY	6
#define USART1_INTERRUPT_PRIORITY	6
#define	SPI_PRIORITY				6
#define TIME_PRIORITY		 		1
#define TIME1_PRIORITY		 		2
#define IRQ_INTERRUPT_PRIORITY		6

//usart baud
#define USART0_BAUD					9600
#define USART1_BAUD					9600
#define DBUG_USART_BAUD_INT		9600
#define	DBUG_USART_BAUD				115200

//Lcd display Error
#define AT45_NOTFIND			0
#define RECORD_ERROR			1
#define	ZIMO_ERROR				2

//Format
#define	LIST_CONFIG			0
#define RECORD_CONFIG		1

#define G_CODE_TICKET_ADR	0x5d0			//0x200
#ifdef NEW_VOICE
#define	NEW_VOICE_ADR		0x600			//0x300
#endif

#define PROTECTED_START		0x401
#define LIST_START			0x40a
#define LIST_END				0x5c9
#define RECORD_START		0x000
#define RECORD_END			0x200

#define BACKUP_TTC_ADRESS(x)	0x200 + (x)

//List save  start page address
#define LIST_HEAD_ADDRESS		0x40a		//1

#define LIST_INTEGRALITY		0x40b
#define CR_CONFIG_ADDRESS		0x40c
#define POWERDOWN_TRANS			0x40d

#define BASE_BLACK_LIST_ADDRESS	0x410		//0x410~0x5b1
#define BASE_BLACK_LIST_END		0x5b1
#define ADD_BLACK_LIST_ADDRESS	0x5b2		//0x5b2~0x5b7
#define ADD_BLACK_LIST_END		0x5b7
#define DEL_BLACK_LIST_ADDRESS	0x5b8		//0x5b8~0x5bd
#define DEL_BLACK_LIST_END		0x5bd
#define WHITE_LIST_ADDRESS		0x5bf		//0x5bf~0x5c4
#define WHITE_LIST_END			0x5c4
#define GRADE_PRICE_ADDRESS		0x5c5		//1
#define LOCAL_PRICE_ADDRESS		0x5c6		//1
#define OIL_STOP_ADDRESS		0x5c7		//1

#define CRC_PROTECTED_FLG_ADR	0x800		//1

//Douzhq 20140925 Add , Config Printer Set
#define POS_TYPE 0
#define LCD_TYPE 0

#ifdef MAKE_CASH_CR
#if POS_TYPE==0
#define CASHPOS_TRANS_ADR		0x900
#define CASHPOS_SWITCH_ADR		0x904
#endif
#endif

#define TICKET_HEAD_ADRESS		0xfff

//Record
#define RECORD_START_PAGE		0x000
#define RECORD_END_PAGE			0x1ff
#define RECORD_SIZE				128

//BACKUP
#define PROTECTED_BACKUP_ADDR	0x201

//protected
#define RECORD_LEN				18
#define PROTECTED_LEN			64

//BlackListLinkMax
#define BASEBLACK_LINKMAX		417
#define ADDBLACK_LINKMAX			5
#define DELBLACK_LINKMAX			5
#define WHITE_LINKMAX			5

//List type
#define BASEBLACKLIST			0
#define ADDBLACKLIST				1
#define DELBLACKLIST				2
#define WHITELIST				3
#define GRADEPRICE				4
#define OILSTOP					5

//
#define RECORD_MAX				2048

#define MON							0
#define VOL							1
#define NO							0

#define POS_READ_RECORD				(1 << 0)
#define LAST_TRANS_RECORD			(1 << 1)
#define TRANS_UPLOADING				(1 << 2)

typedef void (*pTimeFunc)(void);

typedef struct
{
		/// Bitmask indicating which pin(s) to configure.
		unsigned int mask; 
		/// Pointer to the PIO controller which has the pin(s).
		AT91S_PIO    *pio;
		/// Peripheral ID of the PIO controller which has the pin(s).
		unsigned char id;
		/// Pin type (see "Pin types").
		unsigned char type;
		/// Pin attribute (see "Pin attributes").
		unsigned char attribute;
}Pin;

typedef struct _KeyStruct
{
		unsigned char 	outNumber;
		unsigned char 	inNumber;
		unsigned char 	lastScanKey;
		unsigned char	currentScanKey;
		unsigned char const *pOutScan;
		unsigned char const	*pInScan;
		unsigned char const	*pCode;
		unsigned char	lastKey;
		unsigned char 	currentKey;
		unsigned char	keyFlg;
}Key_t;

typedef struct _ButtonStruct
{
	unsigned char last_val;
	unsigned char current_val;
	unsigned char last_button;
	unsigned char buttonFlg;
}Button_t;

//PIO
typedef union
{
    unsigned int   pio;
    struct
    {
        unsigned int   P0 : 1;
        unsigned int   P1 : 1;
        unsigned int   P2 : 1;
        unsigned int   P3 : 1;
        unsigned int   P4 : 1;
        unsigned int   P5 : 1;
        unsigned int   P6 : 1;
        unsigned int   P7 : 1;
        unsigned int   P8 : 1;
        unsigned int   P9 : 1;
        unsigned int   P10 : 1;
        unsigned int   P11 : 1;
        unsigned int   P12 : 1;
        unsigned int   P13 : 1;
        unsigned int   P14 : 1;
        unsigned int   P15 : 1;
        unsigned int   P16 : 1;
        unsigned int   P17 : 1;
        unsigned int   P18 : 1;
        unsigned int   P19 : 1;
        unsigned int   P20 : 1;
        unsigned int   P21 : 1;
        unsigned int   P22 : 1;
        unsigned int   P23 : 1;
        unsigned int   P24 : 1;
        unsigned int   P25 : 1;
        unsigned int   P26 : 1;
        unsigned int   P27 : 1;
        unsigned int   P28 : 1;
        unsigned int   P29 : 1;
        unsigned int   P30 : 1;
        unsigned int   P31 : 1;            
    };
}PIO,*pPIO;

typedef struct _DataProtected
{
	unsigned short RecordStartPageAddr;			//0
	unsigned short RecordEndPageAddr;			//2
	unsigned int RecordWriteTTC;				//4
	//unsigned short RecordCurrentPageAddr;
	//unsigned short RecordCountInPage;
	unsigned int RecordReadTTC;					//8
	unsigned short UpTransferPageAddr;			//12
	unsigned short UpTransferOffsetInPage;		//14
	unsigned short PriceUpdateFlg;		//0:不检测油价更新时间;1:检测油价更新时间;2:油价更新时间到了		//16
	unsigned short RecordCrc;					//18
	
	unsigned short LogWriteFlag;				//20
	unsigned short LogWriteCurrentPageAddr;		//22
	unsigned short LogWritePageNumber;			//24
	
	unsigned short BaseBlackListIntegrality;
	unsigned short AddBlackListIntegrality;
	unsigned short DelBlackListIntegrality;
	unsigned short WhiteListIntegrality;
	unsigned short GradePriceIntegrality;
	unsigned short OilStopIntegrality;
	unsigned char OpenTime;
	unsigned char CloseTime;
	unsigned int	BaseBlackListLength;
	unsigned int 	AddBlackListLength;
	unsigned int	DelBlackListLength;
	unsigned int 	WhiteListLength;
	unsigned int 	GradePriceLength;
	unsigned int 	OilStopLength;

	unsigned short crc;
}DataProtected;

typedef struct _ListWriteStation
{
	unsigned short BaseBlackListPageAddr;
	unsigned short AddBlackListPageAddr;
	unsigned short DelBlackListPageAddr;
	unsigned short WhiteListPageAddr;
	unsigned short GradePricePageAddr;
	unsigned short OilStopPageAddr;
	unsigned short crc;
	unsigned char BaseBlackListCount;
	unsigned char AddBlackListCount;
	unsigned char DelBlackListCount;
	unsigned char WhiteListCount;
	unsigned char GradePriceCount;
	unsigned char OilStopCount;
}ListWriteStation;

typedef struct
{
		unsigned short year;
		unsigned char month;
		unsigned char day;
		unsigned char hour;
		unsigned char minute;
		unsigned char second;
}Time;

typedef struct _ListHead
{
	unsigned char Ver[2];
	unsigned char InuredDate[4];
	unsigned char EndDate[4];
	unsigned char InuredArea[2];
	unsigned char ListAmount[4];
	unsigned char StartPageAdr[4];
	unsigned char EndPageAdr[4];
	unsigned char crc[2];
}ListHead;

typedef struct _GDP
{
	unsigned char nzn;
	unsigned short o_type;
	unsigned int den;
	unsigned char prc_n;
	unsigned short prc[8];
}GDP;

typedef struct _GradePriceList
{
	unsigned char Ver;
	unsigned char InuredDate[6];
	unsigned char FieldNum;
	GDP	gdp[8];
	GDP	next_gdp[8];
}GradePriceList;

typedef struct _OilStationMessage
{
	unsigned char flg;
	unsigned char ver;
	unsigned char prov;
	unsigned char city;
	unsigned char superior[4];
	unsigned char s_id[4];
	unsigned char pos_p;
	unsigned char gun_n;
	unsigned char nzn[250];
}OilStationMessage;

typedef struct _BaseBlackListLink
{
	unsigned char CardNumber[BASEBLACK_LINKMAX + 1][10];
	unsigned short crc;
}BaseBlackListLink;

typedef struct _ListLink
{
	unsigned char CardNumber[ADDBLACK_LINKMAX + 1][10];
	unsigned short crc;
}ListLink;

typedef struct _PrintTrans
{
	//char flg;				//0:idle;others:task
	//struct _PrintTrans *p;
	struct _PrintTrans *pPrev;
	struct _PrintTrans *pNext;
	char s_id[4];
	char typ;				//0:IC query print;1:IC realtime print;2:CASH query print;3:CASH realtime print;
							//6:IC realtime print short coup //20140324 qiyong add for guangdong V2 requirement
	char pos_ttc[4];
	char t_type;
	char time[7];
	char asn[10];
	char bal[4];
	char amn[3];
	char ctc[2];
	char tac[4];
	char gmac[4];
	char psam_tac[4];
	char psam_asn[10];
	char psam_tid[6];
	char psam_ttc[4];
	char ds;
	char unit;
	char c_type;
	char ver;
	char nzn;
	char g_code[2];
	char vol[3];
	char prc[2];
	char emp;
	char v_tot[4];
	char rfu[11];
	char t_mac[4];
//	struct _PrintTrans *pPrev;
//	struct _PrintTrans *pNext;
}sPrintTrans;

typedef struct _IcCommFormat
{
	unsigned char CLA;
	unsigned char INS;
	unsigned char P1;
	unsigned char P2;
	unsigned char P3;
}IcCommFormat;

typedef struct _print_buf
{
	unsigned short len;
	unsigned char buf[2046];
}PrintBuf;

typedef struct _g_code
{
	unsigned short g_code;
	const char *gStr;
}gcode_t;

//对于POS_ERROR错误标志，errno的定义
#define PRICE_ERROR_FLAGS		1			//油品油价表错误
#define BASEBLACK_LEN_ERROR		2			//基础黑名单长度错误
#define BASEBLACK_NUMBER_ERROR	3			//基础黑名单数量超长
#define ADDBLACK_LEN_ERROR		4			//新增黑名单长度错误
#define ADDBLACK_NUMBER_ERROR  	5			//新增黑名单数量超长
#define DELBLACK_LEN_ERROR		6			//新删黑名单长度错误
#define DELBLACK_NUMBER_ERROR	7			//新删黑名数量超长
#define WHITE_LEN_ERROR			8			//白名单长度错误
#define WHITE_NUMBER_ERROR		9			//白名单数量超长
#define GRADE_LEN_ZERO			10			//油品油价表长度为0
#define GRADE_LEN_ERROR			11			//油品油价表超长
#define OILMES_LEN_ZERO			12			//油站通用信息长度为0
#define OILMES_LEN_ERROR		13			//油站通用信息超长


#define MINUTE					60
#define HOUR					(60 * MINUTE)
#define DAY						(24 * HOUR)
#define CYEAR					(365 & DAY)


#define OUT_AUTO				(1 << 0)
#define OUT_REPLAY_AUTO			(1 << 1)
#define OUT_CLOSE				(1 << 2)
#define OUT_OPEN				(1 << 3)

#define OUT_DATA				(1 << 4)

#define OUT_MAX					8

typedef struct _OutputStruct
{
	unsigned char type;			
	unsigned char no;
	OutTmr* openTmr;
	OutTmr* closeTmr;
	int count;
}Output_t;

#endif