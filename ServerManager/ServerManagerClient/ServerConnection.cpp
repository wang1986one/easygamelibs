/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerConnection.cpp                                     */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


#define ASSEMBEL_BUFFER_SIZE	(MAX_DATA_PACKET_SIZE*2)

CServerConnection::CServerConnection(CServerManagerClientView * pView,LPCTSTR szServerAddress,UINT ServerPort)
{
	m_pView=pView;
	m_AssembleBuffer.Create(ASSEMBEL_BUFFER_SIZE);
	m_ServerAddress=szServerAddress;
	m_ServerPort=ServerPort;
	m_ConnectTimer.SaveTime(CEasyTimer::GetTime()+GetRand(-CONNECT_WAIT_TIME,CONNECT_WAIT_TIME));
	m_ServiceInfoFetchTimer.SaveTime(CEasyTimer::GetTime()+GetRand(-CONNECT_WAIT_TIME,CONNECT_WAIT_TIME));
}

CServerConnection::~CServerConnection(void)
{
}


void CServerConnection::OnConnection(BOOL IsSucceed)
{
	if(IsSucceed)
	{
		PrintLog("%s:%u已连接",
			GetRemoteAddress().GetIPString(),
			GetRemoteAddress().GetPort());
	}
	else
	{
		PrintLog("%s:%u连接失败",
			GetRemoteAddress().GetIPString(),
			GetRemoteAddress().GetPort());
	}
}

void CServerConnection::OnDisconnection()
{
	PrintLog("%s:%u已断开",
		GetRemoteAddress().GetIPString(),
		GetRemoteAddress().GetPort());

	if(m_pView)
	{
		m_pView->DisconnectNotify(m_ServerAddress);
	}
}

void CServerConnection::OnRecvData(const CEasyBuffer& DataBuffer)
{
	if(!m_AssembleBuffer.PushBack(DataBuffer.GetBuffer(),DataBuffer.GetUsedSize()))
	{
		PrintLog("装配缓冲已满!");
		Disconnect();
		return;
	}
	while(m_AssembleBuffer.GetUsedSize()>=CSmartStruct::HEAD_SIZE)
	{
		int Type=CSmartValue::GetTypeFromData(m_AssembleBuffer.GetBuffer(),m_AssembleBuffer.GetUsedSize());
		int MsgLen=CSmartStruct::GetDataLenFromData(m_AssembleBuffer.GetBuffer(),m_AssembleBuffer.GetUsedSize());
		if(Type==CSmartValue::VT_STRUCT&&m_AssembleBuffer.GetUsedSize()>=MsgLen)
		{
			CSmartStruct Msg(m_AssembleBuffer.GetBuffer(),MsgLen,false);
			OnMsg(Msg);
			m_AssembleBuffer.PopFront(NULL,MsgLen);
		}
		else
		{
			break;
		}
	}
}

int CServerConnection::Update(int ProcessPacketLimit)
{
	int ProcessCount=CNetConnection::Update(ProcessPacketLimit);
	if(IsDisconnected())
	{
		if(m_ConnectTimer.IsTimeOut(CONNECT_WAIT_TIME))
		{
			m_ConnectTimer.SaveTime();
			Destory();

			CIPAddress Address((LPCTSTR)m_ServerAddress,m_ServerPort);
			Connect(Address,CONNECT_TIME);
		}
	}

	if(IsConnected())
	{
		if(m_ServiceInfoFetchTimer.IsTimeOut(SERVICE_INFO_FETCH_TIME))
		{
			m_ServiceInfoFetchTimer.SaveTime();
			QueryServiceList();
			QueryNetAdapterInfo();
		}
	}

	return ProcessCount;
}

void CServerConnection::PrintLog(LPCTSTR szFormat,...)
{
	if(m_pView)
	{
		va_list vl;

		va_start(vl,szFormat);
		m_pView->PrintLogVL(szFormat,vl);
		va_end(vl);
	}
}

void CServerConnection::OnMsg(CSmartStruct& Msg)
{
	UINT MsgID=Msg.GetMember(SSMM_MSG_TYPE_ID);
	switch(MsgID)
	{
	case SM_MSG_GET_SERVICE_INFO_LIST_RESULT:
		{
			void * Pos=Msg.GetFirstMemberPosition();
			while(Pos)
			{
				WORD MemberID;
				CSmartValue Value=Msg.GetNextMember(Pos,MemberID);
				switch(MemberID)
				{
				case SGSILR_TOTAL_COUNT:
					break;
				case SGSILR_START_INDEX:
					break;
				case SGSILR_SERVICE_INFO:
					{
						CSmartStruct ServiceInfo=Value;

						LPCTSTR ServiceName=ServiceInfo.GetMember(SSIL_NAME);
						UINT Index=ServiceInfo.GetMember(SSIL_INDEX);
						LPCTSTR ImageFilePath=ServiceInfo.GetMember(SSIL_IMAGE_FILE_PATH);
						UINT Status=ServiceInfo.GetMember(SSIL_STATUS);
						float CPUUsed=ServiceInfo.GetMember(SSIL_CPU_USED);
						UINT MemoryUsed=ServiceInfo.GetMember(SSIL_MEMORY);
						UINT VirtualMemoryUsed=ServiceInfo.GetMember(SSIL_VIRTUAL_MEMORY);
						time_t ImageFileTime=ServiceInfo.GetMember(SSIL_IMAGE_FILE_TIME);
						if(m_pView)
						{
							m_pView->SetServiceInfo(Index,m_ServerAddress,ServiceName,ImageFilePath,ImageFileTime,Status,CPUUsed,MemoryUsed,VirtualMemoryUsed);
						}
					}
					break;
				}
			}
		}
		break;
	case SM_MSG_GET_PROCESS_INFO_LIST_RESULT:
		
		break;
	case SM_MSG_STARTUP_SERVICE_RESULT:
		{
			UINT ServiceIndex=Msg.GetMember(SSUS_SERVICE_INDEX);
			int Result=Msg.GetMember(SSMM_MSG_RESULT);
			PrintLog("启动服务器[%s]上的服务[%u],结果[%d]",
				(LPCTSTR)m_ServerAddress,
				ServiceIndex,
				Result);
		}		
		break;
	case SM_MSG_SHUTDOWN_SERVICE_RESULT:
		{
			UINT ServiceIndex=Msg.GetMember(SSDS_SERVICE_INDEX);
			int Result=Msg.GetMember(SSMM_MSG_RESULT);
			PrintLog("关闭服务器[%s]上的服务[%u],结果[%d]",
				(LPCTSTR)m_ServerAddress,
				ServiceIndex,
				Result);
		}
		break;
	case SM_MSG_BROWSE_SERVICE_DIR_RESULT:
		{
			int Result=Msg.GetMember(SSMM_MSG_RESULT);
			if(Result==SSMRT_SUCCEED)
			{
				LPVOID Pos=Msg.GetFirstMemberPosition();
				LPCTSTR szDir;
				bool IsFinish=false;

				while(Pos)
				{
					WORD MemberID;
					CSmartValue Value=Msg.GetNextMember(Pos,MemberID);
					switch(MemberID)
					{
					case SST_SMBSDR_LEFT_COUNT:
						{
							UINT LeftCount=Value;
							if(LeftCount==0)
							{
								IsFinish=true;
							}
						}
						break;
					case SST_SMBSDR_DIRECTORY:
						{
							szDir=(LPCTSTR)Value;						
						}
						break;
					case SST_SMBSDR_FILE_INFO:
						{
							CSmartStruct FileInfo=Value;
							if(m_pView)
							{
								m_pView->OnBrowseResult(szDir,
									FileInfo.GetMember(SST_SMFI_FILE_NAME),
									FileInfo.GetMember(SST_SMFI_FILE_ATTRIBUTE),
									FileInfo.GetMember(SST_SMFI_FILE_SIZE),
									FileInfo.GetMember(SST_SMFI_FILE_CREATE_TIME),
									FileInfo.GetMember(SST_SMFI_FILE_LAST_ACCESS_TIME),
									FileInfo.GetMember(SST_SMFI_FILE_LAST_WRITE_TIME));
							}
						}
						break;
					}
				}
				if(IsFinish)
				{
					m_pView->OnBrowseFinish();
				}
			}		
		}
		break;
	case SM_MSG_START_DOWNLOAD_SERVICE_FILE_RESULT:
		{
			int Result=Msg.GetMember(SSMM_MSG_RESULT);
			UINT TotalSize=Msg.GetMember(SST_SMSDSFR_TOTAL_SIZE);
			UINT OriginSize=Msg.GetMember(SST_SMSDSFR_ORIGIN_TOTAL_SIZE);
			LPCTSTR pPackProp=Msg.GetMember(SST_SMSDSFR_PACK_PROP);
			UINT PackPropSize=Msg.GetMember(SST_SMSDSFR_PACK_PROP).GetLength();
			time_t LastWriteTime=Msg.GetMember(SST_SMSDSFR_FILE_LAST_WRITE_TIME);

			m_pView->GetWorkDirBrowser()->OnStartDownloadResult(Result,TotalSize,OriginSize,pPackProp,PackPropSize,LastWriteTime);
		}
		break;
	case SM_MSG_QUERY_DOWNLOAD_SERVICE_FILE_DATA_RESULT:
		{
			int Result=Msg.GetMember(SSMM_MSG_RESULT);
			UINT TotalSize=Msg.GetMember(SST_SMQDSFDR_TOTAL_SIZE);
			UINT LeftSize=Msg.GetMember(SST_SMQDSFDR_LEFT_SIZE);
			CSmartValue FileData=Msg.GetMember(SST_SMQDSFDR_DATA);
			m_pView->GetWorkDirBrowser()->OnDownloadData(Result,TotalSize,LeftSize,(LPCTSTR)FileData,FileData.GetLength());
		}
		break;
	case SM_MSG_START_UPLOAD_SERVICE_FILE_RESULT:
		{
			int Result=Msg.GetMember(SSMM_MSG_RESULT);
			if(Result==SSMRT_SUCCEED)
			{
				m_pView->GetWorkDirBrowser()->ContinueUploadFile(true);
			}
			else
			{
				m_pView->GetWorkDirBrowser()->ContinueUploadFile(false);
			}
		}
		break;
	case SM_MSG_SEND_UPLOAD_SERVICE_FILE_DATA_RESULT:
		{
			int Result=Msg.GetMember(SSMM_MSG_RESULT);
			if(Result==SSMRT_SUCCEED)
			{
				m_pView->GetWorkDirBrowser()->ContinueUploadFile(true);
			}
			else
			{
				m_pView->GetWorkDirBrowser()->ContinueUploadFile(false);
			}
		}
		break;
	case SM_MSG_CREATE_DIR_RESULT:
		{
			int Result=Msg.GetMember(SSMM_MSG_RESULT);

			m_pView->GetWorkDirBrowser()->OnCreateDirResult(Result);
		}
		break;
	case SM_MSG_DELETE_FILE_RESULT:
		{
			int Result=Msg.GetMember(SSMM_MSG_RESULT);

			if(Result==SSMRT_SUCCEED)
			{
				PrintLog("删除文件成功");
			}
			else
			{
				PrintLog("删除文件失败");
			}
			m_pView->GetWorkDirBrowser()->OnDeleteFileResult(Result);
		}
		break;
	case SM_MSG_CREATE_PROCESS_RESULT:
		{
			int Result=Msg.GetMember(SSMM_MSG_RESULT);

			if(Result==SSMRT_SUCCEED)
			{
				PrintLog("执行程序成功");
			}
			else
			{
				PrintLog("执行程序失败");
			}
		}
		break;
	case SM_MSG_GET_NET_ADAPTER_INFO_RESULT:
		{
			void * Pos=Msg.GetFirstMemberPosition();
			while(Pos)
			{
				WORD MemberID;
				CSmartValue Value=Msg.GetNextMember(Pos,MemberID);
				switch(MemberID)
				{
				case SST_SMGNAIR_TOTAL_COUNT:
					break;
				case SST_SMGNAIR_START_INDEX:
					break;
				case SST_SMGNAIR_NET_ADAPTER_INFO:
					{
						CSmartStruct AdapterInfo=Value;
						CIPAddress ServiceIP(m_ServerAddress,m_ServerPort);

						UINT AdapterIndex=AdapterInfo.GetMember(SST_NAI_INDEX);
						DWORD IPAddress=AdapterInfo.GetMember(SST_NAI_IP_ADDRESS);
						float SendFlux=AdapterInfo.GetMember(SST_NAI_SEND_FLUX);
						float ResvFlux=AdapterInfo.GetMember(SST_NAI_RECV_FLUX);
						
						if(m_pView)
						{
							if(IPAddress==ServiceIP.GetIP())
							{
								m_pView->SetInternetAdapterInfo(m_ServerAddress,SendFlux,ResvFlux);
							}
							else
							{
								m_pView->SetIntranetAdapterInfo(m_ServerAddress,SendFlux,ResvFlux);
							}
						}
					}
					break;
				}
			}
		}
		break;
	case SM_MSG_EXECUTE_SCRIPT_RESULT:
		{
			int Result=Msg.GetMember(SSMM_MSG_RESULT);
			PrintLog("运行脚本结果[%d]",Result);
		}
		break;

	}
}

void CServerConnection::SendMsg(CSmartStruct& Msg)
{
	QuerySend(Msg.GetData(),Msg.GetDataLen());
}

void CServerConnection::SendMsg(UINT MsgID)
{
	char SendBuffer[32];
	CSmartStruct Msg(SendBuffer,32,true);

	Msg.AddMember(SSMM_MSG_TYPE_ID,MsgID);
	QuerySend(Msg.GetData(),Msg.GetDataLen());
}


void CServerConnection::QueryServiceList()
{
	SendMsg(SM_MSG_GET_SERVICE_INFO_LIST);
}

void CServerConnection::QueryStartupService(UINT ServiceIndex)
{
	char SendBuffer[32];
	CSmartStruct Msg(SendBuffer,32,true);

	PrintLog("请求启动服务器[%s]上的服务[%u]",
		(LPCTSTR)m_ServerAddress,
		ServiceIndex);
	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_STARTUP_SERVICE);
	Msg.AddMember(SSUS_SERVICE_INDEX,ServiceIndex);
	QuerySend(Msg.GetData(),Msg.GetDataLen());
}
void CServerConnection::QueryShutDownService(UINT ServiceIndex)
{
	char SendBuffer[32];
	CSmartStruct Msg(SendBuffer,32,true);

	PrintLog("请求关闭服务器[%s]上的服务[%u]",
		(LPCTSTR)m_ServerAddress,
		ServiceIndex);
	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_SHUTDOWN_SERVICE);
	Msg.AddMember(SSDS_SERVICE_INDEX,ServiceIndex);
	QuerySend(Msg.GetData(),Msg.GetDataLen());
}
void CServerConnection::QueryBrowseWorkDir(UINT ServiceIndex,LPCTSTR Dir)
{
	char SendBuffer[32+MAX_PATH];
	CSmartStruct Msg(SendBuffer,32+MAX_PATH,true);

	PrintLog("请求浏览服务器[%s]上的服务[%u]的目录[%s]",
		(LPCTSTR)m_ServerAddress,
		ServiceIndex,Dir);
	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_BROWSE_SERVICE_DIR);
	Msg.AddMember(SSDS_SERVICE_INDEX,ServiceIndex);
	Msg.AddMember(SST_SMBSD_DIRECTORY_NAME,Dir);
	QuerySend(Msg.GetData(),Msg.GetDataLen());
}

void CServerConnection::QueryStartDownload(UINT ServiceIndex,LPCTSTR SourceFilePath,LPCTSTR TargetFilePath)
{
	char SendBuffer[128+MAX_PATH];
	CSmartStruct Msg(SendBuffer,128+MAX_PATH,true);

	PrintLog("请求服务器[%s]上的服务[%u]下载文件[%s]到[%s]",
		(LPCTSTR)m_ServerAddress,
		ServiceIndex,
		SourceFilePath,TargetFilePath);

	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_START_DOWNLOAD_SERVICE_FILE);
	Msg.AddMember(SST_SMSDSF_SERVICE_INDEX,ServiceIndex);
	Msg.AddMember(SST_SMSDSF_FILE_PATH,SourceFilePath);	

	QuerySend(Msg.GetData(),Msg.GetDataLen());
}

void CServerConnection::QueryDownloadData(UINT ServiceIndex)
{
	char SendBuffer[128];
	CSmartStruct Msg(SendBuffer,sizeof(SendBuffer),true);

	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_QUERY_DOWNLOAD_SERVICE_FILE_DATA);
	Msg.AddMember(SST_SMQDSFD_SERVICE_INDEX,ServiceIndex);
	QuerySend(Msg.GetData(),Msg.GetDataLen());
}

void CServerConnection::QueryStartUpload(UINT ServiceIndex,LPCTSTR SourceFilePath,LPCTSTR TargetFilePath,time_t FileTime,UINT TotalSize,UINT OrgTotalSize,LPVOID pPackProps,UINT PackPropSize)
{
	char SendBuffer[128+MAX_PATH];
	CSmartStruct Msg(SendBuffer,128+MAX_PATH,true);

	PrintLog("请求服务器[%s]上的服务[%u]上传文件[%s]到[%s]",
		(LPCTSTR)m_ServerAddress,
		ServiceIndex,
		SourceFilePath,TargetFilePath);

	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_START_UPLOAD_SERVICE_FILE);
	Msg.AddMember(SST_SMSUSF_SERVICE_INDEX,ServiceIndex);
	Msg.AddMember(SST_SMSUSF_FILE_PATH,TargetFilePath);
	Msg.AddMember(SST_SMSUSF_TOTAL_SIZE,TotalSize);
	Msg.AddMember(SST_SMSUSF_ORIGIN_TOTAL_SIZE,OrgTotalSize);
	Msg.AddMember(SST_SMSUSF_PACK_PROP,(LPCTSTR)pPackProps,PackPropSize);
	Msg.AddMember(SST_SMSUSF_FILE_LAST_WRITE_TIME,FileTime);

	QuerySend(Msg.GetData(),Msg.GetDataLen());
}

void CServerConnection::QueryContinueUpload(UINT ServiceIndex,UINT TotalSize,UINT LeftSize,LPVOID pData,UINT DataSize)
{
	char SendBuffer[128+MAX_PATH+MAX_DATA_TRANSFER_SIZE];
	CSmartStruct Msg(SendBuffer,128+MAX_PATH+MAX_DATA_TRANSFER_SIZE,true);

	//PrintLog("请求服务器[%s]上的服务[%u]上传文件数据总长=%u,剩余=%u,原始=%u,当前=%u",
	//	(LPCTSTR)m_ServerAddress,
	//	ServiceIndex,
	//	TotalSize,LeftSize,OrgDataSize,DataSize);

	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_SEND_UPLOAD_SERVICE_FILE_DATA);
	Msg.AddMember(SST_SMSUSFD_SERVICE_INDEX,ServiceIndex);
	Msg.AddMember(SST_SMSUSFD_TOTAL_SIZE,TotalSize);
	Msg.AddMember(SST_SMSUSFD_LEFT_SIZE,LeftSize);	
	Msg.AddMember(SST_SMSUSFD_DATA,(LPCTSTR)pData,DataSize);

	QuerySend(Msg.GetData(),Msg.GetDataLen());
}


void CServerConnection::QueryCreateDir(UINT ServiceIndex,LPCTSTR Dir)
{
	char SendBuffer[128+MAX_PATH];
	CSmartStruct Msg(SendBuffer,128+MAX_PATH,true);

	PrintLog("请求服务器[%s]上的服务[%u]创建目录[%s]",
		(LPCTSTR)m_ServerAddress,
		ServiceIndex,
		Dir);

	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_CREATE_DIR);
	Msg.AddMember(SST_SMCD_SERVICE_INDEX,ServiceIndex);
	Msg.AddMember(SST_SMCD_DIR_NAME,Dir);


	QuerySend(Msg.GetData(),Msg.GetDataLen());
}

void CServerConnection::QueryDeleteFile(UINT ServiceIndex,LPCTSTR FilePath)
{
	char SendBuffer[128+MAX_PATH];
	CSmartStruct Msg(SendBuffer,128+MAX_PATH,true);

	PrintLog("请求服务器[%s]上的服务[%u]删除文件[%s]",
		(LPCTSTR)m_ServerAddress,
		ServiceIndex,
		FilePath);

	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_DELETE_FILE);
	Msg.AddMember(SST_SMDF_SERVICE_INDEX,ServiceIndex);
	Msg.AddMember(SST_SMDF_FILE_PATH,FilePath);

	QuerySend(Msg.GetData(),Msg.GetDataLen());
}

void CServerConnection::QueryCreateProcess(UINT ServiceIndex,LPCTSTR FilePath,LPCTSTR WorkDir,LPCTSTR Param)
{
	char SendBuffer[128+MAX_PATH*3];
	CSmartStruct Msg(SendBuffer,sizeof(SendBuffer),true);

	PrintLog("请求服务器[%s]上的服务[%u]运行程序[%s %s]",
		(LPCTSTR)m_ServerAddress,
		ServiceIndex,
		FilePath,Param);

	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_CREATE_PROCESS);
	Msg.AddMember(SST_SMCP_SERVICE_INDEX,ServiceIndex);
	Msg.AddMember(SST_SMCP_IMAGE_FILE_PATH,FilePath);
	Msg.AddMember(SST_SMCP_WORK_DIR,WorkDir);
	Msg.AddMember(SST_SMCP_PARAM,Param);


	QuerySend(Msg.GetData(),Msg.GetDataLen());
}


void CServerConnection::QueryNetAdapterInfo()
{
	SendMsg(SM_MSG_GET_NET_ADAPTER_INFO);
}

void CServerConnection::QueryRunScript(UINT ServiceIndex,LPCTSTR FilePath)
{
	char SendBuffer[128+MAX_PATH];
	CSmartStruct Msg(SendBuffer,128+MAX_PATH,true);

	PrintLog("请求服务器[%s]上的服务[%u]运行脚本文件[%s]",
		(LPCTSTR)m_ServerAddress,
		ServiceIndex,
		FilePath);

	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_EXECUTE_SCRIPT);
	Msg.AddMember(SST_SMES_SERVICE_INDEX,ServiceIndex);
	Msg.AddMember(SST_SMES_SCRIPT_TYPE,SCRIPT_TYPE_FROM_FILE);
	Msg.AddMember(SST_SMES_SCRIPT,FilePath);

	QuerySend(Msg.GetData(),Msg.GetDataLen());
}