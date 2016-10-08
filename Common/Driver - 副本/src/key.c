#include "global.h"


#define KEY00		0
#define	KEY01		1
#define	KEY02		2
#define	KEY03		3
#define	KEY04		4
#define	KEY10		5
#define	KEY11		6
#define	KEY12		7
#define	KEY13		8
#define	KEY14		9
#define	KEY20		'.'
#define	KEY21		','
#define	KEY22		'a'
#define	KEY23		'b'
#define	KEY24		'c'
#define	KEY30		'd'
#define	KEY31		'f'
#define	KEY32		'g'
#define	KEY33		'h'
#define	KEY34		'i'
#define	KEY40		'j'
#define	KEY41		'k'
#define	KEY42		'l'
#define	KEY43		'm'
#define	KEY44		'n'
#define	KEY50		 0x10//定升数
#define	KEY51		 0x11//定金额
#define	KEY52		 0x12//退卡
#define	KEY53		 0x13//更改
#define	KEY54		 0x14//选择

#define	KEY55		 0x15//确认
#define	KEY56		 0x16//下翻键
#define	KEY57		 0x17//上翻键
#define	KEY58		 0x18//设置
#define	KEY59		 0x19//整/点
/* EasyCODE ) */
/* EasyCODE ( 0 
KeyOutScanCode */
static const unsigned char KeyOutScanCode[] = {
  0xfe, 0xfd, 0xfb, 0xf7, 0xef
};
/* EasyCODE ) */
/* EasyCODE ( 0 
KeyInScanCode */
static const unsigned char KeyInScanCode[] = {
  0xfe, 0xfd, 0xfb, 0xf7, 0xef
};
/* EasyCODE ) */
/* EasyCODE ( 0 
KeyCode */
static const unsigned char KeyCode[] = {
  KEY51,	KEY11,	KEY04,	KEY10,	KEY40,//定升数
  KEY52,	KEY14,	KEY12,	KEY13,	KEY41,//KEY14
  KEY53,	KEY55,	KEY59,	KEY00,	KEY42,//KEY24
  KEY54,	KEY56,	KEY58,	KEY57,	KEY02,//KEY34
  KEY50,	KEY03,	KEY01,	KEY02,	KEY44
};

void InitKeyStruct(void)
{
    memset(&Key,0,sizeof(Key));
    Key.outNumber = sizeof(KeyOutScanCode);
    Key.inNumber = sizeof(KeyInScanCode);
    Key.pOutScan = KeyOutScanCode;
    Key.pInScan = KeyInScanCode;
    Key.pCode = KeyCode;
    Key.lastScanKey = 0xff;
    Key.currentScanKey = 0xff;
    Key.lastKey = 0xff;
    Key.currentKey = 0xff;
}

void ScanKey(void)
{
    unsigned char i,j,k,tmp,flg;
    Key_t *pKey = &Key;

    flg = 0;
    for ( i = 0; i < pKey->outNumber; i++ )
    {
        WRITE_DATA8(KeyOut,pKey->pOutScan[i]);
		for( k = 0; k < 10; k++ )
		{
		}
		tmp = READ_DATA8(KeyIn);
        tmp |= 0xc0;
        if ( tmp != 0xff )
        {
            for ( j = 0; j < pKey->inNumber; j++ )
            {
                if ( tmp == pKey->pInScan[j] )
                {
                    pKey->lastScanKey = pKey->currentScanKey;
                    pKey->currentScanKey = pKey->pCode[i * pKey->outNumber + j];
                    flg = 1;
                    break;
                }
            }
        }
        if ( flg == 1 )
        {
			if(pLcdTmr)
			{
				OpenLcdLight(0);
			}
			else
			{
				LcdBackLightControl();
			}
            break;
        }
    }
    if ( i == pKey->outNumber )
    {
        pKey->lastScanKey = pKey->currentScanKey;
        pKey->currentScanKey = 0xff;
        if ( pKey->lastScanKey == pKey->currentScanKey )
        {
            pKey->lastKey = pKey->currentKey;
            pKey->currentKey = pKey->lastScanKey;
        }
    }
    if ( (flg == 1) && (pKey->keyFlg == 0) )
    {
        if ( (pKey->lastScanKey == pKey->currentScanKey) && (pKey->currentScanKey != 0xff) )
        {
            pKey->lastKey = pKey->currentKey;
            pKey->currentKey = pKey->lastScanKey;
            if ( pKey->currentKey != pKey->lastKey )
            {
                pKey->keyFlg = 1;
                //pKey->keycallback();
            }
        }
    }
}

unsigned char GetKey(void)
{
	unsigned char val;
	if(Key.keyFlg == 0)
	{       //BeepControl(300);
		return (unsigned char)(-1);
	}
	else
	{
		val = Key.currentKey;
		//Key.currentKey = 0xff;
		Key.keyFlg = 0;
	}
    //BeepControl(200);
	return val;
}

const unsigned char scan_mask[] = {0x1f,0xdf,0xbf,0x7f,0xff};

void ScanButton(void)
{
	unsigned char i,tmp,flg = 0;
	
	tmp = READ_DATA8(InputAdr);
	tmp |= 0x1f;
	for(i = 0; i < sizeof(scan_mask); ++i)
	{
		if(tmp == scan_mask[i])
		{
			Button.last_val = Button.current_val;
			Button.current_val = i;
			flg = 1;
			break;
		}
	}
	if(flg)
	{
		if((Button.last_val == Button.current_val) && (Button.last_button != Button.current_val))
		{
			if((Button.current_val != 0) && (Button.current_val < (sizeof(scan_mask) - 1)))
			{
				Button.buttonFlg = 1;
			}
			Button.last_button = Button.current_val;
		}
		/*if((Button.last_val == Button.current_val) && (Button.current_val != 0))
		{
			Button.buttonFlg = 1;
		}*/
	}
}

unsigned char GetButton(void)
{
	unsigned char val;
	
	if(Button.buttonFlg == 0)
	{
		return (unsigned char)(-1);
	}
	else
	{
		val = Button.current_val;
		Button.buttonFlg = 0;
	}
	BeepControl(200);
	return(val);
}

#define CHECK_INFRARE_TIME			20
#define CHECK_INFRARE_DELEAY		20000

void infraredDelay(void)
{
	unsigned char tmp;
	DeleteTimer(pTmrInfrared);
	pTmrInfrared = (Tmr*)0;
	tmp = READ_DATA8(InputAdr);
	infraredFlg = (tmp & 0x10) ? 1: 0;
	infraredTmrFlg = 1;
}

void checkInfrareDelay(void)
{
  	infraredTmrFlg = 1;
}

void infrareCheck(void)
{
	infraredTmrFlg = 4;
}

void infraredProcess(void)
{
	unsigned char tmp;
	
	if(infraredTmrFlg == 1)
	{
		if(pTmrInfrared != (Tmr*)0)
		{
			DeleteTimer(pTmrInfrared);
			pTmrInfrared = (Tmr*)0;
		}
		else
		{
			ResetTimer(pTmrInfrared);
		}
		tmp = READ_DATA8(InputAdr);
		if(tmp & 0x10)
		{
			infraredTmrFlg = 2;
		}
	}
	else if(infraredTmrFlg == 2)
	{
		pTmrInfrared = CreateTimer(TIMER_ONE,CHECK_INFRARE_TIME,infrareCheck);
		infraredTmrFlg = 3;
	}
	else if(infraredTmrFlg == 4)
	{
		tmp = READ_DATA8(InputAdr);
		if(tmp & 0x10)
		{
			infraredFlg = 1;
			infraredTmrFlg = 5;
			DeleteTimer(pTmrInfrared);
			pTmrInfrared = (Tmr*)0;
			pTmrInfrared = CreateTimer(TIMER_ONE,CHECK_INFRARE_DELEAY,checkInfrareDelay);
		}
		else
		{
			infraredTmrFlg = 1;
		}
	}
}
