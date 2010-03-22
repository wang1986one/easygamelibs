#include "StdAfx.h"


CScriptExecutor::CScriptExecutor(void)
	:m_ScriptExecutor(64,32)
{
	m_pManager=NULL;
	m_Param=0;
	m_Status=ST_NONE;
	m_TransferLeftSize=0;
	m_OriginSize=0;

	m_ScriptExecutor.AddFaction("DownloadFile",5,(INT_PTR)this,DownloadFileFN);
}

CScriptExecutor::~CScriptExecutor(void)
{
}

BOOL CScriptExecutor::Init(UINT ID,CServerManagerClient * pManager)
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
	m_AssembleBuffer.SetUsedSize(0);	
	m_ScriptExecutor.Reset();
	m_Param=0;
	m_Status=ST_NONE;
	m_TransferLeftSize=0;
	m_OriginSize=0;

	
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CScriptExecutor::Destory()
{
	FUNCTION_BEGIN;		

	CNetConnection::Destory();
	m_pManager=NULL;	

	FUNCTION_END;
}

void CScriptExecutor::OnConnection(BOOL IsSucceed)
{
	FUNCTION_BEGIN;
	if(IsSucceed)
	{
		Log("%s:%u建立连接",
			GetRemoteAddress().GetIPString(),
			GetRemoteAddress().GetPort());
		if(m_Status==ST_DOWNLOAD)
		{
			QueryStartDownload(m_DownloadServiceIndex,m_DownloadSourcePath,m_DownloadTargetPath);
		}
	}
	else
	{
		if(m_Status==ST_DOWNLOAD)
		{
			ContinueScriptInterrupt(IPT_DOWNLOAD_FILE,-1);
		}
	}
	FUNCTION_END;
}

void CScriptExecutor::OnDisconnection()
{
	FUNCTION_BEGIN;
	Log("%s:%u断开连接",
		GetRemoteAddress().GetIPString(),
		GetRemoteAddress().GetPort());

	if(m_Status==ST_DOWNLOAD)
	{
		ContinueScriptInterrupt(IPT_DOWNLOAD_FILE,-1);
	}

	FUNCTION_END;
}

void CScriptExecutor::OnRecvData(const CEasyBuffer& DataBuffer)
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

int CScriptExecutor::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;	
	int ProcessCount=CNetConnection::Update(ProcessPacketLimit);

	return ProcessCount;
	FUNCTION_END;
	return 0;
}

void CScriptExecutor::OnMsg(CSmartStruct& Msg)
{
	FUNCTION_BEGIN;
	UINT MsgID=Msg.GetMember(SSMM_MSG_TYPE_ID);
	switch(MsgID)
	{
	case SM_MSG_START_DOWNLOAD_SERVICE_FILE_RESULT:
		{
			int Result=Msg.GetMember(SSMM_MSG_RESULT);
			UINT TotalSize=Msg.GetMember(SST_SMSDSFR_TOTAL_SIZE);
			UINT OriginSize=Msg.GetMember(SST_SMSDSFR_ORIGIN_TOTAL_SIZE);
			LPCTSTR pPackProp=Msg.GetMember(SST_SMSDSFR_PACK_PROP);
			UINT PackPropSize=Msg.GetMember(SST_SMSDSFR_PACK_PROP).GetLength();
			time_t LastWriteTime=Msg.GetMember(SST_SMSDSFR_FILE_LAST_WRITE_TIME);

			OnStartDownloadResult(Result,TotalSize,OriginSize,pPackProp,PackPropSize,LastWriteTime);
		}
		break;
	case SM_MSG_QUERY_DOWNLOAD_SERVICE_FILE_DATA_RESULT:
		{
			int Result=Msg.GetMember(SSMM_MSG_RESULT);
			UINT TotalSize=Msg.GetMember(SST_SMQDSFDR_TOTAL_SIZE);
			UINT LeftSize=Msg.GetMember(SST_SMQDSFDR_LEFT_SIZE);
			CSmartValue FileData=Msg.GetMember(SST_SMQDSFDR_DATA);
			OnDownloadData(Result,TotalSize,LeftSize,(LPCTSTR)FileData,FileData.GetLength());
		}
		break;
	}
	FUNCTION_END;
}

void CScriptExecutor::SendMsg(CSmartStruct& Msg)
{
	FUNCTION_BEGIN;
	QuerySend(Msg.GetData(),Msg.GetDataLen());
	FUNCTION_END;
}
void CScriptExecutor::SendMsg(UINT MsgID)
{
	FUNCTION_BEGIN;
	char SendBuffer[32];
	CSmartStruct Msg(SendBuffer,32,true);

	Msg.AddMember(SSMM_MSG_TYPE_ID,MsgID);
	QuerySend(Msg.GetData(),Msg.GetDataLen());
	FUNCTION_END;
}

bool CScriptExecutor::ExcuteScript(LPCTSTR szScript,LPCTSTR szWorkDir,UINT Param)
{
	m_WorkDir=szWorkDir;
	m_Param=Param;
	m_CurScript.Clear();
	int ErrCode=m_CurScript.PushScript(szScript,m_ScriptExecutor.GetVariableList(),m_ScriptExecutor.GetFactionList());
	if(ErrCode==0)
	{
		CBolan Result;
		int StartPos=0;
		ErrCode=m_ScriptExecutor.ExecScript(m_CurScript,Result,StartPos);
		Log("执行脚本结果[%d:%s]",
			ErrCode,m_ScriptExecutor.GetErrorMsg(ErrCode));
		
	}
	else
	{
		Log("解析脚本失败[%d:%s]",
			ErrCode,m_ScriptExecutor.GetErrorMsg(ErrCode));
	}

	if(ErrCode>=0)
	{
		m_pManager->OnScriptExcute(ErrCode,m_Param);
	}
	return true;
}

void CScriptExecutor::ContinueScriptInterrupt(int IPTType,int Result)
{	
	m_Status=ST_NONE;
	CBolan IPTResult,SResult;
	IPTResult.ValueType=VALUE_TYPE_NUMBER;
	IPTResult.value=Result;
	int ErrCode=m_ScriptExecutor.ContinueInterrupt(IPTType,m_CurScript,IPTResult,SResult);
	Log("脚本中断继续执行结果[%d:%s]",
		ErrCode,m_ScriptExecutor.GetErrorMsg(ErrCode));
	if(ErrCode>=0)
	{
		Disconnect();
		m_pManager->OnScriptExcute(ErrCode,m_Param);
	}
}

void CScriptExecutor::QueryStartDownload(UINT ServiceIndex,LPCTSTR SourceFilePath,LPCTSTR TargetFilePath)
{
	char SendBuffer[128+MAX_PATH];
	CSmartStruct Msg(SendBuffer,128+MAX_PATH,true);

	Log("请求服务器上的服务[%u]下载文件[%s]到[%s]",
		ServiceIndex,
		SourceFilePath,TargetFilePath);

	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_START_DOWNLOAD_SERVICE_FILE);
	Msg.AddMember(SST_SMSDSF_SERVICE_INDEX,ServiceIndex);
	Msg.AddMember(SST_SMSDSF_FILE_PATH,SourceFilePath);	

	QuerySend(Msg.GetData(),Msg.GetDataLen());
}

void CScriptExecutor::QueryDownloadData(UINT ServiceIndex)
{
	char SendBuffer[128];
	CSmartStruct Msg(SendBuffer,sizeof(SendBuffer),true);

	Msg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_QUERY_DOWNLOAD_SERVICE_FILE_DATA);
	Msg.AddMember(SST_SMQDSFD_SERVICE_INDEX,ServiceIndex);
	QuerySend(Msg.GetData(),Msg.GetDataLen());
}

void CScriptExecutor::OnStartDownloadResult(int Result,UINT TotalSize,UINT OrgTotalSize,LPCVOID pPackProps,UINT PackPropSize,time_t LastWriteTime)
{
	if(Result==SSMRT_SUCCEED)
	{
		m_TransferBuffer.Create(TotalSize);
		m_OriginSize=OrgTotalSize;
		m_PackProp.Create(PackPropSize);
		m_PackProp.PushBack(pPackProps,PackPropSize);
		m_CurTransferFileLastWriteTime=LastWriteTime;
		if(TotalSize)
		{
			QueryDownloadData(m_DownloadServiceIndex);
		}
		else
		{
			Log("文件[%s]大小为0",
				(LPCTSTR)m_DownloadSourcePath);		
			CWinFileAccessor FileAccessor;
			FileAccessor.Open(m_DownloadTargetPath,CWinFileAccessor::modeCreateAlways|CWinFileAccessor::modeWrite);
			FileAccessor.Close();
			ContinueScriptInterrupt(IPT_DOWNLOAD_FILE,0);
		}		
	}
	else
	{
		Log("下载文件[%s]失败",
			(LPCTSTR)m_DownloadSourcePath);
		ContinueScriptInterrupt(IPT_DOWNLOAD_FILE,-1);
	}
}
void CScriptExecutor::OnDownloadData(int Result,UINT TotalSize,UINT LeftSize,LPCVOID pData,UINT DataSize)
{
	if(Result==SSMRT_SUCCEED)
	{
		m_TransferBuffer.PushBack(pData,DataSize);		
		if(LeftSize)
		{
			QueryDownloadData(m_DownloadServiceIndex);
		}
		else
		{
			Log("文件[%s]下载完毕",
				(LPCTSTR)m_DownloadSourcePath);	

			CEasyBuffer WriteBuffer;
			WriteBuffer.Create(m_OriginSize);

			size_t UnpackSize=m_OriginSize;
			size_t SourceSize=m_TransferBuffer.GetUsedSize();

			if(LzmaUncompress((BYTE *)WriteBuffer.GetBuffer(),&UnpackSize,
				(BYTE *)m_TransferBuffer.GetBuffer(),&SourceSize,
				(BYTE *)m_PackProp.GetBuffer(),m_PackProp.GetUsedSize())==SZ_OK)
			{
				
				
				CWinFileAccessor FileAccessor;
				if(FileAccessor.Open(m_DownloadTargetPath,CWinFileAccessor::modeCreateAlways|CWinFileAccessor::modeWrite))
				{
					if(FileAccessor.Write(WriteBuffer.GetBuffer(),UnpackSize)==UnpackSize)
					{
						FileAccessor.SetLastWriteTime(m_CurTransferFileLastWriteTime);
						Log("下载文件[%s]数据写入完毕,大小%u",
							(LPCTSTR)m_DownloadSourcePath,
							UnpackSize);
						ContinueScriptInterrupt(IPT_DOWNLOAD_FILE,0);
					}
					else
					{
						Log("下载文件[%s]数据写入失败",
							(LPCTSTR)m_DownloadSourcePath);	
						ContinueScriptInterrupt(IPT_DOWNLOAD_FILE,-1);
					}
				}
				else
				{
					Log("下载文件[%s]文件[%s]失败",
						(LPCTSTR)m_DownloadSourcePath,
						(LPCTSTR)m_DownloadTargetPath);	
					ContinueScriptInterrupt(IPT_DOWNLOAD_FILE,-1);
				}

				FileAccessor.Close();
				
			}
			else
			{
				Log("下载文件[%s]数据解压失败",
					(LPCTSTR)m_DownloadSourcePath);	
				ContinueScriptInterrupt(IPT_DOWNLOAD_FILE,-1);
			}
			
		}
	}
	else
	{
		Log("下载文件[%s]下载失败",
			(LPCTSTR)m_DownloadSourcePath);	
		ContinueScriptInterrupt(IPT_DOWNLOAD_FILE,-1);
	}
}

int CScriptExecutor::DownloadFileFN(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount)
{
	CScriptExecutor * pExecutor=(CScriptExecutor *)FnParam;

	CIPAddress Address(pParams[0].StrValue,pParams[1].value);

	pExecutor->m_DownloadServiceIndex=pParams[2].value;
	pExecutor->m_DownloadSourcePath=pParams[3].StrValue;	
	pExecutor->m_DownloadTargetPath=MakeFullPath(pExecutor->m_WorkDir+"\\"+pParams[4].StrValue);

	pExecutor->m_Status=ST_DOWNLOAD;

	if(pExecutor->Connect(Address,DOWNLOAD_CONNECT_TIME))
	{
		return IPT_DOWNLOAD_FILE;
	}
	else
	{
		pExecutor->m_Status=ST_NONE;
		pResult->ValueType=VALUE_TYPE_NUMBER;
		pResult->value=-1;
		return 0;
	}

}