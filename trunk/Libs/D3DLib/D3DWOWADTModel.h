/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DWOWADTModel.h                                         */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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
	bool LoadFromResource(CD3DWOWADTModelResource * pModelResource);

	CD3DWOWADTModelResource * GetModelResource();
public:
	virtual void PickResource(CUSOResourceManager * pResourceManager,UINT Param=0);
	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
public:
	virtual void OnPrepareRender(CD3DBaseRender * pRender,CD3DFX * pFX,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera);
	virtual void OnPrepareRenderSubMesh(CD3DBaseRender * pRender,CD3DFX * pFX,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera);
	virtual void Update(FLOAT Time);
	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(UINT index);
	virtual CD3DSubMeshMaterial * GetSubMeshMaterial(UINT index);

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();

	bool GetHeightByXZ(const CD3DVector3& Pos,FLOAT MinHeight,FLOAT MaxHeight,FLOAT& Height,FLOAT& WaterHeight,bool IncludeChild=true);

protected:
	bool LoadObjects();
	void BuildBoundingBox();
};

inline CD3DWOWADTModelResource * CD3DWOWADTModel::GetModelResource()
{
	return m_pModelResource;
}
}