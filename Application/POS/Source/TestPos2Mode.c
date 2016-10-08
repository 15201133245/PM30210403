#include "POSMode2.h"
#include "TestPos2Mode.h"
#include "global.h"
#include "define.h"
#ifdef TEST_POS_MODE2

unsigned int Ttc = 1;

void testInit(void)
{
	PosOnline = 1;
	ProcExchange[PROCESS_CR][PROCESS_POS] |= AUTH_EMP;
	
	testPos2Var.PollMax = 3;
	testPos2Var.PollCount = 0;
	testPos2Var.AckFlg = 1;
	testPos2Var.ResultFlg = 1;
}

void ClrPollCount(void)
{
	testPos2Var.PollCount = 0;
}

void SetPollMax(unsigned char val)
{
	testPos2Var.PollMax = val;
}

void SetAckFlg(void)
{
	testPos2Var.AckFlg = 1;
}

void ClrAckFlg(void)
{
	testPos2Var.AckFlg = 0;
}

void SetResultFlg(void)
{
	testPos2Var.ResultFlg = 1;
}

void ClrResultFlg(void)
{
	testPos2Var.ResultFlg = 0;
}

void SetAuthBgFlg(void)
{
	testPos2Var.AuthBgFlg = 1;
}

void ClrAuthBgFlg(void)
{
	testPos2Var.AuthBgFlg = 0;
}

void SetAuthModeFlg(void)
{
	testPos2Var.AuthModeFlg = 1;
}

void ClrAuthModeFlg(void)
{
	testPos2Var.AuthModeFlg = 0;
}

void SetPosAuthMode(unsigned char val)
{
	testPos2Var.AuthMode = val;
}

void SetPosStopFlg(void)
{
	testPos2Var.PosStopFlg = 1;
}

void ClrPosStopFlg(void)
{
	testPos2Var.PosStopFlg = 0;
}

void SetStop_PollMax(unsigned int val)
{
	testPos2Var.Stop_PollMax = val;
}

void SetStop_PollCount(unsigned int val)
{
	testPos2Var.Stop_PollCount = val;
}

void SetNDD_PollMax(unsigned char val)
{
	testPos2Var.NDD_PollMax = val;
}

void SetNDD_PollCount(unsigned char val)
{
	testPos2Var.NDD_PollCount = val;
}

void SetPay_X(unsigned char val)
{
	testPos2Var.Pay_X = val;
}

void TestPosMode2(void)
{
	unsigned short ttc,crc;
	unsigned char buf[256];
	
	if(PosState == POS_CommandWaitingResponse)
	{
		if((SndPos.CurState == AUTH_REQ2) && (testPos2Var.AckFlg))
		{
			buf[0] = 0xfa;
			buf[1] = 0;
			buf[2] = 0xff;
			buf[3] = PosSndBuf[3];
			buf[4] = 0;
			buf[5] = 0x04;
			buf[6] = CMD_ACK;
			buf[7] = AUTH_REQ2;
			buf[8] = 1;
			buf[9] = 2;
			crc = Crc16(&buf[1],9,0);
			crc = ShortEndianConvert(crc);
			memcpy(&buf[10],(unsigned char *)(&crc),2);
			memcpy(SinopecRevBuf.buf,buf,12);
			SinopecRevBuf.len = 12;
			PosFlg = 1;
		}
		else if((SndPos.PreState == AUTH_REQ2) && (SndPos.CurState == POLL))
		{
			if(testPos2Var.PollCount == testPos2Var.PollMax)
			{
				crc = 100;
				buf[0] = 0xfa;
				buf[1] = 0;
				buf[2] = 0xff;
				buf[3] = PosSndBuf[3];
				buf[4] = 0;
				buf[5] = 0x48;
				buf[6] = AUTH_FB;
				buf[7] = 1;
				Ttc = IntEndianConvert(Ttc);
				memcpy(&buf[8],(unsigned char *)(&Ttc),4);
				Ttc++;
				buf[12] = 1;
				crc = ShortEndianConvert(crc);
				memcpy(&buf[13],(unsigned char *)(&crc),2);
				buf[15] = 20;
				memset(&buf[16],0,20);
				ttc = 10000;
				ttc = IntEndianConvert(ttc);
				memcpy(&buf[36],(unsigned char *)(&ttc) + 1,3);
				memset(&buf[39],0,10);
				buf[49] = 1;
				memset(&buf[50],1,4);
				crc = Crc16(&buf[1],53,0);
				crc = ShortEndianConvert(crc);
				memcpy(&buf[54],(unsigned char *)(&crc),2);
				memcpy(SinopecRevBuf.buf,buf,56);
				SinopecRevBuf.len = 56;
				PosFlg = 1;
			}
			else
			{
				testPos2Var.PollCount++;
				buf[0] = 0xfa;
				buf[1] = 0;
				buf[2] = 0xff;
				buf[3] = PosSndBuf[3];
				buf[4] = 0;
				buf[5] = 0x17;
				buf[6] = VER_INFO;
				buf[7] = 0x20;
				buf[8] = 0x11;
				buf[9] = 0x09;
				buf[10] = 0x13;
				buf[11] = 0x14;
				buf[12] = 0x30;
				buf[13] = 0x00;
				crc = GetListVersion(0);
				memcpy(&buf[14],(unsigned char *)(&crc),2);
				crc = GetListVersion(1);
				buf[16] = *((unsigned char *)(&crc) + 1);
				crc = GetListVersion(2);
				buf[17] = *((unsigned char *)(&crc) + 1);
				crc = GetListVersion(3);
				buf[18] = *((unsigned char *)(&crc) + 1);
				buf[19] = GradePrice.Ver;
				buf[20] = OilMessage.ver;
				memset(&buf[21],0,2);
				crc = Crc16(&buf[1],22,0);
				crc = ShortEndianConvert(crc);
				memcpy(&buf[23],(unsigned char *)(&crc),2);
				memcpy(SinopecRevBuf.buf,buf,25);
				SinopecRevBuf.len = 25;
				PosFlg = 1;
			}
		}
		else if((SndPos.PreState == NOZZLE_DOWN_DATA) && (SndPos.CurState == POLL))
		{
			if(testPos2Var.NDD_PollCount == testPos2Var.NDD_PollMax)
			{
				buf[0] = 0xfa;
				buf[1] = 0;
				buf[2] = 0xff;
				buf[3] = PosSndBuf[3];
				buf[4] = 0;
				buf[5] = 0x32;
				buf[6] = PAYMENT_DATA;
				buf[7] = 1;
				ttc = IntEndianConvert(Ttc);
				memcpy(&buf[8],(unsigned char *)(&ttc),4);
				buf[12] = testPos2Var.Pay_X;
				ttc = 10000;
				ttc = IntEndianConvert(ttc);
				memcpy(&buf[13],(unsigned char *)(&ttc) + 1,3);
				memcpy(&buf[16],(unsigned char *)(&ttc) + 1,3);
				memset(&buf[19],0,5);
				memset(&buf[24],0x11,14);
				crc = Crc16(&buf[1],37,0);
				crc = ShortEndianConvert(crc);
				memcpy(&buf[38],(unsigned char *)(&crc),2);
				memcpy(SinopecRevBuf.buf,buf,40);
				SinopecRevBuf.len = 40;
				PosFlg = 1;
			}
			else
			{
				testPos2Var.NDD_PollCount++;
				buf[0] = 0xfa;
				buf[1] = 0;
				buf[2] = 0xff;
				buf[3] = PosSndBuf[3];
				buf[4] = 0;
				buf[5] = 0x17;
				buf[6] = VER_INFO;
				buf[7] = 0x20;
				buf[8] = 0x11;
				buf[9] = 0x09;
				buf[10] = 0x13;
				buf[11] = 0x14;
				buf[12] = 0x30;
				buf[13] = 0x00;
				crc = GetListVersion(0);
				memcpy(&buf[14],(unsigned char *)(&crc),2);
				crc = GetListVersion(1);
				buf[16] = *((unsigned char *)(&crc) + 1);
				crc = GetListVersion(2);
				buf[17] = *((unsigned char *)(&crc) + 1);
				crc = GetListVersion(3);
				buf[18] = *((unsigned char *)(&crc) + 1);
				buf[19] = GradePrice.Ver;
				buf[20] = OilMessage.ver;
				memset(&buf[21],0,2);
				crc = Crc16(&buf[1],22,0);
				crc = ShortEndianConvert(crc);
				memcpy(&buf[23],(unsigned char *)(&crc),2);
				memcpy(SinopecRevBuf.buf,buf,25);
				SinopecRevBuf.len = 25;
				PosFlg = 1;
			}
		}
		else if((SndPos.PreState == AUTH_REQ1) && (SndPos.CurState == POLL))
		{
			if(testPos2Var.PollCount == testPos2Var.PollMax)
			{
				crc = 100;
				buf[0] = 0xfa;
				buf[1] = 0;
				buf[2] = 0xff;
				buf[3] = PosSndBuf[3];
				buf[4] = 0;
				buf[5] = 0x48;
				buf[6] = AUTH_FB;
				buf[7] = 1;
				Ttc = IntEndianConvert(Ttc);
				memcpy(&buf[8],(unsigned char *)(&Ttc),4);
				Ttc++;
				buf[12] = 1;
				crc = ShortEndianConvert(crc);
				memcpy(&buf[13],(unsigned char *)(&crc),2);
				buf[15] = 20;
				memset(&buf[16],0,20);
				ttc = 10000;
				ttc = IntEndianConvert(ttc);
				memcpy(&buf[36],(unsigned char *)(&ttc) + 1,3);
				memset(&buf[39],0,10);
				buf[49] = 1;
				memset(&buf[50],1,4);
				crc = Crc16(&buf[1],53,0);
				crc = ShortEndianConvert(crc);
				memcpy(&buf[54],(unsigned char *)(&crc),2);
				memcpy(SinopecRevBuf.buf,buf,56);
				SinopecRevBuf.len = 56;
				PosFlg = 1;
			}
			else
			{
				testPos2Var.PollCount++;
				buf[0] = 0xfa;
				buf[1] = 0;
				buf[2] = 0xff;
				buf[3] = PosSndBuf[3];
				buf[4] = 0;
				buf[5] = 0x17;
				buf[6] = VER_INFO;
				buf[7] = 0x20;
				buf[8] = 0x11;
				buf[9] = 0x09;
				buf[10] = 0x13;
				buf[11] = 0x14;
				buf[12] = 0x30;
				buf[13] = 0x00;
				crc = GetListVersion(0);
				memcpy(&buf[14],(unsigned char *)(&crc),2);
				crc = GetListVersion(1);
				buf[16] = *((unsigned char *)(&crc) + 1);
				crc = GetListVersion(2);
				buf[17] = *((unsigned char *)(&crc) + 1);
				crc = GetListVersion(3);
				buf[18] = *((unsigned char *)(&crc) + 1);
				buf[19] = GradePrice.Ver;
				buf[20] = OilMessage.ver;
				memset(&buf[21],0,2);
				crc = Crc16(&buf[1],22,0);
				crc = ShortEndianConvert(crc);
				memcpy(&buf[23],(unsigned char *)(&crc),2);
				memcpy(SinopecRevBuf.buf,buf,25);
				SinopecRevBuf.len = 25;
				PosFlg = 1;
			}
		}
		else if((SndPos.PreState == POLL) && (SndPos.CurState == POLL))
		{
			if(testPos2Var.AuthBgFlg)
			{
				buf[0] = 0xfa;
				buf[1] = 0;
				buf[2] = 0xff;
				buf[3] = PosSndBuf[3];
				buf[4] = 0;
				buf[5] = 0x19;
				buf[6] = AUTH_BG;
				buf[7] = 1;
				memset(&buf[8],0,4);
				buf[12] = 0x20;
				buf[13] = 0x11;
				buf[14] = 0x09;
				buf[15] = 0x13;
				buf[16] = 0x16;
				buf[17] = 0x00;
				buf[18] = 0x00;
				
				buf[19] = 0x15;
				
				buf[20] = 0x12;
				buf[21] = 0x34;
				
				ttc = 10000;
				ttc = IntEndianConvert(ttc);
				memcpy(&buf[22],(unsigned char *)(&ttc) + 1,3);
				crc = Crc16(&buf[1],24,0);
				crc = ShortEndianConvert(crc);
				memcpy(&buf[25],(unsigned char *)(&crc),2);
				memcpy(SinopecRevBuf.buf,buf,27);
				SinopecRevBuf.len = 27;
				PosFlg = 1;
			}
			else if(testPos2Var.AuthModeFlg)
			{
				buf[0] = 0xfa;
				buf[1] = 0;
				buf[2] = 0xff;
				buf[3] = PosSndBuf[3];
				buf[4] = 0;
				buf[5] = 0x03;
				buf[6] = AUTH_MODE;
				buf[7] = 0;
				buf[8] = testPos2Var.AuthMode;
				crc = Crc16(&buf[1],8,0);
				crc = ShortEndianConvert(crc);
				memcpy(&buf[9],(unsigned char *)(&crc),2);
				memcpy(SinopecRevBuf.buf,buf,11);
				SinopecRevBuf.len = 11;
				PosFlg = 1;
			}
			else if(testPos2Var.PosStopFlg)
			{
				if(testPos2Var.Stop_PollCount == testPos2Var.Stop_PollMax)
				{
					buf[0] = 0xfa;
					buf[1] = 0;
					buf[2] = 0xff;
					buf[3] = PosSndBuf[3];
					buf[4] = 0;
					buf[5] = 0x13;
					buf[6] = STOP;
					buf[7] = 1;
					ttc = IntEndianConvert(Ttc);
					memcpy(&buf[8],(unsigned char *)(&ttc),4);
					buf[12] = 0x20;
					buf[13] = 0x11;
					buf[14] = 0x09;
					buf[15] = 0x14;
					buf[16] = 0x12;
					buf[17] = 0x30;
					buf[18] = 0x00;
					crc = Crc16(&buf[1],18,0);
					crc = ShortEndianConvert(crc);
					memcpy(&buf[19],(unsigned char *)(&crc),2);
					memcpy(SinopecRevBuf.buf,buf,21);
					SinopecRevBuf.len = 21;
					PosFlg = 1;
				}
				else
				{
					testPos2Var.Stop_PollCount++;
					buf[0] = 0xfa;
					buf[1] = 0;
					buf[2] = 0xff;
					buf[3] = PosSndBuf[3];
					buf[4] = 0;
					buf[5] = 0x17;
					buf[6] = VER_INFO;
					buf[7] = 0x20;
					buf[8] = 0x11;
					buf[9] = 0x09;
					buf[10] = 0x13;
					buf[11] = 0x14;
					buf[12] = 0x30;
					buf[13] = 0x00;
					crc = GetListVersion(0);
					memcpy(&buf[14],(unsigned char *)(&crc),2);
					crc = GetListVersion(1);
					buf[16] = *(unsigned char *)((&crc) + 1);
					crc = GetListVersion(2);
					buf[17] = *(unsigned char *)((&crc) + 1);
					crc = GetListVersion(3);
					buf[18] = *(unsigned char *)((&crc) + 1);
					buf[19] = GradePrice.Ver;
					buf[20] = OilMessage.ver;
					memset(&buf[21],0,2);
					crc = Crc16(&buf[1],22,0);
					crc = ShortEndianConvert(crc);
					memcpy(&buf[23],(unsigned char *)(&crc),2);
					memcpy(SinopecRevBuf.buf,buf,25);
					SinopecRevBuf.len = 25;
					PosFlg = 1;
				}
			}
			else
			{
				buf[0] = 0xfa;
				buf[1] = 0;
				buf[2] = 0xff;
				buf[3] = PosSndBuf[3];
				buf[4] = 0;
				buf[5] = 0x17;
				buf[6] = VER_INFO;
				buf[7] = 0x20;
				buf[8] = 0x11;
				buf[9] = 0x09;
				buf[10] = 0x13;
				buf[11] = 0x14;
				buf[12] = 0x30;
				buf[13] = 0x00;
				crc = GetListVersion(0);
				memcpy(&buf[14],(unsigned char *)(&crc),2);
				crc = GetListVersion(1);
				buf[16] = *(unsigned char *)((&crc) + 1);
				crc = GetListVersion(2);
				buf[17] = *(unsigned char *)((&crc) + 1);
				crc = GetListVersion(3);
				buf[18] = *(unsigned char *)((&crc) + 1);
				buf[19] = GradePrice.Ver;
				buf[20] = OilMessage.ver;
				memset(&buf[21],0,2);
				crc = Crc16(&buf[1],22,0);
				crc = ShortEndianConvert(crc);
				memcpy(&buf[23],(unsigned char *)(&crc),2);
				memcpy(SinopecRevBuf.buf,buf,25);
				SinopecRevBuf.len = 25;
				PosFlg = 1;
			}
		}
	}
}

#endif