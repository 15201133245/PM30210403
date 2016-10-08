

#include "global.h"

#include "define.h"
#include "CRMemCopy.h"
#include "CRVaribles.h"
#include "PdProtect.h"
#include "Screen.h"

#include "CRPrint.h"

/*
 ** PrintProcess
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Print\Source\CRPrint.c
 *
 *  PARAMETERS:	None.
 *
 *  DESCRIPTION: Print The Receipt, Entrance For Print Receipt Process
 *
 *  RETURNS: None.
 *
 */
void PrintProcess( void )
{
	// Check Wether A Print Request From Pump
	if( ProcExchange[PROCESS_PUMP][PROCESS_PRINT] & SENDPRINT )
	{
		// There Is A Pump Print Request
//		if( !( Rs485Adr & 0x02 ))
		if( PowerDown.LocalPrinter == PrintLocal )
		{
			// Local Has Printer
//			FillPrintBuffer( PumpPrinterBuf.PrinterData[4], &PumpPrinterBuf.PrinterData[0], &PumpPrinterBuf.PrinterData[5] );
			AddPrintTask( PumpPrinterBuf.PrinterData );
		}

		// Clear the Bit
		ProcExchange[PROCESS_PUMP][PROCESS_PRINT] &= ( ~SENDPRINT );
	}
	
	print_dispose();
}

/*
 ** InitPrintProcess
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Print\Source\CRPrint.c
 *
 *  PARAMETERS:	None
 *
 *  DESCRIPTION: Initialize The Print Process, Set Printer Is Local Or Remote
 *
 *  RETURNS: None.
 *
 */
void InitPrintProcess( void )
{
	unsigned int SetPrintBit;

//	Check Print Bits Should Be Default Or Not
	SetPrintBit = CharToIntCopy( PowerDown.PrintIt );
	if( SetPrintBit & PrintItSet )
	{
		// Should Be Default For Print Set Bits
		SetPrintBit = 0;
		IntToCharCopy( SetPrintBit, PowerDown.PrintIt );

		// Save To DataFlash
		SaveCRConfigFlash();
	}

	if( PowerDown.LocalPrinter == PrintRemote )
	{
		// Printer Is Set Remote, Remain It
		PowerDown.LocalPrinter = PrintRemote;
	}
	else if( PowerDown.LocalPrinter == PrintLocal )
	{
		// Printer Is Set Local, Remain It
		PowerDown.LocalPrinter = PrintLocal;
	}
	else
	{
//		// Did Not Set The Printer, So Use Default
//		if( Rs485Adr & 0x02 )
//		{
//			// Printer Is Remote, Set It
//			PowerDown.LocalPrinter = PrintRemote;
//		}
//		else
//		{
//			// Printer Is Local, Set It
//			PowerDown.LocalPrinter = PrintLocal;
//		}

		// Did Not Set The Printer, So Default To PrintLocal
		PowerDown.LocalPrinter = PrintLocal;
	}
}


/*
 ** FillPrintBuffer
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Print\Source\CRPrint.c
 *
 *  PARAMETERS:	
 *				ReceiptType: 0:Inquire Print; 1: IC Card Trans End Print
 *				pSID: 0: Same Station ID As Listed In Station Information List;
 *				Other: pSID Is Printed
 *				*pRecord: Pointer To Sinopec Record Start From TTC, Total 95 Bytes
 *
 *
 *  DESCRIPTION: Fill The Receipt Data To Printer Buffer
 *
 *  RETURNS: 0: Fill Ok; 1: Fill Buffer Error
 *
 *  ATTEN: 485Adr: 1 And 3: Local Printer; 2 And 4: Remote Printer; 2 Print On 1; 4 Print On 3
 *
 */
unsigned char FillPrintBuffer( unsigned char ReceiptType, unsigned char *pSID, unsigned char *pRecord )
{
	unsigned char cardtype, transtype, receipttype;
	unsigned char retval;
	unsigned int len;
#ifdef MAKE_CASH_CR
	unsigned char type;
#endif	
	
	unsigned char tmpbuf[128];

	// Check Print The Receipt Or Not
	cardtype = *( pRecord + 12 + 2 );
	transtype = *( pRecord + 4 );

	receipttype =  ReceiptType;

	if( receipttype == TRANS_RECEIPT )
	{
		// Only Check Printer It For Trans Receipt
		// For Cash Receipt And Inquiry Receipt, Print It
		if( CheckPrintIt( cardtype, transtype ) == 0 )
		{
			// Do Not Print The Receipt, Return
			return 0 ;
		}
	}
#ifdef MAKE_CASH_CR
	else if( receipttype == CASH_ACCEPTOR_RECEIPT )
	{
		// For Cash Trans Only Print The Normal EOT And Grey Trans
		if( CheckPrintIt_Cash( CASH_ACCEPTOR_RECEIPT, pRecord ) == 1 )
		{
			// Check From Keypad Select Or Not
			if( KeypadPrint == KEYPAD_PRINT_IT )
			{
				// Print It, And Do Not Entre Again
				KeypadPrint	= KEYPAD_PRINT_OVER;
			}
			else if( KeypadPrint == KEYPAD_PRINT_OVER )
			{
				// Print It Over, Just Retrun
				return 0;
			}
			else
			{
//				// Keypad Select Print
//				NextScreenID = SCR_SELECTPRINT;

				// Do Not Print It
				return 0;
			}
		}
		else
		{
			// Balance Not Zero, Print It
			// And Do Not Entre Again
			KeypadPrint	= KEYPAD_PRINT_OVER;
		}
	}
	else if( receipttype == CASH_POS_RECEIPT )
	{
		// For Cash Trans Only Print The Normal EOT And Grey Trans
		if( CheckPrintIt_Cash( CASH_POS_RECEIPT, pRecord ) == 1 )
		{
			// Check From Keypad Select Or Not
			if( KeypadPrint == KEYPAD_PRINT_IT )
			{
				// Print It, And Do Not Entre Again
				KeypadPrint	= KEYPAD_PRINT_OVER;
			}
			else if( KeypadPrint == KEYPAD_PRINT_OVER )
			{
				// Print It Over, Just Retrun
				return 0;
			}
			else
			{
//				// Keypad Select Print
//				NextScreenID = SCR_SELECTPRINT;

				// Do Not Print It
				return 0;
			}
		}
		else
		{
			// Balance Not Zero, Print It
			// And Do Not Entre Again
			KeypadPrint	= KEYPAD_PRINT_OVER;
		}
	}
#endif
	else if( receipttype == INQUIRY_RECEIPT )
	{
#ifdef MAKE_CASH_CR
		// For Inquiry Receipt, Check Cash Inquiry Or IC Card Inquiry
		type = CheckPrintIt_Cash( INQUIRY_RECEIPT, pRecord );

		if( type == 2 )
		{
			// Cash Acceptor Receipt
			receipttype = INQUIRY_CASH_ACCEPTOR_RECEIPT;
		}
		else if( type == 3 )
		{
			// Cash POS Receipt
			receipttype = INQUIRY_CASH_POS_RECEIPT;
		}
		else
#endif
		{
			// IC Card Receipt
			receipttype = INQUIRY_RECEIPT;
		}
	}
	//Douzhq 20140921 Add , Config Printer Set
	else if (receipttype == SHORT_RECEIPT)
	{
		if( CheckPrintIt( cardtype, transtype ) == 0 )
		{
			// Do Not Print The Receipt, Return
			return 0 ;
		}
	}
	else
	{
	}

	// Clear The Buffer
	memset( tmpbuf, 0x00, sizeof( tmpbuf ));
	
	// Get SID
	len = CharToIntCopy( pSID );
	if( len == 0 )
	{
		len = GetS_ID();
//		IntToCharCopy( len, tmpbuf );
		memcpy( tmpbuf, &len, 4 );
	}
	else
	{
		memcpy( tmpbuf, pSID, 4 );
	}

	// Get Receipt Type
	tmpbuf[4] = receipttype;

	// Attach RS485Adr
	tmpbuf[4] |= (( Rs485Adr + 1 ) << 4 );
	
	// Get The Receipt Data
	memcpy( &tmpbuf[5], pRecord, 95 );

	// Check Local Print Or Print On Another CR
//	if( !( Rs485Adr & 0x02 ))
	if( PowerDown.LocalPrinter == PrintLocal )
	{
		// Local Printer
		// Save To Print Buffer
		retval = AddPrintTask( tmpbuf );
	}
	else
	{
		// Remote Printer
		memset( &PumpPrinterBuf, 0x00, sizeof( PumpPrinterBuf ));
		memcpy( PumpPrinterBuf.PrinterData, tmpbuf, sizeof( PumpPrinterBuf.PrinterData ));
		
		// Inform Pump Process
		ProcExchange[PROCESS_PRINT][PROCESS_PUMP] |= SENDPRINT;
	}

	return retval;
}


/*
 ** CheckPrintIt
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Print\Source\CRPrint.c
 *
 *  PARAMETERS:	CardType: IC Card Type; TransType: Transactio Type
 *
 *  DESCRIPTION: Check Wether The Receipt Should Be Printed, According To PrintIt Defined In PdProtect.h
 *
 *  RETURNS: 0: Do Not Print The Receipt; Other Print The Receipt
 *
 */
unsigned char CheckPrintIt( unsigned char CardType, unsigned char TransType )
{
	unsigned char retval;
	unsigned int PrintBit, SetPrintBit;

	// Check The Print Bit
	// Default Do Not Print
	retval = 1;
	PrintBit = 0;

	if(( CardType & 0x0F ) == CUSTOMER_CARD )
	{
		// Customer Card Trans
		if(( TransType & 0x0F ) == TRANSOK )
		{
			PrintBit = CustomTransOkReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSGREY )
		{
			PrintBit = CustomTransGreyReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSERROR )
		{
			PrintBit = CustomTransErrorReceipt;
		}
		else if(( TransType & 0x0F ) == TRANGREYUNLOCK )
		{
			PrintBit = CustomGreyUnlockReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSGREYFLAG )
		{
			PrintBit = CustomGreyFlagReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSCARDERR )
		{
			PrintBit = CustomCardErrorReceipt;
		}
		else
		{
			// Error, Do Not Print
		}
	}
	else if(( CardType & 0x0F ) == EMPLOYEE_CARD )
	{
		// Employee Card Trans
		if(( TransType & 0x0F ) == TRANSOK )
		{
			PrintBit = EMPTransOkReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSGREY )
		{
			PrintBit = EMPTransGreyReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSERROR )
		{
			PrintBit = EMPTransErrorReceipt;
		}
		else if(( TransType & 0x0F ) == TRANGREYUNLOCK )
		{
			PrintBit = EMPGreyUnlockReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSGREYFLAG )
		{
			PrintBit = EMPGreyFlagReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSCARDERR )
		{
			PrintBit = EMPCardErrorReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSLOGON )
		{
			PrintBit = LogOnReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSLOGOFF )
		{
			PrintBit = LogOffReceipt;
		}
		else
		{
			// Error, Do Not Print
		}
	}
	else if(( CardType & 0x0F ) == TEST_CARD )
	{
		// Test Card Trans
		if(( TransType & 0x0F ) == TRANSOK )
		{
			PrintBit = TestTransOkReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSGREY )
		{
			PrintBit = TestTransGreyReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSERROR )
		{
			PrintBit = TestTransErrorReceipt;
		}
		else if(( TransType & 0x0F ) == TRANGREYUNLOCK )
		{
			PrintBit = TestGreyUnlockReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSGREYFLAG )
		{
			PrintBit = TestGreyFlagReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSCARDERR )
		{
			PrintBit = TestCardErrorReceipt;
		}
		else
		{
			// Error, Do Not Print
		}
	}
	else if(( CardType & 0x0F ) == MAINTAINANCE_CARD )
	{
		// Maintainance Card Trans
		if(( TransType & 0x0F ) == TRANSOK )
		{
			PrintBit = MatnTransOkReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSGREY )
		{
			PrintBit = MatnTransGreyReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSERROR )
		{
			PrintBit = MatnTransErrorReceipt;
		}
		else if(( TransType & 0x0F ) == TRANGREYUNLOCK )
		{
			PrintBit = MatnGreyUnlockReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSGREYFLAG )
		{
			PrintBit = MatnGreyFlagReceipt;
		}
		else if(( TransType & 0x0F ) == TRANSCARDERR )
		{
			PrintBit = MatnCardErrorReceipt;
		}
		else
		{
			// Error, Do Not Print
		}
	}
	else
	{
		// Public Trans, Offline Trans And Price Receiving Trans
		// The Log On And Log Off Trans Was Set In EMPLOYEE_CARD
		// Customer Card Trans
		if(( TransType & 0x0F ) == TRANSNONCARD )
		{
			PrintBit = TransNonCardReceipt;
		}
		else if(( TransType & 0x0F ) == TRANPRICERECV )
		{
			PrintBit = PriceRecvReceipt;
		}
		else
		{
			// Error, Do Not Print
		}
	}

	SetPrintBit = CharToIntCopy( PowerDown.PrintIt );

	if( SetPrintBit & PrintBit )
	{
		// Print The Receipt
		retval = 1;
	}
	else
	{
		// Do Not Print The Receipt
		retval = 0;
	}

	return retval;	
}



#ifdef MAKE_CASH_CR
/*
 ** CheckPrintIt_Cash
 *
 *  FILENAME: D:\WORK\SinoPecARM\Software\ARM_SNPC\Application\Print\Source\CRPrint.c
 *
 *  PARAMETERS:	ReceiptType: Cash Receipt Or Inquiry Receipt
 *				*pRecord: Pointer To The Record To Be Printed
 *
 *  DESCRIPTION: Wether Print The Cash Trans Record
 *
 *  RETURNS: For Cash Receipt: 0: Print It; 1: Do Not Print It
 *			 For Inquiry Receipt: 2: Cash Acceptor Trans; 3: Cash POS Trans Trans; 4: IC Card Trans
 *
 */
unsigned char  CheckPrintIt_Cash( unsigned char ReceiptType, unsigned char *pRecord )
{
	unsigned char retval;
	unsigned char transtype;
	unsigned char receipttype;
	unsigned int value0, value1;

	JOURNAL buf;
	unsigned char amount[4];

	memcpy( buf.TRANS_TTC, pRecord, sizeof( JOURNAL ) - 1 );

	// Get Receipt
	receipttype = ReceiptType;

	// Get Trans Type
	transtype = buf.TRANS_TYPE;
	transtype &= 0x0F;

	// Default Not Print
	retval = 1;
	
	if(( transtype == TRANSOK ) || ( transtype == TRANSGREY ))
	{
		// For Cash Trans Only Print The Normal EOT And Grey Trans
		// Get Trans Amount
		memset( amount, 0x00, sizeof( amount ));
		memcpy( &amount[1], buf.TRANS_AMOUNT, sizeof( amount ) - 1);
		value0 = CharToIntCopy( amount );

		// Get Cash Total Accepted(Authed)
		memset( amount, 0x00, sizeof( amount ));
		memcpy( amount, buf.RFU, sizeof( amount ));
		value1 = CharToIntCopy( amount );

		if( receipttype == INQUIRY_RECEIPT )
		{
			if( value1 != 0 )
			{
				// Cash Trans
				if( CheckCashType( buf.RFU ) == 0 )
				{
					// Cash Acceptor Trans
					retval = 2;
				}
				else
				{
					// Cash POS Trans
					retval = 3;
				}
			}
			else
			{
				// IC Card Trans
				retval = 4;
			}
		}
		else if(( receipttype == CASH_ACCEPTOR_RECEIPT ) || ( receipttype == CASH_POS_RECEIPT ))
		{
			// Check value0 and value1
			if( value1 > value0 )
			{
				// Print It
				retval = 0;
			}
			else
			{
				// Do Not Print It
				retval = 1;
			}
		}
		else
		{
		}
	}	

	return retval;
}
#endif
