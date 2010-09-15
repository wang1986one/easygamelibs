/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWADTModel.h                                         */
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

#pragma comment(linker,"/include:?m_CD3DWOWADTModelClassInfoRegister@CD3DWOWADTModel@D3DLib@@1VCClassInfoRegister@@A")

class CD3DWOWADTModel :
	public CD3DBaseStaticModel
{
protected:
	enum SST_MEMBER_ID
	{
		SST_D3DWAM_MODEL_RESOURCE=SST_D3DO_MAX,				
		SST_D3DWAM_MAX=SST_D3DO_MAX+50,
	};

	CD3DWOWADTModelResource *							m_pModelResource;
	CEasyArray<CD3DWOWM2Model *>						m_M2ObjectList;
	CEasyArray<CD3DWOWWMOModel *>						m_WMOObjectList;
	CD3DBoundingBox										m_BoundingBox;

	DECLARE_FILE_CHANNEL_MANAGER
	DECLARE_CLASS_INFO(CD3DWOWADTModel)
public:
	CD3DWOWADTModel(void);
	~CD3DWOWADTModel(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	bool LoadFromFile(LPCTSTR ModelFileName,bool IsBigAlphaMask);
public:
	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);
	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
public:
	virtual void PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera);
	virtual void Update(FLOAT Time);
	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(UINT index);
	virtual CD3DSubMeshMaterial * GetSubMeshMaterial(int index);

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();

	bool GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& Height,FLOAT& WaterHeight);

protected:
	bool LoadObjects();
	void BuildBoundingBox();
};

}