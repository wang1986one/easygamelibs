/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgImport.cpp                                            */
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
/*      �ļ���:    DlgImport.cpp                                            */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgImport.cpp : ʵ���ļ�
//

#include "stdafx.h"



// CDlgImport �Ի���

IMPLEMENT_DYNAMIC(CDlgImport, CDialog)
CDlgImport::CDlgImport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImport::IDD, pParent)
	, m_ImportFilePath(_T(""))
	, m_TextureDir(_T(""))
	, m_FxDir(_T(""))
	, m_UseTextureDir(FALSE)
	, m_UseFxDir(FALSE)
	, m_IsInverseImport(FALSE)
	, m_UseModelDir(FALSE)
	, m_ModelDir(_T(""))
	, m_SkinFileName(_T(""))
{
	m_ImportType=0;
}

CDlgImport::~CDlgImport()
{
}

void CDlgImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ImportFilePath);
	DDX_Text(pDX, IDC_EDIT2, m_TextureDir);
	DDX_Text(pDX, IDC_EDIT3, m_FxDir);
	DDX_Check(pDX, IDC_CHECK_USE_TEXTURE_PATH, m_UseTextureDir);
	DDX_Check(pDX, IDC_CHECK_USE_FX_PATH, m_UseFxDir);
	DDX_Control(pDX, IDC_EDIT2, m_edTextureDir);
	DDX_Control(pDX, IDC_EDIT3, m_edFxDir);
	DDX_Control(pDX, IDC_CHECK1, m_chkInverseImport);
	DDX_Check(pDX, IDC_CHECK1, m_IsInverseImport);
	DDX_Check(pDX, IDC_CHECK_USE_MODEL_PATH, m_UseModelDir);
	DDX_Text(pDX, IDC_EDIT4, m_ModelDir);
	DDX_Control(pDX, IDC_EDIT4, m_edModelDir);
	DDX_Control(pDX, IDC_SELECT_SKIN_FILE, m_btSelectSkinFile);
	DDX_Control(pDX, IDC_EDIT5, m_edSkinFileName);
	DDX_Text(pDX, IDC_EDIT5, m_SkinFileName);
}


BEGIN_MESSAGE_MAP(CDlgImport, CDialog)
	ON_BN_CLICKED(IDC_CHECK_USE_TEXTURE_PATH, OnBnClickedCheckUseTexturePath)
	ON_BN_CLICKED(IDC_CHECK_USE_FX_PATH, OnBnClickedCheckUseFxPath)
	ON_BN_CLICKED(IDC_SELECT_FILE, OnBnClickedSelectFile)
	ON_BN_CLICKED(IDC_SELECT_TEXTURE_PATH, OnBnClickedSelectTexturePath)
	ON_BN_CLICKED(IDC_SELECT_FX_PATH, OnBnClickedSelectFxPath)	
	ON_BN_CLICKED(IDC_CHECK_USE_MODEL_PATH, OnBnClickedCheckUseModelPath)
	ON_BN_CLICKED(IDC_SELECT_MODEL_PATH, OnBnClickedSelectModelPath)
	ON_BN_CLICKED(IDC_SELECT_SKIN_FILE, &CDlgImport::OnBnClickedSelectSkinFile)
END_MESSAGE_MAP()


// CDlgImport ��Ϣ�������

void CDlgImport::OnBnClickedCheckUseTexturePath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_edTextureDir.EnableWindow(m_UseTextureDir);
}

void CDlgImport::OnBnClickedCheckUseFxPath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_edFxDir.EnableWindow(m_UseFxDir);
}

void CDlgImport::OnBnClickedCheckUseModelPath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_edModelDir.EnableWindow(m_UseModelDir);
}

void CDlgImport::OnBnClickedSelectFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(true,m_DefualtFile,m_DefualtFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,m_FileFliter);

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_ImportFilePath=dlg.GetPathName();
		UpdateData(false);
	}
}

void CDlgImport::OnBnClickedSelectTexturePath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_UseTextureDir)
		return;
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
		m_TextureDir=FilePath;
		UpdateData(false);
	}	
}

void CDlgImport::OnBnClickedSelectFxPath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_UseFxDir)
		return;
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
		m_FxDir=FilePath;
		UpdateData(false);
	}	
}

void CDlgImport::OnBnClickedSelectModelPath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_UseFxDir)
		return;
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
		m_ModelDir=FilePath;
		UpdateData(false);
	}	
}

void CDlgImport::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::OnOK();
}

BOOL CDlgImport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_edTextureDir.EnableWindow(m_UseTextureDir);
	m_edFxDir.EnableWindow(m_UseFxDir);
	m_edModelDir.EnableWindow(m_UseModelDir);
	m_chkInverseImport.ShowWindow(SW_HIDE);

	if(m_ImportType==IMP_ACT)
	{
		m_chkInverseImport.ShowWindow(SW_SHOW);
	}
	if(m_ImportType!=IMP_M2)
	{
		m_btSelectSkinFile.EnableWindow(false);
		m_edSkinFileName.EnableWindow(false);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}





void CDlgImport::OnBnClickedSelectSkinFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(true,m_DefualtSkinFile,m_DefualtSkinFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,m_SkinFileFliter);

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_SkinFileName=dlg.GetPathName();
		UpdateData(false);
	}
}
