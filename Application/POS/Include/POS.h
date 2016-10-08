/************************************************************************************
**�ļ����ƣ�Arm70_POS.h
**ʱ�䣺2009.06.23
**��д�ˣ�������
**Verson��00.00.01
**˵����POS����ģ��궨��
**����ԭ��
**����ʱ�䣺
**�޸��ˣ�
**************************************************************************************
*/


#include "global.h"
#include "Arm70_publicfun.h"
#include "PosPublic.h"
#define SEG_STEP 10

//POSͨѶ���� ���庬��μӡ���ʯ��POSЭ�顷
// **PC  ������ʯ����̨
// **POS ���� ���ͻ�����POS����ģ��
#define  ARM70_PC_QUE				0x30	                        //PC��ͨ��ѯ
#define  ARM70_POS_RTOS				0x31	                        //�ͻ�ʵʱ��Ϣ
#define  ARM70_POS_STRIKE			0x32	                        //POS���ͳɽ�����
#define  ARM70_PC_STRIKE_ACK			0x32	                        //PC��Ӧ�ɽ�����
#define  ARM70_POS_APPLY_DATA			0x33				//POS������������
#define  ARM70_PC_DATA_LENG			0x33				//PC������Ҫ���������ܳ���
#define  ARM70_POS_APPLY_DATACON	        0x34				//POS����������������
#define  ARM70_PC_APPLY_DATACON		        0x34				//PC������������
#define  ARM70_POS_RELEASE_ASH		        0x35				//POS������
#define  ARM70_PC_QUE_ASHCARD			0x35				//PC���ͻҿ���ѯ���
#define  ARM70_POS_QUE_BWCARD			0x36				//POS�����ѯ��/������
#define  ARM70_PC_QUE_BWCARD			0x36				//PC���ͺڰ�������ѯ���
#define  ARM70_PC_READ_WMCOIL			0x38				//PC��ȡ���ͻ��ۼ�����
#define  ARM70_POS_OIL_ACK			0x38				//POSӦ���ۼ�����
#define  ARM70_PC_READ_WMCMESS		        0x3a				//PC��ȡ���ͻ���Ϣ
#define  ARM70_POS_MESS_ACK			0x3a				//POSӦ����Ϣ
#define  ARM70_POS_ERROR			0x3b				//POS�����ڲ�����
#define  ARM70_PC_ERROR_ACK			0x3c				//PCӦ��
#define  ARM70_PC_READ_STRIKE			0x3e				//PC��ȡ��¼
#define  ARM70_POS_NO_STRICKE			0x3f				//POS����Ӧ��¼




//POS ����������״̬

/*ProcExchange[PROCESS_POS][PROCESS_POS]  4 ���ֽ� 32λ  0xff ff ff ff 
// 00000000  00000000  00000000 00000000
//���ҵ���  1-32 λ
//  1 �������  2 ������� 3 ������� 4�����������ݳ��ȣ��������ͼۡ���վ��Ϣ�����أ�
//  5 ������������ 6 ���ͻ������ϴ����׼�¼ 7 PC ��TTCҪ��¼ 8���ۻ�
// ��һ���ֽ� ������дȨ��
// �ڶ����ֽ� ����״̬ ���ȼ�  
// �� 3��4�ֽ� ���� ��¼POS �����״̬
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
extern unsigned char Arm_g_posonline;       //PC pos�Ƿ����� 1 ���� 0 ������
//PosQueFlg;			//POS�ؽн������ݱ�־��0���ޣ�1���ؽУ�2���ȴ�CRӦ��������


//POS��̨��ѯ�ṹ
struct _CrQue
{
	unsigned char flg;			//��־��0�������ݣ�1�в�ѯ����2���ȴ���̨Ӧ��,
	unsigned char type;			//���ͣ�0����������1����������2����̨��ѯ�Ҽ�¼,3�����ز�ѯ�Ҽ�¼
	unsigned char Asn[10];		//����
	unsigned char Bal[4];		//���
	unsigned char Ctc[2];		//CTC
	unsigned char Ds;	 		//�ۿ���Դ
	unsigned char Time[7];		//ʱ��
};
//POS��PC�������ݳ���
struct _CrDataLeng
{
	unsigned char flg;			//��־��0�������ݣ�1���յ�CR���ݣ�2���ȴ���̨Ӧ��
	unsigned char type;			//���ͣ�0��������������1�������������汾�ţ�2����ɾ��������3��������������
						//4����Ʒ�ͼ۱�5����վͨ����Ϣ
	unsigned short Ver;
};



//CR���̨��������
typedef struct _CrData
{
	unsigned char flg;			//��־��0�������ݣ�1���յ�CR���ݣ�2�ȴ���̨Ӧ��
	unsigned char type;			//���ͣ�0��������������1�������������汾�ţ�2����ɾ��������3��������������
	                                        //4����Ʒ�ͼ۱�5����վͨ����Ϣ
        
        	
	unsigned short Seg_Offset;	//��ƫ��
	unsigned char Seg;		//����
        unsigned short 	SegTotal;	//�ܶ���
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
**�������ƣ�PosMessage_Polling(void)
** ˵��: ���ͼ���ʵʱ��Ϣ
** ����: ��
** ���:
** ����ֵ: 
** ���ߣ�BILL li
** ʱ�䣺2009-04-24
** ���ԭ��
** Version :00.00.01
*****************************************************************************
**/
//_CrQue			xdata CrQue  ȫ�ֱ���
void PosMessage_Polling(void);

/**************************************************************************** 
**�������ƣ�PosMessage_Trade(void)
** ˵��: PC ����Ӧ���ͽ�� 
** ����: 
** ���:
** ����ֵ: 
** ���ߣ�BILL li
** ʱ�䣺2009-05-29
** ���ԭ��
** Version :00.00.01
*****************************************************************************
**/
//void PosMessage_Trade(void);
void PosMessage_ReadPOSTrade(void);
/**************************************************************************** 
**�������ƣ�PosMessage_DataLeng(void)
** ˵��: PC ����Ӧ���ͽ�� 
** ����: 
** ���:
** ����ֵ: 
** ���ߣ�BILL li
** ʱ�䣺2009-05-29
** ���ԭ��
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
