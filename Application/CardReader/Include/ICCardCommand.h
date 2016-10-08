
#ifndef _ICCARDCOMMAND_H
#define _ICCARDCOMMAND_H


// Save The Record List Number,
// Special Use In Read IC Card Record
extern unsigned char RecordListNumber;


extern unsigned short int CR_IC_ColdReset_7816( void );

extern unsigned short int ICCard_Select_Sinopec_APP( unsigned char *pBuffer );
extern unsigned short int ICCard_Select_Loyalty_APP( unsigned char *pBuffer );
extern unsigned short int ICCard_Read_SFI_0x15( unsigned char *pBuffer );
extern unsigned short int ICCard_Read_SFI_0x16( unsigned char *pBuffer );
extern unsigned short int ICCard_Read_SFI_0x1B( unsigned char *pBuffer );
extern unsigned short int ICCard_Read_SFI_0x1C( unsigned char *pBuffer );
extern unsigned short int ICCard_Verify_Pin( unsigned char *pBuffer );
extern unsigned short int ICCard_Get_Lock_Proof( unsigned char *pBuffer );
extern unsigned short int ICCard_Clear_Lock_Proof( unsigned char *pBuffer );
extern unsigned short int ICCard_Get_Balance( unsigned char *pBuffer );
extern unsigned short int ICCard_Init_Grey_Lock( unsigned char *pBuffer );
extern unsigned short int ICCard_Set_Grey_Lock( unsigned char *pBuffer );
extern unsigned short int ICCard_Debit_Grey_Unlock( unsigned char *pBuffer );
extern unsigned short int ICCard_Debit_Purchase( unsigned char *pBuffer );
extern unsigned short int ICCard_Init_Purchase( unsigned char *pBuffer );
extern unsigned short int ICCard_Get_Response( unsigned char DataLen );
extern unsigned short int ICCard_Read_Record( unsigned char *pBuffer );
extern unsigned short int ICCard_Append_Log( unsigned char *pBuffer );


extern unsigned short int CR_CAPDU_Command_T0_ICCard( unsigned short int ( *ICCARD_COMMAND )( unsigned char * ));

extern unsigned short int CR_ICCARD_Select_Sinopec_APP( void );
extern unsigned short int CR_ICCARD_Select_Loyalty_APP( void );
extern unsigned short int CR_ICCARD_Read_SFI_0x15( void );
extern unsigned short int CR_ICCARD_Read_SFI_0x16( void );
extern unsigned short int CR_ICCARD_Read_SFI_0x1B( void );
extern unsigned short int CR_ICCARD_Read_SFI_0x1C( void );
extern unsigned short int CR_ICCARD_Verify_PIN( void );
extern unsigned short int CR_ICCARD_Get_Lock_Proof( void );
extern unsigned short int CR_ICCARD_Clear_Lock_Proof( void );
extern unsigned short int CR_ICCARD_Get_Balance( void );
extern unsigned short int CR_ICCARD_Init_Grey_Lock( void );
extern unsigned short int CR_ICCARD_Set_Grey_Lock( void );
extern unsigned short int CR_ICCARD_Debit_Grey_Unlock( void );
extern unsigned short int CR_ICCARD_Read_Record( void );
extern unsigned short int CR_ICCARD_Debit_Purchase( void );
extern unsigned short int CR_ICCARD_Init_Purchase( void );
extern unsigned short int CR_ICCard_Append_Log( void );

#ifdef MAKE_PSAM_VERIFY
extern unsigned short int ICCard_Read_SFI_0x1A( unsigned char *pBuffer );
extern unsigned short int CR_ICCARD_ACT_Read_SFI_0x1A( void );
extern unsigned short int CR_ICCARD_RID_Read_SFI_0x1A( void );
extern unsigned short int ICCard_ACT_Encrypted( unsigned char *pBuffer );
extern unsigned short int CR_ICCard_ACT_Encrypted( void );
extern unsigned short int ICCard_RID_Encrypted( unsigned char *pBuffer );
extern unsigned short int CR_ICCard_RID_Encrypted( void );
extern unsigned short int CR_ICCARD_RID_Read_SFI_0x15( void );
extern unsigned short int ICCard_RID_Get_Log_Para_Total( unsigned char *pBuffer );
extern unsigned short int CR_ICCard_RID_Get_Log_Para_Total( void );
extern unsigned short int ICCard_RID_Get_Log_Para_Added( unsigned char *pBuffer );
extern unsigned short int CR_ICCard_RID_Get_Log_Para_Added( void );
extern unsigned short int ICCard_RID_Append_Log( unsigned char *pBuffer );
extern unsigned short int CR_ICCard_RID_Append_Log( void );
#endif

extern unsigned short int CR_Poll_TypeA( void );
extern unsigned short int CR_Poll_TypeB( void );
extern unsigned short int CR_Poll_CardType( void );

//Douzhq 20140612 Add , Save Oil  
#if !defined(MAKE_PSAM_VERIFY) && !defined(MAKE_CASH_CR)

extern unsigned short int CR_ICCARD_Select_Saveoil_APP( void );
extern unsigned short int ICCard_Select_Saveoil_APP( unsigned char *pBuffer );

// 20151222
extern unsigned short int CR_ICCARD_Read_FID_0x15( void );

#endif

#endif
