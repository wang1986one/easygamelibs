/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOTreeView.cpp                                          */
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
/*      �ļ���:    USOTreeView.cpp                                          */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// USOTreeView.cpp : CUSOTreeView ���ʵ��
//

#include "stdafx.h"




// CUSOTreeView

IMPLEMENT_DYNCREATE(CUSOTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CUSOTreeView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnTvnBegindrag)
END_MESSAGE_MAP()


// CUSOTreeView ����/����

CUSOTreeView::CUSOTreeView()
{
	// TODO: �ڴ˴���ӹ������
}

CUSOTreeView::~CUSOTreeView()
{
}

BOOL CUSOTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style|=TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

void CUSOTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: ���� GetTreeCtrl() ֱ�ӷ��� TreeView �����ؼ���
	// �Ӷ������������ TreeView��	
}


// CUSOTreeView ���

#ifdef _DEBUG
void CUSOTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CUSOTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CUSOEditDoc* CUSOTreeView::GetDocument() // �ǵ��԰汾��������
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

// CUSOTreeView ��Ϣ�������

void CUSOTreeView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
					pPopup->AppendMenu(MF_STRING,ID_VIEW_TEXTURE,_T("Ԥ������"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


