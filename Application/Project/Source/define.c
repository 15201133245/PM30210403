/*******************************************************************
 *
 *    DESCRIPTION: Define The Processes For The Sinopec ARM IC Card
 *					Payment Platform
 *
 *    AUTHOR:
 *
 *    HISTORY:    
 *
 *******************************************************************/

/** include files **/

/** local definitions **/

/* default settings */

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/

/** private functions **/




#include "define.h"

// Process Status, For Other Process Inquire And Acess
// Maintain By The Corresponding Process
// While Inquire By Other Process
unsigned int ProcStatus[PROCESS_MAX];

// Process Message Exchange,
// Line = Source; Colomn = Destination
// Source Process Sets The Certain Bit,
// While Destination Process Clear The Set Bit
unsigned int ProcExchange[PROCESS_MAX][PROCESS_MAX];



// Process Message Exchange Buffer
// POS And CR
POS_CR POS_CR_Buf;
// A Trans Can Not Save In DataFlash, So Upload It
// To POS And Inform Error For Maintaince
POS_UPLOAD POSUploadTrans;

// Process Message Exchange Buffer
// Keypad And CR
KEYPAD_CR KEYPAD_CR_Buf;

// Process Message Exchange Buffer
// Display And CR
DISP_CR DISP_CR_Buf;

//TOTALS DispTotal;

// Process Message Exchange Buffer
// PUMP And CR
PUMP_CR Pump_CR_Buf;

// Process Message Exchange Buffer
// PUMP And Print
PUMP_PRINT PumpPrinterBuf;

// Process Message Exchange Buffer
// PUMP And Voice
PUMP_VOICE PumpVoiceBuf;

// Process Message Exchange Buffer
// Schedule Process And Pump
PUMP_PRICE PumpPriceBuf[4];

// Process Message Exchange Buffer
// IC Card Process And Cash Process
CASH_CR Cash_CR_Buf;


// 灰纪录查询结构
GREY_INQUIRY GreyInquiry;

// 灰纪录查询响应结构
GREY_RESPONSE GreyResponse;

#ifdef MAKE_CASH_CR
//Douzhq 20140723 Add , Car Number Recognize
CARNUMBER_GRADE_APP CarNumGradeBuffer;
#endif

unsigned short PSAMCertificateGreyLockRetval=0;

unsigned char CertiMAC2LogType=0;






// For Test
//unsigned char BlackFit = 0;			// 0: Fitted; 1: Unfitted; 2: Error
//unsigned char WhiteFit = 0;			// 0: Fitted; 1: Unfitted; 2: Error
//unsigned char GreyFit = 0;			// 0: Fitted; 1: Unfitted; 2: Error
// For Test

//Douzhq 20140529 Add , Lock Nozzle 
LOCK_NOZZLE_STR  LOCK_NOZZLE;
