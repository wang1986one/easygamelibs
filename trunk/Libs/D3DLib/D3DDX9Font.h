/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DDX9Font.h                                             */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "d3dbasefont.h"

namespace D3DLib{

#pragma comment(linker,"/include:?m_CD3DDX9FontClassInfoRegister@CD3DDX9Font@D3DLib@@1VCClassInfoRegister@@A")

class CD3DDX9Font :
	public CD3DBaseFont
{
protected:
	LPD3DXFONT			m_pD3DFont;
	LOGFONT				m_LogFont;
	int					m_MipLevels;

	DECLARE_CLASS_INFO(CD3DDX9Font)
public:
	CD3DDX9Font();
	CD3DDX9Font(CD3DFontManager * pManager);
	virtual ~CD3DDX9Font(void);

	virtual bool Reset();
	virtual bool Restore();

	bool Create(const LOGFONT * pLogFont,int MipLevels);

	virtual bool DrawTextA(LPCSTR pText,int TextLen,RECT * pRect,DWORD Align,D3DCOLOR Color);
	virtual bool DrawTextW(LPCWSTR pText,int TextLen,RECT * pRect,DWORD Align,D3DCOLOR Color);

	virtual bool GetTextSizeA(LPCSTR pText,int TextLen,LPSIZE pSize,LPINT pCharWidths=NULL);
	virtual bool GetTextSizeW(LPCWSTR pText,int TextLen,LPSIZE pSize,LPINT pCharWidths=NULL);

	virtual bool TranslateTextA(LPCSTR pSrcText,int StrLen,LPTSTR pDestText,int& BufferSize);
	virtual bool TranslateTextW(LPCWSTR pSrcText,int StrLen,LPWSTR pDestText,int& BufferSize);

	virtual bool GetLogFont(LOGFONT * pLogFont);

	virtual HDC GetDC();

	virtual void SetCharSpace(int Space)
	{
		
	}
	virtual int GetCharSpace()
	{
		return 0;
	}

	virtual void SetLineSpace(int Space)
	{
		
	}
	virtual int GetLineSpace()
	{
		return 0;
	}
};

}