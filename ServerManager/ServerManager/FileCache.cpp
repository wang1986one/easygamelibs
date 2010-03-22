#include "StdAfx.h"


CFileCache::CFileCache(void)
{
}

CFileCache::~CFileCache(void)
{
	Destory();
}


bool CFileCache::Create(UINT CacheSize,UINT QueryQueueSize)
{
	if(!m_FileCachePool.Create(CacheSize))
		return false;

	if(!m_QueryQueue.Create(QueryQueueSize))
		return false;

	if(!m_FinishQueue.Create(QueryQueueSize))
		return false;

	return Start();
}
void CFileCache::Destory()
{
	SafeTerminate();
	void * Pos=m_FileCachePool.GetFirstObjectPos();
	while(Pos)
	{
		CEasyString Key;
		FILE_CACHE ** ppObject=m_FileCachePool.GetNextObject(Pos,Key);
		if(ppObject)
		{
			delete *ppObject;
		}
	}
	m_FileCachePool.Destory();

	FILE_CACHE * pObject;
	while(m_QueryQueue.PopFront(pObject))
	{
		delete pObject;
	}
	while(m_FinishQueue.PopFront(pObject))
	{
		delete pObject;
	}
}

bool CFileCache::QueryGetFile(LPCTSTR FilePath,IFileCacheEventHandler * pEventHandler,UINT Param)
{
	CEasyString Key=FilePath;
	Key.Trim();
	Key.MakeUpper();
	FILE_CACHE ** ppObject=m_FileCachePool.Find(Key);
	if(ppObject)
	{		
		
		CWinFileAccessor FileAccessor;
		if(FileAccessor.Open(FilePath,CWinFileAccessor::modeOpen|CWinFileAccessor::modeRead|CWinFileAccessor::shareShareAll))
		{
			CEasyTime LastWriteTime;
			FileAccessor.GetLastWriteTime(LastWriteTime);
			FileAccessor.Close();
			if((*ppObject)->LastWriteTime==LastWriteTime)
			{
				(*ppObject)->pEventHandler=pEventHandler;
				(*ppObject)->QueryParam=Param;
				pEventHandler->OnGetFile(*ppObject);
				return true;
			}
			
		}
		m_FileCachePool.Delete(Key);
		delete *ppObject;
		Log("文件[%s]已过期",
			(LPCTSTR)Key);
	}
	
	FILE_CACHE * pObject=new FILE_CACHE;
	pObject->FilePath=FilePath;
	pObject->pEventHandler=pEventHandler;
	pObject->QueryParam=Param;
	if(!m_QueryQueue.PushBack(pObject))
	{
		pEventHandler->OnGetFile(pObject);
		delete pObject;
	}
	
	return true;
}

BOOL CFileCache::OnRun()
{
	int ProcessCount=0;
	FILE_CACHE * pObject;
	while(m_QueryQueue.PopFront(pObject))
	{
		pObject->IsReady=ReadFile(pObject);
		m_FinishQueue.PushBack(pObject);
		ProcessCount++;
		if(ProcessCount>32)
		{
			break;
		}
	}
	if(ProcessCount==0)
	{
		Sleep(1);
	}

	return TRUE;
}

int CFileCache::Update(int ProcessPacketLimit)
{
	int ProcessCount=0;
	FILE_CACHE * pObject;
	while(m_FinishQueue.PopFront(pObject))
	{
		pObject->pEventHandler->OnGetFile(pObject);
		if(pObject->IsReady)
		{
			CEasyString Key=pObject->FilePath;
			Key.Trim();
			Key.MakeUpper();
			FILE_CACHE ** ppObject=m_FileCachePool.Find(Key);			
			if(ppObject)
			{
				delete pObject;
			}
			else
			{
				if(m_FileCachePool.GetObjectCount()>=m_FileCachePool.GetBufferSize())
				{
					void * Pos=m_FileCachePool.GetFirstObjectPos();
					if(Pos)
					{
						CEasyString DeleteKey;
						ppObject=m_FileCachePool.GetNextObject(Pos,DeleteKey);
						delete *ppObject;
						m_FileCachePool.Delete(DeleteKey);
						Log("文件[%s]被从缓冲删除",
							(LPCTSTR)DeleteKey);
					}
				}
				
				if(m_FileCachePool.New(Key,&ppObject))
				{					
					*ppObject=pObject;					
				}
				else
				{
					delete pObject;
				}
			}
		}
		ProcessCount++;
		if(ProcessCount>ProcessPacketLimit)
		{
			break;
		}
	}
	return ProcessCount;
}

bool CFileCache::ReadFile(FILE_CACHE * pFileCache)
{
	Log("开始读取文件[%s]",
		(LPCTSTR)pFileCache->FilePath);
	CWinFileAccessor FileAccessor;
	if(FileAccessor.Open(pFileCache->FilePath,CWinFileAccessor::modeOpen|CWinFileAccessor::modeRead|CWinFileAccessor::shareShareAll))
	{		
		pFileCache->FileSize=FileAccessor.GetSize();

		if(pFileCache->FileSize)
		{

			CEasyBuffer ReadBuffer;
			CEasyBuffer PackBuffer;

			ReadBuffer.Create(pFileCache->FileSize);
			PackBuffer.Create(pFileCache->FileSize);

			size_t PackSize=PackBuffer.GetBufferSize();

			pFileCache->PackProp.Create(LZMA_PROPS_SIZE);
			size_t PackPropSize=LZMA_PROPS_SIZE;

			if(FileAccessor.Read(ReadBuffer.GetBuffer(),pFileCache->FileSize)==pFileCache->FileSize)
			{
				int ErrCode=LzmaCompress((BYTE *)PackBuffer.GetBuffer(),&PackSize,
					(BYTE *)ReadBuffer.GetBuffer(),pFileCache->FileSize,
					(BYTE *)pFileCache->PackProp.GetBuffer(),&PackPropSize,5,
					1<<24,3,0,2,32,8);
				if(ErrCode==SZ_ERROR_OUTPUT_EOF)
				{
					PackBuffer.Create(pFileCache->FileSize*2);
					PackSize=PackBuffer.GetBufferSize();
					ErrCode=LzmaCompress((BYTE *)PackBuffer.GetBuffer(),&PackSize,
						(BYTE *)ReadBuffer.GetBuffer(),pFileCache->FileSize,
						(BYTE *)pFileCache->PackProp.GetBuffer(),&PackPropSize,5,
						1<<24,3,0,2,32,8);
				}
				if(ErrCode==SZ_OK)
				{
					PackBuffer.SetUsedSize(PackSize);
					pFileCache->FileData=PackBuffer;
					pFileCache->PackSize=PackSize;
					pFileCache->PackProp.SetUsedSize(PackPropSize);

					Log("文件[%s]压缩完毕，压缩比=%f(%u/%u)",
						(LPCTSTR)pFileCache->FilePath,
						(float)PackSize/pFileCache->FileSize,PackSize,pFileCache->FileSize);
					
					CEasyTime LastWriteTime;
					FileAccessor.GetLastWriteTime(pFileCache->LastWriteTime);
					
					return true;
				}
				else
				{
					Log("文件[%s]压缩数据失败[%d]",
						(LPCTSTR)pFileCache->FilePath,ErrCode);
					return false;
				}
			}
			else
			{
				Log("文件[%s]读取数据失败",
					(LPCTSTR)pFileCache->FilePath);
				return false;
			}						
		}
		else
		{
			Log("文件[%s]长度为0",
				(LPCTSTR)pFileCache->FilePath);
			return true;

		}
		FileAccessor.Close();
	}
	else
	{
		Log("文件[%s]打开失败",
			(LPCTSTR)pFileCache->FilePath);
		return false;
	}
	return true;
}
