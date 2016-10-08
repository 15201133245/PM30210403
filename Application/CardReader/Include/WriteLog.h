#ifndef _WRITELOG_H
#define _WRITELOG_H


#define MaxLogLen			63
// Time In Log: MMDDHHMMSS
#define TimeLen				10




extern unsigned char WriteLog( enum MAX_PROCESS Process, unsigned char *pString );


#endif