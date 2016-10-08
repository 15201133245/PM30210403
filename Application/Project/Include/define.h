#ifndef _DEFINE_H
#define _DEFINE_H



enum MAX_PROCESS
{
	PROCESS_START,						// No Use, Only For Enum Start

	PROCESS_PUMP,						// Pump Process
	PROCESS_POS,						// Pos Process
	PROCESS_CR,							// IC Card Process
	PROCESS_KEYPAD,						// Keypad Process
	PROCESS_DISPLAY,					// Display Process
	PROCESS_POWERDOWN,					// Power Down Process
	PROCESS_SCHEDULE,					// Schedule Process
	PROCESS_PRINT,						// Print Process
	PROCESS_VOICE,						// Voice Process
	PROCESS_CASH,						// Cash Trans Comm Process

	PROCESS_MAX,						// No Use, Only For Enum End
};









// Define The Bits Used In ProcExchange
// Keypad And Card Reader Process Exchange
#define ICCARD_PIN				0x00000001					// Input IC Card PIN
#define EMP_PASSWORD			0x00000002					// Input Employee Password
#define VEHICLE_NUMBER			0x00000004					// Input Vehicle Number
#define SPRING_CARD				0x00000008					// Keypad Spring Out IC Card

#define PRESET					0x00000010					// Keypad Process Has Preset Value, IC Card Should Process It When Nozzle On
#define ROUNDING				0x00000020					// Keypad Process Press Rounding Key, IC Card Should Process While Fuelling
#define EMP_LOG_ON				0x00000040					// Keypad Process Request IC Card Process Perform Employee Log ON
#define EMP_LOG_OFF				0x00000080					// Keypad Process Request IC Card Process Perform Employee Log Off

#define SELECT_APP				0x00000100					// Keypad Process Request IC Card Process Select Application
#define SELECT_PAY_MODE			0x00000200					// Keypad Process Request IC Card Process Select Pay Mode
#define AUTH_CODE				0x00000400					// Keypad Process Inform IC Card Process Auth Code Input Ok For Cash Trans
#define ICCARD_ENTER_KEY		0x00000800					// For Default Pin, IC Card Need Press ENTER Key

#define PRESET_MODE2			0x00001000					// Keypad Preset By Press ENTER For Sinopec Mode2
#define EMP_NUMBER			    0x00002000					// Input Employee Number

//Douzhq 20140528 Add , Lock Nozzle 
//This Two Flag Is For Keypad And Card Reader & Keypad And Keypad Process Exchange
#define UNLOCK_PASSWORD			0x00004000					// Input Unlock Nozzle Password
#define UNLOCK_PWD_FUEL			0x00008000					// Input Unlock Nozzle Password

// Keypad And Card Reader Process Exchange End

// Pump And Card Reader Process Exchange
#define PUMP_CALL 				0x00000001					// Pump Nozzle On, And Call For Auth
#define PUMP_AUTH				0x00000002					// Auth Ok For PUMP
#define PUMP_BUSY				0x00000004					// Pump Is Busy, Fuelling
#define PUMP_EOT				0x00000008					// Pump Nozzle Off, Fuelling End On Pump

#define PUMP_STOP				0x00000010					// IC Card Request Pump Stop
#define PUMP_LASTTRANS			0x00000020					// IC Card Need Last Transaction
#define PUMP_OFFLINE_TRANS		0x00000040					// Offline Trans, Need To Form Offline Tran Records
#define PUMP_TOTALS				0x00000080					// Request PUMP For All Totals, 4 At Most

#define PUMP_AUTH_CONFIRM		0x00000100					// Pump Auth Confirm
#define PUMP_ZERO    			0x00000200     				// PUMP ZERO Transaction
#define PUMP_VERIFY				0x00000400					// CR Need PUMP To Encrypted The Random
#define PUMP_VERIFY_ERROR		0x00000800					// CR Just Issued The Pump Verify, But Pump Fail To
															// Verify The Data, eg. Nozzle Off, Offline etc....
#define PUMP_RFU13				0x00001000					// Reserved For Future
// Pump And Card Reader Process Exchange End

// POS And Card Reader Process Exchange
#define POS_CHECK_BLACK			0x00000001					// POS Check Black List
#define POS_CHECK_WHITE			0x00000002					// POS Check White List
#define POS_CHECK_GREY			0x00000004					// POS Check Grey Record
#define PRICE_RECV				0x00000008					// POS Need IC Card Form A Price Receiving Record

#define SENDCARDIN				0x00000010					// IC������,����ʵʱ��Ϣ
#define SENDEOT					0x00000020					// ���յ����ͻ�EOT
#define TRANS_UPLOAD			0x00000040					// A Record Need To Upload To POS, While POS Is Online
															// A Record Can Not Be Saved In DataFlash, So Upload To POS
															// And Inform Error For Maintainance, The Trans Is Saved In	POS_UPLOAD
#define SEND_MESSAGE			0x00000080					// CR Inform POS To Send PSAM Verify Message To Host
#define AUTH_EMP				0x00000100					// Ա��������POS��Ȩ
#define AUTH_CST				0x00000200					// �û�������POS��Ȩ
#define AUTH_HOST				0x00000400					// ��̨��Ȩ
#define FUELLING_DATA			0x00000800					// �������,�ύ��������
#define HOST_STOP				0x00001000					// ��̨֪ͨ�ͻ�ֹͣ����
#define CARD_RECORD				0x00002000					// ����IC����ʷ��¼
#define AUTH_NCSS				0x00004000					// �ǿ���������POS��Ȩ
#define FUELLING_DATA_HOST		0x00008000					// �ύ��̨��Ȩ��������,ֻ��ӦACK, û��֧������
#define POS_RFU016				0x00010000					// Reserved For Future
// POS And Card Reader Process Exchange End

// Schedule And Card Reader Process Exchange
#define PRICEUPDATE_LOGOFF		0x00000001					// Price updating, Need IC Card Perform Log Off
#define PRICE_UPDATED			0x00000002					// Price Updated For Sinopec Mode2
#define SCH_CR_RFU2				0x00000004					// Reserved For Future
#define SCH_CR_RFU3				0x00000008					// Reserved For Future
// Schedule And Card Reader Process Exchange End

// Schedule And Pump Process Exchange
#define PRICE_UPDATE			0x00000001					// Price updating
#define SCH_PUMP_RFU1			0x00000002					// Reserved For Future
#define SCH_PUMP_RFU2			0x00000004					// Reserved For Future
#define SCH_PUMP_RFU3			0x00000008					// Reserved For Future
// Schedule And Pump Process Exchange End

// Printer And Pump Process Exchange
#define SENDPRINT				0x00000001					// A Record Need Pump Send To Remote Printer
#define PRINT_RFU1				0x00000002					// Reserved For Future
#define PRINT_RFU2				0x00000004					// Reserved For Future
#define PRINT_RFU3				0x00000008					// Reserved For Future

// Schedule And Card Reader Process Exchange End

// Keypad And Pump Process Exchange
#define KEYPAD_PUMP_ROUND		0x00000001					// ����֪ͨ�ͻ����Բ��
#define KEYPAD_PUMP_VOL_ROUND	0x00000002					// ����֪ͨ�ͻ���Բ��
#define KEYPAD_PUMP_STOPPUMP	0x00000004					// ����֪ͨ�ͻ���ͣ����
#define KEYPAD_PUMP_RESTART		0x00000008					// ����֪ͨ�ͻ���������

#define K_RFU1					0x00000010					// Reserved For Future

// Schedule And Card Reader Process Exchange End

// Card Reader And Voice Process Exchange
#define VOICE_CARD				0x00000001					// IC Card Insert For Voice
#define VOICE_NOZZLE_ON			0x00000002					// IC Card Nozzle On For Voice
#define VOICE_BUSY				0x00000004					// IC Card Busy For Voice
#define VOICE_EOT				0x00000008					// IC Card EOT For Voice
#define VOICE_RFU0				0x00000010					// RFU For Voice
#define VOICE_RFU1				0x00000020					// RFU For Voice
#define VOICE_RFU2				0x00000040					// RFU For Voice
#define VOICE_RFU3				0x00000080					// RFU For Voice
#define VOICE_NOZZLE_ERR		0x40000000					// Nozzle On But IC Card Not Inserted
#define VOICE_EXC				0x80000000					// IC Card Exception For Voice,

//Douzhq 20140916 Add , Voice Add
#define VOICE_CARDERR			0X00000100					// Card Expired Voice
#define VOICE_SCANCODE			0x00000200					// Please Sacn Code Voice
#define VOICE_VERERR			0X00000400					// Verification Code Error Voice
#define VOICE_VEROVER			0X00000800					// Verification Code Out Of Date Voice
#define VOICE_OILCHOOSE			0X00001000					// Oil Choose Error Voice
#define VOICE_FAULTERR			0x00002000					// Fault Error Voice
#define VOICE_ICPWD				0x00004000					// Please Input IC Card Password
// Card Reader And Voice Process Exchange End
															// Eg. EOT But IC Card Out, Such As GreyTrans

// Keypad And Voice Process Exchange
#define VOICE_VOLUME_UP			0x00000001					// Keypad Set Volume Up
#define VOICE_VOLUME_DOWN		0x00000002					// Keypad Set Volume Down
#define VOICE_VOLUME_RFU0		0x00000004					// Keypad Set RFU
#define VOICE_VOLUME_RFU1		0x00000008					// Keypad Set RFU

// Keypad And Voice Process Exchange End

// Pump And Voice Process Exchange
#define VOLUME_PUMP				0x00000001					// Volume Set To Pump
#define VOICE_PUMP				0x00000002					// Voice To Pump
// Pump And Voice Process Exchange End

// Card Reader And Cash Process Exchange
#define CASH_KEY_PRESSED		0x00000001					// CR-E Pressed The Key To Use Cash Function
															// The Key Number(Phisical Nozzle) Save In Exchange Buffer
#define CASH_INSERTED			0x00000002					// CR-E Inserted A New CashNote
#define CASH_TOTAL				0x00000004					// CR-E Accept All CashNotes Or POS Authed OK
#define CASH_CHECK_AMOUNT		0x00000008					// CR Need CR-E To Check The Amount In Some Specific Routine....
#define CASH_RFU1				0x00000010					// Reserve For Future
#define CASH_RFU2				0x00000020					// Reserve For Future
#define CASH_RFU3				0x00000040					// Reserve For Future
#define CASH_KEY_CANCELLED		0x00000080					// CR-E Cancelled The Apply For Use CR Cash Trans

#define CASH_CARD_READY			0x00000100					// Cash Card Ready For Cash Trans
															// Wether Accept Cash(Auth) Saved In Exchange Buffer
#define CASH_TRANS_RECORD		0x00000200					// Cash Trans End And The Trans Record Need Send To CR-E
#define CASH_AUTH_CODE			0x00000400					// CR Need Send Input Auth Code To CR-E
#define CASH_RFU4				0x00000800					// Reserve For Future

//Douzhq 20140721 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
#define CASH_CARE_RECOGNIZE		0x00001000					// IC Card Limte Car Number 
															// Card Reader And Cash Process Exchange
#define CASH_GRADE_lIMIT		0X00002000					// IC Card Grade Limit
															// Card Reader And Cash Process Exchang
#define CASH_CAR_ONCE 			0x00004000					// Car Limit Information Send Once
#define CASH_CODE_TIMER			0x00008000					// Make Timer Clear Flag CASH_AUTH_CODE
#endif

//Douzhq 20141028 Add , CR - CR Exchange
//-----------------------------------------Down-------------------------------------------------------
#ifdef MAKE_CASH_CR
#define CASH_CODE_ERR			0x00000001					// Cash Auth Code , Then CRE Send WarCode
#define CAR_GREY_FLAG			0x00000002					// Car Limite Number CR--CR Flag															// Set Flag
#endif
#define UNLOCK_POWERWD			0x00000004					// CR-CR Power Down
#define SAFEMODE_RECALL			0x00000008					// CR-CR Safe Mode Recall
//----------------------------------------- Up -------------------------------------------------------
// Card Reader  Cash Process Exchange End

// Define The Bits Used In ProcExchange	End





// Define The Bits Used In ProcStatus
// Define The Status Bits In ProcStatus[PROCESS_PUMP], Must Same As PUMP_C_**** In Pump.h
#define PUMP_ONLINE				0x10000000					// PUMP Is Online And Working
#define PUMP_IDLE				0x10000100					// PUMP Is Online And Idle
#define PUMP_CALL_E				0x10000200					// PUMP Call But IC Card Not Inserted
// Define The Status Bits In ProcStatus[PROCESS_PUMP] End

// Define The Status Bits In ProcStatus[PROCESS_POS]
#define POS_ONLINE				0x00000001					// POS Is Online And Working
// Define The Status Bits In ProcStatus[PROCESS_POS] End

// Define The Status Bits In ProcStatus[PROCESS_CR]
#define CARD_READY				0x00000001					// IC Card Ready For Fuelling
#define CARD_SINOPEC_APP		0x00000002					// IC Card Support Sinopec Application
#define CARD_LOYALTY_APP		0x00000004					// IC Card Support Loyalty Application
#define CARD_SINOPEC_APP_BLOCK	0x00000008					// IC Card Support Sinopec Application, But Was Blocked

#define CARD_LOYALTY_APP_BLOCK	0x00000010					// IC Card Support Loyalty Application, But Was Blocked
#define CARD_BUSY				0x00000020					// IC Card Is Busy, In Fuelling
#define CARD_LOGON				0x00000040					// IC Card Employee Is Logged On
#define CARD_INSERTED			0x00000080					// IC Card Just Inserted, Can Not Justify The IC Card Status

#define CARD_WAITING			0x00000100					// IC Card Refresh Information, But Do Not Spring Out Card,
															// Display The Waiting Screen

#define CARD_CASH_OK			0x00000200					// For Cash Trans, Cash Card OK, And Can Accept Cash
#define CARD_CASH_ACCEPTED		0x00000400					// For Cash Trans, Cash Accept( POS Auth ) OK
#define CARD_WAIT_CASH_PRINT	0x00000800					// For Cash Trans, Wait Keypad Select Pint Receipt

#define CARD_AUTHED				0x00001000					// IC Card Was Authed, Do Not Change The Auth Method, Special For Sinopec Mode2
#define CARD_OVER_FURLING				0x00002000					// For over  fueling processs
#define CARD_OVER_FURLING_PASSWORD		0x00004000					// For over  fueling processs
#define CARD_RFU15				0x00008000					// Reserve For Future

#define CARD_ERROR				0x80000000					// IC Card Is Error, Can Not Connect Card Reader Or Do Anything

//Douzhq 20140610 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
#define CARD_SAVEOIL_APP		0x00010000					// IC Card Support SaveOil Application	
#define CARD_SAVEOIL_APP_BLOCK  0x00020000					// IC Card Support SaveOil Application, But Was Blocked
#endif
//Douzhq 20140926 Add , Config Printer Set
#define CARD_WAIT_PRINT	        0x00040000							// IC Card Wait For Printer Select

// Define The Status Bits In ProcStatus[PROCESS_CR]	End

// Define The Status Bits In ProcStatus[PROCESS_KEYPAD]
#define KEYPAD_OK				0x00000001					// Keypad Is Working
// Define The Status Bits In ProcStatus[PROCESS_KEYPAD] End

// Define The Status Bits In ProcStatus[PROCESS_DISPLAY]
#define DISPLAY_OK				0x00000001					// Display Is Working
// Define The Status Bits In ProcStatus[PROCESS_DISPLAY] End

// Define The Status Bits In ProcStatus[PROCESS_POWERDOWN]	// System PowerDown
// System Is Power Down, Need Other Process Return
#define POWERDOWN				0x00000001
// Define The Status Bits In ProcStatus[PROCESS_POWERDOWN] End

// Define The Status Bits In ProcStatus[PROCESS_SCHEDULE]
#define PRICE_UPDATING			0x00000001					// Need Update Price
// Define The Status Bits In ProcStatus[PROCESS_SCHEDULE] End

// Define The Status Bits In ProcStatus[PROCESS_PRINT]
#define PRINT_READY				0x00000001					// Printer Is Ready And Can Print Receipt
#define PRINT_BUSY				0x00000002					// Printer Is Busy In Printing
#define PRINT_ERROR				0x80000000					// Printer Is Error Or No Printer
// Define The Status Bits In ProcStatus[PROCESS_PRINT] End
// Define The Bits Used In ProcStatus End

// Process Status, For Other Process Inquire And Acess
// Maintain By The Corresponding Process
// While Inquire By Other Process
extern unsigned int ProcStatus[PROCESS_MAX];

// Process Message Exchange,
// Line = Source; Colomn = Destination
// Source Process Sets The Certain Bit,
// While Destination Process Clear The Set Bit
extern unsigned int ProcExchange[PROCESS_MAX][PROCESS_MAX];




//	PROCESS_CR��PROCESS_POS�������壺		
//	�ṹ:		
typedef struct		
{		
	unsigned char CheckFlag;					// ƥ���־	(��MODE2�д洢������־, = 2:��̨��ʱ,...)
	unsigned char ICCARD_ASN[10];				// Ӧ�����к� (��MODE2�д濨��,����Ա��,�ǿ�����ŵ�,...)
	unsigned char ICCARD_Balance[4];			// MSB	���
	unsigned char TransAmount[4];				// MSB	���׽�� (��MODE2�д洢��Ȩ��3���ֽڳ���)

	unsigned char ICCARD_STATE[2];				// IC ��״̬
	unsigned char P_UNIT;                       // ���㵥λ

	unsigned char TransNozzleNumber;			// ��ǰ����ǹ��
	unsigned char TransPrice[2];				// MSB	��ǰ���͵���
	unsigned char TransVolume[4];				// MSB	��ǰ�������� (��MODE2�д洢��Ȩ������3���ֽڳ���)


//#ifdef MAKE_CARD_MODE2

// Define The Buffer For Mode2

	unsigned char Auth_Mode;					// ��ǰ��Ȩģʽ
	unsigned char Auth_TTC[4];					// ������Ȩ���׵�TTC
	unsigned char Auth_Date[4];					// ������Ȩ����
	unsigned char Auth_Time[3];					// ������Ȩʱ��
	unsigned char Auth_Pay_X;					// ������Ȩ���ʽ
	unsigned char Auth_Add_On[30];				// ������Ȩ������Ϣ	(��Ȩ��Ӧ��,�洢��Ӧ������Ϣ:����+����)
	unsigned char Auth_PIN_Block[16];			// ������ȨPIN����
	unsigned char Auth_Resp;					// ������Ȩ���ؽ���ֽ�
	unsigned char Auth_MAC[4];					// ������ȨMAC

	unsigned char Grade_Number[2];				// ������Ʒ����
	unsigned char Card_CTC[2];					// ���Ϳ�����ˮ��
	unsigned char VolumeTotal[4];				// ��������

	unsigned char TransAmount1[4];				// ʵ��֧�����1��IC����
	unsigned char TransAmount2[4];				// ʵ��֧�����2����̨�����ʻ���
	unsigned char DiscountNumber[2];			// �Żݹ���

// Define The Buffer For Mode2 End

//#endif	// MAKE_CARD_MODE2

	unsigned char InforMessage[128];			// MSB	��Ҫ����POS��̨����Ϣ (MODE2��,���Ϳ���¼���5��ÿ��)
}POS_CR;		

extern POS_CR POS_CR_Buf;


typedef struct
{
	unsigned char UploadTrans[96];
}POS_UPLOAD;

extern POS_UPLOAD POSUploadTrans;

//	PROCESS_CR��PROCESS_KEYPAD�������壺		
//	�ṹ:		
typedef struct		
{		
    union		
   {	   										// ����򳵺����볤�ȣ��������ȡ�����򳤶�Ϊ0
        unsigned char ICCardPINLen;	   			// IC�����볤��
        unsigned char ICCardEMPPwdLen;			// Ա�����볤��
        unsigned char PresetMode;				// Ԥ��ģʽ��0�����Ԥ�ã�1����Ԥ��
        unsigned char APPType;					// ѡ��Ӧ�ã�0��ѡ����ʯ��Ӧ�ã�1��ѡ�����Ӧ��
												// 2: ѡ�����Ӧ��
        unsigned char PayMode;					// ѡ��֧����ʽ�� 0���ֽ� 1����Ʊ
        unsigned char FuelEmpNumberLen;         // ����Ա���ų���
    };		

    union		
    {		
        unsigned char ICCardPIN[8];				// IC������
        unsigned char ICCardEMPPwd[4];			// Ա������
        unsigned char PresetValue[4];			// MSB	Ԥ��ֵ
     };		
}KEYPAD_CR;		

extern KEYPAD_CR KEYPAD_CR_Buf;


//	PROCESS_CR��PROCESS_DISPLAY�������壺		
//	�ṹ:		
typedef struct		
{		
	unsigned char LogicalNozzle;			// �߼�ǹ��
	unsigned char PhiscalNozzle;			// ����ǹ��
	unsigned char VolumeTotal[4];			// ��ǹ��������			HEX, MSB
	unsigned char AmountTotal[4];			// ��ǹ�Ž������		HEX, MSB
	unsigned char DutyVolumeTotal[4];		// ��ǹ�ŵ���������		HEX, MSB
	unsigned char DutyAmountTotal[4];		// ��ǹ�ŵ���������	HEX, MSB

#ifdef MAKE_CASH_CR
	unsigned char DutyCashVolumeTotal[4];	// ��ǹ�ŵ����ֽ�������		HEX, MSB
	unsigned char DutyCashAmountTotal[4];	// ��ǹ�ŵ����ֽ�������	HEX, MSB
#endif

}TOTALS;		

//extern TOTALS DispTotal;

//	�ṹ:		
typedef struct		
{		
//	unsigned char BalUpdateFlag;			// 1: ���������ʾ; 0: ������
//	unsigned char AppType;					// Ӧ�����ͣ�0:��ʯ��Ӧ��; 1:����Ӧ��
//	unsigned char EMPNumber;				// ǩ��Ա����: 0xFF: δǩ��
//	unsigned char VehicleNumber[16];		// ����:�޳��ſ��ĳ���
//	unsigned char CardHolderName[20];		// �ֿ�������
	
	TOTALS DispTotal[4];					// ���ۼ�
}DISP_CR;		

extern DISP_CR DISP_CR_Buf;


//	PROCESS_CR��PROCESS_PUMP�������壺		

//	�ṹ:		
typedef struct		
{		
	unsigned char PresetMode;					// Ԥ�÷�ʽ��0�����Ԥ�ã�1����Ԥ��
	unsigned char PresetValue[4];               // MSB	Ԥ��ֵ
	unsigned char NozzleNumber;					// ����ǹ��
	unsigned char Volume[4];					// MSB	����
	unsigned char Amount[4];					// MSB	���
	unsigned char VTotal[4];                    // MSB  ���ʽ����ۼ�
	unsigned char Price[2];						// MSB	����

	unsigned char VolumeTotal[4*4];				// MSB	���ۼ�
	unsigned char AmountTotal[4*4];				// MSB	����ۼ�

	unsigned char OfflineAmount[4*4];			// MSB	���߽��׽��
	unsigned char OfflineVolume[4*4];			// MSB	���߽�����
        
	unsigned char MaxFuellingVolume[4];			// MSB  ��������������

//#ifdef MAKE_PSAM_VERIFY

	unsigned char PumpVersion[4];				// MSB  �ͻ�����汾��
	unsigned char PSAMRandom[4];				// MSB  PSAM�����
	unsigned char DESRandom[8];					// MSB  �ͻ����ܺ����������
	unsigned char PUMPSerialNumber[8];			// MSB  �ͻ����кţ��������̱�ʶ��
	unsigned char PumpKey[16];					// MSB  �ͻ���Կ

//#endif

}PUMP_CR;		

extern PUMP_CR Pump_CR_Buf;


//	�ṹ:		
typedef struct		
{
	unsigned char PrinterData[100];				// Printer Data Send To Remote CR For Print
}PUMP_PRINT;

extern PUMP_PRINT PumpPrinterBuf;

//	�ṹ:		
typedef struct		
{
	unsigned char VolumeDefault;				// Current Volume, Retrived From Voice Board
	unsigned char Voice;						// Voice Flag
	unsigned char VoiceData[16];				// MSB; Content Of Voice
}PUMP_VOICE;

extern PUMP_VOICE PumpVoiceBuf;

//	�ṹ:		
typedef struct		
{
	unsigned short int NextPrice;				// Next Price For Price Update
}PUMP_PRICE;

extern PUMP_PRICE PumpPriceBuf[4];


//	PROCESS_CR��PROCESS_CASH�������壺		

//	�ṹ:		
typedef struct		
{		
	unsigned char CashType;						// �豸����: 0x00: �ֽ�; 0x01: POS��Ȩ
	unsigned char CashVersion[20];              // CR-E����汾��
	unsigned char CashKeyNozzleNumber;			// ����ѡ���ǹ��(����ǹ��)
	unsigned char CashCard_Flag;				// �Ƿ���Կ�ʼռ��CR: 0x00: ����ռ��; 0x01����ռ��
	unsigned char CashNozzleNumber;				// CRѡ����߼�ǹ��
	unsigned char CashGradeNumber[2];           // MSB  CRѡ�����Ʒ���
	unsigned char CashTotal[4];					// MSB CR-E�ܹ����ܵĽ�����Ȩ����Ľ��

	unsigned char CashInserted[2];				// MSB CR-E �ս��ܵ�һ��ֽ�ұ�ֵ: Ԫ
	unsigned char CashLists[2];					// MSB CR-E �ܹ�����ֽ������: ��
	unsigned char CashTotalInserted[2];			// MSB CR-E�ܹ�����ֽ�ҽ��: Ԫ
	
//	unsigned char CashAuthCodeLen;			// MSB CR-E���ܵ���Ȩ���CR�������Ȩ�볤��

	union
	{
		unsigned char CashAuthCode[13];			// MSB CR-E���ܵ���Ȩ���CR�������Ȩ��; ��һ���ֽ�Ϊ��Ȩ�볤��
		unsigned char CashTransRecord[96];		// ���׼�¼, �͵�CR-E
	};
}CASH_CR;		

extern CASH_CR Cash_CR_Buf;



// �����ѯ�Ҽ�¼�ṹ
typedef struct		
{		
	unsigned char GreyCardASN[10];				// �ҿ����к�
	unsigned char GreyCardBalance[4];			// MSB	�ҿ����
	unsigned char GreyCardCTC[2];				// MSB	�ҿ�CTC
	unsigned char GreyCardDebitSource;			// �ҿ��ۿ���Դ
	unsigned char GreyCardDate[4];				// MSB	�ҿ�����
	unsigned char GreyCardTime[3];				// MSB	�ҿ�ʱ��

}GREY_INQUIRY;		

extern GREY_INQUIRY GreyInquiry;

// ������Ӧ��ѯ�Ҽ�¼�ṹ
typedef struct		
{		
	unsigned char CheckFlag;					// ��ѯ��־: 0: ��ѯ����Ӧ�Ҽ�¼;  1: û����Ӧ�Ҽ�¼

	unsigned char GreyCardASN[10];				// �ҿ����к�
	unsigned char GreyCardBalance[4];			// MSB	�ҿ����
	unsigned char GreyTransAmount[3];			// MSB	�Ҽ�¼���׽��
	unsigned char GreyCardCTC[2];				// MSB	�ҿ�CTC
	unsigned char GreyCardDebitSource;			// �ҿ��ۿ���Դ
	unsigned char GreyCardDate[4];				// MSB	�ҿ�����
	unsigned char GreyCardTime[3];				// MSB	�ҿ�ʱ��
	unsigned char GreyTransGMAC[4];				// �Ҽ�¼GMAC
	unsigned char GreyPSAMTID[6];				// �Ҽ�¼PSAM�ն˱��
	unsigned char GreyPSAMTTC[4];				// �Ҽ�¼PSAM TTC

}GREY_RESPONSE;		

extern GREY_RESPONSE GreyResponse;

//Douzhq 20140723 Add , Car Number Recognize
// CR -- CRE Exchange Buffer
#ifdef MAKE_CASH_CR
enum CheckCarRecFlag
{
	Cmd_Checking,							//Recognizing Car Number/Limit Grade
	Cmd_CheckMatch,							//Car Number/Limit Grade is Match
	Cmd_CheckNotMatch,						//Car Number/Limit Grade is not Match
	Cmd_CheckTimeOut,						//Car Number/Limit Grade Check Time Out
	Cmd_CheckWrong,							//Car Number/Limit Grade 's Sending and Recving is not Match
	Cmd_CheckWrongErr,						//Car Number/Limit Grade 's Sending and Recving is not Match Twice
											//Car Number/Limit Grade Resend Number is 2 
};

typedef struct
{
	enum CheckCarRecFlag CarRecFlag;
		
	unsigned char CardAsn[10];
	unsigned char CarNumber[16];
	unsigned char NozzleNumber;
	
	unsigned char LimitGrade[2];
	unsigned char PumpGrade[2];
}CARNUMBER_GRADE_APP;

//Douzhq 20140723 Add , Car Number Recognize
extern CARNUMBER_GRADE_APP CarNumGradeBuffer;
#endif

#define GET_GMAC_RETRY_TIMES 0x04

extern unsigned short int PSAMCertificateGreyLockRetval;

extern unsigned char CertiMAC2LogType;


// For Test
//extern unsigned char BlackFit;			// 0: Fitted; 1: Unfitted; 2: Error
//extern unsigned char WhiteFit;			// 0: Fitted; 1: Unfitted; 2: Error
//extern unsigned char GreyFit;			// 0: Fitted; 1: Unfitted; 2: Error
// For Test

//Douzhq 20140528 Add , Lock Nozzle 
typedef struct 
{
	unsigned char LockFlag;							// Lock Nozzle flage
}LOCK_NOZZLE_STR;

extern LOCK_NOZZLE_STR LOCK_NOZZLE;

#endif
