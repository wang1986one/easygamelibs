/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOResourceManager.cpp                                   */
/*      ��������:  2009��09��25��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"




IMPLEMENT_FILE_PATH_MANAGER(CUSOResourceManager)
IMPLEMENT_FILE_CHANNEL_MANAGER(CUSOResourceManager)

IMPLEMENT_CLASS_INFO(CUSOResourceManager,CNameObject);


CUSOResourceManager::CUSOResourceManager():CNameObject()
{
}


CUSOResourceManager::~CUSOResourceManager(void)
{
	Destory();
	ClearObjectCreateInfoByName();
	ClearObjectCreateInfoByType();
}

void CUSOResourceManager::Destory()
{
	ClearAllObject();
	ClearAllResource();
}

bool CUSOResourceManager::Import(LPCTSTR USOFileName)
{
	CEasyString FileName=FindFileOne(USOFileName);

	IFileAccessor * pFile;

	pFile=CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;
	}
	bool Ret=Import(pFile);
	SAFE_RELEASE(pFile);
	return Ret;
}

bool CUSOResourceManager::Import(IFileAccessor * pFile)
{
	USO_FILE_HEAD FileHead;
	USO_BLOCK_HEAD BlockHead;

	if(pFile==NULL)
		return false;

	Destory();

	pFile->Read(&FileHead,sizeof(FileHead));
	if(FileHead.Flag!=USO_FILE_FLAG||FileHead.Version!=USO_FILE_CUR_VERSION)
		return false;

	pFile->Read(&BlockHead,sizeof(BlockHead));
	while(!pFile->IsEOF())
	{
		switch(BlockHead.Type)
		{
		case USO_BLOCK_RESOURCE:
			if(!ReadResourceBlock(pFile,BlockHead))
				return false;
			break;
		case USO_BLOCK_OBJECT:
			if(!ReadObjectBlock(pFile,BlockHead))
				return false;
			break;
		default:
			pFile->Seek(BlockHead.Size,IFileAccessor::seekCurrent);
		}
		pFile->Read(&BlockHead,sizeof(BlockHead));
	}
	
	return true;
}

bool CUSOResourceManager::ImportFromDir(LPCTSTR Dir)
{
	return false;
}

bool CUSOResourceManager::Export(LPCTSTR USOFileName)
{

	IFileAccessor * pFile;

	pFile=CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(USOFileName,IFileAccessor::modeCreateAlways|IFileAccessor::modeReadWrite))
	{
		pFile->Release();
		return false;
	}
	bool Ret=Export(pFile);
	SAFE_RELEASE(pFile);
	return Ret;
}

bool CUSOResourceManager::Export(IFileAccessor * pFile)
{
	USO_FILE_HEAD FileHead;

	if(pFile==NULL)
		return false;

	
	FileHead.Flag=USO_FILE_FLAG;
	FileHead.Version=USO_FILE_CUR_VERSION;

	//д���ļ�ͷ
	pFile->Write(&FileHead,sizeof(FileHead));

	
	if(!WriteResourceBlock(pFile,NULL))
		return false;

	if(!WriteObjectBlock(pFile,NULL))
		return false;

	return true;
}

bool CUSOResourceManager::ExportToDir(LPCTSTR Dir)
{
	CEasyString ExportDir=MakeFullPath(Dir);

	if(!CreateDirEx(ExportDir+"\\Resource"))
		return false;	

	IFileAccessor * pFile;

	pFile=CreateFileAccessor();
	if(pFile==NULL)
		return false;

	CEasyArray<CNameObject *> ResourceList;

	ResourceList.Reserve(m_Resources.GetObjectCount());

	

	void * Pos=m_Resources.GetFirstObjectPos();
	while(Pos)
	{
		CEasyString Key;
		CNameObject ** ppResource=m_Resources.GetNextObject(Pos,Key);
		if(ppResource)
		{
			ResourceList.Add(*ppResource);
		}
	}

	m_Resources.Clear();

	for(UINT i=0;i<ResourceList.GetCount();i++)
	{
		CEasyString ObjectName=ResourceList[i]->GetName();
		ObjectName.Replace(':','_');
		ObjectName="Resource\\"+ObjectName+".USO";
		
		
		ResourceList[i]->SetName(ObjectName);

		AddResource(ResourceList[i]);

		ObjectName=ExportDir+"\\"+ObjectName;

		CEasyString PathDir=GetPathDirectory(ObjectName);
		CreateDirEx(PathDir);

		if(!pFile->Open(ObjectName,IFileAccessor::modeCreateAlways|IFileAccessor::modeReadWrite))
		{
			pFile->Release();
			return false;
		}

		USO_FILE_HEAD FileHead;

		FileHead.Flag=USO_FILE_FLAG;
		FileHead.Version=USO_FILE_CUR_VERSION;

		pFile->Write(&FileHead,sizeof(FileHead));

		if(!WriteResourceBlock(pFile,ResourceList[i]))
		{
			pFile->Release();
			return false;
		}

		pFile->Close();
	}

	for(int i=0;i<(int)m_Objects.size();i++)
	{
		CEasyString ObjectName=m_Objects[i]->GetName();
		ObjectName.Replace(':','_');		
		ObjectName=ExportDir+"\\"+ObjectName+".USO";
		CEasyString PathDir=GetPathDirectory(ObjectName);
		CreateDirEx(PathDir);
		

		if(!pFile->Open(ObjectName,IFileAccessor::modeCreateAlways|IFileAccessor::modeReadWrite))
		{
			pFile->Release();
			return false;
		}

		USO_FILE_HEAD FileHead;

		FileHead.Flag=USO_FILE_FLAG;
		FileHead.Version=USO_FILE_CUR_VERSION;

		pFile->Write(&FileHead,sizeof(FileHead));

		if(!WriteResourceBlock(pFile,m_Objects[i]))
		{
			pFile->Release();
			return false;
		}

		pFile->Close();
	}

	return true;
}


CNameObject * CUSOResourceManager::FindResource(LPCTSTR szResourceName,CLASS_INFO& ClassInfo)
{
	if(szResourceName==NULL)
		return NULL;
	CEasyString ResourceName=szResourceName;
	ResourceName.MakeUpper();
	CNameObject ** ppResource=m_Resources.Find(ResourceName);
	if(ppResource)
	{
		if((*ppResource)->IsKindOf(ClassInfo))
			return (*ppResource);
	}	
	return NULL;
}

LPCTSTR CUSOResourceManager::FindResourceKey(CNameObject * pResource)
{
	void * Pos;
	Pos=m_Resources.GetFirstObjectPos();
	while(Pos)
	{
		CEasyString Key;
		CNameObject ** ppResource=m_Resources.GetNextObject(Pos,Key);
		if(ppResource)
		{
			if((*ppResource)==pResource)
				return Key;
		}
	}
	return NULL;
}

bool CUSOResourceManager::AddResource(CNameObject * pResource)
{
	if(pResource==NULL)
		return false;

	pResource->RefreshStorageID();
	CEasyString ResourceName=pResource->GetName();
	ResourceName.MakeUpper();
	CNameObject ** ppOldResource=m_Resources.Find(ResourceName);
	if(ppOldResource)
	{
		if((*ppOldResource)!=pResource)
		{
			PrintSystemLog(0,"��Դ[%s]�Ѿ������Ҷ���ͬ���ɶ��󽫱����",(*ppOldResource)->GetName());
		}
		SAFE_RELEASE(*ppOldResource);
	}
	
	m_Resources[ResourceName]=pResource;	
	pResource->AddUseRef();
	return true;
}



int CUSOResourceManager::GetResourceCount()
{
	return m_Resources.GetObjectCount();
}

LPVOID CUSOResourceManager::GetFirstResourcePos()
{
	return m_Resources.GetFirstObjectPos();
}
CNameObject * CUSOResourceManager::GetNextResource(LPVOID& Pos)
{
	CEasyString Key;
	CNameObject ** ppResource=m_Resources.GetNextObject(Pos,Key);
	if(ppResource)
		return *ppResource;
	return NULL;
}

bool CUSOResourceManager::DeleteResource(LPCTSTR szResourceName)
{
	CEasyString ResourceName=szResourceName;
	ResourceName.MakeUpper();
	return m_Resources.Delete(ResourceName);
}

void CUSOResourceManager::ClearAllResource()
{
	void * Pos;
	Pos=m_Resources.GetFirstObjectPos();
	while(Pos)
	{
		CEasyString Key;
		CNameObject ** ppResource=m_Resources.GetNextObject(Pos,Key);
		if(ppResource)
			SAFE_RELEASE(*ppResource);

	}
	m_Resources.Clear();
}

void CUSOResourceManager::AddObject(CNameObject * pObject)
{
	if(pObject==NULL)
		return;
	pObject->AddUseRef();
	m_Objects.push_back(pObject);
	PickResource(pObject);

}

void CUSOResourceManager::PickResource(CNameObject * pObject)
{
	pObject->PickResource(this);
}

CNameObject * CUSOResourceManager::GetObject(LPCTSTR ObjectName)
{
	for(int i=0;i<(int)m_Objects.size();i++)
	{
		if(_stricmp(m_Objects[i]->GetName(),ObjectName)==0)
			return m_Objects[i];
	}
	return NULL;
}

CNameObject * CUSOResourceManager::GetObjectRecursive(LPCTSTR ObjectName)
{
	CNameObject * pObject=GetObject(ObjectName);
	if(pObject)
		return pObject;
	for(int i=0;i<(int)m_Objects.size();i++)
	{
		if(m_Objects[i]->IsKindOf(GET_CLASS_INFO(CTreeObject)))
		{
			pObject=((CTreeObject *)m_Objects[i])->GetChildRecursive(ObjectName,true);
			if(pObject)
				return pObject;
		}
	}
	return NULL;
}

int CUSOResourceManager::GetObjectCount()
{
	return (int)m_Objects.size();
}

CNameObject * CUSOResourceManager::GetObjectByIndex(int Index)
{
	if(Index>=0&&Index<(int)m_Objects.size())
		return m_Objects[Index];
	return NULL;
}

bool CUSOResourceManager::DeleteObject(CNameObject * pObject)
{
	for(int i=0;i<(int)m_Objects.size();i++)
	{
		if(m_Objects[i]==pObject)
		{
			SAFE_RELEASE(m_Objects[i]);
			m_Objects.erase(m_Objects.begin()+i);
			return true;
		}
	}
	return false;
}

void CUSOResourceManager::ClearAllObject()
{
	for(int i=0;i<(int)m_Objects.size();i++)
	{
		SAFE_RELEASE(m_Objects[i]);
	}
	m_Objects.clear();
}

bool CUSOResourceManager::ReadResourceBlock(IFileAccessor * pFile,USO_BLOCK_HEAD& BlockHead)
{
	UINT ReadSize=sizeof(USO_BLOCK_HEAD);

	CEasyTimer Timer;
	Timer.SaveTime();
	UINT FXLoadTime=0;
	UINT TextureLoadTime=0;

	if(BlockHead.Type!=USO_BLOCK_RESOURCE)
		return false;
	if(pFile==NULL)
		return false;
	while(ReadSize<BlockHead.Size&&(!pFile->IsEOF()))
	{
		CSmartStruct ObjectPacket;
		BYTE Type;
		if(pFile->Read(&Type,sizeof(BYTE))!=sizeof(BYTE))
			return false;
		if(Type!=CSmartValue::VT_STRUCT)
			return false;
		UINT Size;
		if(pFile->Read(&Size,sizeof(UINT))!=sizeof(UINT))
			return false;
		ObjectPacket.Create(Size);
		BYTE * pDataBuffer=(BYTE *)ObjectPacket.GetData();
		*pDataBuffer=Type;
		pDataBuffer++;
		*((UINT *)pDataBuffer)=Size;
		pDataBuffer+=sizeof(UINT);
		if(pFile->Read(pDataBuffer,Size)!=Size)
			return false;

		LPCTSTR szType=ObjectPacket.GetMember(CNameObject::SST_NO_CLASS_NAME);
		LPCTSTR szName=ObjectPacket.GetMember(CNameObject::SST_NO_OBJECT_NAME);
		
		CNameObject * pObject=CreateObject(szType,szName);
		if(pObject)
		{
			CEasyTimer ObjectTimer;
			ObjectTimer.SaveTime();
			if(pObject->FromSmartStruct(ObjectPacket,this,0))
			{
				AddResource(pObject);				
			}			
			if(pObject->IsKindOf("CD3DFX"))
			{
				FXLoadTime+=ObjectTimer.GetPastTime();
			}
			if(pObject->IsKindOf("CD3DTexture"))
			{
				TextureLoadTime+=ObjectTimer.GetPastTime();
			}
			SAFE_RELEASE(pObject);
		}		
		ReadSize+=ObjectPacket.GetDataLen();
	}	
	PrintSystemLog(0,"��ȡ%u����Դ����,����%u����,���ж�ȡFX����%u����,��ȡ������%u����",
		m_Resources.GetObjectCount(),Timer.GetPastTime(),FXLoadTime,TextureLoadTime);
	if(ReadSize>=BlockHead.Size)
		return true;
	return false;
}

bool CUSOResourceManager::ReadObjectBlock(IFileAccessor * pFile,USO_BLOCK_HEAD& BlockHead)
{
	CEasyTimer Timer;
	Timer.SaveTime();

	UINT ReadSize=sizeof(USO_BLOCK_HEAD);
	if(BlockHead.Type!=USO_BLOCK_OBJECT)
		return false;
	if(pFile==NULL)
		return false;
	while(ReadSize<BlockHead.Size&&(!pFile->IsEOF()))
	{
		CSmartStruct ObjectPacket;
		BYTE Type;
		if(pFile->Read(&Type,sizeof(BYTE))!=sizeof(BYTE))
			return false;
		if(Type!=CSmartValue::VT_STRUCT)
			return false;
		UINT Size;
		if(pFile->Read(&Size,sizeof(UINT))!=sizeof(UINT))
			return false;
		ObjectPacket.Create(Size);
		BYTE * pDataBuffer=(BYTE *)ObjectPacket.GetData();
		*pDataBuffer=Type;
		pDataBuffer++;
		*((UINT *)pDataBuffer)=Size;
		pDataBuffer+=sizeof(UINT);
		if(pFile->Read(pDataBuffer,Size)!=Size)
			return false;

		LPCTSTR szType=ObjectPacket.GetMember(CNameObject::SST_NO_CLASS_NAME);
		LPCTSTR szName=ObjectPacket.GetMember(CNameObject::SST_NO_OBJECT_NAME);

		CNameObject * pObject=CreateObject(szType,szName);
		if(pObject)
		{
			if(pObject->FromSmartStruct(ObjectPacket,this,0))
			{
				m_Objects.push_back(pObject);
			}
			else
			{
				PrintSystemLog(0,"CUSOFile::ReadObjectBlock:װ�ض���%s:%sʧ��",szType,szName);
				
			}

		}
		

		ReadSize+=ObjectPacket.GetDataLen();
	}
	PrintSystemLog(0,"��ȡ%u������,����%u����",
		m_Objects.size(),Timer.GetPastTime());
	if(ReadSize>=BlockHead.Size)
		return true;
	return false;
}

bool CUSOResourceManager::WriteResourceBlock(IFileAccessor * pFile,CNameObject * pObject)
{
	USO_BLOCK_HEAD BlockHead;

	//д����Դ��ͷ
	UINT BlockHeadOffset=(UINT)pFile->GetCurPos();
	BlockHead.Type=USO_BLOCK_RESOURCE;
	BlockHead.Size=0;
	pFile->Write(&BlockHead,sizeof(BlockHead));

	//д����Դ
	if(pObject)
	{
		UINT ObjectSize=pObject->GetSmartStructSize(0);

		CSmartStruct Packet(ObjectSize);
		if(!pObject->ToSmartStruct(Packet,this,0))
			return false;
		UINT WriteSize=(UINT)pFile->Write(Packet.GetData(),Packet.GetDataLen());
		if(WriteSize<Packet.GetDataLen())
			return false;
	}
	else
	{
		void * Pos=m_Resources.GetFirstObjectPos();
		while(Pos)
		{
			CEasyString Key;
			CNameObject ** ppResource=m_Resources.GetNextObject(Pos,Key);
			if(ppResource)
			{
				UINT ObjectSize=(*ppResource)->GetSmartStructSize(0);

				CSmartStruct Packet(ObjectSize);
				if(!(*ppResource)->ToSmartStruct(Packet,this,0))
					return false;
				UINT WriteSize=(UINT)pFile->Write(Packet.GetData(),Packet.GetDataLen());
				if(WriteSize<Packet.GetDataLen())
					return false;
			}
		}
	}
	
	//������С
	UINT SavedOffset=(UINT)pFile->GetCurPos();
	BlockHead.Size=SavedOffset-BlockHeadOffset;
	//����д���ͷ
	pFile->Seek(BlockHeadOffset,IFileAccessor::seekBegin);
	pFile->Write(&BlockHead,sizeof(BlockHead));
	pFile->Seek(SavedOffset,IFileAccessor::seekBegin);

	return true;
}

bool CUSOResourceManager::WriteObjectBlock(IFileAccessor * pFile,CNameObject * pObject)
{
	USO_BLOCK_HEAD BlockHead;

	//д������ͷ
	UINT BlockHeadOffset=(UINT)pFile->GetCurPos();
	BlockHead.Type=USO_BLOCK_OBJECT;
	BlockHead.Size=0;
	pFile->Write(&BlockHead,sizeof(BlockHead));
	//д�����
	if(pObject)
	{
		UINT ObjectSize=pObject->GetSmartStructSize(0);

		CSmartStruct Packet(ObjectSize);
		if(!pObject->ToSmartStruct(Packet,this,0))
			return false;
		UINT WriteSize=(UINT)pFile->Write(Packet.GetData(),Packet.GetDataLen());
		if(WriteSize<Packet.GetDataLen())
			return false;
	}
	else
	{
		for(int i=0;i<(int)m_Objects.size();i++)
		{
			UINT ObjectSize=m_Objects[i]->GetSmartStructSize(0);

			CSmartStruct Packet(ObjectSize);
			if(!m_Objects[i]->ToSmartStruct(Packet,this,0))
				return false;
			UINT WriteSize=(UINT)pFile->Write(Packet.GetData(),Packet.GetDataLen());
			if(WriteSize<Packet.GetDataLen())
				return false;		
		}
	}

	//������С
	UINT SavedOffset=(UINT)pFile->GetCurPos();
	BlockHead.Size=SavedOffset-BlockHeadOffset;
	//����д���ͷ
	pFile->Seek(BlockHeadOffset,IFileAccessor::seekBegin);
	pFile->Write(&BlockHead,sizeof(BlockHead));

	return true;
}

CNameObject * CUSOResourceManager::CreateObject(LPCTSTR TypeName,LPCTSTR ObjectName,bool ReferenceWhenExist)
{
	if(TypeName==NULL||ObjectName==NULL)
		return NULL;

	CNameObject * pObject=NULL;

	CLASS_INFO * pClassInfo=NULL;
	std::map<std::string,CLASS_INFO *>::iterator itr=m_ObjectCreateInfoByName.find(ObjectName);
	if(itr!=m_ObjectCreateInfoByName.end())
	{
		pClassInfo=itr->second;
	}
	if(pClassInfo==NULL)
	{
		itr=m_ObjectCreateInfoByType.find(TypeName);
		if(itr!=m_ObjectCreateInfoByType.end())
		{
			pClassInfo=itr->second;
		}
	}
	if(pClassInfo==NULL)
	{
		pClassInfo=CNameObject::GetClassInfo(TypeName);
	}
	if(pClassInfo==NULL)
		return NULL;

	if(ReferenceWhenExist)
	{
		for(int i=0;i<(int)m_ObjectCreateFilterList.size();i++)
		{
			pObject=m_ObjectCreateFilterList[i]->FindObject(pClassInfo,ObjectName);
			if(pObject)
			{
				pObject->AddUseRef();
				break;
			}
		}
	}

	if(pObject==NULL)
	{
		pObject=pClassInfo->CreateObject();
		pObject->SetName(ObjectName);
	}

	if(pObject==NULL)
		return NULL;

	for(int i=0;i<(int)m_ObjectCreateFilterList.size();i++)
	{
		if(!m_ObjectCreateFilterList[i]->OnObjectCreate(pObject))
		{
			SAFE_RELEASE(pObject);
			return NULL;
		}
	}

	return pObject;

}



void CUSOResourceManager::AddObjectCreateInfoByName(LPCTSTR ObjectName,CLASS_INFO * pClassInfo)
{
	m_ObjectCreateInfoByName[std::string(ObjectName)]=pClassInfo;
}

void CUSOResourceManager::ClearObjectCreateInfoByName()
{
	m_ObjectCreateInfoByName.clear();
}

void CUSOResourceManager::AddObjectCreateInfoByType(LPCTSTR ObjectType,CLASS_INFO * pClassInfo)
{
	m_ObjectCreateInfoByType[std::string(ObjectType)]=pClassInfo;
}

void CUSOResourceManager::ClearObjectCreateInfoByType()
{
	m_ObjectCreateInfoByType.clear();
}
