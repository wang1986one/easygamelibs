/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWM2Import.cpp                                       */
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
/*      文件名:    DlgWOWM2Import.cpp                                       */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWOWCharImport.cpp : 实现文件
//

#include "stdafx.h"



// CDlgWOWM2Import 对话框

IMPLEMENT_DYNAMIC(CDlgWOWM2Import, CDialog)

CDlgWOWM2Import::CDlgWOWM2Import(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWOWM2Import::IDD, pParent)
	, m_ModelFileName(_T(""))
	, m_SkinFileName(_T(""))		
{
}

CDlgWOWM2Import::~CDlgWOWM2Import()
{
}

void CDlgWOWM2Import::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ModelFileName);
	DDX_Text(pDX, IDC_EDIT6, m_SkinFileName);
	
}


BEGIN_MESSAGE_MAP(CDlgWOWM2Import, CDialog)
	ON_BN_CLICKED(IDC_SELECT_MODEL_FILE, &CDlgWOWM2Import::OnBnClickedSelectModelFile)
	ON_BN_CLICKED(IDC_SELECT_SKIN_FILE, &CDlgWOWM2Import::OnBnClickedSelectSkinFile)
	ON_BN_CLICKED(IDOK, &CDlgWOWM2Import::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgWOWM2Import 消息处理程序

void CDlgWOWM2Import::OnBnClickedSelectModelFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(true,_T("*.m2"),_T("*.m2"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		_T("Blizzard M2 Model File(*.m2)|*.m2|All Files(*.*)|*.*||"));

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_ModelFileName=dlg.GetPathName();
		int Pos=m_ModelFileName.ReverseFind('.');
		if(Pos>=0)
		{
			m_SkinFileName=m_ModelFileName.Left(Pos)+_T("00.skin");
		}
		UpdateData(false);
	}
}

void CDlgWOWM2Import::OnBnClickedSelectSkinFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(true,_T("*.skin"),_T("*.skin"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		_T("Blizzard M2 Model Skin File(*.skin)|*.skin|All Files(*.*)|*.*||"));

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_SkinFileName=dlg.GetPathName();
		UpdateData(false);
	}
}

BOOL CDlgWOWM2Import::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgWOWM2Import::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	OnOK();
}


