#include "global.h"

void At83TimeOut(void)
{
	at83.TimeFlg = 1;
}

unsigned char InitCardInterface(void)
{
	At83Reg	*pReg;
	
	pReg = &at83.reg;
	/*sc1*/
	//sc1_cfg0 = 0x80
	pReg->sc1_cfg0.reserved0 = 2;
	pReg->sc1_cfg0.insert = SET;
	//sc1_cfg1 = 0x04a
	pReg->sc1_cfg1.art = CLR;
	pReg->sc1_cfg1.pull_up = SET;
	pReg->sc1_cfg1.cds = CDS_8_SAMPLE;
	//sc1_cfg2 = 4
	pReg->sc1_cfg2.dck = DCK_1;
	pReg->sc1_cfg2.cks = CKS_A2;
	//sc1_cfg3 = 0
	pReg->sc1_cfg3.icc_adj = 0;
	//sc1_cfg4 = 0
	pReg->sc1_cfg4.dem_boost = DEMBOOST0;
	pReg->sc1_cfg4.step_reg = CLR;
	//sc1_interface = 0x10;
	pReg->sc1_interface.io_dis = CLR;
	pReg->sc1_interface.ck_stop = CLR;
	pReg->sc1_interface.card_rst = CLR;
	pReg->sc1_interface.card_ck = SET;
	
	/*sc2*/
	//sc2_cfg0
	pReg->sc2_cfg0.cfg0 = 0;
	//sc2_cfg1
	pReg->sc2_cfg1.full = CLR;
	pReg->sc2_cfg1.card_det = SET;
	pReg->sc2_cfg1.pull_up = SET;
	pReg->sc2_cfg1.cds = CDS_8_SAMPLE;
	//sc2_cfg2
	pReg->sc2_cfg2.art = SET;
	pReg->sc2_cfg2.card_rst = CLR;
	pReg->sc2_cfg2.ck_stop = CLR;
	pReg->sc2_cfg2.cks = CKS_A2;
	pReg->sc2_cfg2.card_ck = SET;
	
	/*sc3*/
	//sc3_cfg0
	pReg->sc3_cfg0.cfg0 = 0;
	//sc3_cfg2
	pReg->sc3_cfg2.art = SET;
	pReg->sc3_cfg2.card_rst = CLR;
	pReg->sc3_cfg2.ck_stop = CLR;
	pReg->sc3_cfg2.cks = CKS_A2;
	pReg->sc3_cfg2.card_ck = SET;
	
	/*sc4*/
	//sc4_cfg0
	pReg->sc4_cfg0.cfg0 = 0;
	//sc4_cfg2
	pReg->sc4_cfg2.art = SET;
	pReg->sc4_cfg2.card_rst = CLR;
	pReg->sc4_cfg2.ck_stop = CLR;
	pReg->sc4_cfg2.cks = CKS_A2;
	pReg->sc4_cfg2.card_ck = SET;
	
	/*sc5*/
	//sc5_cfg0
	pReg->sc5_cfg0.cfg0 = 0;
	//sc5_cfg2
	pReg->sc5_cfg2.art = SET;
	pReg->sc5_cfg2.card_rst = CLR;
	pReg->sc5_cfg2.ck_stop = CLR;
	pReg->sc5_cfg2.cks = CKS_A2;
	pReg->sc5_cfg2.card_ck = SET;
	
	//dcdcb
	pReg->dcdcb.vdc = VCARD_VCC_5;
	//ldo
	pReg->ldo.ldo = 0xff;
	//io_select
	pReg->io_select.select = 0;
	pReg->interfaceb.interface = 0;
	pReg->itdis.itdis = 0x0f;
	
	AIC_DisableIT(AT91C_ID_IRQ0);
	AIC_ClearIT(AT91C_ID_IRQ0);
    AIC_EnableIT(AT91C_ID_IRQ0);
	c26_write_cmd2(&at83);
	c26_config_init(&at83);
	CheckIrq1Int();
	c26_write_cmd6(&at83);
	CheckIrq1Int();
	pReg->timer_msb = 0x9c;
	pReg->timer_lsb = 0x40;
	c26_write_cmd4(&at83);
	CheckIrq1Int();
	return(0);
}

unsigned char InitCardSamClock(void)
{
	unsigned char val;
	At83Reg *pReg;
	
	pReg = &at83.reg;
	//CKS1[2:0] = 4
	c26_read_cmd1_sc1_cfg2(&at83);
	val = pReg->sc1_cfg2.cfg2;
	((SC1_CFG2 *)&val)->cks = CKS_A2;		//CLK = A2
	c26_write_cmd3_sc1_cfg2(&at83,val);
	CheckIrq1Int();
	
	//CKS2[2:0] = 4
	c26_read_cmd3_sc2_cfg2(&at83);
	val = pReg->sc2_cfg2.cfg2;
	((SCX_CFG2 *)&val)->cks = CKS_A2;
	c26_write_cmd7_sc2_cfg2(&at83,val);
	CheckIrq1Int();
	
	//CKS3[2:0] = 4
	c26_read_cmd4_sc3_cfg2(&at83);
	val = pReg->sc3_cfg2.cfg2;
	((SCX_CFG2 *)&val)->cks = CKS_A2;
	c26_write_cmd8_sc3_cfg2(&at83,val);
	CheckIrq1Int();
	
	//CKS4[2:0] = 4
	c26_read_cmd5_sc4_cfg2(&at83);
	val = pReg->sc4_cfg2.cfg2;
	((SCX_CFG2 *)&val)->cks = CKS_A2;
	c26_write_cmd9_sc4_cfg2(&at83,val);
	CheckIrq1Int();
	
	//CKS5[2:0] = 4
	c26_read_cmd6_sc5_cfg2(&at83);
	val = pReg->sc5_cfg2.cfg2;
	((SCX_CFG2 *)&val)->cks = CKS_A2;
	c26_write_cmd10_sc5_cfg2(&at83,val);
	CheckIrq1Int();
	return(0);
}

unsigned char StartC26_dcdcA(void)
{
	unsigned char status;
	
	status = c26_dcdca_start(&at83);
	return(status);
}

unsigned char StopC26_dcdcA(void)
{
	unsigned char status;
	unsigned char val;
	
	val = at83.reg.sc1_cfg0.cfg0;
	val &= 0xfc;
	status = c26_write_cmd3_sc1_cfg0(&at83,val);
	return(status);
}

unsigned char StartC26_dcdcB(void)
{
	unsigned char status;
	
	status = c26_dcdcb_start(&at83);
	CheckIrq1Int();
	return(status);
}

unsigned char c26_select(unsigned char LastNo,unsigned char No)
{
	At83 *pAt83;
	At83Reg *pReg;
	unsigned char val;
	
	pAt83 = &at83;
	pReg= &pAt83->reg;
	if(LastNo == No)
	{
		return(0);
	}
	if(LastNo == 0xff)
	{
		do
		{
			WatchDog();
			c26_write_cmd6_io_select(pAt83,No);
			c26_read_cmd2_io_select(pAt83);
		}while(No != pReg->io_select.io_sel);
		return(0);
	}
	else if(LastNo == 0)
	{
		c26_read_cmd1_sc1_interface(pAt83);
		val = pReg->sc1_interface.interface;
		((SC1_INTERFACE *)&val)->card_io = SET;
		c26_write_cmd5_sc1_interface(pAt83,val);
		c26_read_cmd1_sc1_interface(pAt83);
		/*val = pReg->sc1_interface.interface;
		((SC1_INTERFACE *)&val)->io_dis = SET;
		c26_write_cmd5_sc1_interface(pAt83,val);*/
		switch(No)
		{
		case 1:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis2 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		case 2:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis3 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		case 3:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis4 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		case 4:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis5 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		}
		do
		{
			WatchDog();
			c26_write_cmd6_io_select(pAt83,No);
			c26_read_cmd2_io_select(pAt83);
		}while(No != pReg->io_select.io_sel);
	}
	else if(LastNo == 1)
	{
		c26_read_cmd2_interfaceb(pAt83);
		val = pReg->interfaceb.interface;
		((INTERFACEB *)&val)->card_io2 = SET;
		c26_write_cmd6_interfaceb(pAt83,val);
		/*c26_read_cmd2_itdis(pAt83);
		val = pReg->itdis.itdis;
		((ITDIS *)&val)->io_dis2 = SET;
		c26_write_cmd6_itdis(pAt83,val);*/
		switch(No)
		{
		case 0:
			c26_read_cmd1_sc1_interface(pAt83);
			val = pReg->sc1_interface.interface;
			((SC1_INTERFACE *)&val)->io_dis = CLR;
			c26_write_cmd5_sc1_interface(pAt83,val);
			break;
		case 2:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis3 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		case 3:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis4 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		case 4:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis5 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		}
		do
		{
			WatchDog();
			c26_write_cmd6_io_select(pAt83,No);
			c26_read_cmd2_io_select(pAt83);
		}while(No != pReg->io_select.io_sel);
	}
	else if(LastNo == 2)
	{
		c26_read_cmd2_interfaceb(pAt83);
		val = pReg->interfaceb.interface;
		((INTERFACEB *)&val)->card_io3_card_c42 = SET;
		c26_write_cmd6_interfaceb(pAt83,val);
		/*c26_read_cmd2_itdis(pAt83);
		val = pReg->itdis.itdis;
		((ITDIS *)&val)->io_dis3 = SET;
		c26_write_cmd6_itdis(pAt83,val);*/
		switch(No)
		{
		case 0:
			c26_read_cmd1_sc1_interface(pAt83);
			val = pReg->sc1_interface.interface;
			((SC1_INTERFACE *)&val)->io_dis = CLR;
			c26_write_cmd5_sc1_interface(pAt83,val);
			break;
		case 1:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis2 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		case 3:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis4 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		case 4:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis5 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		}
		do
		{
			WatchDog();
			c26_write_cmd6_io_select(pAt83,No);
			c26_read_cmd2_io_select(pAt83);
		}while(No != pReg->io_select.io_sel);
	}
	else if(LastNo == 3)
	{
		c26_read_cmd2_interfaceb(pAt83);
		val = pReg->interfaceb.interface;
		((INTERFACEB *)&val)->card_io4 = SET;
		c26_write_cmd6_interfaceb(pAt83,val);
		/*c26_read_cmd2_itdis(pAt83);
		val = pReg->itdis.itdis;
		((ITDIS *)&val)->io_dis4 = SET;
		c26_write_cmd6_itdis(pAt83,val);*/
		switch(No)
		{
		case 0:
			c26_read_cmd1_sc1_interface(pAt83);
			val = pReg->sc1_interface.interface;
			((SC1_INTERFACE *)&val)->io_dis = CLR;
			c26_write_cmd5_sc1_interface(pAt83,val);
			break;
		case 1:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis2 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		case 2:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis3 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		case 4:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis5 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		}
		do
		{
			WatchDog();
			c26_write_cmd6_io_select(pAt83,No);
			c26_read_cmd2_io_select(pAt83);
		}while(No != pReg->io_select.io_sel);
	}
	else
	{
		c26_read_cmd2_interfaceb(pAt83);
		val = pReg->interfaceb.interface;
		((INTERFACEB *)&val)->card_io5 = SET;
		c26_write_cmd6_interfaceb(pAt83,val);
		/*c26_read_cmd2_itdis(pAt83);
		val = pReg->itdis.itdis;
		((ITDIS *)&val)->io_dis5 = SET;
		c26_write_cmd6_itdis(pAt83,val);*/
		switch(No)
		{
		case 0:
			c26_read_cmd1_sc1_interface(pAt83);
			val = pReg->sc1_interface.interface;
			((SC1_INTERFACE *)&val)->io_dis = CLR;
			c26_write_cmd5_sc1_interface(pAt83,val);
			break;
		case 1:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis2 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		case 2:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis3 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		case 3:
			c26_read_cmd2_itdis(pAt83);
			val = pReg->itdis.itdis;
			((ITDIS *)&val)->io_dis4 = CLR;
			c26_write_cmd6_itdis(pAt83,val);
			break;
		}
		do
		{
			WatchDog();
			c26_write_cmd6_io_select(pAt83,No);
			c26_read_cmd2_io_select(pAt83);
		}while(No != pReg->io_select.io_sel);
	}
	//delay(4000);
	return(0);
}

unsigned char StartCardSamPower(unsigned char channel)
{
	unsigned char i;
	unsigned char error = 0;
	At83 *pAt83;
	At83Reg *pReg;
	
	pAt83 = &at83;
	pReg = &pAt83->reg;
	switch(channel)
	{
	case ICCARD:
		c26_start_cvcc1(pAt83);
		break;
	case SAM0:
		for(i = 0; i < 3; i++)
		{
			c26_start_cvcc2(pAt83);
			pAt83->TimeFlg = 0;
			CreateTimer1(TIMER_ONE_DELETE,100,pAt83->RunTime);
			while(!pAt83->TimeFlg)
			{
				CheckTimer1Go();
			}
			c26_read_cmd3_sc2_cfg0(pAt83);
			if((pReg->sc2_cfg0.vcard_ok == SET) && (pReg->sc2_cfg0.vcard == VCC_5))
			{
				CardStatus |= SAM0_POWER_ON;
				break;
			}
		}
		if(i == 3)
		{
			error = 0x02;
		}
		break;
	case SAM1:
		for(i = 0; i < 3; i++)
		{
			c26_start_cvcc3(&at83);
			pAt83->TimeFlg = 0;
			CreateTimer1(TIMER_ONE_DELETE,100,pAt83->RunTime);
			while(!pAt83->TimeFlg)
			{
				CheckTimer1Go();
			}
			c26_read_cmd4_sc3_cfg0(pAt83);
			if((pReg->sc3_cfg0.vcard_ok == SET) && (pReg->sc3_cfg0.vcard == VCC_5))
			{
				CardStatus |= SAM1_POWER_ON;
				break;
			}
		}
		if(i == 3)
		{
			error = 0x04;
		}
		break;
	case SAM2:
		for(i = 0; i < 3; i++)
		{
			c26_start_cvcc4(&at83);
			pAt83->TimeFlg = 0;
			CreateTimer1(TIMER_ONE_DELETE,100,pAt83->RunTime);
			while(!pAt83->TimeFlg)
			{
				CheckTimer1Go();
			}
			c26_read_cmd5_sc4_cfg0(pAt83);
			if((pReg->sc4_cfg0.vcard_ok == SET) && (pReg->sc4_cfg0.vcard == VCC_5))
			{
				CardStatus |= SAM2_POWER_ON;
				break;
			}
		}
		if(i == 3)
		{
			error = 0x08;
		}
		break;
	case SAM3:
		for(i = 0; i < 3; i++)
		{
			c26_start_cvcc5(&at83);
			pAt83->TimeFlg = 0;
			CreateTimer1(TIMER_ONE_DELETE,100,pAt83->RunTime);
			while(!pAt83->TimeFlg)
			{
				CheckTimer1Go();
			}
			c26_read_cmd6_sc5_cfg0(pAt83);
			if((pReg->sc5_cfg0.vcard_ok == SET) && (pReg->sc5_cfg0.vcard == VCC_5))
			{
				CardStatus |= SAM3_POWER_ON;
				break;
			}
		}
		if(i == 3)
		{
			error = 0x10;
		}
		break;
	case ALL:
		c26_start_cvcc1(&at83);
		for(i = 0; i < 3; i++)
		{
			c26_start_cvcc2(pAt83);
			pAt83->TimeFlg = 0;
			CreateTimer1(TIMER_ONE_DELETE,100,pAt83->RunTime);
			while(!pAt83->TimeFlg)
			{
				CheckTimer1Go();
			}
			c26_read_cmd3_sc2_cfg0(pAt83);
			if((pReg->sc2_cfg0.vcard_ok == SET) && (pReg->sc2_cfg0.vcard == VCC_5))
			{
				CardStatus |= SAM0_POWER_ON;
				break;
			}
		}
		if(i == 3)
		{
			error |= 0x02;
		}
		for(i = 0; i < 3; i++)
		{
			c26_start_cvcc3(&at83);
			pAt83->TimeFlg = 0;
			CreateTimer1(TIMER_ONE_DELETE,100,pAt83->RunTime);
			while(!pAt83->TimeFlg)
			{
				CheckTimer1Go();
			}
			c26_read_cmd4_sc3_cfg0(pAt83);
			if((pReg->sc3_cfg0.vcard_ok == SET) && (pReg->sc3_cfg0.vcard == VCC_5))
			{
				CardStatus |= SAM1_POWER_ON;
				break;
			}
		}
		if(i == 3)
		{
			error |= 0x04;
		}
		for(i = 0; i < 3; i++)
		{
			c26_start_cvcc4(&at83);
			pAt83->TimeFlg = 0;
			CreateTimer1(TIMER_ONE_DELETE,100,pAt83->RunTime);
			while(!pAt83->TimeFlg)
			{
				CheckTimer1Go();
			}
			c26_read_cmd5_sc4_cfg0(pAt83);
			if((pReg->sc4_cfg0.vcard_ok == SET) && (pReg->sc4_cfg0.vcard == VCC_5))
			{
				CardStatus |= SAM2_POWER_ON;
				break;
			}
		}
		if(i == 3)
		{
			error |= 0x08;
		}
		for(i = 0; i < 3; i++)
		{
			c26_start_cvcc5(&at83);
			pAt83->TimeFlg = 0;
			CreateTimer1(TIMER_ONE_DELETE,100,pAt83->RunTime);
			while(!pAt83->TimeFlg)
			{
				CheckTimer1Go();
			}
			c26_read_cmd6_sc5_cfg0(pAt83);
			if((pReg->sc5_cfg0.vcard_ok == SET) && (pReg->sc5_cfg0.vcard == VCC_5))
			{
				CardStatus |= SAM3_POWER_ON;
				break;
			}
		}
		if(i == 3)
		{
			error |= 0x10;
		}
		break;
	default:
		error = 0xff;
	}
	return(error);
}

unsigned char CardSamWarmReset(unsigned char channel)
{
	unsigned char val,status;
	At83Reg *pReg;
	
	pReg = &at83.reg;
	if(channel > SAM3)
	{
		return(0xff);
	}
	WRITE_USART(iso.pUsartHw,US_CR,AT91C_US_RXDIS);
	status = c26_select(iso.no,channel);
	if(!status)
	{
		iso.no = channel;
	}
	else
	{
		status = (channel + 1) << 4;
		return(status);
	}
	at83ch[channel].fidi = 372;
	at83ch[channel].baud = USART1_BAUD;
	at83ch[channel].mode.us_mode = AT91C_US_USMODE_ISO7816_0;
	at83ch[channel].mode.us_clks = CLKS_CLOCK;
	at83ch[channel].mode.nb_stop = NBSTOP_1_BIT;
	at83ch[channel].mode.par = PAR_EVEN;
	at83ch[channel].mode.chrl = CHRL_8_BITS;
	at83ch[channel].mode.clko = US_CKLO;
	at83ch[channel].mode.max_iteration = MAX_ITER(3);
	iso.no = channel;
	iso.pAt83ch = &at83ch[channel];
	iso.RevLen = 0;
	iso.pAt83ch->status = GET_ATR;
	iso.pAt83ch->Flg.flg = 0;
	AnewInitUsart1(channel);
	//US_EnableIt(AT91C_BASE_US1,	AT91C_US_RXRDY);
	switch(channel)
	{
	case ICCARD:
		c26_read_cmd1_sc1_cfg1(&at83);
		val = pReg->sc1_interface.interface;
		((SC1_INTERFACE *)&val)->io_dis = CLR;
		((SC1_INTERFACE *)&val)->ck_stop = CLR;
		((SC1_INTERFACE *)&val)->card_rst = SET;
		do
		{
			WatchDog();
			c26_write_cmd5_sc1_interface(&at83,val);
			c26_read_cmd1_sc1_cfg1(&at83);
			status = pReg->sc1_interface.interface;
		}while(val != status);
		c26_read_cmd1_sc1_cfg1(&at83);
		val = pReg->sc1_cfg1.cfg1;
		((SC1_CFG1 *)&val)->art = CLR;
		do
		{
			WatchDog();
			c26_write_cmd3_sc1_cfg1(&at83,val);
			c26_read_cmd1_sc1_cfg1(&at83);
			status = pReg->sc1_cfg1.cfg1;
		}while(val != status);
		val = pReg->sc1_cfg1.cfg1;
		((SC1_CFG1 *)&val)->art = SET;
		do
		{
			WatchDog();
			c26_write_cmd3_sc1_cfg1(&at83,val);
			c26_read_cmd1_sc1_cfg1(&at83);
			status = pReg->sc1_cfg1.cfg1;
		}while(val != status);
		status = 0;
		break;
	case SAM0:
		c26_read_cmd3_sc2_cfg2(&at83);
		val = pReg->sc2_cfg2.cfg2;
		((SCX_CFG2 *)&val)->cks = CKS_A2;
		((SCX_CFG2 *)&val)->card_rst = SET;
		((SCX_CFG2 *)&val)->ck_stop = CLR;
		((SCX_CFG2 *)&val)->art = CLR;
		((SCX_CFG2 *)&val)->card_ck = CLR;
		do
		{
			WatchDog();
			c26_write_cmd7_sc2_cfg2(&at83,val);
			c26_read_cmd3_sc2_cfg2(&at83);
			status = pReg->sc2_cfg2.cfg2;
		}while(val != status);
		((SCX_CFG2 *)&val)->cks = CKS_A2;
		((SCX_CFG2 *)&val)->card_rst = SET;
		((SCX_CFG2 *)&val)->ck_stop = CLR;
		((SCX_CFG2 *)&val)->art = SET;
		((SCX_CFG2 *)&val)->card_ck = CLR;
		do
		{
			WatchDog();
			c26_write_cmd7_sc2_cfg2(&at83,val);
			c26_read_cmd3_sc2_cfg2(&at83);
			status = pReg->sc2_cfg2.cfg2;
		}while(val != status);
		status = 0;
		break;
	case SAM1:
		c26_read_cmd4_sc3_cfg2(&at83);
		val = pReg->sc3_cfg2.cfg2;
		((SCX_CFG2 *)&val)->cks = CKS_A2;
		((SCX_CFG2 *)&val)->card_rst = SET;
		((SCX_CFG2 *)&val)->ck_stop = CLR;
		((SCX_CFG2 *)&val)->art = CLR;
		((SCX_CFG2 *)&val)->card_ck = CLR;
		do
		{
			WatchDog();
			c26_write_cmd8_sc3_cfg2(&at83,val);
			c26_read_cmd4_sc3_cfg2(&at83);
			status = pReg->sc3_cfg2.cfg2;
		}while(val != status);
		((SCX_CFG2 *)&val)->cks = CKS_A2;
		((SCX_CFG2 *)&val)->card_rst = SET;
		((SCX_CFG2 *)&val)->ck_stop = CLR;
		((SCX_CFG2 *)&val)->art = SET;
		((SCX_CFG2 *)&val)->card_ck = CLR;
		do
		{
			WatchDog();
			c26_write_cmd8_sc3_cfg2(&at83,val);
			c26_read_cmd4_sc3_cfg2(&at83);
			status = pReg->sc3_cfg2.cfg2;
		}while(val != status);
		status = 0;
		break;
	case SAM2:
		c26_read_cmd5_sc4_cfg2(&at83);
		val = pReg->sc4_cfg2.cfg2;
		((SCX_CFG2 *)&val)->cks = CKS_A2;
		((SCX_CFG2 *)&val)->card_rst = SET;
		((SCX_CFG2 *)&val)->ck_stop = CLR;
		((SCX_CFG2 *)&val)->art = CLR;
		((SCX_CFG2 *)&val)->card_ck = CLR;
		do
		{
			WatchDog();
			c26_write_cmd9_sc4_cfg2(&at83,val);
			c26_read_cmd5_sc4_cfg2(&at83);
			status = pReg->sc4_cfg2.cfg2;
		}while(val != status);
		((SCX_CFG2 *)&val)->cks = CKS_A2;
		((SCX_CFG2 *)&val)->card_rst = SET;
		((SCX_CFG2 *)&val)->ck_stop = CLR;
		((SCX_CFG2 *)&val)->art = SET;
		((SCX_CFG2 *)&val)->card_ck = CLR;
		do
		{
			WatchDog();
			c26_write_cmd9_sc4_cfg2(&at83,val);
			c26_read_cmd5_sc4_cfg2(&at83);
			status = pReg->sc4_cfg2.cfg2;
		}while(val != status);
		status = 0;
		break;
	case SAM3:
		c26_read_cmd6_sc5_cfg2(&at83);
		val = pReg->sc5_cfg2.cfg2;
		((SCX_CFG2 *)&val)->cks = CKS_A2;
		((SCX_CFG2 *)&val)->card_rst = SET;
		((SCX_CFG2 *)&val)->ck_stop = CLR;
		((SCX_CFG2 *)&val)->art = CLR;
		((SCX_CFG2 *)&val)->card_ck = CLR;
		do
		{
			WatchDog();
			c26_write_cmd10_sc5_cfg2(&at83,val);
			c26_read_cmd6_sc5_cfg2(&at83);
			status = pReg->sc5_cfg2.cfg2;
		}
		while(val != status);
		((SCX_CFG2 *)&val)->cks = CKS_A2;
		((SCX_CFG2 *)&val)->card_rst = SET;
		((SCX_CFG2 *)&val)->ck_stop = CLR;
		((SCX_CFG2 *)&val)->art = SET;
		((SCX_CFG2 *)&val)->card_ck = CLR;
		do
		{
			WatchDog();
			c26_write_cmd10_sc5_cfg2(&at83,val);
			c26_read_cmd6_sc5_cfg2(&at83);
			status = pReg->sc5_cfg2.cfg2;
		}
		while(val != status);
		status = 0;
		break;
	default:
		status = 0xff;
		break;
	}
	return(status);
}