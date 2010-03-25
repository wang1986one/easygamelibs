/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DStaticModelResource.h                                 */
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

//struct STATIC_MODEL_SUB_MESH : public SUB_MESH
//{
//	WORD MeshProperty;
//};

class CD3DStaticModelResource :
	public CD3DObjectResource
{
protected:	

	//struct STORAGE_STRUCT:public CD3DObjectResource::STORAGE_STRUCT
	//{		
	//	int					DummyCount;	
	//};
	enum SST_MEMBER_ID
	{
		SST_D3DSMR_DUMMY=SST_D3DOR_MAX,
		SST_D3DSMR_MAX=SST_D3DOR_MAX+50,
	};
	enum SST_DUMMY_INFO
	{
		SST_DI_NAME=1,
		SST_DI_TRANSFROM_MATRIX,
	};

	struct STATIC_MODEL_DUMMY
	{
		char				szName[256];
		CD3DMatrix			TransformMatrix;
	};

	DECLARE_CLASS_INFO(CD3DStaticModelResource)
public :

	enum STATIC_MODEL_SUB_MESH_PROPERTY
	{
		SMSMP_GROUND_LAYER=0x40,
	};
	struct STATIC_MODEL_VERTEXT_TEX1
	{
		CD3DVector3 Pos;
		CD3DVector3 Normal;
		CD3DVector2 Tex1;
	};
	struct STATIC_MODEL_VERTEXT_TEX2
	{
		CD3DVector3 Pos;
		CD3DVector3 Normal;
		CD3DVector2 Tex1;
		CD3DVector2 Tex2;
	};
	struct STATIC_MODEL_VERTEXT_DIFFUSE_TEX1
	{
		CD3DVector3 Pos;		
		D3DCOLOR	Diffuse;
		CD3DVector2 Tex1;		
	};
	struct STATIC_MODEL_VERTEXT_NORMAL_DIFFUSE_TEX1
	{
		CD3DVector3 Pos;
		CD3DVector3 Normal;
		D3DCOLOR	Diffuse;
		CD3DVector2 Tex1;
	};

	

protected:	
	STATIC_MODEL_DUMMY *		m_pDummys;
	int							m_DummyCount;
	

	DECLARE_FILE_PATH_MANAGER
	DECLARE_FILE_CHANNEL_MANAGER

	
public:
	CD3DStaticModelResource();
	CD3DStaticModelResource(CD3DObjectResourceManager* pManager);
	virtual ~CD3DStaticModelResource(void);

	virtual void Destory();


	bool LoadFromXFile(LPCTSTR FileName,bool UseSystemMemoryCopy=false);
	bool LoadFromMDL(LPCTSTR FileName,bool UseSystemMemoryCopy=false);
	bool LoadFromSMDL(LPCTSTR FileName,bool UseSystemMemoryCopy=false);

	bool LoadFromMDA(IFileAccessor * pFile,bool UseSystemMemoryCopy=false);

	
public:
	
	virtual int GetDummyCount();
	virtual char * GetDummyName(int Index);
	virtual CD3DMatrix * GetDummyMatrix(int Index);


	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
protected:
	

	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual bool USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);	

	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual int USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param=0);
};

}