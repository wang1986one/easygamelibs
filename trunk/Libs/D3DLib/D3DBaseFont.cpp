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
}

void CD3DBaseFont::Release()
{
	DecUseRef();
	if(GetUseRef()<=0)
	{
		m_pManager->DeleteD3DFont(GetID());
		delete this;
	}
}

}