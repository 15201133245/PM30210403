#include <string.h>
#include "SocialCard.h"
#include "ICCardCommand.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "CRCommand.h"
#include "define.h"
#include "Screen.h"
#include "Message.h"
#include "PdProtect.h"
#include "WriteLog.h"
#include "global.h"
#include "Pdprotect.h"
#include "global.h"
#include "ICCardInProc.h"
#include "CRPumpCall.h"

//Douzhq 20140929 Add , Config Printer Set
// When One CR Has Two Or More Nozzle,
// Trans Logon , Trans Logoff ,Trans Recvpice , Trans NonCard Ect,
// Set This Flag , Cross Printer Inquiry Function
unsigned char PrintFlag = 0;	//0x00 , Next Print Choose Inquiry , Other No 
								//0x01 , Next Print No Choose Inquiry , PrintIt
								//0x02 , Next Print No Choose Inquiry , Not Print It

//Douzhq 20140826 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
Tmr *CarNumberRecWait;
unsigned char CarWaitBlack = 30;
unsigned char CarOverFlag = 0;
unsigned char CarOverFlag2 = 0;
#endif

//Douzhq 20140510 Add , IC Card Recognize
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
static const unsigned char SOCIAL_APP_AID[12] = { 0xA0, 0x00, 0x00, 0x00, 0x03, 'O', 'I', 'L', 'P', 'L', '0', '1' };
#endif

//Douzhq 20140610 Add , IC Card Recognize
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)

enum AID_TYPE AidType;

unsigned short int ICCard_Select_Social_APP( unsigned char *pBuffer )
{
	unsigned short int len;

	len = 0;

	// Spare For len

	len += 2;

	// CLA
	*( pBuffer + len ) = 0x00;
	len += 1;
	
	// INS
	*( pBuffer + len ) = 0xA4;
	len += 1;
	
	// Para1 = 0x04;
	*( pBuffer + len ) = 0x04;
	len += 1;
	
	// Para2 = 0x00;
	*( pBuffer + len ) = 0x00;
	len += 1;

	*( pBuffer + len ) = sizeof( SOCIAL_APP_AID );
	len += 1;
	
	memcpy( pBuffer + len, SOCIAL_APP_AID, sizeof( SOCIAL_APP_AID ));
	len += sizeof( SOCIAL_APP_AID );
	
	// Save To The first 2 Bytes
	// 
	ShortIntToCharCopy( len - 2, pBuffer );

	return len;
}

unsigned short int CR_ICCARD_Select_Social_APP( void )
{
	unsigned short int retval;

	// Default Excuting
	retval = 1;

	// Select Sinopec_APP Application Command
	retval = CR_CAPDU_Command_T0_ICCard( ICCard_Select_Social_APP );

	if( retval == 0x9000 )
	{
		// Command Excuted Ok, Save The Response
//		ResponseLen = CharToShortIntCopy( &RecvBuffer[RespDATOffset] );
				
		// Debit Source
		TRINP.TR_TRANS_DEBITSOURCE = 0x00;

		// Trans Unit
		TRINP.TR_TRANS_UNIT = TRINP.TR_TRANS_DEBITSOURCE;

		// Trans Card Type
		TRINP.TR_CARD_TYPE = 0;

		// Inform Display
//		DISP_CR_Buf.AppType = TRINP.TR_TRANS_DEBITSOURCE;


		// Mark The Sinopec Application
//		TRINP.TR_TRANS_APP = 0x00;
	}

	// Return
	return retval;
}

//Init IC Card Recognize Switch And Show "*"
void InitICCardRecognizeSwitch(unsigned int RecognizeFlag)
{
  	unsigned int TempFlag = RecognizeFlag;
	
	memset(CustomMsg0,0x00,sizeof(CustomMsg0));
	CustomMsg0[0] = ' ';
	memcpy(&CustomMsg0[1],DISPMSG[M_SinopecAppRecognize],strlen(DISPMSG[M_SinopecAppRecognize]));
	
	memset(CustomMsg1,0x00,sizeof(CustomMsg1));
	CustomMsg1[0] = ' ';
	memcpy(&CustomMsg1[1],DISPMSG[M_SocialAppRecognize],strlen(DISPMSG[M_SocialAppRecognize]));
	
	memset(CustomMsg2,0x00,sizeof(CustomMsg2));
	CustomMsg2[0] = ' ';
	memcpy(&CustomMsg2[1],DISPMSG[M_Other1AppRecognize],strlen(DISPMSG[M_Other1AppRecognize]));
	
	memset(CustomMsg3,0x00,sizeof(CustomMsg3));
	CustomMsg3[0] = ' ';
	memcpy(&CustomMsg3[1],DISPMSG[M_Other2AppRecognize],strlen(DISPMSG[M_Other2AppRecognize]));
	
	switch (TempFlag)
	{
		case SinopecAppFlag:
			AidType = M_SINOPEC;
			CustomMsg0[0] = '*';
			break;
		case SocialApp1Flag:
			AidType = M_SOCIAL;
			CustomMsg1[0] = '*';
			break; 
		case OtherApp1Flag:
			AidType = M_OTHER1;
			CustomMsg2[0] = '*';
			break;	
		case OtherApp2Flag:
			AidType = M_OTHER2;
			CustomMsg3[0] = '*';
			break;
		default:
		    AidType = M_SINOPEC;
		  	break;
	}
}

//Clear Recognize Flag And Clear Show "*" 
void ClearAllRecognizeFlag(unsigned int *RecognizeFlag)
{
	*RecognizeFlag = 0;
	
	CustomMsg0[0] = ' ';
	CustomMsg1[0] = ' ';
	CustomMsg2[0] = ' ';
	CustomMsg3[0] = ' ';	
}

//Set Recognize Flag And AidType
void SetRecognizeFlag(unsigned int AppFlag,unsigned int *RecognizeFlag)
{
	*RecognizeFlag |= AppFlag;
	
	switch (AppFlag)
	{
		case SinopecAppFlag:
			AidType = M_SINOPEC;
			break;
		case SocialApp1Flag:
			AidType = M_SOCIAL;
			break; 
		case OtherApp1Flag:
			AidType = M_OTHER1;
			break;	
		case OtherApp2Flag:
			AidType = M_OTHER2;
			break;
		default:
		  	AidType = M_SINOPEC;
		  	break;
	}
	
}

//Read AIT_TYPE
extern enum AID_TYPE ReadRecognizeFlag(void)
{
	unsigned int TempCardRecognizeMode = CharToIntCopy(PowerDown.IcCardSwitchMode);
	
	switch (TempCardRecognizeMode)
	{
		case SinopecAppFlag:
			AidType = M_SINOPEC;
			break;
		case SocialApp1Flag:
			AidType = M_SOCIAL;
			break; 
		case OtherApp1Flag:
			AidType = M_OTHER1;
			break;	
		case OtherApp2Flag:
			AidType = M_OTHER2;
			break;
		default:
		  	AidType = M_SINOPEC;
		  	break;
	}
	
	return AidType;
}

/*
 ** GetVipGradePrice
 *
 *  PARAMETERS:	nozzle : nozzle Number
 *  			vipPrice : Get Vip Price 
 *
 *  DESCRIPTION: Input nozzle and Get Vip Price And Vip Grade
 *
 *  RETURNS: Vip Grade.
 *
 */
//Douzhq 20140620 Add , Vip Fueling
unsigned char GetVipGradePrice(unsigned char nozzle,unsigned short *vipPrice)
{
  	unsigned char vipGrade = 0;
	char errorMessage[30];
	
	vipGrade = TRINP.TR_CARD_SFI_28_RFU[29];
	// Vip Fueling Testing
	//vipGrade = 2;
	
	if (vipGrade <= 0x07)
	{
		*vipPrice = GetPrice( nozzle, vipGrade );
	}
	else
	{
		*vipPrice = GetPrice(nozzle , 0);
		vipGrade = 0;
	}
	
	if (*vipPrice > 0 && *vipPrice < 9900)
	{
		return vipGrade;
	}
	else 
	{
	  	sprintf(errorMessage,"err:price:%u,greap:%u",*vipPrice,vipGrade);
	  	WriteLog(PROCESS_POS,(unsigned char *)errorMessage);
		*vipPrice = (GetPrice(nozzle , 0));
		vipGrade = 0;
	}
	return vipGrade;
}

#endif

//Douzhq 20140820 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
void GradeLimitRec(void)
{
	// Brand Limited, Power Off Next
	// Inform POS Brand Limited Card
	memcpy( POS_CR_Buf.ICCARD_STATE, "\x10\x01", sizeof( POS_CR_Buf.ICCARD_STATE ));
									
	// For Record Brand Limited Card
	memcpy( TRINP.TR_CARD_GREY_TAC, "\x10\x01\x00\x00", sizeof( TRINP.TR_CARD_GREY_TAC ));
									
	SaveICCardData();
			
	// Save It For Record
	FillCardErrorData();
			
	// Auth Error
	FillAuthError();
			
	// Write Log
	//CRWriteLog( M_MsgBrandLimitedExceed );
			
	// Display: "  Grade Brand Limited "
	FillInfoMsg( M_MsgBrandLimitedExceed, 0, 0 );
								
	// For Write The Running Log
	JournalFlag = 1;
			
	// Form Journal Next
	//Set_Next_Command( CR_TRANS_JOURNAL, CR_FILL_JOURNAL );
}
#endif
