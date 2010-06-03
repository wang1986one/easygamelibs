// DlgServerStatus.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgServerStatus.h"




// CDlgServerStatus �Ի���

IMPLEMENT_DYNAMIC(CDlgServerStatus, CDialog)

CDlgServerStatus::CDlgServerStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServerStatus::IDD, pParent)
{

}

CDlgServerStatus::~CDlgServerStatus()
{
}

void CDlgServerStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STATUS, m_lvServerStatus);
}


BEGIN_MESSAGE_MAP(CDlgServerStatus, CDialog)
END_MESSAGE_MAP()


// CDlgServerStatus ��Ϣ�������

BOOL CDlgServerStatus::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_lvServerStatus.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_lvServerStatus.InsertColumn(0,"��",LVCFMT_LEFT,200);
	m_lvServerStatus.InsertColumn(1,"ֵ",LVCFMT_LEFT,150);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgServerStatus::FlushStatus(CSmartStruct& ServerStatus)
{
	m_lvServerStatus.DeleteAllItems();
	LPVOID Pos=ServerStatus.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=ServerStatus.GetNextMember(Pos,MemberID);
		CEasyString MemberIDStr;
		MemberIDStr=CControlPanel::GetInstance()->GetServerStatusName(MemberID);
		CEasyString ValueStr;
		switch(Value.GetType())
		{
		case CSmartValue::VT_CHAR:
			ValueStr.Format("%d",(char)Value);
			break;
		case CSmartValue::VT_UCHAR:
			ValueStr.Format("%u",(BYTE)Value);
			break;
		case CSmartValue::VT_SHORT:
			ValueStr.Format("%d",(short)Value);
			break;
		case CSmartValue::VT_USHORT:
			ValueStr.Format("%u",(WORD)Value);
			break;
		case CSmartValue::VT_INT:
			ValueStr.Format("%d",(int)Value);
			break;
		case CSmartValue::VT_UINT:
			ValueStr.Format("%u",(UINT)Value);
			break;
		case CSmartValue::VT_BIGINT:
			ValueStr.Format("%lld",(INT64)Value);
			break;
		case CSmartValue::VT_UBIGINT:
			ValueStr.Format("%llu",(UINT64)Value);
			break;
		case CSmartValue::VT_FLOAT:
			ValueStr.Format("%g",(float)Value);
			break;
		case CSmartValue::VT_DOUBLE:
			ValueStr.Format("%g",(double)Value);
			break;
		case CSmartValue::VT_STRING:
			ValueStr=(LPCTSTR)Value;
			break;
		case CSmartValue::VT_USTRING:
			ValueStr=(LPCWSTR)Value;
			break;
		default:
			ValueStr="δ֪��ʽ����";
		}
		int Item=m_lvServerStatus.InsertItem(m_lvServerStatus.GetItemCount(),MemberIDStr);
		m_lvServerStatus.SetItemText(Item,1,ValueStr);
	}
}