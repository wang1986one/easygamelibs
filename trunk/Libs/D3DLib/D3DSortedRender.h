/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DSortedRender.h                                        */
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

	DECLARE_CLASS_INFO(CD3DSortedRender)
	
public:
	CD3DSortedRender(void);
	virtual ~CD3DSortedRender(void);

	virtual void Render();
	virtual void Update(FLOAT Time);

	void InsertSubMeshSorted(CEasyList<SORT_DATA>& SubMeshList,SORT_DATA& SortData);


};

}
