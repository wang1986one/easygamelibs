/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DFrameDynamicModel.h                                   */
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

#pragma comment(linker,"/include:?m_CD3DFrameDynamicModelClassInfoRegister@CD3DFrameDynamicModel@D3DLib@@1VCClassInfoRegister@@A")

class CD3DFrameDynamicModel :
	public CD3DBaseDynamicModel
{
protected:
	//struct STORAGE_STRUCT:public CD3DObject::STORAGE_STRUCT
	//{		
	//	int				FrameCount;
	//	int				PlayStatus;
	//};
	enum SST_MEMBER_ID
	{
		SST_D3DFDM_FRAME_INFO=SST_D3DO_MAX,
		SST_D3DFDM_BOUDING_BOX,
		SST_D3DFDM_BOUDING_SPHERE,
		SST_D3DFDM_PLAY_STATUS,
		SST_D3DFDM_MAX=SST_D3DO_MAX+50,
	};
	enum SST_FRAME_INFO
	{
		SST_FI_START_TIME=1,
		SST_FI_END_TIME,
		SST_FI_MODEL_RESOURCE,
	};

	//struct STORAGE_FRAME_STRUCT
	//{
	//	float						StartTime;
	//	float						EndTime;
	//	int							ResourceID;
	//};

	struct FRAME_DATA
	{
		FLOAT						StartTime;
		FLOAT						EndTime;
		CD3DStaticModelResource*	pModelResource;		
	};
	enum ANI_PLAY_MODE
	{
		PLAY_STOP,
		PLAY_ONCE,
		PLAY_LOOP,
	};

	CEasyArray<FRAME_DATA>		m_AniFrames;
	int							m_CurFrameIndex;
	int							m_PlayStatus;
	float						m_PlayStartTime;
	float						m_TotalFrameTime;
	CD3DBoundingBox				m_BoundingBox;
	CD3DBoundingSphere			m_BoundingSphere;

	DECLARE_CLASS_INFO(CD3DFrameDynamicModel)
	DECLARE_FILE_CHANNEL_MANAGER
public:
	CD3DFrameDynamicModel(void);
	virtual ~CD3DFrameDynamicModel(void);

	virtual void Destory();
	virtual bool Reset();
	virtual bool Restore();

	virtual bool Play(bool IsLoop);	
	virtual bool Stop();
	virtual bool IsPlaying();

	bool LoadFromMDA(LPCTSTR FileName);

	
	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);
	virtual bool CloneFrom(CNameObject * pObject,UINT Param=0);

	int GetFrameCount();
	CD3DStaticModelResource* GetFrameModelResource(int Index);

public:
	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(UINT index);	

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();

	virtual bool RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly);
	virtual bool GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y);

	virtual void Update(FLOAT Time);

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
protected:
	void BulidBouding();

	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual bool USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);

	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual int USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param=0);
	//virtual bool USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param=0);
};

}