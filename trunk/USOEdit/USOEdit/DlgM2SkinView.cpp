/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgM2SkinView.cpp                                        */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgM2SkinView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgM2SkinView.h"




// CDlgM2SkinView �Ի���

IMPLEMENT_DYNAMIC(CDlgM2SkinView, CPropertyConfigDialog)

CDlgM2SkinView::CDlgM2SkinView(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgM2SkinView::IDD, pParent)
{
	m_IsModual=false;
	m_pModel=NULL;
}

CDlgM2SkinView::~CDlgM2SkinView()
{
}

void CDlgM2SkinView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyConfigDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SKINS, m_lvSkins);
}


BEGIN_MESSAGE_MAP(CDlgM2SkinView, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_SKIN, &CDlgM2SkinView::OnBnClickedButtonChangeSkin)
END_MESSAGE_MAP()


// CDlgM2SkinView ��Ϣ�������

BOOL CDlgM2SkinView::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_lvSkins.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_lvSkins.InsertColumn(0,_T("Index"),LVCFMT_LEFT,50);
	m_lvSkins.InsertColumn(1,_T("SkinFileName"),LVCFMT_LEFT,200);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgM2SkinView::OnBnClickedButtonChangeSkin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

bool CDlgM2SkinView::Init(CD3DWOWM2Model * pModel)
{
	m_pModel=pModel;
	m_lvSkins.DeleteAllItems();
	CEasyString Temp;
	for(UINT i=0;i<m_pModel->GetModelResource()->GetSkinCount();i++)
	{
		Temp.Format(_T("%d"),i);
		int Item=m_lvSkins.InsertItem(i,Temp);
		m_lvSkins.SetItemText(Item,1,m_pModel->GetModelResource()->GetSkinFileName(i));
		m_lvSkins.SetItemData(Item,i);
	}
	return true;
}
