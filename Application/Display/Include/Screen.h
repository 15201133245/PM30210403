#ifndef _SCREEN_H
#define _SCREEN_H

#include "Message.h"

#define DispDelayTime100ms		100
#define DispDelayTime1Second	10 * DispDelayTime100ms
#define DispDelayTime2Second	20 * DispDelayTime100ms
#define DispDelayTime3Second	30 * DispDelayTime100ms
#define DispDelayTime4Second	40 * DispDelayTime100ms
#define DispDelayTime30Second	300 * DispDelayTime100ms
#define DispDelayTime1Minute	600 * DispDelayTime100ms


// Define The Whole Display Area
#define DispScrLeftX			0
#define DispScrTopY				0
#define DispScrBottomX			127
#define DispScrBottomY			63

// Define The Trans Display Area
#define DispTransLeftX			1
#define DispTransTopY			9
#define DispTransBottomX		126
#define DispTransBottomY		49

// Define The Bottom Information Line Area
#define DispInfoLeftX			1
#define DispInfoTopY			51
#define DispInfoBottomX			126
#define DispInfoBottomY			62

// Define The Trans Display Lines Area
#define DispTransLine1		 	DispTransTopY + 1
#define DispTransLine2		 	DispTransLine1 + 1 + 12
#define DispTransLine3		 	DispTransLine2 + 1 + 12




enum SCREENTYPE
{
	DEFAULT_SCR,						// Screen Type: PopUp The Screen
	NORMAL_SCR,							// Screen Type: Normal Screen With Select Function
	INFORMATION_SCR,						// Screen Type: Information Screen, Just For Display Something, No Select Function
	POPUP_SCR,
};

enum SCREEN
{
	SCREEN_START,						// Just For Enum Start

	DEFAULT_SCR_CARD,					// Default Screen, Inserted IC Card Screen
	DEFAULT_SCR_BUSY,					// Defalut Screen, IC Card Busy Screen
	DEFAULT_SCR_BALANCE,				// Default Screen, IC Card Ready, Display Balance Screen
										// 3 Default Screens Is According To The ProcStatus[PROCESS_CR]

	DEFAULT_SCR_WAITING,				// Display Waiting Screen, Instead Of Insert Card Screen
										// For The IC Card Not Spring Out, But Refresh The IC Card Information

	DEFAULT_SCR_ERROR,					// Default Screen Card Reader Error, Include: Card Reader Reset Error And PSAM Error
	
	SCR_PRICE_UPDATE,					// Display Price Update Screen
	
	SCR_PAYMODE,						// Display Select Pay Mode
	SCR_SELECT_APP,						// Display Select Application
//Douzhq 20140523 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	SCR_SELECT_APP_SAVEOIL,				// Display Select Application , Add Saveoil App
#endif

	SCR_INPUT_PIN,						// Display Input Pin
	SCR_INPUT_EMP_PWD,					// Display Input Employee Password
	SCR_INPUT_ENTER_KEY,				// Display Input Enter Key
	SCR_INPUT_MAINTAIN_PWD,				// Display Input Maintainance Password
	SCR_INPUT_FUNCTION_PWD,				// Display Input Function Password
	
//Douzhq 20140528 Add , Lock Nozzle 
	SCR_INPUT_UNLOCK_PWD,				//Display Input Unlock Password

	SCR_INPUT_FORMAT_PWD,				// Display Input Format Config And Data Password

	SCR_INPUT_AUTHCODE,					// Display Input Auth Code For Cash Trans

	SCR_FUNCTION,						// Display Function Setup Screen When SETUP Key Pressed

	SCR_VERSION,						// Display Software Version

	SCR_INQUIRE,						// Display Inquire Screen
	
	SCR_STATION,						// Display Station Information
	SCR_RECORDSPACE,					// Display Record Space Information
	SCR_VOLUMETOTAL,					// Display Volume Total
	SCR_VOLUMETOTALLOGON,				// Display Volume Total By This Log On Employee
	SCR_AMOUNTTOTAL,					// Display Amount Total
	SCR_AMOUNTTOTALLOGON,				// Display Amount Total By This Log On Employee

#ifdef MAKE_CASH_CR
	SCR_CASHVOLUMETOTALLOGON,			// Display Cash Volume Total By This Log On Employee
	SCR_CASHAMOUNTTOTALLOGON,			// Display Cash Amount Total By This Log On Employee
	SCR_SELECTPRINT,					// Display Cash Trans Print Receipt Or Not
#endif
	
	//Douzhq 20140922 Add , Config Printer Set
	SCR_CHOOSEPRINT,					// Display Choose Print Screen
	
	SCR_PRICEINQUIRY,					// Display Price Inquiry

	SCR_INPUT_TTC,						// Display Input TTC Screen

	SCR_SETUP,							// Display Setup Screen

	SCR_SETUPBACKLIGHT,					// Display Setup Backlight
	SCR_SETUPPRINT,						// Display Setup Print
	SCR_SETUPVOLUME,					// Display Setup Volume
	SCR_SETUPPSAM,						// Display Setup PSAM Validate And Expire Date Check

	SCR_CUSTOMER_CARD_PRINT,			// Display Customer Card Print Setup
	SCR_EMP_CARD_PRINT,					// Display Employee Card Print Setup
	SCR_TEST_CARD_PRINT,				// Display Test Card Print Setup
	SCR_MAINTAIN_CARD_PRINT,			// Display Maintainance Card Print Setup
	SCR_PUB_TRANS_PRINT,				// Display Special Trans Print Setup
	//Douzhq 20140919 Add , Config Printer Set
	SCR_EXPAND_SETUP_PRINT,				// Display Expand SetUp Printer Set
	SCR_PRINTER_SWITCH_SET,				// Display Printer Enabled Switch Set Screen
	SCR_INQUIRY_SWITCH_SET,				// Display Inquiry Switch Set Screen
	SCR_RECEIPT_SWITCH_SET,				// Display Receipt Switch Set Screen
	
	SCR_MATN_FUNCTION,					// Display Maintainance Screen
	SCR_UNFINISHED_TRANS,				// Display Unfinished Trans Screen
	SCR_UPLOAD,							// Display Upload Managment Screen
	SCR_UPLOAD_READY,					// Display Upload Management Ready Screen
	SCR_FORMAT,							// Display Format Flash Screen
	SCR_POSP,							// Display Setup POSP Screen
	SCR_LOCAL_PRINTER,					// Display Setup Local Printer Screen
	SCR_SOFTWARE_UPGRADE,				// Display Software Upgrade Screen

	SCR_FORMATCONFIG,					// Display Format Config Flash Screen
	SCR_FORMATRECORD,					// Display Format Record Flash Screen
	SCR_DEBITPOWERON,					// Display The Information For Power Down Debit Or Power On
	SCR_POSBLACKLIST,					// Display The Information For Check Black List First On POS Or Local

	SCR_DISP_TRANS,						// Display The Trans Record

#ifdef MAKE_PSAM_VERIFY

	SCR_PSAM_V_UNSUPPORT,				// Display PSAM Unsupport Verify Function
	SCR_PSAM_V_UNREGISTERED,			// Display PSAM UnCertificate Pump
	SCR_PSAM_V_PSAM_LOCK,				// Display PSAM Locked For Certificate Pump
	SCR_PSAM_V_INSERT_RID,				// Display Insert RID Card Please
	SCR_PSAM_V_REGISTER_OK,				// Display PSAM Register OK Screen

#endif

#ifdef MAKE_CARD_MODE2
	SCR_WORK_MODE,						// Display Work Mode Set Screen
#endif

//#ifndef MAKE_CARD_MODE2
    SCR_WORK_MANAGE,                    // Display Work Manage Set Screen
	
//Douzhq 20140609 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	SCR_MANAGE_SOCIAL,					// Display Social Manage Set Screen
#endif

	SCR_LOCK_NOZZLE,	// Display Lock Nozzle Set Screen

//Douzhq 20141010 Add , Safe Mode
//-------  Begin , Douzhq Add , Safe Mode , 20141010 -------
	SCR_SAFE_MODE,						// Display Safe Mode Set Screen
//-------  End   , Douzhq Add , Safe Mode , 20141010 -------
	
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	SCR_ICCARD_RECOGINZE,				// Display IC Card Recoginze Set Screen
	SCR_ICCARD_SELECT,					// Display IC Card Recoginze Set Screen (On / Off)
	SCR_CARDAPP_SELECT,					// Display IC Card Recognize Applaction Set Screen
	SCR_SAVEOIL_FUN,					// Display Save Oil Set Screen
	SCR_VIPFUELING_FUN,					// Display Vip Fueling Set Screen
#endif

//Douzhq 20140819 Add , Menu About
#if defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	SCR_MANAGE_SOCIAL1,					// Display Social Manage Set Screen
#endif

//Douzhq 20140819 Add , Car Number Recognize
//Menu About
#ifdef MAKE_CASH_CR
	SCR_MANAGE_CARDNUM,					// Display Car Number Manage Set Screen
	SCR_CARNUMBER_RECOGNIZE,			// Display Car Number Recognize Screen
	
	DEFAULT_CARNUM_REC,					// Display Car Number Recognizing Screen
	DEFAULT_GRADELIMIT_REC,				// Display Grade Limit Recognizing Screen
#endif

    SCR_INPUT_EMP_NUMBER,               // Display Input Emp Number Screen
//#endif    

    SRC_OVERFUELING,                    // Display over fueling screen  20110906
    SRC_OVERFUELPASSWORD,               // Display input password screen  20110906

	SCREEN_MAX,							// Just For Enum END
};


//// Define The List Matrix Coordinate
//#define List_Line_Interval				1
//
//#define List_Title_Line_TopX			0
//#define List_Title_Line_TopY			0
//#define List_Title_Line_BottomX			127
//#define List_Title_Line_BottomY			12
//
//// Left Short List Item 1
//#define List_Left_Short_1_TopX			0
//#define List_Left_Short_1_TopY			14
//#define List_Left_Short_1_BottomX		62
//#define List_Left_Short_1_BottomY		List_Left_Short_1_TopY + 12
//
//// Right Short List Item 1
//#define List_Right_Short_1_TopX			64
//#define List_Right_Short_1_TopY			14
//#define List_Right_Short_1_BottomX		127
//#define List_Right_Short_1_BottomY		List_Right_Short_1_TopY + 12
//
//// Left Short List Item 2
//#define List_Left_Short_2_TopX			List_Left_Short_1_TopX
//#define List_Left_Short_2_TopY			List_Left_Short_1_TopY + List_Line_Interval + 12
//#define List_Left_Short_2_BottomX		List_Left_Short_1_BottomX
//#define List_Left_Short_2_BottomY		List_Left_Short_2_TopY + 12
//
//// Right Short List Item 2
//#define List_Right_Short_2_TopX			List_Right_Short_1_TopX
//#define List_Right_Short_2_TopY			List_Right_Short_1_TopY + List_Line_Interval + 12
//#define List_Right_Short_2_BottomX		List_Right_Short_1_BottomX
//#define List_Right_Short_2_BottomY		List_Right_Short_2_TopY + 12
//
//// Left Short List Item 3
//#define List_Left_Short_3_TopX			List_Left_Short_2_TopX
//#define List_Left_Short_3_TopY			List_Left_Short_2_TopY + List_Line_Interval + 12
//#define List_Left_Short_3_BottomX		List_Left_Short_2_BottomX
//#define List_Left_Short_3_BottomY		List_Left_Short_3_TopY + 12
//
//// Right Short List Item 3
//#define List_Right_Short_3_TopX			List_Right_Short_2_TopX
//#define List_Right_Short_3_TopY			List_Right_Short_2_TopY + List_Line_Interval + 12
//#define List_Right_Short_3_BottomX		List_Right_Short_2_BottomX
//#define List_Right_Short_3_BottomY		List_Right_Short_3_TopY + 12
//
//// Left Short List Item 4
//#define List_Left_Short_4_TopX			List_Left_Short_3_TopX
//#define List_Left_Short_4_TopY			List_Left_Short_3_TopY + List_Line_Interval + 12
//#define List_Left_Short_4_BottomX		List_Left_Short_3_BottomX
//#define List_Left_Short_4_BottomY		List_Left_Short_4_TopY + 12
//
//// Right Short List Item 4
//#define List_Right_Short_4_TopX			List_Right_Short_3_TopX
//#define List_Right_Short_4_TopY			List_Right_Short_3_TopY + List_Line_Interval + 12
//#define List_Right_Short_4_BottomX		List_Right_Short_3_BottomX
//#define List_Right_Short_4_BottomY		List_Right_Short_4_TopY + 12
//
//// Long List Item 1
//#define List_Long_1_TopX				List_Left_Short_1_TopX
//#define List_Long_1_TopY				List_Left_Short_1_TopY
//#define List_Long_1_BottomX				127
//#define List_Long_1_BottomY				List_Long_1_TopY + 12
//
//// Long List Item 2
//#define List_Long_2_TopX				List_Long_1_TopX
//#define List_Long_2_TopY				List_Long_1_TopY + List_Line_Interval + 12
//#define List_Long_2_BottomX				List_Long_1_BottomX
//#define List_Long_2_BottomY				List_Long_2_TopY + 12
//
//// Long List Item 3
//#define List_Long_3_TopX				List_Long_2_TopX
//#define List_Long_3_TopY				List_Long_2_TopY + List_Line_Interval + 12
//#define List_Long_3_BottomX				List_Long_2_BottomX
//#define List_Long_3_BottomY				List_Long_3_TopY + 12
//
//// Long List Item 4
//#define List_Long_4_TopX				List_Long_3_TopX
//#define List_Long_4_TopY				List_Long_3_TopY + List_Line_Interval + 12
//#define List_Long_4_BottomX				List_Long_3_BottomX
//#define List_Long_4_BottomY				List_Long_4_TopY + 12
//// Define End

// Define The List Matrix Coordinate
#define List_Line_Interval				1

#define List_Title_Line_TopX			0
#define List_Title_Line_TopY			0
#define List_Title_Line_BottomX			127
#define List_Title_Line_BottomY			12

// Left Short List Item 1 ( 0, 14; 62, 26 )
#define List_Left_Short_1_TopX			0
#define List_Left_Short_1_TopY			14
#define List_Left_Short_1_BottomX		62
#define List_Left_Short_1_BottomY		List_Left_Short_1_TopY + 12

// Right Short List Item 1 ( 64, 14; 127, 26 )
#define List_Right_Short_1_TopX			64
#define List_Right_Short_1_TopY			List_Left_Short_1_TopY
#define List_Right_Short_1_BottomX		127
#define List_Right_Short_1_BottomY		List_Right_Short_1_TopY + 12

// Left Short List Item 2 ( 0, 26; 62, 38 )
#define List_Left_Short_2_TopX			List_Left_Short_1_TopX
#define List_Left_Short_2_TopY			List_Left_Short_1_BottomY
#define List_Left_Short_2_BottomX		List_Left_Short_1_BottomX
#define List_Left_Short_2_BottomY		List_Left_Short_2_TopY + 12

// Right Short List Item 2 ( 64, 26; 127, 38 )
#define List_Right_Short_2_TopX			List_Right_Short_1_TopX
#define List_Right_Short_2_TopY			List_Left_Short_2_TopY
#define List_Right_Short_2_BottomX		List_Right_Short_1_BottomX
#define List_Right_Short_2_BottomY		List_Right_Short_2_TopY + 12

// Left Short List Item 3 ( 0, 38; 62, 50 )
#define List_Left_Short_3_TopX			List_Left_Short_2_TopX
#define List_Left_Short_3_TopY			List_Left_Short_2_BottomY
#define List_Left_Short_3_BottomX		List_Left_Short_2_BottomX
#define List_Left_Short_3_BottomY		List_Left_Short_3_TopY + 12

// Right Short List Item 3 ( 64, 38; 127, 50 )
#define List_Right_Short_3_TopX			List_Right_Short_2_TopX
#define List_Right_Short_3_TopY			List_Right_Short_2_BottomY
#define List_Right_Short_3_BottomX		List_Right_Short_2_BottomX
#define List_Right_Short_3_BottomY		List_Right_Short_3_TopY + 12

// Left Short List Item 4 ( 0, 50; 62, 62 )
#define List_Left_Short_4_TopX			List_Left_Short_3_TopX
#define List_Left_Short_4_TopY			List_Left_Short_3_BottomY
#define List_Left_Short_4_BottomX		List_Left_Short_3_BottomX
#define List_Left_Short_4_BottomY		List_Left_Short_4_TopY + 12

// Right Short List Item 4 ( 64, 50; 127, 62 )
#define List_Right_Short_4_TopX			List_Right_Short_3_TopX
#define List_Right_Short_4_TopY			List_Right_Short_3_BottomY
#define List_Right_Short_4_BottomX		List_Right_Short_3_BottomX
#define List_Right_Short_4_BottomY		List_Right_Short_4_TopY + 12

// Long List Item 1 ( 0, 14; 127, 26 )
#define List_Long_1_TopX				List_Left_Short_1_TopX
#define List_Long_1_TopY				List_Left_Short_1_TopY
#define List_Long_1_BottomX				127
#define List_Long_1_BottomY				List_Long_1_TopY + 12

// Long List Item 2 ( 0, 26; 127, 38 )
#define List_Long_2_TopX				List_Long_1_TopX
#define List_Long_2_TopY				List_Long_1_BottomY
#define List_Long_2_BottomX				List_Long_1_BottomX
#define List_Long_2_BottomY				List_Long_2_TopY + 12

// Long List Item 3 ( 0, 38; 127, 50 )
#define List_Long_3_TopX				List_Long_2_TopX
#define List_Long_3_TopY				List_Long_2_BottomY
#define List_Long_3_BottomX				List_Long_2_BottomX
#define List_Long_3_BottomY				List_Long_3_TopY + 12

// Long List Item 4 ( 0, 50; 127, 62 )
#define List_Long_4_TopX				List_Long_3_TopX
#define List_Long_4_TopY				List_Long_3_BottomY
#define List_Long_4_BottomX				List_Long_3_BottomX
#define List_Long_4_BottomY				List_Long_4_TopY + 12
// Define End



// Disp List Item Structer
typedef struct
{
	unsigned char ListTopX;				// List Item TopX
	unsigned char ListTopY;				// List Item TopY
//	unsigned char ListWidth;			// List Item Width
//	unsigned char ListHeight;			// List Item Height
	unsigned char ListBottomX;			// List Item BottomX
	unsigned char ListBottomY;			// List Item BottomY

	enum DISPMSGID ListItemTxtID;		// List Item Text ID

}LISTITEM;

// Screen Structer
typedef struct
{
	unsigned char ListCnt;				// List Items Cnt In This Screen, Not Include The Title List, 8 Item At Most
	enum SCREENTYPE ScrType;			// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
	LISTITEM ScrTitle;					// Screen Title Bar Msg ID
	LISTITEM DispItem[8];				// List Items, Up to 8 List Items
} SCRNSTR;

typedef struct
{
    void (*pScreenFunc)(void);  /* Format for Screen Function */
} SCREENFUNC;







extern const SCRNSTR ScreenList[SCREEN_MAX];


//extern unsigned int MsgAttrib;

//extern unsigned int MsgDelayTime;

extern unsigned char InfoLine[21];
extern unsigned char CurrentInfoLine[21];

// Attention: Do Not Change The CustomMsgs's Defination; Seriously
// Attention: Do Not Change The CustomMsgs's Defination; Seriously
extern unsigned char CustomMsg0[21];
extern unsigned char CurrentCustomMsg0[21];

extern unsigned char CustomMsg1[21];
extern unsigned char CurrentCustomMsg1[21];

extern unsigned char CustomMsg2[21];
extern unsigned char CurrentCustomMsg2[21];

extern unsigned char CustomMsg3[21];
extern unsigned char CurrentCustomMsg3[21];

extern unsigned char CustomMsg4[21];
extern unsigned char CurrentCustomMsg4[21];

extern unsigned char CustomMsg5[21];
extern unsigned char CurrentCustomMsg5[21];

extern unsigned char CustomMsg6[21];
extern unsigned char CurrentCustomMsg6[21];

extern unsigned char CustomMsg7[21];
extern unsigned char CurrentCustomMsg7[21];

extern unsigned char CustomMsg8[21];
extern unsigned char CurrentCustomMsg8[21];

extern unsigned char CustomMsg9[21];
extern unsigned char CurrentCustomMsg9[21];
// Attention: Do Not Change The CustomMsgs's Defination; Seriously
// Attention: Do Not Change The CustomMsgs's Defination; Seriously




// Screen Display Attrib
//extern unsigned int ScrAttrib;

// Current Screen Delay TimeOut
// Current Screen Will Remain On Display, Until TimeOut
//extern unsigned int ScrDelayTime;

// Screen Id: For Display SCREEN
extern enum SCREEN ScreenID;
extern enum SCREEN NextScreenID;
extern enum SCREEN LastScreenID;

// Save The Displayed Default Screen Status
// According To ProcStatus[PROCESS_CR]
// Is Changed, Display The New Default Screen
extern unsigned int LastDefaultSCRStatus;

// The Inversed Display List Item Index
extern unsigned char DispListInversedIdx;

// The Display Item Index For Trans Record
extern unsigned char DispRecordIdx;



#endif
