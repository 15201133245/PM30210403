#ifndef	_FUNC_H
#define	_FUNC_H
#include "def.h"

//init.c
void InitSys(void);

void InitIO(void);

void InitTWI(void);

void InitUSART0(void);

void InitUSART1(void);

void InitDbugUSART(void);

void InitDbugUSART_Func(void);

void InitTimer0(void);

void InitTimer1(void);

void InitPIT(void);

void InitSMC(void);

void InitSPI(void);

void InitM1172(unsigned char typ);

void InitSysTime(void);

void InitMemList(void);

void CheckZimo(void);
//f_irq.c
void Sys_irq(void);

void ISR_irq0(void);

void ISR_irq1(void);

void ISR_fiq(void);

void ISR_usart0(void);

void ISR_usart1(void);

void ISR_timer0(void);

void ISR_timer1(void);

void ISR_Spi(void);

//tools.c
unsigned char GetRs485Adr(unsigned int adr);

int WRITE(unsigned int id,unsigned char *pBuffer,
		  unsigned int address,unsigned int len);

int READ(unsigned int id,unsigned char *pBuffer,
		 unsigned int address,unsigned int len);

unsigned int IntEndianConvert(unsigned int val);

unsigned short ShortEndianConvert(unsigned short val);

unsigned long long LongLongEndianConvert(unsigned long long val);

void WatchDog(void);

unsigned short Crc16(unsigned char *s,unsigned short len,unsigned short LastCrc);

void SinopecToResolve(void);

void SetPosOffline(void);

void AdjustSystime(unsigned char *s,unsigned char typ);

unsigned short AddFA(unsigned char *s,unsigned short len);

void ReadSystimer(unsigned char *s);

void SetPosFlg(void);

void PowerDownDispose(unsigned char *s,unsigned int len);

void OpenOrCloseLcdLight(void);
 
void HexToComBcd(unsigned short val,unsigned char *s);

unsigned char CrcEor(unsigned char *s,unsigned short len);

void PowerDownCheck(void);

unsigned char PumpCmdRecevice(void);

void UpdateSoftwareCloseAIC(void);

unsigned short CheckCardTime(unsigned char *s);

void ForceRebuildProtected(void);

unsigned int mktime(Time* tm);

//key.c
void InitKeyStruct(void);

void ScanKey(void);

unsigned char GetKey(void);

void ScanButton(void);

unsigned char GetButton(void);

void infraredDelay(void);

void infraredProcess(void);

#if LCD_TYPE==0  //OK
//lcd_128x64.c
void InitLcd(void);

//void ReadLcdLeftState(void);

//void ReadLcdRightState(void);

void DrawHorizontalLine(unsigned char x1,
						unsigned char x2,
						unsigned char y);

void DrawVerticalline(unsigned char x,
					  unsigned char y1,
					  unsigned char y2);

void DrawRectangle(unsigned char x1,
				   unsigned char y1,
				   unsigned char x2,
				   unsigned char y2);

void LcdErr(unsigned short error,unsigned int chipId);

unsigned char ClearLcd(unsigned char x1,
					   unsigned char y1,
					   unsigned char x2,
					   unsigned char y2);

unsigned char ComputeZimo(const unsigned char *s,const unsigned char **p);

void DisplayError(unsigned char x,unsigned y,
				  const unsigned char **p,unsigned char len);

unsigned char DisplayLogo(void);

unsigned char DisplayCartoon(unsigned int no);

void DisplayChar4x5(unsigned char x,unsigned char y,unsigned char val);

void DisplayString4x5(unsigned char x,unsigned char y,unsigned char *s);

unsigned char DisplayChar5x7(unsigned char x,unsigned char y,unsigned char val);

unsigned char DisplayString5x7(unsigned char x,unsigned char y,unsigned char *s);

unsigned char DisplayChar7x12(unsigned char x,unsigned char y,unsigned char val);

unsigned char DisplayText12x12(unsigned char x,
							   unsigned char y,
							   unsigned char valx,
							   unsigned char valy);

unsigned char DisplayString12(unsigned char x,unsigned char y,unsigned char *s);

unsigned char PresetDisplay(unsigned char *s,unsigned char point,unsigned char type);

void DisplayContra(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);

void DisplayOnLine(void);

void DisBufToLcd(void);

void DisPlayHelp(const unsigned char *s);

void DisplayVerNumber(void);
#else
void InitLcd(void);

unsigned char ClearLcd(unsigned short x1,unsigned char y1,unsigned short x2,unsigned char y2);

void DrawHorizontalLine(unsigned short x1,unsigned short x2,unsigned char y);

void DrawVerticalline(unsigned short x,unsigned char y1,unsigned char y2);

void DrawRectangle(unsigned short x1,unsigned char y1,unsigned short x2,unsigned char y2);

unsigned char DisplayCartoon(unsigned int no);

void DisplayVerNumber(void);

void DisplayOnLine(void);

//16x16
void DisplayString12(unsigned short x,unsigned char y,const unsigned char* s);

void DisPlayHelp(const unsigned char* s);

void DisplayLogo(void);

void DisplayContra(unsigned short x1,unsigned char y1,unsigned short x2,unsigned char y2);

void ClearLcd2Area(void);

void LcdErr(unsigned short error,unsigned int chipId);

inline void DisBufToLcd(void) {};

unsigned char PresetDisplay(unsigned char *s,unsigned char point,unsigned char type);

void DisplayIconFormAutoCompress(unsigned char* s);

void DisplayIconFormAuto(unsigned char* s);

void setLcdControl(void);
#endif

//control.c
unsigned char ReadCardStatus(void);

unsigned char ReadCardConfig(void);

void CardOut(void);

unsigned char CheckPrint(void);

unsigned char CheckPowerDownPin(void);

void RunLed(void);

void BeepOpen(void);

void BeepClose(void);

void BeepClose1(void);

void ClearBeepFlg(void);

Tmr *BeepControl(unsigned int time);

Tmr *BeepControl2(unsigned int time);

Tmr *BeepDelay(unsigned int time);

void EnableLcdBlackLight(void);

void DisableLcdBlackLight();

void OpenLcdLight(unsigned char typ);

void CloseLcdLightOil(void);

void CloseLcdLight(void);

void LcdBackLightControl(void);

void LCD_BlackLightControl(unsigned int time);

void HexTimeToComBcdTime(void);

void SysTimer(void);

void CheckPrintStatus(void);

#ifdef OUT_TIME_MAX
unsigned char lampControl(unsigned char no,unsigned char typ,int openTime,int closeTime,int count);

unsigned char lampOpen(unsigned char no);

unsigned char lampClose(unsigned char no);

unsigned char lampGlint(unsigned char no,int openTime,int closeTime,int count);

unsigned char lampAfreshGlint(unsigned char no);
#endif

unsigned char WriteNoFormatData(unsigned char*s,unsigned int len);

unsigned char ReadNoFormatData(unsigned char* s,unsigned int len);

//AscLog.c
unsigned char AscLog(unsigned char *s);

unsigned char LoadConfig(unsigned char *s,unsigned int len);

unsigned char SaveConfig(unsigned char *s,unsigned int len);

unsigned char ReBuildLogProtected(void);

void CheckUpdatePriceTime(void);

void ReadCrConfig(unsigned char *s,unsigned short len);

void WriteCrConfig(unsigned char *s,unsigned short len);

void ReadOpenOrCloseLcdLightTime(void);

void WriteOpenOrCloseLcdLightTime(unsigned char sta,unsigned char val);

void SavePowerDownTrans(unsigned char *s,unsigned short len);

void LoadPowerDownTrans(unsigned char *s,unsigned short len);

void ErasePowerDownTrans(void);

void WriteDriveLog_Reset(unsigned char typ,unsigned int adr);

void WriteDriveLog(unsigned char typ,unsigned char *s);

//record.c
unsigned char WriteRecord(unsigned char *s);

unsigned char FindRecordTTC(unsigned int ttc,unsigned char *s);

unsigned char FindRecordTactic(unsigned int ttc,unsigned char *s);

unsigned char FindGrayRecord(unsigned char *dest,unsigned char *source);

unsigned char ReBuildRecordProtected(void);

unsigned int ReadRecord(unsigned char *s);

unsigned char MakeOverRecord(void);

//list.c
void FormatConfig(void);

void FormatRecord(void);

void FormatProtected(void);

void FormatLog(void);

void FormatAll(void);

void FormatDataFlash(unsigned char typ);

unsigned char WriteList(unsigned char typ,unsigned char *s,unsigned int len,unsigned char style);

unsigned char FindList(unsigned char typ,unsigned char *s);

void LoadGradePriceTime(void);

unsigned char BuildListLink(unsigned char typ);

void LoadOilStopMessage(void);

unsigned char ReBuildListProtected(void);

unsigned char FindBaseBlackList(unsigned char *s);

unsigned char FindAddBlackList(unsigned char *s);

unsigned char FindDelBlackList(unsigned char *s);

unsigned char FindWhiteList(unsigned char *s);

unsigned char BuildBaseBlackListLink(void);

unsigned char BuildAddBlackListLink(void);

unsigned char BuildDelBlackListLink(void);

unsigned char BuildWhiteListLink(void);

unsigned char DestroyListIntegrality(unsigned char typ);

unsigned char WriteListHead(unsigned char typ,unsigned char *s,unsigned char len);

unsigned char LoadListHead(unsigned char typ);

unsigned char LoadGradePriceList(void);

unsigned char CombinationPriceList(void);

unsigned int GetBaseBlackListCount(void);

unsigned short GetListVersion(unsigned char typ);

unsigned char WriteListLength(unsigned char typ,unsigned int len);

unsigned short GetPrice(unsigned char gdp,unsigned char prc_n);

unsigned char GetNZN(unsigned char gdp);

unsigned short GetO_Type(unsigned char gdp);

unsigned short GetCurrentPrice(unsigned char gdp,unsigned char prc_n);

unsigned char GetCurrentNZN(unsigned char gdp);

unsigned short GetCurrentO_Type(unsigned char gdp);

unsigned short GetNextPrice(unsigned char gdp,unsigned char prc_n);

unsigned char GetNextNZN(unsigned char gdp);

unsigned short GetNextO_Type(unsigned char gdp);

unsigned char CheckListVersion(unsigned char *pList);

unsigned char GetProv(void);

unsigned char GetCity(void);

unsigned int GetSuperior(void);

unsigned int GetS_ID(void);

unsigned char GetPOS_P(void);

unsigned char GetGUN_N(void);

unsigned char GetLogicNzn(unsigned char PhysicsNo);

unsigned char GetPhysicsNzn(unsigned char LogicNo);

void SetListHead(unsigned char typ);

unsigned char GetBaseListData_Area(unsigned char *s);

unsigned char GetAddListData_Area(unsigned char *s);

unsigned char GetDelListData_Area(unsigned char *s);

unsigned char GetWhiteListData_Area(unsigned char *s);

void ListIntegrality(void);

unsigned short SetPriceUpdateFlg(unsigned short val);

void WritePriceFlgToDataFlash(unsigned short PriceFlg);

unsigned short ReadPriceFlgFromDataFlash(void);

void SetPriceReceviceFlg(void);

unsigned char ReadPriceReceviceFlg(void);

void ClrPriceReceviceFlg(void);

unsigned char SetOverTurnNum(unsigned char *p,unsigned char len);

unsigned char GetOverTurnNum(unsigned char *p,unsigned char len);

//print.c
unsigned char BcdToAscii(char *d, char *s,unsigned char len);

unsigned char HexToAscii(char *s,unsigned int val);

unsigned char ReadG_Code(char* s, unsigned short g_code);

void InitPrint(void);

void ReadTicketHead(char *p);

void print_dispose(void);

void InitPrintTaskList(void);

unsigned char AddPrintTask(unsigned char *s);

unsigned char DelPrintTask(sPrintTrans *pPrint);

void PrintComplete(unsigned char *s,unsigned char val);

//CardDrive.c
void At83C26Check(void);

void Iso7816WarmReset(void);

void AnewInitUsart1(unsigned char channel);

void SndComplete(unsigned char *s,unsigned int len,unsigned char error);

int WriteToCardSam(unsigned char *pBuffer,int len,IsoCallback callback);

unsigned char SendToAt83c26(IsoCallback callback);

void At83c26Back(unsigned char type);

unsigned char IcCardComDispose(void);

unsigned char SamComDispose(void);

void SndToUsart7816(unsigned char *p,unsigned int len,IsoCallback pSndCallBack,IsoCallback pRevCallBack);

void CheckIrq1Int(void);

//at83c26_app.c
void At83TimeOut(void);

unsigned char InitCardInterface(void);

unsigned char InitCardSamClock(void);

unsigned char StartC26_dcdcA(void);

unsigned char StopC26_dcdcA(void);

unsigned char StartC26_dcdcB(void);

unsigned char StartCardSamPower(unsigned char channel);

unsigned char CardSamWarmReset(unsigned char channel);

unsigned char c26_select(unsigned char LastNo,unsigned char No);

//DbugUsartApp.c
void DbugStartRecevice(unsigned char *s,unsigned int len,unsigned char error);

void ReadRecordAdr(unsigned char *s,unsigned int page,unsigned char index);

void DbugDispose(void);

//memoryCheck.c
void SetCrcError(unsigned char val);

unsigned char GetCrcError(void);

void CalculateCrc(void);

unsigned char CheckProtectedCrc(void);

#endif