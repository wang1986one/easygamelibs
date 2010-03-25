/****************************************************************************/
/*                                                                          */
/*      文件名:    BaseProtocol.h                                           */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#pragma pack(push)
#pragma pack(4)

struct SMSG_HEADER			//基本消息头
{
	DWORD Size;				//消息包大小
	DWORD MsgID;				//消息ID
};

struct SMSG					//基本消息结构
{
	SMSG_HEADER	Header;
	char		Data[1];
};

enum SERVER_TYPE
{
	SRV_DBPROXY=1,					//数据库代理服务器
	SRV_STATUS,						//状态服务器
	SRV_STATUS_DISPATCH,			//状态服务器平衡负载服务器
	SRV_AUTH,						//认证服务器
	SRV_LOBBY,						//大厅服务器
	SRV_ROOM,						//房间服务器
	SRV_CHANNEL,					//频道服务器	
};


#define MAX_MSG_SIZE	4096

#pragma pack(pop)