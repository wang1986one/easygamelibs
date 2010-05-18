/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSObjectGroup.cpp                                       */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO(CDOSObjectGroup,CEasyThread);

CDOSObjectGroup::CDOSObjectGroup(void)
{
	FUNCTION_BEGIN;
	m_pManager=NULL;
	m_Index=0;
	m_Weight=0;
	FUNCTION_END;
}

CDOSObjectGroup::~CDOSObjectGroup(void)
{
	FUNCTION_BEGIN;
	Destory();
	FUNCTION_END;
}

bool CDOSObjectGroup::Initialize(CDOSObjectManager * pManager,UINT Index)
{
	FUNCTION_BEGIN;
	m_pManager=pManager;
	m_Index=Index;
	UINT MaxObjectCount=m_pManager->GetServer()->GetConfig().MaxGroupObjectCount;
	if(MaxObjectCount<=0)
	{
		PrintDOSLog(0,"CDOSObjectGroup::Initialize:最大对象数错误");
		return false;
	}
	if(!m_ObjectRegisterQueue.Create(MaxObjectCount))
	{
		PrintDOSLog(0,"CDOSObjectGroup::Initialize:创建%u大小的注册队列失败");
		return false;
	}
	if(!m_ObjectUnregisterQueue.Create(MaxObjectCount))
	{
		PrintDOSLog(0,"CDOSObjectGroup::Initialize:创建%u大小的注销队列失败");
		return false;
	}
	if(!m_ObjectPool.Create(MaxObjectCount))
	{
		PrintDOSLog(0,"CDOSObjectGroup::Initialize:创建%u大小的对象池失败");
		return false;
	}
	return true;
	FUNCTION_END;
	return false;
}

void CDOSObjectGroup::Destory()
{
	FUNCTION_BEGIN;
	DOS_OBJECT_INFO ObjectInfo;
	while(m_ObjectRegisterQueue.PopFront(ObjectInfo))
	{
		ObjectInfo.pObject->Destory();
		SAFE_RELEASE(ObjectInfo.pObject);
	}
	m_ObjectUnregisterQueue.Clear();
	void * Pos=m_ObjectPool.GetFirstObjectPos();
	while(Pos)
	{		
		DOS_OBJECT_INFO *pObjectInfo=m_ObjectPool.GetNext(Pos);
		if(pObjectInfo)
		{
			pObjectInfo->pObject->Destory();
			SAFE_RELEASE(pObjectInfo->pObject);
		}
	}
	m_ObjectPool.Clear();
	FUNCTION_END;
}


BOOL CDOSObjectGroup::RegisterObject(DOS_OBJECT_INFO& ObjectInfo)
{
	FUNCTION_BEGIN;
	ObjectInfo.pObject->AddUseRef();
	if(m_ObjectRegisterQueue.PushBack(ObjectInfo))
	{
		m_Weight+=ObjectInfo.Weight;		
		return TRUE;
	}
	else
	{
		SAFE_RELEASE(ObjectInfo.pObject);
		return FALSE;
	}
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSObjectGroup::UnregisterObject(OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	return m_ObjectUnregisterQueue.PushBack(ObjectID)!=NULL;
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSObjectGroup::OnStart()
{
	FUNCTION_BEGIN;
	
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSObjectGroup::OnRun()
{
	FUNCTION_BEGIN;
	
	int ProcessCount=0;
	

	ProcessCount+=ProcessObjectRegister();	

	void * Pos=m_ObjectPool.GetFirstObjectPos();
	while(Pos)
	{
		DOS_OBJECT_INFO * pObjectInfo=m_ObjectPool.GetNext(Pos);
		if(pObjectInfo)
		{
			ProcessCount+=pObjectInfo->pObject->DoCycle();			
		}
	}

	ProcessCount+=ProcessObjectUnregister();
	
	
	if(ProcessCount==0)
	{
		DoSleep(1);
	}

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSObjectGroup::PushMessage(OBJECT_ID ObjectID,CDOSMessagePacket * pPacket)
{
	FUNCTION_BEGIN;
	CAutoLock Lock(m_EasyCriticalSection);

	if(ObjectID.ObjectIndex==BROAD_CAST_OBJECT_INDEX)
	{
		void * Pos=m_ObjectPool.GetFirstObjectPos();
		while(Pos)
		{
			DOS_OBJECT_INFO * pObjectInfo=m_ObjectPool.GetNext(Pos);
			if(pObjectInfo)
			{
				if(ObjectID.ObjectTypeID==BROAD_CAST_OBJECT_TYPE_ID||
					pObjectInfo->ObjectID.ObjectTypeID==ObjectID.ObjectTypeID)
				{
					pObjectInfo->pObject->PushMessage(pPacket);
				}
				
			}
		}		
		return TRUE;
	}
	else
	{
		DOS_OBJECT_INFO * pObjectInfo=m_ObjectPool.GetObject(ObjectID.ObjectIndex);
		if(pObjectInfo)
		{
			return pObjectInfo->pObject->PushMessage(pPacket);
		}
	}
	FUNCTION_END;
	return FALSE;
}


int CDOSObjectGroup::ProcessObjectRegister(int ProcessLimit)
{
	FUNCTION_BEGIN;
	CAutoLock Lock(m_EasyCriticalSection);

	int ProcessCount=0;
	DOS_OBJECT_INFO ObjectInfo;
	while(m_ObjectRegisterQueue.PopFront(ObjectInfo))
	{
		UINT ID;
		DOS_OBJECT_INFO * pObjectInfo=NULL;
		ID=m_ObjectPool.NewObject(&pObjectInfo);
		if(pObjectInfo)
		{
			*pObjectInfo=ObjectInfo;
			pObjectInfo->ObjectID.GroupIndex=m_Index;
			pObjectInfo->ObjectID.ObjectIndex=ID;
			pObjectInfo->pObject->SetObjectID(pObjectInfo->ObjectID);
			if(!pObjectInfo->pObject->Initialize())
			{
				UnregisterObject(pObjectInfo->ObjectID);
			}
		}
		else
		{
			m_Weight-=ObjectInfo.Weight;
			ObjectInfo.pObject->Destory();
			SAFE_RELEASE(ObjectInfo.pObject);
		}		
		ProcessLimit--;
		ProcessCount++;
		if(ProcessLimit<=0)
			break;
	}
	return ProcessCount;
	FUNCTION_END;
	return 0;
}
int CDOSObjectGroup::ProcessObjectUnregister(int ProcessLimit)
{
	FUNCTION_BEGIN;
	CAutoLock Lock(m_EasyCriticalSection);

	int ProcessCount=0;
	OBJECT_ID UnregisterObjectID;	
	while(m_ObjectUnregisterQueue.PopFront(UnregisterObjectID))
	{
		DOS_OBJECT_INFO * pObjectInfo=m_ObjectPool.GetObject(UnregisterObjectID.ObjectIndex);
		if(pObjectInfo)
		{
			m_Weight-=pObjectInfo->Weight;
			pObjectInfo->pObject->Destory();
			SAFE_RELEASE(pObjectInfo->pObject);
			m_ObjectPool.DeleteObject(UnregisterObjectID.ObjectIndex);
		}
		else
		{
			PrintDOSLog(0,"对象[%llX]无法找到",UnregisterObjectID.ID);
		}
		ProcessLimit--;
		ProcessCount++;
		if(ProcessLimit<=0)
			break;
	}
	FUNCTION_END;
	return 0;
}