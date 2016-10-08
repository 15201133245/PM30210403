#include "pump.h"
#include "global.h"


/******************************************************************************/
//function title: unsigned short PumpCrc16(unsigned char *s,unsigned short len,unsigned short LastCrc)
//function describe:crc
//param:none
//return:noe
//call function:none
//global variable: 
//author: steven
//date:2009-7-20
//mender:
//date:
//version:.01
/******************************************************************************/

unsigned short PumpCrc16(unsigned char *s,unsigned short len,unsigned short LastCrc)
{
    unsigned char i;
    unsigned char tmp;
    unsigned short crc;
    
    tmp=*s;
    crc=LastCrc;
    while ( len-- != 0 )
    {
        for ( i = 0x01; i != 0; i <<= 1 )
        {
            if ( (crc & 0x0001) != 0 )
            {
                crc >>= 1;
                crc ^= 0xa001;
            }
            else
            {
                crc >>= 1;
            }
            if ( (tmp & i) != 0 )
            {
                crc ^= 0xa001;
            }
        }
        s++;
        tmp=*s;
    }
    return (crc);
}
/******************************************************************************/
//function title: unsigned short AddFAPump(unsigned char *s,unsigned short len)
//function describe:
//param:none
//return:noe
//call function:none
//global variable: 
//author: steven
//date:2009-7-20
//mender:
//date:
//version:.01
/******************************************************************************/
unsigned short AddFAPump(unsigned char *s,unsigned short len)
{
    unsigned char *p;
    unsigned short i,j,k;
    
    p = malloc(len + 20);
    if ( p == NULL )
    {
        return 0;
    }
    k = len;
    for ( i = 6,j = 0; i < len; i++,j++ )
    {
        if ( s[i] == 0xfa )
        {
            k++;
            p[j++] = 0xfa;
            p[j] = 0xfa;
        }
        else
        {
            p[j] = s[i];
        }
    }
    len = k;
    memcpy(&s[6],p,j);
    free(p);
    return (len);
}
/******************************************************************************/
//function title: 
//function describe:
//param:none
//return:noe
//call function:none
//global variable: 
//author: steven
//date:2009-7-20
//mender:
//date:
//version:.01
/******************************************************************************/

/*
void recevicer(unsigned char *s,unsigned int len, unsigned char val)  //only for test comm
{
	if(*s == 0xfa)
	{
		WRITE(USART0_ID,time,0,strlen(time));
	}
}
*/

/******************************************************************************/
//function title: void PumpCMD_Recevice(unsigned char *s,unsigned int Rlen, unsigned char val)
//function describe:
//param:none
//return:noe
//call function:none
//global variable: 
//author: steven
//date:2009-7-20
//mender:
//date:
//version:.01
/******************************************************************************/


void PumpCMD_Recevice(unsigned char *s,unsigned int Rlen, unsigned char val)
{
    unsigned char tmp;	
  
    tmp=*s;
    
   
 
    {
        if ( (PumpLen == 0) && (tmp == 0xfa) )                   //check FA 
        {
              
            Pump_Receive_Data.bHead_Flg= tmp;
            PumpLen++;
        }
        else
        {
            if ( PumpLen == 1 )
            {
                if ( tmp != 0xfa )
                {
                    
                    if(tmp==CR_ID)  //CR id check
                    {
                                     
                          Pump_Receive_Data.bTarget_ID=tmp;
                          
                          PumpLen++;
                    }  
                    else if (tmp==Pump_ID)  //same ID check
                    {
                                
                          Pump_Receive_Data.bTarget_ID=tmp;
                          
                          PumpLen++;
                    }
                    else     //not the pump data
                    {
                          PumpLen = 0;
                          PumpDataLen = 0;
                    }
                    
                }
                else
                {
                    PumpLen = 0;
                   
                    Pump_Receive_Data.bHead_Flg= tmp;
                    PumpLen++;
                }
            }
            else
            {
                if ( PumpLen == 2 )
                {
                    if ( tmp != 0xfa )
                    {
                       
                        if(tmp==Pump_ID)  //pump address check
                         {
                                Pump_Receive_Data.bSource_ID=tmp;  //ID CHECK
                                PumpLen++;
                           
                         }
                         else if(tmp==CR_ID)  //same ID check
                         {
                            
                                Pump_Receive_Data.bSource_ID=tmp;  //ID CHECK
                                PumpLen++;
                         }
                         else  //ERR data
                         {
                                PumpLen = 0;
                                PumpDataLen = 0;
                         }
                       
                        
                    }
                    else
                    {
                        PumpLen = 0;
                        
                        Pump_Receive_Data.bHead_Flg= tmp;
                        PumpLen++;
                    }
                }
                //---------------
                else
                    {
                        if ( PumpLen == 3 )
                          {
                              if ( tmp != 0xfa )
                               {
                                  
                                      Pump_Receive_Data.bControl_Flg1= tmp;
                                      PumpLen++;
                                }
                                else
                                {
                                       PumpLen = 0;
                                       
                                       Pump_Receive_Data.bHead_Flg= tmp;
                                       PumpLen++;
                                  
                                 }
                          }
                          else
                          {
                             if ( PumpLen == 4 )  
                                {
                                   if ( tmp != 0xfa )
                                      {
                                       
                                              Pump_Receive_Data.bControl_Flg2= tmp;
                                              PumpLen++;
                                       }
                                       else
                                       {
                                              PumpLen = 0;
                                              
                                              Pump_Receive_Data.bHead_Flg= tmp;
                                              PumpLen++;
                                        }
                          }
                    
                    //---------------------
                    else
                    {
                        if ( PumpLen == 5)
                        {
                          if ( tmp != 0xfa )
                               {
                                  
                                    Pump_Receive_Data.bData_Len= tmp;                               
                                    PumpDataLen= tmp;
                                    PumpLen++;
                                    if(PumpDataLen>200)
                                    {
                                      //ERR DEAL
                                    }
                               }
                               else     //restart receive
                               {
                                    PumpLen = 0;
                                   
                                    Pump_Receive_Data.bHead_Flg= tmp;
                                    PumpLen++;
                                }
                            
                        }
                        else
                            {
                               if ( (PumpLen > 5) && (PumpLen < (PumpDataLen + 6)) && (PumpDataLen != 0) )
                                {
                                    if ( (tmp == 0xfa) && (PumpTmp == 0xfa) )
                                    {
                                          PumpTmp = 0;
                                         
                                          Pump_Receive_Data.bREV_Buf[PumpLen-6]=tmp;
                                          PumpLen++;
                                        
                                    }
                                    else
                                    {
                                        if ( (tmp == 0xfa) && (PumpTmp != 0xfa) )
                                        {
                                              PumpTmp = tmp;
                                        }
                                        else
                                        {
                                            if ( (tmp != 0xfa) && (PumpTmp == 0xfa) )
                                            {
                                                    PumpTmp = 0;
                                                    if ( tmp == CR_ID )  //  resatrt receive data from CR_ID
                                                    {
                                                        PumpLen = 2;
                                                        
                                                        Pump_Receive_Data.bHead_Flg= 0xfa;
                                                        Pump_Receive_Data.bTarget_ID=CR_ID;
                                                        PumpDataLen = 0;
                                                    }
                                                    else
                                                    {
                                                        PumpLen = 0;
                                                        PumpDataLen = 0;
                                                    }
                                            }
                                            else
                                            {
                                                    PumpTmp = 0;
                                                    
                                                    Pump_Receive_Data.bREV_Buf[PumpLen-6]=tmp;
                                                    PumpLen++;                                      
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    if ( (PumpLen == (PumpDataLen + 6)) && (PumpDataLen != 0) )
                                    {
                                        if ( (tmp == 0xfa) && (PumpTmp == 0xfa) )
                                        {
                                                PumpTmp = 0;
                                                
                                                Pump_Receive_Data.bREV_Buf[PumpLen-6]=tmp;
                                                PumpLen++;  
                                            
                                        }
                                        else
                                        {
                                            if ( (tmp == 0xfa) && (PumpTmp != 0xfa) )
                                            {
                                                PumpTmp = tmp;
                                            }
                                            else
                                            {
                                                if ( (tmp != 0xfa) && (PumpTmp == 0xfa) )
                                                {
                                                    PumpTmp = 0;
                                                    if ( tmp == CR_ID )
                                                    {
                                                        PumpLen = 2;
                                                      
                                                        Pump_Receive_Data.bHead_Flg= 0xfa;
                                                        Pump_Receive_Data.bTarget_ID=CR_ID;                                                      
                                                        PumpDataLen = 0;
                                                    }
                                                    else
                                                    {
                                                        PumpLen = 0;
                                                        PumpDataLen = 0;
                                                    }
                                                }
                                                else
                                                {
                                                    PumpTmp = tmp;
                                                    
                                                    Pump_Receive_Data.bREV_Buf[PumpLen-6]=tmp;
                                                    PumpLen++;
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if ( (PumpLen == (PumpDataLen + 7)) && (PumpDataLen != 0) )
                                        {
                                            if ( (tmp == 0xfa) && (PumpTmp == 0xfa) )
                                            {
                                                PumpTmp = 0;
                                                
                                                Pump_Receive_Data.bREV_Buf[PumpLen-6]=tmp;                                               
                                                
                                                PumpCrc = tmp;
                                                PumpCrc <<= 8;
                                               
                                                tmp=Pump_Receive_Data.bREV_Buf[PumpLen-6-1];
                                                PumpCrc += tmp;
                                                
                                               
                                                
                                                if(!DataCheckFlg)
                                                { 
                                                    PumpRevFlg=1;                                 
                                                   
                                                    PumpSendTimeOut=timer_val.current_val;
                                                    memcpy(&Pump_Receive_BUF,&Pump_Receive_Data,PumpLen+1);  //receive data len
                                                    PumpLen=0;  
                                                }
                                                else  //cheching data 
                                                {
                                                    PumpLen=0;                   //init data for next recerve
                                                }
                                               
                                                //break;
                                            }
                                            else
                                            {
                                                if ( (tmp == 0xfa) && (PumpTmp != 0xfa) )
                                                {
                                                    PumpTmp = tmp;
                                                }
                                                else
                                                {
                                                    if ( (tmp != 0xfa) && (PumpTmp == 0xfa) )
                                                    {
                                                        PumpTmp = 0;
                                                        if ( tmp == CR_ID)
                                                        {
                                                            PumpLen = 2;
                                                            
                                                            Pump_Receive_Data.bHead_Flg= 0xfa;
                                                            Pump_Receive_Data.bTarget_ID=CR_ID;   
                                                            PumpDataLen = 0;
                                                        }
                                                        else //err
                                                        {
                                                            PumpLen = 0;
                                                            PumpDataLen = 0;
                                                        }
                                                    }
                                                    else
                                                    {
                                                          PumpTmp = tmp;
                                                         
                                                          Pump_Receive_Data.bREV_Buf[PumpLen-6]=tmp;                                               
                                                
                                                          PumpCrc = tmp;
                                                          PumpCrc <<= 8;
                                                         
                                                          tmp=Pump_Receive_Data.bREV_Buf[PumpLen-6-1];
                                                          PumpCrc += tmp;
                                                        
                                                         
                                                          
                                                          if(!DataCheckFlg)
                                                          { 
                                                              PumpRevFlg=1;                                 
                                                             
                                                              PumpSendTimeOut=timer_val.current_val;
                                                              memcpy(&Pump_Receive_BUF,&Pump_Receive_Data,PumpLen+1);  //receive data len
                                                              PumpLen=0;  
                                                          }
                                                          else  //cheching data 
                                                          {
                                                              PumpLen=0;                   //init data for next recerve
                                                          }
                                                    }
                                                }
                                            }
                                        } 
                                        else
                                        {
                                                 PumpLen = 0;  
                                                 PumpDataLen = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

  
    }       
}



/******************************************************************************/
//function title: 
//function describe:
//param:none
//return:noe
//call function:none
//global variable: 
//author: steven
//date:2009-7-20
//mender:
//date:
//version:.01
/******************************************************************************/

void Send_Data_PUMP(void )
{

      unsigned short crc;
      unsigned char len ;
      unsigned int SendTimeOut ;
      
      SendTimeOut=timer_val.current_val-PumpSendTimeOut;
      if(SendTimeOut<20)   //  < 200ms
      {
            Pump_Send_BUF.bHead_Flg = 0xfa;
            Pump_Send_BUF.bTarget_ID= 0x00;      //pump ID =0X00
            Pump_Send_BUF.bSource_ID= CR_ID;      //CR ID .can be read from hardware set
            Pump_Send_BUF.bControl_Flg1= 0x00;  
            Pump_Send_BUF.bControl_Flg2= Pump_Receive_BUF.bControl_Flg2;      
            
            len=Pump_Send_BUF.bData_Len+5;      
            crc = PumpCrc16(&Pump_Send_BUF.bTarget_ID,len,0);      
           
            memcpy(&Pump_Send_BUF.bSend_Buf[Pump_Send_BUF.bData_Len],(unsigned char *)(&crc),2);
            len=len+2;
            len=(unsigned short)(AddFA(&Pump_Send_BUF.bTarget_ID,(unsigned short)(len)));          //added FA if the data have FA and modified the bSlen,
            len=len+1;              //+FA
            WRITE(USART0_ID,&Pump_Send_BUF.bHead_Flg,0,len);	//????????????
            
            
            
      }
      else
      {
           //
      }
       
      
}

/******************************************************************************/
//function title: void Pump_Time(void) 
//function describe: time our process
//param:none
//return:noe
//call function:none
//global variable: 
//author: steven
//date:2009-7-20
//mender:
//date:
//version:.01
/******************************************************************************/
void Pump_Time(void)         //100ms
{
    PumpCOMTimeOut++;
    
    if(PumpCOMTimeOut>=60)  //6s  timeout,set pumpoffline
    {
        
        Clear_Pump_Timeout();
        //init data
        PGradeMax=1;
        
        PPreState=P_IDLE;                   
        PCurState=P_IDLE;  
        CRstate=0x00;
        PGrade=1; 
        
        
        SetPumpOffline();    
        ProcStatus[PROCESS_PUMP]=P_VERIFY_PUMP;
        ProcExchange[PROCESS_PUMP][PROCESS_PUMP]=0;     
        SetDispPumpMsg(D_PUMP_OFF_LINE);
    }
    
}

/******************************************************************************/
//function title: void Clear_Pump_Timeout(void)
//function describe:
//param:none
//return:noe
//call function:none
//global variable: 
//author: steven
//date:2009-7-20
//mender:
//date:
//version:.01
/******************************************************************************/
void Clear_Pump_Timeout(void)
{
    PumpCOMTimeOut=0;
}