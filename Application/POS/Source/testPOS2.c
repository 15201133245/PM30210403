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
//	unsigned char CheckFlag;					// ƥ���־	(��MODE2�д洢������־, = 2:��̨��ʱ,...)
//	unsigned char ICCARD_ASN[10];				// Ӧ�����к� (��MODE2�д濨��,����Ա��,�ǿ�����ŵ�,...)
//	unsigned char ICCARD_Balance[4];			// MSB	���
//	unsigned char TransAmount[4];				// MSB	���׽�� (��MODE2�д洢��Ȩ��3���ֽڳ���)
//
//	unsigned char ICCARD_STATE[2];				// IC ��״̬
//	unsigned char P_UNIT;                       // ���㵥λ
//
//	unsigned char TransNozzleNumber;			// ��ǰ����ǹ��
//	unsigned char TransPrice[2];				// MSB	��ǰ���͵���
//	unsigned char TransVolume[4];				// MSB	��ǰ�������� (��MODE2�д洢��Ȩ������3���ֽڳ���)
//
//
//#ifdef MAKE_CARD_MODE2
//
//// Define The Buffer For Mode2
//
//	unsigned char AUTH_MODE;					// ��ǰ��Ȩģʽ
//	unsigned char AUTH_TTC[4];					// ������Ȩ���׵�TTC
//	unsigned char AUTH_DATE[4];					// ������Ȩ����
//	unsigned char AUTH_TIME[3];					// ������Ȩʱ��
//	unsigned char AUTH_PAY_X;					// ������Ȩ���ʽ
//	unsigned char AUTH_ADD_ON[30];				// ������Ȩ������Ϣ	(��Ȩ��Ӧ��,�洢��Ӧ������Ϣ:����+����)
//	unsigned char AUTH_PIN_BLOCK[8];			// ������ȨPIN����
//	unsigned char AUTH_FB;						// ������Ȩ���ؽ���ֽ�
//	unsigned char AUTH_MAC[4];					// ������ȨMAC
//
//	unsigned char GRADE_NUMBER[2];				// ������Ʒ����
//	unsigned char CARD_CTC[2];					// ���Ϳ�����ˮ��
//	unsigned char VolumeTotal[4];				// ��������
//
//	unsigned char TransAmount1[4];				// ʵ��֧�����1��IC����
//	unsigned char TransAmount2[4];				// ʵ��֧�����2����̨�����ʻ���
//	unsigned char DiscountNumber[2];			// �Żݹ���
//
//// Define The Buffer For Mode2 End
//
//#endif	// MAKE_CARD_MODE2
//
//	unsigned char InforMessage[128];			// MSB	��Ҫ����POS��̨����Ϣ (MODE2��,���Ϳ���¼���5��ÿ��)
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