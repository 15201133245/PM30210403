
#include "global.h"

#include "define.h"

#include "Pdprotect.h"
#include "powerdown.h"
#include "WriteLog.h"
#include "CRVaribles.h"





unsigned char Pdtimeoutflag = 0;
unsigned char PdRunning = 0;

Tmr * PowerDown_TimeOut;


/*
 ** PD_TimeOut
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Powerdown\Source\PowerDown.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Call Back For Power Down Check 
 *
 *  RETURNS: None.
 *
 */
void PD_TimeOut_CallBack( void )
{
	// Power Down For Time Out 
	Pdtimeoutflag = 1;
}


/*
 ** PwoerDownProcess
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Powerdown\Source\PowerDown.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Main Entrance For Power Down Process, Information Other Process Power Is Down,
 *				 And Inform Driver To Save The PwoerDown Datas After A Certain Time or Response From Other Process
 *
 *  RETURNS: None.
 *
 */
void PwoerDownProcess( void )
{
	unsigned char i;

	// Check Wether The Power Flag Is Set By Driver
	// Maybe 1: Issued PowerDown; 2: Issue Card Spring Out
	if( PowerDownFlg != 0 )
	{
		// Power Down Is Issued
		if( PdRunning == 0 )
		{
			// First Entry
			// Save Last Trans AnyWay
			SavePowerDownTrans( &PowerDown.PDStartFlag, sizeof( PD_All ));

#ifdef MAKE_CASH_CR
			if( CRType == CR_TYPE_CASH )
			{
		   		// Debit On Power Down For Cash Trans
		   		// Delay PDTimeOut = 6 Seconds
		   		PowerDown_TimeOut = CreateTimer( TIMER_ONE_DELETE, PDTimeOut * 3, PD_TimeOut_CallBack );
			}
			else
#endif
			{
				// Create The Timer Check For Waiting The Response from Other Processes
				if( PowerDown.SpecialFunction & DebitOnPowerOn )
				{
			   		// Debit On Power On
			   		// Delay PDTimeOut = 2 Seconds
			   		PowerDown_TimeOut = CreateTimer( TIMER_ONE_DELETE, PDTimeOut, PD_TimeOut_CallBack );
				}
				else
				{
			   		// Debit On Power Down
			   		// Delay PDTimeOut = 4 Seconds
			   		PowerDown_TimeOut = CreateTimer( TIMER_ONE_DELETE, PDTimeOut * 2, PD_TimeOut_CallBack );
				}
			}

			// Inform Other Process
			for( i = PROCESS_START; i < PROCESS_MAX; i++ )
			{
				ProcExchange[PROCESS_POWERDOWN][i] |= POWERDOWN;
			}
		
			// Write " Beging Power Down Log"
			WritePdLog( 0 );

			PdRunning = 1;
		}
		else
		{
			// Check Power Down Time Out
			if( Pdtimeoutflag == 1 )
			{
				// Write " Power Down TimeOut Log"
				WritePdLog( 2 );

				// Must Save The Data, Inform Driver
				PowerDownDispose( &PowerDown.PDStartFlag, sizeof( PD_All ));
			
			}
			else
			{
				// Not Time Out, Check Other Process Response For PowerDown
				if(( ProcExchange[PROCESS_CR][PROCESS_POWERDOWN] & POWERDOWN ) &&

#ifdef MAKE_CASH_CR
					( ProcExchange[PROCESS_CASH][PROCESS_POWERDOWN] & POWERDOWN ) &&
#endif

					( ProcExchange[PROCESS_CR][PROCESS_POWERDOWN] & POWERDOWN ))
				{
					// Write " Power Down Ok Log"
					WritePdLog( 1 );

					// Current Only Check CR Process Response
					// Inform Driver, Can Start To Save The Data And Power Down
					PowerDownDispose( &PowerDown.PDStartFlag, sizeof( PD_All ));

				}
				else
				{
					// Waiting Response
				}
			}
		}
	}
}


/*
 ** WritePdLog
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Powerdown\Source\PowerDown.c
 *
 *  PARAMETERS:	PdType: 0: Begin PowerDown; 1: PowerDown Save End
 *
 *  DESCRIPTION: Write PowerDown Log
 *
 *  RETURNS: None.
 *
 */
void WritePdLog( unsigned char PdType )
{
	unsigned char PdLogTxt[52];
	unsigned char len;

	// Clear Buffers
	memset( PdLogTxt, 0x00, sizeof( PdLogTxt ));
	len = strlen(( char * )PdLogTxt );

	// Fill " Begin PowerDown "
	if( PdType == 0 )
	{
		memcpy( &PdLogTxt[len], "开始掉电保存: ", 14 );
	}
	else if( PdType == 1 )
	{
		memcpy( &PdLogTxt[len], "掉电保存正常完成: ", 18 );
	}
	else if( PdType == 2 )
	{
		memcpy( &PdLogTxt[len], "掉电保存超时完成: ", 18 );
	}
	else
	{
		memcpy( &PdLogTxt[len], "掉电保存错误完成: ", 18 );
	}

	len = strlen(( char * )PdLogTxt );

	// Save To Log
	WriteLog( PROCESS_POWERDOWN, PdLogTxt );
}
