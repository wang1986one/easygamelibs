/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DBonedModelResource.h                                  */
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

struct BONED_MODEL_VERTEXT_TEX1
{
	CD3DVector3			Pos;
	FLOAT				BoneWeight[4];
	BYTE				BoneID[4];
	CD3DVector3			Normal;		
	CD3DVector2			Tex1;	
	
};
struct BONED_MODEL_VERTEXT_TEX2
{
	CD3DVector3			Pos;
	CD3DVector3			Normal;		
	CD3DVector2			Tex1;
	CD3DVector2			Tex2;
	FLOAT				BoneID[4];
	FLOAT				BoneWeight[4];
};

#define MAX_BONE_NAME_LEN	64

struct BONED_MODEL_BONE
{
	char		Name[MAX_BONE_NAME_LEN];
	int			ParentID;
	CD3DMatrix	BoneInitMatrix;
};

//struct BONED_DYNAMIC_MODEL_SUB_MESH : public SUB_MESH
//{
//	BYTE MinBoneIndex;
//	BYTE MaxBoneIndex;
//};

class CD3DBonedModelResource :
	public CD3DObjectResource
{
public :	
	

protected:
	//struct STORAGE_STRUCT:public CD3DObjectResource::STORAGE_STRUCT
	//{		
	//	int					BoneCount;	
	//};	
	enum SST_MEMEBR_ID
	{
		SST_D3DBMR_BONE=SST_D3DOR_MAX,
		SST_D3DBMR_MAX=SST_D3DOR_MAX+50,
	};
	enum SST_BONE_INFO
	{
		SST_BI_NAME=1,
		SST_BI_PARENT_ID,
		SST_BI_INIT_MATRIX,
	};
	
	BONED_MODEL_BONE *				m_pBones;
	int								m_BoneCount;
	

	DECLARE_FILE_CHANNEL_MANAGER

	DECLARE_CLASS_INFO(CD3DBonedModelResource)
public:
	CD3DBonedModelResource();
	CD3DBonedModelResource(CD3DObjectResourceManager* pManager);
	virtual ~CD3DBonedModelResource(void);

	virtual void Destory();
	

	bool LoadFromSKN(LPCTSTR FileName);
	
public:
	int GetBoneCount();
	BONED_MODEL_BONE * GetBone(int Index);	

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);

protected:	
	bool LoadSkeleton(LPCTSTR FileName);
	//void RebuildBoneIndex();

	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual bool USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);

	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual int USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param=0);
};

}