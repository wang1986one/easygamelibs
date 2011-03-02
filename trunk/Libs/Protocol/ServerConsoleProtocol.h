/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerConsoleProtocol.h                                  */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#pragma pack(push)
#pragma pack(4)

enum SERVER_CONSOLE_MSG
{
	SC_MSG_QUERY_SERVER_STATUS=0x1000,				//��ȡ������״̬
	SC_MSG_QUERY_ALL_SERVER_STATUS,
	SC_MSG_QUERY_SERVER_STATUS_RESULT,

	SC_MSG_SET_SERVER_STATUS,						//���÷�����״̬
	SC_MSG_SET_SERVER_STATUS_RESULT,
	
	SC_MSG_LINK_LOG,								//��������Log
	SC_MSG_UNLINK_LOG,								//�رս���Log
	SC_MSG_SERVER_LOG,								//������Log

	SC_MSG_EXEC_COMMAND,							//ִ������

	SC_MSG_GET_SERVER_STATUS_FORMAT_INFO,			//��ȡ������״̬��ʽ��Ϣ
	SC_MSG_GET_SERVER_STATUS_FORMAT_INFO_RESULT,
};

enum SC_SST_QUERY_SERVER_STATUS
{
	SC_SST_QSS_STATUS_ID=1,
};


enum SC_SST_SERVER_LOG
{
	SC_SST_SL_LOG_STR=1,
};

enum SC_SST_EXEC_COMMAND
{
	SC_SST_EC_COMMAND_STR=1,
};



enum SC_SST_SERVER_STATUS
{
	SC_SST_SS_CLIENT_COUNT=1,
	SC_SST_SS_CYCLE_TIME,
	SC_SST_SS_TCP_RECV_FLOW,
	SC_SST_SS_TCP_SEND_FLOW,
	SC_SST_SS_UDP_RECV_FLOW,
	SC_SST_SS_UDP_SEND_FLOW,
	SC_SST_SS_TCP_RECV_COUNT,
	SC_SST_SS_TCP_SEND_COUNT,
	SC_SST_SS_UDP_RECV_COUNT,
	SC_SST_SS_UDP_SEND_COUNT,
	SC_SST_SS_PROGRAM_VERSION,
};

enum SC_SST_SERVER_STATUS_FORMAT_INFO_LIST
{
	SC_SST_SSFIL_SERVER_STATUS_FORMAT_INFO=1,
};

enum SC_SST_SERVER_STATUS_FORMAT_INFO
{
	SC_SST_SSFI_STATUS_ID=1,
	SC_SST_SSFI_FORMAT_TYPE,
	SC_SST_SSFI_NAME,
};

#pragma pack(pop)