/* EasyCODE V8 */
/* EasyCODE ( 0 */
#include "at45.h"
#include "tc.h"
#include <string.h>

/// Number of dataflash which can be recognized.
#define NUMDATAFLASH    (sizeof(at45Devices) / sizeof(At45Desc))
/* EasyCODE ( 0 
Internal variables
at45Devices */
static const At45Desc at45Devices[] = {
    {  512,  264,  9, 0x0C, "AT45DB011B"},
    { 1024,  264,  9, 0x14, "AT45DB021B"},
    { 2048,  264,  9, 0x1C, "AT45DB041B"},
    { 4096,  264,  9, 0x24, "AT45DB081B"},
    { 4096,  528, 10, 0x2C, "AT45DB161B"},
    { 8192,  528, 10, 0x34, "AT45DB321B"},
    { 8192, 1056, 11, 0x3C, "AT45DB642D"},
    {16384, 1056, 11, 0x10, "AT45DB1282"},
    {16384, 2112, 12, 0x18, "AT45DB2562"},
    {32768, 2112, 12, 0x20, "AT45DB5122"}
};

unsigned char AT45_Configure(At45 *pAt45, Spid *pSpid, unsigned char spiCs)
{
    SpidCmd *pCommand;
    
    //Initialize the At45  instance;
    pAt45->pSpid = pSpid;
    pAt45->pDesc = 0;
    memset(pAt45->pCmdBuffer,0,8);
    
    //Iniialize the spidCmd structure
    pCommand = &(pAt45->command);
    pCommand->pCmd = pAt45->pCmdBuffer;
    pCommand->callback = 0;
    pCommand->pArgument = 0;
    pCommand->spiCs = spiCs;
    return 0;
}

unsigned char AT45_SendCommand(
    At45 *pAt45,
    unsigned char cmd,
    unsigned char cmdSize,
    unsigned char *pData,
    unsigned int dataSize,
    unsigned int address,
    SpidCallback callback,
    void *pArgument)
{
    SpidCmd *pCommand;
    const At45Desc *pDesc = pAt45->pDesc;
    unsigned int dfAddress = 0;
    
    // Check if the SPI driver is available
    if ( AT45_IsBusy(pAt45) )
    {
        return AT45_ERROR_LOCK;
    }
    // Compute command pattern
    pAt45->pCmdBuffer[0] = cmd;
    
    // Add address bytes if necessary
    if ( cmdSize > 1 )
    {
        dfAddress =
          ((address / (pDesc->pageSize)) << pDesc->pageOffset)
           + (address % (pDesc->pageSize));
        if ( pDesc->pageNumber >= 16384 )
        {
            pAt45->pCmdBuffer[1] = ((dfAddress & 0x0F000000) >> 24);
            pAt45->pCmdBuffer[2] = ((dfAddress & 0x00FF0000) >> 16);
            pAt45->pCmdBuffer[3] = ((dfAddress & 0x0000FF00) >> 8);
            pAt45->pCmdBuffer[4] = ((dfAddress & 0x000000FF) >> 0);
            if ( (cmd != AT45_CONTINUOUS_READ) && (cmd != AT45_PAGE_READ) )
            {
                cmdSize++;
            }
        }
        else
        {
            pAt45->pCmdBuffer[1] = ((dfAddress & 0x00FF0000) >> 16);
            pAt45->pCmdBuffer[2] = ((dfAddress & 0x0000FF00) >> 8);
            pAt45->pCmdBuffer[3] = ((dfAddress & 0x000000FF) >> 0);
        }
    }
    // Update the SPI Transfer descriptors
    pCommand = &(pAt45->command);
    pCommand->cmdSize = cmdSize;
    pCommand->pData = pData;
    pCommand->dataSize = dataSize;
    pCommand->callback = callback;
    pCommand->pArgument = pArgument;
    
    // Send Command and data through the SPI
    if ( SPID_SendCommand(pAt45->pSpid, pCommand) )
    {
        return AT45_ERROR_SPI;
    }
    return 0;
}

unsigned char AT45_IsBusy(At45 *pAt45)
{
    return SPID_IsBusy(pAt45->pSpid);
}

const At45Desc * AT45_FindDevice(At45 *pAt45, unsigned char status)
{
    unsigned int i;
    unsigned char id = AT45_STATUS_ID(status);
    
    // Check if status is all one; in which case, it is assumed that no device
    // is connected
    if ( status == 0xFF )
    {
        return 0;
    }
    // Look in device array
    i = 0;
    pAt45->pDesc = 0;
    while ( (i < NUMDATAFLASH) && !(pAt45->pDesc) )
    {
        if ( at45Devices[i].id == id )
        {
            pAt45->pDesc = &(at45Devices[i]);
        }
        i++;
    }
    return pAt45->pDesc;
}

void AT45_WaitReady(At45 *pAt45)
{
    unsigned char ready = 0;
    while ( !ready )
    {
        ready = AT45_STATUS_READY(AT45_GetStatus(pAt45));
    }
}

unsigned char AT45_GetCompare(At45 *pAt45)
{
    unsigned char ready = 0;
    unsigned char status = 0;
    while ( !ready )
    {
        status = AT45_GetStatus(pAt45);
        ready = AT45_STATUS_READY(status);
    }
    return AT45_STATUS_COMPARE(status);
}

unsigned char AT45_EraseBlock(At45 *pAt45,unsigned int block)				//0x50
{
    unsigned char error;
    if ( block > AT45_BlockNumber(pAt45) )
    {
        return 1;
    }
    error = AT45_SendCommand(pAt45, AT45_BLOCK_ERASE,4,0,0,
    						 block * AT45_PageNumber(pAt45) * 8,0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_PageToBuf1Tx(At45 *pAt45,unsigned int page)				//0x53
{
    unsigned char error;
    if ( page > AT45_PageNumber(pAt45) )
    {
        return 1;
    }
    error = AT45_SendCommand(pAt45,AT45_PAGE_BUF1_TX,4,0,0,
    						 page * AT45_PageNumber(pAt45),0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_PageToBuf2Tx(At45 *pAt45,unsigned int page)				//0x55
{
    unsigned char error;
    if ( page > AT45_PageNumber(pAt45) )
    {
        return 1;
    }
    error = AT45_SendCommand(pAt45,AT45_PAGE_BUF2_TX,4,0,0,
    						 page * AT45_PageNumber(pAt45),0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_AutoRewriteBuf1(At45 *pAt45,unsigned int page)			//0x58
{
    unsigned char error;
    if ( page > AT45_PageNumber(pAt45) )
    {
        return 1;
    }
    error = AT45_SendCommand(pAt45,AT45_AUTO_REWRITE_BUF1,4,0,0,
    						 page * AT45_PageNumber(pAt45),0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_AutoRewriteBuf2(At45 *pAt45,unsigned int page)			//0x59
{
    unsigned char error;
    if ( page > AT45_PageNumber(pAt45) )
    {
        return 1;
    }
    error = AT45_SendCommand(pAt45,AT45_AUTO_REWRITE_BUF2,4,0,0,
    						 page * AT45_PageNumber(pAt45),0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_Buf1ComparePage(At45 *pAt45,unsigned int address)		//0x60
{
    unsigned char error;
    
    //Issue a page compare buf1 command
    error = AT45_SendCommand(pAt45,AT45_PAGE_BUF1_CMP,4,
    					 0,0,address,0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    error = AT45_GetCompare(pAt45);
    return (!error);
}

unsigned char AT45_Buf2ComparePage(At45 *pAt45,unsigned int address)		//0x61
{
    unsigned char error;
    
    //Issue a page compare buf1 command
    error = AT45_SendCommand(pAt45,AT45_PAGE_BUF2_CMP,4,
    					 0,0,address,0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    error = AT45_GetCompare(pAt45);
    return (!error);
}

unsigned char AT45_ErasePage(At45 *pAt45,unsigned int address)				//0x81
{
    unsigned char error;
    
    //Issue a page ease command
    error = AT45_SendCommand(pAt45, AT45_PAGE_ERASE, 4, 0, 0, address, 0, 0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_PageWriteBuf1(At45 *pAt45,unsigned char *pBuffer,unsigned int page)//0x82
{
    unsigned char error;
    if ( page > AT45_PageNumber(pAt45) )
    {
        return 1;
    }
    error = AT45_SendCommand(pAt45,AT45_PAGE_WRITE_BUF1,4,
    						 pBuffer,AT45_PageSize(pAt45),
    						 page * AT45_PageSize(pAt45),0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_Buf1MemErase(At45 *pAt45,unsigned int page)				//0x83
{
    unsigned char error;
    if ( page > AT45_PageNumber(pAt45) )
    {
        return 1;
    }
    error = AT45_SendCommand(pAt45,AT45_BUF1_MEM_ERASE,4,0,0,
    						 page * AT45_PageSize(pAt45),0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_Buf1Write(At45 *pAt45,unsigned char *pBuffer)			//0x84
{
    unsigned char error;
    
    //Issuse a page write to buffer1 command
    error = AT45_SendCommand(pAt45,AT45_BUF1_WRITE,4,
    					 pBuffer, pAt45->pDesc->pageSize,0,0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_PageWriteBuf2(At45 *pAt45,unsigned char *pBuffer,unsigned int page)//0x85
{
    unsigned char error;
    if ( page > AT45_PageNumber(pAt45) )
    {
        return 1;
    }
    error = AT45_SendCommand(pAt45,AT45_PAGE_WRITE_BUF2,4,
    						 pBuffer,AT45_PageSize(pAt45),
    						 page * AT45_PageSize(pAt45),0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_Buf2MemErase(At45 *pAt45,unsigned int page)				//0x86
{
    unsigned char error;
    if ( page > AT45_PageNumber(pAt45) )
    {
        return 1;
    }
    error = AT45_SendCommand(pAt45,AT45_BUF2_MEM_ERASE,4,0,0,
    						 page * AT45_PageSize(pAt45),0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_Buf2Write(At45 *pAt45,unsigned char *pBuffer)			//0x87
{
    unsigned char error;
    
    //Issuse a page write to buffer1 command
    error = AT45_SendCommand(pAt45,AT45_BUF2_WRITE,4,
    					 pBuffer, pAt45->pDesc->pageSize,0,0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_Buf1MemNoErase(At45 *pAt45,unsigned int page)			//0x88
{
    unsigned char error;
    if ( page > AT45_PageNumber(pAt45) )
    {
        return 1;
    }
    error = AT45_SendCommand(pAt45,AT45_BUF1_MEM_NOERASE,4,0,0,
    						 page * AT45_PageSize(pAt45),0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_Buf2MemNoErase(At45 *pAt45,unsigned int page)			//0x89
{
    unsigned char error;
    if ( page > AT45_PageNumber(pAt45) )
    {
        return 1;
    }
    error = AT45_SendCommand(pAt45,AT45_BUF2_MEM_NOERASE,4,0,0,
    						 page * AT45_PageSize(pAt45),0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    AT45_WaitReady(pAt45);
    return 0;
}

unsigned char AT45_PageRead(At45 *pAt45,unsigned char *pBuffer,unsigned int page)	//0xd2
{
    unsigned char error;
    if ( page > AT45_PageNumber(pAt45) )
    {
        return 1;
    }
    error = AT45_SendCommand(pAt45,AT45_PAGE_READ,8,
    						 pBuffer,AT45_PageSize(pAt45),
    						 page * AT45_PageSize(pAt45),0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    return 0;
}

unsigned char AT45_Buf1Read(At45 *pAt45,unsigned char *pBuffer)				//0xd4
{
    unsigned char error;
    
    error = AT45_SendCommand(pAt45,AT45_BUF1_READ_SER,5,pBuffer,
    					 AT45_PageSize(pAt45), 0,0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    return 0;
}

unsigned char AT45_Buf2Read(At45 *pAt45,unsigned char *pBuffer)				//0xd6
{
    unsigned char error;
    
    error = AT45_SendCommand(pAt45,AT45_BUF2_READ_SER,5,pBuffer,
    					 AT45_PageSize(pAt45), 0,0,0);
    if ( error )
    {
        return AT45_ERROR_LOCK;
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    return 0;
}

unsigned char AT45_GetStatus(At45 *pAt45)									//0xd7
{
    unsigned char status;
    
    AT45_SendCommand(pAt45,AT45_STATUS_READ,1,&status,1,0,0,0);
    while ( AT45_IsBusy(pAt45) )
    {
    }
    return status;
}

int AT45_Read(At45 *pAt45,unsigned char *pBuffer,
							unsigned int address,unsigned int len)			//0xe8
{
    unsigned char error;
	int length;
    
	length = len;
    //Issue a continuous read array command
    error = AT45_SendCommand(pAt45,AT45_CONTINUOUS_READ_LEG,8,
    						 pBuffer,len,address,0,0);
    if ( error )
    {
        return (-AT45_ERROR_LOCK);
    }
    while ( AT45_IsBusy(pAt45) )
    {
    }
    return length;
}

int AT45_Write(At45 *pAt45,unsigned char *pBuffer,
			   unsigned int address,unsigned int len)
{
    unsigned int page1,page2;
    unsigned int res1,res2;
    unsigned char *p;
    char error;
    unsigned int i,j,page;
    unsigned int adr,pagesize;
	unsigned char buf[528];
    int	RealLen,length;
    
    p = buf;
    pagesize = AT45_PageSize(pAt45);
    page1 = address / pagesize;
    page2 = (address + len) / pagesize;
    res1 = address % pagesize;
    res2 = (address + len) % pagesize;
	adr = page1 * pagesize;
    if ( (page1 == page2) || ((page1 == (page2 - 1)) && (res2 == 0)) )
    {
		length = AT45_Read(pAt45,p,adr,pagesize);
		if ( length != pagesize )
		{
			return (length);
		}
		error = AT45_ErasePage(pAt45,adr);
		if ( error )
		{
			return (-error);
		}
		if ( res2 == 0 )
		{
			j = pagesize;
		}
		else
		{
			j = res2;
		}
		for ( i = res1; i < j; i++ )
		{
			p[i] = *pBuffer++;
		}
		RealLen = j - res1;
		error = AT45_PageWriteBuf1(pAt45,p,page1);
        /*memset(p,0xff,pagesize);
        error = AT45_Buf1Write(pAt45,p);
        if ( error )
        {
            return (-error);
        }
        adr = page1 * pagesize;
        error = AT45_Buf1ComparePage(pAt45,pagesize);
        if ( error )
        {
			//length = AT45_Read(pAt45,p,page1 * AT45_PageSize(pAt45),AT45_PageSize(pAt45)); 
            if ( res2 == 0 )
            {
                j = pagesize;
            }
            else
            {
                j = res2;
            }
            for ( i = res1; i < j; i++ )
            {
                p[i] = *pBuffer++;
            }
            RealLen = j - res1;
            error = AT45_PageWriteBuf1(pAt45,p,page1);
            if ( error )
            {
                return (-error);
            }
        }
        else
        {
            length = AT45_Read(pAt45,p,adr,pagesize);
            if ( length != pagesize )
            {
                return (length);
            }
            error = AT45_ErasePage(pAt45,adr);
            if ( error )
            {
                return (-error);
            }
            if ( res2 == 0 )
            {
                j = pagesize;
            }
            else
            {
                j = res2;
            }
            for ( i = res1; i < j; i++ )
            {
                p[i] = *pBuffer++;
            }
            RealLen = j - res1;
            error = AT45_PageWriteBuf1(pAt45,p,page1);
        }*/
    }
    else
    {
        RealLen = 0;
        if ( res1 != 0 )
        {
			length = AT45_Read(pAt45,p,adr,pagesize);
			if ( length != pagesize )
			{
				return (length);
			}
			error = AT45_ErasePage(pAt45,adr);
			if ( error )
			{
				return (-error);
			}
			for ( i = res1; i < pagesize; i++ )
			{
				p[i] = *pBuffer++;
			}
			RealLen += pagesize - res1;
			error = AT45_PageWriteBuf1(pAt45,p,page1);
			if ( error )
			{
				return (-error);
			}
			j = page1 + 1;
            /*memset(p,0xff,pagesize);
            error = AT45_Buf1Write(pAt45,p);
            if ( error )
            {
                return (-error);
            }
            adr = page1 * pagesize;
            error = AT45_Buf1ComparePage(pAt45,adr);
            if ( error )
            {
                for ( i = res1; i < pagesize; i++ )
                {
                    p[i] = *pBuffer++;
                }
                RealLen += pagesize - res1;
                error = AT45_PageWriteBuf1(pAt45,p,page1);
                if ( error )
                {
                    return (-error);
                }
            }
            else
            {
                length = AT45_Read(pAt45,p,adr,pagesize);
                if ( length != pagesize )
                {
                    return (length);
                }
                error = AT45_ErasePage(pAt45,adr);
                if ( error )
                {
                    return (-error);
                }
                for ( i = res1; i < pagesize; i++ )
                {
                    p[i] = *pBuffer++;
                }
                RealLen += pagesize - res1;
                error = AT45_PageWriteBuf1(pAt45,p,page1);
                if ( error )
                {
                    return (-error);
                }
            }
            j = page1 + 1;*/
        }
        else
        {
            j = page1;
        }
        for ( page = j; page < page2; page++ )
        {
			adr = page * pagesize;
			error = AT45_ErasePage(pAt45,adr);
			if ( error )
			{
				return (-error);
			}
			memcpy(p,pBuffer,pagesize);
            error = AT45_PageWriteBuf1(pAt45,p,page);
            if ( error )
            {
                return (-error);
            }
            pBuffer += pagesize;
            RealLen += pagesize;
            /*adr = page * pagesize;
            memset(p,0xff,pagesize);
            error = AT45_Buf1Write(pAt45,p);
            if ( error )
            {
                return (-error);
            }
            error = AT45_Buf1ComparePage(pAt45,adr);
            if ( !error )
            {
                error = AT45_ErasePage(pAt45,adr);
                if ( error )
                {
                    return (-error);
                }
            }
            memcpy(p,pBuffer,pagesize);
            error = AT45_PageWriteBuf1(pAt45,p,page);
            if ( error )
            {
                return (-error);
            }
            pBuffer += pagesize;
            RealLen += pagesize;*/
        }
        if ( res2 )
        {
			length = AT45_Read(pAt45,p,adr,pagesize);
			if ( length != pagesize )
			{
				return (length);
			}
			error = AT45_ErasePage(pAt45,adr);
			if ( error )
			{
				return (-error);
			}
			for ( i = 0; i < res2; i++ )
			{
				p[i] = *pBuffer++;
			}
			RealLen += res2;
			error = AT45_PageWriteBuf1(pAt45,p,page2);
			if ( error )
			{
				return (-error);
			}
            /*memset(p,0xff,pagesize);
            error = AT45_Buf1Write(pAt45,p);
            if ( error )
            {
                return (-error);
            }
            adr = page2 * pagesize;
            error = AT45_Buf1ComparePage(pAt45,adr);
            if ( error )
            {
                for ( i = 0; i < res2; i++ )
                {
                    p[i] = *pBuffer++;
                }
                RealLen += res2;
                error = AT45_PageWriteBuf1(pAt45,p,page2);
                if ( error )
                {
                    return (-error);
                }
            }
            else
            {
                length = AT45_Read(pAt45,p,adr,pagesize);
                if ( length != pagesize )
                {
                    return (length);
                }
                error = AT45_ErasePage(pAt45,adr);
                if ( error )
                {
                    return (-error);
                }
                for ( i = 0; i < res2; i++ )
                {
                    p[i] = *pBuffer++;
                }
                RealLen += res2;
                error = AT45_PageWriteBuf1(pAt45,p,page2);
                if ( error )
                {
                    return (-error);
                }
            }*/
        }
    }
    return RealLen;
}
