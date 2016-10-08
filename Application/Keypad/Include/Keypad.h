#ifndef _KEYPAD_H
#define _KEYPAD_H



// Define The Display Trans
extern JOURNAL KeyDispTrans; 

// Define The Maintainance Password: 1309
extern const char MatnPWD[2];

// Define The Function Password: 9999
extern const char FuncPWD[2];

typedef struct
{
    void (*pKeypadFunc)(void);  /* Format for Keypad Function */
} KEYPADFUNC;

extern const KEYPADFUNC KeypadTable[]; 


extern void InputStart( void );

extern void InputCard( void );
extern void InputCardBusy( void );
extern void InputCardBalance( void );

extern void InputCardBalance_Card( void );
#ifdef MAKE_CASH_CR
extern void InputCardBalance_Auth( void );
extern void InputCardBalance_Acceptor( void );
#endif

extern void InputCardWaiting( void );
extern void InputCardError( void );
extern void InputPriceUpdate( void );
extern void InputSelectPayMode( void );
extern void InputSelectApp( void );

//Douzhq 20140523 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	extern void InputSelectAppSaveoil(void);
#endif

extern void InputPin( void );
extern void InputEMPPWD( void );
extern void InputENTERKey( void );
extern void InputMaintainPWD( void );
extern void InputFunctionPWD( void );

//Douzhq 20140523 Add , Lock Nozzle
extern void InputUnlockPWD(void);

extern void InputFormatPWD( void );
extern void InputAuthCode( void );
extern void InputFunction( void );
extern void InputVersion( void );
extern void InputInquire( void );
extern void InputSetupFunction( void );
extern void InputBacklighttime( void );
extern void InputPrint( void );
extern void InputVolume( void );
extern void InputPSAM( void );
extern void InputCustomerPrint( void );
extern void InputEMPPrint( void );
extern void InputTestPrint( void );
extern void InputMatnPrint( void );
extern void InputPubTransPrint( void );

//Douzhq 20140919 Add , Config Printer Set
extern void InputExpandSetPrint( void );
extern void InputPrinterSwitchSet( void );
extern void InputInquirySwitchSet( void );
extern void InputReceiptSwitchSet( void );

extern void InputMatnFunction( void );
extern void InputUnfinishedTrans( void );
extern void InputUpload( void );
extern void	InputUploadReady( void );
extern void InputFormat( void );
extern void InputPOSP( void );
extern void InputLocalPrinter( void );
extern void InputSoftwareUpgrade( void );
extern void InputFormatConfig( void );
extern void InputFormatRecord( void );
extern void InputDebitPowerDown( void );
extern void InputBlackListSetup( void );
extern void InputDispTrans( void );
extern void InputStation( void );
extern void InputRecordSpace( void );
extern void InputVolumeTotal( void );
extern void InputVolumeTotalLogOn( void );
extern void InputAmountTotal( void );
extern void InputAmountTotalLogOn( void );

#ifdef MAKE_CASH_CR
extern void InputCashVolumeTotalLogOn( void );
extern void InputCashAmountTotalLogOn( void );
extern void InputSelectPrint( void );
#endif

//Douzhq 20140922 Add , Config Printer Set
extern void InputChoosePrint( void );

extern void InputPriceInquiry( void);
extern void InputTTC( void );
extern void InputOverFueling( void );
extern void InputOverFuelingPassword( void );

extern void InputEnd( void );





extern unsigned char IssueLog( unsigned char flag );
extern void IssueLogDispInfoLine( unsigned char flag );
extern void GetPreset( void );
extern void GetCurrentBackLightTime( void );
extern void ScrollDownSelect( void );
extern void ScrollUpSelect( void );
extern void GetPrintSetStatus( unsigned char type );
extern void UpdateInversedDisplay( unsigned char index );
extern void SetPrintIt( unsigned int PrintFlag );
extern void ClearPrintIt( unsigned int PrintFlag );
extern void GetUnfinishedTrans( void );
extern void GetTrans( void );
extern void GetTrans0( void );
extern void GetTrans1( void );
extern void GetTrans2( void );
extern void GetTrans3( void );
extern void GetTrans4( void );
extern void GetTrans5( void );
extern void GetTrans6( void );
extern void GetTransType( unsigned char *pDest, unsigned char TransType );
extern void TrimDecimal( unsigned char *pBuffer, unsigned char type );
extern void GetStationInfo( void );
extern void GetDispRecordSpace( void );

extern void GetVolumeTotal( void );
extern void GetAmountTotal( void );
extern void GetVolumeTotalLogOn( void );
extern void GetAmountTotalLogOn( void );

#ifdef MAKE_CASH_CR
extern void GetCashVolumeTotalLogOn( void );
extern void GetCashAmountTotalLogOn( void );
#endif

extern void GetPSAMCheck( void );
extern void GetPriceInfo( void );
extern void GetVersionDispMessage( void );


#ifdef MAKE_PSAM_VERIFY

extern void InputPSAM_V_UnSupport( void );

extern void InputPSAM_V_UnRegistered( void );

extern void InputPSAM_V_Locked( void );
extern void InputPSAM_V_Insert_RID( void );
extern void InputPSAM_V_Register_Ok( void );


#endif


#ifdef MAKE_CARD_MODE2
extern void InputWorkModeSet( void );
#endif

//#ifndef MAKE_CARD_MODE2
extern void InputWorkManage( void );

//Douzhq 20140610 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
extern void InputManageSocial( void );
#endif

extern void InputLockNozzleSet( void );

//Douzhq 20141010 Add , Safe Mode
//-------  Begin , Douzhq Add , Safe Mode , 20141010 -------
extern void InputSafeModeSet( void );
//-------  End   , Douzhq Add , Safe Mode , 20141010 -------

#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
//IC Card Recognize
extern void InputCardRecoginze( void );
extern void InputICCardSwitch( void );
extern void InputCardAppSwitch( void );

extern void InputSaveOilAppSet( void );
extern void InputVipFuelingSet( void );
#endif

//Douzhq 20140819 Add , Menu About
//Manage Menu , PASM Verify and No CASH
#if defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
extern void InputManageSocial1(void);
#endif

#ifdef MAKE_CASH_CR
	void InputManageCarNum(void);
	void InputCarNumberRec(void);
#endif

extern void InputEMPNumber( void );
//#endif




extern void DispIdle( void );






extern void KeypadProcess( void );
extern void InitKeypadProcess( void );

extern void KeyPressed( void );

extern void DestroyInput( void );

extern unsigned char ASC2BCD_F( unsigned char *pBCD, unsigned char *pASC, unsigned char len );


#endif
