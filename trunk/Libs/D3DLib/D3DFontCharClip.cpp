/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DFontCharClip.cpp                                      */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DFontCharClip,CD3DObject);

CD3DFontCharClip::CD3DFontCharClip(void):CD3DObject()
{

	m_SubMesh.GetVertexFormat().FVF=D3DFVF_RECTVERTEX;
	m_SubMesh.GetVertexFormat().VertexSize=sizeof(RECTVERTEX);
	m_SubMesh.GetVertexFormat().IndexSize=sizeof(WORD);
	m_SubMesh.SetVertices((BYTE *)m_Vertexs);
	m_SubMesh.SetRenderBufferUsed(CD3DSubMesh::BUFFER_USE_CUSTOM);
	m_SubMesh.SetVertexCount(4);
	m_SubMesh.SetPrimitiveType(D3DPT_TRIANGLESTRIP);
	m_SubMesh.SetPrimitiveCount(2);	

	m_Rect.left=0;
	m_Rect.top=0;
	m_Rect.right=100;
	m_Rect.bottom=100;
	m_TextureRect.left=0;
	m_TextureRect.top=0;
	m_TextureRect.right=100;
	m_TextureRect.bottom=100;
	

	m_VertexColor=D3DCOLOR_XRGB(255, 255, 255);	
}

CD3DFontCharClip::~CD3DFontCharClip(void)
{
	//m_SubMesh.Destory();
}


void CD3DFontCharClip::CreateVertex()
{
	FLOAT x1,y1,tx1,ty1;
	FLOAT x2,y2,tx2,ty2;		

	x1=(FLOAT)m_Rect.left;
	y1=(FLOAT)m_Rect.top;	
	x2=(FLOAT)m_Rect.right;
	y2=(FLOAT)m_Rect.bottom;
	if(m_SubMesh.GetMaterial().GetTexture(0))
	{
		tx1=(FLOAT)m_TextureRect.left/(m_SubMesh.GetMaterial().GetTexture(0)->GetWidth()-1);
		ty1=(FLOAT)m_TextureRect.top/(m_SubMesh.GetMaterial().GetTexture(0)->GetHeight()-1);
		tx2=(FLOAT)m_TextureRect.right/(m_SubMesh.GetMaterial().GetTexture(0)->GetWidth()-1);
		ty2=(FLOAT)m_TextureRect.bottom/(m_SubMesh.GetMaterial().GetTexture(0)->GetHeight()-1);
	}
	else
	{
		tx1=0;tx2=0;
		ty1=0;ty2=0;
	}
	m_Vertexs[0] = InitRECTVertex( D3DXVECTOR4(x1,y1,0.9f,1.0f), m_VertexColor, tx1, ty1 );
	m_Vertexs[1] = InitRECTVertex( D3DXVECTOR4(x2,y1,0.9f,1.0f), m_VertexColor, tx2, ty1 );
	m_Vertexs[2] = InitRECTVertex( D3DXVECTOR4(x1,y2,0.9f,1.0f), m_VertexColor, tx1, ty2 );
	m_Vertexs[3] = InitRECTVertex( D3DXVECTOR4(x2,y2,0.9f,1.0f), m_VertexColor, tx2, ty2 );

}

void CD3DFontCharClip::SetRect(RECT& rect)
{
	m_Rect=rect;	
}

void CD3DFontCharClip::SetTexture(CD3DTexture * pTexture,RECT * pTextureRect)
{

	
	if(!m_SubMesh.GetMaterial().SetTexture(0,pTexture))
		m_SubMesh.GetMaterial().AddTexture(pTexture,0);
	if(pTextureRect)
		m_TextureRect=*pTextureRect;
	else
	{
		m_TextureRect.left=0;
		m_TextureRect.top=0;
		m_TextureRect.right=pTexture->GetWidth();
		m_TextureRect.bottom=pTexture->GetHeight();	
	}
	
}


void CD3DFontCharClip::SetVertexColor(DWORD Color)
{
	m_VertexColor=Color;
	
}

bool CD3DFontCharClip::MoveToTop()
{
	return GetRender()->MoveToTop(this);	
}

bool CD3DFontCharClip::MoveToTop(CD3DObject *before)
{
	return GetRender()->MoveToTop(this,before);
}


}