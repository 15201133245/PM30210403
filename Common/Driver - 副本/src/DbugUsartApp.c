#include "global.h"

#define DBUG_TIME		2

#define MaxFunctions	9

void DbugReceviceCommand1(unsigned char *s,unsigned int len,unsigned char error);
void DbugReceviceForamt(unsigned char *s,unsigned int len,unsigned char error);
void DbugReceviceLoading(unsigned char *s,unsigned int len,unsigned char error);

extern	PrintBuf printStruct;

const char start_dbug[] = "start dbug";
const char exit_dbug[] = "exit dbug";
const char loading[] = "loading";
const char formating[] = "format";
const char ticket_head[] = "ticket head";
const char g_code[] = "g_code";
#ifdef NEW_VOICE
#define	VOICE_FLG_IDLE				0
#define VOICE_FLG_DOWN_C			1
#define VOICE_FLG_R_DOWN_C_OK		2
#define VOICE_FLG_R_LENGTH			3
#define VOICE_FLG_R_LENGTH_OK		4
#define VOICE_FLG_R_LIST			5
#define VOICE_FLG_R_LIST_OK			6
#define VOCIE_FLG_R_GLIST			7
#define VOICE_FLG_R_GLIST_OK		8
#define VOICE_LIST_COMPLETE			9
#define VOICE_FLG_S_LENGTH_WAIT		10
#define VOICE_FLG_WAIT_LENGTH		11
#define VOICE_FLG_S_VDATA_WAIT		12
#define VOICE_FLG_WAIT_VDATA		13
#define VOICE_FLG_S_GDATA_WAIT		14
#define VOICE_FLG_WAIT_GDATA		15
#define VOICE_LIST_COMPLETE_WAIT	16
#define VOICE_LIST_COMPLETE_OK		17
unsigned char voiceFlg = VOICE_FLG_IDLE;
const char newVoiceCode[] = "voice_list";
const char newVoiceCodeOk[] = "voice list ok\n\r";
const char requestListLength[] = "list length";
const char downLoadComplete[] = "down complete";
#endif
const char g_code_len[] = "glen";
const char backing[] = "backing out";
const char recording[] = "record";
const char loging[] = "log";
const char configing[] = "config";
const char protecting[] = "protected";
const char nexting[] = "next";
const char alling[] = "all";
const char yes_com[] = "y";
const char no_com[] = "n";

const char download_ticket_head[] = "Downlaod Ticket Head\n\r";
const char download_g_code[] = "Download g_code\n\r";
const char download_gcode_len[] = "Download g_code len\n\r";

const char ticket_head_ok[] = "Ticket Head Recevice OK!\n\r";
const char ticket_head_err[] = "Ticket Head Recevice Error!\n\r";
const char gcode_data_ok[] = "g_code data Recevice OK!\n\r";
const char gcode_data_err[] = "g_code data Recevice Error!\n\r";

const char recevice_ok[] = "success";
const char recevice_error[] = "fail";

const char start_dbug_ok[] = "start dbug ok\n\r";
const char start_loading[] = "going loading\n\r";
const char start_formating[] = "going format\n\r";
const char exit_dbug_ok[] = "exit dbug ok\n\r";
const char returning_ok[] = "return ok\n\r";
const char recording_ok[] = "record ok\n\r";
const char record_snd_ok[] = "record send complete\n\r";
const char log_snd_ok[] = "log send complete\n\r";
const char command_err[] = "command error\n\r";
const char log_ok[] = "going loading log\n\r";

const char format_config_y_n[] = "format config y/n?\n\r";
const char format_record_y_n[] = "format record y/n?\n\r";
const char format_protected_y_n[] = "format protected y/n?\n\r";
const char format_log_y_n[] = "format log y/n\n\r";
const char format_all_y_n[] = "format all y/n\n\r";
const char begin_load_all[] = "begin loading all!!!\n\r";

const char formated[] = "format completed!!!\n\r";
const char formatinging[] ="formating...!!!\n\r"; 
const char no_allow[] = "formating,no back or no exit!!!\n\r";

static unsigned char dbug_flg;
static unsigned int dbug_len;
static unsigned char SendFlg = 1;

const char *pCommand;

static unsigned int RecordAdr;
static unsigned char RecordIndex;
static unsigned int LogAdr = 0x202;
static unsigned char LogIndex;
static unsigned char DbugFlg;

static unsigned int DbugTime;

static unsigned short FormatFlg;

static unsigned char ChipId;
static unsigned int PageNumber;
static Tmr *pDbugTmr;

static unsigned char TicketHeadBuf[1024];
static unsigned char TicketHeadLen;

static unsigned int GcodeLen;
static unsigned int GcodePackNum;
static unsigned int GcodeCurNum;

void RepeatAll(void)
{
	DbugFlg = 3;
}

void DbugSendBack(unsigned char *s,unsigned int len,unsigned char error)
{
	s = s;
	len = len;
	error = error;
	SendFlg = 1;
	DbugSerial.pUsartHw->DBGU_IER = AT91C_US_RXRDY;
#ifdef NEW_VOICE
	if(voiceFlg == VOICE_FLG_DOWN_C)
	{
		voiceFlg = VOICE_FLG_R_DOWN_C_OK;
	}
	else if(voiceFlg == VOICE_FLG_S_LENGTH_WAIT)
	{
		voiceFlg = VOICE_FLG_WAIT_LENGTH;
	}
	else if(voiceFlg == VOICE_FLG_WAIT_VDATA)
	{
		voiceFlg = VOICE_FLG_WAIT_VDATA;
	}
	else if(voiceFlg == VOICE_FLG_S_GDATA_WAIT)
	{
		voiceFlg = VOICE_FLG_WAIT_GDATA;
	}
	else if(voiceFlg == VOICE_LIST_COMPLETE_WAIT)
	{
		voiceFlg = VOICE_LIST_COMPLETE_OK;
	}
#endif
	if(DbugFlg == MaxFunctions)
	{
		if(pDbugTmr == (Tmr *)0)
		{
			pDbugTmr = CreateTimer(TIMER_ONE,10000,RepeatAll);
		}
		else
		{
			ResetTimer(pDbugTmr);
		}
	}
}

void DbugReceviceLoadingRecord(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char length;
	unsigned int time;
	
	while(len--)
	{
		time = timer_val.current_val;
		if(dbug_len == 0)
		{
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = backing;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = nexting;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			/*dbug_len = 0;
			length = strlen(command_err);
			WriteToDBUG_USART((unsigned char*)&command_err,length);*/
		}
		else
		{
			length = strlen(pCommand);
			if(*s == pCommand[dbug_len++])
			{
				if(length == dbug_len)
				{
					dbug_len = 0;
					if(memcmp(pCommand,exit_dbug,length) == 0)
					{
						dbug_flg = 0;
						dbug_len = 0;
						DbugFlg = 0;
						length = strlen(exit_dbug_ok);
						WriteToDBUG_USART((unsigned char*)&exit_dbug_ok,length);
						DbugSerial.receviceback = DbugStartRecevice;
					}
					else if(memcmp(pCommand,backing,length) == 0)
					{
						dbug_len = 0;
						length = strlen(returning_ok);
						WriteToDBUG_USART((unsigned char*)&returning_ok,length);
						DbugSerial.receviceback = DbugReceviceLoading;
					}
					else if(memcmp(pCommand,nexting,length) == 0)
					{
						dbug_len = 0;
						DbugFlg = 1;
					}
					else
					{
						dbug_len = 0;
						length = strlen(command_err);
						WriteToDBUG_USART((unsigned char*)&command_err,length);
					}
				}
			}
			else if(length == dbug_len)
			{
				dbug_len = 0;
				length = strlen(command_err);
				WriteToDBUG_USART((unsigned char*)&command_err,length);
			}
			else
			{
				if((DbugTime + DBUG_TIME) > time)
				{
					dbug_len = 0;
				}
			}
		}
		DbugTime = time;
		s++;
	}
}

void DbugReceviceLoadingLog(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char length;
	unsigned int time;
	
	while(len--)
	{
		time = timer_val.current_val;
		if(dbug_len == 0)
		{
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = backing;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = nexting;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
		}
		else
		{
			length = strlen(pCommand);
			if(*s == pCommand[dbug_len++])
			{
				if(length == dbug_len)
				{
					dbug_len = 0;
					if(memcmp(pCommand,exit_dbug,length) == 0)
					{
						dbug_flg = 0;
						dbug_len = 0;
						DbugFlg = 0;
						length = strlen(exit_dbug_ok);
						WriteToDBUG_USART((unsigned char*)&exit_dbug_ok,length);
						DbugSerial.receviceback = DbugStartRecevice;
					}
					else if(memcmp(pCommand,backing,length) == 0)
					{
						dbug_len = 0;
						length = strlen(returning_ok);
						WriteToDBUG_USART((unsigned char*)&returning_ok,length);
						DbugSerial.receviceback = DbugReceviceLoading;
					}
					else if(memcmp(pCommand,nexting,length) == 0)
					{
						dbug_len = 0;
						DbugFlg = 2;
					}
				}
			}
			else if(length == dbug_len)
			{
				dbug_len = 0;
				length = strlen(command_err);
				WriteToDBUG_USART((unsigned char*)&command_err,length);
			}
			else
			{
				if((DbugTime + DBUG_TIME) > time)
				{
					dbug_len = 0;
				}
			}
		}
		DbugTime = time;
		s++;
	}
}

void DbugReceviceLoadingAll(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char length;
	
	while(len--)
	{
		if(dbug_len == 0)
		{
			pCommand = recevice_ok;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = recevice_error;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = backing;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
		}
		else
		{
			length = strlen(pCommand);
			if(*s == pCommand[dbug_len++])
			{
				if(length == dbug_len)
				{
					dbug_len = 0;
					if(memcmp(pCommand,recevice_ok,length) == 0)
					{
						StopTimer(pDbugTmr);
						PageNumber++;
						DbugFlg = 3;
					}
					else if(memcmp(pCommand,recevice_error,length) == 0)
					{
						StopTimer(pDbugTmr);
						DbugFlg = 3;
					}
					else if(memcmp(pCommand,backing,length) == 0)
					{
						StopTimer(pDbugTmr);
						dbug_len = 0;
						DbugFlg = 0;
						length = strlen(start_loading);
						WriteToDBUG_USART((unsigned char*)&start_loading,length);
						DbugSerial.receviceback = DbugReceviceLoading;
					}
					else if(memcmp(pCommand,exit_dbug,length) == 0)
					{
						StopTimer(pDbugTmr);
						dbug_flg = 0;
						dbug_len = 0;
						length = strlen(exit_dbug_ok);
						WriteToDBUG_USART((unsigned char*)&exit_dbug_ok,length);
						DbugSerial.receviceback = DbugStartRecevice;
					}
				}
			}
			else
			{
				dbug_len = 0;
			}
		}
		s++;
	}
}

void DbugReceviceLoading(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char length;
	unsigned int time;
	
	while(len--)
	{
		time = timer_val.current_val;
		if(dbug_len == 0)
		{
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = backing;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = recording;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = loging;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = alling;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			/*dbug_len = 0;
			length = strlen(command_err);
			WriteToDBUG_USART((unsigned char*)&command_err,length);*/
		}
		else
		{
			length = strlen(pCommand);
			if(*s == pCommand[dbug_len++])
			{
				if(length == dbug_len)
				{
					dbug_len = 0;
					if(memcmp(pCommand,exit_dbug,length) == 0)
					{
						dbug_flg = 0;
						dbug_len = 0;
						length = strlen(exit_dbug_ok);
						WriteToDBUG_USART((unsigned char*)&exit_dbug_ok,length);
						DbugSerial.receviceback = DbugStartRecevice;
					}
					else if(memcmp(pCommand,backing,length) == 0)
					{
						dbug_len = 0;
						length = strlen(returning_ok);
						WriteToDBUG_USART((unsigned char*)&returning_ok,length);
						DbugSerial.receviceback = DbugReceviceCommand1;
					}
					else if(memcmp(pCommand,recording,length) == 0)
					{
						dbug_len = 0;
						RecordAdr = RECORD_START_PAGE;
						RecordIndex = 0;
						length = strlen(recording_ok);
						WriteToDBUG_USART((unsigned char*)&recording_ok,length);
						DbugSerial.receviceback = DbugReceviceLoadingRecord;
					}
					else if(memcmp(pCommand,loging,length) == 0)
					{
						dbug_len = 0;
						length = strlen(log_ok);
						WriteToDBUG_USART((unsigned char*)&log_ok,length);
						DbugSerial.receviceback = DbugReceviceLoadingLog;
					}
					else if(memcmp(pCommand,alling,length) == 0)
					{
						dbug_len = 0;
						length = strlen(begin_load_all);
						WriteToDBUG_USART((unsigned char*)&begin_load_all,length);
						DbugSerial.receviceback = DbugReceviceLoadingAll;
						ChipId = 0;
						PageNumber = 0;
						DbugFlg = 3;
					}
					else
					{
						dbug_len = 0;
						length = strlen(command_err);
						WriteToDBUG_USART((unsigned char*)&command_err,length);
					}
				}
			}
			else if(length == dbug_len)
			{
				dbug_len = 0;
				length = strlen(command_err);
				WriteToDBUG_USART((unsigned char*)&command_err,length);
			}
			else
			{
				if((DbugTime + DBUG_TIME) > time)
				{
					dbug_len = 0;
				}
			}
		}
		DbugTime = time;
		s++;
	}
}

void DbugFormatConfig(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char length;
	
	while(len--)
	{
		if(dbug_len == 0)
		{
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = backing;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = yes_com;
			if(*s == pCommand[0])
			{
				if(( FormatFlg & 0x01) == 0)
				{
					FormatFlg |= 0x01;
				}
				else if(FormatFlg & 0x10)
				{
					length = strlen(formated);
					WriteToDBUG_USART((unsigned char*)&formated,length);
				}
				else if(FormatFlg & 0x01)
				{
					length = strlen(formatinging);
					WriteToDBUG_USART((unsigned char*)&formatinging,length);
				}
				break;
			}
			pCommand = no_com;
			if(*s == pCommand[0])
			{
				if((FormatFlg & 0x01) == 0)
				{
					length = strlen(start_formating);
					WriteToDBUG_USART((unsigned char*)&start_formating,length);
					DbugSerial.receviceback = DbugReceviceForamt;
					break;
				}
				else
				{
					length = strlen(no_allow);
					WriteToDBUG_USART((unsigned char*)&no_allow,length);
				}
			}
		}
		else
		{
			length = strlen(pCommand);
			if(*s == pCommand[dbug_len++])
			{
				if(length == dbug_len)
				{
					dbug_len = 0;
					if(memcmp(pCommand,exit_dbug,length) == 0)
					{
						if(((FormatFlg & 0x11) == 0) || ((FormatFlg & 0x10) == 0x10))
						{
							dbug_flg = 0;
							dbug_len = 0;
							length = strlen(exit_dbug_ok);
							WriteToDBUG_USART((unsigned char*)&exit_dbug_ok,length);
							DbugSerial.receviceback = DbugStartRecevice;
						}
						else
						{
							dbug_len = 0;
							length = strlen(no_allow);
							WriteToDBUG_USART((unsigned char*)&no_allow,length);
						}
					}
					else if(memcmp(pCommand,backing,length) == 0)
					{
						if(((FormatFlg & 0x11) == 0) || ((FormatFlg & 0x10) == 0x10))
						{
							dbug_len = 0;
							length = strlen(returning_ok);
							WriteToDBUG_USART((unsigned char*)&returning_ok,length);
							DbugSerial.receviceback = DbugReceviceForamt;
						}
						else
						{
							dbug_len = 0;
							length = strlen(no_allow);
							WriteToDBUG_USART((unsigned char*)&no_allow,length);
						}
					}
					else
					{
						dbug_len = 0;
						length = strlen(command_err);
						WriteToDBUG_USART((unsigned char*)&command_err,length);
					}
				}
			}
		}
		s++;
	}
}

void DbugFormatRecord(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char length;
	
	while(len--)
	{
		if(dbug_len == 0)
		{
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = backing;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = yes_com;
			if(*s == pCommand[0])
			{
				if(( FormatFlg & 0x02) == 0)
				{
					FormatFlg |= 0x02;
				}
				else if(FormatFlg & 0x20)
				{
					length = strlen(formated);
					WriteToDBUG_USART((unsigned char*)&formated,length);
				}
				else if(FormatFlg & 0x02)
				{
					length = strlen(formatinging);
					WriteToDBUG_USART((unsigned char*)&formatinging,length);
				}
				break;
			}
			pCommand = no_com;
			if(*s == pCommand[0])
			{
				if((FormatFlg & 0x02) == 0)
				{
					length = strlen(start_formating);
					WriteToDBUG_USART((unsigned char*)&start_formating,length);
					DbugSerial.receviceback = DbugReceviceForamt;
					break;
				}
				else
				{
					length = strlen(no_allow);
					WriteToDBUG_USART((unsigned char*)&no_allow,length);
				}
			}
		}
		else
		{
			length = strlen(pCommand);
			if(*s == pCommand[dbug_len++])
			{
				if(length == dbug_len)
				{
					dbug_len = 0;
					if(memcmp(pCommand,exit_dbug,length) == 0)
					{
						if(((FormatFlg & 0x22) == 0) || ((FormatFlg & 0x20) == 0x20))
						{
							dbug_flg = 0;
							dbug_len = 0;
							length = strlen(exit_dbug_ok);
							WriteToDBUG_USART((unsigned char*)&exit_dbug_ok,length);
							DbugSerial.receviceback = DbugStartRecevice;
						}
						else
						{
							dbug_len = 0;
							length = strlen(no_allow);
							WriteToDBUG_USART((unsigned char*)&no_allow,length);
						}
					}
					else if(memcmp(pCommand,backing,length) == 0)
					{
						if(((FormatFlg & 0x22) == 0) || ((FormatFlg & 0x20) == 0x20))
						{
							dbug_len = 0;
							length = strlen(returning_ok);
							WriteToDBUG_USART((unsigned char*)&returning_ok,length);
							DbugSerial.receviceback = DbugReceviceForamt;
						}
						else
						{
							dbug_len = 0;
							length = strlen(no_allow);
							WriteToDBUG_USART((unsigned char*)&no_allow,length);
						}
					}
					else
					{
						dbug_len = 0;
						length = strlen(command_err);
						WriteToDBUG_USART((unsigned char*)&command_err,length);
					}
				}
			}
		}
		s++;
	}
}

void DbugFormatProtected(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char length;
	
	while(len--)
	{
		if(dbug_len == 0)
		{
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = backing;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = yes_com;
			if(*s == pCommand[0])
			{
				if(( FormatFlg & 0x04) == 0)
				{
					FormatFlg |= 0x04;
				}
				else if(FormatFlg & 0x40)
				{
					length = strlen(formated);
					WriteToDBUG_USART((unsigned char*)&formated,length);
				}
				else if(FormatFlg & 0x04)
				{
					length = strlen(formatinging);
					WriteToDBUG_USART((unsigned char*)&formatinging,length);
				}
				break;
			}
			pCommand = no_com;
			if(*s == pCommand[0])
			{
				if((FormatFlg & 0x04) == 0)
				{
					length = strlen(start_formating);
					WriteToDBUG_USART((unsigned char*)&start_formating,length);
					DbugSerial.receviceback = DbugReceviceForamt;
					break;
				}
				else
				{
					length = strlen(no_allow);
					WriteToDBUG_USART((unsigned char*)&no_allow,length);
				}
			}
		}
		else
		{
			length = strlen(pCommand);
			if(*s == pCommand[dbug_len++])
			{
				if(length == dbug_len)
				{
					dbug_len = 0;
					if(memcmp(pCommand,exit_dbug,length) == 0)
					{
						if(((FormatFlg & 0x44) == 0) || ((FormatFlg & 0x40) == 0x40))
						{
							dbug_flg = 0;
							dbug_len = 0;
							length = strlen(exit_dbug_ok);
							WriteToDBUG_USART((unsigned char*)&exit_dbug_ok,length);
							DbugSerial.receviceback = DbugStartRecevice;
						}
						else
						{
							dbug_len = 0;
							length = strlen(no_allow);
							WriteToDBUG_USART((unsigned char*)&no_allow,length);
						}
					}
					else if(memcmp(pCommand,backing,length) == 0)
					{
						if(((FormatFlg & 0x44) == 0) || ((FormatFlg & 0x40) == 0x40))
						{
							dbug_len = 0;
							length = strlen(returning_ok);
							WriteToDBUG_USART((unsigned char*)&returning_ok,length);
							DbugSerial.receviceback = DbugReceviceForamt;
						}
						else
						{
							dbug_len = 0;
							length = strlen(no_allow);
							WriteToDBUG_USART((unsigned char*)&no_allow,length);
						}
					}
					else
					{
						dbug_len = 0;
						length = strlen(command_err);
						WriteToDBUG_USART((unsigned char*)&command_err,length);
					}
				}
			}
		}
		s++;
	}
}

void DbugFormatLog(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char length;
	
	while(len--)
	{
		if(dbug_len == 0)
		{
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = backing;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = yes_com;
			if(*s == pCommand[0])
			{
				if(( FormatFlg & 0x08) == 0)
				{
					FormatFlg |= 0x08;
				}
				else if(FormatFlg & 0x80)
				{
					length = strlen(formated);
					WriteToDBUG_USART((unsigned char*)&formated,length);
				}
				else if(FormatFlg & 0x08)
				{
					length = strlen(formatinging);
					WriteToDBUG_USART((unsigned char*)&formatinging,length);
				}
				break;
			}
			pCommand = no_com;
			if(*s == pCommand[0])
			{
				if((FormatFlg & 0x08) == 0)
				{
					length = strlen(start_formating);
					WriteToDBUG_USART((unsigned char*)&start_formating,length);
					DbugSerial.receviceback = DbugReceviceForamt;
					break;
				}
				else
				{
					length = strlen(no_allow);
					WriteToDBUG_USART((unsigned char*)&no_allow,length);
				}
			}
		}
		else
		{
			length = strlen(pCommand);
			if(*s == pCommand[dbug_len++])
			{
				if(length == dbug_len)
				{
					dbug_len = 0;
					if(memcmp(pCommand,exit_dbug,length) == 0)
					{
						if(((FormatFlg & 0x88) == 0) || ((FormatFlg & 0x80) == 0x80))
						{
							dbug_flg = 0;
							dbug_len = 0;
							length = strlen(exit_dbug_ok);
							WriteToDBUG_USART((unsigned char*)&exit_dbug_ok,length);
							DbugSerial.receviceback = DbugStartRecevice;
						}
						else
						{
							dbug_len = 0;
							length = strlen(no_allow);
							WriteToDBUG_USART((unsigned char*)&no_allow,length);
						}
					}
					else if(memcmp(pCommand,backing,length) == 0)
					{
						if(((FormatFlg & 0x88) == 0) || ((FormatFlg & 0x80) == 0x80))
						{
							dbug_len = 0;
							length = strlen(returning_ok);
							WriteToDBUG_USART((unsigned char*)&returning_ok,length);
							DbugSerial.receviceback = DbugReceviceForamt;
						}
						else
						{
							dbug_len = 0;
							length = strlen(no_allow);
							WriteToDBUG_USART((unsigned char*)&no_allow,length);
						}
					}
					else
					{
						dbug_len = 0;
						length = strlen(command_err);
						WriteToDBUG_USART((unsigned char*)&command_err,length);
					}
				}
			}
		}
		s++;
	}
}

void DbugFormatAll(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char length;
	
	while(len--)
	{
		if(dbug_len == 0)
		{
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = backing;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = yes_com;
			if(*s == pCommand[0])
			{
				if(( FormatFlg & 0x100) == 0)
				{
					FormatFlg |= 0x100;
				}
				else if(FormatFlg & 0x200)
				{
					length = strlen(formated);
					WriteToDBUG_USART((unsigned char*)&formated,length);
				}
				else if(FormatFlg & 0x100)
				{
					length = strlen(formatinging);
					WriteToDBUG_USART((unsigned char*)&formatinging,length);
				}
				break;
			}
			pCommand = no_com;
			if(*s == pCommand[0])
			{
				if((FormatFlg & 0x100) == 0)
				{
					length = strlen(start_formating);
					WriteToDBUG_USART((unsigned char*)&start_formating,length);
					DbugSerial.receviceback = DbugReceviceForamt;
					break;
				}
				else
				{
					length = strlen(no_allow);
					WriteToDBUG_USART((unsigned char*)&no_allow,length);
				}
			}
		}
		else
		{
			length = strlen(pCommand);
			if(*s == pCommand[dbug_len++])
			{
				if(length == dbug_len)
				{
					dbug_len = 0;
					if(memcmp(pCommand,exit_dbug,length) == 0)
					{
						if(((FormatFlg & 0x300) == 0) || ((FormatFlg & 0x200) == 0x200))
						{
							dbug_flg = 0;
							dbug_len = 0;
							length = strlen(exit_dbug_ok);
							WriteToDBUG_USART((unsigned char*)&exit_dbug_ok,length);
							DbugSerial.receviceback = DbugStartRecevice;
						}
						else
						{
							dbug_len = 0;
							length = strlen(no_allow);
							WriteToDBUG_USART((unsigned char*)&no_allow,length);
						}
					}
					else if(memcmp(pCommand,backing,length) == 0)
					{
						if(((FormatFlg & 0x300) == 0) || ((FormatFlg & 0x200) == 0x200))
						{
							dbug_len = 0;
							length = strlen(returning_ok);
							WriteToDBUG_USART((unsigned char*)&returning_ok,length);
							DbugSerial.receviceback = DbugReceviceForamt;
						}
						else
						{
							dbug_len = 0;
							length = strlen(no_allow);
							WriteToDBUG_USART((unsigned char*)&no_allow,length);
						}
					}
					else
					{
						dbug_len = 0;
						length = strlen(command_err);
						WriteToDBUG_USART((unsigned char*)&command_err,length);
					}
				}
			}
		}
		s++;
	}
}

void DbugReceviceForamt(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char length;
	unsigned int time;
	
	while(len--)
	{
		time = timer_val.current_val;
		if(dbug_len == 0)
		{
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = configing;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = recording;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = protecting;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = loging;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = backing;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = alling;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
		}
		else
		{
			length = strlen(pCommand);
			if(*s == pCommand[dbug_len++])
			{
				if(length == dbug_len)
				{
					dbug_len = 0;
					FormatFlg = 0;
					if(memcmp(pCommand,exit_dbug,length) == 0)
					{
						dbug_flg = 0;
						dbug_len = 0;
						length = strlen(exit_dbug_ok);
						WriteToDBUG_USART((unsigned char*)&exit_dbug_ok,length);
						DbugSerial.receviceback = DbugStartRecevice;
					}
					else if(memcmp(pCommand,configing,length) == 0)
					{
						dbug_len = 0;
						length = strlen(format_config_y_n);
						WriteToDBUG_USART((unsigned char *)&format_config_y_n,length);
						DbugSerial.receviceback = DbugFormatConfig;
					}
					else if(memcmp(pCommand,recording,length) == 0)
					{
						dbug_len = 0;
						length = strlen(format_record_y_n);
						WriteToDBUG_USART((unsigned char *)&format_record_y_n,length);
						DbugSerial.receviceback = DbugFormatRecord;
					}
					else if(memcmp(pCommand,protecting,length) == 0)
					{
						dbug_len = 0;
						length = strlen(format_protected_y_n);
						WriteToDBUG_USART((unsigned char *)&format_protected_y_n,length);
						DbugSerial.receviceback = DbugFormatProtected;
					}
					else if(memcmp(pCommand,loging,length) == 0)
					{
						dbug_len = 0;
						length = strlen(format_log_y_n);
						WriteToDBUG_USART((unsigned char *)&format_log_y_n,length);
						DbugSerial.receviceback = DbugFormatLog;
					}
					else if(memcmp(pCommand,backing,length) == 0)
					{
						dbug_len = 0;
						length = strlen(returning_ok);
						WriteToDBUG_USART((unsigned char*)&returning_ok,length);
						DbugSerial.receviceback = DbugReceviceCommand1;
					}
					else if(memcmp(pCommand,alling,length) == 0)
					{
						dbug_len = 0;
						length = strlen(format_all_y_n);
						WriteToDBUG_USART((unsigned char *)&format_all_y_n,length);
						DbugSerial.receviceback = DbugFormatAll;
					}
					else
					{
						dbug_len = 0;
						length = strlen(command_err);
						WriteToDBUG_USART((unsigned char*)&command_err,length);
					}
				}
			}
			else if(length == dbug_len)
			{
				dbug_len = 0;
				length = strlen(command_err);
				WriteToDBUG_USART((unsigned char*)&command_err,length);
			}
			else
			{
				if((DbugTime + DBUG_TIME) > time)
				{
					dbug_len = 0;
				}
			}
		}
		DbugTime = time;
		s++;
	}
}

void DbugReceviceTicketHead(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned int time;
	
	while(len--)
	{
		time = timer_val.current_val;	
		if(dbug_len == 0)
		{
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			else
			{
				TicketHeadBuf[dbug_len++] = *s;
			}
		}
		else
		{
			if(*s == '\0')
			{
				TicketHeadLen = dbug_len + 2;
			}
			TicketHeadBuf[dbug_len++] = *s;
			if(TicketHeadLen == (dbug_len - 1))
			{
				DbugFlg = 4;
			}
		}
		DbugTime = time;
		s++;
	}
}

void DbugReceviceGcodeData(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned int time;
	
	while(len--)
	{
		time = timer_val.current_val;	
		if(dbug_len < 530)
		{
			TicketHeadBuf[dbug_len++] = *s;
		}
		else
		{
			DbugFlg = 5;
		}
		DbugTime = time;
		s++;
	}
}

void DbugReceviceGcodeLen(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned int time;
	unsigned char length;
	
	while(len--)
	{
		time = timer_val.current_val;	
		if(dbug_len < 8)
		{
			TicketHeadBuf[dbug_len++] = *s;
		}
		else
		{
			memcpy((char *)(&GcodeLen),TicketHeadBuf,4);
			memcpy((char *)(&GcodePackNum),&TicketHeadBuf[4],4);
			GcodeCurNum = 0;
			dbug_len = 0;
			length = strlen(download_gcode_len);
			WriteToDBUG_USART((unsigned char*)&download_gcode_len,length);
			DbugSerial.receviceback = DbugReceviceGcodeData;
		}
		DbugTime = time;
		s++;
	}
}

void DbugReceviceGcode(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned int time;
	unsigned char length;
	
	while(len--)
	{
		time = timer_val.current_val;	
		if(dbug_len == 0)
		{
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = g_code_len;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
		}
		else
		{
			length = strlen(pCommand);
			if(*s == pCommand[dbug_len++])
			{
				if(length == dbug_len)
				{
					dbug_len = 0;
					FormatFlg = 0;
					if(memcmp(pCommand,exit_dbug,length) == 0)
					{
						dbug_flg = 0;
						dbug_len = 0;
						length = strlen(exit_dbug_ok);
						WriteToDBUG_USART((unsigned char*)&exit_dbug_ok,length);
						DbugSerial.receviceback = DbugStartRecevice;
					}
					else if(memcmp(pCommand,g_code_len,length) == 0)
					{
						dbug_len = 0;
						length = strlen(download_gcode_len);
						WriteToDBUG_USART((unsigned char*)&download_gcode_len,length);
						DbugSerial.receviceback = DbugReceviceGcodeLen;
					}
					else
					{
						dbug_len = 0;
						length = strlen(command_err);
						WriteToDBUG_USART((unsigned char*)&command_err,length);
					}
				}
			}
		}
		DbugTime = time;
		s++;
	}
}

void DbugReceviceCommand1(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char length;
	unsigned int time;
	
	while(len--)
	{
		time = timer_val.current_val;
		if(dbug_len == 0)
		{
			pCommand = exit_dbug;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = loading;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = formating;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = ticket_head;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
			pCommand = g_code;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
#ifdef NEW_VOICE
			pCommand = newVoiceCode;
			if(*s == pCommand[0])
			{
				dbug_len++;
				continue;
			}
#endif
			/*dbug_len = 0;
			length = strlen(command_err);
			WriteToDBUG_USART((unsigned char*)&command_err,length);*/
		}
		else
		{
			length = strlen(pCommand);
			if(*s == pCommand[dbug_len++])
			{
				if(length == dbug_len)
				{
					dbug_len = 0;
					if(memcmp(pCommand,exit_dbug,length) == 0)
					{
						dbug_flg = 0;
						dbug_len = 0;
						length = strlen(exit_dbug_ok);
						WriteToDBUG_USART((unsigned char*)&exit_dbug_ok,length);
						DbugSerial.receviceback = DbugStartRecevice;
					}
					else if(memcmp(pCommand,loading,length) == 0)
					{
						dbug_len = 0;
						length = strlen(start_loading);
						WriteToDBUG_USART((unsigned char*)&start_loading,length);
						DbugSerial.receviceback = DbugReceviceLoading;
					}
					else if(memcmp(pCommand,formating,length) == 0)
					{
						dbug_len = 0;
						FormatFlg = 0;
						length = strlen(start_formating);
						WriteToDBUG_USART((unsigned char*)&start_formating,length);
						DbugSerial.receviceback = DbugReceviceForamt;
					}
					else if(memcmp(pCommand,ticket_head,length) == 0)
					{
						dbug_len = 0;
						FormatFlg = 0;
						length = strlen(download_ticket_head);
						WriteToDBUG_USART((unsigned char*)&download_ticket_head,length);
						DbugSerial.receviceback = DbugReceviceTicketHead;
					}
					else if(memcmp(pCommand,g_code,length) == 0)
					{
						dbug_len = 0;
						FormatFlg = 0;
						length = strlen(download_g_code);
						WriteToDBUG_USART((unsigned char*)&download_g_code,length);
						DbugSerial.receviceback = DbugReceviceGcode;
					}
#ifdef NEW_VOICE
					else if(memcmp(pCommand,newVoiceCode,length) == 0)
					{
						dbug_len = 0;
						FormatFlg = 0;
						length = strlen(newVoiceCodeOk);
						WriteToDBUG_USART((unsigned char*)&newVoiceCodeOk,length);
						voiceFlg = VOICE_FLG_DOWN_C;
					}
#endif
					else
					{
						dbug_len = 0;
						length = strlen(command_err);
						WriteToDBUG_USART((unsigned char*)&command_err,length);
					}
				}
			}
			else if(length == dbug_len)
			{
				dbug_len = 0;
				length = strlen(command_err);
				WriteToDBUG_USART((unsigned char*)&command_err,length);
			}
			else
			{
				if((DbugTime + DBUG_TIME) > time)
				{
					dbug_len = 0;
				}
			}
		}
		DbugTime = time;
		s++;
	}
}

void DbugStartRecevice(unsigned char *s,unsigned int len,unsigned char error)
{
	unsigned char length;
	unsigned int time;
	
	length = strlen(start_dbug);
	while(len--)
	{
		time = timer_val.current_val;
		if(*s == start_dbug[dbug_len++])
		{
			if(length == dbug_len)
			{
				dbug_flg = 1;
				dbug_len = 0;
				DbugFlg = 0;
				length = strlen(start_dbug_ok);
				WriteToDBUG_USART((unsigned char*)&start_dbug_ok,length);
				DbugSerial.receviceback = DbugReceviceCommand1;
			}
		}
		else if(length == dbug_len)
		{
			dbug_len = 0;
			length = strlen(command_err);
			WriteToDBUG_USART((unsigned char*)&command_err,length);
		}
		else
		{
			if((DbugTime + DBUG_TIME) > time)
			{
				dbug_len = 0;
			}
		}
		DbugTime = time;
		s++;
	}
}

void ReadRecordAdr(unsigned char *s,unsigned int page,unsigned char index)
{
	unsigned int adr;
	At45 *pAt45;
    
    pAt45 = &at45[TRANS_ID];
	adr = page * AT45_PageSize(pAt45);
	adr += index * RECORD_SIZE;
	READ(TRANS_ID,s,adr,96);
}

void RecordComBcdToAscii(unsigned char *s,unsigned char val)
{
	if((val & 0xf0) > 0x90)
	{
		*s = '-';
	}
	else
	{
		*s = '0' + (val >> 4);
	}
	s++;
	if((val & 0x0f) > 0x09)
	{
		*s = '-';
	}
	else
	{
		*s = '0' + (val & 0x0f);
	}
}

static const char POS_TTC[] = "POS-TTC:";
static const char T_TYPE[] = "T-TYPE:";
static const char TIME[] = "TIME:";
static const char ASN[] = "ASN:";
static const char BAL[] = "BAL:";
static const char AMN[] = "AMN:";
static const char CTC[] = "CTC:";
static const char TAC[] = "TAC:";
static const char GMAC[] = "GMAC:";
static const char PSAM_TAC[] = "PSAM-TAC:";
static const char PSAM_ASN[] = "PSAM-ASN:";
static const char PSAM_TID[] = "PSAM-TID:";
static const char PSAM_TTC[] = "PSAM-TTC:";
static const char DS[] = "DS:";
static const char UNIT[] = "UNIT:";
static const char C_TYPE[] = "C-TYPE:";
static const char VER[] = "VER:";
static const char NZN[] = "NZN:";
static const char G_CODE[] = "G-CODE:";
static const char V_VOL[] = "VOL:";
static const char PRC[] = "PRC:";
static const char EMP[] = "EMP:";
static const char V_TOT[] = "V-TOT:";
static const char RFU[] = "RFU:";
static const char T_MAC[] = "T-MAC:";

void SndRecordToDbugUsart(void)
{
	unsigned char buf[96];
	unsigned int len;
	unsigned int itmp;
	unsigned short stmp;
	unsigned char i;
	unsigned char *p;
	
	p = printStruct.buf;
	
	if(RecordAdr < RECORD_END_PAGE + 1)
	{
		ReadRecordAdr(buf,RecordAdr,RecordIndex);
		len = 0;
		i = strlen(POS_TTC);
		//POS-TTC
		memcpy(&p[len],POS_TTC,i);
		len += i;
		memcpy((unsigned char *)(&itmp),&buf[1],4);
		itmp = IntEndianConvert(itmp);
		i = sprintf((char *)(&p[len]),"%lu",itmp);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//T-TYPE
		i = strlen(T_TYPE);
		memcpy(&p[len],T_TYPE,i);
		len += i;
		if((buf[5] & 0xf0) > 0x90)
		{
			p[len++] = 'A' + (buf[5] >> 4);
		}
		else
		{
			p[len++] = '0' + (buf[5] >> 4);
		}
		if((buf[5] & 0xf0) > 0x09)
		{
			p[len++] = 'A' + (buf[5] & 0x0f);
		}
		else
		{
			p[len++] = '0' + (buf[5] & 0x0f);
		}
		p[len++] = '\n';
		p[len++] = '\r';
		//TIME
		i = strlen(TIME);
		memcpy(&p[len],TIME,i);
		len += i;
		//year
		RecordComBcdToAscii(&p[len],buf[6]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[7]);
		len += 2;
		p[len++] = '/';
		//month
		RecordComBcdToAscii(&p[len],buf[8]);
		len += 2;
		p[len++] = '/';
		//day
		RecordComBcdToAscii(&p[len],buf[9]);
		len += 2;
		p[len++] = ' ';
		//hour
		RecordComBcdToAscii(&p[len],buf[10]);
		len += 2;
		p[len++] = ':';
		//minute
		RecordComBcdToAscii(&p[len],buf[11]);
		len += 2;
		p[len++] = ':';
		//second
		RecordComBcdToAscii(&p[len],buf[12]);
		len += 2;
		p[len++] = '\n';
		p[len++] = '\r';
		//ASN
		i = strlen(ASN);
		memcpy(&p[len],ASN,i);
		len += i;
		RecordComBcdToAscii(&p[len],buf[13]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[14]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[15]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[16]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[17]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[18]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[19]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[20]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[21]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[22]);
		len += 2;
		p[len++] = '\n';
		p[len++] = '\r';
		//BAL
		i = strlen(BAL);
		memcpy(&p[len],BAL,i);
		len += i;
		memcpy((unsigned char *)(&itmp),&buf[23],4);
		itmp = IntEndianConvert(itmp);
		i = sprintf((char *)(&p[len]),"%lu",itmp);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//AMN
		i = strlen(AMN);
		memcpy(&p[len],AMN,i);
		len += i;
		itmp = 0;
		memcpy((unsigned char *)(&itmp),&buf[27],3);
		itmp = IntEndianConvert(itmp);
		itmp >>= 8;
		i = HexToAscii((char *)&p[len],itmp);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//CTC
		i = strlen(CTC);
		memcpy(&p[len],CTC,i);
		len += i;
		memcpy((unsigned char *)(&stmp),&buf[30],2);
		stmp = ShortEndianConvert(stmp);
		i = BcdToAscii((char *)(&p[len]),(char *)(&stmp),2);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//TAC
		i = strlen(TAC);
		memcpy(&p[len],TAC,i);
		len += i;
		i = BcdToAscii((char *)(&p[len]),(char *)(&buf[32]),4);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//GMAC
		i = strlen(GMAC);
		memcpy(&p[len],GMAC,i);
		len += i;
		i = BcdToAscii((char *)(&p[len]),(char *)(&buf[36]),4);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//PSAM-TAC
		i = strlen(PSAM_TAC);
		memcpy(&p[len],PSAM_TAC,i);
		len += i;
		i = BcdToAscii((char *)(&p[len]),(char *)(&buf[40]),4);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//PSAM-ASN
		i = strlen(PSAM_ASN);
		memcpy(&p[len],PSAM_ASN,i);
		len += i;
		RecordComBcdToAscii(&p[len],buf[44]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[45]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[46]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[47]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[48]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[49]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[50]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[51]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[52]);
		len += 2;
		RecordComBcdToAscii(&p[len],buf[53]);
		len += 2;
		p[len++] = '\n';
		p[len++] = '\r';
		//PSAM-TID
		i = strlen(PSAM_TID);
		memcpy(&p[len],PSAM_TID,i);
		len += i;
		i = BcdToAscii((char *)(&p[len]),(char *)(&buf[54]),6);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//PSAM-TTC
		i = strlen(PSAM_TTC);
		memcpy(&p[len],PSAM_TTC,i);
		len += i;
		memcpy((unsigned char *)(&itmp),&buf[60],4);
		itmp = IntEndianConvert(itmp);
		i = sprintf((char *)(&p[len]),"%lu",itmp);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//DS
		i = strlen(DS);
		memcpy(&p[len],DS,i);
		len += i;
		i = BcdToAscii((char *)(&p[len]),(char *)(&buf[64]),1);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//UNIT
		i = strlen(UNIT);
		memcpy(&p[len],UNIT,i);
		len += i;
		i = BcdToAscii((char *)(&p[len]),(char *)(&buf[65]),1);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//C-TYPE
		i = strlen(C_TYPE);
		memcpy(&p[len],C_TYPE,i);
		len += i;
		i = BcdToAscii((char *)(&p[len]),(char *)(&buf[66]),1);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//VER
		i = strlen(VER);
		memcpy(&p[len],VER,i);
		len += i;
		i = BcdToAscii((char *)(&p[len]),(char *)(&buf[67]),1);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//NZN
		i = strlen(NZN);
		memcpy(&p[len],NZN,i);
		len += i;
		i = sprintf((char *)(&p[len]),"%lu",buf[68]);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//G-CODE
		i = strlen(G_CODE);
		memcpy(&p[len],G_CODE,i);
		len += i;
		i = BcdToAscii((char *)(&p[len]),(char *)(&buf[69]),2);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//VOL
		i = strlen(V_VOL);
		memcpy(&p[len],V_VOL,i);
		len += i;
		memcpy((unsigned char *)(&itmp),&buf[71],3);
		itmp = IntEndianConvert(itmp);
		itmp >>= 8;
		i = HexToAscii((char *)(&p[len]),itmp);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//PRC
		i = strlen(PRC);
		memcpy(&p[len],PRC,i);
		len += i;
		memcpy((unsigned char *)(&stmp),&buf[74],2);
		stmp = ShortEndianConvert(stmp);
		i = HexToAscii((char *)(&p[len]),stmp);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//EMP
		i = strlen(EMP);
		memcpy(&p[len],EMP,i);
		len += i;
		i = sprintf((char *)(&p[len]),"%lu",buf[76]);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//V-TOT
		i = strlen(V_TOT);
		memcpy(&p[len],V_TOT,i);
		len += i;
		memcpy((unsigned char *)(&itmp),&buf[77],4);
		itmp = IntEndianConvert(itmp);
		i = HexToAscii((char *)(&p[len]),itmp);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//RFU
		i = strlen(RFU);
		memcpy(&p[len],RFU,i);
		len += i;
		i = BcdToAscii((char *)(&p[len]),(char *)(&buf[81]),11);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		//T-MAC
		i = strlen(T_MAC);
		memcpy(&p[len],T_MAC,i);
		len += i;
		i = BcdToAscii((char *)(&p[len]),(char *)(&buf[92]),4);
		len += i;
		p[len++] = '\n';
		p[len++] = '\r';
		
		p[len++] = '\n';
		p[len++] = '\r';
		
		
		while(!SendFlg)
		{
			WatchDog();
			CheckTimerGo();
		}
		SendFlg = 0;
		WriteToDBUG_USART(p,len);
		if(RecordIndex == 3)
		{
			RecordAdr++;
		}
		RecordIndex = (RecordIndex + 1) & 0x03;
	}
	else
	{
		i = strlen(record_snd_ok);
		WriteToDBUG_USART((unsigned char*)&record_snd_ok,i);
	}
}

#define LOG_MAXLEN		66

void SndLogToDbugUsart(void)
{
	unsigned char buf[66];
	unsigned int adr;
	unsigned short i,len;
	unsigned char *p;
	
	p = printStruct.buf;
	
	if(LogAdr < 0x1000)
	{
		adr = LogAdr + LogIndex * (LogIndex * LOG_MAXLEN);
		READ(LOG_ID,buf,adr,66);
		memcpy((unsigned char *)(&i),buf,2);
		len = 0;
		i = sprintf((char *)(&p[len]),"%0.5lu",i);
		len += i;
		memset(&p[len],' ' ,2);
		len += 2;
		for(i = 2; i < 66; i++)
		{
			if(buf[i] == '\0')
			{
				break;
			}
		}
		memcpy(&p[len],&buf[2],i - 2);
		len += i - 2;
		p[len++] = '\n';
		p[len++] = '\r';
		LogIndex++;
		if(LogIndex % (AT45_PageSize(&at45[1]) / LOG_MAXLEN) == 0)
		{
			LogAdr++;
		}
		while(!SendFlg)
		{
			WatchDog();
			CheckTimerGo();
		}
		SendFlg = 0;
		WriteToDBUG_USART(p,len);
	}
	else
	{
		i = strlen(log_snd_ok);
		WriteToDBUG_USART((unsigned char*)&log_snd_ok,i);
	}
}

void SndDataAllUsart(void)
{
	unsigned short crc;
	unsigned int adr;
	unsigned char *p;
	unsigned int len;
	
	p = printStruct.buf;
	
	if(PageNumber >= AT45_PageNumber(&at45[0]))
	{
		ChipId++;
		PageNumber = 0;
	}
	if(ChipId == 0)
	{
		len = AT45_PageSize(&at45[0]);
		adr = PageNumber * len;
		READ(0,p,adr,len);
		*(p + len++) = ChipId;
		memcpy(p + len,(unsigned char *)(&PageNumber),4);
		len += 4;
		crc = Crc16(p,len,0);
		crc = ShortEndianConvert(crc);
		memcpy(&p[len],(unsigned char *)(&crc),2);
		len += 2;
		while(!SendFlg)
		{
			WatchDog();
			CheckTimerGo();
		}
		SendFlg = 0;
		WriteToDBUG_USART(p,len);
		DbugFlg = MaxFunctions;
	}
	else if(ChipId == 1)
	{
		len = AT45_PageSize(&at45[1]);
		adr = PageNumber * len;
		READ(1,p,adr,len);
		*(p + len++) = ChipId;
		memcpy(p + len,(unsigned char *)(&PageNumber),4);
		len += 4;
		crc = Crc16(p,len,0);
		crc = ShortEndianConvert(crc);
		memcpy(&p[len],(unsigned char *)(&crc),2);
		len += 2;
		while(!SendFlg)
		{
			WatchDog();
			CheckTimerGo();
		}
		SendFlg = 0;
		WriteToDBUG_USART(p,len);
		DbugFlg = MaxFunctions;
	}
	else if(ChipId == 2)
	{
		WriteToDBUG_USART((unsigned char*)&formated,strlen(formated));
		ChipId = 3;
	}
}

void WriteTicketHeadAndSndResult(void)
{
	unsigned short crc,crctmp;
	
	crc = Crc16(TicketHeadBuf,dbug_len - 2,0);
	memcpy((unsigned char *)(&crctmp),&TicketHeadBuf[dbug_len - 2],2);
	crctmp = ShortEndianConvert(crctmp);
	if(crc == crctmp)
	{
//		WRITE(CONFIG_ID,TicketHeadBuf,TICKET_HEAD_ADRESS * AT45_PageSize(&at45[0]),TicketHeadLen);
		WRITE(CONFIG_ID,TicketHeadBuf,TICKET_HEAD_ADRESS * AT45_PageSize(&at45[0]),TicketHeadLen+1);
		WriteToDBUG_USART((unsigned char *)(&ticket_head_ok),strlen(ticket_head_ok));
		dbug_len = 0; 
		DbugSerial.receviceback = DbugReceviceCommand1;
	}
	else
	{
		dbug_len = 0;
		WriteToDBUG_USART((unsigned char *)(&ticket_head_err),strlen(ticket_head_err));
	}
}

void WriteGcodeData(void)
{
	unsigned short crc,crctmp;
	unsigned int adr;
	
	crc = Crc16(TicketHeadBuf,528,0);
	memcpy((unsigned char *)(&crctmp),&TicketHeadBuf[528],2);
	if(crc == crctmp)
	{
		adr = (G_CODE_TICKET_ADR + GcodeCurNum) * AT45_PageSize(&at45[0]);
		WRITE(CONFIG_ID,TicketHeadBuf,adr,528);
		WriteToDBUG_USART((unsigned char *)(&gcode_data_ok),strlen(gcode_data_ok));
		GcodeCurNum++;
		dbug_len = 0;
		if(GcodeCurNum == GcodePackNum)
		{
			DbugSerial.receviceback = DbugReceviceCommand1;
		}
	}
	else
	{
		dbug_len = 0;
		WriteToDBUG_USART((unsigned char *)(&gcode_data_err),strlen(gcode_data_err));
	}
}

#ifdef NEW_VOICE
unsigned int voiceLen;
unsigned int gCodeLen;
unsigned int voiceSta;
unsigned int gCodeSta;
unsigned short newRevLen;
void revListLength(unsigned char* s,unsigned int len,unsigned char err)
{
	unsigned char tmp;
	
	while(len--)
	{
		tmp = *s;
		if(printStruct.len < 10)
		{
			printStruct.buf[printStruct.len++] = tmp;
		}
		else if(printStruct.len == 2)
		{
			if((printStruct.buf[0] == 0) && (printStruct.buf[1] == 0))
			{
				voiceFlg = VOICE_LIST_COMPLETE_OK;
			}
		}
		if(printStruct.len == 10)
		{
			voiceFlg = VOICE_FLG_R_LENGTH_OK;
		}
		s++;
	}
}

void revListData(unsigned char* s,unsigned int len,unsigned char err)
{
	unsigned char tmp;
	
	//格式:长度(2B,偏移+数据) + 表中的偏移(4B) + 数据(...) + CRC(2B)
	while(len--)
	{
		tmp = *s;
		if(printStruct.len == 0)
		{
			newRevLen = tmp;
			newRevLen <<= 8;
			printStruct.buf[printStruct.len++] = tmp;
		}
		else if(printStruct.len == 1)
		{
			newRevLen += tmp;
			printStruct.buf[printStruct.len++] = tmp;
		}
		else if((printStruct.len >= 2) && (printStruct.len < (newRevLen + 4)))
		{
			if(newRevLen == 0)
			{
				voiceFlg = VOICE_LIST_COMPLETE_OK;
			}
			printStruct.buf[printStruct.len++] = tmp;
		}
		if((newRevLen != 0) && (printStruct.len == (newRevLen + 4)))
		{
			printStruct.len = newRevLen;
			newRevLen = 0;
			if(voiceFlg == VOICE_FLG_WAIT_VDATA)
			{
				voiceFlg = VOICE_FLG_R_LIST_OK;
			}
			else if(voiceFlg == VOICE_FLG_WAIT_GDATA)
			{
				voiceFlg = VOICE_FLG_R_GLIST_OK;
			}
		}
	}
}

void sndRequestListLength(void)
{
	DbugSerial.receviceback = revListLength;
	printStruct.len = 0;
	WriteToDBUG_USART((unsigned char*)&requestListLength,strlen(requestListLength));
	voiceFlg = VOICE_FLG_S_LENGTH_WAIT;
}

void sndRequstVoiceList(void)
{
	unsigned int tmp,j;
	unsigned short crc;
	
	if(voiceLen == 0)
	{
		voiceFlg = VOCIE_FLG_R_GLIST;
		return;
	}
	if((voiceLen - voiceSta) > 528)
	{
		j = 528;
	}
	else if(voiceLen <= voiceSta)
	{
		voiceFlg = VOCIE_FLG_R_GLIST;
	}
	else
	{
		j = voiceLen - voiceSta;
	}
	printStruct.buf[0] = 0;
	printStruct.buf[1] = 9;
	printStruct.buf[2] = 'v';
	tmp = IntEndianConvert(voiceSta);
	memcpy(&printStruct.buf[3],(unsigned char*)(&tmp),4);
	j = IntEndianConvert(j);
	memcpy(&printStruct.buf[7],(unsigned char*)(&j),4);
	crc = Crc16(printStruct.buf,11,0);
	crc = ShortEndianConvert(crc);
	memcpy(&printStruct.buf[11],(unsigned char*)(&crc),2);
	DbugSerial.receviceback = revListData;
	printStruct.len = 0;
	WriteToDBUG_USART(printStruct.buf,13);
	voiceFlg = VOICE_FLG_WAIT_VDATA;
}

void sndGCodeList(void)
{
	unsigned int tmp,j;
	unsigned short crc;
	
	if(gCodeLen == 0)
	{
		voiceFlg = VOICE_LIST_COMPLETE;
		return;
	}
	if((gCodeLen - gCodeSta) > 528)
	{
		j = 528;
	}
	else if(gCodeLen <= gCodeSta)
	{
		voiceFlg = VOICE_LIST_COMPLETE;
	}
	else
	{
		j = gCodeLen - gCodeSta;
	}
	printStruct.buf[0] = 0;
	printStruct.buf[1] = 9;
	printStruct.buf[2] = 'g';
	tmp = IntEndianConvert(gCodeSta);
	memcpy(&printStruct.buf[3],(unsigned char*)(&tmp),4);
	j = IntEndianConvert(j);
	memcpy(&printStruct.buf[7],(unsigned char*)(&j),4);
	crc = Crc16(printStruct.buf,11,0);
	crc = ShortEndianConvert(crc);
	memcpy(&printStruct.buf[11],(unsigned char*)(&crc),2);
	DbugSerial.receviceback = revListData;
	printStruct.len = 0;
	WriteToDBUG_USART(printStruct.buf,13);
	voiceFlg = VOICE_FLG_S_GDATA_WAIT;
}

void revVoiceListDispose(void)
{
	unsigned short crc,crctmp,len;
	unsigned int adr,offset;
	At45* pAt45;
	
	pAt45 = &at45[0];
	if(((voiceSta + (printStruct.len - 4)) != voiceLen) && (printStruct.len != (528 + 4)))
	{
		voiceFlg = VOICE_FLG_R_LENGTH;
		return;
	}
	memcpy((unsigned char*)(&offset),&printStruct.buf[2],4);
	offset = IntEndianConvert(offset);
	if(offset != voiceSta)
	{
		voiceFlg = VOICE_FLG_R_LENGTH;
		return;
	}
	memcpy((unsigned char*)(&len),printStruct.buf,2);
	len = ShortEndianConvert(len);
	crc = Crc16(printStruct.buf,len + 2,0);
	crc = ShortEndianConvert(crc);
	memcpy((unsigned char*)(&crctmp),&printStruct.buf[printStruct.len + 2],2);
	if(crc != crctmp)
	{
		voiceFlg = VOICE_FLG_R_LIST;
		return;
	}
	len -= 4;
	adr = NEW_VOICE_ADR * AT45_PageSize(pAt45) + offset;
	WRITE(CONFIG_ID,&printStruct.buf[6],adr,len);
	voiceSta += len;
	if(voiceSta >= voiceLen)
	{
		if(gCodeLen == 0)
		{
			voiceFlg = VOICE_LIST_COMPLETE;
		}
		else
		{
			voiceFlg = VOCIE_FLG_R_GLIST;
		}
	}
	else
	{
		voiceFlg = VOICE_FLG_R_LIST;
	}
}
void revGCodelistDispose(void)
{
	unsigned short crc,crctmp,len;
	unsigned int adr,offset;
	At45* pAt45;
	
	pAt45 = & at45[0];
	if(((gCodeSta + (printStruct.len - 4)) != gCodeLen) && (printStruct.len != (528 + 4)))
	{
		voiceFlg = VOICE_FLG_R_LENGTH;
		return;
	}
	memcpy((unsigned char*)(&offset),&printStruct.buf[2],4);
	offset = IntEndianConvert(offset);
	if(offset != gCodeSta)
	{
		voiceFlg = VOCIE_FLG_R_GLIST;
		return;
	}
	memcpy((unsigned char*)(&len),printStruct.buf,2);
	len = ShortEndianConvert(len);
	crc = Crc16(printStruct.buf,len + 2,0);
	crc = ShortEndianConvert(crc);
	memcpy((unsigned char*)(&crctmp),&printStruct.buf[printStruct.len + 2],2);
	if(crc != crctmp)
	{
		voiceFlg = VOCIE_FLG_R_GLIST;
		return;
	}
	len -= 4;
	adr = G_CODE_TICKET_ADR *  AT45_PageSize(pAt45) + offset;
	WRITE(CONFIG_ID,&printStruct.buf[6],adr,len);
	voiceSta += len;
	if(voiceSta >= voiceLen)
	{
		voiceFlg = VOICE_LIST_COMPLETE;
	}
	else
	{
		voiceFlg = VOCIE_FLG_R_GLIST;
	}
}

static unsigned char voiceCheckFlg = 0;

void voiceTimeout(void)
{
	if(voiceCheckFlg == 1)
	{
		voiceCheckFlg = 2;
	}
}

void sndDownLoadComplete(void)
{
	printStruct.len = 0;
	WriteToDBUG_USART((unsigned char*)&downLoadComplete,strlen(downLoadComplete));
	voiceFlg = VOICE_LIST_COMPLETE_WAIT;
}

#endif

void DbugDispose(void)
{	
#ifdef NEW_VOICE
	int i;
	unsigned short crc,crctmp;
#endif
	InitDbugUSART_Func();
	AIC_DisableIT(AT91C_ID_IRQ0);  
	AIC_DisableIT(AT91C_ID_IRQ1);
	AIC_DisableIT(AT91C_ID_US0);
	AIC_DisableIT(AT91C_ID_US1);
	AIC_DisableIT(AT91C_ID_TC1);
	DbugSerial.sendback = DbugSendBack;
	dbug_flg = 1;
#ifdef NEW_VOICE
	voiceFlg = 0;
#endif
	while(dbug_flg)
	{
		WatchDog();
#ifdef NEW_VOICE
		if(voiceFlg == VOICE_FLG_R_DOWN_C_OK)
		{
			i = 5000;
			while(i--)
			{
			}
			voiceFlg = VOICE_FLG_R_LENGTH;
			if(pDbugTmr != (Tmr*)0)
			{
				DeleteTimer(pDbugTmr);
			}
			pDbugTmr = CreateTimer(TIMER_ONE,10000,voiceTimeout);
			StopTimer(pDbugTmr);
		}
		else if(voiceFlg == VOICE_FLG_R_LENGTH)
		{
			voiceCheckFlg = 0;
			sndRequestListLength();
		}
		else if(voiceFlg == VOICE_FLG_WAIT_LENGTH)
		{
			if(voiceCheckFlg == 0)
			{
				ResetTimer(pDbugTmr);
				voiceCheckFlg = 1;
			}
			else if(voiceCheckFlg == 2)
			{
				voiceFlg = VOICE_FLG_R_LENGTH;
			}
		}
		else if(voiceFlg == VOICE_FLG_R_LENGTH_OK)
		{
			memcpy((unsigned char*)(&crctmp),&printStruct.buf[8],2);
			crctmp = ShortEndianConvert(crctmp);
			crc = Crc16(printStruct.buf,8,0);
			if(crc == crctmp)
			{
				memcpy((unsigned char*)(&voiceLen),printStruct.buf,4);
				voiceLen = IntEndianConvert(voiceLen);
				memcpy((unsigned char*)(&gCodeLen),&printStruct.buf[4],4);
				gCodeLen = IntEndianConvert(gCodeLen);
				voiceSta = 0;
				gCodeSta = 0;
				if((voiceLen > (0x100 * 528)) || (gCodeLen > (0x100 * 528)))
				{
					voiceFlg = VOICE_FLG_R_LENGTH;
				}
				else
				{
					voiceFlg = VOICE_FLG_R_LIST;
				}
			}
			else
			{
				voiceFlg = VOICE_FLG_R_LENGTH;
			}
		}
		else if(voiceFlg == VOICE_FLG_R_LIST)
		{
			voiceCheckFlg = 0;
			sndRequstVoiceList();
		}
		else if(voiceFlg == VOICE_FLG_WAIT_VDATA)
		{
			if(voiceCheckFlg == 0)
			{
				ResetTimer(pDbugTmr);
				voiceCheckFlg = 1;
			}
			else if(voiceCheckFlg == 2)
			{
				voiceFlg = VOICE_FLG_R_LIST;
			}
		}
		else if(voiceFlg == VOICE_FLG_R_LIST_OK)
		{
			revVoiceListDispose();
		}
		else if(voiceFlg == VOCIE_FLG_R_GLIST)
		{
			voiceCheckFlg = 0;
			sndGCodeList();
		}
		else if(voiceFlg == VOICE_FLG_WAIT_GDATA)
		{
			if(voiceCheckFlg == 0)
			{
				ResetTimer(pDbugTmr);
				voiceCheckFlg = 1;
			}
			else if(voiceCheckFlg == 2)
			{
				voiceFlg = VOCIE_FLG_R_GLIST;
			}
		}
		else if(voiceFlg == VOICE_FLG_R_GLIST_OK)
		{
			revGCodelistDispose();
		}
		else if(voiceFlg == VOICE_LIST_COMPLETE)
		{
			sndDownLoadComplete();
		}
		else if(voiceFlg == VOICE_LIST_COMPLETE_OK)
		{
			break;
		}
#endif
		if(DbugFlg == 1)
		{
			DbugFlg = 0;
			SndRecordToDbugUsart();
		}
		else if(DbugFlg == 2)
		{
			DbugFlg = 0;
			SndLogToDbugUsart();
		}
		else if(DbugFlg == 3)
		{
			SndDataAllUsart();
		}
		else if(DbugFlg == 4)
		{
			DbugFlg = 0;
			WriteTicketHeadAndSndResult();
		}
		else if(DbugFlg == 5)
		{
			DbugFlg = 0;
			WriteGcodeData();
		}
		if((FormatFlg & 0x11) == 0x01)
		{
			FormatConfig();
			FormatFlg |= 0x10;
		}
		else if((FormatFlg & 0x22) == 0x02)
		{
			FormatRecord();
			FormatFlg |= 0x20;
		}
		else if((FormatFlg & 0x44) == 0x04)
		{
			FormatProtected();
			FormatFlg |= 0x40;
		}
		else if((FormatFlg & 0x88) == 0x08)
		{
			FormatLog();
			FormatFlg |= 0x80;
		}
		else if((FormatFlg & 0x300) == 0x100)
		{
			FormatAll();
			FormatFlg |= 0x200;
		}
		CheckTimerGo();
	}
	while(1)
	{
	}
}


extern	unsigned int IntEndianConvert(unsigned int val);

#ifdef USE_DBUG
static unsigned int ComPumpErr;
extern	unsigned char dbgu_buf[];

void InputToDbugUsart(unsigned char *s,unsigned short crc)
{
	unsigned int i,j,length,len;
	unsigned short crctmp;
	
	len = s[5];
	len += 6;
	ComPumpErr++;
	length = 0;
	for(j = 0; j < len; j++)
	{
		i = sprintf((char *)(&dbgu_buf[length]),"%.2X",s[j]);
		length += i;
		dbgu_buf[length++] = ' ';
	}
	dbgu_buf[length++] = '-';
	dbgu_buf[length++] = '-';
	crctmp = s[len + 1];
	crctmp <<= 8;
	crctmp |= s[len];
	i = sprintf((char *)(&dbgu_buf[length]),"%.4X",crctmp);
	length += i;
	dbgu_buf[length++] = '-';
	dbgu_buf[length++] = '-';
	i = sprintf((char *)(&dbgu_buf[length]),"%.4X",crc);
	length += i;
	dbgu_buf[length++] = '|';
	i = sprintf((char *)(&dbgu_buf[length]),"%0.10lu",ComPumpErr);
	length += i;
	dbgu_buf[length++] = '-';
	dbgu_buf[length++] = '-';
	i = sprintf((char *)(&dbgu_buf[length]),"%.4lu",SysTime.year);
	length += i;
	dbgu_buf[length++] = '/';
	i = sprintf((char *)(&dbgu_buf[length]),"%.2lu",SysTime.month);
	length += i;
	dbgu_buf[length++] = '/';
	i = sprintf((char *)(&dbgu_buf[length]),"%.2lu",SysTime.day);
	length += i;
	dbgu_buf[length++] = ' ';
	i = sprintf((char *)(&dbgu_buf[length]),"%.2lu",SysTime.hour);
	length += i;
	dbgu_buf[length++] = ':';
	i = sprintf((char *)(&dbgu_buf[length]),"%.2lu",SysTime.minute);
	length += i;
	dbgu_buf[length++] = ':';
	i = sprintf((char *)(&dbgu_buf[length]),"%.2lu",SysTime.second);
	length += i;
	dbgu_buf[length++] = '\n';
	dbgu_buf[length++] = '\r';
	WriteToDBUG_USART(dbgu_buf,length);
}

unsigned int UsartError,LastUsartError;

void SendError(void)
{
	if(UsartError != 0)
	{
		dbug_printf("%s%08x\n\r","UsartError:0x",UsartError);
		UsartError = 0;
	}
}

#endif