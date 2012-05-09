/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWCharImport.cpp                                     */
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
/*      �ļ���:    DlgWOWCharImport.cpp                                     */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWOWCharImport.cpp : ʵ���ļ�
//

#include "stdafx.h"



// CDlgWOWCharImport �Ի���

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


// CDlgWOWCharImport ��Ϣ�������

BOOL CDlgWOWCharImport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgWOWCharImport::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	ProcessRaceChange();
}

void CDlgWOWCharImport::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			Temp.Format(_T("[%02u]��"),i);
		}
		else if(i==1)
		{
			Temp.Format(_T("[%02u]Ů"),i);
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
