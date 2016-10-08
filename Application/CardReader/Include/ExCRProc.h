
#ifndef _EXCRPROC_H
#define _EXCRPROC_H


extern void ICCard_Exist_External_Proc( void );
extern void ICCard_NonExist_External_Proc( void );
extern void ICCard_Busy_External_Proc( void );

extern unsigned char IssueOfflineTrans( void );



#ifdef MAKE_CARD_MODE2

extern void SaveHostAuth( void );
extern void SaveNonCardAuth_Mode2( void );


#endif





#endif
