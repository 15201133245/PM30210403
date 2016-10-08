
#ifndef _CRPRINT_H
#define _CRPRINT_H


#define PrintRemote				0x00					// Printer Is Remote
#define PrintLocal				0x01					// Printer Is Local


// Define The Bits Used To Illustrate Wether The Receipt
// Will Be Print, And Combine The Bytes PrintIt Defined In PdProtect.h
#define TransNonCardReceipt			0x00000001			// Print The Offline Trans Receipt
#define LogOnReceipt				0x00000002			// Print The Log On Trans Receipt
#define LogOffReceipt				0x00000004			// Print The Log Off Trans Receipt
#define PriceRecvReceipt			0x00000008			// Print The Price Receiving Trans Receipt

// Define The Customer Card Receipt Print
#define CustomTransOkReceipt		0x00000010			// Print The Customer Card Trans Ok Receipt
#define CustomTransGreyReceipt		0x00000020			// Print The Customer Card Trans Grey Receipt
#define CustomTransErrorReceipt		0x00000040			// Print The Customer Card Trans Error Receipt
#define CustomGreyUnlockReceipt		0x00000080			// Print The Customer Card Trans Grey Unlock Receipt
#define CustomGreyFlagReceipt		0x00000100			// Print The Customer Card Trans Clear Grey Flag Receipt
#define CustomCardErrorReceipt		0x00000200			// Print The Customer Card Trans Card Error Receipt

// Define The Employee Card Receipt Print
#define EMPTransOkReceipt			0x00000400			// Print The Employee Card Trans Ok Receipt
#define EMPTransGreyReceipt			0x00000800			// Print The Employee Card Trans Grey Receipt
#define EMPTransErrorReceipt		0x00001000			// Print The Employee Card Trans Error Receipt
#define EMPGreyUnlockReceipt		0x00002000			// Print The Employee Card Trans Grey Unlock Receipt
#define EMPGreyFlagReceipt			0x00004000			// Print The Employee Card Trans Clear Grey Flag Receipt
#define EMPCardErrorReceipt			0x00008000			// Print The Employee Card Trans Card Error Receipt

// Define The Test Card Receipt Print
#define TestTransOkReceipt			0x00010000			// Print The Test Card Trans Ok Receipt
#define TestTransGreyReceipt		0x00020000			// Print The Test Card Trans Grey Receipt
#define TestTransErrorReceipt		0x00040000			// Print The Test Card Trans Error Receipt
#define TestGreyUnlockReceipt		0x00080000			// Print The Test Card Trans Grey Unlock Receipt
#define TestGreyFlagReceipt			0x00100000			// Print The Test Card Trans Clear Grey Flag Receipt
#define TestCardErrorReceipt		0x00200000			// Print The Test Card Trans Card Error Receipt

// Define The Maintainance Card Receipt Print
#define MatnTransOkReceipt			0x00400000			// Print The Maintainance Card Trans Ok Receipt
#define MatnTransGreyReceipt		0x00800000			// Print The Maintainance Card Trans Grey Receipt
#define MatnTransErrorReceipt		0x01000000			// Print The Maintainance Card Trans Error Receipt
#define MatnGreyUnlockReceipt		0x02000000			// Print The Maintainance Card Trans Grey Unlock Receipt
#define MatnGreyFlagReceipt			0x04000000			// Print The Maintainance Card Trans Clear Grey Flag Receipt
#define MatnCardErrorReceipt		0x08000000			// Print The Maintainance Card Trans Card Error Receipt

// Define The Non Used Bits In Print It
#define PrintItRFU0					0x10000000			// Bit0 For RFU
#define PrintItRFU1					0x20000000			// Bit1 For RFU
#define PrintItRFU2					0x40000000			// Bit2 For RFU
#define PrintItSet					0x80000000			// If This Bit Cleared, Means The Print Set Bit Is Set.
														// If Set, Means The Print Bits Should Be Default
// Define The Non Used Bits In Print It	End




extern void PrintProcess( void );

extern void InitPrintProcess( void );

extern unsigned char FillPrintBuffer( unsigned char ReceiptType, unsigned char *pSID, unsigned char *pRecord );

extern unsigned char CheckPrintIt( unsigned char CardType, unsigned char TransType );

#ifdef MAKE_CASH_CR
extern unsigned char  CheckPrintIt_Cash( unsigned char ReceiptType, unsigned char *pRecord );
#endif

#endif