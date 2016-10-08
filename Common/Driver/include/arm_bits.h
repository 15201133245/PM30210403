
#ifndef	_ARM_BITS_H
#define _ARM_BITS_H
//Little
//RSTC
typedef union
{
    unsigned int   rstc_cr;
    struct
    {
        unsigned int   PROCRST : 1;        //Processor Reset
        unsigned int   Reserved0 : 1;
        unsigned int   PERRST : 1;         //Peripheral Reset
        unsigned int   EXTRST : 1;         //External Reset
        unsigned int   Reserved1 : 20;
        unsigned int   KEY : 8;            //Password
    };
}RSTC_CR,*pRSTC_CR;                         //Reset Control Register

typedef union
{
    unsigned int   rstc_sr;
    struct
    {
        unsigned int   URSTS : 1;          //User Reset Status
        unsigned int   BODSTS : 1;         //Brownout Detection Status
        unsigned int   Reserved0 : 6;
        unsigned int   RSTTYP : 3;         //Reset Type
        unsigned int   Reserved1 : 5;
        unsigned int   NRSTL : 1;          //NRST Pin Level
        unsigned int   SRCMP : 1;          //Software Reset Command in Progress
        unsigned int   Reserved2 : 14;
    };
}RSTC_SR,*pRSTC_SR;                         //Reset Control Status Register

typedef union
{
    unsigned int   rstc_mr;
    struct
    {
        unsigned int   URSTEN : 1;         //User Reset Enable
        unsigned int   Reserved0 : 3;      
        unsigned int   URSTIEN : 1;        //User Reset Interrupt Enable
        unsigned int   Reserved1 : 3;
        unsigned int   ERSTL : 4;          //External Reset Length
        unsigned int   Reserved2 : 4;
        unsigned int   BODIEN : 1;         //Brownout Detection Interrupt Enable
        unsigned int   Reserved3 : 7;
        unsigned int   KEY : 8;            //Password
    };
}RSTC_MR,*pRSTC_MR;                         //Reset Controller Mode Register

//RTT
typedef union
{
    unsigned int   rtt_mr;
    struct
    {
        unsigned int   RTPRES : 16;        //Real-time Timer Prescaler Value
        unsigned int   ALMIEN : 1;         //Alarm Interrupt Enable
        unsigned int   RTTINCIEN : 1;      //Real-time Timer Increment Interrupt Enable
        unsigned int   RTTRST : 1;         //Real-time Timer Restart
        unsigned int   Reserved0 : 13;
    };
}RTT_MR,*pRTT_MR;                           //Real-time Timer Mode Register

typedef union
{
    unsigned int   rtt_ar;
    struct
    {
        unsigned int   ALMV : 32;          //Alarm Value
    };
}RTT_AR,*pRTT_AR;                           //Real-time Timer Alarm Register

typedef union
{
    unsigned int   rtt_vr;
    struct
    {
        unsigned int   CRTV : 32;          //Current Real-time Value
    };
}RTT_VR,*pRTT_VR;                           //Real-time Timer Value Register

typedef union
{
    unsigned int   rtt_sr;
    struct
    {
        unsigned int   ALMS : 1;           //Real-time Alarm Status
        unsigned int   RTTINC : 1;         //Real-time Timer Increment
        unsigned int   Reserved0 : 30;
    };
}RTT_SR,*pRTT_SR;                           //Real-time Timer Status Register

//PIT
typedef union
{
    unsigned int   pit_mr;
    struct
    {
        unsigned int   PIV : 20;           //Periodic Interval Value
        unsigned int   Reserved0 : 4;
        unsigned int   PITEN : 1;          //Periodic Interval Timer Enabled
        unsigned int   PITIEN : 1;         //Periodic Interval Timer Interrupt Enable
        unsigned int   Reserved1 : 6;
    };
}PIT_MR,*pPIT_MR;                           //Periodic Interval Timer Mode Register

typedef union
{
    unsigned int   pit_sr;
    struct
    {
        unsigned int   PITS : 1;           //Periodic Interval Timer Status
        unsigned int   Reserved0 : 31;
    };
}PIT_SR,*pPIT_SR;                           //Periodic Interval Timer Status Register

typedef union
{
    unsigned int   pit_pivr;
    struct
    {
        unsigned int   CPIV : 20;          //Current Periodic Interval Value
        unsigned int   PICNT : 12;         //Periodic Interval Counter
    };
}PIT_PIVR,*pPIT_PIVR;                       //Periodic Interval Timer Value Register

typedef union
{
    unsigned int   pit_piir;
    struct
    {
        unsigned int   CPIV : 20;          //Current Periodic Interval Value
        unsigned int   PICNT : 12;         //Periodic Interval Counter
    };
}PIT_PIIR,*pPIT_PIIR;                       //Periodic Interval Timer Image Register

//WDT
typedef union
{
    unsigned int   wdt_cr;
    struct
    {
        unsigned int   WDRSTT : 1;         //Watchdog Restart
        unsigned int   Reserved0 : 23;
        unsigned int   KEY : 8;            //Password
    };
}WDT_CR,*pWDT_CR;                           //Watchdog Timer Control Register

typedef union
{
    unsigned int   wdt_mr;
    struct
    {
        unsigned int   WDV : 12;           //Watchdog Counter Value
        unsigned int   WDFIEN : 1;         //Watchdog Fault Interrupt Enable
        unsigned int   WDRSTEN : 1;        //Watchdog Reset Enable
        unsigned int   WDRPROC : 1;        //Watchdog Reset Processor
        unsigned int   WDDIS : 1;          //Watchdog Disable
        unsigned int   WDD : 12;           //Watchdog Delta Value
        unsigned int   WDDBGHLT : 1;       //Watchdog Debug Halt
        unsigned int   WDIDLEHLT : 1;      //Watchdog Idle Halt
        unsigned int   Reserved0 : 2;
    };
}WDT_MR,*pWDT_MR;                           //Watchdog Timer Mode Register

typedef union
{
    unsigned int   wdt_sr;
    struct
    {
        unsigned int   WDUNF : 1;          //Watchdog Underflow
        unsigned int   WDERR : 1;          //Watchdog Error
        unsigned int   Reserved0 : 30;
    };
}WDT_SR,*pWDT_SR;                           //Watchdog Timer Status Register

//VREG
typedef union
{
    unsigned int   vreg_mr;
    struct
    {
        unsigned int   PSTDBY : 1;         //Periodic Interval Value
        unsigned int   Reserved0 : 31;
    };
}VREG_MR,*pVREG_MR;                         //Voltage Regulator Mode Register

//MC
typedef union
{
    unsigned int   mc_rcr;
    struct
    {
        unsigned int   RCB : 1;            //Remap Command Bit
        unsigned int   Reserved0 : 31;
    };
}MC_RCR,*pMC_RCR;                           //RC Remap Control Register

typedef union
{
    unsigned int   mc_asr;
    struct
    {
        unsigned int   UNDADD : 1;         //Undefined Address Abort Status
        unsigned int   MISADD : 1;         //Misaligned Address Abort Status
        unsigned int   MPU : 1;            //Memory Protection Unit Abort Status
        unsigned int   Reserved0 : 5;
        unsigned int   ABTSZ : 2;          //Abort Size Status
        unsigned int   ABTTYP : 2;         //Abort Type Status
        unsigned int   Reserved1 : 4;
        unsigned int   MST0 : 1;           //ARM7TDMI Abort Source
        unsigned int   MST1 : 1;           //PDC Abort Source
        unsigned int   Reserved2 : 6;
        unsigned int   SVMST0 : 1;         // Saved ARM7TDMI Abort Source
        unsigned int   SVMST1 : 1;         //Saved PDC Abort Source
        unsigned int   Reserved3 : 6;
    };
}MC_ASR,*pMC_ASR;                           //MC Abort Status Register

typedef union
{
    unsigned int   mc_aasr;
    struct
    {
        unsigned int   ABTADD : 32;        //Abort Address
    };
}MC_AASR,*pMC_AASR;                         //MC Abort Address Status Register

typedef union
{
    unsigned int   mc_puia;
    struct
    {
        unsigned int   PROT : 2;           //Protection
        unsigned int   Reserved0 : 2;
        unsigned int   SIZE : 4;           //Internal Area Size
        unsigned int   Reserved1 : 2;
        unsigned int   BA : 12;            //Internal Area Base Address
        unsigned int   Reserved2 : 10;
    };
}MC_PUIA,*pMC_PUIA;                         //MC Protection Unit Area 0 to 15 Registers

typedef union
{
    unsigned int   mc_pup;
    struct
    {
        unsigned int   PROT : 2;           //Protection
        unsigned int   Reserved0 : 30;
    };
}MC_PUP,*pMC_PUP;                           //MC Protection Unit Peripheral

typedef union
{
    unsigned int   mc_puer;
    struct
    {
        unsigned int   PUEB : 1;           //Protection Unit Enable Bit
        unsigned int   Reserved0 : 31;
    };
}MC_PUER,*pMC_PUER;                         //MC Protection Unit Enable Register

//EFC
typedef union
{
    unsigned int   mc_fmr;
    struct
    {
        unsigned int   FRDY : 1;           //Flash Ready Interrupt Enable
        unsigned int   Reserved0 : 1;
        unsigned int   LOCKE : 1;          //Lock Error Interrupt Enable
        unsigned int   PROGE : 1;          //Programming Error Interrupt Enable
        unsigned int   Reserved1 : 3;
        unsigned int   NEBP : 1;           //No Erase Before Programming
        unsigned int   FWS : 2;            //Flash Wait State
        unsigned int   Reserved2 : 6;
        unsigned int   FMCN : 8;           //Flash Microsecond Cycle Number
        unsigned int   Reserved3 : 8;
    };
}MC_FMR,*pMC_FMR;                           //MC Flash Mode Register

typedef union
{
    unsigned int   mc_fcr;
    struct
    {
        unsigned int   FCMD : 4;           //Flash Command
        unsigned int   Reserved0 : 4;
        unsigned int   PAGEN : 10;         //Page Number
        unsigned int   Reserved1 : 6;
        unsigned int   KEY : 8;            //Write Protection Key
    };
}MC_FCR,*pMC_FCR;                           //MC Flash Command Register

typedef union
{
    unsigned int   mc_fsr;
    struct
    {
        unsigned int   FRDY : 1;           //Flash ready Status
        unsigned int   Reserved0 : 1;
        unsigned int   LOCKE : 1;          //Lock Error Status
        unsigned int   PROGE : 1;          //Programming Error Status
        unsigned int   SECURITY : 1;       //Security Bit Status
        unsigned int   Reserved1 :3;
        unsigned int   GPNVM0 : 1;
        unsigned int   GPNVM1 : 1;
        unsigned int   GPNVM2 : 1;         //General-purpose NVM Bit Status
        unsigned int   Reserved2 : 5;
        unsigned int   LOCKS0 : 1;
        unsigned int   LOCKS1 : 1;
        unsigned int   LOCKS2 : 1;
        unsigned int   LOCKS3 : 1;
        unsigned int   LOCKS4 : 1;
        unsigned int   LOCKS5 : 1;
        unsigned int   LOCKS6 : 1;
        unsigned int   LOCKS7 : 1;
        unsigned int   LOCKS8 : 1;
        unsigned int   LOCKS9 : 1;
        unsigned int   LOCKS10 : 1;
        unsigned int   LOCKS11 : 1;
        unsigned int   LOCKS12 : 1;
        unsigned int   LOCKS13 : 1;
        unsigned int   LOCKS14 : 1;
        unsigned int   LOCKS15 : 1;        //Lock Region x Lock Status
    };
}MC_FSR,*pMC_FSR;                           //MC Flash Status Register

//EBI
typedef union
{
    unsigned int   ebi_csa;
    struct
    {
        unsigned int   Reserved0 : 1;
        unsigned int   CS1A : 1;           //Chip Select 1 Assignment
        unsigned int   CS2A : 1;           //Chip Select 2 Assignment
        unsigned int   CS3A : 1;           //Chip Select 3 Assignment
        unsigned int   CS4A : 1;           //Chip Select 4 Assignment
        unsigned int   Reserved1 : 11;
        unsigned int   NWPC : 1;           //NWAIT Pin Configuration
        unsigned int   Reserved2 : 15;
    };
}EBI_CSA,*pEBI_CSA;                         //EBI Chip Select Assignment Register

typedef union
{
    unsigned int   smc_csr;
    struct
    {
        unsigned int   NWS : 7;            //Number of Wait States
        unsigned int   WSEN : 1;           //Wait State Enable
        unsigned int   TDF : 4;            //Data Float Time
        unsigned int   BAT : 1;            //Byte Access Type
        unsigned int   DBW : 2;            //Data Bus Width
        unsigned int   DRP : 1;            //Data Read Protocol
        unsigned int   ACSS : 2;           //Address to Chip Select Setup
        unsigned int   Reserved0 : 6;
        unsigned int   RWSETUP : 3;        //Read and Write Signal Setup Time
        unsigned int   Reserved1 : 1;
        unsigned int   RWHOLD : 3;         //Read and Write Signal Hold Time
        unsigned int   Reserved2 : 1;
    };
}SMC_CSR,*pSMC_CSR;                         //SMC Chip Select 0~7 Register

//SDRAMC
typedef union
{
    unsigned int   sdramc_mr;
    struct
    {
        unsigned int   MODE : 4;           //SDRAMC Command Mode
        unsigned int   DBW : 1;            //Data Bus Width
        unsigned int   Reserved0 : 27;
    };
}SDRAMC_MR,*pSDRAMC_MR;                     //SDRAMC Mode Register

typedef union
{
    unsigned int   sdramc_tr;
    struct
    {
        unsigned int   COUNT : 12;         //SDRAMC Refresh Timer Count
        unsigned int   Reserved0 : 20;
    };
}SDRAMC_TR,*pSDRAMC_TR;                     //SDRAMC Refresh Timer Register

typedef union
{
    unsigned int   sdramc_cr;
    struct
    {
        unsigned int   NC : 2;             //Number of Column Bits
        unsigned int   NR : 2;             //Number of Row Bits
        unsigned int   NB : 1;             //Number of Banks
        unsigned int   CAS : 2;            //CAS Latency
        unsigned int   TWR : 4;            //Write Recovery Delay
        unsigned int   TRC : 4;            //Row Cycle Delay
        unsigned int   TRP : 4;            //Row Precharge Delay
        unsigned int   TRCD : 4;           //Row to Column Delay
        unsigned int   TRAS : 4;           //Active to Precharge Delay
        unsigned int   TXSR : 4;           //Exit Self Refresh to Active Delay
        unsigned int   Reserved0 : 1;
    };
}SDRAMC_CR,*pSDRAMC_CR;                     //SDRMAC Configuration Register

typedef union
{
    unsigned int   sdramc_srr;
    struct
    {
        unsigned int   SRCB : 1;           //Self-refresh Command Bit
        unsigned int   Reserved0 : 31;
    };
}SDRAMC_SRR,*pSDRAMC_SRR;                   //SDRAMC Self-refresh Register

typedef union
{
    unsigned int   sdramc_lpr;
    struct
    {
        unsigned int   LPCB : 1;           //Low-power Command Bit
        unsigned int   Reserved0 : 31;
    };
}SDRAMC_LPR,*pSDRAMC_LPR;                   //SDRAMC Low-power Register

typedef union
{
    unsigned int   sdramc_ier;
    struct
    {
        unsigned int   RES : 1;            //Refresh Error Status
        unsigned int   Reserved0 : 31;
    };
}SDRAMC_IER,*pSDRAMC_IER;                   //SDRAMC Interrupt Enable Register

typedef union
{
    unsigned int   sdramc_idr;
    struct
    {
        unsigned int   RES : 1;            //Refresh Error Status
        unsigned int   Reserved0 : 31;
    };
}SDRAMC_IDR,*pSDRAMC_IDR;                   //SDRAMC Interrupt Disable Register

typedef union
{
    unsigned int   sdramc_imr;
    struct
    {
        unsigned int   RES : 1;            //Refresh Error Status
        unsigned int   Reserved0 : 31;
    };
}SDRAMC_IMR,*pSDRAMC_IMR;                   //SDRAMC Interrupt Mask Register

typedef union
{
    unsigned int   sdramc_isr;
    struct
    {
        unsigned int   RES : 1;            //Refresh Error Status
        unsigned int   Reserved0 : 31;
    };
}SDRAMC_ISR,*pSDRAMC_ISR;                   //SDRAMC Interrupt Status Register

//ECC
typedef union
{
    unsigned int   ecc_cr;
    struct
    {
        unsigned int   RST : 1;            //RESET Parity
        unsigned int   Reserved0 : 31;
    };
}ECC_CR,*pECC_CR;                           //ECC Control Register

typedef union
{
    unsigned int   ecc_mr;
    struct
    {
        unsigned int   PAGESIZE : 2;       //Page Size
        unsigned int   Reserved0 : 30;
    };
}ECC_MR,*pECC_MR;                           //ECC Mode Register

typedef union
{
    unsigned int   ecc_sr;
    struct
    {
        unsigned int   RECERR : 1;         //Recoverable Error
        unsigned int   ECCERR : 1;         //ECC Error
        unsigned int   MULERR : 1;         //Multiple Error
        unsigned int   Reserved0 : 29;
    };
}ECC_SR,*pECC_SR;                           //ECC Status Register

typedef union
{
    unsigned int   ecc_pr;
    struct
    {
        unsigned int   BITADDR : 4;        
        unsigned int   WORDADDR : 12;
        unsigned int   Reserved0 : 16;
    };
}ECC_PR,*pECC_PR;                           //ECC Parity Register

typedef union
{
    unsigned int   ecc_npr;
    struct
    {
        unsigned int   NPARITY : 16;
        unsigned int   Reserved0 : 16;
    };
}ECC_NPR,*pECC_NPR;                         //ECC NParity Register

//PDC
typedef union
{
    unsigned int   pdc_rpr;
    struct
    {
        unsigned int   RXPTR : 32;         //Receive Pointer Address
    };
}PDC_RPR,*pPDC_RPR;                         //PDC Receive Pointer Register

typedef union
{
    unsigned int   pdc_rcr;
    struct
    {
        unsigned int   RXCTR : 16;         //Receive Counter Value
        unsigned int   Reserved0 : 16;
    };
}PDC_RCR,*pPDC_RCR;                         //PDC Receive Counter Register

typedef union
{
    unsigned int   pdc_tpr;
    struct
    {
        unsigned int   TXPTR : 32;         //Transmit Pointer Address
    };
}PDC_TPR,*pPDC_TPR;                         //PDC Transmit Pointer Register

typedef union
{
    unsigned int   pdc_tcr;
    struct
    {
        unsigned int   RXCTR : 16;         //Transmit Counter Value
        unsigned int   Reserved0 : 16;
    };
}PDC_TCR,*pPDC_TCR;                         //PDC Transmit Counter Register

typedef union
{
    unsigned int   pdc_rnpr;
    struct
    {
        unsigned int   RXNPTR : 32;        //Receive Next Pointer Address
    };
}PDC_RNPR,*pPDC_RNPR;                       //PDC Receive Next Pointer Register

typedef union
{
    unsigned int   pdc_rncr;
    struct
    {
        unsigned int   RXNCR : 16;         //Receive Next Counter Value
        unsigned int   Reserved0 : 16;
    };
}PDC_RNCR,*pPDC_RNCR;                       //PDC Receive Next Counter Register

typedef union
{
    unsigned int   pdc_tnpr;
    struct
    {
        unsigned int   TXNPTR : 32;        //Transmit Next Pointer Address
    };
}PDC_TNPR,*pPDC_TNPR;                       //PDC Transmit Next Pointer Register

typedef union
{
    unsigned int   pdc_tncr;
    struct
    {
        unsigned int   TXNCR : 16;         //Transmit Next Counter Value
        unsigned int   Reserved0 : 16;
    };
}PDC_TNCR,*pPDC_TNCR;                       //PDC Transmint Next Counter Register

typedef union
{
    unsigned int   pdc_ptcr;
    struct
    {
        unsigned int   RXTEN : 1;          //Receiver Transfer Enable
        unsigned int   RXTDIS : 1;         //Receiver Transfer Disable
        unsigned int   Reserved0 : 6;
        unsigned int   TXTEN : 1;          //Transmitter Transfer Enable
        unsigned int   TXTDIS : 1;         //Transmitter Transfer Disable
        unsigned int   Reserved1 : 22;
    };
}PDC_PTCR,*pPDC_PTCR;                       //PDC Transfer Control Register

typedef union
{
    unsigned int   pdc_ptsr;
    struct
    {
        unsigned int   RXTEN : 1;          //Receiver Transfer Enable
        unsigned int   Reserved0 : 7;
        unsigned int   TXTEN : 1;          //Transmitter Transfer Enable
        unsigned int   Reserved1 : 23;
    };
}PDC_PTSR,*pPDC_PTSR;                       //PDC Transfer Status Register

//AIC
typedef union
{
    unsigned int   aic_smr;
    struct
    {
        unsigned int   PRIOR : 3;          //Priority Level
        unsigned int   Reserved0 : 2;
        unsigned int   SRCTYPE : 2;        //Interrupt Source Type
        unsigned int   Reserved1 : 25;
    };
}AIC_SMR,*pAIC_SMR;                         //AIC Source Mode 0~31 Register

typedef union
{
    unsigned int   aic_svr;
    struct
    {
        unsigned int   VECTOR : 32;        //Source Vector
    };
}AIC_SVR,*pAIC_SVR;                         //AIC Source Vector 0~31 Register

typedef union
{
    unsigned int   aic_ivr;
    struct
    {
        unsigned int   IRQV : 32;          //Interrupt Vector Register
    };
}AIC_IVR,*pAIC_IVR;                         //AIC Interrupt Vector Register

typedef union
{
    unsigned int   aic_fvr;
    struct
    {
        unsigned int   FIQV : 32;          //FIQ Vector Register
    };
}AIC_FVR,*pAIC_FVR;                         //AIC FIQ Vector Register

typedef union
{
    unsigned int   aic_isr;
    struct
    {
        unsigned int   IRQID : 5;          //Current Interrupt Identifer
        unsigned int   Reserved0 : 27;
    };
}AIC_ISR,*pAIC_ISR;                         //AIC Interrupt Status Register

typedef union
{
    unsigned int   aic_ipr;
    struct
    {
        unsigned int   nFIQ : 1;
        unsigned int   SYS : 1;
        unsigned int   PID2 : 1;
        unsigned int   PID3 : 1;
        unsigned int   PID4 : 1;
        unsigned int   PID5 : 1;
        unsigned int   PID6 : 1;
        unsigned int   PID7 : 1;
        unsigned int   PID8 : 1;
        unsigned int   PID9 : 1;
        unsigned int   PID10 : 1;
        unsigned int   PID11 : 1;
        unsigned int   PID12 : 1;
        unsigned int   PID13 : 1;
        unsigned int   PID14 : 1;
        unsigned int   PID15 : 1;
        unsigned int   PID16 : 1;
        unsigned int   PID17 : 1;
        unsigned int   PID18 : 1;
        unsigned int   PID19 : 1;
        unsigned int   PID20 : 1;
        unsigned int   PID21 : 1;
        unsigned int   PID22 : 1;
        unsigned int   PID23 : 1;
        unsigned int   PID24 : 1;
        unsigned int   PID25 : 1;
        unsigned int   PID26 : 1;
        unsigned int   PID27 : 1;
        unsigned int   PID28 : 1;
        unsigned int   PID29 : 1;
        unsigned int   PID30 : 1;
        unsigned int   PID31 : 1;          //Interrupt Pending
    };
}AIC_IPR,*pAIC_IPR;                         //AIC Interrupt Pendign Register

typedef union
{
    unsigned int   aic_imr;
    struct
    {
        unsigned int   nFIQ : 1;
        unsigned int   SYS : 1;
        unsigned int   PID2 : 1;
        unsigned int   PID3 : 1;
        unsigned int   PID4 : 1;
        unsigned int   PID5 : 1;
        unsigned int   PID6 : 1;
        unsigned int   PID7 : 1;
        unsigned int   PID8 : 1;
        unsigned int   PID9 : 1;
        unsigned int   PID10 : 1;
        unsigned int   PID11 : 1;
        unsigned int   PID12 : 1;
        unsigned int   PID13 : 1;
        unsigned int   PID14 : 1;
        unsigned int   PID15 : 1;
        unsigned int   PID16 : 1;
        unsigned int   PID17 : 1;
        unsigned int   PID18 : 1;
        unsigned int   PID19 : 1;
        unsigned int   PID20 : 1;
        unsigned int   PID21 : 1;
        unsigned int   PID22 : 1;
        unsigned int   PID23 : 1;
        unsigned int   PID24 : 1;
        unsigned int   PID25 : 1;
        unsigned int   PID26 : 1;
        unsigned int   PID27 : 1;
        unsigned int   PID28 : 1;
        unsigned int   PID29 : 1;
        unsigned int   PID30 : 1;
        unsigned int   PID31 : 1;          //Interrupt Mask
    };
}AIC_IMR,*pAIC_IMR;                         //AIC Interrupt Mask Register

typedef union
{
    unsigned int   aic_cisr;
    struct
    {
        unsigned int   NIFQ : 1;           //NFIQ Status
        unsigned int   NIRQ : 1;           //NIRQ Status
        unsigned int   Reserved0 : 30;
    };
}AIC_CISR,*pAIC_CISR;                       //AIC Core Interrupt Status Register

typedef union
{
    unsigned int   aic_iecr;
    struct
    {
        unsigned int   nFIQ : 1;
        unsigned int   SYS : 1;
        unsigned int   PID2 : 1;
        unsigned int   PID3 : 1;
        unsigned int   PID4 : 1;
        unsigned int   PID5 : 1;
        unsigned int   PID6 : 1;
        unsigned int   PID7 : 1;
        unsigned int   PID8 : 1;
        unsigned int   PID9 : 1;
        unsigned int   PID10 : 1;
        unsigned int   PID11 : 1;
        unsigned int   PID12 : 1;
        unsigned int   PID13 : 1;
        unsigned int   PID14 : 1;
        unsigned int   PID15 : 1;
        unsigned int   PID16 : 1;
        unsigned int   PID17 : 1;
        unsigned int   PID18 : 1;
        unsigned int   PID19 : 1;
        unsigned int   PID20 : 1;
        unsigned int   PID21 : 1;
        unsigned int   PID22 : 1;
        unsigned int   PID23 : 1;
        unsigned int   PID24 : 1;
        unsigned int   PID25 : 1;
        unsigned int   PID26 : 1;
        unsigned int   PID27 : 1;
        unsigned int   PID28 : 1;
        unsigned int   PID29 : 1;
        unsigned int   PID30 : 1;
        unsigned int   PID31 : 1;          //Interrupt Enable
    };
}AIC_IECR,*pAIC_IECR;                       //AIC Interrupt Enable Command Register

typedef union
{
    unsigned int   aic_idcr;
    struct
    {
        unsigned int   nFIQ : 1;
        unsigned int   SYS : 1;
        unsigned int   PID2 : 1;
        unsigned int   PID3 : 1;
        unsigned int   PID4 : 1;
        unsigned int   PID5 : 1;
        unsigned int   PID6 : 1;
        unsigned int   PID7 : 1;
        unsigned int   PID8 : 1;
        unsigned int   PID9 : 1;
        unsigned int   PID10 : 1;
        unsigned int   PID11 : 1;
        unsigned int   PID12 : 1;
        unsigned int   PID13 : 1;
        unsigned int   PID14 : 1;
        unsigned int   PID15 : 1;
        unsigned int   PID16 : 1;
        unsigned int   PID17 : 1;
        unsigned int   PID18 : 1;
        unsigned int   PID19 : 1;
        unsigned int   PID20 : 1;
        unsigned int   PID21 : 1;
        unsigned int   PID22 : 1;
        unsigned int   PID23 : 1;
        unsigned int   PID24 : 1;
        unsigned int   PID25 : 1;
        unsigned int   PID26 : 1;
        unsigned int   PID27 : 1;
        unsigned int   PID28 : 1;
        unsigned int   PID29 : 1;
        unsigned int   PID30 : 1;
        unsigned int   PID31 : 1;          //Interrupt Disable
    };
}AIC_IDCR,*pAIC_IDCR;                       //AIC Interrupt Disable Command Register

typedef union
{
    unsigned int   aic_iccr;
    struct
    {
        unsigned int   nFIQ : 1;
        unsigned int   SYS : 1;
        unsigned int   PID2 : 1;
        unsigned int   PID3 : 1;
        unsigned int   PID4 : 1;
        unsigned int   PID5 : 1;
        unsigned int   PID6 : 1;
        unsigned int   PID7 : 1;
        unsigned int   PID8 : 1;
        unsigned int   PID9 : 1;
        unsigned int   PID10 : 1;
        unsigned int   PID11 : 1;
        unsigned int   PID12 : 1;
        unsigned int   PID13 : 1;
        unsigned int   PID14 : 1;
        unsigned int   PID15 : 1;
        unsigned int   PID16 : 1;
        unsigned int   PID17 : 1;
        unsigned int   PID18 : 1;
        unsigned int   PID19 : 1;
        unsigned int   PID20 : 1;
        unsigned int   PID21 : 1;
        unsigned int   PID22 : 1;
        unsigned int   PID23 : 1;
        unsigned int   PID24 : 1;
        unsigned int   PID25 : 1;
        unsigned int   PID26 : 1;
        unsigned int   PID27 : 1;
        unsigned int   PID28 : 1;
        unsigned int   PID29 : 1;
        unsigned int   PID30 : 1;
        unsigned int   PID31 : 1;          //Interrupt Clear
    };
}AIC_ICCR,*pAIC_ICCR;                       //AIC Interrupt Clear Command Register

typedef union
{
    unsigned int   aic_iscr;
    struct
    {
        unsigned int   nFIQ : 1;
        unsigned int   SYS : 1;
        unsigned int   PID2 : 1;
        unsigned int   PID3 : 1;
        unsigned int   PID4 : 1;
        unsigned int   PID5 : 1;
        unsigned int   PID6 : 1;
        unsigned int   PID7 : 1;
        unsigned int   PID8 : 1;
        unsigned int   PID9 : 1;
        unsigned int   PID10 : 1;
        unsigned int   PID11 : 1;
        unsigned int   PID12 : 1;
        unsigned int   PID13 : 1;
        unsigned int   PID14 : 1;
        unsigned int   PID15 : 1;
        unsigned int   PID16 : 1;
        unsigned int   PID17 : 1;
        unsigned int   PID18 : 1;
        unsigned int   PID19 : 1;
        unsigned int   PID20 : 1;
        unsigned int   PID21 : 1;
        unsigned int   PID22 : 1;
        unsigned int   PID23 : 1;
        unsigned int   PID24 : 1;
        unsigned int   PID25 : 1;
        unsigned int   PID26 : 1;
        unsigned int   PID27 : 1;
        unsigned int   PID28 : 1;
        unsigned int   PID29 : 1;
        unsigned int   PID30 : 1;
        unsigned int   PID31 : 1;          //Interrupt Set
    };
}AIC_ISCR,*pAIC_ISCR;                       //AIC Interrupt Set Command Register*

typedef union
{
    unsigned int   aic_eoicr;
    struct
    {
        unsigned int   Reserved0 : 32;
    };
}AIC_EOICR,*pAIC_EOICR;                     //AIC End of Interrupt Command Register

typedef union
{
    unsigned int   aic_spu;
    struct
    {
        unsigned int   SIQV : 32;          //Spurious Interrupt Vector Register
    };
}AIC_SPU,*pAIC_SPU;                         //AIC Spurious Interrupt Vector Register

typedef union
{
    unsigned int   aic_debug;
    struct
    {
        unsigned int   PROT : 1;           //Protection Mode
        unsigned int   GMSK : 1;           //General Mask
        unsigned int   Reserved0 : 30;
    };
}AIC_DEBUG,*pAIC_DEBUG;                     //AIC Debug Control Register

typedef union
{
    unsigned int   aic_ffer;
    struct
    {
        unsigned int   Reserved0 : 1;
        unsigned int   SYS : 1;
        unsigned int   PID2 : 1;
        unsigned int   PID3 : 1;
        unsigned int   PID4 : 1;
        unsigned int   PID5 : 1;
        unsigned int   PID6 : 1;
        unsigned int   PID7 : 1;
        unsigned int   PID8 : 1;
        unsigned int   PID9 : 1;
        unsigned int   PID10 : 1;
        unsigned int   PID11 : 1;
        unsigned int   PID12 : 1;
        unsigned int   PID13 : 1;
        unsigned int   PID14 : 1;
        unsigned int   PID15 : 1;
        unsigned int   PID16 : 1;
        unsigned int   PID17 : 1;
        unsigned int   PID18 : 1;
        unsigned int   PID19 : 1;
        unsigned int   PID20 : 1;
        unsigned int   PID21 : 1;
        unsigned int   PID22 : 1;
        unsigned int   PID23 : 1;
        unsigned int   PID24 : 1;
        unsigned int   PID25 : 1;
        unsigned int   PID26 : 1;
        unsigned int   PID27 : 1;
        unsigned int   PID28 : 1;
        unsigned int   PID29 : 1;
        unsigned int   PID30 : 1;
        unsigned int   PID31 : 1;          //Fast Forcing Enable
    };
}AIC_FFER,*pAIC_FFER;                       //AIC Fast Forcing Enable Register

typedef union
{
    unsigned int   aic_ffdr;
    struct
    {
        unsigned int   Reserved0 : 1;
        unsigned int   SYS : 1;
        unsigned int   PID2 : 1;
        unsigned int   PID3 : 1;
        unsigned int   PID4 : 1;
        unsigned int   PID5 : 1;
        unsigned int   PID6 : 1;
        unsigned int   PID7 : 1;
        unsigned int   PID8 : 1;
        unsigned int   PID9 : 1;
        unsigned int   PID10 : 1;
        unsigned int   PID11 : 1;
        unsigned int   PID12 : 1;
        unsigned int   PID13 : 1;
        unsigned int   PID14 : 1;
        unsigned int   PID15 : 1;
        unsigned int   PID16 : 1;
        unsigned int   PID17 : 1;
        unsigned int   PID18 : 1;
        unsigned int   PID19 : 1;
        unsigned int   PID20 : 1;
        unsigned int   PID21 : 1;
        unsigned int   PID22 : 1;
        unsigned int   PID23 : 1;
        unsigned int   PID24 : 1;
        unsigned int   PID25 : 1;
        unsigned int   PID26 : 1;
        unsigned int   PID27 : 1;
        unsigned int   PID28 : 1;
        unsigned int   PID29 : 1;
        unsigned int   PID30 : 1;
        unsigned int   PID31 : 1;          //Fast Forcing Disable
    };
}AIC_FFDR,*pAIC_FFDR;                       //AIC Fast Forcing Disable Register

typedef union
{
    unsigned int   aic_ffsr;
    struct
    {
        unsigned int   Reserved0 : 1;
        unsigned int   SYS : 1;
        unsigned int   PID2 : 1;
        unsigned int   PID3 : 1;
        unsigned int   PID4 : 1;
        unsigned int   PID5 : 1;
        unsigned int   PID6 : 1;
        unsigned int   PID7 : 1;
        unsigned int   PID8 : 1;
        unsigned int   PID9 : 1;
        unsigned int   PID10 : 1;
        unsigned int   PID11 : 1;
        unsigned int   PID12 : 1;
        unsigned int   PID13 : 1;
        unsigned int   PID14 : 1;
        unsigned int   PID15 : 1;
        unsigned int   PID16 : 1;
        unsigned int   PID17 : 1;
        unsigned int   PID18 : 1;
        unsigned int   PID19 : 1;
        unsigned int   PID20 : 1;
        unsigned int   PID21 : 1;
        unsigned int   PID22 : 1;
        unsigned int   PID23 : 1;
        unsigned int   PID24 : 1;
        unsigned int   PID25 : 1;
        unsigned int   PID26 : 1;
        unsigned int   PID27 : 1;
        unsigned int   PID28 : 1;
        unsigned int   PID29 : 1;
        unsigned int   PID30 : 1;
        unsigned int   PID31 : 1;          //Fast Forcing status
    };
}AIC_FFSR,*pAIC_FFSR;                       //AIC Fast Forcing Status Register

//PMC
typedef union
{
    unsigned int   pmc_scer;
    struct
    {
        unsigned int   Reserved0 : 7;
        unsigned int   UDP : 1;            //USB Device Port Clock Enable
        unsigned int   PCK0 : 1;
        unsigned int   PCK1 : 1;
        unsigned int   PCK2 : 1;           //Programmable Clock x Output Enable
        unsigned int   Reserved1 : 21;
    };
}PMC_SCER,*pPMC_SCER;                       //PMC System Clock Enable Register

typedef union
{
    unsigned int   pmc_scdr;
    struct
    {
        unsigned int   PCK : 1;            //Processor Clock Disable
        unsigned int   Reserved0 : 6;
        unsigned int   UDP : 1;            //USB Device Port Clock Disable
        unsigned int   PCK0 : 1;
        unsigned int   PCK1 : 1;
        unsigned int   PCK2 : 1;           //Programmable Clock x output Disable
        unsigned int   Reserved1 : 27;
    };
}PMC_SCDR,*pPMC_SCDR;                       //PMC System Clock Disable register

typedef union
{
    unsigned int   pmc_scsr;
    struct
    {
        unsigned int   PCK : 1;            //Processor Clock status
        unsigned int   Reserved0 : 6;
        unsigned int   UDP : 1;            //USB Device Port Status
        unsigned int   PCK0 : 1;
        unsigned int   PCK1 : 1;
        unsigned int   PCK2 : 1;           //Programmable Clock x output Status
        unsigned int   Reserved1 : 27;
    };
}PMC_SCSR,*pPMC_SCSR;                       //PMC System Clock Status Register

typedef union
{
    unsigned int   pmc_pcer;
    struct
    {
        unsigned int   Reserved0 : 2;
        unsigned int   PID2 : 1;
        unsigned int   PID3 : 1;
        unsigned int   PID4 : 1;
        unsigned int   PID5 : 1;
        unsigned int   PID6 : 1;
        unsigned int   PID7 : 1;
        unsigned int   PID8 : 1;
        unsigned int   PID9 : 1;
        unsigned int   PID10 : 1;
        unsigned int   PID11 : 1;
        unsigned int   PID12 : 1;
        unsigned int   PID13 : 1;
        unsigned int   PID14 : 1;
        unsigned int   PID15 : 1;
        unsigned int   PID16 : 1;
        unsigned int   PID17 : 1;
        unsigned int   PID18 : 1;
        unsigned int   PID19 : 1;
        unsigned int   PID20 : 1;
        unsigned int   PID21 : 1;
        unsigned int   PID22 : 1;
        unsigned int   PID23 : 1;
        unsigned int   PID24 : 1;
        unsigned int   PID25 : 1;
        unsigned int   PID26 : 1;
        unsigned int   PID27 : 1;
        unsigned int   PID28 : 1;
        unsigned int   PID29 : 1;
        unsigned int   PID30 : 1;
        unsigned int   PID31 : 1;          //Peripheral Clock x Enable
    };
}PMC_PCER,*pPMC_PCER;                       //PMC Peripheral Clock Enable Register

typedef union
{
    unsigned int pmc_pcdr;
    struct
    {
        unsigned int   Reserved0 : 2;
        unsigned int   PID2 : 1;
        unsigned int   PID3 : 1;
        unsigned int   PID4 : 1;
        unsigned int   PID5 : 1;
        unsigned int   PID6 : 1;
        unsigned int   PID7 : 1;
        unsigned int   PID8 : 1;
        unsigned int   PID9 : 1;
        unsigned int   PID10 : 1;
        unsigned int   PID11 : 1;
        unsigned int   PID12 : 1;
        unsigned int   PID13 : 1;
        unsigned int   PID14 : 1;
        unsigned int   PID15 : 1;
        unsigned int   PID16 : 1;
        unsigned int   PID17 : 1;
        unsigned int   PID18 : 1;
        unsigned int   PID19 : 1;
        unsigned int   PID20 : 1;
        unsigned int   PID21 : 1;
        unsigned int   PID22 : 1;
        unsigned int   PID23 : 1;
        unsigned int   PID24 : 1;
        unsigned int   PID25 : 1;
        unsigned int   PID26 : 1;
        unsigned int   PID27 : 1;
        unsigned int   PID28 : 1;
        unsigned int   PID29 : 1;
        unsigned int   PID30 : 1;
        unsigned int   PID31 : 1;          //Peripheral Clock x Disable
    };
}PMC_PCDR,*pPMC_PCDR;                       //PMC Peripheral Clock Disable Register

typedef union
{
    unsigned int   pmc_pcsr;
    struct
    {
        unsigned int   Reserved0 : 2;
        unsigned int   PID2 : 1;
        unsigned int   PID3 : 1;
        unsigned int   PID4 : 1;
        unsigned int   PID5 : 1;
        unsigned int   PID6 : 1;
        unsigned int   PID7 : 1;
        unsigned int   PID8 : 1;
        unsigned int   PID9 : 1;
        unsigned int   PID10 : 1;
        unsigned int   PID11 : 1;
        unsigned int   PID12 : 1;
        unsigned int   PID13 : 1;
        unsigned int   PID14 : 1;
        unsigned int   PID15 : 1;
        unsigned int   PID16 : 1;
        unsigned int   PID17 : 1;
        unsigned int   PID18 : 1;
        unsigned int   PID19 : 1;
        unsigned int   PID20 : 1;
        unsigned int   PID21 : 1;
        unsigned int   PID22 : 1;
        unsigned int   PID23 : 1;
        unsigned int   PID24 : 1;
        unsigned int   PID25 : 1;
        unsigned int   PID26 : 1;
        unsigned int   PID27 : 1;
        unsigned int   PID28 : 1;
        unsigned int   PID29 : 1;
        unsigned int   PID30 : 1;
        unsigned int   PID31 : 1;          //Peripheral Clock x Status
    };
}PMC_PCSR,*pPMC_PCSR;                       //PMC Peripheral Clock Status register

typedef union
{
    unsigned int   ckgr_mor;
    struct
    {
        unsigned int   MOSCEN : 1;         //Main OscllaTor Enable
        unsigned int   OSCBYPASS : 1;      //Oscillator Bypass
        unsigned int   Reserved0 : 6;
        unsigned int   OSCOUNT : 8;        //Main Oscillator Start-up Time
        unsigned int   Reserved1 : 16;
    };
}CKGR_MOR,*pCKGR_MOR;                       //PMC Clock Generator Main Oscillator Register

typedef union
{
    unsigned int   ckgr_mcfr;
    struct
    {
        unsigned int   MAINF : 16;         //Main Clock Frequency
        unsigned int   MAINRDY : 1;        //Main Clock Ready
        unsigned int   Reserved0 : 15;
    };
}CKGR_MCFR,*pCKGR_MCFR;                     //PMC Clock Generator Main Clock Frequency Register

typedef union
{
    unsigned int   ckgr_pllr;
    struct
    {
        unsigned int   DIV : 8;            //Divider
        unsigned int   PLLCOUNT : 6;       //PLL Counter
        unsigned int   OUT : 2;            //PLL Clock Frequency Range
        unsigned int   MUL : 11;           //PLL Multipller
        unsigned int   Reserved0 : 1;
        unsigned int   USBDIV : 2;         //Divider for USB Clock
        unsigned int   Reserved1 : 2;
    };
}CKGR_PLLR,*pCKGR_PLLR;                     //PMC Clock Generator PLL Register

typedef union
{
    unsigned int   pmc_mckr;
    struct
    {
        unsigned int   CSS : 2;            //Master Clock Selection
        unsigned int   PRES : 3;           //Processor Clock Prescaler
        unsigned int   Reserved0 : 3;
        unsigned int   MDIV : 2;
        unsigned int   Reserved1 : 22;
    };
}PMC_MCKR,*pPMC_MCKR;                       //PMC Master Clock Register

typedef union
{
    unsigned int   pmc_pck;
    struct
    {
        unsigned int   CSS : 2;            //Master Clock Selection
        unsigned int   PRES : 3;           //Programmable Clock Prescaler
        unsigned int   Reserved0 : 27;
    };
}PMC_PCK,*pPMC_PCK;                         //PMC Programmable Clock 0~7 Register

typedef union
{
    unsigned int   pmc_ier;
    struct
    {
        unsigned int   MOSCS : 1;          //Main Oscillator Status Interrupt Interrupt Enable
        unsigned int   Reserved0 : 1;
        unsigned int   LOCK : 1;           //PLL Lock Interrupt Enable
        unsigned int   MCKRDY : 1;         //Master Clock Ready Interrupt Enable
        unsigned int   Reserved1 : 4;
        unsigned int   PCKRDY0 : 1;
        unsigned int   PCKRDY1 : 1;
        unsigned int   PCKRDY2 : 1;        //Programable Clock Ready x Interrupt Enable
        unsigned int   Reserved2 : 21;
    }; 
}PMC_IER,*pPMC_IER;                         //PMC Interrupt Enable Register

typedef union
{
    unsigned int   pmc_idr;
    struct
    {
        unsigned int   MOSCS : 1;          //Main Oscillator Status Interrupt Disable
        unsigned int   Reserved0 : 1;
        unsigned int   LOCK : 1;           //PLL Lock Interrupt Disable
        unsigned int   MCKRDY : 1;         //Master Clock Ready Interrupt Disable
        unsigned int   Reserved1 : 4;
        unsigned int   PCKRDY0 : 1;
        unsigned int   PCKRDY1 : 1;
        unsigned int   PCKRDY2 : 1;        //Programmable Clock Ready x Interrupt Disable
        unsigned int   Reserved2 : 21;
    };
}PMC_IDR,*pPMC_IDR;                         //PMC Interrupt Disable Register

typedef union
{
    unsigned int   pmc_sr;
    struct
    {
        unsigned int   MOSCS : 1;          //MOSCS Flag Status
        unsigned int   Reserved0 : 1;
        unsigned int   LOCK : 1;           //PLL Lock Status
        unsigned int   MCKRDY : 1;         //Master Clcok Status 
        unsigned int   Reserved1 : 4;
        unsigned int   PCKRDY0 : 1;
        unsigned int   PCKRDY1 : 1;
        unsigned int   PCKRDY2 : 1;        //Programmable Clock Ready Status
        unsigned int   Reserved2 : 21;
    };
}PMC_SR,*pPMC_SR;                           //PMC Status Register

typedef union
{
    unsigned int   pmc_imr;
    struct
    {
        unsigned int   MOSCS : 1;          //Main Oscillator Status Interrupt Mask
        unsigned int   Reserved0 : 1;
        unsigned int   LOCK : 1;           //PLL Lock Interrupt Mask
        unsigned int   MCKRDY : 1;         //Master Clock ready Interupt Mask
        unsigned int   Reserved1 : 4;
        unsigned int   PCKRDY0 : 1;
        unsigned int   PCKRDY1 : 1;
        unsigned int   PCKRDY2 : 1;        //Programmable Clock Ready x Interrupt Mask
        unsigned int   Reserved2 : 21;
    };
}PMC_IMR,*pPMC_IMR;                         //PMC Interrupt Mask Register

//DBGU
typedef union
{
    unsigned int   dbgu_cr;
    struct
    {
        unsigned int   Reserved0 : 2;
        unsigned int   RSTRX : 1;          //Reset Receiver
        unsigned int   RSTTX : 1;          //Reset Transmitter
        unsigned int   RXEN : 1;           //Receiver Enable
        unsigned int   RXDIS : 1;          //Receiver Disable
        unsigned int   TXEN : 1;           //Transmitter Enable
        unsigned int   TXDIS : 1;          //Transmitter Diable
        unsigned int   DSTSTA : 1;         //Reset Status Bits
        unsigned int   Reserved1 : 23;
    };
}DBGU_CR,*pDBGU_CR;                         //Debug Unit Control Register

typedef union
{
    unsigned int   dbgu_mr;
    struct
    {
        unsigned int   Reserved0 : 9;
        unsigned int   PAR : 3;            //Parity Type
        unsigned int   Reserved1 : 2;
        unsigned int   CHMODE : 2;         //Channel Mode
        unsigned int   Reserved2 : 16;
    };
}DBGU_MR,*pDBGU_MR;                         //Debug Unit Mode Register

typedef union
{
    unsigned int   dbgu_ier;
    struct
    {
        unsigned int   RXRDY : 1;          //Enable RXRDY Interrupt
        unsigned int   TXRDY : 1;          //Enable TXRDY Interrupt
        unsigned int   Reserved0 : 1;
        unsigned int   ENDRX : 1;          //Enable End of Receive Transfer Interrupt
        unsigned int   ENDTX : 1;          //Enable End of Transmit Interrupt
        unsigned int   OVRE : 1;           //Enable Overrun Error Interrupt
        unsigned int   FRAME : 1;          //Enable Framing Error Interrupt
        unsigned int   PARE : 1;           //Enable Parity Error Interrupt
        unsigned int   Reserved1 : 1;  
        unsigned int   TXEMPTY : 1;        //Enable TXEMPTY Interrupt
        unsigned int   Reserved2 : 1;
        unsigned int   TXBUFE : 1;         //Enable Buffer Empty Interrupt
        unsigned int   RXBUFF : 1;         //Enable Buffer Full Interrupt
        unsigned int   Reserved3 : 17;
        unsigned int   COMMTX : 1;         //Enable COMMTX(from ARM)Interrupt
        unsigned int   COMMRX : 1;         //Enable COMMRX(from ARM)Interrupt
    };
}DBGU_IER,*pDBGU_IER;                       //Debug Unit Interupt Enable Register

typedef union
{
    unsigned int   dbgu_idr;
    struct
    {
        unsigned int   RXRDY : 1;          //Disable RXRDY Interrupt
        unsigned int   TXRDY : 1;          //Disable TXRDY Interrupt
        unsigned int   Reserved0 : 1;
        unsigned int   ENDRX : 1;          //Disable End of Receive Transfer Interrupt
        unsigned int   ENDTX : 1;          //Disable End of Transmit Interrupt
        unsigned int   OVRE : 1;           //Disable Overrun Error Interrupt
        unsigned int   FRAME : 1;          //Disable Framing Error Interrupt
        unsigned int   PARE : 1;           //Disable Parity Error Interrupt
        unsigned int   Reserved1 : 1;
        unsigned int   TXEMPTY : 1;        //Disable TXEMPTY Interrupt
        unsigned int   Reserved2 : 1;
        unsigned int   TXBUFE : 1;         //Disable Buffer Empty Interrupt
        unsigned int   RXBUFF : 1;         //Disable Buffer Full Interrupt
        unsigned int   Reserved3 : 17;
        unsigned int   COMMTX : 1;         //Disable COMMTX(from ARM)Interrupt
        unsigned int   COMMRX : 1;         //Disable COMMRX(from ARM)Interrupt
    };
}DBGU_IDR,*pDBGU_IDR;                       //Debug Unit Interrupt Disable Register

typedef union
{
    unsigned int   dbgu_imr;
    struct
    {
        unsigned int   RXRDY : 1;          //Mask RXRDY Interrupt
        unsigned int   TXRDY : 1;          //Mask TXRDY Interrupt
        unsigned int   Reserved0 : 1;
        unsigned int   ENDRX : 1;          //Mask End of Receive Transfer Interrupt
        unsigned int   ENDTX : 1;          //Mask End of Transmit Interrupt
        unsigned int   OVRE : 1;           //Mask Overrun Error Interrupt
        unsigned int   FRAME : 1;          //Mask Framing Error Interrupt
        unsigned int   PARE : 1;           //Mask Parity Error Interrupt
        unsigned int   Reserved1 : 1;
        unsigned int   TXEMPTY : 1;        //Mask TXEMPTY Interrupt
        unsigned int   Reserved2 : 1;
        unsigned int   TXBUFE : 1;         //Mask TXBUFE Interrupt
        unsigned int   RXBUFF : 1;         //Mask RXBUFF interrupt
        unsigned int   Reserved3 : 17;
        unsigned int   COMMTX : 1;         //Mask COMMTX Interrupt
        unsigned int   COMMRX : 1;         //Mask COMMRX Interrupt
    };
}DBGU_IMR,*pDBGU_IMR;                       //Debug Unit Interrupt Mask Register

typedef union
{
    unsigned int   dbgu_sr;
    struct
    {
        unsigned int   RXRDY : 1;          //Receiver Ready
        unsigned int   TXRDY : 1;          //Transmitter Ready
        unsigned int   Reserved0 : 1;
        unsigned int   ENDRX : 1;          //End of Receiver Transfer
        unsigned int   ENDTX : 1;          //End of Transmitter Transfer
        unsigned int   OVRE : 1;           //Overrun Error
        unsigned int   FRAME : 1;          //Framing Error
        unsigned int   PARE : 1;           //Parity Error
        unsigned int   Reserved1 : 1;
        unsigned int   TXEMPTY : 1;        //Transmitter Empty
        unsigned int   Reserved2 : 1;
        unsigned int   TXBUFE : 1;         //Transmission Buffer Empty
        unsigned int   RXBUFF : 1;         //Receive Buffer Full
        unsigned int   Reserved3 : 17;
        unsigned int   COMMTX : 1;         //Debug Communication Channel Write Status
        unsigned int   COMMRX : 1;         //Debug Communication Channel Read Status
    };
}DBGU_SR,*pDBGU_SR;                         //debug Unit Status Register

typedef union
{
    unsigned int   dbgu_rhr;
    struct
    {
        unsigned int   RXCHR : 8;          //Received Character
        unsigned int   Reserved0 : 24;
    };
}DBGU_RHR,*pDBGU_RHR;                       //Debug Unit Receiver Holding Register

typedef union
{
    unsigned int   dbgu_thr;
    struct
    {
        unsigned int   TXCHR : 8;          //Character to be Transmitted
        unsigned int   Reserved0 : 24;
    };
}DBGU_THR,*pDBGU_THR;                       //Debug Unit Transmit Holding Register

typedef union
{
    unsigned int   dbgu_brgr;
    struct
    {
        unsigned int   CD : 16;            //Clcok Divisor
        unsigned int   Reserved0 : 16;
    };
}DBGU_BRGR,*pDBGU_BRGR;                     //Debug Unit Baud Rate Generator Register

typedef union
{
    unsigned int   dbgu_cidr;
    struct
    {
        unsigned int   VERSION : 5;        //Version of the Device
        unsigned int   EPROC : 3;          //Embedded Processor
        unsigned int   NVPSIZ : 4;         //Nonvolatile Program Memory Size
        unsigned int   NVPSIZ2 : 4;        //Second Nonvolatile Program Memory Size
        unsigned int   SRAMSIZ : 4;        //Internal SRAM Size
        unsigned int   ARCH : 8;           //Architecture Identifier
        unsigned int   NVPTYP : 3;         //Nonvolatile Program memory Type
        unsigned int   EXT : 1;            //Extension Flag
    };
}DBGU_CIDR,*pDBGU_CIDR;                     //Debug Unit Chip ID Register

typedef union
{
    unsigned int   dbgu_exid;
    struct
    {
        unsigned int   EXID : 32;          //Chip ID Extension
    };
}DBGU_EXID,*pDBGU_EXID;                     //Debug Unit Chip ID Extension Register

typedef union
{
    unsigned int   dbgu_fnr;
    struct
    {
        unsigned int   FNTRST : 1;         //Force NTRST
        unsigned int   Reserved0 : 31;
    };
}DBGU_FNR,*pDBGU_FNR;                       //Debug Unit Force NTRST Register

//PIO
typedef union
{
    unsigned int   pio;
    struct
    {
        unsigned int   P0 : 1;
        unsigned int   P1 : 1;
        unsigned int   P2 : 1;
        unsigned int   P3 : 1;
        unsigned int   P4 : 1;
        unsigned int   P5 : 1;
        unsigned int   P6 : 1;
        unsigned int   P7 : 1;
        unsigned int   P8 : 1;
        unsigned int   P9 : 1;
        unsigned int   P10 : 1;
        unsigned int   P11 : 1;
        unsigned int   P12 : 1;
        unsigned int   P13 : 1;
        unsigned int   P14 : 1;
        unsigned int   P15 : 1;
        unsigned int   P16 : 1;
        unsigned int   P17 : 1;
        unsigned int   P18 : 1;
        unsigned int   P19 : 1;
        unsigned int   P20 : 1;
        unsigned int   P21 : 1;
        unsigned int   P22 : 1;
        unsigned int   P23 : 1;
        unsigned int   P24 : 1;
        unsigned int   P25 : 1;
        unsigned int   P26 : 1;
        unsigned int   P27 : 1;
        unsigned int   P28 : 1;
        unsigned int   P29 : 1;
        unsigned int   P30 : 1;
        unsigned int   P31 : 1;            
    };
}PIO,*pPIO;

//SPI
typedef union
{
    unsigned int   spi_cr;
    struct
    {
        unsigned int   SPIEN : 1;          //SPI Enable
        unsigned int   SPIDIS : 1;         //SPI Disable
        unsigned int   Reserved0 : 5;
        unsigned int   SWRST : 1;          //SPI Sortware Reset
        unsigned int   Reserved1 : 16;
        unsigned int   LASTXFER : 1;       //Last Transfer
        unsigned int   Reserved2 : 7;
    };
}SPI_CR,*pSPI_CR;                           //SPI Control Register

typedef union
{
    unsigned int   spi_mr;
    struct
    {
        unsigned int   MSTR : 1;           //Master/Slave Mode
        unsigned int   PS : 1;             //Peripheral Select
        unsigned int   PCSDEC : 1;         //Chip Select Decode
        unsigned int   Reserved0 : 1;
        unsigned int   MODFDIS : 1;        //Mode Failt Detection
        unsigned int   Reserved1 : 2;
        unsigned int   LLB : 1;            //Local Loopback Enable
        unsigned int   Reserved2 : 8;
        unsigned int   PCS : 4;            //Peripheral Chip Select
        unsigned int   Reserved3 : 4;
        unsigned int   DLYBCS : 8;         //Delay Between Chip Selects
    };
}SPI_MR,*pSPI_MR;                           //SPI Mode Register

typedef union
{
    unsigned int   spi_rdr;
    struct
    {
        unsigned int   RD : 16;            //Receive Data
        unsigned int   PCS : 4;            //Peripheral Chip Select
        unsigned int   Reserved0 : 12;
    };
}SPI_RDR,*pSPI_RDR;                         //SPI Receive Data Register

typedef union
{
    unsigned int   spi_tdr;
    struct
    {
        unsigned int   TD : 16;            //Transmit Data
        unsigned int   PCS : 4;            //Peripheral Chip Select
        unsigned int   Reserved0 : 4;
        unsigned int   LASTXFER : 1;       //Last Transfer
        unsigned int   Reserved1 : 7;
    };
}SPI_TDR,*pSPI_TDR;                         //SPI Transmit Data Register

typedef union
{
    unsigned int   spi_sr;
    struct
    {
        unsigned int   RDRF : 1;           //Receive Data Register Full
        unsigned int   TDRE : 1;           //Transmit Data Register Empty
        unsigned int   MODF : 1;           //Mode Fault Error
        unsigned int   OVRES : 1;          //Overrun Error Status
        unsigned int   ENDRX : 1;          //End of RX Buffer
        unsigned int   ENDTX : 1;          //End of TX Buffer
        unsigned int   RXBUFF : 1;         //RX Buffer Full
        unsigned int   TXBUFE : 1;         //TX Buffer Empty
        unsigned int   NSSR : 1;           //NSS Rising
        unsigned int   TXEMPTY : 1;        //Transmission Register Empty
        unsigned int   Reserved0 : 6;
        unsigned int   SPIENS : 1;         //SPI Enable Status
        unsigned int   Reserved1 : 15;
    };
}SPI_SR,*pSPI_SR;                           //SPI Status Register

typedef union
{
    unsigned int   spi_ier;
    struct 
    {
        unsigned int   RDRF : 1;           //Receive Data Register Full Interrup Enable
        unsigned int   TDRE : 1;           //SPI Transmit Data Register Empty Interrupt Enable
        unsigned int   MODF : 1;           //Mode Fault Error Interrupt Enable
        unsigned int   OVRES : 1;          //Overrun Error Interrupt Enable
        unsigned int   ENDRX : 1;          //End of Receive Buffer Interrupt Enable
        unsigned int   ENDTX : 1;          //End of Transmit Buffer Interrupt Enable
        unsigned int   RXBUFF : 1;         //Receive Buffer Full Interrupt Enable
        unsigned int   TXBUFE : 1;         //Transmit Buffer Empty Interrupt Enable
        unsigned int   NSSR : 1;           //NSS Rising Interrupt Enable
        unsigned int   TXEMPTY : 1;        //Transmission Registers Empty Enable
        unsigned int   Reserved0 : 22;
    };
}SPI_IER,*pSPI_IER;                         //SPI Interrupt Enable Register

typedef union
{
    unsigned int   spi_idr;
    struct
    {
        unsigned int   RDRF : 1;           //Receive Data Register Full Interrupt Disable
        unsigned int   TDRE : 1;           //SPI Transmit Data Register Empty Interrupt Disable
        unsigned int   MODF : 1;           //Mode Fault Error Interrupt Disable
        unsigned int   OVRES : 1;          //Overrun Error Interrupt Disable
        unsigned int   ENDRX : 1;          //End of Receive Buffer Interrupt Disable
        unsigned int   ENDTX : 1;          //End of Transmit Buffer Interrupt Disable
        unsigned int   RXBUFF : 1;         //receive Buffer Full Interrupt Disable
        unsigned int   TXBUFE : 1;         //Transmit Buffer Empty Interrupt Disable
        unsigned int   NSSR : 1;           //NSS Rising Interrupt Disable
        unsigned int   TXEMPTY : 1;        //Transmission Register Empty Disable
        unsigned int   Reserved0 : 22;
    };
}SPI_IDR,*pSPI_IDR;                         //SPI Interrupt Disable Register

typedef union
{
    unsigned int   spi_imr;
    struct
    {
        unsigned int   RDRF : 1;           //Receive Data Register Full Interrupt Mask
        unsigned int   TDRE : 1;           //SPI Transmit Data Register Empty Interrupt Mask
        unsigned int   MODF : 1;           //Mode Fault Error Interrupt Mask
        unsigned int   OVRES : 1;          //Overrun Error Interrupt Mask
        unsigned int   ENDRX : 1;          //End of Receive Buffer Interrupt Mask
        unsigned int   ENDTX : 1;          //End of Transmit Buffer Interrupt Mask
        unsigned int   RXBUFF : 1;         //Receive Buffer Full Interrupt Mask
        unsigned int   RXBUFE : 1;         //Transmit Buffer Empty Interrupt Mask
        unsigned int   NSSR : 1;           //NSS Rising Interrupt Mask
        unsigned int   TXEMPTY : 1;        //Transmission Registers Empty Mask
        unsigned int   Reserved0 : 22;
    };
}SPI_IMR,*pSPI_IMR;                         //SPI Interrupt Mask register

typedef union
{
    unsigned int   spi_csr;
    struct
    {
        unsigned int   CPOL : 1;           //Clock Polarity
        unsigned int   NCPHA : 1;          //Clock Phase
        unsigned int   Reserved0 : 1;
        unsigned int   CSAAT : 1;          //Chip Select Active After Transfer
        unsigned int   BITS : 4;           //Bits Per Transfer
        unsigned int   SCBR : 8;           //Serial Clock Baud Rate
        unsigned int   DLYBS : 8;          //Delay Before SPCK
        unsigned int   DLYBCT : 8;         //Delay Between Consecutive Transfers
    };
}SPI_CSR,*pSPI_CSR;                         //SPI Chip Select 0~3 Regsiter

//TWI
typedef union
{
    unsigned int   twi_cr;
    struct
    {
        unsigned int   START : 1;          //Send a START Condition
        unsigned int   STOP : 1;           //Send a STOP Condition
        unsigned int   MSEN : 1;           //TWI Master Mode Enabled
        unsigned int   MSDIS : 1;          //TWI Master Mode Disabled
        unsigned int   SVEN : 1;           //TWI Slave Mode Enabled
        unsigned int   SVDIS : 1;          //TWI Slave Mode Disabled
        unsigned int   Reserved0 : 1;
        unsigned int   SWRST : 1;          //Software Reset
        unsigned int   Reserved1 : 24;
    };
}TWI_CR,*pTWI_CR;                           //TWI Control Register

typedef union
{
    unsigned int   twi_mmr;
    struct
    {
        unsigned int   Reserved0 : 8;
        unsigned int   IADRSZ : 2;         //Internal Device Address Size
        unsigned int   Reserved1 : 2;
        unsigned int   MREAD : 1;          //Master Read Direction
        unsigned int   Reserved2 : 3;
        unsigned int   DADR : 7;           //Device Address
        unsigned int   Reserved3 : 9;
    };
}TWI_MMR,*pTWI_MMR;                         //TWI Master Mode Register

typedef union
{
    unsigned int   twi_smr;
    struct
    {
        unsigned int   Reserved0 : 16;
        unsigned int   SADR : 7;           //Slave Address
        unsigned int   Reserved1 : 9;
    };
}TWI_SMR,*pTWI_SMR;                         //TWI Slave Mode Register

typedef union
{
    unsigned int   twi_iadr;
    struct
    {
        unsigned int   IADR : 24;          //Internal Address(0,1,2,3 bytes depending on IADRSZ)
        unsigned int   Reserved0 : 8;
    };
}TWI_IADR,*pTWI_IADR;                       //TWi Internal Address Register

typedef union
{
    unsigned int   twi_cwgr;
    struct
    {
        unsigned int   CLDIV : 8;          //Clcok Low Divider
        unsigned int   CHDIV : 8;          //Clcok High Divider
        unsigned int   CKDIV : 3;          //Clock Divider
        unsigned int   Reserved0 : 13;
    };
}TWI_CWGR,*pTWI_CWGR;                       //TWI Clock WaveForm Generator Register

typedef union
{
    unsigned int   twi_sr;
    struct
    {
        unsigned int   TXCOMP : 1;         //Transmission Completed
        unsigned int   RXRDY : 1;          //Receive Holding Register Ready
        unsigned int   TXRDY : 1;          //Transmit Holding Register Ready
        unsigned int   SVREAD : 1;         //Slave Read
        unsigned int   SVACC : 1;          //Slave Access
        unsigned int   GACC : 1;           //General Call Access
        unsigned int   OVRE : 1;           //Overrun error
        unsigned int   Reserved0 : 1;
        unsigned int   NACK : 1;           //Not Acknowledged
        unsigned int   ARBLST : 1;         //Arbitration Lost
        unsigned int   SCLWS : 1;          //Clock Wait State
        unsigned int   EOSACC : 1;         //End of Slave Access
        unsigned int   Reserved1 : 20;
    };
}TWI_SR,*pTWI_SR;                           //TWI Status Register

typedef union
{
    unsigned int   twi_ier;
    struct
    {
        unsigned int   TXCOMP : 1;         //Transmission Completed Interrupt Enable
        unsigned int   RXRDY : 1;          //Receive Holding Register Ready Interrupt Enable
        unsigned int   TXRDY : 1;          //Transmit Holding Register Ready Interrupt Enable
        unsigned int   Reserved0 : 1;
        unsigned int   SVACC : 1;          //Slave Access Interrupt Enable
        unsigned int   GACC : 1;           //General Call Access Interrupt Enable
        unsigned int   OVRE : 1;           //Overrun Error Interrupt Enable
        unsigned int   Reserved1 : 1;
        unsigned int   NACK : 1;           //Not Acknowledge Interrupt Enable
        unsigned int   ARBLST : 1;         //Arbitration Lost Interrupt Enable
        unsigned int   SCLWS : 1;          //Clcok Wait State Interrupt Enable
        unsigned int   EOSACC : 1;         //End of Slave Access Interrupt Enable
        unsigned int   Reserved2 : 20;
    };
}TWI_IER,*pTWI_IER;                         //TWI Interrupt Enable Register

typedef union
{
    unsigned int   twi_idr;
    struct
    {
        unsigned int   TXCOMP : 1;         //Transmission Completed Interrupt Disable
        unsigned int   RXRDY : 1;          //Receive Holding Register Ready Interrupt Disable
        unsigned int   TXRDY : 1;          //Transmit Holding Register Ready Interrupt Disable
        unsigned int   Reserved0 : 1;
        unsigned int   SVACC : 1;          //Slave Access Interrupt Disable
        unsigned int   GACC : 1;           //General Call Access Interrupt Disable
        unsigned int   OVRE : 1;           //Overrun Error Interrupt Disable
        unsigned int   Reserved1 : 1;      
        unsigned int   NACK : 1;           //Not Acknowledge Interrupt Disable
        unsigned int   ARBLST : 1;         //Arbitration Lost Interrupt Disable
        unsigned int   SCLWS : 1;          //Clock Wait State Interrupt Disable
        unsigned int   EOSACC : 1;         //End of Slave Access Interrupt Disable
        unsigned int   Reserved2 : 20;
    };
}TWI_IDR,*pTWI_IDR;                         //TWI Interrupt Disable Register

typedef union
{
    unsigned int   twi_imr;
    struct
    {
        unsigned int   TXCOMP : 1;         //Transmission Completed Interrupt Mask
        unsigned int   RXRDY : 1;          //Receive Holding Register Ready Interrupt Mask
        unsigned int   TXRDY : 1;          //Transmit Holding Register Ready Interrupt Mask
        unsigned int   Reserved0 : 1;
        unsigned int   SVACC : 1;          //Slave Access Interrupt Mask
        unsigned int   GACC : 1;           //General Call Access Interrupt Mask
        unsigned int   OVRE : 1;           //Overrun Error Interrupt Mask
        unsigned int   Reserved1 : 1;
        unsigned int   NACK : 1;           //Not Acknowledge Interrupt Mask
        unsigned int   ARBLST : 1;         //Arbitration Lost interrupt Mask
        unsigned int   SCLWS : 1;          //Clock Wait State Interrupt Mask
        unsigned int   EOSACC : 1;         //End of Slave Access Interrupt Mask
        unsigned int   Reserved2 : 20;
    };
}TWI_IMR,*pTWI_IMR;                         //TWI Interrupt Mask Register

typedef union
{
    unsigned int   twi_rhr;
    struct
    {
        unsigned int   RXDATA : 8;         //Master or Slave Receive Holding Data
        unsigned int   Reserved0 : 24;
    };
}TWI_RHR,*pTWI_RHR;                         //TWI Receive Holding Register

typedef union
{
    unsigned int   twi_thr;
    struct
    {
        unsigned int   TXDATA : 8;         //Master or Slave Transmit Holding Data
        unsigned int   Reserved0 : 24;
    };
}TWI_THR,*pTWI_THR;                         //TWI Transmit Holding Register

//USART
typedef union
{
    unsigned int   us_cr;
    struct
    {
        unsigned int   Reserved0 : 2;
        unsigned int   RSTRX : 1;          //Reset Receiver
        unsigned int   RSTTX : 1;          //Reset Transmitter
        unsigned int   RXEN : 1;           //Receiver Enable
        unsigned int   RXDIS : 1;          //Receiver Disable
        unsigned int   TXEN : 1;           //Transmitter Enable
        unsigned int   TXDIS : 1;          //Transmitter Disable
        unsigned int   RSTSTA : 1;         //Reset Status Bits
        unsigned int   STTBRK : 1;         //Start Break
        unsigned int   STPBRK : 1;         //Stop Break
        unsigned int   STTTO : 1;          //Start Time-out
        unsigned int   SEMDA : 1;          //Send Address
        unsigned int   RSTIT : 1;          //Reset Iterations
        unsigned int   RSTNACK : 1;        //Reset Non Acknowledge
        unsigned int   RETTO : 1;          //Restart Time-out
        unsigned int   DTREN : 1;          //Data Terminal Ready Enable
        unsigned int   DTRDIS : 1;         //Data Terminal Ready Disable
        unsigned int   RTSEN : 1;          //Request to Send Enable
        unsigned int   RTSDIS : 1;         //Request to Send Disable
        unsigned int   Reserved1 : 12;
    };
}US_CR,*pUS_CR;                             //USART Control Register

typedef union
{
    unsigned int   us_mr;
    struct
    {
        unsigned int   USART_MODE : 4;     //USART Mode
        unsigned int   USCLKS : 2;         //Clock Selection
        unsigned int   CHRL : 2;           //Character Length
        unsigned int   SYNC : 1;           //Synchronous Mode Select
        unsigned int   PAR : 3;            //Parity Type
        unsigned int   NBSTOP : 2;         //Number of Stop Bits
        unsigned int   CHMODE : 2;         //Channel Mode
        unsigned int   MSBF : 1;           //Bit Order
        unsigned int   MODE9 : 1;          //9-bit Character Length
        unsigned int   CLKO : 1;           //Clock Output Select
        unsigned int   OVER : 1;           //Oversampling Mode
        unsigned int   INACK : 1;          //Inhibit Non Acknowledge
        unsigned int   DSNACK : 1;         //Disable Successive NACK
        unsigned int   Reserved0 : 2;  
        unsigned int   MAX_ITERATION : 3;  //
        unsigned int   Reserved1 : 1;
        unsigned int   FILTER : 1;         //Infrared Receive Line Filter
        unsigned int   Reserved2 : 3;
    };
}US_MR,*pUS_MR;                             //USART Mode Register

typedef union
{
    unsigned int   us_ier;
    struct
    {
        unsigned int   RXRDY : 1;          //RXRDY Interrupt Enable
        unsigned int   TXRDY : 1;          //TXRDY Interrupt Enable
        unsigned int   RXBRK : 1;          //Receiver Break Interrupt Enable
        unsigned int   ENDRX : 1;          //End of Receiver Transfer Interrupt Enable
        unsigned int   ENDTX : 1;          //End of Transmit Interrupt Enable
        unsigned int   OVRE : 1;           //Overrun Error Interrupt Enable
        unsigned int   FRAME : 1;          //Framing Error Interrupt Enable
        unsigned int   PARE : 1;           //Parity Error Interrupt Enable
        unsigned int   TIMEOUT : 1;        //Time-out Interrupt Enable
        unsigned int   TXEMPRY : 1;        //TXEMPTY Interrupt Enable
        unsigned int   ITERATION : 1;      //Iteration Interrupt enable
        unsigned int   TXBUFE : 1;         //Buffer Empty Interrupt Enable
        unsigned int   RXBUFF : 1;         //Buffer Full interrupt Enable
        unsigned int   NACK : 1;           //Non Acknowledge Interrupt Enable
        unsigned int   Reserved0 : 2;
        unsigned int   RIIC : 1;           //Ring Indicator Input Change Enable
        unsigned int   DSRIC : 1;          //Data Set Ready Input Change Interrupt Enable
        unsigned int   DCDIC : 1;          //Data carrier Detect Input Change Interrupt Enable
        unsigned int   CTSIC : 1;          //Clear to Send Input Change Interrupt Enable
        unsigned int   Reserved1 : 12;
    };
}US_IER,*pUS_IER;                           //USART Interrupt Enable Register

typedef union
{
    unsigned int   us_idr;
    struct
    {
        unsigned int   RXRDY : 1;          //RXRDY Interrupt Disable
        unsigned int   TXRDY : 1;          //TXRDY Interrupt Disable
        unsigned int   RXBRK : 1;          //Receiver Break Interrupt Disable
        unsigned int   ENDRX : 1;          //End of Receiver Transfer Interrupt Disable
        unsigned int   ENDTX : 1;          //End of Transmit Interrupt Disable
        unsigned int   OVRE : 1;           //Overrun Error Interrupt Disable
        unsigned int   FRAME : 1;          //Framing Error Interrupt Disable
        unsigned int   PARE : 1;           //Parity Error Interrupt Disable
        unsigned int   TIMEOUT : 1;        //Time-out Interrupt Disable
        unsigned int   TXEMPRY : 1;        //TXEMPTY Interrupt Disable
        unsigned int   ITERATION : 1;      //Iteration Interrupt Disable
        unsigned int   TXBUFE : 1;         //Buffer Empty Interrupt Disable
        unsigned int   RXBUFF : 1;         //Buffer Full interrupt Disable
        unsigned int   NACK : 1;           //Non Acknowledge Interrupt Disable
        unsigned int   Reserved0 : 2;
        unsigned int   RIIC : 1;           //Ring Indicator Input Change Disable
        unsigned int   DSRIC : 1;          //Data Set Ready Input Change Interrupt Disable
        unsigned int   DCDIC : 1;          //Data carrier Detect Input Change Interrupt Disable
        unsigned int   CTSIC : 1;          //Clear to Send Input Change Interrupt Disable
        unsigned int   Reserved1 : 12;
    };
}US_IDR,*pUS_IDR;                           //USART Interrupt Disable Register

typedef union
{
    unsigned int   us_imr;
    struct
    {
        unsigned int   RXRDY : 1;          //RXRDY Interrupt Mask
        unsigned int   TXRDY : 1;          //TXRDY Interrupt Mask
        unsigned int   RXBRK : 1;          //Receiver Break Interrupt Mask
        unsigned int   ENDRX : 1;          //End of Receiver Transfer Interrupt Mask
        unsigned int   ENDTX : 1;          //End of Transmit Interrupt Mask
        unsigned int   OVRE : 1;           //Overrun Error Interrupt Mask
        unsigned int   FRAME : 1;          //Framing Error Interrupt Mask
        unsigned int   PARE : 1;           //Parity Error Interrupt Mask
        unsigned int   TIMEOUT : 1;        //Time-out Interrupt Mask
        unsigned int   TXEMPRY : 1;        //TXEMPTY Interrupt Mask
        unsigned int   ITERATION : 1;      //Iteration Interrupt Mask
        unsigned int   TXBUFE : 1;         //Buffer Empty Interrupt Mask
        unsigned int   RXBUFF : 1;         //Buffer Full interrupt Mask
        unsigned int   NACK : 1;           //Non Acknowledge Interrupt Mask
        unsigned int   Reserved0 : 2;
        unsigned int   RIIC : 1;           //Ring Indicator Input Change Mask
        unsigned int   DSRIC : 1;          //Data Set Ready Input Change Interrupt Mask
        unsigned int   DCDIC : 1;          //Data carrier Detect Input Change Interrupt Mask
        unsigned int   CTSIC : 1;          //Clear to Send Input Change Interrupt Mask
        unsigned int   Reserved1 : 12;
    };
}US_IMR,*pUS_IMR;                           //USART Interrupt Mask Register

typedef union
{
    unsigned int   us_csr;
    struct
    {
        unsigned int   RXRDY : 1;          //Receiver Ready
        unsigned int   TXRDY : 1;          //Transmitter Ready
        unsigned int   RXBRK : 1;          //Break Received/End of Break
        unsigned int   ENDRX : 1;          //End Receiver Transfer
        unsigned int   ENDTX : 1;          //End of Transmitter Transfer
        unsigned int   OVRE : 1;           //Overrun error
        unsigned int   FRAME : 1;          //Framing error
        unsigned int   PARE : 1;           //Parity Error
        unsigned int   TIMEOUT : 1;        //Receiver Timer-out
        unsigned int   TXEMPTY : 1;        //Transmitter Empty
        unsigned int   ITERATION : 1;      //Max Number of Repetitions Reached
        unsigned int   TXBUFER : 1;        //Transmission Buffer Empty
        unsigned int   RXBUFF : 1;         //Reception Buffer Full
        unsigned int   NACK : 1;           //Non Acknowledge
        unsigned int   Reserved0 : 2;
        unsigned int   RIIC : 1;           //Ring Indicator Input Change Flag
        unsigned int   DSRIC : 1;          //Data Set Ready Input Change Flag
        unsigned int   DCDIC : 1;          //Data Carrier Detect Input Change Flag
        unsigned int   CTSIC : 1;          //Clear to Send Input Change Flag
        unsigned int   RI : 1;             //Image of RI Input
        unsigned int   DSR : 1;            //Image of DSR Input
        unsigned int   DCD : 1;            //Image of DCD Input
        unsigned int   CTS : 1;            //Image of CTS Input
        unsigned int   Reserved1 : 8;
    };
}US_CSR,*pUS_CSR;                           //USART Channel Status Register

typedef union
{
    unsigned int   us_rhr;
    struct
    {
        unsigned int   RXCHR : 9;          //Received Character
        unsigned int   Reserved0 : 6;
        unsigned int   RXSYNH : 1;         //Received Sync
        unsigned int   Reserved1 : 16;
    };
}US_RHR,*pUS_RHR;                           //USART Receive Holding Register

typedef union
{
    unsigned int   us_thr;
    struct
    {
        unsigned int   TXCHR : 9;          //Character to be Transmitted
        unsigned int   Reserved0 : 6;
        unsigned int   TXSYNH : 1;         //Sync Field to be transmitted
        unsigned int   Reserved1 : 16;
    };
}US_THR,*pUS_THR;                           //USART Transmit Holding Register

typedef union
{
    unsigned int   us_brgr;
    struct
    {
        unsigned int   CD : 16;            //Clock Divider
        unsigned int   FP : 3;             //Fractional Part
        unsigned int   Reserved0 : 13;  
    };
}US_BRGR,*pUS_BRGR;                         //USART Baud Rate Generator Register

typedef union
{
    unsigned int   us_rtor;
    struct
    {
        unsigned int   TO : 16;            //Time-out Value
        unsigned int   Reserved0 : 16;
    }; 
}US_RTOR,*pUS_RTOR;                         //USART Receiver Time-out Register

typedef union
{
    unsigned int   us_ttgr;
    struct
    {
        unsigned int   TG : 8;             //Timeguard Value
        unsigned int   Reserved0 : 24;
    };
}US_TTGR,*pUS_TTGR;                         //USART Transmitter Timeguard Register

typedef union
{
    unsigned int   us_fidi;
    struct
    {
        unsigned int   FI_DI_RATIO : 11;   //FI Over DI Radtio Value
        unsigned int   Reserved0 : 21;
    };
}US_FIDI,*pUS_FIDI;                         //USART FI DI RATIO Register

typedef union
{
    unsigned int   us_ner;
    struct
    {
        unsigned int   NB_ERRORS : 8;      //Number of Errors
        unsigned int   Reserved0 : 24;
    };
}US_NER,*pUS_NER;                           //USART Number of Errors Register

typedef union
{
    unsigned int   us_if;
    struct
    {
        unsigned int   IRDA_FILTER : 8;    //IrDA Filter
        unsigned int   Reserved0 : 24;
    };
}US_IF,*pUS_IF;                             //USART IrDA FILTER Register

//SSC
typedef union
{
    unsigned int   ssc_cr;
    struct
    {
        unsigned int   RXEN : 1;           //Receive Enable
        unsigned int   RXDIS : 1;          //Receive Disable
        unsigned int   Reserved0 : 6;
        unsigned int   TXEN : 1;           //Transmit Enable
        unsigned int   TXDIS : 1;          //Transmit Disable
        unsigned int   Reserved1 : 5;
        unsigned int   SWRST : 1;          //Software Reset
        unsigned int   Reserved2 : 16;
    };
}SSC_CR,*pSSC_CR;                           //SSC Control Register

typedef union
{
    unsigned int   ssc_cmr;
    struct
    {
        unsigned int   DIV : 12;           //Clock Divider
        unsigned int   Reserved0 : 20;  
    };
}SSC_CMR,*pSSC_CMR;                         //SSC Clock Mode Register

typedef union
{
    unsigned int   ssc_rcmr;
    struct
    {
        unsigned int   CKS : 2;            //Receive Clock Selection
        unsigned int   CKO : 3;            //Receive Clock Output Mode Selection
        unsigned int   CKI : 1;            //Receive Clock Inversion
        unsigned int   CKG : 2;            //Receive Clock Gating Selection
        unsigned int   START : 4;          //Receive Start Selection
        unsigned int   STOP : 1;           //Receive Stop Selection
        unsigned int   Reserved0 : 3;
        unsigned int   STDDLY : 8;         //Receive Start Delay
        unsigned int   PERIOD : 8;         //Receive Period Divider Selection
    };
}SSC_RCMR,*pSSC_RCMR;                       //SSC Receive Clock Mode Register

typedef union
{
    unsigned int   ssc_rfmr;
    struct
    {
        unsigned int   DATLEN : 5;         //Data Length
        unsigned int   LOOP : 1;           //Loop Mode
        unsigned int   Reserved0 : 1;
        unsigned int   MSBF : 1;           //Most Significant Bit First
        unsigned int   DATNB : 4;          //Data Number per Frame
        unsigned int   Reserved1 : 4;
        unsigned int   FSLEN : 4;          //Receive Frmae Sync Length
        unsigned int   FSOS : 3;           //Receive Frame Sync Output Selection
        unsigned int   Reserved2 : 1;
        unsigned int   FSEDGE : 1;         //Frame Sync Edge Detection
        unsigned int   Reserved3 : 7;
    };
}SSC_RFMR,*pSSC_RFMR;                       //SSC Receive Frame Mode Register

typedef union
{
    unsigned int   ssc_tcmr;
    struct
    {
        unsigned int   CKS : 2;            //Transmit Clock Selection
        unsigned int   CKO : 3;            //Transmit Clock Output Mode Selection
        unsigned int   CKI : 1;            //Transmit Clock Inversion
        unsigned int   CKG : 2;            //Transmit Clock Gating Selection
        unsigned int   START : 4;          //Transmit Start Selection
        unsigned int   Reserved0 : 4;
        unsigned int   STTDLY : 8;         //Transmit Start Delay
        unsigned int   PERIOD : 8;         //Transmit Period Divider Selection
    };
}SSC_TCMR,*pSSC_TCMR;                       //SSC Transmit Clock Mode Register

typedef union
{
    unsigned int   ssc_tfmr;
    struct
    {
        unsigned int   DATALEN : 5;        //Data Length
        unsigned int   DATDEF : 1;         //Data Default Value
        unsigned int   Reserved0 : 1;
        unsigned int   MSBF : 1;           //Most Significant Bit First
        unsigned int   DATNB : 4;          //Data Number per frame
        unsigned int   Reserved1 : 4;
        unsigned int   FSLEN : 4;          //Transmit Frame Sync Length
        unsigned int   FSOS : 3;           //Transmit Frame Sync Output Selection
        unsigned int   FSDEN : 1;          //Frame Sync Data Enable
        unsigned int   FSEDGE : 1;         //Frame Sync Edge Detection
        unsigned int   Reserved2 : 7;
    };
}SSC_TFMR,*pSSC_TFMR;                       //SSC Transmit Frame Mode Register

typedef union
{
    unsigned int   ssc_rhr;
    struct
    {
        unsigned int   RDAT : 32;          //Receive Data
    };
}SSC_RHR,*pSSC_RHR;                         //SSC Receive Holding Register

typedef union
{
    unsigned int   ssc_thr;
    struct
    {
        unsigned int   TDAT : 32;          //Transmit Data
    };
}SSC_THR,*pSSC_THR;                         //SSC Transmit Holding Register

typedef union
{
    unsigned int   ssc_rshr;
    struct
    {
        unsigned int   RSDAT : 16;         //Receive Synchronization Data
        unsigned int   Reserved0 : 16;
    };
}SSC_RSHR,*pSSC_RSHR;                       //SSC Receive Synchronization Holding Register

typedef union
{
    unsigned int   ssc_tshr;
    struct
    {
        unsigned int   TSDAT : 16;         //Transmit Synchronization Data
        unsigned int   Reserved0 : 16;
    };
}SSC_TSHR,*pSSC_TSHR;                       //SSC Transmit Synchronization Holding Register

typedef union
{
    unsigned int   ssc_rc0r;
    struct
    {
        unsigned int   CP0 : 16;           //Receive Compare Data 0
        unsigned int   Reserved0 : 16;
    };
}SSC_RC0R,*pSSC_RC0R;                       //SSC Receive Compare 0 Register

typedef union
{
    unsigned int   ssc_rc1r;
    struct
    {
        unsigned int   CP1 : 16;           //Recceive Compare Data 1
        unsigned int   Reserved0 : 16;
    };
}SSC_RC1R,*pSSC_RC1R;                       //SSC Receive Compare 1 Register

typedef union
{
    unsigned int   ssc_sr;
    struct
    {
        unsigned int   TXRDY : 1;          //Transmit Ready
        unsigned int   TXEMPTY : 1;        //Transmit Empty
        unsigned int   ENDTX : 1;          //End of Transmission
        unsigned int   TXBUFE : 1;         //Transmit Buffer Empty
        unsigned int   RXRDY : 1;          //Receive Ready
        unsigned int   OVRUN : 1;          //Receiv Overrun
        unsigned int   ENDRX : 1;          //End of Reception
        unsigned int   RXBUFF : 1;         //Receive Buffer Full
        unsigned int   CP0 : 1;            //Compare 0
        unsigned int   CP1 : 1;            //Compare 1
        unsigned int   TXSYN : 1;          //Transmit Sync
        unsigned int   RXSYN : 1;          //Receive Sync
        unsigned int   Reserved0 : 4;
        unsigned int   TXEN : 1;           //Transmit Enable
        unsigned int   RXEN : 1;           //Receive Enable
        unsigned int   Reserved1 : 14;
    };
}SSC_SR,*pSSC_SR;                           //SSC Status Register

typedef union
{
    unsigned int   ssc_ier;
    struct
    {
        unsigned int   TXRDY : 1;          //Transmit Ready Interrupt Enable
        unsigned int   TXEMPTY : 1;        //Transmit Empty Interrupt Enable
        unsigned int   ENDTX : 1;          //End of Transmission Interrupt Enable
        unsigned int   TXBUFE : 1;         //Transmit Buffer Empty Interrupt Enable
        unsigned int   RXRDY : 1;          //Receive Ready Interrupt Enable
        unsigned int   OVRUN : 1;          //Receive Overrun Interrupt Enable
        unsigned int   ENDRX : 1;          //End of Reception Interrupt Enable
        unsigned int   RXBUFF : 1;         //Receive Buffer Full Interrupt Enable
        unsigned int   CP0 : 1;            //Compare 0 Interrupt Enable
        unsigned int   CP1 : 1;            //Compare 1 Interrupt Enable
        unsigned int   TXSYN : 1;          //Tx Sync Interrupt Enable
        unsigned int   RXSYN : 1;          //Rx Sync Interrupt Enable
        unsigned int   Reserved0 : 20;
    };
}SSC_IER,*pSSC_IER;                         //SSC Interrupt Enable register

typedef union
{
    unsigned int   ssc_idr;
    struct
    {
        unsigned int   TXRDY : 1;          //Transmit Ready Interrupt Disable
        unsigned int   TXEMPTY : 1;        //Transmit Empty Interrupt Disable
        unsigned int   ENDTX : 1;          //End of Transmission Interrupt Disable
        unsigned int   TXBUFE : 1;         //transmit Buffer Empty Interrupt Disable
        unsigned int   RXRDY : 1;          //Receive Ready Interrupt Disable
        unsigned int   OVRUN : 1;          //Receive Overrun Interrupt Disable
        unsigned int   ENDRX : 1;          //End of Reception Interrupt Disable
        unsigned int   RXBUFF: 1;          //Receive Buffer Full Interrupt Disable
        unsigned int   CP0 : 1;            //Compare 0 Interrupt Disable
        unsigned int   CP1 : 1;            //Compare 1 Interrupt Disable
        unsigned int   TXSYN : 1;          //Tx Sync Interrupt Disable
        unsigned int   RXSYN : 1;          //Rx Sync Interrupt Disable
        unsigned int   Reserved0 : 20;
    };
}SSC_IDR,*pSSC_IDR;                         //SSC Interrupt Disable Register

typedef union
{
    unsigned int   ssc_imr;
    struct
    {
        unsigned int   TXRDY : 1;          //Transmit Ready Interrupt Mask
        unsigned int   TXEMPTY : 1;        //Transmit Empty Interrupt Mask
        unsigned int   ENDTX : 1;          //End of Transmission Interrupt Mask
        unsigned int   TXBUFE : 1;         //transmit Buffer Empty Interrupt Mask
        unsigned int   RXRDY : 1;          //Receive Ready Interrupt Mask
        unsigned int   OVRUN : 1;          //Receive Overrun Interrupt Mask
        unsigned int   ENDRX : 1;          //End of Reception Interrupt Mask
        unsigned int   RXBUFF: 1;          //Receive Buffer Full Interrupt Mask
        unsigned int   CP0 : 1;            //Compare 0 Interrupt Mask
        unsigned int   CP1 : 1;            //Compare 1 Interrupt Mask
        unsigned int   TXSYN : 1;          //Tx Sync Interrupt Mask
        unsigned int   RXSYN : 1;          //Rx Sync Interrupt Mask
        unsigned int   Reserved0 : 20;
    };
}SSC_IMR,*pSSC_IMR;                         //SSC Interrupt Mask Register

//TC
typedef union
{
    unsigned int   tc_bcr;
    struct
    {
        unsigned int   SYNC : 1;           //SYNC Synchro Command
        unsigned int   Reserved0 : 31;
    };
}TC_BCR,*pTC_BCR;                           //TC Block Control Register

typedef union
{
    unsigned int   tc_bmr;
    struct
    {
        unsigned int   TC0XC0S : 2;        //External Clock Signal 0 Selection
        unsigned int   TC1XC1S : 2;        //External Clock Signal 1 Selection
        unsigned int   TC2XC2S : 2;        //External Clock Signal 2 Selection
        unsigned int   Reserved0 : 26;
    };
}TC_BMR,*pTC_BMR;                           //TC Block Mode Register

//TC(0~3)
typedef union
{
    unsigned int   tc_ccr;
    struct
    {
        unsigned int   CLKEN : 1;          //Counter Clock Enable Command
        unsigned int   CLKDIS : 1;         //Counter Clock Disable Command
        unsigned int   SWTRG : 1;          //Software Trigger Command
        unsigned int   Reserved0 : 29;
    };
}TC_CCR,*pTC_CCR;                           //TC Channel Control Register

typedef union
{
    unsigned int   tc_cmrc;
    struct
    {
        unsigned int   TCCLKS : 3;         //Clcok Selection
        unsigned int   CLKI : 1;           //Clock Invert
        unsigned int   BURST : 2;          //Burst Signal Selection
        unsigned int   LDBSTOP : 1;        //Counter Clock Stopped With RB Loading
        unsigned int   LDBDIS : 1;         //Counter Clock Disable With RB Loading
        unsigned int   ETRGEDG : 2;        //External Trigger Edge Selection
        unsigned int   ABETRG : 1;         //TIOA or TIOB External Trigger Selection
        unsigned int   Reserved0 : 3;
        unsigned int   CPCTRG : 1;         //RC Compare Trigger Enable
        unsigned int   WAVE : 1;           //=0
        unsigned int   LDRA : 2;           //RA Loading Selection
        unsigned int   LDRB : 2;           //RB Loading Selection
        unsigned int   Reserved1 : 12;
    };
}TC_CMRC,*pTC_CMRC;                         //TC Channel Mode Register:Capture Mode

typedef union
{
    unsigned int   tc_cmrw;
    struct
    {
        unsigned int   TCCLKS : 3;         //Clock Selection
        unsigned int   CLKI : 1;           //Clock Invert
        unsigned int   BURST : 2;          //Burst Signal Selection
        unsigned int   CPCSTOP : 1;        //Counter Clock Stopped With RC Compare
        unsigned int   CPCDIS : 1;         //Counter Clock Disable With RC Compare
        unsigned int   EEVTEDG : 2;        //External Event Edge Selection
        unsigned int   EEVT : 2;           //External Eeent Selection
        unsigned int   ENETRG : 1;         //External Event Trigger Enable
        unsigned int   WAVSEL : 2;         //Waveform Selection
        unsigned int   WAVE : 1;           //=1
        unsigned int   ACPA : 2;           //RA Compare Effect on TIOA
        unsigned int   ACPC : 2;           //RC Compare Effect on TIOA
        unsigned int   AEEVT : 2;          //External Event Effect on TIOA
        unsigned int   ASWTRG : 2;         //Software Trigger Effect on TIOA
        unsigned int   BCPB : 2;           //RB Compare Effect on TIOB
        unsigned int   BCPC : 2;           //RC Compare Effect on TIOB
        unsigned int   BEEVT : 2;          //External Event Effect on TIOB
        unsigned int   BSWTRG : 2;         //Software Trigger Effect on TIOB
    };
}TC_CMRW,*pTC_CMRW;                         //TC Channel Mode Register:Waveform Mode

typedef union
{
    unsigned int   tc_cv;
    struct
    {
        unsigned int   CV : 16;            //Counter Value
        unsigned int   Reserved0 : 16;
    };
}TC_CV,*pTC_CV;                             //TC Counter Value Register

typedef union
{
    unsigned int   tc_ra;
    struct
    {                                       //Register A
        unsigned int   RA : 16;
        unsigned int   Reserved0 : 16;
    };
}TC_RA,*pTC_RA;                             //TC Register A

typedef union
{
    unsigned int   tc_rb;
    struct
    {
        unsigned int   RB : 16;            //Register B
        unsigned int   Reserved0 : 16;
    };
}TC_RB,*pTC_RB;                             //TC Register B

typedef union
{
    unsigned int   tc_rc;
    struct
    {
        unsigned int   RC : 16;            //Register C
        unsigned int   Reserved0 : 16; 
    };
}TC_RC,*pTC_RC;                             //TC Register C

typedef union
{
    unsigned int   tc_sr;
    struct
    {
        unsigned int   COVFS : 1;          //Counter Overflow Status
        unsigned int   LOVRS : 1;          //Load Overrun Status
        unsigned int   CPAS : 1;           //RA Compare Status
        unsigned int   CPBS : 1;           //RB Compare Status
        unsigned int   CPCS : 1;           //RC Compate Staus
        unsigned int   LDRAS : 1;          //RA Loading Status
        unsigned int   LDRBS : 1;          //RB Loading Status
        unsigned int   ETRGS : 1;          //External Trigger Status
        unsigned int   Reserved0 : 8;
        unsigned int   CLKSTA : 1;         //Clock Enabling Status
        unsigned int   MTIOA : 1;          //TIOA Mirror
        unsigned int   MTIOB : 1;          //TIOB Mirror
        unsigned int   Reserved1 : 13;
    };
}TC_SR,*pTC_SR;                             //TC Status Register

typedef union
{
    unsigned int   tc_ier;
    struct
    {
        unsigned int   COVFS : 1;          //Counter Overflow Interrupt Enable
        unsigned int   LOVRS : 1;          //Load Overrun Interrupt Enable
        unsigned int   CPAS : 1;           //RA Compare Interrupt Enable
        unsigned int   CPBS : 1;           //RB Compare Interrupt Enable
        unsigned int   CPCS : 1;           //RC Compare Interrupt Enable
        unsigned int   LDRAS : 1;          //RA Loading Interrupt Enable
        unsigned int   LDRBS : 1;          //RB Loading Interrupt Enable
        unsigned int   ETRGS : 1;          //External Trigger Interrupt Enable
        unsigned int   Reserved0 : 24;
    };
}TC_IER,*pTC_IER;                           //TC Interrupt Enable Register

typedef union
{
    unsigned int   tc_idr;
    struct
    {
        unsigned int   COVFS : 1;          //Counter Overflow Interrupt Disable
        unsigned int   LOVRS : 1;          //Load Overrun Interrupt Disable
        unsigned int   CPAS : 1;           //RA Compare Interrupt Disable
        unsigned int   CPBS : 1;           //RB Compare Interrupt Disable
        unsigned int   CPCS : 1;           //RC Compare Interrupt Disable
        unsigned int   LDRAS : 1;          //RA Loading Interrupt Disable
        unsigned int   LDRBS : 1;          //RB Loading Interrupt Disable
        unsigned int   ETRGS : 1;          //External Trigger Interrupt Disable
        unsigned int   Reserved0 : 24;
    };
}TC_IDR,*pTC_IDR;                           //TC Interrupt Disable Register

typedef union
{
    unsigned int   tc_imr;
    struct
    {
        unsigned int   COVFS : 1;          //Counter Overflow Interrupt Mask
        unsigned int   LOVRS : 1;          //Load Overrun Interrupt Mask
        unsigned int   CPAS : 1;           //RA Compare Interrupt Mask
        unsigned int   CPBS : 1;           //RB Compare Interrupt Mask
        unsigned int   CPCS : 1;           //RC Compare Interrupt Mask
        unsigned int   LDRAS : 1;          //RA Loading Interrupt Mask
        unsigned int   LDRBS : 1;          //RB Loading Interrupt Mask
        unsigned int   ETRGS : 1;          //External Trigger Interrupt Mask
        unsigned int   Reserved0 : 24;
    };
}TC_IMR,*pTC_IMR;                           //TC Interrupt Mask Register

//PWM 
typedef union
{
    unsigned int   pwm_mr;
    struct
    {
        unsigned int   DIVA : 8;
        unsigned int   PREA : 4;
        unsigned int   Reserved0 : 4;
        unsigned int   DIVB : 8;
        unsigned int   PREB : 4;
        unsigned int   Reserved1 : 4;
    };    
}PWM_MR,*pPWM_MR;                           //PWM Mode Register

typedef union
{
    unsigned int   pwm_ena;
    struct
    {
        unsigned int   CHID0 : 1;
        unsigned int   CHID1 : 1;
        unsigned int   CHID2 : 1;
        unsigned int   CHID3 : 1;          //Channel ID Enable
        unsigned int   Reserved0 : 28;
    };
}PWM_ENA,*pPWM_ENA;                         //PWM Enable Register

typedef union
{
    unsigned int   pwm_dis;
    struct
    {
        unsigned int   CHID0 : 1;
        unsigned int   CHID1 : 1;
        unsigned int   CHID2 : 1;
        unsigned int   CHID3 : 1;          //Channel ID Disable
        unsigned int   Reserved0 : 28;
    };
}PWM_DIS,*pPWM_DIS;                         //PWM Disable Register

typedef union
{
    unsigned int   pwm_sr;
    struct
    {
        unsigned int   CHID0 : 1;
        unsigned int   CHID1 : 1;
        unsigned int   CHID2 : 1;
        unsigned int   CHID3 : 1;          //Channel ID Status
        unsigned int   Reserved0 : 28;
    };
}PWM_SR,*pPWM_SR;                         //PWM Status Register

typedef union
{
    unsigned int   pwm_ier;
    struct
    {
        unsigned int   CHID0 : 1;
        unsigned int   CHID1 : 1;
        unsigned int   CHID2 : 1;
        unsigned int   CHID3 : 1;          //Channel ID Interrupt Enable
        unsigned int   Reserved0 : 28;
    };
}PWM_IER,*pPWM_IER;                         //PWM Interrupt Enable Register

typedef union
{
    unsigned int   pwm_idr;
    struct
    {
        unsigned int   CHID0 : 1;
        unsigned int   CHID1 : 1;
        unsigned int   CHID2 : 1;
        unsigned int   CHID3 : 1;          //Channel ID Interrupt Disable
        unsigned int   Reserved0 : 28;
    };
}PWM_IDR,*pPWM_IDR;                         //PWM Interrupt Disable Register

typedef union
{
    unsigned int   pwm_imr;
    struct
    {
        unsigned int   CHID0 : 1;
        unsigned int   CHID1 : 1;
        unsigned int   CHID2 : 1;
        unsigned int   CHID3 : 1;          //Channel ID Interrupt Mask
        unsigned int   Reserved0 : 28;
    };
}PWM_IMR,*pPWM_IMR;                         //PWM Interrupt Mask Register

typedef union
{
    unsigned int   pwm_isr;
    struct
    {
        unsigned int   CHID0 : 1;
        unsigned int   CHID1 : 1;
        unsigned int   CHID2 : 1;
        unsigned int   CHID3 : 1;          //Channel ID Interrupt Status
        unsigned int   Reserved0 : 28;
    };
}PWM_ISR,*pPWM_ISR;                         //PWM Interrupt Status Register

typedef union
{
    unsigned int   pwm_cmr;
    struct
    {
        unsigned int   CPRE : 4;           //Channel Pre-scaler
        unsigned int   Reserved0 : 4;
        unsigned int   CALG : 1;           //Channel Alignment
        unsigned int   CPOL : 1;           //Channel Polarity
        unsigned int   CPD : 1;            //Channel Update Period
        unsigned int   Reserved1 : 21;
    };
}PWM_CMR,*pPWM_CMR;                         //PWM Channel Mode Register(0~3)

typedef union
{
    unsigned int   pwm_cdty;
    struct
    {
        unsigned int   CDTY : 32;          //Channel Duty Cycle
    };
}PWM_CDTY,*pPWM_CDTY;                       //PWM Channel Duty Cycle Register(0~3)

typedef union
{
    unsigned int   pwm_cprd;
    struct
    {
        unsigned int   CPRD : 32;          //Channel Period
    };
}PWM_CPRD,*pPWM_CPRD;                       //PWM Changnel Period Register(0~3)

typedef union
{
    unsigned int   pwm_ccnt;
    struct
    {
        unsigned int   CNT : 32;           //Channel Counter Register
    };
}PWM_CCNT,*pPWM_CCNT;                       //PWM Channel Counter Register(0~3)

typedef union
{
    unsigned int   pwm_cupd;
    struct
    {
        unsigned int   CUPD : 32;          
    };
}PWM_CUPD,*pPWM_CUPD;                       //PWM Channel Counter Register(0~3)

//UDP
typedef union
{
    unsigned int   udp_frm_num;
    struct
    {
        unsigned int   FRM_NUM : 11;       //Frame Number as Defined in the Packet Field Formats
        unsigned int   Reserved0 : 5;
        unsigned int   FRM_ERR : 1;        //Frame Error
        unsigned int   FRM_OK : 1;         //Frame OK
        unsigned int   Reserved1 : 14;
    };
}UDP_FRM_NUM,*pUDP_FRM_NUM;                 //UDP Frame Number Register

typedef union
{
    unsigned int   udp_glb_stat;
    struct
    {
        unsigned int   FADDEN : 1;         //Function Address Enable
        unsigned int   CONFG : 1;          //Configured
        unsigned int   ESR : 1;            //Enable Send Resume
        unsigned int   Reserved0 : 1;
        unsigned int   RMWUPE : 1;         //Remote Wake Up Enable
        unsigned int   Reserved1 : 27;
    };
}UDP_GLB_STAT,*pUDP_GLB_STAT;               //UDP Global State Register

typedef union
{
    unsigned int   udp_faddr;
    struct
    {
        unsigned int   FADD : 7;           //Function Address Value
        unsigned int   Reserved0 : 1;
        unsigned int   FEN : 1;            //Function Enable
        unsigned int   Reserved1 : 23;
    };
}UDP_FADDR,*pUDP_FADDR;                     //UDP Function Address Register

typedef union
{
    unsigned int   udp_ier;
    struct
    {
        unsigned int   EP0INT : 1;         //Enable Endpoint 0 Interrupt
        unsigned int   EP1INT : 1;         //Enable Endpoint 1 Interrupt
        unsigned int   EP2INT : 1;         //Enable Endpoint 2 Interrupt
        unsigned int   EP3INT : 1;         //Enable Endpoint 3 Interrupt
        unsigned int   EP4INT : 1;         //Enable Endpoint 4 Interrupt
        unsigned int   EP5INT : 1;         //Enable Endpoint 5 Interrupt
        unsigned int   EP6INT : 1;         //Enable Endpoint 6 Interrupt
        unsigned int   EP7INT : 1;         //Enable Endpoint 7 Interrupt
        unsigned int   RXSUSP : 1;         //Enable UDP Suspend Interrupt
        unsigned int   RXRSM : 1;          //Enable UDP Resume Interrupt
        unsigned int   Reserved0 : 1;
        unsigned int   SOFINT : 1;         //Enable Start Of Frame Interrupt
        unsigned int   Reserved1 : 1;
        unsigned int   WAKEUP : 1;         //Enable UDP bus Wakeup Interrupt
        unsigned int   Reserved2 : 18;
    };
}UDP_IER,*pUDP_IER;                         //UDP Interrupt Enable Register

typedef union
{
    unsigned int   udp_idr;
    struct
    {
        unsigned int   EP0INT : 1;         //Disable Endpoint 0 Interrupt
        unsigned int   EP1INT : 1;         //Disable Endpoint 1 Interrupt
        unsigned int   EP2INT : 1;         //Disable Endpoint 2 Interrupt
        unsigned int   EP3INT : 1;         //Disable Endpoint 3 Interrupt
        unsigned int   EP4INT : 1;         //Disable Endpoint 4 Interrupt
        unsigned int   EP5INT : 1;         //Disable Endpoint 5 Interrupt
        unsigned int   EP6INT : 1;         //Disable Endpoint 6 Interrupt
        unsigned int   EP7INT : 1;         //Disable Endpoint 7 Interrupt
        unsigned int   RXSUSP : 1;         //Disable UDP Suspend Interrupt
        unsigned int   RXRSM : 1;          //Disable UDP Resume Interrupt
        unsigned int   Reserved0 : 1;
        unsigned int   SOFINT : 1;         //Disable Start Of Frame Interrupt
        unsigned int   Reserved1 : 1;
        unsigned int   WAKEUP : 1;         //Disable UDP bus Wakeup Interrupt
        unsigned int   Reserved2 : 18;
    };
}UDP_IDR,*pUDP_IDR;                         //UDP Interrupt Disable Register

typedef union
{
    unsigned int   udp_imr;
    struct
    {
        unsigned int   EP0INT : 1;         //Endpoint 0 Interrupt Status
        unsigned int   EP1INT : 1;         //Endpoint 1 Interrupt Status
        unsigned int   EP2INT : 1;         //Endpoint 2 Interrupt Status
        unsigned int   EP3INT : 1;         //Endpoint 3 Interrupt Status
        unsigned int   EP4INT : 1;         //Endpoint 4 Interrupt Status
        unsigned int   EP5INT : 1;         //Endpoint 5 Interrupt Status
        unsigned int   EP6INT : 1;         //Endpoint 6 Interrupt Status
        unsigned int   EP7INT : 1;         //Endpoint 7 Interrupt Status
        unsigned int   RXSUSP : 1;         //UDP Suspend Interrupt Status
        unsigned int   RXRSM : 1;          //UDP Resume Interrupt Status
        unsigned int   ENDBUSRES : 1;      //End of BUS Reset Interrupt Status
        unsigned int   SOFINT : 1;         //Start Of Frame Interrupt Status
        unsigned int   Reserved0 : 1;
        unsigned int   WAKEUP : 1;         //UDP bus Wakeup Interrupt Status
        unsigned int   Reserved1 : 18;
    };
}UDP_IMR,*pUDP_IMR;                         //UDP Interrupt Mask Register

typedef union
{
    unsigned int   udp_isr;
    struct
    {
        unsigned int   EP0INT : 1;         //Mask Endpoint 0 Interrupt
        unsigned int   EP1INT : 1;         //Mask Endpoint 1 Interrupt
        unsigned int   EP2INT : 1;         //Mask Endpoint 2 Interrupt
        unsigned int   EP3INT : 1;         //Mask Endpoint 3 Interrupt
        unsigned int   EP4INT : 1;         //Mask Endpoint 4 Interrupt
        unsigned int   EP5INT : 1;         //Mask Endpoint 5 Interrupt
        unsigned int   EP6INT : 1;         //Mask Endpoint 6 Interrupt
        unsigned int   EP7INT : 1;         //Mask Endpoint 7 Interrupt
        unsigned int   RXSUSP : 1;         //Mask UDP Suspend Interrupt
        unsigned int   RXRSM : 1;          //Mask UDP Resume Interrupt
        unsigned int   Reserved0 : 1;
        unsigned int   SOFINT : 1;         //Mask Start Of Frame Interrupt
        unsigned int   Reserved1 : 1;
        unsigned int   WAKEUP : 1;         //Mask UDP bus Wakeup Interrupt
        unsigned int   Reserved2 : 18;
    };
}UDP_ISR,*pUDP_ISR;                         //UDP Interrupt Status Register

typedef union
{
    unsigned int   udp_icr;
    struct
    {
        unsigned int   Reserved0 : 8;
        unsigned int   RXSUSP : 1;         //Clear UDP Suspend Interrupt
        unsigned int   RXRSM : 1;          //Clear UDP Resume Interrupt
        unsigned int   Reserved1 : 1;
        unsigned int   SOFINT : 1;         //Clear Start Of Frame Interrupt
        unsigned int   ENDBUSRES : 1;      //Clear End of Buf Reset Interrupt
        unsigned int   WAKEUP : 1;         //Clear Wakeup Interrupt
        unsigned int   Reserved2 : 18;
    }; 
}UDP_ICR,*pUDP_ICR;                         //UDP Interrupt Clear Register

typedef union
{
    unsigned int   udp_rst_ep;
    struct
    {
        unsigned int   EP0 : 1;
        unsigned int   EP1 : 1;
        unsigned int   EP2 : 1;
        unsigned int   EP3 : 1;
        unsigned int   EP4 : 1;
        unsigned int   EP5 : 1;
        unsigned int   EP6 : 1;
        unsigned int   EP7 : 1;            //Reset Endpoint x
        unsigned int   Reserved0 : 24;
    };
}UDP_RST_EP,*pUDP_RST_EP;                   //UDP Reset Endpoint Register

typedef union
{
    unsigned int   udp_csr;
    struct
    {
        unsigned int   TXCOMP : 1;         //Generates an IN Packet with Data Previously Written in the DPR
        unsigned int   RX_DATA_BK0 : 1;    //Receive Data Bank 0
        unsigned int   RXSETUP : 1;        //Received Steup
        unsigned int   STALLSENT_ISOERROR : 1;//(Control,Buld Interrupt Endpoint)/(Isochronous Endpoints)
        unsigned int   TXPKTRDY : 1;       //Transmit Packet Ready
        unsigned int   PROCE_STALL : 1;    //Force Stall
        unsigned int   RX_DATA_BK1 : 1;    //Receive Data Bank 1
        unsigned int   DIR : 1;            //Transfer Direction
        unsigned int   EPTYPE : 3;         //Endpoint Type
        unsigned int   DTGLE : 1;          //Data Toggle
        unsigned int   Reserved0 :3;
        unsigned int   EPEDS : 1;          //Endpoint Enable Disable
        unsigned int   RXBYTECNT : 11;     //Number of Bytes Available in the FIFO
        unsigned int   Reserved1 : 5;
    };
}UDP_CSR,*pUDP_CSR;                         //UDP Endpoint Control and Status Register(0~7)

typedef union
{
    unsigned int   udp_fdr;
    struct
    {
        unsigned int   FIFO_DATA : 8;      //FIFO Data Value
        unsigned int   Reserved0 : 24;
    };
}UDP_FDR,*pUDP_FDR;                         //UDP FIFO Data Register(0~7)

typedef union
{
    unsigned int   udp_txvc;
    struct
    {
        unsigned int   Reserved0 : 8;
        unsigned int   TXVDIS : 1;         //Transceiver Disable
        unsigned int   PUON : 1;           //Pullup On
        unsigned int   Reserved1 : 22;
    };
}UDP_TXVC,*pUDP_TXVC;                       //UDP Transceiver Control Register

//ADC
typedef union
{
    unsigned int   adc_cr;
    struct
    {
        unsigned int   SWRST : 1;          //Sofrware Reset
        unsigned int   START : 1;          //Start Conversion
        unsigned int   Reserved0 : 30;
    };
}ADC_CR,*pADC_CR;                           //ADC Control Register

typedef union
{
    unsigned int   TRGEN : 1;              //Trigger Enable
    unsigned int   TRGSEL : 3;             //Trigger Selection
    unsigned int   LOWRES : 1;             //Resolution
    unsigned int   SLEEP : 1;              //Sleep Mode
    unsigned int   Reserved0 : 2;
    unsigned int   PRESCAL : 6;            //Prescaler Rate Selection
    unsigned int   Reserved1 : 2;
    unsigned int   STARTUP : 5;            //Start Up Time
    unsigned int   Reserved2 : 3;
    unsigned int   SHTIM : 4;              //Sample & Hold Time
    unsigned int   Reserved3 : 4;
}ADC_MR,*pADC_MR;                           //ADC Mode Register

typedef union
{
    unsigned int   adc_cher;
    struct
    {
        unsigned int   CH0 : 1;
        unsigned int   CH1 : 1;
        unsigned int   CH2 : 1;
        unsigned int   CH3 : 1;
        unsigned int   CH4 : 1;
        unsigned int   CH5 : 1;
        unsigned int   CH6 : 1;
        unsigned int   CH7 : 1;            //Channel x Enable
        unsigned int   Reserved0 : 24;
    };
}ADC_CHER,*pADC_CHER;                       //ADC Channel Enable Register

typedef union
{
    unsigned int   adc_chdr;
    struct
    {
        unsigned int   CH0 : 1;
        unsigned int   CH1 : 1;
        unsigned int   CH2 : 1;
        unsigned int   CH3 : 1;
        unsigned int   CH4 : 1;
        unsigned int   CH5 : 1;
        unsigned int   CH6 : 1;
        unsigned int   CH7 : 1;            //Channel x Disable
        unsigned int   Reserved0 :24;
    };
}ADC_CHDR,*pADC_CHDR;                       //ADC Channel Disable Register

typedef union
{
    unsigned int   adc_chsr;
    struct
    {
        unsigned int   CH0 : 1;
        unsigned int   CH1 : 1;
        unsigned int   CH2 : 1;
        unsigned int   CH3 : 1;
        unsigned int   CH4 : 1;
        unsigned int   CH5 : 1;
        unsigned int   CH6 : 1;
        unsigned int   CH7 : 1;            //Channel x Status
        unsigned int   Reserved0 : 24;
    };
}ADC_CHSR,*pADC_CHSR;                       //ADC Channel Status Register

typedef union
{
    unsigned int   adc_sr;
    struct
    {
        unsigned int   EOC0 : 1;
        unsigned int   EOC1 : 1;
        unsigned int   EOC2 : 1;
        unsigned int   EOC3 : 1;
        unsigned int   EOC4 : 1;
        unsigned int   EOC5 : 1;
        unsigned int   EOC6 : 1;
        unsigned int   EOC7 : 1;           //End of Conversion x
        unsigned int   OVRE0 : 1;
        unsigned int   OVRE1 : 1;
        unsigned int   OVRE2 : 1;
        unsigned int   OVRE3 : 1;
        unsigned int   OVRE4 : 1;
        unsigned int   OVRE5 : 1;
        unsigned int   OVRE6 : 1;
        unsigned int   OVRE7 : 1;          //Overrun Error x
        unsigned int   DRDY : 1;           //Data Ready
        unsigned int   GOVRE : 1;          //General Overrun Error
        unsigned int   ENDRX : 1;          //End of Rx Buffer
        unsigned int   RXBUFF : 1;         //RX Buffer Full
        unsigned int   Reserved0 : 12;
    };
}ADC_SR,*pADC_SR;                           //ADC Status Register

typedef union
{
    unsigned int   adc_lcdr;
    struct
    {
        unsigned int   LDATA : 10;         //Last Data Converted
        unsigned int   Reserved0 : 22;
    };
}ADC_LCDR,*pADC_LCDR;                       //ADC Last Converted Data Register

typedef union
{
    unsigned int   adc_ier;
    struct
    {
        unsigned int   EOC0 : 1;
        unsigned int   EOC1 : 1;
        unsigned int   EOC2 : 1;
        unsigned int   EOC3 : 1;
        unsigned int   EOC4 : 1;
        unsigned int   EOC5 : 1;
        unsigned int   EOC6 : 1;
        unsigned int   EOC7 : 1;           //End of Conversion Interrupt Enable x
        unsigned int   OVRE0 : 1;
        unsigned int   OVRE1 : 1;
        unsigned int   OVRE2 : 1;
        unsigned int   OVRE3 : 1;
        unsigned int   OVRE4 : 1;
        unsigned int   OVRE5 : 1;
        unsigned int   OVRE6 : 1;
        unsigned int   OVRE7 : 1;          //Overrun Error Interrupt Enable x
        unsigned int   DRDY : 1;           //Data Ready Interrupt Enable
        unsigned int   GOVRE : 1;          //General Overrun Error Interrupt Enable
        unsigned int   ENDRX : 1;          //End of Rx Buffer Interrupt Enable
        unsigned int   RXBUFF : 1;         //RX Buffer Full Interrupt Enable
        unsigned int   Reserved0 : 12;
    };
}ADC_IER,*pADC_IER;                           //ADC Interrupt Enable Register

typedef union
{
    unsigned int   adc_idr;
    struct
    {
        unsigned int   EOC0 : 1;
        unsigned int   EOC1 : 1;
        unsigned int   EOC2 : 1;
        unsigned int   EOC3 : 1;
        unsigned int   EOC4 : 1;
        unsigned int   EOC5 : 1;
        unsigned int   EOC6 : 1;
        unsigned int   EOC7 : 1;           //End of Conversion Interrupt Disable x
        unsigned int   OVRE0 : 1;
        unsigned int   OVRE1 : 1;
        unsigned int   OVRE2 : 1;
        unsigned int   OVRE3 : 1;
        unsigned int   OVRE4 : 1;
        unsigned int   OVRE5 : 1;
        unsigned int   OVRE6 : 1;
        unsigned int   OVRE7 : 1;          //Overrun Error Interrupt Disable x
        unsigned int   DRDY : 1;           //Data Ready Interrupt Disable
        unsigned int   GOVRE : 1;          //General Overrun Error Interrupt Disable
        unsigned int   ENDRX : 1;          //End of Rx Buffer Interrupt Disable
        unsigned int   RXBUFF : 1;         //RX Buffer Full Interrupt Disable
        unsigned int   Reserved0 : 12;
    };
}ADC_IDR,*pADC_IDR;                           //ADC Interrupt Disable Register

typedef union
{
    unsigned int   adc_imr;
    struct
    {
        unsigned int   EOC0 : 1;
        unsigned int   EOC1 : 1;
        unsigned int   EOC2 : 1;
        unsigned int   EOC3 : 1;
        unsigned int   EOC4 : 1;
        unsigned int   EOC5 : 1;
        unsigned int   EOC6 : 1;
        unsigned int   EOC7 : 1;           //End of Conversion Interrupt Mask x
        unsigned int   OVRE0 : 1;
        unsigned int   OVRE1 : 1;
        unsigned int   OVRE2 : 1;
        unsigned int   OVRE3 : 1;
        unsigned int   OVRE4 : 1;
        unsigned int   OVRE5 : 1;
        unsigned int   OVRE6 : 1;
        unsigned int   OVRE7 : 1;          //Overrun Error Interrupt Mask x
        unsigned int   DRDY : 1;           //Data Ready Interrupt Mask
        unsigned int   GOVRE : 1;          //General Overrun Error Interrupt Mask
        unsigned int   ENDRX : 1;          //End of Rx Buffer Interrupt Mask
        unsigned int   RXBUFF : 1;         //RX Buffer Full Interrupt Mask
        unsigned int   Reserved0 : 12;
    };
}ADC_IMR,*pADC_IMR;                         //ADC Interrupt Mask Register

typedef union
{
    unsigned int   adc_cdr;
    struct
    {
        unsigned int   DATA : 10;          //Converted Data
        unsigned int   Reserved0 : 22;
    };
}ADC_CDR,*pADC_CDR;                         //ADC Channel Data Register(0~7)  

//#define TC_IER AT91C_TC0_IER
#endif