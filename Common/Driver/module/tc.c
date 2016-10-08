#include "tc.h"
#include <string.h>

Tmr TimeTmr[TIME_MAX];

extern	Timer_Val timer_val;

void InitTmrList(void)
{
	memset(TimeTmr,0,TIME_MAX * sizeof(Tmr));
}

Tmr *CreateTimer(unsigned char type,
				 unsigned int val,
				 TcCallBack callback)
{
	Tmr *pTmr;
	int i;
	
	for(i = 0; i < TIME_MAX; i++)
	{
		if(TimeTmr[i].type == 0)
		{
			pTmr = &TimeTmr[i];
			pTmr->type = type;
			pTmr->style = 0;
			pTmr->timer_val = val / TIMER_MS;
			pTmr->current_val = 0;
			pTmr->cv = timer_val.current_val;
			pTmr->TimerRun = callback;
			return(pTmr);
		}
	}
	return((Tmr *)0);
}

unsigned char DeleteTimer(Tmr *pTmr)
{
	int i;
	
	for(i = 0; i < TIME_MAX; i++)
	{
		if(pTmr == &TimeTmr[i])
		{
			break;
		}
	}
	if(i == TIME_MAX)
	{
		return 3;
	}
	if(pTmr->type != 0)
	{
		memset((unsigned char *)(pTmr),0,sizeof(Tmr));
		return 0;
	}
	return 3;
}

unsigned char ResetTimer(Tmr *pTmr)
{
	int i;
	
	for(i = 0; i < TIME_MAX; i++)
	{
		if(pTmr == &TimeTmr[i])
		{
			break;
		}
	}
	if(i == TIME_MAX)
	{
		return 3;
	}
	if(pTmr->type != 0)
	{
		pTmr->type &= (unsigned char)(~TIMER_PAUSE);
		pTmr->current_val = 0;
		return 0;
	}
	return 3;
}

unsigned char StopTimer(Tmr *pTmr)
{
	int i;
	
	for(i = 0; i < TIME_MAX; i++)
	{
		if(pTmr == &TimeTmr[i])
		{
			break;
		}
	}
	if(i == TIME_MAX)
	{
		return 3;
	}
	if(pTmr->type != 0)
	{
		pTmr->type |= TIMER_PAUSE;
		pTmr->current_val = 0;
		return 0;
	}
	return 3;
}

unsigned char ChangeTimerFunc(Tmr *pTmr,TcCallBack callback)
{
	int i;
	
	for(i = 0; i < TIME_MAX; i++)
	{
		if(pTmr == &TimeTmr[i])
		{
			break;
		}
	}
	if(i == TIME_MAX)
	{
		return 3;
	}
	if(pTmr->type != 0)
	{
		pTmr->TimerRun = callback;
		return 0;
	}
	return 3;
}

void CheckTimerGo(void)
{
	int i;
	Tmr *pTmr;
	TcCallBack callback;
	unsigned int val;
	
	val = timer_val.current_val - timer_val.last_val;
    timer_val.last_val = timer_val.current_val;
	for(i = 0; i < TIME_MAX; i++)
	{
		pTmr = &TimeTmr[i];
		if(((pTmr->type & TIMER_PAUSE) != TIMER_PAUSE) && (pTmr->type != 0))
		{
			pTmr->current_val += val;
			if(pTmr->current_val >= pTmr->timer_val)
			{
				pTmr->current_val = 0;
				callback = pTmr->TimerRun;
				if(callback != (TcCallBack)0)
				{
					callback();
				}
				if(pTmr->type == TIMER_ONE)
				{
					pTmr->type |= TIMER_PAUSE;
				}
				else if(pTmr->type == TIMER_ONE_DELETE)
				{
					DeleteTimer(pTmr);
					continue;
				}
				else if(pTmr->type == TIMER_ONE_STOP)
				{
					pTmr->type |= TIMER_PAUSE;
				}
			}
		}
	}
}

//TC1
#define	TIME1_MAX		4
static Tmr Time1Tmr[TIME1_MAX];
extern Timer_Val timer1_val;

void InitTmrList1(void)
{
	memset(Time1Tmr,0,TIME1_MAX * sizeof(Tmr));
}

Tmr *CreateTimer1(unsigned char type,
				  unsigned int val,
				  TcCallBack callback)
{
	Tmr *pTmr;
	int i;
	
	for(i = 0; i < TIME1_MAX; i++)
	{
		if(Time1Tmr[i].type == 0)
		{
			pTmr = &Time1Tmr[i];
			pTmr->type = type;
			pTmr->style = 0;
			pTmr->timer_val = val / TIMER_MS;
			pTmr->current_val = 0;
			pTmr->cv = timer_val.current_val;
			pTmr->TimerRun = callback;
			return(pTmr);
		}
	}
	return((Tmr *)0);
}

unsigned char DeleteTimer1(Tmr *pTmr)
{
	int i;
	
	for(i = 0; i < TIME1_MAX; i++)
	{
		if(pTmr == &Time1Tmr[i])
		{
			break;
		}
	}
	if(i == TIME1_MAX)
	{
		return 3;
	}
	if(pTmr->type != 0)
	{
		memset((unsigned char *)(pTmr),0,sizeof(Tmr));
		return 0;
	}
	return 3;
}

unsigned char ResetTimer1(Tmr *pTmr)
{
	int i;
	
	for(i = 0; i < TIME1_MAX; i++)
	{
		if(pTmr == &Time1Tmr[i])
		{
			break;
		}
	}
	if(i == TIME1_MAX)
	{
		return 3;
	}
	if(pTmr->type != 0)
	{
		pTmr->type &= (unsigned char)(~TIMER_PAUSE);
		pTmr->current_val = 0;
		return 0;
	}
	return 3;
}

void CheckTimer1Go(void)
{
	int i;
	Tmr *pTmr;
	TcCallBack callback;
	unsigned int val;
	
	val = timer_val.current_val - timer_val.last_val;
    timer_val.last_val = timer_val.current_val;
	for(i = 0; i < TIME1_MAX; i++)
	{
		pTmr = &Time1Tmr[i];
		if((pTmr->type & TIMER_PAUSE) != TIMER_PAUSE)
		{
			pTmr->current_val += val;
			if(pTmr->current_val >= pTmr->timer_val)
			{
				pTmr->current_val = 0;
				callback = pTmr->TimerRun;
				if(callback != (TcCallBack)0)
				{
					callback();
				}
				if(pTmr->type == TIMER_ONE)
				{
					pTmr->type |= TIMER_PAUSE;
				}
				else if(pTmr->type == TIMER_ONE_DELETE)
				{
					DeleteTimer1(pTmr);
					continue;
				}
				else if(pTmr->type == TIMER_ONE_STOP)
				{
					pTmr->type |= TIMER_PAUSE;
				}
			}
		}
	}
}

void ResetAllTimer(void)
{
	int i;
	Tmr *pTmr;
	
	for(i = 0; i < TIME_MAX; i++)
	{
		pTmr = &TimeTmr[i];
		if((pTmr->type & TIMER_PAUSE) != TIMER_PAUSE)
		{
			ResetTimer(pTmr);
		}
	}
}