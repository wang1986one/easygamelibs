/****************************************************************************/
/*                                                                          */
/*      文件名:    TextureViewRect.h                                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    TextureViewRect.h                                        */
/*      创建日期:  2010年04月30日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CTextureViewRect :
	public CD3DObject
{
protected:
	struct VERTEX
	{ 
		CD3DVector4 Vertex;   
		CD3DVector2 Tex; 
	};
	CD3DSubMesh			m_SubMesh;
	VERTEX				m_Vertices[4];

public:
	CTextureViewRect(void);
	~CTextureViewRect(void);

	virtual int GetSubMeshCount()
	{
		return 1;
	}
	virtual CD3DSubMesh * GetSubMesh(UINT index)
	{
		return &m_SubMesh;
	}
	virtual CD3DSubMeshMaterial * GetSubMeshMaterial(int index)
	{
		return &(m_SubMesh.GetMaterial());
	}

	virtual void Update(FLOAT Time);

	void SetTexture(CD3DTexture * pTexture);
	CD3DTexture * GetTexture();
};

inline CD3DTexture * CTextureViewRect::GetTexture()
{
	return m_SubMesh.GetMaterial().GetTexture(0);
}