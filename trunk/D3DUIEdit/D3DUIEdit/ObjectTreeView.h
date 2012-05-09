/****************************************************************************/
/*                                                                          */
/*      文件名:    ObjectTreeView.h                                         */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CObjectTreeView 视图

class CD3DUIEditDoc;

class CObjectTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CObjectTreeView)

protected:
	CObjectTreeView();           // 动态创建所使用的受保护的构造函数
	virtual ~CObjectTreeView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	inline CD3DUIEditDoc* GetDocument() const
	{ 
		return reinterpret_cast<CD3DUIEditDoc*>(m_pDocument); 
	}
protected:
	HTREEITEM	m_ObjectParentItem;
public:
	virtual void OnInitialUpdate();
	void AddGUIObject(CD3DWnd * pWnd);
	void UpdateGUIObject(CD3DSizeBar * pWnd);
	HTREEITEM GetParentItem(CD3DWnd * pWnd);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


