/*******************************************************************
 *
 *    DESCRIPTION: Define The Data Buffer Need To Protect While Power Down
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

#include "Pdprotect.h"

//PD_Protect_CR  Pd_Save;
//PD_TRANS  PdTrans;

//LOG_ON_CARD	LogOnCard;

// Used For Multi-Journal In One Step,
// Such As LogOn, LogOff, Price Receiving ....
// Save Total Nozzle Number Need For Journal
//unsigned char NozzleNumbers = 0;
// Save Current Nozzle Journal
//unsigned char CurrentNozzleNumber = 0;


PD_All PowerDown;

// Power Down Flag
// Maybe From global.h defined by Zuogang In Future
// 0: Normal Work Process
// 1: Power Down, Need Process For The Procedures
unsigned char Pdflag = 0;
