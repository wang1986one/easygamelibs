#pragma once

namespace D3DGUI{

class CD3DGUITextRect :
	public CD3DObject,public IUITextRect
{
protected:
	CD3DSubMesh			m_SubMesh;	
	FLOAT_RECT			m_Rect;	
	RECTVERTEX 			m_Vertexs[4];	
	CD3DTextTexture*	m_pTexture;

	DECLARE_CLASS_INFO(CD3DGUITextRect)

public:
	CD3DGUITextRect();
	CD3DGUITextRect(FLOAT_RECT& Rect);
	virtual ~CD3DGUITextRect(void);

	virtual void SetRender(CD3DRender * pRender);

	virtual void SetPos(FLOAT X,FLOAT Y);
	virtual FLOAT GetXPos();
	virtual FLOAT GetYPos();

	virtual void SetSize(FLOAT Width,FLOAT Height);
	virtual FLOAT GetWidth();
	virtual FLOAT GetHeight();

	virtual void SetVisible(bool IsVisible);
	virtual bool IsVisible();

	virtual void SetRect(FLOAT_RECT * pRect);
	virtual FLOAT_RECT GetRect();

	virtual bool SetFont(const LOGFONT* pLogFont);	
	virtual void SetColor(DWORD Color);	
	virtual void SetTextW(LPCWSTR szText);	
	virtual void SetAlign(DWORD dwAlign);
	virtual void SetShadowMode(DWORD ShadowMode);
	virtual void SetShadowColor(DWORD ShadowColor);
	virtual void SetShadowWidth(int Width);
	virtual void SetBKColor(DWORD Color);
	virtual void SetCharSpace(int Space);
	virtual void SetLineSpace(int Space);

	virtual LOGFONT* GetFont();	
	virtual DWORD GetColor();	
	virtual void GetText(CEasyString& Text);
	virtual DWORD GetAlign();
	virtual DWORD GetShadowMode();
	virtual DWORD GetShadowColor();
	virtual int GetShadowWidth();
	virtual DWORD GetBKColor();
	virtual int GetCharSpace();
	virtual int GetLineSpace();

	virtual void EnableUpdate(bool AllowUpdate);
	
	virtual bool GetTextSizeW(LPCWSTR pText,int StrLen,FLOAT& Width,FLOAT& Height,LPINT pCharWidths=NULL);
	virtual bool TranslateTextW(LPCWSTR pSrcText,int StrLen,LPWSTR pDestText,int& BufferSize);

	virtual bool SetFX(LPCTSTR FXFileName);
	virtual bool SetFXFromMemory(LPCTSTR FXName,LPCTSTR FXContent,int Size);
	virtual CEasyString GetFX();

	virtual void TopTo(IUIBaseRect* pRect);

	virtual void Release();
public:
	virtual void PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera);
	virtual void Update(FLOAT Time);
	virtual int GetSubMeshCount()
	{
		return 1;
	}
	virtual CD3DSubMesh * GetSubMesh(int index)
	{
		return &m_SubMesh;
	}	
	


protected:
	void CreateVertex();
};

}