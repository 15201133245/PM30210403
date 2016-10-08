#include "testPOS.h"
#include "define.h"
#include "global.h"


#ifdef MAKE_CARD_MODE2

unsigned char POSdelayed = 0;
void POSdelaytimer( void )
{
	POSdelayed = 1;
}

unsigned char POSdelayed1 = 0;
void POSdelaytimer1( void )
{
	POSdelayed1 = 1;
}

unsigned char POSdelayed2 = 0;
void POSdelaytimer2( void )
{
	POSdelayed2 = 1;
}

unsigned char POSdelayed3 = 0;
void POSdelaytimer3( void )
{
	POSdelayed3 = 1;
}

unsigned char POSdelayed4 = 0;
void POSdelaytimer4( void )
{
	POSdelayed4 = 2;
}



//typedef struct		
//{		
//	unsigned char CheckFlag;					// 匹配标志	(在MODE2中存储交互标志, = 2:后台超时,...)
//	unsigned char ICCARD_ASN[10];				// 应用序列号 (在MODE2中存卡号,操作员号,非卡条码号等,...)
//	unsigned char ICCARD_Balance[4];			// MSB	余额
//	unsigned char TransAmount[4];				// MSB	交易金额 (在MODE2中存储授权金额：3个字节长度)
//
//	unsigned char ICCARD_STATE[2];				// IC 卡状态
//	unsigned char P_UNIT;                       // 结算单位
//
//	unsigned char TransNozzleNumber;			// 当前加油枪号
//	unsigned char TransPrice[2];				// MSB	当前加油单价
//	unsigned char TransVolume[4];				// MSB	当前加油升数 (在MODE2中存储授权升数：3个字节长度)
//
//
//#ifdef MAKE_CARD_MODE2
//
//// Define The Buffer For Mode2
//
//	unsigned char AUTH_MODE;					// 当前授权模式
//	unsigned char AUTH_TTC[4];					// 申请授权交易的TTC
//	unsigned char AUTH_DATE[4];					// 申请授权日期
//	unsigned char AUTH_TIME[3];					// 申请授权时间
//	unsigned char AUTH_PAY_X;					// 申请授权付款方式
//	unsigned char AUTH_ADD_ON[30];				// 申请授权附加信息	(授权回应中,存储回应附加信息:长度+内容)
//	unsigned char AUTH_PIN_BLOCK[8];			// 申请授权PIN密文
//	unsigned char AUTH_FB;						// 申请授权返回结果字节
//	unsigned char AUTH_MAC[4];					// 申请授权MAC
//
//	unsigned char GRADE_NUMBER[2];				// 加油油品代码
//	unsigned char CARD_CTC[2];					// 加油卡卡流水号
//	unsigned char VolumeTotal[4];				// 加油总累
//
//	unsigned char TransAmount1[4];				// 实际支付金额1（IC卡）
//	unsigned char TransAmount2[4];				// 实际支付金额2（后台备付帐户）
//	unsigned char DiscountNumber[2];			// 优惠规则
//
//// Define The Buffer For Mode2 End
//
//#endif	// MAKE_CARD_MODE2
//
//	unsigned char InforMessage[128];			// MSB	需要上送POS后台的信息 (MODE2中,上送卡记录，最长5条每包)
//}POS_CR;		
//
//extern POS_CR POS_CR_Buf;

void FillAuthBackData( void )
{
	POS_CR_Buf.CheckFlag = 0x00;

//	memcpy( POS_CR_Buf.ICCARD_ASN, TRINP.TR_CARD_ASN, sizeof( POS_CR_Buf.ICCARD_ASN ));
//	memcpy( POS_CR_Buf.ICCARD_Balance, TRINP.TR_CARD_APP_BALANCE, sizeof( POS_CR_Buf.ICCARD_Balance ));
//	memcpy( POS_CR_Buf.TransAmount, "\x99\x99\x00\x00", sizeof( POS_CR_Buf.TransAmount ));
//	POS_CR_Buf.TransNozzleNumber = TRINP.TR_TRANS_NOZZLENUMBER;
//	memcpy( POS_CR_Buf.TransPrice, TRINP.TR_TRANS_PRICE, sizeof( POS_CR_Buf.TransPrice ));

	if( POS_CR_Buf.Auth_Pay_X == 0x05 )
	{
		POS_CR_Buf.Auth_Pay_X = 0x01;
	}
	else
	{
		POS_CR_Buf.Auth_Pay_X = POS_CR_Buf.Auth_Pay_X;
	}

	POS_CR_Buf.Auth_Add_On[0] = 0x09;
	memcpy( &POS_CR_Buf.Auth_Add_On[1], "ABCDEFGHIJKLMNOPQRSTUVWXYZ", POS_CR_Buf.Auth_Add_On[0] );
	memcpy( POS_CR_Buf.TransAmount, "\x00\x00\x99\x99", sizeof( POS_CR_Buf.TransAmount ));
	POS_CR_Buf.Auth_Resp = 0x01;


//	memcpy( POS_CR_Buf., "", sizeof( POS_CR_Buf. ));
}

void FillFuellingBackData( void )
{
	POS_CR_Buf.CheckFlag = 0x00;

	memcpy( POS_CR_Buf.TransAmount, "\x00\x00\x99\x99", sizeof( POS_CR_Buf.TransAmount ));

	memcpy( POS_CR_Buf.TransAmount1, "\x00\x00\x88\x88", sizeof( POS_CR_Buf.TransAmount1 ));

	memcpy( POS_CR_Buf.TransAmount2, "\x00\x00\x11\x11", sizeof( POS_CR_Buf.TransAmount2 ));

	memcpy( POS_CR_Buf.DiscountNumber, "\x44\x44", sizeof( POS_CR_Buf.TransAmount ));
}


void testpos2( void )
{
//	unsigned int i;

	// Check Need POS Auth EMP
	if(( ProcExchange[PROCESS_CR][PROCESS_POS] & AUTH_EMP ) || ( ProcExchange[PROCESS_CR][PROCESS_POS] & AUTH_NCSS ))
	{
		// Need Check Black List
		// Clear The Bit First
		ProcExchange[PROCESS_CR][PROCESS_POS] &= ( ~AUTH_EMP );
	
		// Start Counter
	   	CreateTimer( TIMER_ONE_DELETE, 1990, POSdelaytimer );
	}

	if( POSdelayed == 1 )
	{
		// Clear delay
		POSdelayed = 0;

		FillAuthBackData();

		// Inform IC Card, Check End
		ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_EMP;
	}


	// Check Need POS Auth CST
	if( ProcExchange[PROCESS_CR][PROCESS_POS] & AUTH_CST )
	{
		// Need Check Black List
		// Clear The Bit First
		ProcExchange[PROCESS_CR][PROCESS_POS] &= ( ~AUTH_CST );
	
		// Start Counter
	   	CreateTimer( TIMER_ONE_DELETE, 1990, POSdelaytimer2 );
	}

	if( POSdelayed2 == 1 )
	{
		// Clear delay
		POSdelayed2 = 0;

		FillAuthBackData();

		// Inform IC Card, Check End
		ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_CST;
	}






	// Fuelling Data
	if( ProcExchange[PROCESS_CR][PROCESS_POS] & FUELLING_DATA )
	{
		// Clear The Bit First
		ProcExchange[PROCESS_CR][PROCESS_POS] &= ( ~FUELLING_DATA );
	
		// Start Counter
	   	CreateTimer( TIMER_ONE_DELETE, 1990, POSdelaytimer1 );
	}


	if( POSdelayed1 == 1 )
	{
		// Clear delay
		POSdelayed1 = 0;

		// Fill Data
		FillFuellingBackData();

		// Inform IC Card, Check End
		ProcExchange[PROCESS_POS][PROCESS_CR] |= FUELLING_DATA;
	}


	// Send Record
	if( ProcExchange[PROCESS_CR][PROCESS_POS] & CARD_RECORD )
	{
		// Clear The Bit First
		ProcExchange[PROCESS_CR][PROCESS_POS] &= ( ~CARD_RECORD );
	
		// Start Counter
	   	CreateTimer( TIMER_ONE_DELETE, 1990, POSdelaytimer3 );
	}


	if( POSdelayed3 == 1 )
	{
		// Clear delay
		POSdelayed3 = 0;

		// Inform IC Card, Check End
		ProcExchange[PROCESS_POS][PROCESS_CR] |= CARD_RECORD;
	}

//	if( POSdelayed4 == 0 )
//	{
//		// Clear delay
//		POSdelayed4 = 1;
//
//		// Start Counter
//	   	CreateTimer( TIMER_ONE_DELETE, 24000, POSdelaytimer4 );
//	}
//
//	if( POSdelayed4 == 2 )
//	{
//		// Clear delay
//		POSdelayed4 = 1;
//
//
//		FillAuthBackData();
//		POS_CR_Buf.Auth_Pay_X = 0x15;
//
//		// Inform IC Card, Check End
//		ProcExchange[PROCESS_POS][PROCESS_CR] |= AUTH_HOST;
//	}





}







#endif