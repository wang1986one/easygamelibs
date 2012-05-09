/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWWMOImport.cpp                                      */
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
/*      文件名:    DlgWOWWMOImport.cpp                                      */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWOWWMOImport.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgWOWWMOImport.h"




// CDlgWOWWMOImport 对话框

IMPLEMENT_DYNAMIC(CDlgWOWWMOImport, CDialog)

CDlgWOWWMOImport::CDlgWOWWMOImport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWOWWMOImport::IDD, pParent)
	, m_ModelFileName(_T(""))	
{
}

CDlgWOWWMOImport::~CDlgWOWWMOImport()
{
}

void CDlgWOWWMOImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MODEL_FILE_NAME, m_ModelFileName);
}


BEGIN_MESSAGE_MAP(CDlgWOWWMOImport, CDialog)
	ON_BN_CLICKED(IDC_SELECT_MODEL_FILE, &CDlgWOWWMOImport::OnBnClickedSelectModelFile)
	ON_BN_CLICKED(IDOK, &CDlgWOWWMOImport::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgWOWWMOImport 消息处理程序

void CDlgWOWWMOImport::OnBnClickedSelectModelFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(true,_T("*.wmo"),_T("*.wmo"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		_T("Blizzard WMO File(*.wmo)|*.wmo|All Files(*.*)|*.*||"));

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_ModelFileName=dlg.GetPathName();		
		UpdateData(false);
	}
}

void CDlgWOWWMOImport::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	OnOK();
}

BOOL CDlgWOWWMOImport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
