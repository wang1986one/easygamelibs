/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgMethodEditor.cpp                                      */
/*      ��������:  2012��04��22��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgMethodEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgMethodEditor.h"


// CDlgMethodEditor �Ի���

IMPLEMENT_DYNAMIC(CDlgMethodEditor, CDialog)

CDlgMethodEditor::CDlgMethodEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMethodEditor::IDD, pParent)
	, m_Name(_T(""))
	, m_Description(_T(""))
	, m_ID(0)
	,m_ParamIDSeed(0)
{

}

CDlgMethodEditor::~CDlgMethodEditor()
{
}

void CDlgMethodEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PARAM, m_lvParamList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_Description);
	DDX_Text(pDX, IDC_EDIT_METHOD_ID, m_ID);
}


BEGIN_MESSAGE_MAP(CDlgMethodEditor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PARAM, &CDlgMethodEditor::OnBnClickedButtonAddParam)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_PARAM, &CDlgMethodEditor::OnBnClickedButtonEditParam)
	ON_BN_CLICKED(IDC_BUTTON_DEL_PARAM, &CDlgMethodEditor::OnBnClickedButtonDelParam)
	ON_BN_CLICKED(IDC_BUTTON_PARAM_UP, &CDlgMethodEditor::OnBnClickedButtonParamUp)
	ON_BN_CLICKED(IDC_BUTTON_PARAM_DOWN, &CDlgMethodEditor::OnBnClickedButtonParamDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PARAM, &CDlgMethodEditor::OnNMDblclkListParam)
END_MESSAGE_MAP()


// CDlgMethodEditor ��Ϣ�������

BOOL CDlgMethodEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_lvParamList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


	m_lvParamList.InsertColumn(0,"����",LVCFMT_LEFT,150);
	m_lvParamList.InsertColumn(1,"ID",LVCFMT_LEFT,50);
	m_lvParamList.InsertColumn(2,"����",LVCFMT_LEFT,60);	
	m_lvParamList.InsertColumn(3,"����",LVCFMT_LEFT,50);
	m_lvParamList.InsertColumn(4,"ȱʡֵ",LVCFMT_LEFT,50);
	m_lvParamList.InsertColumn(5,"��Ϊ��",LVCFMT_LEFT,60);
	m_lvParamList.InsertColumn(6,"����",LVCFMT_LEFT,200);


	FillListItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgMethodEditor::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(true);
	CDialog::OnOK();
}

void CDlgMethodEditor::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::OnCancel();
}

void CDlgMethodEditor::OnBnClickedButtonAddParam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgParamEditor Dlg;

	

	m_ParamIDSeed++;

	
	Dlg.SetID(m_ParamIDSeed);
	
	if(Dlg.DoModal()==IDOK)
	{
		
		METHOD_PARAM ParamInfo;

		Dlg.GetData(ParamInfo);
		m_ParamList.push_back(ParamInfo);
		AddListItem(ParamInfo,m_ParamList.size()-1);
	}
}

void CDlgMethodEditor::OnBnClickedButtonEditParam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvParamList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvParamList.GetItemData(Item);
		if(Index<m_ParamList.size())
		{
			CDlgParamEditor Dlg;

			Dlg.SetData(m_ParamList[Index]);

			if(Dlg.DoModal()==IDOK)
			{
				Dlg.GetData(m_ParamList[Index]);
				EditListItem(Item,m_ParamList[Index]);
			}
		}
	}
	else
	{
		MessageBox("��ѡ��һ������");
	}
}

void CDlgMethodEditor::OnBnClickedButtonDelParam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvParamList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvParamList.GetItemData(Item);
		if(Index<m_ParamList.size())
		{
			CString Msg;
			Msg.Format("�Ƿ�Ҫɾ������[%s]",
				m_ParamList[Index].Name);
			if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
			{
				m_ParamList.erase(m_ParamList.begin()+Index);
				FillListItem();
			}			
		}
	}
}
void CDlgMethodEditor::OnBnClickedButtonParamUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvParamList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvParamList.GetItemData(Item);
		if(Index>0&&Index<m_ParamList.size())
		{
			METHOD_PARAM Temp=m_ParamList[Index];
			m_ParamList.erase(m_ParamList.begin()+Index);
			m_ParamList.insert(m_ParamList.begin()+Index-1,Temp);
			FillListItem();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgMethodEditor::OnBnClickedButtonParamDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvParamList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvParamList.GetItemData(Item);
		if(Index<m_ParamList.size()-1)
		{
			METHOD_PARAM Temp=m_ParamList[Index];
			m_ParamList.erase(m_ParamList.begin()+Index);
			m_ParamList.insert(m_ParamList.begin()+Index+1,Temp);
			FillListItem();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgMethodEditor::AddListItem(METHOD_PARAM& ParamInfo,UINT Index)
{
	CString Temp;
	int Item=m_lvParamList.InsertItem(m_lvParamList.GetItemCount(),ParamInfo.Name);
	m_lvParamList.SetItemData(Item,(INT_PTR)Index);

	Temp.Format("%u",ParamInfo.ID);
	m_lvParamList.SetItemText(Item,1,Temp);

	TYPE_DEFINE * pTypeInfo=GetMainDlg()->FindVarType(ParamInfo.Type);
	if(pTypeInfo)
	{
		if(ParamInfo.IsReference)		
		{
			CString Temp=pTypeInfo->GenerateOperations.ReferenceDefine;
			Temp.Replace("<Type>",pTypeInfo->Name);
			m_lvParamList.SetItemText(Item,2,Temp);
		}
		else
			m_lvParamList.SetItemText(Item,2,pTypeInfo->Name);
	}
	else
	{
		m_lvParamList.SetItemText(Item,2,"δ֪");
	}
	Temp.Format("%d",ParamInfo.Length);
	m_lvParamList.SetItemText(Item,3,Temp);
	m_lvParamList.SetItemText(Item,4,ParamInfo.DefaultValue);
	if(ParamInfo.CanNull)
	{
		m_lvParamList.SetItemText(Item,5,"��");
	}
	else
	{
		m_lvParamList.SetItemText(Item,5,"��");
	}
	m_lvParamList.SetItemText(Item,6,ParamInfo.Description);
}

void CDlgMethodEditor::EditListItem(int Item,METHOD_PARAM& ParamInfo)
{
	CString Temp;
	m_lvParamList.SetItemText(Item,0,ParamInfo.Name);
	Temp.Format("%u",ParamInfo.ID);
	m_lvParamList.SetItemText(Item,1,Temp);

	TYPE_DEFINE * pTypeInfo=GetMainDlg()->FindVarType(ParamInfo.Type);
	if(pTypeInfo)
	{
		if(ParamInfo.IsReference)		
		{
			CString Temp=pTypeInfo->GenerateOperations.ReferenceDefine;
			Temp.Replace("<Type>",pTypeInfo->Name);
			m_lvParamList.SetItemText(Item,2,Temp);
		}
		else
			m_lvParamList.SetItemText(Item,2,pTypeInfo->Name);
	}
	else
	{
		m_lvParamList.SetItemText(Item,2,"δ֪");
	}
	Temp.Format("%d",ParamInfo.Length);
	m_lvParamList.SetItemText(Item,3,Temp);
	m_lvParamList.SetItemText(Item,4,ParamInfo.DefaultValue);
	if(ParamInfo.CanNull)
	{
		m_lvParamList.SetItemText(Item,5,"��Ϊ��");
	}
	else
	{
		m_lvParamList.SetItemText(Item,5,"����Ϊ��");
	}
	m_lvParamList.SetItemText(Item,6,ParamInfo.Description);
}

void CDlgMethodEditor::FillListItem()
{
	m_lvParamList.DeleteAllItems();
	for(UINT i=0;i<m_ParamList.size();i++)
	{
		AddListItem(m_ParamList[i],i);
	}
}

void CDlgMethodEditor::SelectItemByName(LPCTSTR szName)
{
	for(int i=0;i<m_lvParamList.GetItemCount();i++)
	{
		if(m_lvParamList.GetItemText(i,0)==szName)
		{
			m_lvParamList.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
			m_lvParamList.EnsureVisible(i,false);
		}
		else
		{
			m_lvParamList.SetItemState(i,0,LVIS_SELECTED);
		}
	}
}


void CDlgMethodEditor::OnNMDblclkListParam(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	OnBnClickedButtonEditParam();
}
