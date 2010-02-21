// PropertyView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PropertyView.h"
#include ".\propertyview.h"

namespace PropertyGrid
{

// CPropertyView

IMPLEMENT_DYNCREATE(CPropertyView, CView)

CPropertyView::CPropertyView()
{
}

CPropertyView::~CPropertyView()
{
}

BEGIN_MESSAGE_MAP(CPropertyView, CView)
	ON_WM_SIZE()
	ON_MESSAGE(WM_PROPERTY_GRID_VALUE_CHANGED,OnPropertyChanged)
END_MESSAGE_MAP()


// CPropertyView ��ͼ

void CPropertyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CPropertyView ���

#ifdef _DEBUG
void CPropertyView::AssertValid() const
{
	CView::AssertValid();
}

void CPropertyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CPropertyView ��Ϣ�������
void CPropertyView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(::IsWindow(m_PropertyGrid.GetSafeHwnd()))
	{
		RECT rect;

		GetClientRect(&rect);
		m_PropertyGrid.MoveWindow(&rect);
	}
}

void CPropertyView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	CRect Rect;
	GetClientRect(&Rect);
	m_PropertyGrid.Create("",WS_VSCROLL|WS_HSCROLL|WS_CHILD|WS_VISIBLE,Rect,this,393787);
	m_PropertyGrid.ShowWindow(SW_SHOW);	
	m_PropertyGrid.SetScrollBars(flexScrollBarBoth);
}

LRESULT CPropertyView::OnPropertyChanged( WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

}