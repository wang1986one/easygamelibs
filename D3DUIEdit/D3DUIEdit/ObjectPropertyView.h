/****************************************************************************/
/*                                                                          */
/*      文件名:    ObjectPropertyView.h                                     */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "DlgTextureConfig.h"
#include "DlgFontConfig.h"
#include "DlgColorConfig.h"
#include "DlgButtonTextureConfig.h"
#include "DlgScrollBarTextureConfig.h"
#include "DlgEditTextureConfig.h"
#include "DlgSimpleListTextureConfig.h"
#include "DlgComboTextureConfig.h"
#include "DlgProgressBarTextureConfig.h"

// CObjectPropertyView 视图
class CD3DUIEditDoc;

class CObjectPropertyView : public CView
{
	DECLARE_DYNCREATE(CObjectPropertyView)

protected:
	CObjectPropertyView();           // 动态创建所使用的受保护的构造函数
	virtual ~CObjectPropertyView();
protected:
	CPropertyGrid m_PropertyGrid;

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制此视图
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CD3DWnd *						m_CurObject;
	CDlgTextureConfig				m_DlgWndTextureConfig;
	CDlgButtonTextureConfig			m_DlgButtonTextureConfig;
	CDlgScrollBarTextureConfig		m_DlgScrollBarTextureConfig;
	CDlgFontConfig					m_DlgFontConfig; 
	CDlgColorConfig					m_DlgColorConfig;
	CDlgEditTextureConfig			m_DlgEditTextureConfig;
	CDlgSimpleListTextureConfig		m_DlgSimpleListTextureConfig;
	CDlgComboTextureConfig			m_DlgComboTextureConfig;
	CDlgProgressBarTextureConfig	m_DlgProgressBarTextureConfig;

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT OnPropertyChanged( WPARAM wParam, LPARAM lParam);

public:
	inline CD3DUIEditDoc* GetDocument() const
	{ 
		return reinterpret_cast<CD3DUIEditDoc*>(m_pDocument); 
	}

	void SetGUIObject(CD3DWnd * pObject);

	void RefreshObjectProperty();
	void UpdateObjectRect();
	void UpdateObjectText();
};


