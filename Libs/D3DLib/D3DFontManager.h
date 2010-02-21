#pragma once

namespace D3DLib{

class CD3DFontManager :
	public CNameObject
{
protected:
	CD3DDevice *						m_pD3DDevice;
	CNameStorage<CD3DBaseFont *>		m_FontStorage;

	DECLARE_CLASS_INFO_STATIC(CD3DFontManager)
public:
	CD3DFontManager(CD3DDevice * pD3DDevice,int StorageSize);
	virtual ~CD3DFontManager(void);

	bool Reset();
	bool Restore();		

	CD3DDevice * GetDevice();

	CD3DBaseFont * GetFont(UINT ID);

	CD3DBaseFont * GetFont(LPCTSTR FontName);

	bool AddD3DFont(CD3DBaseFont * pFont);
	bool AddD3DFont(CD3DBaseFont * pFont,LPCTSTR FontName);

	bool DeleteD3DFont(UINT ID);
	bool DeleteD3DFont(LPCTSTR FontName);

	CD3DBaseFont * CreateDX9Font(const LOGFONT * pLogFont,int MipLevels);
	CD3DBaseFont * CreateFontCN(const LOGFONT * pLogFont,int MipLevels,int BufferSize=4096);

	static CEasyString GetNameFromLogFont(const LOGFONT * pLogFont);

	int GetCount();
	LPVOID GetFirstPos();
	LPVOID GetLastPos();
	CD3DBaseFont * GetNext(LPVOID& Pos);
	CD3DBaseFont * GetPrev(LPVOID& Pos);

};

inline CD3DDevice * CD3DFontManager::GetDevice()
{
	return m_pD3DDevice;
}

inline CD3DBaseFont * CD3DFontManager::GetFont(UINT ID)
{
	CD3DBaseFont ** ppFont=m_FontStorage.GetObject(ID);
	if(ppFont)
		return *ppFont;
	else
		return NULL;
}

inline CD3DBaseFont * CD3DFontManager::GetFont(LPCTSTR FontName)
{
	CD3DBaseFont ** ppFont=m_FontStorage.GetObject(FontName);
	if(ppFont)
		return *ppFont;
	else
		return NULL;
}

}
