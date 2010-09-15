/****************************************************************************/
/*                                                                          */
/*      文件名:    FileCache.h                                              */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class IFileCacheEventHandler;

struct FILE_CACHE
{
	CEasyString					FilePath;
	UINT						FileSize;
	UINT						PackSize;
	CEasyTime					LastWriteTime;
	CEasyBuffer					PackProp;
	CEasyBuffer					FileData;
	UINT						QueryParam;
	bool						IsReady;
	IFileCacheEventHandler *	pEventHandler;
	FILE_CACHE()
	{
		FileSize=0;
		PackSize=0;
		QueryParam=0;
		IsReady=false;
		pEventHandler=NULL;
	}
};

class IFileCacheEventHandler
{
public:
	virtual void OnGetFile(FILE_CACHE * pFileCache)=0;
};

class CFileCache:
	public CEasyThread
{
protected:
	CStaticMap<CEasyString,FILE_CACHE *>	m_FileCachePool;
	CThreadSafeIDStorage<FILE_CACHE *>			m_QueryQueue;
	CThreadSafeIDStorage<FILE_CACHE *>			m_FinishQueue;
public:
	CFileCache(void);
	~CFileCache(void);

	bool Create(UINT CacheSize,UINT QueryQueueSize);
	void Destory();

	bool QueryGetFile(LPCTSTR FilePath,IFileCacheEventHandler * pEventHandler,UINT Param);	

	virtual BOOL OnRun();

	int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
protected:
	bool ReadFile(FILE_CACHE * pFileCache);
};
