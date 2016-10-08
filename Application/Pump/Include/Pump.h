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

#define PUMP_POLL             0x30         // ��ͨ��ѯ
#define PRE_AUTH              0x31         //CR ��Ȩ����
#define GET_REAL_DATA         0x32         //CR Ҫʵʱ��(����Ϣ),PUMP �Է���	
#define GET_TRANS             0x33         //IC������PUMP�����ϴν�������
#define GET_TOT               0x34         //CR ��ȡ ALL GRADE TOT
#define PPU_NEW               0x35         //PPU NEW
#define PPU_CHECK             0x36         //PPU CHECK  ,PUMP ���� 
#define GET_OFFLINE_TRANS     0x37         //CR ��ȡ���߽���,����IC�����ɷǿ���¼
#define CLEAR_OFFLINE_TRANS   0x38         //CR ��ȡ���߽���
#define STOP_PUMP             0x39         //CR ֹͣ���ͻ�����
#define CHECK_TIME            0x3A         //CR ��ȡʱ������
#define UPLOAD_CUR_TRANS      0x3B         //CR ��ȡ��ǰ����

#define PUMP_IDENTIFY         0x3C         //PUNP identify

#define UPLOAD_CONFIG         0x41         //CR ��ȡ PUMP config
#define UPLOAD_ERR_NO         0x42         //��ȡ�ͻ�������Ϣ����
#define UPLOAD_VERSION        0x43         //��ȡ�ͻ��汾������
#define RESTART_PUMP          0x44         //������������
#define PUMP_ROUNDING         0x45         //�����������Ԫ���������������
#define VOLUME_MODIFY         0x61         //��������
#define VOICE_DATA            0x62         //����������
#define PRINT_DATA            0x63         //����ӡ����
#define TRANSMIT_DATA         0x80         //����ת������

#define VERIFY_PUMP           0x90         //�ͻ���֤

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

//PUMP STATE:	ProcStatus[PUMP]  //��ѭ��������������
//POWER ON STATE
#define P_LOAD_TIME               0x00000001       //Ҫ���ͻ�ʱ��
#define P_LOAD_COFIG              0x00000002       //Ҫ���ͻ�����
#define P_PPU_CHECK               0x00000004       //�ͼ۲�ѯ
#define P_PPU_NEW	          0x00000008       //�ͼ۸���
#define P_OFFLINE_TRANS           0x00000010       //Ҫ�ѻ�����
#define P_CLR_OFFLINE_TRANS       0x00000020       //���ѻ����ױ�־
#define P_UPLOAD_CUR_TRANS        0x00000040       //Ҫ��ǰ����
#define  P_VERIFY_PUMP  0x00000080                 //���ͻ���֤

//PUMP STATE   ��PUMPONLING ��־��ֱ�Ӹ�ֵ
#define PUMP_C_IDLE            0x10000100
#define PUMP_C_CALL            0x10000200
#define PUMP_C_AUTH            0x10000400
#define PUMP_C_BUSY            0x10000800
#define PUMP_C_AEOT            0x10001000
#define PUMP_C_BEOT            0x10002000
#define PUMP_C_STOP            0x10004000
#define PUMP_C_ERR	       0x10008000	//���ͻ����ڴ���������ʾ�������  0:no ERR 1: has ERR
#define PUMP_C_ONLINE	       0x10000000   
#define PUMP_C_OFFLINE	       0x00000000


//pump task 
// pump & pump
//ProcExchange[PROCESS_PUMP][PROCESS_PUMP]	

#define CR_NO_TASK	        0x00000000	//CR û�����񣬴��ڿ���̬
#define CR_LOAD_PUMP_TIME       0x00000001      //��ȡʱ������
#define CR_LOAD_PUMP_CONFIG	0x00000002	//CR LOAD PUMP TYPE
#define CR_PPU_CHECK 	        0x00000004	//PPU CHECK  ,PUMP ����
#define CR_PPU_NEW	        0x00000008	//PPU NEW
#define CR_LOAD_OFFLINE_TRANS	0x00000010	//CRҪ���߽���,����IC�����ɷǿ���¼
#define CR_CLEAR_OFFLINE_TRANS  0x00000020      //���ѻ�����
#define CR_LOAD_REAL_DATA       0x00000040	//CR Ҫʵʱ��(����Ϣ),PUMP �Է���
#define CR_STOP_PUMP	        0x00000080	//CR ֹͣ���ͻ�����
#define CR_LOAD_PUMP_ERR_NO	0x00000100	//��ȡ�ͻ�������Ϣ����
#define CR_LOAD_CUR_TRANS       0x00000200       //Ҫ��ǰ����

#define CR_VERIFY_PUMP          0x00000400       //���ͻ���֤





#define PUMP_PPU_NEW	                0x00001000	//PPU NEW
#define PUMP_LOAD_ERR_NO	        0x00002000	//��ȡ�ͻ�������Ϣ����
#define CR_LOAD_PUMP_VER	        0x00004000	//��ȡ�ͻ��汾������

//#define CR_RESTART_PUMP  	        0x00008000	//������������
//#define CR_PUMP_ROUNDING	        0x00010000	//Ԫ������ ����


//pump task 
// pump & pump
//ProcExchange[PROCESS_VOICE][PROCESS_PUMP]	
#define CR_VOICE_VOLUME_ADJUST	        0x00000001	//������������
#define CR_VOICE_DATA                   0x00000002      //����������



//pump task 
// pump & Print
//ProcExchange[PROCESS_PRINT][PROCESS_PUMP]	
#define CR_PRINT_DATA           0x00000001      //����ӡ����
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
#define D_PPU_New             1   //�ͼ۸�����
#define D_AUTH                2   //������Ȩ��
#define D_FUEL                3   //������

#define D_PUMP_OFF_LINE       101           //���ͻ�����
#define D_PUMP_ERR            102           //���ͻ�����
#define D_INVALID_GRADE       103           //��Ч̧ǹ
#define D_PRESET_ERR          104           //Ԥ��ֵ����
#define D_PPU_NEW_ERR         105          //˰�ؾܸ��ͼ�
#define D_AUTH_ERR            106          //��Ȩʧ��
#define D_GET_TRANS_ERR       107          //ȡ�ͻ�����ʧ��
#define D_GRADE_ERR           108          //ǹ�Ŵ���
#define D_GET_OFFTRANS_ERR    109      //ȡ�ѻ�����ʧ��
#define D_GET_TIME_ERR        110          //ȡ���ͻ�ʱ��ʧ��
#define D_PPU_ERR             111          //���ͻ��ͼ۲���
#define D_PUMP_SET_ERR        112          //���ͻ����ò���
#define D_PUMP_STATE_ERR      113         //���ͻ�����״̬����
#define D_CR_ID_ERR           114         //CR��ַ�ظ�

#define D_CR_COMM_ERR         115        //ͨѶ����
#define D_CR_PPU_ERR          116        //�ͼ۱����

#define D_VERIFY_PUMP_ERR     117        //���ͻ���֤ʧ��
//#define D_VERIFY_PUMP_ERR     102        //���ͻ���֤ʧ��


#define D_PUMP_ERR1      201        //���ͻ�δ��⵽���巢����
#define D_PUMP_ERR2      202        //EMT������
#define D_PUMP_ERR3      203        //���ͻ���Ӳ������λ
#define D_PUMP_ERR4      204        //�ϵ��⵽��ǹδ��
#define D_PUMP_ERR5      205        //Ԥ�ù���
#define D_PUMP_ERR6      206        //Ԥ�ù�С
#define D_PUMP_ERR7      207        //���ͽ���δ��ǹ
#define D_PUMP_ERR8      208        //������˰��ͨ��Υ��



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
  unsigned char VolumeTotal[4][4];	     //MSB	���ۼ�
  unsigned char AmountTotal[4][4];	     //MSB	����ۼ�
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