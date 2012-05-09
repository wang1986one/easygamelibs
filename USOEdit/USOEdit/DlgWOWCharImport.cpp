/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWCharImport.cpp                                     */
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
/*      文件名:    DlgWOWCharImport.cpp                                     */
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



// CDlgWOWCharImport 对话框

IMPLEMENT_DYNAMIC(CDlgWOWCharImport, CDialog)

CDlgWOWCharImport::CDlgWOWCharImport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWOWCharImport::IDD, pParent)	
	, m_Race(0)
	, m_Sex(0)		
{
}

CDlgWOWCharImport::~CDlgWOWCharImport()
{
}

void CDlgWOWCharImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO1, m_Race);	
	DDX_CBIndex(pDX, IDC_COMBO2, m_Sex);
	

	DDX_Control(pDX, IDC_COMBO1, m_cbRace);	
	DDX_Control(pDX, IDC_COMBO2, m_cbSex);	

	
}


BEGIN_MESSAGE_MAP(CDlgWOWCharImport, CDialog)	
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgWOWCharImport::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CDlgWOWCharImport::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgWOWCharImport 消息处理程序

BOOL CDlgWOWCharImport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString Temp;
	m_cbRace.ResetContent();
	m_cbRace.SetRedraw(false);
	for(UINT i=1;i<=CBLZWOWDatabase::GetInstance()->GetMaxCharRace();i++)
	{
		CBLZWOWDatabase::BLZ_DB_CHAR_RACE_INFO * pRaceInfo=CBLZWOWDatabase::GetInstance()->FindRaceInfo(i);
		if(pRaceInfo)
		{
			Temp.Format(_T("[%04u]%s"),pRaceInfo->RaceID,(LPCTSTR)pRaceInfo->RaceName);
			m_cbRace.AddString(Temp);
		}
		
	}	
	m_Race=0;
	m_cbRace.SetRedraw(true);
	m_cbRace.SetCurSel(m_Race);
	
	
	ProcessRaceChange();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgWOWCharImport::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	ProcessRaceChange();
}

void CDlgWOWCharImport::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	OnOK();
}

void CDlgWOWCharImport::ProcessRaceChange()
{

	int MaxSex=CBLZWOWDatabase::GetInstance()->GetMaxCharSex(m_Race+1);

	CString Temp;
	m_cbSex.SetRedraw(false);
	m_cbSex.ResetContent();
	for(UINT i=0;i<MaxSex;i++)
	{
		if(i==0)
		{
			Temp.Format(_T("[%02u]男"),i);
		}
		else if(i==1)
		{
			Temp.Format(_T("[%02u]女"),i);
		}
		else
		{
			Temp.Format(_T("[%02u]"),i);
		}
		
		m_cbSex.AddString(Temp);
	}
	if(m_Sex<0||m_Sex>=MaxSex)
	{
		m_Sex=0;
	}
	m_cbSex.SetRedraw(true);
	m_cbSex.SetCurSel(m_Sex);
	
}
