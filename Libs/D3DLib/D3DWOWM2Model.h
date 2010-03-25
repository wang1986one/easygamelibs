/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWM2Model.h                                          */
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

#pragma comment(linker,"/include:?m_CD3DWOWM2ModelClassInfoRegister@CD3DWOWM2Model@D3DLib@@1VCClassInfoRegister@@A")

class CD3DWOWM2Model :
	public CD3DBaseDynamicModel
{
protected:	

	//struct STORAGE_STRUCT:public CD3DObject::STORAGE_STRUCT
	//{		
	//	int				ModelResourceID;
	//	int				CurAnimationID;
	//	int				CurSubAnimationID;
	//	int				ActivedAttachmentCount;
	//};

	enum SST_MEMBER_ID
	{
		SST_D3DWMM_MODEL_RESOURCE=SST_D3DO_MAX,		
		SST_D3DWMM_CUR_ANI_ID,
		SST_D3DWMM_CUR_SUB_ANI_ID,
		SST_D3DWMM_ATCIVE_ATTACHMENT,
		SST_D3DWMM_MAX=SST_D3DO_MAX+50,
	};
	enum SST_ACTIVE_ATTACHMENT_INFO
	{
		SST_AAI_ATTACHMENT_ID=1,
		SST_AAI_DUMMY_STORAGE_ID,
	};
	//struct STORAGE_STRUCT_ATTACHMENT
	//{		
	//	WORD	AttachmentID;
	//	UINT	DummyStorageID;
	//};
	enum INTERIM_STATUS
	{
		IS_NONE,
		IS_INIT,
		IS_DOING,
	};

	struct ADDITIONAL_BONE_MATRIX
	{
		UINT		BoneID;
		CD3DMatrix	Matrix;
	};
	

	CD3DWOWM2ModelResource *							m_pModelResource;
	CEasyArray<CD3DWOWM2ModelResource::BONE_MATRIX>		m_BoneMatrices;
	CEasyArray<CD3DMatrix>								m_BoneMatList;
	bool												m_UseSoftSkinMesh;

	CEasyArray<CD3DMatrix>								m_InterimBoneMatStart;
	CEasyArray<CD3DMatrix>								m_InterimBoneMatEnd;
	int													m_InterimStatus;
	FLOAT												m_InterimTime;

	CEasyArray<ADDITIONAL_BONE_MATRIX>					m_AdditionalBoneMatrix;

	int													m_CurAnimationID;
	int													m_CurSubAnimationID;
	int													m_CurAnimationIndex;
	FLOAT												m_CurAnimationLength;

	bool												m_IsPlaying;
	bool												m_IsLoop;
	FLOAT												m_PlayStartTime;
	FLOAT												m_CurPlayTime;
	FLOAT												m_PlaySpeedRate;
	

	CD3DBoundingBox *									m_pCurBoundingBox;
	//STORAGE_STRUCT_ATTACHMENT *							m_pActivedAttachments;
	


	DECLARE_FILE_CHANNEL_MANAGER
	DECLARE_CLASS_INFO(CD3DWOWM2Model)
public:
	CD3DWOWM2Model(void);
	~CD3DWOWM2Model(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	bool Play(int AnimationID,int SubAnimationID,FLOAT InterimTime,bool IsLoop);
	virtual bool Play(bool IsLoop);	
	virtual bool Stop();
	virtual bool IsPlaying();
	virtual void SetPlaySpeed(FLOAT Rate);

	bool AddBoneTrans(int KeyBone,CD3DMatrix& Matrix);
	void ClearAdditionalBoneTrans();
	

	bool LoadFromFile(LPCTSTR szModelFileName);
	bool LoadFromFile(LPCTSTR ModelFileName,LPCTSTR SkinFileName);
	bool LoadFromResource(CD3DWOWM2ModelResource * pModelResource);

	
	virtual bool CloneFrom(CNameObject * pObject,UINT Param=0);
	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);

	CD3DWOWM2ModelResource * GetModelResource();

	int GetCurAnimationID();
	int GetCurSubAnimationID();

	CD3DDummy * EnableAttachment(UINT ID);

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
protected:
	virtual void PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera);
	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(int index);
	//virtual CD3DSubMesh::MESH_MATERIAL * GetSubMeshMaterial(int index);

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();

	//virtual bool RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly=true);
	//virtual bool GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y);
	

	virtual void Update(FLOAT Time);

	void PrepareSoftSkinMesh();
	void CaculateSoftSkinMesh();

	virtual void FetchAnimationFrames(UINT Time);

	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);	
	//virtual bool USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);	
	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual int USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param=0);
	//virtual bool USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param=0);
};

inline void CD3DWOWM2Model::SetPlaySpeed(FLOAT Rate)
{
	m_PlaySpeedRate=Rate;
}

inline int CD3DWOWM2Model::GetCurAnimationID()
{
	return m_CurAnimationID;
}
inline int CD3DWOWM2Model::GetCurSubAnimationID()
{
	return m_CurSubAnimationID;
}

}