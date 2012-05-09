/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgBoneViewer.cpp                                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgBoneViewer.cpp : 实现文件
//

#include "stdafx.h"




// CDlgBoneViewer 对话框

IMPLEMENT_DYNAMIC(CDlgBoneViewer, CPropertyConfigDialog)

CDlgBoneViewer::CDlgBoneViewer(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgBoneViewer::IDD, pParent)
{
	m_IsModual=false;
	m_pModelResource=NULL;
}

CDlgBoneViewer::~CDlgBoneViewer()
{
}

void CDlgBoneViewer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyConfigDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BONES, m_lvBones);
}


BEGIN_MESSAGE_MAP(CDlgBoneViewer, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_ANI_SEQUENCE, &CDlgBoneViewer::OnBnClickedButtonViewAniSequence)
END_MESSAGE_MAP()


// CDlgBoneViewer 消息处理程序

BOOL CDlgBoneViewer::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_lvBones.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_lvBones.InsertColumn(0,_T("ID"),LVCFMT_LEFT,30);
	m_lvBones.InsertColumn(1,_T("Flag"),LVCFMT_LEFT,50);
	m_lvBones.InsertColumn(2,_T("动画序列"),LVCFMT_LEFT,70);
	m_lvBones.InsertColumn(3,_T("父骨骼"),LVCFMT_LEFT,60);
	m_lvBones.InsertColumn(4,_T("子骨骼数"),LVCFMT_LEFT,70);
	m_lvBones.InsertColumn(5,_T("位置"),LVCFMT_LEFT,100);
	m_lvBones.InsertColumn(6,_T("网格号"),LVCFMT_LEFT,60);

	m_DlgAniFrameViewer.Create(m_DlgAniFrameViewer.IDD,this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgBoneViewer::Init(CD3DWOWM2ModelResource * pModelResource)
{
	m_lvBones.DeleteAllItems();
	m_pModelResource=pModelResource;
	for(UINT i=0;i<m_pModelResource->GetBoneCount();i++)
	{
		CD3DWOWM2ModelResource::MODEL_BONE * pBone=m_pModelResource->GetBoneInfo(i);
		if(pBone)
		{
			CEasyString Temp;
			Temp.Format(_T("%d"),i);
			int Item=m_lvBones.InsertItem(i,Temp);
			Temp.Format(_T("0x%X"),pBone->Flags);
			m_lvBones.SetItemText(Item,1,Temp);
			Temp.Format(_T("%d"),pBone->AnimationSeq);
			m_lvBones.SetItemText(Item,2,Temp);
			Temp.Format(_T("%d"),pBone->ParentBone);
			m_lvBones.SetItemText(Item,3,Temp);
			Temp.Format(_T("%d"),pBone->ChildCount);
			m_lvBones.SetItemText(Item,4,Temp);
			Temp.Format(_T("%g,%g,%g"),pBone->PivotPoint.x,pBone->PivotPoint.y,pBone->PivotPoint.x);
			m_lvBones.SetItemText(Item,5,Temp);
			Temp.Format(_T("%d"),pBone->GeosetID);
			m_lvBones.SetItemText(Item,6,Temp);
			m_lvBones.SetItemData(Item,i);
		}
	}
}

void CDlgBoneViewer::OnBnClickedButtonViewAniSequence()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvBones.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvBones.GetNextSelectedItem(Pos);
		UINT Bone=m_lvBones.GetItemData(Item);
		CD3DWOWM2ModelResource::MODEL_BONE * pBone=m_pModelResource->GetBoneInfo(Bone);
		if(pBone)
		{
			m_DlgAniFrameViewer.InitTransAniFrames(pBone->Translations,pBone->Rotations,pBone->Scalings);
		}
	}
	
	m_DlgAniFrameViewer.ShowWindow(SW_SHOW);
}
