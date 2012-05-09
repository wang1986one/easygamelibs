/****************************************************************************/
/*                                                                          */
/*      �ļ���:    TextureViewRect.cpp                                      */
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
/*      �ļ���:    TextureViewRect.cpp                                      */
/*      ��������:  2010��04��30��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"



CTextureViewRect::CTextureViewRect(void)
{
	m_SubMesh.GetVertexFormat().FVF=D3DFVF_XYZRHW|D3DFVF_TEX1;
	m_SubMesh.GetVertexFormat().VertexSize=sizeof(VERTEX);
	m_SubMesh.GetVertexFormat().IndexSize=sizeof(WORD);
	m_SubMesh.SetVertices((BYTE *)m_Vertices);
	m_SubMesh.SetRenderBufferUsed(CD3DSubMesh::BUFFER_USE_CUSTOM);
	m_SubMesh.SetVertexCount(4);
	m_SubMesh.SetPrimitiveType(D3DPT_TRIANGLESTRIP);
	m_SubMesh.SetPrimitiveCount(2);

	ZeroMemory(&m_Vertices,sizeof(m_Vertices));
}

CTextureViewRect::~CTextureViewRect(void)
{
	
}

void CTextureViewRect::Update(FLOAT Time)
{
	for(UINT i=0;i<m_SubMesh.GetMaterial().GetTextureLayerCount();i++)
	{
		m_SubMesh.GetMaterial().GetTexture(i)->Update(Time);
	}
}


void CTextureViewRect::SetTexture(CD3DTexture * pTexture)
{
	m_SubMesh.GetMaterial().ClearAllTexture();
	if(pTexture)
	{
		m_SubMesh.GetMaterial().AddTexture(pTexture,0,"","");
		FLOAT Width,Height;
		if(pTexture->IsKindOf(GET_CLASS_INFO(CD3DIFLTexture)))
		{
			CD3DIFLTexture * pIFLTexture=(CD3DIFLTexture *)pTexture;
			Width=(FLOAT)pIFLTexture->GetFrameTexture(0)->GetTextureInfo().Width;
			Height=(FLOAT)pIFLTexture->GetFrameTexture(0)->GetTextureInfo().Height;
		}
		else
		{
			Width=(FLOAT)pTexture->GetTextureInfo().Width;
			Height=(FLOAT)pTexture->GetTextureInfo().Height;
		}
		
		
		m_Vertices[0].Vertex.SetValue(0.0f,0.0f,0.9f,1.0f);
		m_Vertices[0].Tex.SetValue(0.0f,0.0f);

		m_Vertices[1].Vertex.SetValue(Width,0.0f,0.9f,1.0f);
		m_Vertices[1].Tex.SetValue(1.0f,0.0f);

		m_Vertices[2].Vertex.SetValue(0.0f,Height,0.9f,1.0f);
		m_Vertices[2].Tex.SetValue(0.0f,1.0f);

		m_Vertices[3].Vertex.SetValue(Width,Height,0.9f,1.0f);
		m_Vertices[3].Tex.SetValue(1.0f,1.0f);
		
	}
}

