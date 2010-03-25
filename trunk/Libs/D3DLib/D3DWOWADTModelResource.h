/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWADTModelResource.h                                 */
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

class CD3DWOWADTModelResource :
	public CD3DObjectResource
{
protected:
	struct MODEL_VERTEXT
	{
		CD3DVector3			Pos;
		CD3DVector3			Normal;
		D3DCOLOR			Color;
		CD3DVector2			Tex;		
	};

	struct TEXTURE_INFO
	{
		BYTE						TextureIndex;
		UINT						Flag;
		CSmartPtr<CD3DTexture>		pAlphaMap;
	};

	enum ALPHA_DATA_TYPE
	{
		ADT_UNCOMPRESS_2048,
		ADT_UNCOMPRESS_4096,
		ADT_COMPRESS,
	};

	struct TEXTURE_LAYER_INFO
	{
		BYTE						LayerCount;
		TEXTURE_INFO				Layers[BLZ_ADT_TEXTURE_LAYER_COUNT];
		CSmartPtr<CD3DTexture>		pShadowMap;
		TEXTURE_LAYER_INFO()
		{
			LayerCount=0;
			pShadowMap=NULL;
		}
	};
public:
	struct M2_OBJECT_INFO
	{
		UINT						ID;
		CD3DWOWM2ModelResource *	pModelResource;
		CD3DVector3					Position;
		CD3DQuaternion				Orientation;
		FLOAT						Scale;
	};
	struct WMO_OBJECT_INFO
	{
		UINT						ID;
		CD3DWOWWMOModelResource *	pModelResource;
		CD3DVector3					Position;
		CD3DQuaternion				Orientation;
		UINT						DoodadSet;
	};

protected:

	enum SST_MEMBER_ID
	{
		SST_D3DWAMR_M2_OBJECT_LIST=SST_D3DOR_MAX,
		SST_D3DWAMR_WMO_OBJECT_LIST,
		SST_D3DWAMR_MAX=SST_D3DOR_MAX+50,
	};

	enum SST_M2_OBJECT_LIST
	{
		SST_MOL_M2_OBJECT_INFO=1,
	};

	enum SST_M2_OBJECT_INFO
	{
		SST_MOI_ID=1,
		SST_MOI_MODEL_RESOURCE,
		SST_MOI_POSITION,
		SST_MOI_ORIENTATION,
		SST_MOI_SCALE,
	};

	enum SST_WMO_OBJECT_LIST
	{
		SST_WOL_WMO_OBJECT_INFO=1,
	};

	enum SST_WMO_OBJECT_INFO
	{
		SST_WOI_ID=1,
		SST_WOI_MODEL_RESOURCE,
		SST_WOI_POSITION,
		SST_WOI_ORIENTATION,
		SST_WOI_DOODAD_SET,
	};

	

	
	CEasyArray<M2_OBJECT_INFO>		m_M2ObjectList;
	CEasyArray<WMO_OBJECT_INFO>		m_WMOObjectList;
	
	DECLARE_CLASS_INFO(CD3DWOWADTModelResource)
public:
	CD3DWOWADTModelResource(void);
	CD3DWOWADTModelResource(CD3DObjectResourceManager* pManager);
	~CD3DWOWADTModelResource(void);


	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	bool LoadFromFile(LPCTSTR ModelFileName);

	UINT GetM2ObjectCount();
	M2_OBJECT_INFO * GetM2ObjectInfo(UINT Index);

	UINT GetWMOObjectCount();
	WMO_OBJECT_INFO * GetWMOObjectInfo(UINT Index);
public:	

	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);
	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
protected:
	CD3DFX * BuildFX(UINT Index,UINT LayCount,bool HaveShadowMap);
	bool LoadAlphaLayer(TEXTURE_LAYER_INFO& LayInfo,UINT LayerCount,BLZ_CHUNK_MCLY * pMCLY,BLZ_CHUNK_MCAL * pMCAL);
	CD3DTexture * CreateAlphaMap(int DataType,LPBYTE pData,UINT DataSize,UINT& ProcessSize);
	bool LoadShadowMap(TEXTURE_LAYER_INFO& LayInfo,BLZ_CHUNK_MCSH * pMCSH);
};


}