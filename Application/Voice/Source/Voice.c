
#include <string.h>


#include "define.h"
#include "CRVaribles.h"

#include "Voice.h"

//Douzhq 20140917 Add , Voice Add
#include "CRMemCopy.h"




/*
 ** InitVoiceProcess
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Voice\Source\Voice.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Initialize For Voice Process, Called Before The Main Loop
 *
 *  RETURNS: None.
 *
 */
void InitVoiceProcess( void )
{
	memset( &PumpVoiceBuf.VolumeDefault, 0x00, sizeof( PumpVoiceBuf ));
}


/*
 ** VoiceProcess
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Voice\Source\Voice.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Entrance For Voice Process
 *
 *  RETURNS: None.
 *
 */
void VoiceProcess( void )
{
	unsigned int CR_VoiceAction, KeyPad_VoiceAction;

	CR_VoiceAction = VOICE_CARD | VOICE_NOZZLE_ON |	VOICE_BUSY
								| VOICE_EOT | VOICE_NOZZLE_ERR | VOICE_EXC
//Douzhq 20140916 Add , Voice Add
								| VOICE_CARDERR | VOICE_SCANCODE | VOICE_VERERR	
								| VOICE_VEROVER | VOICE_OILCHOOSE | VOICE_FAULTERR
								| VOICE_ICPWD;

	KeyPad_VoiceAction = VOICE_VOLUME_UP | VOICE_VOLUME_DOWN;
	
	//Douzhq 20140917 Add , Voice Add
#ifdef MAKE_CASH_CR
	unsigned char TmpBalance[4];
	unsigned int  Tmp_Balance;
#endif	
	// Check The CR Voice Actions
	switch( ProcExchange[PROCESS_CR][PROCESS_VOICE] & CR_VoiceAction )
	{
		case VOICE_CARD:
			
			// IC Card Inserted
			// Clear The Bit
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_CARD );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = CardInVoice;
			
			//Douzhq 20140916 Add , Voice Add
#ifdef MAKE_CASH_CR
			if (Cash_CR_Buf.CashType == 0x01 && CRType == CR_TYPE_CASH)
			{
				memcpy(&PumpVoiceBuf.VoiceData[0],TRINP.TR_CASH_TOTAL,sizeof(TRINP.TR_CASH_TOTAL));
			}
			else
			{
				//....
			}
#endif
			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;

		break;

		case VOICE_NOZZLE_ON:

			// Nozzle On
			// Clear The Bit
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_NOZZLE_ON );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = NozzleOnVoice;

			// Fill Grade Code
			memcpy( &PumpVoiceBuf.VoiceData[0], TRINP.TR_TRANS_GRADE_CODE, sizeof( TRINP.TR_TRANS_GRADE_CODE ));
			// Fill Price
			memcpy( &PumpVoiceBuf.VoiceData[2], TRINP.TR_TRANS_PRICE, sizeof( TRINP.TR_TRANS_PRICE ));
			// Fill Nozzle Number
			PumpVoiceBuf.VoiceData[4] = TRINP.TR_TRANS_NOZZLE_NUMBER;
			
			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;

		break;

		case VOICE_BUSY:

			// IC Card Busy
			// Clear The Bit
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_BUSY );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = BusyVoice;

			// Fill Grade Code
			memcpy( &PumpVoiceBuf.VoiceData[0], TRINP.TR_TRANS_GRADE_CODE, sizeof( TRINP.TR_TRANS_GRADE_CODE ));
			// Fill Price
			memcpy( &PumpVoiceBuf.VoiceData[2], TRINP.TR_TRANS_PRICE, sizeof( TRINP.TR_TRANS_PRICE ));
			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;

		break;

		case VOICE_EOT:

			//IC Card EOT
			// Clear The Bit
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_EOT );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = EOTVoice;

			// Fill Trans Amount
			memcpy( &PumpVoiceBuf.VoiceData[0], TRINP.TR_TRANS_AMOUNT, sizeof( TRINP.TR_TRANS_AMOUNT ));
			// Fill Price
			memcpy( &PumpVoiceBuf.VoiceData[4], TRINP.TR_TRANS_VOLUME, sizeof( TRINP.TR_TRANS_VOLUME ));
			//Douzhq 20140916 Add , Voice Add
			//Fill Balance
#ifdef MAKE_CASH_CR
			if (Cash_CR_Buf.CashType == 0x01 && CRType == CR_TYPE_CASH)
			{
				if (CharToIntCopy(TRINP.TR_CASH_TOTAL) >= CharToIntCopy(TRINP.TR_TRANS_AMOUNT))
				{
					Tmp_Balance = CharToIntCopy(TRINP.TR_CASH_TOTAL) - CharToIntCopy(TRINP.TR_TRANS_AMOUNT);
					IntToCharCopy(Tmp_Balance,TmpBalance);
				}
				else
				{
					Tmp_Balance = 0;
					IntToCharCopy(Tmp_Balance,TmpBalance);
				}
				
				memcpy( &PumpVoiceBuf.VoiceData[8], TmpBalance, sizeof( TmpBalance ));
			}
#endif
			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;

		break;

		case VOICE_NOZZLE_ERR:

			// Nozzle On But Card Out, Maybe Issued By Pump Process
			// Clear The Bit
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_NOZZLE_ERR );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = IdleNozzleOnVoice;

			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;

		break;

		case VOICE_EXC:

			// IC Card EOT But Card Out, Such As Grey Trans
			// Clear The Bit
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_EXC );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = ExceptionVoice;

			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;

		break;
		
		//Douzhq 20140916 Add , Voice Add
		case VOICE_ICPWD:
			// Please Input IC Card PassWord
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_ICPWD );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = InputICCardPwdVoice;

			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;
		break;
		
		case VOICE_CARDERR:
			// Card Expired Voice
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_CARDERR );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = CardErrVoice;

			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;
		break;
		
		case VOICE_SCANCODE:
			// Please Sacn Code Voice
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_SCANCODE );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = ScanCodeVoice;

			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;
		break;
			
		case VOICE_VERERR:
			// Verification Code Error Voice
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_VERERR );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = VerifiCodeErrVoice;

			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;
		break;
		
		case VOICE_VEROVER:
			// Verification Code Out Of Date Voice
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_VEROVER );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = VerifiCodeOverdutVoice;

			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;
		break;
		
		case VOICE_OILCHOOSE:
			// Oil Choose Error Voice
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_OILCHOOSE );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = OilChooseErrVoice;

			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;
		break;
		
		case VOICE_FAULTERR:
			// Fault Error Voice
			ProcExchange[PROCESS_CR][PROCESS_VOICE] &= ( ~VOICE_FAULTERR );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = FaultErrorVoice;

			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOICE_PUMP;
		break;
			
		default:

			// No Actions

		break;
	}

	// Check The Keypad Voice Actions
	switch( ProcExchange[PROCESS_KEYPAD][PROCESS_VOICE] & KeyPad_VoiceAction )
	{
		case VOICE_VOLUME_UP:

			// Volume Set Up
			// Clear The Bit
			ProcExchange[PROCESS_KEYPAD][PROCESS_VOICE] &= ( ~VOICE_VOLUME_UP );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = VolumeUp;

			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOLUME_PUMP;

		break;

		case VOICE_VOLUME_DOWN:

			// Volume Set Down
			// Clear The Bit
			ProcExchange[PROCESS_KEYPAD][PROCESS_VOICE] &= ( ~VOICE_VOLUME_DOWN );

			memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));

			PumpVoiceBuf.Voice = VolumeDown;

			// Inform Pump
			ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= VOLUME_PUMP;

		break;

		default:

			// No Actions

		break;
	}
}

