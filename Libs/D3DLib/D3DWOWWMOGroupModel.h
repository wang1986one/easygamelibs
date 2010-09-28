/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWWMOGroupModel.h	                                */
/*      创建日期:  2010年08月04日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

#pragma comment(linker,"/include:?m_CD3DWOWWMOGroupModelClassInfoRegister@CD3DWOWWMOGroupModel@D3DLib@@1VCClassInfoRegister@@A")

class CD3DWOWWMOGroupModel :
	public CD3DBaseStaticModel
{
protected:
	CD3DWOWWMOModelResource *							m_pModelResource;
	CD3DWOWWMOModelResource::GROUP_INFO *				m_pGroupInfo;
	UINT												m_GroupIndex;
	CEasyArray<CD3DBoard *>								m_PortalBoards;

	DECLARE_CLASS_INFO(CD3DWOWWMOGroupModel)
public:
	CD3DWOWWMOGroupModel(void);
	~CD3DWOWWMOGroupModel(void);

	virtual void Destory();

	bool LoadFromResource(CD3DWOWWMOModelResource * pModelResource,UINT GroupIndex);

	CD3DWOWWMOModelResource * GetModelResource();
	CD3DWOWWMOModelResource::GROUP_INFO * GetGroupInfo();
	UINT GetGroupIndex();

public:
	virtual void PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera);
	

	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(UINT index);
	virtual CD3DSubMeshMaterial * GetSubMeshMaterial(int index);

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();
protected:
	void CreatePortalBoard();
};

inline CD3DWOWWMOModelResource * CD3DWOWWMOGroupModel::GetModelResource()
{
	return m_pModelResource;
}

inline CD3DWOWWMOModelResource::GROUP_INFO * CD3DWOWWMOGroupModel::GetGroupInfo()
{
	return m_pGroupInfo;
}
inline UINT CD3DWOWWMOGroupModel::GetGroupIndex()
{
	return m_GroupIndex;
}

}