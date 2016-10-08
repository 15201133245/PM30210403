#include "tc.h"

#include "Screen.h"




const SCRNSTR ScreenList[SCREEN_MAX] =
{
	// SCREEN_START, Only For Define 
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

	// DEFAULT_SCR_CARD, Only For Define 
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

	// DEFAULT_SCR_BUSY, Only For Define 
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

	// DEFAULT_SCR_BALANCE, Only For Define 
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	DEFAULT_SCR_WAITING,				// Display Waiting Screen, Instead Of Insert Card Screen
//										// For The IC Card Not Spring Out, But Refresh The IC Card Information
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},


//	DEFAULT_SCR_ERROR,					// Default Screen Card Reader Error, Include: Card Reader Reset Error And PSAM Error
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_PRICE_UPDATE,					// Default Screen Card Reader Error, Include: Card Reader Reset Error And PSAM Error
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},


	// SCR_PAYMODE: Select Pay Mode Screen
	{
		6,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_SelectPayMode,				// Title Message ID

		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_Cash,							// List Item 1 Message ID

		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_OilTicket,					// List Item 2 Message ID

		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_Ticket,						// List Item 3 Message ID

		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_CreditCard,					// List Item 4 Message ID

		List_Left_Short_3_TopX,			// List Item 5 TopX
		List_Left_Short_3_TopY,			// List Item 5 Topy
		List_Left_Short_3_BottomX,		// List Item 5 BottomX
		List_Left_Short_3_BottomY,		// List Item 5 BottomY
		M_Other1,						// List Item 5 Message ID

		List_Right_Short_3_TopX,		// List Item 6 TopX
		List_Right_Short_3_TopY,		// List Item 6 Topy
		List_Right_Short_3_BottomX,		// List Item 6 BottomX
		List_Right_Short_3_BottomY,		// List Item 6 BottomY
		M_Other2,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

	// SCR_SELECT_APP: Select IC Card Application
	{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_SelectApp,					// Title Message ID

		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_SinopecApp,							// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_LoyaltyApp,					// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},
	
//Douzhq 20140523 Add , Save Oil
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	// SCR_SELECT_APP_SAVEOIL: Display Select Application , Add Saveoil App
	{
		3,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_SelectApp,					// Title Message ID

		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_SinopecApp,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_LoyaltyApp,					// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_SaveoilApp,					// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},
#endif 

	// SCR_INPUT_PIN: Display Input Pin
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		POPUP_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_CARD_PIN,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

	// SCR_INPUT_EMP_PWD,					// Display Input Employee Password
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		POPUP_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_EMP_PWD,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

	// SCR_INPUT_ENTER_KEY,				// Display Input Enter Key
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		POPUP_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_ENTER3,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_ENTER4,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

	// SCR_INPUT_MAINTAIN_PWD,				// Display Input Maintainance Password
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		POPUP_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_MAINTAIN_PWD,					// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_INPUT_FUNCTION_PWD,				// Display Input Function Password
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		POPUP_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_FUNCTION_PWD,					// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},
	
//Douzhq 20140528 Add , Lock Nozzle , Unlock Window
	//SCR_INPUT_UNLOCK_PWD
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		POPUP_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_UNLOCK_PWD,					// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID		
	},
	
//	SCR_INPUT_FORMAT_PWD,				// Display Input Format Config And Data Password
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		POPUP_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_FORMAT_PWD,					// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_INPUT_AUTHCODE,					// Display Input Auth Code For Cash Trans
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		POPUP_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_InputAuthCode,				// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_FUNCTION,							// Display Screen When SETUP Key Pressed
#ifdef MAKE_CARD_MODE2

	{
		7,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_Function,						// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_Version,						// List Item 1 Message ID

		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_Inquire,						// List Item 2 Message ID

		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_Setup,						// List Item 3 Message ID

		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_Maintain,						// List Item 4 Message ID

		List_Left_Short_3_TopX,			// List Item 5 TopX
		List_Left_Short_3_TopY,			// List Item 5 Topy
		List_Left_Short_3_BottomX,		// List Item 5 BottomX
		List_Left_Short_3_BottomY,		// List Item 5 BottomY
		M_Logon,						// List Item 5 Message ID

		List_Right_Short_3_TopX,		// List Item 6 TopX
		List_Right_Short_3_TopY,		// List Item 6 Topy
		List_Right_Short_3_BottomX,		// List Item 6 BottomX
		List_Right_Short_3_BottomY,		// List Item 6 BottomY
		M_Logoff,						// List Item 6 Message ID

		List_Left_Short_4_TopX,			// List Item 7 TopX
		List_Left_Short_4_TopY,			// List Item 7 Topy
		List_Left_Short_4_BottomX,		// List Item 7 BottomX
		List_Left_Short_4_BottomY,		// List Item 7 BottomY
		M_WorkMode,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

#else

	{
		6,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_Function,						// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_Version,						// List Item 1 Message ID

		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_Inquire,						// List Item 2 Message ID

		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_Setup,						// List Item 3 Message ID

		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_Maintain,						// List Item 4 Message ID

		List_Left_Short_3_TopX,			// List Item 5 TopX
		List_Left_Short_3_TopY,			// List Item 5 Topy
		List_Left_Short_3_BottomX,		// List Item 5 BottomX
		List_Left_Short_3_BottomY,		// List Item 5 BottomY
		M_Logon,						// List Item 5 Message ID

		List_Right_Short_3_TopX,		// List Item 6 TopX
		List_Right_Short_3_TopY,		// List Item 6 Topy
		List_Right_Short_3_BottomX,		// List Item 6 BottomX
		List_Right_Short_3_BottomY,		// List Item 6 BottomY
		M_Logoff,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

#endif

//	SCR_VERSION,						// Display Software Version
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_VersionTitle,					// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_VersionNumber_New,//M_CustomMsg0,					// List Item 1 Message ID	//20131021 qiyong modify

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_VersionNumber,				// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_CustomMsg1,					// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_CustomMsg2,					// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_INQUIRE,						// Display Inquire Screen
	{
		5,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_InquireFunc,						// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_Station,						// List Item 1 Message ID

		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_RecordSpace,						// List Item 2 Message ID

		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_Record,						// List Item 3 Message ID

		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_Total,						// List Item 4 Message ID

		List_Left_Short_3_TopX,			// List Item 5 TopX
		List_Left_Short_3_TopY,			// List Item 5 Topy
		List_Left_Short_3_BottomX,		// List Item 5 BottomX
		List_Left_Short_3_BottomY,		// List Item 5 BottomY
		M_PriceInq,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_STATION,						// Display Station Information
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_StationFunc,					// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_RECORDSPACE,					// Display Record Space Information
	{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_RecordSpaceFunc,				// Title Message ID
	
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Long_3_TopX,				// List Item 2 TopX
		List_Long_3_TopY,				// List Item 2 Topy
		List_Long_3_BottomX,			// List Item 2 BottomX
		List_Long_3_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_VOLUMETOTAL,					// Display Volume Total
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_VolumeTotal,					// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_VOLUMETOTALLOGON,				// Display Volume Total By This Log On Employee
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_VolumeTotalLogOn,				// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_AMOUNTTOTAL,					// Display Amount Total
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_AmountTotal,					// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_AMOUNTTOTALLOGON,				// Display Amount Total By This Log On Employee
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_AmountTotalLogOn,				// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},


#ifdef MAKE_CASH_CR
//	SCR_CASHVOLUMETOTALLOGON,			// Display Cash Volume Total By This Log On Employee
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_CashVolumeTotalLogOn,			// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_CASHAMOUNTTOTALLOGON,			// Display Cash Amount Total By This Log On Employee
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_CashAmountTotalLogOn,			// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_SELECTPRINT,					// Display Cash Trans Print Receipt Or Not
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

#endif
//Douzhq 20140922 Add , Config Printer Set
//	SCR_CHOOSEPRINT
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_PRICEINQUIRY,					// Display Price Inquiry
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_PriceInqiry,					// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_INPUT_TTC,						// Display Input TTC Screen
	{
		3,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_InputTTC,						// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID

		List_Long_3_TopX,				// List Item 2 TopX
		List_Long_3_TopY,				// List Item 2 Topy
		List_Long_3_BottomX,			// List Item 2 BottomX
		List_Long_3_BottomY,			// List Item 2 BottomY
		M_ENTER2,						// List Item 2 Message ID

		List_Long_4_TopX,				// List Item 3 TopX
		List_Long_4_TopY,				// List Item 3 Topy
		List_Long_4_BottomX,			// List Item 3 BottomX
		List_Long_4_BottomY,			// List Item 3 BottomY
		M_LastTrans,					// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_SETUP,							// Display Setup Screen
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_SetUpFunction,				// Title Message ID

		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_BackLightTime,				// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_SetUpPrint,					// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_SetUpVolume,					// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_SetUpPSAMCheck,				// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_SETUPBACKLIGHT,					// Display Setup Backlight
{
		3,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,					// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_SetBackLightTime,				// Title Message ID

		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_4_TopX,				// List Item 3 TopX
		List_Long_4_TopY,				// List Item 3 Topy
		List_Long_4_BottomX,			// List Item 3 BottomX
		List_Long_4_BottomY,			// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_SETUPPRINT,						// Display Setup Print
{
		//Douzhq 20140919 Add , Config Printer Set
		6,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type:
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_PrintSetUp,					// Title Message ID

		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_CustomerCard,					// List Item 1 Message ID

		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_EMPCard,						// List Item 2 Message ID

		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_TestCard,						// List Item 3 Message ID

		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_MatnCard,						// List Item 4 Message ID

		List_Left_Short_3_TopX,			// List Item 5 TopX
		List_Left_Short_3_TopY,			// List Item 5 Topy
		List_Left_Short_3_BottomX,		// List Item 5 BottomX
		List_Left_Short_3_BottomY,		// List Item 5 BottomY
		M_PubTrans,						// List Item 5 Message ID

//Douzhq 20140919 Add , Config Printer Set
		List_Right_Short_3_TopX,		// List Item 6 TopX
		List_Right_Short_3_TopY,		// List Item 6 Topy
		List_Right_Short_3_BottomX,		// List Item 6 BottomX
		List_Right_Short_3_BottomY,		// List Item 6 BottomY
		M_ExpandSetUp,					// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_SETUPVOLUME,					// Display Setup Volume
	{
		3,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_VolumeSetup,					// Title Message ID

		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_VolumeUp,						// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_VolumeDown,					// List Item 2 Message ID

		List_Long_4_TopX,				// List Item 3 TopX
		List_Long_4_TopY,				// List Item 3 Topy
		List_Long_4_BottomX,			// List Item 3 BottomX
		List_Long_4_BottomY,			// List Item 3 BottomY
		M_CANCEL,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_SETUPPSAM,						// Display Setup PSAM Validate And Expire Date Check
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_PSAMSetUp,					// Title Message ID

		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,					// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_CUSTOMER_CARD_PRINT,			// Display Customer Card Print Setup
	{
		6,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type:
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_CustomerCardPrint,			// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		List_Left_Short_3_TopX,			// List Item 5 TopX
		List_Left_Short_3_TopY,			// List Item 5 Topy
		List_Left_Short_3_BottomX,		// List Item 5 BottomX
		List_Left_Short_3_BottomY,		// List Item 5 BottomY
		M_CustomMsg4,					// List Item 5 Message ID

		List_Right_Short_3_TopX,		// List Item 6 TopX
		List_Right_Short_3_TopY,		// List Item 6 Topy
		List_Right_Short_3_BottomX,		// List Item 6 BottomX
		List_Right_Short_3_BottomY,		// List Item 6 BottomY
		M_CustomMsg5,					// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_EMP_CARD_PRINT,					// Display Employee Card Print Setup
	{
		6,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type:
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_EMPCardPrint,					// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		List_Left_Short_3_TopX,			// List Item 5 TopX
		List_Left_Short_3_TopY,			// List Item 5 Topy
		List_Left_Short_3_BottomX,		// List Item 5 BottomX
		List_Left_Short_3_BottomY,		// List Item 5 BottomY
		M_CustomMsg4,					// List Item 5 Message ID

		List_Right_Short_3_TopX,		// List Item 6 TopX
		List_Right_Short_3_TopY,		// List Item 6 Topy
		List_Right_Short_3_BottomX,		// List Item 6 BottomX
		List_Right_Short_3_BottomY,		// List Item 6 BottomY
		M_CustomMsg5,					// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_TEST_CARD_PRINT,				// Display Test Card Print Setup
	{
		6,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type:
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_TestCardPrint,				// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		List_Left_Short_3_TopX,			// List Item 5 TopX
		List_Left_Short_3_TopY,			// List Item 5 Topy
		List_Left_Short_3_BottomX,		// List Item 5 BottomX
		List_Left_Short_3_BottomY,		// List Item 5 BottomY
		M_CustomMsg4,					// List Item 5 Message ID

		List_Right_Short_3_TopX,		// List Item 6 TopX
		List_Right_Short_3_TopY,		// List Item 6 Topy
		List_Right_Short_3_BottomX,		// List Item 6 BottomX
		List_Right_Short_3_BottomY,		// List Item 6 BottomY
		M_CustomMsg5,					// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_MAINTAIN_CARD_PRINT,			// Display Maintainance Card Print Setup
	{
		6,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type:
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_MatnCardPrint,				// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		List_Left_Short_3_TopX,			// List Item 5 TopX
		List_Left_Short_3_TopY,			// List Item 5 Topy
		List_Left_Short_3_BottomX,		// List Item 5 BottomX
		List_Left_Short_3_BottomY,		// List Item 5 BottomY
		M_CustomMsg4,					// List Item 5 Message ID

		List_Right_Short_3_TopX,		// List Item 6 TopX
		List_Right_Short_3_TopY,		// List Item 6 Topy
		List_Right_Short_3_BottomX,		// List Item 6 BottomX
		List_Right_Short_3_BottomY,		// List Item 6 BottomY
		M_CustomMsg5,					// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_PUB_TRANS_PRINT,				// Display Trans Without ASN Print Setup
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type:
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_PubTransPrint,				// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},
//Douzhq 20140919 Add , Config Printer Set
//SCR_EXPAND_SETUP_PRINT
{
		3,
		NORMAL_SCR,
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_ExpandSetPrint,				// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_PrinterSwitch,				// List Item 1 Message ID		
	
		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_InquirySwitch,				// List Item 2 Message ID	
		
		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_ReceiptNumber,				// List Item 3 Message ID	
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID	
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID	
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID	
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID	
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID	
},

//SCR_PRINTER_SWITCH_SET
{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,				        // Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_PrinterSwitchSet,				// Title Message ID
		
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID
		
		List_Long_4_TopX,				// List Item 2 TopX
		List_Long_4_TopY,				// List Item 2 Topy
		List_Long_4_BottomX,			// List Item 2 BottomX
		List_Long_4_BottomY,			// List Item 2 BottomY
		M_PrinterMsg,					// List Item 2 Message ID
		
		0,				                // List Item 3 TopX
		0,				                // List Item 3 Topy
		0,			                    // List Item 3 BottomX
		0,			                    // List Item 3 BottomY
		M_Started,					    // List Item 3 Message ID
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
},

//SCR_INQUIRY_SWITCH_SET
{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,				        // Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_InquirySwitchSet,				// Title Message ID
		
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID
		
		List_Long_4_TopX,				// List Item 2 TopX
		List_Long_4_TopY,				// List Item 2 Topy
		List_Long_4_BottomX,			// List Item 2 BottomX
		List_Long_4_BottomY,			// List Item 2 BottomY
		M_InquiryMsg,					// List Item 2 Message ID
		
		0,				                // List Item 3 TopX
		0,				                // List Item 3 Topy
		0,			                    // List Item 3 BottomX
		0,			                    // List Item 3 BottomY
		M_Started,					    // List Item 3 Message ID
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
},

//SCR_RECEIPT_SWITCH_SET
{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,				        // Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_ReceiptNumberSet,				// Title Message ID
		
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID
		
		List_Long_4_TopX,				// List Item 2 TopX
		List_Long_4_TopY,				// List Item 2 Topy
		List_Long_4_BottomX,			// List Item 2 BottomX
		List_Long_4_BottomY,			// List Item 2 BottomY
		M_ReceiptMsg,					// List Item 2 Message ID
		
		0,				                // List Item 3 TopX
		0,				                // List Item 3 Topy
		0,			                    // List Item 3 BottomX
		0,			                    // List Item 3 BottomY
		M_Started,					    // List Item 3 Message ID
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
},
//	SCR_MATN_FUNCTION,					// Display Maintainance Screen
	{
#ifdef MAKE_CARD_MODE2        
		7,								// List Count, Not Include The Title List, 8 Items At Most
#else

#ifdef MAKE_CASH_CR
        8,								// List Count, Not Include The Title List, 8 Items At Most
#else
        8,								// List Count, Not Include The Title List, 8 Items At Most
#endif // MAKE_CASH_CR

#endif //MAKE_CARD_MODE2
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_MatnFunction,					// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_UnfinishedTrans,				// List Item 1 Message ID

		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_RunningLog,					// List Item 2 Message ID

		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_LocalPrinter,					// List Item 3 Message ID

		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_SetPOSP,						// List Item 4 Message ID

		List_Left_Short_3_TopX,			// List Item 5 TopX
		List_Left_Short_3_TopY,			// List Item 5 Topy
		List_Left_Short_3_BottomX,		// List Item 5 BottomX
		List_Left_Short_3_BottomY,		// List Item 5 BottomY
//		M_FormatFlash,					// List Item 5 Message ID
		M_SpecialFunction,				// List Item 5 Message ID

		List_Right_Short_3_TopX,		// List Item 6 TopX
		List_Right_Short_3_TopY,		// List Item 6 Topy
		List_Right_Short_3_BottomX,		// List Item 6 BottomX
		List_Right_Short_3_BottomY,		// List Item 6 BottomY
		M_SoftwareUpgrade,				// List Item 6 Message ID

		List_Left_Short_4_TopX,			// List Item 7 TopX
		List_Left_Short_4_TopY,			// List Item 7 Topy
		List_Left_Short_4_BottomX,		// List Item 7 BottomX
		List_Left_Short_4_BottomY,		// List Item 7 BottomY
		M_RebuildProtected,				// List Item 7 Message ID

#ifdef MAKE_CARD_MODE2
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
#else


//#ifdef MAKE_CASH_CR
//      0,								// List Item 8 TopX
//		0,								// List Item 8 Topy
//		0,								// List Item 8 BottomX
//		0,								// List Item 8 BottomY
//		M_Started,						// List Item 8 Message ID
#ifdef MAKE_CASH_CR
		List_Right_Short_4_TopX,		// List Item 8 TopX
		List_Right_Short_4_TopY,		// List Item 8 Topy
		List_Right_Short_4_BottomX,		// List Item 8 BottomX
		List_Right_Short_4_BottomY,		// List Item 8 BottomY
		M_Manage,
#else
        List_Right_Short_4_TopX,		// List Item 8 TopX
		List_Right_Short_4_TopY,		// List Item 8 Topy
		List_Right_Short_4_BottomX,		// List Item 8 BottomX
		List_Right_Short_4_BottomY,		// List Item 8 BottomY
		
//Douzhq 20140609 Add , Menu About
#ifndef MAKE_CASH_CR
		M_Manage,
#else
		M_WorkManage,					// List Item 8 Message ID
#endif

#endif // MAKE_CASH_CR

#endif // MAKE_CARD_MODE2
	},


//	SCR_UNFINISHED_TRANS,				// Display Clear Unfinished Trans Screen
	{
		3,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_UnfinishedTransSetUp,			// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_UPLOAD,							// Display Inquire Running Log Screen
	{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,					// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_RunningLogSetup,				// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_UploadPWD,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_UPLOAD_READY,					// Display Upload Management Ready Screen
	{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,					// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_RunningLogSetup,				// Title Message ID
	
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_UploadSoftware,				// List Item 1 Message ID

		List_Long_4_TopX,				// List Item 2 TopX
		List_Long_4_TopY,				// List Item 2 Topy
		List_Long_4_BottomX,			// List Item 2 BottomX
		List_Long_4_BottomY,			// List Item 2 BottomY
		M_UploadReady,					// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_FORMAT,							// Display Format Flash Screen
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
//		M_FormatFlashFunction,			// Title Message ID
		M_SpecialFunctionTitle,			// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_PowerOnDebit,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_BlackListSetup,				// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_FormatTransFlash,				// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_FormatConfigFlash,			// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_POSP,							// Display Setup POSP Screen
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_SetUpPOSP,					// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_FunctionNotSupported,			// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_LOCAL_PRINTER,					// Display Setup Local Printer Screen
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_LocalPrinterSetUp,			// Title Message ID
	
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_SOFTWARE_UPGRADE,				// Display Software Upgrade Screen
	{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_UpgradeSW,					// Title Message ID
	
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_UpgradeSW0,					// List Item 1 Message ID

		List_Long_3_TopX,				// List Item 2 TopX
		List_Long_3_TopY,				// List Item 2 Topy
		List_Long_3_BottomX,			// List Item 2 BottomX
		List_Long_3_BottomY,			// List Item 2 BottomY
		M_UpgradeSW1,					// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},


//	SCR_FORMATCONFIG,					// Display Format Config Flash Screen
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_FormatConfigFunction,			// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_Warning,						// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_FormatWarning1,				// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_FormatWarning2,				// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_FormatWarning3,				// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_FORMATRECORD,					// Display Format Record Flash Screen
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_FormatTransFunction,			// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_Warning,						// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_FormatWarning4,				// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
//		M_FormatWarning5,				// List Item 3 Message ID
		M_Started,						// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_FormatWarning3,				// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_DEBITPOWERON,					// Display The Information For Power Down Debit Or Power On
	{
		3,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_DebitPowerOn,					// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CurrentSpecialFunc,			// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_4_TopX,				// List Item 3 TopX
		List_Long_4_TopY,				// List Item 3 Topy
		List_Long_4_BottomX,			// List Item 3 BottomX
		List_Long_4_BottomY,			// List Item 3 BottomY
		M_UpdateSpecialFunc,			// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_POSBLACKLIST,					// Display The Information For Check Black List First On POS Or Local
	{
		3,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_BlackListPOSSetup,			// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CurrentSpecialFunc,			// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_4_TopX,				// List Item 3 TopX
		List_Long_4_TopY,				// List Item 3 Topy
		List_Long_4_BottomX,			// List Item 3 BottomX
		List_Long_4_BottomY,			// List Item 3 BottomY
		M_UpdateSpecialFunc,			// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_DISP_TRANS						// Display The Trans Record
	{
		4,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_TransDisp,					// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID

		List_Long_2_TopX,				// List Item 2 TopX
		List_Long_2_TopY,				// List Item 2 Topy
		List_Long_2_BottomX,			// List Item 2 BottomX
		List_Long_2_BottomY,			// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID

		List_Long_3_TopX,				// List Item 3 TopX
		List_Long_3_TopY,				// List Item 3 Topy
		List_Long_3_BottomX,			// List Item 3 BottomX
		List_Long_3_BottomY,			// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID

		List_Long_4_TopX,				// List Item 4 TopX
		List_Long_4_TopY,				// List Item 4 Topy
		List_Long_4_BottomX,			// List Item 4 BottomX
		List_Long_4_BottomY,			// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

#ifdef MAKE_PSAM_VERIFY

//	SCR_PSAM_V_UNSUPPORT,				// Display PSAM Unsupport Verify Function
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_PSAM_V_UNREGISTERED,			// Display PSAM UnCertificate Pump
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_PSAM_V_PSAM_LOCK,				// Display PSAM Locked For Certificate Pump
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_PSAM_V_INSERT_RID,				// Display Insert RID Card Please
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//	SCR_PSAM_V_REGISTER_OK,				// Display PSAM Register OK Screen
	{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

#endif

#ifdef MAKE_CARD_MODE2
	
//  SCR_WORK_MODE                       // Display Work Mode Set Screen    
	{
		3,								// List Count, Not Include The Title List, 8 Items At Most
		INFORMATION_SCR,				// Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_WorkModeSet,					// Title Message ID
	
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID

		List_Long_3_TopX,				// List Item 2 TopX
		List_Long_3_TopY,				// List Item 2 Topy
		List_Long_3_BottomX,			// List Item 2 BottomX
		List_Long_3_BottomY,			// List Item 2 BottomY
		M_WorkModeSwitch,				// List Item 2 Message ID

		List_Long_4_TopX,				// List Item 3 TopX
		List_Long_4_TopY,				// List Item 3 Topy
		List_Long_4_BottomX,			// List Item 3 BottomX
		List_Long_4_BottomY,			// List Item 3 BottomY
		M_WorkModeExit,					// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},


#endif

//#ifndef MAKE_CARD_MODE2

    // SCR_WORK_MANAGE,                 // Display Work Manage Set Screen
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,				        // Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_WorkManageSetup,				// Title Message ID
	
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID

		0,				                // List Item 2 TopX
		0,				                // List Item 2 Topy
		0,			                    // List Item 2 BottomX
		0,			                    // List Item 2 BottomY
		M_Started,				        // List Item 2 Message ID

		0,				                // List Item 3 TopX
		0,				                // List Item 3 Topy
		0,			                    // List Item 3 BottomX
		0,			                    // List Item 3 BottomY
		M_Started,					    // List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},
	
//Douzhq 20140610 Add , Menu About
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	//SCR_MANAGE_SOCIAL
{
		6,
		NORMAL_SCR,
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_ManageFunction,				// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_WorkManage2,					// List Item 1 Message ID		
	
		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_LockNozzleManage,				// List Item 2 Message ID	
		
		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_CardRecognize,				// List Item 3 Message ID	
		
		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_SaveOilManage,				// List Item 4 Message ID	
		
		List_Left_Short_3_TopX,			// List Item 5 TopX
		List_Left_Short_3_TopY,			// List Item 5 Topy
		List_Left_Short_3_BottomX,		// List Item 5 BottomX
		List_Left_Short_3_BottomY,		// List Item 5 BottomY
		M_DiscountManage,				// List Item 5 Message ID	
		
		List_Right_Short_3_TopX,		// List Item 6 TopX
		List_Right_Short_3_TopY,		// List Item 6 Topy
		List_Right_Short_3_BottomX,		// List Item 6 BottomX
		List_Right_Short_3_BottomY,		// List Item 6 BottomY
		M_SafeMode,						// List Item 6 Message ID	
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID	
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID	
},
#endif
	//SCR_LOCK_NOZZLE
{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,				        // Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_SetLockNozzle,				// Title Message ID
		
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID
		
		List_Long_4_TopX,				// List Item 2 TopX
		List_Long_4_TopY,				// List Item 2 Topy
		List_Long_4_BottomX,			// List Item 2 BottomX
		List_Long_4_BottomY,			// List Item 2 BottomY
		M_SelLockNozzle,				// List Item 2 Message ID
		
		0,				                // List Item 3 TopX
		0,				                // List Item 3 Topy
		0,			                    // List Item 3 BottomX
		0,			                    // List Item 3 BottomY
		M_Started,					    // List Item 3 Message ID
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
},
//Douzhq 20141010 Add , Safe Mode
//-------  Begin , Douzhq Add , Safe Mode , 20141010 -------
//SCR_SAFE_MODE
{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,				        // Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_SetSafeMode,					// Title Message ID
		
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID
		
		List_Long_4_TopX,				// List Item 2 TopX
		List_Long_4_TopY,				// List Item 2 Topy
		List_Long_4_BottomX,			// List Item 2 BottomX
		List_Long_4_BottomY,			// List Item 2 BottomY
		M_SelSafeMode,					// List Item 2 Message ID
		
		0,				                // List Item 3 TopX
		0,				                // List Item 3 Topy
		0,			                    // List Item 3 BottomX
		0,			                    // List Item 3 BottomY
		M_Started,					    // List Item 3 Message ID
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
},

//-------  End   , Douzhq Add , Safe Mode , 20141010 -------

#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	//SCR_ICCARD_RECOGINZE
{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,				        // Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_SetCardRecoginze,				// Title Message ID
		
		List_Long_1_TopX,				// List Item 1 TopX
		List_Long_1_TopY,				// List Item 1 Topy
		List_Long_1_BottomX,			// List Item 1 BottomX
		List_Long_1_BottomY,			// List Item 1 BottomY
		M_ICCardAppSwitch,				// List Item 1 Message ID
		
		List_Long_2_TopX,		        // List Item 2 TopX
		List_Long_2_TopY,		        // List Item 2 Topy
		List_Long_2_BottomX,	        // List Item 2 BottomX
		List_Long_2_BottomY,		    // List Item 2 BottomY
		M_SwitchCardRecognize,		    // List Item 2 Message ID
		
		0,				                // List Item 3 TopX
		0,				                // List Item 3 Topy
		0,			                    // List Item 3 BottomX
		0,			                    // List Item 3 BottomY
		M_Started,					    // List Item 3 Message ID
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID	
},

//SCR_ICCARD_SELECT
{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,				        // Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_TitleCardRecognize,			// Title Message ID
		
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID
		
		List_Long_4_TopX,		       	// List Item 2 TopX
		List_Long_4_TopY,		        // List Item 2 Topy
		List_Long_4_BottomX,	        // List Item 2 BottomX
		List_Long_4_BottomY,		    // List Item 2 BottomY
		M_SelCardRecognize,		    	// List Item 2 Message ID
		
		0,				                // List Item 3 TopX
		0,				                // List Item 3 Topy
		0,			                    // List Item 3 BottomX
		0,			                    // List Item 3 BottomY
		M_Started,					    // List Item 3 Message ID
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID	
},
	//SCR_CARDAPP_SELECT
{
		4,
		NORMAL_SCR,
	
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_TitleCardSwitch,				// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_CustomMsg0,					// List Item 1 Message ID		
	
		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_CustomMsg1,					// List Item 2 Message ID	
		
		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_CustomMsg2,					// List Item 3 Message ID	
		
		List_Right_Short_2_TopX,		// List Item 4 TopX
		List_Right_Short_2_TopY,		// List Item 4 Topy
		List_Right_Short_2_BottomX,		// List Item 4 BottomX
		List_Right_Short_2_BottomY,		// List Item 4 BottomY
		M_CustomMsg3,					// List Item 4 Message ID	
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID	
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID	
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID	
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID	
},
	//SCR_SAVEOIL_FUN
{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,				        // Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_SetSaveOilApp,				// Title Message ID
		
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID
		
		List_Long_4_TopX,				// List Item 2 TopX
		List_Long_4_TopY,				// List Item 2 Topy
		List_Long_4_BottomX,			// List Item 2 BottomX
		List_Long_4_BottomY,			// List Item 2 BottomY
		M_SelSaveOilApp,				// List Item 2 Message ID
		
		0,				                // List Item 3 TopX
		0,				                // List Item 3 Topy
		0,			                    // List Item 3 BottomX
		0,			                    // List Item 3 BottomY
		M_Started,					    // List Item 3 Message ID
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID	
},
	
	//SCR_VIPFUELING_FUN
{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,				        // Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_SetVipFueling,				// Title Message ID
		
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID
		
		List_Long_4_TopX,				// List Item 2 TopX
		List_Long_4_TopY,				// List Item 2 Topy
		List_Long_4_BottomX,			// List Item 2 BottomX
		List_Long_4_BottomY,			// List Item 2 BottomY
		M_SelVipFueling,				// List Item 2 Message ID
		
		0,				                // List Item 3 TopX
		0,				                // List Item 3 Topy
		0,			                    // List Item 3 BottomX
		0,			                    // List Item 3 BottomY
		M_Started,					    // List Item 3 Message ID
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID	
},

#endif

//Douzhq 20140819 Add , Menu About
//Manage Menu , No PSAM and CASH
#if defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)
	//SCR_MANAGE_SOCIAL1
	{
		3,
		NORMAL_SCR,
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_ManageFunction,				// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_WorkManage2,					// List Item 1 Message ID		
	
		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_LockNozzleManage,				// List Item 2 Message ID	
		
		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_SafeMode,						// List Item 3 Message ID	
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID	
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID	
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID	
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID	
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID	
	},
#endif

//Douzhq 20140819 Add , Car Number Recognize
#ifdef MAKE_CASH_CR
//Douzhq 20140718 Add , Card Number Recognize
//SCR_MANAGE_CARDNUM
{
		
		3,
		NORMAL_SCR,
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_ManageFunction,				// Title Message ID
	
		List_Left_Short_1_TopX,			// List Item 1 TopX
		List_Left_Short_1_TopY,			// List Item 1 Topy
		List_Left_Short_1_BottomX,		// List Item 1 BottomX
		List_Left_Short_1_BottomY,		// List Item 1 BottomY
		M_LockNozzleManage,				// List Item 1 Message ID		
	
		List_Right_Short_1_TopX,		// List Item 2 TopX
		List_Right_Short_1_TopY,		// List Item 2 Topy
		List_Right_Short_1_BottomX,		// List Item 2 BottomX
		List_Right_Short_1_BottomY,		// List Item 2 BottomY
		M_CardNumberRecognize,			// List Item 2 Message ID	
		
		List_Left_Short_2_TopX,			// List Item 3 TopX
		List_Left_Short_2_TopY,			// List Item 3 Topy
		List_Left_Short_2_BottomX,		// List Item 3 BottomX
		List_Left_Short_2_BottomY,		// List Item 3 BottomY
		M_SafeMode,						// List Item 3 Message ID	
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID	
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID	
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID	
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID	
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID	
},
//Douzhq 20140718 Add , Car Number Recognize
//SCR_CARNUMBER_RECOGNIZE
{
		2,								// List Count, Not Include The Title List, 8 Items At Most
		NORMAL_SCR,				        // Screen Type
		
		List_Title_Line_TopX,			// Title TopX
		List_Title_Line_TopY,			// Title Topy
		List_Title_Line_BottomX,		// Title BottomX
		List_Title_Line_BottomY,		// Title BottomY
		M_SetCarNumberRecognize,		// Title Message ID
		
		List_Long_2_TopX,				// List Item 1 TopX
		List_Long_2_TopY,				// List Item 1 Topy
		List_Long_2_BottomX,			// List Item 1 BottomX
		List_Long_2_BottomY,			// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID
		
		List_Long_4_TopX,				// List Item 2 TopX
		List_Long_4_TopY,				// List Item 2 Topy
		List_Long_4_BottomX,			// List Item 2 BottomX
		List_Long_4_BottomY,			// List Item 2 BottomY
		M_SelCarNumberRecognize,	    // List Item 2 Message ID
		
		0,				                // List Item 3 TopX
		0,				                // List Item 3 Topy
		0,			                    // List Item 3 BottomX
		0,			                    // List Item 3 BottomY
		M_Started,					    // List Item 3 Message ID
		
		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID
		
		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID
		
		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID
		
		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID
		
		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID	
},

//DEFAULT_CARNUM_REC
{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
},

//DEFAULT_GRADELIMIT_REC
{
		0,								// List Count, Not Include The Title List, 8 Items At Most
		DEFAULT_SCR,					// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Started,						// Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_Started,						// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
},
#endif

    // SCR_INPUT_EMP_NUMBER: Display Input Emp Number Screen
	{
		1,								// List Count, Not Include The Title List, 8 Items At Most
		POPUP_SCR,						// Screen Type: 0: Default Screen; 1: Normal List Items Screen; 2: Popup Screen
		
		0,								// Title TopX
		0,								// Title Topy
		0,								// Title BottomX
		0,								// Title BottomY
		M_Emp_Number,				    // Title Message ID

		0,								// List Item 1 TopX
		0,								// List Item 1 Topy
		0,								// List Item 1 BottomX
		0,								// List Item 1 BottomY
		M_CustomMsg1,					// List Item 1 Message ID

		0,								// List Item 2 TopX
		0,								// List Item 2 Topy
		0,								// List Item 2 BottomX
		0,								// List Item 2 BottomY
		M_Started,						// List Item 2 Message ID

		0,								// List Item 3 TopX
		0,								// List Item 3 Topy
		0,								// List Item 3 BottomX
		0,								// List Item 3 BottomY
		M_Started,						// List Item 3 Message ID

		0,								// List Item 4 TopX
		0,								// List Item 4 Topy
		0,								// List Item 4 BottomX
		0,								// List Item 4 BottomY
		M_Started,						// List Item 4 Message ID

		0,								// List Item 5 TopX
		0,								// List Item 5 Topy
		0,								// List Item 5 BottomX
		0,								// List Item 5 BottomY
		M_Started,						// List Item 5 Message ID

		0,								// List Item 6 TopX
		0,								// List Item 6 Topy
		0,								// List Item 6 BottomX
		0,								// List Item 6 BottomY
		M_Started,						// List Item 6 Message ID

		0,								// List Item 7 TopX
		0,								// List Item 7 Topy
		0,								// List Item 7 BottomX
		0,								// List Item 7 BottomY
		M_Started,						// List Item 7 Message ID

		0,								// List Item 8 TopX
		0,								// List Item 8 Topy
		0,								// List Item 8 BottomX
		0,								// List Item 8 BottomY
		M_Started,						// List Item 8 Message ID
	},

//#endif  


};







// Message Display Attrib
//unsigned int MsgAttrib;

// Current Message Delay TimeOut
// Current Message Will Remain On Display, Until TimeOut
//unsigned int MsgDelayTime;

// Info Line: For Display Information Message At The InfoLine
unsigned char InfoLine[21];
unsigned char CurrentInfoLine[21];


// Attention: Do Not Change The CustomMsgs's Defination; Seriously
// Attention: Do Not Change The CustomMsgs's Defination; Seriously
// CustomMsg: For Display Custom Message
unsigned char CustomMsg0[21];
unsigned char CurrentCustomMsg0[21];

unsigned char CustomMsg1[21];
unsigned char CurrentCustomMsg1[21];

unsigned char CustomMsg2[21];
unsigned char CurrentCustomMsg2[21];

unsigned char CustomMsg3[21];
unsigned char CurrentCustomMsg3[21];

unsigned char CustomMsg4[21];
unsigned char CurrentCustomMsg4[21];

unsigned char CustomMsg5[21];
unsigned char CurrentCustomMsg5[21];

unsigned char CustomMsg6[21];
unsigned char CurrentCustomMsg6[21];

unsigned char CustomMsg7[21];
unsigned char CurrentCustomMsg7[21];

unsigned char CustomMsg8[21];
unsigned char CurrentCustomMsg8[21];

unsigned char CustomMsg9[21];
unsigned char CurrentCustomMsg9[21];
// Attention: Do Not Change The CustomMsgs's Defination; Seriously
// Attention: Do Not Change The CustomMsgs's Defination; Seriously


// Screen Display Attrib
//unsigned int ScrAttrib;

// Current Screen Delay TimeOut
// Current Screen Will Remain On Display, Until TimeOut
//unsigned int ScrDelayTime;

// Screen Id: For Display SCREEN
enum SCREEN ScreenID;
enum SCREEN NextScreenID;
enum SCREEN LastScreenID;


// Save The Displayed Default Screen Status
// According To ProcStatus[PROCESS_CR]
// Is Changed, Display The New Default Screen
unsigned int LastDefaultSCRStatus;

// The Inversed Display List Item Index
unsigned char DispListInversedIdx;

// The Display Item Index For Trans Record
unsigned char DispRecordIdx;

