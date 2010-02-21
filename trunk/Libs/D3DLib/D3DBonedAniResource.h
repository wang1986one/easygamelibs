#pragma once

namespace D3DLib{

struct BONE_TRANS
{	
	CD3DVector3		Pos;
	CD3DQuaternion	Rotation;
	CD3DVector3		Scale;
};

typedef CEasyArray<BONE_TRANS> CBoneTransList;

struct ANI_FRAME
{
	FLOAT			Time;
	CBoneTransList	BonesTrans;
};

class CD3DBonedAniResource :
	public CD3DObjectResource
{
protected:
	//struct STORAGE_STRUCT:public CNameObject::STORAGE_STRUCT
	//{		
	//	int				FrameCount;
	//};
	enum SST_MEMBER_ID
	{
		SST_D3DBAR_FRAME=SST_D3DOR_MAX,
		SST_D3DBAR_MAX=SST_D3DOR_MAX+50,
	};

	enum SST_FRAME_INFO
	{
		SST_FI_TIME=1,
		SST_FI_TRANSFORM,
	};

	enum SST_TRANSFORM_INFO
	{
		SST_TI_TRANSLATION=1,
		SST_TI_ROTATION,
		SST_TI_SCALING,
	};

	//struct STORAGE_FRAME_STRUCT
	//{
	//	FLOAT	Time;
	//	int		BoneCount;
	//};

	CEasyArray<ANI_FRAME>	m_Frames;
	DECLARE_FILE_CHANNEL_MANAGER

	DECLARE_CLASS_INFO(CD3DBonedAniResource)
public:
	CD3DBonedAniResource();
	CD3DBonedAniResource(CD3DObjectResourceManager* pManager);
	virtual ~CD3DBonedAniResource(void);
	
	virtual void Destory();

	bool LoadFromACT(LPCTSTR FileName,bool Reverse=false);
	
public:
	FLOAT GetAnimationLen();
	int GetBoneCount();

	bool GetBoneMatrix(FLOAT Time,int BoneID,CD3DMatrix& Mat);

	int GetBoneFrame(FLOAT Time,CD3DMatrix * pMats,BONE_TRANS * pTrans);

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);

protected:
	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);	
	//virtual bool USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual bool USOWriteChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//
	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual int USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param=0);
	//virtual bool USOReadChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual bool USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param=0);
};

}