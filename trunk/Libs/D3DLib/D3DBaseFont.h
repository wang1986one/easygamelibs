/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DBaseFont.h                                            */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


namespace D3DLib{

class CD3DFontManager;

class CD3DBaseFont :
	public CNameObject
{
protected:
	CD3DFontManager *	m_pManager;

	DECLARE_CLASS_INFO_STATIC(CD3DBaseFont)
public:
	CD3DBaseFont();
	CD3DBaseFont(CD3DFontManager * pManager);
	virtual ~CD3DBaseFont(void);

	virtual bool Reset()=0;
	virtual bool Restore()=0;

	void SetManager(CD3DFontManager * pManager);

	virtual bool DrawTextA(LPCSTR pText,int TextLen,RECT * pRect,DWORD Align,D3DCOLOR Color)=0;
	virtual bool DrawTextW(LPCWSTR pText,int TextLen,RECT * pRect,DWORD Align,D3DCOLOR Color)=0;

	virtual bool GetTextSizeA(LPCSTR pText,int TextLen,LPSIZE pSize,LPINT pCharWidths=NULL)=0;
	virtual bool GetTextSizeW(LPCWSTR pText,int TextLen,LPSIZE pSize,LPINT pCharWidths=NULL)=0;

	virtual bool TranslateTextA(LPCSTR pSrcText,int StrLen,LPTSTR pDestText,int& BufferSize)=0;
	virtual bool TranslateTextW(LPCWSTR pSrcText,int StrLen,LPWSTR pDestText,int& BufferSize)=0;

	virtual bool GetLogFont(LOGFONT * pLogFont)=0;

	virtual HDC GetDC()=0;

	//�����������嶼֧��
	virtual void SetCharSpace(int Space)=0;
	virtual int GetCharSpace()=0;

	//�����������嶼֧��
	virtual void SetLineSpace(int Space)=0;
	virtual int GetLineSpace()=0;

};

inline void CD3DBaseFont::SetManager(CD3DFontManager * pManager)
{
	m_pManager=pManager;	
}

}