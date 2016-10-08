#include "pump.h"
#include "global.h"



/******************************************************************************/
//function title: void InitPumpProcess(void)
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
void InitPumpProcess(void)

{
     //init data
    Pump_CR_Buf.PresetMode=0x01;           //money  preset,      // 0:money 1:volume  //only for test 
   
    
    PumpPriceBuf[0].NextPrice=555;      //only for test  //default value
    PumpPriceBuf[1].NextPrice=555;      //only for test  
    PumpPriceBuf[2].NextPrice=555;      //only for test  
    PumpPriceBuf[3].NextPrice=555;      //only for test  
    
    CR_ID=Rs485Adr+1;                  // Rs485Adr=0,1,2,3  ,CR_ID=1,2,3,4
    Pump_ID=0x00;
    PGradeMax=1;                         //default value
                        
    PPreState=P_IDLE;                    //default value
    PCurState=P_IDLE;  
    CRstate=0x00;
    PGrade=1;    
   
    
    ProcStatus[PROCESS_PUMP]=P_VERIFY_PUMP ;            // pump online just,init pump state as power on process  
    ProcExchange[PROCESS_PUMP][PROCESS_PUMP]=0; 
    ProcExchange[PROCESS_CR][PROCESS_PUMP]=0;
    ProcExchange[PROCESS_SCHEDULE][PROCESS_PUMP]=0;
    ProcExchange[PROCESS_PRINT][PROCESS_PUMP]=0;
    
    
    Clear_Pump_Timeout();
    Serial0.receviceback = PumpCMD_Recevice;   //pump comm init    //200910 steven
    pTimer1=Pump_Time;
}
