/* EasyCODE V8 */
/* EasyCODE ( 0 */
#include "global.h"
#if LCD_TYPE==0	//OK
static const unsigned short ZimoCrc = 0xf9db;
#else
static const unsigned short ZimoCrc = 0x4d51;
#endif
//#define _DBUG_

static const unsigned char InitFormating[] = "税控正在初始化";

void PowenOnDelay(void)
{
	IdleFlg = 1;
}

void InitSys(void)
{
    unsigned int reset_state;
    
    reset_state = AT91F_RSTGetStatus(AT91C_BASE_RSTC);
    reset_typ = (reset_state >> 8) & 0x07;
	InitIO();
	InitMemList();
	InitUSART0();
	
	InitDbugUSART();
	InitTimer0();
	InitTimer1();
	InitPIT();
	InitSMC();
	InitLcd();
	__enable_interrupt();
	TC_Start(AT91C_BASE_TC0);
	TC_Start(AT91C_BASE_TC1);
	InitSPI();
	PIT_Start();
	InitTWI();
	CreateTimer(TIMER_AUTO,500,RunLed);
	//IdleFlg = 1;
	pTmrInfrared = CreateTimer(TIMER_ONE,30 * 1000,infraredDelay);
	CheckZimo();
	//CreateTimer(TIMER_AUTO,30,ScanKey);
	LcdBackLightControl();
	DisplayVerNumber();
	
	
	if(IcTypeFlg)
	{
		CardOut();
		//while(CardFlg == 1)
		//{
		//	CheckTimerGo();
		//}
		CheckIrq1Int();
		Iso7816WarmReset();
		InitCardInterface();
		InitCardSamClock();	
		StartC26_dcdcB();
		CardSamWarmReset(1);
		CardSamWarmReset(2);
		CardSamWarmReset(3);
		CardSamWarmReset(4);
		CheckIrq1Int();
		CreateTimer(TIMER_AUTO,50,At83C26Check);
		//CardOut();
	}
	InitUSART1();
	PIO_SetInput(fiq.pio,fiq.mask,DISABLE_PULLUP,ENABLE_FILETER);
	PMC_EnablePeripheral(AT91C_ID_PIOB);
	while(!CheckPowerDownPin())
	{
		WatchDog();
	}
	DisPlayHelp(InitFormating);
	IdleFlg = 0;
	
	//liuzg start 20121105
	if(reset_typ == 0)
	{
		CreateTimer(TIMER_ONE_DELETE,6 * 1000,PowenOnDelay);
		while(!IdleFlg)
		{
			CheckTimerGo();
			WatchDog();
		}
		IdleFlg = 0;
	}
	//liuzg end 20121105
	if(!CheckPowerDownPin())
	{
		while(1)
		{
		}
	}
#if LCD_TYPE==0		//OK
	ClearLcd(2,9,126,49);
	ClearLcd(2,51,126,62);
	DisplayLogo(); 
#else
	ClearLcd(2,21,317,218);
	DisplayLogo();
#endif
	DisBufToLcd();
	PIO_SetPeripheralA(fiq.pio,fiq.mask,PIO_PULLUP);
    PMC_EnablePeripheral(AT91C_ID_FIQ);
    AIC_ConfigureIT(AT91C_ID_FIQ,IRQ_INTERRUPT_PRIORITY | AT91C_AIC_SRCTYPE_INT_POSITIVE_EDGE,ISR_fiq);
    AIC_EnableIT(AT91C_ID_FIQ);
	CreateTimer(TIMER_AUTO,30,ScanKey);
	pTmrButton = CreateTimer(TIMER_AUTO,150,ScanButton);
	StopTimer(pTmrButton);
	//CreateTimer(TIMER_AUTO,50,At83C26Check);	
}

/* EasyCODE ( 0 
						InitIO
function describe:Init input and output
param:none
return:none
call fucntion:PIO_SetOutput,PIO_SetInput,PIO_SetPeripheralA
	PMC_EnablePeripheral,AIC_Configure,AIC_EnableIT
global variable:run_led,beep,lcd_bl,irq1,fiq,irq0,card_open,card_in
author:liuzg
date:2009-3-22
version:.01 */
/* EasyCODE F */
void InitIO(void)
{
	/*PIO_SetInput(fiq.pio,fiq.mask,DISABLE_PULLUP,ENABLE_FILETER);
	PMC_EnablePeripheral(AT91C_ID_PIOB);
	while(!CheckPowerDownPin())
	{
		WatchDog();
	}*/
    //PA0~PA2
    PIO_SetOutput(run_led.pio,run_led.mask,CLOSE,DISABLE_MULTIDRIVE,DISABLE_PULLUP);
    PIO_SetOutput(beep.pio,beep.mask,CLOSE,DISABLE_MULTIDRIVE,DISABLE_PULLUP);
    PIO_SetOutput(lcd_bl.pio,lcd_bl.mask,CLOSE,DISABLE_MULTIDRIVE,DISABLE_PULLUP);
	//PA23
	PIO_SetOutput(twi_adr.pio,twi_adr.mask,OPEN,DISABLE_MULTIDRIVE,ENABLE_PULLUP);
    //PA24
    PIO_SetInput(usbcnx.pio,usbcnx.mask,ENABLE_PULLUP,ENABLE_FILETER);
	//PA25
	PIO_SetInput(prn_cts.pio,prn_cts.mask,ENABLE_PULLUP,ENABLE_FILETER);
	PMC_EnablePeripheral(AT91C_ID_PIOA);
    //PA30
    PIO_SetPeripheralA(irq1.pio,irq1.mask,PIO_PULLUP);
    PMC_EnablePeripheral(AT91C_ID_IRQ1);
    AIC_ConfigureIT(AT91C_ID_IRQ1,IRQ_INTERRUPT_PRIORITY | AT91C_AIC_SRCTYPE_EXT_NEGATIVE_EDGE,ISR_irq1);
    //AIC_ClearIT(AT91C_ID_IRQ1);
    //AIC_EnableIT(AT91C_ID_IRQ1);
	//PB0
	PIO_SetInput(jump.pio,jump.mask,ENABLE_PULLUP,ENABLE_FILETER);
    //PB19
    /*PIO_SetPeripheralA(fiq.pio,fiq.mask,PIO_PULLUP);
    PMC_EnablePeripheral(AT91C_ID_FIQ);
    AIC_ConfigureIT(AT91C_ID_FIQ,IRQ_INTERRUPT_PRIORITY | AT91C_AIC_SRCTYPE_INT_POSITIVE_EDGE,ISR_fiq);
    AIC_EnableIT(AT91C_ID_FIQ);*/
    //PB20
    PIO_SetPeripheralA(irq0.pio,irq0.mask,PIO_OPENDRAIN);
    PMC_EnablePeripheral(AT91C_ID_IRQ0);
    AIC_ConfigureIT(AT91C_ID_IRQ0,IRQ_INTERRUPT_PRIORITY | AT91C_AIC_SRCTYPE_EXT_NEGATIVE_EDGE,ISR_irq0);
    //AIC_EnableIT(AT91C_ID_IRQ0);
    //PB21
    PIO_SetOutput(card_open.pio,card_open.mask, CLOSE,DISABLE_MULTIDRIVE,DISABLE_PULLUP);
    //PB22
    PIO_SetInput(card_in.pio,card_in.mask,ENABLE_PULLUP,ENABLE_FILETER);
	PMC_EnablePeripheral(AT91C_ID_PIOB);
	IcTypeFlg = ReadCardConfig();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
				InitTWI
function describe:Init TWI interface
param:none
return:none
call fucntion:PIO_SetPeripheralA,TWI_Configure,
	PMC_EnablePeriapherial,TWID_Initialize
global variable:pin_twi,twid,at24
author:liuzg
date:2009-3-22
version:.01 */
/* EasyCODE F */
static const unsigned char IC_INIT_ERROR[] = "AT83C26 error!";
void InitTWI(void)
{
	unsigned char status;
    if(IcTypeFlg)
	{
		AT91S_RSTC *pRstc;
		At83 *pAt83;
		//PA3,PA4
		PIO_SetPeripheralA(pin_twi.pio,pin_twi.mask,DISABLE_PULLUP);
		//Clock
		TWI_Configure(AT91C_BASE_TWI,TWI_DEFAULT_CLK,MAIN_MCK);
		//PMC
		PMC_EnablePeripheral(AT91C_ID_TWI);
		TWID_Initialize(&twid,AT91C_BASE_TWI);
		memset((unsigned char *)(&at83),0,sizeof(At83));
		at83.DeviceAdr = AT83_ADDR1;
		at83.pTwid = &twid;
		at83.TimeFlg = 0;
		at83.RunTime = At83TimeOut;
		
		pAt83 = &at83;
		pRstc = AT91C_BASE_RSTC;
		PIO_Set(&twi_adr);
		pAt83->TimeOutFlg = 0;
		CreateTimer1(TIMER_ONE_DELETE,50,TwiTimeOut);
		while(!pAt83->TimeOutFlg)
		{
			CheckTimer1Go();
		}
		WatchDog();
		pRstc->RSTC_RMR = 0xa5000401;
		pRstc->RSTC_RCR = 0xa5000008;
		while(!(pRstc->RSTC_RSR & 0x01))
		{
		}
		pAt83->TimeOutFlg = 0;
		CreateTimer1(TIMER_ONE_DELETE,400,TwiTimeOut);
		while(!pAt83->TimeOutFlg)
		{
			WatchDog();
			CheckTimer1Go();
		}
		
		status = c26_read_cmd1_init(&at83);
		if(!status)
		{
			return;
		}
		at83.DeviceAdr = AT83_ADDR2;
		status = c26_read_cmd1_init(&at83);
		if(!status)
		{
			return;
		}
		at83.DeviceAdr = AT83_ADDR3;
		status = c26_read_cmd1_init(&at83);
		if(!status)
		{
			return;
		}
		at83.DeviceAdr = AT83_ADDR4;
		status = c26_read_cmd1_init(&at83);
		if(!status)
		{
			return;
		}
		DisPlayHelp(IC_INIT_ERROR);
		while(1)
		{
			WatchDog();
		}
	}
}

/* EasyCODE ) */
/* EasyCODE ( 0 
						InitUSART0
function describe:Init USART0 interface
param:none
return:none
call fucntion:PIO_SetPeripheralA,PMC_EnablePeripheral,AIC_ConfigureIT,
	memset,USART_ConfigStruct,USART_Configure,PDC_DisableRx,
	PDC_DisableTx,US_EnableIt,AIC_EnableIT
global variable:usart0,Serial0
author:liuzg
date:2009-3-22
version:.01 */
/* EasyCODE F */
void InitUSART0(void)
{
    unsigned int mode;
    //PA5,PA6,PA7
    PIO_SetPeripheralA(usart0.pio,usart0.mask,ENABLE_PULLUP);
    //PMC
    PMC_EnablePeripheral(AT91C_ID_US0);
    //AIC
    AIC_ConfigureIT(AT91C_ID_US0,
    			USART0_INTERRUPT_PRIORITY |AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
    			ISR_usart0);
    
    memset(&Serial0,0,sizeof(Serial0));
    //US
    USART_ConfigStruct(&Serial0,AT91C_BASE_US0);
    
    mode = AT91C_US_USMODE_RS485 | AT91C_US_CHRL_8_BITS | 
    AT91C_US_NBSTOP_1_BIT | AT91C_US_PAR_NONE | AT91C_US_CLKS_CLOCK;
    /*mode = AT91C_US_USMODE_RS485 | AT91C_US_CHRL_8_BITS | 
    AT91C_US_NBSTOP_1_BIT | AT91C_US_PAR_NONE | AT91C_US_CLKS_CLOCK | AT91C_US_CHMODE_LOCAL;*/
    USART_Configure(AT91C_BASE_US0,mode,USART0_BAUD,MAIN_MCK);
    //WRITE_USART(Serial0.pUsartHw,US_CR,AT91C_US_RTSEN);
    PDC_DisableRx((AT91PS_PDC)(&AT91C_BASE_US0->US_RPR));
    PDC_DisableTx((AT91PS_PDC)(&AT91C_BASE_US0->US_RPR));
    //INT
    US_EnableIt(AT91C_BASE_US0, 
    		AT91C_US_RXRDY | AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE);
    WRITE_USART(Serial0.pUsartHw,US_CR,AT91C_US_TXEN | AT91C_US_RXEN);
    AIC_EnableIT(AT91C_ID_US0);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
						InitUSART1
function describe:Init USART1 interface
param:none
return:none
call fucntion:PIO_SetPeripheralA,PMC_EnablePeripherial,AIC_ConfigureIT,
	memset,USART_ConfigStruct,USART_Configure,PDC_DisableRx,
	PDC_DisableTx,US_EnableIt,AIC_EnableIT
global variable:usart1,Serial1
author:liuzg
date:2009-3-22
version:.01 */
/* EasyCODE F */
void InitUSART1(void)
{
	if(IcTypeFlg)
	{
		//IO
		PIO_SetPeripheralA(iso7816.pio,iso7816.mask,DISABLE_PULLUP);
		//US
		memset(&iso,0,sizeof(iso));
		iso.pUsartHw = AT91C_BASE_US1;
		iso.no = 0xff;
		at83ch[0].mode.us_mr = AT91C_US_USMODE_ISO7816_0
						| AT91C_US_CLKS_CLOCK
						| AT91C_US_NBSTOP_1_BIT
						| AT91C_US_PAR_EVEN
						| AT91C_US_CHRL_8_BITS
						| AT91C_US_CKLO
						| (3<<24);
		at83ch[0].fidi = 372;
		at83ch[0].baud = USART1_BAUD;
		at83ch[0].us_ttgr = 5;
		memcpy(&at83ch[1],&at83ch[0],sizeof(At83Ch));
		memcpy(&at83ch[2],&at83ch[0],sizeof(At83Ch));
		memcpy(&at83ch[3],&at83ch[0],sizeof(At83Ch));
		memcpy(&at83ch[4],&at83ch[0],sizeof(At83Ch));
		
		//PMC
		PMC_EnablePeripheral(AT91C_ID_US1);
		AT91C_BASE_US1->US_IDR = (unsigned int) -1;
		//AIC
		AIC_ConfigureIT(AT91C_ID_US1,
					USART1_INTERRUPT_PRIORITY |AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
					ISR_usart1);
		//AT91C_BASE_US1->US_IDR = (unsigned int)-1;
		iso.pUsartHw->US_MR = at83ch[0].mode.us_mr;
		AT91C_BASE_US1->US_FIDI = at83ch[0].fidi;
		AT91C_BASE_US1->US_BRGR = MAIN_MCK / (at83ch[0].fidi * at83ch[0].baud);
		AT91C_BASE_US1->US_TTGR = at83ch[0].us_ttgr;
		PDC_DisableRx((AT91PS_PDC)(&AT91C_BASE_US1->US_RPR));
		PDC_DisableTx((AT91PS_PDC)(&AT91C_BASE_US1->US_RPR));
		US_EnableIt(AT91C_BASE_US1, 
				AT91C_US_RXRDY);
		//WRITE_USART(iso.pUsartHw,US_CR,AT91C_US_RXEN);// | AT91C_US_TXEN);
		AIC_ClearIT(AT91C_ID_US1);
		AIC_EnableIT(AT91C_ID_US1);
	}
	else
	{
		unsigned int mode;
		
		//IO
		PIO_SetPeripheralA(usart1.pio,usart1.mask,ENABLE_PULLUP);
		//PMC
		PMC_EnablePeripheral(AT91C_ID_US1);
		//AIC
		AIC_ConfigureIT(AT91C_ID_US1,
					USART1_INTERRUPT_PRIORITY |AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
					ISR_usart1);
		memset(&Serial1,0,sizeof(Serial1));
		//US
		USART_ConfigStruct(&Serial1,AT91C_BASE_US1);
		mode = AT91C_US_USMODE_NORMAL | AT91C_US_CHRL_8_BITS | 
		AT91C_US_NBSTOP_1_BIT | AT91C_US_PAR_NONE | AT91C_US_CLKS_CLOCK;
		/*mode = AT91C_US_USMODE_NORMAL | AT91C_US_CHRL_8_BITS | 
		AT91C_US_NBSTOP_1_BIT | AT91C_US_PAR_NONE | AT91C_US_CLKS_CLOCK | AT91C_US_CHMODE_LOCAL;*/
		USART_Configure(AT91C_BASE_US1,mode,USART1_BAUD,MAIN_MCK);
		PDC_DisableRx((AT91PS_PDC)(&AT91C_BASE_US1->US_RPR));
		PDC_DisableTx((AT91PS_PDC)(&AT91C_BASE_US1->US_RPR));
		//INT
		US_EnableIt(AT91C_BASE_US1, 
				AT91C_US_RXRDY | AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE);
		WRITE_USART(Serial1.pUsartHw,US_CR,AT91C_US_TXEN | AT91C_US_RXEN);
		AIC_EnableIT(AT91C_ID_US1);
	}
}

void InitDbugUSART(void)
{
    unsigned int mode;
    
    PIO_SetPeripheralA(dbug_usart.pio,dbug_usart.mask,ENABLE_PULLUP);
    mode = AT91C_US_PAR_NONE | AT91C_US_CHMODE_NORMAL;
    DbugUsart_Configure(AT91C_BASE_DBGU,mode,DBUG_USART_BAUD,MAIN_MCK);
    PDC_DisableRx((AT91PS_PDC)(&AT91C_BASE_DBGU->DBGU_RPR));
    PDC_DisableTx((AT91PS_PDC)(&AT91C_BASE_DBGU->DBGU_TPR));
    //INT
    //DBUG_US_EnableIt(AT91C_BASE_DBGU, 
    //		AT91C_US_RXRDY);
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;//AT91C_US_RXEN | AT91C_US_TXEN;
	DbugSerial.pUsartHw = AT91C_BASE_DBGU;

	// Bellow LLLLA
//	DbugSerial.sendback = DbugCashSentOver;
}

void InitDbugUSART_Func(void)
{
	unsigned int mode;
	
	PIO_SetPeripheralA(dbug_usart.pio,dbug_usart.mask,ENABLE_PULLUP);
	memset(&DbugSerial,0,sizeof(DbugSerial));
	DbugSerial.pUsartHw = AT91C_BASE_DBGU;
    mode = AT91C_US_PAR_NONE | AT91C_US_CHMODE_NORMAL;
	DbugUsart_Configure(AT91C_BASE_DBGU,mode,DBUG_USART_BAUD,MAIN_MCK);
	PDC_DisableRx((AT91PS_PDC)(&AT91C_BASE_DBGU->DBGU_RPR));
    PDC_DisableTx((AT91PS_PDC)(&AT91C_BASE_DBGU->DBGU_TPR));
    //INT
    DBUG_US_EnableIt(AT91C_BASE_DBGU, 
    		AT91C_US_RXRDY);
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN | AT91C_US_TXEN;
	DbugSerial.receviceback = DbugStartRecevice;
}


/* EasyCODE ) */
/* EasyCODE ( 0 
					InitTimer0
function describe:Init TC0 interface
param:none
return:none
call fucntion:PMC_EnablePeripheral,TC_Configure,TC_RCConfigure,
	AIC_ConfigureIT,AIC_EnableIT
global variable:pHead
author:liuzg
date:2009-3-22
version:.01 */
/* EasyCODE F */
void InitTimer0(void)
{
    unsigned int i;
    //PMC
    PMC_EnablePeripheral(AT91C_ID_TC0); 
    //TC
    TC_Configure(AT91C_BASE_TC0,TC_CLKS_MCK1024 | AT91C_TC_CPCTRG);
    AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;
    
    
    //TC
    TC_RCConfigure(AT91C_BASE_TC0,TIMER_MS, MAIN_MCK);
    //enable CPC interrupt
    AT91C_BASE_TC0->TC_IER  = AT91C_TC_CPCS;		
    
    i = AT91C_BASE_TC0->TC_SR;
    i = i;
    //AIC
    AIC_ConfigureIT(AT91C_ID_TC0,
    			TIME_PRIORITY | AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
    			ISR_timer0);
    AIC_EnableIT(AT91C_ID_TC0);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
					InitTimer1
function describe:Init TC0 interface
param:none
return:none
call fucntion:PMC_EnablerPeripheral,TC_Configure,TC_RCConfigure,
	AIC_ConfigureIT,AIC_EnableIT
global variable:none
author:liuzg
date:2009-6-1
version:.01 */
/* EasyCODE F */
void InitTimer1(void)
{
    unsigned int i;
    //PMC
    PMC_EnablePeripheral(AT91C_ID_TC1);
    //TC
    TC_Configure(AT91C_BASE_TC1,TC_CLKS_MCK1024 | AT91C_TC_CPCTRG);
    AT91C_BASE_TC1->TC_IER = AT91C_TC_CPCS;
    
    //TC
    TC_RCConfigure(AT91C_BASE_TC1,TIMER_10MS,MAIN_MCK);
    //enable CPC interrupt
    AT91C_BASE_TC1->TC_IER = AT91C_TC_CPCS;
    
    i = AT91C_BASE_TC1->TC_SR;
    i = i;
    //AIC
    AIC_ConfigureIT(AT91C_ID_TC1,
    			TIME1_PRIORITY | AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
    			ISR_timer1);
    AIC_EnableIT(AT91C_ID_TC1);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
				InitPIT
function describe:Init PIT interface
param:none
return:none
call fucntion:PMC_EnablePeripheral,AIC_ConfigureIT,
	PIT_Configure,AIC_EnableIT
global variable:none
author:liuzg
date:2009-4-24
version:.01 */
/* EasyCODE F */
void InitPIT(void)
{
    PMC_EnablePeripheral(AT91C_ID_SYS);
    AIC_ConfigureIT(AT91C_ID_SYS,AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,Sys_irq);
    PIT_Configure(CYC_MS(250));
    AIC_EnableIT(AT91C_ID_SYS);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
				InitSMC
function describe:Init SMC interface
param:none
return:none
call fucntion:PIO_SetPeripheralA,PIO_SetPeripheralB,
	SMC_SetCsr,GerRs485Adr,InitKeyStruct
global variable:data_buf,addr_bus,ctrl_bus,Rs485Adr
author:liuzg
date:2009-3-22
version:.01 */
/* EasyCODE F */
void InitSMC(void)
{
    PIO_SetPeripheralA(data_bus.pio,data_bus.mask,ENABLE_PULLUP);
    PIO_SetPeripheralA(addr_bus.pio,addr_bus.mask,ENABLE_PULLUP);
    PIO_SetPeripheralB(ctrl_bus.pio,ctrl_bus.mask,ENABLE_PULLUP);
#if LCD_TYPE==0	//OK
	SMC_SetCsr(AT91C_BASE_SMC,0,127,1,0,0,2,0,0,2,2);
#else
	SMC_SetCsr(AT91C_BASE_SMC,0,10,1,0,0,2,0,0,2,1);
	//SMC_SetCsr(AT91C_BASE_SMC,0,20,1,6,0,2,1,3,2,1);
#endif
    Rs485Adr = GetRs485Adr(RS485Adr);
    InitKeyStruct();
	lampValue = 0;
	WRITE_DATA8(OutputAdr,lampValue);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
		InitSysTime
function describe:init System Time
param:none
return:none
call fucntion:HexTimeToComBcdTime
global variable:SysTime
author:liuzg
date:2009-3-22
version:.01 */
/* EasyCODE F */
void InitSysTime(void)
{
    SysTime.year = 2009;
    SysTime.month = 3;
    SysTime.day = 25;
    SysTime.hour = 9;
    SysTime.minute = 0;
    SysTime.second = 0;
    //HexTimeToComBcdTime();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
					InitSPI
function describe:Init SPI interface
param:none
return:none
call fucntion:PIO_SetPeripheralA,AIC_ConfigureIT,SPID_Configure,
	SPID_ConfigureCS,memset,AT45_Configure,AIC_EnableIT,
	AT45_GetStatus,AT45_FindDevice,ClearLcd,LcdErr,
	BeepControl2,BeepDelay,CheckTimerGo
global variable:spi,spi_ncs2,spid,at45[],BeepFlg
author:liuzg
date:2009-3-23
version:.01 */
/* EasyCODE F */
void InitSPI(void)
{
    const At45Desc *pDesc;
    unsigned char i,status;
	AT91S_RSTC *pRstc;
	
	pRstc = AT91C_BASE_RSTC;
    //IO
    PIO_SetPeripheralA(spi.pio,spi.mask,DISABLE_PULLUP);
	PIO_SetPeripheralA(spi_ncs1.pio,spi_ncs1.mask,DISABLE_PULLUP);
    PIO_SetPeripheralA(spi_ncs2.pio,spi_ncs2.mask,DISABLE_PULLUP);
    
    AIC_ConfigureIT(AT91C_ID_SPI,SPI_PRIORITY,ISR_Spi);
    
    SPID_Configure(&spid, AT91C_BASE_SPI, AT91C_ID_SPI);
    
    SPID_ConfigureCS(&spid, 0, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
    SPID_ConfigureCS(&spid, 1, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
    SPID_ConfigureCS(&spid, 2, SPI_CSR(MAIN_MCK, SPCK));
    
    memset(&at45[0],0,sizeof(At45));
    memset(&at45[1],0,sizeof(At45));
    AT45_Configure(&at45[0], &spid, BOARD_AT45_NPCS0);
    AT45_Configure(&at45[1], &spid, BOARD_AT45_NPCS1);
    //M1172_Configure(&m1172,&spid,BOARD_M1172_NPCS);
    
    AIC_EnableIT(AT91C_ID_SPI);
    
    pDesc = 0;
    i = 0;
	SPID_ConfigureCS(&spid, 0, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
    while ( !pDesc )
    {
        status = AT45_GetStatus(&at45[0]);
        if ( status != 0xff )
        {
            pDesc = AT45_FindDevice(&at45[0],status);
#if LCD_TYPE==0	//OK
           	ClearLcd(2,9,126,49);
#else 
			ClearLcd(2,21,317,218);
#endif
        }
        else
        {
            if ( !i )
            {
                LcdErr(AT45_NOTFIND,0);
                i = 1;
            }
            if ( BeepFlg == 0 )
            {
                pBeepTmr = BeepControl2(100);
            }
            else
            {
                if ( BeepFlg == 2 )
                {
                    BeepDelay(100);
                }
            }
            CheckTimerGo();
			pRstc->RSTC_RMR = 0xa5000401;
			pRstc->RSTC_RCR = 0xa5000008;
			while(!(pRstc->RSTC_RSR & 0x01))
			{
			}
        }
    }
    AT45_Configure(&at45[1], &spid, BOARD_AT45_NPCS1);
	pDesc = 0;
	i = 0;
	SPID_ConfigureCS(&spid, 1, SPI_CSR(MAIN_MCK, AT45_SPI_MCK));
	while ( !pDesc )
	{
		status = AT45_GetStatus(&at45[1]);
		if ( status != 0xff )
		{
			pDesc = AT45_FindDevice(&at45[1],status);
#if LCD_TYPE==0		//OK
			ClearLcd(2,9,126,49);
#else
			ClearLcd(2,21,317,218);
#endif
		}
		else
		{
			if ( !i )
			{
				LcdErr(AT45_NOTFIND,1);
				i = 1;
			}
			if ( BeepFlg == 0 )
			{
				pBeepTmr = BeepControl2(100);
			}
			else
			{
				if ( BeepFlg == 2 )
				{
					BeepDelay(100);
				}
			}
			CheckTimerGo();
			pRstc->RSTC_RMR = 0xa5000401;
			pRstc->RSTC_RCR = 0xa5000008;
			while(!(pRstc->RSTC_RSR & 0x01))
			{
			}
		}
	}
}

/* EasyCODE ) */
/* EasyCODE ( 0 
					InitM1172
function describe:Init SPI interface to M1172
param:none
return:none
call fucntion:AIC_DisableIT,M1172_Configure,SpiUsartConfigure,
	M1172_ConfigureChannel,AIC_ClearIT,AIC_EnableIT
global variable:m1172,spid,chCtrl[],queue,spi_status
author:liuzg
date:2009-3-24
version:.01 */
/* EasyCODE F */
void InitM1172(unsigned char typ)
{
    AIC_DisableIT(AT91C_ID_IRQ1);
	SPID_Configure(&spid, AT91C_BASE_SPI, AT91C_ID_SPI);
    
    SPID_ConfigureCS(&spid, 2, SPI_CSR(MAIN_MCK, SPCK));
    M1172_Configure(&m1172, &spid, BOARD_M1172_NPCS);
    m1172.pChannel = &chCtrl[0];
    SpiUsartConfigure(18,&m1172,0,0,0,CHA,9600,
    				  M1172_UART_PARITY_NONE,
    				  M1172_UART_LENGTH_8,
    				  M1172_UART_STOP_1,
    				  M1172_CHANNEL_TX_FIFOTRIGGER_16,
    				  M1172_CHANNEL_RX_FIFOTRIGGER_8,
    				  M1172_UART_AUTO485_DISABLE,
    				  0,(unsigned char *)(&queue),0,0,0,0);
    if(typ == 0)
	{
		M1172_ConfigureChannel(&chCtrl[0],0x00, 0x10);
	}
	else
	{
		M1172_ConfigureChannel(&chCtrl[0],0x07, 0x10);
	}
    
	if(!typ)
	{
    	m1172.pChannel = &chCtrl[1];
    	SpiUsartConfigure(18,&m1172,0,0,0,CHB,9600,
    				  M1172_UART_PARITY_NONE,
    				  M1172_UART_LENGTH_8,
    				  M1172_UART_STOP_1,
    				  M1172_CHANNEL_TX_FIFOTRIGGER_16,
    				  M1172_CHANNEL_RX_FIFOTRIGGER_8,
    				  M1172_UART_AUTO485_DISABLE,
    				  0,0,0,0,0,0);
    	M1172_ConfigureChannel(&chCtrl[1],0x07, 0x90);
    }
    spi_status =0;
	if(typ == 0)
	{
		queue.head = 0;
		queue.tail = 1;
	}
    AIC_ClearIT(AT91C_ID_IRQ1);
    AIC_EnableIT(AT91C_ID_IRQ1);
	if(typ == 0)
	{
		InitPrint();
		CreateTimer(TIMER_AUTO,50,M1172Check);
		CheckPrintStatus();
	}
}

void InitMemList(void)
{
	InitPrintTaskList();
	InitTmrList();
	if(IcTypeFlg)
	{
		InitTmrList1();
	}
	InitSysTime();
}

void CheckZimo(void)
{
	unsigned int pageMax,adr;
	unsigned int pageOffset,i;
	unsigned short crc = 0;
	unsigned char buf[528];
	
#if LCD_TYPE==0
	pageMax = (256 * 1024);
#else
	pageMax = (94 * 94 * 32) + (16 * 256);
#endif
	pageOffset = pageMax % AT45_PageSize(&at45[0]);
	pageMax /= AT45_PageSize(&at45[0]);
	for(i = 0; i < pageMax; i++)
	{
		adr = i * AT45_PageSize(&at45[0]);
		READ(ZIMO_ID,buf,adr,528);
		crc = Crc16(buf,528,crc);
		CheckTimerGo();
		WatchDog();
	}
	adr = pageMax * AT45_PageSize(&at45[0]);
	READ(ZIMO_ID,buf,adr,pageOffset);
	crc = Crc16(buf,pageOffset,crc);
	if(crc != ZimoCrc)
	{
		LcdErr(ZIMO_ERROR,0);
		while(1)
		{
			CheckTimerGo();
			WatchDog();
		}
	}
}

/* EasyCODE ) */
/* EasyCODE ) */
