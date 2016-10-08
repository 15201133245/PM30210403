#include "Verify.h"


// * x8modf - computing function for verify data 
//  	    x^8 s(x) mod f(x).
unsigned long x8modfify(unsigned char s)
{
    unsigned long A;

    A=0x00;
    if((s&0x80)!=0) A=A^0xa0000194;
    if((s&0x40)!=0) A=A^0x500000ca;
    if((s&0x20)!=0) A=A^0x28000065;
    if((s&0x10)!=0) A=A^0x98000033;
    if((s&0x08)!=0) A=A^0xc0000018;
    if((s&0x04)!=0) A=A^0x6000000c;
    if((s&0x02)!=0) A=A^0x30000006;
    if((s&0x01)!=0) A=A^0x18000003;
	
    return(A);
}


//计算四字节认证码 
//参数:
//[in] content 计算认证码的数据
//[in] len 数据长度
// [out] authdata 计算得到的认证码,长度为4字节
void ComputeVerifyData(unsigned char	*content, unsigned char  len,unsigned char	*authdata)
{
    unsigned char 	i;
    union ShareData
    {
    unsigned long	x;
            struct _TempData_
            {
                    unsigned char x0;
                    unsigned char x1;
                    unsigned char x2;
                    unsigned char x3;
            } x4x;
    } tempx;

    unsigned char	s;

    authdata[0]=0x51;
    authdata[1]=0xac;
    authdata[2]=0x68;
    authdata[3]=0x72;
    
    for (i=0; i<len; i++)
	  {
  		s = authdata[0]^content[i];
  		tempx.x = x8modfify(s);            
  		authdata[0]=authdata[1]^tempx.x4x.x0;
  		authdata[1]=authdata[2]^tempx.x4x.x1;
  		authdata[2]=authdata[3]^tempx.x4x.x2;
  		authdata[3]=tempx.x4x.x3;

          }
}



