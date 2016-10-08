#ifndef _VERIFY_H
#define _VERIFY_H

static const unsigned char ARMVerifyData[] = {0x67,0x71,0X73,0x67};     //CGIC
static const unsigned char PUMPVerifyData[] = {0x33,0x37,0x38,0x35};     //3.785
extern  unsigned long x8modfify(unsigned char s);
extern  void ComputeVerifyData(unsigned char	*content, unsigned char  len,unsigned char	*authdata);

#endif