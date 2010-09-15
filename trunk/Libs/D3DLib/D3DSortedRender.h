/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DSortedRender.h                                        */
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

class CD3DSortedRender :
	public CD3DRender
{
protected:
	struct SORT_DATA
	{			
		FLOAT			ZMin;
		FLOAT			ZMax;
		CD3DObject *	pObj;
		CD3DSubMesh *	pSubMesh;
		CD3DSubMeshMaterial * pMaterial;
	};		
	typedef std::multimap<FLOAT,SORT_DATA> CSubMeshSortMap;
	typedef std::pair<FLOAT,SORT_DATA> CSubMeshSortMapPair;

	CEasyArray<SORT_DATA>	m_TransparentSubMeshList;
	CD3DTexture *			m_pDepthTexture;

	DECLARE_CLASS_INFO(CD3DSortedRender)
	
public:
	CD3DSortedRender(void);
	virtual ~CD3DSortedRender(void);
	virtual void Destory();

	virtual void Render();
	virtual void Update(FLOAT Time);

	void InsertSubMeshSorted(CEasyList<SORT_DATA>& SubMeshList,SORT_DATA& SortData);

	bool CreateDepthTexture(UINT Size);
	virtual CD3DTexture * GetDepthTexture();


	void RenderDepthTexture();


};

inline CD3DTexture * CD3DSortedRender::GetDepthTexture()
{
	return m_pDepthTexture;
}

}
