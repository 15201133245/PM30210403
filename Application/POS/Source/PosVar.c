#ifdef MAKE_CARD_MODE2
#include "POSMode2.h"
#else
#include "POS.H"
#endif
#include "PosPublic.h"
#include "PdProtect.h"

//全局变量
unsigned char Arm_g_posbuff[255];
unsigned char Arm_g_posonline;       //PC pos是否在线 1 在线 0 不在线
PcSeg PosSeg;
unsigned char PosSndBuf[256];
unsigned short PosCastLeng;
unsigned char DownLoadStatus;
Tmr *DownLoadTmr;
Tmr *SndTransTmr;
Tmr *PosAckTmr;
Tmr *PosErrTmr;
Tmr *PosSndTimeOut;
unsigned char PosReadRecordFlg = 0;
unsigned char SendCountToPos = 0;
unsigned char DownLoadPriceFlg = 0;
unsigned char PosQueListCount = 0;
#ifdef MAKE_CARD_MODE2
unsigned char RecordLast[104];
#else
unsigned char RecordLast[96];
#endif
unsigned char PosToPosBuf[16];
unsigned char BaseBlackVer[2];    
unsigned int PosReadTTC;   
unsigned int ProcExchangePos;
unsigned char limit_flg;

const unsigned char WriteDataFlashErr[] = "写名单出错";

void Save(unsigned int flg)
{
	PIO_SetInput(fiq.pio,fiq.mask,DISABLE_PULLUP,ENABLE_FILETER);
	PMC_EnablePeripheral(AT91C_ID_PIOB);
	AT91C_BASE_AIC->AIC_EOICR = 0;
	AIC_DisableIT(AT91C_ID_FIQ);
	AT91C_BASE_AIC->AIC_EOICR = 0;
	WriteDriveLog(9 + flg,0);
	SavePowerDownTrans( &PowerDown.PDStartFlag, sizeof( PD_All ));
	PowerDownDispose( &PowerDown.PDStartFlag, sizeof( PD_All ));
}

#ifdef TEST_POS_MODE2
TestPosMode2Fun testPos2Var;   
#endif

static unsigned short PosLen = 0;
static unsigned char PosTmp = 0;
static unsigned short PosDataLen = 0;
static unsigned short PosCrc = 0;
#define	POS_POLLING_TIMEOUT		10000

void SinopecToResolve(void)
{
    unsigned char tmp,flg;	
    unsigned char RevFlg = 0;
    unsigned short len = 0;
    unsigned short crc;
//liuzg ////20111020
#ifndef MAKE_CARD_MODE2	
    static Tmr *pPosTimeTmr = (Tmr *)0;
#endif
    
    m1172.pChannel = &chCtrl[0];
    flg  = ReadFromQue(&queue,&tmp);
//liuzg ////20111020
#ifndef MAKE_CARD_MODE2	
	if ( pPosTimeTmr == (Tmr *)0 )
	{
		pPosTimeTmr = CreateTimer(TIMER_ONE,POS_POLLING_TIMEOUT,SetPosOffline);
	}
#endif
    while ( !flg )
    {
        if ( (PosLen == 0) && (tmp == 0xfa) )
        {
            SinopecRevBuf.buf[PosLen++] = tmp;
        }
		else if(PosLen == 1)
		{
			if ( tmp != 0xfa )
			{

				SinopecRevBuf.buf[PosLen++] = tmp;
				POS_P = tmp;
			}
			else
			{
				PosLen = 0;
				SinopecRevBuf.buf[PosLen++] = tmp;
			}
		}
		else if(PosLen == 2)
		{
			if ( tmp != 0xfa )
			{
				SinopecRevBuf.buf[PosLen++] = tmp;
				PC_ADDR = tmp;
			}
			else
			{
				PosLen = 0;
				SinopecRevBuf.buf[PosLen++] = tmp;
			}
		}
		else if(PosLen == 3)
		{
			if ( tmp != 0xfa )
			{
				SinopecRevBuf.buf[PosLen++] = tmp;
			}
			else
			{
				PosLen = 0;
				SinopecRevBuf.buf[PosLen++] = tmp;
			}
		}
		else if(PosLen == 4)
		{
			if ( tmp != 0xfa )
			{
				PosDataLen = (tmp >> 4);
				PosDataLen *= 10;
				PosDataLen += (tmp & 0x0f);
				SinopecRevBuf.buf[PosLen++] = tmp;
			}
			else
			{
				PosLen = 0;
				SinopecRevBuf.buf[PosLen++] = tmp;
			}
		}
		else if(PosLen == 5)
		{
			if ( tmp != 0xfa )
			{
				PosDataLen *= 10;
				PosDataLen += (tmp >> 4);
				PosDataLen *= 10;
				PosDataLen += (tmp & 0x0f);
				SinopecRevBuf.buf[PosLen++] = tmp;
				if ( PosDataLen > 240 )
				{
					PosLen = 0;
				}
				PosTmp = 0;
			}
			else
			{
				PosLen = 0;
				SinopecRevBuf.buf[PosLen++] = tmp;
			}
		}
		else if((PosLen > 5) && (PosLen < (PosDataLen + 6)) && (PosDataLen != 0))
		{
			if ( (tmp == 0xfa) && (PosTmp == 0xfa) )
			{
				PosTmp = 0;
				SinopecRevBuf.buf[PosLen++] = tmp;
			}
			else if((tmp == 0xfa) && (PosTmp != 0xfa))
			{
				PosTmp = tmp;
			}
			else if((tmp != 0xfa) && (PosTmp == 0xfa))
			{
				PosTmp = 0;
				if ( tmp == POS_P )
				{
					PosLen = 2;
					SinopecRevBuf.buf[0] = 0xfa;
					SinopecRevBuf.buf[1] = POS_P;
					PosDataLen = 0;
				}
				else
				{
					PosLen = 0;
					PosDataLen = 0;
				}
			}
			else
			{
				PosTmp = 0;
                SinopecRevBuf.buf[PosLen++] = tmp;
			}
		}
		else if((PosLen == (PosDataLen + 6)) && (PosDataLen != 0))
		{
			if((tmp == 0xfa) && (PosTmp == 0xfa))
			{
				PosTmp = 0;
				PosCrc = tmp;
				PosCrc <<= 8;
				PosLen++;
			}
			else if((tmp == 0xfa) && (PosTmp != 0xfa))
			{
				 PosTmp = tmp;
			}
			else if((tmp != 0xfa) && (PosTmp == 0xfa))
			{
				PosTmp = 0;
				if ( tmp == POS_P )
				{
					PosLen = 2;
					SinopecRevBuf.buf[0] = 0xfa;
					SinopecRevBuf.buf[1] = POS_P;
					PosDataLen = 0;
				}
				else
				{
					PosLen = 0;
					PosDataLen = 0;
				}
			}
			else
			{
				PosTmp = tmp;
				PosCrc = tmp;
				PosCrc <<= 8;
				PosLen++;
			}
		}
		else if((PosLen == (PosDataLen + 7)) && (PosDataLen != 0))
		{
			if((tmp == 0xfa) && (PosTmp == 0xfa))
			{
				PosTmp = 0;
				PosCrc += tmp;
				len = PosDataLen + 6;
				RevFlg = 1;
				break;
			}
			else if((tmp == 0xfa) && (PosTmp != 0xfa))
			{
				PosTmp = tmp;
			}
			else if((tmp != 0xfa) && (PosTmp == 0xfa))
			{
				PosTmp = 0;
				if ( tmp == POS_P )
				{
					PosLen = 2;
					SinopecRevBuf.buf[0] = 0xfa;
					SinopecRevBuf.buf[1] = POS_P;
					PosDataLen = 0;
				}
				else
				{
					PosLen = 0;
					PosDataLen = 0;
				}
			}
			else
			{
				PosTmp = tmp;
				PosCrc += tmp;
				len = PosDataLen + 6;
				RevFlg = 1;
				break;
			}
		}
		else
		{
			PosLen = 0;
			PosDataLen = 0;
		}
        flg = ReadFromQue(&queue,&tmp);
    }
    if ( RevFlg )
    {
        crc = Crc16(&SinopecRevBuf.buf[1],PosDataLen + 5,0);
		PosLen = 0;
        if ( crc == PosCrc )
        {
            SinopecRevBuf.len = len;
            PosFlg = 1;
//liuzg ////20111020
#ifndef MAKE_CARD_MODE2			
            if(pPosTimeTmr != (Tmr *)0)
			{
            	ResetTimer(pPosTimeTmr);
			}
#endif 			
			PosCrc = 0;
			/*else
			{
				PosTimeId = CreateTimer(TIMER_ONE,0,60000,SetPosOffline,0);
			}*/
			flg = PosOnline;
            PosOnline = 1;
			if(flg != PosOnline)
			{
				WriteDriveLog(6,0);
			}
        }
    }
}

void SetPosOffline(void)
{
    PosOnline = 0;
	queue.head = 0;
	queue.tail = 1;
	PosLen = 0;
	PosTmp = 0;
	PosDataLen = 0;
	PosCrc = 0;
	PosFlg = 0;
//	memset((unsigned char *)(&SinopecRevBuf),0,sizeof(SinopecRevBuf));
}
