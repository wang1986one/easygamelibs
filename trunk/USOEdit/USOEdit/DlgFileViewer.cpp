/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgFileViewer.cpp                                        */
/*      创建日期:  2011年11月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgFileViewer.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgFileViewer.h"




// CDlgFileViewer 对话框

IMPLEMENT_DYNAMIC(CDlgFileViewer, CDialog)

CDlgFileViewer::CDlgFileViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileViewer::IDD, pParent)
{
	m_pUSOEditDoc=NULL;
	m_IsInMPQ=false;
}

CDlgFileViewer::~CDlgFileViewer()
{
}

void CDlgFileViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_FILE, m_tcFiles);
}


BEGIN_MESSAGE_MAP(CDlgFileViewer, CDialog)
	ON_BN_CLICKED(IDC_SELECT_DIR, &CDlgFileViewer::OnBnClickedSelectDir)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILE, &CDlgFileViewer::OnTvnSelchangedTreeFile)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_FILE, &CDlgFileViewer::OnTvnItemexpandingTreeFile)
	ON_BN_CLICKED(IDC_BROWSER_MPQ, &CDlgFileViewer::OnBnClickedBrowserMpq)
	ON_BN_CLICKED(IDC_CLOSE_PREVIEW, &CDlgFileViewer::OnBnClickedClosePreview)
END_MESSAGE_MAP()


// CDlgFileViewer 消息处理程序

void CDlgFileViewer::OnBnClickedSelectDir()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bi;
	TCHAR FilePath[MAX_PATH];
	LPITEMIDLIST pidl;

	ZeroMemory(&bi,sizeof(bi));

	//bi.pszDisplayName=FilePath;
	bi.lpszTitle=_T("请选择目标目录");
	bi.ulFlags=BIF_RETURNONLYFSDIRS;
	pidl=::SHBrowseForFolder(&bi);
	if(pidl!=NULL)
	{
		::SHGetPathFromIDList(pidl,FilePath);
		IMalloc *pMalloc=NULL;
		::SHGetMalloc(&pMalloc);
		pMalloc->Free(pidl);
		pMalloc->Release();
		pMalloc=NULL;	

		UpdateData(true);
		BrowserDir(FilePath);		
		UpdateData(false);
	}	
}

void CDlgFileViewer::OnTvnSelchangedTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(m_pUSOEditDoc)
	{
		int ItemType;
		if(m_IsInMPQ)
		{
			ItemType=(pNMTreeView->itemNew.lParam>>24)&0x7F;
		}
		else
		{
			ItemType=LOWORD(pNMTreeView->itemNew.lParam);
		}
		if(ItemType!=ITEM_TYPE_DIR)
		{
			CString FileName=m_tcFiles.GetItemText(pNMTreeView->itemNew.hItem);
			HTREEITEM ParentItem=m_tcFiles.GetParentItem(pNMTreeView->itemNew.hItem);
			while(ParentItem)
			{
				CString ItemText=m_tcFiles.GetItemText(ParentItem);
				ParentItem=m_tcFiles.GetParentItem(ParentItem);
				if((!m_IsInMPQ)||ParentItem)
					FileName=ItemText+_T("\\")+FileName;
			}
			m_pUSOEditDoc->SetPreviewObject(FileName,ItemType);
		}
	}
	*pResult = 0;
}

void CDlgFileViewer::OnTvnItemexpandingTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(pNMTreeView->action==TVE_EXPAND)
	{
		if(m_IsInMPQ)
		{
			BYTE ItemType=(pNMTreeView->itemNew.lParam>>24)&0x7F;
			BYTE IsFetched=(pNMTreeView->itemNew.lParam>>24)&0x80;
			UINT TreeIndex=pNMTreeView->itemNew.lParam&0xFFFFFF;
			if(ItemType==ITEM_TYPE_DIR&&(!IsFetched))
			{
				UINT ItemData=((ItemType|0x80)<<24)|TreeIndex;
				m_tcFiles.SetItemData(pNMTreeView->itemNew.hItem,ItemData);
				FetchChild(pNMTreeView->itemNew.hItem);

			}
		}
		else
		{
			BOOL IsFetched=HIWORD(pNMTreeView->itemNew.lParam);
			int ItemType=LOWORD(pNMTreeView->itemNew.lParam);
			if(ItemType==ITEM_TYPE_DIR&&(!IsFetched))
			{
				ItemType=MAKELONG(TRUE,ITEM_TYPE_DIR);
				m_tcFiles.SetItemData(pNMTreeView->itemNew.hItem,ItemType);
				FetchChild(pNMTreeView->itemNew.hItem);

			}
		}
	}
	*pResult = 0;
}

void CDlgFileViewer::FetchChild(HTREEITEM Item)
{
	HTREEITEM ChildItem=m_tcFiles.GetChildItem(Item);
	while(ChildItem)
	{
		HTREEITEM NextItem=m_tcFiles.GetNextSiblingItem(ChildItem);
		m_tcFiles.DeleteItem(ChildItem);
		ChildItem=NextItem;
	}

	if(m_IsInMPQ)
	{
		UINT ItemData=m_tcFiles.GetItemData(Item);
		BYTE ItemType=(ItemData>>24)&0x7F;
		UINT TreeIndex=ItemData&0xFFFFFF;
		if(TreeIndex>=m_Tree.GetCount())
		{
			AfxMessageBox(_T("MPQ浏览缓冲数据有错误"));
			return;
		}
		UINT ChildCount=m_Tree[TreeIndex];
		if(TreeIndex+ChildCount>=m_Tree.GetCount())
		{
			AfxMessageBox(_T("MPQ浏览缓冲数据有错误"));
			return;
		}
		for(UINT i=0;i<ChildCount;i++)
		{
			UINT ChileTree=m_Tree[TreeIndex+i*2+1];
			//if(ChileTree>=m_Tree.GetCount())
			//{
			//	AfxMessageBox(_T("MPQ浏览缓冲数据有错误"));
			//	return;
			//}
			UINT DataPtr=m_Tree[TreeIndex+i*2+2];
			if(DataPtr+2>=m_TreeData.GetUsedSize())
			{
				AfxMessageBox(_T("MPQ浏览缓冲数据有错误"));
				return;
			}
			BYTE * pData=((BYTE *)m_TreeData.GetBuffer())+DataPtr;
			BYTE ChildItemType=*pData;
			TCHAR * pName=(TCHAR *)(pData+1);

			HTREEITEM FileItem=m_tcFiles.InsertItem(pName,Item);
			m_tcFiles.SetItemData(FileItem,(ChildItemType<<24)|ChileTree);
			if(ChildItemType==ITEM_TYPE_DIR)
				m_tcFiles.InsertItem(_T(""),FileItem);
		}

	}
	else
	{
		CString SearchPath=m_tcFiles.GetItemText(Item);
		HTREEITEM ParentItem=m_tcFiles.GetParentItem(Item);
		while(ParentItem)
		{
			CString ItemText=m_tcFiles.GetItemText(ParentItem);
			SearchPath=ItemText+_T("\\")+SearchPath;
			ParentItem=m_tcFiles.GetParentItem(ParentItem);
		}

		SearchPath+=_T("\\*");
		CFileSearcher FileSearcher;

		FileSearcher.FindFirst(SearchPath);
		while(FileSearcher.FindNext())
		{
			if(FileSearcher.IsDirectory())
			{
				if(!FileSearcher.IsDots())
				{
					HTREEITEM FileItem=m_tcFiles.InsertItem(FileSearcher.GetFileName(),Item);
					m_tcFiles.SetItemData(FileItem,ITEM_TYPE_DIR);
					m_tcFiles.InsertItem(_T(""),FileItem);
				}
			}
			else
			{
				if(FileSearcher.GetFileExt().CompareNoCase(_T(".m2"))==0)
				{
					HTREEITEM FileItem=m_tcFiles.InsertItem(FileSearcher.GetFileName(),Item);
					m_tcFiles.SetItemData(FileItem,ITEM_TYPE_M2);
				}
				else if(FileSearcher.GetFileExt().CompareNoCase(_T(".wmo"))==0)
				{
					HTREEITEM FileItem=m_tcFiles.InsertItem(FileSearcher.GetFileName(),Item);
					m_tcFiles.SetItemData(FileItem,ITEM_TYPE_WMO);
				}
				else if(FileSearcher.GetFileExt().CompareNoCase(_T(".blp"))==0)
				{
					HTREEITEM FileItem=m_tcFiles.InsertItem(FileSearcher.GetFileName(),Item);
					m_tcFiles.SetItemData(FileItem,ITEM_TYPE_BLP);
				}
			}
		}
	}
}
BOOL CDlgFileViewer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	TCHAR CurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,CurDir);
	BrowserDir(CurDir);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgFileViewer::BrowserDir(LPCTSTR szDir)
{
	m_tcFiles.DeleteAllItems();
	m_CurDir=szDir;
	m_IsInMPQ=false;
	HTREEITEM item=m_tcFiles.InsertItem(m_CurDir,TVI_ROOT);
	m_tcFiles.SetItemData(item,ITEM_TYPE_DIR);
	m_tcFiles.InsertItem(_T(""),item);
}
void CDlgFileViewer::BrowserMPQ()
{
	m_tcFiles.DeleteAllItems();
	m_CurDir=_T("MPQ");
	m_IsInMPQ=true;

#ifdef UNICODE
	CEasyString CacheFileName=MakeModuleFullPath(NULL,_T("MPQFileListU.Cache"));
#else
	CEasyString CacheFileName=MakeModuleFullPath(NULL,_T("MPQFileList.Cache"));
#endif
	
	CWinFileAccessor FileAccessor;
	if(FileAccessor.Open(CacheFileName,IFileAccessor::modeOpen|IFileAccessor::modeRead))
	{
		UINT Size;
		FileAccessor.Read(&Size,sizeof(UINT));
		m_Tree.Resize(Size/sizeof(UINT));
		FileAccessor.Read(m_Tree.GetBuffer(),Size);
		FileAccessor.Read(&Size,sizeof(UINT));
		m_TreeData.Create(Size);
		FileAccessor.Read((LPVOID)m_TreeData.GetBuffer(),Size);
		m_TreeData.SetUsedSize(Size);
		FileAccessor.Close();
	}
	else
	{
		CEasyArray<CEasyString> FileNameList;
		CUSOEditApp::GetInstance()->GetMPQFileSystem().GetFileList(FileNameList);

		MPQ_FILE_TREE TreeRoot;
		TreeRoot.Type=ITEM_TYPE_DIR;

		m_TreeNodeCount=0;
		m_TreeDataLen=0;

		for(UINT i=0;i<FileNameList.GetCount();i++)
		{
			CEasyString FilePath=FileNameList[i];		
			CEasyString FileExt=GetPathFileExtName(FilePath);		
			if(FileExt.CompareNoCase(_T(".m2"))==0)
			{
				FilePath.Replace('/','\\');
				AddFilePathToTree(TreeRoot,FilePath,ITEM_TYPE_M2);
			}
			else if(FileExt.CompareNoCase(_T(".wmo"))==0)
			{
				FilePath.Replace('/','\\');
				AddFilePathToTree(TreeRoot,FilePath,ITEM_TYPE_WMO);
			}
			//else if(FileExt.CompareNoCase(_T(".blp"))==0)
			//{
			//	FilePath.Replace('/','\\');
			//	AddFilePathToTree(TreeRoot,FilePath,ITEM_TYPE_BLP);
			//}
		}

		m_Tree.Reserve(m_TreeNodeCount*2);
		m_TreeData.Create(m_TreeDataLen);

		PushTreeData(TreeRoot,m_Tree,m_TreeData);

		if(FileAccessor.Open(CacheFileName,IFileAccessor::modeCreateAlways|IFileAccessor::modeWrite))
		{
			UINT Size=m_Tree.GetCount()*sizeof(UINT);
			FileAccessor.Write(&Size,sizeof(UINT));
			FileAccessor.Write(m_Tree.GetBuffer(),Size);
			Size=m_TreeData.GetUsedSize();
			FileAccessor.Write(&Size,sizeof(UINT));
			FileAccessor.Write(m_TreeData.GetBuffer(),Size);
			FileAccessor.Close();
		}
		else
		{
			AfxMessageBox(_T("创建MPQ浏览缓冲文件失败"));
		}
	}

	HTREEITEM item=m_tcFiles.InsertItem(m_CurDir,TVI_ROOT);
	m_tcFiles.SetItemData(item,ITEM_TYPE_DIR<<24);
	m_tcFiles.InsertItem(_T(""),item);
}

void CDlgFileViewer::AddFilePathToTree(HTREEITEM ParentItem,CEasyString FilePath,int FileType)
{
	int Pos=FilePath.Find('\\');
	if(Pos>=0)
	{
		CEasyString Dir=FilePath.Left(Pos);
		
		HTREEITEM ChildItem = m_tcFiles.GetChildItem(ParentItem);

		while (ChildItem != NULL)
		{
			CString ItemText=m_tcFiles.GetItemText(ChildItem);
			if(Dir.CompareNoCase(ItemText)==0)
				break;
			ChildItem = m_tcFiles.GetNextItem(ChildItem, TVGN_NEXT);			
		}
		if(ChildItem==NULL)
		{
			ChildItem=m_tcFiles.InsertItem(Dir,ParentItem);
			m_tcFiles.SetItemData(ChildItem,ITEM_TYPE_DIR);
		}		
		AddFilePathToTree(ChildItem,FilePath.Right(FilePath.GetLength()-Pos-1),FileType);
	}
	else
	{
		HTREEITEM Item=m_tcFiles.InsertItem(FilePath,ParentItem);
		m_tcFiles.SetItemData(Item,FileType);
	}
}

void CDlgFileViewer::AddFilePathToTree(MPQ_FILE_TREE& FileTree,CEasyString FilePath,int FileType)
{
	int Pos=FilePath.Find('\\');
	if(Pos>=0)
	{
		CEasyString Dir=FilePath.Left(Pos);

		UINT Index=0;
		bool IsNew=false;
		for(;Index<FileTree.Childs.GetCount();Index++)
		{
			if(FileTree.Childs[Index].Name.CompareNoCase(Dir)==0)
			{
				break;
			}
		}
		if(Index>=FileTree.Childs.GetCount())
		{
			FileTree.Childs.Resize(FileTree.Childs.GetCount()+1);
			Index=FileTree.Childs.GetCount()-1;
			IsNew=true;
		}
		FileTree.Childs[Index].Type=ITEM_TYPE_DIR;
		FileTree.Childs[Index].Name=Dir;		
		if(IsNew)
		{
			m_TreeNodeCount++;
			m_TreeDataLen+=sizeof(FileTree.Childs[Index].Type)+(FileTree.Childs[Index].Name.GetLength()+1)*sizeof(TCHAR);
		}
		AddFilePathToTree(FileTree.Childs[Index],FilePath.Right(FilePath.GetLength()-Pos-1),FileType);
	}
	else
	{
		MPQ_FILE_TREE FileInfo;

		FileInfo.Type=FileType;
		FileInfo.Name=FilePath;
		FileTree.Childs.Add(FileInfo);	

		m_TreeNodeCount++;
		m_TreeDataLen+=sizeof(FileInfo.Type)+(FileInfo.Name.GetLength()+1)*sizeof(TCHAR);
	}
}

UINT CDlgFileViewer::PushTreeData(MPQ_FILE_TREE& FileTree,CEasyArray<UINT>& Tree,CEasyBuffer& TreeData)
{
	if(FileTree.Childs.GetCount())
	{
		UINT Index=Tree.GetCount();
		Tree.Add(FileTree.Childs.GetCount());
		for(UINT i=0;i<FileTree.Childs.GetCount();i++)
		{
			Tree.Add(0);
			Tree.Add(TreeData.GetUsedSize());
			if(!TreeData.PushConstBack(FileTree.Childs[i].Type,sizeof(FileTree.Childs[i].Type)))
			{
				AfxMessageBox(_T("PushError"));
			}
			if(!TreeData.PushBack((LPCTSTR)FileTree.Childs[i].Name,(FileTree.Childs[i].Name.GetLength()+1)*sizeof(TCHAR)))
			{
				AfxMessageBox(_T("PushError"));
			}
		}
		for(UINT i=0;i<FileTree.Childs.GetCount();i++)
		{
			Tree[Index+i*2+1]=PushTreeData(FileTree.Childs[i],Tree,TreeData);
		}
		return Index;
	}
	else
	{
		return 0xffffffff;
	}
}
void CDlgFileViewer::OnBnClickedBrowserMpq()
{
	// TODO: 在此添加控件通知处理程序代码
	BrowserMPQ();
}

void CDlgFileViewer::OnBnClickedClosePreview()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pUSOEditDoc->SetPreviewObject(NULL,0);
}
