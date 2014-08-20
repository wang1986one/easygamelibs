/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSObjectManager.cpp                                     */
/*      ��������:  2009��10��23��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
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
		PrintDOSLog(0xff0000,_T("û�г�ʼ��������������������޷���ʼ����"));
		return false;
	}
	if(m_pServer->GetConfig().ObjectGroupCount<=0)
	{
		PrintDOSLog(0xff0000,_T("������û����ȷ���ö���������������������޷���ʼ����"));
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
	
	PrintDOSLog(0xffff,_T("���������������%d�������飡"),m_pServer->GetConfig().ObjectGroupCount);
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

BOOL CDOSObjectManager::RegisterObject(DOS_OBJECT_REGISTER_INFO& ObjectRegisterInfo)
{
	FUNCTION_BEGIN;
	


	if(ObjectRegisterInfo.pObject==NULL)
	{
		PrintDOSLog(0xff0000,_T("�ն����޷�ע�ᣡ"));
		return FALSE;
	}

	if(ObjectRegisterInfo.Weight<=0)
	{
		PrintDOSLog(0xff0000,_T("����Ȩ�ر�������㣡"));
		return FALSE;
	}
		

	ObjectRegisterInfo.pObject->SetManager(this);
	ObjectRegisterInfo.pObject->SetRouter(GetServer()->GetRouter());	

	
	CDOSObjectGroup * pGroup=SelectGroup(ObjectRegisterInfo.ObjectGroupIndex);

	if(pGroup==NULL)
	{
		PrintDOSLog(0xff0000,_T("�޷�������ʵĶ����飡"));
		return FALSE;
	}

	ObjectRegisterInfo.ObjectGroupIndex=pGroup->GetIndex();
		
	ObjectRegisterInfo.ObjectID.RouterID=GetServer()->GetRouter()->GetRouterID();
		

	if(!pGroup->RegisterObject(ObjectRegisterInfo))
	{
		PrintDOSLog(0xff0000,_T("�޷���������ӵ������飡"));
		return FALSE;
	}

	return TRUE;
	FUNCTION_END;
	return FALSE;

}

BOOL CDOSObjectManager::UnregisterObject(OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;	


	UINT GroupIndex=ObjectID.GroupIndex;
	if(GroupIndex>=m_ObjectGroups.GetCount())
	{
		PrintDOSLog(0xff0000,_T("����������%u��Ч"),GroupIndex);
		return FALSE;
	}
	
	CDOSObjectGroup * pGroup=m_ObjectGroups[GroupIndex];
	
	if(pGroup)
	{
		if(!pGroup->UnregisterObject(ObjectID))
		{
			PrintDOSLog(0xff0000,_T("�����������ע������ʧ��"));
			return FALSE;
		}
	}
	else
	{
		PrintDOSLog(0xff0000,_T("�޷��ҵ��������ڵĶ�����"));
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
			PrintDOSLog(0xff0000,_T("CDOSObjectManager::PushMessage:����������%u��Ч"),GroupIndex);
			return FALSE;
		}
		return m_ObjectGroups[GroupIndex]->PushMessage(ObjectID,pPacket);
	}
	
	FUNCTION_END;
	return FALSE;
}


CDOSObjectGroup * CDOSObjectManager::SelectGroup(int GroupIndex)
{
	FUNCTION_BEGIN;
	if(GroupIndex>=0&&GroupIndex<m_ObjectGroups.GetCount())
	{
		return m_ObjectGroups[GroupIndex];
	}
	else
	{
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
	}
	FUNCTION_END;
	return NULL;
}


void CDOSObjectManager::PrintGroupInfo(UINT LogChannel)
{
	FUNCTION_BEGIN;
	for(UINT i=0;i<m_ObjectGroups.GetCount();i++)
	{		
		
		CLogManager::GetInstance()->PrintLog(LogChannel,ILogPrinter::LOG_LEVEL_NORMAL,0,
			_T("������[%u]:������[%u],Ȩ��[%u],CPUռ����[%0.2f%%],ѭ��ʱ��[%gMS]"),
			i,m_ObjectGroups[i]->GetObjectCount(),m_ObjectGroups[i]->GetWeight(),
			m_ObjectGroups[i]->GetCPUUsedRate()*100,
			m_ObjectGroups[i]->GetCycleTime());

		m_ObjectGroups[i]->PrintObjectStat(LogChannel);
	}
	FUNCTION_END;
}