#pragma once

namespace D3DGUI{

class IUITexture
{
public:
	virtual CEasyString GetName()=0;
	virtual int GetWidth()=0;
	virtual int GetHeight()=0;
	virtual FLOAT X2U(int X)=0;
	virtual FLOAT Y2V(int Y)=0;
	virtual int U2X(FLOAT U)=0;
	virtual int V2X(FLOAT V)=0;

	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0)=0;

	virtual void Release()=0;

};
typedef IUITexture * LPIUITEXTURE;

class IUIBaseRect
{
public:
	virtual void SetPos(FLOAT X,FLOAT Y)=0;
	virtual FLOAT GetXPos()=0;
	virtual FLOAT GetYPos()=0;

	virtual void SetSize(FLOAT Width,FLOAT Height)=0;
	virtual FLOAT GetWidth()=0;
	virtual FLOAT GetHeight()=0;

	virtual void SetVisible(bool IsVisible)=0;
	virtual bool IsVisible()=0;

	virtual void SetRect(FLOAT_RECT * pRect)=0;
	virtual FLOAT_RECT GetRect()=0;

	virtual void TopTo(IUIBaseRect* pRect)=0;		//pWndRect==NULL,提到最高

	virtual void Release()=0;

};

typedef IUIBaseRect* LPIBASERECT;

class IUIWndRect:public IUIBaseRect
{
public:
	

	virtual void SetColor(DWORD Color)=0;
	virtual DWORD GetColor()=0;

	virtual void SetTexture(IUITexture * pTexture)=0;
	virtual IUITexture * GetTexture()=0;

	virtual void SetUV(FLOAT_RECT * pUVRect)=0;
	virtual FLOAT_RECT GetUV()=0;

	virtual bool SetFX(LPCTSTR FXFileName)=0;
	virtual bool SetFXFromMemory(LPCTSTR FXName,LPCTSTR FXContent,int Size)=0;
	virtual CEasyString GetFX()=0;
	
	

};

typedef IUIWndRect * LPIWNDRECT;

class IUITextRect:public IUIBaseRect
{
public:	

	virtual bool SetFont(const LOGFONT* pLogFont)=0;	
	virtual void SetColor(DWORD Color)=0;	
	virtual void SetTextW(LPCWSTR szText)=0;	
	virtual void SetAlign(DWORD dwAlign)=0;
	virtual void SetShadowMode(DWORD ShadowMode)=0;
	virtual void SetShadowColor(DWORD ShadowColor)=0;
	virtual void SetShadowWidth(int Width)=0;
	virtual void SetBKColor(DWORD Color)=0;
	virtual void SetCharSpace(int Space)=0;
	virtual void SetLineSpace(int Space)=0;

	virtual LOGFONT* GetFont()=0;	
	virtual DWORD GetColor()=0;	
	virtual void GetText(CEasyString& Text)=0;
	virtual DWORD GetAlign()=0;
	virtual DWORD GetShadowMode()=0;
	virtual DWORD GetShadowColor()=0;
	virtual int GetShadowWidth()=0;
	virtual DWORD GetBKColor()=0;
	virtual int GetCharSpace()=0;
	virtual int GetLineSpace()=0;

	virtual void EnableUpdate(bool AllowUpdate)=0;
	
	virtual bool GetTextSizeW(LPCWSTR pText,int StrLen,FLOAT& Width,FLOAT& Height,LPINT pCharWidths=NULL)=0;
	virtual bool TranslateTextW(LPCWSTR pSrcText,int StrLen,LPWSTR pDestText,int& BufferSize)=0;

	virtual bool SetFX(LPCTSTR FXFileName)=0;
	virtual bool SetFXFromMemory(LPCTSTR FXName,LPCTSTR FXContent,int Size)=0;
	virtual CEasyString GetFX()=0;	
	
};

typedef IUITextRect * LPITEXTRECT;

class IUIObjectCreator
{
public:
	virtual IUIWndRect * CreateWndRect(FLOAT_RECT * pRect)=0;
	virtual IUITextRect * CreateTextRect(FLOAT_RECT * pRect,const LOGFONT* pLogFont)=0;
	virtual IUITexture * CreateTextureFromFile(LPCTSTR FileName)=0;
	virtual void SetTextureDir(LPCTSTR Dir)=0;

	virtual void Release()=0;

};

typedef IUIObjectCreator * LPIOBJECTCREATOR;

}