#pragma once

namespace D3DGUI{

class CD3DGUIWndRect :
	public CD3DObject,public IUIWndRect
{
protected:
	CD3DSubMesh		m_SubMesh;	
	FLOAT_RECT		m_Rect;
	FLOAT_RECT		m_TextureUV;
	RECTVERTEX 		m_Vertexs[6];


	DWORD			m_VertexColor;

	DECLARE_CLASS_INFO(CD3DGUIWndRect)

public:
	CD3DGUIWndRect();
	CD3DGUIWndRect(FLOAT_RECT& Rect);
	virtual ~CD3DGUIWndRect(void);

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

	virtual void SetColor(DWORD Color);
	virtual DWORD GetColor();

	virtual void SetTexture(IUITexture * pTexture);
	virtual IUITexture * GetTexture();

	virtual void SetUV(FLOAT_RECT * pUVRect);
	virtual FLOAT_RECT GetUV();

	virtual bool SetFX(LPCTSTR FXFileName);
	virtual bool SetFXFromMemory(LPCTSTR FXName,LPCTSTR FXContent,int Size);
	virtual CEasyString GetFX();

	virtual void TopTo(IUIBaseRect* pRect);		//pWndRect==NULL,提到最高

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