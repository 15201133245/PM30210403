

#include "CRvaribles.h"



#ifdef MAKE_PSAM_VERIFY

const unsigned char GilbarcoID[2] = { 0x30, 0x32 };

unsigned char RegisterStep = 0;
unsigned char ACTRegisterStep = 0;

#endif


// Default Sinopec IC Card
enum CR_TYPE CRType = CR_TYPE_CARD;
// The IC Card Command Running Flag	Offset In RecvBuffer
unsigned char RespCMDOffset = 0;
// The IC Card Command Response Data Offset In RecvBuffer
unsigned char RespDATOffset = 0;

// Spring Out IC Card Times
unsigned char SpringOutCardTimes = 0;
unsigned char CRCardCommErrTimes=0;
unsigned char GetGMACRetryTimes = 0;
unsigned char SaveRecordFailFlag = 0;

TRINP_DEF TRINP; 

JOURNAL RECORD; 

PININFO PINInfo;

//unsigned char TempBuffer[36] = { "0123456789ABCDEF0123456789ABCDEF\x0D\x0A" };
//int TimeInterval = 0;
int ProcTimes = 0;

unsigned char RecvBuffer[256];
unsigned int RecvBufferLen = 0;
unsigned int ReceivedFlag = 0;

unsigned char SendBuffer[256];
unsigned int SendBufferLen = 0;
unsigned char SentFlag = 0;


// The CR Power On And Down Flag
unsigned char CRPowerFlag;

// Sping Our IC Card Flag, For The Procedure But Not Spring The IC Card
// 0: Send The Spring Out Command
// 1: Do Not Send The COmmand, Only Return OK
unsigned char HoldCard = 0;

// Update Balance Screen Display Flag For Customer Card
unsigned char KeepBalScreen = 0;
// Keep Balance Screen Display Timer
Tmr * BalScreenKeepTimer;

// Save The Communication Error Times
// Compare With CR_COMM_ERR_MAX_TIMES
unsigned char CommErrorTimes = 0;

// For Test Time
//unsigned char TestTimeOut = 0;

//// For Check BCC Of Card Reader
//unsigned char TmpBCC;

// Process Flag
// Define The Local Message Exchange Flag With Other Processes
// Maintained By Card Reader Porcess
unsigned int CR_Task_Flag = 0;


// CR_ICCARD_Status: Save The Current IC Card Status
// CR_ICCARD_Status: Same As The Public Status For Other Process Inquire
//unsigned int CR_ICCARD_Status = 0;


// CR_Proc_Step: Save The Current Steps For The IC Card Process
// CR_Proc_Step:
//unsigned int CR_Proc_Step = 0;
//unsigned int CR_Proc_Last_Step = 0;		// Save Last Step
enum CR_PROC_STEP CR_Proc_Step = CR_PROC_START;
enum CR_PROC_STEP CR_Proc_Last_Step = CR_PROC_START;		// Save Last Step


// CR_Command_Type: Save The Current Command Type
// Command Type: All The Commands To Card Reader
//unsigned int CR_CommandType = 0;
//unsigned int CR_Last_CommandType = 0;	// Save Last Command Type
enum CR_COMMAND_TYPE CR_CommandType = CR_CommandStart;
enum CR_COMMAND_TYPE CR_Last_CommandType = CR_CommandStart;	// Save Last Command Type

// CR_CommandStatus: Save the Current Command Status To Card Reader
// Command Status: enum{}CR_COMMANDSTATUS;
//unsigned int CR_CommandStatus = 0;enum CR_COMMAND_STATUS
enum CR_COMMAND_STATUS CR_CommandStatus = CR_CommandStatusStart;

// Save The Cala MAC Or Encrypt Data
enum CR_MAC_TYPE CR_MACType;
// Save The Process And Command After Calc MAC
enum CR_COMMAND_TYPE CR_MAC_CommandType;
enum CR_PROC_STEP CR_Proc_MAC_Step;


// Used For ACK TimeOut Process
Tmr * CR_Command_ACK_TimeOut;
// Used For Response TimeOut Process
Tmr * CR_Command_Response_TimeOut;

// IC Card Default PIN Length
//unsigned char ICCard_Default_PIN_Len = 0x04;
unsigned char ICCard_Default_PIN_Len = 0x02;
// IC Card Default PIN
unsigned char ICCard_Default_PIN[2] = { 0x99, 0x99 };



// Card Reader Command 2 Bytes
// Card Reader Reset
const unsigned char CR_RESET_CR[2] = { 0x30, 0x30 };
// Card Reader Reset And Spring Out Card
const unsigned char CR_RESET_SPRING_CARD[2] = { 0x30, 0x31 };
// Card Reader State, Wether Card In Or Not
const unsigned char CR_CARD_STATE[2] = { 0x31, 0x30 };
// Card Reader Spring Out Card
const unsigned char CR_SPRING_CARD[2] = { 0x32, 0x30 };
// IC Card Power On
const unsigned char CR_CARDPOWERON[2] = { 0x33, 0x30 };
// IC Card Power Off
const unsigned char CR_CARDPOWEROFF[2] = { 0x33, 0x31 };

// PSAM Power Off
const unsigned char CR_PSAMPOWEROFF[2] = { 0x4A, 0x31 };


// Polling Type A Contactless IC Card
const unsigned char CR_POLL_TYPE_A[2] = { 0x35, 0x60 };

// Polling Type B Contactless IC Card
const unsigned char CR_POLL_TYPE_B[2] = { 0x35, 0x61 };

// Deselect Contactless IC Card
const unsigned char CR_DESELECT[2] = { 0x35, 0x68 };

// Contactless IC Card C_APDU Command
const unsigned char ICCARD_CAPDU_COMMAND_CL[2] = { 0x35, 0x65 };

// Get Card Type
const unsigned char CR_GET_CARDTYPE[2] = { 0x31, 0x31 };





// IC Card Cold Reset In 7816 Mode
const unsigned char ICCARD_COLD_RESET_7816[3] = { 0x37, 0x30, MODE_7816 };
// IC Card Cold Reset In EMV Mode
const unsigned char ICCARD_COLD_RESET_EMV[3] = { 0x37, 0x30, MODE_EMV };
// IC Card Warm Reset
const unsigned char ICCARD_WARM_RESET_7816[2] = { 0x37, 0x2F };
// IC Card T0 CAPDU Command
const unsigned char ICCARD_CAPDU_COMMAND_T0[2] = { 0x37, 0x31 };
// IC Card T1 CAPDU Command
const unsigned char ICCARD_CAPDU_COMMAND_T1[2] = { 0x37, 0x32 };


// PSAM CAPDU Command T0 Mode
const unsigned char PSAM_CAPDU_COMMAND_T0[2] = { 0x3D, 0x31 };
// PSAM CAPDU Command T1 Mode
const unsigned char PSAM_CAPDU_COMMAND_T1[2] = { 0x3D, 0x32 };


// TTC For The Transaction
unsigned int TransTTC;


// Voice Busy Issued Or Not Flag
unsigned char CRVoiceBusyFlag = 0;

// For Save RunningLog
unsigned char CRLogTxt[52];
// For Save CR Running Step Log
unsigned char CRStepLog[52];



// Save The Power Down And Spring Card Flag
// Used For Write Spring Log
// 00 ---- 189: Save Power Down And SPring Card
// 190 ----   : Save Spring Card Without Power Down
// Current Is Pd_Flag_110 For Power Down Log
// And Pd_Flag_198 For Spring Card Without Power Down
enum POWER_DOWN_FLAG PowerDownFlag = Pd_Flag_Start;

// Record some error information, such as 'SW'...
unsigned int ErrorFlag = 0;

// IC Card Type
// Contact, Contactless Type A And Type B
enum ICCARDTYPE CardType;

#ifdef MAKE_CASH_CR
// For Keypad Select Print Or Not Print Cash
enum KEYPAD_SELECT_PRINT KeypadPrint;
#endif

//Douzhq 20140926 Add , Config Printer Set
enum KEYPAD_SELECT_PRINT_CARD KeypadPrintCard;

// Journal Flag
// For Running Log, Mark The Journal Write
// Current: 33
unsigned char JournalFlag = 0;


// MAC Buffer
// For Calc MAC Or Encrypt Data Source Buffer
unsigned char MACDataLen;
unsigned char MACBuffer[128];

#ifdef MAKE_CARD_MODE2
	unsigned char SendCardRecord = 0;				// 1: Sinopec Mode2: Card Is Yellow, Need Send Card Trans Record After Fuelling
													// Other: Don't Care
	unsigned char CurrentLogs = 0;					// Save The Logs Number For Current POS Package
	unsigned char LogItems = 0;						// Save The Total POS Package Number

	unsigned char TransAmountPay = 0;				// Save The Employee Card Payment Methode: 1: Payment In Office
													// 										   Other: Payment Out Of Office
	unsigned char PresetMode2 = 0;					// Save The Preset Is PumpCall Or CardReady: 1: Preset In CardReady

	unsigned char OnlineFlag = 0;					// Online Abnormal Trans Flag: 1: Should Be Online Abnormal Trans
#endif	// MAKE_CARD_MODE2


// For Test, Save The List Status, Ok Or Not 0 = Ok
//unsigned char BlackListStatus = 0;
//unsigned char WhiteListStatus = 0;
//unsigned char GreyStatus = 0;

// For Test, Save The List Status, Ok Or Not 0 = Ok

// For Test, Save The POS Status, 0 = POS Online; 1= POS Offline
//unsigned char POSStatus = 0;
// For Test, Save The POS Status, 0 = POS Online; 1= POS Offline

