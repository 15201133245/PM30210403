#include "pump.h"

unsigned char PTime[7];           //pump time
unsigned char PConfig[4];         //pump config
unsigned char PErrInfor;          //pump ErrNO
unsigned char PVersion[3];        //pump version
unsigned char PPreState;          //PUMP STATE define  
unsigned char PCurState;
unsigned char PCurGrade;   
//unsigned char TaskDoFlag;            //CMD doing flag
unsigned char PumpCMDTye;            //CMD type
unsigned char PumpGrade;             //PUMP grade define
unsigned char CRstate;               //CR state
unsigned char CRTrsFlg=0;              //=0 idle ,=1trans on

unsigned char PumpCurTask;          //pump current task
unsigned char PGrade;               //pump grade
unsigned char PGradeMax;            //pump max grade 
unsigned char PumpCommFlg =0;           //pump have receive correct data
unsigned char PumpRevFlg = 0;        //have receive a package data
unsigned char  DataCheckFlg=0 ;        //checking data 
unsigned char Pump_ID;
unsigned char CR_ID;                   //CR ID 
PUMP_SEND_CMD    Pump_Send_BUF;        //send data structure
PUMP_REV_CMD     Pump_Receive_BUF;     //receive data structure
PUMP_REV_CMD     Pump_Receive_Data;    //receive data structure

unsigned short int PumpCOMTimeOut;
unsigned int PumpSendTimeOut;

unsigned char VerifyPumpData[4];
unsigned char VerifyTimeData[4];
//down added for over fueling process - 20110906
unsigned char OverFuelFlg=0;   
unsigned char OverFuelTimes=0;     
unsigned int  OverFuelValue=0;
unsigned int  OverFuelToTValue=0;
unsigned int  OverFuelMoney=0;
unsigned int  OverPresetMoney=0;
unsigned int  OverPresetValue=0;
//unsigned int  OverPresetMode=0;
unsigned int  OverFuelTransFlg=0;

unsigned char heZeroVolumeFlag=0;

//up added for over fueling process - 20110906
unsigned char ZeroVolumeFlag=0;       //20130201
unsigned char TempGrade=0x01;       //20130601