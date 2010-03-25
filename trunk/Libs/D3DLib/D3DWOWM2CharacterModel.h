/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWM2CharacterModel.h                                 */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

#pragma comment(linker,"/include:?m_CD3DWOWM2CharacterModelClassInfoRegister@CD3DWOWM2CharacterModel@D3DLib@@1VCClassInfoRegister@@A")

class CD3DWOWM2ItemModel;

class CD3DWOWM2CharacterModel :
	public CD3DWOWM2Model
{
public:
	enum CHAR_EQUIPMENT_SLOT
	{
		CES_HEAD,
		CES_SHOULDER,
		CES_LEFT_HAND,
		CES_RIGHT_HAND,
		CES_BACK,	
		CES_SHIRT,		
		CES_LEG,
		CES_FOOT,
		CES_BUST,
		CES_WRIST,
		CES_HAND,		
		CES_WAIST,
		CES_ENSIGN,
		CES_MAX,
	};
protected:	
	//struct STORAGE_STRUCT:CD3DWOWM2Model::STORAGE_STRUCT
	//{
	//	int				MaterialCount;
	//	int				CharRace;
	//	int				CharSex;
	//	int				CharSexMax;
	//	int				CharSkinColor;
	//	int				CharSkinColorMax;
	//	int				CharHairColor;
	//	int				CharHairColorMax;
	//	int				CharFaceType;
	//	int				CharFaceTypeMax;
	//	int				CharHairType;
	//	int				CharHairTypeMax;
	//	int				CharWhiskerType;
	//	int				CharWhiskerTypeMax;
	//	bool			IsCharBald;
	//	UINT			Equipments[CES_MAX];
	//	UINT			HelmetModelStorageID;
	//	UINT			LeftShoulderModelStorageID;
	//	UINT			RightShoulderModelStorageID;
	//	UINT			LeftWeaponModelStorageID;
	//	UINT			RightWeaponModelStorageID;

	//};

	enum SST_MEMBER_ID
	{
		SST_D3DWMCM_CHAR_ACITVE_SUB_MESH=SST_D3DWMM_MAX,
		SST_D3DWMCM_CHAR_MATERIAL,
		SST_D3DWMCM_CHAR_RACE,
		SST_D3DWMCM_CHAR_SEX,
		SST_D3DWMCM_CHAR_SKIN_COLOR,
		SST_D3DWMCM_CHAR_HAIR_COLOR,
		SST_D3DWMCM_CHAR_FACE_TYPE,
		SST_D3DWMCM_CHAR_HAIR_TYPE,
		SST_D3DWMCM_CHAR_WHISKER_TYPE,
		SST_D3DWMCM_CHAR_IS_BALD,
		SST_D3DWMCM_CHAR_EQUIPMENTS,
		SST_D3DWMCM_CHAR_HELMET_MODEL,
		SST_D3DWMCM_CHAR_LEFT_SHOULDER_MODEL,
		SST_D3DWMCM_CHAR_RIGHT_SHOULDER_MODEL,
		SST_D3DWMCM_CHAR_LEFT_WEAPON_MODEL,
		SST_D3DWMCM_CHAR_RIGHT_WEAPON_MODEL,		
		SST_D3DWMCM_MAX=SST_D3DWMM_MAX+50,
	};

	CEasyArray<CD3DSubMesh *>					m_SubMeshList;
	CEasyArray<CD3DSubMeshMaterial>				m_SubMeshMaterialList;

	int								m_CharRace;
	int								m_CharSex;
	int								m_CharSexMax;
	int								m_CharSkinColor;
	int								m_CharSkinColorMax;
	int								m_CharHairColor;
	int								m_CharHairColorMax;
	int								m_CharFaceType;
	int								m_CharFaceTypeMax;
	int								m_CharHairType;
	int								m_CharHairTypeMax;
	int								m_CharWhiskerType;
	int								m_CharWhiskerTypeMax;
	bool							m_IsCharBald;

	UINT							m_Equipments[CES_MAX];

	CD3DWOWM2ItemModel *			m_pHelmetModel;
	CD3DWOWM2ItemModel *			m_pLeftShoulderModel;
	CD3DWOWM2ItemModel *			m_pRightShoulderModel;
	CD3DWOWM2ItemModel *			m_pLeftWeaponModel;
	CD3DWOWM2ItemModel *			m_pRightWeaponModel;

	int								m_CloseHandAnimationIndex;
	
	DECLARE_CLASS_INFO(CD3DWOWM2CharacterModel)
public:
	CD3DWOWM2CharacterModel(void);
	~CD3DWOWM2CharacterModel(void);

	virtual void Destory();
	void DestoryModel();

	virtual bool Reset();
	virtual bool Restore();

	bool SetCharRace(int Value);
	bool SetCharSex(int Value);
	bool SetCharSkinColor(int Value);
	bool SetCharHairColor(int Value);
	bool SetCharFaceType(int Value);
	bool SetCharHairType(int Value);
	bool SetCharWhiskerType(int Value);
	void SetCharBald(bool IsCharBald);

	int GetCharRace();
	int GetCharSex();
	int GetCharSkinColor();
	int GetCharHairColor();
	int GetCharFaceType();
	int GetCharHairType();
	int GetCharWhiskerType();
	bool IsCharBald();

	bool SetEquipment(UINT Slot,UINT ItemID);
	UINT GetEquipment(UINT Slot);

	bool BuildCharModel();

	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(int index);
	virtual CD3DSubMeshMaterial * GetSubMeshMaterial(int index);

	virtual bool CloneFrom(CNameObject * pObject,UINT Param=0);
	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);

protected:
	bool MakeCharSkinTexture(CD3DDevice * pD3DDevice,CD3DTexture *& pCharSkinTexture,CD3DTexture *& pCharSkinExtraTexture,CD3DTexture *& pCharHairTexture,bool HaveSleeve);
	void AlphaMix(D3D_A8B8G8R8_PIXEL& DestPixel,D3D_A8B8G8R8_PIXEL& SrcPixel);
	bool AddTexture(CD3DTexture * pDestTexture,CD3DTexture * pSrcTexture,UINT SrcWifth,UINT SrcHeight,UINT DestOffsetX,UINT DestOffsetY);
	bool MixTexture(CD3DTexture * pSrcTexture,int MipLevel,UINT SrcWifth,UINT SrcHeight,BYTE * pDestPixels,UINT DestPitch,UINT DestOffsetX,UINT DestOffsetY);
	CD3DTexture * LoadTextureBySex(CD3DDevice * pD3DDevice,LPCTSTR TextureFileName,int Sex);
	virtual void FetchAnimationFrames(UINT Time);

	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);	
	//virtual bool USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual bool USOWriteChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual int USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param=0);
	//virtual bool USOReadChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual bool USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param=0);
};

inline void CD3DWOWM2CharacterModel::SetCharBald(bool IsCharBald)
{
	m_IsCharBald=IsCharBald;
}

inline int CD3DWOWM2CharacterModel::GetCharRace()
{
	return m_CharRace;
}
inline int CD3DWOWM2CharacterModel::GetCharSex()
{
	return m_CharSex;
}
inline int CD3DWOWM2CharacterModel::GetCharSkinColor()
{
	return m_CharSkinColor;
}
inline int CD3DWOWM2CharacterModel::GetCharHairColor()
{
	return m_CharHairColor;
}
inline int CD3DWOWM2CharacterModel::GetCharFaceType()
{
	return m_CharFaceType;
}
inline int CD3DWOWM2CharacterModel::GetCharHairType()
{
	return m_CharHairType;
}
inline int CD3DWOWM2CharacterModel::GetCharWhiskerType()
{
	return m_CharWhiskerType;
}
inline bool CD3DWOWM2CharacterModel::IsCharBald()
{
	return m_IsCharBald;
}

inline UINT CD3DWOWM2CharacterModel::GetEquipment(UINT Slot)
{
	if(Slot<CES_MAX)
	{
		return m_Equipments[Slot];
	}
	return 0;
}

}