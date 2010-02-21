#include "StdAfx.h"

IMPLEMENT_CLASS_INFO(CDOSObjectManager,CNameObject);

CDOSObjectManager::CDOSObjectManager(void)
{
	FUNCTION_BEGIN;
	m_pServer=NULL;
	FUNCTION_END;
}

CDOSObjectManager::~CDOSObjectManager(void)
{
	FUNCTION_BEGIN;
	Destory();
	FUNCTION_END;
}

bool CDOSObjectManager::Initialize()
{
	FUNCTION_BEGIN;
	if(m_pServer==NULL)
	{
		PrintDOSLog(0xff0000,"没有初始化服务器，对象管理器无法初始化！");
		return false;
	}
	if(m_pServer->GetConfig().ObjectGroupCount<=0)
	{
		PrintDOSLog(0xff0000,"服务器没有正确配置对象组数量，对象管理器无法初始化！");
		return false;
	}	
	m_ObjectGroups.Resize(m_pServer->GetConfig().ObjectGroupCount);
	for(UINT i=0;i<m_pServer->GetConfig().ObjectGroupCount;i++)
	{
		CDOSObjectGroup * pGroup=new CDOSObjectGroup();
		m_ObjectGroups[i]=pGroup;
		pGroup->Initialize(this,i);
		if(!pGroup->Start())
		{
			return false;
		}
	}
	
	PrintDOSLog(0xffff,"对象管理器创建了%d个对象组！",m_pServer->GetConfig().ObjectGroupCount);
	return true;
	FUNCTION_END;
	return false;
}

void CDOSObjectManager::Destory()
{
	FUNCTION_BEGIN;	
	for(int i=0;i<(int)m_ObjectGroups.GetCount();i++)
	{
		m_ObjectGroups[i]->SafeTerminate();		
	}
	for(int i=0;i<(int)m_ObjectGroups.GetCount();i++)
	{		
		SAFE_RELEASE(m_ObjectGroups[i]);
	}
	m_ObjectGroups.Clear();
	FUNCTION_END;
}

BOOL CDOSObjectManager::RegisterObject(CDOSBaseObject * pObject,OBJECT_ID ObjectID,int Weight)
{
	FUNCTION_BEGIN;
	


	if(pObject==NULL)
	{
		PrintDOSLog(0xff0000,"空对象无法注册！");
		return FALSE;
	}

	if(Weight<=0)
	{
		PrintDOSLog(0xff0000,"对象权重必须大于零！");
		return FALSE;
	}
		

	pObject->SetManager(this);
	pObject->SetRouter(GetServer()->GetRouter());	

	
	CDOSObjectGroup * pGroup=SelectGroup();

	if(pGroup==NULL)
	{
		PrintDOSLog(0xff0000,"无法分配合适的对象组！");
		return FALSE;
	}

	DOS_OBJECT_INFO ObjectInfo;
	ObjectInfo.ObjectID.ID=0;
	ObjectInfo.ObjectID.RouterID=GetServer()->GetRouter()->GetRouterID();
	ObjectInfo.ObjectID.ObjectTypeID=ObjectID.ObjectTypeID;
	ObjectInfo.Weight=Weight;
	ObjectInfo.pObject=pObject;

	if(!pGroup->RegisterObject(ObjectInfo))
	{
		PrintDOSLog(0xff0000,"无法将对象添加到对象组！");
		return FALSE;
	}

	return TRUE;
	FUNCTION_END;
	return FALSE;

}

BOOL CDOSObjectManager::UnregisterObject(CDOSBaseObject * pObject)
{
	FUNCTION_BEGIN;	


	if(pObject==NULL)
	{
		PrintDOSLog(0xff0000,"空对象无法注销！");
		return FALSE;
	}

	UINT GroupIndex=pObject->GetObjectID().GroupIndex;
	if(GroupIndex>=m_ObjectGroups.GetCount())
	{
		PrintDOSLog(0xff0000,"对象所在组%u无效",GroupIndex);
		return FALSE;
	}
	
	CDOSObjectGroup * pGroup=m_ObjectGroups[GroupIndex];
	
	if(pGroup)
	{
		if(!pGroup->UnregisterObject(pObject->GetObjectID()))
		{
			PrintDOSLog(0xff0000,"向对象组请求注销对象失败");
			return FALSE;
		}
	}
	else
	{
		PrintDOSLog(0xff0000,"无法找到对象所在的对象组");
		return FALSE;
	}	
	
	return TRUE;
	FUNCTION_END;
	return FALSE;

}

BOOL CDOSObjectManager::PushMessage(OBJECT_ID ObjectID,CDOSMessagePacket * pPacket)
{
	FUNCTION_BEGIN;
	UINT GroupIndex=ObjectID.GroupIndex;
	if(GroupIndex==BROAD_CAST_GROUP_INDEX)
	{
		for(UINT i=0;i<m_ObjectGroups.GetCount();i++)
		{
			m_ObjectGroups[i]->PushMessage(ObjectID,pPacket);
		}
		return TRUE;
	}
	else
	{
		if(GroupIndex>=m_ObjectGroups.GetCount())
		{
			PrintDOSLog(0xff0000,"CDOSObjectManager::PushMessage:对象所在组%u无效",GroupIndex);
			return FALSE;
		}
		return m_ObjectGroups[GroupIndex]->PushMessage(ObjectID,pPacket);
	}
	
	FUNCTION_END;
	return FALSE;
}


CDOSObjectGroup * CDOSObjectManager::SelectGroup()
{
	FUNCTION_BEGIN;
	int Weight=0x7fffffff;
	CDOSObjectGroup * pGroup=NULL;
	for(UINT i=0;i<m_ObjectGroups.GetCount();i++)
	{
		if(m_ObjectGroups[i]->GetWeight()<Weight)
		{
			Weight=m_ObjectGroups[i]->GetWeight();
			pGroup=m_ObjectGroups[i];
		}
	}
	return pGroup;
	FUNCTION_END;
	return NULL;
}
