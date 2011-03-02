/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DObjectResource.cpp                                    */
/*      ��������:  2009��09��25��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DObjectResource,CNameObject);

CD3DObjectResource::CD3DObjectResource():CNameObject()
{
	m_pManager=NULL;	
}

CD3DObjectResource::CD3DObjectResource(CD3DObjectResourceManager* pManager):CNameObject()
{
	m_pManager=pManager;	
}

CD3DObjectResource::~CD3DObjectResource(void)
{
	Destory();
	if(m_pManager)
		m_pManager->DeleteResource(this->GetID());
}

void CD3DObjectResource::Destory()
{
	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{
		SAFE_RELEASE(m_SubMeshList[i]);
	}
	m_SubMeshList.Clear();	
	CNameObject::Destory();
}

bool CD3DObjectResource::Reset()
{
	bool Ret=true;
	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{		
		Ret=Ret&&m_SubMeshList[i]->Reset();
	}	
	return Ret;
}

bool CD3DObjectResource::Restore()
{
	bool Ret=true;
	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{		
		Ret=Ret&&m_SubMeshList[i]->Restore();
	}	
	return Ret;
}

int CD3DObjectResource::GetSubMeshCount()
{
	return (int)m_SubMeshList.GetCount();
}

CD3DSubMesh * CD3DObjectResource::GetSubMesh(UINT index)
{
	if((UINT)index<m_SubMeshList.GetCount())
		return m_SubMeshList[index];
	else
		return NULL;
}


CD3DBoundingBox * CD3DObjectResource::GetBoundingBox()
{
	return &m_BoundingBox;
}

CD3DBoundingSphere * CD3DObjectResource::GetBoundingSphere()
{
	return &m_BoundingSphere;
}

void CD3DObjectResource::PickResource(CUSOResourceManager * pResourceManager,UINT Param)
{
	for(int i=0;i<GetSubMeshCount();i++)
	{
		CD3DSubMesh * pSubMesh=GetSubMesh(i);
		if(pSubMesh)
		{
			pSubMesh->PickResource(pResourceManager,Param);
		}
	}	
}

bool CD3DObjectResource::ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
{
	if(!CNameObject::ToSmartStruct(Packet,pResourceManager,Param))
		return false;	

	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DOR_BOUDING_BOX,(char *)&m_BoundingBox,sizeof(m_BoundingBox)));	
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DOR_BOUDING_SPHERE,(char *)&m_BoundingSphere,sizeof(m_BoundingSphere)));

	
	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		if(!m_SubMeshList[i]->ToSmartStruct(SubPacket,pResourceManager,Param))
			return false;
		if(!Packet.FinishMember(SST_D3DOR_SUB_MESH,SubPacket.GetDataLen()))
			return false;
	}
	
	return true;
}

bool CD3DObjectResource::FromSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
{
	if(!CNameObject::FromSmartStruct(Packet,pResourceManager,Param))
		return false;

	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_D3DOR_BOUDING_BOX:
			memcpy(&m_BoundingBox,(LPCTSTR)Value,sizeof(m_BoundingBox));
			break;
		case SST_D3DOR_BOUDING_SPHERE:
			memcpy(&m_BoundingSphere,(LPCTSTR)Value,sizeof(m_BoundingSphere));
			break;
		case SST_D3DOR_SUB_MESH:
			{
				CSmartStruct SubPacket=Value;
				LPCTSTR ClassName=SubPacket.GetMember(SST_NO_CLASS_NAME);
				LPCTSTR ObjectName=SubPacket.GetMember(SST_NO_OBJECT_NAME);
				CNameObject * pObject=pResourceManager->CreateObject(ClassName,ObjectName);
				if(pObject==NULL)
					return false;
				if(!pObject->IsKindOf(GET_CLASS_INFO(CD3DSubMesh)))
				{
					SAFE_RELEASE(pObject);
					return false;
				}
				if(!pObject->FromSmartStruct(SubPacket,pResourceManager,(UINT)(m_pManager->GetDevice())))
					return false;
				m_SubMeshList.Add((CD3DSubMesh *)pObject);
			}
			break;
		}
	}
	if(m_pManager)
		m_pManager->AddResource(this,GetName());
	return true;
}

UINT CD3DObjectResource::GetSmartStructSize(UINT Param)
{
	UINT Size=CNameObject::GetSmartStructSize(Param);
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_BoundingBox));
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_BoundingSphere));
	
	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(m_SubMeshList[i]->GetSmartStructSize(Param));
	}
	
	return Size;
}

void CD3DObjectResource::CreateBounding()
{

	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{		
		m_SubMeshList[i]->CreateBounding();
		if(i==0)
		{
			m_BoundingBox=m_SubMeshList[i]->GetBoundingBox();
			m_BoundingSphere=m_SubMeshList[i]->GetBoundingSphere();
		}
		else
		{
			m_BoundingBox+=m_SubMeshList[i]->GetBoundingBox();
			m_BoundingSphere+=m_SubMeshList[i]->GetBoundingSphere();
		}		
	}
}



//CNameObject::STORAGE_STRUCT * CD3DObjectResource::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//
//int CD3DObjectResource::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pResourceManager,UINT Param)
//{
//	int HeadSize=CNameObject::USOWriteHead(pHead,pResourceManager,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	pLocalHead->BoundingBox=m_BoundingBox;
//	pLocalHead->BoundingSphere=m_BoundingSphere;
//	pLocalHead->SubMeshCount=(int)m_SubMeshList.GetCount();
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//bool CD3DObjectResource::USOWriteChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pResourceManager,UINT Param)
//{
//	if(!CNameObject::USOWriteChild(pHead,pResourceManager,Param))
//		return false;
//	if(pResourceManager==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pResourceManager->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
//	{
//		if(!m_SubMeshList[i]->ToUSOFile(pResourceManager,(UINT)(m_pManager->GetDevice())))
//			return false;		
//	}
//
//	return true;
//}
//
//
//int CD3DObjectResource::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pResourceManager,UINT Param)
//{	
//	int ReadSize=CNameObject::USOReadHead(pHead,pResourceManager,Param);
//	if(ReadSize<0)
//		return -1;
//
//	if(pHead->Size<sizeof(STORAGE_STRUCT))
//		return false;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//
//	m_BoundingBox=pLocalHead->BoundingBox;
//	m_BoundingSphere=pLocalHead->BoundingSphere;	
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//
//bool CD3DObjectResource::USOReadChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pResourceManager,UINT Param)
//{	
//	if(!CNameObject::USOReadChild(pHead,pResourceManager,Param))
//		return false;
//
//	if(pResourceManager==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pResourceManager->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	//PrintImportantLog(0,"CD3DObjectResource��Ҫ��ȡ%d��CD3DSubMesh",pLocalHead->SubMeshCount);
//
//	for(int i=0;i<pLocalHead->SubMeshCount;i++)
//	{
//		CD3DSubMesh * pD3DSubMesh=new CD3DSubMesh;
//		m_SubMeshList.Add(pD3DSubMesh);
//		if(!pD3DSubMesh->FromUSOFile(pResourceManager,(UINT)(m_pManager->GetDevice())))
//		{			
//			return false;
//		}
//	}	
//	return true;
//}
//
//bool CD3DObjectResource::USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param)
//{
//	if(!CNameObject::USOReadFinish(pHead,Param))
//		return false;
//
//	if(m_pManager)
//		m_pManager->AddResource(this,GetName());
//
//	return true;
//}

}