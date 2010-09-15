/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWM2ModelResource.h                                  */
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

class CD3DWOWM2ModelResource :
	public CD3DObjectResource
{
public:
	enum SUB_MESH_PROPERTY:UINT64
	{		
		SMP_RENDER_FLAG=(0x1F<<4),
		SMP_NO_LIGHT=(1<<4),
		SMP_NO_FOG=(1<<5),
		SMP_NO_CULL=(1<<6),
		SMP_BILL_BOARD=(1<<7),
		SMP_DISABLE_ZBUFFER=(1<<8),

		SMP_BLENDING_MODE=(7<<9),
		SMP_BLENDING_ALPHA_TEST=(1<<9),
		SMP_BLENDING_ALPHA_BLENDING=(2<<9),
		SMP_BLENDING_ADDITIVE=(3<<9),
		SMP_BLENDING_ADDITIVE_ALPHA=(4<<9),
		SMP_BLENDING_MODULATE=(5<<9),
		SMP_BLENDING_MUL=(6<<9),

		SMP_USE_VERTEX_ALPHA1=(1<<14),
		SMP_USE_VERTEX_ALPHA2=(1<<15),

		SMP_TEXTURE_TYPE=(0xF<<16),
		SMP_TEXTURE_TYPE_DIRECT=0,
		SMP_TEXTURE_TYPE_BODY=(1<<16),
		SMP_TEXTURE_TYPE_CAPE=(2<<16),
		SMP_TEXTURE_TYPE_HAIR=(6<<16),
		SMP_TEXTURE_TYPE_FUR=(8<<16),
		SMP_TEXTURE_TYPE_CREATURE_SKIN1=(11<<16),
		SMP_TEXTURE_TYPE_CREATURE_SKIN2=(12<<16),
		SMP_TEXTURE_TYPE_CREATURE_SKIN3=(13<<16),

		SMP_TEXTURE_FLAG=(7<<20),
		SMP_TEXTURE_WRAP_X=(1<<20),
		SMP_TEXTURE_WRAP_Y=(1<<21),
		SMP_TEXTURE_ENV_MAP=(1<<22),

		SMP_COLOR_ANI_INDEX=((UINT64)0xFF<<24),
		SMP_TRANSPARENCY_ANI_INDEX=((UINT64)0xFF<<32),
		SMP_UV_ANI_INDEX=((UINT64)0xFF<<40),

	};
	

	struct MODEL_VERTEXT
	{
		CD3DVector3			Pos;
		FLOAT				BoneWeight[MAX_VERTEX_BONE_BIND];
		BYTE				BoneID[MAX_VERTEX_BONE_BIND];
		CD3DVector3			Normal;		
		D3DCOLOR			Diffuse;
		CD3DVector2			TextureCoord;	
	};
	template<class KEY_TYPE>
	struct ANIMATION_FRAME
	{		
		CEasyArray<UINT>			TimeStamps;
		CEasyArray<KEY_TYPE>		Keys;
	};
	template<class KEY_TYPE>
	struct FAKE_ANIMATION_FRAME
	{		
		CEasyArray<FLOAT>			TimeStamps;
		CEasyArray<KEY_TYPE>		Keys;
	};
	template<class KEY_TYPE>
	struct ANIMATION_BLOCK
	{
		int										InterpolationType;
		int										GlobalSequenceID;
		CEasyArray<ANIMATION_FRAME<KEY_TYPE> >	Animations;
	};
	typedef ANIMATION_BLOCK<CD3DVector3> ANIMATION_TRANSLATION_BLOCK;
	typedef ANIMATION_BLOCK<CD3DQuaternion> ANIMATION_ROTATION_BLOCK;
	typedef ANIMATION_BLOCK<CD3DVector3> ANIMATION_SCALING_BLOCK;
	typedef ANIMATION_BLOCK<CD3DVector3> ANIMATION_COLOR_BLOCK;
	typedef ANIMATION_BLOCK<FLOAT> ANIMATION_ALPHA_BLOCK;
	struct ANIMATION_SEQUENCE
	{
		UINT				AnimationID;
		CEasyString			AnimationName;		
		UINT				SubAnimationID;
		int					Index;
		UINT				Length;
		FLOAT				MovingSpeed;
		UINT				Flags;
		UINT				Flags2;		
		UINT				PlaybackSpeed;
		CD3DBoundingBox		BoundingBox;
		CD3DBoundingSphere	BoundingSphere;		
	};	
	struct COLOR_ANIMATION_INFO
	{
		ANIMATION_COLOR_BLOCK	ColorAnimations;
		ANIMATION_ALPHA_BLOCK	AlphaAnimations;
	};
	struct TRANSPARENCY_ANIMATION_INFO
	{
		ANIMATION_ALPHA_BLOCK	AlphaAnimations;
	};
	struct MODEL_BONE
	{
		int								AnimationSeq;
		UINT							Flags;
		int								ChildCount;
		short							ParentBone;
		WORD							GeosetID;				
		CD3DVector3						PivotPoint;		
		ANIMATION_TRANSLATION_BLOCK		Translations;
		ANIMATION_ROTATION_BLOCK		Rotations;
		ANIMATION_SCALING_BLOCK			Scalings;


	};
	enum MODLE_BONE_FLAG
	{
		BONE_FLAG_BILLBOARD=(1<<3),
	};
	enum MODEL_ATTACHMENT_TYPE
	{
		MAT_DUMMY,
		MAT_PARTICLE_EMITTER,
		MAT_RIBBON_EMITTER,
	};
	struct BONE_MATRIX
	{
		WORD			BoneID;		
		short			ParentID;
		UINT			Flags;
		CD3DVector3		PivotPoint;
		CD3DMatrix		Matrix;
		WORD			AttachmentType;
		WORD			AttachmentID;
		CD3DMatrix		AttachTransform;
		CD3DObject *	pAttachObject;
		BONE_MATRIX()
		{
			BoneID=0;
			ParentID=0;
			AttachmentType=0;
			AttachmentID=0;
			pAttachObject=NULL;
		}
	};	
	struct MODEL_ATTACHMENT
	{
		WORD						ID;
		WORD						Bone;
		CEasyString					Name;
		CD3DVector3					Position;
		//ANIMATION_TRANSLATION_BLOCK	AnimationData;
	};

	struct PARTICLE_EMITTER_INFO
	{
		UINT								Flags;					
		CD3DVector3							Position;				
		WORD								Bone;					
		CD3DTexture *						pTexture;				
		BYTE								BlendingType;			
		BYTE								EmitterType;			
		WORD								ParticleColorRef;		
		BYTE								ParticleType;			
		BYTE								HeadOrTail;				
		short								TextureTileRotation;	
		WORD								TextureRows;			
		WORD								TextureCols;	
		CD3DVector3							Scale;					
		FLOAT								Slowdown;				
		FLOAT								Rotation;				
		CD3DVector3							Trans;
		CD3DFX *							pFX;
		ANIMATION_BLOCK<FLOAT>				Speed;			
		ANIMATION_BLOCK<FLOAT>				SpeedVariation;			
		ANIMATION_BLOCK<FLOAT>				VerticalRange;			
		ANIMATION_BLOCK<FLOAT>				HorizontalRange;		
		ANIMATION_BLOCK<FLOAT>				Gravity;				
		ANIMATION_BLOCK<FLOAT>				LifeSpan;				
		ANIMATION_BLOCK<FLOAT>				EmissionRate;			
		ANIMATION_BLOCK<FLOAT>				EmissionAreaLength;		
		ANIMATION_BLOCK<FLOAT>				EmissionAreaWidth;		
		ANIMATION_BLOCK<FLOAT>				Decelerate;		
		//ANIMATION_BLOCK<bool>				EnabledIn;
		FAKE_ANIMATION_FRAME<CD3DVector3>	ParticleColor;			
		FAKE_ANIMATION_FRAME<FLOAT>			ParticleOpacity;		
		FAKE_ANIMATION_FRAME<CD3DVector2>	ParticleSizes;			
		FAKE_ANIMATION_FRAME<short>			Intensity;
		//FAKE_ANIMATION_FRAME<short>			UnknowBlock;
	};
	struct PARTICLE_PARAM
	{
		FLOAT								Speed;
		FLOAT								SpeedVariation;
		FLOAT								VerticalRange;
		FLOAT								HorizontalRange;
		FLOAT								Gravity;
		FLOAT								LifeSpan;
		FLOAT								EmissionRate;
		FLOAT								EmissionAreaLength;
		FLOAT								EmissionAreaWidth;
		FLOAT								Decelerate;
	};
	struct RIBBON_EMITTER_INFO
	{			
		UINT							BoneID;			
		CD3DVector3						Position;		
		CEasyArray<CD3DTexture *>		Textures;	
		CD3DFX *						pFX;		
		bool							IsTransparent;
		FLOAT							Resolution;		
		FLOAT							Length;			
		FLOAT							Emissionangle;	
		ANIMATION_BLOCK<CD3DVector3>	Color;			
		ANIMATION_BLOCK<FLOAT>			Opacity;		
		ANIMATION_BLOCK<FLOAT>			Above;			
		ANIMATION_BLOCK<FLOAT>			Below;			
	};
	struct RIBBON_PARAM
	{
		D3DCOLOR	Color;	
		FLOAT		Above;	
		FLOAT		Below;			
	};
	struct TEXTURE_UV_ANI
	{		
		ANIMATION_TRANSLATION_BLOCK		Translations;
		ANIMATION_ROTATION_BLOCK		Rotations;
		ANIMATION_SCALING_BLOCK			Scalings;
	};

protected:
	enum LOCAL_DEFINES
	{
		MAX_M2_ANI_NAME_LEN=64,
		MAX_M2_ATTACHMENT_NAME_LEN=64,
	};
	enum SST_MEMBER_ID
	{
		SST_D3DWMMR_BONE_ANIMATION_SEQUENCE=SST_D3DOR_MAX,
		SST_D3DWMMR_COLOR_ANIMATION,
		SST_D3DWMMR_TRANSPARENCY_ANIMATION,
		SST_D3DWMMR_TEXTURE_UV_ANIMATION,
		SST_D3DWMMR_BONE,
		SST_D3DWMMR_SKIN_MESH_BONE_COUNT,
		SST_D3DWMMR_KEY_BONE,
		SST_D3DWMMR_GLOBAL_SEQUENCE,
		SST_D3DWMMR_ATTACHMENT,
		SST_D3DWMMR_PARTICLE_EMITTER,
		SST_D3DWMMR_RIBBON_EMITTER,
		SST_D3DWMMR_MAX=SST_D3DOR_MAX+50,
	};
	enum SST_BONE_ANIMATION_SEQUENCE
	{
		SST_BAS_ANIMATION_ID=1,
		SST_BAS_ANIMATION_NAME,
		SST_BAS_SUBANIMATION_ID,
		SST_BAS_INDEX,
		SST_BAS_LENGTH,
		SST_BAS_MOVING_SPEED,
		SST_BAS_FLAGS,
		SST_BAS_FLAGS2,
		SST_BAS_PLAYBACK_SPEED,
		SST_BAS_BOUNDING_BOX,
		SST_BAS_BOUNDING_SPHERE,
	};
	enum SST_ANIMATION_BLOCK
	{
		SST_AB_INTERPOLATION_TYPE=1,
		SST_AB_GLOBALSEQUENCE_ID,
		SST_AB_ANIMATION_FRAME,		
	};
	enum SST_ANIMATION_FRAME
	{
		SST_AF_TIMESTAMPS=1,
		SST_AF_KEYS,
	};
	enum SST_COLOR_ANIMATION
	{
		SST_CA_COLOR_ANIMATION=1,
		SST_CA_ALPHA_ANIMATION,
	};
	enum SST_TRANSPARENCY_ANIMATION
	{
		SST_TA_ALPHA_ANIMATION=1,
	};
	enum SST_TEXTURE_UV_ANIMATION
	{
		SST_TUVA_TRANSLATIONS=1,
		SST_TUVA_ROTATIONS,
		SST_TUVA_SCALINGS,
	};
	enum SST_BONE_INFO
	{
		SST_BI_ANIMATION_SEQ=1,
		SST_BI_FLAGS,
		SST_BI_PARENT_BONE,
		SST_BI_GEOSET_ID,	
		SST_BI_PIVOT_POINT,
		SST_BI_TRANSLATIONS,
		SST_BI_ROTATIONS,
		SST_BI_SCALINGS,	
	};
	enum SST_ATTACHMENT_INFO
	{
		SST_AI_ID=1,
		SST_AI_BONE,
		SST_AI_NAME,
		SST_AI_POSITION,
	};
	enum SST_PARTICLE_EMITTER_INFO
	{
		SST_PEI_FLAGS=1,
		SST_PEI_POSITION,
		SST_PEI_BONE,
		SST_PEI_TEXTURE,
		SST_PEI_BLENDING_TYPE,
		SST_PEI_EMITTER_TYPE,
		SST_PEI_PARTICLE_COLOR_REF,
		SST_PEI_PARTICLE_TYPE,
		SST_PEI_HEAD_OR_TAIL,
		SST_PEI_TEXTURE_TILE_ROTATION,
		SST_PEI_TEXTURE_ROWS,
		SST_PEI_TEXTURE_COLS,
		SST_PEI_SCALE,
		SST_PEI_SLOWDOWN,
		SST_PEI_ROTATION,
		SST_PEI_TRANS,
		SST_PEI_FX,
		SST_PEI_SPEED,
		SST_PEI_SPEED_VARIATION,
		SST_PEI_VERTICAL_RANGE,
		SST_PEI_HORIZONTAL_RANGE,
		SST_PEI_GRAVITY,
		SST_PEI_LIFE_SPAN,
		SST_PEI_EMISSION_RATE,
		SST_PEI_EMISSION_AREA_LENGTH,
		SST_PEI_EMISSION_AREA_WIDTH,
		SST_PEI_DECELERATE,
		SST_PEI_PARTICLE_COLOR,
		SST_PEI_PARTICLE_OPACITY,
		SST_PEI_PARTICLE_SIZES,
		SST_PEI_INTENSITY,
		
	};
	enum SST_RIBBON_EMITTER_INFO
	{			
		SST_REI_BONE_ID=1,
		SST_REI_POSITION,
		SST_REI_TEXTURE,
		SST_REI_FX,
		SST_REI_IS_TRANSPARENT,
		SST_REI_RESOLUTION,
		SST_REI_LENGTH,
		SST_REI_EMISSION_ANGLE,
		SST_REI_COLOR,
		SST_REI_OPACITY,
		SST_REI_ABOVE,
		SST_REI_BELOW,
	};
	
	struct BONE_ANI_CACHE_FRAME
	{
		int							BoneID;
		CEasyArray<CD3DMatrix>		Matrix;		
	};


	struct BONE_ANI_CACHE
	{
		int									AniIndex;
		CEasyArray<BONE_ANI_CACHE_FRAME>	Bones;
	};

		

	CEasyArray<ANIMATION_SEQUENCE>				m_AnimationSequence;
	CEasyArray<COLOR_ANIMATION_INFO>			m_ColorAnimations;
	CEasyArray<TRANSPARENCY_ANIMATION_INFO>		m_TransparencyAnimations;	
	CEasyArray<TEXTURE_UV_ANI>					m_TextureUVAnimations;

	CEasyArray<MODEL_BONE>						m_Bones;
	UINT										m_SkinMeshBoneCount;
	CEasyArray<short>							m_KeyBoneIndex;

	CEasyArray<UINT>							m_GlobalSequences;

	CEasyArray<MODEL_ATTACHMENT>				m_Attachments;

	CEasyArray<PARTICLE_EMITTER_INFO>			m_ParticleEmitterInfos;
	CEasyArray<RIBBON_EMITTER_INFO>				m_RibbonEmitterInfos;

	bool										m_HasBoneAni;
	bool										m_HasMaterialAni;
	UINT										m_BoneAniCacheSize;
	UINT										m_BoneAniCacheFreq;
	CEasyList<BONE_ANI_CACHE>					m_BoneAniCache;
	CEasyArray<BONE_ANI_CACHE>					m_GlobalBoneAniCache;
	

	
	DECLARE_CLASS_INFO(CD3DWOWM2ModelResource)
public:

	

	CD3DWOWM2ModelResource(void);
	CD3DWOWM2ModelResource(CD3DObjectResourceManager* pManager);	
	~CD3DWOWM2ModelResource(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	bool LoadFromFile(LPCTSTR ModelFileName);
	bool LoadFromFile(LPCTSTR ModelFileName,LPCTSTR SkinFileName);

public:	

	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);
	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);

	int GetAnimationID(LPCTSTR AnimationName);
	ANIMATION_SEQUENCE * FindAnimationInfo(UINT AnimationID,UINT SubAnimationID);
	int GetAnimationIndex(UINT AnimationID,UINT SubAnimationID);
	ANIMATION_SEQUENCE * GetAnimationInfo(UINT AnimationID,UINT SubAnimationID);
	UINT GetAnimationCount();
	ANIMATION_SEQUENCE * GetAnimationInfoByIndex(UINT AnimationIndex);

	UINT GetBoneCount();
	UINT GetSkinMeshBoneCount();
	MODEL_BONE * GetBoneInfo(UINT Index);
	int GetKeyBone(UINT Key);
	UINT GetKeyBoneCount();
	bool HasBoneAni();
	bool HasMaterialAni();

	bool MakeAnimationBoneFrame(UINT AnimationIndex,UINT Time,BONE_MATRIX * pMatrixBuffer,UINT MatrixBufferSize,bool ResetMatrix=true);
	bool MakeAnimationBoneFrameByTree(UINT AnimationIndex,UINT Time,UINT StartBone,BONE_MATRIX * pMatrixBuffer,UINT MatrixBufferSize,bool ResetMatrix=true);
	bool MakeAnimationBoneFrameByTree(ANIMATION_SEQUENCE * pAniInfo,UINT Time,UINT StartBone,BONE_MATRIX * pMatrixBuffer,UINT MatrixBufferSize,bool ResetMatrix=true);

	bool MakeColorAnimationFrame(UINT AniIndex,UINT Time,D3DCOLORVALUE& Color);
	bool MakeTransparencyAnimationFrame(UINT AniIndex,UINT Time,FLOAT& Alpha);

	MODEL_ATTACHMENT * GetAttachment(UINT ID);
	UINT GetAttachmentCount();
	MODEL_ATTACHMENT * GetAttachmentByIndex(UINT Index);

	UINT GetParticleEmitterCount();
	PARTICLE_EMITTER_INFO * GetParticleEmitterInfo(UINT Index);

	bool MakeParticleParam(UINT Index,UINT Time,PARTICLE_PARAM& Param);

	UINT GetRibbonEmitterCount();
	RIBBON_EMITTER_INFO * GetRibbonEmitterInfo(UINT Index);

	bool MakeRibbonParam(UINT Index,UINT Time,RIBBON_PARAM& Param);

	bool MakeTextureUVAniFrame(UINT Index,UINT Time,CD3DMatrix& Frame);

	void SetBoneAniCache(UINT CacheSize,UINT CacheFreq);

	UINT CaculateDataSize();
	

protected:
	void MakeSubmeshMaterial(BYTE * pModelData,BYTE * pSkinData,UINT SubMeshIndex,CD3DSubMesh * pD3DSubMesh);
	void CreateBounding();
	bool LoadBones(BYTE * pModelData,LPCTSTR szModelPath);
	bool LoadAnimationFromFile(BYTE * pData,UINT AnimationID,UINT SubAnimationID,UINT AniIndex,LPCTSTR szModelPath,M2_MODEL_BONE * pOrgBoneInfo);

	bool LoadColorAnimation(BYTE * pModelData);
	bool LoadTransparencyAnimation(BYTE * pModelData);
	bool LoadRibbonEmitters(BYTE * pModelData);
	bool LoadParticleEmitters(BYTE * pModelData);
	bool LoadTextureUVAnimation(BYTE * pModelData);

	void LoadBoneAnimation(BYTE * pData,BYTE * pAniData,MODEL_BONE& BoneInfo,M2_MODEL_BONE& OrgBoneInfo,UINT AniIndex);

	void LoadTranslationFrames(BYTE * pData,UINT TimeStampCount,UINT TimeStampOffset,UINT KeyCount,UINT KeyOffset,ANIMATION_FRAME<CD3DVector3>& AniFrame);
	void LoadRotationFrames(BYTE * pData,UINT TimeStampCount,UINT TimeStampOffset,UINT KeyCount,UINT KeyOffset,ANIMATION_FRAME<CD3DQuaternion>& AniFrame);
	void LoadScalingFrames(BYTE * pData,UINT TimeStampCount,UINT TimeStampOffset,UINT KeyCount,UINT KeyOffset,ANIMATION_FRAME<CD3DVector3>& AniFrame);

	void LoadAniBlockFloat(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<FLOAT>& AniBlock);
	void LoadAniBlockVector3(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<CD3DVector3>& AniBlock);
	void LoadAniBlockQuaternion(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<CD3DQuaternion>& AniBlock);
	void LoadAniBlockQuaternion2(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<CD3DQuaternion>& AniBlock);
	void LoadAniBlockAlpha(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<FLOAT>& AniBlock);
	void LoadAniBlockTranslation(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<CD3DVector3>& AniBlock);
	void LoadAniBlockRotation(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<CD3DQuaternion>& AniBlock);
	void LoadAniBlockScaling(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<CD3DVector3>& AniBlock);
	void LoadAniBlockBool(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<bool>& AniBlock);

	void LoadFakeAniBlockColor(BYTE * pData,M2_MODEL_FAKE_ANIMATION_BLOCK& AniBlockInfo,FAKE_ANIMATION_FRAME<CD3DVector3>& AniBlock);
	void LoadFakeAniBlockAlpha(BYTE * pData,M2_MODEL_FAKE_ANIMATION_BLOCK& AniBlockInfo,FAKE_ANIMATION_FRAME<FLOAT>& AniBlock);
	void LoadFakeAniBlockSize(BYTE * pData,M2_MODEL_FAKE_ANIMATION_BLOCK& AniBlockInfo,FAKE_ANIMATION_FRAME<CD3DVector2>& AniBlock);
	void LoadFakeAniBlockShort(BYTE * pData,M2_MODEL_FAKE_ANIMATION_BLOCK& AniBlockInfo,FAKE_ANIMATION_FRAME<short>& AniBlock);

	bool GetInterpolationValue(UINT Time,int InterpolationType,UINT AniLength,ANIMATION_FRAME<FLOAT>& AniFrame,FLOAT& Value);
	bool GetInterpolationValue(UINT Time,int InterpolationType,UINT AniLength,ANIMATION_FRAME<CD3DVector3>& AniFrame,CD3DVector3& Value);
	bool GetInterpolationValue(UINT Time,int InterpolationType,UINT AniLength,ANIMATION_FRAME<CD3DQuaternion>& AniFrame,CD3DQuaternion& Value);


	WORD RebuildVertexIndex(CEasyArray<WORD>& VertexIndexList,WORD VertexIndex);

	bool LoadAttachments(BYTE * pModelData);

	void BuildFX(CD3DSubMeshMaterial * pSubMeshMaterial);
	CD3DFX * BuildParticleFX(UINT BlendingType);
	CD3DFX * BuildRibbonFX(UINT RenderFlag,UINT BlendingMode);

	bool MakeOneAnimationBoneFrame(ANIMATION_SEQUENCE * pAniInfo,UINT Time,UINT Bone,BONE_MATRIX * pMatrix,bool UseCache);
	bool GetInterpolationTransform(UINT Time,UINT AniIndex,UINT AniLength,CD3DMatrix& TransformMatrix,ANIMATION_BLOCK<CD3DVector3>& Translations,ANIMATION_BLOCK<CD3DQuaternion>& Rotations,ANIMATION_BLOCK<CD3DVector3>& Scalings);
	bool GetInterpolationTransformGlobal(UINT Time,CD3DMatrix& TransformMatrix,ANIMATION_BLOCK<CD3DVector3>& Translations,ANIMATION_BLOCK<CD3DQuaternion>& Rotations,ANIMATION_BLOCK<CD3DVector3>& Scalings,bool ForceGlobalSequence);
	bool GetInterpolationTransformWithGlobal(UINT Time,UINT AniIndex,UINT AniLength,CD3DMatrix& TransformMatrix,ANIMATION_BLOCK<CD3DVector3>& Translations,ANIMATION_BLOCK<CD3DQuaternion>& Rotations,ANIMATION_BLOCK<CD3DVector3>& Scalings);

	bool BuildGlobalBoneAniCache();
	BONE_ANI_CACHE * BuildBoneAniCache(ANIMATION_SEQUENCE * pAniInfo);
	BONE_ANI_CACHE * FindBoneAniCache(UINT AniIndex);
	bool MakeOneAnimationBoneFrameFromCache(BONE_ANI_CACHE * pAniCache,UINT Time,UINT Bone,CD3DMatrix& TransformMatrix);


	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual bool USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);	

	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual int USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param=0);

	template<class KEY_TYPE>
	bool PackAnimationBlock(ANIMATION_BLOCK<KEY_TYPE>& AniBlock,CSmartStruct& Packet,WORD MemberID);
	template<class KEY_TYPE>
	bool PackFakeAnimationBlock(FAKE_ANIMATION_FRAME<KEY_TYPE>& AniBlock,CSmartStruct& Packet,WORD MemberID);
	UINT GetPacketAniBlockCount(CSmartStruct& Packet);
	template<class KEY_TYPE>
	bool UnpackAnimationBlock(ANIMATION_BLOCK<KEY_TYPE>& AniBlock,CSmartStruct& Packet,WORD MemberID);
	template<class KEY_TYPE>
	bool UnpackFakeAnimationBlock(FAKE_ANIMATION_FRAME<KEY_TYPE>& AniBlock,CSmartStruct& Packet,WORD MemberID);
	template<class KEY_TYPE>
	UINT GetAnimationBlockSmartStructSize(ANIMATION_BLOCK<KEY_TYPE>& AniBlock);
	template<class KEY_TYPE>
	UINT GetFakeAnimationBlockSmartStructSize(FAKE_ANIMATION_FRAME<KEY_TYPE>& AniBlock);

	void CheckAni();

	template<class KEY_TYPE>
	UINT CaculateAnimationBlockSize(ANIMATION_BLOCK<KEY_TYPE>& AniBlock);

	//void PrintBoneInfo();
	
};


inline bool CD3DWOWM2ModelResource::HasBoneAni()
{
	return m_HasBoneAni;
}

inline bool CD3DWOWM2ModelResource::HasMaterialAni()
{
	return m_HasMaterialAni;
}


}