#include "global.h"

const unsigned char residue[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
const unsigned char mask_down[] = {0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80};
const unsigned char mask_up[] = {0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f};
const unsigned char mask4x5_down[] = {0xe0, 0xc1, 0x83, 0x07, 0x0f, 0x1f, 0x3f, 0x7f};
const unsigned char mask4x5_up[] = {0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xf8, 0xf0};
const unsigned char mask5x7_down[] = {0x80, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f};
const unsigned char mask5x7_up[] = {0xff, 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0};
const unsigned char mask12_down[] = {0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f};
const unsigned char mask12_up[] = {0xf0, 0xe0, 0xc0, 0x80, 0x00, 0xfe, 0xfc, 0xf8};

static const unsigned char computer_zimo[] = {0x00,0xE0,0xBE,0xB2,0xB2,0xBE,0xE0,0x00};
static const unsigned char pumpOnline[] = {0x24,0x5A,0xFE,0x82,0x8A,0x8A,0x82,0xFE};
static const unsigned char pumpOffline[] = {0x00,0x00,0xFE,0x82,0x8A,0x8A,0x82,0xFE};


#define	StartLine				0xc0
#define SetPage					0xb8
#define SetRow					0x40
#define DisplayOn				0x3f

typedef struct _LcdError
{
        unsigned char len;
        const unsigned char **sta;
}LcdError;

typedef struct _ZimoAddr_t
{
		unsigned char zimo_dh;
		unsigned char zimo_dv;
		unsigned char addr_h;
		unsigned char zimo_size;
		unsigned int addr_ml;
}ZimoAddr_t;

static const unsigned char NumChar[37][16] = {
  	{0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00},	//0
	{0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},	//1
	{0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00},	//2
	{0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00},	//3
	{0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00},	//4
	{0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00},	//5
	{0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00},	//6
	{0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00},	//7
	{0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00},	//8
	{0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00},	//9
	{0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20},	//A
	{0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00},	//B
	{0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00},	//C
	{0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00},	//D
	{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00},	//E
	{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00},	//F
	{0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00},	//G
	{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20},	//H
	{0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},	//I
	{0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00},	//J
	{0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00},	//K
	{0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00},	//L
	{0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00},	//M
	{0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00},	//N
	{0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00},	//O
	{0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00},	//P
	{0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00},	//Q
	{0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20},	//R
	{0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00},	//S
	{0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},	//T
	{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},	//U
	{0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00},	//V
	{0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00},	//W
	{0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20},	//X
	{0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},	//Y
	{0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00},	//Z
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01}	//-
};

/*static const unsigned char NumChar4x5[12][4] = 
{
	{0x0e,0x11,0x11,0x0e},
	{0x00,0x12,0x1f,0x10},
	{0x12,0x19,0x15,0x12},
	{0x11,0x15,0x15,0x0a},
	{0x0c,0x0a,0x1f,0x08},
	{0x17,0x15,0x15,0x08},
	{0x0e,0x15,0x15,0x08},
	{0x01,0x19,0x05,0x03},
	{0x0a,0x15,0x15,0x0a},
	{0x02,0x15,0x15,0x0e},
	{0x00,0x0a,0x00,0x00},
	{0x04,0x04,0x04,0x04}
};

static const unsigned char Cartoon[4][37 * 5] = 
{
	{
		0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x00,0x34,0x00,0x00,0x00,0x00,0x32,0x00,0x00,0x00,0x00,0x35,0x00,0x00,0x00,0x80,0x32,0x00,0x00,0x00,0x40,0x34,0x00,0x00, 
		0x00,0x20,0x32,0x00,0x00,0x00,0x10,0x34,0x00,0x00,0x00,0x08,0x32,0x00,0x00,0x00,0x04,0x34,0x00,0x00,0x00,0x02,0x32,0x00,0x00,0x00,0x22,0x34,0x00,0x00,0x00,0x32,0x1a,0x00,0x00,0x00,0x3a, 
		0x0c,0x00,0x00,0x00,0x1a,0x06,0x00,0xc0,0x0f,0x0a,0x03,0x00,0x20,0x10,0x82,0x01,0x10,0xb2,0x17,0xc2,0x00,0xf0,0xab,0x15,0x62,0x00,0x10,0xaa,0x15,0x32,0x00,0x00,0xa8,0x15,0x1a,0x00,0xe0, 
		0xa9,0x15,0x0e,0x00,0x10,0xaa,0x15,0x06,0x00,0x10,0xaa,0x15,0x00,0x00,0x30,0xa9,0x15,0x00,0x00,0x00,0xa8,0x15,0x00,0x00,0x40,0xa8,0x15,0x00,0x00,0xf8,0xab,0x15,0x00,0x00,0xd0,0xa8,0x15, 
		0x00,0x00,0x50,0xa9,0x15,0x00,0x00,0x40,0xaa,0x15,0x00,0x00,0x00,0xa8,0x17,0x00,0x00,0xf0,0x2b,0x10,0x00,0x00,0x10,0xea,0x0f,0x00,0x00,0x10,0xf2,0x07,0x00,0x00,0xf0,0xe3,0x03,0x00,0x00
	},
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0xc0,0x01,0x00,0x00,
		0x00,0xa0,0x01,0x00,0x00,0x00,0x90,0x01,0x00,0x00,0x00,0xa8,0x01,0x00,0x00,0x00,0x94,0x01,0x00,0x00,0x00,0xa2,0x01,0x00,0x00,0x00,0x91,0x01,0x00,0x00,0x80,0xa0,0x01,0x00,0x00,0x40,0x90, 
		0x01,0x00,0x00,0x20,0xa0,0x01,0x00,0xc0,0x1f,0x90,0x01,0x00,0x20,0x10,0xa1,0x01,0x10,0xb2,0x97,0xd1,0x00,0xf0,0xab,0xd5,0x61,0x00,0x10,0xaa,0xd5,0x30,0x00,0x00,0xa8,0x55,0x18,0x00,0xe0, 
		0xa9,0x15,0x0c,0x00,0x10,0xaa,0x15,0x06,0x00,0x10,0xaa,0x15,0x03,0x00,0x30,0xa9,0x95,0x01,0x00,0x00,0xa8,0xd5,0x00,0x00,0x40,0xa8,0x75,0x00,0x00,0xf8,0xab,0x35,0x00,0x00,0xd0,0xa8,0x15, 
		0x00,0x00,0x50,0xa9,0x15,0x00,0x00,0x40,0xaa,0x15,0x00,0x00,0x00,0xa8,0x17,0x00,0x00,0xf0,0x2b,0x10,0x00,0x00,0x10,0xea,0x0f,0x00,0x00,0x10,0xf2,0x07,0x00,0x00,0xf0,0xe3,0x03,0x00,0x00
	},
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x80,0x0c,0x00,0x00,0x00,0x40,0x0d,
		0x00,0x00,0x00,0xa0,0x0c,0x00,0x00,0xc0,0x1f,0x0d,0x00,0x00,0x20,0x88,0x0c,0x00,0x10,0xb2,0x07,0x0d,0x00,0xf0,0xab,0x83,0x0c,0x00,0x10,0xaa,0x01,0x0d,0x00,0x00,0xa8,0x81,0x0c,0x00,0xe0,
		0xa9,0x09,0x0d,0x00,0x10,0xaa,0x8d,0x06,0x00,0x10,0xaa,0x0f,0x03,0x00,0x30,0xa9,0x87,0x01,0x00,0x00,0xa8,0xc3,0x00,0x00,0x40,0xa8,0x61,0x00,0x00,0xf8,0xab,0x31,0x00,0x00,0xd0,0xa8,0x19, 
		0x00,0x00,0x50,0xa9,0x1d,0x00,0x00,0x40,0xaa,0x17,0x00,0x00,0x00,0xa8,0x17,0x00,0x00,0xf0,0x2b,0x10,0x00,0x00,0x10,0xea,0x0f,0x00,0x00,0x10,0xf2,0x07,0x00,0x00,0xf0,0xe3,0x03,0x00,0x00
	},
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
		0x00,0x00,0x00,0x60,0x00,0x00,0x00,0xc0,0x7f,0x00,0x00,0x00,0x20,0x68,0x00,0x00,0x10,0xb2,0x67,0x00,0x00,0xf0,0xab,0x6b,0x00,0x00,0x10,0xaa,0x65,0x00,0x00,0x00,0xa8,0x69,0x00,0x00,0xe0, 
		0xa9,0x65,0x00,0x00,0x10,0xaa,0x69,0x00,0x00,0x10,0xaa,0x65,0x00,0x00,0x30,0xa9,0x69,0x00,0x00,0x00,0xa8,0x65,0x00,0x00,0x40,0xa8,0x69,0x00,0x00,0xf8,0xab,0x35,0x00,0x00,0xd0,0xa8,0x19, 
		0x00,0x00,0x50,0xa9,0x1d,0x00,0x00,0x40,0xaa,0x17,0x00,0x00,0x00,0xa8,0x17,0x00,0x00,0xf0,0x2b,0x10,0x00,0x00,0x10,0xea,0x0f,0x00,0x00,0x10,0xf2,0x07,0x00,0x00,0xf0,0xe3,0x03,0x00,0x00
	}
};

static const unsigned char PleaseCard[72 * 3] = 
{
	0x00,0x30,0x00,0x00,0x70,0x00,0x00,0x38,0x00,0x00,0xf8,0x07,0x00,0xfc,0x1f,0x70,0x3c,0x0c,0xf0,0x10,0x06,0xe0,0x80,0x01, 
	0x00,0x18,0x00,0x00,0x08,0x00,0x20,0x0b,0x7c,0x60,0xed,0x3f,0x60,0xf5,0x06,0xff,0xd7,0x22,0xff,0x57,0x22,0xbb,0x17,0x60, 
	0xd0,0xfe,0xff,0xd8,0xf2,0xff,0x00,0x16,0xe0,0x00,0x06,0x00,0x00,0x06,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x04,0x00,0x00,0x0e,0x00,0x0c,0x07,0x00,0x0e,0x13,0x00,0x8e,0x11,0x00,0xc6,0x30,0xfe,0xff,0x3f,0xfe,0xff,0x7f,
	0x00,0x23,0x00,0x00,0x04,0x00,0x00,0xcc,0x07,0x40,0x64,0x3f,0x40,0x26,0x33,0x60,0x06,0x30,0xf0,0xff,0x1f,0xb0,0xff,0x1f,
	0x18,0x03,0x18,0x18,0x23,0x11,0x1c,0xa3,0x39,0x10,0xe3,0x3f,0x80,0xf3,0x0f,0x00,0xe2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x30,0x00,0x00,0x30,0x00,0x00,0x10,0x00,
	0x00,0x18,0x00,0x00,0x18,0x00,0xfe,0xff,0xff,0xfe,0xff,0x07,0x76,0x1c,0x00,0x70,0x0c,0x00,0x30,0x8c,0x03,0x30,0x8c,0x07,
	0x30,0x0c,0x07,0x00,0x0c,0x04,0x00,0x0c,0x00,0x00,0x0c,0x00,0x00,0x0e,0x00,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};*/

static const unsigned char AT45_ERROR0[] = "DATAFLASH0-ERR";
static const unsigned char AT45_ERROR1[] = "DATAFLASH1-ERR";
static const unsigned char RECORD_ERR[] = "RECORD-ERR";
static const unsigned char CHARAC_LIB_ERR[] = "CHARAC-LIB-ERR";

static const unsigned char Welcome[] = "欢 迎 使 用";
static const unsigned char Gilbarco[] = "长 吉 系 统";

const ZimoAddr_t cZimo4x5Addr = {4, 5, 0, 4, 0x8000};
const ZimoAddr_t cZimo5x7Addr = {5, 7, 0, 5, 0x80b0};
const ZimoAddr_t cZimo7x12Addr = {7, 12, 0,14,0x4fc8};

void InitLcd(void)
{
    memset(DisBuf, 0, 1024);
    DisBufToLcd();
    //DrawHorizontalLine(0,127,0);
    DrawRectangle(0,0,127,63);
    DrawHorizontalLine(0, 128, 8);
    DrawHorizontalLine(0, 128, 50);
    DrawVerticalline(40, 0, 8);
    DrawVerticalline(92, 0, 8);
    DisBufToLcd();
}

/*static int ReadLcdLeftState(void)
{
   unsigned char val;
	int i;
	
	i = 1000;
	do
	{
		val = READ_DATA8(LcdLeftStateAdr);
	}
	while((val & 0x80) && (--i));
    if(!i)
    {
        WriteDriveLog(12,0);
    }
	return(i);
}

static int ReadLcdRightState(void)
{
    unsigned char val;
	int i;
	
	i = 1000;
	do
	{
		val = READ_DATA8(LcdRightStateAdr);
	}
	while((val & 0x80) && (--i));
    if(!i)
    {
        WriteDriveLog(12,0);
    }
	return(i);
}*/

static int ReadLcdLeftState(void)
{
   	unsigned char val;
	
	do
	{
		val = READ_DATA8(LcdLeftStateAdr);
	}
	while(val & 0x80);
	return 0;
}

static int ReadLcdRightState(void)
{
    unsigned char val;
	
	do
	{
		val = READ_DATA8(LcdRightStateAdr);
	}
	while(val & 0x80);
	return 0;
}	

void DrawHorizontalLine(unsigned char x1,
						unsigned char x2,
						unsigned char y)
{
    unsigned char i,res;
    unsigned int sta;
	unsigned char x;
	
	if(y > 63)
	{
		return;
	}
    if ( x1 > x2 )
    {
        i = x1;
        x1 = x2;
        x2 = i;
    }
    sta = y >> 3;
    sta *= 128;
    res = y & 0x07;
    res = residue[res];
	x = x2;
	if((sta + x) >= 1024)
	{
		x = 128;
	}
    for ( i = x1; i < x; i++ )
    {
        DisBuf[sta + i] |= res;
    }
}

void DrawVerticalline(unsigned char x,
					  unsigned char y1,
					  unsigned char y2)
{
    unsigned char i,res1,res2;
    unsigned char tmp,page1,page2;
    unsigned int sta;
	unsigned char y;
	
	if(x > 127)
	{
		return;
	}
    if ( y1 > y2 )
    {
        i = y1;
        y1 = y2;
        y2 = i;
    }
	y = y2;
	if(y > 63)
	{
		y = 63;
	}
    page1 = y1 >> 3;
    page2 = y >> 3;
    res1 = y1 & 0x07;
    res2 = y & 0x07;
    sta = page1;
    sta *= 128;
    sta += x;
    if ( page1 == page2 )
    {
        res1 = mask_down[res1];
        res2 = mask_up[res2];
        tmp = 0xff& res1 & res2;
        DisBuf[sta] |= tmp;
    }
    else
    {
        res1 = mask_down[res1];
        tmp = 0xff & res1;
        DisBuf[sta] |= tmp;
        sta += 128;
        for ( i = 0; i < (page2 - page1 - 1); i++ )
        {
            DisBuf[sta] |= 0xff;
            sta += 128;
        }
        res2 = mask_up[res2];
        tmp = 0xff;
        tmp &= res2;
        DisBuf[sta] |= tmp;
    }
}

void DrawRectangle(unsigned char x1,
				   unsigned char y1,
				   unsigned char x2,
				   unsigned char y2)
{
    DrawHorizontalLine(x1,x2 + 1,y1);
    DrawHorizontalLine(x1,x2 + 1,y2);
    DrawVerticalline(x1,y1,y2 + 1);
    DrawVerticalline(x2,y1,y2 + 1);
}

void LcdErr(unsigned short error,unsigned int chipId)
{
    LcdError lcd;
	unsigned char const *p;
	unsigned char buf[32];
    memset(&lcd,0,sizeof(LcdError));
	
	p = buf;
	lcd.sta = &p;
	lcd.len = 0;
    switch ( error )
    {
    case AT45_NOTFIND :
        if ( chipId == 0 )
        {
            lcd.len = ComputeZimo(AT45_ERROR0,lcd.sta);
        }
        else
        {
            if ( chipId == 1 )
            {
                lcd.len = ComputeZimo(AT45_ERROR1,lcd.sta);
            }
        }
        break;
    case RECORD_ERROR :
        lcd.len = ComputeZimo(RECORD_ERR,lcd.sta);
        break;
	case ZIMO_ERROR:
		lcd.len = ComputeZimo(CHARAC_LIB_ERR,lcd.sta);
		break;
    default:
        break;
    }
    if ( lcd.len != 0 )
    {
        DisplayError(8,16,lcd.sta,lcd.len);
    }
}

unsigned char ClearLcd(unsigned char x1,
					   unsigned char y1,
					   unsigned char x2,
					   unsigned char y2)
{
    unsigned char page1,page2;
    unsigned char res1,res2;
    unsigned int adr;
    unsigned char i,page;
    if ( (y1 > 63) || (y2 > 63) )
    {
        return 1;
    }
    if ( (x1 > 127) || (x2 > 127) )
    {
        return 1;
    }
    if ( x1 > x2 )
    {
        page = x1;
        x1 = x2;
        x2 = page;
    }
    if ( y1 > y2 )
    {
        page = y1;
        y1 = y2;
        y2 = page;
    }
    //
    //x2++;
    //y2++;
    //
    page1 = y1 >> 3;
    page2 = (y2 + 1) >> 3;
    res1 = y1 & 0x07;
    res2 = (y2 + 1) & 0x07;
    adr = page1;
    adr <<= 7;
    if ( page1 == page2 )
    {
        for ( i = x1; i < x2 + 1; i++ )
        {
            DisBuf[adr + i] &= (mask_up[res1] | mask_down[res2]);
            //DisBuf[adr + i] &= mask_down[res2];
        }
    }
    else
    {
        for ( i = x1; i < x2 + 1; i++ )
        {
            DisBuf[adr + i] &= mask_up[res1];
        }
		for ( page = page1 + 1; page < page2; page++ )
        {
            adr = page;
            adr <<= 7;
            for ( i = x1; i < x2 + 1; i++ )
            {
                DisBuf[adr + i] = 0;
            }
        }
		adr = page;
		adr <<= 7;
        if ( res2 != 0 )
        {
            for ( i = x1; i < x2 + 1; i++ )
            {
                DisBuf[adr + i] &= mask_down[res2];
            }
        }
    }
    //DisBufToLcd();
    return 0;
}

unsigned char ComputeZimo(const unsigned char *s,const unsigned char **p)
{
    unsigned char len = 0;
    if ( strlen((char *)s) > 16 )
    {
        return 0;
    }
    while ( *s )
    {
        if ( *s > '9' )
        {
            *p = NumChar[*s - 0x41 + 10];
        }
        else
        {
            if ( *s == 0x2d )
            {
                *p = NumChar[36];
            }
            else
            {
                if ( *s >= '0' )
                {
                    *p = NumChar[*s - 0x30];
                }
                else
                {
                    return 0;
                }
            }
        }
        *s++;
        *p++;
        len++;
    }
    return len;
}

void DisplayError(unsigned char x,unsigned y,
				  const unsigned char **p,unsigned char len)
{
    unsigned char i,page;
    const unsigned char *s;
    unsigned int adr;
    
    s = *p;
    page = y >> 3;
    adr = 128 * page + x;
    while ( len-- )
    {
        s = *p;
        for ( i =0; i < 8; i++ )
        {
            DisBuf[adr + i] = *(s + i);
            DisBuf[adr + 128 + i] = *(s + 8 + i);
        }
        adr += 8;
        p++;
    }
    DisBufToLcd();
}

unsigned char DisplayLogo(void)
{
    unsigned char *p;
    unsigned char i;
    unsigned int adr;
	unsigned char buf[44 * 5];
    
    p = buf;
    adr = 0x3c2e4;
    if ( READ(ZIMO_ID,p,adr,44 * 5) < 0 )
    {
        return 1;
    }
    adr = 128 + 6;
    for ( i = 0; i < 44; i++ )
    {
        DisBuf[adr + i] &= 0x03;
        DisBuf[adr + i] |= (p[ 5 * i] & 0xfc);
        DisBuf[adr + i + 128] = p[5 * i + 1];
        DisBuf[adr + i + 128 * 2] = p[5 * i + 2];
        DisBuf[adr + i + 128 * 3] = p[5 * i + 3];
        DisBuf[adr + i + 128 * 4] = p[5 * i + 4];
    }
    if ( DisplayString12(54, 16, (unsigned char *)Welcome) )
    {
        return 1;
    }
    if ( DisplayString12(54,30,(unsigned char *)Gilbarco) )
    {
        return 0;
    }
    DisBufToLcd();
    return 0;
}

unsigned char DisplayCartoon(unsigned int no)
{
    unsigned char i;
    unsigned int adr;
    unsigned char buf[72 * 3];
    
    adr = 0x3c000 + no * 37 * 5;
    if ( READ(ZIMO_ID,buf,adr,37 * 5) < 0 )
    {
        return 1;
    }
	//memcpy(buf,Cartoon[no],37 * 5);
    if ( LcdStatus == 0 )
    {
        ClearLcd(1,9,126,49);
        LcdStatus = 1;
    }
    adr = 128 + 6;
    for ( i = 0; i < 37; i++ )
    {
        DisBuf[adr + i] &= 0x01;
        DisBuf[adr + i] |= (buf[5 * i] << 1);
        DisBuf[adr + i + 128] = (buf[5 * i] >> 7) | (buf[5 * i + 1] << 1);
        DisBuf[adr + i + 128 * 2] = (buf[5 * i + 1] >> 7) | (buf[5 * i + 2] << 1);
        DisBuf[adr + i + 128 * 3] = (buf[5 * i + 2] >> 7) | (buf[5 * i + 3] << 1);
        DisBuf[adr + i + 128 * 4] = (buf[5 * i + 3] >> 7) | (buf[5 * i + 4] << 1);
    }
    adr = 0x56c8 + 72 * 12;
    if ( READ(ZIMO_ID,buf,adr, 72 * 3) < 0 )
    {
        return 1;
    }
	//memcpy(buf,PleaseCard,72 * 3);
    adr = 256 + 50;
    for ( i = 0; i < 24; i ++ )
    {
        DisBuf[adr + i] = buf[3 * i];
        DisBuf[adr + i + 128] = buf[3 * i + 1];
        DisBuf[adr + i + 256] = buf[3 * i + 2];
    }
    adr += 24;
    for ( i = 0; i < 24; i ++ )
    {
        DisBuf[adr + i] = buf[3 * i + 72];
        DisBuf[adr + i + 128] = buf[3 * i + 1 + 72];
        DisBuf[adr + i + 256] = buf[3 * i + 2 + 72];
    }
    adr += 24;
    for ( i = 0; i < 24; i ++ )
    {
        DisBuf[adr + i] = buf[3 * i + 72 * 2];
        DisBuf[adr + i + 128] = buf[3 * i + 1 + 72 * 2];
        DisBuf[adr + i + 256] = buf[3 * i + 2 + 72 * 2];
    }
	DrawHorizontalLine(0, 128, 50);
    DisBufToLcd();
    return 0;
}

void DisplayChar4x5(unsigned char x,unsigned char y,unsigned char val)
{
    //unsigned char i,j,tmp;
	unsigned char i,tmp;
    unsigned char page,res;
    unsigned int adr;
    unsigned char buf[4];
	
	/*if(val == ':')
	{
		j = 10;
	}
	else if(val == '-')
	{
		j = 11;
	}
	else if((val >= '0') && (val <= '9'))
	{
		j =  val - '0';
	}
	else
	{
		return;
	}
    memcpy(buf,NumChar4x5[j],4);*/
    if(((x + 4) > 127) || ((y + 5) > 63))
	{
		return;
	}
    adr = val - 0x20;
    adr *= cZimo4x5Addr.zimo_size;
    adr += cZimo4x5Addr.addr_ml;
    if ( READ(ZIMO_ID,buf,adr,4) < 0 )
    {
        return ;
    }
    page = y >> 3;
    res = y & 0x07;
    adr = page;
    adr <<= 7;
    adr += x;
    if ( res < 4 )
    {
        for ( i = 0; i < 4; i++ )
        {
            buf[i] <<= res;
            DisBuf[adr] &= mask4x5_down[res];
            DisBuf[adr++] |= buf[i];
        }
    }
    else
    {
        for ( i = 0; i < 4; i++ )
        {
            tmp = buf[i];
            tmp <<= res;
            DisBuf[adr + i] &= mask4x5_down[res];
            DisBuf[adr + i] |= tmp;
        }
        adr += 128;
        for ( i = 0; i < 4; i++ )
        {
            tmp = buf[i];
            tmp >>= (8 - res);
            DisBuf[adr + i] &= mask4x5_up[res];
            DisBuf[adr + i] |= tmp;
        }
    }
}

void DisplayString4x5(unsigned char x,unsigned char y,unsigned char *s)
{
	if(s == (unsigned char*)0)
	{
		return;
	}
    while ( *s )
    {
        DisplayChar4x5(x,y,*s);
        if ( *s == ':' )
        {
            x += 3;
        }
        else
        {
            if ( *s == ' ' )
            {
                x += 1;
            }
            else
            {
                x += 5;
            }
        }
        s++;
        if ( *s == ':' )
        {
            x -= 1;
        }
    }
    DrawVerticalline(92,0,8);
}

unsigned char DisplayChar5x7(unsigned char x,unsigned char y,unsigned char val)
{
    unsigned char i,tmp;
    unsigned char page,res;
    unsigned int adr;
    unsigned char buf[5];
    
	if(((x + 5) > 127) || ((y + 7) > 63))
	{
		return 2;
	}
    adr = val;// - 0x20;
    adr *= cZimo5x7Addr.zimo_size;
    adr += cZimo5x7Addr.addr_ml;
    if ( READ(ZIMO_ID,buf,adr,5) < 0 )
    {
        return 1;
    }
    page = y >> 3;
    res = y & 0x07;
    adr = page;
    adr <<= 7;
    adr += x;
    if ( res < 2 )
    {
        for ( i = 0; i < 5; i++ )
        {
            buf[i] <<= res;
            DisBuf[adr + i] &= mask5x7_down[res];
            DisBuf[adr + i] |= buf[i];
        }
    }
    else
    {
        for ( i = 0; i < 5; i++ )
        {
            tmp = buf[i];
            tmp <<= res;
            DisBuf[adr + i] &= mask5x7_down[res];
            DisBuf[adr + i] |= tmp;
        }
        adr += 128;
        for ( i = 0; i < 5; i++ )
        {
            tmp = buf[i];
            tmp >>= (8 - res);
            DisBuf[adr + i] &= mask5x7_up[res];
            DisBuf[adr + i] |= tmp;
        }
    }
    return 0;
}

unsigned char DisplayString5x7(unsigned char x,unsigned char y,unsigned char *s)
{
    unsigned char tmp,tmpx;
    
	if(s == (unsigned char*)0)
	{
		return -1;
	}
    tmpx = x;
    tmp = *s;
    while ( tmp )
    {
        if ( DisplayChar5x7(tmpx,y,tmp) )
        {
            return 1;
        }
        tmpx += 6;
        s++;
        tmp = *s;
    }
    return 0;
}

unsigned char DisplayChar7x12(unsigned char x,unsigned char y,unsigned char val)
{
    unsigned char i,tmp1,tmp2;
    unsigned char page,res;
    unsigned int adr;
    unsigned char buf[14];
    
	if(((x + 7) > 127) || ((y + 12) > 63))
	{
		return 2;
	}
    adr = val;
    adr *= cZimo7x12Addr.zimo_size;
    adr += cZimo7x12Addr.addr_ml;
    if ( READ(ZIMO_ID,buf,adr,14) < 0 )
    {
        return 1;
    }
    page = y >> 3;
    res = y & 0x07;
    adr = page;
    adr <<= 7;
    adr += x;
    if ( res < 5 )
    {
        for ( i = 0; i < 7; i++ )
        {
            tmp1 = buf[i << 1];
            tmp1 <<= res;
            DisBuf[adr + i] &= mask12_down[res];
            DisBuf[adr + i] |= tmp1;
        }
        adr += 128;
        for ( i = 0; i < 7; i++ )
        {
            tmp1 = buf[i << 1];
            tmp1 >>= (8 - res);
            tmp2 = buf[(i << 1) + 1];
            tmp2 <<= res;
            DisBuf[adr + i] &= mask12_up[res];
            DisBuf[adr + i] |= tmp1 | tmp2;
        }
    }
    else
    {
        for ( i = 0; i < 7; i++ )
        {
            tmp1 = buf[i << 1];
            tmp1 <<= res;
            DisBuf[adr + i] &= mask12_down[res];
            DisBuf[adr + i] |= tmp1;
        }
        adr += 128;
        for ( i = 0; i < 7; i++ )
        {
            tmp1 = buf[i << 1];
            tmp1 >>= (8 - res);
            tmp2 = buf[(i << 1) + 1];
            tmp2 <<= res;
            DisBuf[adr + i] = tmp1 | tmp2;
        }
        adr += 128;
        for ( i = 0; i < 7; i++ )
        {
            tmp2 = buf[(i << 1) + 1];
            tmp2 >>= (8 - res);
            DisBuf[adr + i] &= mask12_up[res];
            DisBuf[adr + i] |= tmp2;
        }
    }
    return 0;
}

unsigned char DisplayText12x12(unsigned char x,
							   unsigned char y,
							   unsigned char valx,
							   unsigned char valy)
{
    unsigned char i,page,res;
    unsigned char tmp1,tmp2;
    unsigned int adr;
    unsigned char buf[24];
    
	if(((x + 12) > 127) || ((y + 12) > 63))
	{
		return 2;
	}
    adr = valx;
    adr -= 0xa1;
    adr *= 94;
    adr += valy;
    adr -= 0xa1;
    adr *= 24;
    if ( READ(ZIMO_ID,buf,adr,24) < 0 )
    {
        return 1;
    }
    page = y >> 3;
    res = y & 0x07;
    adr = page;
    adr <<= 7;
    adr += x;
    if ( res < 5 )
    {
        for ( i = 0; i < 12; i++ )
        {
            tmp1 = buf[i << 1];
            tmp1 <<= res;
            DisBuf[adr + i] &= mask12_down[res];
            DisBuf[adr + i] |= tmp1;
        }
        adr += 128;
        for ( i = 0; i < 12; i++ )
        {
            tmp1 = buf[i << 1];
            tmp1 >>= (8 - res);
            tmp2 = buf[(i << 1) + 1];
            tmp2 <<= res;
            DisBuf[adr + i] &= mask12_up[res];
            DisBuf[adr + i] |= tmp1 | tmp2;
        }
    }
    else
    {
        for ( i = 0; i < 12; i++ )
        {
            tmp1 = buf[i << 1];
            tmp1 <<= res;
            DisBuf[adr + i] &= mask12_down[res];
            DisBuf[adr + i] |= tmp1;
        }
        adr += 128;
        for ( i = 0; i < 12; i++ )
        {
            tmp1 = buf[i << 1];
            tmp1 >>= (8 - res);
            tmp2 = buf[(i << 1) + 1];
            tmp2 <<= res;
            DisBuf[adr + i] = tmp1 | tmp2;
        }
        adr += 128;
        for ( i = 0; i < 12; i++ )
        {
            tmp2 = buf[(i << 1) + 1];
            tmp2 >>= (8 - res);
            DisBuf[adr + i] &= mask12_up[res];
            DisBuf[adr + i] |= tmp2;
        }
    }
    return 0;
}

unsigned char DisplayString12(unsigned char x,unsigned char y,unsigned char *s)
{
    unsigned char xtmp,tmp1,tmp2;
    unsigned char *p;
    
	if(s == (unsigned char*)0)
	{
		return -1;
	}
    p = s;
    tmp1 = *p;
    xtmp = x;
    while ( tmp1 )
    {
        if ( tmp1 > 0xa0 )
        {
            p++;
            tmp2 = *p;
            if ( DisplayText12x12(xtmp,y,tmp1,tmp2) )
            {
                return 1;
            }
            xtmp += 12;
            p++;
        }
        else
        {
            if ( DisplayChar7x12(xtmp,y,tmp1) )
            {
                return 1;
            }
//            if ( tmp1 != ' ' )
//            {
                xtmp += 8;
//            }
//            else
//            {
//                xtmp += 4;
//            }
            p++;
        }
        tmp1 = *p;
    }
    return 0;
}

unsigned char PresetDisplay(unsigned char *s,unsigned char point,unsigned char type)
{
    unsigned char i,len;
    unsigned char x,y;
    unsigned int adr;
    unsigned char *p,*pl;
    unsigned char buf[72];
	unsigned char buffer[32];
    
	if(s == (unsigned char*)0)
	{
		return -1;
	}
    len = strlen((char *)s);
    ClearLcd(4,14,125,38);
	DrawRectangle(4,14,124,36);
	DrawHorizontalLine(4,125,37);
	x = 104;
    y = 16;
    x = x - 12 * len;
    if ( x <= 4 )
    {
        return 2;
    }
    p = (unsigned char *)0;
    if ( point == NO )
    {
        while ( *s )
        {
            adr = *s - '0';
            adr *= 72;
            adr += 0x56c8;
            if ( READ(ZIMO_ID,buf,adr,72) < 0 )
            {
                return 1;
            }
            adr = ((y >> 3) << 7) + x;
            for ( i = 0; i < 12; i++ )
            {
                DisBuf[adr + i] = buf[3 * i];
                DisBuf[adr + i + 128] = buf[3 * i + 1];
                DisBuf[adr + i + 256] &= 0xf0;
                DisBuf[adr + i + 256] |= (buf[3 * i + 2] & 0x0f);
            }
            x += 12;
            s++;
        }
        if ( type == MON )
        {
            adr = 0x56c8 + 72 * 10;
        }
        else
        {
            adr = 0x56c8 + 72 * 11;
        }
        if ( READ(ZIMO_ID,buf,adr,72) < 0 )
        {
            return 1;
        }
        adr = ((y >> 3) << 7) + x;
        for ( i = 0; i < 20; i++ )
        {
            DisBuf[adr + i] = buf[3 * i];
            DisBuf[adr + i + 128] = buf[3 * i + 1];
            DisBuf[adr + i + 256] &= 0xf0;
            DisBuf[adr + i + 256] |= (buf[3 * i + 2] & 0x0f);
        }
    }
    else
    {
		p = buffer;
        if ( len < 3 )
        {
            if ( p == NULL )
            {
                return 2;
            }
            p[0] = '0';
            p[1] = '.';
            if ( len == 2 )
            {
                p[2] = *s++;
                p[3] = *s;
                p[4] = '\0';
            }
            else
            {
                p[2] = '0';
                p[3] = *s;
                p[4] = '\0';
            }
        }
        else
        {
            memcpy(p,s,len - 2);
            p[len - 2] = '.';
            memcpy(&p[len - 1],&s[len -2],2);
            p[len + 1] = '\0';
        }
        x -= 4;
        if ( x <= 4 )
        {
            return 2;
        }
        pl = p;
        while ( *pl )
        {
            adr = ((y >> 3) << 7) + x;
            if ( *pl == '.' )
            {
                for ( i = 0; i < 3; i++ )
                {
                    DisBuf[adr + i + 128] = 0xc0;
                    DisBuf[adr + i + 256] &= 0xf0;
                    DisBuf[adr + i + 256] |= 0x01;
                }
                x += 4;
            }
            else
            {
                adr = *pl - '0';
                adr *= 72;
                adr += 0x56c8;
                if ( READ(ZIMO_ID,buf,adr,72) < 0 )
                {
                    return 1;
                }
                adr = ((y >> 3) << 7) + x;
                for ( i = 0; i < 12; i++ )
                {
                    DisBuf[adr + i] = buf[3 * i];
                    DisBuf[adr + i + 128] = buf[3 * i + 1];
                    DisBuf[adr + i + 256] &= 0xf0;
                    DisBuf[adr + i + 256] |= (buf[3 * i + 2] & 0x0f);
                }
                x += 12;
            }
            pl++;
        }
        if ( type == MON )
        {
            adr = 0x56c8 + 72 * 10;
        }
        else
        {
            adr = 0x56c8 + 72 * 11;
        }
        if ( READ(ZIMO_ID,buf,adr,72) < 0 )
        {
            return 1;
        }
        adr = ((y >> 3) << 7) + x;
        for ( i = 0; i < 20; i++ )
        {
            DisBuf[adr + i] = buf[3 * i];
            DisBuf[adr + i + 128] = buf[3 * i + 1];
            DisBuf[adr + i + 256] &= 0xf0;
            DisBuf[adr + i + 256] |= (buf[3 * i + 2] & 0x0f);
        }
    }
    return 0;
}

void DisplayContra(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
{
    unsigned char i,page,tmp;
    unsigned char page1,page2,res1,res2;
    unsigned int adr;
	unsigned char x,y;
    if ( x1 > x2 )
    {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    if ( y1 > y2 )
    {
        tmp = y1;
        y1 = y2;
        y2 = y1;
    }
    x2++;
    y2++;
	x = x2;
	y = y2;
	if(x > 127)
	{
		x = 127;
	}
	if(y > 63)
	{
		y = 63;
	}
    page1 = y1 >> 3;
    res1 = y1 & 0x07;
    page2 = y >> 3;
    res2 = y & 0x07;
    adr = page1;
    adr <<= 7;
    if ( page1 == page2 )
    {
        tmp = 0xff;
        tmp &= mask_down[res1] & mask_up[res2 - 1];
        for ( i = x1; i < x; i++ )
        {
            DisBuf[adr + i] &= mask_down[res1] & mask_up[res2];
            DisBuf[adr + i] ^= tmp;
        }
    }
    else
    {
        tmp = 0xff;
        tmp &= mask_down[res1];
        for ( i = x1; i < x; i++ )
        {
            //DisBuf[adr + i] &= mask_down[res1];
            DisBuf[adr + i] ^= tmp;
        }
        for ( page = page1 + 1; page < page2; page++ )
        {
            adr = page;
            adr <<= 7;
            tmp = 0xff;
            for ( i = x1; i < x; i++ )
            {
                DisBuf[adr + i] ^= tmp;
            }
        }
        if ( res2 != 0 )
        {
            adr = page2;
            adr <<= 7;
            tmp = 0xff;
            tmp &= mask_up[res2 - 1];
            for ( i = x1; i < x; i++ )
            {
                //DisBuf[adr + i] &= mask_up[res2];
                DisBuf[adr + i] ^= tmp;
            }
        }
    }
    //DisBufToLcd();
}

void DisplayOnLine(void)
{
	unsigned char i;
	
	for(i = 0; i < 8; i++)
	{
		DisBuf[20 + i] &= mask_up[1];
		DisBuf[20 + i] |= computer_zimo[i];
	}
	if(PosOnline)
	{
		DrawHorizontalLine(10,22,3);
	}
	else
	{
		ClearLcd(12,3,19,5);
	}
	if(IdleFlg & 0x04)
	{
		for(i = 0; i < 8; i++)
		{
			DisBuf[4 + i] &= mask_up[1];
			DisBuf[4 + i] |= pumpOnline[i];
		}
	}
	else
	{
		for(i = 0; i < 8; i++)
		{
			DisBuf[4 + i] &= mask_up[1];
			DisBuf[4 + i] |= pumpOffline[i];
		}
	}
	DisBufToLcd();
}

/*void DisBufToLcd(void)
{
    unsigned char i,j,page;
    unsigned int sta,val;
    
    page = 0;
    sta = 0;
    val = ReadLcdLeftState();
	if(!val)
	{
		PowerDownFlg = 1;
		return;
	}
    WRITE_DATA8(LcdLeftComWriteAdr, StartLine);
    val = ReadLcdRightState();
	if(!val)
	{
		PowerDownFlg = 1;
		return;
	}
    WRITE_DATA8(LcdRightComWriteAdr, StartLine);
    for ( i = 0; i < 8; i++ )
    {
        val = ReadLcdLeftState();
		if(!val)
		{
			PowerDownFlg = 1;
			return;
		}
        WRITE_DATA8(LcdLeftComWriteAdr, SetPage | page);
        val = ReadLcdLeftState();
		if(!val)
		{
			PowerDownFlg = 1;
			return;
		}
        WRITE_DATA8(LcdLeftComWriteAdr, SetRow);
        for ( j = 0; j < 64; j++ )
        {
            val = ReadLcdLeftState();
			if(!val)
			{
				PowerDownFlg = 1;
				return;
			}
            WRITE_DATA8(LcdLeftDataWriteAdr, DisBuf[sta++]);
        }
        val = ReadLcdRightState();
		if(!val)
		{
			PowerDownFlg = 1;
			return;
		}
        WRITE_DATA8(LcdRightComWriteAdr, SetPage | page);
        if(!val)
		{
			PowerDownFlg = 1;
			return;
		}
        WRITE_DATA8(LcdRightComWriteAdr, SetRow);
        for ( j = 0; j < 64; j++ )
        {
            val = ReadLcdRightState();
			if(!val)
			{
				PowerDownFlg = 1;
				return;
			}
            WRITE_DATA8(LcdRightDataWriteAdr, DisBuf[sta++]);
        }
        page++;
    }
    WRITE_DATA8(LcdLeftComWriteAdr, DisplayOn);
    WRITE_DATA8(LcdRightComWriteAdr, DisplayOn);
}*/

void DisBufToLcd(void)
{
    unsigned char i,j,page;
    unsigned int sta;
    
    page = 0;
    sta = 0;
    ReadLcdLeftState();
    WRITE_DATA8(LcdLeftComWriteAdr, StartLine);
    ReadLcdRightState();
	WRITE_DATA8(LcdRightComWriteAdr, StartLine);
    for ( i = 0; i < 8; i++ )
    {
        ReadLcdLeftState();
		WRITE_DATA8(LcdLeftComWriteAdr, SetPage | page);
        ReadLcdLeftState();
		WRITE_DATA8(LcdLeftComWriteAdr, SetRow);
        for ( j = 0; j < 64; j++ )
        {
            ReadLcdLeftState();
			WRITE_DATA8(LcdLeftDataWriteAdr, DisBuf[sta++]);
        }
        ReadLcdRightState();
        WRITE_DATA8(LcdRightComWriteAdr, SetPage | page);
        ReadLcdRightState();
        WRITE_DATA8(LcdRightComWriteAdr, SetRow);
        for ( j = 0; j < 64; j++ )
        {
            ReadLcdRightState();
			WRITE_DATA8(LcdRightDataWriteAdr, DisBuf[sta++]);
        }
        page++;
    }
    WRITE_DATA8(LcdLeftComWriteAdr, DisplayOn);
    WRITE_DATA8(LcdRightComWriteAdr, DisplayOn);
}

void DisPlayHelp(const unsigned char *s)
{
	unsigned char len;
	unsigned char x;
	
	len = strlen((char *)s);
	if(len > 20)
	{
		return;
	}
	x = 128 - 6 * len;
	x >>= 1;
	DisplayString12(x,51,(unsigned char *)s);
	DisBufToLcd();
}

extern	const char DISPMSG[][20];

void DisplayVerNumber(void)
{
//20131021 qiyong modify	DisplayString12(16,24,(unsigned char *)DISPMSG[2]);
	DisplayString12(10,18,(unsigned char *)DISPMSG[3]);
	DisplayString12(10,34,(unsigned char *)DISPMSG[2]);
	DisBufToLcd();
}