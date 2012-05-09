/****************************************************************************/
/*                                                                          */
/*      文件名:    USOTreeView.cpp                                          */
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
/*      文件名:    USOTreeView.cpp                                          */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// USOTreeView.cpp : CUSOTreeView 类的实现
//

#include "stdafx.h"




// CUSOTreeView

IMPLEMENT_DYNCREATE(CUSOTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CUSOTreeView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnTvnBegindrag)
END_MESSAGE_MAP()


// CUSOTreeView 构造/析构

CUSOTreeView::CUSOTreeView()
{
	// TODO: 在此处添加构造代码
}

CUSOTreeView::~CUSOTreeView()
{
}

BOOL CUSOTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	cs.style|=TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

void CUSOTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: 调用 GetTreeCtrl() 直接访问 TreeView 的树控件，
	// 从而可以用项填充 TreeView。	
}


// CUSOTreeView 诊断

#ifdef _DEBUG
void CUSOTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CUSOTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CUSOEditDoc* CUSOTreeView::GetDocument() // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUSOEditDoc)));
	return (CUSOEditDoc*)m_pDocument;
}
#endif //_DEBUG

CUSOObject * CUSOTreeView::GetSelectedObject()
{
	HTREEITEM item=GetTreeCtrl().GetSelectedItem();
	if(item)
	{
		return (CUSOObject *)GetTreeCtrl().GetItemData(item);
	}
	return NULL;
}

// CUSOTreeView 消息处理程序

void CUSOTreeView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CUSOObject * pObject;
	
	pObject=(CUSOObject *)pNMTreeView->itemOld.lParam;
	if(pObject)
	{
		CNameObject * pBindObject=pObject->GetBindObject();
		if(pBindObject)
		{
			if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
			{			
				((CD3DObject *)pBindObject)->ShowBoundingFrame(DBFO_RELEASE);
			}
			else if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DSubMesh)))
			{
				((CD3DSubMesh *)pBindObject)->SetSelected(false);
			}
		}		
		
	}
	pObject=(CUSOObject *)pNMTreeView->itemNew.lParam;
	if(pObject)
	{
		GetDocument()->GetPropertyView()->SetUSOObject(pObject);
		CNameObject * pBindObject=pObject->GetBindObject();

		if(pBindObject)
		{
			if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
			{			
				((CD3DObject *)pBindObject)->ShowBoundingFrame(DBFO_SHOW);
				GetDocument()->GetD3DViewPort()->SetSelectedObject((CD3DObject *)pBindObject);
			}
			else if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DSubMesh)))
			{
				((CD3DSubMesh *)pBindObject)->SetSelected(true);
			}		
		}
	}
	*pResult = 0;
}



void CUSOTreeView::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CPoint point;

	GetCursorPos(&point);

	ScreenToClient(&point);
	HTREEITEM item=GetTreeCtrl().HitTest(point);
	if(item)
	{
		GetTreeCtrl().SelectItem(item);
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU_POPUP));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		CUSOObject * pUSOObject=(CUSOObject *)GetTreeCtrl().GetItemData(item);

		

		if(pUSOObject)
		{
			CNameObject * pObject=pUSOObject->GetBindObject();
			if(pObject)
			{
				pPopup->AppendMenu(MF_SEPARATOR,0,_T(""));
				if(pObject->IsKindOf(GET_CLASS_INFO(CD3DTexture)))
				{
					pPopup->AppendMenu(MF_STRING,ID_VIEW_TEXTURE,_T("预览纹理"));
				}
			}
		}

		

		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);	
	}


	*pResult = 0;
}

void CUSOTreeView::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


