
#ifndef _KEYMASK_H
#define _KEYMASK_H

//#include "Screen.h"

#define NOKEY				0xFF					// No Key Press, Same As Define In Driver
#define DISABLE				0xFE					// Pressed Key Is Disable

#define	ENTER				0x0A					// Enter Key Pressed
#define VOLUME				0x0B					// Volume Key Pressed
#define	AMOUNT				0x0C					// Amount Key Pressed
#define	SPRING				0x0D					// Spring Out Card Key Pressed
#define	CLEAR				0x0E					// Clear Key Pressed
#define	SELECT				0x0F					// Select Key Pressed
#define	SCROLL_UP			0x10					// Up Scroll Key Pressed
#define	SCROLL_DOWN			0x11					// Down Scroll Key Pressed
#define	SETUP				0x12					// Setup Key Pressed
#define	AMT_ROUND			'.'						// Amount Rounding Key Pressed, Same Key With '.'
#define KEY_RFU0			0x14					// Reserved For Future
#define KEY_RFU1			0x15					// Reserved For Future
#define KEY_RFU2			0x16					// Reserved For Future
#define KEY_RFU3			0x17					// Reserved For Future
#define KEY_RFU4			0x18					// Reserved For Future


#define KEY_COL_MAX			5						// Max Keypad Column
#define KEY_ROW_MAX			5						// Max Keypad Row


enum KEYMASK
{
	KEYMASK_START,

	KEYMASK_DEFAULT,
	KEYMASK_FUELLING,

	KEYMASK_MAX,
};

extern const unsigned char KeyMask_Default[KEY_ROW_MAX][KEY_COL_MAX];
extern const unsigned char KeyMask_Fuelling[KEY_ROW_MAX][KEY_COL_MAX];

extern enum KEYMASK KeyMask;
extern unsigned char KeyValue;

// Public Input Buffer And Length
extern unsigned char InputLen;
extern unsigned char InputBuf[16];

// Preset Decimal Point Flag
extern unsigned char PresetDotFlag;

extern enum SCREEN InputState;

// Display Price Update Screen Flag
// Special
extern unsigned char PriceUpdateFlag;



#endif
