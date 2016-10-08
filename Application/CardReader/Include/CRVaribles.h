#ifndef _CRVARIBLES_H
#define _CRVARIBLES_H

#include "tc.h"


#define CARD_AUTH_V				1000				// Changed To 1000 According To Sinopec New Specification 2011-08-19
//#define CARD_AUTH_V				1100				// Reserved 1100ml For IC Card Auth Condition, 1.1Litre Volume
													// Means: For A New Auth, Only The IC Card Balance > 1.1L * Price
													// Then The Auth Can Be Issued
													// Attention: Difference With CARD_RESERVE_V

#define CARD_RESERVE_V			0					// Changed To 0 According To Sinopec New Specification 2011-08-19
													// 
//#define CARD_RESERVE_V			100					// Reserved 100ml For IC Card, 0.1Litre Volume
													// Different With CARD_AUTH_V

#define CARD_PRESET_MAX			990000				// Max Preset Value For IC Card, Both Volume And Amount
// For Test
//#define CARD_PRESET_MAX			99000				// Max Preset Value For IC Card, Both Volume And Amount
// For Test
#define BlackListMax			20000				// Max Black list Items

#define RecordMax				2048				// Max Record Space, 2048
#define ReserveRecordSpace		48					// Reserved Record Space, If The Un-Uploaded Journal Exceed
													// 2048 - ReserveRecordSpace, The Do Not Allow Fuelling
#define ReserveRecordSpaceMin	16					// Before Save Record, If The Un-Uploaded Journal Exceed
													// 2048 - ReserveRecordSpaceMin, Then Do Not Allow Save

// Below Define The Bits Used In CRPowerFlag
#define CRPOWERON				0x01				// Power On Flag, Set In Init CR Process
#define CRPOWERDOWN				0x02				// Power Down Flag, Set In CR Power Down Process
#define CRPUMPSTOP				0x04				// Pump Stop Command Issued, Used To Mark The 
													// Pump Stop Issued Or Not, While Card Busy And Debit Unlock On Power Down

// Special, For A Trans Can Not Completed
// And Format The Config Area
// Need To DEC TTC
// And The Un-Completed Trans Will Be Lost
// The Volume Totals Will Be Not Continous
// But The System Can Running Again
#define	ReCallTTC	0x80

// Define The Bits Used In CRPowerFlag End



#define MODE_EMV				0x30
#define MODE_7816				0x31

#define PSAM1 					0x30
#define PSAM2 					0x31
#define PSAM3 					0x32
#define PSAM4 					0x33

#define ICCARD2					PSAM4



#define PSAM_30V				0x2F
#define PSAM_50V				0x30

#define PSAM_COMMAND_LEADING	0x3D


#define CR_COMM_ERR_MAX_TIMES	0x03				// Max Retry Times For Communication

// Define Card Type, Same As ASN[2]
#define CUSTOMER_CARD			0x01				// Customer Card
#define MANAGE_CARD				0x02				// Manage Card
#define EMPLOYEE_CARD			0x04				// Employee	Card
#define TEST_CARD				0x05				// Test Card
#define	MAINTAINANCE_CARD		0x06				// Maintainance Card
#define	PSAM_CARD				0x10				// PSAM Card

#define CARD_LOG_LEN			23					// Card Log Lenght
#define CARD_LOG_ITEM			5					// Items For Card Log To Host Every Package, Maybe Limited By POS_CR_BUF Lengh


#ifdef MAKE_CARD_MODE2

// Use To Check Sinopec Mode2 Card Or Not
// In TR_CARD_HOLDER_NEW_FLAG
#define	CARD_MODE2_NON			0xC0				// NotSinopec Mode2 Card
#define	CARD_MODE2_2			0x80				// Sinopec Mode2 Card Type 2
#define	CARD_MODE2_1			0x40				// Sinopec Mode2 Card Type 1

// Used To Check Grade Limited Extend
// In TR_GRADE_LIMITED_EXTEND[3]
#define	PETROL_RFU0				0x80				// Petrol Limit Byte RFU0 TR_GRADE_LIMITED_EXTEND[0]
#define	PETROL_ALL				0x40				// Petrol Limit Byte All Petrol TR_GRADE_LIMITED_EXTEND[0]
#define	PETROL_90				0x20				// Petrol Limit Byte 90 Petrol TR_GRADE_LIMITED_EXTEND[0]
#define	PETROL_93				0x10				// Petrol Limit Byte 93 Petrol TR_GRADE_LIMITED_EXTEND[0]
#define	PETROL_95				0x08				// Petrol Limit Byte 95 Petrol TR_GRADE_LIMITED_EXTEND[0]
#define	PETROL_97				0x04				// Petrol Limit Byte 97 Petrol TR_GRADE_LIMITED_EXTEND[0]
#define	PETROL_98				0x02				// Petrol Limit Byte 98 Petrol TR_GRADE_LIMITED_EXTEND[0]
#define	PETROL_RFU1				0x01				// Petrol Limit Byte RFU1 TR_GRADE_LIMITED_EXTEND[0]

#define	DIESEL_RFU0				0x80				// Diesel Limit Byte RFU0 TR_GRADE_LIMITED_EXTEND[1]
#define	DIESEL_AIR				0x40				// Diesel Limit Byte AirPlane Diesel TR_GRADE_LIMITED_EXTEND[1]
#define	DIESEL_NORM				0x20				// Diesel Limit Byte Normal Diesel TR_GRADE_LIMITED_EXTEND[1]
#define	DIESEL_LGT				0x10				// Diesel Limit Byte Light Diesel TR_GRADE_LIMITED_EXTEND[1]
#define	DIESEL_HEV				0x08				// Diesel Limit Byte Heavy Diesel TR_GRADE_LIMITED_EXTEND[1]
#define	DIESEL_MIL				0x04				// Diesel Limit Byte Military Diesel TR_GRADE_LIMITED_EXTEND[1]
#define	DIESEL_RFU1				0x02				// Diesel Limit Byte RFU1 TR_GRADE_LIMITED_EXTEND[1]
#define	DIESEL_RFU2				0x01				// Diesel Limit Byte RFU2 TR_GRADE_LIMITED_EXTEND[1]


#endif






#ifdef MAKE_PSAM_VERIFY

#define ACT_CARD				0x50				// ACT Card
#define RID_CARD				0x51				// RID Card
#define VROOT_CARD				0x52				// VRoot Card



extern const unsigned char GilbarcoID[2];

extern unsigned char RegisterStep;
extern unsigned char ACTRegisterStep;

#endif


#define INQUIRY_RECEIPT					0x00				// Receipt Type For Print, Inquiry Receipt
#define TRANS_RECEIPT					0x01				// Receipt Type For Print, Trans EOT Receipt
#define INQUIRY_CASH_ACCEPTOR_RECEIPT	0x02				// Receipt Type For Print, Cash Acceptor Trans Inquiry Receipt
#define CASH_ACCEPTOR_RECEIPT			0x03				// Receipt Type For Print, Cash Acceptor Trans EOT Receipt
#define INQUIRY_CASH_POS_RECEIPT		0x04				// Receipt Type For Print, Cash POS Trans Inquiry Receipt
#define CASH_POS_RECEIPT				0x05				// Receipt Type For Print, Cash POS Trans EOT Receipt

//Douzhq 20140921 Add , Config Printer Set
#define SHORT_RECEIPT					0x06				// Receipt Type For Print, Trans EOT Short Receipt 

enum CR_TYPE
{
	CR_TYPE_START,									// For Start

	CR_TYPE_CARD,									// Only Sinopce IC Card, Do Not Support Cash Function

#ifdef MAKE_CASH_CR

	CR_TYPE_CASH,									// Both Sinopec IC Card And Sinopec Cash Function

#endif

	CR_TYPE_MAX,									// For End

};

extern enum CR_TYPE CRType;
// The IC Card Command Response Running Flag Offset In RecvBuffer
extern unsigned char RespCMDOffset;
// The IC Card Command Response Data Offset In RecvBuffer
extern unsigned char RespDATOffset;

#define SPRINGOUTTIMES	  0x04							// Max Spring Out Card Times Retries....
#define CRCOMMERRTIMES    0x02
#define GETGMACRETRYTIMES 0x05
// Spring Out IC Card Times
extern unsigned char SpringOutCardTimes;
extern unsigned char CRCardCommErrTimes;
extern unsigned char GetGMACRetryTimes;
extern unsigned char SaveRecordFailFlag;

// Save Transaction Data
typedef struct
{
	unsigned char TR_CR_VERSION[16];				// Card Reader Hw/Sw Version

	// Save The PSAM Messages
	// 0. Save The Contents Of ATR
	unsigned char TR_PSAM_ATR[32];					// PSAM ATR
	// 0. END

	// 1. Save The Contents Followed 9F0C,
	// 		After Select PSE
	unsigned char TR_PSAM_ASN[10];					// PSAM ASN
	unsigned char TR_PSAM_VERSION;					// PSAM VERSION
	unsigned char TR_PSAM_KEY_TYPE;					// PSAM Key Type
	unsigned char TR_PSAM_COMMAND_VERSION;			// PSAM Command Version
	unsigned char TR_PSAM_PSE_FCI_RFU;				// PSAM RFU After Select PSE
	unsigned char TR_PSAM_TID[6];					// PSAM Terminal ID
	// 1. END
	
	// 2. Save The Contents Followed 9F0C,
	//		After Select SINOPEC1
	unsigned char TR_PSAM_KEY_INDEX;				// PSAM Key Index For Fuelling
	unsigned char TR_PSAM_ISSUER_ID[8];				// PSAM Issuer ID
	unsigned char TR_PSAM_ACQUIRE_ID[8];			// PSAM Acquirer ID
	unsigned char TR_PSAM_VALIDATE_DATE[4];			// PSAM Validate Date, Application Start Date
	unsigned char TR_PSAM_EXPIRE_DATE[4];			// PSAM Expire Date
	
	unsigned char TR_PSAM_CHECK_CRC[2];				// Save PSAM Information CRC
													// From TR_PSAM_ATR To TR_PSAM_EXPIRE_DATE

	// 2. END
	
	// 3. Save The Response For PSAM Init Grey Lock
	unsigned char TR_PSAM_TTC[4];					// PSAM Transaction TTC
	unsigned char TR_PSAM_RANDOM[4];				// PSAM Random for This Transaction
	unsigned char TR_PSAM_MAC1[4];					// PSAM MAC1
	// 3. END

	// 4. Save The Response For PSAM Credit Grey Debit
	unsigned char TR_PSAM_GMAC[4];					// PSAM GMAC
	unsigned char TR_PSAM_SAMTAC[4];				// PSAM TAC
	// 4. END

	// 5. Save The Response For PSAM DES Crypt
	unsigned char TR_PSAM_TRANS_MAC[4];				// PSAM Crypt Result, For Transaction Journal TAC
	// 5. END


#ifdef	MAKE_PSAM_VERIFY
	unsigned char TR_PSAM_ANTIFRAUD_TIMES;			// For Anti Fraud Function, Save The Allowed AntiFrand Verify Times
	unsigned char TR_PSAM_ACT_RID_RANDOM[4];		// For Anti Fraud Function, Save The PSAM Random( Get Challenge Command Response )
#endif

	
	// Save The IC Card Messages
	// 6. Save The Content Of IC Card ATR
	unsigned char TR_CARD_ATR[32];					// IC Card ATR
	// 6. END

	// 7. Save The Contents of SFI = 0x15
	//		Followed Select Sinopec_APP
	//		Or Select Loyalty_APP
#ifdef MAKE_PSAM_VERIFY
	unsigned char TR_CARD_ISSUER_ID[8];				// IC Card Issuer ID
	unsigned char TR_CARD_APP_TYPE;					// IC Card Application Type
	unsigned char TR_CARD_APP_VERSION;				// IC Card Application Version
	unsigned char TR_CARD_ASN[10];					// IC Card Application ASN
	unsigned char TR_CARD_APP_VALIDATE_DATE[4];		// IC Card Application Validate Date, Application Start Date
	unsigned char TR_CARD_APP_EXPIRY_DATE[4];		// IC Card Application Expire Date
	unsigned char TR_CARD_INSTRUCTION_VERSION;		// IC Card Instruction Version
	unsigned char TR_CARD_SFI_21_RFU;				// IC Card SFI 0x15 RFU

	// Save RID
	unsigned char TR_RID_CARD_ISSUER_ID[8];			// IC Card Issuer ID
	unsigned char TR_RID_CARD_APP_TYPE;				// IC Card Application Type
	unsigned char TR_RID_CARD_APP_VERSION;			// IC Card Application Version
	unsigned char TR_RID_CARD_ASN[10];				// IC Card Application ASN
	unsigned char TR_RID_CARD_APP_VALIDATE_DATE[4];	// IC Card Application Validate Date, Application Start Date
	unsigned char TR_RID_CARD_APP_EXPIRY_DATE[4];	// IC Card Application Expire Date
	unsigned char TR_RID_CARD_INSTRUCTION_VERSION;	// IC Card Instruction Version
	unsigned char TR_RID_CARD_SFI_21_RFU;			// IC Card SFI 0x15 RFU
#else
	unsigned char TR_CARD_ISSUER_ID[8];				// IC Card Issuer ID
	unsigned char TR_CARD_APP_TYPE;					// IC Card Application Type
	unsigned char TR_CARD_APP_VERSION;				// IC Card Application Version
	unsigned char TR_CARD_ASN[10];					// IC Card Application ASN
	unsigned char TR_CARD_APP_VALIDATE_DATE[4];		// IC Card Application Validate Date, Application Start Date
	unsigned char TR_CARD_APP_EXPIRY_DATE[4];		// IC Card Application Expire Date
	unsigned char TR_CARD_INSTRUCTION_VERSION;		// IC Card Instruction Version
	unsigned char TR_CARD_SFI_21_RFU;				// IC Card SFI 0x15 RFU
#endif
	// 7. END
	
	// 8. Save The Contents Of SFI = 0x16
#ifdef MAKE_PSAM_VERIFY
	union
	{
		unsigned char TR_CARD_HOLDER_FLAG;			// IC Card Type: 1: Single Customer Card; 2: Multi Customer Card
		unsigned char TR_ACT_KEY_INDEX;				// ACT Card Key Index
	};
	union
	{
		unsigned char TR_CARD_SINOPEC_EMP;			// 0: IC Card Customer Is Sinopce Employee
		unsigned char TR_ACT_RID_RFU;				// ACT Card Reserved
	};
	unsigned char TR_CARD_CUSTOMER_NAME[20];		// IC Card Customer Name
	unsigned char TR_CARD_CUSTOMER_LICENSE[18];		// IC Card Customer License
	unsigned char TR_CARD_CUSTOMER_LICENSE_TYPE;	// IC Card Customer License Type

	unsigned char TR_RID_KEY_INDEX;					// ACT Card Key Index
	unsigned char TR_RID_KEY_VERSION;				// ACT Card Reserved
	unsigned char TR_RID_CARD_CUSTOMER_NAME[20];	// IC Card Customer Name
	unsigned char TR_RID_CARD_CUSTOMER_LICENSE[18];	// IC Card Customer License
	unsigned char TR_RID_CARD_CUSTOMER_LICENSE_TYPE;// IC Card Customer License Type


#else
	unsigned char TR_CARD_HOLDER_FLAG;				// IC Card Type: 1: Single Customer Card; 2: Multi Customer Card
	unsigned char TR_CARD_SINOPEC_EMP;				// 0: IC Card Customer Is Sinopce Employee
	unsigned char TR_CARD_CUSTOMER_NAME[20];		// IC Card Customer Name
	unsigned char TR_CARD_CUSTOMER_LICENSE[18];		// IC Card Customer License
	unsigned char TR_CARD_CUSTOMER_LICENSE_TYPE;	// IC Card Customer License Type
#endif
	// 8. END

	// 9. Save The Contents Of SFI = 0x1B
	unsigned char TR_CARD_PIN_DEFAULT;				// IC Card Use Default PIN; 0: Use Default PIN 9999; 1: Use Customer PIN
	unsigned char TR_CARD_EMP_NUMBER;				// IC Card Employee Number, Only For Employee Card
	unsigned char TR_CARD_EMP_PASSWORD[2];			// Employee Password, Only For Employee Card
	unsigned char TR_CARD_SFI_27_RFU[28];			// IC Card SFI 0x1B RFU
	// 9. END

	// 10. Save The Contents Of SFI = 0x1C
	unsigned char TR_CARD_GRADE_LIMIT[2];			// IC Card Grade Limited; Only Fuelling The Listed Grade Product
	unsigned char TR_CARD_LOCATION_LIMIT_MODE;		// IC Card Fuelling Location Limited Mode
	unsigned char TR_CARD_LOCATION_LIMIT[40];		// IC Card Fuelling Location Limited
	unsigned char TR_CARD_VOLUM_AMOUNT_LIMIT[2];	// IC Card Fuelling Amount Or Volume Limit Per Fuelling
	unsigned char TR_CARD_TIMES_DAY_LIMIT;			// IC Card Fuelling Times Limit Per Day
	unsigned char TR_CARD_AMOUNT_DAY_LIMIT[4];		// IC Card Fuelling Amount Limit Per Day
	unsigned char TR_CARD_VEHICLE_NUMBER[16];		// IC Card Fuelling Vehicle Limit

#ifdef MAKE_CARD_MODE2

	unsigned char TR_NON_OIL_FLAG;					// Sinopec Mode2: Non Oil Type Allow Or Not
	unsigned char TR_NON_OIL_TYPE[3];				// Sinopec Mode2: Non Oil Type List
	unsigned char TR_CARD_HOLDER_NEW_FLAG;			// Sinopec Mode2: Card Holder Flag
	unsigned char TR_CARD_HOLDER_BIRTHDAY[2];		// Sinopec Mode2: Card Holder Birthday
	unsigned char TR_CARD_HOLDER_TYPE[2];			// Sinopec Mode2: Card Holder Type
	unsigned char TR_CARD_HOLDER_EXPIRY_DATE[2];	// Sinopec Mode2: Card Holder Expiry Date
	unsigned char TR_CRM_CLASS;						// Sinopec Mode2: CRM Card Holder Class
	unsigned char TR_GRADE_LIMITED_EXTEND[3];		// Sinopec Mode2: Grade Limit Extend Chars
	unsigned char TR_INVOICE_FLAG;					// Sinopec Mode2: Invoice/Receipt Flag
	unsigned char TR_HOST_ACCOUNT_FLAG;				// Sinopec Mode2: Host Account Allow Or Not
	unsigned char TR_PUBLIC_RFU1;					// Sinopec Mode2: Public Rfu1
	unsigned char TR_PUBLIC_RFU2;					// Sinopec Mode2: Public Rfu2
	unsigned char TR_PUBLIC_RFU3;					// Sinopec Mode2: Public Rfu3

	unsigned char TR_CARD_SFI_28_RFU[10];			// Sinopec Mode2: RFU

#else

	unsigned char TR_CARD_SFI_28_RFU[30];			// IC Card SFI 0x1C RFU

#endif	// MAKE_CARD_MODE2
	unsigned char TR_VEHICLE_NUMBER_LIMITED_FLAG;	// Added, Not In The IC Card File SFI = 0x1C
													// Just For Program Process For The Vehicle Number Limit
													// 0: No Limited, 1: Vehicle Number Limited
	// 10. END

	// 11. Save The Contents OF SFI = 0x18, Only The Last Record In IC Card
	// For Check Black List Or....
	unsigned char TR_CARD_RECORD_TTC[2];			// IC Card Record TTC
	unsigned char TR_CARD_RECORD_OVERDRAW_LIMIT[3];	// IC Card Record OverDraw Limit
	unsigned char TR_CARD_RECORD_AMOUNT[4];			// IC Card Record Amount
	unsigned char TR_CARD_RECORD_TRANS_TYPE;		// IC Card Record Transaction Type
	unsigned char TR_CARD_RECORD_PSAM_TID[6];		// IC Card Record PSAM TID
	unsigned char TR_CARD_RECORD_DATE[4];			// IC Card Record Transaction Date
	unsigned char TR_CARD_RECORD_TIME[3];			// IC Card Record Transaction Time
	// 11. END

	// 12. Save The Response Of
	//		IC Card Get Lock Proof
	unsigned char TR_CARD_GREY_FLAG;				// IC Card Grey Flag: 00, 01 or 10
	unsigned char TR_CARD_GREY_TRANS_TYPE;			// IC Card Grey Transaction Type
	unsigned char TR_CARD_GREY_APP_TYPE;			// IC Card Grey Application Type
	unsigned char TR_CARD_GREY_BALANCE[4];			// IC Card Grey Application Balance
	unsigned char TR_CARD_GREY_TTC[2];				// IC Card Grey Transaction TTC
	unsigned char TR_CARD_GREY_PSAM_TID[6];			// IC Card Grey Transaction PSAM Terminal ID
	unsigned char TR_CARD_GREY_DATE[4];				// IC Card Grey Transaction Date
	unsigned char TR_CARD_GREY_TIME[3];				// IC Card Grey Transaction Time
	union
	{
		unsigned char TR_CARD_GREY_AMOUNT[4];		// IC Card Grey Transaction Amount, If Grey Flag = 0 or 0x10
		unsigned char TR_CARD_GREY_MAC2[4];			// IC Card Grey Transaction MAC2, If Grey Flag = 0x01
	};
	
	union
	{
		unsigned char TR_CARD_GREY_TAC[4];			// IC Card Grey Transaction TAC, If Grey Flag = 0 or 0x10
		unsigned char TR_CARD_GREY_GTAC[4];			// IC Card Grey Transaction GTAC, If Grey Flag = 0x01
	};
	// 12. END

	// 13. Save The Response Of
	//		IC Card Get Balance
	unsigned char TR_CARD_APP_BALANCE[4];			// IC Card Balance
	// 13. END

	// 14. Save The Response Of
	//		IC Card Init Grey Lock
	unsigned char TR_CARD_APP_BALANCE0[4];			// IC Card Balance, Should Be Same With TR_CARD_APP_BALANCE
	unsigned char TR_CARD_APP_TTC[2];				// IC Card Transaction TTC
	unsigned char TR_CARD_APP_OVERDRAW_LIMIT[3];	// IC Card OverDraw Limit
	unsigned char TR_CARD_KEY_VERSION;				// IC Card Key Version
	unsigned char TR_CARD_ALGORITHM_FLAG;			// IC Card Algorithm Flag
	unsigned char TR_CARD_RANDOM[4];				// IC Card Ramdom For This Transaction
	// 14. END

	// 15. Save The Response Of
	//		IC Card Grey Lock
	unsigned char TR_CARD_GTAC[4];					// IC Card Grey Lock GTAC
	unsigned char TR_CARD_MAC2[4];					// IC Card Grey Lock MAC2
	// 15. END

	// 16. Save The Response Of
	//		IC Card Debit Unlock
	unsigned char TR_CARD_MAC3[4];					// IC Card Debit Unlock MAC3
	// 16. END

	// 17. Save The Data From Transaction
	//		Not From IC Card And PSAM
	unsigned char TR_TRANS_APP;						// Transaction Application Type
													// 0: Default, Sinopce; 1: Loyalty
	unsigned char TR_TRANS_TYPE;					// Transaction Type
	unsigned char TR_TRANS_TTC[4];					// Transaction TTC, Send To POS
	
	unsigned char TR_TRANS_DEBITSOURCE;				// Transaction IC Card Debit Source; 0: Sinopec App; 1: Loyalty App;
													//									 2: PBOC E-Purse; 3: PBOC E-Deposit
													// Douzhq 20140613 Add , 			 4: SaveOil App

	unsigned char TR_TRANS_UNIT;					// Transaction Settlement Unit;	bit0 = 0: Cash( fen ); bit0 = 1: Loyalty( 0.01 Dot )
													// bit7 - bit4:	Same As PayMode
													// 0: Cash ( xian jin ); 1: Oil Ticket ( you piao ); 2: Ticket ( ti you ping zheng );
													// 3: CreditCard ( xin yong ka ); 4: Other1 ( qi ta 1 ); 5: Other2 ( qi ta 2 );
	 
	unsigned char TR_CARD_TYPE;						// Transaction Card Type; b0 = 0: Sinopec IC Card; b0 = 1: PBOC IC Card
	
	unsigned char TR_TRANS_NOZZLE_NUMBER;			// Transaction Nozzle Number
	unsigned char TR_TRANS_GRADE_CODE[2];			// Transaction Grade Code
	
	unsigned char TR_TRANS_EMP_NUMBER;				// Transaction Employee Number
	
	unsigned char TR_TRANS_DATE[4];					// Transaction Nozzle On Date
	unsigned char TR_TRANS_TIME[3];					// Transaction nozzle On Time
	unsigned char TR_TRANS_VOLUME[4];				// Transaction Volume
	unsigned char TR_TRANS_AMOUNT[4];				// Transaction Amount
	unsigned char TR_TRANS_PRICE[2];				// Transaction Price
	unsigned char TR_TRANS_PRESET_MODE;				// Transaction Preset Mode: Default = 0: Amount; 1: Volume
	unsigned char TR_TRANS_PRESET_VALUE[4];			// Transaction Preset Value
	unsigned char TR_TRANS_VOLUME_TOTAL[4];			// Transaction Volume Total
	unsigned char TR_TRANS_AMOUNT_TOTAL[4];			// Transaction Amount Total From Pump
	// 17. END

	// 18. Save The Data From Listed Tables
	unsigned char TR_PROVINCE;						// Province	Code
	unsigned char TR_CITY;							// City Code
	unsigned char TR_SUPERIOR_UNIT[4];				// Station Supiror Unit Code
	unsigned char TR_STATION_ID[4];					// Station ID
	unsigned char TR_POSP[4];						// POSP
	// 18. END

	// 19. Save Input Data, Such as PIN
	unsigned char TR_INPUT_PIN_LEN;					// Length Of The Inputed PIN
	unsigned char TR_INPUT_PIN[8];					// The Inputed PIN

	unsigned char TR_CARD_PRESET_MODE;				// IC Card Preset Mode, For Card Limited
													// Combine With The Keypad Preset Mode,
													// To Determine The Preset Mode To Pump

	unsigned char TR_MAX_PRESET_AMOUNT[4];			// Max Preset Amount For Fulling
	unsigned char TR_MAX_PRESET_VOLUME[4];			// Max Preset Volume For Fulling
	// 19. END

	// 20. Save The Last IC Card Record,
	// For Check IC Card Limit Elements,
	// From The IC Card Record
	unsigned char TR_TIMES_DAY;						// Fuelling Times Of The Transaction Day
	unsigned char TR_VOLUME_TOTAL_DAY[4];			// Volume Total of The Transaction Day
	unsigned char TR_AMOUNT_TOTAL_DAY[4];			// Amount Total Of The Transaction Day
	// 20.END

	// 21. Save Cash Trans
	unsigned char TR_CASH_TOTAL[4];
	unsigned char TR_AUTH_CODE_LEN;
	unsigned char TR_AUTH_CODE[12];
	// 21. End

	// 22. Save The Sinopec Mode2 Data
	unsigned char TR_WORK_MODE;						// Save The Work Mode
	unsigned char TR_AUTH_MODE;						// Save The Auth Mode
	unsigned char TR_PAY_X;							// Save Pay Mode
	unsigned char TR_ADD_IN_LEN;					// POS Auth Response Add-in Length
	unsigned char TR_ADD_IN[20];					// POS Auth Response Add-In
	unsigned char TR_AUTH_FEEDBACK;					// POS Auth FeedBack Byte
	unsigned char TR_PIN_ENCRYPTED[8];				// Save The Encrypted PIN Block, All 8 Bytes Data Saved
	unsigned char TR_TRANS_MAC[8];					// Save The Calc MAC, All 8 Bytes MAC(Encrypted) Data Saved
	unsigned char TR_TRANS_AMOUNT_T1[4];			// Actual Debit Amount By EMP Or Customer Card Applicaiton
	unsigned char TR_TRANS_AMOUNT_T2[4];			// Debit Online Account Amount
													// TR_TRANS_AMOUNT: Save The Fuelling Amount
	unsigned char TR_TRANS_AMOUNT_OVER_F[4];		// The OverFuelling Amount
	unsigned char TR_DISCOUNT_NUMBER[2];			// Discount Number
	
//	unsigned char TR_BALANCE_PRESET[4];				// Balance For Preset, Used In Fuelling Amount Total Limit
													// Updated In Get Balance
	// 22. Save The Sinopec Mode2 Data End


#ifdef MAKE_PSAM_VERIFY
	unsigned char TR_ACT_ENCRYPTED[8];				// Save The Encrypted Data For ACT Card Process
	unsigned char TR_RID_ENCRYPTED[8];				// Save The Encrypted Data For RID Card Process
	unsigned char TR_RID_LOG_PARA_TOTAL[2];			// Save The RID Card Log Para Total Allowed
	unsigned char TR_RID_LOG_PARA_ADDED[2];			// Save The RID Card Log Para Appended
#endif

// 20151217 qiyong add for zhonghangyou require
	//  Save The Contents of FID = 15
	//		Before Select Sinopec_APP
	//		Or Select Loyalty_APP
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	unsigned char TR_CARD_FID15;					// IC Card FID 0x15 flag
#endif

	unsigned char TR_RFU;
}TRINP_DEF;


// Journal Format
// Used In Journal TRANS_TYPE
#define CARDERROR		0x80						// Error Card b7 = 1
#define LOCALLIST		0x40						// Local Black/White List b6 = 1
#define WORKOFFLINE		0x20						// Work Offline b5 = 1
#define CARDTACREAD 	0x10						// IC Card TAC Was Get b4 = 1

#define TRANSOK			0x00						// Normal IC Card Transaction
#define TRANSGREY		0x01						// IC Card Absence After Set Grey, Grey Record
#define TRANSERROR		0x02						// IC Card Error But The Transaction Is Right
#define TRANGREYUNLOCK	0x03						// Grey Card, Unlock Transaction
#define TRANSGREYFLAG	0x04						// Clear Grey Flag Transaction
#define TRANSLOGON		0x05						// Log On Transaction
#define TRANSLOGOFF		0x06						// Log Off Transaction
#define TRANSNONCARD	0x07						// Non IC Card Transaction
#define TRANPRICERECV	0x08						// Price Receiving Transaction
#define TRANSCARDERR	0x09						// Limited IC Card Transaction

#ifdef MAKE_CARD_MODE2

#define TRANSTYPERFU	0x0A						// Trans Type RFU
#define TRANSONLINEABN	0x0B						// Trans Online Abnormal
#define TRANSNONCARDSS	0x0C						// Non Card Self-Service
#define TRANSHOSTAUTH	0x0D						// Host Auth Trans
#define TRANSCANCELAUTH	0x0E						// Auth Cancel
#define TRANSOVERFUEL	0x0F						// Overfuelling Trans

#endif	// MAKE_CARD_MODE2

// Same As Sinopec Specification
typedef struct
{
	unsigned char JOURNALSIGN;						// Journal Start Flag
	unsigned char TRANS_TTC[4];						// Transaction TTC
	unsigned char TRANS_TYPE;						// Transaction Type: 0 ---- 9
	unsigned char TRANS_DATE[4];					// Transaction Date
	unsigned char TRANS_TIME[3];					// Transaction Time
	unsigned char CARD_ASN[10];						// Transaction IC Card ASN
	unsigned char CARD_APP_BALANCE[4];				// Transaction IC Card Application Balance
	unsigned char TRANS_AMOUNT[3];					// Transaction Amount
	unsigned char CARD_TTC[2];						// Transaction IC Card TTC
	unsigned char CARD_TAC[4];						// IC Card TAC
	unsigned char PSAM_GMAC[4];						// PSAM GMAC
	unsigned char PSAM_TAC[4];						// PSAM TAC
	unsigned char PSAM_ASN[10];						// PSAM ASN
	unsigned char PSAM_TID[6];						// PSAM TID
	unsigned char PSAM_TTC[4];						// PSAM TTC
	unsigned char TRANS_DEBITSOURCE;				// Transaction Debit Source
	unsigned char TRANS_UNIT;						// Transaction Settlement Unit
	unsigned char CARD_TYPE;						// Transaction IC Card Type
	unsigned char CARD_KEY;							// IC Card Key Index(b7....b4) And Version(b3....b0)
	unsigned char NOZZLENUMBER;						// Transaction Nozzle Number
	unsigned char GRADE_CODE[2];					// Grade Code, Product Code
	unsigned char TRANS_VOLUME[3];					// Transaction Volume
	unsigned char TRANS_PRICE[2];					// Transaction Price
	unsigned char EMPLOYEE_NUMBER;					// Transaction Employee Number
	unsigned char VOLUME_TOTAL[4];					// Total Volume
	unsigned char RFU[11];							// Journal RFU
	unsigned char TRANS_TAC[4];						// Transaction TAC
}JOURNAL;





// PIN Message, Used To Avoid Input PIN Repeatly
typedef struct
{
	unsigned char CardASN[10];
	unsigned char CardPINLen;
	unsigned char CardPIN[8];
}PININFO;


//extern unsigned char TempBuffer[36];
//extern int TimeInterval;
extern int ProcTimes;

extern unsigned char RecvBuffer[256];
extern unsigned int RecvBufferLen;
extern unsigned int ReceivedFlag;

extern unsigned char SendBuffer[256];
extern unsigned int SendBufferLen;
extern unsigned char SentFlag;

extern unsigned char CRPowerFlag;

extern unsigned char HoldCard;

extern unsigned char KeepBalScreen;
extern Tmr * BalScreenKeepTimer;

extern unsigned char CommErrorTimes;

//extern unsigned char TestTimeOut;

//// For Check BCC Of Card Reader
//extern unsigned char TmpBCC;

// IC Card Default PIN Length
extern unsigned char ICCard_Default_PIN_Len;
// IC Card Default PIN
extern unsigned char ICCard_Default_PIN[2];



extern TRINP_DEF TRINP;

extern JOURNAL RECORD; 

extern PININFO PINInfo;
 
// CR_ICCARD_Status: Save The Current IC Card Status
// CR_ICCARD_Status: Same As The Public Status For Other Process Inquire
//extern unsigned int CR_ICCARD_Status;

// Status Use In CR_ICCARD_Status
//#define ICCARD_READY 					0x00000001		// Bit00 = 1: IC Card Ready
//#define ICCARD_BUSY 					0x00000010		// Bit01 = 1: IC Card Is Fuelling, Busy
//#define ICCARD_EMP_LOG_ON				0x00000100		// Bit02 = 1: IC Card Employee Logged On
//#define ICCARD_ERROR 					0x80000000		// Bit31 = 1: Card Reader Error, Can not Connect Card Reader


// Process Flag
// Define The Local Message Exchange Flag With Other Processes
// Maintained By Card Reader Porcess
extern unsigned int CR_Task_Flag;

// Define The Bits For CR_Task_Flag
// Bit0: Need Keypad Input PIN
#define ICCARD_PIN_K					0x00000001
// Bit1: Need Keypad Input Employee Password
#define ICCARD_EMP_PWD_K				0x00000002
// Bit2: Need Keypad Input Vehicle Number
#define ICCARD_VEHICLE_NUMBER			0x00000004
// Bit3: Need POS Check Blacklist
#define ICCARD_POS_BLACK_LIST			0x00000008
// Bit4: Need POS Check Whitelist
#define ICCARD_POS_WHITE_LIST			0x00000010
// Bit5: Need POS Grey Journal
#define ICCARD_POS_GREY					0x00000020

// Bit6: Need PUMP Stop
#define ICCARD_PUMP_STOP				0x00000040
// Bit7: Need PUMP Last Trans data
#define ICCARD_PUMP_LAST_TRAN			0x00000080
// Bit8: Need PUMP All Totals
#define ICCARD_PUMP_TOTALS				0x00000100
// Bit9: Need PUMP Offline data
#define ICCARD_PUMP_OFFL_TRAN			0x00000200

#ifdef MAKE_PSAM_VERIFY

// Bit10: Need PUMP Cala Encrypted Verify data
#define ICCARD_PUMP_VERIFY				0x00000400

#endif

// Bit11: Need Press ENTER Key For Sinopec Requirement
#define ICCARD_ENTER_KEY_K				0x00000800

// Bit12: Sinopec Mode2: IC Card Wait Host Auth
#define ICCARD_AUTH						0x00001000

// Bit13: Sinopec Mode2: IC Card Wait Host Fuelling Data
#define ICCARD_FUELLING_DATA			0x00002000

// Bit14: Sinopec Mode2: IC Card Issue STOP to Host
#define ICCARD_STOP						0x00004000

// Bit15: Sinopec Mode2: IC Card Upload IC Card Record To Host
#define ICCARD_UPLOAD_CARD_RECORD		0x00008000

// Bit16: Need Keypad Input Emp Number
#define ICCARD_EMP_NUMBER_K				0x00010000

// Bit17: RFU
#define ICCARD_BIT17					0x00020000

// Bit18: RFU
#define ICCARD_BIT18					0x00040000

// Bit19: RFU
#define ICCARD_BIT19					0x00080000




enum CR_PROC_STEP
{
	CR_PROC_START,							// No Use, Only For Enum Start

	CR_CARDREADER_PROC,						// Process For Card Reader
											// Perform Card Reader Reset

	CR_CRPOWERON_PROC,						// Process For Card Reader
											// First Power On

	CR_PSAM_PROC,							// Process For PSAM
											// Perform PSAM Cold Reset, Select PSE And ADF

	CR_ICCARD_ABSENCE_PROC,					// Process For Card Reader WithOut IC Card
											// Perform Check IC Card Inserted Or Not
											// Trans On And Credit PSAM Grye Unlock, Grey Record

	CR_ICCARD_INSERT_PROC,					// Process For IC Card Inserted
											// Perform IC Card Reset, Select ADF, Check Black Or White
											// External Input PIN, IC Card Grey Flag
											// And Check The IC Card Limits

	CR_ICCARD_READY_PROC,					// Process For IC Card Ready
											// Perform IC Card Check Spring Out Or Not,
											// External Spring Out Card, Pump Nozzle On

	CR_ICCARD_GREY_CARD_PROC,				// Process For Grey IC Card
											// Perform IC Card Grey Debit, Or Read GTAC

	CR_ICCARD_PUMP_CALL_PROC,				// Process For Beginning Set Grey For IC Card
											// Perform IC Card Set Grey, Preset

	CR_ICCARD_FUELLING_PROC,				// Process For IC Card In Fuelling
											// Perform IC Card Spring Out Or Not, External Rounding

	CR_ICCARD_EOT_PROC,						// Process For IC Card EOT
											// Perform IC Card Debit Unlock

	CR_TRANS_JOURNAL,						// Form Transaction Journal

	CR_ICCARD_EXIST_EXTERNAL_PROC,			// Process For External Process Issues, While IC Card Exist

	CR_ICCARD_NONEXIST_EXTERNAL_PROC,		// Process For External Process Issues, While IC Card Does Not Exist

	CR_ICCARD_BUSY_EXTERNAL_PROC,			// Process For External Process Issues, While IC Card IS Busy In Fuelling

	CR_LOGON_PROC,							// Process For Employee Log On Issues, Form 4 Journal At Most

	CR_LOGOFF_PROC,							// Process For Employee Log Off Issues, Form 4 Journal At Most

	CR_OFFL_TRANS_PROC,						// Process For Offline Trans Issues, Form 4 Journal At Most

	CR_PRICE_RECV,							// Process For Price Receiving, Form 4 Journal At Most

	CR_CALC_MAC_PROC,						// Process For PSAM Sinopec2 Appliaction Cala MAC Or Encrypted Data

	CR_SEND_CARD_RECORD_PROC,				// Process For Sinopec Mode2 Send IC Card Log To Host

#ifdef MAKE_PSAM_VERIFY
	CR_ACT_CARD_PROC,						// Process For ACT Card
	CR_RID_CARD_PROC,						// Process For RID Card
#endif

	CR_PROC_MAX,							// No Use, Only For Enum End
};
// CR_Proc_Step: Save The Current Steps For The IC Card Process
// CR_Proc_Step:
//extern unsigned int CR_Proc_Step;
//extern unsigned int CR_Proc_Last_Step;		// Save Last Step
extern enum CR_PROC_STEP CR_Proc_Step;
extern enum CR_PROC_STEP CR_Proc_Last_Step;		// Save Last Step


enum CR_COMMAND_TYPE
{
	CR_CommandStart,						// No Use, Only For Enum Start

	// Command Use For Card Reader Operation
	CR_CARDREADER_RESET = 0x10,				// Card Reader Reset, Start From 0x10
	CR_CARDREADER_CHECK_CARD,				// Check The Card In Card Reader Or Not
	CR_CARDREADER_POWERON_CARD,				// Power On The IC Card In Card Reader
	CR_CARDREADER_POWEROFF_CARD,			// Power Off The IC Card In Card Reader
	CR_CARDREADER_SPINGOUT_CARD,			// Spring Out The IC Card In Card Reader
	CR_CARDREADER_POWEROFF_PSAM,			// Power Off The PSAM In Card Reader

	CR_CARDREADER_CHECK_CARD_POWERON,		// Check The Card In Card Reader Or Not, Special For Power ON
	CR_CARDREADER_SPINGOUT_CARD_POWERON,	// Spring Out The IC Card In Card Reader, Special For Power ON
	// Command Use For Card Reader Operation End

	// Command Used For PSAM Operation
	CR_PSAM_COLD_RESET = 0x20,				// Cold Reset The PSAM1 As Mode 7816, 5.0V, Start From 0x20
	CR_PSAM_SELECT_PSE,						// Select PSE " 1PAY.SYS.DDF01 " In PSAM1
	CR_PSAM_SELECT_SINOPEC1,				// Select Sinopec1 Application In PSAM1
	CR_PSAM_SELECT_SINOPEC2,				// Select Sinopec2 Application In PSAM1
	CR_PSAM_CHECK_VALIDATE,					// Check PSAM Application Validate Or Not In PSAM1
	
	CR_PSAM_INIT_GREY_LOCK,					// PSAM Initialize Grey Lock Command
	CR_PSAM_CERTIFICATE_GREY_LOCK,			// Certificate MAC2 To PSAM
	CR_PSAM_CREDIT_GREY_DEBIT,				// Credit PSAM For GMAC And PSAM_TAC
	CR_PSAM_GET_GMAC,						// PSAM Read GMAC, After Credit PSAM When GMAC Was Not Received
	CR_PSAM_INIT_DES_ENCRYPT,				// PSAM Initiate For DES Crypt
	CR_PSAM_DES_ENCRYPT0,					// PSAM DES Encrypt Whole Data Buffer
	CR_PSAM_INIT_DES_ENCRYPT1,				// PSAM Initiate For DES Encrypt In 2 Halfs
	CR_PSAM_DES_ENCRYPT1,					// PSAM DES Encrypt First Half Data Buffer
	CR_PSAM_DES_ENCRYPT2,					// PSAM DES Encrypt Second Half Data Buffer
	CR_PSAM_INIT_PURCHASE,					// PSAM Intialize Purchase
	CR_PSAM_CREDIT_MAC2,					// PSAM Credit Pruchase MAC2


#ifdef MAKE_PSAM_VERIFY

	CR_PSAM_GET_ANTI_FRAUD_PROOF,			// PSAM Get AntiFraud Proof
	CR_PSAM_ANTI_FRAUD_VERIFY,				// PSAM Verify The Encrypted Data From PUMP
	CR_PSAM_WAITING,						// For PSAM Not Registered Or Unsupport Certificate
											// Just Looping Here, And Waiting For Reset And Change PSAM
	CR_PSAM_GET_RANDOM,						// For PSAM Register, Get PSAM Random
	CR_PSAM_START_BINDING,					// Start Binding Service For PSAM
	CR_PSAM_INIT_BINDING,					// Initialize Binding Service For PSAM
	CR_PSAM_BINDING,						// Binding Service For PSAM
	CR_PSAM_VERIFY_MSG_SINOPEC2,			// For PSAM Verify Function, PSAM Select Sinopec2 Appliaction
	CR_PSAM_VERIFY_MSG_SINOPEC1,			// For PSAM Verify Function, PSAM Select Sinopec1 Appliaction
	CR_PSAM_VERIFY_MSG_INIT_MAC,			// For PSAM Verify Functino, PSAM Initialize Message MAC
	CR_PSAM_VERIFY_MSG_MAC,					// For PSAM Verify Function, PSAM Calc POS Message MAC



	CR_CARDREADER_CHECK_CARD_PSAM,			// Check Card Insert Or Not, While PSAM Do Not Support Verify Or Locked
	CR_CARDREADER_SPINGOUT_CARD_PSAM,		// Spring Out Card, While PSAM Do Not Support Verify Or Locked
#endif
	// Command Use For PSAM Operation End


	// Command Used For	IC Card Operation
	CR_ICCARD_RESET = 0x40,					// Cold Reset The IC Card As Mode 7816 5.0V, Start From 0x40
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
    CR_ICCARD_READ_FID_0X15,				// IC Card Read The FID 0x15 // 20151217 qiyong add for zhonghangyou requirement
#endif
	CR_ICCARD_SELECT_LOYALTY_APP,			// IC Card Select The Loyalty Application
	CR_ICCARD_SELECT_SINOPEC_APP,			// IC Card Select The Sinopec Application
	CR_ICCARD_USED_APP,						// IC Card Determine The Used Application, Default Is Sinopec Application;
											// If Sinopec Application Was Blocked, Select Loyalty Application,
											// If Both Were Blocked, Return	Error
	CR_ICCARD_READ_SFI_0X15,				// IC Card Read The SFI 0x15
	CR_ICCARD_READ_SFI_0X16,				// IC Card Read The SFI 0x16

#ifdef MAKE_PSAM_VERIFY
	CR_ICCARD_READ_SFI_0X1A,				// IC Card Read The SFI 0x1A
#endif

	CR_ICCARD_READ_SFI_0X1B,				// IC Card Read The SFI 0x1B
	CR_ICCARD_READ_SFI_0X1C,				// IC Card Read The SFI 0x1C
	
//	CR_ICCARD_GET_PIN,						// IC Card Get PIN Either From Default PIN Or Keypad Input
	CR_ICCARD_VERIFY_PIN,					// IC Card Verify PIN
	CR_ICCARD_GET_BALANCE,					// IC Card Read Application Balance
	CR_ICCARD_GET_LOCK_PROOF,				// IC Card Read The Grey Message
	
	CR_ICCARD_CHECK_LIMIT,					// IC Card Limits Check
	CR_ICCARD_CHECK_PRESET,					// IC Card Check Preset Mode And Value
	CR_ICCARD_READ_RECORD,					// IC Card Read The Trans Record In IC Card
	CR_ICCARD_INIT_GREY_LOCK,				// IC Card Initiate Grey Lock
	CR_ICCARD_SET_GREY_LOCK,				// IC Card Set Grey Lock
	CR_ICCARD_DEBIT_GREY_UNLOCK,			// IC Card Debit Grey Unlock Command
	CR_ICCARD_CLEAR_LOCK_PROOF,				// IC Card Clear Lock Proof, After The Debit Unlock
	
	CR_ICCARD_PIN_LOCK,						// Retry PINs To Lock The PIN, For Black List Card
	
	CR_ICCARD_CHECK_GRADE_BRAND_LIMIT,		// Check Grade Brand Limit
	CR_ICCARD_CHECK_LOCATION_LIMIT,			// Check Locationg Limit
	CR_ICCARD_CHECK_TIMES_LIMIT,			// Check Fuelling Times Per Day Limit
	CR_ICCARD_CHECK_AMOUNT_TOTAL_LIMIT,		// Check Fuelling Amount Total Per Day Limit
	CR_ICCARD_CHECK_FUELLING_LIMIT,			// Check Volume And Amount Per Fuelling Limit
	CR_ICCARD_INIT_PURCHASE,				// IC Card Initialize Purchase
	CR_ICCARD_DEBIT_PURCHASE,				// IC Card Debit Purchase
	// Command Used For	IC Card Operation End

	// Command Used For Contactless IC Card Poll
	CR_ICCARD_POLL_TYPEA = 0x60,
	CR_ICCARD_POLL_TYPEB,
	CR_ICCARD_DESELECT,
	CR_ICCARD_GET_CARD_TYPE,
	CR_ICCARD_DELAY,
	// Command Used For Contactless IC Card Poll End

	// Command For Process Sinopec2
	CR_WAITING_MAC,
	CR_WAITING_MAC_END,
	CR_WAITING_AUTH,
	CR_WAITING_FUELLING_END,
	// Command For Process Sinopec2 End

	// Command Used For	Transaction Process
	CR_FILL_JOURNAL = 0x70,					// Transaction Fill Record, In CR_TRANS_JOURNAL Step
	// Command Used For	Transaction Process End
	
	// Command Used For	External Process
	// Source: IC Card Process, Destination: Keypad; Start From 0x80
	// Keypad Process
	CR_WAITING_PIN = 0x80,					// IC Card Need PIN Input
	CR_WAITING_EMP_PASSWORD,				// IC Card Need Employee Password Input
	CR_WAITING_VEHICLE_NUMBER,				// IC Card Need Vehicle Number Input
	CR_WAITING_ENTER_KEY,					// IC Card Need Vehicle Number Input

    CR_WAITING_EMP_NUMBER,                  // IC Card Need Emp Number Input
	// Keypad Process End

	// Source: IC Card Process, Destination: POS; Start From 0x90
	// POS Process
	CR_CHECK_LIST = 0x90,					// IC Card Check Lists Both Black Lists And White List
	CR_CHECK_BLACK_LIST,					// IC Card Check Black Lists
	CR_CHECK_WHITE_LIST,					// IC Card Check White List
	CR_CHECK_GREY_RECORD,					// IC Card Check Grey Record From POS
	// POS Process End

	// Source: IC Card Process, Destination: Pump; Start From 0xA0
	// PUMP Process
	CR_PUMP_STOP = 0xA0,					// IC Card Process Stop The PUMP
	CR_GET_LAST_TRANS,						// IC Card Process Need The Last Trans Data
	CR_GET_LAST_TRANS_ZERO,					// IC Card Process Get Last ZERO Trans Data
	CR_GET_LAST_TRANS_POWERON,				// IC Card Process Get Last Trans At Power On, Not Actuall Meaning Now....
											// Only For Meet The Pump Process
#ifdef MAKE_PSAM_VERIFY
	CR_PUMP_VERIFY,							// CR Need Pump Calc The Encrypted Verify Data
#endif
	// PUMP Process	End


	// Source: Keypad, Destination: IC Card; Start From 0xB0
	// Keypad
	CR_SPRING_CARD_KB = 0xB0,				// Keypad Spring Out The Card
	CR_PRESET,								// Keypad Preset For The IC Card Fuelling
	CR_ROUNDING,							// Keypad Rounding For The Fuelling
	CR_EMP_LOG_ON,							// Keypad Need IC Card Employee Log on
	CR_EMP_LOG_OFF,							// Keypad Need IC Card Employee Log Off
	CR_SELECT_APP,							// Keypad Select The IC Card Application: Loyalty Or Sinopec
	CR_SELECT_PAY_MODE,						// Keypad Select The Pay Mode
	// Keypad End

	// Source: Schedule, Destination: IC Card; Start From 0xC0
	// Schedule
	CR_SCH_LOG_OFF = 0xC0,					// Schedule Process Need IC Card Employee Log Off
	// Schedule End

#ifdef MAKE_PSAM_VERIFY
	CR_ACT_RID_ENCRYPTED = 0xD0,			// IC Card Calc Encrypted Data
	CR_RID_GET_LOG_PARA_TOTAL,				// Get RID Card Log Para Total Allowed
	CR_RID_GET_LOG_PARA_ADDED,				// Get RID Card Log Para Added Currently
	CR_RID_APPEND_LOG,						// Append Log To RID Card
#endif

	CR_AUTH = 0xE0,							// Pump Need Auth For The Fuelling
	CR_AUTH_CONFIRM,						// Pump Need Auth Confirm For The Fuelling
	CR_OFFLINE_TRANS,						// Pump Has Offline Trans, And Need IC Card Form A Offline Trans Record
											// 1 Offline Trans Per Call
	CR_PUMP_TOTALS,							// All 4 Totals	Was Read Per Call

	CR_CHECK_NOZZLES,						// Get NozzleNumbers, Logical And Phisical etc....

	CR_CHECK_TIME,							// Check Pump Time With The Last Transaction, While Pos Is Offline
	// Pump End

	// Source: POS, Destination: IC Card; Start From 0xF0
	// POS
	CR_PRICE_RECORD = 0xF0,					// POS Need IC Card Form A Price Receive Record

	CR_CARD_LOG_START,						// Start Send IC Card Log To Host For Sinopec Mode2
	CR_CARD_LOG_END,						// End Send IC Card Log To Host For Sinopec Mode2

	CR_OFFLINE_RECORD_START,				// Append Offline Record To EMP Card Start
	CR_OFFLINE_RECORD_APPEND,				// Append Offline Record To EMP Card
	CR_OFFLINE_RECORD_END,					// Append Offline Record To EMP Card End
	CR_OFFLINE_MAC,							// Get The MAC
	// POS End
	// Command Used For	External Process End
        
        CR_OVER_FUEL,
        CR_OVER_FUEL_PASSWORD,
	
//Douzhq 20140603 Add , Lock Nozzle , CMD
//Lock Nozzle Feature , All(Cash & No Cash & PASM & No PASM)
	CR_ICCARD_UNLOCK_NOZZLE,				// IC Card Auth Fuel Unlock Command
	CR_WAITING_UNLOCK_PASSWORD,				// IC Card Need Unlock Nozzle Password Input
	
//Douzhq 20140819 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	CR_ICCARD_SELECT_SAVEOIL_APP,			// IC Card Select The SaveOil Application
#endif

//Douzhq 20140820 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
	CR_ICCARD_CARNUM_RECOGNIZE,				// IC Card Car Number Recognize Command
	CR_ICCARD_GRADE_LIMIT_RECOHNIZE,		// IC Card Grade Limit Recognize Command
#endif
//Douzhq 20140926 Add , Config Printer Set
	CR_WAIT_PRINT_CHOOSE,					// IC Card Wait Print Choose
//Douzhq 20141030 Add , Safe Mode
//----------------------------Down------------------------------------------------------------------	
	CR_WAIT_SAFEMODE_PASSWD,				// In Safe Mode , Wait Input Emp Password
//---------------------------- Up ------------------------------------------------------------------	
	CR_CommandMax,							// No Use, Only For Enum End
};
// CR_Command_Type: Save The Current Command Type
// Command Type: All The Commands To Card Reader
//extern unsigned int CR_CommandType;
//extern unsigned int CR_Last_CommandType;	// Save Last Command Type
extern enum CR_COMMAND_TYPE CR_CommandType;
extern enum CR_COMMAND_TYPE CR_Last_CommandType;	// Save Last Command Type



// CR_CommandStatus: Save the Current Command Status To Card Reader
// Command Status: enum{}CR_COMMANDSTATUS;
extern Tmr * CR_Command_ACK_TimeOut;
extern Tmr * CR_Command_Response_TimeOut;
enum CR_COMMAND_STATUS
{
	CR_CommandStatusStart,					// No Use, Only For Enum
	
	CR_CommandSending,						// Command Prepared and Waiting for Sending Or Sending In Progress
	CR_CommandWaitingACK,					// Command Sent And Waiting For the ACK Response
	CR_CommandWaitingResponse,				// Command Received ACK and ENQ was Sent, Waiting For Response Data
	CR_CommandResponseReceived,				// The Command Response was Received
	CR_CommandResponseError,				// Response Check BCC Error
	CR_CommandACKTimeOut,					// ACK Receiving TimeOut
	CR_CommandResponseTimeOut,				// Response TimeOut

	CR_CommandStatusMax,					// No Use, Only For Enum Max

};
//extern unsigned int CR_CommandStatus;
extern enum CR_COMMAND_STATUS CR_CommandStatus;


// Used To Inform Cala MAC Or Encrypted
enum CR_MAC_TYPE
{
	CR_MAC_TYPE_START,						// For Start

	CR_MAC_TYPE_MAC,						// The Calc MAC Process Calc MAC
	CR_MAC_TYPE_ENC,						// The Calc MAC Process Encrypt Data

	CR_MAC_TYPE_MAX,						// For Enum Max
};
// Save The Cala MAC Or Encrypt Data
extern enum CR_MAC_TYPE CR_MACType;
// Save The Process And Command After Calc MAC
extern enum CR_COMMAND_TYPE CR_MAC_CommandType;
extern enum CR_PROC_STEP CR_Proc_MAC_Step;



// Use For Trans Status
// Wether Trans Is On Or Not Or Journal Information
// If
enum TRANS_FLAG
{
	TRANS_START,								// No Use, Only For Enum Start



	SET_GREY,									// Send Set Grey Command To IC Card ????
												// But Fail To Get MAC2, Where To Go?????

#ifdef MAKE_CARD_MODE2
	SEND_AUTH,									// Auth Request Sent For Sinopec Mode2
	AUTH_RESP,									// Auth Response Was Received For Sinopec Mode2
	DEBIT_ZERO,									// Debit 0, For Employee Card Or Customer Card Host Account
	AUTH_PUMP,									// Auth The Pump For Fuelling Send
												// Attn: SEND_AUTH, AUTH_RESP, DEBIT_ZERO Need Zerp Amount Process For Exception
												// AUTH_PUMP Need GetLastTrans From PUMP For Exception
	SEND_FUELLING,								// Send Fuelling Data To POS
	FUELLING_RESP,								// POS Response Fuelling Data
#endif	// MAKE_CARD_MODE2

	MAC2_RECEIVED,								// IC Card Was Set Grey, And Received MAC2
	GET_LAST_TRANS,								// Get The Last Trans From PUMP
	CREDIT_PSAM,								// Credit PSAM For GMAC, And Return OK
	GMAC_RECEIVED,								// Get PSAM GMAC
	UNLOCK_GREY,								// Send Grey Unlock To IC Card, And Get IC Card TAC
	CLEAR_GREY_FLAG,							// Send Clear Grey Flag To IC Card, And Return Sucess
	
	JOURNALIT = 0x40,							// Journal Is Recording, Get Trans TAC Or ....

	TRANS_END,									// No Use, Only For Enum END
};

enum MULTI_TRANS_FLAG
{
	MULTI_TRANS_START,

// For The Multi - Journals In 1 Process
	LOGON_JOURNAL = 0x50,						// Logon Journal Is Recording....
	LOGOFF_JOURNAL,								// Logoff Journal Is Recording....
	

	NON_ICCARD_JOURNAL = 0x60,					// Offline Transaction Journal Is Recording

	PRICE_RECV_JOURNAL = 0x70,					// Price Receive Journal Is Recording

	MULTI_TRANS_END,
};



// Card Reader Command 2 Bytes
// Card Reader Reset
extern const unsigned char CR_RESET_CR[2];
// Card Reader Reset And Spring Out Card
extern const unsigned char CR_RESET_SPRING_CARD[2];
// Card Reader State, Wether Card In Or Not
extern const unsigned char CR_CARD_STATE[2];
// Card Reader Spring Out Card
extern const unsigned char CR_SPRING_CARD[2];
// IC Card Power On
extern const unsigned char CR_CARDPOWERON[2];
// IC Card Power Off
extern const unsigned char CR_CARDPOWEROFF[2];

// PSAM Power Off
extern const unsigned char CR_PSAMPOWEROFF[2];


// Polling Type A Contactless IC Card
extern const unsigned char CR_POLL_TYPE_A[2];

// Polling Type B Contactless IC Card
extern const unsigned char CR_POLL_TYPE_B[2];

// Deselect Contactless IC Card
extern const unsigned char CR_DESELECT[2];

// Contactless IC Card C_APDU Command
extern const unsigned char ICCARD_CAPDU_COMMAND_CL[2];

// Get Card Type
extern const unsigned char CR_GET_CARDTYPE[2];





// IC Card Cold Reset In 7816 Mode
extern const unsigned char ICCARD_COLD_RESET_7816[3];
// IC Card Cold Reset In EMV Mode
extern const unsigned char ICCARD_COLD_RESET_EMV[3];
// IC Card Warm Reset
extern const unsigned char ICCARD_WARM_RESET_7816[2];
// IC Card T0 CAPDU Command
extern const unsigned char ICCARD_CAPDU_COMMAND_T0[2];
// IC Card T1 CAPDU Command
extern const unsigned char ICCARD_CAPDU_COMMAND_T1[2];


// PSAM CAPDU Command T0 Mode
extern const unsigned char PSAM_CAPDU_COMMAND_T0[2];
// PSAM CAPDU Command T1 Mode
extern const unsigned char PSAM_CAPDU_COMMAND_T1[2];


// TTC For The Transaction
extern unsigned int TransTTC;


// Voice Busy Issued Or Not Flag
extern unsigned char CRVoiceBusyFlag;



// For Save RunningLog
extern unsigned char CRLogTxt[52];
// For Save CR Running Step Log
extern unsigned char CRStepLog[52];


// For Write Power Down And Spring Card Log
enum POWER_DOWN_FLAG
{
	Pd_Flag_Start,								// 0

	Pd_Flag_1,									// 1
	Pd_Flag_2,									// 2
	Pd_Flag_3,									// 3
	Pd_Flag_4,									// 4
	Pd_Flag_5,									// 5
	Pd_Flag_6,									// 6
	Pd_Flag_7,									// 7
	Pd_Flag_8,									// 8
	Pd_Flag_9,									// 9

	Pd_Flag_10,									// 10
	Pd_Flag_11,									// 11
	Pd_Flag_12,									// 12
	Pd_Flag_13,									// 13
	Pd_Flag_14,									// 14
	Pd_Flag_15,									// 15
	Pd_Flag_16,									// 16
	Pd_Flag_17,									// 17
	Pd_Flag_18,									// 18
	Pd_Flag_19,									// 19

	Pd_Flag_20,									// 20
	Pd_Flag_21,									// 21
	Pd_Flag_22,									// 22
	Pd_Flag_23,									// 23
	Pd_Flag_24,									// 24
	Pd_Flag_25,									// 25
	Pd_Flag_26,									// 26
	Pd_Flag_27,									// 27
	Pd_Flag_28,									// 28
	Pd_Flag_29,									// 29

	Pd_Flag_30,									// 30
	Pd_Flag_31,									// 31
	Pd_Flag_32,									// 32
	Pd_Flag_33,									// 33
	Pd_Flag_34,									// 34
	Pd_Flag_35,									// 35
	Pd_Flag_36,									// 36
	Pd_Flag_37,									// 37
	Pd_Flag_38,									// 38
	Pd_Flag_39,									// 39

	Pd_Flag_40,									// 40
	Pd_Flag_41,									// 41
	Pd_Flag_42,									// 42
	Pd_Flag_43,									// 43
	Pd_Flag_44,									// 44
	Pd_Flag_45,									// 45
	Pd_Flag_46,									// 46
	Pd_Flag_47,									// 47
	Pd_Flag_48,									// 48
	Pd_Flag_49,									// 49

	Pd_Flag_50,									// 50
	Pd_Flag_51,									// 51
	Pd_Flag_52,									// 52
	Pd_Flag_53,									// 53
	Pd_Flag_54,									// 54
	Pd_Flag_55,									// 55
	Pd_Flag_56,									// 56
	Pd_Flag_57,									// 57
	Pd_Flag_58,									// 58
	Pd_Flag_59,									// 59

	Pd_Flag_60,									// 60
	Pd_Flag_61,									// 61
	Pd_Flag_62,									// 62
	Pd_Flag_63,									// 63
	Pd_Flag_64,									// 64
	Pd_Flag_65,									// 65
	Pd_Flag_66,									// 66
	Pd_Flag_67,									// 67
	Pd_Flag_68,									// 68
	Pd_Flag_69,									// 69

	Pd_Flag_70,									// 70
	Pd_Flag_71,									// 71
	Pd_Flag_72,									// 72
	Pd_Flag_73,									// 73
	Pd_Flag_74,									// 74
	Pd_Flag_75,									// 75
	Pd_Flag_76,									// 76
	Pd_Flag_77,									// 77
	Pd_Flag_78,									// 78
	Pd_Flag_79,									// 79

	Pd_Flag_80,									// 80
	Pd_Flag_81,									// 81
	Pd_Flag_82,									// 82
	Pd_Flag_83,									// 83
	Pd_Flag_84,									// 84
	Pd_Flag_85,									// 85
	Pd_Flag_86,									// 86
	Pd_Flag_87,									// 87
	Pd_Flag_88,									// 88
	Pd_Flag_89,									// 89

	Pd_Flag_90,									// 90
	Pd_Flag_91,									// 91
	Pd_Flag_92,									// 92
	Pd_Flag_93,									// 93
	Pd_Flag_94,									// 94
	Pd_Flag_95,									// 95
	Pd_Flag_96,									// 96
	Pd_Flag_97,									// 97
	Pd_Flag_98,									// 98
	Pd_Flag_99,									// 99

	Pd_Flag_100,								// 100
	Pd_Flag_101,								// 101
	Pd_Flag_102,								// 102
	Pd_Flag_103,								// 103
	Pd_Flag_104,								// 104
	Pd_Flag_105,								// 105
	Pd_Flag_106,								// 106
	Pd_Flag_107,								// 107
	Pd_Flag_108,								// 108
	Pd_Flag_109,								// 109

	Pd_Flag_110,								// 110
	Pd_Flag_111,								// 111
	Pd_Flag_112,								// 112
	Pd_Flag_113,								// 113
	Pd_Flag_114,								// 114
	Pd_Flag_115,								// 115
	Pd_Flag_116,								// 116
	Pd_Flag_117,								// 117
	Pd_Flag_118,								// 118
	Pd_Flag_119,								// 119

	Pd_Flag_190 = 190,							// 190
	Pd_Flag_191,								// 191
	Pd_Flag_192,								// 192
	Pd_Flag_193,								// 193
	Pd_Flag_194,								// 194
	Pd_Flag_195,								// 195
	Pd_Flag_196,								// 196
	Pd_Flag_197,								// 197
	Pd_Flag_198,								// 198
	Pd_Flag_199,								// 199

    Pd_Flag_200,                                // 200

    Pd_Flag_201,                                // 201
    Pd_Flag_202,                                // 202
    Pd_Flag_203,                                // 203

	Pd_Flag_Max,
};

// Save The Power Down And Spring Card Flag
// Used For Write Spring Log
// 00 ---- 189: Save Power Down And SPring Card
// 190 ----   : Save Spring Card Without Power Down
// Current Is Pd_Flag_110 For Power Down Log
// And Pd_Flag_198 For Spring Card Without Power Down
// 201 & 202 reserved
// 203 For Spring Card Without Power Down in Record Space Limit
extern enum POWER_DOWN_FLAG PowerDownFlag;

// Record some error information, such as 'SW'...
extern unsigned int ErrorFlag;


// For Contactless IC Card
enum ICCARDTYPE
{
	CardTypeStart,

	CardContact,
	CardTypeA,
	CardTypeB,

	CardTypeMax
};
extern enum ICCARDTYPE CardType;

#ifdef MAKE_CASH_CR
// For Keypad Select Print Or Not Print Cash
enum KEYPAD_SELECT_PRINT
{
	KEYPAD_SELECT_PRINT_START,

	KEYPAD_SELECT_PRINT_BEGIN,

	KEYPAD_PRINT_IT,

	KEYPAD_PRINT_OVER,

	KEYPAD_SELECT_PRINT_MAX
};
extern enum KEYPAD_SELECT_PRINT KeypadPrint;
#endif

//Douzhq 20140926 Add , Config Printer Set
enum KEYPAD_SELECT_PRINT_CARD
{
	CARD_KEYPAD_SELECT_PRINT_START,

	CARD_KEYPAD_SELECT_PRINT_BEGIN,

	CARD_KEYPAD_PRINT_IT,

	CARD_KEYPAD_PRINT_OVER,

	CARD_KEYPAD_SELECT_PRINT_MAX
};
extern enum KEYPAD_SELECT_PRINT_CARD KeypadPrintCard;

// Journal Flag
// For Running Log, Mark The Journal Write
// Current: 33
extern unsigned char JournalFlag;

// MAC Buffer
// For Calc MAC Or Encrypt Data Source Buffer
extern unsigned char MACDataLen;
extern unsigned char MACBuffer[128];

#ifdef MAKE_CARD_MODE2
extern unsigned char SendCardRecord;				// 1: Sinopec Mode2: Card Is Yellow, Need Send Card Trans Record After Fuelling
													// Other: Don't Care
extern unsigned char CurrentLogs;					// Save The Logs Number For Current POS Package
extern unsigned char LogItems;						// Save The Total POS Package Number

extern unsigned char TransAmountPay;				// Save The Employee Card Payment Methode: 1: Payment In Office
													// 										   Other: Payment Out Of Office
extern unsigned char PresetMode2;					// Save The Preset Is PumpCall Or CardReady: 1: Preset In CardReady

extern unsigned char OnlineFlag;					// Online Abnormal Trans Flag: 1: Should Be Online Abnormal Trans
#endif	// MAKE_CARD_MODE2




// For Test, Save The List Status, Ok Or Not 0 = Ok
//extern unsigned char BlackListStatus;
//extern unsigned char WhiteListStatus;
//extern unsigned char GreyStatus;
// For Test, Save The List Status, Ok Or Not 0 = Ok

// For Test, Save The POS Status, 0 = POS Online; 1= POS Offline
//extern unsigned char POSStatus;
// For Test, Save The POS Status, 0 = POS Online; 1= POS Offline




#endif