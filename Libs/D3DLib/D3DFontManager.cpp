/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DFontManager.cpp                                       */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


namespace D3DLib{

IMPLEMENT_CLASS_INFO_STATIC(CD3DFontManager,CNameObject);

CD3DFontManager::CD3DFontManager(CD3DDevice * pD3DDevice,int StorageSize)
{
	m_pD3DDevice=pD3DDevice;
	m_FontStorage.Create(StorageSize);
}

CD3DFontManager::~CD3DFontManager(void)
{
#ifdef _DEBUG
	void * Pos;

	Pos=m_FontStorage.GetFirstObjectPos();
	while(Pos)
	{
		char Msg[256];
		CD3DBaseFont * pFont=*(m_FontStorage.GetNext(Pos));
		sprintf_s(Msg,256,"字体<%s>未释放！\r\n",(LPCTSTR)pFont->GetName());
		OutputDebugString(Msg);
	}
	m_FontStorage.Destory();
#endif
}

bool CD3DFontManager::Reset()
{
	void * Pos;

	Pos=m_FontStorage.GetFirstObjectPos();
	while(Pos)
	{		
		CD3DBaseFont * pFont=*(m_FontStorage.GetNext(Pos));
		pFont->Reset();
	}
	return true;
}

bool CD3DFontManager::Restore()
{
	void * Pos;

	Pos=m_FontStorage.GetFirstObjectPos();
	while(Pos)
	{		
		CD3DBaseFont * pFont=*(m_FontStorage.GetNext(Pos));
		pFont->Restore();
	}
	return true;
}

bool CD3DFontManager::AddD3DFont(CD3DBaseFont * pFont)
{
	LOGFONT LogFont;
	CEasyString FontName;

	if(pFont==NULL)
		return false;
	pFont->GetLogFont(&LogFont);
	FontName=GetNameFromLogFont(&LogFont);
	return AddD3DFont(pFont,FontName);
	
}

bool CD3DFontManager::AddD3DFont(CD3DBaseFont * pFont,LPCTSTR FontName)
{
	UINT ID=m_FontStorage.AddObject(pFont,FontName);
	if(ID)
	{
		pFont->SetID(ID);
		pFont->SetName(FontName);
		return true;
	}
	else
	{
		return false;
	}
}

bool CD3DFontManager::DeleteD3DFont(UINT ID)
{
	CD3DBaseFont * pFont=*(m_FontStorage.GetObject(ID));
	if(pFont)
	{
		m_FontStorage.DeleteObject(ID);		
		return true;
	}
	return false;
}

bool CD3DFontManager::DeleteD3DFont(LPCTSTR FontName)
{
	CD3DBaseFont * pFont=*(m_FontStorage.GetObject(FontName));
	if(pFont)
	{
		m_FontStorage.DeleteObject(pFont->GetID());		
		return true;
	}
	return false;
}

CD3DBaseFont * CD3DFontManager::CreateDX9Font(const LOGFONT * pLogFont,int MipLevels)
{
	if(pLogFont==NULL)
		return false;	
	
	CEasyString	FontName=GetNameFromLogFont(pLogFont);

	CD3DDX9Font * pFont=(CD3DDX9Font *)GetFont(FontName);
	if(pFont)
	{
		pFont->AddUseRef();
		return pFont;
	}

	pFont=new CD3DDX9Font(this);

	if(pFont->Create(pLogFont,MipLevels))
	{
		if(AddD3DFont(pFont,FontName))
			return pFont;
	}
	delete pFont;
	return NULL;
}

CD3DBaseFont * CD3DFontManager::CreateFontCN(const LOGFONT * pLogFont,int MipLevels,int BufferSize)
{
	if(pLogFont==NULL)
		return false;	

	CEasyString	FontName=GetNameFromLogFont(pLogFont);

	CD3DFontCN * pFont=(CD3DFontCN *)GetFont(FontName);
	if(pFont)
	{
		pFont->AddUseRef();
		return pFont;
	}

	pFont=new CD3DFontCN(this);

	if(pFont->Create(pLogFont,MipLevels,BufferSize))
	{
		if(AddD3DFont(pFont,FontName))
			return pFont;
	}
	delete pFont;
	return NULL;
}

CEasyString CD3DFontManager::GetNameFromLogFont(const LOGFONT * pLogFont)
{
	CEasyString Name;
	Name.Format("%s:%d:%d%d%d%d%d%d%d",
		pLogFont->lfFaceName,
		pLogFont->lfHeight,
		pLogFont->lfWidth,	
		pLogFont->lfWeight,
		pLogFont->lfCharSet,
		pLogFont->lfItalic,
		pLogFont->lfOutPrecision,
		pLogFont->lfPitchAndFamily,
		pLogFont->lfQuality
	);
	return Name;
}

int CD3DFontManager::GetCount()
{
	return m_FontStorage.GetObjectCount();
}

LPVOID CD3DFontManager::GetFirstPos()
{
	return m_FontStorage.GetFirstObjectPos();
}

LPVOID CD3DFontManager::GetLastPos()
{
	return m_FontStorage.GetLastObjectPos();
}

CD3DBaseFont * CD3DFontManager::GetNext(LPVOID& Pos)
{
	return *m_FontStorage.GetNext(Pos);
}

CD3DBaseFont * CD3DFontManager::GetPrev(LPVOID& Pos)
{
	return *m_FontStorage.GetPrev(Pos);
}

}