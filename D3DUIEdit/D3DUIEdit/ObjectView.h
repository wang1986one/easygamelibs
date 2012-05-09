/****************************************************************************/
/*                                                                          */
/*      文件名:    ObjectView.h                                             */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "ObjectTreeView.h"
#include "ObjectPropertyView.h"

// CObjectView 视图
class CD3DUIEditDoc;

class CObjectView : public CView
{
	DECLARE_DYNCREATE(CObjectView)

protected:
	CObjectView();           // 动态创建所使用的受保护的构造函数
	virtual ~CObjectView();
protected:
	CSplitterWnd m_wndSplitter;

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制此视图
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

public:
	inline CD3DUIEditDoc* GetDocument() const
	{ 
		return reinterpret_cast<CD3DUIEditDoc*>(m_pDocument); 
	}
protected:
	CObjectTreeView * m_pObjectTreeView;
	CObjectPropertyView * m_pObjectPropertyView;
};


