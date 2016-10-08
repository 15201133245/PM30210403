#ifndef _SOCIALCARD_H
#define _SOCIALCARD_H

#include "Message.h"
#include "global.h"

//Douzhq 20140929 Add , Config Printer Set
extern unsigned char PrintFlag;

//Douzhq 20140826 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
extern Tmr *CarNumberRecWait;
extern unsigned char CarWaitBlack;
extern unsigned char CarOverFlag;
extern unsigned char CarOverFlag2;
#define CARWAITTiMEOUT 300
#endif

//Douzhq 20140610 Add , IC Card Recognize
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)

//IC Card Recognize Flag , It's For RecognizeFlag
#define SinopecAppFlag 0x00000001				// Sinopec  App Select
#define SocialApp1Flag 0x00000002				// 新华联   App Select
#define OtherApp1Flag  0x00000004				// Reserve1 App Select
#define OtherApp2Flag  0x00000008				// Reserve2 App Select

//AID Type 
enum AID_TYPE
{
	M_SINOPEC,
	M_SOCIAL,
	M_OTHER1,
	M_OTHER2
};

extern enum AID_TYPE AidType;

//Douzhq 20140610 Add , IC Card Recognize
//新华联 Applaction
extern unsigned short int ICCard_Select_Social_APP( unsigned char *pBuffer );
extern unsigned short int CR_ICCARD_Select_Social_APP( void );

extern void InitICCardRecognizeSwitch(unsigned int RecognizeFlag);
extern void ClearAllRecognizeFlag(unsigned int *RecognizeFlag);
extern void SetRecognizeFlag(unsigned int AppFlag,unsigned int *RecognizeFlag);
extern enum AID_TYPE ReadRecognizeFlag(void);

//Get Vip Grade And Vip Price 
extern unsigned char GetVipGradePrice(unsigned char nozzle,unsigned short *vipPrice);

#endif

#ifdef MAKE_CASH_CR
extern void GradeLimitRec(void);
#endif

#endif
