#include "StdAfx.h"




IMPLEMENT_CLASS_INFO_STATIC(CServerManagerClient,CNetConnection);

CServerManagerClient::CServerManagerClient(void)
{
	FUNCTION_BEGIN;
	m_pManager=NULL;
	m_WantDelete=false;
	m_OriginSize=0;
	m_TransferLeftSize=0;
	m_IsInScriptExecute=false;
	
	FUNCTION_END;
}

CServerManagerClient::~CServerManagerClient(void)
{
	FUNCTION_BEGIN;
	FUNCTION_END;
}


BOOL CServerManagerClient::Init(UINT ID,CServerManagerService * pManager)
{
	FUNCTION_BEGIN;
	m_pManager=pManager;
	SetID(ID);
	SetServer(m_pManager->GetServer());

	if(m_AssembleBuffer.GetBufferSize()<ASSEMBEL_BUFFER_SIZE)
	{
		if(!m_AssembleBuffer.Create(ASSEMBEL_BUFFER_SIZE))
		{
			Log("创建%u大小的装配缓冲失败",ASSEMBEL_BUFFER_SIZE);
			return FALSE;
		}
	}
	if(!m_ScriptExecutor.Init(0,this))
	{
		Log("初始化脚本执行器失败");
		return FALSE;
	}
	m_AssembleBuffer.SetUsedSize(0);	
	m_WantDelete=false;
	m_OriginSize=0;
	m_TransferLeftSize=0;
	m_IsInScriptExecute=false;

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CServerManagerClient::Destory()
{
	FUNCTION_BEGIN;	

	m_TransferBuffer.Destory();
	m_PackProp.Destory();

	m_ScriptExecutor.Destory();

	CNetConnection::Destory();
	m_pManager=NULL;	

	FUNCTION_END;
}

void CServerManagerClient::OnConnection(BOOL IsSucceed)
{
	FUNCTION_BEGIN;
	if(IsSucceed)
	{
		Log("客户端%s:%u建立连接",
			GetRemoteAddress().GetIPString(),
			GetRemoteAddress().GetPort());
	}
	else
	{
		m_WantDelete=true;
	}
	FUNCTION_END;
}

void CServerManagerClient::OnDisconnection()
{
	FUNCTION_BEGIN;
	Log("客户端%s:%u断开连接",
		GetRemoteAddress().GetIPString(),
		GetRemoteAddress().GetPort());
	m_WantDelete=true;
		
	FUNCTION_END;
}

void CServerManagerClient::OnRecvData(const CEasyBuffer& DataBuffer)
{
	FUNCTION_BEGIN;	

	if(!m_AssembleBuffer.PushBack(DataBuffer.GetBuffer(),DataBuffer.GetUsedSize()))
	{
		Log("装配缓冲已满!");
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
	FUNCTION_END;
}

int CServerManagerClient::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;	
	int ProcessCount=CNetConnection::Update(ProcessPacketLimit);
	ProcessCount+=m_ScriptExecutor.Update(ProcessPacketLimit);

	return ProcessCount;
	FUNCTION_END;
	return 0;
}

void CServerManagerClient::OnMsg(CSmartStruct& Msg)
{
	FUNCTION_BEGIN;
	UINT MsgID=Msg.GetMember(SSMM_MSG_TYPE_ID);
	switch(MsgID)
	{
	case SM_MSG_GET_SERVICE_INFO_LIST:	
		{
			CEasyArray<SERVICE_INFO>& ServiceList=m_pManager->GetServiceInfoList();
			char Buffer[MAX_DATA_PACKET_SIZE];
			CSmartStruct Packet(Buffer,MAX_DATA_PACKET_SIZE,true);
			UINT StartIndex=0;
			do{
				Packet.Clear();
				Packet.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_GET_SERVICE_INFO_LIST_RESULT);
				Packet.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
				Packet.AddMember(SGSILR_TOTAL_COUNT,ServiceList.GetCount());
				Packet.AddMember(SGSILR_START_INDEX,StartIndex);
				StartIndex+=PackServiceInfo(ServiceList,StartIndex,Packet,SGSILR_SERVICE_INFO);
				SendMsg(Packet);
			}while(StartIndex<ServiceList.GetCount());
		}
		break;	
	case SM_MSG_GET_PROCESS_INFO_LIST:
		{
			CEasyArray<SERVICE_INFO>& ProcessList=m_pManager->GetProcessInfoList();
			char Buffer[MAX_DATA_PACKET_SIZE];
			CSmartStruct Packet(Buffer,MAX_DATA_PACKET_SIZE,true);
			UINT StartIndex=0;
			do{
				Packet.Clear();
				Packet.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_GET_PROCESS_INFO_LIST_RESULT);
				Packet.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
				Packet.AddMember(SGPILR_TOTAL_COUNT,ProcessList.GetCount());
				Packet.AddMember(SGPILR_START_INDEX,StartIndex);
				StartIndex+=PackServiceInfo(ProcessList,StartIndex,Packet,SGPILR_SERVICE_INFO);
				SendMsg(Packet);
			}while(StartIndex<ProcessList.GetCount());
		}
		break;
	case SM_MSG_STARTUP_SERVICE:
		{
			UINT ServiceIndex=Msg.GetMember(SSUS_SERVICE_INDEX);
			Log("收到请求启动服务[%u]",ServiceIndex);
			char Buffer[32];
			CSmartStruct Result(Buffer,32,true);
			Result.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_STARTUP_SERVICE_RESULT);
			Result.AddMember(SSUS_SERVICE_INDEX,ServiceIndex);
			if(m_pManager->StartupService(ServiceIndex))
			{
				Result.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);				
			}
			else
			{
				Result.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
			}
			SendMsg(Result);
		}
		break;
	case SM_MSG_SHUTDOWN_SERVICE:
		{
			UINT ServiceIndex=Msg.GetMember(SSDS_SERVICE_INDEX);
			Log("收到请求关闭服务[%u]",ServiceIndex);
			char Buffer[32];
			CSmartStruct Result(Buffer,32,true);
			Result.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_SHUTDOWN_SERVICE_RESULT);
			Result.AddMember(SSDS_SERVICE_INDEX,ServiceIndex);
			if(m_pManager->ShutdownService(ServiceIndex))
			{
				Result.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);				
			}
			else
			{
				Result.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
			}
			SendMsg(Result);
		}
		break;
	case SM_MSG_BROWSE_SERVICE_DIR:
		{
			UINT ServiceIndex=Msg.GetMember(SST_SMBSD_SERVICE_INDEX);
			LPCTSTR QueryDir=Msg.GetMember(SST_SMBSD_DIRECTORY_NAME);
			if(QueryDir&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
			{
				Log("请求服务[%u]目录[%s]的信息",
					ServiceIndex,
					QueryDir);

				SERVICE_INFO& ServiceInfo=m_pManager->GetServiceInfoList()[ServiceIndex];

				CEasyString SearchDir;
				CEasyString WorkDir=ServiceInfo.WorkDir;
				WorkDir=MakeFullPath(WorkDir+"\\");
				WorkDir.MakeUpper();
				SearchDir=WorkDir+QueryDir+"\\";
				SearchDir=MakeFullPath(SearchDir);
				SearchDir.MakeUpper();
				if(SearchDir.Find(WorkDir)!=-1)
				{
					if(SendFileList(ServiceIndex,WorkDir,SearchDir))
					{
						return;
					}
					else
					{
						Log("请求服务[%u]目录[%s]的信息,读取目录失败",
							ServiceIndex,
							(LPCTSTR)SearchDir);
					}
				}
				else
				{
					Log("请求服务[%u]目录[%s]的信息,目录非法",
						ServiceIndex,
						(LPCTSTR)SearchDir);
				}
			}
			Log("请求服务[%u]目录信息失败",
				ServiceIndex);

			char SendBuffer[64];
			CSmartStruct ReturnMsg(SendBuffer,64,true);
			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_BROWSE_SERVICE_DIR_RESULT);
			ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMBSD_DIRECTORY_NOT_EXIST);
			ReturnMsg.AddMember(SST_SMBSD_SERVICE_INDEX,ServiceIndex);			
			SendMsg(ReturnMsg);
			return;
		}
		break;
	case SM_MSG_START_DOWNLOAD_SERVICE_FILE:
		{
			UINT ServiceIndex=Msg.GetMember(SST_SMSDSF_SERVICE_INDEX);
			LPCTSTR szFilePath=(LPCTSTR)Msg.GetMember(SST_SMSDSF_FILE_PATH);

			char SendBuffer[MAX_PATH+128];
			CSmartStruct ReturnMsg(SendBuffer,sizeof(SendBuffer),true);
			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_START_DOWNLOAD_SERVICE_FILE_RESULT);			
			ReturnMsg.AddMember(SST_SMSDSFR_SERVICE_INDEX,ServiceIndex);
			ReturnMsg.AddMember(SST_SMSDSFR_FILE_PATH,szFilePath);

			if(szFilePath&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
			{
				CEasyString FilePath=m_pManager->GetServiceInfoList()[ServiceIndex].WorkDir;
				FilePath=FilePath+"\\"+szFilePath;

				CMainThread::GetInstance()->GetFileCache()->QueryGetFile(FilePath,this,ServiceIndex);

				
			}
			else
			{
				Log("请求服务[%u]下载文件[%s]失败",
					ServiceIndex,szFilePath);
				ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
				SendMsg(ReturnMsg);
			}
			
		}
		break;
	case SM_MSG_QUERY_DOWNLOAD_SERVICE_FILE_DATA:
		{
			UINT ServiceIndex=Msg.GetMember(SST_SMQDSFD_SERVICE_INDEX);


			UINT DataSize=MAX_DATA_TRANSFER_SIZE;
			if(m_TransferLeftSize<DataSize)
				DataSize=m_TransferLeftSize;
			BYTE * pDataBuffer=(BYTE *)m_TransferBuffer.GetBuffer()+m_TransferBuffer.GetUsedSize()-m_TransferLeftSize;
			m_TransferLeftSize-=DataSize;


			char SendBuffer[MAX_DATA_TRANSFER_SIZE+128];
			CSmartStruct ReturnMsg(SendBuffer,sizeof(SendBuffer),true);
			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_QUERY_DOWNLOAD_SERVICE_FILE_DATA_RESULT);
			ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
			ReturnMsg.AddMember(SST_SMQDSFDR_SERVICE_INDEX,ServiceIndex);
			ReturnMsg.AddMember(SST_SMQDSFDR_TOTAL_SIZE,m_TransferBuffer.GetUsedSize());
			ReturnMsg.AddMember(SST_SMQDSFDR_LEFT_SIZE,m_TransferLeftSize);
			ReturnMsg.AddMember(SST_SMQDSFDR_DATA,(LPCTSTR)pDataBuffer,DataSize);
			SendMsg(ReturnMsg);
		}
		break;
	case SM_MSG_START_UPLOAD_SERVICE_FILE:
		{
			UINT ServiceIndex=Msg.GetMember(SST_SMSUSF_SERVICE_INDEX);
			LPCTSTR szFilePath=(LPCTSTR)Msg.GetMember(SST_SMSUSF_FILE_PATH);
			UINT TotalSize=Msg.GetMember(SST_SMSUSF_TOTAL_SIZE);
			m_TransferBuffer.Create(TotalSize);
			m_OriginSize=Msg.GetMember(SST_SMSUSF_ORIGIN_TOTAL_SIZE);
			UINT PackPropSize=Msg.GetMember(SST_SMSUSF_PACK_PROP).GetLength();
			m_PackProp.Create(PackPropSize);
			m_PackProp.PushBack((LPCTSTR)Msg.GetMember(SST_SMSUSF_PACK_PROP),PackPropSize);
			if(Msg.IsMemberExist(SST_SMSUSF_FILE_LAST_WRITE_TIME))
			{
				m_CurTransferFileLastWriteTime=(time_t)Msg.GetMember(SST_SMSUSF_FILE_LAST_WRITE_TIME);
			}
			else
			{
				m_CurTransferFileLastWriteTime.FetchSystemTime();
			}

			
			

			char SendBuffer[128+MAX_PATH];
			CSmartStruct ReturnMsg(SendBuffer,128+MAX_PATH,true);
			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_START_UPLOAD_SERVICE_FILE_RESULT);			
			ReturnMsg.AddMember(SST_SMSUSFR_SERVICE_INDEX,ServiceIndex);
			ReturnMsg.AddMember(SST_SMSUSFR_FILE_PATH,szFilePath);

			if(szFilePath&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
			{
				m_TransferFilePath=m_pManager->GetServiceInfoList()[ServiceIndex].WorkDir;
				m_TransferFilePath=m_TransferFilePath+"\\"+szFilePath;
				CEasyString FileDir=GetPathDirectory(m_TransferFilePath);
				if(CreateDirEx(FileDir))				
				{
					CWinFileAccessor FileAccessor;
					if(FileAccessor.Open(m_TransferFilePath,CWinFileAccessor::modeCreateAlways|CWinFileAccessor::modeWrite|CWinFileAccessor::shareShareAll))
					{

						FileAccessor.Close();
						
						Log("请求服务[%u]开始上传文件[%s]成功,数据传输大小%u,原始数据大小%u",
							ServiceIndex,szFilePath,
							TotalSize,m_OriginSize);

						ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
					}
					else
					{
						int Err=GetLastError();
						Log("请求服务[%u]开始上传文件[%s]打开失败%d",
							ServiceIndex,szFilePath,Err);

						ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
					}
				}
				else
				{
					Log("请求服务[%u]开始上传文件[%s]创建目录失败",
						ServiceIndex,szFilePath);

					ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
				}	
			}
			else
			{
				Log("请求服务[%u]开始上传文件[%s]失败",
					ServiceIndex,szFilePath);

				ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
			}

			SendMsg(ReturnMsg);
		}
		break;
	case SM_MSG_SEND_UPLOAD_SERVICE_FILE_DATA:
		{
			UINT ServiceIndex=Msg.GetMember(SST_SMSUSFD_SERVICE_INDEX);
			UINT TotalSize=Msg.GetMember(SST_SMSUSFD_TOTAL_SIZE);
			UINT LeftSize=Msg.GetMember(SST_SMSUSFD_LEFT_SIZE);
			CSmartValue FileData=Msg.GetMember(SST_SMSUSFD_DATA);

			char SendBuffer[64];
			CSmartStruct ReturnMsg(SendBuffer,64,true);
			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_SEND_UPLOAD_SERVICE_FILE_DATA_RESULT);			
			ReturnMsg.AddMember(SST_SMSUSFDR_SERVICE_INDEX,ServiceIndex);


			m_TransferBuffer.PushBack((LPCTSTR)FileData,FileData.GetLength());
			
			
			//Log("请求服务[%u]上传文件写入数据[%u]成功,未解压大小[%u]",
			//	ServiceIndex,UnpackSize,FileData.GetLength());
			ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);				
			
			SendMsg(ReturnMsg);

			if(LeftSize==0)
			{
				Log("请求服务[%u]上传文件完毕",
					ServiceIndex);

				CWinFileAccessor FileAccessor;
				if(FileAccessor.Open(m_TransferFilePath,CWinFileAccessor::modeCreateAlways|CWinFileAccessor::modeWrite))
				{
					CEasyBuffer WriteBuffer;
					WriteBuffer.Create(m_OriginSize);

					size_t UnpackSize=m_OriginSize;
					size_t SourceSize=m_TransferBuffer.GetUsedSize();

					if(LzmaUncompress((BYTE *)WriteBuffer.GetBuffer(),&UnpackSize,
						(BYTE *)m_TransferBuffer.GetBuffer(),&SourceSize,
						(BYTE *)m_PackProp.GetBuffer(),m_PackProp.GetUsedSize())==SZ_OK)
					{
						Log("请求服务[%u]上传文件,数据解压完毕,大小%u",
							ServiceIndex,UnpackSize);
						if(FileAccessor.Write(WriteBuffer.GetBuffer(),UnpackSize)==UnpackSize)
						{
							FileAccessor.SetLastWriteTime(m_CurTransferFileLastWriteTime);
							Log("请求服务[%u]上传文件,数据写入完毕,大小%u",
								ServiceIndex,UnpackSize);
						}
						else
						{
							Log("请求服务[%u]上传文件,数据写入失败",
								ServiceIndex);
						}
					}
					else
					{
						Log("请求服务[%u]上传文件,数据解压失败",
							ServiceIndex);
					}

					FileAccessor.Close();
				}
			}
		}
		break;
	case SM_MSG_CREATE_DIR:
		{
			UINT ServiceIndex=Msg.GetMember(SST_SMCD_SERVICE_INDEX);
			LPCTSTR szDir=(LPCTSTR)Msg.GetMember(SST_SMCD_DIR_NAME);

			char SendBuffer[64];
			CSmartStruct ReturnMsg(SendBuffer,64,true);
			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_CREATE_DIR_RESULT);			
			ReturnMsg.AddMember(SST_SMCDR_SERVICE_INDEX,ServiceIndex);

			if(szDir&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
			{
				CEasyString Dir=m_pManager->GetServiceInfoList()[ServiceIndex].WorkDir;
				Dir=MakeFullPath(Dir+"\\"+szDir);	

				Log("请求服务[%u]创建目录[%s]",
					ServiceIndex,(LPCTSTR)Dir);
				if(CreateDirEx(Dir))				
				{
					ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
				}
				else
				{
					ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
				}
			}
			else
			{
				ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
			}

			SendMsg(ReturnMsg);
		}
		break;
	case SM_MSG_DELETE_FILE:
		{
			UINT ServiceIndex=Msg.GetMember(SST_SMDF_SERVICE_INDEX);
			LPCTSTR szFilePath=(LPCTSTR)Msg.GetMember(SST_SMDF_FILE_PATH);

			char SendBuffer[64];
			CSmartStruct ReturnMsg(SendBuffer,64,true);
			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_DELETE_FILE_RESULT);			
			ReturnMsg.AddMember(SST_SMDFR_SERVICE_INDEX,ServiceIndex);

			if(szFilePath&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
			{
				CEasyString WorkDir=MakeFullPath(m_pManager->GetServiceInfoList()[ServiceIndex].WorkDir);
				CEasyString FilePath=MakeFullPath(WorkDir+"\\"+szFilePath);	

				Log("请求服务[%u]删除文件[%s]",
					ServiceIndex,(LPCTSTR)FilePath);

				WorkDir.MakeUpper();
				FilePath.MakeUpper();

				if(FilePath.Find(WorkDir)>=0)				
				{
					char szFileFrom[MAX_PATH];
					strncpy_0(szFileFrom,MAX_PATH,FilePath,MAX_PATH);
					if(FilePath.GetLength()<MAX_PATH-1)
						szFileFrom[FilePath.GetLength()+1]=0;

					SHFILEOPSTRUCT Operation;
					Operation.hwnd=NULL;
					Operation.wFunc=FO_DELETE;
					Operation.pFrom=szFileFrom;
					Operation.pTo=NULL;
					Operation.fAnyOperationsAborted=FALSE;
					Operation.hNameMappings=NULL;
					Operation.lpszProgressTitle=NULL;
					Operation.fFlags=FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_SILENT;

					UINT Code=SHFileOperation(&Operation);
					if(Code==0)
					{
						ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
					}
					else
					{
						ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
					}
				}
				else
				{
					ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
				}
			}
			else
			{
				ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
			}

			SendMsg(ReturnMsg);
		}
		break;
	case SM_MSG_CREATE_PROCESS:
		{
			UINT ServiceIndex=Msg.GetMember(SST_SMCP_SERVICE_INDEX);
			LPCTSTR szImageFilePath=(LPCTSTR)Msg.GetMember(SST_SMCP_IMAGE_FILE_PATH);
			LPCTSTR szWorkDir=(LPCTSTR)Msg.GetMember(SST_SMCP_WORK_DIR);
			LPCTSTR szParam=(LPCTSTR)Msg.GetMember(SST_SMCP_PARAM);

			char SendBuffer[64];
			CSmartStruct ReturnMsg(SendBuffer,64,true);
			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_CREATE_PROCESS_RESULT);			
			ReturnMsg.AddMember(SST_SMCPR_SERVICE_INDEX,ServiceIndex);

			if(szImageFilePath&&szWorkDir&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
			{
				
				CEasyString ServiceDir=MakeFullPath(m_pManager->GetServiceInfoList()[ServiceIndex].WorkDir);
				CEasyString ImageFilePath=MakeFullPath(ServiceDir+"\\"+szImageFilePath);
				CEasyString WorkDir=MakeFullPath(ServiceDir+"\\"+szWorkDir);
				

				Log("请求服务[%u]启动程序[%s %s]在[%s]",
					ServiceIndex,
					(LPCTSTR)ImageFilePath,
					szParam,
					WorkDir);

				ServiceDir.MakeUpper();
				ImageFilePath.MakeUpper();

				if(ImageFilePath.Find(ServiceDir)>=0)				
				{					

					SERVICE_INFO ProcessInfo;

					strncpy_0(ProcessInfo.ImageFilePath,MAX_PATH,ImageFilePath,MAX_PATH);
					strncpy_0(ProcessInfo.WorkDir,MAX_PATH,WorkDir,MAX_PATH);
					if(szParam)
						strncpy_0(ProcessInfo.StartupParam,MAX_PATH,szParam,MAX_PATH);

					if(m_pManager->StartupProcess(ProcessInfo))				
					{
						ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
					}
					else
					{
						ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
					}
				}
				else
				{
					ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
				}
			}
			else
			{
				ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
			}

			SendMsg(ReturnMsg);
		}
		break;
	case SM_MSG_GET_NET_ADAPTER_INFO:
		{
			CEasyArray<NET_ADAPTER_INFO>& AdapterInfoList=m_pManager->GetNetAdapterInfo();

			char Buffer[MAX_DATA_PACKET_SIZE];
			CSmartStruct Packet(Buffer,MAX_DATA_PACKET_SIZE,true);
			Packet.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_GET_NET_ADAPTER_INFO_RESULT);
			Packet.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
			Packet.AddMember(SST_SMGNAIR_TOTAL_COUNT,AdapterInfoList.GetCount());
			Packet.AddMember(SST_SMGNAIR_START_INDEX,0);

			for(UINT i=0;i<AdapterInfoList.GetCount();i++)
			{
				if(Packet.GetDataLen()>=MAX_DATA_TRANSFER_SIZE)
				{
					SendMsg(Packet);
					Packet.Clear();
					Packet.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_GET_NET_ADAPTER_INFO_RESULT);
					Packet.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
					Packet.AddMember(SST_SMGNAIR_TOTAL_COUNT,AdapterInfoList.GetCount());
					Packet.AddMember(SST_SMGNAIR_START_INDEX,i);
				}
				PackNetAdapterInfo(AdapterInfoList[i],Packet,SST_SMGNAIR_NET_ADAPTER_INFO);				
				
			}
			SendMsg(Packet);
		}
		break;
	case SM_MSG_EXECUTE_SCRIPT:
		{
			UINT ServiceIndex=Msg.GetMember(SST_SMES_SERVICE_INDEX);
			int ScriptType=Msg.GetMember(SST_SMES_SCRIPT_TYPE);
			LPCTSTR szScript=(LPCTSTR)Msg.GetMember(SST_SMES_SCRIPT);

			char SendBuffer[64];
			CSmartStruct ReturnMsg(SendBuffer,64,true);
			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_EXECUTE_SCRIPT_RESULT);			
			ReturnMsg.AddMember(SST_SMESR_SERVICE_INDEX,ServiceIndex);
			if(szScript&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
			{
				if(!m_IsInScriptExecute)
				{
					CEasyString WorkDir=MakeFullPath(m_pManager->GetServiceInfoList()[ServiceIndex].WorkDir);
					m_IsInScriptExecute=true;
					if(ScriptType==SCRIPT_TYPE_FROM_FILE)
					{
						CWinFileAccessor FileAccessor;

						
						CEasyString FilePath=MakeFullPath(WorkDir+"\\"+szScript);	
						if(FileAccessor.Open(FilePath,CWinFileAccessor::modeOpen|CWinFileAccessor::modeRead|CWinFileAccessor::shareShareAll))
						{
							CEasyBuffer ReadBuffer;
							UINT Size=FileAccessor.GetSize();
							ReadBuffer.Create(Size+1);
							if(FileAccessor.Read(ReadBuffer.GetBuffer(),Size)==Size)
							{
								ReadBuffer.SetUsedSize(Size);
								ReadBuffer.PushConstBack(0,1);
								m_ScriptExecutor.ExcuteScript((LPCTSTR)ReadBuffer.GetBuffer(),WorkDir,ServiceIndex);
							}
							else
							{
								ReturnMsg.AddMember(SSMM_MSG_RESULT,SMESRT_SCRIPT_FILE_READ_ERROR);
								SendMsg(ReturnMsg);
							}
						}
						else
						{
							ReturnMsg.AddMember(SSMM_MSG_RESULT,SMESRT_SCRIPT_FILE_NOT_EXIST);
							SendMsg(ReturnMsg);
						}

					}
					else
					{
						m_ScriptExecutor.ExcuteScript(szScript,WorkDir,ServiceIndex);
					}
				}
				else
				{
					ReturnMsg.AddMember(SSMM_MSG_RESULT,SMESRT_ONE_SCRIPT_IS_EXCUTING);
					SendMsg(ReturnMsg);
				}
			}
			else
			{
				ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
				SendMsg(ReturnMsg);
			}

		}
		break;
	}
	
	FUNCTION_END;
}

void CServerManagerClient::SendMsg(CSmartStruct& Msg)
{
	FUNCTION_BEGIN;
	QuerySend(Msg.GetData(),Msg.GetDataLen());
	FUNCTION_END;
}
void CServerManagerClient::SendMsg(UINT MsgID)
{
	FUNCTION_BEGIN;
	char SendBuffer[32];
	CSmartStruct Msg(SendBuffer,32,true);

	Msg.AddMember(SSMM_MSG_TYPE_ID,MsgID);
	QuerySend(Msg.GetData(),Msg.GetDataLen());
	FUNCTION_END;
}

void CServerManagerClient::OnGetFile(FILE_CACHE * pFileCache)
{
	char SendBuffer[MAX_PATH+128];
	CSmartStruct ReturnMsg(SendBuffer,sizeof(SendBuffer),true);
	ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_START_DOWNLOAD_SERVICE_FILE_RESULT);			
	ReturnMsg.AddMember(SST_SMSDSFR_SERVICE_INDEX,pFileCache->QueryParam);
	ReturnMsg.AddMember(SST_SMSDSFR_FILE_PATH,(LPCTSTR)pFileCache->FilePath);
	if(pFileCache->IsReady)
	{
		m_TransferBuffer.SetUsedSize(0);
		m_OriginSize=pFileCache->FileSize;

		if(m_OriginSize)
		{	
			m_TransferBuffer=pFileCache->FileData;
			m_PackProp=pFileCache->PackProp;
			m_TransferLeftSize=pFileCache->PackSize;			

			ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
			ReturnMsg.AddMember(SST_SMSDSFR_TOTAL_SIZE,pFileCache->PackSize);
			ReturnMsg.AddMember(SST_SMSDSFR_ORIGIN_TOTAL_SIZE,m_OriginSize);
			ReturnMsg.AddMember(SST_SMSDSFR_PACK_PROP,(LPCTSTR)m_PackProp.GetBuffer(),m_PackProp.GetUsedSize());
			ReturnMsg.AddMember(SST_SMSDSFR_FILE_LAST_WRITE_TIME,(time_t)pFileCache->LastWriteTime);


			SendMsg(ReturnMsg);
					
		}
		else
		{
			Log("请求服务[%u]下载文件[%s]长度为0",
				pFileCache->QueryParam,
				(LPCTSTR)pFileCache->FilePath);

			ReturnMsg.AddMember(SST_SMSDSFR_TOTAL_SIZE,0);
			ReturnMsg.AddMember(SST_SMSDSFR_ORIGIN_TOTAL_SIZE,0);						
			ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
			SendMsg(ReturnMsg);

		}
		
	}
	else
	{
		Log("请求服务[%u]下载文件[%s]读取失败失败",
			pFileCache->QueryParam,
			(LPCTSTR)pFileCache->FilePath);
		ReturnMsg.AddMember(SSMM_MSG_RESULT,SMSDSFRT_FILE_NOT_EXIST);
		SendMsg(ReturnMsg);
	}
}

void CServerManagerClient::OnScriptExcute(int ErrorCode,UINT Param)
{
	char SendBuffer[64];
	CSmartStruct ReturnMsg(SendBuffer,64,true);
	ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_EXECUTE_SCRIPT_RESULT);			
	ReturnMsg.AddMember(SST_SMESR_SERVICE_INDEX,Param);
	ReturnMsg.AddMember(SSMM_MSG_RESULT,ErrorCode);
	SendMsg(ReturnMsg);
}

UINT CServerManagerClient::PackServiceInfo(CEasyArray<SERVICE_INFO>& ServiceList,UINT StartIndex,CSmartStruct& Packet,WORD MemberID)
{
	UINT Count=0;
	for(;StartIndex<ServiceList.GetCount();StartIndex++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		SubPacket.AddMember(SSIL_NAME,(LPCTSTR)ServiceList[StartIndex].Name);
		SubPacket.AddMember(SSIL_INDEX,StartIndex);		
		SubPacket.AddMember(SSIL_IMAGE_FILE_PATH,(LPCTSTR)ServiceList[StartIndex].ImageFilePath);
		SubPacket.AddMember(SSIL_STATUS,ServiceList[StartIndex].Status);
		SubPacket.AddMember(SSIL_CPU_USED,ServiceList[StartIndex].CPUUsed);
		SubPacket.AddMember(SSIL_MEMORY,ServiceList[StartIndex].MemoryUsed);
		SubPacket.AddMember(SSIL_VIRTUAL_MEMORY,ServiceList[StartIndex].VirtualMemoryUsed);
		SubPacket.AddMember(SSIL_IMAGE_FILE_TIME,(time_t)ServiceList[StartIndex].ImageFileTime);
		Packet.FinishMember(MemberID,SubPacket.GetDataLen());
		Count++;
		if(Packet.GetDataLen()>=MAX_DATA_TRANSFER_SIZE)
		{
			break;
		}
	}	
	return Count;
}

void CServerManagerClient::PackNetAdapterInfo(NET_ADAPTER_INFO& NetAdapterInfo,CSmartStruct& Packet,WORD MemberID)
{
	UINT BufferSize;
	void * pBuffer=Packet.PrepareMember(BufferSize);
	CSmartStruct SubPacket(pBuffer,BufferSize,true);
	SubPacket.AddMember(SST_NAI_INDEX,NetAdapterInfo.Index);
	SubPacket.AddMember(SST_NAI_IP_ADDRESS,NetAdapterInfo.IPAddress);		
	SubPacket.AddMember(SST_NAI_SEND_FLUX,NetAdapterInfo.SendFlux);
	SubPacket.AddMember(SST_NAI_RECV_FLUX,NetAdapterInfo.RecvFlux);
	Packet.FinishMember(MemberID,SubPacket.GetDataLen());
}


bool CServerManagerClient::SendFileList(UINT ServiceIndex,CEasyString& WorkDir,CEasyString& SearchDir)
{
	FUNCTION_BEGIN;
	CFileSearcher FileSearcher;
	CEasyString UserDir;

	bool IsInRoot=false;

	if(GetPathDirectory(SearchDir)==GetPathDirectory(WorkDir))
	{
		IsInRoot=true;
	}

	vector<FILE_INFO>	FileInfoList;


	if(FileSearcher.FindFirst(SearchDir+"\\*"))
	{
		while(FileSearcher.FindNext())
		{
			FILE_INFO FileInfo;

			if((!IsInRoot)||(!FileSearcher.IsDots()))
			{
				FileInfo.FileName=FileSearcher.GetFileName();
				FileInfo.FileAttr=FileSearcher.GetFileAttributes();
				FileInfo.FileSize=FileSearcher.GetFileSize();
				FileInfo.FileCreateTime=FileSearcher.GetCreationTime();
				FileInfo.FileLastAccessTime=FileSearcher.GetLastAccessTime();
				FileInfo.FileLastWriteTime=FileSearcher.GetLastWriteTime();

				FileInfoList.push_back(FileInfo);
			}
		}
		CSmartStruct MsgPacket(4096);
		MsgPacket.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_BROWSE_SERVICE_DIR_RESULT);
		MsgPacket.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
		MsgPacket.AddMember(SST_SMBSDR_SERVICE_INDEX,ServiceIndex);		
		MsgPacket.AddMember(SST_SMBSDR_TOTAL_COUNT,(short)FileInfoList.size());

		UserDir=SearchDir;
		UserDir.Replace(WorkDir,"");

		MsgPacket.AddMember(SST_SMBSDR_DIRECTORY,(LPCTSTR)UserDir);
		int FileInfoCount=0;
		for(size_t i=0;i<FileInfoList.size();i++)
		{
			CSmartStruct FileInfo(512);

			FileInfo.AddMember(SST_SMFI_FILE_NAME,(LPCTSTR)FileInfoList[i].FileName);
			FileInfo.AddMember(SST_SMFI_FILE_ATTRIBUTE,FileInfoList[i].FileAttr);
			FileInfo.AddMember(SST_SMFI_FILE_SIZE,FileInfoList[i].FileSize);
			FileInfo.AddMember(SST_SMFI_FILE_CREATE_TIME,FileInfoList[i].FileCreateTime);
			FileInfo.AddMember(SST_SMFI_FILE_LAST_ACCESS_TIME,FileInfoList[i].FileLastAccessTime);
			FileInfo.AddMember(SST_SMFI_FILE_LAST_WRITE_TIME,FileInfoList[i].FileLastWriteTime);

			if(MsgPacket.GetDataLen()+FileInfo.GetDataLen()>4000)
			{
				MsgPacket.AddMember(SST_SMBSDR_LEFT_COUNT,(short)(FileInfoList.size()-i));
				SendMsg(MsgPacket);

				MsgPacket.Clear();
				MsgPacket.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_BROWSE_SERVICE_DIR_RESULT);				
				MsgPacket.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
				MsgPacket.AddMember(SST_SMBSDR_SERVICE_INDEX,ServiceIndex);
				MsgPacket.AddMember(SST_SMBSDR_TOTAL_COUNT,(short)FileInfoList.size());
				MsgPacket.AddMember(SST_SMBSDR_DIRECTORY,(LPCTSTR)UserDir);
				FileInfoCount=0;
			}
			MsgPacket.AddMember(SST_SMBSDR_FILE_INFO,FileInfo);
			FileInfoCount++;
		}
		if(FileInfoCount)
		{
			MsgPacket.AddMember(SST_SMBSDR_LEFT_COUNT,0);
			SendMsg(MsgPacket);
		}
		return true;
	}
	else
	{
		return false;
	}
	FUNCTION_END;
	return false;
}
