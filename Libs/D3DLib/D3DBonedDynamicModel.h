/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DBonedDynamicModel.h                                   */
/*      创建日期:  2009年09月25日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


namespace D3DLib{

#define KEY_NAME_LEN	64
typedef char KEY_NAME[KEY_NAME_LEN];

#pragma comment(linker,"/include:?m_CD3DBonedDynamicModelClassInfoRegister@CD3DBonedDynamicModel@D3DLib@@1VCClassInfoRegister@@A")



class CD3DBonedDynamicModel :
	public CD3DBaseDynamicModel
{
protected:
	//struct STORAGE_STRUCT:public CD3DObject::STORAGE_STRUCT
	//{		
	//	int				MeshPartCount;
	//	int				SkinResourceCount;
	//	int				AnimationResourceCount;
	//};

	enum SST_MEMBER_ID
	{
		SST_D3DBDM_ANI_INFO=SST_D3DO_MAX,		
		SST_D3DBDM_SKIN_PART_INFO,
		SST_D3DBDM_SKIN_INFO,
		SST_D3DBDM_ENABLE_INTERIM,
		SST_D3DBDM_INTERIM_TIME,
		SST_D3DBDM_CUR_ANI,
		SST_D3DBDM_MAX=SST_D3DO_MAX+50,
	};
	enum SST_ANI_INFO
	{
		SST_AI_NAME=1,
		SST_AI_ANI_RESOURCE_ID,
	};
	enum SST_SKIN_INFO
	{
		SST_SI_NAME=1,
		SST_SI_SKIN_RESOURCE_ID,
	};

	//struct STORAGE_RESOURCE_STRUCT
	//{
	//	char	Name[KEY_NAME_LEN];
	//	int		ResourceID;
	//};

	struct SKIN_PART_INFO
	{
		char						Name[KEY_NAME_LEN];
		CD3DSubMesh *				pSubMesh;
		CD3DBonedModelResource*		pSkin;
		SKIN_PART_INFO()
		{
			ZeroMemory(this,sizeof(*this));
		}
	};

	
	typedef std::map<std::string,CD3DBonedAniResource*> CModelAnimationMap;
	typedef std::map<std::string,CD3DBonedModelResource*> CModelSkinMap;

	
	CModelAnimationMap			m_Animations;
	CModelSkinMap				m_Skins;
	CEasyArray<SKIN_PART_INFO>	m_SkinParts;

	
	CD3DBonedAniResource *		m_pCurAnimation;	
	LPCTSTR						m_CurAnimationName;
	CD3DMatrix					m_BoneMatrixs[MAX_BONE_NUM];
	int							m_BoneCount;
	CD3DDummy*					m_pBoneDummys[MAX_BONE_NUM];
	bool						m_IsPlaying;
	bool						m_IsLoop;
	FLOAT						m_CurAnimationLen;
	FLOAT						m_PlayStartTime;
	FLOAT						m_CurUpdateTime;
	FLOAT						m_CurFrameTime;

	bool						m_IsEnableInterim;
	bool						m_IsInInterim;
	FLOAT						m_InterimTime;	
	BONE_TRANS					m_InterimStartBone[MAX_BONE_NUM];
	BONE_TRANS					m_InterimEndBone[MAX_BONE_NUM];

	

	DECLARE_CLASS_INFO(CD3DBonedDynamicModel)
	
public:
	CD3DBonedDynamicModel(void);
	virtual ~CD3DBonedDynamicModel(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	virtual void PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera);

	bool LoadM2(LPCTSTR FileName);

	bool AddSkinFromSKN(LPCTSTR FileName,LPCTSTR SkinName);
	bool AddSkin(CD3DBonedModelResource * pSkin,LPCTSTR SkinName);
	bool DeleteSkin(LPCTSTR SkinName);
	void ClearAllSkin();
	bool RenameSkin(LPCTSTR SkinName,LPCTSTR NewName);
	int GetSkinCount();
	CD3DBonedModelResource * GetSkin(int Index);
	CD3DBonedModelResource* GetSkin(LPCTSTR SkinName);
	const char * GetSkinName(int Index);

	int GetSkinPartCount();
	bool SetSkinPart(int PartIndex,LPCTSTR SkinName);//PartIndex<0则使用整个Skin
	LPCTSTR GetSkinPart(int PartIndex);

	bool AddAnimationFromACT(LPCTSTR FileName,LPCTSTR AnimationName,bool Reverse=false);
	bool AddAnimation(CD3DBonedAniResource* pAni,LPCTSTR AnimationName);

	bool DeleteAnimation(LPCTSTR AnimationName);
	void ClearAllAnimation();
	bool RenameAnimation(LPCTSTR AnimationName,LPCTSTR NewName);
	int GetAnimationCount();
	CD3DBonedAniResource *	GetAnimation(int Index);	
	CD3DBonedAniResource* GetAnimation(LPCTSTR AnimationName);
	const char * GetAnimationName(int Index);

	virtual bool Play(bool IsLoop);
	bool Play(LPCTSTR AnimationName,bool IsLoop);
	virtual bool Stop();

	virtual bool IsPlaying();
	

	void EnableInterim(bool Enable);
	void SetInterimTime(FLOAT Time);

	
	virtual bool CloneFrom(CNameObject * pObject,UINT Param=0);
	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
public:
	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(UINT index);

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();

	virtual bool RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly=true);
	virtual bool GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y);
	


	virtual void Update(FLOAT Time);
protected:
	void CreateBoneDummy(CD3DBonedModelResource* pSkin);
	bool IsBoneDummy(CD3DObject* pDummy);
	void PickNoneDummyObject(CD3DObject* pDummy);

	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual bool USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);

	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual int USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param=0);
	//virtual bool USOReadChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
};

inline bool CD3DBonedDynamicModel::IsPlaying()
{
	return m_IsPlaying;
}

inline void CD3DBonedDynamicModel::EnableInterim(bool Enable)
{
	m_IsEnableInterim=Enable;
}

inline void CD3DBonedDynamicModel::SetInterimTime(FLOAT Time)
{
	m_InterimTime=Time;
}

inline int CD3DBonedDynamicModel::GetSkinCount()
{
	return (int)m_Skins.size();
}

inline CD3DBonedModelResource *	CD3DBonedDynamicModel::GetSkin(int Index)
{
	CModelSkinMap::iterator Itr=m_Skins.begin();
	while(Index&&Itr!=m_Skins.end())
	{
		Itr++;
		Index--;
	}
	if(Itr!=m_Skins.end())
		return Itr->second;
	else
		return NULL;
}

inline CD3DBonedModelResource* CD3DBonedDynamicModel::GetSkin(LPCTSTR SkinName)
{
	return m_Skins[std::string(SkinName)];
}

inline const char * CD3DBonedDynamicModel::GetSkinName(int Index)
{
	CModelSkinMap::iterator Itr=m_Skins.begin();
	while(Index&&Itr!=m_Skins.end())
	{
		Itr++;
		Index--;
	}
	if(Itr!=m_Skins.end())
		return Itr->first.c_str();
	else
		return NULL;
}


inline int CD3DBonedDynamicModel::GetAnimationCount()
{
	return (int)m_Animations.size();
}

inline CD3DBonedAniResource *	CD3DBonedDynamicModel::GetAnimation(int Index)
{
	CModelAnimationMap::iterator Itr=m_Animations.begin();
	while(Index&&Itr!=m_Animations.end())
	{
		Itr++;
		Index--;
	}
	if(Itr!=m_Animations.end())
		return Itr->second;
	else
		return NULL;
}

inline CD3DBonedAniResource* CD3DBonedDynamicModel::GetAnimation(LPCTSTR AnimationName)
{
	return m_Animations[std::string(AnimationName)];
}

inline const char * CD3DBonedDynamicModel::GetAnimationName(int Index)
{
	CModelAnimationMap::iterator Itr=m_Animations.begin();
	while(Index&&Itr!=m_Animations.end())
	{
		Itr++;
		Index--;
	}
	if(Itr!=m_Animations.end())
		return Itr->first.c_str();
	else
		return NULL;
}

}