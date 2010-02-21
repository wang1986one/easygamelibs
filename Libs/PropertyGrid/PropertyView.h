#pragma once


// CPropertyView 视图
namespace PropertyGrid
{

class CPropertyView : public CView
{
	DECLARE_DYNCREATE(CPropertyView)

protected:
	CPropertyView();           // 动态创建所使用的受保护的构造函数
	virtual ~CPropertyView();

	CPropertyGrid m_PropertyGrid;

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制此视图
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

	virtual afx_msg LRESULT OnPropertyChanged( WPARAM wParam, LPARAM lParam);
public:
	virtual afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();

	CPropertyGrid& GetPropertyCtrl()
	{
		return m_PropertyGrid;
	}
};


}