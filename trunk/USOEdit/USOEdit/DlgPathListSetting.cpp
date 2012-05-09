/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgPathListSetting.cpp                                   */
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
/*      文件名:    DlgPathListSetting.cpp                                   */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgPathListSetting.cpp : 实现文件
//

#include "stdafx.h"



// CDlgPathListSetting 对话框

IMPLEMENT_DYNAMIC(CDlgPathListSetting, CDialog)
CDlgPathListSetting::CDlgPathListSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPathListSetting::IDD, pParent)
	, m_PathListText(_T(""))
{
	m_Title=_T("");
	m_WorkMode=WORK_MODE_PATH_LIST;
}

CDlgPathListSetting::~CDlgPathListSetting()
{
}

void CDlgPathListSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PATH_LIST, m_PathListText);
	DDX_Control(pDX, IDC_EDIT_PATH_LIST, m_edPathListText);
}


BEGIN_MESSAGE_MAP(CDlgPathListSetting, CDialog)	
	ON_BN_CLICKED(ID_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
END_MESSAGE_MAP()


// CDlgPathListSetting 消息处理程序



void CDlgPathListSetting::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_WorkMode==WORK_MODE_PATH_LIST)
	{
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
			if((!m_PathListText.IsEmpty())&&
				m_PathListText[m_PathListText.GetLength()-1]!='\r'&&
				m_PathListText[m_PathListText.GetLength()-1]!='\n')
			{
				m_PathListText+=_T("\r\n");
			}
			m_PathListText+=FilePath;
			m_PathListText+=_T("\r\n");
			UpdateData(false);
		}	
	}
	else
	{
		CFileDialog Dlg(true,NULL,NULL,OFN_ALLOWMULTISELECT);
		if(Dlg.DoModal()==IDOK)
		{
			UpdateData(true);
			POSITION Pos=Dlg.GetStartPosition();
			while(Pos)
			{
				CString FilePath=Dlg.GetNextPathName(Pos);
				if((!m_PathListText.IsEmpty())&&
					m_PathListText[m_PathListText.GetLength()-1]!='\r'&&
					m_PathListText[m_PathListText.GetLength()-1]!='\n')
				{
					m_PathListText+=_T("\r\n");
				}
				m_PathListText+=FilePath;
				m_PathListText+=_T("\r\n");
			}
			UpdateData(false);
		}
	}
}

void CDlgPathListSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

BOOL CDlgPathListSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_PathListText=_T("");
	for(int i=0;i<(int)m_PathList.GetCount();i++)
	{
		m_PathListText+=m_PathList[i]+_T("\r\n");
	}
	UpdateData(false);
	SetWindowText(m_Title);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgPathListSetting::OnEditCut()
{
	// TODO: 在此添加命令处理程序代码
	m_edPathListText.Cut();
}

void CDlgPathListSetting::OnEditCopy()
{
	// TODO: 在此添加命令处理程序代码
	m_edPathListText.Copy();
}

void CDlgPathListSetting::OnEditPaste()
{
	// TODO: 在此添加命令处理程序代码
	m_edPathListText.Paste();
}

void CDlgPathListSetting::OnEditUndo()
{
	// TODO: 在此添加命令处理程序代码
	m_edPathListText.Undo();
}

void CDlgPathListSetting::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CStringFile Lines;

	UpdateData(true);
	Lines.LoadFromString(m_PathListText);
	m_PathList.Clear();
	for(int i=0;i<Lines.GetLineCount();i++)
	{
		CEasyString Line=Lines[i];
		Line.Trim();
		if(!Line.IsEmpty())
		{
			m_PathList.Add(Line);
		}
	}
	CDialog::OnOK();
}
