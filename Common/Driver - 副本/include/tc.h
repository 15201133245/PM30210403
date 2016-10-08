#ifndef	_TC_H
#define	_TC_H
#include "AT91SAM7SE512.h"
#include <stdlib.h>

#define TIME_MAX					64

#define OUT_TIME_MAX				16

#define	TIMER_ONE					0x01
#define	TIMER_AUTO					0x02
#define	TIMER_ONE_DELETE			0x04
#define	TIMER_ONE_STOP				0x08

#define TIMER_DELETE				0x40
#define	TIMER_PAUSE					0x80

#define TIMER_MS					10
#define TIMER_5MS					(5 * TIMER_MS)
#define TIMER_10MS					(10*TIMER_MS)

typedef void (*TcCallBack )();

//timer struct
typedef struct
{
		unsigned int last_val;
		unsigned int current_val;
}Timer_Val;

typedef struct _Tmr
{
		unsigned char	type;
		unsigned char	style;
		unsigned int	timer_val;
		unsigned int	current_val;
		TcCallBack		TimerRun;
}Tmr;

#ifdef OUT_TIME_MAX 
typedef void (*tcCallback)(void*);
typedef struct _OutTmr
{
	unsigned char type;
	unsigned char style;
	unsigned int timer_val;
	unsigned int current_val;
	tcCallback TimerRun;
	void* para;
}OutTmr;
#endif


inline void TC_Configure(AT91S_TC *tc,unsigned int mode)
{
	//Disable TC clock
	tc->TC_CCR = AT91C_TC_CLKDIS;
	//Disable interrupts
	tc->TC_IDR = 0xffffffff;
	//Clear status register
	tc->TC_SR;
	tc->TC_CMR = mode;
}

inline void TC_RCConfigure(AT91S_TC *tc,
						   unsigned int ms,
						   unsigned int mck)
{
	unsigned int val;
	
	val = mck >> 10;
	val *= ms;
	val /= 100;
	if ( (val % 10) >= 5 )
	{
		val = (val / 10) + 1;
	}
	else
	{
		val = val / 10;
	}
	tc->TC_RC = val;
}

inline void TC_Start(AT91S_TC *tc)
{
	tc->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
}

void InitTmrList(void);

Tmr *CreateTimer(unsigned char type,
				 unsigned int val,
				 TcCallBack callback);

unsigned char DeleteTimer(Tmr *pTmr);

unsigned char ResetTimer(Tmr *pTmr);

unsigned char StopTimer(Tmr *pTmr);

unsigned char ChangeTimerFunc(Tmr *pTmr,TcCallBack callback);

unsigned char ChangeTime(Tmr* pTmr,unsigned int time);

OutTmr *CreateOutTimer(unsigned char type,
					   unsigned int val,
					   tcCallback callback,
					   void* para);

#ifdef OUT_TIME_MAX
unsigned char DeleteOutTimer(OutTmr* pOutTmr);

unsigned char ResetOutTimer(OutTmr *pOutTmr);

unsigned char StopOutTimer(OutTmr *pOutTmr);

unsigned char ChangeOutTimerFunc(OutTmr *pOutTmr,tcCallback callback,void* para);

unsigned char ChangeOutTime(OutTmr* pOutTmr,unsigned int time);
#endif

void CheckTimerGo(void);

void InitTmrList1(void);

Tmr *CreateTimer1(unsigned char type,
				  unsigned int val,
				  TcCallBack callback);

unsigned char DeleteTimer1(Tmr *pTmr);

unsigned char ResetTimer1(Tmr *pTmr);

void CheckTimer1Go(void);

void ResetAllTimer(void);

#endif