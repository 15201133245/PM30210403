
#ifndef _VOICE_H
#define _VOICE_H


//#define	VolumeUp				0x01			// Volume Up
//#define VolumeDown				0x02			// Volume Down
//
//#define CardInVoice				0x01			// IC Card In Voice
//#define NozzleOnVoice			0x02			// Nozzle On Voice
//#define BusyVoice				0x03			// Busy Voice
//#define EOTVoice				0x04			// EOT Voice
//#define ExceptionVoice			0xF0			// EOT But Card Out Voice

enum SETVOLUME
{
	SetVolumeStart,

	VolumeDown,									// Volume Down
	VolumeUp,									// Volume Up

	SetVolumeEnd
};

enum VOICEACTION
{
	VoiceStart,

	CardInVoice,								// Card In Voice
	NozzleOnVoice,								// Nozzle On Voice
	BusyVoice,									// Busy Voice
	EOTVoice,									// EOT Voice
	IdleNozzleOnVoice,							// Nozzle On But IC Card Is Not Inserted Voice
	ExceptionVoice,								// Exception Voice, Such EOT But Card Out
	
	//Douzhq 20140916 Add , Voice Add
	InputICCardPwdVoice,						// Please Input IC Card Password Voice
	TakeNozzleOnVoice,							// Take Nozzle On Voice
	
	CardErrVoice,								// Card Expired Voice
	ScanCodeVoice,								// Please Sacn Code Voice
	VerifiCodeErrVoice,							// Verification Code Error Voice
	VerifiCodeOverdutVoice,						// Verification Code Out Of Date Voice
	OilChooseErrVoice,							// Oil Choose Error Voice
	FaultErrorVoice,							// Fault Error Voice
	
	VoiceEnd
};



extern void InitVoiceProcess( void );

extern void VoiceProcess( void );

#endif