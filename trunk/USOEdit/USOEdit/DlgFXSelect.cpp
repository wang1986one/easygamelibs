/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgFXSelect.cpp                                          */
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
/*      �ļ���:    DlgFXSelect.cpp                                          */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgFXSelect.cpp : ʵ���ļ�
//

#include "stdafx.h"



// CDlgFXSelect �Ի���

IMPLEMENT_DYNAMIC(CDlgFXSelect, CPropertyConfigDialog)
CDlgFXSelect::CDlgFXSelect(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgFXSelect::IDD, pParent)
{
	m_SelectedFX=NULL;
}

CDlgFXSelect::~CDlgFXSelect()
{
}

void CDlgFXSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lvFXList);
}


BEGIN_MESSAGE_MAP(CDlgFXSelect, CPropertyConfigDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgFXSelect ��Ϣ�������

void CDlgFXSelect::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvFXList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int item=m_lvFXList.GetNextSelectedItem(Pos);
		m_SelectedFX=m_FXPtrArray[item];
	}
	OnOK();
}

BOOL CDlgFXSelect::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	int item=0;
	for(int i=0;i<(int)m_FXPtrArray.size();i++)
	{
		item=m_lvFXList.InsertItem(item+1,m_FXPtrArray[i]->GetName());
		if(m_FXPtrArray[i]==m_SelectedFX)
			m_lvFXList.SetItemState(item,LVIS_SELECTED,LVIS_SELECTED);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
