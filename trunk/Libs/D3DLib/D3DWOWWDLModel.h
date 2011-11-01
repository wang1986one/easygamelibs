/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DWOWWDLModel.h					                        */
/*      ��������:  2011��06��08��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

#pragma comment(linker,"/include:?m_CD3DWOWWDLModelClassInfoRegister@CD3DWOWWDLModel@D3DLib@@1VCClassInfoRegister@@A")

class CD3DWOWWDLModel :
	public CD3DBaseStaticModel
{
protected:
	CD3DWOWWDLModelResource *							m_pModelResource;
	CD3DBoundingBox										m_BoundingBox;

	DECLARE_FILE_CHANNEL_MANAGER
	DECLARE_CLASS_INFO(CD3DWOWWDLModel)
public:
	CD3DWOWWDLModel(void);
	~CD3DWOWWDLModel(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	bool LoadFromFile(LPCTSTR ModelFileName);
	bool LoadFromResource(CD3DWOWWDLModelResource * pModelResource);

	void SetAreaVisible(UINT AreaX,UINT AreaY,bool IsVisible);


	CD3DWOWWDLModelResource * GetModelResource();
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
};

inline CD3DWOWWDLModelResource * CD3DWOWWDLModel::GetModelResource()
{
	return m_pModelResource;
}

}