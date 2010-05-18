/****************************************************************************/
/*                                                                          */
/*      文件名:    BLZWOWDatabase.cpp                                       */
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


LPCTSTR CHAR_ATTACHMENT_NAME[CAI_MAX]={
	"LeftWrist",
	"RightPalm1",
	"LeftPalm1",
	"RightElbow",
	"LeftElbow",
	"RightShoulder",
	"LeftShoulder",
	"RightKnee",
	"LeftKnee",
	"RightWaist",
	"LeftFrontWaist",
	"Helmet",
	"Back",
	"InRightShoulder",
	"LeftUpperArm",
	"Bust1",
	"Bust2",
	"Face",
	"AboveCharacter",
	"Ground",
	"TopOfHead",
	"LeftPalm2",
	"RightPalm2",
	"-",
	"-",
	"-",
	"RightBackSheath",
	"LeftBackSheath",
	"MiddleBackSheath",
	"Belly",
	"LeftBack",
	"RightBack",
	"LeftHipSheath",
	"RightHipSheath",
	"Bust3",
	"RightPalm3",
	"RightPalm4",
	"LeftPalm3",
	"LeftPalm4",
};

LPCTSTR ITEM_INVENTORY_SLOT_NAME[IISI_MAX]={"无","头部","颈部","肩膀","衬衣","内衣","腰带","腿部","鞋","手腕","手套",
"戒指","饰品","单手","盾牌","弓","背部","双手","背包","大衣","长袍","主手","副手","副手物品","弹药","投掷武器",
"未知","未知","纪念品"};

LPCTSTR ITEM_PATH_BY_SLOT[IISI_MAX]=
{
	"IISI_NONE",
	"ITEM\\OBJECTCOMPONENTS\\HEAD",
	"IISI_NECK",
	"ITEM\\OBJECTCOMPONENTS\\SHOULDER",
	"IISI_SHIRT",
	"IISI_VEST",
	"IISI_WAIST",
	"IISI_LEGS",
	"IISI_FEET",
	"IISI_WRIST",
	"IISI_HANDS",
	"IISI_RING",
	"IISI_TRINKET",
	"ITEM\\OBJECTCOMPONENTS\\WEAPON",
	"ITEM\\OBJECTCOMPONENTS\\SHIELD",
	"ITEM\\OBJECTCOMPONENTS\\WEAPON",
	"ITEM\\OBJECTCOMPONENTS\\CAPE",
	"ITEM\\OBJECTCOMPONENTS\\WEAPON",
	"ITEM\\OBJECTCOMPONENTS\\Quiver",
	"IISI_TABARD",
	"IISI_ROBE",
	"ITEM\\OBJECTCOMPONENTS\\WEAPON",
	"ITEM\\OBJECTCOMPONENTS\\WEAPON",
	"IISI_HELD",
	"ITEM\\OBJECTCOMPONENTS\\AMMO",
	"ITEM\\OBJECTCOMPONENTS\\WEAPON",
	"ITEM\\OBJECTCOMPONENTS\\WEAPON",
	"IISI_RANGED2",
	"IISI_RELIC",
};


CBLZWOWDatabase::CBLZWOWDatabase(void)
{
	m_CharRaceMax=0;
}

CBLZWOWDatabase::~CBLZWOWDatabase(void)
{
	//PrintSystemLog(0,"卸载CharSectionData");
	m_CharSectionData.Clear();

	//PrintSystemLog(0,"卸载AnimationData");
	m_AnimationData.Clear();

	//PrintSystemLog(0,"卸载CharHairSubMeshInfo");
	m_CharHairSubMeshInfo.Clear();

	//PrintSystemLog(0,"卸载CharWhiskerSubMeshInfo");
	m_CharWhiskerSubMeshInfo.Clear();

	//PrintSystemLog(0,"卸载CharRaceInfo");
	m_CharRaceInfo.Clear();

	//PrintSystemLog(0,"卸载CreatureDisplayInfo");
	m_CreatureDisplayInfo.Clear();

	//PrintSystemLog(0,"卸载CreatureModelInfo");
	m_CreatureModelInfo.Clear();

	//PrintSystemLog(0,"卸载ItemClass");
	m_ItemClass.Clear();

	//PrintSystemLog(0,"卸载ItemSubClassMask");
	m_ItemSubClassMask.Clear();

	//PrintSystemLog(0,"卸载ItemData");
	m_ItemData.Clear();

	//PrintSystemLog(0,"卸载ItemDisplayInfo");
	m_ItemDisplayInfo.Clear();

	//PrintSystemLog(0,"卸载HelmetGeosetVisibleInfo");
	m_HelmetGeosetVisibleInfo.Clear();

	//PrintSystemLog(0,"CBLZWOWDatabase卸载完毕");

	m_MaoInfo.Clear();
}

bool CBLZWOWDatabase::LoadDBCs(LPCTSTR DBCPath,CEasyString& ErrorMsg)
{
	m_CharSectionData.Clear();

	m_AnimationData.Clear();

	m_CharHairSubMeshInfo.Clear();

	m_CharWhiskerSubMeshInfo.Clear();

	m_CharRaceInfo.Clear();
	m_CreatureDisplayInfo.Clear();
	m_CreatureModelInfo.Clear();

	m_ItemClass.Clear();
	m_ItemSubClassMask.Clear();
	m_ItemData.Clear();

	m_ItemDisplayInfo.Clear();
	m_HelmetGeosetVisibleInfo.Clear();

	m_MaoInfo.Clear();

	CEasyString FileName;
	UINT LoadTime;

	
	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_CHAR_SECTION_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadCharSectionData(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_ANIMATION_DATA_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadAnimationData(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_CHAR_HAIR_GEOSET_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadCharHairSubMeshInfo(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_CHAR_WHISKER_GEOSET_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadCharWhiskerSubMeshInfo(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_CHAR_RACE_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadCharRaceInfo(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_CREATURE_DISPLAY_INFO_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadCreatureDisplayInfo(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_CREATURE_MODEL_INFO_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadCreatureModelInfo(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_ITEM_CLASS_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadItemClass(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_ITEM_SUB_CLASS_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadItemSubClass(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_ITEM_SUB_CLASS_MASK_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadItemSubClassMask(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_ITEM_DATA_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadItemData(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_ITEM_CACHE_DATA_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadItemCacheData(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_ITEM_DISPLAY_INFO_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadItemDisplayInfo(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_HELMET_GEOSET_VISIBLE_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadHelmetGeosetVisibleInfo(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	FileName=DBCPath;
	FileName+="\\";
	FileName+=BLZ_DBC_MAP_FILE_NAME;
	PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	LoadTime=CEasyTimer::GetTime();
	if(!LoadMapInfo(FileName))
	{
		ErrorMsg=FileName+"装载失败！";
		return false;
	}
	PrintSystemLog(0,"装载%s花了%u毫秒",
		(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	//FileName=DBCPath;
	//FileName+="\\";
	//FileName+=BLZ_DBC_SPELL_VISUAL_EFFECT_NAME_FILE_NAME;
	//PrintSystemLog(0,"装载%s",(LPCTSTR)FileName);
	//LoadTime=CEasyTimer::GetTime();
	//if(!m_SpellVisualEffectNameInfo.Load(FileName,
	//	BLZ_DBC_SPELL_VISUAL_EFFECT_NAME_RECORD_SIZE))
	//{
	//	ErrorMsg=FileName+"装载失败！";
	//	return false;
	//}
	//PrintSystemLog(0,"装载%s花了%u毫秒",
	//	(LPCTSTR)FileName,GetTimeToTime(LoadTime,CEasyTimer::GetTime()));

	PrintSystemLog(0,"CBLZWOWDatabase装载完毕");

	return true;
}

bool CBLZWOWDatabase::Save(LPCTSTR szFileName)
{
	IFileAccessor * pFile;

	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(szFileName,IFileAccessor::modeCreateAlways|IFileAccessor::modeWrite))
	{
		pFile->Release();
		return false;	
	}

	return true;
}
bool CBLZWOWDatabase::Load(LPCTSTR szFileName)
{
	return true;
}

bool CBLZWOWDatabase::LoadCharSectionData(LPCTSTR FileName)
{	
	IFileAccessor * pFile;

	CEasyBuffer CharSectionData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	CharSectionData.Create(FileSize);
	pFile->Read(CharSectionData.GetBuffer(),FileSize);
	CharSectionData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)CharSectionData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_CHAR_SECTION_RECORD_SIZE)
		return false;


	DBC_CHAR_SECTION_RECORD * pRecord=(DBC_CHAR_SECTION_RECORD *)((BYTE *)CharSectionData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)CharSectionData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);



	std::set<UINT> RaceSet;

	m_CharSectionData.Resize(pHeader->RecordCount);
	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		m_CharSectionData[i].ID=pRecord[i].ID;
		m_CharSectionData[i].Race=pRecord[i].Race;
		m_CharSectionData[i].Sex=pRecord[i].Sex;
		m_CharSectionData[i].GeneralType=pRecord[i].GeneralType;
		m_CharSectionData[i].Texture1=pStringTable+pRecord[i].Texture1;
		m_CharSectionData[i].Texture2=pStringTable+pRecord[i].Texture2;
		m_CharSectionData[i].Texture3=pStringTable+pRecord[i].Texture3;
		m_CharSectionData[i].Flags=pRecord[i].Flags;
		m_CharSectionData[i].Type=pRecord[i].Type;
		m_CharSectionData[i].Variation=pRecord[i].Variation;

		m_CharSectionData[i].Texture1.Trim();
		m_CharSectionData[i].Texture2.Trim();
		m_CharSectionData[i].Texture3.Trim();

		if(m_CharSectionData[i].Race)
		{
			RaceSet.insert(m_CharSectionData[i].Race);
		}
		
	}	
	m_CharRaceMax=(UINT)RaceSet.size();

	return true;
}

int CBLZWOWDatabase::GetMaxCharRace()
{
	return m_CharRaceMax;
}
int CBLZWOWDatabase::GetMaxCharSex(int Race)
{
	std::set<UINT> SexSet;
	for(UINT i=0;i<m_CharSectionData.GetCount();i++)
	{
		if(m_CharSectionData[i].Race==Race)
		{
			SexSet.insert(m_CharSectionData[i].Sex);
		}
	}
	return (UINT)SexSet.size();
}
void CBLZWOWDatabase::GetCharMaxInfo(int Race,int Sex,int& SkinColor,int& HairColor,int& FaceType,int& HairType,int& WhiskerType)
{
	std::set<UINT> SkinColorSet;
	std::set<UINT> HairColorSet;
	std::set<UINT> FaceTypeSet;
	std::set<UINT> HairTypeSet;
	std::set<UINT> WhiskerTypeSet;
	for(UINT i=0;i<m_CharSectionData.GetCount();i++)
	{
		if(m_CharSectionData[i].Race==Race&&m_CharSectionData[i].Sex==Sex)
		{
			if(m_CharSectionData[i].GeneralType==0)
			{
				SkinColorSet.insert(m_CharSectionData[i].Variation);
			}
			else if(m_CharSectionData[i].GeneralType==1)
			{
				FaceTypeSet.insert(m_CharSectionData[i].Type);
			}
			else if(m_CharSectionData[i].GeneralType==2)
			{
				WhiskerTypeSet.insert(m_CharSectionData[i].Type);
			}
			else if(m_CharSectionData[i].GeneralType==3)
			{
				HairTypeSet.insert(m_CharSectionData[i].Type);
				HairColorSet.insert(m_CharSectionData[i].Variation);
			}			
		}
	}
	SkinColor=SkinColorSet.size();
	HairColor=HairColorSet.size();
	FaceType=FaceTypeSet.size();
	HairType=HairTypeSet.size();
	WhiskerType=WhiskerTypeSet.size();
}


CBLZWOWDatabase::BLZ_DB_CHAR_SECTION * CBLZWOWDatabase::FindCharSection(UINT GeneralType,UINT Race,UINT Sex,UINT Type,UINT Color)
{
	for(UINT i=0;i<m_CharSectionData.GetCount();i++)
	{
		if(m_CharSectionData[i].GeneralType==GeneralType&&
			m_CharSectionData[i].Race==Race&&
			m_CharSectionData[i].Sex==Sex&&
			m_CharSectionData[i].Type==Type&&
			m_CharSectionData[i].Variation==Color)
		{
			return &(m_CharSectionData[i]);
		}
	}
	return NULL;
}

bool CBLZWOWDatabase::LoadAnimationData(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_ANIMATION_DATA_RECORD_SIZE)
		return false;


	DBC_ANIMATION_DATA_RECORD * pRecord=(DBC_ANIMATION_DATA_RECORD *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);


	m_AnimationData.Resize(pHeader->RecordCount);
	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		

		m_AnimationData[i].ID=pRecord[i].ID;
		m_AnimationData[i].Name=pStringTable+pRecord[i].Name;
		m_AnimationData[i].WeaponState=pRecord[i].WeaponState;
		m_AnimationData[i].Flags=pRecord[i].Flags;
		m_AnimationData[i].Preceeding=pRecord[i].Preceeding;
		m_AnimationData[i].RealId=pRecord[i].RealId;
		m_AnimationData[i].Group=pRecord[i].Group;		

		m_AnimationData[i].Name.Trim();	
		
	}	
	

	return true;
}

CBLZWOWDatabase::BLZ_DB_ANIMATION_DATA_RECORD * CBLZWOWDatabase::FindAnimationData(UINT AnimationID)
{
	for(UINT i=0;i<m_AnimationData.GetCount();i++)
	{
		if(m_AnimationData[i].ID==AnimationID)
		{
			return &(m_AnimationData[i]);
		}
	}
	return NULL;
}


bool CBLZWOWDatabase::LoadCharHairSubMeshInfo(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_CHAR_HAIR_GEOSET_RECORD_SIZE)
		return false;


	DBC_CHAR_HAIR_GEOSET_RECORD * pRecord=(DBC_CHAR_HAIR_GEOSET_RECORD *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);


	m_CharHairSubMeshInfo.Resize(pHeader->RecordCount);
	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		m_CharHairSubMeshInfo[i].Race=pRecord[i].Race;
		m_CharHairSubMeshInfo[i].Sex=pRecord[i].Sex;
		m_CharHairSubMeshInfo[i].HairType=pRecord[i].HairType;
		m_CharHairSubMeshInfo[i].SubMeshID=pRecord[i].SubMeshID;
		m_CharHairSubMeshInfo[i].IsBald=pRecord[i].IsBald;
	
	}	


	return true;
}

int CBLZWOWDatabase::FindCharHairSubMesh(UINT Race,UINT Sex,UINT HairType,bool IsCharBald)
{
	for(UINT i=0;i<m_CharHairSubMeshInfo.GetCount();i++)
	{
		if(m_CharHairSubMeshInfo[i].Race==Race&&
			m_CharHairSubMeshInfo[i].Sex==Sex&&
			((m_CharHairSubMeshInfo[i].HairType==HairType&&(!IsCharBald))||
			m_CharHairSubMeshInfo[i].IsBald&&IsCharBald))
		{			
			if(m_CharHairSubMeshInfo[i].SubMeshID==0)
				return 1;
			else
				return m_CharHairSubMeshInfo[i].SubMeshID;
		}
	}
	return -1;
}

bool CBLZWOWDatabase::LoadCharWhiskerSubMeshInfo(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_CHAR_WHISKER_GEOSET_RECORD_SIZE)
		return false;


	DBC_CHAR_WHISKER_TYPE_RECORD * pRecord=(DBC_CHAR_WHISKER_TYPE_RECORD *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);


	m_CharWhiskerSubMeshInfo.Resize(pHeader->RecordCount);
	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		m_CharWhiskerSubMeshInfo[i].Race=pRecord[i].Race;
		m_CharWhiskerSubMeshInfo[i].Sex=pRecord[i].Sex;
		m_CharWhiskerSubMeshInfo[i].WhiskerType=pRecord[i].WhiskerType;
		m_CharWhiskerSubMeshInfo[i].SubMeshID1=pRecord[i].SubMeshID1;
		m_CharWhiskerSubMeshInfo[i].SubMeshID2=pRecord[i].SubMeshID2;
		m_CharWhiskerSubMeshInfo[i].SubMeshID3=pRecord[i].SubMeshID3;
				
	}	


	return true;
}

void CBLZWOWDatabase::FindCharWhiskerSubMesh(UINT Race,UINT Sex,UINT WhiskerType,int& SubMeshID1,int& SubMeshID2,int& SubMeshID3)
{
	SubMeshID1=0;
	SubMeshID2=0;
	SubMeshID3=0;

	for(UINT i=0;i<m_CharWhiskerSubMeshInfo.GetCount();i++)
	{
		if(m_CharWhiskerSubMeshInfo[i].Race==Race&&
			m_CharWhiskerSubMeshInfo[i].Sex==Sex&&
			m_CharWhiskerSubMeshInfo[i].WhiskerType==WhiskerType)
		{
			SubMeshID1=m_CharWhiskerSubMeshInfo[i].SubMeshID1;
			SubMeshID2=m_CharWhiskerSubMeshInfo[i].SubMeshID2;
			SubMeshID3=m_CharWhiskerSubMeshInfo[i].SubMeshID3;
		}
	}
}


bool CBLZWOWDatabase::LoadCharRaceInfo(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_CHAR_RACE_RECORD_SIZE)
		return false;


	DBC_CHAR_RACE_RECORD * pRecord=(DBC_CHAR_RACE_RECORD *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);


	m_CharRaceInfo.Resize(pHeader->RecordCount);
	for(UINT i=0;i<pHeader->RecordCount;i++)
	{

		m_CharRaceInfo[i].RaceID=pRecord[i].RaceID;
		m_CharRaceInfo[i].Flags=pRecord[i].Flags;			
		m_CharRaceInfo[i].FactionID=pRecord[i].FactionID;		
		m_CharRaceInfo[i].Exploration=pRecord[i].Exploration;	
		m_CharRaceInfo[i].MaleModelID=pRecord[i].MaleModel;	
		m_CharRaceInfo[i].FemaleModelID=pRecord[i].FemaleModel;	
		m_CharRaceInfo[i].Abbrev=pStringTable+pRecord[i].Abbrev;
		m_CharRaceInfo[i].Faction=pRecord[i].Faction;		
		m_CharRaceInfo[i].CreatureType=pRecord[i].CreatureType;	
		m_CharRaceInfo[i].InternalName=pStringTable+pRecord[i].InternalName;	
		m_CharRaceInfo[i].Camera=pRecord[i].Camera;	
		m_CharRaceInfo[i].RaceName=UTF8ToLocal(pStringTable+pRecord[i].RaceNameMale[BLZ_DBC_STR_LOCAL_ZH_CN],
			strlen(pStringTable+pRecord[i].RaceNameMale[BLZ_DBC_STR_LOCAL_ZH_CN]));
		m_CharRaceInfo[i].Feature1=pStringTable+pRecord[i].Feature1;		
		m_CharRaceInfo[i].Feature2=pStringTable+pRecord[i].Feature2;		
		m_CharRaceInfo[i].Feature3=pStringTable+pRecord[i].Feature3;		
		m_CharRaceInfo[i].Expansion=pRecord[i].Expansion;		
			
	}	


	return true;
}

CBLZWOWDatabase::BLZ_DB_CHAR_RACE_INFO * CBLZWOWDatabase::FindRaceInfo(UINT Race)
{
	for(UINT i=0;i<m_CharRaceInfo.GetCount();i++)
	{
		if(m_CharRaceInfo[i].RaceID==Race)
		{
			return &(m_CharRaceInfo[i]);
		}
	}
	return NULL;
}

bool CBLZWOWDatabase::LoadCreatureDisplayInfo(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_CREATURE_DISPLAY_INFO_RECORD_SIZE)
		return false;


	DBC_CREATURE_DISPLAY_INFO_RECORD * pRecord=(DBC_CREATURE_DISPLAY_INFO_RECORD *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);

	m_CreatureDisplayInfo.Resize(pHeader->RecordCount);
	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		m_CreatureDisplayInfo[i].ID=pRecord[i].ID;
		m_CreatureDisplayInfo[i].ModelID=pRecord[i].ModelID;						
		m_CreatureDisplayInfo[i].SoundID=pRecord[i].SoundID;						
		m_CreatureDisplayInfo[i].ExtraDisplayInformationID=pRecord[i].ExtraDisplayInformationID;	
		m_CreatureDisplayInfo[i].Scale=pRecord[i].Scale;						
		m_CreatureDisplayInfo[i].Opacity=pRecord[i].Opacity;					
		m_CreatureDisplayInfo[i].Skin1=pStringTable+pRecord[i].Skin1;						
		m_CreatureDisplayInfo[i].Skin2=pStringTable+pRecord[i].Skin2;						
		m_CreatureDisplayInfo[i].Skin3=pStringTable+pRecord[i].Skin3;	
		m_CreatureDisplayInfo[i].Icon=pStringTable+pRecord[i].Icon;	
		m_CreatureDisplayInfo[i].BloodLevelID=pRecord[i].BloodLevelID;			
		m_CreatureDisplayInfo[i].BloodID=pRecord[i].BloodID;		
		m_CreatureDisplayInfo[i].NPCSoundID=pRecord[i].NPCSoundID;		
		m_CreatureDisplayInfo[i].ParticleID=pRecord[i].ParticleID;	
		m_CreatureDisplayInfo[i].CreatureGeosetDataID=pRecord[i].CreatureGeosetDataID;	
		m_CreatureDisplayInfo[i].ObjectEffectPackageID=pRecord[i].ObjectEffectPackageID;
	}	


	return true;
}

CBLZWOWDatabase::BLZ_DB_CREATURE_DISPLAY_INFO * CBLZWOWDatabase::FindCreatureDisplayInfo(UINT ID)
{
	for(UINT i=0;i<m_CreatureDisplayInfo.GetCount();i++)
	{
		if(m_CreatureDisplayInfo[i].ID==ID)
		{
			return &(m_CreatureDisplayInfo[i]);
		}
	}
	return NULL;
}

bool CBLZWOWDatabase::LoadCreatureModelInfo(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_CREATURE_MODEL_INFO_RECORD_SIZE)
		return false;


	DBC_CREATURE_MODEL_INFO_RECORD * pRecord=(DBC_CREATURE_MODEL_INFO_RECORD *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);

	
	m_CreatureModelInfo.Resize(pHeader->RecordCount);
	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		m_CreatureModelInfo[i].ID=pRecord[i].ID;				
		m_CreatureModelInfo[i].Flags=pRecord[i].Flags;
		m_CreatureModelInfo[i].ModelPath=pStringTable+pRecord[i].ModelPath;
		m_CreatureModelInfo[i].AltermateModel=pStringTable+pRecord[i].AltermateModel;
		m_CreatureModelInfo[i].Scale=pRecord[i].Scale;
		m_CreatureModelInfo[i].BloodLevelID=pRecord[i].BloodLevelID;
		m_CreatureModelInfo[i].SoundDataID=pRecord[i].SoundDataID;
		m_CreatureModelInfo[i].CollisionWidth=pRecord[i].CollisionWidth;
		m_CreatureModelInfo[i].CollisionHeight=pRecord[i].CollisionHeight;		
	}	


	return true;
}

CBLZWOWDatabase::BLZ_DB_CREATURE_MODEL_INFO * CBLZWOWDatabase::FindCreatureModelInfo(UINT ID)
{
	for(UINT i=0;i<m_CreatureModelInfo.GetCount();i++)
	{
		if(m_CreatureModelInfo[i].ID==ID)
		{
			return &(m_CreatureModelInfo[i]);
		}
	}
	return NULL;
}

bool CBLZWOWDatabase::FindCharModelInfo(UINT Race,UINT Sex,CEasyString& ModelFileName,CEasyString& SkinFileName)
{
	BLZ_DB_CHAR_RACE_INFO * pRaceInfo=FindRaceInfo(Race);
	if(pRaceInfo)
	{
		BLZ_DB_CREATURE_DISPLAY_INFO * pDisplayInfo=NULL;
		if(Sex==0)
			pDisplayInfo=FindCreatureDisplayInfo(pRaceInfo->MaleModelID);
		else
			pDisplayInfo=FindCreatureDisplayInfo(pRaceInfo->FemaleModelID);
		if(pDisplayInfo)
		{
			
			BLZ_DB_CREATURE_MODEL_INFO * pModelInfo=FindCreatureModelInfo(pDisplayInfo->ModelID);
			if(pModelInfo)
			{
				ModelFileName=pModelInfo->ModelPath;
				SkinFileName=pDisplayInfo->Skin1;
				return true;
			}
		}
	}
	return false;
}

bool CBLZWOWDatabase::LoadItemClass(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_ITEM_CLASS_RECORD_SIZE)
		return false;


	DBC_ITEM_CLASS * pRecord=(DBC_ITEM_CLASS *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);


	m_ItemClass.Resize(pHeader->RecordCount);
	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		m_ItemClass[i].ClassID=pRecord[i].ClassID;				
		m_ItemClass[i].SubClassID=pRecord[i].SubClassID;
		m_ItemClass[i].IsWeapon=(pRecord[i].IsWeapon!=0);
		m_ItemClass[i].Name=UTF8ToLocal(pStringTable+pRecord[i].Name[BLZ_DBC_STR_LOCAL_ZH_CN],
			strlen(pStringTable+pRecord[i].Name[BLZ_DBC_STR_LOCAL_ZH_CN]));
		
	}	


	return true;
}
bool CBLZWOWDatabase::LoadItemSubClass(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_ITEM_SUB_CLASS_RECORD_SIZE)
		return false;


	DBC_ITEM_SUB_CLASS * pRecord=(DBC_ITEM_SUB_CLASS *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);


	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		BLZ_DB_ITEM_CLASS * pClassInfo=GetItemClassInfo(pRecord[i].ClassID);
		if(pClassInfo)
		{
			BLZ_DB_ITEM_SUB_CLASS Info;

			Info.ClassID=pRecord[i].ClassID;
			Info.SubClassID=pRecord[i].SubClassID;
			Info.HandsNumber=pRecord[i].HandsNumber;		
			Info.Name=UTF8ToLocal(pStringTable+pRecord[i].Name[BLZ_DBC_STR_LOCAL_ZH_CN],
				strlen(pStringTable+pRecord[i].Name[BLZ_DBC_STR_LOCAL_ZH_CN]));
			Info.UniquePlural=UTF8ToLocal(pStringTable+pRecord[i].UniquePlural[BLZ_DBC_STR_LOCAL_ZH_CN],
				strlen(pStringTable+pRecord[i].UniquePlural[BLZ_DBC_STR_LOCAL_ZH_CN]));
			pClassInfo->SubClassInfo.Add(Info);
		}
	}	


	return true;
}
bool CBLZWOWDatabase::LoadItemSubClassMask(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_ITEM_SUB_CLASS_MASK_RECORD_SIZE)
		return false;


	DBC_ITEM_SUB_CLASS_MASK * pRecord=(DBC_ITEM_SUB_CLASS_MASK *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);


	m_ItemSubClassMask.Resize(pHeader->RecordCount);
	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		m_ItemSubClassMask[i].ClassID=pRecord[i].ID;				
		m_ItemSubClassMask[i].SubClassIDMask=pRecord[i].Mask;
		m_ItemSubClassMask[i].Name=UTF8ToLocal(pStringTable+pRecord[i].Name[BLZ_DBC_STR_LOCAL_ZH_CN],
			strlen(pStringTable+pRecord[i].Name[BLZ_DBC_STR_LOCAL_ZH_CN]));		
	}	


	return true;
}

UINT CBLZWOWDatabase::GetItemClassCount()
{
	return m_ItemClass.GetCount();
}
CBLZWOWDatabase::BLZ_DB_ITEM_CLASS * CBLZWOWDatabase::GetItemClassInfo(UINT ClassID)
{
	for(UINT i=0;i<m_ItemClass.GetCount();i++)
	{
		if(m_ItemClass[i].ClassID==ClassID)
		{
			return &(m_ItemClass[i]);
		}
	}
	return NULL;
}
CBLZWOWDatabase::BLZ_DB_ITEM_CLASS * CBLZWOWDatabase::GetItemClassInfoByIndex(UINT Index)
{
	if(Index<m_ItemClass.GetCount())
	{
		return &(m_ItemClass[Index]);
	}
	return NULL;
}

CBLZWOWDatabase::BLZ_DB_ITEM_SUB_CLASS * CBLZWOWDatabase::GetItemSubClassInfo(UINT ClassID,UINT SubClassID)
{
	for(UINT i=0;i<m_ItemClass.GetCount();i++)
	{
		if(m_ItemClass[i].ClassID==ClassID)
		{
			for(UINT j=0;j<m_ItemClass[i].SubClassInfo.GetCount();j++)
			{
				if(m_ItemClass[i].SubClassInfo[j].SubClassID==SubClassID)
				{
					return &(m_ItemClass[i].SubClassInfo[j]);
				}
			}
		}
	}
	return NULL;
}

UINT CBLZWOWDatabase::GetItemSubClassMaskCount()
{
	return m_ItemSubClassMask.GetCount();
}

CEasyString CBLZWOWDatabase::UTF8ToLocal(LPCTSTR szStr,int StrLen)
{	
	WCHAR	Buffer[1024];
	
	int len=MultiByteToWideChar(CP_UTF8,0,szStr,StrLen,Buffer,1024);
	Buffer[len]=0;
	return Buffer;
}


bool CBLZWOWDatabase::LoadItemData(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_ITEM_DATA_RECORD_SIZE)
		return false;


	DBC_ITEM_DATA * pRecord=(DBC_ITEM_DATA *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);



	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		BLZ_DB_ITEM_DATA& Info=m_ItemData[pRecord[i].ItemID];

		Info.ItemID=pRecord[i].ItemID;
		Info.ItemClass=pRecord[i].ItemClass;
		Info.ItemSubClass=pRecord[i].ItemSubClass;
		Info.MaterialID=pRecord[i].MaterialID;
		Info.ItemDisplayInfo=pRecord[i].ItemDisplayInfo;
		Info.InventorySlotID=pRecord[i].InventorySlotID;
		Info.SheathID=pRecord[i].SheathID;	

		
		Info.Type=0;
		Info.SheathType=0;
		Info.Quality=0;

		BLZ_DB_ITEM_SUB_CLASS * pSubClassInfo=GetItemSubClassInfo(Info.ItemClass,Info.ItemSubClass);
		if(pSubClassInfo)
		{
			pSubClassInfo->ItemInfo.Add(&Info);
		}
	}	


	return true;
}

bool CBLZWOWDatabase::LoadItemCacheData(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize+sizeof(TCHAR));
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();
	DBData.PushConstBack(0,sizeof(TCHAR));


	//pFile=CD3DWOWM2Model::CreateFileAccessor();
	//if(pFile==NULL)
	//	return false;
	//if(!pFile->Open("ItemData.csv",IFileAccessor::modeCreateAlways|IFileAccessor::modeWrite))
	//{
	//	pFile->Release();
	//	return false;	
	//}



	LPCTSTR szText=(LPCTSTR)DBData.GetBuffer();

	bool IsNewLine=true;

	while(*szText)
	{
		if(*szText=='\n')
		{
			IsNewLine=true;
			szText++;
		}
		else if(*szText=='\r')
		{
			szText++;
		}
		else
		{
			if(IsNewLine)
			{
				UINT ItemID;
				UINT ModelID;
				UINT ClassID;
				UINT SubClassID;
				UINT Type;
				UINT SheathType;
				UINT Quality;
				TCHAR szName[128];

				sscanf_s(szText,"%u,%u,%u,%u,%u,%u,%u,%[^,\r\n]s",
					&ItemID,&ModelID,&ClassID,&SubClassID,&Type,&SheathType,&Quality,szName,128);
				szName[127]=0;			

				BLZ_DB_ITEM_DATA * pItemData=m_ItemData.Find(ItemID);
				if(pItemData!=NULL)
				{
					pItemData->Type=Type;
					pItemData->SheathType=SheathType;
					pItemData->Quality=Quality;
					pItemData->Name=szName;				

					//CEasyString Output;
					//Output.Format("%u,%u,%u,%u,%u,%u,%u,%s\r\n",ItemID,ModelID,ClassID,SubClassID,Type,SheathType,Quality,szName);
					//pFile->Write(Output.GetBuffer(),Output.GetLength());
				}
				szText=strchr(szText,'\n');
				IsNewLine=false;

				
			}
			else
			{
				szText++;
			}
		}
		
	}	

	//pFile->Release();

	return true;
}


bool CBLZWOWDatabase::LoadItemDisplayInfo(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_ITEM_DISPLAY_INFO_RECORD_SIZE)
		return false;


	DBC_ITEM_DISPLAY_INFO * pRecord=(DBC_ITEM_DISPLAY_INFO *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);



	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		BLZ_DB_ITEM_DISPLAY_INFO& Info=m_ItemDisplayInfo[pRecord[i].ID];

		Info.ID=pRecord[i].ID;
		Info.LeftModel=pStringTable+pRecord[i].LeftModel;			
		Info.RightModel=pStringTable+pRecord[i].RightModel;			
		Info.LeftModelTexture=pStringTable+pRecord[i].LeftModelTexture;	
		Info.RightModelTexture=pStringTable+pRecord[i].RightModelTexture;	
		Info.Icon=pStringTable+pRecord[i].Icon;				
		Info.Texture=pStringTable+pRecord[i].Texture;			
		Info.GloveGeosetFlags=pRecord[i].GloveGeosetFlags;	
		Info.PantsGeosetFlags=pRecord[i].PantsGeosetFlags;	
		Info.RobeGeosetFlags=pRecord[i].RobeGeosetFlags;	
		Info.BootsGeosetFlags=pRecord[i].BootsGeosetFlags;
		Info.UnkownGeosetFlags=pRecord[i].UnkownGeosetFlags;
		Info.ItemGroupSounds=pRecord[i].ItemGroupSounds;	
		Info.HelmetGeosetVisData1=pRecord[i].HelmetGeosetVisData1;
		Info.HelmetGeosetVisData2=pRecord[i].HelmetGeosetVisData2;
		Info.UpperArmTexture=pStringTable+pRecord[i].UpperArmTexture;	
		Info.LowerArmTexture=pStringTable+pRecord[i].LowerArmTexture;	
		Info.HandsTexture=pStringTable+pRecord[i].HandsTexture;		
		Info.UpperTorsoTexture=pStringTable+pRecord[i].UpperTorsoTexture;	
		Info.LowerTorsoTexture=pStringTable+pRecord[i].LowerTorsoTexture;	
		Info.UpperLegTexture=pStringTable+pRecord[i].UpperLegTexture;	
		Info.LowerLegTexture=pStringTable+pRecord[i].LowerLegTexture;	
		Info.FootTexture=pStringTable+pRecord[i].FootTexture;		
		Info.ItemVisuals=pRecord[i].ItemVisuals;			
		
	}		

	return true;
}

CBLZWOWDatabase::BLZ_DB_ITEM_DATA * CBLZWOWDatabase::GetItemData(UINT ItemID)
{
	return m_ItemData.Find(ItemID);	
}

CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * CBLZWOWDatabase::GetItemDisplayInfo(UINT ItemDisplayID)
{
	return m_ItemDisplayInfo.Find(ItemDisplayID);	
}


bool CBLZWOWDatabase::LoadHelmetGeosetVisibleInfo(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_HELMET_GEOSET_VISIBLE_RECORD_SIZE)
		return false;


	DBC_HELMET_GEOSET_VISIBLE_INFO * pRecord=(DBC_HELMET_GEOSET_VISIBLE_INFO *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);



	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		BLZ_DB_HELMET_GEOSET_VISIBLE_INFO& Info=m_HelmetGeosetVisibleInfo[pRecord[i].ID];

		Info.ID=pRecord[i].ID;
		Info.HairVisible=pRecord[i].HairFlags;			
		Info.Facial1Visible=pRecord[i].Facial1Flags;			
		Info.Facial2Visible=pRecord[i].Facial2Flags;	
		Info.Facial3Visible=pRecord[i].Facial3Flags;	
		Info.EarsVisible=pRecord[i].EarsFlags;				
	}	

	return true;
}

CBLZWOWDatabase::BLZ_DB_HELMET_GEOSET_VISIBLE_INFO * CBLZWOWDatabase::GetHelmetGeosetVisibleInfo(UINT ID)
{
	return m_HelmetGeosetVisibleInfo.Find(ID);	
}

bool CBLZWOWDatabase::LoadMapInfo(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	CEasyBuffer DBData;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	DBData.Create(FileSize);
	pFile->Read(DBData.GetBuffer(),FileSize);
	DBData.SetUsedSize(FileSize);
	pFile->Release();

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)DBData.GetBuffer();
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(pHeader->RecordSize!=BLZ_DBC_MAP_RECORD_SIZE)
		return false;


	DBC_MAP_INFO * pRecord=(DBC_MAP_INFO *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER));
	char * pStringTable=(char *)((BYTE *)DBData.GetBuffer()+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);

	m_MaoInfo.Resize(pHeader->RecordCount);

	for(UINT i=0;i<pHeader->RecordCount;i++)
	{
		BLZ_DB_MAP_INFO& Info=m_MaoInfo[i];

		Info.ID=						pRecord[i].ID;
		Info.InternalName=				pStringTable+pRecord[i].InternalName;			
		Info.AreaType=					pRecord[i].AreaType;				
		Info.IsBattleground=			pRecord[i].IsBattleground;			
		Info.Name=						UTF8ToLocal(pStringTable+pRecord[i].Name[BLZ_DBC_STR_LOCAL_ZH_CN],
			strlen(pStringTable+pRecord[i].Name[BLZ_DBC_STR_LOCAL_ZH_CN]));
		Info.AreaTable=					pRecord[i].AreaTable;				
		Info.AllianceDesc=				UTF8ToLocal(pStringTable+pRecord[i].AllianceDesc[BLZ_DBC_STR_LOCAL_ZH_CN],
			strlen(pStringTable+pRecord[i].AllianceDesc[BLZ_DBC_STR_LOCAL_ZH_CN]));
		Info.HordeDesc=					UTF8ToLocal(pStringTable+pRecord[i].HordeDesc[BLZ_DBC_STR_LOCAL_ZH_CN],
			strlen(pStringTable+pRecord[i].HordeDesc[BLZ_DBC_STR_LOCAL_ZH_CN]));
		Info.LoadingScreen=				pRecord[i].LoadingScreen;			
		Info.BattlefieldMapIconScale=	pRecord[i].BattlefieldMapIconScale;
		Info.Requirement;				UTF8ToLocal(pStringTable+pRecord[i].Requirement[BLZ_DBC_STR_LOCAL_ZH_CN],
			strlen(pStringTable+pRecord[i].Requirement[BLZ_DBC_STR_LOCAL_ZH_CN]));
		Info.Heroic=					UTF8ToLocal(pStringTable+pRecord[i].Heroic[BLZ_DBC_STR_LOCAL_ZH_CN],
			strlen(pStringTable+pRecord[i].Heroic[BLZ_DBC_STR_LOCAL_ZH_CN]));
		Info.ParentArea=				pRecord[i].ParentArea;
		Info.XCoord=					pRecord[i].XCoord;					
		Info.YCoord=					pRecord[i].YCoord;					
		Info.ResetTimeRaid=				pRecord[i].ResetTimeRaid;			
		Info.ResetTimeHeroicRaid=		pRecord[i].ResetTimeHeroicRaid;			
		Info.TimeOfDayOverride=			pRecord[i].TimeOfDayOverride;		
		Info.ExpansionID=				pRecord[i].ExpansionID;			
			

	}		

	return true;
}

UINT CBLZWOWDatabase::GetMapInfoCount()
{
	return m_MaoInfo.GetCount();
}

CBLZWOWDatabase::BLZ_DB_MAP_INFO * CBLZWOWDatabase::GetMapInfo(UINT Index)
{
	return m_MaoInfo.GetObject(Index);
}

}