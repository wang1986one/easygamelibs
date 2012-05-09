/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USORightView.cpp                                         */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USORightView.cpp                                         */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// USORightView.cpp : CUSORightView ���ʵ��
//

#include "stdafx.h"




// CUSORightView

IMPLEMENT_DYNCREATE(CUSORightView, CView)

BEGIN_MESSAGE_MAP(CUSORightView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CUSORightView ����/����

CUSORightView::CUSORightView()
{
	// TODO: �ڴ˴���ӹ������

}

CUSORightView::~CUSORightView()
{
}

BOOL CUSORightView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	cs.dwExStyle|=WS_EX_NOACTIVATE|WS_EX_NOINHERITLAYOUT;
	return CView::PreCreateWindow(cs);
}

// CUSORightView ����

void CUSORightView::OnDraw(CDC* /*pDC*/)
{
	CUSOEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CUSORightView ���

#ifdef _DEBUG
void CUSORightView::AssertValid() const
{
	CView::AssertValid();
}

void CUSORightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUSOEditDoc* CUSORightView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUSOEditDoc)));
	return (CUSOEditDoc*)m_pDocument;
}
#endif //_DEBUG


// CUSORightView ��Ϣ�������

BOOL CUSORightView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���

	if(!CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
		return false;

	if(!m_wndSplitter.CreateStatic(this,2,1))
		return false;

	if(!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CD3DView),CSize(200,300),pContext))
		return false;
	if(!m_wndSplitter.CreateView(1,0,RUNTIME_CLASS(CControlPanelView),CSize(0,0),pContext))
		return false;

	//m_pObjectTreeView=(CObjectTreeView *)m_wndSplitter.GetPane(0,0);
	//m_pObjectPropertyView=(CObjectPropertyView *)m_wndSplitter.GetPane(1,0);

	//GetDocument()->SetItemTreeView((CObjectTreeView *)m_wndSplitter.GetPane(0,0));
	//GetDocument()->SetPropertyView((CObjectPropertyView *)m_wndSplitter.GetPane(1,0));
	m_wndSplitter.SetActivePane(0,0);
	return true;
}

void CUSORightView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	CRect rect;	
	GetClientRect(rect);
	m_wndSplitter.SetRowInfo(0,rect.Height()*5/6,0);
	m_wndSplitter.RecalcLayout();
}

void CUSORightView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	int row1,row2,col1,col2,SizeMin;
	if(::IsWindow(m_wndSplitter.GetSafeHwnd()))
	{
		CRect Rect;
		m_wndSplitter.GetWindowRect(&Rect);
		if(m_wndSplitter.GetRowCount()>1)
		{
			m_wndSplitter.GetRowInfo(0,row1,SizeMin);
			m_wndSplitter.GetRowInfo(1,row2,SizeMin);
		}
		if(m_wndSplitter.GetColumnCount()>1)
		{
			m_wndSplitter.GetColumnInfo(0,col1,SizeMin);
			m_wndSplitter.GetColumnInfo(1,col2,SizeMin);
		}
		m_wndSplitter.MoveWindow(0,0,cx,cy);
		if(m_wndSplitter.GetRowCount()>1&&Rect.Height())
		{
			m_wndSplitter.SetRowInfo(0,(int)floor(cy*(double)row1/Rect.Height()+0.5),0);			
			m_wndSplitter.RecalcLayout();
		}
		if(m_wndSplitter.GetColumnCount()>1&&Rect.Width())
		{
			m_wndSplitter.SetColumnInfo(0,(int)floor(cx*(double)col1/Rect.Width()+0.5),0);			
			m_wndSplitter.RecalcLayout();
		}
	}
}
