#include "Screen.h"
#include "message.h"

#include "KeyMask.h"



const unsigned char KeyMask_Default[KEY_ROW_MAX][KEY_COL_MAX] = 
	{
		{ '1',			'2',		'3',			AMOUNT,		DISABLE },
		{ '4',			'5',		'6',			VOLUME,		DISABLE },
		{ '7',			'8',		'9',			SPRING,		DISABLE },
		{ '.', 			'0',		ENTER,			CLEAR,		DISABLE },
		{ SETUP,		SCROLL_UP,	SCROLL_DOWN,	SELECT,		DISABLE },
	};

//Douzhq 20140530 Add , Lock Nozzle , Free SELECT Key
const unsigned char KeyMask_Fuelling[KEY_ROW_MAX][KEY_COL_MAX] = 
	{
		{ DISABLE,		DISABLE,	DISABLE,	DISABLE,		DISABLE },
		{ DISABLE,		DISABLE,	DISABLE,	VOLUME,			DISABLE },
		{ DISABLE,		DISABLE,	DISABLE,	DISABLE,		DISABLE },
		{ '.', 			DISABLE,	DISABLE,	DISABLE,		DISABLE },
		{ DISABLE,		DISABLE,	DISABLE,	SELECT,		    DISABLE },
	};

//Before Add Lock Nozzle Feature
//const unsigned char KeyMask_Fuelling[KEY_ROW_MAX][KEY_COL_MAX] = 
//	{
//		{ DISABLE,		DISABLE,	DISABLE,	DISABLE,		DISABLE },
//		{ DISABLE,		DISABLE,	DISABLE,	VOLUME,			DISABLE },
//		{ DISABLE,		DISABLE,	DISABLE,	DISABLE,		DISABLE },
//		{ '.', 			DISABLE,	DISABLE,	DISABLE,		DISABLE },
//		{ DISABLE,		DISABLE,	DISABLE,	DISABLE,		DISABLE },
//	};

enum KEYMASK KeyMask;
unsigned char KeyValue;

// Public Input Buffer And Length
unsigned char InputLen;
unsigned char InputBuf[16];

// Preset Decimal Point Flag
unsigned char PresetDotFlag;

// Current Input State
enum SCREEN InputState;

// Display Price Update Screen Flag
// Special
unsigned char PriceUpdateFlag = 0;
