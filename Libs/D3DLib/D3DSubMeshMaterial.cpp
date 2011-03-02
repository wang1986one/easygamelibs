/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DSubMeshMaterial.cpp                                   */
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

IMPLEMENT_CLASS_INFO(CD3DSubMeshMaterial,CNameObject);

CD3DSubMeshMaterial::CD3DSubMeshMaterial(void)
{
	ZeroMemory(&m_Material,sizeof(m_Material));	
	m_GlobalColor.a=1.0f;
	m_GlobalColor.r=1.0f;
	m_GlobalColor.g=1.0f;
	m_GlobalColor.b=1.0f;	
	m_pFX=NULL;
}

CD3DSubMeshMaterial::~CD3DSubMeshMaterial(void)
{
	Destory();
}

void CD3DSubMeshMaterial::Destory()
{
	ZeroMemory(&m_Material,sizeof(m_Material));
	m_GlobalColor.a=1.0f;
	m_GlobalColor.r=1.0f;
	m_GlobalColor.g=1.0f;
	m_GlobalColor.b=1.0f;
	for(size_t i=0;i<m_TextureList.GetCount();i++)
	{
		SAFE_RELEASE(m_TextureList[i].pTexture);
	}
	m_TextureList.Clear();
	SAFE_RELEASE(m_pFX);
	CNameObject::Destory();
}

bool CD3DSubMeshMaterial::Reset()
{
	bool Ret=true;
	if(m_pFX)
		Ret=m_pFX->Reset();
	for(size_t i=0;i<m_TextureList.GetCount();i++)
	{
		Ret=Ret&&m_TextureList[i].pTexture->Reset();		
	}
	return Ret;			
}

bool CD3DSubMeshMaterial::Restore()
{
	bool Ret=true;
	if(m_pFX)
		Ret=m_pFX->Restore();
	for(size_t i=0;i<m_TextureList.GetCount();i++)
	{
		Ret=Ret&&m_TextureList[i].pTexture->Restore();		
	}
	return Ret;		
}

void CD3DSubMeshMaterial::SetFX(CD3DFX * pFX)
{
	SAFE_RELEASE(m_pFX);
	m_pFX=pFX;
}

bool CD3DSubMeshMaterial::SetTexture(UINT Layer,CD3DTexture * pTexture)
{
	if(Layer<m_TextureList.GetCount())
	{
		SAFE_RELEASE(m_TextureList[Layer].pTexture);
		m_TextureList[Layer].pTexture=pTexture;
		return true;
	}
	return false;
}

void CD3DSubMeshMaterial::ClearAllTexture()
{
	for(size_t i=0;i<m_TextureList.GetCount();i++)
	{
		SAFE_RELEASE(m_TextureList[i].pTexture);
	}
	m_TextureList.Clear();
}

bool CD3DSubMeshMaterial::CloneFrom(CNameObject * pObject,UINT Param)
{
	if(!pObject->IsKindOf(GET_CLASS_INFO(CD3DSubMeshMaterial)))
		return false;
	CD3DSubMeshMaterial * pSrcObject=(CD3DSubMeshMaterial *)pObject;
	m_Material=pSrcObject->m_Material;
	m_GlobalColor=pSrcObject->m_GlobalColor;
	m_TextureList=pSrcObject->m_TextureList;
	m_pFX=pSrcObject->m_pFX;

	for(UINT i=0;i<m_TextureList.GetCount();i++)
	{
		if(m_TextureList[i].pTexture)
			m_TextureList[i].pTexture->AddUseRef();
	}

	if(m_pFX)
		m_pFX->AddUseRef();

	return true;
	
}

void CD3DSubMeshMaterial::PickResource(CUSOResourceManager * pResourceManager,UINT Param)
{
	if(m_pFX)
		pResourceManager->AddResource(m_pFX);
	for(size_t i=0;i<m_TextureList.GetCount();i++)
	{
		if(m_TextureList[i].pTexture)
		{
			m_TextureList[i].pTexture->PickResource(pResourceManager,Param);
			pResourceManager->AddResource(m_TextureList[i].pTexture);
		}
	}
}


bool CD3DSubMeshMaterial::ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
{
	if(!CNameObject::ToSmartStruct(Packet,pResourceManager,Param))
		return false;	

	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSMM_MATERIAL,(char *)&m_Material,sizeof(m_Material)));
	for(UINT i=0;i<m_TextureList.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		if(m_TextureList[i].pTexture)
			CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_TEX_TEXTURE,m_TextureList[i].pTexture->GetName()));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_TEX_PROPERTY,m_TextureList[i].Property));
		if(!Packet.FinishMember(SST_D3DSMM_TEXTURE,SubPacket.GetDataLen()))
			return false;
	}
	if(m_pFX)
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSMM_FX,m_pFX->GetName()));		
	}
	return true;
}
bool CD3DSubMeshMaterial::FromSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
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
		case SST_D3DSMM_MATERIAL:
			memcpy(&m_Material,(LPCTSTR)Value,sizeof(m_Material));
			break;
		case SST_D3DSMM_TEXTURE:
			{
				CSmartStruct SubPacket=Value;
				LPCTSTR szResourceName=SubPacket.GetMember(SST_TEX_TEXTURE);				
				UINT64 TextureProperty=SubPacket.GetMember(SST_TEX_PROPERTY);
				CD3DTexture * pTexture=(CD3DTexture *)pResourceManager->FindResource(szResourceName,GET_CLASS_INFO(CD3DTexture));
				if(pTexture)
				{
					pTexture->AddUseRef();					
				}
				AddTexture(pTexture,TextureProperty);
			}
			break;
		case SST_D3DSMM_FX:			
			{
				LPCTSTR szResourceName=Value;
				m_pFX=(CD3DFX *)pResourceManager->FindResource(szResourceName,GET_CLASS_INFO(CD3DFX));
				if(m_pFX)
				{
					m_pFX->AddUseRef();
				}				
			}
			break;
		}
	}
	return true;
}

UINT CD3DSubMeshMaterial::GetSmartStructSize(UINT Param)
{
	UINT Size=CNameObject::GetSmartStructSize(Param);
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_Material));
	for(UINT i=0;i<m_TextureList.GetCount();i++)
	{
		if(m_TextureList[i].pTexture)
			Size+=SMART_STRUCT_FIX_MEMBER_SIZE(m_TextureList[i].pTexture->GetNameLength());
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_TextureList[i].Property));
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	if(m_pFX)
	{
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_pFX->GetNameLength());
	}	
	return Size;
}


}