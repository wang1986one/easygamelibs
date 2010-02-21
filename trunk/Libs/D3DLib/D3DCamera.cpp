#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DCamera,CD3DObject);

CD3DCamera::CD3DCamera():CD3DObject()
{	
	m_ProjectMatrix.SetIdentity();
	m_ViewMatrix.SetIdentity();		
}

CD3DCamera::~CD3DCamera(void)
{
}

void CD3DCamera::GetPickRay(FLOAT ScreenX,FLOAT ScreenY,CD3DVector3& Point,CD3DVector3& Dir)
{
	CD3DVector3 v;

	v.x =  ScreenX / m_ProjectMatrix._11;
	v.y =  ScreenY / m_ProjectMatrix._22;
	v.z = 1.0f;
	CD3DMatrix Mat=GetWorldMatrix();
	

	Dir.x  = v.x*Mat._11 + v.y*Mat._21 + v.z*Mat._31;//+ Mat._41;
	Dir.y  = v.x*Mat._12 + v.y*Mat._22 + v.z*Mat._32;//+ Mat._42;
	Dir.z  = v.x*Mat._13 + v.y*Mat._23 + v.z*Mat._33;//+ Mat._43;
	Dir.Normalize();

	Point.x = Mat._41;
	Point.y = Mat._42;
	Point.z = Mat._43;
}

void CD3DCamera::GetPickRay(int ScreenX,int ScreenY,int ScreenWidth,int ScreenHeight,CD3DVector3& Point,CD3DVector3& Dir)
{
	GetPickRay((FLOAT)((2.0f*ScreenX/ScreenWidth)-1),
		(FLOAT)-((2.0f*ScreenY/ScreenHeight)-1),
		Point,Dir);
}

void CD3DCamera::Update(FLOAT Time)
{
	//更新世界矩阵
	FUNCTION_BEGIN;
	m_WorldMatrix=m_LocalMatrix;
	if(GetParent())
	{
		m_WorldMatrix*=GetParent()->GetWorldMatrix().GetRotation();
		m_WorldMatrix*=CD3DMatrix::FromTranslation(GetParent()->GetWorldMatrix().GetTranslation());
	}
	
	FUNCTION_END;

	//更新子对象
	for(UINT i=0;i<GetChildCount();i++)
		GetChildByIndex(i)->Update(Time);
}



//bool CD3DCamera::ToUSOFile(CUSOFile * pUSOFile,UINT Param)
//{
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	STORAGE_STRUCT Data;
//	
//	Data.ObjectHead.Size=sizeof(STORAGE_STRUCT);
//	strncpy_0(Data.ObjectHead.Type,USO_FILE_MAX_TYPE_LEN,GetClassInfo().ClassName,USO_FILE_MAX_TYPE_LEN);		
//	strncpy_0(Data.ObjectHead.Name,USO_FILE_MAX_OBJECT_NAME,GetName(),USO_FILE_MAX_OBJECT_NAME);
//	Data.ObjectHead.StorageID=GetStorageID();
//	Data.LocalMatrix=GetLocalMatrix();
//	Data.ProjMatrix=GetProjectMat();
//
//	pFile->Write(&Data,sizeof(Data));
//	
//	return true;
//}
//
//bool CD3DCamera::FromUSOFile(CUSOFile * pUSOFile,UINT Param)
//{
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	STORAGE_STRUCT * pData;
//	BYTE * pBuff;
//	UINT Size;
//
//	pFile->Read(&Size,sizeof(UINT));
//	pBuff=new BYTE[Size];
//	pFile->Read(pBuff+sizeof(UINT),Size-sizeof(UINT));
//	pData=(STORAGE_STRUCT *)pBuff;
//	pData->ObjectHead.Size=Size;
//	
//	
//	if((!GetClassInfo().IsKindOf(pData->ObjectHead.Type))||
//		pData->ObjectHead.Size<sizeof(STORAGE_STRUCT))
//	{	
//		delete[] pBuff;
//		return false;
//	}
//	pData->ObjectHead.Name[USO_FILE_MAX_OBJECT_NAME-1]=0;
//	SetName(pData->ObjectHead.Name);
//	SetStorageID(pData->ObjectHead.StorageID);
//	SetLocalMatrix(pData->LocalMatrix);
//	SetProjectMat(pData->ProjMatrix);
//	delete[] pBuff;
//	return true;
//}

//CNameObject::STORAGE_STRUCT * CD3DCamera::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//
//int CD3DCamera::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	UINT HeadSize=CD3DObject::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	pLocalHead->ProjMatrix=GetProjectMat();
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//int CD3DCamera::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int ReadSize=CD3DObject::USOReadHead(pHead,pUSOFile,Param);
//	if(ReadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	SetProjectMat(pLocalHead->ProjMatrix);
//	return sizeof(STORAGE_STRUCT);
//}

}