/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DStaticModel.cpp                                       */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DStaticModel,CD3DBaseStaticModel);

CD3DStaticModel::CD3DStaticModel(void):CD3DBaseStaticModel()
{
	m_pResource=NULL;
	m_Property=0;
}

CD3DStaticModel::~CD3DStaticModel(void)
{
	Destory();	
}

void CD3DStaticModel::Destory()
{
	CD3DBaseStaticModel::Destory();
}

bool CD3DStaticModel::Reset()
{	
	return m_pResource->Reset()&&CD3DBaseStaticModel::Reset();
}

bool CD3DStaticModel::Restore()
{
	return m_pResource->Restore()&&CD3DBaseStaticModel::Restore();
}

bool CD3DStaticModel::LoadFromXFile(LPCTSTR FileName)
{
	if(GetDevice()==NULL)
		return false;

	CD3DStaticModelResource* pResource=
		dynamic_cast<CD3DStaticModelResource*>(GetDevice()->GetObjectResourceManager()->GetResource(FileName));
	if(!pResource)
	{
		pResource=new CD3DStaticModelResource(GetDevice()->GetObjectResourceManager());
		if(!pResource->LoadFromXFile(FileName))
		{
			pResource->Release();
			return false;
		}
		if(!GetDevice()->GetObjectResourceManager()->AddResource(pResource,FileName))
		{
			pResource->Release();
			return false;
		}
	}
	else
	{
		pResource->AddUseRef();
	}
	Destory();
	m_pResource=pResource;
	SetName(m_pResource->GetName());
	CreateDummy();
	return true;
}

bool CD3DStaticModel::LoadFromMDL(LPCTSTR FileName)
{
	if(GetDevice()==NULL)
		return false;

	CD3DStaticModelResource* pResource=
		dynamic_cast<CD3DStaticModelResource*>(GetDevice()->GetObjectResourceManager()->GetResource(FileName));
	if(!pResource)
	{
		pResource=new CD3DStaticModelResource(GetDevice()->GetObjectResourceManager());
		if(!pResource->LoadFromMDL(FileName))
		{
			pResource->Release();
			return false;
		}
		if(!GetDevice()->GetObjectResourceManager()->AddResource(pResource,FileName))
		{
			pResource->Release();
			return false;
		}
	}
	else
	{
		pResource->AddUseRef();
	}
	Destory();
	m_pResource=pResource;
	SetName(m_pResource->GetName());
	CreateDummy();
	return true;
}

bool CD3DStaticModel::LoadFromSMDL(LPCTSTR FileName)
{
	if(GetDevice()==NULL)
		return false;

	CD3DStaticModelResource* pResource=
		dynamic_cast<CD3DStaticModelResource*>(GetDevice()->GetObjectResourceManager()->GetResource(FileName));
	if(!pResource)
	{
		pResource=new CD3DStaticModelResource(GetDevice()->GetObjectResourceManager());
		if(!pResource->LoadFromSMDL(FileName))
		{
			pResource->Release();
			return false;
		}
		if(!GetDevice()->GetObjectResourceManager()->AddResource(pResource,FileName))
		{
			pResource->Release();
			return false;
		}
	}
	else
	{
		pResource->AddUseRef();
	}
	Destory();
	m_pResource=pResource;
	SetName(m_pResource->GetName());
	CreateDummy();
	return true;
}



//void CD3DStaticModel::SetFx(CD3DFX * pFX)
//{
//	for(int i=0;i<GetSubMeshCount();i++)
//	{
//		CD3DSubMesh * pSubMesh=GetSubMesh(i);
//		if(pSubMesh)
//		{
//			SAFE_RELEASE(pSubMesh->GetMaterial().pFX);
//			pSubMesh->GetMaterial().pFX=pFX;
//		}
//	}
//}


//void CD3DStaticModel::PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera)
//{
//	if(pSubMesh&&pMaterial)
//	{	
//
//		if(pMaterial->GetFX())
//		{
//			//设置灯光
//			if(pLight[0])
//			{			
//				D3DLIGHT9 Light;
//				pLight[0]->GetCurLight(Light);
//				pMaterial->GetFX()->SetVector("LightDir",CD3DVector4(Light.Direction));
//				pMaterial->GetFX()->SetColor("LightAmbient",Light.Ambient);
//				pMaterial->GetFX()->SetColor("LightDiffuse",Light.Diffuse);
//				pMaterial->GetFX()->SetColor("LightSpecular",Light.Specular);
//			}
//			//设置材质
//			D3DMATERIAL9 * pD3DMaterial;
//			if(pSubMesh->IsSelected())
//				pD3DMaterial=&SELECTED_SUBMESH_MATERIAL;
//			else
//				pD3DMaterial=&(pMaterial->GetMaterial());
//			pMaterial->GetFX()->SetColor("MaterialAmbient",pD3DMaterial->Ambient);
//			pMaterial->GetFX()->SetColor("MaterialDiffuse",pD3DMaterial->Diffuse);
//			pMaterial->GetFX()->SetColor("MaterialSpecular",pD3DMaterial->Specular);
//			pMaterial->GetFX()->SetColor("MaterialEmissive",pD3DMaterial->Emissive);
//			pMaterial->GetFX()->SetFloat("MaterialPower",pD3DMaterial->Power);
//
//			//设置纹理
//			pMaterial->GetFX()->SetTexture("TexLay0",pMaterial->GetTexture(0));
//			pMaterial->GetFX()->SetTexture("TexLay1",pMaterial->GetTexture(1));
//
//			pMaterial->GetFX()->SetMatrix("WorldMatrix",GetWorldMatrix());
//			//设置视投影矩阵
//			pMaterial->GetFX()->SetMatrix("ViewMatrix",pCamera->GetViewMat());
//			pMaterial->GetFX()->SetMatrix("ProjMatrix",pCamera->GetProjectMat());
//			pMaterial->GetFX()->SetVector("CameraPos",pCamera->GetWorldMatrix().GetTranslation());
//			
//		}	
//	}
//	else
//	{
//
//	}
//}

int CD3DStaticModel::GetSubMeshCount()
{
	if(m_pResource)
		return m_pResource->GetSubMeshCount();
	else
		return 0;
}
CD3DSubMesh * CD3DStaticModel::GetSubMesh(int index)
{
	if(m_pResource)
		return m_pResource->GetSubMesh(index);
	else
		return NULL;
}

CD3DBoundingBox * CD3DStaticModel::GetBoundingBox()
{
	if(m_pResource)
		return m_pResource->GetBoundingBox();
	else
		return NULL;
}
CD3DBoundingSphere * CD3DStaticModel::GetBoundingSphere()
{
	if(m_pResource)
		return m_pResource->GetBoundingSphere();
	else
		return NULL;
}
//CD3DBoundingBox * CD3DStaticModel::GetWholeBoundingBox()
//{
//	return &m_WholeBoundingBox;
//}
//CD3DBoundingSphere * CD3DStaticModel::GetWholeBoundingSphere()
//{
//	return &m_WholeBoundingSphere;
//}

bool CD3DStaticModel::RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly)
{	
	bool IsIntersect=false;

	if(m_pResource==NULL)
		return false;

	Distance=3.4E+38f;
	FLOAT Dis;
	CD3DVector3 InterPoint;

	CD3DBoundingBox BBox;

	BBox=(*GetBoundingBox())*GetWorldMatrix();

	if(!BBox.RayIntersect(Point,Dir,IntersectPoint,Distance))
		return false;
	

	for(int i=0;i<m_pResource->GetSubMeshCount();i++)
	{
		CD3DSubMesh * pSubMesh=m_pResource->GetSubMesh(i);
		if(pSubMesh)
		{
			if(pSubMesh->RayIntersect(GetWorldMatrix(),Point,Dir,InterPoint,Dis,TestOnly))
			{
				IsIntersect=true;
				if(Dis<Distance)
				{
					Distance=Dis;
					IntersectPoint=InterPoint;
				}
				if(TestOnly)
				{
					break;
				}
			}
		}
	}
	return IsIntersect;
}

bool CD3DStaticModel::GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y)
{
	if(m_pResource==NULL)
		return false;



	CD3DBoundingBox Box=(*GetBoundingBox())*GetWorldMatrix();

	if(x<Box.m_Min.x||x>Box.m_Max.x||z<Box.m_Min.z||z>Box.m_Max.z)
		return false;
	CD3DVector3 Point(x,Box.m_Max.y,z);
	CD3DVector3 Dir(0,-1,0);
	CD3DVector3 IntersectPoint;
	FLOAT Distance;
	FLOAT Height=-MAX_HEIGHT;

	for(int i=0;i<m_pResource->GetSubMeshCount();i++)
	{
		CD3DSubMesh * pSubMesh=m_pResource->GetSubMesh(i);
		if(pSubMesh)
		{
			if(pSubMesh->GetProperty()&CD3DStaticModelResource::SMSMP_GROUND_LAYER)
			{
				if(pSubMesh->RayIntersect(GetWorldMatrix(),Point,Dir,IntersectPoint,Distance,false))
				{
					if(IntersectPoint.y>Height)					
						Height=IntersectPoint.y;				
				}			
			}
		}
	}
	if(Height>-MAX_HEIGHT)
	{
		y=Height;
		return true;
	}
	return false;
}

void CD3DStaticModel::Update(FLOAT Time)
{
	CD3DBaseStaticModel::Update(Time);
	//if(m_pResource)
	//{		
	//	m_WholeBoundingBox=*m_pResource->GetBoundingBox();
	//	m_WholeBoundingSphere=*m_pResource->GetBoundingSphere();
	//	for(int i=0;i<(int)m_ChildList.size();i++)
	//	{
	//		if(m_ChildList[i]->GetWholeBoundingBox())
	//			m_WholeBoundingBox+=*m_ChildList[i]->GetWholeBoundingBox();
	//		if(m_ChildList[i]->GetWholeBoundingSphere())
	//			m_WholeBoundingSphere+=*m_ChildList[i]->GetWholeBoundingSphere();
	//		
	//	}
	//}
}



bool CD3DStaticModel::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	SAFE_RELEASE(m_pBoundingFrame);

	if(!CD3DBaseStaticModel::ToSmartStruct(Packet,pUSOFile,Param))
		return false;	

	int ResourceID=pUSOFile->ResourceObjectToIndex(m_pResource);
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSM_MODEL_RESOURCE,ResourceID));	
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSM_PROPERTY,m_Property));	
	return true;
}
bool CD3DStaticModel::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DBaseStaticModel::FromSmartStruct(Packet,pUSOFile,Param))
		return false;
	int ResourceID=Packet.GetMember(SST_D3DSM_MODEL_RESOURCE);
	m_pResource=(CD3DStaticModelResource *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DStaticModelResource));
	if(m_pResource)
	{
		m_pResource->AddUseRef();
	}	
	m_Property=Packet.GetMember(SST_D3DSM_PROPERTY);
	return true;
}

UINT CD3DStaticModel::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DBaseStaticModel::GetSmartStructSize(Param);
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Property));
	return Size;
}

void CD3DStaticModel::CreateDummy()
{
	if(m_pResource==NULL)
		return;
	for(int i=0;i<m_pResource->GetDummyCount();i++)
	{
		CD3DDummy * pDummy=new CD3DDummy();
		pDummy->SetName(m_pResource->GetDummyName(i));
		pDummy->SetLocalMatrix(*m_pResource->GetDummyMatrix(i));
		pDummy->SetParent(this);
	}
}



//bool CD3DStaticModel::ToUSOFile(CUSOFile * pUSOFile,UINT Param)
//{	
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	SAFE_RELEASE(m_pBoundingFrame);
//
//
//	STORAGE_STRUCT Data;
//
//	strncpy_0(Data.ObjectHead.Type,USO_FILE_MAX_TYPE_LEN,GetClassInfo().ClassName,USO_FILE_MAX_TYPE_LEN);		
//	strncpy_0(Data.ObjectHead.Name,USO_FILE_MAX_OBJECT_NAME,GetName(),USO_FILE_MAX_OBJECT_NAME);
//	Data.ObjectHead.Size=sizeof(STORAGE_STRUCT);
//	Data.ObjectHead.StorageID=GetStorageID();
//
//	Data.LocalMatrix=GetLocalMatrix();
//	Data.IsVisible=IsVisible();
//	Data.ChildCount=(int)m_ChildList.size();
//	Data.ModelResourceID=pUSOFile->ResourceObjectToIndex(m_pResource);	
//
//	pFile->Write(&Data,sizeof(STORAGE_STRUCT));
//
//	for(int i=0;i<(int)m_ChildList.size();i++)
//	{
//		m_ChildList[i]->ToUSOFile(pUSOFile,Param);
//	}
//
//	return true;
//}
//
//bool CD3DStaticModel::FromUSOFile(CUSOFile * pUSOFile,UINT Param)
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
//	
//	
//	Destory();	
//
//	pData->ObjectHead.Name[USO_FILE_MAX_OBJECT_NAME-1]=0;
//	SetName(pData->ObjectHead.Name);
//	SetStorageID(pData->ObjectHead.StorageID);
//	SetLocalMatrix(pData->LocalMatrix);
//	SetVisible(pData->IsVisible);
//	m_pResource=(CD3DStaticModelResource*)pUSOFile->ResourceIndexToObject(pData->ModelResourceID);
//	if(m_pResource)
//	{			
//		m_pResource->AddUseRef();
//	}	
//
//	for(int i=0;i<pData->ChildCount;i++)
//	{
//		USO_OBJECT_HEAD ObjectHead;
//		pUSOFile->GetObjectHead(ObjectHead);
//
//		CD3DBaseStaticModel * pObject=(CD3DBaseStaticModel *)pUSOFile->CreateObject(ObjectHead.Type,ObjectHead.Name);
//		if(pObject)
//		{
//			if(pObject->FromUSOFile(pUSOFile,Param))
//			{
//				if(pObject->IsKindOf(GET_CLASS_INFO(CD3DBaseStaticModel)))
//				{
//					pObject->SetParent(this);
//				}
//				else
//					pObject->Release();
//			}
//			else
//			{
//				SAFE_RELEASE(pObject);
//			}
//		}
//	}
//
//	delete[] pBuff;
//	return true;
//
//}

void CD3DStaticModel::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	if(m_pResource)
	{	
		m_pResource->PickResource(pObjectSet,Param);
		pObjectSet->Add(m_pResource);		
	}
	for(UINT i=0;i<GetChildCount();i++)
	{
		GetChildByIndex(i)->PickResource(pObjectSet,Param);
	}
}

bool CD3DStaticModel::CloneFrom(CNameObject * pObject,UINT Param)
{
	if(!pObject->IsKindOf(GET_CLASS_INFO(CD3DStaticModel)))
		return false;

	Destory();

	if(!CD3DBaseStaticModel::CloneFrom(pObject,Param))
		return false;

	m_pResource=((CD3DStaticModel *)pObject)->GetStaticModelResource();
	m_pResource->AddUseRef();
	return true;
}

//CNameObject::STORAGE_STRUCT * CD3DStaticModel::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//int CD3DStaticModel::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int HeadSize=CD3DBaseStaticModel::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	pLocalHead->ModelResourceID=pUSOFile->ResourceObjectToIndex(m_pResource);	
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//
//int CD3DStaticModel::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int ReadSize=CD3DBaseStaticModel::USOReadHead(pHead,pUSOFile,Param);
//	if(ReadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	m_pResource=(CD3DStaticModelResource*)pUSOFile->ResourceIndexToObject(pLocalHead->ModelResourceID,GET_CLASS_INFO(CD3DStaticModelResource));
//	if(m_pResource)
//	{			
//		m_pResource->AddUseRef();
//	}	
//	return sizeof(STORAGE_STRUCT);
//}


}