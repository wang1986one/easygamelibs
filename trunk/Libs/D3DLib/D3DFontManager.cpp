/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DFontManager.cpp                                       */
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
		CEasyString Key;
		CD3DBaseFont * pFont=*(m_FontStorage.GetNextObject(Pos,Key));
		PrintSystemLog(0,_T("����<%s>δ�ͷţ�"),(LPCTSTR)pFont->GetName());
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
		CEasyString Key;
		CD3DBaseFont * pFont=*(m_FontStorage.GetNextObject(Pos,Key));
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
		CEasyString Key;
		CD3DBaseFont * pFont=*(m_FontStorage.GetNextObject(Pos,Key));
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
	CEasyString Key=FontName;
	Key.MakeUpper();
	UINT ID=m_FontStorage.Insert(Key,pFont);
	if(ID)
	{
		pFont->SetID(ID);
		pFont->SetName(FontName);
		return true;
	}
	else
	{
		PrintD3DLog(0,_T("������������������ʧ��(%u,%u)"),
			m_FontStorage.GetObjectCount(),m_FontStorage.GetBufferSize());
		return false;
	}
}

bool CD3DFontManager::DeleteD3DFont(UINT ID)
{
	if(m_FontStorage.DeleteByID(ID))
	{
		return true;
	}
	else
	{
		PrintD3DLog(0,_T("CD3DFontManager::DeleteD3DFont:����[%u]δ�ҵ�"),
			ID);
		return false;
	}
}

bool CD3DFontManager::DeleteD3DFont(LPCTSTR FontName)
{
	CD3DBaseFont ** ppFont=m_FontStorage.Find(FontName);
	if(ppFont)
	{
		m_FontStorage.DeleteByID((*ppFont)->GetID());		
		return true;
	}
	else
	{
		PrintD3DLog(0,_T("CD3DFontManager::DeleteD3DFont:����[%s]δ�ҵ�"),
			FontName);
		return false;
	}
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
	Name.Format(_T("%s:%d:%d%d%d%d%d%d%d"),
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
	CEasyString Key;
	return *m_FontStorage.GetNextObject(Pos,Key);
}

CD3DBaseFont * CD3DFontManager::GetPrev(LPVOID& Pos)
{
	CEasyString Key;
	return *m_FontStorage.GetPrevObject(Pos,Key);
}

}