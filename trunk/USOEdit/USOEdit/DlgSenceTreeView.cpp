/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgSenceTreeView.cpp                                     */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgSenceTreeView.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSenceTreeView.h"


LPCTSTR g_SenceTreeNodeName[]={_T("左前"),_T("右前"),_T("左后"),_T("右后"),_T("左前上"),_T("右前上"),_T("左后上"),_T("右后上")};

// CDlgSenceTreeView 对话框

IMPLEMENT_DYNAMIC(CDlgSenceTreeView, CDialog)

CDlgSenceTreeView::CDlgSenceTreeView(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSenceTreeView::IDD, pParent)
{
	m_pView=NULL;
	m_pDoc=NULL;
}

CDlgSenceTreeView::~CDlgSenceTreeView()
{
}

void CDlgSenceTreeView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tvSenceTree);
}


BEGIN_MESSAGE_MAP(CDlgSenceTreeView, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CDlgSenceTreeView::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_RENDER_SELECTED, &CDlgSenceTreeView::OnBnClickedButtonRenderSelected)
	ON_BN_CLICKED(IDC_BUTTON_HIDE_CUT_OBJECT, &CDlgSenceTreeView::OnBnClickedButtonHideCutObject)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_OBJECT_NODE, &CDlgSenceTreeView::OnBnClickedButtonShowObjectNode)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CDlgSenceTreeView::OnTvnSelchangedTree1)
END_MESSAGE_MAP()


// CDlgSenceTreeView 消息处理程序

BOOL CDlgSenceTreeView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSenceTreeView::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pView&&m_pView->GetRender())
	{
		CAutoLock Lock(m_pView->GetRender()->GetRenderLock());
		m_tvSenceTree.DeleteAllItems();
		CD3DSceneRender::TREE_NODE * pRoot=m_pView->GetRender()->GetSceneRoot();
		CEasyString Temp;
		Temp.Format(_T("根节点(%u)"),
			pRoot->ObjectList.GetCount());
		HTREEITEM hParent=m_tvSenceTree.InsertItem(Temp,TVI_ROOT,TVI_LAST);
		m_tvSenceTree.SetItemData(hParent,(DWORD_PTR)pRoot);
		FillTree(pRoot,hParent);
	}
}

void CDlgSenceTreeView::OnBnClickedButtonRenderSelected()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem=m_tvSenceTree.GetSelectedItem();
	if(hItem)
	{
		CD3DSceneRender::TREE_NODE * pTree=(CD3DSceneRender::TREE_NODE *)m_tvSenceTree.GetItemData(hItem);
		if(pTree&&m_pView&&m_pView->GetRender())
		{
			m_pView->GetRender()->SetSelectTree(pTree);
		}
	}
}

void CDlgSenceTreeView::FillTree(CD3DSceneRender::TREE_NODE * pRoot,HTREEITEM hTreeRoot)
{
	if(m_pView&&m_pView->GetRender())
	{
		CEasyString Temp;

		for(UINT i=0;i<m_pView->GetRender()->GetSceneTreeMode();i++)
		{
			if(pRoot->Childs[i])
			{
				Temp.Format(_T("%d:%s(%u)%s"),
					pRoot->Depth,g_SenceTreeNodeName[i],
					pRoot->Childs[i]->ObjectList.GetCount(),
					pRoot->Childs[i]->IsRendered?"已渲染":"已剔除");
				HTREEITEM hItem=m_tvSenceTree.InsertItem(Temp,hTreeRoot,TVI_LAST);
				m_tvSenceTree.SetItemData(hItem,(DWORD_PTR)pRoot->Childs[i]);
				FillTree(pRoot->Childs[i],hItem);
			}
		}
	}
}
void CDlgSenceTreeView::OnBnClickedButtonHideCutObject()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pView&&m_pView->GetRender())
	{
		CAutoLock Lock(m_pView->GetRender()->GetRenderLock());
		m_pView->GetRender()->HideCutObject(m_pView->GetRender()->GetSceneRoot());
	}
}

void CDlgSenceTreeView::OnBnClickedButtonShowObjectNode()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pView&&m_pView->GetRender())
	{
		CUSOObject * pUSOObject=m_pDoc->GetSelectedObject();
		if(pUSOObject)
		{
			CNameObject * pObject=pUSOObject->GetBindObject();
			if(pObject)
			{
				CEasyArray<CD3DSceneRender::TREE_NODE *> TreeList;

				if(pObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
					m_pView->GetRender()->FindTreeByObject(m_pView->GetRender()->GetSceneRoot(),(CD3DObject *)pObject,TreeList);
				else if(pObject->IsKindOf(GET_CLASS_INFO(CD3DSubMesh)))
				{
					CUSOObject * pParentUSOObject=pUSOObject->GetParent();
					CNameObject * pParentObject=pParentUSOObject->GetBindObject();
					if(pParentObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
						m_pView->GetRender()->FindTreeBySubMesh(m_pView->GetRender()->GetSceneRoot(),
						(CD3DObject *)pParentObject,(CD3DSubMesh *)pObject,TreeList);
				}

				for(UINT i=0;i<TreeList.GetCount();i++)
				{
					m_pView->GetRender()->ShowNodeFrame(TreeList[i],true);
					HTREEITEM hItem=FindTree(m_tvSenceTree.GetRootItem(),TreeList[i]);
					if(hItem)
					{				
						CString Text=m_tvSenceTree.GetItemText(hItem);
						Text+=_T("!!");
						m_tvSenceTree.SetItemText(hItem,Text);
						m_tvSenceTree.SelectItem(hItem);
						m_tvSenceTree.EnsureVisible(hItem);					
					}
				}
			}
		}
	}
}

HTREEITEM CDlgSenceTreeView::FindTree(HTREEITEM hRoot,CD3DSceneRender::TREE_NODE * pNode)
{
	if(m_tvSenceTree.GetItemData(hRoot)==(DWORD_PTR)pNode)
		return hRoot;
	HTREEITEM hChild=m_tvSenceTree.GetChildItem(hRoot);
	while(hChild)
	{
		HTREEITEM hItem=FindTree(hChild,pNode);
		if(hItem)
			return hItem;
		hChild=m_tvSenceTree.GetNextItem(hChild,TVGN_NEXT);
	}
	return NULL;
}

void CDlgSenceTreeView::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if(pNMTreeView->itemOld.hItem)
	{
		CD3DSceneRender::TREE_NODE * pTree=(CD3DSceneRender::TREE_NODE *)m_tvSenceTree.GetItemData(pNMTreeView->itemOld.hItem);
		if(pTree&&pTree->pBoundingFrame)
		{
			pTree->pBoundingFrame->SetColor(0xFF0000FF);
		}
	}

	if(pNMTreeView->itemNew.hItem)
	{
		CD3DSceneRender::TREE_NODE * pTree=(CD3DSceneRender::TREE_NODE *)m_tvSenceTree.GetItemData(pNMTreeView->itemNew.hItem);
		if(pTree&&pTree->pBoundingFrame)
		{
			pTree->pBoundingFrame->SetColor(0xFF00FFFF);
		}
	}

	
}
