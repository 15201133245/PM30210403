#include "at83.h"
#include <intrinsics.h>
#define	TWI_OK			0
#define	TWI_BUSY		2
#define TWI_ERROR		3

#define	TWI_TIMEOUT		5

#define TRUE			0
#define FALSE			1

extern	unsigned int TwiErrCount;
extern	At83 at83;

void TwiTimeOut(void)
{
	at83.TimeOutFlg = 1;
}

unsigned char c26_read_init(At83 *pAt83,
					   unsigned char len,
					   unsigned char *p)
{
	unsigned char error;
	Twid *pTwid;
	Tmr *pTmr;
	
	pTwid = pAt83->pTwid;
	error = AT83_IsBusy(pAt83);
	if(error)
	{
		return(TWI_BUSY);
	}
	pTwid->semaphore--;
	TWI_StartRead(pTwid->pTwi,pAt83->DeviceAdr,IADR,ISIZE);
	while(len--)
	{
		if(!len)
		{
			TWI_Stop(pTwid->pTwi);
		}
		pAt83->TimeOutFlg = 0;
		pTmr = CreateTimer1(TIMER_ONE_DELETE,AT83_TIMES,TwiTimeOut);
		while(!TWI_GetRXRDY(pTwid->pTwi))
		{
			CheckTimer1Go();
			if(pAt83->TimeOutFlg)
			{
				break;
			}
		}
		if(!pAt83->TimeOutFlg)
		{
			DeleteTimer1(pTmr);
		}
		else
		{
			pTwid->semaphore++;
			return(TWI_TIMEOUT);
		}
		*p++ = TWI_ReadByte(pTwid->pTwi);
	}
	pAt83->TimeOutFlg = 0;
	pTmr = CreateTimer1(TIMER_ONE_DELETE,AT83_TIMES,TwiTimeOut);
	while(!TWI_TransferComplete(pTwid->pTwi))
	{
		CheckTimer1Go();
		if(pAt83->TimeOutFlg)
		{
			break;
		}
	}
	if(!pAt83->TimeOutFlg)
	{
		DeleteTimer1(pTmr);
	}
	else
	{
		pTwid->semaphore++;
		return(TWI_TIMEOUT);
	}
	pTwid->semaphore++;
	return(TWI_OK);
}

unsigned char c26_write_init(At83 *pAt83,unsigned char len,unsigned char *p)
{
	unsigned char error;
	Twid *pTwid;
	Tmr *pTmr;
	
	pTwid = pAt83->pTwid;
	error = AT83_IsBusy(pAt83);
	if(error)
	{
		return(TWI_BUSY);
	}
	pTwid->semaphore--;
	TWI_StartWrite(pTwid->pTwi,pAt83->DeviceAdr,IADR,ISIZE,*p++);
	while(len--)
	{
		if(!len)
		{
			TWI_Stop(pTwid->pTwi);
		}
		pAt83->TimeOutFlg = 0;
		pTmr = CreateTimer1(TIMER_ONE_DELETE,AT83_TIMES,TwiTimeOut);
		while(!TWI_GetTXRDY(pTwid->pTwi))
		{
			CheckTimer1Go();
			if(pAt83->TimeOutFlg)
			{
				break;
			}
		}
		if(!pAt83->TimeOutFlg)
		{
			DeleteTimer1(pTmr);
		}
		else
		{
			pTwid->semaphore++;
			return(TWI_TIMEOUT);
		}
		if(!len)
		{
			break;
		}
		TWI_WriteByte(pTwid->pTwi,*p++);
	}
	pAt83->TimeOutFlg = 0;
	pTmr = CreateTimer1(TIMER_ONE_DELETE,AT83_TIMES,TwiTimeOut);
	while(!TWI_TransferComplete(pTwid->pTwi))
	{
		CheckTimer1Go();
		if(pAt83->TimeOutFlg)
		{
			break;
		}
	}
	if(!pAt83->TimeOutFlg)
	{
		DeleteTimer1(pTmr);
	}
	else
	{
		pTwid->semaphore++;
		return(TWI_TIMEOUT);
	}
	pTwid->semaphore++;
	return(TWI_OK);
}

unsigned char c26_read(At83 *pAt83,
					   unsigned char len,
					   unsigned char *p)
{
	unsigned char error;
	Twid *pTwid;
	unsigned int i;
	//Tmr *pTmr;
	
	pTwid = pAt83->pTwid;
	error = AT83_IsBusy(pAt83);
	if(error)
	{
		return(TWI_BUSY);
	}
	pTwid->semaphore--;
	__disable_interrupt();
	TWI_StartRead(pTwid->pTwi,pAt83->DeviceAdr,IADR,ISIZE);
	while(len--)
	{
		if(!len)
		{
			TWI_Stop(pTwid->pTwi);
		}
		i = 0;
		while(!TWI_GetRXRDY(pTwid->pTwi))
		{
			i++;
			if(i > 0x8000)
			{
				__enable_interrupt();
				pTwid->semaphore++;
				return(TWI_ERROR);
			}
		}
		*p++ = TWI_ReadByte(pTwid->pTwi);
	}
	i = 0;
	while(!TWI_TransferComplete(pTwid->pTwi))
	{
		i++;
		if(i > 0x8000)
		{
			__enable_interrupt();
			pTwid->semaphore++;
			return(TWI_ERROR);
		}
	}
	__enable_interrupt();
	pTwid->semaphore++;
	return(TWI_OK);
}

unsigned char c26_write(At83 *pAt83,unsigned char len,unsigned char *p)
{
	unsigned char error;
	Twid *pTwid;
	unsigned int i;
	
	pTwid = pAt83->pTwid;
	error = AT83_IsBusy(pAt83);
	if(error)
	{
		return(TWI_BUSY);
	}
	pTwid->semaphore--;
	__disable_interrupt();
	TWI_StartWrite(pTwid->pTwi,pAt83->DeviceAdr,IADR,ISIZE,*p++);
	while(len--)
	{
		if(!len)
		{
			TWI_Stop(pTwid->pTwi);
		}
		i = 0;
		while(!TWI_GetTXRDY(pTwid->pTwi))
		{
			i++;
			if(i > 0x8000)
			{
				__enable_interrupt();
				pTwid->semaphore++;
				return(TWI_ERROR);
			}
		}
		if(!len)
		{
			break;
		}
		TWI_WriteByte(pTwid->pTwi,*p++);
	}
	i = 0;
	while(!TWI_TransferComplete(pTwid->pTwi))
	{
		i++;
		if(i > 0x8000)
		{
			__enable_interrupt();
			pTwid->semaphore++;
			return(TWI_ERROR);
		}
	}
	__enable_interrupt();
	pTwid->semaphore++;
	return(TWI_OK);
}

unsigned char c26_read_cmd1_init(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write_init(pAt83,1,&status);
	status = c26_read_init(pAt83,READ_CMD1_LENGTH,(unsigned char *)(&pReg->statusa));
	return(status);
}

unsigned char c26_read_cmd1(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,READ_CMD1_LENGTH,(unsigned char *)(&pReg->statusa));
	return(status);
}

unsigned char c26_read_cmd1_capture_lsb(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,11,(unsigned char *)(&pReg->statusa));
	return(status);
}

unsigned char c26_read_cmd1_capture_msb(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,10,(unsigned char *)(&pReg->statusa));
	return(status);
}	

unsigned char c26_read_cmd1_timer_lsb(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,9,(unsigned char *)(&pReg->statusa));
	return(status);
}

unsigned char c26_read_cmd1_timer_msb(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,8,(unsigned char *)(&pReg->statusa));
	return(status);
}

unsigned char c26_read_cmd1_sc1_interface(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,7,(unsigned char *)(&pReg->statusa));
	return(status);
}

unsigned char c26_read_cmd1_sc1_cfg4(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,6,(unsigned char *)(&pReg->statusa));
	return(status);
}

unsigned char c26_read_cmd1_sc1_cfg3(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,5,(unsigned char *)(&pReg->statusa));
	return(status);
}

unsigned char c26_read_cmd1_sc1_cfg2(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,4,(unsigned char *)(&pReg->statusa));
	return(status);
}

unsigned char c26_read_cmd1_sc1_cfg1(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,3,(unsigned char *)(&pReg->statusa));
	return(status);
}

unsigned char c26_read_cmd1_sc1_cfg0(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,2,(unsigned char *)(&pReg->statusa));
	return(status);
}

unsigned char c26_read_cmd1_sc1_status(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_WRITE_TIMER;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,1,(unsigned char *)(&pReg->statusa));
	return(status);
}

unsigned char c26_read_cmd2(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC_DCDCB;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,READ_CMD2_LENGTH,(unsigned char *)(&pReg->statusb));
	return(status);
}

unsigned char c26_read_cmd2_itdis(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC_DCDCB;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,4,(unsigned char *)(&pReg->statusb));
	return(status);
}

unsigned char c26_read_cmd2_interfaceb(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC_DCDCB;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,3,(unsigned char *)(&pReg->statusb));
	return(status);
}

unsigned char c26_read_cmd2_io_select(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC_DCDCB;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,2,(unsigned char *)(&pReg->statusb));
	return(status);
}

unsigned char c26_read_cmd2_statusb(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC_DCDCB;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,1,(unsigned char *)(&pReg->statusb));
	return(status);
}

unsigned char c26_read_cmd3(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC2;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,READ_CMD3_LENGTH,(unsigned char *)(&pReg->sc2_cfg0));
	return(status);
}

unsigned char c26_read_cmd3_sc2_cfg2(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC2;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,3,(unsigned char *)(&pReg->sc2_cfg0));
	return(status);
}

unsigned char c26_read_cmd3_sc2_cfg1(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC2;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,2,(unsigned char *)(&pReg->sc2_cfg0));
	return(status);
}

unsigned char c26_read_cmd3_sc2_cfg0(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC2;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,1,(unsigned char *)(&pReg->sc2_cfg0));
	return(status);
}

unsigned char c26_read_cmd4(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC3;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,READ_CMD4_LENGTH,(unsigned char *)(&pReg->sc3_cfg0));
	return(status);
}

unsigned char c26_read_cmd4_sc3_cfg2(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC3;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,2,(unsigned char *)(&pReg->sc3_cfg0));
	return(status);
}

unsigned char c26_read_cmd4_sc3_cfg0(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC3;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,1,(unsigned char *)(&pReg->sc3_cfg0));
	return(status);
}

unsigned char c26_read_cmd5(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC4;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,READ_CMD5_LENGTH,(unsigned char *)(&pReg->sc4_cfg0));
	return(status);
}

unsigned char c26_read_cmd5_sc4_cfg2(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC4;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,2,(unsigned char *)(&pReg->sc4_cfg0));
	return(status);
}

unsigned char c26_read_cmd5_sc4_cfg0(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC4;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,1,(unsigned char *)(&pReg->sc4_cfg0));
	return(status);
}

unsigned char c26_read_cmd6(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC5;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,READ_CMD6_LENGTH,(unsigned char *)(&pReg->sc5_cfg0));
	return(status);
}

unsigned char c26_read_cmd6_sc5_cfg2(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC5;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,2,(unsigned char *)(&pReg->sc5_cfg0));
	return(status);
}

unsigned char c26_read_cmd6_sc5_cfg0(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SC5;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,1,(unsigned char *)(&pReg->sc5_cfg0));
	return(status);
}

unsigned char c26_read_cmd7(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_DCDCB_CMD;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,READ_CMD7_LENGTH,(unsigned char *)(&pReg->dcdcb));
	return(status);
}

unsigned char c26_read_cmd7_ldo(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_DCDCB_CMD;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,2,(unsigned char *)(&pReg->dcdcb));
	return(status);
}

unsigned char c26_read_cmd7_dcdcb(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_DCDCB_CMD;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,1,(unsigned char *)(&pReg->dcdcb));
	return(status);
}

unsigned char c26_read_cmd8(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SLEW_CTRL;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,READ_CMD8_LENGTH,(unsigned char *)(&pReg->slew_ctrl_1));
	return(status);
}

unsigned char c26_read_cmd8_slew_ctrl_3(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SLEW_CTRL;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,3,(unsigned char *)(&pReg->slew_ctrl_1));
	return(status);
}

unsigned char c26_read_cmd8_slew_ctrl_2(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SLEW_CTRL;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,2,(unsigned char *)(&pReg->slew_ctrl_1));
	return(status);
}

unsigned char c26_read_cmd8_slew_ctrl_1(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	status = C26_W_SLEW_CTRL;
	c26_write(pAt83,1,&status);
	status = c26_read(pAt83,1,(unsigned char *)(&pReg->slew_ctrl_1));
	return(status);
}

unsigned char c26_write_cmd1(At83 *pAt83)
{
	unsigned char status;
	unsigned char command;
	
	command = C26_W_GENERAL_CALL_RESET;
	status = c26_write(pAt83,1,&command);
	return(status);
}

unsigned char c26_write_cmd2(At83 *pAt83)
{
	unsigned char status;
	unsigned char command;
	
	command = C26_W_RESET;
	status = c26_write(pAt83,1,&command);
	return(status);
}

unsigned char c26_write_cmd3(At83 *pAt83)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	pReg->sc1_cfg0.reserved0 = 2;
	status = c26_write(pAt83,5,
					   (unsigned char *)(&pReg->sc1_cfg0));
	return(status);
}

unsigned char c26_write_cmd3_sc1_cfg0(At83 *pAt83,unsigned char sc1_cfg0)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	pReg->sc1_cfg0.cfg0 = sc1_cfg0;
	pReg->sc1_cfg0.reserved0 = 2;
	status = c26_write(pAt83,1,
					   (unsigned char *)(&pReg->sc1_cfg0));
	return(status);
}

unsigned char c26_write_cmd3_sc1_cfg1(At83 *pAt83,unsigned char sc1_cfg1)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	pReg->sc1_cfg1.cfg1 = sc1_cfg1;
	status = c26_write(pAt83,2,
					   (unsigned char *)(&pReg->sc1_cfg0));
	return(status);
}

unsigned char c26_write_cmd3_sc1_cfg2(At83 *pAt83,unsigned char sc1_cfg2)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	pReg->sc1_cfg2.cfg2 = sc1_cfg2;
	status = c26_write(pAt83,3,
					   (unsigned char *)(&pReg->sc1_cfg0));
	return(status);
}

unsigned char c26_write_cmd3_sc1_cfg3(At83 *pAt83,unsigned char sc1_cfg3)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	pReg->sc1_cfg3.cfg3 = sc1_cfg3;
	status = c26_write(pAt83,4,
					   (unsigned char *)(&pReg->sc1_cfg0));
	return(status);
}

unsigned char c26_write_cmd3_sc1_cfg4(At83 *pAt83,unsigned char sc1_cfg4)
{
	unsigned char status;
	At83Reg *pReg;
	
	pReg = &pAt83->reg;
	pReg->sc1_cfg4.cfg4 = sc1_cfg4;
	status = c26_write(pAt83,5,
					   (unsigned char *)(&pReg->sc1_cfg0));
	return(status);
}

unsigned char c26_write_cmd4(At83 *pAt83)
{
	unsigned char status;
	unsigned char buf[3];
	
	buf[0] = C26_W_WRITE_TIMER;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.timer_msb),2);
	status = c26_write(pAt83,3,buf);
	return(status);
}

unsigned char c26_write_cmd4_timer_msb(At83 *pAt83,unsigned char timer_msb)
{
	unsigned char status;
	unsigned char buf[2];
	
	pAt83->reg.timer_msb = timer_msb;
	buf[0] = C26_W_WRITE_TIMER;
	buf[1] = timer_msb;
	status = c26_write(pAt83,2,buf);
	return(status);
}

unsigned char c26_write_cmd4_timer_lsb(At83 *pAt83,unsigned char timer_lsb)
{
	unsigned char status;
	unsigned char buf[3];
	
	pAt83->reg.timer_lsb = timer_lsb;
	buf[0] = C26_W_WRITE_TIMER;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.timer_msb),2);
	status = c26_write(pAt83,3,buf);
	return(status);
}

unsigned char c26_write_cmd5(At83 *pAt83)
{
	unsigned char status;
	unsigned char sc1_interface;
	
	sc1_interface = pAt83->reg.sc1_interface.interface;
	status = c26_write(pAt83,1,&sc1_interface);
	return(status);
}

unsigned char c26_write_cmd5_sc1_interface(At83 *pAt83,unsigned char sc1_interface)
{
	unsigned char status;
	
	sc1_interface &= (~0x80);
	pAt83->reg.sc1_interface.interface = sc1_interface;
	status = c26_write(pAt83,1,&sc1_interface);
	return(status);
}

unsigned char c26_write_cmd6(At83 *pAt83)
{
	unsigned char status;
	unsigned char buf[4];
	
	buf[0] = C26_W_SC_DCDCB;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.io_select),3);
	status = c26_write(pAt83,4,buf);
	return(status);
}

unsigned char c26_write_cmd6_io_select(At83 *pAt83,unsigned char io_select)
{
	unsigned char status;
	unsigned char buf[2];
	
	pAt83->reg.io_select.select = io_select;
	buf[0] = C26_W_SC_DCDCB;
	buf[1] = io_select;
	status = c26_write(pAt83,2,buf);
	return(status);
}

unsigned char c26_write_cmd6_interfaceb(At83 *pAt83,unsigned char interfaceb)
{
	unsigned char status;
	unsigned char buf[3];
	
	pAt83->reg.interfaceb.interface = interfaceb;
	buf[0] = C26_W_SC_DCDCB;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.io_select),2);
	status = c26_write(pAt83,3,buf);
	return(status);
}

unsigned char c26_write_cmd6_itdis(At83 *pAt83,unsigned char itdis)
{
	unsigned char status;
	unsigned char buf[4];
	
	pAt83->reg.itdis.itdis = itdis;
	buf[0] = C26_W_SC_DCDCB;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.io_select),3);
	status = c26_write(pAt83,4,buf);
	return(status);
}

unsigned char c26_write_cmd7(At83 *pAt83)
{
	unsigned char status;
	unsigned char buf[4];
	
	buf[0] = C26_W_SC2;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.sc2_cfg0),3);
	status = c26_write(pAt83,4,buf);
	return(status);
}

unsigned char c26_write_cmd7_sc2_cfg0(At83 *pAt83,unsigned char sc2_cfg0)
{
	unsigned char status;
	unsigned char buf[2];
	
	pAt83->reg.sc2_cfg0.cfg0 = sc2_cfg0;
	buf[0] = C26_W_SC2;
	buf[1] = sc2_cfg0;
	status = c26_write(pAt83,2,buf);
	return(status);
}

unsigned char c26_write_sc2_cfg1(At83 *pAt83,unsigned char sc2_cfg1)
{
	unsigned char status;
	unsigned char buf[3];
	
	pAt83->reg.sc2_cfg1.cfg1 = sc2_cfg1;
	buf[0] = C26_W_SC2;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.sc2_cfg0),2);
	status = c26_write(pAt83,3,buf);
	return(status);
}

unsigned char c26_write_cmd7_sc2_cfg2(At83 *pAt83,unsigned char sc2_cfg2)
{
	unsigned char status;
	unsigned char buf[4];
	
	pAt83->reg.sc2_cfg2.cfg2 = sc2_cfg2;
	buf[0] = C26_W_SC2;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.sc2_cfg0),3);
	status = c26_write(pAt83,4,buf);
	return(status);
}

unsigned char c26_write_cmd8(At83 *pAt83)
{
	unsigned char status;
	unsigned char buf[3];
	
	buf[0] = C26_W_SC3;
	pAt83->reg.sc3_cfg0.cfg0 &= 0x03;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.sc3_cfg0),2);
	status = c26_write(pAt83,3,buf);
	return(status);
}

unsigned char c26_write_cmd8_sc3_cfg0(At83 *pAt83,unsigned char sc3_cfg0)
{
	unsigned char status;
	unsigned char buf[2];
	
	pAt83->reg.sc3_cfg0.cfg0 = sc3_cfg0;
	buf[0] = C26_W_SC3;
	buf[1] = sc3_cfg0;
	status = c26_write(pAt83,2,buf);
	return(status);
}

unsigned char c26_write_cmd8_sc3_cfg2(At83 *pAt83,unsigned char sc3_cfg2)
{
	unsigned char status;
	unsigned char buf[3];
	
	pAt83->reg.sc3_cfg2.cfg2 = sc3_cfg2;
	buf[0] = C26_W_SC3;
	pAt83->reg.sc3_cfg0.cfg0 &= 0x03;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.sc3_cfg0),2);
	status = c26_write(pAt83,3,buf);
	return(status);
}

unsigned char c26_write_cmd9(At83 *pAt83)
{
	unsigned char status;
	unsigned char buf[3];
	
	buf[0] = C26_W_SC4;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.sc4_cfg0),2);
	status = c26_write(pAt83,3,buf);
	return(status);
}

unsigned char c26_write_cmd9_sc4_cfg0(At83 *pAt83,unsigned char sc4_cfg0)
{
	unsigned char status;
	unsigned char buf[2];
	
	pAt83->reg.sc4_cfg0.cfg0 = sc4_cfg0;
	buf[0] = C26_W_SC4;
	buf[1] = sc4_cfg0;
	status = c26_write(pAt83,2,buf);
	return(status);
}

unsigned char c26_write_cmd9_sc4_cfg2(At83 *pAt83,unsigned char sc4_cfg2)
{
	unsigned char status;
	unsigned char buf[3];
	
	pAt83->reg.sc4_cfg2.cfg2 = sc4_cfg2;
	buf[0] = C26_W_SC4;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.sc4_cfg0),2);
	status = c26_write(pAt83,3,buf);
	return(status);
}

unsigned char c26_write_cmd10(At83 *pAt83)
{
	unsigned char status;
	unsigned char buf[3];
	
	buf[0] = C26_W_SC5;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.sc5_cfg0),2);
	status = c26_write(pAt83,3,buf);
	return(status);
}

unsigned char c26_write_cmd10_sc5_cfg0(At83 *pAt83,unsigned char sc5_cfg0)
{
	unsigned char status;
	unsigned char buf[2];
	
	pAt83->reg.sc5_cfg0.cfg0 = sc5_cfg0;
	buf[0] = C26_W_SC5;
	buf[1] = sc5_cfg0;
	status = c26_write(pAt83,2,buf);
	return(status);
}

unsigned char c26_write_cmd10_sc5_cfg2(At83 *pAt83,unsigned char sc5_cfg2)
{
	unsigned char status;
	unsigned char buf[3];
	
	pAt83->reg.sc5_cfg2.cfg2 = sc5_cfg2;
	buf[0] = C26_W_SC5;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.sc5_cfg0),2);
	status = c26_write(pAt83,3,buf);
	return(status);
}

unsigned char c26_write_cmd11(At83 *pAt83)
{
	unsigned char status;
	unsigned char buf[3];
	
	buf[0] = C26_W_DCDCB_CMD;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.dcdcb),2);
	status = c26_write(pAt83,3,buf);
	return(status);
}

unsigned char c26_write_cmd11_dcdcb(At83 *pAt83,unsigned char dcdcb)
{
	unsigned char status;
	unsigned char buf[2];
	
	pAt83->reg.dcdcb.dcdcb = dcdcb;
	buf[0] = C26_W_DCDCB_CMD;
	buf[1] = dcdcb;
	status = c26_write(pAt83,2,buf);
	return(status);
}

unsigned char c26_write_cmd11_ldo(At83 *pAt83,unsigned char ldo)
{
	unsigned char status;
	unsigned char buf[3];
	
	pAt83->reg.ldo.ldo = ldo;
	buf[0] = C26_W_DCDCB_CMD;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.dcdcb),2);
	status = c26_write(pAt83,3,buf);
	return(status);
}

unsigned char c26_write_cmd12(At83 *pAt83)
{
  	unsigned char status;
	unsigned char buf[4];
	
	buf[0] = C26_W_SLEW_CTRL;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.slew_ctrl_1),3);
	status = c26_write(pAt83,4,buf);
	return status;
}

unsigned char c26_write_cmd12_slew_ctrl_1(At83 *pAt83,unsigned char slew_ctrl_1)
{
  	unsigned char status;
	unsigned char buf[2];
	
	pAt83->reg.slew_ctrl_1.slew = slew_ctrl_1;
	buf[0] = C26_W_SLEW_CTRL;
	buf[1] = slew_ctrl_1;
	status = c26_write(pAt83,2,buf);
	return status;
}

unsigned char c26_write_cmd12_slew_ctrl_2(At83 *pAt83,unsigned char slew_ctrl_2)
{
  	unsigned char status;
	unsigned char buf[3];
	
	pAt83->reg.slew_ctrl_2.slew = slew_ctrl_2;
	buf[0] = C26_W_SLEW_CTRL;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.slew_ctrl_1),2);
	status = c26_write(pAt83,3,buf);
	return status;
}

unsigned char c26_write_cmd12_slew_ctrl_3(At83 *pAt83,unsigned char slew_ctrl_3)
{
  	unsigned char status;
	unsigned char buf[4];
	
	pAt83->reg.slew_ctrl_3.slew = slew_ctrl_3;
	buf[0] = C26_W_SLEW_CTRL;
	memcpy(&buf[1],(unsigned char *)(&pAt83->reg.slew_ctrl_1),3);
	status = c26_write(pAt83,4,buf);
	return status;
}


unsigned char c26_config_init(At83 *pAt83)
{
	c26_write_cmd3(pAt83);
	c26_write_cmd5(pAt83);
	c26_write_cmd7(pAt83);
	c26_write_cmd8(pAt83);
	c26_write_cmd9(pAt83);
	c26_write_cmd10(pAt83);
	c26_write_cmd11(pAt83);
	return(0);
}

unsigned char c26_dcdca_start(At83 *pAt83)
{
	At83Reg *pReg;
	Tmr *pTmr;
  	
	//init SHUDOWNA bit
	pReg = &pAt83->reg;
	c26_read_cmd1_sc1_cfg4(pAt83);
	//DEMBOOST = 00
	pReg->sc1_cfg4.dem_boost = DEMBOOST0;
	c26_write_cmd3(pAt83);
	
	//start DCDC 5V
	pReg->sc1_cfg0.vcard = VCARD_VCC_5;
	pReg->sc1_cfg1.shut_down = CLR;
	c26_write_cmd3(pAt83);
	//delay 3ms
	pAt83->TimeFlg = 0;
	pTmr = CreateTimer1(TIMER_ONE,AT83_TIMES,pAt83->RunTime);
	while(!pAt83->TimeFlg)
	{
		CheckTimer1Go();
	}
	DeleteTimer1(pTmr);
	//read VCARD_OK1
	c26_read_cmd1_sc1_status(pAt83);
	if(pReg->statusa.vcard_ok == SET)
	{
		return(TRUE);
	}
	
	//DEMBOOST = 01
	pReg->sc1_cfg4.dem_boost = DEMBOOST1;
	c26_write_cmd3(pAt83);
	//delay 3ms
	pAt83->TimeFlg = 0;
	pTmr = CreateTimer1(TIMER_ONE,AT83_TIMES,pAt83->RunTime);
	while(!pAt83->TimeFlg)
	{
		CheckTimer1Go();
	}
	DeleteTimer1(pTmr);
	//read VCARD_OK1
	c26_read_cmd1_sc1_status(pAt83);
	if(pReg->statusa.vcard_ok == SET)
	{
		return(TRUE);
	}
	
	//DEMBOOST = 11
	pReg->sc1_cfg4.dem_boost = DEMBOOST3;
	c26_write_cmd3(pAt83);
	//delay 3ms
	pAt83->TimeFlg = 0;
	pTmr = CreateTimer1(TIMER_ONE, AT83_TIMES,pAt83->RunTime);
	while(!pAt83->TimeFlg)
	{
		CheckTimer1Go();
	}
	DeleteTimer1(pTmr);
	
	//read VCAARD_OK1
	c26_read_cmd1_sc1_status(pAt83);
	if(pReg->statusa.vcard_ok == SET)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

unsigned char c26_dcdcb_start(At83 *pAt83)
{
	unsigned char val;
	At83Reg *pReg;
	Tmr *pTmr;
	
	pReg = &pAt83->reg;
  	//init SHUDOWNB bit
	val = 0x00;
	c26_write_cmd11_dcdcb(pAt83,val);
	
	c26_read_cmd2_interfaceb(pAt83);
	//val = pReg->interfaceb.interfaceb;
	//DEMBOOSTB = 00
	((INTERFACEB *)&val)->dem_boostb = DEMBOOST0;
	c26_write_cmd6_interfaceb(pAt83,val);
	//start DCDC
	//val = pReg->dcdcb.dcdcb;
	((DCDCB *)&val)->vdc = VCARD_VCC_5;
	c26_write_cmd11_dcdcb(pAt83,val);
	//delay 3nms
	pAt83->TimeFlg = 0;
	pTmr =  CreateTimer1(TIMER_ONE,AT83_TIMES,pAt83->RunTime);
	while(!pAt83->TimeFlg)
	{
		CheckTimer1Go();
	}
	DeleteTimer1(pTmr);
	//read VDCB_OK
	c26_read_cmd7_dcdcb(pAt83);
	if(pReg->dcdcb.vdc_ok == SET)
	{
		return(TRUE);
	}
	
	//DEMBOOST = 01
	c26_read_cmd2(pAt83);
	//val = pReg->dcdcb.dcdcb;
	((INTERFACEB *)&val)->dem_boostb = DEMBOOST1;
	c26_write_cmd6_interfaceb(pAt83,val);
	//delay 3nms
	pAt83->TimeFlg = 0;
	pTmr =  CreateTimer1(TIMER_ONE,AT83_TIMES,pAt83->RunTime);
	while(!pAt83->TimeFlg)
	{
		CheckTimer1Go();
	}
	DeleteTimer1(pTmr);
	//read VDCB_OK
	c26_read_cmd7_dcdcb(pAt83);
	if(pReg->dcdcb.vdc_ok == SET)
	{
		return(TRUE);
	}
	
	//DEMBOOST = 11
	c26_read_cmd2(pAt83);
	//val = pReg->dcdcb.dcdcb;
	((INTERFACEB *)&val)->dem_boostb = DEMBOOST3;
	c26_write_cmd6_interfaceb(pAt83,val);
	//delay 3nms
	pAt83->TimeFlg = 0;
	pTmr =  CreateTimer1(TIMER_ONE,AT83_TIMES,pAt83->RunTime);
	while(!pAt83->TimeFlg)
	{
		CheckTimer1Go();
	}
	DeleteTimer1(pTmr);
	//read VDCB_OK
	c26_read_cmd7_dcdcb(pAt83);
	if(pReg->dcdcb.vdc_ok == SET)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

unsigned char c26_start_cvcc1(At83 *pAt83)
{
  	unsigned char uc_status;
  	
	uc_status = c26_dcdca_start(pAt83);
	return(uc_status);
}

unsigned char c26_start_cvcc2(At83 *pAt83)
{
  	unsigned char uc_status;
	unsigned char val = 0;

	//read VDCB_OK
	uc_status = c26_read_cmd7_dcdcb(pAt83);
	if(pAt83->reg.dcdcb.vdc_ok == CLR)
	{
		return(FALSE);
	}
	//start LDO
	//val = pAt83->reg.sc2_cfg0.sc2_cfg0;
	((SCX_CFG0 *)&val)->vcard = VCARD_VCC_5;
	uc_status = c26_write_cmd7_sc2_cfg0(pAt83,val);
	return(uc_status);
}

unsigned char c26_stop_cvcc2(At83 *pAt83)
{
	unsigned char uc_status;
	unsigned char val = 0;
	
	((SCX_CFG0 *)&val)->vcard = VCARD_VCC_0;
	uc_status = c26_write_cmd7_sc2_cfg0(pAt83,val);
	return(uc_status);
}

unsigned char c26_start_cvcc3(At83 *pAt83)
{
  	unsigned char uc_status;
	unsigned char val = 0;
	
	//read VDCB_OK
	uc_status = c26_read_cmd7_dcdcb(pAt83);
	if(pAt83->reg.dcdcb.vdc_ok == CLR)
	{
		return(FALSE);
	}
	//start LDO
	//val = pAt83->reg.sc3_cfg0.sc3_cfg0;
	((SCX_CFG0 *)&val)->vcard = VCARD_VCC_5;
	uc_status = c26_write_cmd8_sc3_cfg0(pAt83,val);
	return(uc_status);
}

unsigned char c26_stop_cvcc3(At83 *pAt83)
{
	unsigned char uc_status;
	unsigned char val = 0;
	
	((SCX_CFG0 *)&val)->vcard = VCARD_VCC_0;
	uc_status = c26_write_cmd8_sc3_cfg0(pAt83,val);
	return(uc_status);
}

//! Start CVCC4:
unsigned char c26_start_cvcc4(At83 *pAt83)
{
  	unsigned char uc_status;
	unsigned char val = 0;
	
	//read VDCB_OK
	uc_status = c26_read_cmd7_dcdcb(pAt83);
	if(pAt83->reg.dcdcb.vdc_ok == CLR)
	{
		return(FALSE);
	}
	//start LDO
	//val = pAt83->reg.sc4_cfg0.sc4_cfg0;
	((SCX_CFG0 *)&val)->vcard = VCARD_VCC_5;
	uc_status = c26_write_cmd9_sc4_cfg0(pAt83,val);
	return(uc_status);
}

unsigned char c26_stop_cvcc4(At83 *pAt83)
{
	unsigned char uc_status;
	unsigned char val = 0;
	
	((SCX_CFG0 *)&val)->vcard = VCARD_VCC_0;
	uc_status = c26_write_cmd9_sc4_cfg0(pAt83,val);
	return(uc_status);
}

//! Start CVCC5:
unsigned char c26_start_cvcc5(At83 *pAt83)
{
  	unsigned char uc_status;
	unsigned char val = 0;
	
	//read VDCB_OK
	uc_status = c26_read_cmd7_dcdcb(pAt83);
	if(pAt83->reg.dcdcb.vdc_ok == CLR)
	{
		return(FALSE);
	}
	//start LDO
	//val = pAt83->reg.sc5_cfg0.sc5_cfg0;
	((SCX_CFG0 *)&val)->vcard = VCARD_VCC_5;
	uc_status = c26_write_cmd10_sc5_cfg0(pAt83,val);
	return(uc_status);
}

unsigned char c26_stop_cvcc5(At83 *pAt83)
{
	unsigned char uc_status;
	unsigned char val = 0;
	
	((SCX_CFG0 *)&val)->vcard = VCARD_VCC_0;
	uc_status = c26_write_cmd10_sc5_cfg0(pAt83,val);
	return(uc_status);
}