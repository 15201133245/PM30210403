/************************************************************************************
**文件名称：Arm70_POS.h
**时间：2009.06.23
**编写人：李立国
**Verson：00.00.01
**说明：POS功能模块宏定义
**更改原因：
**更改时间：
**修改人：
**************************************************************************************
*/


#include "global.h"
#include "Arm70_publicfun.h"
#include "PosPublic.h"
#define SEG_STEP 10

//POS通讯命令 具体含义参加《中石化POS协议》
// **PC  代表中石化后台
// **POS 代表 加油机程序POS更能模块
#define  ARM70_PC_QUE				0x30	                        //PC普通查询
#define  ARM70_POS_RTOS				0x31	                        //油机实时信息
#define  ARM70_POS_STRIKE			0x32	                        //POS发送成交数据
#define  ARM70_PC_STRIKE_ACK			0x32	                        //PC回应成交数据
#define  ARM70_POS_APPLY_DATA			0x33				//POS申请下载数据
#define  ARM70_PC_DATA_LENG			0x33				//PC发送需要下载数据总长度
#define  ARM70_POS_APPLY_DATACON	        0x34				//POS申请下载数据内容
#define  ARM70_PC_APPLY_DATACON		        0x34				//PC发送下载数据
#define  ARM70_POS_RELEASE_ASH		        0x35				//POS申请解灰
#define  ARM70_PC_QUE_ASHCARD			0x35				//PC发送灰卡查询结果
#define  ARM70_POS_QUE_BWCARD			0x36				//POS申请查询黑/白名单
#define  ARM70_PC_QUE_BWCARD			0x36				//PC发送黑白名单查询结果
#define  ARM70_PC_READ_WMCOIL			0x38				//PC读取加油机累计数据
#define  ARM70_POS_OIL_ACK			0x38				//POS应答累计数据
#define  ARM70_PC_READ_WMCMESS		        0x3a				//PC读取加油机信息
#define  ARM70_POS_MESS_ACK			0x3a				//POS应答信息
#define  ARM70_POS_ERROR			0x3b				//POS发送内部出错
#define  ARM70_PC_ERROR_ACK			0x3c				//PC应答
#define  ARM70_PC_READ_STRIKE			0x3e				//PC读取记录
#define  ARM70_POS_NO_STRICKE			0x3f				//POS无相应记录




//POS 本身所处的状态

/*ProcExchange[PROCESS_POS][PROCESS_POS]  4 个字节 32位  0xff ff ff ff 
// 00000000  00000000  00000000 00000000
//从右到左  1-32 位
//  1 查黑名单  2 查白名单 3 查灰名单 4数据下载数据长度（名单、油价、油站信息的下载）
//  5 下载数据内容 6 加油机主动上传交易记录 7 PC 按TTC要记录 8读累积
// 第一个字节 用做读写权限
// 第二个字节 用作状态 优先级  
// 第 3，4字节 用作 记录POS 本身的状态
*/
#define  RESULT_TRUE                 1

#define  RESULT_FALSE                0

#define  ARM70_POS_STATE_IDLE   	0         
#define  ARM70_POS_STATE_BLACK		1        
#define  ARM70_POS_STATE_WHITE		2         
#define  ARM70_POS_STATE_GERY   	3        
#define  ARM70_POS_DOWN_LIST		4       
#define  ARM70_POS_DOWNWAIT_DATA 	5        
#define  ARM70_POS_RECORD_FIRST         6 
#define  ARM70_POS_RECORD_SCEND         7
#define  ARM70_PC_READ_ACCOUNT          8
//


//extern void AdjustSystime(unsigned char *s);


extern unsigned char Arm_g_posbuff[255];
extern unsigned char Arm_g_posonline;       //PC pos是否在线 1 在线 0 不在线
//PosQueFlg;			//POS回叫交易数据标志。0：无，1：回叫；2：等待CR应答交易数据


//POS后台查询结构
struct _CrQue
{
	unsigned char flg;			//标志，0：无数据，1有查询任务，2：等待后台应答,
	unsigned char type;			//类型，0：白名单，1：黑名单，2：后台查询灰记录,3：本地查询灰记录
	unsigned char Asn[10];		//卡号
	unsigned char Bal[4];		//余额
	unsigned char Ctc[2];		//CTC
	unsigned char Ds;	 		//扣款来源
	unsigned char Time[7];		//时间
};
//POS向PC下载数据长度
struct _CrDataLeng
{
	unsigned char flg;			//标志，0：无数据，1：收到CR数据，2：等待后台应答，
	unsigned char type;			//类型，0：基础黑名单，1：新增黑名单版本号，2：新删黑名单，3：新增黑名单，
						//4：油品油价表，5：油站通用信息
	unsigned short Ver;
};



//CR向后台下载数据
typedef struct _CrData
{
	unsigned char flg;			//标志，0：无数据，1：收到CR数据，2等待后台应答，
	unsigned char type;			//类型，0：基础黑名单，1：新增黑名单版本号，2：新删黑名单，3：新增黑名单，
	                                        //4：油品油价表，5：油站通用信息
        
        	
	unsigned short Seg_Offset;	//段偏移
	unsigned char Seg;		//段数
        unsigned short 	SegTotal;	//总段数
}st_Crdata;

extern 	SinopecRev SinopecRevBuf;
extern 	unsigned char Arm_g_posbuff[];
extern 	unsigned char Arm_g_posonline;
extern 	PcSeg PosSeg;
extern	unsigned char PosSndBuf[];
extern	unsigned short PosCastLeng;
extern	unsigned char DownLoadStatus;
extern	Tmr *DownLoadTmr;
extern	Tmr *SndTransTmr;
extern	unsigned char PosReadRecordFlg;
extern	unsigned char SendCountToPos;
extern	unsigned char DownLoadPriceFlg;
extern	unsigned char PosQueListCount;
extern	unsigned char RecordLast[];
extern  unsigned char limit_flg;

extern	const unsigned char WriteDataFlashErr[];


void  Arm70_POS_main();
//extern st_Crdata  Pos_PCDowndata;
/**************************************************************************** 
**函数名称：PosMessage_Polling(void)
** 说明: 上送加油实时信息
** 输入: 无
** 输出:
** 返回值: 
** 作者：BILL li
** 时间：2009-04-24
** 变更原因：
** Version :00.00.01
*****************************************************************************
**/
//_CrQue			xdata CrQue  全局变量
void PosMessage_Polling(void);

/**************************************************************************** 
**函数名称：PosMessage_Trade(void)
** 说明: PC 机回应上送结果 
** 输入: 
** 输出:
** 返回值: 
** 作者：BILL li
** 时间：2009-05-29
** 变更原因：
** Version :00.00.01
*****************************************************************************
**/
//void PosMessage_Trade(void);
void PosMessage_ReadPOSTrade(void);
/**************************************************************************** 
**函数名称：PosMessage_DataLeng(void)
** 说明: PC 机回应上送结果 
** 输入: 
** 输出:
** 返回值: 
** 作者：BILL li
** 时间：2009-05-29
** 变更原因：
** Version :00.00.01
*****************************************************************************
**/
void PosMessage_DataLeng(void);

void PosMessage_Data(void);

void PosMessage_GrayRecord(void);

void PosMessage_BWL(void);

void PosMessage_AskTotal(void);


void PosMessage_AskInfo(void);

void PosMessage_ReadTrade();

void ReqPosQueBWL(void);

void ReqDownloadList(unsigned char type);


int Check_listVersion(unsigned char *s);

void SndPosLastTrade(void);

void SndPosTrade(void);

void ReqPosDataLeng(void);
void ReqPosData(void);


//unsigned int BYTE2DOWRD(unsigned char *bsource,int Bytelen);

void insert_testrecord(void);
void ReqPosQueGreyList(void);
void PosMessage_Trade_TTC(void);
//unsigned char * DEC_To_ZBC(unsigned short len);

void InitPosProcess(void);
