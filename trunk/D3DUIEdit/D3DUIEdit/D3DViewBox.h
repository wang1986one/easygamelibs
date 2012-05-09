/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DViewBox.h                                             */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once



// CD3DViewBox 视图

class CD3DViewBox : public CScrollView
{
	DECLARE_DYNCREATE(CD3DViewBox)

protected:
	CD3DViewBox();           // 动态创建所使用的受保护的构造函数
	virtual ~CD3DViewBox();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CWnd  m_ContentWnd;
	CD3DDevice *		m_pD3DDevice;
	CD3DUIRender *		m_pRender;
	CD3DGUIObjectCreator *	m_pObjectCreator;
	CD3DGUI *			m_pGUI;
	

	virtual void OnDraw(CDC* pDC);      // 重写以绘制此视图
	virtual void OnInitialUpdate();     // 构造后的第一次

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);

	void Init()
	{
		OnInitialUpdate();
	}

	CD3DGUIObjectCreator * GetObjectCreator()
	{
		return m_pObjectCreator;
	}
	CD3DDevice * GetDevice()
	{
		return m_pD3DDevice;
	}
	CD3DGUI * GetGUI()
	{
		return m_pGUI;
	}
	void SetContentSize(int Width,int Height);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


