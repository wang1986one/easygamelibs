/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgPathListSetting.cpp                                   */
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
/*      �ļ���:    DlgPathListSetting.cpp                                   */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgPathListSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"



// CDlgPathListSetting �Ի���

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


// CDlgPathListSetting ��Ϣ�������



void CDlgPathListSetting::OnBnClickedAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_WorkMode==WORK_MODE_PATH_LIST)
	{
		BROWSEINFO bi;
		TCHAR FilePath[MAX_PATH];
		LPITEMIDLIST pidl;

		ZeroMemory(&bi,sizeof(bi));

		//bi.pszDisplayName=FilePath;
		bi.lpszTitle=_T("��ѡ��Ŀ��Ŀ¼");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL CDlgPathListSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_PathListText=_T("");
	for(int i=0;i<(int)m_PathList.GetCount();i++)
	{
		m_PathListText+=m_PathList[i]+_T("\r\n");
	}
	UpdateData(false);
	SetWindowText(m_Title);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgPathListSetting::OnEditCut()
{
	// TODO: �ڴ���������������
	m_edPathListText.Cut();
}

void CDlgPathListSetting::OnEditCopy()
{
	// TODO: �ڴ���������������
	m_edPathListText.Copy();
}

void CDlgPathListSetting::OnEditPaste()
{
	// TODO: �ڴ���������������
	m_edPathListText.Paste();
}

void CDlgPathListSetting::OnEditUndo()
{
	// TODO: �ڴ���������������
	m_edPathListText.Undo();
}

void CDlgPathListSetting::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
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
