/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWCreatureImport.cpp                                 */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWOWCreatureImport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgWOWCreatureImport.h"




// DlgWOWCreatureImport �Ի���

IMPLEMENT_DYNAMIC(DlgWOWCreatureImport, CDialog)

DlgWOWCreatureImport::DlgWOWCreatureImport(CWnd* pParent /*=NULL*/)
	: CDialog(DlgWOWCreatureImport::IDD, pParent)
	, m_FilterText(_T(""))
{
	m_CreatureDisplayID=0;
}

DlgWOWCreatureImport::~DlgWOWCreatureImport()
{
}

void DlgWOWCreatureImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lvCreatureList);
	DDX_Text(pDX, IDC_EDIT1, m_FilterText);
}


BEGIN_MESSAGE_MAP(DlgWOWCreatureImport, CDialog)
	ON_BN_CLICKED(IDOK, &DlgWOWCreatureImport::OnBnClickedOk)
	ON_BN_CLICKED(IDC_FILTER, &DlgWOWCreatureImport::OnBnClickedFilter)
END_MESSAGE_MAP()


// DlgWOWCreatureImport ��Ϣ�������

BOOL DlgWOWCreatureImport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_lvCreatureList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvCreatureList.InsertColumn(0,_T("ID"),LVCFMT_LEFT,50);
	m_lvCreatureList.InsertColumn(1,_T("Name"),LVCFMT_LEFT,200);
	
	FillList(m_FilterText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void DlgWOWCreatureImport::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvCreatureList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvCreatureList.GetNextSelectedItem(Pos);
		m_CreatureDisplayID=m_lvCreatureList.GetItemData(Item);
		OnOK();
	}
	else
	{
		AfxMessageBox(_T("��ѡ��һ����"));
	}
	
}

void DlgWOWCreatureImport::OnBnClickedFilter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	FillList(m_FilterText);
}

void DlgWOWCreatureImport::FillList(LPCTSTR szFilter)
{
	m_lvCreatureList.DeleteAllItems();
	m_lvCreatureList.SetRedraw(false);
	LPVOID Pos=CBLZWOWDatabase::GetInstance()->GetFirstCreatureDisplayInfoPos();
	while(Pos)
	{
		CBLZWOWDatabase::BLZ_DB_CREATURE_DISPLAY_INFO * pDisplayInfo=CBLZWOWDatabase::GetInstance()->GetNextCreatureDisplayInfo(Pos);
		if(pDisplayInfo)
		{
			if(szFilter[0])
			{
				if(pDisplayInfo->Name.Find(szFilter)<0)
					continue;
			}
			CEasyString Temp;
			Temp.Format(_T("%u"),pDisplayInfo->ID);
			int Item=m_lvCreatureList.InsertItem(m_lvCreatureList.GetItemCount(),Temp);
			m_lvCreatureList.SetItemText(Item,1,pDisplayInfo->Name);
			m_lvCreatureList.SetItemData(Item,(DWORD_PTR)pDisplayInfo->ID);
		}
	}
	m_lvCreatureList.SetRedraw(true);
	m_lvCreatureList.Invalidate();
}