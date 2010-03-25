/****************************************************************************/
/*                                                                          */
/*      文件名:    USOFile.cpp                                              */
/*      创建日期:  2009年09月25日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"




IMPLEMENT_FILE_PATH_MANAGER(CUSOFile)
IMPLEMENT_FILE_CHANNEL_MANAGER(CUSOFile)

IMPLEMENT_CLASS_INFO(CUSOFile,CNameObject);


CUSOFile::CUSOFile():CNameObject()
{
	m_pFile=NULL;
}


CUSOFile::~CUSOFile(void)
{
	Destory();
}

void CUSOFile::Destory()
{
	for(int i=0;i<(int)m_Objects.size();i++)
	{
		SAFE_RELEASE(m_Objects[i]);
	}
	m_Objects.clear();

	ClearAllResource();


}

bool CUSOFile::Open(LPCTSTR USOFileName)
{
	CEasyString FileName=FindFileOne(USOFileName);

	IFileAccessor * pFile;

	pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;
	}
	bool Ret=Open(pFile);
	SAFE_RELEASE(pFile);
	return Ret;
}

bool CUSOFile::Open(IFileAccessor * pFile)
{
	USO_FILE_HEAD FileHead;
	USO_BLOCK_HEAD BlockHead;

	if(pFile==NULL)
		return false;

	m_pFile=pFile;
	Destory();

	m_pFile->Read(&FileHead,sizeof(FileHead));
	if(FileHead.Flag!=USO_FILE_FLAG||FileHead.Version!=USO_FILE_CUR_VERSION)
		return false;

	m_pFile->Read(&BlockHead,sizeof(BlockHead));
	while(!m_pFile->IsEOF())
	{
		switch(BlockHead.Type)
		{
		case USO_BLOCK_RESOURCE:
			if(!ReadResourceBlock(BlockHead))
				return false;
			break;
		case USO_BLOCK_OBJECT:
			if(!ReadObjectBlock(BlockHead))
				return false;
			break;
		default:
			m_pFile->Seek(BlockHead.Size,IFileAccessor::seekCurrent);
		}
		m_pFile->Read(&BlockHead,sizeof(BlockHead));
	}
	//整理资源，去除NULL
	for(int i=(int)m_Resources.GetObjectCount()-1;i>=0;i--)
	{
		if(m_Resources.GetObject(i)==NULL)
			m_Resources.DeleteObject(i);
	}
	return true;
}

bool CUSOFile::Save(LPCTSTR USOFileName)
{

	IFileAccessor * pFile;

	pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);
	if(pFile==NULL)
		return false;
	if(!pFile->Open(USOFileName,IFileAccessor::modeCreateAlways|IFileAccessor::modeReadWrite))
	{
		pFile->Release();
		return false;
	}
	bool Ret=Save(pFile);
	SAFE_RELEASE(pFile);
	return Ret;
}

bool CUSOFile::Save(IFileAccessor * pFile)
{
	USO_FILE_HEAD FileHead;
	USO_BLOCK_HEAD BlockHead;
	UINT BlockHeadOffset;
	UINT SavedOffset;

	if(pFile==NULL)
		return false;

	m_pFile=pFile;

	FileHead.Flag=USO_FILE_FLAG;
	FileHead.Version=USO_FILE_CUR_VERSION;

	//写入文件头
	m_pFile->Write(&FileHead,sizeof(FileHead));

	//写入资源块头
	BlockHeadOffset=(UINT)m_pFile->GetCurPos();
	BlockHead.Type=USO_BLOCK_RESOURCE;
	BlockHead.Size=0;
	m_pFile->Write(&BlockHead,sizeof(BlockHead));

	//写入资源
	for(int i=0;i<(int)m_Resources.GetObjectCount();i++)
	{
		UINT ObjectSize=m_Resources.GetObject(i)->GetSmartStructSize(0);
	
		CSmartStruct Packet(ObjectSize);
		if(!m_Resources.GetObject(i)->ToSmartStruct(Packet,this,0))
			return false;
		UINT WriteSize=(UINT)m_pFile->Write(Packet.GetData(),Packet.GetDataLen());
		if(WriteSize<Packet.GetDataLen())
			return false;
	}
	//计算块大小
	SavedOffset=(UINT)m_pFile->GetCurPos();
	BlockHead.Size=SavedOffset-BlockHeadOffset;
	//重新写入块头
	m_pFile->Seek(BlockHeadOffset,IFileAccessor::seekBegin);
	m_pFile->Write(&BlockHead,sizeof(BlockHead));
	m_pFile->Seek(SavedOffset,IFileAccessor::seekBegin);


	//写入对象块头
	BlockHeadOffset=(UINT)m_pFile->GetCurPos();
	BlockHead.Type=USO_BLOCK_OBJECT;
	BlockHead.Size=0;
	m_pFile->Write(&BlockHead,sizeof(BlockHead));
	//写入对象
	for(int i=0;i<(int)m_Objects.size();i++)
	{
		UINT ObjectSize=m_Objects[i]->GetSmartStructSize(0);

		CSmartStruct Packet(ObjectSize);
		if(!m_Objects[i]->ToSmartStruct(Packet,this,0))
			return false;
		UINT WriteSize=(UINT)m_pFile->Write(Packet.GetData(),Packet.GetDataLen());
		if(WriteSize<Packet.GetDataLen())
			return false;		
	}

	//计算块大小
	SavedOffset=(UINT)m_pFile->GetCurPos();
	BlockHead.Size=SavedOffset-BlockHeadOffset;
	//重新写入块头
	m_pFile->Seek(BlockHeadOffset,IFileAccessor::seekBegin);
	m_pFile->Write(&BlockHead,sizeof(BlockHead));

	return true;
}

IFileAccessor * CUSOFile::GetFile()
{
	return m_pFile;
}

int CUSOFile::ResourceObjectToIndex(CNameObject * pResource)
{
	return m_Resources.GetIndex(pResource);
}

CNameObject * CUSOFile::ResourceIndexToObject(UINT Index,CLASS_INFO& ClassInfo)
{
	if(Index<m_Resources.GetObjectCount())
	{
		CNameObject * pObject=m_Resources.GetObject(Index);
		if(pObject->IsKindOf(ClassInfo))
			return pObject;
		else
			return NULL;
	}
	else
		return NULL;
}

bool CUSOFile::AddResource(CNameObject * pResource)
{
	if(pResource==NULL)
		return false;
	pResource->RefreshStorageID();
	if(!m_Resources.Add(pResource))
		return false;
	pResource->AddUseRef();
	return true;
}

bool CUSOFile::AddResourceFront(CNameObject * pResource)
{
	if(pResource==NULL)
		return false;
	pResource->RefreshStorageID();
	if(!m_Resources.AddFront(pResource))
		return false;
	pResource->AddUseRef();
	return true;
}

CNameObject * CUSOFile::GetResource(LPCTSTR ResourceName)
{
	for(int i=0;i<(int)m_Resources.GetObjectCount();i++)
	{
		if(strcmp(m_Resources.GetObject(i)->GetName(),ResourceName)==0)
			return m_Resources.GetObject(i);
	}
	return NULL;
}
int CUSOFile::GetResourceCount()
{
	return m_Resources.GetObjectCount();
}

CNameObject * CUSOFile::GetResourceByIndex(int Index)
{
	if(Index>=0&&Index<(int)m_Resources.GetObjectCount())
		return m_Resources.GetObject(Index);
	return NULL;
}

bool CUSOFile::DeleteResource(CNameObject * pResource)
{
	if(m_Resources.DeleteObject(m_Resources.GetIndex(pResource)))
	{
		pResource->Release();
		return true;
	}
	return false;
}

void CUSOFile::ClearAllResource()
{
	for(int i=0;i<(int)m_Resources.GetObjectCount();i++)
	{
		SAFE_RELEASE(m_Resources.GetObject(i));
	}
	m_Resources.Clear();
}

void CUSOFile::AddObject(CNameObject * pObject)
{
	if(pObject==NULL)
		return;
	pObject->AddUseRef();
	m_Objects.push_back(pObject);
	PickResource(pObject);

}

void CUSOFile::PickResource(CNameObject * pObject)
{
	int Index=m_Resources.GetObjectCount();
	pObject->PickResource(&m_Resources);
	for(;Index<(int)m_Resources.GetObjectCount();Index++)
	{
		m_Resources.GetObject(Index)->AddUseRef();
	}
}

CNameObject * CUSOFile::GetObject(LPCTSTR ObjectName)
{
	for(int i=0;i<(int)m_Objects.size();i++)
	{
		if(_stricmp(m_Objects[i]->GetName(),ObjectName)==0)
			return m_Objects[i];
	}
	return NULL;
}

CNameObject * CUSOFile::GetObjectRecursive(LPCTSTR ObjectName)
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

int CUSOFile::GetObjectCount()
{
	return (int)m_Objects.size();
}

CNameObject * CUSOFile::GetObjectByIndex(int Index)
{
	if(Index>=0&&Index<(int)m_Objects.size())
		return m_Objects[Index];
	return NULL;
}

bool CUSOFile::DeleteObject(CNameObject * pObject)
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

//bool CUSOFile::GetObjectHead(CNameObject::STORAGE_STRUCT& Head)
//{
//	if(m_pFile==NULL)
//		return false;
//
//	UINT SavedOffset=(UINT)m_pFile->GetCurPos();
//
//	m_pFile->Read(&Head,sizeof(Head));
//	m_pFile->Seek(SavedOffset,IFileAccessor::seekBegin);
//	return true;
//}

//bool CUSOFile::ReadResourceBlock(USO_BLOCK_HEAD& BlockHead)
//{
//	UINT ReadSize=sizeof(USO_BLOCK_HEAD);
//	UINT StartOffset;
//
//	if(BlockHead.Type!=USO_BLOCK_RESOURCE)
//		return false;
//	if(m_pFile==NULL)
//		return false;
//	StartOffset=(UINT)m_pFile->GetCurPos();
//	while(ReadSize<BlockHead.Size&&(!m_pFile->IsEOF()))
//	{
//		CNameObject::STORAGE_STRUCT ObjectHead;
//
//		UINT SavedOffset=(UINT)m_pFile->GetCurPos();
//
//		GetObjectHead(ObjectHead);
//		CNameObject * pObject=CreateObject(ObjectHead.Type,ObjectHead.Name);
//		if(pObject)
//		{
//			pObject->FromUSOFile(this,USO_SAVE_MODE_RESOURCE);
//			m_Resources.Add(pObject);
//		}
//		else
//		{
//			//装入失败也插入一个NULL，保证顺序
//			m_pFile->Seek(SavedOffset+ObjectHead.Size,IFileAccessor::seekBegin);
//			m_Resources.AddForce(NULL);
//		}
//		ReadSize=(UINT)m_pFile->GetCurPos()-StartOffset+sizeof(USO_BLOCK_HEAD);
//	}
//	if(ReadSize>=BlockHead.Size)
//		return true;
//	return false;
//}
//
//bool CUSOFile::ReadObjectBlock(USO_BLOCK_HEAD& BlockHead)
//{
//	UINT ReadSize=sizeof(USO_BLOCK_HEAD);
//	UINT StartOffset;
//
//	if(BlockHead.Type!=USO_BLOCK_OBJECT)
//		return false;
//	if(m_pFile==NULL)
//		return false;
//	StartOffset=(UINT)m_pFile->GetCurPos();
//	while(ReadSize<BlockHead.Size&&(!m_pFile->IsEOF()))
//	{
//		CNameObject::STORAGE_STRUCT ObjectHead;
//
//		UINT SavedOffset=(UINT)m_pFile->GetCurPos();
//
//		GetObjectHead(ObjectHead);
//		CNameObject * pObject=CreateObject(ObjectHead.Type,ObjectHead.Name);
//		if(pObject)
//		{
//			pObject->FromUSOFile(this,USO_SAVE_MODE_OBJECT);
//			m_Objects.push_back(pObject);
//		}
//		else
//		{
//			m_pFile->Seek(SavedOffset+ObjectHead.Size,IFileAccessor::seekBegin);
//		}
//
//		ReadSize=(UINT)m_pFile->GetCurPos()-StartOffset+sizeof(USO_BLOCK_HEAD);
//	}
//	if(ReadSize>=BlockHead.Size)
//		return true;
//	return false;
//}

bool CUSOFile::ReadResourceBlock(USO_BLOCK_HEAD& BlockHead)
{
	UINT ReadSize=sizeof(USO_BLOCK_HEAD);

	if(BlockHead.Type!=USO_BLOCK_RESOURCE)
		return false;
	if(m_pFile==NULL)
		return false;
	while(ReadSize<BlockHead.Size&&(!m_pFile->IsEOF()))
	{
		CSmartStruct ObjectPacket;
		BYTE Type;
		if(m_pFile->Read(&Type,sizeof(BYTE))!=sizeof(BYTE))
			return false;
		if(Type!=CSmartValue::VT_STRUCT)
			return false;
		UINT Size;
		if(m_pFile->Read(&Size,sizeof(UINT))!=sizeof(UINT))
			return false;
		ObjectPacket.Create(Size);
		BYTE * pDataBuffer=(BYTE *)ObjectPacket.GetData();
		*pDataBuffer=Type;
		pDataBuffer++;
		*((UINT *)pDataBuffer)=Size;
		pDataBuffer+=sizeof(UINT);
		if(m_pFile->Read(pDataBuffer,Size)!=Size)
			return false;

		LPCTSTR szType=ObjectPacket.GetMember(CNameObject::SST_NO_CLASS_NAME);
		LPCTSTR szName=ObjectPacket.GetMember(CNameObject::SST_NO_OBJECT_NAME);
		
		CNameObject * pObject=CreateObject(szType,szName);
		if(pObject)
		{
			if(pObject->FromSmartStruct(ObjectPacket,this,0))
			{
				m_Resources.Add(pObject);
			}
			else
			{
				PrintSystemLog(0,"CUSOFile::ReadResourceBlock:装载对象%s:%s失败",szType,szName);
				m_Resources.AddForce(NULL);
			}
			
		}
		else
		{
			//装入失败也插入一个NULL，保证顺序			
			m_Resources.AddForce(NULL);
		}
		ReadSize+=ObjectPacket.GetDataLen();
	}
	if(ReadSize>=BlockHead.Size)
		return true;
	return false;
}

bool CUSOFile::ReadObjectBlock(USO_BLOCK_HEAD& BlockHead)
{
	UINT ReadSize=sizeof(USO_BLOCK_HEAD);
	if(BlockHead.Type!=USO_BLOCK_OBJECT)
		return false;
	if(m_pFile==NULL)
		return false;
	while(ReadSize<BlockHead.Size&&(!m_pFile->IsEOF()))
	{
		CSmartStruct ObjectPacket;
		BYTE Type;
		if(m_pFile->Read(&Type,sizeof(BYTE))!=sizeof(BYTE))
			return false;
		if(Type!=CSmartValue::VT_STRUCT)
			return false;
		UINT Size;
		if(m_pFile->Read(&Size,sizeof(UINT))!=sizeof(UINT))
			return false;
		ObjectPacket.Create(Size);
		BYTE * pDataBuffer=(BYTE *)ObjectPacket.GetData();
		*pDataBuffer=Type;
		pDataBuffer++;
		*((UINT *)pDataBuffer)=Size;
		pDataBuffer+=sizeof(UINT);
		if(m_pFile->Read(pDataBuffer,Size)!=Size)
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
				PrintSystemLog(0,"CUSOFile::ReadObjectBlock:装载对象%s:%s失败",szType,szName);
				
			}

		}
		

		ReadSize+=ObjectPacket.GetDataLen();
	}
	if(ReadSize>=BlockHead.Size)
		return true;
	return false;
}

CNameObject * CUSOFile::CreateObject(LPCTSTR TypeName,LPCTSTR ObjectName,bool ReferenceWhenExist)
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



void CUSOFile::AddObjectCreateInfoByName(LPCTSTR ObjectName,CLASS_INFO * pClassInfo)
{
	m_ObjectCreateInfoByName[std::string(ObjectName)]=pClassInfo;
}

void CUSOFile::AddObjectCreateInfoByType(LPCTSTR ObjectType,CLASS_INFO * pClassInfo)
{
	m_ObjectCreateInfoByType[std::string(ObjectType)]=pClassInfo;
}


