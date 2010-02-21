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
		PrintDOSLog(0xff0000,"û�г�ʼ��������������������޷���ʼ����");
		return false;
	}
	if(m_pServer->GetConfig().ObjectGroupCount<=0)
	{
		PrintDOSLog(0xff0000,"������û����ȷ���ö���������������������޷���ʼ����");
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
	
	PrintDOSLog(0xffff,"���������������%d�������飡",m_pServer->GetConfig().ObjectGroupCount);
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
		PrintDOSLog(0xff0000,"�ն����޷�ע�ᣡ");
		return FALSE;
	}

	if(Weight<=0)
	{
		PrintDOSLog(0xff0000,"����Ȩ�ر�������㣡");
		return FALSE;
	}
		

	pObject->SetManager(this);
	pObject->SetRouter(GetServer()->GetRouter());	

	
	CDOSObjectGroup * pGroup=SelectGroup();

	if(pGroup==NULL)
	{
		PrintDOSLog(0xff0000,"�޷�������ʵĶ����飡");
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
		PrintDOSLog(0xff0000,"�޷���������ӵ������飡");
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
		PrintDOSLog(0xff0000,"�ն����޷�ע����");
		return FALSE;
	}

	UINT GroupIndex=pObject->GetObjectID().GroupIndex;
	if(GroupIndex>=m_ObjectGroups.GetCount())
	{
		PrintDOSLog(0xff0000,"����������%u��Ч",GroupIndex);
		return FALSE;
	}
	
	CDOSObjectGroup * pGroup=m_ObjectGroups[GroupIndex];
	
	if(pGroup)
	{
		if(!pGroup->UnregisterObject(pObject->GetObjectID()))
		{
			PrintDOSLog(0xff0000,"�����������ע������ʧ��");
			return FALSE;
		}
	}
	else
	{
		PrintDOSLog(0xff0000,"�޷��ҵ��������ڵĶ�����");
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
			PrintDOSLog(0xff0000,"CDOSObjectManager::PushMessage:����������%u��Ч",GroupIndex);
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
