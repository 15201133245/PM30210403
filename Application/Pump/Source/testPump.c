
#include "define.h"
#include "global.h"
#include "tc.h"



//#include "Des.h"


unsigned int fuellingamount, fuellingvolume;


unsigned char pumpdelayed = 0;
void pumpdelaytimer( void )
{
	pumpdelayed = 1;
}

unsigned char pumpdelayed1 = 0;
void pumpdelaytimer1( void )
{
	pumpdelayed1 = 1;
}

unsigned int pumpdelayed0 = 0;
unsigned char pumpdelayed0sign = 0;
void pumpdelaytimer0( void )
{
	pumpdelayed0 = 1;
}

unsigned int pumptotal = 0;
void pumptotaltimer( void )
{
	pumptotal = 1;
}

unsigned int pumpofflinetotal = 0;
unsigned int pumpofflinetotalsign = 0;
void pumpofflinetotaltimer( void )
{
	pumpofflinetotal = 1;
}

// Delay Time Timer
Tmr * testpump1;

unsigned char pump_power_on	= 0;



// Define The Pump ID And Pump Key

#ifdef MAKE_PSAM_VERIFY

// PumpID = 3032000000000001; Key = 879CAE60E76D8F70C26FDA8DD2557C5F
// PumpID = 3032000000000002; Key = B0CC611E1D8625842656B40B18B7FF6E
// PumpID = 3032000000000003; Key = 18BDED409C2B10ADD672FDDA6259928A
// PumpID = 3032000000000012; Key = FD0A3BBEEA3C28690D11F1B5D26F88CB
// PumpID = 3032000000000123; Key = 1B8E34A06751BADCA89BC99B96727995
// PumpID = 3032000000001234; Key = 3443D1D8555E652A4705343AB6740D45
// PumpID = 3032000000012345; Key = 7FE337D39DA2C1D466CFE0007BDE5653
// PumpID = 3032000000999999; Key = 78CA397F2A9989638B2CB2CD3FB017AF
// PumpID = 3032000009999999; Key = 73A6CEDCFCE56429FB88E3677614C243
// PumpID = 3032999999999999; Key = 375BDB18452E2A925D9D80667897C9A6


const char PumpID_0[8] = { 0x30, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
const char PumpKey_0[16] = { 0x87, 0x9C, 0xAE, 0x60, 0xE7, 0x6D, 0x8F, 0x70, 0xC2, 0x6F, 0xDA, 0x8D, 0xD2, 0x55, 0x7C, 0x5F };

const char PumpID_1[8] = { 0x30, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 };
const char PumpKey_1[16] = { 0xB0, 0xCC, 0x61, 0x1E, 0x1D, 0x86, 0x25, 0x84, 0x26, 0x56, 0xB4, 0x0B, 0x18, 0xB7, 0xFF, 0x6E };

const char PumpID_2[8] = { 0x30, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03 };
const char PumpKey_2[16] = { 0x18, 0xBD, 0xED, 0x40, 0x9C, 0x2B, 0x10, 0xAD, 0xD6, 0x72, 0xFD, 0xDA, 0x62, 0x59, 0x92, 0x8A };

const char PumpID_3[8] = { 0x30, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12 };
const char PumpKey_3[16] = { 0xFD, 0x0A, 0x3B, 0xBE, 0xEA, 0x3C, 0x28, 0x69, 0x0D, 0x11, 0xF1, 0xB5, 0xD2, 0x6F, 0x88, 0xCB };

const char PumpID_4[8] = { 0x30, 0x32, 0x00, 0x00, 0x00, 0x00, 0x01, 0x23 };
const char PumpKey_4[16] = { 0x1B, 0x8E, 0x34, 0xA0, 0x67, 0x51, 0xBA, 0xDC, 0xA8, 0x9B, 0xC9, 0x9B, 0x96, 0x72, 0x79, 0x95 };

const char PumpID_5[8] = { 0x30, 0x32, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34 };
const char PumpKey_5[16] = { 0x34, 0x43, 0xD1, 0xD8, 0x55, 0x5E, 0x65, 0x2A, 0x47, 0x05, 0x34, 0x3A, 0xB6, 0x74, 0x0D, 0x45 };

const char PumpID_6[8] = { 0x30, 0x32, 0x00, 0x00, 0x00, 0x01, 0x23, 0x45 };
const char PumpKey_6[16] = { 0x7F, 0xE3, 0x37, 0xD3, 0x9D, 0xA2, 0xC1, 0xD4, 0x66, 0xCF, 0xE0, 0x00, 0x7B, 0xDE, 0x56, 0x53 };

const char PumpID_7[8] = { 0x30, 0x32, 0x00, 0x00, 0x00, 0x99, 0x99, 0x99 };
const char PumpKey_7[16] = { 0x78, 0xCA, 0x39, 0x7F, 0x2A, 0x99, 0x89, 0x63, 0x8B, 0x2C, 0xB2, 0xCD, 0x3F, 0xB0, 0x17, 0xAF };

const char PumpID_8[8] = { 0x30, 0x32, 0x00, 0x00, 0x09, 0x99, 0x99, 0x99 };
const char PumpKey_8[16] = { 0x73, 0xA6, 0xCE, 0xDC, 0xFC, 0xE5, 0x64, 0x29, 0xFB, 0x88, 0xE3, 0x67, 0x76, 0x14, 0xC2, 0x43 };

const char PumpID_9[8] = { 0x30, 0x32, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99 };
const char PumpKey_9[16] = { 0x37, 0x5B, 0xDB, 0x18, 0x45, 0x2E, 0x2A, 0x92, 0x5D, 0x9D, 0x80, 0x66, 0x78, 0x97, 0xC9, 0xA6 };






#endif






void testpump( void )
{
#ifdef MAKE_PSAM_VERIFY
	unsigned char buf1[8], buf2[8];
#endif

	if( pump_power_on == 0 )
	{
		// Default Pump Online
		ProcStatus[PROCESS_PUMP] |= PUMP_ONLINE;
		ProcStatus[PROCESS_PUMP] = PUMP_IDLE;
//		ProcStatus[PROCESS_PUMP] = PUMP_CALL_E;
	
		pump_power_on = 1;

#ifdef MAKE_PSAM_VERIFY

		// Fill Pump ID And Pump Key
		memcpy( Pump_CR_Buf.PUMPSerialNumber, PumpID_4, sizeof( Pump_CR_Buf.PUMPSerialNumber ));
		memcpy( Pump_CR_Buf.PumpKey, PumpKey_4, sizeof( Pump_CR_Buf.PumpKey ));
#endif
	
	}

	if(( ProcStatus[PROCESS_CR] & CARD_READY ) && ( ProcStatus[PROCESS_CR] & CARD_LOGON ))
	{
		if( pumpdelayed0sign == 0 )
		{
			if( !( ProcStatus[PROCESS_CR] & CARD_BUSY ))
			{
				// Start Counter For Pump Call
			   	CreateTimer( TIMER_ONE_DELETE, 6000, pumpdelaytimer );
//			   	CreateTimer( TIMER_ONE_DELETE, 9000, pumpdelaytimer );
//			   	CreateTimer( TIMER_ONE_DELETE, 4000, pumpdelaytimer );
		
				pumpdelayed0sign = 1;
			}
		}
	}
	else
	{
		if( pumpdelayed0sign == 1 )
		{
			if( pumpdelayed == 1 )
			{
				pumpdelayed0sign = 0;
				pumpdelayed = 0;
			}
		}
	}
	
	
	// pump Call
	if( pumpdelayed == 1 )
	{
		// Set Spring Card
		ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_CALL;

		Pump_CR_Buf.NozzleNumber = 1;
		memcpy( Pump_CR_Buf.Price, "\x09\x00", 2 );
		memcpy( Pump_CR_Buf.MaxFuellingVolume, "\x0F\x27\x00\x00", 4 );
		
		// Start Counter For Pump EOT
   		testpump1 = CreateTimer( TIMER_ONE_DELETE, 10000, pumpdelaytimer1 );
//   		testpump1 = CreateTimer( TIMER_ONE_DELETE, 4000, pumpdelaytimer1 );
		
		fuellingamount = 0;
		fuellingvolume = 0;

		pumpdelayed = 0;
	}

	// Pump Call Over

	// Pump Busy
	memcpy( Pump_CR_Buf.Amount, &fuellingamount, 4 );
	memcpy( Pump_CR_Buf.Volume, &fuellingvolume, 4 );
	fuellingamount++;
	fuellingvolume += 2;

	// Spring Out Card EOT
	if(	ProcExchange[PROCESS_CR][PROCESS_PUMP] & PUMP_STOP )
	{
		ProcExchange[PROCESS_CR][PROCESS_PUMP] &= ( ~PUMP_STOP );
		// Set Spring Card
		ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_EOT;

		DeleteTimer( testpump1 );
	}

	// pump EOT
	if( pumpdelayed1 == 1 )
	{
		if( !( ProcExchange[PROCESS_PUMP][PROCESS_CR] & PUMP_EOT ))
		{
			// Set Spring Card
			ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_EOT;
//			pumpdelayed0sign = 0;
		}
		pumpdelayed1 = 0;
	}

	// Pump EOT Over


	// pump get last trans
	if( ProcExchange[PROCESS_CR][PROCESS_PUMP] & PUMP_LASTTRANS )
	{
		// Start Counter For Pump get last trans
//   		CreateTimer( TIMER_ONE_DELETE, 2000, pumpdelaytimer0 );
   		CreateTimer( TIMER_ONE_DELETE, 1000, pumpdelaytimer0 );
		// Clear The Bit
		ProcExchange[PROCESS_CR][PROCESS_PUMP] &= ( ~PUMP_LASTTRANS );
	}

	if( pumpdelayed0 == 1 )
	{
		// Set get last trans
		ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_LASTTRANS;

		Pump_CR_Buf.NozzleNumber = 1;
		memcpy( Pump_CR_Buf.Volume, "\x99\x09\x00\x00", 4 );
//		memcpy( Pump_CR_Buf.Amount, "\x40\x20\x00\x00", 4 );
		memcpy( Pump_CR_Buf.Amount, "\xAF\x0E\x00\x00", 4 );
		memcpy( Pump_CR_Buf.Price, "\x99\x00", 2 );

		memcpy( &Pump_CR_Buf.VTotal, "\x78\x56\x34\x12", 4 );

		pumpdelayed0 = 0;
			
		pumpdelayed0sign = 0;
	}

	// Pump get last trans Over


	// pump get Totals
	if( ProcExchange[PROCESS_CR][PROCESS_PUMP] & PUMP_TOTALS )
	{
		// Start Counter For Pump get last trans
   		CreateTimer( TIMER_ONE_DELETE, 2000, pumptotaltimer );
		// Clear The Bit
		ProcExchange[PROCESS_CR][PROCESS_PUMP] &= ( ~PUMP_TOTALS );
	}

	if( pumptotal == 1 )
	{
		// Set get last trans
		ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_TOTALS;

		memcpy( &Pump_CR_Buf.VolumeTotal[0], "\x78\x56\x34\x12", 4 );
		memcpy( &Pump_CR_Buf.AmountTotal[0], "\x21\x43\x65\x87", 4 );

		memcpy( &Pump_CR_Buf.VolumeTotal[4], "\x89\x67\x45\x23", 4 );
		memcpy( &Pump_CR_Buf.AmountTotal[4], "\x32\x54\x76\x98", 4 );

		memcpy( &Pump_CR_Buf.VolumeTotal[8], "\x90\x78\x56\x34", 4 );
		memcpy( &Pump_CR_Buf.AmountTotal[8], "\x43\x65\x87\x09", 4 );

		memcpy( &Pump_CR_Buf.VolumeTotal[12], "\x01\x89\x67\x45", 4 );
		memcpy( &Pump_CR_Buf.AmountTotal[12], "\x54\x76\x98\x01", 4 );

		pumptotal = 0;
	}

	// Pump get totals Over


	// Pump Issue Offline Trans
	// Start Counter For Pump Offline Trans
	if( pumpofflinetotalsign == 0 )
	{
//	   	CreateTimer( TIMER_ONE_DELETE, 19000, pumpofflinetotaltimer );
//		
//		pumpofflinetotalsign = 1;
	}
	
	if( pumpofflinetotal == 1 )
	{
		// Issue Offline Totals
		pumpofflinetotal = 0;

		// Set Offline trans Totals
		ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_OFFLINE_TRANS;

		memcpy( &Pump_CR_Buf.VolumeTotal[0], "\x34\x12\x00\x00", 4 );
		memcpy( &Pump_CR_Buf.AmountTotal[0], "\x21\x43\x00\x00", 4 );

		memcpy( &Pump_CR_Buf.VolumeTotal[4], "\x78\x56\x00\x00", 4 );
		memcpy( &Pump_CR_Buf.AmountTotal[4], "\x65\x87\x00\x00", 4 );

		memcpy( &Pump_CR_Buf.VolumeTotal[8], "\x90\x78\x00\x00", 4 );
		memcpy( &Pump_CR_Buf.AmountTotal[8], "\x87\x09\x00\x00", 4 );

		memcpy( &Pump_CR_Buf.VolumeTotal[12], "\x19\x24\x00\x00", 4 );
		memcpy( &Pump_CR_Buf.AmountTotal[12], "\x42\x91\x00\x00", 4 );

		// Set Offline Trans Offline Volume And Amount
//		memcpy( &Pump_CR_Buf.OfflineVolume[0], "\x23\x01\x00\x00", 4 );
//		memcpy( &Pump_CR_Buf.OfflineAmount[0], "\x99\x00\x00\x00", 4 );
		memcpy( &Pump_CR_Buf.OfflineVolume[0], "\x00\x00\x00\x00", 4 );
		memcpy( &Pump_CR_Buf.OfflineAmount[0], "\x99\x00\x00\x00", 4 );

//		memcpy( &Pump_CR_Buf.OfflineVolume[4], "\x90\x09\x00\x00", 4 );
//		memcpy( &Pump_CR_Buf.OfflineAmount[4], "\x90\x19\x00\x00", 4 );
		memcpy( &Pump_CR_Buf.OfflineVolume[4], "\x00\x00\x00\x00", 4 );
		memcpy( &Pump_CR_Buf.OfflineAmount[4], "\x90\x19\x00\x00", 4 );

//		memcpy( &Pump_CR_Buf.OfflineVolume[8], "\x56\x01\x00\x00", 4 );
//		memcpy( &Pump_CR_Buf.OfflineAmount[8], "\x40\x20\x00\x00", 4 );
		memcpy( &Pump_CR_Buf.OfflineVolume[8], "\x00\x00\x00\x00", 4 );
		memcpy( &Pump_CR_Buf.OfflineAmount[8], "\x40\x20\x00\x00", 4 );

//		memcpy( &Pump_CR_Buf.OfflineVolume[12], "\x19\x00\x00\x00", 4 );
//		memcpy( &Pump_CR_Buf.OfflineAmount[12], "\x99\x01\x00\x00", 4 );
		memcpy( &Pump_CR_Buf.OfflineVolume[12], "\x00\x00\x00\x00", 4 );
		memcpy( &Pump_CR_Buf.OfflineAmount[12], "\x99\x01\x00\x00", 4 );
	}
	// Pump Issue Offline Trans	End


	// Pump Update Price
	if( ProcExchange[PROCESS_SCHEDULE][PROCESS_PUMP] & PRICE_UPDATE )
	{
		ProcExchange[PROCESS_SCHEDULE][PROCESS_PUMP] &= ( ~PRICE_UPDATE );

		SetPriceUpdateFlg( 10 );
	}
	// Pump Update Price End



#ifdef MAKE_PSAM_VERIFY
	// Pump Verrify
	if( ProcExchange[PROCESS_CR][PROCESS_PUMP] & PUMP_VERIFY )
	{
		// Need Pump Verify
		// Clear Bit
		ProcExchange[PROCESS_CR][PROCESS_PUMP] &= ( ~PUMP_VERIFY );

		// Calc Random Encrypted
		memset( buf1, 0x00, sizeof( buf1 ));
		memset( buf2, 0x00, sizeof( buf2 ));
		memcpy( buf1, Pump_CR_Buf.PSAMRandom, 4 );

//		TriDes( Pump_CR_Buf.DESRandom, Pump_CR_Buf.PumpKey, buf1, 0 );
		
		// Inform OK
		ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_VERIFY;

	}
#endif






	// Pump Voice Exchange
	if( ProcExchange[PROCESS_VOICE][PROCESS_PUMP] & VOICE_PUMP )
	{
		ProcExchange[PROCESS_VOICE][PROCESS_PUMP] &= ( ~VOICE_PUMP );
	}

	if( ProcExchange[PROCESS_VOICE][PROCESS_PUMP] & VOLUME_PUMP )
	{
		ProcExchange[PROCESS_VOICE][PROCESS_PUMP] &= ( ~VOLUME_PUMP );
	}
}
