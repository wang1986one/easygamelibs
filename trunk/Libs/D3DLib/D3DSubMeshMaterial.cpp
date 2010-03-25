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
	for(size_t i=0;i<m_TextureList.size();i++)
	{
		SAFE_RELEASE(m_TextureList[i].pTexture);
	}
	m_TextureList.clear();
	SAFE_RELEASE(m_pFX);
	CNameObject::Destory();
}

bool CD3DSubMeshMaterial::Reset()
{
	bool Ret=true;
	if(m_pFX)
		Ret=m_pFX->Reset();
	for(size_t i=0;i<m_TextureList.size();i++)
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
	for(size_t i=0;i<m_TextureList.size();i++)
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
	if(Layer<m_TextureList.size())
	{
		SAFE_RELEASE(m_TextureList[Layer].pTexture);
		m_TextureList[Layer].pTexture=pTexture;
		return true;
	}
	return false;
}

void CD3DSubMeshMaterial::ClearAllTexture()
{
	for(size_t i=0;i<m_TextureList.size();i++)
	{
		SAFE_RELEASE(m_TextureList[i].pTexture);
	}
	m_TextureList.clear();
}


bool CD3DSubMeshMaterial::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CNameObject::ToSmartStruct(Packet,pUSOFile,Param))
		return false;	

	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSMM_MATERIAL,(char *)&m_Material,sizeof(m_Material)));
	for(UINT i=0;i<m_TextureList.size();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		int ResourceID=pUSOFile->ResourceObjectToIndex(m_TextureList[i].pTexture);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_TEX_TEXTURE,ResourceID));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_TEX_PROPERTY,m_TextureList[i].Property));
		if(!Packet.FinishMember(SST_D3DSMM_TEXTURE,SubPacket.GetDataLen()))
			return false;
	}
	if(m_pFX)
	{
		int ResourceID=pUSOFile->ResourceObjectToIndex(m_pFX);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSMM_FX,ResourceID));		
	}
	return true;
}
bool CD3DSubMeshMaterial::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CNameObject::FromSmartStruct(Packet,pUSOFile,Param))
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
				int ResourceID=SubPacket.GetMember(SST_TEX_TEXTURE);				
				UINT64 TextureProperty=SubPacket.GetMember(SST_TEX_PROPERTY);
				CD3DTexture * pTexture=(CD3DTexture *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DTexture));
				if(pTexture)
				{
					pTexture->AddUseRef();					
				}
				AddTexture(pTexture,TextureProperty);
			}
			break;
		case SST_D3DSMM_FX:			
			{
				int ResourceID=Value;
				m_pFX=(CD3DFX *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DFX));
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
	for(UINT i=0;i<m_TextureList.size();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_TextureList[i].Property));
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	if(m_pFX)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
	}	
	return Size;
}


}