/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DView.h                                                */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DView.h                                                */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#include "d3dviewport.h"
// CD3DView 视图

class CUSOEditDoc;

class CD3DView : public CScrollView
{
	DECLARE_DYNCREATE(CD3DView)

protected:
	CD3DView();           // 动态创建所使用的受保护的构造函数
	virtual ~CD3DView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制此视图
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnInitialUpdate();


protected:
	CD3DViewPort * m_pViewPort;

protected:
//	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);


	CD3DGUI * GetGUI()
	{
		return m_pViewPort->GetGUI();
	}

	CD3DViewPort * GetViewPort()
	{
		return m_pViewPort;
	}

	inline CUSOEditDoc* GetDocument() const
	{ 
		return reinterpret_cast<CUSOEditDoc*>(m_pDocument); 
	}
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
//	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


