/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DWOWM2RibbonEmitter.h                                  */
/*      ��������:  2009��09��25��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

class CD3DWOWM2RibbonEmitter :
	public CD3DObject
{
protected:
	enum
	{
		MAX_RIBBON_COUNT=50000,
	};
	struct RIBBON_VERTEX
	{
		CD3DVector3	Pos;
		D3DCOLOR	Color;
		CD3DVector2	Tex;
	};
	struct RIBBON_PAIR
	{
		RIBBON_VERTEX	Vertex[2];
	};
	CD3DWOWM2ModelResource *	m_pModelResource;
	CD3DSubMesh					m_SubMesh;
	UINT						m_EmitterIndex;
	UINT						m_MaxRibbonCount;	
	RIBBON_PAIR *				m_pRibbonVertexBuffer;
	UINT						m_RibbonCount;
	FLOAT						m_StartTime;	
	CD3DVector3					m_RecentCreatePos;
	FLOAT						m_ClipLength;
	FLOAT						m_RecentCreateTime;
	FLOAT						m_RecentClipTime;

	DECLARE_CLASS_INFO(CD3DWOWM2RibbonEmitter)
public:
	CD3DWOWM2RibbonEmitter(void);
	~CD3DWOWM2RibbonEmitter(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(UINT index);

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();

	virtual bool CanDoSubMeshViewCull();

	virtual bool CloneFrom(CNameObject * pObject,UINT Param=0);

	virtual void PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera);


	virtual void Update(FLOAT Time);

	bool Init(CD3DWOWM2ModelResource * pModelResource,UINT EmitterIndex);
protected:
	void BuildRibbon(CD3DWOWM2ModelResource::RIBBON_EMITTER_INFO * pRibbonEmitterInfo,CD3DWOWM2ModelResource::RIBBON_PARAM * pParam);
	void DelRibbon();
};

}