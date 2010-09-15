/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWM2ItemModel.cpp                                    */
/*      创建日期:  2009年10月20日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

namespace D3DLib{


IMPLEMENT_CLASS_INFO(CD3DWOWM2ItemModel,CD3DWOWM2Model);

CD3DWOWM2ItemModel::CD3DWOWM2ItemModel(void)
{
	m_ItemDisplayID=0;
	m_ItemHandType=IHT_LEFT;	
}

CD3DWOWM2ItemModel::~CD3DWOWM2ItemModel(void)
{
}


bool CD3DWOWM2ItemModel::BuildModel()
{
	
	CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pDisplayInfo=CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(m_ItemDisplayID);
	if(pDisplayInfo)
	{
		CEasyString TexturePath=GetPathDirectory(GetName());
		if(m_ItemHandType==IHT_LEFT)
			TexturePath+=pDisplayInfo->LeftModelTexture+".blp";
		else
			TexturePath+=pDisplayInfo->RightModelTexture+".blp";
		CD3DTexture * pTexture=GetDevice()->GetTextureManager()->LoadTexture(TexturePath);
		if(pTexture)
		{
			for(int i=0;i<GetSubMeshCount();i++)
			{
				CD3DSubMesh  * pSubMesh=GetSubMesh(i);
				if(pSubMesh)
				{
					for(UINT i=0;i<pSubMesh->GetMaterial().GetTextureLayerCount();i++)
					{
						UINT TextureType=pSubMesh->GetMaterial().GetTextureProperty(i)&CD3DWOWM2ModelResource::SMP_TEXTURE_TYPE;
						if(TextureType!=CD3DWOWM2ModelResource::SMP_TEXTURE_TYPE_DIRECT)
						{
							pSubMesh->GetMaterial().SetTexture(i,pTexture);
							pTexture->AddUseRef();
						}
					}

				}
			}
			SAFE_RELEASE(pTexture);			
		}
	}
	
	return true;
}

bool CD3DWOWM2ItemModel::CloneFrom(CNameObject * pObject,UINT Param)
{
	if(!pObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2ItemModel)))
		return false;

	if(!CD3DWOWM2Model::CloneFrom(pObject,Param))
		return false;

	CD3DWOWM2ItemModel * pSrcObject=(CD3DWOWM2ItemModel *)pObject;

	m_ItemDisplayID=pSrcObject->m_ItemDisplayID;
	m_ItemHandType=pSrcObject->m_ItemHandType;	
	return true;
}

bool CD3DWOWM2ItemModel::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	SAFE_RELEASE(m_pBoundingFrame);

	if(!CD3DWOWM2Model::ToSmartStruct(Packet,pUSOFile,Param))
		return false;

	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMIM_ITEM_DISPLAY_ID,m_ItemDisplayID));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMIM_ITEM_HAND_TYPE,m_ItemHandType));

	return true;
}
bool CD3DWOWM2ItemModel::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DWOWM2Model::FromSmartStruct(Packet,pUSOFile,Param))
		return false;

	m_ItemDisplayID=Packet.GetMember(SST_D3DWMIM_ITEM_DISPLAY_ID);
	m_ItemHandType=Packet.GetMember(SST_D3DWMIM_ITEM_HAND_TYPE);
	return true;
}
UINT CD3DWOWM2ItemModel::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DWOWM2Model::GetSmartStructSize(Param);
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ItemDisplayID));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ItemHandType));

	return Size;
}


//CNameObject::STORAGE_STRUCT * CD3DWOWM2ItemModel::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//int CD3DWOWM2ItemModel::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int HeadSize=CD3DWOWM2Model::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	pLocalHead->ItemID=m_ItemID;
//	pLocalHead->ItemHandType=m_ItemHandType;	
//	return sizeof(STORAGE_STRUCT);
//}
//
//int CD3DWOWM2ItemModel::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int ReadSize=CD3DWOWM2Model::USOReadHead(pHead,pUSOFile,Param);
//	if(ReadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//
//	m_ItemID=pLocalHead->ItemID;
//	m_ItemHandType=pLocalHead->ItemHandType;	
//	return sizeof(STORAGE_STRUCT);
//}


void CD3DWOWM2ItemModel::Update(FLOAT Time)
{
	FUNCTION_BEGIN;
	CD3DWOWM2Model::Update(Time);
	FUNCTION_END;
}

}