
#ifndef _PSAMCOMMAND_H
#define _PSAMCOMMAND_H





extern unsigned short int PSAM_Select_PSE( unsigned char *pBuffer );
extern unsigned short int PSAM_Select_Sinopec1( unsigned char *pBuffer );
extern unsigned short int PSAM_Select_Sinopec2( unsigned char *pBuffer );
extern unsigned short int PSAM_Init_Grey_Lock( unsigned char *pBuffer );
extern unsigned short int PSAM_Certificate_Grey_Lock( unsigned char *pBuffer );
extern unsigned short int PSAM_Credit_Grey_Debit( unsigned char *pBuffer );
extern unsigned short int PSAM_Get_GMAC( unsigned char *pBuffer );
extern unsigned short int PSAM_Credit_MAC2( unsigned char *pBuffer );
extern unsigned short int PSAM_Init_Purchase( unsigned char *pBuffer );
extern unsigned short int PSAM_Init_DES_Encrypt( unsigned char *pBuffer );
extern unsigned short int PSAM_DES_Encrypt0( unsigned char *pBuffer );
extern unsigned short int PSAM_DES_Encrypt1( unsigned char *pBuffer );
extern unsigned short int PSAM_DES_Encrypt2( unsigned char *pBuffer );
extern unsigned short int PSAM_Get_Response( unsigned char PSAM_Location, unsigned char DataLen );

#ifdef MAKE_PSAM_VERIFY
extern unsigned short int PSAM_Get_Anti_Fraud_Proof( unsigned char *pBuffer );
extern unsigned short int PSAM_Get_Random( unsigned char *pBuffer );
extern unsigned short int PSAM_Start_Binding( unsigned char *pBuffer );
extern unsigned short int PSAM_Init_Binding( unsigned char *pBuffer );
extern unsigned short int PSAM_Binding( unsigned char *pBuffer );
extern unsigned short int PSAM_Init_DES_Encrypt_RID_Log( unsigned char *pBuffer );
extern unsigned short int PSAM_Anti_Fraud_Verify( unsigned char *pBuffer );


#endif



extern unsigned short int CR_PSAM_ColdReset( unsigned char PSAM_Voltage, unsigned char PSAM_Location, unsigned char PSAM_Mode );

extern unsigned short int CR_CAPDU_Command_T0_PSAM( unsigned short int ( *PSAM_COMMAND )( unsigned char * ), unsigned char PSAM_Location );

extern unsigned short int CR_PSAM1_ColdReset_7816_50V( void );
extern unsigned short int CR_PSAM_Select_PSE( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Select_Sinopec1( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Select_Sinopec2( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Init_Grey_Lock( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Certificate_Grey_Lock( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Credit_Grey_Debit( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Get_GMAC( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Credit_MAC2( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Init_Purchase( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Init_DES_Encrypt( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_DES_Encrypt0( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_DES_Encrypt1( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_DES_Encrypt2( unsigned char PSAM_Location );

#ifdef MAKE_PSAM_VERIFY
extern unsigned short int CR_PSAM_Get_Anti_Fraud_Proof( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Get_Random( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Start_Binding( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Init_Binding( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Binding( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Init_DES_RID_Log( unsigned char PSAM_Location );
extern unsigned short int CR_PSAM_Anti_Fraud_Verify( unsigned char PSAM_Location );


#endif







#endif
