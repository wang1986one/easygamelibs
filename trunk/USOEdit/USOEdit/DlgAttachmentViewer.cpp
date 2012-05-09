/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgAttachmentViewer.cpp                                  */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgAttachmentViewer.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAttachmentViewer.h"




// CDlgAttachmentViewer 对话框

IMPLEMENT_DYNAMIC(CDlgAttachmentViewer, CPropertyConfigDialog)

CDlgAttachmentViewer::CDlgAttachmentViewer(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgAttachmentViewer::IDD, pParent)
{
	m_IsModual=false;
	m_pModel=NULL;
	m_pUSOObject=NULL;
}

CDlgAttachmentViewer::~CDlgAttachmentViewer()
{
}

void CDlgAttachmentViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ATTACHMENTS, m_lvAttachments);
}


BEGIN_MESSAGE_MAP(CDlgAttachmentViewer, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ENABLE_ATTACHMENT, &CDlgAttachmentViewer::OnBnClickedButtonEnableAttachment)
END_MESSAGE_MAP()


// CDlgAttachmentViewer 消息处理程序

BOOL CDlgAttachmentViewer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lvAttachments.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_lvAttachments.InsertColumn(0,_T("ID"),LVCFMT_LEFT,50);
	m_lvAttachments.InsertColumn(1,_T("名称"),LVCFMT_LEFT,100);
	m_lvAttachments.InsertColumn(2,_T("骨骼"),LVCFMT_LEFT,50);
	m_lvAttachments.InsertColumn(3,_T("位置"),LVCFMT_LEFT,100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgAttachmentViewer::OnBnClickedButtonEnableAttachment()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvAttachments.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvAttachments.GetNextSelectedItem(Pos);
		UINT ID=m_lvAttachments.GetItemData(Item);
		m_pModel->EnableAttachment(ID);
		m_pUSOObject->ReBindChilds();
	}
}


void CDlgAttachmentViewer::Init(CD3DWOWM2Model * pModel,CUSOObject * pUSOObject)
{
	CEasyString Temp;
	m_pModel=pModel;
	m_pUSOObject=pUSOObject;
	m_lvAttachments.DeleteAllItems();
	for(UINT i=0;i<m_pModel->GetModelResource()->GetAttachmentCount();i++)
	{
		CD3DWOWM2ModelResource::MODEL_ATTACHMENT * pAttachmentInfo=m_pModel->GetModelResource()->GetAttachmentByIndex(i);
		Temp.Format(_T("%d"),pAttachmentInfo->ID);
		int Item=m_lvAttachments.InsertItem(i,Temp);
		m_lvAttachments.SetItemData(Item,pAttachmentInfo->ID);
		m_lvAttachments.SetItemText(Item,1,pAttachmentInfo->Name);
		Temp.Format(_T("%d"),pAttachmentInfo->Bone);
		m_lvAttachments.SetItemText(Item,2,Temp);
		Temp.Format(_T("%g,%g,%g"),pAttachmentInfo->Position.x,pAttachmentInfo->Position.y,pAttachmentInfo->Position.z);
		m_lvAttachments.SetItemText(Item,3,Temp);
	}
}