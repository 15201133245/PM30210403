#ifndef _Arm70_publicfun_h
#define _Arm70_publicfun_h

extern	unsigned char PosSndBuf[];
extern	unsigned short PosCastLeng;
extern	unsigned char QueStatus;
extern	unsigned char DownLoadStatus;
extern	unsigned char PosReadRecordFlg;

/*  inline unsigned int GetBit_int(unsigned int isource,int bitaccount);
  
  unsigned int BYTE2DOWRD(unsigned char *bsource,int Bytelen);
  unsigned char BCD_to_ZBCD(unsigned char* Cdest,int Clen,unsigned char *Csource );
  void ZBCD_to_ASC(unsigned char *dest, unsigned char *source, unsigned char len);
  unsigned char * DEC_To_ZBC(unsigned short len);
  
  unsigned char HEX_to_ASC(unsigned char *dest, unsigned char *source, unsigned char len);

//void ZBCD_to_ASC(unsigned char *dest, unsigned char *source, unsigned char len);
//extern unsigned int BYTE2DOWRD(unsigned char *bsource,int Bytelen);
int DOT_x_CharConvert(char *csource ,int Dotnum);
int DataTypeConvertX_TO_Y(unsigned char *Bsource ,unsigned int Dsource,int Destype,char * cDest,int len);
unsigned int uchar_convert_Hexuint(unsigned char * csource,unsigned char* Cdest, int len);
*/
#endif