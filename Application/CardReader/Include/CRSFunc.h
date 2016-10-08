

#ifndef _CRSFUNC_H
#define _CRSFUNC_H


extern void ICCard_LogOn_Proc( void );

extern void ICCard_LogOff_Proc( void );

extern void ICCard_Offl_Trans_Proc( void );

extern void ICCard_Price_Recv_Proc( void );



extern unsigned short int GetPumpTotals( void );
extern unsigned short int GetEmpPWD( void );

extern void GetSFuncData( unsigned char type );
extern unsigned char GetOfflineData( void );
extern void SaveTotals( void );
extern void SaveLogOnTotals( void );
extern void SaveDispTotal( unsigned char PhNozzle );
extern void WriteTotalLog( unsigned char type );
extern void WriteOfflineLog( void );
extern void ReadTotals( void );



#endif
