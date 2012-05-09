/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgAniSequenceViewer.cpp                                 */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgAniSequenceViewer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgAniSequenceViewer.h"




// CDlgAniSequenceViewer �Ի���

IMPLEMENT_DYNAMIC(CDlgAniSequenceViewer, CPropertyConfigDialog)

CDlgAniSequenceViewer::CDlgAniSequenceViewer(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgAniSequenceViewer::IDD, pParent)
{
	m_IsModual=false;
}

CDlgAniSequenceViewer::~CDlgAniSequenceViewer()
{
}

void CDlgAniSequenceViewer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyConfigDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ANI, m_lvAniSeqs);
}


BEGIN_MESSAGE_MAP(CDlgAniSequenceViewer, CPropertyConfigDialog)
END_MESSAGE_MAP()


// CDlgAniSequenceViewer ��Ϣ�������

BOOL CDlgAniSequenceViewer::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_lvAniSeqs.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_lvAniSeqs.InsertColumn(0,_T("Index"),LVCFMT_LEFT,50);
	m_lvAniSeqs.InsertColumn(1,_T("ID"),LVCFMT_LEFT,30);
	m_lvAniSeqs.InsertColumn(2,_T("SubID"),LVCFMT_LEFT,50);
	m_lvAniSeqs.InsertColumn(3,_T("����"),LVCFMT_LEFT,80);
	m_lvAniSeqs.InsertColumn(4,_T("����"),LVCFMT_LEFT,50);
	m_lvAniSeqs.InsertColumn(5,_T("�ƶ��ٶ�"),LVCFMT_LEFT,70);
	m_lvAniSeqs.InsertColumn(6,_T("�����ٶ�"),LVCFMT_LEFT,70);
	m_lvAniSeqs.InsertColumn(7,_T("Flag1"),LVCFMT_LEFT,50);
	m_lvAniSeqs.InsertColumn(8,_T("Flag2"),LVCFMT_LEFT,50);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgAniSequenceViewer::Init(CD3DWOWM2ModelResource * pModelResource)
{
	m_pModelResource=pModelResource;
	m_lvAniSeqs.DeleteAllItems();
	for(UINT i=0;i<m_pModelResource->GetAnimationCount();i++)
	{
		CD3DWOWM2ModelResource::ANIMATION_SEQUENCE * pAni=m_pModelResource->GetAnimationInfoByIndex(i);
		if(pAni)
		{
			CEasyString Temp;

			Temp.Format(_T("%d"),i);
			int Item=m_lvAniSeqs.InsertItem(i,Temp);
			Temp.Format(_T("%d"),pAni->AnimationID);
			m_lvAniSeqs.SetItemText(Item,1,Temp);
			Temp.Format(_T("%d"),pAni->SubAnimationID);
			m_lvAniSeqs.SetItemText(Item,2,Temp);
			m_lvAniSeqs.SetItemText(Item,3,pAni->AnimationName);
			Temp.Format(_T("%d"),pAni->Length);
			m_lvAniSeqs.SetItemText(Item,4,Temp);
			Temp.Format(_T("%g"),pAni->MovingSpeed);
			m_lvAniSeqs.SetItemText(Item,5,Temp);
			Temp.Format(_T("%d"),pAni->PlaybackSpeed);
			m_lvAniSeqs.SetItemText(Item,6,Temp);
			Temp.Format(_T("0x%X"),pAni->Flags);
			m_lvAniSeqs.SetItemText(Item,7,Temp);
			Temp.Format(_T("0x%X"),pAni->Flags2);
			m_lvAniSeqs.SetItemText(Item,8,Temp);
		}
	}
}