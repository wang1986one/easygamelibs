/****************************************************************************/
/*                                                                          */
/*      文件名:    BLZWOWDatabase.h                                         */
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

class CBLZWOWDatabase :
	public CStaticObject<CBLZWOWDatabase>
{
public:
	struct BLZ_DB_CHAR_SECTION
	{
		UINT32		ID;
		int			Race;
		BYTE		Sex;
		BYTE		GeneralType;
		BYTE		Type;
		BYTE		Variation;
		UINT32		Flags;
		CEasyString	Texture1;
		CEasyString	Texture2;
		CEasyString	Texture3;		
	};
	struct BLZ_DB_ANIMATION_DATA_RECORD
	{
		UINT32		ID;
		CEasyString	Name;
		UINT32		WeaponState;
		UINT32		Flags;		
		UINT32		Preceeding;
		UINT32		RealId;
		UINT32		Group;
	};

	struct BLZ_DB_CHAR_HAIR_SUBMESH_INFO
	{		
		BYTE	Race;
		BYTE	Sex;
		BYTE	HairType;
		BYTE	SubMeshID;
		BYTE	IsBald;
	};

	struct BLZ_DB_CHAR_WHISKER_SUBMESH_INFO
	{	
		BYTE	Race;
		BYTE	Sex;
		BYTE	WhiskerType;	
		BYTE	SubMeshID1;		
		BYTE	SubMeshID2;
		BYTE	SubMeshID3;
	};

	struct BLZ_DB_CHAR_RACE_INFO
	{	
		UINT32			RaceID;
		UINT32			Flags;				
		UINT32			FactionID;			
		UINT32			Exploration;		
		UINT32			MaleModelID;			
		UINT32			FemaleModelID;		
		CEasyString		Abbrev;				
		UINT32			Faction;			
		UINT32			CreatureType;		
		CEasyString		InternalName;		
		UINT32			Camera;				
		CEasyString		RaceName;
		CEasyString		Feature1;			
		CEasyString		Feature2;			
		CEasyString		Feature3;			
		UINT32			Expansion;			

	};
	struct BLZ_DB_CREATURE_DISPLAY_INFO
	{
		UINT32			ID;							
		UINT32			ModelID;						
		UINT32			SoundID;						
		UINT32			ExtraDisplayInformationID;	
		FLOAT			Scale;						
		UINT32			Opacity;					
		CEasyString		Skin1;						
		CEasyString		Skin2;						
		CEasyString		Skin3;						
		CEasyString		Icon;						
		UINT32			BloodLevelID;					
		UINT32			BloodID;						
		UINT32			NPCSoundID;					
		UINT32			ParticleID;					
		UINT32			CreatureGeosetDataID;			
		UINT32			ObjectEffectPackageID;		
	};
	struct BLZ_DB_CREATURE_MODEL_INFO
	{
		UINT32			ID;							
		UINT32			Flags;						
		CEasyString		ModelPath;					
		CEasyString		AltermateModel;				
		FLOAT			Scale;						
		UINT32			BloodLevelID;					
		UINT32			SoundDataID;					
		FLOAT			CollisionWidth; 			
		FLOAT			CollisionHeight;			
	};

	struct BLZ_DB_ITEM_DATA
	{
		UINT			ItemID;			
		UINT			ItemClass;		
		UINT			ItemSubClass;	
		int				MaterialID;		
		UINT			ItemDisplayInfo;
		UINT			InventorySlotID;
		UINT			SheathID;	
		UINT 			Type;
		UINT 			SheathType;
		UINT 			Quality;
		CEasyString 	Name;
	};

	struct BLZ_DB_ITEM_SUB_CLASS
	{		
		UINT			ClassID;
		UINT			SubClassID;		
		UINT			HandsNumber;
		CEasyString		Name;
		CEasyString		UniquePlural;

		CEasyArray<BLZ_DB_ITEM_DATA *>	ItemInfo;
	};


	struct BLZ_DB_ITEM_CLASS
	{
		UINT			ClassID;
		UINT			SubClassID;
		bool			IsWeapon;
		CEasyString		Name;
		CEasyArray<BLZ_DB_ITEM_SUB_CLASS>	SubClassInfo;
	};

	

	struct BLZ_DB_ITEM_SUB_CLASS_MASK
	{
		UINT32			ClassID;
		UINT32			SubClassIDMask;
		CEasyString		Name;
	};

	

	struct BLZ_DB_ITEM_DISPLAY_INFO
	{
		UINT32			ID;								
		CEasyString		LeftModel;						
		CEasyString		RightModel;						
		CEasyString		LeftModelTexture;				
		CEasyString		RightModelTexture;				
		CEasyString		Icon;							
		CEasyString		Texture;						
		UINT32			GloveGeosetFlags;				
		UINT32			PantsGeosetFlags;				
		UINT32			RobeGeosetFlags;				
		UINT32			BootsGeosetFlags;
		UINT32			UnkownGeosetFlags;
		UINT32			ItemGroupSounds;				
		UINT32			HelmetGeosetVisData1;			
		UINT32			HelmetGeosetVisData2;			
		CEasyString		UpperArmTexture;				
		CEasyString		LowerArmTexture;				
		CEasyString		HandsTexture;					
		CEasyString		UpperTorsoTexture;				
		CEasyString		LowerTorsoTexture;				
		CEasyString		UpperLegTexture;				
		CEasyString		LowerLegTexture;				
		CEasyString		FootTexture;					
		UINT32			ItemVisuals;					

	};

	struct BLZ_DB_HELMET_GEOSET_VISIBLE_INFO
	{
		UINT	ID;						
		int		HairVisible;				
		int		Facial1Visible;			
		int		Facial2Visible;			
		int		Facial3Visible;			
		int		EarsVisible;
	};

	struct BLZ_DB_MAP_INFO
	{
		UINT		ID;
		CEasyString	InternalName;							
		UINT		AreaType;								
		BOOL		IsBattleground;							
		CEasyString	Name;			
		UINT		AreaTable;								
		CEasyString	AllianceDesc;	
		CEasyString	HordeDesc;		
		UINT		LoadingScreen;							
		FLOAT		BattlefieldMapIconScale;				
		CEasyString	Requirement;		
		CEasyString	Heroic;			
		UINT		ParentArea;								
		FLOAT		XCoord;									
		FLOAT		YCoord;									
		UINT		ResetTimeRaid;							
		UINT		ResetTimeHeroicRaid;							
		UINT		TimeOfDayOverride;						
		UINT		ExpansionID;							

	};



protected:
	

	CEasyArray<BLZ_DB_CHAR_SECTION>								m_CharSectionData;
	int															m_CharRaceMax;

	CEasyArray<BLZ_DB_ANIMATION_DATA_RECORD>					m_AnimationData;

	CEasyArray<BLZ_DB_CHAR_HAIR_SUBMESH_INFO>					m_CharHairSubMeshInfo;

	CEasyArray<BLZ_DB_CHAR_WHISKER_SUBMESH_INFO>				m_CharWhiskerSubMeshInfo;

	CEasyArray<BLZ_DB_CHAR_RACE_INFO>							m_CharRaceInfo;
	CEasyArray<BLZ_DB_CREATURE_DISPLAY_INFO>					m_CreatureDisplayInfo;
	CEasyArray<BLZ_DB_CREATURE_MODEL_INFO>						m_CreatureModelInfo;

	CEasyArray<BLZ_DB_ITEM_CLASS>								m_ItemClass;
	
	CEasyArray<BLZ_DB_ITEM_SUB_CLASS_MASK>						m_ItemSubClassMask;


	CEasyMap<UINT,BLZ_DB_ITEM_DATA>								m_ItemData;
	CEasyMap<UINT,BLZ_DB_ITEM_DISPLAY_INFO>						m_ItemDisplayInfo;

	CEasyMap<UINT,BLZ_DB_HELMET_GEOSET_VISIBLE_INFO>			m_HelmetGeosetVisibleInfo;

	CEasyArray<BLZ_DB_MAP_INFO>									m_MaoInfo;

	CBLZDBCFile													m_SpellVisualEffectNameInfo;

public:
	CBLZWOWDatabase(void);
	~CBLZWOWDatabase(void);

	bool LoadDBCs(LPCTSTR DBCPath,CEasyString& ErrorMsg);
	bool Save(LPCTSTR szFileName);
	bool Load(LPCTSTR szFileName);

	bool LoadCharSectionData(LPCTSTR FileName);
	int GetMaxCharRace();
	int GetMaxCharSex(int Race);
	void GetCharMaxInfo(int Race,int Sex,int& SkinColor,int& HairColor,int& FaceType,int& HairType,int& WhiskerType);

	BLZ_DB_CHAR_SECTION * FindCharSection(UINT GeneralType,UINT Race,UINT Sex,UINT Type,UINT Color);


	bool LoadAnimationData(LPCTSTR FileName);

	BLZ_DB_ANIMATION_DATA_RECORD * FindAnimationData(UINT AnimationID);

	bool LoadCharHairSubMeshInfo(LPCTSTR FileName);

	int FindCharHairSubMesh(UINT Race,UINT Sex,UINT HairType,bool IsCharBald);

	bool LoadCharWhiskerSubMeshInfo(LPCTSTR FileName);

	void FindCharWhiskerSubMesh(UINT Race,UINT Sex,UINT WhiskerType,int& SubMeshID1,int& SubMeshID2,int& SubMeshID3);

	bool LoadCharRaceInfo(LPCTSTR FileName);

	BLZ_DB_CHAR_RACE_INFO * FindRaceInfo(UINT Race);

	bool LoadCreatureDisplayInfo(LPCTSTR FileName);

	BLZ_DB_CREATURE_DISPLAY_INFO * FindCreatureDisplayInfo(UINT ID);

	bool LoadCreatureModelInfo(LPCTSTR FileName);

	BLZ_DB_CREATURE_MODEL_INFO * FindCreatureModelInfo(UINT ID);

	bool FindCharModelInfo(UINT Race,UINT Sex,CEasyString& ModelFileName,CEasyString& SkinFileName);

	bool LoadItemClass(LPCTSTR FileName);
	bool LoadItemSubClass(LPCTSTR FileName);
	bool LoadItemSubClassMask(LPCTSTR FileName);

	UINT GetItemClassCount();
	BLZ_DB_ITEM_CLASS * GetItemClassInfo(UINT ClassID);
	BLZ_DB_ITEM_CLASS * GetItemClassInfoByIndex(UINT Index);
	BLZ_DB_ITEM_SUB_CLASS * GetItemSubClassInfo(UINT ClassID,UINT SubClassID);
	UINT GetItemSubClassMaskCount();

	bool LoadItemData(LPCTSTR FileName);

	bool LoadItemCacheData(LPCTSTR FileName);

	bool LoadItemDisplayInfo(LPCTSTR FileName);

	BLZ_DB_ITEM_DATA * GetItemData(UINT ItemID);

	BLZ_DB_ITEM_DISPLAY_INFO * GetItemDisplayInfo(UINT ItemDisplayID);

	bool LoadHelmetGeosetVisibleInfo(LPCTSTR FileName);

	BLZ_DB_HELMET_GEOSET_VISIBLE_INFO * GetHelmetGeosetVisibleInfo(UINT ID);

	bool LoadMapInfo(LPCTSTR FileName);

	UINT GetMapInfoCount();
	BLZ_DB_MAP_INFO * GetMapInfo(UINT Index);
	

protected:
	CEasyString UTF8ToLocal(LPCTSTR szStr,int StrLen);

	
};

}