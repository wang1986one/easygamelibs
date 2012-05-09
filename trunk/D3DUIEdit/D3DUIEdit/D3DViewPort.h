/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DViewPort.h                                            */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CD3DViewPort
class CD3DUIEditDoc;

class CD3DViewPort : public CWnd
{
	DECLARE_DYNAMIC(CD3DViewPort)

public:
	CD3DViewPort();
	virtual ~CD3DViewPort();

	BOOL Create(CWnd * pParent);

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CD3DDevice *			m_pD3DDevice;
	CD3DUIRender *			m_pRender;
	CD3DGUIObjectCreator *	m_pObjectCreator;
	CD3DGUI *				m_pGUI;
	CSizeBarManager			m_pSizeBarManager;
	int						m_FrameCount;
	
	int						m_SelectStatus;
	int						m_SelectStartX;
	int						m_SelectStartY;

	IUIWndRect *			m_SelectRect[4];
	CD3DGUITexture *		m_pDotLineTexture;
	CD3DGUITexture *		m_pSizeBarTexture;

	int						m_CreateCount;
	

public:
	afx_msg void OnTimer(UINT nIDEvent);
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:	
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CD3DGUI * GetGUI()
	{
		return m_pGUI;
	}
	CD3DUIRender * GetRender()
	{
		return m_pRender;
	}
	CSizeBarManager * GetSizeBarManager()
	{
		return &m_pSizeBarManager;
	}

	CD3DUIEditDoc * GetDocument();

	void OnSelected(CRect& SelectRect);
	void OnSelecting(CRect& SelectRect);
	void SelectWnds(CRect& SelectRect);

	CD3DSizeBar * CreateSizeBar(CRect Rect);

	void AddGUIObject(CD3DWnd * pGUIObject,bool AutoBindParent);
	void AddGUIObjectRecursion(CD3DWnd * pGUIObject,bool AutoBindParent);


	afx_msg LRESULT OnWndLoaded(WPARAM wParam,LPARAM lParam);
	
	void Cut();
	void Copy();
	void Paste();

	void AlignLeft();
	void AlignRight();
	void AlignTop();
	void AlignBottom();

	void VCenter();
	void HCenter();

	void HArrange();
	void VArrange();

	void SameWidth();
	void SameHeight();
	void SameSize();

	void SetParent();

	void Update();

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


