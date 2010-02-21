#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DWOWM2CharacterModel,CD3DWOWM2Model);
CD3DWOWM2CharacterModel::CD3DWOWM2CharacterModel(void)
{
	m_CharRace=0;
	m_CharSex=0;
	m_CharSexMax=0;
	m_CharSkinColor=0;
	m_CharSkinColorMax=0;
	m_CharHairColor=0;
	m_CharHairColorMax=0;
	m_CharFaceType=0;
	m_CharFaceTypeMax=0;
	m_CharHairType=0;
	m_CharHairTypeMax=0;
	m_CharWhiskerType=0;
	m_CharWhiskerTypeMax=0;
	m_IsCharBald=false;
	ZeroMemory(m_Equipments,sizeof(m_Equipments));
	m_pHelmetModel=NULL;
	m_pLeftShoulderModel=NULL;
	m_pRightShoulderModel=NULL;
	m_pLeftWeaponModel=NULL;
	m_pRightWeaponModel=NULL;

	m_CloseHandAnimationIndex=-1;



}

CD3DWOWM2CharacterModel::~CD3DWOWM2CharacterModel(void)
{
	Destory();
}


void CD3DWOWM2CharacterModel::Destory()
{
	DestoryModel();
	
	CD3DWOWM2Model::Destory();
}

void CD3DWOWM2CharacterModel::DestoryModel()
{
	m_CloseHandAnimationIndex=-1;

	if(m_pHelmetModel)
		m_pHelmetModel->SetParent(NULL);
	if(m_pLeftShoulderModel)
		m_pLeftShoulderModel->SetParent(NULL);
	if(m_pRightShoulderModel)
		m_pRightShoulderModel->SetParent(NULL);
	if(m_pLeftWeaponModel)
		m_pLeftWeaponModel->SetParent(NULL);
	if(m_pRightWeaponModel)
		m_pRightWeaponModel->SetParent(NULL);
	SAFE_RELEASE(m_pHelmetModel);
	SAFE_RELEASE(m_pLeftShoulderModel);
	SAFE_RELEASE(m_pRightShoulderModel);
	SAFE_RELEASE(m_pLeftWeaponModel);
	SAFE_RELEASE(m_pRightWeaponModel);	
	m_SubMeshMaterialList.Clear();
	m_SubMeshList.Clear();
}

bool CD3DWOWM2CharacterModel::Reset()
{
	bool Ret=true;
	for(UINT i=0;i<m_SubMeshMaterialList.GetCount();i++)
	{		
		Ret=Ret&&m_SubMeshMaterialList[i].Reset();
	}
	return Ret&&CD3DWOWM2Model::Reset();
}
bool CD3DWOWM2CharacterModel::Restore()
{
	bool Ret=true;
	for(UINT i=0;i<m_SubMeshMaterialList.GetCount();i++)
	{		
		Ret=Ret&&m_SubMeshMaterialList[i].Restore();
	}
	return Ret&&CD3DWOWM2Model::Restore();
}

bool CD3DWOWM2CharacterModel::SetCharRace(int Value)
{
	if(Value<=0||Value>CBLZWOWDatabase::GetInstance()->GetMaxCharRace())
		return false;
	m_CharRace=Value;	
	m_CharSexMax=CBLZWOWDatabase::GetInstance()->GetMaxCharSex(m_CharRace);
	return true;
}
bool CD3DWOWM2CharacterModel::SetCharSex(int Value)
{
	if(Value<0||Value>=m_CharSexMax)
		return false;
	m_CharSex=Value;

	CBLZWOWDatabase::GetInstance()->GetCharMaxInfo(m_CharRace,m_CharSex,
		m_CharSkinColorMax,m_CharHairColorMax,m_CharFaceTypeMax,m_CharHairTypeMax,m_CharWhiskerTypeMax);
	
	return true;

}
bool CD3DWOWM2CharacterModel::SetCharSkinColor(int Value)
{
	if(Value<0||Value>=m_CharSkinColorMax)
		return false;
	m_CharSkinColor=Value;
	return true;
}
bool CD3DWOWM2CharacterModel::SetCharHairColor(int Value)
{
	if(Value<0||Value>=m_CharHairColorMax)
		return false;
	m_CharHairColor=Value;
	return true;
}
bool CD3DWOWM2CharacterModel::SetCharFaceType(int Value)
{
	if(Value<0||Value>=m_CharFaceTypeMax)
		return false;
	m_CharFaceType=Value;
	return true;
}
bool CD3DWOWM2CharacterModel::SetCharHairType(int Value)
{
	if(Value<0||Value>=m_CharHairTypeMax)
		return false;
	m_CharHairType=Value;
	return true;
}
bool CD3DWOWM2CharacterModel::SetCharWhiskerType(int Value)
{
	if(Value<0||Value>=m_CharWhiskerTypeMax)
		return false;
	m_CharWhiskerType=Value;
	return true;
}		

bool CD3DWOWM2CharacterModel::SetEquipment(UINT Slot,UINT ItemID)
{
	if(Slot<CES_MAX)
	{
		if(ItemID)
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
			if(pItemData)
			{
				CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
					CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(pItemData->ItemDisplayInfo);
				if(pItemDisplayInfo)
				{
					m_Equipments[Slot]=ItemID;
					return true;
				}
			}		
		}
		else
		{
			m_Equipments[Slot]=0;
		}
		
	}
	return false;
}


bool CD3DWOWM2CharacterModel::BuildCharModel()
{
	if(GetDevice()==NULL||m_pModelResource==NULL)
		return false;

	DestoryModel();

	m_CloseHandAnimationIndex=m_pModelResource->GetAnimationIndex(15,0);

	int MaxSkinColor;
	int MaxHairColor;
	int MaxFaceType;
	int MaxHairType;
	int MaxWhiskerType;
	CBLZWOWDatabase::GetInstance()->GetCharMaxInfo(m_CharRace,m_CharSex,MaxSkinColor,MaxHairColor,MaxFaceType,MaxHairType,MaxWhiskerType);

	int CharHairSubMesh=CBLZWOWDatabase::GetInstance()->FindCharHairSubMesh(m_CharRace,m_CharSex,m_CharHairType,m_IsCharBald);

	int CharWhiskerSubMeshID1=0;
	int CharWhiskerSubMeshID2=0;
	int CharWhiskerSubMeshID3=0;
	CBLZWOWDatabase::GetInstance()->FindCharWhiskerSubMesh(m_CharRace,m_CharSex,m_CharWhiskerType,
		CharWhiskerSubMeshID1,CharWhiskerSubMeshID2,CharWhiskerSubMeshID3);

	int BackSubMeshType=1;
	CEasyString BackTextureFileName;
	int GloveType=1;
	int PantsType=1;
	int RobeType=1;
	int BootsType=1;
	int SleeveType=1;
	int EnsignType=1;

	bool	HairVisible=true;				
	bool	Facial1Visible=true;			
	bool	Facial2Visible=true;			
	bool	Facial3Visible=true;			
	bool	EarsVisible=true;

	if(m_Equipments[CES_HEAD])
	{
		CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(m_Equipments[CES_HEAD]);
		if(pItemData)
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
				CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(pItemData->ItemDisplayInfo);
			if(pItemDisplayInfo)
			{
				CBLZWOWDatabase::BLZ_DB_CHAR_RACE_INFO * pRaceInfo=CBLZWOWDatabase::GetInstance()->FindRaceInfo(m_CharRace);
				if(pRaceInfo)
				{
					CEasyString ModelFileName=ITEM_PATH_BY_SLOT[pItemData->InventorySlotID];
					ModelFileName=ModelFileName+"\\"+pItemDisplayInfo->LeftModel;
					int Pos=ModelFileName.ReverseFind('.');
					if(Pos>=0)
					{
						ModelFileName=ModelFileName.Left(Pos);
					}				
					ModelFileName=ModelFileName+"_"+pRaceInfo->Abbrev;
					if(m_CharSex==0)
						ModelFileName+="M";
					else
						ModelFileName+="F";
					CEasyString SkinFileName=ModelFileName+"00.skin";
					ModelFileName=ModelFileName+".m2";

					CD3DDummy * pDummy=EnableAttachment(CAI_HELMET);
					if(pDummy)
					{
						m_pHelmetModel=new CD3DWOWM2ItemModel();
						m_pHelmetModel->SetDevice(GetDevice());
						if(m_pHelmetModel->LoadFromFile(ModelFileName,SkinFileName))
						{
							m_pHelmetModel->Play(0,0,0,true);
							m_pHelmetModel->SetParent(pDummy);
							//GetRender()->AddObject(m_pHelmetModel);
							m_pHelmetModel->SetItemID(m_Equipments[CES_HEAD]);
							m_pHelmetModel->BuildModel();

							CBLZWOWDatabase::BLZ_DB_HELMET_GEOSET_VISIBLE_INFO * pVisibleInfo=NULL;
							
							pVisibleInfo=CBLZWOWDatabase::GetInstance()->GetHelmetGeosetVisibleInfo(pItemDisplayInfo->HelmetGeosetVisData1);
						
							//pVisibleInfo=CBLZWOWDatabase::GetInstance()->GetHelmetGeosetVisibleInfo(pItemDisplayInfo->HelmetGeosetVisData2);

							if(pVisibleInfo)
							{
								HairVisible=((pVisibleInfo->HairVisible>>m_CharRace)&1)==0;
								Facial1Visible=((pVisibleInfo->Facial1Visible>>m_CharRace)&1)==0;
								Facial2Visible=((pVisibleInfo->Facial2Visible>>m_CharRace)&1)==0;
								Facial3Visible=((pVisibleInfo->Facial3Visible>>m_CharRace)&1)==0;
								EarsVisible=((pVisibleInfo->EarsVisible>>m_CharRace)&1)==0;
							}

						}
						else
						{
							SAFE_RELEASE(m_pHelmetModel);
						}
					}
				}
			}
		}
	}

	if(m_Equipments[CES_SHOULDER])
	{
		CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(m_Equipments[CES_SHOULDER]);
		if(pItemData)
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
				CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(pItemData->ItemDisplayInfo);
			if(pItemDisplayInfo)
			{
				CEasyString ModelFileName;
				CEasyString SkinFileName;
				int Pos;
				CD3DDummy * pDummy=NULL;
				
				ModelFileName=ITEM_PATH_BY_SLOT[pItemData->InventorySlotID];
				ModelFileName=ModelFileName+"\\"+pItemDisplayInfo->LeftModel;
				Pos=ModelFileName.ReverseFind('.');
				if(Pos>=0)
				{
					ModelFileName=ModelFileName.Left(Pos);
				}
				SkinFileName=ModelFileName+"00.skin";
				ModelFileName=ModelFileName+".m2";

				pDummy=EnableAttachment(CAI_LEFT_SHOULDER);
				if(pDummy)
				{
					m_pLeftShoulderModel=new CD3DWOWM2ItemModel();
					m_pLeftShoulderModel->SetDevice(GetDevice());
					
					if(m_pLeftShoulderModel->LoadFromFile(ModelFileName,SkinFileName))
					{
						m_pLeftShoulderModel->Play(0,0,0,true);
						m_pLeftShoulderModel->SetParent(pDummy);
						//GetRender()->AddObject(m_pLeftShoulderModel);
						m_pLeftShoulderModel->SetItemID(m_Equipments[CES_SHOULDER]);
						m_pLeftShoulderModel->SetItemHandType(CD3DWOWM2ItemModel::IHT_LEFT);
						m_pLeftShoulderModel->BuildModel();
					}
					else
					{
						SAFE_RELEASE(m_pLeftShoulderModel);
					}
				}

				ModelFileName=ITEM_PATH_BY_SLOT[pItemData->InventorySlotID];
				ModelFileName=ModelFileName+"\\"+pItemDisplayInfo->RightModel;
				Pos=ModelFileName.ReverseFind('.');
				if(Pos>=0)
				{
					ModelFileName=ModelFileName.Left(Pos);
				}
				SkinFileName=ModelFileName+"00.skin";
				ModelFileName=ModelFileName+".m2";

				pDummy=EnableAttachment(CAI_RIGHT_SHOULDER);
				if(pDummy)
				{
					m_pRightShoulderModel=new CD3DWOWM2ItemModel();
					m_pRightShoulderModel->SetDevice(GetDevice());
					if(m_pRightShoulderModel->LoadFromFile(ModelFileName,SkinFileName))
					{
						m_pRightShoulderModel->Play(0,0,0,true);
						m_pRightShoulderModel->SetParent(pDummy);
						//GetRender()->AddObject(m_pRightShoulderModel);
						m_pRightShoulderModel->SetItemID(m_Equipments[CES_SHOULDER]);
						m_pRightShoulderModel->SetItemHandType(CD3DWOWM2ItemModel::IHT_RIGHT);						
						m_pRightShoulderModel->BuildModel();
					}
					else
					{
						SAFE_RELEASE(m_pRightShoulderModel);
					}
				}
				
			}
		}
	}


	if(m_Equipments[CES_LEFT_HAND])
	{
		CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(m_Equipments[CES_LEFT_HAND]);
		if(pItemData)
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
				CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(pItemData->ItemDisplayInfo);
			if(pItemDisplayInfo)
			{
				CEasyString ModelFileName;
				CEasyString SkinFileName;
				int Pos;
				CD3DDummy * pDummy=NULL;

				ModelFileName=ITEM_PATH_BY_SLOT[pItemData->InventorySlotID];
				ModelFileName=ModelFileName+"\\"+pItemDisplayInfo->LeftModel;
				Pos=ModelFileName.ReverseFind('.');
				if(Pos>=0)
				{
					ModelFileName=ModelFileName.Left(Pos);
				}
				SkinFileName=ModelFileName+"00.skin";
				ModelFileName=ModelFileName+".m2";

				if(pItemData->InventorySlotID==IISI_SHIELD)
				{
					pDummy=EnableAttachment(CAI_LEFT_WRIST);
				}
				else
				{
					pDummy=EnableAttachment(CAI_LEFT_PALM2);
				}				
				if(pDummy)
				{
					m_pLeftWeaponModel=new CD3DWOWM2ItemModel();
					m_pLeftWeaponModel->SetDevice(GetDevice());
					if(m_pLeftWeaponModel->LoadFromFile(ModelFileName,SkinFileName))
					{
						m_pLeftWeaponModel->Play(0,0,0,true);
						m_pLeftWeaponModel->SetParent(pDummy);
						//GetRender()->AddObject(m_pLeftWeaponModel);
						m_pLeftWeaponModel->SetItemID(m_Equipments[CES_LEFT_HAND]);						
						m_pLeftWeaponModel->BuildModel();
					}
					else
					{
						SAFE_RELEASE(m_pLeftWeaponModel);
					}
				}
			}
		}
	}

	if(m_Equipments[CES_RIGHT_HAND])
	{
		CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(m_Equipments[CES_RIGHT_HAND]);
		if(pItemData)
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
				CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(pItemData->ItemDisplayInfo);
			if(pItemDisplayInfo)
			{
				CEasyString ModelFileName;
				CEasyString SkinFileName;
				int Pos;
				CD3DDummy * pDummy=NULL;

				ModelFileName=ITEM_PATH_BY_SLOT[pItemData->InventorySlotID];
				ModelFileName=ModelFileName+"\\"+pItemDisplayInfo->LeftModel;
				Pos=ModelFileName.ReverseFind('.');
				if(Pos>=0)
				{
					ModelFileName=ModelFileName.Left(Pos);
				}
				SkinFileName=ModelFileName+"00.skin";
				ModelFileName=ModelFileName+".m2";

				pDummy=EnableAttachment(CAI_RIGHT_PALM2);
				if(pDummy)
				{
					m_pRightWeaponModel=new CD3DWOWM2ItemModel();
					m_pRightWeaponModel->SetDevice(GetDevice());					
					if(m_pRightWeaponModel->LoadFromFile(ModelFileName,SkinFileName))
					{
						m_pRightWeaponModel->Play(0,0,0,true);
						m_pRightWeaponModel->SetParent(pDummy);
						//GetRender()->AddObject(m_pRightWeaponModel);
						m_pRightWeaponModel->SetItemID(m_Equipments[CES_RIGHT_HAND]);						
						m_pRightWeaponModel->BuildModel();
					}
					else
					{
						SAFE_RELEASE(m_pRightWeaponModel);
					}
				}
			}
		}
	}
	

	if(m_Equipments[CES_BACK])
	{
		CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(m_Equipments[CES_BACK]);
		if(pItemData)
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
				CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(pItemData->ItemDisplayInfo);
			if(pItemDisplayInfo)
			{
				BackSubMeshType=pItemDisplayInfo->GloveGeosetFlags+1;
				BackTextureFileName=ITEM_PATH_BY_SLOT[pItemData->InventorySlotID];
				BackTextureFileName=BackTextureFileName+"\\"+pItemDisplayInfo->LeftModelTexture+".blp";
			}
		}
	}
	
	if(m_Equipments[CES_HAND])
	{
		CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(m_Equipments[CES_HAND]);
		if(pItemData)
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
				CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(pItemData->ItemDisplayInfo);
			if(pItemDisplayInfo)
			{
				GloveType=pItemDisplayInfo->GloveGeosetFlags+1;
			}
		}
	}

	
	if(m_Equipments[CES_LEG])
	{
		CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(m_Equipments[CES_LEG]);
		if(pItemData)
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
				CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(pItemData->ItemDisplayInfo);
			if(pItemDisplayInfo)
			{
				PantsType=pItemDisplayInfo->PantsGeosetFlags+1;
				RobeType=pItemDisplayInfo->RobeGeosetFlags+1;
			}
		}
	}

	if(m_Equipments[CES_FOOT])
	{
		CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(m_Equipments[CES_FOOT]);
		if(pItemData)
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
				CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(pItemData->ItemDisplayInfo);
			if(pItemDisplayInfo)
			{				
				BootsType=pItemDisplayInfo->GloveGeosetFlags+1;					
			}
		}
	}

	if(m_Equipments[CES_BUST])
	{
		CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(m_Equipments[CES_BUST]);
		if(pItemData)
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
				CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(pItemData->ItemDisplayInfo);
			if(pItemDisplayInfo)
			{				
				SleeveType=pItemDisplayInfo->GloveGeosetFlags+1;
				RobeType=pItemDisplayInfo->RobeGeosetFlags+1;
				//PantsType=pItemDisplayInfo->PantsGeosetFlags+1;
			}
		}
	}
	if(m_Equipments[CES_ENSIGN])
	{
		CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(m_Equipments[CES_ENSIGN]);
		if(pItemData)
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
				CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(pItemData->ItemDisplayInfo);
			if(pItemDisplayInfo)
			{				
				EnsignType=pItemDisplayInfo->GloveGeosetFlags+1;				
			}
		}
	}
	
	if(RobeType>1)
	{		
		PantsType=0;
		BootsType=0;
		EnsignType=0;
	}
	if(GloveType>1)
	{
		SleeveType=1;
	}
	if(BootsType>1)
	{
		if(PantsType==2)
			PantsType=1;
	}
	
	for(int i=0;i<m_pModelResource->GetSubMeshCount();i++)
	{
		CD3DSubMesh * pD3DSubMesh=m_pModelResource->GetSubMesh(i);

		pD3DSubMesh->SetVisible(false);
		
		int Part=pD3DSubMesh->GetID()/100;
		int Type=pD3DSubMesh->GetID()%100;

		switch(Part)
		{
		case CSP_HAIR:
			if(Type==0)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}
			else
			{
				if(Type==CharHairSubMesh&&HairVisible)
				{
					m_SubMeshList.Add(pD3DSubMesh);
				}				
			}
			break;
		case CSP_WHISKER1:
			if(Type==CharWhiskerSubMeshID1&&Facial1Visible)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}	
			break;
		case CSP_WHISKER2:
			if(Type==CharWhiskerSubMeshID2&&Facial2Visible)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}	
			break;
		case CSP_WHISKER3:
			if(Type==CharWhiskerSubMeshID3&&Facial2Visible)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}	
			break;
		case CSP_GLOVE:
			if(Type==GloveType)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}	
			break;
		case CSP_FOOT:
			if(Type==BootsType)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}	
			break;
		case CSP_EAR:
			if(EarsVisible)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}	
			break;
		case CSP_SLEEVE:
			if(Type==SleeveType)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}	
			break;
		case CSP_PAINTS:
			if(Type==PantsType)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}	
			break;
		case CSP_ENSIGN:
			if(Type==EnsignType)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}	
			break;
		case CSP_ROBE:
			if(Type==RobeType)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}	
			break;
		case CSP_BACK:			
			if(Type==BackSubMeshType)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}					
			break;
		default:
			if(Type==1)
			{
				m_SubMeshList.Add(pD3DSubMesh);
			}	
		}	
		
	}

	m_SubMeshMaterialList.Resize(m_SubMeshList.GetCount());

	CD3DTexture * pCharSkinTexture;
	CD3DTexture * pCharSkinExtraTexture;
	CD3DTexture * pCharHairTexture;	
	MakeCharSkinTexture(GetDevice(),pCharSkinTexture,pCharSkinExtraTexture,pCharHairTexture,SleeveType>1);

	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{

		m_SubMeshList[i]->SetVisible(true);
		m_SubMeshMaterialList[i].SetMaterial(m_SubMeshList[i]->GetMaterial().GetMaterial());

		int Part=m_SubMeshList[i]->GetID()/100;
		int Type=m_SubMeshList[i]->GetID()%100;		

		switch(Part)
		{		
		case CSP_BACK:			
			if(BackSubMeshType!=1)
			{				
				CD3DTexture * pTexture=GetDevice()->GetTextureManager()->
					LoadTexture(BackTextureFileName);
				UINT64 TextureProperty=m_SubMeshList[i]->GetMaterial().GetTextureProperty(0);
				m_SubMeshMaterialList[i].AddTexture(pTexture,TextureProperty);
			}
			break;		

		}	
		if(m_SubMeshList[i]->GetMaterial().GetTextureLayerCount()>0)
		{
			UINT64 TextureProperty=m_SubMeshList[i]->GetMaterial().GetTextureProperty(0);
			int TextureType=TextureProperty&CD3DWOWM2ModelResource::SMP_TEXTURE_TYPE;
			switch(TextureType)
			{
			case CD3DWOWM2ModelResource::SMP_TEXTURE_TYPE_BODY:
				if(pCharSkinTexture)
				{				
					m_SubMeshMaterialList[i].AddTexture(pCharSkinTexture,TextureProperty);
					pCharSkinTexture->AddUseRef();
				}
				break;
			case CD3DWOWM2ModelResource::SMP_TEXTURE_TYPE_HAIR:
				if(pCharHairTexture)
				{

					m_SubMeshMaterialList[i].AddTexture(pCharHairTexture,TextureProperty);
					pCharHairTexture->AddUseRef();
				}
				break;
			case CD3DWOWM2ModelResource::SMP_TEXTURE_TYPE_FUR:
				if(pCharSkinExtraTexture)
				{

					m_SubMeshMaterialList[i].AddTexture(pCharSkinExtraTexture,TextureProperty);
					pCharSkinExtraTexture->AddUseRef();
				}
				break;		
			default:
				PrintSystemLog(0,"SubMesh[%s] has no Texture",m_SubMeshList[i]->GetName());
				break;
			}
		}

		
		m_SubMeshMaterialList[i].SetFX(m_SubMeshList[i]->GetMaterial().GetFX());
		if(m_SubMeshList[i]->GetMaterial().GetFX())
			m_SubMeshList[i]->GetMaterial().GetFX()->AddUseRef();
				
	}
	SAFE_RELEASE(pCharSkinTexture);
	SAFE_RELEASE(pCharSkinExtraTexture);
	SAFE_RELEASE(pCharHairTexture);
	return true;
}
int CD3DWOWM2CharacterModel::GetSubMeshCount()
{
	return (int)m_SubMeshList.GetCount();
}
CD3DSubMesh * CD3DWOWM2CharacterModel::GetSubMesh(int index)
{
	return m_SubMeshList[index];
}
CD3DSubMeshMaterial * CD3DWOWM2CharacterModel::GetSubMeshMaterial(int index)
{
	return &(m_SubMeshMaterialList[index]);
}

bool CD3DWOWM2CharacterModel::CloneFrom(CNameObject * pObject,UINT Param)
{
	if(!pObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2CharacterModel)))
		return false;

	if(!CD3DWOWM2Model::CloneFrom(pObject,Param))
		return false;

	CD3DWOWM2CharacterModel * pSrcObject=(CD3DWOWM2CharacterModel *)pObject;

	m_SubMeshList=pSrcObject->m_SubMeshList;

	m_SubMeshMaterialList.Resize(pSrcObject->m_SubMeshMaterialList.GetCount());
	for(UINT i=0;i<pSrcObject->m_SubMeshMaterialList.GetCount();i++)
	{
		m_SubMeshMaterialList[i].SetMaterial(pSrcObject->m_SubMeshMaterialList[i].GetMaterial());
		for(UINT j=0;j<pSrcObject->m_SubMeshMaterialList[i].GetTextureLayerCount();j++)
		{
			m_SubMeshMaterialList[i].AddTexture(pSrcObject->m_SubMeshMaterialList[i].GetTexture(j),
				pSrcObject->m_SubMeshMaterialList[i].GetTextureProperty(j));
			if(pSrcObject->m_SubMeshMaterialList[i].GetTexture(j))
				pSrcObject->m_SubMeshMaterialList[i].GetTexture(j)->AddUseRef();
		}	
		m_SubMeshMaterialList[i].SetFX(pSrcObject->m_SubMeshMaterialList[i].GetFX());
		if(pSrcObject->m_SubMeshMaterialList[i].GetFX())
			pSrcObject->m_SubMeshMaterialList[i].GetFX()->AddUseRef();
	}

	m_CharRace=pSrcObject->m_CharRace;
	m_CharSex=pSrcObject->m_CharSex;
	m_CharSexMax=pSrcObject->m_CharSexMax;
	m_CharSkinColor=pSrcObject->m_CharSkinColor;
	m_CharSkinColorMax=pSrcObject->m_CharSkinColorMax;
	m_CharHairColor=pSrcObject->m_CharHairColor;
	m_CharHairColorMax=pSrcObject->m_CharHairColorMax;
	m_CharFaceType=pSrcObject->m_CharFaceType;
	m_CharFaceTypeMax=pSrcObject->m_CharFaceTypeMax;
	m_CharHairType=pSrcObject->m_CharHairType;
	m_CharHairTypeMax=pSrcObject->m_CharHairTypeMax;
	m_CharWhiskerType=pSrcObject->m_CharWhiskerType;
	m_CharWhiskerTypeMax=pSrcObject->m_CharWhiskerTypeMax;
	m_IsCharBald=pSrcObject->m_IsCharBald;

	memcpy(m_Equipments,pSrcObject->m_Equipments,sizeof(m_Equipments));

	if(pSrcObject->m_pHelmetModel)
		m_pHelmetModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(pSrcObject->m_pHelmetModel->GetStorageID());
	if(pSrcObject->m_pLeftShoulderModel)
		m_pLeftShoulderModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(pSrcObject->m_pLeftShoulderModel->GetStorageID());
	if(pSrcObject->m_pRightShoulderModel)
		m_pRightShoulderModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(pSrcObject->m_pRightShoulderModel->GetStorageID());
	if(pSrcObject->m_pLeftWeaponModel)
		m_pLeftWeaponModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(pSrcObject->m_pLeftWeaponModel->GetStorageID());
	if(pSrcObject->m_pRightWeaponModel)
		m_pRightWeaponModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(pSrcObject->m_pRightWeaponModel->GetStorageID());

	return true;

}

void CD3DWOWM2CharacterModel::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	CD3DWOWM2Model::PickResource(pObjectSet,Param);
	for(UINT i=0;i<m_SubMeshMaterialList.GetCount();i++)
	{
		if(m_SubMeshMaterialList[i].GetFX())
		{			
			pObjectSet->Add(m_SubMeshMaterialList[i].GetFX());
		}
		for(UINT j=0;j<m_SubMeshMaterialList[i].GetTextureLayerCount();j++)
		{
			if(m_SubMeshMaterialList[i].GetTexture(j))
			{
				pObjectSet->Add(m_SubMeshMaterialList[i].GetTexture(j));
			}
		}
	}
}

bool CD3DWOWM2CharacterModel::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	SAFE_RELEASE(m_pBoundingFrame);

	if(!CD3DWOWM2Model::ToSmartStruct(Packet,pUSOFile,Param))
		return false;

	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_ACITVE_SUB_MESH,
			m_SubMeshList[i]->GetStorageID()));
	}
	for(UINT i=0;i<m_SubMeshMaterialList.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);

		if(!m_SubMeshMaterialList[i].ToSmartStruct(SubPacket,pUSOFile,Param))
			return false;

		if(!Packet.FinishMember(SST_D3DWMCM_CHAR_MATERIAL,SubPacket.GetDataLen()))
			return false;
	}
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_RACE,m_CharRace));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_SEX,m_CharSex));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_SKIN_COLOR,m_CharSkinColor));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_HAIR_COLOR,m_CharHairColor));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_FACE_TYPE,m_CharFaceType));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_HAIR_TYPE,m_CharHairType));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_WHISKER_TYPE,m_CharWhiskerType));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_IS_BALD,(BYTE)m_IsCharBald));

	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_EQUIPMENTS,(char *)m_Equipments,sizeof(m_Equipments)));

	if(m_pHelmetModel)
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_HELMET_MODEL,m_pHelmetModel->GetStorageID()));
	}
	if(m_pLeftShoulderModel)
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_LEFT_SHOULDER_MODEL,m_pLeftShoulderModel->GetStorageID()));
	}
	if(m_pRightShoulderModel)
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_RIGHT_SHOULDER_MODEL,m_pRightShoulderModel->GetStorageID()));
	}
	if(m_pLeftWeaponModel)
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_LEFT_WEAPON_MODEL,m_pLeftWeaponModel->GetStorageID()));
	}
	if(m_pRightWeaponModel)
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMCM_CHAR_RIGHT_WEAPON_MODEL,m_pRightWeaponModel->GetStorageID()));
	}
	
	return true;
}
bool CD3DWOWM2CharacterModel::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DWOWM2Model::FromSmartStruct(Packet,pUSOFile,Param))
		return false;

	UINT SubMeshCount=0;
	UINT MaterialCount=0;

	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DWMCM_CHAR_ACITVE_SUB_MESH:
			SubMeshCount++;	
			break;
		case SST_D3DWMCM_CHAR_MATERIAL:
			MaterialCount++;
			break;
		}
	}
	m_SubMeshList.Resize(SubMeshCount);
	m_SubMeshMaterialList.Resize(MaterialCount);

	SubMeshCount=0;
	MaterialCount=0;

	Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DWMCM_CHAR_ACITVE_SUB_MESH:
			{
				UINT StorageID=Value;
				for(int i=0;i<m_pModelResource->GetSubMeshCount();i++)
				{
					if(m_pModelResource->GetSubMesh(i)->GetStorageID()==StorageID)
					{
						m_SubMeshList[SubMeshCount]=m_pModelResource->GetSubMesh(i);
						break;
					}
				}
			}	
			SubMeshCount++;
			break;
		case SST_D3DWMCM_CHAR_MATERIAL:
			{
				CSmartStruct MaterialPacket=Value;
				if(!m_SubMeshMaterialList[MaterialCount].FromSmartStruct(MaterialPacket,pUSOFile,Param))
					return false;
			}
			MaterialCount++;
			break;
		case SST_D3DWMCM_CHAR_RACE:
			m_CharRace=Value;
			break;
		case SST_D3DWMCM_CHAR_SEX:
			m_CharSex=Value;
			break;
		case SST_D3DWMCM_CHAR_SKIN_COLOR:
			m_CharSkinColor=Value;
			break;
		case SST_D3DWMCM_CHAR_HAIR_COLOR:
			m_CharHairColor=Value;
			break;
		case SST_D3DWMCM_CHAR_FACE_TYPE:
			m_CharFaceType=Value;
			break;
		case SST_D3DWMCM_CHAR_HAIR_TYPE:
			m_CharHairType=Value;
			break;
		case SST_D3DWMCM_CHAR_WHISKER_TYPE:
			m_CharWhiskerType=Value;
			break;
		case SST_D3DWMCM_CHAR_IS_BALD:
			m_IsCharBald=(BYTE)Value!=0;
			break;
		case SST_D3DWMCM_CHAR_EQUIPMENTS:
			memcpy(m_Equipments,(LPCTSTR)Value,sizeof(m_Equipments));
			break;
		case SST_D3DWMCM_CHAR_HELMET_MODEL:
			{
				UINT StorageID=Value;
				if(StorageID)
				{
					m_pHelmetModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(StorageID);
					if(!m_pHelmetModel->IsKindOf(GET_CLASS_INFO(CD3DWOWM2ItemModel)))
					{
						m_pHelmetModel=NULL;
					}
				}
			}
			break;
		case SST_D3DWMCM_CHAR_LEFT_SHOULDER_MODEL:
			{
				UINT StorageID=Value;
				if(StorageID)
				{
					m_pLeftShoulderModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(StorageID);
					if(!m_pLeftShoulderModel->IsKindOf(GET_CLASS_INFO(CD3DWOWM2ItemModel)))
					{
						m_pLeftShoulderModel=NULL;
					}
				}
			}
			break;
		case SST_D3DWMCM_CHAR_RIGHT_SHOULDER_MODEL:
			{
				UINT StorageID=Value;
				if(StorageID)
				{
					m_pRightShoulderModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(StorageID);
					if(!m_pRightShoulderModel->IsKindOf(GET_CLASS_INFO(CD3DWOWM2ItemModel)))
					{
						m_pRightShoulderModel=NULL;
					}
				}
			}
			break;
		case SST_D3DWMCM_CHAR_LEFT_WEAPON_MODEL:
			{
				UINT StorageID=Value;
				if(StorageID)
				{
					m_pLeftWeaponModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(StorageID);
					if(!m_pLeftWeaponModel->IsKindOf(GET_CLASS_INFO(CD3DWOWM2ItemModel)))
					{
						m_pLeftWeaponModel=NULL;
					}
				}
			}
			break;
		case SST_D3DWMCM_CHAR_RIGHT_WEAPON_MODEL:
			{
				UINT StorageID=Value;
				if(StorageID)
				{
					m_pRightWeaponModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(StorageID);
					if(!m_pRightWeaponModel->IsKindOf(GET_CLASS_INFO(CD3DWOWM2ItemModel)))
					{
						m_pRightWeaponModel=NULL;
					}
				}
			}
			break;
		}
	}
	m_CharSexMax=CBLZWOWDatabase::GetInstance()->GetMaxCharSex(m_CharRace);
	CBLZWOWDatabase::GetInstance()->GetCharMaxInfo(m_CharRace,m_CharSex,
		m_CharSkinColorMax,m_CharHairColorMax,m_CharFaceTypeMax,m_CharHairTypeMax,m_CharWhiskerTypeMax);
	return true;
}
UINT CD3DWOWM2CharacterModel::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DWOWM2Model::GetSmartStructSize(Param);

	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_SubMeshList[i]->GetStorageID()));
	}
	for(UINT i=0;i<m_SubMeshMaterialList.GetCount();i++)
	{
		Size+=m_SubMeshMaterialList[i].GetSmartStructSize(Param);
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_CharRace));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_CharSex));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_CharSkinColor));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_CharHairColor));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_CharFaceType));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_CharHairType));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_CharWhiskerType));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_IsCharBald));
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_Equipments));

	if(m_pHelmetModel)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_pHelmetModel->GetStorageID()));
	}
	if(m_pLeftShoulderModel)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_pLeftShoulderModel->GetStorageID()));
	}
	if(m_pRightShoulderModel)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_pRightShoulderModel->GetStorageID()));
	}
	if(m_pLeftWeaponModel)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_pLeftWeaponModel->GetStorageID()));
	}
	if(m_pRightWeaponModel)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_pRightWeaponModel->GetStorageID()));
	}

	

	return Size;
}

bool CD3DWOWM2CharacterModel::MakeCharSkinTexture(CD3DDevice * pD3DDevice,CD3DTexture *& pCharSkinTexture,CD3DTexture *& pCharSkinExtraTexture,CD3DTexture *& pCharHairTexture,bool HaveSleeve)
{
	pCharSkinTexture=NULL;
	pCharSkinExtraTexture=NULL;
	pCharHairTexture=NULL;
	if(m_CharRace==0)
		return false;

	CSmartPtr<CD3DTexture> pSkinTexture=NULL;
	CSmartPtr<CD3DTexture> pSkinExtraTexture=NULL;	
	CSmartPtr<CD3DTexture> pHairTexture=NULL;
	

	CBLZWOWDatabase::BLZ_DB_CHAR_SECTION * pSkinSection=CBLZWOWDatabase::GetInstance()->FindCharSection(0,m_CharRace,m_CharSex,0,m_CharSkinColor);
	if(pSkinSection==NULL)
		return false;
	pSkinTexture=new CD3DTexture(pD3DDevice->GetTextureManager());;
	if(!pSkinTexture->LoadTexture(pSkinSection->Texture1))
	{			
		return false;
	}

	pSkinExtraTexture=pD3DDevice->GetTextureManager()->LoadTexture(pSkinSection->Texture2);	

	CBLZWOWDatabase::BLZ_DB_CHAR_SECTION * pFaceSection=CBLZWOWDatabase::GetInstance()->FindCharSection(1,m_CharRace,m_CharSex,m_CharFaceType,m_CharSkinColor);
	if(pFaceSection)
	{		
		if(!pFaceSection->Texture1.IsEmpty())
		{
			CSmartPtr<CD3DTexture> pTexture=pD3DDevice->GetTextureManager()->LoadTexture(pFaceSection->Texture1);
			AddTexture(pSkinTexture,pTexture,128,64,0,192);
		}
		if(!pFaceSection->Texture2.IsEmpty())
		{
			CSmartPtr<CD3DTexture> pTexture=pD3DDevice->GetTextureManager()->LoadTexture(pFaceSection->Texture2);	
			AddTexture(pSkinTexture,pTexture,128,32,0,160);
		}
	}
	


	CBLZWOWDatabase::BLZ_DB_CHAR_SECTION * pWhiskerSection=CBLZWOWDatabase::GetInstance()->FindCharSection(2,m_CharRace,m_CharSex,m_CharWhiskerType,m_CharHairColor);
	if(pWhiskerSection)
	{		
		if(!pWhiskerSection->Texture1.IsEmpty())
		{
			CSmartPtr<CD3DTexture> pTexture=pD3DDevice->GetTextureManager()->LoadTexture(pWhiskerSection->Texture1);
			AddTexture(pSkinTexture,pTexture,128,64,0,192);
		}
		if(!pWhiskerSection->Texture2.IsEmpty())
		{
			CSmartPtr<CD3DTexture> pTexture=pD3DDevice->GetTextureManager()->LoadTexture(pWhiskerSection->Texture2);	
			AddTexture(pSkinTexture,pTexture,128,32,0,160);
		}
	}

	CBLZWOWDatabase::BLZ_DB_CHAR_SECTION * pHairSection=CBLZWOWDatabase::GetInstance()->FindCharSection(3,m_CharRace,m_CharSex,m_CharHairType,m_CharHairColor);
	if(pHairSection)
	{		
		if(!pHairSection->Texture1.IsEmpty())
		{
			pHairTexture=pD3DDevice->GetTextureManager()->LoadTexture(pHairSection->Texture1);			
		}
		if(!pHairSection->Texture2.IsEmpty())
		{
			CSmartPtr<CD3DTexture> pTexture=pD3DDevice->GetTextureManager()->LoadTexture(pHairSection->Texture2);
			AddTexture(pSkinTexture,pTexture,128,64,0,192);
		}
		if(!pHairSection->Texture3.IsEmpty())
		{
			CSmartPtr<CD3DTexture> pTexture=pD3DDevice->GetTextureManager()->LoadTexture(pHairSection->Texture3);
			AddTexture(pSkinTexture,pTexture,128,32,0,160);
		}
	}

	CBLZWOWDatabase::BLZ_DB_CHAR_SECTION * pUnderwearSection=CBLZWOWDatabase::GetInstance()->FindCharSection(4,m_CharRace,m_CharSex,0,m_CharSkinColor);
	if(pUnderwearSection)
	{
		if(!pUnderwearSection->Texture1.IsEmpty())
		{
			CSmartPtr<CD3DTexture> pTexture=pD3DDevice->GetTextureManager()->LoadTexture(pUnderwearSection->Texture1);
			AddTexture(pSkinTexture,pTexture,128,64,128,96);
		}
		if(!pUnderwearSection->Texture2.IsEmpty())
		{
			CSmartPtr<CD3DTexture> pTexture=pD3DDevice->GetTextureManager()->LoadTexture(pUnderwearSection->Texture2);
			AddTexture(pSkinTexture,pTexture,128,64,128,0);
		}
	}	

	for(int i=0;i<CES_MAX;i++)
	{
		if(m_Equipments[i])
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(m_Equipments[i]);
			if(pItemData)
			{
				CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
					CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(pItemData->ItemDisplayInfo);
				if(pItemDisplayInfo)
				{		
					
					if(!pItemDisplayInfo->UpperArmTexture.IsEmpty()&&(i==CES_SHIRT||i==CES_BUST))
					{
						CEasyString TextureFileName=EQUIPMENT_ARM_UPPER_TEXTURE_PATH;						
						TextureFileName=TextureFileName+"\\"+pItemDisplayInfo->UpperArmTexture;						
						CSmartPtr<CD3DTexture> pTexture=LoadTextureBySex(pD3DDevice,TextureFileName,m_CharSex);
						AddTexture(pSkinTexture,pTexture,128,64,0,0);
					}
					if((!HaveSleeve)||i==CES_BUST)
					{
						if(!pItemDisplayInfo->LowerArmTexture.IsEmpty()&&(i==CES_WRIST||i==CES_SHIRT||i==CES_BUST||i==CES_HAND))
						{
							CEasyString TextureFileName=EQUIPMENT_ARM_LOWER_TEXTURE_PATH;
							TextureFileName=TextureFileName+"\\"+pItemDisplayInfo->LowerArmTexture;
							CSmartPtr<CD3DTexture> pTexture=LoadTextureBySex(pD3DDevice,TextureFileName,m_CharSex);
							AddTexture(pSkinTexture,pTexture,128,64,0,64);
						}
					}

					if(!pItemDisplayInfo->HandsTexture.IsEmpty()&&(i==CES_HAND))
					{
						CEasyString TextureFileName=EQUIPMENT_HAND_TEXTURE_PATH;
						TextureFileName=TextureFileName+"\\"+pItemDisplayInfo->HandsTexture;
						CSmartPtr<CD3DTexture> pTexture=LoadTextureBySex(pD3DDevice,TextureFileName,m_CharSex);
						AddTexture(pSkinTexture,pTexture,128,32,0,128);
					}

					if(!pItemDisplayInfo->UpperTorsoTexture.IsEmpty()&&(i==CES_BUST||i==CES_SHIRT))
					{
						CEasyString TextureFileName=EQUIPMENT_TORSO_UPPER_TEXTURE_PATH;						
						TextureFileName=TextureFileName+"\\"+pItemDisplayInfo->UpperTorsoTexture;						
						CSmartPtr<CD3DTexture> pTexture=LoadTextureBySex(pD3DDevice,TextureFileName,m_CharSex);
						AddTexture(pSkinTexture,pTexture,128,64,128,0);
					}
					
					if(!pItemDisplayInfo->LowerTorsoTexture.IsEmpty()&&(i==CES_BUST||i==CES_SHIRT))
					{
						CEasyString TextureFileName=EQUIPMENT_TORSO_LOWER_TEXTURE_PATH;						
						TextureFileName=TextureFileName+"\\"+pItemDisplayInfo->LowerTorsoTexture;						
						CSmartPtr<CD3DTexture> pTexture=LoadTextureBySex(pD3DDevice,TextureFileName,m_CharSex);
						AddTexture(pSkinTexture,pTexture,128,32,128,64);
					}

					
					if(!pItemDisplayInfo->UpperLegTexture.IsEmpty()&&(i==CES_BUST||i==CES_WAIST||i==CES_LEG))
					{
						CEasyString TextureFileName=EQUIPMENT_LEG_UPPER_TEXTURE_PATH;
						TextureFileName=TextureFileName+"\\"+pItemDisplayInfo->UpperLegTexture;
						CSmartPtr<CD3DTexture> pTexture=LoadTextureBySex(pD3DDevice,TextureFileName,m_CharSex);
						AddTexture(pSkinTexture,pTexture,128,64,128,96);
					}

					if(!pItemDisplayInfo->LowerLegTexture.IsEmpty()&&(i==CES_BUST||i==CES_LEG||i==CES_FOOT))
					{
						CEasyString TextureFileName=EQUIPMENT_LEG_LOWER_TEXTURE_PATH;
						TextureFileName=TextureFileName+"\\"+pItemDisplayInfo->LowerLegTexture;
						CSmartPtr<CD3DTexture> pTexture=LoadTextureBySex(pD3DDevice,TextureFileName,m_CharSex);
						AddTexture(pSkinTexture,pTexture,128,64,128,160);
					}
					

					if(!pItemDisplayInfo->FootTexture.IsEmpty()&&(i==CES_LEG||i==CES_FOOT))
					{
						CEasyString TextureFileName=EQUIPMENT_FOOT_TEXTURE_PATH;
						TextureFileName=TextureFileName+"\\"+pItemDisplayInfo->FootTexture;
						CSmartPtr<CD3DTexture> pTexture=LoadTextureBySex(pD3DDevice,TextureFileName,m_CharSex);
						AddTexture(pSkinTexture,pTexture,128,32,128,224);
					}
					
				}
			}
		}	
	}

	pCharSkinTexture=pSkinTexture;
	pSkinTexture.Detach();
	pCharSkinExtraTexture=pSkinExtraTexture;
	pSkinExtraTexture.Detach();
	pCharHairTexture=pHairTexture;
	pHairTexture.Detach();
	return true;

}

void CD3DWOWM2CharacterModel::AlphaMix(D3D_A8B8G8R8_PIXEL& DestPixel,D3D_A8B8G8R8_PIXEL& SrcPixel)
{
	DestPixel.Red=(DestPixel.Red*(255-SrcPixel.Alpha)+SrcPixel.Red*SrcPixel.Alpha)/255;
	DestPixel.Green=(DestPixel.Green*(255-SrcPixel.Alpha)+SrcPixel.Green*SrcPixel.Alpha)/255;
	DestPixel.Blue=(DestPixel.Blue*(255-SrcPixel.Alpha)+SrcPixel.Blue*SrcPixel.Alpha)/255;	
}

bool CD3DWOWM2CharacterModel::AddTexture(CD3DTexture * pDestTexture,CD3DTexture * pSrcTexture,UINT SrcWifth,UINT SrcHeight,UINT DestOffsetX,UINT DestOffsetY)
{
	if((pDestTexture->GetTextureInfo().Format==D3DFMT_X8B8G8R8||
		pDestTexture->GetTextureInfo().Format==D3DFMT_A8B8G8R8)&&
		pDestTexture->GetTextureInfo().Width==256&&
		pDestTexture->GetTextureInfo().Height==256)
	{
		int MipLevels=pDestTexture->GetTextureInfo().MipLevels;
		for(int i=0;i<MipLevels;i++)
		{
			D3DLOCKED_RECT DestLockRect;			
			if(pDestTexture->GetD3DTexture()->LockRect(i,&DestLockRect,NULL,0)==D3D_OK)
			{			

				MixTexture(pSrcTexture,i,SrcWifth,SrcHeight,(BYTE *)DestLockRect.pBits,DestLockRect.Pitch,DestOffsetX,DestOffsetY);

				pDestTexture->GetD3DTexture()->UnlockRect(i);
			}

		}

		return true;
	}	
	return false;
}

bool CD3DWOWM2CharacterModel::MixTexture(CD3DTexture * pSrcTexture,int MipLevel,UINT SrcWifth,UINT SrcHeight,BYTE * pDestPixels,UINT DestPitch,UINT DestOffsetX,UINT DestOffsetY)
{
	if(pSrcTexture!=NULL)
	{
		if(pSrcTexture->GetTextureInfo().Width==SrcWifth&&
			pSrcTexture->GetTextureInfo().Height==SrcHeight)
		{
			UINT OffsetX=(DestOffsetX>>MipLevel);
			UINT OffsetY=(DestOffsetY>>MipLevel);
			UINT Width=(SrcWifth>>MipLevel);
			UINT Height=(SrcHeight>>MipLevel);
			if(Width==0)
				Width=1;
			if(Height==0)
				Height=1;
			D3DLOCKED_RECT SrcLockRect;
			if(pSrcTexture->GetD3DTexture()->LockRect(MipLevel,&SrcLockRect,NULL,0)==D3D_OK)
			{								
				pDestPixels=pDestPixels+DestPitch*OffsetY;
				BYTE * pSrcPixels=(BYTE *)SrcLockRect.pBits;
				for(UINT y=0;y<Height;y++)
				{
					D3D_A8B8G8R8_PIXEL * pDestLine=(D3D_A8B8G8R8_PIXEL *)(pDestPixels+sizeof(D3D_A8B8G8R8_PIXEL)*OffsetX);
					D3D_A8B8G8R8_PIXEL * pSrcLine=(D3D_A8B8G8R8_PIXEL *)pSrcPixels;
					if(pSrcTexture->GetTextureInfo().Format==D3DFMT_X8B8G8R8)
					{
						memcpy(pDestLine,pSrcLine,sizeof(D3D_A8B8G8R8_PIXEL)*Width);
					}
					else if(pSrcTexture->GetTextureInfo().Format==D3DFMT_A8B8G8R8)
					{
						for(UINT x=0;x<Width;x++)
						{
							AlphaMix(pDestLine[x],pSrcLine[x]);
						}
					}								
					pDestPixels+=DestPitch;
					pSrcPixels+=SrcLockRect.Pitch;
				}							
				pSrcTexture->GetD3DTexture()->UnlockRect(MipLevel);
				return true;
			}
		}
	}
	return false;
}

CD3DTexture * CD3DWOWM2CharacterModel::LoadTextureBySex(CD3DDevice * pD3DDevice,LPCTSTR TextureFileName,int Sex)
{
	CEasyString FileName=TextureFileName;
	if(Sex==0)
		FileName+="_M.blp";
	else
		FileName+="_F.blp";
	CD3DTexture * pTexture=pD3DDevice->GetTextureManager()->LoadTexture(FileName);
	if(pTexture==NULL)
	{
		FileName=TextureFileName;
		FileName+="_U.blp";
		pTexture=pD3DDevice->GetTextureManager()->LoadTexture(FileName);
	}
	return pTexture;
}

void CD3DWOWM2CharacterModel::FetchAnimationFrames(UINT Time)
{
	CD3DWOWM2Model::FetchAnimationFrames(Time);
	if(m_Equipments[CES_LEFT_HAND])
	{
		for(UINT i=BONE_LFINGER1;i<=BONE_LTHUMB;i++)
		{
			int Bone=m_pModelResource->GetKeyBone(i);
			if(Bone>=0)
			{					
				m_pModelResource->MakeAnimationBoneFrameByTree(m_CloseHandAnimationIndex,32,Bone,					
					m_BoneMatrices.GetBuffer(),m_BoneMatrices.GetCount());
			}
		}		
	}
	if(m_Equipments[CES_RIGHT_HAND])
	{
		for(UINT i=BONE_RFINGER1;i<=BONE_RTHUMB;i++)
		{
			int Bone=m_pModelResource->GetKeyBone(i);
			if(Bone>=0)
			{
				m_pModelResource->MakeAnimationBoneFrameByTree(m_CloseHandAnimationIndex,32,Bone,				
					m_BoneMatrices.GetBuffer(),m_BoneMatrices.GetCount());
			}
		}		
	}
}


//CNameObject::STORAGE_STRUCT * CD3DWOWM2CharacterModel::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//int CD3DWOWM2CharacterModel::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int HeadSize=CD3DWOWM2Model::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//	
//	pLocalHead->MaterialCount=(int)m_SubMeshMaterialList.GetCount();
//	pLocalHead->Size+=sizeof(UINT)*pLocalHead->MaterialCount;
//
//	pLocalHead->CharRace			=m_CharRace;
//	pLocalHead->CharSex				=m_CharSex;
//	pLocalHead->CharSexMax			=m_CharSexMax;
//	pLocalHead->CharSkinColor		=m_CharSkinColor;
//	pLocalHead->CharSkinColorMax	=m_CharSkinColorMax;
//	pLocalHead->CharHairColor		=m_CharHairColor;
//	pLocalHead->CharHairColorMax	=m_CharHairColorMax;
//	pLocalHead->CharFaceType		=m_CharFaceType;
//	pLocalHead->CharFaceTypeMax		=m_CharFaceTypeMax;
//	pLocalHead->CharHairType		=m_CharHairType;
//	pLocalHead->CharHairTypeMax		=m_CharHairTypeMax;
//	pLocalHead->CharWhiskerType		=m_CharWhiskerType;
//	pLocalHead->CharWhiskerTypeMax	=m_CharWhiskerTypeMax;
//	pLocalHead->IsCharBald			=m_IsCharBald;
//
//	memcpy(pLocalHead->Equipments,m_Equipments,sizeof(m_Equipments));
//
//	pLocalHead->HelmetModelStorageID=0;
//	pLocalHead->LeftShoulderModelStorageID=0;
//	pLocalHead->RightShoulderModelStorageID=0;
//	pLocalHead->LeftWeaponModelStorageID=0;
//	pLocalHead->RightWeaponModelStorageID=0;
//	if(m_pHelmetModel)
//	{
//		m_pHelmetModel->RefreshStorageID();
//		pLocalHead->HelmetModelStorageID=m_pHelmetModel->GetStorageID();
//	}
//	if(m_pLeftShoulderModel)
//	{
//		m_pLeftShoulderModel->RefreshStorageID();
//		pLocalHead->LeftShoulderModelStorageID=m_pLeftShoulderModel->GetStorageID();
//	}
//	if(m_pRightShoulderModel)
//	{
//		m_pRightShoulderModel->RefreshStorageID();
//		pLocalHead->RightShoulderModelStorageID=m_pRightShoulderModel->GetStorageID();
//	}
//	if(m_pLeftWeaponModel)
//	{
//		m_pLeftWeaponModel->RefreshStorageID();
//		pLocalHead->LeftWeaponModelStorageID=m_pLeftWeaponModel->GetStorageID();
//	}
//	if(m_pRightWeaponModel)
//	{
//		m_pRightWeaponModel->RefreshStorageID();
//		pLocalHead->RightWeaponModelStorageID=m_pRightWeaponModel->GetStorageID();
//	}
//	return sizeof(STORAGE_STRUCT);
//}
//
//bool CD3DWOWM2CharacterModel::USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	if(!CD3DWOWM2Model::USOWriteData(pHead,pUSOFile,Param))
//		return false;
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//
//	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
//	{
//		UINT StorageID=m_SubMeshList[i]->GetStorageID();
//		if(pFile->Write(&StorageID,sizeof(StorageID))<sizeof(StorageID))
//			return false;
//	}	
//
//	return true;
//}
//
//bool CD3DWOWM2CharacterModel::USOWriteChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	if(!CD3DWOWM2Model::USOWriteChild(pHead,pUSOFile,Param))
//		return false;
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	for(UINT i=0;i<m_SubMeshMaterialList.GetCount();i++)
//	{
//		if(!m_SubMeshMaterialList[i].ToUSOFile(pUSOFile,(UINT)(GetDevice())))
//			return false;		
//	}
//
//	return true;
//}
//
//int CD3DWOWM2CharacterModel::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int ReadSize=CD3DWOWM2Model::USOReadHead(pHead,pUSOFile,Param);
//	if(ReadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	
//	m_CharRace				=pLocalHead->CharRace			;
//	m_CharSex				=pLocalHead->CharSex			;
//	m_CharSexMax			=pLocalHead->CharSexMax			;
//	m_CharSkinColor			=pLocalHead->CharSkinColor		;
//	m_CharSkinColorMax		=pLocalHead->CharSkinColorMax	;
//	m_CharHairColor			=pLocalHead->CharHairColor		;
//	m_CharHairColorMax		=pLocalHead->CharHairColorMax	;
//	m_CharFaceType			=pLocalHead->CharFaceType		;
//	m_CharFaceTypeMax		=pLocalHead->CharFaceTypeMax	;
//	m_CharHairType			=pLocalHead->CharHairType		;
//	m_CharHairTypeMax		=pLocalHead->CharHairTypeMax	;
//	m_CharWhiskerType		=pLocalHead->CharWhiskerType	;
//	m_CharWhiskerTypeMax	=pLocalHead->CharWhiskerTypeMax	;
//	m_IsCharBald			=pLocalHead->IsCharBald			;
//
//	memcpy(m_Equipments,pLocalHead->Equipments,sizeof(m_Equipments));
//	return sizeof(STORAGE_STRUCT);
//}
//
//int CD3DWOWM2CharacterModel::USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param)
//{
//	int ReadSize=CD3DWOWM2Model::USOReadData(pHead,pUSOFile,pData,DataSize,Param);
//
//	pData+=ReadSize;
//	DataSize-=ReadSize;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	UINT * pSubMeshIDs=(UINT *)pData;
//	ReadSize+=sizeof(UINT)*pLocalHead->MaterialCount;
//	m_SubMeshList.Resize(pLocalHead->MaterialCount);
//	for(int i=0;i<pLocalHead->MaterialCount;i++)
//	{
//		m_SubMeshList[i]=(CD3DSubMesh *)pSubMeshIDs[i];
//	}
//
//	return ReadSize;
//}
//
//bool CD3DWOWM2CharacterModel::USOReadChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{	
//	if(!CD3DWOWM2Model::USOReadChild(pHead,pUSOFile,Param))
//		return false;
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	
//	m_SubMeshMaterialList.Resize(pLocalHead->MaterialCount);
//	for(int i=0;i<pLocalHead->MaterialCount;i++)
//	{		
//		if(!m_SubMeshMaterialList[i].FromUSOFile(pUSOFile,(UINT)(GetDevice())))
//		{			
//			return false;
//		}
//	}	
//	return true;
//}
//
//bool CD3DWOWM2CharacterModel::USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param)
//{
//	if(!CD3DWOWM2Model::USOReadFinish(pHead,Param))
//		return false;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	if(m_pModelResource)
//	{
//		for(UINT i=0;i<m_SubMeshList.GetCount();i++)
//		{
//			UINT StorageID=(UINT)m_SubMeshList[i];
//			m_SubMeshList[i]=NULL;
//			for(int j=0;j<m_pModelResource->GetSubMeshCount();j++)
//			{
//				if(m_pModelResource->GetSubMesh(j)->GetStorageID()==StorageID)
//				{
//					m_SubMeshList[i]=m_pModelResource->GetSubMesh(j);
//					break;
//				}
//			}
//			if(m_SubMeshList[i]==NULL)
//				return false;
//		}
//	}
//	else
//	{
//		m_SubMeshList.Clear();
//	}
//
//	if(pLocalHead->HelmetModelStorageID)
//		m_pHelmetModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(pLocalHead->HelmetModelStorageID);
//
//	if(pLocalHead->LeftShoulderModelStorageID)
//		m_pLeftShoulderModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(pLocalHead->LeftShoulderModelStorageID);
//
//	if(pLocalHead->RightShoulderModelStorageID)
//		m_pRightShoulderModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(pLocalHead->RightShoulderModelStorageID);
//
//	if(pLocalHead->LeftWeaponModelStorageID)
//		m_pLeftWeaponModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(pLocalHead->LeftWeaponModelStorageID);
//
//	if(pLocalHead->RightWeaponModelStorageID)
//		m_pRightWeaponModel=(CD3DWOWM2ItemModel *)GetChildByStorageIDRecursive(pLocalHead->RightWeaponModelStorageID);
//	
//	return true;
//}

}