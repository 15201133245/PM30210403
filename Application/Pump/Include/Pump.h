#ifndef _PUMP_H
#define _PUMP_H
#include "global.h"
#include "define.h"
#include "DispPumpMsg.h"
#include "Verify.h"


//PUMP COMM LINE STATE
#define PUMP_OFF_LINE	  0x00      // PUMP_OFFLINE
#define PUMP_ON_LINE	  0x01       //PUMP_ONLINE
#define money             0x01
#define litre             0x02
#define GRADEMAX           4

#define TRANSOFF          0x00
#define TRANSAUTH         0x01
#define TRANSON           0x02
#define TRANSCHECK        0X03




#define DATAREADY         0x01


extern unsigned char PTime[7];         //pump time
extern unsigned char PConfig[4];       //pump config
extern unsigned char PErrInfor;        //pump ErrNO
extern unsigned char PVersion[3];      //pump version
#define P_IDLE  0x06
#define P_CALL  0x07
#define P_AUTH  0x08
#define P_BUSY  0x09
#define P_AEOT  0x0A
#define P_BEOT  0x0B
#define P_STOP  0x0C
//#define P_ERR   0x0E
#define P_ERR   0x00

extern unsigned char PPreState;           //PUMP state
extern unsigned char PCurState;
extern unsigned char PCurGrade;   
static unsigned char PumpGrade=0;        //PUMP grade ,=0,no grade
extern unsigned char CRstate;
extern unsigned char CRTrsFlg;   //=0 idle ,=1trans on


//PUMP TASK //CMD type

#define PUMP_POLL             0x30         // 普通查询
#define PRE_AUTH              0x31         //CR 授权加油
#define GET_REAL_DATA         0x32         //CR 要实时量(卡信息),PUMP 自发起	
#define GET_TRANS             0x33         //IC卡请求PUMP发送上次交易数据
#define GET_TOT               0x34         //CR 索取 ALL GRADE TOT
#define PPU_NEW               0x35         //PPU NEW
#define PPU_CHECK             0x36         //PPU CHECK  ,PUMP 发起 
#define GET_OFFLINE_TRANS     0x37         //CR 索取离线交易,申请IC卡生成非卡记录
#define CLEAR_OFFLINE_TRANS   0x38         //CR 清取离线交易
#define STOP_PUMP             0x39         //CR 停止加油机加油
#define CHECK_TIME            0x3A         //CR 索取时钟命令
#define UPLOAD_CUR_TRANS      0x3B         //CR 索取当前交易

#define PUMP_IDENTIFY         0x3C         //PUNP identify

#define UPLOAD_CONFIG         0x41         //CR 索取 PUMP config
#define UPLOAD_ERR_NO         0x42         //索取油机报警信息命令
#define UPLOAD_VERSION        0x43         //索取油机版本号命令
#define RESTART_PUMP          0x44         //继续加油命令
#define PUMP_ROUNDING         0x45         //凑整命令包含元整命令和升整命令
#define VOLUME_MODIFY         0x61         //音量调整
#define VOICE_DATA            0x62         //发语音播报
#define PRINT_DATA            0x63         //发打印数据
#define TRANSMIT_DATA         0x80         //数据转发命令

#define VERIFY_PUMP           0x90         //油机认证

#define PUMP_PPU_NEW_OK         0x00 
#define PUMP_PPU_NEW_DOING      0x01 
#define P_Task_doing            0x01
#define P_Task_end              0x00 
#define Load_Trans              0x01
#define Load_REC                0X02

extern unsigned char PumpCMDTye;            //pump tast //CMD type
//extern unsigned char TaskDoFlag;            //CMD doing flag  =1,have a task =0 ,no task.
extern unsigned char PumpCurTask;            //pump current task
extern unsigned char PGrade;               //current pump grade
extern unsigned char PGradeMax;            //the pump real grade max ,load from config CMD

//PUMP STATE:	ProcStatus[PUMP]  //自循环，自立，自清
//POWER ON STATE
#define P_LOAD_TIME               0x00000001       //要加油机时间
#define P_LOAD_COFIG              0x00000002       //要加油机配置
#define P_PPU_CHECK               0x00000004       //油价查询
#define P_PPU_NEW	          0x00000008       //油价更新
#define P_OFFLINE_TRANS           0x00000010       //要脱机交易
#define P_CLR_OFFLINE_TRANS       0x00000020       //清脱机交易标志
#define P_UPLOAD_CUR_TRANS        0x00000040       //要当前交易
#define  P_VERIFY_PUMP  0x00000080                 //加油机认证

//PUMP STATE   带PUMPONLING 标志，直接赋值
#define PUMP_C_IDLE            0x10000100
#define PUMP_C_CALL            0x10000200
#define PUMP_C_AUTH            0x10000400
#define PUMP_C_BUSY            0x10000800
#define PUMP_C_AEOT            0x10001000
#define PUMP_C_BEOT            0x10002000
#define PUMP_C_STOP            0x10004000
#define PUMP_C_ERR	       0x10008000	//加油机存在错误申请显示错误代码  0:no ERR 1: has ERR
#define PUMP_C_ONLINE	       0x10000000   
#define PUMP_C_OFFLINE	       0x00000000


//pump task 
// pump & pump
//ProcExchange[PROCESS_PUMP][PROCESS_PUMP]	

#define CR_NO_TASK	        0x00000000	//CR 没有任务，处于空闲态
#define CR_LOAD_PUMP_TIME       0x00000001      //索取时钟命令
#define CR_LOAD_PUMP_CONFIG	0x00000002	//CR LOAD PUMP TYPE
#define CR_PPU_CHECK 	        0x00000004	//PPU CHECK  ,PUMP 发起
#define CR_PPU_NEW	        0x00000008	//PPU NEW
#define CR_LOAD_OFFLINE_TRANS	0x00000010	//CR要离线交易,申请IC卡生成非卡记录
#define CR_CLEAR_OFFLINE_TRANS  0x00000020      //清脱机交易
#define CR_LOAD_REAL_DATA       0x00000040	//CR 要实时量(卡信息),PUMP 自发起
#define CR_STOP_PUMP	        0x00000080	//CR 停止加油机加油
#define CR_LOAD_PUMP_ERR_NO	0x00000100	//索取油机报警信息命令
#define CR_LOAD_CUR_TRANS       0x00000200       //要当前交易

#define CR_VERIFY_PUMP          0x00000400       //加油机认证





#define PUMP_PPU_NEW	                0x00001000	//PPU NEW
#define PUMP_LOAD_ERR_NO	        0x00002000	//索取油机报警信息命令
#define CR_LOAD_PUMP_VER	        0x00004000	//索取油机版本号命令

//#define CR_RESTART_PUMP  	        0x00008000	//继续加油命令
//#define CR_PUMP_ROUNDING	        0x00010000	//元整命令 凑整


//pump task 
// pump & pump
//ProcExchange[PROCESS_VOICE][PROCESS_PUMP]	
#define CR_VOICE_VOLUME_ADJUST	        0x00000001	//语音调节命令
#define CR_VOICE_DATA                   0x00000002      //发语音播报



//pump task 
// pump & Print
//ProcExchange[PROCESS_PRINT][PROCESS_PUMP]	
#define CR_PRINT_DATA           0x00000001      //发打印数据
#define CR_PRINT_RFU01	        0x00000002	

#define VOLUMEDATA     0x61
#define VOICEDATA      0x62
#define PRINTDATA      0x63



#define TAX_PPU_NEW_ERR   0x53
#define CRC_ERR           0x81
#define STATE_ERR         0x82
#define GRADE_ERR         0x83
#define PRESET_ERR        0x84
#define LOAD_CUR_TRANS_ERR   0x86



//ERR_MSG_ID=PUMP_ERR_MSG_ID

//LCD display  message
//0--100  state message
//100-- > err message
#define D_PPU_New             1   //油价更改中
#define D_AUTH                2   //加油授权中
#define D_FUEL                3   //加油中

#define D_PUMP_OFF_LINE       101           //加油机离线
#define D_PUMP_ERR            102           //加油机故障
#define D_INVALID_GRADE       103           //无效抬枪
#define D_PRESET_ERR          104           //预置值超限
#define D_PPU_NEW_ERR         105          //税控拒改油价
#define D_AUTH_ERR            106          //授权失败
#define D_GET_TRANS_ERR       107          //取油机交易失败
#define D_GRADE_ERR           108          //枪号错误
#define D_GET_OFFTRANS_ERR    109      //取脱机交易失败
#define D_GET_TIME_ERR        110          //取加油机时间失败
#define D_PPU_ERR             111          //加油机油价不符
#define D_PUMP_SET_ERR        112          //加油机配置不符
#define D_PUMP_STATE_ERR      113         //加油机命令状态不符
#define D_CR_ID_ERR           114         //CR地址重复

#define D_CR_COMM_ERR         115        //通讯错误
#define D_CR_PPU_ERR          116        //油价表错误

#define D_VERIFY_PUMP_ERR     117        //加油机认证失败
//#define D_VERIFY_PUMP_ERR     102        //加油机认证失败


#define D_PUMP_ERR1      201        //加油机未检测到脉冲发生器
#define D_PUMP_ERR2      202        //EMT检测错误
#define D_PUMP_ERR3      203        //加油机做硬件主复位
#define D_PUMP_ERR4      204        //上电检测到有枪未挂
#define D_PUMP_ERR5      205        //预置过大
#define D_PUMP_ERR6      206        //预置过小
#define D_PUMP_ERR7      207        //加油结束未挂枪
#define D_PUMP_ERR8      208        //计量与税控通信违法



static unsigned char VerifyPumpData[4];
static unsigned char VerifyTimeData[4];


//PUMP communication variable define
static unsigned short PumpLen = 0;
static unsigned char  PumpTmp = 0;
static unsigned char  PumpDataLen = 0;
static unsigned short PumpCrc = 0;
static unsigned char  PumpOnlineflg=0;
static unsigned char  PosOnlineflg=0;       //for test ,may be set by POS process 0 :offline   1: online
extern unsigned char  PumpCommFlg ;             //have receive a  package success data
extern unsigned char  PumpRevFlg ;          //have receive a package data 
extern unsigned char  DataCheckFlg ;        //checking data 

extern unsigned char  Pump_ID;             //PUMP ID
extern unsigned char  CR_ID;               //CR ID 

extern unsigned char ZeroVolumeFlag;             //20130201 steven,  zero trans deal with,added for fixing the pump SW bug when power down in fueling ,the pump can not save the correct tr
extern unsigned char TempGrade;                   //20130601

typedef struct _PUMP_SEND_CMD
{
  unsigned char  bHead_Flg;
  unsigned char  bTarget_ID;
  unsigned char  bSource_ID;
  unsigned char  bControl_Flg1;
  unsigned char  bControl_Flg2; 
  unsigned char  bData_Len;
  unsigned char  bSend_Buf[200];
 
  
}PUMP_SEND_CMD;

 
typedef struct _PUMP_REV_CMD
{
  unsigned char  bHead_Flg;
  unsigned char  bTarget_ID;
  unsigned char  bSource_ID;
  unsigned char  bControl_Flg1;
  unsigned char  bControl_Flg2; 
  unsigned char  bData_Len;
  unsigned char  bREV_Buf[200];
 
}PUMP_REV_CMD;


extern PUMP_SEND_CMD    Pump_Send_BUF;       //send data structure
extern PUMP_REV_CMD     Pump_Receive_BUF;    //receive data structure
extern PUMP_REV_CMD     Pump_Receive_Data;    //receive data structure

typedef struct PUMP_POS		
{		
  unsigned char VolumeTotal[4][4];	     //MSB	升累计
  unsigned char AmountTotal[4][4];	     //MSB	金额累计
}PUMP_POS;		

extern unsigned short int PumpCOMTimeOut;
extern unsigned int PumpSendTimeOut;
void Send_Data_PUMP(void);
void PumpCMD_Recevice(unsigned char *s,unsigned int Rlen, unsigned char val);
void PumpProcess(void);
void InitPumpProcess(void);
void SetPumpOffline(void);
void SetPumpOnlineflg(void);
unsigned char ReadPumpOnline(void);
void Pump_Time(void);                                //100ms
void Clear_Pump_Timeout(void);
void PumpStateCheck(void);
void Rev_Data_Check(void );
extern unsigned short PumpCrc16(unsigned char *s,unsigned short len,unsigned short LastCrc);


#endif 