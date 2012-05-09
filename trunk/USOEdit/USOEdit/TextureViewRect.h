/****************************************************************************/
/*                                                                          */
/*      �ļ���:    TextureViewRect.h                                        */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      �ļ���:    TextureViewRect.h                                        */
/*      ��������:  2010��04��30��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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