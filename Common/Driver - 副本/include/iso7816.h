#ifndef	_ISO7816_H
#define	_ISO7816_H

typedef void (*IsoCallback )(unsigned char *,unsigned int, unsigned char);

//GET RESPONSE
/*
CLA	= 0x00;
INS = 0xc0;
P1 	= 0x00;
P2	= 0x00;
Le 	= 预期数据的最大长度

//回应
	SW1		SW2		含义
	0x64	0x00	GET RESPONSE失败
	0x67	0x00	域长度错误
	0x6a	0x86	P1 P2 != 0x00
*/

typedef union
{
	unsigned int flg;
	struct
	{
		unsigned int ta1 : 1;
		unsigned int tb1 : 1;
		unsigned int tc1 : 1;
		unsigned int td1 : 1;
		unsigned int ta2 : 1;
		unsigned int tb2 : 1;
		unsigned int tc2 : 1;
		unsigned int td2 : 1;
	};
}ISO_FLG;

typedef	union
{
	unsigned int us_mr;
	struct
	{
		unsigned int us_mode : 4;
		unsigned int us_clks : 2;
		unsigned int chrl : 2;
		unsigned int sync : 1;
		unsigned int par : 3;
		unsigned int nb_stop : 2;
		unsigned int ch_mode : 2;
		unsigned int msbf : 1;
		unsigned int mode9 : 1;
		unsigned int clko : 1;
		unsigned int over : 1;
		unsigned int inack : 1;
		unsigned int dsnack : 1;
		unsigned int reserved0 : 2;
		unsigned int max_iteration : 3;
		unsigned int reserved1 : 1;
		unsigned int filter : 1;
		unsigned int reserved2 : 3;
	};
}US_MR;

typedef	struct _At83Channel
{
	int 		status;
	ISO_FLG		Flg;
	US_MR	 	mode;
	unsigned int us_ttgr;
	unsigned int fidi;
	unsigned int baud;
	unsigned int len;
	unsigned int head_len;
}At83Ch;

typedef struct _Iso7816
{
  	AT91S_USART   *pUsartHw;
  	IsoCallback receviceback;
	IsoCallback sendback;
	IsoCallback returnback;
	unsigned char *pReceiver;
	unsigned int  ReceiveLen;
	unsigned char RevBuf[128];
	unsigned char SndBuf[128];
	unsigned short RevLen;
	unsigned short SndLen;
	unsigned short RevLength;
	unsigned short SndStartSta;
	unsigned short SndLength;
	unsigned char typ;
	unsigned char no;
	unsigned char NoTmp;
	unsigned char status;
	unsigned char para;
	At83Ch	*pAt83ch;
}Iso7816;

#endif