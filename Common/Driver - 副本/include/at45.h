#ifndef	_AT45_H
#define	_AT45_H
#include "spi.h"

#define AT45_ERROR_LOCK         1
#define AT45_ERROR_SPI          2
#define OVER_STACK				(-0xff)

/// Main memory page read command code.
#define AT45_PAGE_READ              0xD2
/// Continous array read (legacy) command code.
#define AT45_CONTINUOUS_READ_LEG    0xE8
/// Continous array read (low frequency) command code.
#define AT45_CONTINUOUS_READ_LF     0x03
/// Continous array read command code.
#define AT45_CONTINUOUS_READ        0x0B
/// Buffer 1 read (low frequency) command code.
#define AT45_BUF1_READ_LF           0xD1
/// Buffer 2 read (low frequency) command code.
#define AT45_BUF2_READ_LF           0xD3
/// Buffer 1 read (serial) command code.
#define AT45_BUF1_READ_SER          0xD4
/// Buffer 2 read (serial) command code.
#define AT45_BUF2_READ_SER          0xD6
/// Buffer 1 read (8-bit) command code.
#define AT45_BUF1_READ_8B           0x54			//
/// Buffer 2 read (8-bit) command code.
#define AT45_BUF2_READ_8B           0x56			//

/// Buffer 1 write command code.
#define AT45_BUF1_WRITE             0x84
/// Buffer 2 write command code.
#define AT45_BUF2_WRITE             0x87
/// Buffer 1 to main memory page program with erase command code.
#define AT45_BUF1_MEM_ERASE         0x83
/// Buffer 2 to main memory page program with erase command code.
#define AT45_BUF2_MEM_ERASE         0x86
/// Buffer 1 to main memory page program without erase command code.
#define AT45_BUF1_MEM_NOERASE       0x88
/// Buffer 2 to main memory page program without erase command code.
#define AT45_BUF2_MEM_NOERASE       0x89
/// Page erase command code.
#define AT45_PAGE_ERASE             0x81
/// Block erase command code.
#define AT45_BLOCK_ERASE            0x50
/// Sector erase command code.
#define AT45_SECTOR_ERASE           0x7C
/// Chip erase command code.
#define AT45_CHIP_ERASE             0xC7, 0x94, 0x80, 0x9A
/// Main memory page program through buffer 1 command code.
#define AT45_PAGE_WRITE_BUF1        0x82
/// Main memory page program through buffer 2 command code.
#define AT45_PAGE_WRITE_BUF2        0x85

/// Main memory page to buffer 1 transfer command code.
#define AT45_PAGE_BUF1_TX           0x53
/// Main memory page to buffer 2 transfer command code.
#define AT45_PAGE_BUF2_TX           0x55
/// Main memory page to buffer 1 compare command code.
#define AT45_PAGE_BUF1_CMP          0x60
/// Main memory page to buffer 2 compare command code.
#define AT45_PAGE_BUF2_CMP          0x61
/// Auto page rewrite through buffer 1 command code.
#define AT45_AUTO_REWRITE_BUF1      0x58
/// Auto page rewrite through buffer 2 command code.
#define AT45_AUTO_REWRITE_BUF2      0x59
/// Deep power-down command code.
#define AT45_DEEP_PDOWN             0xB9
/// Resume from deep power-down command code.
#define AT45_RES_DEEP_PDOWN         0xAB
/// Status register read command code.
#define AT45_STATUS_READ            0xD7
/// Manufacturer and device ID read command code.
#define AT45_ID_READ                0x9F

#define AT45_PageSize(pAt45)   		((pAt45)->pDesc->pageSize)
#define AT45_PageOffset(pAt45) 		((pAt45)->pDesc->pageOffset)
#define AT45_PageNumber(pAt45) 		((pAt45)->pDesc->pageNumber)
#define AT45_BlockNumber(pAt45)		(((pAt45)->pDesc->pageNumber) >> 3)

#define AT45_STATUS_READY(status)       (status & 0x80)
#define AT45_STATUS_ID(status)          (status & 0x3c)
#define AT45_STATUS_COMPARE(states)		(status & 0x40)

typedef void (*pFunc)(unsigned char );

typedef struct
{
		/// dataflash page number.
		unsigned int pageNumber;
		/// dataflash page size.
		unsigned int pageSize;
		/// page offset in command.
		unsigned int pageOffset;
		/// Dataflash ID.
		unsigned char id;
		/// Identifier.
		const char *name;
}At45Desc;

typedef struct _Dataflash
{
		/// Pointer to Spi Structure (SPI low level driver).
		Spid *pSpid;
		/// Current SPI command sent to the SPI low level driver.
		SpidCmd command;
		/// Pointer to the dataflash description.
		const At45Desc *pDesc;
		/// Buffer to store the current command (opcode + dataflash address.
		unsigned char pCmdBuffer[8];
}At45;

unsigned char AT45_Configure(At45 *pAt45, Spid *pSpid, unsigned char spiCs);
    
    unsigned char AT45_SendCommand(
        At45 *pAt45,
        unsigned char cmd,
        unsigned char cmdSize,
        unsigned char *pData,
        unsigned int dataSize,
        unsigned int address,
        SpidCallback callback,
        void *pArgument);
    
    unsigned char AT45_GetStatus(At45 *pAt45);
    
    unsigned char AT45_IsBusy(At45 *pAt45);
    
    const At45Desc * AT45_FindDevice(At45 *pAt45, unsigned char status);
    
    void AT45_WaitReady(At45 *pAt45);
    
    unsigned char AT45_GetCompare(At45 *pAt45);
    
    unsigned char AT45_EraseBlock(At45 *pAt45,unsigned int block);
    
    unsigned char AT45_PageToBuf1Tx(At45 *pAt45,unsigned int page);
    
    unsigned char AT45_PageToBuf2Tx(At45 *pAt45,unsigned int page);
    
    unsigned char AT45_AutoRewriteBuf1(At45 *pAt45,unsigned int page);
    
    unsigned char AT45_AutoRewriteBuf2(At45 *pAt45,unsigned int page);
    
    unsigned char AT45_Buf1ComparePage(At45 *pAt45,unsigned int address);
    
    unsigned char AT45_Buf2ComparePage(At45 *pAt45,unsigned int address);
    
    unsigned char AT45_ErasePage(At45 *pAt45,unsigned int address);
    
    unsigned char AT45_PageWriteBuf1(At45 *pAt45,unsigned char *pBuffer,unsigned int page);
    
    unsigned char AT45_Buf1MemErase(At45 *pAt45,unsigned int page);
    
    unsigned char AT45_Buf1Write(At45 *pAt45,unsigned char *pBuffer);
    
    unsigned char AT45_PageWriteBuf2(At45 *pAt45,unsigned char *pBuffer,unsigned int page);
    
    unsigned char AT45_Buf2MemErase(At45 *pAt45,unsigned int page);
    
    unsigned char AT45_Buf2Write(At45 *pAt45,unsigned char *pBuffer);
    
    unsigned char AT45_Buf1MemNoErase(At45 *pAt45,unsigned int page);
    
    unsigned char AT45_Buf2MemNoErase(At45 *pAt45,unsigned int page);
    
    unsigned char AT45_PageRead(At45 *pAt45,unsigned char *pBuffer,unsigned int page);
    
    unsigned char AT45_Buf1Read(At45 *pAt45,unsigned char *pBuffer);
    
    unsigned char AT45_Buf2Read(At45 *pAt45,unsigned char *pBuffer);
    
    int AT45_Read(At45 *pAt45,unsigned char *pBuffer,
    							unsigned int address,unsigned int len);
    
    
    int AT45_Write(At45 *pAt45,unsigned char *pBuffer,
    			   unsigned int address,unsigned int len);

#endif