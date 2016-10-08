#include "pump.h"
#include "global.h"
#include "Voice.h"
#include "CRMemCopy.h"

#include "PdProtect.h"

void Write_STOP_PUMP(void);
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
void PumpGetCurPrice( void )
{

    unsigned char nozzle, nozzle_num;

    
    memset( PumpPriceBuf, 0x00, 4 * sizeof( PumpPriceBuf[0].NextPrice ));

    nozzle_num = GetGUN_N();	
    for( nozzle = 0; nozzle < nozzle_num; nozzle++ )
    {                      
      PumpPriceBuf[nozzle].NextPrice = GetPrice( nozzle, 0 );   //current Price
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
void SetPumpOffline(void)
{
    PumpOnlineflg= 0;
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
void SetPumpOnline(void)
{
    PumpOnlineflg= 1;
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
unsigned char ReadPumpOnlineFlg(void)
{
    if(PumpOnlineflg)
    {
        return PUMP_ON_LINE;
    }
    else
    {
        return PUMP_OFF_LINE;
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
void Write_VERIFY_PUMP(void)  //加油机认证
{
 
      Pump_Send_BUF.bData_Len=9;
      Pump_Send_BUF.bSend_Buf[0]=VERIFY_PUMP;                                    //CMD
      memcpy(&Pump_Send_BUF.bSend_Buf[1],&VerifyTimeData,4);     //preset      
      memcpy(&Pump_Send_BUF.bSend_Buf[5],&VerifyPumpData,4);     //preset 
      
      Send_Data_PUMP();              
      PumpCommFlg=0;
   
  
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
    
void Write_General_Answer_POLL(void)             //普通poll
{
      unsigned char tmp;
     
      Pump_Send_BUF.bData_Len=9;
      Pump_Send_BUF.bSend_Buf[0]=PUMP_POLL;                                    //CMD
      
      if( ProcStatus[PROCESS_POS] & POS_ONLINE )
      {
          memcpy(&Pump_Send_BUF.bSend_Buf[1],(unsigned char*)(&SysBCDTime),7);     //system times
      }
      else     //pos off line
      {
          memset(&Pump_Send_BUF.bSend_Buf[1],0x00,7);     //system times  //pos off line
      }
      tmp=Pump_Send_BUF.bSend_Buf[1];
      Pump_Send_BUF.bSend_Buf[1]=Pump_Send_BUF.bSend_Buf[2];   //big to little change
      Pump_Send_BUF.bSend_Buf[2]=tmp;
      Pump_Send_BUF.bSend_Buf[8]=CRstate;                //CR state
        
      Send_Data_PUMP();
      
      
      PumpCommFlg=0;
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

   
void Write_PRE_AUTH(void)
{
   if(P_CALL==PCurState)
   {
          Pump_Send_BUF.bData_Len=7;
          Pump_Send_BUF.bSend_Buf[0]=PRE_AUTH;                                    //CMD
          Pump_Send_BUF.bSend_Buf[1]=Pump_CR_Buf.PresetMode+1;     //1:money 2:volume         
          memcpy(&Pump_Send_BUF.bSend_Buf[2],&Pump_CR_Buf.PresetValue,4);     //preset         
          Pump_Send_BUF.bSend_Buf[6]=Pump_CR_Buf.NozzleNumber;              //grade
    
          Send_Data_PUMP();
          
          PowerDown.PDCRTrsFlg=TRANSAUTH;
          PumpCommFlg=0;
   }
   else   //state change ,
   {
          //授权失败
          if(P_IDLE==PCurState)  //挂枪了
          {           
              ProcExchange[PROCESS_CR][PROCESS_PUMP] &= (~PUMP_AUTH);         //结束任务
              ProcExchange[PROCESS_PUMP][PROCESS_CR]|=PUMP_ZERO;              //通知CR做零交易
              
              PowerDown.PDCRTrsFlg=TRANSOFF;
              SetDispPumpMsg(D_AUTH_ERR);
              Write_General_Answer_POLL();              
          }
          else if((P_AUTH==PCurState)||(P_BUSY==PCurState)) 
          {
              ProcExchange[PROCESS_CR][PROCESS_PUMP] &= (~PUMP_AUTH);         //结束任务
              ProcExchange[PROCESS_PUMP][PROCESS_CR]|=PUMP_BUSY;              //通知CR已授过权
              
              PowerDown.PDCRTrsFlg=TRANSON;             //20120828
              
              Write_General_Answer_POLL(); 
          }
          else //ERR display err infor 
          {
              ProcExchange[PROCESS_CR][PROCESS_PUMP] &= (~PUMP_AUTH);         //结束任务
              ProcExchange[PROCESS_PUMP][PROCESS_CR]|=PUMP_ZERO;              //通知CR做零交易
              
              PowerDown.PDCRTrsFlg=TRANSOFF;     
              SetDispPumpMsg(D_AUTH_ERR);
              Write_General_Answer_POLL(); 
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
void Write_GET_REAL_DATA(void)
{
  
     if(P_BUSY==PCurState)
     { 
          Pump_Send_BUF.bData_Len=2;
          Pump_Send_BUF.bSend_Buf[0]=GET_REAL_DATA;                         //CMD      
          Pump_Send_BUF.bSend_Buf[1]=Pump_CR_Buf.NozzleNumber;              //nozzle
    
          Send_Data_PUMP();
          
         
          PumpCommFlg=0;
     }
     else  //state change ,
     {
      
      
          if((P_AEOT==PCurState)||(P_BEOT==PCurState))  //挂枪了，结束任务
          {           
              ProcExchange[PROCESS_PUMP][PROCESS_PUMP] &= (~CR_LOAD_REAL_DATA);
              ProcExchange[PROCESS_CR][PROCESS_PUMP] &= (~CR_LOAD_REAL_DATA);
              
              Write_General_Answer_POLL();              
          }
          else if((P_STOP==PCurState)) //STOP
          {
              ProcExchange[PROCESS_PUMP][PROCESS_PUMP] &= (~CR_LOAD_REAL_DATA);  //结束任务
              ProcExchange[PROCESS_CR][PROCESS_PUMP] &= (~CR_LOAD_REAL_DATA); 
              ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_STOP; 
              
              Write_General_Answer_POLL();  
          }
          else //ERR display err infor 
          {
              ProcExchange[PROCESS_PUMP][PROCESS_PUMP] &= (~CR_LOAD_REAL_DATA);    //结束任务
              ProcExchange[PROCESS_CR][PROCESS_PUMP] &= (~CR_LOAD_REAL_DATA); 
              
              SetDispPumpMsg(D_PUMP_ERR);
              Write_General_Answer_POLL();  
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
void Write_GET_TRANS(void)
{
   unsigned short int  val;
   unsigned int value;
    //GET TRANS
    if(PowerDown.PDCRTrsFlg==TRANSON)
    {
          PowerDown.PDCRTrsFlg=TRANSCHECK;     //clear trans on flag
    }
    else
    {
          //
    }
    //PowerDown.PDCRTrsFlg=TRANSCHECK;     //clear trans on flag
    if((P_AEOT==PCurState)||(P_BEOT==PCurState)||(P_STOP==PCurState)) //the pump state need to be EOT(not change to IDLE) until CR send the get trans CMD
    { 
          Pump_Send_BUF.bData_Len=3;
          Pump_Send_BUF.bSend_Buf[0]=GET_TRANS;                             //CMD      
          Pump_Send_BUF.bSend_Buf[1]=Pump_CR_Buf.NozzleNumber;              //nozzle
          Pump_Send_BUF.bSend_Buf[2]=Load_Trans;              //type
    
          Send_Data_PUMP();
                 
          PumpCommFlg=0;
          PowerDown.PDCRTrsFlg=TRANSCHECK;     //clear trans on flag
          
     }
     else if(P_IDLE==PCurState)
     {
         //doun modified for pump offline when CR auth  20120522
          if(PowerDown.PDCRTrsFlg==TRANSCHECK)   
          {
              
                  if(ZeroVolumeFlag==1)    //down 20130201 steven  zero trans deal with,added for fixing the pump SW bug when power down in fueling ,the pump can not save the correct trans 
                  {
                            Pump_Send_BUF.bData_Len=3;
                            Pump_Send_BUF.bSend_Buf[0]=GET_TRANS;                             //CMD      
                            //Pump_Send_BUF.bSend_Buf[1]=0x01;                  //nozzle
                            Pump_Send_BUF.bSend_Buf[1]=TempGrade;               //nozzle
                            Pump_Send_BUF.bSend_Buf[2]=Load_Trans;              //type
                      
                            Send_Data_PUMP();
                                   
                            PumpCommFlg=0;
                            //PowerDown.PDCRTrsFlg=TRANSCHECK;     //clear trans on flag 
                  }        
                  else     //up 20130201 steven  zero trans deal with,added for fixing the pump SW bug when power down in fueling ,the pump can not save the correct tr
                      {
                            Pump_Send_BUF.bData_Len=3;
                            Pump_Send_BUF.bSend_Buf[0]=GET_TRANS;                             //CMD      
                            Pump_Send_BUF.bSend_Buf[1]=Pump_CR_Buf.NozzleNumber;              //nozzle
                            Pump_Send_BUF.bSend_Buf[2]=Load_REC;                              //type
                  
                            Send_Data_PUMP();
                   
                            PumpCommFlg=0;
                            //PowerDown.PDCRTrsFlg=TRANSCHECK;     //clear trans on flag
                      }
                      
          }
          else   //not has auth  ,make zero trans
          {                                                             
                  memset( Pump_CR_Buf.Volume, 0x00, sizeof( Pump_CR_Buf.Volume ));
	          memset( Pump_CR_Buf.Amount, 0x00, sizeof( Pump_CR_Buf.Amount ));
	         // memset( Pump_CR_Buf.VTotal, 0x00, sizeof( Pump_CR_Buf.VTotal ));
                  
                  
                  
                  // Get Price For Current Zero Trans
                  val = GetPrice( Pump_CR_Buf.NozzleNumber - 1, 0 );
                  memcpy( Pump_CR_Buf.Price, &val, 2 );
                  //Get TOT              
                  value = CharToIntCopy( DISP_CR_Buf.DispTotal[Pump_CR_Buf.NozzleNumber - 1].VolumeTotal );
                  memcpy( Pump_CR_Buf.VTotal, &value, sizeof( Pump_CR_Buf.VTotal ));
                 
               
                                 
                  
                  ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_LASTTRANS );  //clear CR task ,and told CR CMD is process ok!           
                  ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_LASTTRANS;
                  PowerDown.PDCRTrsFlg=TRANSOFF;     //clear trans flag
                
                      
                  ProcExchange[PROCESS_PUMP][PROCESS_CR]&= ( ~PUMP_ZERO);              //清CR做零交易
              
                  SetDispPumpMsg(D_AUTH_ERR);
                  Write_General_Answer_POLL();     
          }
          //up modified for pump offline when CR auth  20120522
          
     }
     //20120512 modified by steven for pump state err 
     else if((P_BUSY==PCurState)||(P_AUTH==PCurState))
     {
          Write_STOP_PUMP(); 
     }
     //20120512 modified by steven for pump state err 
     else
     {    
          SetDispPumpMsg(D_GET_TRANS_ERR);
          Write_General_Answer_POLL();  
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
void Write_UPLOAD_CUR_TRANS(void)
{
  
     //GET CURTRANS
    if((P_AEOT==PCurState)||(P_BEOT==PCurState)||(P_IDLE==PCurState)||(P_STOP==PCurState)) //
    { 
        Pump_Send_BUF.bData_Len=1;
        Pump_Send_BUF.bSend_Buf[0]=UPLOAD_CUR_TRANS;                         //CMD      
      
    
        Send_Data_PUMP();
 
        PumpCommFlg=0;
    }
    else
    {
    
        ProcExchange[PROCESS_PUMP][PROCESS_PUMP] &= (~CR_LOAD_CUR_TRANS);  //clear task
        
        
        SetDispPumpMsg(D_GET_TRANS_ERR);
        Write_General_Answer_POLL();  
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
void Write_GET_TOT(void)
{
  
   if(P_IDLE==PCurState)     
   { 
        Pump_Send_BUF.bData_Len=2;
        Pump_Send_BUF.bSend_Buf[0]=GET_TOT;                                //CMD      
        Pump_Send_BUF.bSend_Buf[1]=Pump_CR_Buf.NozzleNumber;              //nozzle
  
        Send_Data_PUMP();

        PumpCommFlg=0;
   }
   else  //state ERR
   {
         //不清任务，保持任务请求，继续等待
         // pump state err
        if(P_CALL==PCurState)
        {
              SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
        }
        else
        {
              SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
        }           
        
        Write_General_Answer_POLL();  
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
void Write_PPU_NEW(void)
{
  
   if(P_IDLE==PCurState)    
   { 
        Pump_Send_BUF.bData_Len=4;
        Pump_Send_BUF.bSend_Buf[0]=PPU_NEW;                         //CMD      
        if(PGrade>PGradeMax)
        {
            PGrade=1;
        }        
        Pump_Send_BUF.bSend_Buf[1]=PGrade;                  
        memcpy(&Pump_Send_BUF.bSend_Buf[2],(unsigned char*)(&PumpPriceBuf[PGrade-1]),2);    
        
        
        Send_Data_PUMP();

        PumpCommFlg=0;
   }
   else
   {
        //不清任务，保持任务请求，继续等待
        // pump state err
        if(P_CALL==PCurState)
        {
              SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
        }
        else
        {
              SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
        }   
        
        
        Write_General_Answer_POLL(); 
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
void Write_PPU_CHECK(void)
{
  
   if(P_IDLE==PCurState)   
   { 
        Pump_Send_BUF.bData_Len=2;
        Pump_Send_BUF.bSend_Buf[0]=PPU_CHECK;                         //CMD      
      
        if(PGrade>PGradeMax)
        {
            PGrade=1;
        }        
        Pump_Send_BUF.bSend_Buf[1]=PGrade;
 
        Send_Data_PUMP();
       
        PumpCommFlg=0;
   }
   else
   {
          //不清任务，保持任务请求，继续等待
          // pump state err
          if(P_CALL==PCurState)
          {
                SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
          }
          else
          {
                SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
          }  
          
          
          Write_General_Answer_POLL();                  
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
void Write_GET_OFFLINE_TRANS(void)
{
  
     if(P_IDLE==PCurState)    
     { 
          Pump_Send_BUF.bData_Len=2;
          Pump_Send_BUF.bSend_Buf[0]=GET_OFFLINE_TRANS;                         //CMD      
         
          if(PGrade>PGradeMax)
          {
              PGrade=1;
          }        
          Pump_Send_BUF.bSend_Buf[1]=PGrade;
                  
          Send_Data_PUMP();
 
          PumpCommFlg=0;
     }
     else
     {
          //不清任务，保持任务请求，继续等待
          // pump state err
          if(P_CALL==PCurState)
          {
                SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
          }
          else
          {
                SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
          }                   
      
          Write_General_Answer_POLL(); 
           
           
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
void Write_CLEAR_OFFLINE_TRANS(void)
{
  
     if(P_IDLE==PCurState)    
     { 
          Pump_Send_BUF.bData_Len=2;
          Pump_Send_BUF.bSend_Buf[0]=CLEAR_OFFLINE_TRANS;                         //CMD      
          
          if(PGrade>PGradeMax)
          {
            PGrade=1;
          }        
          Pump_Send_BUF.bSend_Buf[1]=PGrade;
          
          
          Send_Data_PUMP();
     
          PumpCommFlg=0;
     }
     else
     {
          //不清任务，保持任务请求，继续等待
          // pump state err
          if(P_CALL==PCurState)
          {
                SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
          }
          else
          {
                SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
          }                   
 
          Write_General_Answer_POLL();                
           
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
void Write_STOP_PUMP(void)
{
   
      if((P_BUSY==PCurState)||(P_AUTH==PCurState))
      {
          Pump_Send_BUF.bData_Len=1;
          Pump_Send_BUF.bSend_Buf[0]=STOP_PUMP;                         //CMD      
    
          Send_Data_PUMP();
                 
          PumpCommFlg=0;
        
     }
     else
     {
        
        if(P_STOP==PCurState)
        {
             ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_STOP  );  //clear CR task ,and told CR CMD is process
             ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_STOP_PUMP ); 
             
             Write_General_Answer_POLL();  
        }
        else
        {

            ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_STOP  );    //clear CR task ,and told CR CMD is process
            ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_STOP_PUMP ); 
            
            SetDispPumpMsg(D_PUMP_STATE_ERR);
            Write_General_Answer_POLL();    
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
void Write_CHECK_TIME(void)
{
 
     if(P_IDLE==PCurState)    
     { 
          Pump_Send_BUF.bData_Len=1;
          Pump_Send_BUF.bSend_Buf[0]=CHECK_TIME;                              //CMD      
         
          Send_Data_PUMP();
    
          PumpCommFlg=0;
     }
     else
     {
          //不清任务，保持任务请求，继续等待
          // pump state err
          if(P_CALL==PCurState)
          {
                SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
          }
          else
          {
                SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
          }    
          
          Write_General_Answer_POLL();     
       
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
void Write_UPLOAD_CONFIG(void)
{
  
   if(P_IDLE==PCurState)    
   { 
        Pump_Send_BUF.bData_Len=1;
        Pump_Send_BUF.bSend_Buf[0]=UPLOAD_CONFIG;                         //CMD      
         
        Send_Data_PUMP();
 
        PumpCommFlg=0;
   }
   else
   {
        //不清任务，保持任务请求，继续等待
        // pump state err
        if(P_CALL==PCurState)
        {
              SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
        }
        else
        {
              SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
        }                   

        Write_General_Answer_POLL();     
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
void Write_UPLOAD_ERR_NO(void)
{
  
   if(P_ERR==PCurState)    
   { 
        Pump_Send_BUF.bData_Len=1;
        Pump_Send_BUF.bSend_Buf[0]=UPLOAD_ERR_NO;                         //CMD      
        
        Send_Data_PUMP();

        PumpCommFlg=0;
   }
   else
   {
        
        //不清任务，保持任务请求，继续等待
        // pump state err
        if(P_CALL==PCurState)
        {
              SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
        }
        else
        {
              SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
        }                   
          

        Write_General_Answer_POLL();    
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
void Write_UPLOAD_VERSION(void)
{
  
   if(P_IDLE==PCurState)    
   { 
        Pump_Send_BUF.bData_Len=1;
        Pump_Send_BUF.bSend_Buf[0]=UPLOAD_VERSION;                         //CMD      
      
        Send_Data_PUMP();
        
        ProcExchange[PROCESS_PUMP][PROCESS_PUMP] &= (~CR_LOAD_PUMP_VER); 
        
        PumpCommFlg=0;
   }
   else
   {
        //不清任务，保持任务请求，继续等待
        // pump state err
        if(P_CALL==PCurState)
        {
              SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
        }
        else
        {
              SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
        }                   

        Write_General_Answer_POLL(); 
       
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
void Write_RESTART_PUMP(void)
{
 
   if(P_STOP==PCurState)    
   { 
        Pump_Send_BUF.bData_Len=1;
        Pump_Send_BUF.bSend_Buf[0]=RESTART_PUMP;                           //CMD      
       
        Send_Data_PUMP();
 
        PumpCommFlg=0;
   }
   else
   {
       
        ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP]&=(~KEYPAD_PUMP_RESTART);
        
        SetDispPumpMsg(D_PUMP_STATE_ERR);            //state err, 通知lcd  
        Write_General_Answer_POLL(); 
     
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
void Write_PUMP_ROUNDING(void)
{
  
   if(P_BUSY==PCurState)    
   { 
        Pump_Send_BUF.bData_Len=2;
        Pump_Send_BUF.bSend_Buf[0]=PUMP_ROUNDING;                           //CMD 
        //Pump_Send_BUF.bSend_Buf[1]=Pump_CR_Buf.PresetMode+1;                //rounding type 1:money 2:litre
        Pump_Send_BUF.bSend_Buf[1]=(Pump_CR_Buf.PresetMode+1)&0x01;                //rounding type 1:money 0:litre   
        Send_Data_PUMP();
 
        PumpCommFlg=0;
   }
   else
   {
        ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP]&=(~KEYPAD_PUMP_ROUND);
        
        
        SetDispPumpMsg(D_PUMP_STATE_ERR);            //state err, 通知lcd  
        Write_General_Answer_POLL();     
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
void Write_VOLUME_MODIFY(void)
{
  //
  // if(P_IDLE==PCurState)    //???
  // { 
        Pump_Send_BUF.bData_Len=3;
        Pump_Send_BUF.bSend_Buf[0]=VOLUME_MODIFY;                         //CMD 
        Pump_Send_BUF.bSend_Buf[1]=CR_ID;                                 //address
        Pump_Send_BUF.bSend_Buf[2]=PumpVoiceBuf.Voice;                    //volume
      
        Send_Data_PUMP();
        
       
        PumpCommFlg=0;
   //}
   // else
   //{
   //    不清任务，保持任务请求，继续等待
   //    SetDispPumpMsg(D_PUMP_STATE_ERR);            //state err, 通知lcd  
   //    Write_General_Answer_POLL(); 
   //}
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
void Write_VOICE_DATA(void)
{
  //
   
  // if(P_IDLE==PCurState)    //
  // { 
        Pump_Send_BUF.bData_Len=19;
        Pump_Send_BUF.bSend_Buf[0]=VOICE_DATA; //CMD   
        Pump_Send_BUF.bSend_Buf[1]=CR_ID;                    //address 
        Pump_Send_BUF.bSend_Buf[2]=PumpVoiceBuf.Voice;                      //address
        memcpy(&Pump_Send_BUF.bSend_Buf[3],&PumpVoiceBuf.VoiceData[0],16);     ///preset money
        
        Send_Data_PUMP();
        //TaskDoFlag=1;
        PumpCommFlg=0;
   //}
   //else
   //{
   //    不清任务，保持任务请求，继续等待
   //    SetDispPumpMsg(D_PUMP_STATE_ERR);            //state err, 通知lcd  
   //    Write_General_Answer_POLL(); 
     
  //}
  
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
void Write_PRINT_DATA(void)
{
  
   if((P_IDLE==PPreState)||(P_CALL==PPreState))    
   { 
        Pump_Send_BUF.bData_Len=101;
        Pump_Send_BUF.bSend_Buf[0]=PRINT_DATA;                         //CMD      
        memcpy(&Pump_Send_BUF.bSend_Buf[1],&PumpPrinterBuf.PrinterData[0],100);     ///preset money
        
        Send_Data_PUMP();
        
        PumpCommFlg=0;
   }
   else
   {
        //清任务，
        ProcExchange[PROCESS_PRINT][PROCESS_PUMP]&= ( ~CR_PRINT_DATA );  //clear CR task ,and told CR CMD is process ok!
        
        SetDispPumpMsg(D_PUMP_STATE_ERR);            //state err, 通知lcd  
        Write_General_Answer_POLL(); 
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
void Write_PUMP_IDENTIFY(void)
{
  
   if(P_CALL==PPreState) 
   { 
        Pump_Send_BUF.bData_Len=5;
        Pump_Send_BUF.bSend_Buf[0]=PUMP_IDENTIFY;                             //CMD      
        memcpy(&Pump_Send_BUF.bSend_Buf[1],&Pump_CR_Buf.PSAMRandom[0],4);     ///preset money
        
        Send_Data_PUMP();
        
        PumpCommFlg=0;
   }
   else
   {
        if(P_IDLE==PPreState)
        {
           //清任务，
           ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_VERIFY );  //clear CR task ,and told CR CMD is process ok!
           //通知CR，清任务，
           ProcExchange[PROCESS_PUMP][PROCESS_CR]|= PUMP_VERIFY_ERROR;
        }
        else
        {
          //
        }
        SetDispPumpMsg(D_PUMP_STATE_ERR);            //state err, 通知lcd  
        Write_General_Answer_POLL(); 
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

   
void Read_PUMP_IDENTIFY(void)
{
  
      
      unsigned char CmdType,CmdState;
   
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];  
   
      if((PUMP_IDENTIFY==PumpCurTask)&&(PUMP_IDENTIFY==CmdType))
      {          
          if(!CmdState)  //CMD is receive by PUMP OK
          {
                memcpy(&Pump_CR_Buf.DESRandom[0],&Pump_Receive_BUF.bREV_Buf[2],8);  
                 //清任务，
                ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_VERIFY );  //clear CR task ,and told CR CMD is process ok!
                ProcExchange[PROCESS_PUMP][PROCESS_CR]|=PUMP_VERIFY ;
                
          }
          else
          {
                //
                //err ,通知CR，清任务，
                ProcExchange[PROCESS_PUMP][PROCESS_CR]|= PUMP_VERIFY_ERROR;
                SetDispPumpMsg(D_VERIFY_PUMP_ERR);
          }
 
      }
      else
      {
                 //
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

   
void Read_VERIFY_PUMP(void)
{
  
      
   unsigned char CmdType;
   
   CmdType=Pump_Receive_BUF.bREV_Buf[0];
   
   if((VERIFY_PUMP==PumpCurTask)&&(VERIFY_PUMP==CmdType))
     {          
            memcpy(VerifyTimeData,&Pump_Receive_BUF.bREV_Buf[1],4);
                      
            ComputeVerifyData(VerifyTimeData,4,VerifyPumpData);
      
            VerifyPumpData[0]= VerifyPumpData[0]^PUMPVerifyData[0];
            VerifyPumpData[1]= VerifyPumpData[1]^PUMPVerifyData[1];  
            VerifyPumpData[2]= VerifyPumpData[2]^PUMPVerifyData[2];
            VerifyPumpData[3]= VerifyPumpData[3]^PUMPVerifyData[3];
            if(memcmp(VerifyPumpData,&Pump_Receive_BUF.bREV_Buf[5],4)==0)   //相等
            {
                 ProcExchange[PROCESS_PUMP][PROCESS_PUMP] &= ( ~ CR_VERIFY_PUMP);         // pump on line just ,                                                          
                 ProcStatus[PROCESS_PUMP]=P_LOAD_TIME ;           //the next state        
            }
            else  //不相等
            {
              
                SetDispPumpMsg(D_VERIFY_PUMP_ERR);
            }
              
           
 
     }
     else
     {
       //
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

   
void Read_PRE_AUTH(void)
{
     unsigned char CmdType,CmdState;
     
     CmdType=Pump_Receive_BUF.bREV_Buf[0];
     CmdState=Pump_Receive_BUF.bREV_Buf[1];   
     if((PRE_AUTH==PumpCurTask)&&(PRE_AUTH==CmdType))
     {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
              ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_AUTH );  //clear CR task ,and told CR CMD is process ok!
              ProcStatus[PROCESS_PUMP]=PUMP_C_AUTH; 
              PowerDown.PDCRTrsFlg=TRANSON;
             
          }
          else  //have some  err
          {
              if(STATE_ERR==CmdState)
              {
                
                  ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_AUTH );    //clear CR task ,and told CR CMD is process ok!
                  ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_ZERO;     
                  
                  PowerDown.PDCRTrsFlg=TRANSOFF;
                  SetDispPumpMsg(D_AUTH_ERR);
              }
              else if(GRADE_ERR==CmdState)
              {
                  //the  pump grade err ,told CR.形成0交易
                  ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_AUTH );  //clear CR task ,and told CR CMD is process ok!
                  ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_ZERO; 
                  
                  PowerDown.PDCRTrsFlg=TRANSOFF;
                  SetDispPumpMsg(D_AUTH_ERR);
               
              }
              else if(PRESET_ERR==CmdState)
              {
                  //the  pump grade err ,told CR.形成0交易
                  ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_AUTH );  //clear CR task ,and told CR CMD is process ok!
                  ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_ZERO; 
                  
                  PowerDown.PDCRTrsFlg=TRANSOFF;
                  SetDispPumpMsg(D_AUTH_ERR);
               
              }
              else
              {
                  //PowerDown.PDCRTrsFlg=TRANSOFF;
                  SetDispPumpMsg(D_AUTH_ERR);
              }
              
             
             
          }
     }
     else
     {
       //
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
void Read_GET_REAL_DATA(void)
{
     unsigned char CmdType,CmdState;
     CmdType=Pump_Receive_BUF.bREV_Buf[0];
     CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((GET_REAL_DATA==PumpCurTask)&&(GET_REAL_DATA==CmdType))
      {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
                memcpy(&Pump_CR_Buf.Amount[0],&Pump_Receive_BUF.bREV_Buf[2],4);
                memcpy(&Pump_CR_Buf.Volume[0],&Pump_Receive_BUF.bREV_Buf[6],4);
                memcpy(&Pump_CR_Buf.Price[0],&Pump_Receive_BUF.bREV_Buf[10],2);
                Pump_CR_Buf.NozzleNumber=Pump_Receive_BUF.bREV_Buf[12];
                  
                ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_LOAD_REAL_DATA );  //clear CR task ,and told CR CMD is process ok!
               
            
          }
          else  //have some  err
          {
              if(STATE_ERR==CmdState)
              {
                
                    SetDispPumpMsg(D_PUMP_STATE_ERR);
              }
              else if(GRADE_ERR==CmdState)
              {
                    SetDispPumpMsg(D_GRADE_ERR);
              }
  
            
          }
     }
     else
     {
       //
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
void Read_GET_TRANS(void)
{
  
      unsigned char CmdType,CmdState;
      unsigned int value;
      
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((GET_TRANS==PumpCurTask)&&(GET_TRANS==CmdType))
      {
            if(!CmdState)  //CMD is receive by PUMP OK
            {
                  memcpy(&Pump_CR_Buf.Amount[0],&Pump_Receive_BUF.bREV_Buf[2],4);
                  memcpy(&Pump_CR_Buf.Volume[0],&Pump_Receive_BUF.bREV_Buf[6],4);
                  memcpy(&Pump_CR_Buf.Price[0],&Pump_Receive_BUF.bREV_Buf[10],2);
                  memcpy(&Pump_CR_Buf.VTotal[0],&Pump_Receive_BUF.bREV_Buf[12],4);
                  Pump_CR_Buf.NozzleNumber=Pump_Receive_BUF.bREV_Buf[16];            
                  
                  // Get Volume
	          memcpy( &value, Pump_CR_Buf.Volume, 4 );
                  if((value==0x00) && (ZeroVolumeFlag ==0x00))  //down 20130201 steven  zero trans deal with,added for fixing the pump SW bug when power down in fueling ,the pump can not save the correct tr      
                  {
                         ZeroVolumeFlag=0x01;
                  }
                  else  //up 20130201 steven  zero trans deal with,added for fixing the pump SW bug when power down in fueling ,the pump can not save the correct tr   
                  {
                        ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_LASTTRANS );  //clear CR task ,and told CR CMD is process ok!           
                        ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_LASTTRANS;
                        //PowerDown.PDCRTrsFlg=TRANSOFF;     //clear trans flag
                  
                        ZeroVolumeFlag=0x00;
                        PowerDown.PDCRTrsFlg=TRANSOFF;
                  }
                  
                 TempGrade=0x01;
                  
                  
            }
            else  //have some  err
            {
                  if(STATE_ERR==CmdState)
                  {
                        //state err
                       SetDispPumpMsg(D_PUMP_STATE_ERR);
                   
                  }
                  else if(GRADE_ERR==CmdState)
                  {
                        //
                        
                         
                         if(TempGrade<PGradeMax)   //modified by steven  20130606
                         {
                              TempGrade+=1;
                         }
                         else
                         {
                              TempGrade=0x01;
                              SetDispPumpMsg(D_GRADE_ERR);  //Grade err
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
void Read_UPLOAD_CUR_TRANS(void)
{
  
      unsigned char CmdType,CmdState;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((UPLOAD_CUR_TRANS==PumpCurTask)&&(UPLOAD_CUR_TRANS==CmdType))
      {
              if(!CmdState)  //CMD is receive by PUMP OK
              {
                    memcpy(&Pump_CR_Buf.Amount[0],&Pump_Receive_BUF.bREV_Buf[2],4);
                    memcpy(&Pump_CR_Buf.Volume[0],&Pump_Receive_BUF.bREV_Buf[6],4);
                    memcpy(&Pump_CR_Buf.Price[0],&Pump_Receive_BUF.bREV_Buf[10],2);
                    memcpy(&Pump_CR_Buf.VTotal[0],&Pump_Receive_BUF.bREV_Buf[12],4);
                    Pump_CR_Buf.NozzleNumber=Pump_Receive_BUF.bREV_Buf[16];            
                    
 
                    ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_LOAD_CUR_TRANS);
                   
              }
              else  //have some  err
              {
                     ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_LOAD_CUR_TRANS);
                    
                    if(STATE_ERR==CmdState)
                    {
                       //
                    }
                    else if(LOAD_CUR_TRANS_ERR==CmdState)
                    {
                       //
                    }
                    SetDispPumpMsg(D_PUMP_STATE_ERR);
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
void Read_GET_TOT(void)
{
      unsigned char CmdType,CmdState;
      unsigned char DataLen;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
      DataLen=Pump_Receive_BUF.bData_Len-2 ;   //TOT data len
      
      if((GET_TOT==PumpCurTask)&&(GET_TOT==CmdType))
      {
              if(!CmdState)  //CMD is receive by PUMP OK
              {
                    if(DataLen==PGradeMax*8)
                    {
                        memcpy(&Pump_CR_Buf.VolumeTotal[0],&Pump_Receive_BUF.bREV_Buf[2],PGradeMax*4);  //VOLUME TOT 
                        memcpy(&Pump_CR_Buf.AmountTotal[0],&Pump_Receive_BUF.bREV_Buf[2+PGradeMax*4],PGradeMax*4);  //Amount TOT 
                    } 
                   else
                   {
                    
                   }
                     
                    ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_TOTALS );  //clear CR task ,and told CR CMD is process ok!
                    ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_TOTALS;
                    
              }
              else  //have some  err
              {
                    if(STATE_ERR==CmdState)
                    {
                          //the  pump cur state is not do PreAuth ,told CR
                          SetDispPumpMsg(D_PUMP_STATE_ERR);
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
void Read_PPU_NEW(void)
{
  //
      unsigned char CmdType,CmdState;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((PPU_NEW==PumpCurTask)&&(PPU_NEW==CmdType))
      {
            if(!CmdState)  //CMD is receive by PUMP OK
            {
                 
                  ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_PPU_NEW );  //clear CR task ,and told CR CMD is process ok!                 
                  ProcExchange[PROCESS_PUMP][PROCESS_PUMP]|= CR_PPU_CHECK;    //the next task
            
                  ProcStatus[PROCESS_PUMP]=P_PPU_CHECK ;                        //the next state     
       
                 
            }
            else  //have some  err
            {
                  if(STATE_ERR==CmdState)
                  {
                      //the  pump cur state is not do PreAuth ,told CR
                       SetDispPumpMsg(D_PUMP_STATE_ERR);
                  }
                  else if(GRADE_ERR==CmdState)
                  {
                      
                       SetDispPumpMsg(D_GRADE_ERR);
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
void Read_PPU_CHECK(void)
{
      unsigned char CmdType,CmdState,NewFlag;
      unsigned short Gradeppu;
      unsigned char Grade;
        
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
     
         
      if((PPU_CHECK==PumpCurTask)&&(PPU_CHECK==CmdType))
      {
          if(!CmdState)  //CMD is receive by PUMP OK  ,and ppu is modified by pump ok 
          {
              NewFlag=Pump_Receive_BUF.bREV_Buf[2];
              Grade=Pump_Receive_BUF.bREV_Buf[3];                 
              memcpy((unsigned char*)(&Gradeppu),&Pump_Receive_BUF.bREV_Buf[4],2); 
                     
              if(PUMP_PPU_NEW_DOING==NewFlag)  // in modification ,check again  ,and need display
              {
               
                  SetDispPumpMsg(D_PPU_New);
              }
              else      //ppu new ok
              {
                    if(Grade==PGrade)      //???   ppu check 
                    {
                
                         
                        if(PumpPriceBuf[PGrade-1].NextPrice==Gradeppu)  //if ppu the same ,then check next
                        {
                              if(Grade<PGradeMax)  //check next grade 
                              {
                                    PGrade+=1;
                                   
                              }
                              else   //end ppu check
                              {
                                
                                  ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_PPU_CHECK ); 
                                
                                  if(ReadPumpOnlineFlg()==PUMP_OFF_LINE)
                                  {
                                      ProcExchange[PROCESS_PUMP][PROCESS_PUMP]|= CR_LOAD_OFFLINE_TRANS;    //the next task
                                 
                                      ProcStatus[PROCESS_PUMP]=P_OFFLINE_TRANS ;         //the next state   
                                      
                                  }
                                  else  //pump online , ppu new 
                                  {
                                      SetPriceUpdateFlg( 10 );
                                      ProcExchange[PROCESS_SCHEDULE][PROCESS_PUMP]&= ( ~PRICE_UPDATE);  //begin ppu new 
          
                                      ProcStatus[PROCESS_PUMP]=PUMP_C_IDLE;
                                      //<3 lie,; 3-9;steven; //3=begin ppu new ;  =10 end ppu new;
                                      //end the task
                                  }
                                  PGrade=1;          //init pgrade
                                  
                             }
                                  
                       }
                       else  //PPU not the same  ,the next task is PPU NEW
                       {
                            
                            ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_PPU_CHECK );    //clear CR pretask ,and told CR CMD is process ok!                
                            ProcExchange[PROCESS_PUMP][PROCESS_PUMP]|= CR_PPU_NEW;    //the next task                                                             
                            ProcStatus[PROCESS_PUMP]=P_PPU_NEW ;                       //the next state
                                  
                       }
                
                 
                }
                else //Grade is not right
                {                 
                    //grade is not right,  resend check ppu CMD
                }
                   
              }
         
          }         
          else  //have some  err ,and  DISPLAY
          {
 
              if(STATE_ERR==CmdState)
              {
                   //the  pump cur state is not do PreAuth ,told CR
                   SetDispPumpMsg(D_PUMP_STATE_ERR);
              }
              else if(GRADE_ERR==CmdState)
              {
                   //
                   SetDispPumpMsg(D_GRADE_ERR);
              }
              else if(TAX_PPU_NEW_ERR==CmdState)
              {
                   //
                   SetDispPumpMsg(D_PPU_NEW_ERR);
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
void Read_GET_OFFLINE_TRANS(void)
{
  //
      unsigned char CmdType,CmdState,Grade;
      
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
      Grade=Pump_Receive_BUF.bREV_Buf[14];
      
      if((GET_OFFLINE_TRANS==PumpCurTask)&&(GET_OFFLINE_TRANS==CmdType))
      {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
            
                if(Grade==PGrade)
                {
                      memcpy(&Pump_CR_Buf.OfflineAmount[(Grade-1)*4],&Pump_Receive_BUF.bREV_Buf[2],4);
                      memcpy(&Pump_CR_Buf.OfflineVolume[(Grade-1)*4],&Pump_Receive_BUF.bREV_Buf[6],4);        
                      memcpy(&Pump_CR_Buf.VolumeTotal[(Grade-1)*4],&Pump_Receive_BUF.bREV_Buf[10],4);
                     
                      if(Grade==PGradeMax )  //END load offline trans
                      {
                        ProcExchange[PROCESS_PUMP][PROCESS_CR] |=PUMP_OFFLINE_TRANS;         //told CR have a offline trans need to be process
                      } 
                      
                    
                      ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_LOAD_OFFLINE_TRANS );                          
                      ProcExchange[PROCESS_PUMP][PROCESS_PUMP]|= CR_CLEAR_OFFLINE_TRANS;    //the next task           
                      
                     
                      ProcStatus[PROCESS_PUMP]=P_CLR_OFFLINE_TRANS ;         //the next state                          
             
                     
                }
                else  //not the need grade ,resend CMD
                {
                      //RESEND THE CMD
                }
          }
          else  //have some  err
          {
            
                if(STATE_ERR==CmdState)
                {
                      //the  pump cur state is not do PreAuth ,told CR
                       SetDispPumpMsg(D_PUMP_STATE_ERR);
                }
                else if(GRADE_ERR==CmdState)
                {
                      
                       SetDispPumpMsg(D_GRADE_ERR);
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
void Read_CLEAR_OFFLINE_TRANS(void)
{
  //
      unsigned char CmdType,CmdState;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((CLEAR_OFFLINE_TRANS==PumpCurTask)&&(CLEAR_OFFLINE_TRANS==CmdType))
      {
            if(!CmdState)  //CMD is receive by PUMP OK
            {                               
           
                if(PGrade<PGradeMax)  //Load next grade offline trans 
                {
                     PGrade+=1;
                     ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_CLEAR_OFFLINE_TRANS );    
                     ProcExchange[PROCESS_PUMP][PROCESS_PUMP]|= CR_LOAD_OFFLINE_TRANS;    //the next task

                     ProcStatus[PROCESS_PUMP]=P_OFFLINE_TRANS ;         //the next state 
                   
                }
                else   //end TASK
                {
                     PGrade=1;
                     ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_CLEAR_OFFLINE_TRANS );  //clear CR task ,and told CR CMD is process ok!
                      //SET PUMP ONLINE FLAG
                     if((ProcStatus[PROCESS_PUMP]&PUMP_C_ONLINE)==PUMP_C_OFFLINE)   //pump online just ,check the pump PPU
                     {
                         ProcStatus[PROCESS_PUMP]=PUMP_C_ONLINE ;  //pump online state
                         SetPumpOnline();                         //set pump online flag
                     }
             
                }
 
          }
          else  //have some  err
          {
              if(STATE_ERR==CmdState)
              {
                  //the  pump cur state is not do PreAuth ,told CR
                   SetDispPumpMsg(D_PUMP_STATE_ERR);
              }
              else if(GRADE_ERR==CmdState)
              {
                  //
                   SetDispPumpMsg(D_GRADE_ERR);
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
void Read_STOP_PUMP(void)
{
  // 
      unsigned char CmdType,CmdState;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((STOP_PUMP==PumpCurTask)&&(STOP_PUMP==CmdType))
      {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
            
 
                ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_STOP  );    //clear CR task ,and told CR CMD is process ok!
                ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_STOP_PUMP );  //PUMP STOP               
 
          }
          else  //have some  err
          {
               if(STATE_ERR==CmdState)
                {
                    //the  pump cur state is not do PreAuth ,told CR
                    SetDispPumpMsg(D_PUMP_STATE_ERR);
                   
                }
                else if(GRADE_ERR==CmdState)
                {
                    //
                    SetDispPumpMsg(D_GRADE_ERR);
                }
                ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~PUMP_STOP  );    //clear CR task ,and told CR CMD is process ok!
                ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_STOP_PUMP );  //PUMP STOP           
             
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
void Read_CHECK_TIME(void)
{
  //
      unsigned char CmdType,CmdState;
      
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((CHECK_TIME==PumpCurTask)&&(CHECK_TIME==CmdType))
      {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
            memcpy(&PTime[0],&Pump_Receive_BUF.bREV_Buf[2],7);
            AdjustSystime(PTime, 1);    //modify system time ,author is liuzuogang
            
            ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_LOAD_PUMP_TIME );  //clear CR task ,and told CR CMD is process ok!          
            ProcExchange[PROCESS_PUMP][PROCESS_PUMP]|= CR_LOAD_PUMP_CONFIG;    //next task          
            ProcStatus[PROCESS_PUMP]=P_LOAD_COFIG ;         //next state

          }
          else  //have some  err
          {
              if(STATE_ERR==CmdState)
              {
                  //the  pump cur state is not do PreAuth ,told CR
                  SetDispPumpMsg(D_PUMP_STATE_ERR);
              }
              else if(GRADE_ERR==CmdState)
              {
                  //
                  SetDispPumpMsg(D_GRADE_ERR);
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
void Read_UPLOAD_CONFIG(void)
{
  //
      unsigned char CmdType,CmdState;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((UPLOAD_CONFIG==PumpCurTask)&&(UPLOAD_CONFIG==CmdType))
      {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
            
              memcpy(&PConfig[0],&Pump_Receive_BUF.bREV_Buf[2],4);
              PGradeMax=Pump_Receive_BUF.bREV_Buf[6] ;                  //pump max grade 
              if(PGradeMax>GRADEMAX)
              {
                SetDispPumpMsg(D_PUMP_SET_ERR);                          //pump config err
              }
              else
              {
                 memcpy(&PVersion[0],&Pump_Receive_BUF.bREV_Buf[7],3);     //pump version
                 memcpy(&Pump_CR_Buf.MaxFuellingVolume[0],&Pump_Receive_BUF.bREV_Buf[10],4);  //max volume
                 
                 memcpy(&Pump_CR_Buf.PUMPSerialNumber[0],&Pump_Receive_BUF.bREV_Buf[14],8);  //PUMPSerialNumber
                 memcpy(&Pump_CR_Buf.PumpKey[0],&Pump_Receive_BUF.bREV_Buf[22],16);  //PUMPSerialNumber
                 
                 ProcExchange[PROCESS_PUMP][PROCESS_PUMP]&= ( ~CR_LOAD_PUMP_CONFIG );  //clear CR task ,and told CR CMD is process ok!
                 ProcStatus[PROCESS_PUMP]=P_PPU_CHECK ;           //next state
                
                 PGrade=1;
                
              }
            
          }
          else  //have some  err
          {
              if(STATE_ERR==CmdState)
              {
                    //the  pump cur state is not do PreAuth ,told CR
                    SetDispPumpMsg(D_PUMP_STATE_ERR);
              }
              else if(GRADE_ERR==CmdState)
              {
                    //
                    SetDispPumpMsg(D_GRADE_ERR);
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
void Read_UPLOAD_ERR_NO(void)
{
  //
      unsigned char CmdType,CmdState;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((UPLOAD_ERR_NO==PumpCurTask)&&(UPLOAD_ERR_NO==CmdType))
      {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
                PErrInfor=Pump_Receive_BUF.bREV_Buf[2];
                ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~CR_LOAD_PUMP_ERR_NO );  //clear CR task ,and told CR CMD is process ok!
                
          }
          else  //have some  err
          {
                if(STATE_ERR==CmdState)
                {
                      //the  pump cur state is not do PreAuth ,told CR
                      SetDispPumpMsg(D_PUMP_STATE_ERR);
                }
                else if(GRADE_ERR==CmdState)
                {
                      //
                      SetDispPumpMsg(D_GRADE_ERR);
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
void Read_UPLOAD_VERSION(void)
{
  //
      unsigned char CmdType,CmdState;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((UPLOAD_VERSION==PumpCurTask)&&(UPLOAD_VERSION==CmdType))
      {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
            
              memcpy(&PVersion[0],&Pump_Receive_BUF.bREV_Buf[2],3);
              ProcExchange[PROCESS_CR][PROCESS_PUMP]&= ( ~CR_LOAD_PUMP_VER );  //clear CR task ,and told CR CMD is process ok!
              
          }
          else  //have some  err
          {
              if(STATE_ERR==CmdState)
              {
                    //the  pump cur state is not do PreAuth ,told CR
                    SetDispPumpMsg(D_PUMP_STATE_ERR);
              }
              else if(GRADE_ERR==CmdState)
              {
                    //
                    SetDispPumpMsg(D_GRADE_ERR);
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
void Read_RESTART_PUMP(void)
{
  //
      unsigned char CmdType,CmdState;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((RESTART_PUMP==PumpCurTask)&&(RESTART_PUMP==CmdType))
      {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
             //
          }
          else  //have some  err
          {
              if(STATE_ERR==CmdState)
              {
                    //the  pump cur state is not do PreAuth ,told CR
                    SetDispPumpMsg(D_PUMP_STATE_ERR);
              }
              else if(GRADE_ERR==CmdState)
              {
                    //
                    SetDispPumpMsg(D_GRADE_ERR);
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
void Read_PUMP_ROUNDING(void)
{
  //
      unsigned char CmdType,CmdState;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((PUMP_ROUNDING==PumpCurTask)&&(PUMP_ROUNDING==CmdType))
      {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
                  //
          }
          else  //have some  err
          {
              if(STATE_ERR==CmdState)
              {
                    //the  pump cur state is not do PreAuth ,told CR
                    SetDispPumpMsg(D_PUMP_STATE_ERR);
              }
              else if(GRADE_ERR==CmdState)
              {
                    //
                    SetDispPumpMsg(D_GRADE_ERR);
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
void Read_VOLUME_MODIFY(void)
{
  //
      unsigned char CmdType,CmdState;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         
      if((VOLUME_MODIFY==PumpCurTask)&&(VOLUME_MODIFY==CmdType))
      {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
           
                ProcExchange[PROCESS_VOICE][PROCESS_PUMP]&= ( ~CR_VOICE_VOLUME_ADJUST );  //clear CR task ,and told CR CMD is process ok!
               
          }
          else  //have some  err
          {
                 //
                if(STATE_ERR==CmdState)
                {
                        //the  pump cur state is not do PreAuth ,told CR
                        SetDispPumpMsg(D_PUMP_STATE_ERR);
                }
                else if(GRADE_ERR==CmdState)
                {
                        //
                        SetDispPumpMsg(D_GRADE_ERR);
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
void Read_VOICE_DATA(void)
{
  //
      unsigned char CmdType,CmdState;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
      
      if(VOICE_DATA==CmdType)
      {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
              ProcExchange[PROCESS_VOICE][PROCESS_PUMP]&= (~CR_VOICE_DATA );  //clear CR task ,and told CR CMD is process ok!
             
          }
          else  //have some  err
          {
              if(STATE_ERR==CmdState)
              {
                    //the  pump cur state is not do PreAuth ,told CR
                    SetDispPumpMsg(D_PUMP_STATE_ERR);
              }
              else if(GRADE_ERR==CmdState)
              {
                    //
                    SetDispPumpMsg(D_GRADE_ERR);
              }
               ProcExchange[PROCESS_VOICE][PROCESS_PUMP]&= (~CR_VOICE_DATA );  //clear CR task ,and told CR CMD is process ok!
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
void Read_PRINT_DATA(void)
{
  //
      unsigned char CmdType,CmdState;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      CmdState=Pump_Receive_BUF.bREV_Buf[1];
         

      if(PRINT_DATA==CmdType)
      {
          if(!CmdState)  //CMD is receive by PUMP OK
          {
    
              ProcExchange[PROCESS_PRINT][PROCESS_PUMP]&= ( ~CR_PRINT_DATA );  //clear CR task ,and told CR CMD is process ok!
 
          }
          else  //have some  err
          {
              if(STATE_ERR==CmdState)
              {
                    //the  pump cur state is not do PreAuth ,told CR
                    SetDispPumpMsg(D_PUMP_STATE_ERR);
              }
              else if(GRADE_ERR==CmdState)
              {
                    //
                    SetDispPumpMsg(D_GRADE_ERR);
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
void ANSWER_TRANSMIT(void)
{
     
     //if(P_IDLE==PPreState)   
     //{ 
      Pump_Send_BUF.bData_Len=2;
      Pump_Send_BUF.bSend_Buf[0]=TRANSMIT_DATA;                         //CMD      
      Pump_Send_BUF.bSend_Buf[1]=0x00;                  //       
      
      Send_Data_PUMP();
      
      PumpCommFlg=0;
      
     //}
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
void ANSWER_TRANSMIT_ERR(void)
{
  
     //if(P_IDLE==PPreState)   
     //{ 
      Pump_Send_BUF.bData_Len=2;
      Pump_Send_BUF.bSend_Buf[0]=TRANSMIT_DATA;                         //CMD      
      Pump_Send_BUF.bSend_Buf[1]=0x83;                  //  DATA ERR     
      
      Send_Data_PUMP();
      
      PumpCommFlg=0;
     //}
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
void Read_TRANSMIT_DATA(void)
{
  
      unsigned char CmdType,DataType;
      CmdType=Pump_Receive_BUF.bREV_Buf[0];
      DataType=Pump_Receive_BUF.bREV_Buf[1];
         
    
      if(TRANSMIT_DATA==CmdType)
      {
 
            if(VOLUMEDATA==DataType)
            {
              
                 ANSWER_TRANSMIT();            //answer the CMD
            }
            else if(VOICEDATA==DataType)
            {
                  //
                  ANSWER_TRANSMIT();            //answer the CMD
            }
            else if(PRINTDATA==DataType)
            {
                  memcpy(&PumpPrinterBuf.PrinterData[0],&Pump_Receive_BUF.bREV_Buf[2],100);
                  ProcExchange[PROCESS_PUMP][PROCESS_PRINT]|= CR_PRINT_DATA ;
                  ANSWER_TRANSMIT();            //answer the CMD
            }
            else
            {
                  //err
                  ANSWER_TRANSMIT_ERR();
            }
  
      }
} 


/******************************************************************************/
//function title: Scan_CRTask
//function describe:Scan CR task ,then distribute the pump task
//param:none
//return:noe
//call function:none
//global variable: ProcExchange[PROCESS_CR][PROCESS_PUMP]
//author: steven
//date:2009-7-20
//mender:
//date:
//version:.01
/******************************************************************************/


unsigned char ScanCRTask(void)
{
   unsigned char retval;
   unsigned int CRtask;
     
    retval=0;
    
    //interior task
    if(ProcExchange[PROCESS_PUMP][PROCESS_PUMP])   //have interior task
    {
          CRtask=ProcExchange[PROCESS_PUMP][PROCESS_PUMP];   //interior task
          if(CRtask&CR_VERIFY_PUMP)
          {
                retval=VERIFY_PUMP;
          }           
          else if(CRtask&CR_LOAD_CUR_TRANS)
          {
                retval=UPLOAD_CUR_TRANS;
          }
          else if(CRtask&CR_LOAD_PUMP_TIME)
          {
                retval=CHECK_TIME;
            
          } 
          else if(CRtask&CR_LOAD_PUMP_CONFIG)
          {
                retval=UPLOAD_CONFIG;
            
          } 
          else if(CRtask&CR_PPU_CHECK)
          {
                retval=PPU_CHECK;
            
          }
          else if(CRtask&CR_PPU_NEW)
          {
                retval=PPU_NEW;
            
          }
          else if(CRtask&CR_LOAD_OFFLINE_TRANS)
          {
                retval=GET_OFFLINE_TRANS;
            
          }
          else if(CRtask&CR_CLEAR_OFFLINE_TRANS)
          {
                retval=CLEAR_OFFLINE_TRANS;
            
          }
          else if(CRtask&CR_LOAD_REAL_DATA)
          {
                retval=GET_REAL_DATA;
            
          }    
          else if(CRtask&CR_STOP_PUMP )
          {
                retval=STOP_PUMP;
            
          }               
          else if(CRtask&CR_LOAD_PUMP_ERR_NO)
          {
                retval=UPLOAD_ERR_NO;
            
          }
          else //err task ,clear 
          {
                ProcExchange[PROCESS_PUMP][PROCESS_PUMP]=0;
          }
  
  
    }
    else if(ProcExchange[PROCESS_SCHEDULE][PROCESS_PUMP])
    {
          CRtask=ProcExchange[PROCESS_SCHEDULE][PROCESS_PUMP];
          if(CRtask&PRICE_UPDATE)
          {
                retval=PPU_CHECK;            
          }
          else //err task ,clear 
          {
                ProcExchange[PROCESS_SCHEDULE][PROCESS_PUMP]=0;
          }
      
    }
    else if(ProcExchange[PROCESS_CR][PROCESS_PUMP])
    {
          CRtask=ProcExchange[PROCESS_CR][PROCESS_PUMP];
          if(CRtask&PUMP_VERIFY)
          {
                retval=PUMP_IDENTIFY;          
          } 
          else if(CRtask&PUMP_AUTH)
          {
                retval=PRE_AUTH;
            
          }
          else if(CRtask&PUMP_LASTTRANS)
          {
                retval=GET_TRANS;
            
          }
          else if(CRtask&PUMP_TOTALS)
          {
                retval=GET_TOT;
            
          }
          else if(CRtask&PUMP_STOP )
          {
                retval=STOP_PUMP;
            
          }
          else if(CRtask&PUMP_LOAD_ERR_NO)
          {
                retval=UPLOAD_ERR_NO;           
          }
          else if(CRtask&CR_LOAD_PUMP_VER)
          {
                retval=UPLOAD_VERSION;          
          }      
          else  //err task ,clear 
          {
                ProcExchange[PROCESS_CR][PROCESS_PUMP]=0;
          }
          
                               
     }
     else if(ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP])
     {
          CRtask=ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP];
          if(CRtask&KEYPAD_PUMP_ROUND)
          {
                ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP]&=(~KEYPAD_PUMP_ROUND);
                Pump_CR_Buf.PresetMode=0;   //money 
                retval=PUMP_ROUNDING;           
          }
          else if(CRtask&KEYPAD_PUMP_VOL_ROUND)
          {
                ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP]&=(~KEYPAD_PUMP_VOL_ROUND);
                Pump_CR_Buf.PresetMode=1;   //litre
                retval=PUMP_ROUNDING;   
          }
          else if(CRtask&KEYPAD_PUMP_STOPPUMP)
          {
                 ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP]&=(~KEYPAD_PUMP_STOPPUMP);
                 retval=STOP_PUMP;
          }
          else if(CRtask&KEYPAD_PUMP_RESTART)
          {
                ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP]&=(~KEYPAD_PUMP_RESTART);
                retval=RESTART_PUMP;     
          }
          else  //err task ,clear 
          {
                ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP]=0;
          }
     }
     else if(ProcExchange[PROCESS_PRINT][PROCESS_PUMP])  //2010 
     {
          CRtask=ProcExchange[PROCESS_PRINT][PROCESS_PUMP];
          if(CRtask&CR_PRINT_DATA)
          {
                retval=PRINT_DATA;           
          }
          else  //err task ,clear 
          {
                ProcExchange[PROCESS_PRINT][PROCESS_PUMP]=0;
          }
     }
    //201009
     else if(ProcExchange[PROCESS_VOICE][PROCESS_PUMP])  //2010 09
     {
          CRtask=ProcExchange[PROCESS_VOICE][PROCESS_PUMP];
          if(CRtask&CR_VOICE_VOLUME_ADJUST)  //voice adjust
          {
                retval=VOLUME_MODIFY;           
          }
          else if(CRtask&CR_VOICE_DATA)
          {
               ProcExchange[PROCESS_VOICE][PROCESS_PUMP]&= (~CR_VOICE_DATA );  //clear CR task ,and told CR CMD is process ok! 
               retval=VOICE_DATA;           
          }
          else  //err task ,clear 
          {
                ProcExchange[PROCESS_VOICE][PROCESS_PUMP]=0;
          }
     }
    
   
     return retval;
    
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

void PumpTask(void)
{

  unsigned char Pump_Task;
  
  Pump_Task=ScanCRTask();
  PumpCurTask=Pump_Task;            //pump current task
  
  switch( Pump_Task)
    {
      
     case UPLOAD_CUR_TRANS:    
     Write_UPLOAD_CUR_TRANS();   
     break;
     
     case PRE_AUTH:    
     Write_PRE_AUTH();   
     break;
     
     case GET_REAL_DATA:     
     Write_GET_REAL_DATA();    
     break; 
     
     case GET_TRANS:
     Write_GET_TRANS();   
     break; 
     
     case GET_TOT:
     Write_GET_TOT();       
     break;
     
     case PPU_NEW:  
     Write_PPU_NEW();
     break;
        
     case PPU_CHECK:    
     Write_PPU_CHECK();
     break;
     
     case GET_OFFLINE_TRANS:    
     Write_GET_OFFLINE_TRANS();
     break;
     
     case CLEAR_OFFLINE_TRANS:    
     Write_CLEAR_OFFLINE_TRANS();
     break;
     
     case STOP_PUMP:      
     Write_STOP_PUMP();      
     break;
       
     case CHECK_TIME:     
     Write_CHECK_TIME();
     break;
        
     case UPLOAD_CONFIG:     
     Write_UPLOAD_CONFIG();
     break;
     
     case UPLOAD_ERR_NO:
     Write_UPLOAD_ERR_NO();
     break;
        
     case UPLOAD_VERSION:
     Write_UPLOAD_VERSION();  
     break;
     
     case RESTART_PUMP:    
     Write_RESTART_PUMP();  
     break;
     
     case PUMP_ROUNDING:    
     Write_PUMP_ROUNDING();    
     break;
     
     case VOLUME_MODIFY:
     Write_VOLUME_MODIFY();
     break;
     
     case VOICE_DATA:    
     Write_VOICE_DATA();   
     break;
     
     case PRINT_DATA:             //打印数据
     Write_PRINT_DATA();   
     break;
     
     case VERIFY_PUMP:             //加油机认证
     Write_VERIFY_PUMP();   
     break;
     
     case PUMP_IDENTIFY:             //加油机PSAM认证
     Write_PUMP_IDENTIFY();
     break;
     
     default: 
       
     Write_General_Answer_POLL();             //普通回应
    
     break;
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

void RevDataResolve(void)
{

  unsigned char Pump_Task;
  unsigned char CmdType;
  
  CmdType=Pump_Receive_BUF.bREV_Buf[0];
  Pump_Task=PumpCurTask;            //pump current task
  if(CmdType==Pump_Task)
  {
    switch( CmdType)
    {
      
     case UPLOAD_CUR_TRANS: 
     Read_UPLOAD_CUR_TRANS();
     break;
     
     case PRE_AUTH:    
     Read_PRE_AUTH();   
     break;
     
     case GET_REAL_DATA:     
     Read_GET_REAL_DATA();    
     break; 
     
     case GET_TRANS:
     Read_GET_TRANS();   
     break; 
     
     case GET_TOT:
     Read_GET_TOT();       
     break;
     
     case PPU_NEW:  
     Read_PPU_NEW();
     break;
        
     case PPU_CHECK:    
     Read_PPU_CHECK();
     break;
     
     case GET_OFFLINE_TRANS:    
     Read_GET_OFFLINE_TRANS();
     break;
     
     case CLEAR_OFFLINE_TRANS:   
     Read_CLEAR_OFFLINE_TRANS();
     break;
     
     case STOP_PUMP:      
     Read_STOP_PUMP();      
     break;
       
     case CHECK_TIME:     
     Read_CHECK_TIME();
     break;
        
     case UPLOAD_CONFIG:     
     Read_UPLOAD_CONFIG();
     break;
     
     case UPLOAD_ERR_NO:
     Read_UPLOAD_ERR_NO();
     break
       ;
        
     case UPLOAD_VERSION:
     Read_UPLOAD_VERSION();  
     break;
     
     case RESTART_PUMP:    
     Read_RESTART_PUMP();  
     break;
     
     case PUMP_ROUNDING:    
     Read_PUMP_ROUNDING();    
     break;
     
     case VOLUME_MODIFY:
     Read_VOLUME_MODIFY();
     break;
     
     case VOICE_DATA:    
     Read_VOICE_DATA();   
     break;
     
     case PRINT_DATA:             //打印数据
     Read_PRINT_DATA();   
     break;
     
     case VERIFY_PUMP:             //认证数据
     Read_VERIFY_PUMP();   
     break;
    
     case PUMP_IDENTIFY:             //加油机PSAM认证
     Read_PUMP_IDENTIFY();
     break;
     
     default: 
    
     break;
    }
  }
  else
  {
     if(CmdType==TRANSMIT_DATA)
     {
          Read_TRANSMIT_DATA();  
     }
     else
     {
         //not the pump need data
         //wait next data
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
void ReSendCMD(void)
{
  
    if((PPreState==PCurState)&&(PumpGrade==PCurGrade))  //the state same and the same grade
    {
          Send_Data_PUMP();
          PumpCommFlg=0;
    }
    else  //else the state change ,tell the CR the state is change and wait next CMD
    {
        // err management
        if(PRE_AUTH==PumpCurTask)  //if preset CMD, 
        {
                if((PCurState==P_IDLE)&&(PPreState==P_CALL))  //maby cancel
                {
                      //cancel the task and told CR 
                     
                      ProcExchange[PROCESS_CR][PROCESS_PUMP]&= (~PUMP_AUTH);   //结束任务
                      ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_ZERO;      //0交易             
  
                      PowerDown.PDCRTrsFlg=TRANSOFF;      //20130201
                      SetDispPumpMsg(D_AUTH_ERR);
                     
                    
                }
                else if((PCurState==P_CALL)&&(PumpGrade!=PCurGrade))  //nozzle chhange 
                {
                      //told CR modify the grade and resned pre_auth
                      // cancel the CMD
                      ProcExchange[PROCESS_CR][PROCESS_PUMP]&= (~PUMP_AUTH);   //结束任务
                      ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_ZERO;   //0交易  

                      PowerDown.PDCRTrsFlg=TRANSOFF;      //20130201
                      SetDispPumpMsg(D_GRADE_ERR);
 
                 }
                 else if((PCurState==P_AUTH)||(PCurState==P_BUSY))  //SEND STOP  
                 {
                       ProcExchange[PROCESS_CR][PROCESS_PUMP]&= (~PUMP_AUTH);       //结束任务
 
                       SetDispPumpMsg(D_PUMP_STATE_ERR);
                      
                 }
                 else if((PCurState==P_AEOT)||(PCurState==P_BEOT))
                 {
                       ProcExchange[PROCESS_CR][PROCESS_PUMP]&= (~PUMP_AUTH);       //结束任务
                       ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_LASTTRANS;     //要交易  //???  or 做零交易
  
                       SetDispPumpMsg(D_PUMP_STATE_ERR);
                      
                 }
                 else if(PCurState==P_ERR)
                 {
                   
                        SetDispPumpMsg(D_PUMP_STATE_ERR);
                       
                 }
                 else
                 {
                        SetDispPumpMsg(D_PUMP_STATE_ERR);
                      
                 }
         
        }
        else  //state change 
        {
                SetDispPumpMsg(D_PUMP_STATE_ERR);
             
                PumpStateCheck();
        }
              
         
         Write_General_Answer_POLL();             //普通回应   
         PumpCommFlg=0;  //receive the next CMD
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
void PumpStateCheck(void)
{   
    switch( PCurState)
    {
    case P_CALL:
         if(P_IDLE==PPreState) 
         {

            if((ProcStatus[PROCESS_CR] & CARD_READY )&&( ProcStatus[PROCESS_CR] & CARD_LOGON )&&(!(ProcStatus[PROCESS_CR] & CARD_BUSY )))             
            {
                ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_CALL;  
                Pump_CR_Buf.NozzleNumber=PCurGrade;
            }
            else
            {     
                Pump_CR_Buf.NozzleNumber=PCurGrade;
                
                SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
                ProcExchange[PROCESS_VOICE][PROCESS_PUMP] |= CR_VOICE_DATA; 
  
                PumpVoiceBuf.Voice=IdleNozzleOnVoice;
                memset( PumpVoiceBuf.VoiceData, 0x00, sizeof( PumpVoiceBuf.VoiceData ));
                
            }
         
         }       
         else if(P_CALL==PPreState)   //wait AUTH
         {
             //wait state,answer poll
         }
         else if(P_AUTH==PPreState)   //stop
         {
               
           //SetDispPumpMsg(D_AUTH_ERR);    
           //wait state,answer poll
         }
         else if(P_BUSY==PPreState)   //stop
         {
             //wait state,answer poll
         }
         else //prestae= auth,busy
         {
              //wait state ,answer poll
         }
         ProcStatus[PROCESS_PUMP]=PUMP_C_CALL;        //call state
         
               
    break;
    case P_AUTH:  
         
          //if(PowerDown.PDCRTrsFlg==TRANSON)  //have a trans,
          if((PowerDown.PDCRTrsFlg==TRANSAUTH)||(PowerDown.PDCRTrsFlg==TRANSON))
          {
                 //
            
                ProcExchange[PROCESS_CR][PROCESS_PUMP] &= (~PUMP_AUTH);         //结束任务
                ProcExchange[PROCESS_PUMP][PROCESS_CR]|=PUMP_BUSY;              //通知CR已授过权
                if(PowerDown.PDCRTrsFlg==TRANSAUTH)
                {
                    
                    
                    PowerDown.PDCRTrsFlg=TRANSON;
                }
          }
          else  //state err 
          {
               ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_STOP_PUMP;
               SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
              
          }
         ProcStatus[PROCESS_PUMP]=PUMP_C_AUTH; 
         
    break;
    case P_BUSY:   
          //if(PowerDown.PDCRTrsFlg==TRANSON)  //have a trans,
          if((PowerDown.PDCRTrsFlg==TRANSAUTH)||(PowerDown.PDCRTrsFlg==TRANSON))   //20120515 modifid for pump state err
          {
                 // 
                  if((P_CALL==PPreState)||(P_AUTH==PPreState))
                  {
                       ProcExchange[PROCESS_CR][PROCESS_PUMP] &= (~PUMP_AUTH);         //结束任务
                       ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_BUSY;       //told CR PUMP BUSY
                       PowerDown.PDCRTrsFlg=TRANSON;                     //20120515 modifid for pump state err
                      
                       if( ProcExchange[PROCESS_CR][PROCESS_PUMP]||ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP]
                          ||ProcExchange[PROCESS_PRINT][PROCESS_PUMP]||ProcExchange[PROCESS_VOICE][PROCESS_PUMP])  //CR 有任务，等待
                       {
                          //
                          //CR 有任务，发CR任务先
                       }
                       else//CR 无任务，要实时量
                       {
                              ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_LOAD_REAL_DATA;
                       }
                    
                   }
                   else if((P_BUSY==PPreState)&&(ProcStatus[PROCESS_PUMP]&PUMP_C_BUSY))
                   {
                       
                       if( ProcExchange[PROCESS_CR][PROCESS_PUMP]||ProcExchange[PROCESS_KEYPAD][PROCESS_PUMP]
                          ||ProcExchange[PROCESS_PRINT][PROCESS_PUMP]||ProcExchange[PROCESS_VOICE][PROCESS_PUMP])  //CR 有任务，等待
                       {
                          //
                          //CR 有任务，发CR任务先
                         
                       }
                       else  //CR 无任务，要实时量
                       {
                            ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_LOAD_REAL_DATA;
                       }
                     
                   }
                   else  //state err
                   {
                       
                        if(PowerDown.PDCRTrsFlg==TRANSON)  //have a trans,
                        {
                               // 
                        }
                        else  //state err 
                        {
                            ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_STOP_PUMP;
                            SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
                            
                        }
                   }
                   ProcStatus[PROCESS_PUMP]=PUMP_C_BUSY;
            
          }
          else  //state err 
          {
              ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_STOP_PUMP;
              SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
              
          }
           
   break;     
   case P_AEOT: 
   case P_BEOT: 

        if(PowerDown.PDCRTrsFlg==TRANSON)  //have a CR trans,not right end by EOT 
        {
              PowerDown.PDCRTrsFlg=TRANSCHECK;
              ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_EOT;                      //told CR PUMP  EOT  
 
        }
        else if(PowerDown.PDCRTrsFlg==TRANSOFF)
        {
             ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_LOAD_CUR_TRANS;         // pump on line just 
             
             SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
            
        }
        else
        {
            //
            
        }
  
        ProcStatus[PROCESS_PUMP]=PUMP_C_AEOT;             
   break;
   case P_STOP: 
        ProcStatus[PROCESS_PUMP]=PUMP_C_STOP;
   break;
   case P_ERR: 
       
        SetDispPumpMsg(D_PUMP_ERR);      
        ProcStatus[PROCESS_PUMP]=PUMP_C_ERR;
   break;
   case P_IDLE:  
        
        if(PowerDown.PDCRTrsFlg==TRANSON)  //have a CR trans,not right end by EOT 
        {
              PowerDown.PDCRTrsFlg=TRANSCHECK;
              ProcExchange[PROCESS_PUMP][PROCESS_CR] |= PUMP_EOT;                      //told CR PUMP  EOT  
        }
        else
        {
          //
            if(PowerDown.PDCRTrsFlg==TRANSAUTH)          //only send auth to pump amd not get respone
            {
                  ProcExchange[PROCESS_CR][PROCESS_PUMP] &= (~PUMP_AUTH);         //结束任务
                  ProcExchange[PROCESS_PUMP][PROCESS_CR]|=PUMP_ZERO;              //通知CR做零交易
                  
                  PowerDown.PDCRTrsFlg=TRANSOFF;      //20130201
              
                  SetDispPumpMsg(D_AUTH_ERR);
                  
            }
        }
        ProcStatus[PROCESS_PUMP]=PUMP_C_IDLE; 
     
   break;
   default: 
     
   break;
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
void PumpOnlineProcess(void)
{
    
    switch( ProcStatus[PROCESS_PUMP])
    {
        
         case P_VERIFY_PUMP:
           
              memcpy(VerifyTimeData,&Pump_Receive_BUF.bREV_Buf[6],4);   //日，时，分，秒
              ComputeVerifyData(VerifyTimeData,4,VerifyPumpData);
              
              VerifyPumpData[0]= VerifyPumpData[0]^ARMVerifyData[0];
              VerifyPumpData[1]= VerifyPumpData[1]^ARMVerifyData[1];  
              VerifyPumpData[2]= VerifyPumpData[2]^ARMVerifyData[2];
              VerifyPumpData[3]= VerifyPumpData[3]^ARMVerifyData[3];
             
              ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_VERIFY_PUMP;         // pump on line just 
          
              break;
         case P_LOAD_TIME: 
            
              if((P_AEOT==PCurState)||(P_BEOT==PCurState))
              {
                      ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_LOAD_CUR_TRANS;         // pump on line just 
                      

              }
              else if(P_BUSY==PCurState)
              {
                      ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_STOP_PUMP;         // pump on line just ,STOP pump
                    

              }
              else if(P_IDLE==PCurState)
              {
                      ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_LOAD_PUMP_TIME;         // pump on line just and check ppu
              }
              else  //err
              {
                    // pump state err
                    if(P_CALL==PCurState)
                    {
                          SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
                    }
                    else
                    {
                          SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
                    }           
              }
        break;
        case P_LOAD_COFIG: 
             
             if(P_IDLE==PCurState)
             {
                  ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_LOAD_PUMP_CONFIG;         // pump on line just and check ppu
             }
             else
             {
                  // pump state err
                  if(P_CALL==PCurState)
                  {
                        SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
                  }
                  else
                  {
                        SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
                  }
            }
        break;
        case P_PPU_CHECK: 
             if(ProcExchange[PROCESS_SCHEDULE][PROCESS_PUMP]&PRICE_UPDATE)  //ppu updata
             {
                    //Default price
             }
             else
             {
                  if(!GetGradePriceIntegrality())  //=0: 完整  ；＝其它值 ,不完整
                  {
                        PumpGetCurPrice();    
                  }
                  else
                  {
                        SetDispPumpMsg(D_CR_PPU_ERR);  //加油机错误状态
                  }                                               
              }
            
              if((P_IDLE==PCurState)&&(!GetGradePriceIntegrality()))
              {
                    ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_PPU_CHECK;         // pump on line just and check ppu
              }
              else
              {
                    // pump state err
                    if(GetGradePriceIntegrality())             
                    {
                          SetDispPumpMsg(D_CR_PPU_ERR);  //加油机错误状态
                    }
                    else if(P_CALL==PCurState)
                    {
                          SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
                    }
                    else
                    {
                          SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
                    }
              }
        break;
        case P_PPU_NEW: 
             
             if(P_IDLE==PCurState)
             {
                  ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_PPU_NEW;         // pump on line just and check ppu
             }
             else
             {
                  // pump state err
                  if(P_CALL==PCurState)
                  {
                        SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
                  }
                  else
                  {
                        SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
                  }
            }
        break;
        case P_OFFLINE_TRANS: 
            
             if(P_IDLE==PCurState)
             {
                    ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_LOAD_OFFLINE_TRANS;         // pump on line just and check ppu
             }
             else
             {
                  // pump state err
                  if(P_CALL==PCurState)
                  {
                        SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
                  }
                  else
                  {
                        SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
                  }
            }
        break;
        case P_CLR_OFFLINE_TRANS:
             if(P_IDLE==PCurState)
             {
                  ProcExchange[PROCESS_PUMP][PROCESS_PUMP] |= CR_CLEAR_OFFLINE_TRANS;         // pump on line just and check ppu
             }
             else
             {
                  // pump state err
                  if(P_CALL==PCurState)
                  {
                        SetDispPumpMsg(D_INVALID_GRADE);   //无效抬枪
                  }
                  else
                  {
                        SetDispPumpMsg(D_PUMP_STATE_ERR);  //加油机错误状态
                  }
            }  
        break;  
        default: 
       
        break;
    }
 
}

/******************************************************************************/
//function title: void AnswerPOLL(void)
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

void AnswerPOLL(void)
{
  
     
    PCurState=Pump_Receive_BUF.bREV_Buf[1];
    PCurGrade=Pump_Receive_BUF.bREV_Buf[2];
    
    if((ProcStatus[PROCESS_PUMP]&PUMP_C_ONLINE)==PUMP_C_OFFLINE)        //pump online just
    {
          PumpOnlineProcess();
          PumpTask();
    
    }
    else
    {
 
         PumpStateCheck(); 
         PumpTask();   
    }

    PPreState=PCurState;  //
    PumpGrade=PCurGrade;  //  
    
      
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

void Rev_Data_Check(void )
{
  unsigned short crc,RevCrc;
  unsigned char tmp;
  
  if( PumpRevFlg )  //have receive some datas from pump
    {           

        DataCheckFlg=1;
        tmp=Pump_Receive_BUF.bREV_Buf[Pump_Receive_BUF.bData_Len+1] ;                                                                                  
        RevCrc = tmp;
        RevCrc <<= 8;
        tmp=Pump_Receive_Data.bREV_Buf[Pump_Receive_BUF.bData_Len];
        RevCrc += tmp;
        crc=PumpCrc16(&Pump_Receive_BUF.bTarget_ID,Pump_Receive_BUF.bData_Len+5,0);  //check CRC        
        if ( crc == RevCrc )        
        {
 
             if(Pump_Receive_BUF.bTarget_ID==CR_ID)
             {
                    PumpCommFlg = 1;                 //have receive correct datas from pump
                    
                    PumpLen=0;                   //init data for next recerve
                    Clear_Pump_Timeout();        //receive pump data ,clear the time out volume
             }
             else 
             {
                  if(Pump_Receive_BUF.bSource_ID==CR_ID)
                   {
                    
                        SetDispPumpMsg(D_CR_ID_ERR);     //  cr地址重复
                        while(1)
                        {
                              WatchDog();
                        }
                   }
                
             }                
        }
        else     //200910 only for test SW   //CRC err
        {
            
            DataCheckFlg=0;
            SetDispPumpMsg(D_CR_COMM_ERR);     //  通讯错误
           
        }
      PumpRevFlg=0;    
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
void PumpProcess(void)
{  
   unsigned char CMDType;
   
   Rev_Data_Check();     
   if(PumpCommFlg== DATAREADY)    //have receive a package correct datas from the pump
   {       
       CMDType=Pump_Receive_BUF.bREV_Buf[0];        //PUMP CMD TYPE
       if(CMDType==PUMP_POLL)  //POLLING ,answer POLLING
       {
              
              if( ProcStatus[PROCESS_POS] & POS_ONLINE )
              {
                    //
              }
              else  //pos offline ,time from pump,check PUMP time
              {
                    AdjustSystime(&Pump_Receive_BUF.bREV_Buf[3], 1);
              }
             
              AnswerPOLL(); 
          
       }
       else   //PUMP CMD data ,resolve it
       {
              RevDataResolve();  //answer data check
             
       }
       PumpCommFlg=0;
       DataCheckFlg=0;
   }  
}

