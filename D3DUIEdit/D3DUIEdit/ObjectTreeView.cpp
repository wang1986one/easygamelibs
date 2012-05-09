/****************************************************************************/
/*                                                                          */
/*      文件名:    ObjectTreeView.cpp                                       */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// ObjectTreeView.cpp : 实现文件
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "D3DUIEditDoc.h"
#include "ObjectTreeView.h"
#include ".\objecttreeview.h"

// CObjectTreeView

IMPLEMENT_DYNCREATE(CObjectTreeView, CTreeView)

CObjectTreeView::CObjectTreeView()
{
	m_ObjectParentItem=NULL;
}

CObjectTreeView::~CObjectTreeView()
{
}

BEGIN_MESSAGE_MAP(CObjectTreeView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
END_MESSAGE_MAP()


// CObjectTreeView 诊断

#ifdef _DEBUG
void CObjectTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CObjectTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG


// CObjectTreeView 消息处理程序

void CObjectTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	
	
}

BOOL CObjectTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style|=TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

void CObjectTreeView::AddGUIObject(CD3DWnd * pWnd)
{
	HTREEITEM item,ParentItem;
	CString ObjName,TypeName;
	CD3DWnd * pObject;

	if(pWnd->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
	{
		pObject=((CD3DSizeBar *)pWnd)->GetChild();
	}
	else
	{
		pObject=pWnd;
	}
	
	ParentItem=GetParentItem(pWnd);

	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DWnd)))
		TypeName=_T("窗口");
	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DButton)))
		TypeName=_T("按钮");
	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DEdit)))
		TypeName=_T("编辑框");
	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DSimpleList)))
		TypeName=_T("简单列表");
	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DCombo)))
		TypeName=_T("组合框");
	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DScrollBar)))
		TypeName=_T("滚动条");
	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DProgressBar)))
		TypeName=_T("进度条");
	ObjName.Format(_T("%s(%s)"),pObject->GetName(),TypeName);
	item=GetTreeCtrl().InsertItem(ObjName,0,0,ParentItem);
	if(pWnd->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
	{
		((CD3DSizeBar *)pWnd)->SetTreeItem(this,item);
	}
	else
	{
		pWnd->SetWndData(item);
	}
	GetTreeCtrl().SetItemData(item,(DWORD_PTR)pWnd);
	GetTreeCtrl().Expand(ParentItem,TVE_EXPAND);	
}

void CObjectTreeView::UpdateGUIObject(CD3DSizeBar * pWnd)
{	
	CString ObjName,TypeName;

	CD3DWnd * pObject=pWnd->GetChild();

	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DWnd)))
		TypeName=_T("窗口");
	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DButton)))
		TypeName=_T("按钮");
	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DEdit)))
		TypeName=_T("编辑框");
	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DSimpleList)))
		TypeName=_T("简单列表");
	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DCombo)))
		TypeName=_T("组合框");
	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DScrollBar)))
		TypeName=_T("滚动条");
	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DProgressBar)))
		TypeName=_T("进度条");
	ObjName.Format(_T("%s(%s)"),pObject->GetName(),TypeName);
	GetTreeCtrl().SetItemText(pWnd->GetTreeItem(),ObjName);
}

HTREEITEM CObjectTreeView::GetParentItem(CD3DWnd * pWnd)
{
	HTREEITEM item=NULL;
	if(pWnd)
	{
		CD3DWnd * pParent=pWnd->GetParent();
		if(pParent&&pParent->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
		{
			pParent=pParent->GetParent();
		}
		if(pParent&&(!pParent->IsInternal()))
		{
			pParent=pParent->GetParent();
		}
		if(pParent)
		{
			if(pParent->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
			{
				item=((CD3DSizeBar *)pParent)->GetTreeItem();
			}
			else
			{
				item=(HTREEITEM)pParent->GetWndData();
			}
		}
	}
	if(item==NULL)
	{
		if(m_ObjectParentItem==NULL)
			m_ObjectParentItem=GetTreeCtrl().InsertItem(_T("UI对象"),0,0);
		item=m_ObjectParentItem;
	}
	return item;
}

void CObjectTreeView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CD3DWnd * pWnd=(CD3DWnd *)(pNMTreeView->itemNew.lParam);
	if(pWnd)
	{
		if(pWnd->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
		{
			((CD3DSizeBar *)pWnd)->GetChild()->SetFocus();
		}
		else
		{
			pWnd->SetFocus();
		}		
	}
	GetDocument()->GetObjectPropertyView()->SetGUIObject(pWnd);
}
