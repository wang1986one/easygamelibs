/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DBonedDynamicModel.cpp                                 */
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

IMPLEMENT_CLASS_INFO(CD3DBonedDynamicModel,CD3DBaseDynamicModel);

CD3DBonedDynamicModel::CD3DBonedDynamicModel():CD3DBaseDynamicModel()
{	
	m_pCurAnimation=NULL;	
	m_CurAnimationName=NULL;
	m_BoneCount=0;	
	for(int i=0;i<MAX_BONE_NUM;i++)
	{
		m_BoneMatrixs[i].SetIdentity();
	}
	ZeroMemory(m_pBoneDummys,sizeof(m_pBoneDummys));
	m_IsPlaying=false;
	m_IsLoop=true;
	m_CurAnimationLen=0;
	m_PlayStartTime=0;
	m_CurUpdateTime=0;
	m_CurFrameTime=0;

	m_IsEnableInterim=false;
	m_IsInInterim=false;
	m_InterimTime=0.2f;	
	
}

CD3DBonedDynamicModel::~CD3DBonedDynamicModel(void)
{
	Destory();	
}

void CD3DBonedDynamicModel::Destory()
{
	ClearAllSkin();
	ClearAllAnimation();		
	m_BoneCount=0;
	CD3DBaseDynamicModel::Destory();
}

bool CD3DBonedDynamicModel::Reset()
{	
	bool Ret=true;
	CModelSkinMap::iterator itr;
	for(itr=m_Skins.begin();itr!=m_Skins.end();itr++)
	{
		Ret=Ret&&itr->second->Reset();
	}
	
	return Ret&&CD3DBaseDynamicModel::Reset();
}

bool CD3DBonedDynamicModel::Restore()
{
	bool Ret=true;
	CModelSkinMap::iterator itr;
	for(itr=m_Skins.begin();itr!=m_Skins.end();itr++)
	{
		Ret=Ret&&itr->second->Restore();
	}

	return Ret&&CD3DBaseDynamicModel::Restore();
}

void CD3DBonedDynamicModel::PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera)
{
	if(pSubMesh&&pMaterial)
	{	
		
		if(pMaterial->GetFX())
		{
			//设置灯光
			if(LightList.GetCount())
			{			
				D3DLIGHT9 Light;
				LightList[0]->GetCurLight(Light);
				pMaterial->GetFX()->SetVector("LightDir",CD3DVector4(Light.Direction));
				pMaterial->GetFX()->SetColor("LightAmbient",Light.Ambient);
				pMaterial->GetFX()->SetColor("LightDiffuse",Light.Diffuse);
				pMaterial->GetFX()->SetColor("LightSpecular",Light.Specular);
			}
			//设置材质
			D3DMATERIAL9 * pD3DMaterial;
			if(pSubMesh->IsSelected())
				pD3DMaterial=&SELECTED_SUBMESH_MATERIAL;
			else
				pD3DMaterial=&(pMaterial->GetMaterial());
			pMaterial->GetFX()->SetColor("MaterialAmbient",pD3DMaterial->Ambient);
			pMaterial->GetFX()->SetColor("MaterialDiffuse",pD3DMaterial->Diffuse);
			pMaterial->GetFX()->SetColor("MaterialSpecular",pD3DMaterial->Specular);
			pMaterial->GetFX()->SetColor("MaterialEmissive",pD3DMaterial->Emissive);
			pMaterial->GetFX()->SetFloat("MaterialPower",pD3DMaterial->Power);
			
			//设置纹理
			pMaterial->GetFX()->SetTexture("TexLay0",pMaterial->GetTexture(0));
			pMaterial->GetFX()->SetTexture("TexLay1",pMaterial->GetTexture(1));
			
			//设置视投影矩阵
			pMaterial->GetFX()->SetMatrix("ViewMatrix",pCamera->GetViewMat());
			pMaterial->GetFX()->SetMatrix("ProjMatrix",pCamera->GetProjectMat());

			//设置骨骼矩阵
			pMaterial->GetFX()->SetMatrixArray("BoneMatrixArray",m_BoneMatrixs,m_BoneCount);
		}	
	}
	else
	{
		
	}
}

bool CD3DBonedDynamicModel::LoadM2(LPCTSTR FileName)
{
	return false;
}

bool CD3DBonedDynamicModel::AddSkinFromSKN(LPCTSTR FileName,LPCTSTR SkinName)
{
	if(GetDevice()==NULL)
		return false;

	

	CD3DBonedModelResource* pResource=
		dynamic_cast<CD3DBonedModelResource*>(GetDevice()->GetObjectResourceManager()->GetResource(FileName));
	if(!pResource)
	{
		pResource=new CD3DBonedModelResource(GetDevice()->GetObjectResourceManager());
		if(!pResource->LoadFromSKN(FileName))
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
	if(AddSkin(pResource,SkinName))
	{
		return true;
	}
	else
	{
		pResource->Release();
		return false;
	}
}

bool CD3DBonedDynamicModel::AddSkin(CD3DBonedModelResource * pSkin,LPCTSTR SkinName)
{
	if(pSkin->GetBoneCount()>MAX_BONE_NUM)
		return false;
	if(m_Skins.size()<=0)
	{	
		m_SkinParts.Resize(pSkin->GetSubMeshCount());
		for(int i=0;i<pSkin->GetSubMeshCount();i++)
		{
			strncpy_0(m_SkinParts[i].Name,KEY_NAME_LEN,SkinName,KEY_NAME_LEN);
			m_SkinParts[i].Name[KEY_NAME_LEN-1]=0;
			m_SkinParts[i].pSkin=pSkin;
			m_SkinParts[i].pSubMesh=pSkin->GetSubMesh(i);
		}
		m_BoneCount=pSkin->GetBoneCount();
		CreateBoneDummy(pSkin);
	}
	else
	{
		if(pSkin->GetBoneCount()!=m_BoneCount)
			return false;
		if(pSkin->GetSubMeshCount()!=(int)m_SkinParts.GetCount())
			return false;
	}
	CD3DBonedModelResource * pOldSkin;

	pOldSkin=m_Skins[SkinName];
	if(pOldSkin)
		SAFE_RELEASE(pOldSkin);

	m_Skins[SkinName]=pSkin;
	return true;
}

bool CD3DBonedDynamicModel::DeleteSkin(LPCTSTR SkinName)
{
	if(SkinName==NULL)
		return false;	

	CModelSkinMap::iterator Itr=m_Skins.find(std::string(SkinName));

	if(Itr!=m_Skins.end())
	{			
		SAFE_RELEASE(Itr->second);
		m_Skins.erase(Itr);	
		if(m_Skins.size()>0)
		{		
			CD3DBonedModelResource * pSkin=m_Skins.begin()->second;
			for(int i=0;i<pSkin->GetSubMeshCount();i++)
			{
				strncpy_0(m_SkinParts[i].Name,KEY_NAME_LEN,SkinName,KEY_NAME_LEN);
				m_SkinParts[i].Name[KEY_NAME_LEN-1]=0;
				m_SkinParts[i].pSkin=pSkin;
				m_SkinParts[i].pSubMesh=pSkin->GetSubMesh(i);
			}
		}
		else
		{
			m_SkinParts.Clear();
		}
		return true;
	}
	return false;
}

void  CD3DBonedDynamicModel::ClearAllSkin()
{
	for(CModelSkinMap::iterator Itr=m_Skins.begin();Itr!=m_Skins.end();Itr++)
	{
		SAFE_RELEASE(Itr->second);
	}
	m_Skins.clear();
	m_SkinParts.Clear();
}

bool CD3DBonedDynamicModel::RenameSkin(LPCTSTR SkinName,LPCTSTR NewName)
{
	if(SkinName==NULL||NewName==NULL)
		return false;

	if(strcmp(SkinName,NewName)==0)
		return false;

	CModelSkinMap::iterator Itr=m_Skins.find(std::string(SkinName));

	if(Itr!=m_Skins.end())
	{
		CD3DBonedModelResource * pSkin;

		pSkin=m_Skins[std::string(NewName)];
		if(pSkin)
			SAFE_RELEASE(pSkin);
		pSkin=Itr->second;
		m_Skins.erase(Itr);
		m_Skins[std::string(NewName)]=pSkin;		
		return true;
	}
	return false;
}

int CD3DBonedDynamicModel::GetSkinPartCount()
{
	return (int)m_SkinParts.GetCount();
}

bool CD3DBonedDynamicModel::SetSkinPart(int PartIndex,LPCTSTR SkinName)
{
	if(PartIndex<0)
	{
		CModelSkinMap::iterator Itr=m_Skins.find(std::string(SkinName));

		if(Itr!=m_Skins.end())
		{	
			for(int i=0;i<(int)m_SkinParts.GetCount();i++)
			{
				strncpy_0(m_SkinParts[i].Name,KEY_NAME_LEN,SkinName,KEY_NAME_LEN);
				m_SkinParts[i].Name[KEY_NAME_LEN-1]=0;
				m_SkinParts[i].pSkin=Itr->second;
				m_SkinParts[i].pSubMesh=Itr->second->GetSubMesh(i);
			}
			return true;
		}
	}
	if(PartIndex>=0&&PartIndex<(int)m_SkinParts.GetCount())
	{
		CModelSkinMap::iterator Itr=m_Skins.find(std::string(SkinName));

		if(Itr!=m_Skins.end())
		{		
			strncpy_0(m_SkinParts[PartIndex].Name,KEY_NAME_LEN,SkinName,KEY_NAME_LEN);
			m_SkinParts[PartIndex].Name[KEY_NAME_LEN-1]=0;
			m_SkinParts[PartIndex].pSkin=Itr->second;
			m_SkinParts[PartIndex].pSubMesh=Itr->second->GetSubMesh(PartIndex);
			return true;
		}
	}
	return false;
}
LPCTSTR CD3DBonedDynamicModel::GetSkinPart(int PartIndex)
{
	if(PartIndex>=0&&PartIndex<(int)m_SkinParts.GetCount())
	{
		return m_SkinParts[PartIndex].Name;
	}
	return NULL;
}


bool CD3DBonedDynamicModel::AddAnimationFromACT(LPCTSTR FileName,LPCTSTR AnimationName,bool Reverse)
{
	if(GetDevice()==NULL)
		return false;	

	CEasyString ObjectName;
	ObjectName.Format("%s-%d",FileName,Reverse);

	CD3DBonedAniResource* pResource=
		dynamic_cast<CD3DBonedAniResource*>(GetDevice()->GetObjectResourceManager()->GetResource(ObjectName));
	if(!pResource)
	{
		pResource=new CD3DBonedAniResource(GetDevice()->GetObjectResourceManager());
		if(!pResource->LoadFromACT(FileName,Reverse))
		{
			pResource->Release();
			return false;
		}
		
		if(!GetDevice()->GetObjectResourceManager()->AddResource(pResource,ObjectName))
		{
			pResource->Release();
			return false;
		}
	}
	else
	{
		pResource->AddUseRef();
	}
	if(AddAnimation(pResource,AnimationName))
	{
		return true;
	}
	else
	{
		pResource->Release();
		return false;
	}
}

bool CD3DBonedDynamicModel::AddAnimation(CD3DBonedAniResource* pAni,LPCTSTR AnimationName)
{
	if(pAni->GetBoneCount()!=m_BoneCount)
	{	
		return false;
	}

	std::string MapName=AnimationName;

	CD3DBonedAniResource * pAnimation;

	pAnimation=m_Animations[MapName];
	if(pAnimation)
		SAFE_RELEASE(pAnimation);
	m_Animations[MapName]=pAni;
	return true;
}

bool CD3DBonedDynamicModel::DeleteAnimation(LPCTSTR AnimationName)
{
	if(AnimationName==NULL)
		return false;	

	CModelAnimationMap::iterator Itr=m_Animations.find(std::string(AnimationName));

	if(Itr!=m_Animations.end())
	{		
		if(m_pCurAnimation==Itr->second)
		{
			m_pCurAnimation=NULL;
			m_IsPlaying=false;
		}
		SAFE_RELEASE(Itr->second);
		m_Animations.erase(Itr);	
		return true;
	}
	return false;
}

void  CD3DBonedDynamicModel::ClearAllAnimation()
{
	for(CModelAnimationMap::iterator Itr=m_Animations.begin();Itr!=m_Animations.end();Itr++)
	{
		SAFE_RELEASE(Itr->second);
	}
	m_Animations.clear();
	m_pCurAnimation=NULL;
	m_IsPlaying=false;
}

bool CD3DBonedDynamicModel::RenameAnimation(LPCTSTR AnimationName,LPCTSTR NewName)
{
	if(AnimationName==NULL||NewName==NULL)
		return false;

	if(strcmp(AnimationName,NewName)==0)
		return false;

	CModelAnimationMap::iterator Itr=m_Animations.find(std::string(AnimationName));
	
	if(Itr!=m_Animations.end())
	{
		CD3DBonedAniResource * pAnimation;

		pAnimation=m_Animations[std::string(NewName)];
		if(pAnimation)
			SAFE_RELEASE(pAnimation);
		pAnimation=Itr->second;
		m_Animations.erase(Itr);
		m_Animations[std::string(NewName)]=pAnimation;		
		return true;
	}
	return false;
}

bool CD3DBonedDynamicModel::Play(bool IsLoop)
{
	if(m_Animations.size()<=0)
		return false;

	if(m_CurAnimationName)
	{		
		Play(m_CurAnimationName,IsLoop);
	}
	else
	{
		Play(m_Animations.begin()->first.c_str(),IsLoop);
	}
	return true;
}

bool CD3DBonedDynamicModel::Play(LPCTSTR AnimationName,bool IsLoop)
{
	CD3DBonedAniResource * pNewAnimation;

	CModelAnimationMap::iterator Itr=m_Animations.find(std::string(AnimationName));

	if(Itr==m_Animations.end())
		return false;

	pNewAnimation=Itr->second;
	m_CurAnimationName=Itr->first.c_str();

	if(pNewAnimation==NULL)
		return false;

	m_PlayStartTime=m_CurUpdateTime;
	m_IsPlaying=true;
	m_IsLoop=IsLoop;

	//处理动作过渡
	if(m_IsEnableInterim&&m_pCurAnimation)
	{
		m_IsInInterim=true;		
		m_pCurAnimation->GetBoneFrame(m_CurFrameTime,NULL,m_InterimStartBone);
		pNewAnimation->GetBoneFrame(0,NULL,m_InterimEndBone);
	}
	//SAFE_RELEASE(m_pCurAnimation);
	m_pCurAnimation=pNewAnimation;
	m_CurAnimationLen=m_pCurAnimation->GetAnimationLen();
	m_pCurAnimation->GetBoneFrame(m_CurFrameTime,m_BoneMatrixs,NULL);
	for(int i=0;i<m_BoneCount;i++)
	{			
		m_pBoneDummys[i]->SetLocalMatrix(m_BoneMatrixs[i]);
	}

	
	return true;
}

bool CD3DBonedDynamicModel::Stop()
{
	if(m_pCurAnimation==NULL)
		return false;
	m_IsPlaying=false;
	return true;
}


int CD3DBonedDynamicModel::GetSubMeshCount()
{
	return (int)m_SkinParts.GetCount();
}
CD3DSubMesh * CD3DBonedDynamicModel::GetSubMesh(UINT index)
{	
	if(index>=0&&index<(int)m_SkinParts.GetCount())
		return m_SkinParts[index].pSubMesh;
	else
		return NULL;
}

CD3DBoundingBox * CD3DBonedDynamicModel::GetBoundingBox()
{
	if(m_Skins.size()>0)
		return m_Skins.begin()->second->GetBoundingBox();
	else
		return NULL;
}
CD3DBoundingSphere * CD3DBonedDynamicModel::GetBoundingSphere()
{
	if(m_Skins.size()>0)
		return m_Skins.begin()->second->GetBoundingSphere();
	else
		return NULL;	
}

void CD3DBonedDynamicModel::Update(FLOAT Time)
{
	FUNCTION_BEGIN;
	m_CurUpdateTime=Time;	
	if(m_pCurAnimation&&m_IsPlaying)
	{
		m_CurFrameTime=m_CurUpdateTime-m_PlayStartTime;
		if(!m_IsLoop)
		{
			if(m_CurFrameTime>=m_CurAnimationLen)
			{			
				m_CurFrameTime=m_CurAnimationLen;
				m_IsPlaying=false;
			}
		}
		//计算骨骼矩阵
		if(m_IsInInterim)
		{
			//动作过渡
			if(m_CurFrameTime<m_InterimTime)
			{
				FLOAT S=m_CurFrameTime/m_InterimTime;
				for(int i=0;i<m_BoneCount;i++)
				{
					BONE_TRANS CurBone;

					CurBone.Pos=CD3DVector3::Lerp(m_InterimStartBone[i].Pos,m_InterimEndBone[i].Pos,S);
					CurBone.Rotation=CD3DQuaternion::SLerp(m_InterimStartBone[i].Rotation,m_InterimEndBone[i].Rotation,S);
					CurBone.Scale=CD3DVector3::Lerp(m_InterimStartBone[i].Scale,m_InterimEndBone[i].Scale,S);

					m_BoneMatrixs[i]=CD3DMatrix::FromScale(CurBone.Scale)*
						CD3DMatrix::FromRotationQuaternion(CurBone.Rotation);
					m_BoneMatrixs[i].SetTranslation(CurBone.Pos);
				}
			}
			else
			{
				m_IsInInterim=false;
				m_CurFrameTime-=m_InterimTime;
				m_PlayStartTime=m_CurUpdateTime-m_CurFrameTime;				
				m_pCurAnimation->GetBoneFrame(m_CurFrameTime,m_BoneMatrixs,NULL);
			}
		}
		else
		{
			m_pCurAnimation->GetBoneFrame(m_CurFrameTime,m_BoneMatrixs,NULL);
		}

		
		for(int i=0;i<m_BoneCount;i++)
		{			
			m_pBoneDummys[i]->SetLocalMatrix(m_BoneMatrixs[i]);
		}
	}
	FUNCTION_END;
	CD3DBaseDynamicModel::Update(Time);	


	FUNCTION_BEGIN;

	if(m_pCurAnimation)
	{
		//计算骨骼矩阵		
		for(int i=0;i<m_BoneCount;i++)
		{
			m_BoneMatrixs[i]=m_pBoneDummys[i]->GetWorldMatrix();	
			if(m_Skins.begin()!=m_Skins.end())
				m_BoneMatrixs[i]= m_Skins.begin()->second->GetBone(i)->BoneInitMatrix*m_BoneMatrixs[i];
		}
			
	}
	else
	{
		//计算骨骼矩阵		
		for(int i=0;i<m_BoneCount;i++)
		{
			m_BoneMatrixs[i]=m_pBoneDummys[i]->GetWorldMatrix();			
		}	
	}
	FUNCTION_END;
	
}

void CD3DBonedDynamicModel::CreateBoneDummy(CD3DBonedModelResource* pSkin)
{
	if(m_BoneCount>0&&pSkin)
	{	
		for(int i=0;i<m_BoneCount;i++)
		{		
			m_pBoneDummys[i]=new CD3DDummy();			
		}
		for(int i=0;i<m_BoneCount;i++)
		{				
			BONED_MODEL_BONE * pBone=pSkin->GetBone(i);
			m_pBoneDummys[i]->SetID(i);
			m_pBoneDummys[i]->SetName(pBone->Name);
			if(pBone->ParentID>=0)
				m_pBoneDummys[i]->SetParent(m_pBoneDummys[pBone->ParentID]);
			else
				m_pBoneDummys[i]->SetParent(this);
		}
	}
}

bool CD3DBonedDynamicModel::IsBoneDummy(CD3DObject* pDummy)
{
	for(int i=0;i<m_BoneCount;i++)
	{
		if(m_pBoneDummys[i]==pDummy)
			return true;
	}
	return false;
}



//bool CD3DBonedDynamicModel::ToUSOFile(CUSOFile * pUSOFile,UINT Param)
//{	
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	ShowBoundingFrame(DBFO_RELEASE);
//
//
//	STORAGE_STRUCT Data;
//
//	strncpy_0(Data.ObjectHead.Type,USO_FILE_MAX_TYPE_LEN,GetClassInfo().ClassName,USO_FILE_MAX_TYPE_LEN);		
//	strncpy_0(Data.ObjectHead.Name,USO_FILE_MAX_OBJECT_NAME,GetName(),USO_FILE_MAX_OBJECT_NAME);
//	Data.ObjectHead.StorageID=GetStorageID();
//
//	Data.LocalMatrix=GetLocalMatrix();
//	Data.IsVisible=IsVisible();
//	Data.ChildCount=0;
//	for(int i=0;i<(int)m_ChildList.size();i++)
//	{
//		//if(!IsBoneDummy(m_ChildList[i]))
//			Data.ChildCount++;
//	}
//	Data.MeshPartCount=GetSubMeshCount();
//	Data.SkinResourceCount=GetSkinCount();	
//	Data.AnimationResourceCount=GetAnimationCount();
//	Data.ObjectHead.Size=sizeof(STORAGE_STRUCT)+
//		GetSkinCount()*sizeof(STORAGE_RESOURCE_STRUCT)+
//		GetAnimationCount()*sizeof(STORAGE_RESOURCE_STRUCT)+
//		GetSkinPartCount()*KEY_NAME_LEN;
//
//	pFile->Write(&Data,sizeof(STORAGE_STRUCT));
//
//	for(int i=0;i<GetSkinCount();i++)
//	{
//		STORAGE_RESOURCE_STRUCT Skin;
//
//		ZeroMemory(&Skin,sizeof(Skin));
//		Skin.ResourceID=pUSOFile->ResourceObjectToIndex(GetSkin(i));
//		const char * pName=GetSkinName(i);
//		if(pName)
//			strncpy_0(Skin.Name,KEY_NAME_LEN,pName,KEY_NAME_LEN);
//		Skin.Name[KEY_NAME_LEN-1]=0;
//		pFile->Write(&Skin,sizeof(Skin));
//	}
//
//	for(int i=0;i<GetAnimationCount();i++)
//	{
//		STORAGE_RESOURCE_STRUCT Ani;
//
//		ZeroMemory(&Ani,sizeof(Ani));
//		Ani.ResourceID=pUSOFile->ResourceObjectToIndex(GetAnimation(i));
//		const char * pName=GetAnimationName(i);
//		if(pName)
//			strncpy_0(Ani.Name,KEY_NAME_LEN,pName,KEY_NAME_LEN);
//		Ani.Name[KEY_NAME_LEN-1]=0;
//		pFile->Write(&Ani,sizeof(Ani));
//	}
//
//	for(int i=0;i<GetSkinPartCount();i++)
//	{
//		pFile->Write((LPVOID)GetSkinPart(i),KEY_NAME_LEN);
//	}
//
//	for(int i=0;i<(int)m_ChildList.size();i++)
//	{
//		//if(!IsBoneDummy(m_ChildList[i]))
//			m_ChildList[i]->ToUSOFile(pUSOFile,Param);
//	}
//
//	return true;
//}
//
//bool CD3DBonedDynamicModel::FromUSOFile(CUSOFile * pUSOFile,UINT Param)
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
//	if(!GetClassInfo().IsKindOf(pData->ObjectHead.Type)||
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
//
//	STORAGE_RESOURCE_STRUCT* pSkins=(STORAGE_RESOURCE_STRUCT *)
//		(pBuff+sizeof(STORAGE_STRUCT));
//	for(int i=0;i<pData->SkinResourceCount;i++)
//	{			
//		CD3DBonedModelResource * pSkin;
//
//		pSkin=(CD3DBonedModelResource *)pUSOFile->ResourceIndexToObject(pSkins[i].ResourceID);
//		if(pSkin)
//		{
//			if(pSkin->IsKindOf(GET_CLASS_INFO(CD3DBonedModelResource)))
//			{
//				pSkin->AddUseRef();
//				AddSkin(pSkin,pSkins[i].Name);
//			}
//		}
//	}
//	
//	STORAGE_RESOURCE_STRUCT * pAnis=(STORAGE_RESOURCE_STRUCT *)
//		(pBuff+sizeof(STORAGE_STRUCT)+
//		pData->SkinResourceCount*sizeof(STORAGE_RESOURCE_STRUCT));
//	for(int i=0;i<pData->AnimationResourceCount;i++)
//	{
//		CD3DBonedAniResource * pAnimation;
//
//		pAnimation=(CD3DBonedAniResource *)pUSOFile->ResourceIndexToObject(pAnis[i].ResourceID);
//		if(pAnimation)
//		{
//			if(pAnimation->IsKindOf(GET_CLASS_INFO(CD3DBonedAniResource)))
//			{
//				pAnimation->AddUseRef();
//				AddAnimation(pAnimation,pAnis[i].Name);
//			}
//		}
//	}
//	
//
//	KEY_NAME * pPartNames=(KEY_NAME *)
//		(pBuff+sizeof(STORAGE_STRUCT)+
//		pData->SkinResourceCount*sizeof(STORAGE_RESOURCE_STRUCT)+
//		pData->AnimationResourceCount*sizeof(STORAGE_RESOURCE_STRUCT));
//	for(int i=0;i<pData->MeshPartCount;i++)
//	{
//		SetSkinPart(i,pPartNames[i]);
//	}
//
//	std::vector<CD3DObject *> Dummys;
//
//	for(int i=0;i<pData->ChildCount;i++)
//	{
//		USO_OBJECT_HEAD ObjectHead;
//		pUSOFile->GetObjectHead(ObjectHead);
//
//		CD3DObject * pObject=(CD3DObject *)pUSOFile->CreateObject(ObjectHead.Type,ObjectHead.Name);
//
//		if(pObject)
//		{
//			if(pObject->FromUSOFile(pUSOFile,Param))
//			{
//				if(pObject->IsKindOf(GET_CLASS_INFO(CD3DDummy)))
//				{
//					Dummys.push_back(pObject);
//				}
//				else if(pObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
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
//	for(int i=0;i<(int)Dummys.size();i++)
//	{
//		PickNoneDummyObject(Dummys[i]);
//		SAFE_RELEASE(Dummys[i]);
//	}
//
//	delete[] pBuff;
//	return true;
//
//}

bool CD3DBonedDynamicModel::CloneFrom(CNameObject * pObject,UINT Param)
{
	if(!pObject->IsKindOf(GET_CLASS_INFO(CD3DBonedDynamicModel)))
		return false;

	

	Destory();	
	
	if(!CD3DObject::CloneFrom(pObject,Param|OBJECT_CLONE_WITHOUT_CHILD))
		return false;

	CD3DBonedDynamicModel * pSource=(CD3DBonedDynamicModel *)pObject;

	m_pD3DDevice=pSource->GetDevice();
	m_pRender=pSource->GetRender();	
	m_Layer=pSource->GetLayer();
	//SetParent(pSource->GetParent());
	m_LocalMatrix=pSource->GetLocalMatrix();
	m_WorldMatrix=pSource->GetWorldMatrix();		
	m_Flag=pSource->m_Flag;	
	//ClearAllChild();
	if((Param&OBJECT_CLONE_WITHOUT_CHILD)==0)
	{	
		for(int i=0;i<(int)pSource->GetChildCount();i++)
		{
			if(!pSource->IsBoneDummy(pSource->GetChildByIndex(i)))
			{		
				CD3DObject * pNewChild=dynamic_cast<CD3DObject *>(pSource->GetChildByIndex(i)->GetClassInfo().CreateObject());
				if(pNewChild)
				{
					if(pNewChild->CloneFrom(pSource->GetChildByIndex(i)))
					{
						pNewChild->SetParent(this);						
					}
					else
					{
						pNewChild->Release();
					}
				}
			}
		}
	}
	

	//CModelAnimationMap			m_Animations;
	//CModelSkinMap				m_Skins;
	//std::vector<SKIN_PART_INFO>	m_SkinParts;


	m_pCurAnimation=pSource->m_pCurAnimation;
	m_CurAnimationName=pSource->m_CurAnimationName;
	m_BoneCount=pSource->m_BoneCount;
	
	
	
	m_IsPlaying=pSource->m_IsPlaying;
	m_IsLoop=pSource->m_IsLoop;
	m_CurAnimationLen=pSource->m_CurAnimationLen;
	m_PlayStartTime=pSource->m_PlayStartTime;
	m_CurUpdateTime=pSource->m_CurUpdateTime;
	m_CurFrameTime=pSource->m_CurFrameTime;

	m_IsEnableInterim=pSource->m_IsEnableInterim;
	m_IsInInterim=pSource->m_IsInInterim;
	m_InterimTime=pSource->m_InterimTime;

	for(int i=0;i<m_BoneCount;i++)
	{
		m_BoneMatrixs[i]=pSource->m_BoneMatrixs[i];
		m_InterimStartBone[i]=pSource->m_InterimStartBone[i];
		m_InterimEndBone[i]=pSource->m_InterimEndBone[i];		
	}

	for(int i=0;i<pSource->GetAnimationCount();i++)
	{
		pSource->GetAnimation(i)->AddUseRef();
		AddAnimation(pSource->GetAnimation(i),pSource->GetAnimationName(i));
	}

	for(int i=0;i<pSource->GetSkinCount();i++)
	{
		pSource->GetSkin(i)->AddUseRef();
		AddSkin(pSource->GetSkin(i),pSource->GetSkinName(i));
	}

	for(int i=0;i<pSource->GetSkinPartCount();i++)
		SetSkinPart(i,pSource->GetSkinPart(i));



	return true;
}

void CD3DBonedDynamicModel::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	for(int i=0;i<GetAnimationCount();i++)
	{
		GetAnimation(i)->PickResource(pObjectSet,Param);
		pObjectSet->Add(GetAnimation(i));				
	}

	for(int i=0;i<GetSkinCount();i++)
	{
		GetSkin(i)->PickResource(pObjectSet,Param);
		pObjectSet->Add(GetSkin(i));		
	}

	for(UINT i=0;i<GetChildCount();i++)
	{
		GetChildByIndex(i)->PickResource(pObjectSet,Param);
	}
}

bool CD3DBonedDynamicModel::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	SAFE_RELEASE(m_pBoundingFrame);

	if(!CD3DObject::ToSmartStruct(Packet,pUSOFile,Param))
		return false;	


	for(CModelSkinMap::iterator itr=m_Skins.begin();itr!=m_Skins.end();itr++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_SI_NAME,itr->first.c_str()));	
		int ResourceID=pUSOFile->ResourceObjectToIndex(itr->second);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_SI_SKIN_RESOURCE_ID,ResourceID));	
		if(!Packet.FinishMember(SST_D3DBDM_SKIN_INFO,SubPacket.GetDataLen()))
			return false;
	}
	for(UINT i=0;i<m_SkinParts.GetCount();i++)
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DBDM_SKIN_PART_INFO,m_SkinParts[i].Name));	
	}

	for(CModelAnimationMap::iterator itr=m_Animations.begin();itr!=m_Animations.end();itr++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_AI_NAME,itr->first.c_str()));	
		int ResourceID=pUSOFile->ResourceObjectToIndex(itr->second);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_AI_ANI_RESOURCE_ID,ResourceID));	
		if(!Packet.FinishMember(SST_D3DBDM_ANI_INFO,SubPacket.GetDataLen()))
			return false;
	}


	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DBDM_ENABLE_INTERIM,(BYTE)m_IsEnableInterim));	
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DBDM_INTERIM_TIME,m_InterimTime));	

	if(m_CurAnimationName)
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DBDM_CUR_ANI,m_CurAnimationName));	
	}
	
	
	return true;
}
bool CD3DBonedDynamicModel::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DObject::FromSmartStruct(Packet,pUSOFile,Param|OPP_WITHOUT_CHILD))
		return false;

	void * Pos=Packet.GetFirstMemberPosition();
	int PartIndex=0;
	CEasyArray<CD3DObject *> Dummys;
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DBDM_SKIN_INFO:
			{
				CSmartStruct SubPacket=Value;
				LPCTSTR szSkinName=SubPacket.GetMember(SST_SI_NAME);
				int ResourceID=SubPacket.GetMember(SST_SI_SKIN_RESOURCE_ID);
				CD3DBonedModelResource * pSkin=(CD3DBonedModelResource *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DBonedModelResource));
				if(pSkin)
				{
					pSkin->AddUseRef();
					AddSkin(pSkin,szSkinName);
				}
			}
			break;
		case SST_D3DBDM_SKIN_PART_INFO:
			{
				LPCTSTR szPartName=Value;
				SetSkinPart(PartIndex,szPartName);
				PartIndex++;
			}
			break;
		case SST_D3DBDM_ANI_INFO:
			{
				CSmartStruct SubPacket=Value;
				LPCTSTR szAniName=SubPacket.GetMember(SST_AI_NAME);
				int ResourceID=SubPacket.GetMember(SST_AI_ANI_RESOURCE_ID);
				CD3DBonedAniResource * pAni=(CD3DBonedAniResource *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DBonedAniResource));
				if(pAni)
				{
					pAni->AddUseRef();
					AddAnimation(pAni,szAniName);
				}
			}
			break;		
		case SST_D3DBDM_ENABLE_INTERIM:
			EnableInterim((BYTE)Value!=0);
			break;
		case SST_D3DBDM_INTERIM_TIME:
			SetInterimTime(Value);
			break;
		case SST_D3DBDM_CUR_ANI:
			{
				LPCTSTR szCurAniName=Value;
				Play(szCurAniName,true);
			}		
			break;		
		case SST_TO_CHILD:
			if((Param&OPP_WITHOUT_CHILD)==0)
			{
				CSmartStruct ChildPacket=Value;
				LPCTSTR ClassName=ChildPacket.GetMember(SST_NO_CLASS_NAME);
				LPCTSTR ObjectName=ChildPacket.GetMember(SST_NO_OBJECT_NAME);
				CNameObject * pObject=pUSOFile->CreateObject(ClassName,ObjectName);				
				if(pObject==NULL)
					return false;
				if(!pObject->IsKindOf(GET_CLASS_INFO(CTreeObject)))
				{
					SAFE_RELEASE(pObject);
					return false;
				}
				if(!pObject->FromSmartStruct(ChildPacket,pUSOFile,Param))
					return false;
				if(pObject->IsKindOf(GET_CLASS_INFO(CD3DDummy)))
				{
					Dummys.Add((CD3DDummy *)pObject);
				}
				else if(pObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
				{
					((CD3DObject *)pObject)->SetParent(this);
				}
				else
				{
					SAFE_RELEASE(pObject);
				}
				
			}
			break;
		}
	}	

	for(int i=0;i<(int)Dummys.GetCount();i++)
	{
		PickNoneDummyObject(Dummys[i]);
		SAFE_RELEASE(Dummys[i]);
	}
	
	return true;
}

UINT CD3DBonedDynamicModel::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DObject::GetSmartStructSize(Param);
	for(CModelAnimationMap::iterator itr=m_Animations.begin();itr!=m_Animations.end();itr++)
	{
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(itr->first.size());
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}

	for(CModelSkinMap::iterator itr=m_Skins.begin();itr!=m_Skins.end();itr++)
	{
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(itr->first.size());
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));	
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	for(UINT i=0;i<m_SkinParts.GetCount();i++)
	{
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(strlen(m_SkinParts[i].Name));
	}
	
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_IsEnableInterim));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_InterimTime));

	if(m_CurAnimationName)
	{
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(strlen(m_CurAnimationName));
	}
	
	return Size;
}

bool CD3DBonedDynamicModel::RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly)
{	

	if(GetBoundingBox()==NULL)
		return false;

	CD3DBoundingBox BBox;

	BBox=(*GetBoundingBox())*GetWorldMatrix();

	if(BBox.RayIntersect(Point,Dir,IntersectPoint,Distance))
		return true;
	
	return false;
}

bool CD3DBonedDynamicModel::GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y)
{
	if(GetBoundingBox()==NULL)
		return false;



	CD3DBoundingBox Box=(*GetBoundingBox())*GetWorldMatrix();

	if(x<Box.m_Min.x||x>Box.m_Max.x||z<Box.m_Min.z||z>Box.m_Max.z)
		return false;

	y=Box.m_Max.y;

	return true;
}

void CD3DBonedDynamicModel::PickNoneDummyObject(CD3DObject* pDummy)
{
	for(UINT i=0;i<pDummy->GetChildCount();i++)
	{
		CD3DObject* pChild=pDummy->GetChildByIndex(i);
		if(pChild)
		{
			if(!pChild->IsKindOf(GET_CLASS_INFO(CD3DDummy)))
			{
				CD3DObject* pParent=GetChildRecursive(pDummy->GetName());
				if(pParent)
				{
					pChild->SetParent(pParent);
				}
				else
				{
					SAFE_RELEASE(pChild);
				}
			}
			else
			{
				PickNoneDummyObject(pChild);
			}
		}
	}
}

//CNameObject::STORAGE_STRUCT * CD3DBonedDynamicModel::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//int CD3DBonedDynamicModel::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int HeadSize=CD3DObject::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	pLocalHead->MeshPartCount=GetSubMeshCount();
//	pLocalHead->SkinResourceCount=GetSkinCount();	
//	pLocalHead->AnimationResourceCount=GetAnimationCount();
//	pLocalHead->Size+=GetSkinCount()*sizeof(STORAGE_RESOURCE_STRUCT)+
//		GetAnimationCount()*sizeof(STORAGE_RESOURCE_STRUCT)+
//		GetSkinPartCount()*KEY_NAME_LEN;
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//bool CD3DBonedDynamicModel::USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	if(!CD3DObject::USOWriteData(pHead,pUSOFile,Param))
//		return false;
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	for(int i=0;i<GetSkinCount();i++)
//	{
//		STORAGE_RESOURCE_STRUCT Skin;
//
//		ZeroMemory(&Skin,sizeof(Skin));
//		Skin.ResourceID=pUSOFile->ResourceObjectToIndex(GetSkin(i));
//		const char * pName=GetSkinName(i);
//		if(pName)
//			strncpy_0(Skin.Name,KEY_NAME_LEN,pName,KEY_NAME_LEN);
//		Skin.Name[KEY_NAME_LEN-1]=0;
//		if(pFile->Write(&Skin,sizeof(Skin))<sizeof(Skin))
//			return false;
//	}
//
//	for(int i=0;i<GetAnimationCount();i++)
//	{
//		STORAGE_RESOURCE_STRUCT Ani;
//
//		ZeroMemory(&Ani,sizeof(Ani));
//		Ani.ResourceID=pUSOFile->ResourceObjectToIndex(GetAnimation(i));
//		const char * pName=GetAnimationName(i);
//		if(pName)
//			strncpy_0(Ani.Name,KEY_NAME_LEN,pName,KEY_NAME_LEN);
//		Ani.Name[KEY_NAME_LEN-1]=0;
//		if(pFile->Write(&Ani,sizeof(Ani))<sizeof(Ani))
//			return false;
//	}
//
//	for(int i=0;i<GetSkinPartCount();i++)
//	{
//		if(pFile->Write((LPVOID)GetSkinPart(i),KEY_NAME_LEN)<KEY_NAME_LEN)
//			return false;
//	}
//
//	return true;
//}
//
//int CD3DBonedDynamicModel::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{	
//	int ReadSize=CD3DObject::USOReadHead(pHead,pUSOFile,Param);
//	if(ReadSize<0)
//		return -1;	
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//int CD3DBonedDynamicModel::USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param)
//{
//	int ReadSize=CD3DObject::USOReadData(pHead,pUSOFile,pData,DataSize,Param);
//
//	pData+=ReadSize;
//	DataSize-=ReadSize;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//	
//	STORAGE_RESOURCE_STRUCT* pSkins=(STORAGE_RESOURCE_STRUCT *)pData;
//	pData+=pLocalHead->SkinResourceCount*sizeof(STORAGE_RESOURCE_STRUCT);
//	ReadSize+=pLocalHead->SkinResourceCount*sizeof(STORAGE_RESOURCE_STRUCT);
//	for(int i=0;i<pLocalHead->SkinResourceCount;i++)
//	{			
//		CD3DBonedModelResource * pSkin;
//
//		pSkin=(CD3DBonedModelResource *)pUSOFile->ResourceIndexToObject(pSkins[i].ResourceID,GET_CLASS_INFO(CD3DBonedModelResource));
//		if(pSkin)
//		{
//			if(pSkin->IsKindOf(GET_CLASS_INFO(CD3DBonedModelResource)))
//			{
//				pSkin->AddUseRef();
//				AddSkin(pSkin,pSkins[i].Name);
//			}
//		}
//	}
//
//	STORAGE_RESOURCE_STRUCT * pAnis=(STORAGE_RESOURCE_STRUCT *)pData;
//	pData+=pLocalHead->AnimationResourceCount*sizeof(STORAGE_RESOURCE_STRUCT);
//	ReadSize+=pLocalHead->AnimationResourceCount*sizeof(STORAGE_RESOURCE_STRUCT);
//	for(int i=0;i<pLocalHead->AnimationResourceCount;i++)
//	{
//		CD3DBonedAniResource * pAnimation;
//
//		pAnimation=(CD3DBonedAniResource *)pUSOFile->ResourceIndexToObject(pAnis[i].ResourceID,GET_CLASS_INFO(CD3DBonedAniResource));
//		if(pAnimation)
//		{
//			if(pAnimation->IsKindOf(GET_CLASS_INFO(CD3DBonedAniResource)))
//			{
//				pAnimation->AddUseRef();
//				AddAnimation(pAnimation,pAnis[i].Name);
//			}
//		}
//	}
//
//	KEY_NAME * pPartNames=(KEY_NAME *)pData;
//	pData+=pLocalHead->MeshPartCount*sizeof(KEY_NAME);
//	ReadSize+=pLocalHead->MeshPartCount*sizeof(KEY_NAME);		
//	for(int i=0;i<pLocalHead->MeshPartCount;i++)
//	{
//		SetSkinPart(i,pPartNames[i]);
//	}	
//	
//	return ReadSize;
//}
//
//bool CD3DBonedDynamicModel::USOReadChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	if(!CNameObject::USOReadChild(pHead,pUSOFile,Param))
//		return false;
//	if(pHead==NULL||pUSOFile==NULL)
//		return false;
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	CEasyArray<CD3DObject *> Dummys;
//
//	//PrintImportantLog(0,"CD3DBonedDynamicModel需要读取%d个子对象",pLocalHead->ChildCount);
//
//	for(int i=0;i<pLocalHead->ChildCount;i++)
//	{
//		CNameObject::STORAGE_STRUCT ObjectHead;
//		pUSOFile->GetObjectHead(ObjectHead);
//
//		CD3DObject * pObject=(CD3DObject *)pUSOFile->CreateObject(ObjectHead.Type,ObjectHead.Name);
//
//		if(pObject)
//		{
//			if(pObject->FromUSOFile(pUSOFile,Param))
//			{
//				if(pObject->IsKindOf(GET_CLASS_INFO(CD3DDummy)))
//				{
//					Dummys.Add(pObject);
//				}
//				else if(pObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
//				{
//					pObject->SetParent(this);
//				}
//				else
//				{
//					SAFE_RELEASE(pObject);
//				}
//			}
//			else
//			{
//				SAFE_RELEASE(pObject);
//			}
//		}
//	}
//
//	for(int i=0;i<(int)Dummys.GetCount();i++)
//	{
//		PickNoneDummyObject(Dummys[i]);
//		SAFE_RELEASE(Dummys[i]);
//	}
//	return true;
//}



}