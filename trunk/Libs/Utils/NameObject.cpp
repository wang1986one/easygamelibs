/****************************************************************************/
/*                                                                          */
/*      文件名:    NameObject.cpp                                           */
/*      创建日期:  2009年09月25日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"
#include "NameObject.h"

CLASS_INFO	CNameObject::m_CNameObjectClassInfo={"CNameObject",NULL,CNameObject::CreateObject};

std::map<std::string,CLASS_INFO *> * CNameObject::m_pAllClassInfo=NULL;
int CNameObject::m_AllClassCount=0;

CClassInfoRegister CNameObject::m_CNameObjectClassInfoRegister("CNameObject",&m_CNameObjectClassInfo);

CClassInfoRegister::CClassInfoRegister(LPCTSTR ClassName,CLASS_INFO * pClassInfo)
{
	if(CNameObject::m_pAllClassInfo==NULL)
		CNameObject::m_pAllClassInfo=new std::map<std::string,CLASS_INFO *>;
	(*CNameObject::m_pAllClassInfo)[std::string(ClassName)]=pClassInfo;
	CNameObject::m_AllClassCount++;
}
CClassInfoRegister::~CClassInfoRegister()
{
	CNameObject::m_AllClassCount--;
	if(CNameObject::m_AllClassCount<=0)
		SAFE_DELETE(CNameObject::m_pAllClassInfo);
}

CNameObject::CNameObject()
{
	m_ID=0;
	m_UseRef=1;
	m_pParam=NULL;		
}

CNameObject::~CNameObject()
{

}

CNameObject * CNameObject::CreateObject()
{
	return new CNameObject();
}


CLASS_INFO& CNameObject::GetThisClassInfo()
{
	return m_CNameObjectClassInfo;
}

CLASS_INFO * CNameObject::GetClassInfo(LPCTSTR ClassName)
{
	return (*m_pAllClassInfo)[ClassName];
}

int CNameObject::GetAllClassCount()
{
	return m_AllClassCount;
}

void CNameObject::RefreshStorageID()
{
	m_StorageID=(UINT)this;
}

//bool CNameObject::ToUSOFile(CUSOFile * pResourceManager,UINT Param)
//{
//	if(pResourceManager==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pResourceManager->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	RefreshStorageID();
//
//	UINT ObjectSize=GetSmartStructSize(Param);
//
//	CSmartStruct Packet(ObjectSize);
//	if(!ToSmartStruct(Packet,pResourceManager,Param))
//		return false;
//	UINT WriteSize=(UINT)pFile->Write(Packet.GetData(),Packet.GetDataLen());
//	if(WriteSize<Packet.GetDataLen())
//		return false;
//	return true;
//
//	//STORAGE_STRUCT * pHead=USOCreateHead(Param);
//	//if(pHead==NULL)
//	//	return false;
//	//UINT64 HeadPos=pFile->GetCurPos();
//	//int HeadSize=USOWriteHead(pHead,pResourceManager,Param);
//	//if(HeadSize<0)
//	//{
//	//	delete pHead;
//	//	return false;
//	//}
//	//int WriteSize=(int)pFile->Write(pHead,HeadSize);
//	//if(WriteSize<HeadSize)
//	//{
//	//	delete pHead;
//	//	return false;
//	//}	
//	//if(!USOWriteData(pHead,pResourceManager,Param))
//	//{
//	//	delete pHead;
//	//	return false;
//	//}
//	//HeadPos=pFile->GetCurPos()-HeadPos;
//	//if(HeadPos!=(UINT64)pHead->Size)
//	//{
//	//	delete pHead;
//	//	return false;
//	//}
//	//if(!USOWriteChild(pHead,pResourceManager,Param))
//	//{
//	//	delete pHead;
//	//	return false;
//	//}
//	//if(!USOWriteFinish(pHead,Param))
//	//{
//	//	delete pHead;
//	//	return false;
//	//}
//	//delete pHead;
//	return true;
//}
//bool CNameObject::FromUSOFile(CUSOFile * pResourceManager,UINT Param)
//{
//	if(pResourceManager==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pResourceManager->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	STORAGE_STRUCT * pHead;
//	BYTE * pBuff;
//	UINT Size;
//	int ReadSize;
//
//	pFile->Read(&Size,sizeof(UINT));
//	if(Size==0)
//		return false;
//	pBuff=new BYTE[Size];
//	pFile->Read(pBuff+sizeof(UINT),Size-sizeof(UINT));
//	pHead=(STORAGE_STRUCT *)pBuff;
//	pHead->Size=Size;
//
//// 	PrintImportantLog(0,"装载对象Type=%s,Name=%s",
//// 		pHead->Type,pHead->Name);
//
//
//	if((!GetClassInfo().IsKindOf(pHead->Type)))
//	{	
//		delete[] pBuff;
//		return false;
//	}
//
//	Destory();
//
//	ReadSize=USOReadHead(pHead,pResourceManager,Param);
//	//PrintImportantLog(0,"大小=%u,类型=%s",ReadSize,pHead->Type);
//
//	if(ReadSize<0)
//	{
//		delete[] pBuff;
//		return false;
//	}
//
//	BYTE * pData=pBuff+ReadSize;
//	int DataSize=Size-ReadSize;
//
//	ReadSize=USOReadData(pHead,pResourceManager,pData,DataSize,Param);
//	if(ReadSize<0)
//	{
//		delete[] pBuff;
//		return false;
//	}
//
//	if(!USOReadChild(pHead,pResourceManager,Param))
//	{
//		delete[] pBuff;
//		return false;
//	}
//
//	if(!USOReadFinish(pHead,Param))
//	{
//		delete[] pBuff;
//		return false;
//	}
//
//	delete[] pBuff;
//	return true;
//}

bool CNameObject::CloneFrom(CNameObject * pObject,UINT Param)
{
	Destory();

	m_Name=pObject->m_Name;
	//strncpy_0(m_Name,MAX_OBJECT_NAME,pObject->m_Name,MAX_OBJECT_NAME);		
	m_StorageID=pObject->m_StorageID;
	m_pParam=pObject->m_pParam;
	return true;
}
bool CNameObject::StealFrom(CNameObject * pObject,UINT Param)
{
	m_Name=pObject->m_Name;
	//strncpy_0(m_Name,MAX_OBJECT_NAME,pObject->m_Name,MAX_OBJECT_NAME);
	m_ID=pObject->m_ID;
	m_pParam=pObject->m_pParam;
	pObject->m_ID=0;
	return true;
}

inline void CNameObject::PickResource(CUSOResourceManager * pResourceManager,UINT Param)
{
}

bool CNameObject::ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
{
	RefreshStorageID();
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_NO_CLASS_NAME,GetClassInfo().ClassName));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_NO_OBJECT_NAME,(LPCTSTR)m_Name));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_NO_OBJECT_ID,m_ID));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_NO_STORAGE_ID,m_StorageID));
	return true;
}

bool CNameObject::FromSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
{
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_NO_CLASS_NAME:
			if(!IsKindOf(Value))
				return false;
			break;
		case SST_NO_OBJECT_NAME:
			m_Name=(LPCTSTR)Value;
			break;
		case SST_NO_OBJECT_ID:
			m_ID=Value;
			break;
		case SST_NO_STORAGE_ID:
			m_StorageID=Value;
			break;
		}
	}
	return true;
}

 UINT CNameObject::GetSmartStructSize(UINT Param)
{
	return SMART_STRUCT_STRING_MEMBER_SIZE(strlen(GetClassInfo().ClassName))+
		SMART_STRUCT_STRING_MEMBER_SIZE(m_Name.GetLength())+
		SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ID))+
		SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_StorageID));
}


//CNameObject::STORAGE_STRUCT * CNameObject::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//int CNameObject::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pResourceManager,UINT Param)
//{
//	if(pHead==NULL)
//		return false;	
//	strncpy_0(pHead->Type,MAX_TYPE_LEN,GetClassInfo().ClassName,MAX_TYPE_LEN);
//	strncpy_0(pHead->Name,MAX_OBJECT_NAME,GetName(),MAX_OBJECT_NAME);	
//	pHead->StorageID=GetStorageID();
//	pHead->ID=GetID();
//	return sizeof(STORAGE_STRUCT);
//}
//bool CNameObject::USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pResourceManager,UINT Param)
//{
//	return true;
//}
//bool CNameObject::USOWriteChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pResourceManager,UINT Param)
//{
//	return true;
//}
//bool CNameObject::USOWriteFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param)
//{
//	return true;
//}
//
//int CNameObject::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pResourceManager,UINT Param)
//{
//	pHead->Name[MAX_OBJECT_NAME-1]=0;
//	SetName(pHead->Name);
//	SetStorageID(pHead->StorageID);	
//	SetID(pHead->ID);
//	return sizeof(STORAGE_STRUCT);
//}
//int CNameObject::USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pResourceManager,BYTE * pData,int DataSize,UINT Param)
//{
//	return 0;
//}
//bool CNameObject::USOReadChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pResourceManager,UINT Param)
//{
//	return true;
//}
//bool CNameObject::USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param)
//{
//	return true;
//}