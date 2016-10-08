
#ifndef _PDPROTECT_H
#define _PDPROTECT_H


// Save Power Down Protect Data
typedef struct
{
	unsigned char PD_MULTI_TRANS_FLAG;			// Save The Multi - Trans Running Flag
												// Include: Log On, Log Off, Offline Trasn And Price Recv
												// Set When The Trans Was started, And Clear When All The Process Is End

	unsigned char PD_TRANS_FLAG;				// Save The Trans Status,
												// If PD_TRANS_FLAG >= PRE_SET_GREY And <= CLEAR_GREY_FLAG
												// Means A Trans Is Saved, Need To Form A Grey Record

	unsigned char PD_TRANS_TTC[4];				// Save The Trans TTC
	unsigned char PD_TRANS_TYPE;				// Save The Transaction Type

	unsigned char PD_TRANS_DATE[4];				// Save The Trans Date
	unsigned char PD_TRANS_TIME[3];				// Save The Trans Time

	unsigned char PD_CARD_ASN[10];				// Save The IC Card ASN
	unsigned char PD_CARD_APP_BALANCE[4];		// Save The IC Card	Application Balance
	unsigned char PD_TRANS_AMOUNT[4];			// Save The Trans Amount
	unsigned char PD_CARD_APP_TTC[2];			// Save The IC Card TTC
	unsigned char PD_CARD_MAC2[4];				// Save The IC Card MAC2
	unsigned char PD_CARD_GTAC[4];				// Save The TAC Or GTAC, Read From Grey Message
	unsigned char PD_CARD_MAC3[4];				// Save The TAC, Read From Debit Unlock Ok
	
	unsigned char PD_PSAM_GMAC[4];				// Save The PSAM GMAC
	unsigned char PD_PSAM_TAC[4];				// PSAM TAC
	unsigned char PD_PSAM_ASN[10];				// PSAM ASN
	unsigned char PD_PSAM_TID[6];				// PSAM TID
	unsigned char PD_PSAM_TTC[4];				// Save The PSAM TTC

	unsigned char PD_TRANS_DEBITSOURCE;			// Debit Source
	unsigned char PD_TRANS_UNIT;				// Debit Unit
	unsigned char PD_CARD_TYPE;					// Transaction IC Card Type

	unsigned char PD_CARD_KEY_VERSION;			// Save The IC Card Key Version
	unsigned char PD_PSAM_KEY_INDEX;			// Save The PSAM Key Index, Combine With The PD_CARD_KEY_VERSION,
												// To Generate The CARD_KEY In Journal

	unsigned char PD_NOZZLENUMBER;				// Transaction Nozzle Number
	unsigned char PD_GRADE_CODE[2];				// Grade Code, Product Code
	unsigned char PD_TRANS_VOLUME[4];			// Transaction Volume
	unsigned char PD_TRANS_PRICE[2];			// Transaction Price

	unsigned char PD_CASH_TOTAL[4];				// Total Cash Amount Authed Or Accepted For Cash Trans
	unsigned char PD_AUTH_CODE_LEN;				// Auth Code Length For Cash Trans
	unsigned char PD_AUTH_CODE[12];				// Auth Code For Cash Trans

#ifdef MAKE_CARD_MODE2
	// For Sinopec Mode2
	unsigned char PD_WORK_MODE;					// Save The Work Mode
	unsigned char PD_AUTH_MODE;					// Save The Auth Mode
	unsigned char PD_PAY_X;						// Save The PayX
	unsigned char PD_TRANS_AMOUNT_T1[4];		// Save The TransAmount1
	unsigned char PD_TRANS_AMOUNT_T2[4];		// Save The TransAmount2
	unsigned char PD_TRANS_AMOUNT_OVER_F[4];	// Save The OverFuelling Amount
	unsigned char PD_DISCOUNT_NUMBER[2];		// Save The Discount Number
	unsigned char PD_CARD_HOLDER_NEW_FLAG;		// Sinopec Mode2: Card Holder Flag ( 71st Byte SFI 1C )
	unsigned char PD_CARD_HOLDER_TYPE[2];		// Sinopec Mode2: Card Holder Type ( 74th And 75th Bytes SFI 1C )
#endif

	unsigned char PD_CLEAR_FLAG;				// Up Was Clear By Every Card Inserted

	unsigned char PD_EMPLOYEE_NUMBER;			// Transaction Employee Number
	unsigned char PD_TRANS_VOLUME_TOTAL[4];		// Total Volume	For This Transaction

	unsigned char PD_VOLUME_TOTALS[4*4];		// Save The Volume Total, For Employee Log On,
												// Log Off And Non IC Card Transaction Journal
	
	unsigned char PD_AMOUNT_TOTALS[4*4];		// Save The Amount Total
	
	unsigned char PD_OFFL_TRANS_AMOUNT[4*4];	// Save The Offline Trans Amount
	unsigned char PD_OFFL_TRANS_VOLUME[4*4];	// Save The Offline Trans Volume

}PD_TRANS;

//extern PD_Protect_CR  Pd_Save;
//extern PD_TRANS  PdTrans;


// Save LogOn Employee Information
typedef struct
{
	unsigned char LogOnFlag;					// Employee Log On Flag
	unsigned char EMPNumber;					// Employee Number
	unsigned char EMPPassword[2];				// Employee Password

	unsigned char EMPPinLen;					// Employee PIN Length
	unsigned char EMPPin[8];					// Employee PIN
	unsigned char EMPCardASN[10];				// Employee Card ASN
	unsigned char EMPCardBalance[4];			// Emoplyee Card Balance For PriceUpdate Logogg Trans

	unsigned char EMPGradeCode[4][2];			// Employee Card Log On Grade Code

	unsigned char LogOnVolumeTotals[4*4];		// Log On Volume Totals For Calc Duty Total
	unsigned char LogOnAmountTotals[4*4];		// Log On Amount Totals For Calc Duty Total
}LOG_ON_CARD;

//extern LOG_ON_CARD	LogOnCard;

// Used For Multi-Journal In One Step,
// Such As LogOn, LogOff, Price Receiving ....
// Save Total Nozzle Number Need For Journal
//extern unsigned char NozzleNumbers;
// Save Current Nozzle Journal
//extern unsigned char CurrentNozzleNumber;


// Define The Bits Of SpecialFunction
#define	DebitOnPowerOn 		0x01				// Debit On Power On While Power Down On Fuelling
												// If Set, Do Not Issue Debit Unlock On Power Down
												// If Clear, Issue STOP And Issue Debit Unlock On Power Down While Fuelling

#define	CheckBlackListPOS	0x02				// Check Black List On POS Or Local First
												// If Set, First Check Black List On POS
												// If Clear, Check Black List Local First

#define	CheckPSAMValidate	0x04				// Check PSAM Validate Date And Expire Date Or Not
												// If Set, Check PSAM Validate
												// If Clear, Do Not Check PSAM Validate


// Save All PowerDown Information
typedef struct
{
	unsigned char PDStartFlag;					// Power Down Protecting Start Flag

	unsigned char PdVersion[20];				// Version Number, Get When IC Card Process Initialize, For Reference

	unsigned char NozzleNumbers;				// Used For Multi-Journal In One Step,
												// Such As LogOn, LogOff, Price Receiving ....
												// Save Total Nozzle Number Need For Journal
	
	unsigned char CurrentNozzleNumber;			// Save Current Nozzle Journal
	
	unsigned char SpecialFunction;				// Save The Special Functions For The Smart Card System
												// Bit0: Debit On Power On Or Power Down
												// Bit1: Check Black List On POS Or Local First; 1: Check On POS First
												// Bit2: Check PSAM Validate Or Not; 1: Check PSAM Validate
												// Bit3: RFU
												//

	unsigned char LocalPrinter;					// 1: Local Printer; 0: Remote Printer
	unsigned char PrintIt[4];					// Print The Receipt Or Not

	LOG_ON_CARD	LogOnCard;						// Log On Information

	unsigned char WorkMode;						// Work Mode: 0x01: Online Mode; Other: Offline Mode
	unsigned char AuthMode;						// Auth Mode: 0x01: HostPos Auth; Other: Front Pump Auth
    
    unsigned char WorkManage;                   // Work Manage: 0x01: Need To Input Emp Number; Other: Not Need
	
//Douzhq Add 20140625 , Mode
	unsigned char LockNozzleMode;				// Lock Nozzle Mode        : 0X00 : off ; 0x01 : on
	
//Douzhq Add 20141010 , Safe Mode
//-------  Begin , Douzhq Add , Safe Mode , 20141010 -------
	unsigned char SafeMode;						// Safe Mode 			   : 0x00 : off , 0x01 : on
//-------  End   , Douzhq Add , Safe Mode , 20141010 -------
	
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	unsigned char CardRecoginMode;				// Card Auto Recoginze Mode: 0x00 : off ; 0x01 : on
	unsigned char IcCardSwitchMode[4];			// Switch IC Card Applaction
	
	unsigned char SaveOilAppMode;				// Save Oil Applaction Mode: 0x00 : off ; 0x01 : on
	
	unsigned char VipFuelingMode;				// Vip Fueling Mode : 0x00 : off ; 0x01 : on
#endif
//Douzhq 20140819 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
	unsigned char CarNumRecognizeModw;			// Car Number Recognize Mode 
												// 0x00 : off ; 0x01 : on ;
#endif
//Douzhq 20140922 Add , Config Printer Set 
	unsigned char PrinterSwitchMode;			// Printer Enabled Switch Mode
												// 0x00 : off ; 0x01 : on
	unsigned char InquirySwitchMode;			// Printer Inquiry Switch Mode
												// 0x00 : off ; 0x01 : on
	
	unsigned char ReceiptSwitchMode;			// Printer Receipt Switch Mode
												// 0x00 : Double ; 0x01 : Short
	
	unsigned char PDCRFlag;						// Flag For Protected Data In DataFlash, For DataFlash Operations Times Reason
												// Upper Of The Bytes Was Saved In DataFlash By Every Operation
												// Bellow Of The Bytes Was Saved In DataFlash Only PowerDown Issued

	PD_TRANS  PdTrans;							// Transaction Datas
	
	unsigned char OverFuelling_OfflineTrans;	// 1: Over Fuelling, Need Divide 1 Trans 
												// As A Normal Trans And A Offline Non IC Card Trans
												// Other: No Over Fuelling

//#ifdef MAKE_CARD_MODE2
//	unsigned char SendCardRecord;				// 1: Sinopec Mode2: Card Is Yellow, Need Send Card Trans Record After Fuelling
//												// Other: Don't Care
//#endif	// MAKE_CARD_MODE2


#ifdef MAKE_CASH_CR
	unsigned char PDDutyCashVolumeTotal[4*4];	// Logged On Duty Cash Volume Total		HEX, MSB
	unsigned char PDDutyCashAmountTotal[4*4];	// Logged On Duty Cash Amount Total	HEX, MSB
#endif
    unsigned char PDCertifyPSAMRetryTimes;      // PSAM Certify Retry Limit Times, To Avoid Lock PSAM
    unsigned char PDPhysicalNozzleNumber;       // Physical Nozzle Number, To Check Whether The Pump Nozzle Number Is Correct While Fueling
    unsigned char PDCRTrsFlg;                   // pump trans state        =0 idle ,=1trans on,has auth
  
//#ifndef MAKE_CARD_MODE2    
    unsigned char FuelEmpNumber;                // Fuel Emp Number In Work Load Management Mode
//#endif
    
	unsigned char PDEndFlag;					// Power Down Protecting End Flag
}PD_All;

extern PD_All PowerDown;

extern unsigned char Pdflag;



#endif
