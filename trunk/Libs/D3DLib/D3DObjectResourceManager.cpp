#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO_STATIC(CD3DObjectResourceManager,CNameObject);

CD3DObjectResourceManager::CD3DObjectResourceManager(CD3DDevice * pD3DDevice,int StorageSize):CNameObject()
{
	m_pD3DDevice=pD3DDevice;
	m_ObjectStorage.Create(StorageSize);
}

CD3DObjectResourceManager::~CD3DObjectResourceManager(void)
{
#ifdef _DEBUG
	void * Pos;

	Pos=m_ObjectStorage.GetFirstObjectPos();
	while(Pos)
	{
		char Msg[256];
		CD3DObjectResource * pResource=*(m_ObjectStorage.GetNext(Pos));
		sprintf_s(Msg,256,"对象资源<%s>未释放！\r\n",(LPCTSTR)pResource->GetName());
		OutputDebugString(Msg);

	}
	m_ObjectStorage.Destory();
#endif
}

bool CD3DObjectResourceManager::AddResource(CD3DObjectResource * pResource,LPCTSTR ResourceName)
{
	UINT ID=m_ObjectStorage.AddObject(pResource,ResourceName);
	if(ID)
	{
		pResource->SetID(ID);
		pResource->SetName(ResourceName);
		return true;
	}
	else
	{
		return false;
	}
}

CD3DObjectResource * CD3DObjectResourceManager::DeleteResource(UINT ID)
{
	CD3DObjectResource ** ppResource=m_ObjectStorage.GetObject(ID);
	if(ppResource)
	{
		m_ObjectStorage.DeleteObject(ID);		
		return *ppResource;
	}
	return NULL;
}

CD3DObjectResource * CD3DObjectResourceManager::DeleteResource(LPCTSTR ResourceName)
{
	CD3DObjectResource * pResource=*(m_ObjectStorage.GetObject(ResourceName));
	if(pResource)
	{
		m_ObjectStorage.DeleteObject(pResource->GetID());		
		return pResource;
	}
	return NULL;
}

int CD3DObjectResourceManager::GetCount()
{
	return m_ObjectStorage.GetObjectCount();
}

LPVOID CD3DObjectResourceManager::GetFirstPos()
{
	return m_ObjectStorage.GetFirstObjectPos();
}

LPVOID CD3DObjectResourceManager::GetLastPos()
{
	return m_ObjectStorage.GetLastObjectPos();
}

CD3DObjectResource * CD3DObjectResourceManager::GetNext(LPVOID& Pos)
{
	return *m_ObjectStorage.GetNext(Pos);
}

CD3DObjectResource * CD3DObjectResourceManager::GetPrev(LPVOID& Pos)
{
	return *m_ObjectStorage.GetPrev(Pos);
}

}