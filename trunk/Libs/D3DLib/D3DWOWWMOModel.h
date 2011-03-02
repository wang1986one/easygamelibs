/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DWOWWMOModel.h                                         */
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

#pragma comment(linker,"/include:?m_CD3DWOWWMOModelClassInfoRegister@CD3DWOWWMOModel@D3DLib@@1VCClassInfoRegister@@A")

class CD3DWOWWMOModel :
	public CD3DBaseStaticModel
{
protected:
	enum SST_MEMBER_ID
	{
		SST_D3DWWM_MODEL_RESOURCE=SST_D3DO_MAX,		
		SST_D3DWWM_CUR_DOODAD_SET,
		SST_D3DWWM_MAX=SST_D3DO_MAX+50,
	};


	CD3DWOWWMOModelResource *							m_pModelResource;
	CEasyArray<CD3DWOWWMOGroupModel *>					m_GroupList;
	CEasyArray<CD3DWOWDoodadModel *>					m_DoodadList;
	int													m_CurDoodadSet;

	DECLARE_FILE_CHANNEL_MANAGER
	DECLARE_CLASS_INFO(CD3DWOWWMOModel)
public:
	CD3DWOWWMOModel(void);
	~CD3DWOWWMOModel(void);

	virtual void Destory();
	

	bool LoadFromFile(LPCTSTR ModelFileName);
	bool LoadFromResource(CD3DWOWWMOModelResource * pModelResource);

	UINT GetDoodadSetCount();
	bool ShowDoodads(UINT SetIndex);
	void ClearDoodads();
	int GetCurDoodadSet();

	CD3DWOWWMOModelResource * GetModelResource();

	void DoPortalCull(CD3DCamera * pCamera);

public:
	virtual bool CloneFrom(CNameObject * pObject,UINT Param=0);

	virtual void PickResource(CUSOResourceManager * pResourceManager,UINT Param=0);
	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);

	virtual void OnPrepareRender(CD3DBaseRender * pRender,CD3DFX * pFX,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera);
	virtual void OnPrepareRenderSubMesh(CD3DBaseRender * pRender,CD3DFX * pFX,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera);

	virtual void Update(FLOAT Time);
	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(UINT index);
	virtual CD3DSubMeshMaterial * GetSubMeshMaterial(UINT index);

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();

	bool GetHeightByXZ(const CD3DVector3& Pos,FLOAT MinHeight,FLOAT MaxHeight,FLOAT& Height,FLOAT& WaterHeight);

protected:
	void BuildGroups();
	void GetGroupByPos(const CD3DVector3& Pos,CEasyArray<CD3DWOWWMOGroupModel *>& Groups);
};

inline int CD3DWOWWMOModel::GetCurDoodadSet()
{
	return m_CurDoodadSet;
}

inline CD3DWOWWMOModelResource * CD3DWOWWMOModel::GetModelResource()
{
	return m_pModelResource;
}

}