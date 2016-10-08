
#ifndef _CASHTRANS_H
#define _CASHTRANS_H

#define	STX_C			0xFA
#define ETX_C			0xFB

#define	POLLING			'0'
#define	ENGROSS			'1'
#define	SND_RECORD		'2'
#define	CARD_CPDU		'3'
#define	WARNING			'4'
#define	CASH_AMN		'5'
#define	CPDU_END		'6'
#define	CR_INPUT		'7'
#define	AUTH_CR			'8'
#define	LOCK_ASK_ESC	'9'

//Douzhq 20140721 Add , Car Number Recognize
//#define STATUS_QUE		0x3D
//#define QUERY_CARREC	0x3E
#define CAR_NUMBER_REC  0x3F
#define CAR_NUMBER_CAL	0x40

//Douzhq 20140721 Add , Grade Limte 
#define GRADE_LIMIT     0x41
#define GRADE_LIM_RES   0x42

#define WAIT_RESPONSE_TIME	1000

//Douzhq 20140721 Add , Car Number Recognize 
#define WAIT_CAR_RECOGNIZE  (4 * 1000)
#define WAIT_RESPONSE_CAR   (2 * 1000)
#define WAIT_CMD_REQ		(9 * 1000)

enum CRE_COMMAND_STATUS
{
	CRE_Command_Rev,
	CRE_Command_Sending,
	CRE_Command_WaitCr,
	CRE_Command_WaitRespose,
	CRE_Command_ResposeTimeOut
};


extern void InitCashProcess( void );

extern void CashProcess( void );



#endif