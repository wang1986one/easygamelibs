/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DBaseFont.cpp                                          */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\d3dbasefont.h"
#include ".\d3dfontmanager.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO_STATIC(CD3DBaseFont,CNameObject);

CD3DBaseFont::CD3DBaseFont()
{
	m_pManager=NULL;	
}

CD3DBaseFont::CD3DBaseFont(CD3DFontManager * pManager)
{
	m_pManager=pManager;	
}

CD3DBaseFont::~CD3DBaseFont(void)
{
	if(m_pManager&&GetID())
	{
		m_pManager->DeleteD3DFont(GetID());
	}
}


}