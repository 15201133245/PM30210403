
#ifndef _AT83_H
#define _AT83_H
#include "twi.h"
#include "tc.h"

#include <string.h>

#define		SET			1
#define		CLR			0

#define AT83_TIMES		50

// WRITE COMMAND number 1
#define C26_W_GENERAL_CALL_RESET	0x06
// WRITE COMMAND number 2
#define C26_W_RESET					0xFF
// WRITE COMMAND number 4
#define C26_W_WRITE_TIMER			0xFC
// WRITE COMMAND number 6
#define C26_W_SC_DCDCB				0xF8
// WRITE COMMAND number 7
#define C26_W_SC2					0xF9
// WRITE COMMAND number 8
#define C26_W_SC3					0xFA
// WRITE COMMAND number 9
#define C26_W_SC4					0xFB
// WRITE COMMAND number 10
#define C26_W_SC5					0xFD
// WRITE COMMAND number 11
#define C26_W_DCDCB_CMD				0xFE
// WRITE COMMAND number 12
#define C26_W_SLEW_CTRL				0xF7

// READ functions based on read commands
#define READ_CMD1_LENGTH 			11
#define READ_CMD2_LENGTH 			4
#define READ_CMD3_LENGTH 			3
#define READ_CMD4_LENGTH 			2
#define READ_CMD5_LENGTH 			2
#define READ_CMD6_LENGTH 			2
#define READ_CMD7_LENGTH 			2
#define READ_CMD8_LENGTH 			3


//MARCO
//AT83C26 
//bit2-0
#define	VCARD_VCC_0						0		//0v
#define	VCARD_VCC_1						1		//1.8v
#define	VCARD_VCC_3						2		//3v
#define	VCARD_VCC_5						3		//5v

//bit2-0
#define	CDS_NO_SAMPLE					0		//no sample
#define	CDS_4_SAMPLE					1		//4 identical samples
#define CDS_8_SAMPLE					2		//8 identical samples
#define	CDS_16_SAMPLE					3		//16 identical samples
#define	CDS_32_SAMPLE					4		//32 identical samples
#define	CDS_64_SAMPLE					5		//64 identical samples
#define	CDS_128_SAMPLE					6		//128 identical samples
#define	CDS_256_SAMPLE					7		//256 identical samples

//bit6-4
#define	DCK_1							0		//prescaler factor equals 1
#define	DCK_2							1		//prescaler factor equals 2
#define	DCK_4							2		//prescaler factor equals 4
#define DCK_6							3		//prescaler factor equals 6
#define	DCK_8							4		//prescaler factor equals 8
#define DCK_10							5		//prescaler factor equals 10
#define DCK_12							6		//prescaler factor equals 12
//bit2-0
#define	CKS_CLK							0		//CCLK1 = CLK
#define	CKS_DCCLK						1		//CCLK1 = DDCLK
#define	CKS_DCCLK_2						2		//CCLK1 = DDCLK / 2
#define	CKS_DDCLK_4						3		//CCLK1 = DDCLK / 4
#define	CKS_A2							4		//CCLK1 = A2
#define	CKS_A2_2						5		//CCLK1 = A2 / 2
#define	CKS_CLK_2						6		//CCLK1 = CLK / 2
#define CKS_CLK_4						7		//CCLK1 = CLK / 4

#define	DEMBOOST0						0		//normal
#define	DEMBOOST1						1		//normal + 18%
#define DEMBOOST2						2		//normal + 18%
#define	DEMBOOST3						3		//normal + 40%

#define	CIO1							0
#define	CIO2							1
#define	CIO3							2
#define	CIO4							3
#define	CIO5							4

#define	CCLK_MODE1						0
#define	CCLK_MODE2						1
#define	CCLK_MODE3						2
#define	CCLK_MODE4						3

typedef union
{
  	unsigned char 	cfg0;
	struct
	{
		unsigned char 	vcard : 2;
		unsigned char	vcard_err : 1;
		unsigned char 	icard_err : 1;
		unsigned char 	insert : 1;
		unsigned char	atr_err : 1;
		unsigned char	reserved0 : 2;
	};
}SC1_CFG0,*pSC1_CFG0;

typedef union
{
  	unsigned char	cfg1;
	struct
	{
		unsigned char 	cds : 3;
		unsigned char 	pull_up : 1;
		unsigned char 	card_det : 1;
		unsigned char 	shut_down : 1;
		unsigned char 	art : 1;
		unsigned char	reserved0 : 1;
	};
}SC1_CFG1,*pSC1_CFG1;

typedef union
{
  	unsigned char	cfg2;
	struct
	{
		unsigned char 	cks : 3;
		unsigned char 	reserved0 : 1;
		unsigned char	dck : 3;
		unsigned char	reserved : 1;
	};
}SC1_CFG2,*pSC1_CFG2;

typedef union
{
  	unsigned char	cfg3;
	struct
	{
	  	unsigned char	reserved0 : 4;
		unsigned char	icc_adj : 1;
		unsigned char	reserved1 : 3;
	};
}SC1_CFG3,*pSC1_CFG3;

typedef union
{
  	unsigned char	cfg4;
	struct
	{
		unsigned char 	crst_sel : 1;
		unsigned char	reserved0 : 2;
		unsigned char 	int_pullup : 1;
		unsigned char	step_reg : 1;
		unsigned char	dem_boost : 2;
		unsigned char 	reserved1 : 1;
	};
}SC1_CFG4,*pSC1_CFG4;

typedef union
{
  	unsigned char	interface;
	struct
	{
		unsigned char 	card_io : 1;
		unsigned char	card_ck : 1;
		unsigned char	card_c41 : 1;
		unsigned char	card_c81 : 1;
		unsigned char	card_rst : 1;
		unsigned char	ck_stop : 1;
		unsigned char 	io_dis : 1;
		unsigned char	reserved0 : 1;
	};
}SC1_INTERFACE,*pSC1_INTERFACE;

typedef union
{
  	unsigned char	status;
	struct
	{
		unsigned char	cio : 1;
		unsigned char	crst : 1;
		unsigned char	vcard_int : 1;
		unsigned char	reserved0 : 1;
		unsigned char	vcard_ok : 1;
		unsigned char	card_in : 1;
		unsigned char 	cc41 : 1;
		unsigned char	cc81 : 1;
	};
}STATUSA,*pSTATUSA;

typedef union
{
  	unsigned char	cfg0;
	struct
	{
		unsigned char	vcard : 2;
		unsigned char	vcard_err : 1;
		unsigned char	reserved : 2;
		unsigned char	atr_err : 1;
		unsigned char	vcard_ok : 1;
		unsigned char	vcard_int : 1;
	};
}SCX_CFG0,*pSCX_CFG0;

typedef union
{
  	unsigned char	cfg1;
	struct
	{
		unsigned char	cds : 3;
		unsigned char	pull_up : 1;
		unsigned char	card_det : 1;
		unsigned char	full : 1;
		unsigned char	reserved0 : 2;
	};
}SC2_CFG1,*pSC2_CFG1;

typedef union
{
  	unsigned char	cfg2;
	struct
	{
		unsigned char	cks : 3;
		unsigned char	ck_stop : 1;
		unsigned char	card_ck : 1;
		unsigned char 	card_rst : 1;
		unsigned char	crst_sel : 1;
		unsigned char	art : 1;
	};
}SCX_CFG2,*pSCX_CFG2;

typedef union
{
  	unsigned char	select;
	struct
	{
		unsigned char	io_sel : 4;
		unsigned char	reserved0 : 4;
	};
}IO_SELECT,*pIO_SELECT;

typedef union
{
  	unsigned char	interface;
	struct
	{
		unsigned char	dem_boostb : 2;
		unsigned char	card_io2 : 1;
		unsigned char	card_io3_card_c42 : 1;
		unsigned char	card_io4 : 1;
		unsigned char	card_io5 : 1;
		unsigned char	card_c82 : 1;
		unsigned char	reserved0 : 1;
	};
}INTERFACEB,*pINTERFACEB;

typedef union
{
  	unsigned char	statusb;
	struct
	{
		unsigned char	cio2 : 1;
		unsigned char	crst2 : 1;
		unsigned char	cio3_cc42 : 1;
		unsigned char	crst3_cc82 : 1;
		unsigned char	cio4 : 1;
		unsigned char	cio5 : 1;
		unsigned char	card_in2 : 1;
		unsigned char	reserved0 : 1;
	};
}STATUSB,*pSTATUSB;

typedef union
{
  	unsigned char	itdis;
	struct
	{
		unsigned char	it_dis2 : 1;
		unsigned char	it_dis3 : 1;
		unsigned char	it_dis4 : 1;
		unsigned char	it_dis5 : 1;
		unsigned char	io_dis2 : 1;
		unsigned char	io_dis3 : 1;
		unsigned char	io_dis4 : 1;
		unsigned char	io_dis5 : 1;
	};
}ITDIS,*pITDIS;

typedef union
{
  	unsigned char	dcdcb;
	struct
	{
		unsigned char	vdc : 2;
		unsigned char	step_reg : 1;
		unsigned char	icc_adj : 1;
		unsigned char	reserved0 : 1;
		unsigned char	vdc_ok : 1;
		unsigned char	vdc_int : 1;
		unsigned char	shut_down : 1;
	};
}DCDCB,*pDCDCB;

typedef union
{
  	unsigned char	ldo;
	struct
	{
		unsigned char	reserved0 : 4;
		unsigned char	iplus2 : 1;
		unsigned char	iplus3 : 1;
		unsigned char	iplus4 : 1;
		unsigned char 	iplus5 : 1;
	};
}LDO,*pLDO;

typedef union
{
  	unsigned char	slew;
	struct
	{
		unsigned char	cio1_slew_ctrl : 2;
		unsigned char	cclk1_slew_ctrl : 2;
		unsigned char	cio2_slew_ctrl : 2;
		unsigned char	cclk2_clew_ctrl : 2;
	};
}SLEW_CTRL_1,*pSLEW_CTRL_1;

typedef union
{
  	unsigned char	slew;
	struct
	{
		unsigned char	cio3_slew_ctrl : 2;
		unsigned char	cclk3_slew_ctrl : 2;
		unsigned char	cio4_slew_ctrl : 2;
		unsigned char	cclk4_slew_ctrl : 2;
	};
}SLEW_CTRL_2,*pSLEW_CTRL_2;

typedef union
{
  	unsigned char	slew;
	struct
	{
		unsigned char	cio5_slew_ctrl : 2;
		unsigned char	cclk5_slew_ctrl : 2;
		unsigned char	reserved0 : 4;
	};
}SLEW_CTRL_3,*pSLEW_CTRL_3;

typedef struct _At83Reg
{
  	//Smart Card 1 interface register
	STATUSA			statusa;
  	SC1_CFG0		sc1_cfg0;
	SC1_CFG1		sc1_cfg1;
	SC1_CFG2		sc1_cfg2;
	SC1_CFG3		sc1_cfg3;
	SC1_CFG4		sc1_cfg4;
	SC1_INTERFACE	sc1_interface;
	unsigned char	timer_msb;
	unsigned char	timer_lsb;
	unsigned char	capture_msb;
	unsigned char	capture_lsb;
	//Smart Card 2 interface register
	SCX_CFG0		sc2_cfg0;
	SC2_CFG1		sc2_cfg1;
	SCX_CFG2		sc2_cfg2;
	//SIM/SAM 3 interface register
	SCX_CFG0		sc3_cfg0;
	SCX_CFG2		sc3_cfg2;
	//SIM/SAM 4 interface register
	SCX_CFG0		sc4_cfg0;
	SCX_CFG2		sc4_cfg2;
	//SIM/SAM 5 interface register
	SCX_CFG0		sc5_cfg0;
	SCX_CFG2		sc5_cfg2;
	//Common register for SC1/SC2/SC3/SC4/SC5
	STATUSB			statusb;
	IO_SELECT		io_select;
	//Common register for SC2/SC3/SC4/SC5
	INTERFACEB		interfaceb;
	ITDIS			itdis;
	//DC/DC B register
	DCDCB			dcdcb;
	LDO				ldo;
	//Slew control register for CIO and CCLK pins
	SLEW_CTRL_1		slew_ctrl_1;
	SLEW_CTRL_2		slew_ctrl_2;
	SLEW_CTRL_3		slew_ctrl_3;
}At83Reg;

typedef struct _At83
{
	Twid 	*pTwid;
	unsigned char DeviceAdr;
	unsigned char TimeFlg;
	unsigned char TimeOutFlg;
	void (*RunTime)(void);
	At83Reg		reg;
}At83;

#define IADR							0
#define ISIZE							0

inline unsigned char AT83_IsBusy(At83 *pAt83)
{
    return TWID_IsBusy(pAt83->pTwid);
}

void TwiTimeOut(void);

unsigned char c26_read(At83 *pAt83,
					   unsigned char len,
					   unsigned char *p);

unsigned char c26_write(At83 *pAt83,unsigned char len,unsigned char *p);

unsigned char c26_read_cmd1_init(At83 *pAt83);

unsigned char c26_read_cmd1(At83 *pAt83);

unsigned char c26_read_cmd1_capture_lsb(At83 *pAt83);

unsigned char c26_read_cmd1_capture_msb(At83 *pAt83);

unsigned char c26_read_cmd1_timer_lsb(At83 *pAt83);

unsigned char c26_read_cmd1_timer_msb(At83 *pAt83);

unsigned char c26_read_cmd1_sc1_interface(At83 *pAt83);

unsigned char c26_read_cmd1_sc1_cfg4(At83 *pAt83);

unsigned char c26_read_cmd1_sc1_cfg3(At83 *pAt83);

unsigned char c26_read_cmd1_sc1_cfg2(At83 *pAt83);

unsigned char c26_read_cmd1_sc1_cfg1(At83 *pAt83);

unsigned char c26_read_cmd1_sc1_cfg0(At83 *pAt83);

unsigned char c26_read_cmd1_sc1_status(At83 *pAt83);

unsigned char c26_read_cmd2(At83 *pAt83);

unsigned char c26_read_cmd2_itdis(At83 *pAt83);

unsigned char c26_read_cmd2_interfaceb(At83 *pAt83);

unsigned char c26_read_cmd2_io_select(At83 *pAt83);

unsigned char c26_read_cmd2_statusb(At83 *pAt83);

unsigned char c26_read_cmd3(At83 *pAt83);

unsigned char c26_read_cmd3_sc2_cfg2(At83 *pAt83);

unsigned char c26_read_cmd3_sc2_cfg1(At83 *pAt83);

unsigned char c26_read_cmd3_sc2_cfg0(At83 *pAt83);

unsigned char c26_read_cmd4(At83 *pAt83);

unsigned char c26_read_cmd4_sc3_cfg2(At83 *pAt83);

unsigned char c26_read_cmd4_sc3_cfg0(At83 *pAt83);

unsigned char c26_read_cmd5(At83 *pAt83);

unsigned char c26_read_cmd5_sc4_cfg2(At83 *pAt83);

unsigned char c26_read_cmd5_sc4_cfg0(At83 *pAt83);

unsigned char c26_read_cmd6(At83 *pAt83);

unsigned char c26_read_cmd6_sc5_cfg2(At83 *pAt83);

unsigned char c26_read_cmd6_sc5_cfg0(At83 *pAt83);

unsigned char c26_read_cmd7(At83 *pAt83);

unsigned char c26_read_cmd7_ldo(At83 *pAt83);

unsigned char c26_read_cmd7_dcdcb(At83 *pAt83);

unsigned char c26_read_cmd8(At83 *pAt83);

unsigned char c26_read_cmd8_slew_ctrl_3(At83 *pAt83);

unsigned char c26_read_cmd8_slew_ctrl_2(At83 *pAt83);

unsigned char c26_read_cmd8_slew_ctrl_1(At83 *pAt83);

unsigned char c26_write_cmd1(At83 *pAt83);

unsigned char c26_write_cmd2(At83 *pAt83);

unsigned char c26_write_cmd3(At83 *pAt83);

unsigned char c26_write_cmd3_sc1_cfg0(At83 *pAt83,
									  unsigned char sc1_cfg0);

unsigned char c26_write_cmd3_sc1_cfg1(At83 *pAt83,
									  unsigned char sc1_cfg1);

unsigned char c26_write_cmd3_sc1_cfg2(At83 *pAt83,
									  unsigned char sc1_cfg2);

unsigned char c26_write_cmd3_sc1_cfg3(At83 *pAt83,
									  unsigned char sc1_cfg3);

unsigned char c26_write_cmd3_sc1_cfg4(At83 *pAt83,
									  unsigned char sc1_cfg4);

unsigned char c26_write_cmd4(At83 *pAt83);

unsigned char c26_write_cmd4_timer_msb(At83 *pAt83,
									   unsigned char timer_msb);

unsigned char c26_write_cmd4_timer_lsb(At83 *pAt83,
									   unsigned char timer_lsb);

unsigned char c26_write_cmd5(At83 *pAt83);

unsigned char c26_write_cmd5_sc1_interface(At83 *pAt83,
										   unsigned char sc1_interface);

unsigned char c26_write_cmd6(At83 *pAt83);

unsigned char c26_write_cmd6_io_select(At83 *pAt83,
									   unsigned char io_select);

unsigned char c26_write_cmd6_interfaceb(At83 *pAt83,
										unsigned char interfaceb);

unsigned char c26_write_cmd6_itdis(At83 *pAt83,
								   unsigned char itdis);

unsigned char c26_write_cmd7(At83 *pAt83);

unsigned char c26_write_cmd7_sc2_cfg0(At83 *pAt83,
									  unsigned char sc2_cfg0);

unsigned char c26_write_sc2_cfg1(At83 *pAt83,
								 unsigned char sc2_cfg1);

unsigned char c26_write_cmd7_sc2_cfg2(At83 *pAt83,
									  unsigned char sc2_cfg2);

unsigned char c26_write_cmd8(At83 *pAt83);

unsigned char c26_write_cmd8_sc3_cfg0(At83 *pAt83,
									  unsigned char sc3_cfg0);

unsigned char c26_write_cmd8_sc3_cfg2(At83 *pAt83,
									  unsigned char sc3_cfg2);

unsigned char c26_write_cmd9(At83 *pAt83);

unsigned char c26_write_cmd9_sc4_cfg0(At83 *pAt83,
									  unsigned char sc4_cfg0);

unsigned char c26_write_cmd9_sc4_cfg2(At83 *pAt83,
									  unsigned char sc4_cfg2);

unsigned char c26_write_cmd10(At83 *pAt83);

unsigned char c26_write_cmd10_sc5_cfg0(At83 *pAt83,
									   unsigned char sc5_cfg0);

unsigned char c26_write_cmd10_sc5_cfg2(At83 *pAt83,
									   unsigned char sc5_cfg2);

unsigned char c26_write_cmd11(At83 *pAt83);

unsigned char c26_write_cmd11_dcdcb(At83 *pAt83,
									unsigned char dcdcb);

unsigned char c26_write_cmd11_ldo(At83 *pAt83,
								  unsigned char ldo);

unsigned char c26_write_cmd12(At83 *pAt83);

unsigned char c26_write_cmd12_slew_ctrl_1(At83 *pAt83,
										  unsigned char slew_ctrl_1);

unsigned char c26_write_cmd12_slew_ctrl_2(At83 *pAt83,
										  unsigned char slew_ctrl_2);

unsigned char c26_write_cmd12_slew_ctrl_3(At83 *pAt83,
										  unsigned char slew_ctrl_3);

unsigned char c26_config_init(At83 *pAt83);

unsigned char c26_dcdca_start(At83 *pAt83);

unsigned char c26_dcdcb_start(At83 *pAt83);

unsigned char c26_start_cvcc1(At83 *pAt83);

unsigned char c26_start_cvcc2(At83 *pAt83);

unsigned char c26_stop_cvcc2(At83 *pAt83);

unsigned char c26_start_cvcc3(At83 *pAt83);

unsigned char c26_stop_cvcc3(At83 *pAt83);

unsigned char c26_start_cvcc4(At83 *pAt83);

unsigned char c26_stop_cvcc4(At83 *pAt83);

unsigned char c26_start_cvcc5(At83 *pAt83);

unsigned char c26_stop_cvcc5(At83 *pAt83);

#endif