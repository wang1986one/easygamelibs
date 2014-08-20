// DlgListSelector.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgListSelector.h"


// CDlgListSelector �Ի���

IMPLEMENT_DYNAMIC(CDlgListSelector, CDialog)

CDlgListSelector::CDlgListSelector(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgListSelector::IDD, pParent)
{

}

CDlgListSelector::~CDlgListSelector()
{
}

void CDlgListSelector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_lvList);
}


BEGIN_MESSAGE_MAP(CDlgListSelector, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgListSelector::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgListSelector::OnBnClickedCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CDlgListSelector::OnNMDblclkList)
END_MESSAGE_MAP()


// CDlgListSelector ��Ϣ�������

void CDlgListSelector::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);		
		m_SelectedItem=m_lvList.GetItemText(Item,0);
		OnOK();
		return;
	}
	AfxMessageBox("��ѡ��һ����Ŀ");
}

void CDlgListSelector::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL CDlgListSelector::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_lvList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvList.InsertColumn(0,_T("����"),LVCFMT_LEFT,200);

	SetWindowText(m_Title);

	m_lvList.DeleteAllItems();
	for(size_t i=0;i<m_Items.size();i++)
	{
		m_lvList.InsertItem(i,m_Items[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgListSelector::Init(CString Title,vector<CString> Items)
{
	m_Title=Title;
	m_Items=Items;	
	
}
void CDlgListSelector::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);		
		m_SelectedItem=m_lvList.GetItemText(Item,0);
		OnOK();
		return;
	}
}
