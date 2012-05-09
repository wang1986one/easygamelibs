/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWCharSetting.cpp                                    */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWCharSetting.cpp                                    */
/*      创建日期:  2009年09月25日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWOWCharSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "USOEdit.h"
#include "DlgWOWCharSetting.h"


// CDlgWOWCharSetting 对话框

IMPLEMENT_DYNAMIC(CDlgWOWCharSetting, CPropertyConfigDialog)

CDlgWOWCharSetting::CDlgWOWCharSetting(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgWOWCharSetting::IDD, pParent)
	, m_PlaySpeedTip(_T(""))
	, m_IsBald(FALSE)
	, m_IsLoopPlay(TRUE)
{
	m_pModel=NULL;
	m_pUSOObject=NULL;	
	m_IsModual=false;
}

CDlgWOWCharSetting::~CDlgWOWCharSetting()
{
}

void CDlgWOWCharSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SKIN_COLOR, m_cbSkinColor);
	DDX_Control(pDX, IDC_COMBO_HAIR_TYPE, m_cbHairType);
	DDX_Control(pDX, IDC_COMBO_HAIR_COLOR, m_cbHairColor);
	DDX_Control(pDX, IDC_COMBO_WHISKER_TYPE, m_cbWhiskerType);
	DDX_Control(pDX, IDC_COMBO_ANIMATION, m_cbAnimation);
	DDX_Control(pDX, IDC_COMBO_FACE_TYPE, m_cbFaceType);
	DDX_Control(pDX, IDC_COMBO_ATTACHMENT, m_cbAttachment);

	DDX_Control(pDX, IDC_SLIDER_PLAY_SPEED, m_slPlaySpeed);
	DDX_Text(pDX, IDC_STATIC_PLAY_SPEED, m_PlaySpeedTip);
	DDX_Check(pDX, IDC_CHECK_IS_BALD, m_IsBald);
	DDX_Check(pDX, IDC_PLAY_LOOP, m_IsLoopPlay);
}


BEGIN_MESSAGE_MAP(CDlgWOWCharSetting, CDialog)
	ON_BN_CLICKED(IDC_ENABLE_ATTACHMENT, &CDlgWOWCharSetting::OnBnClickedEnableAttachment)
	ON_CBN_SELCHANGE(IDC_COMBO_SKIN_COLOR, &CDlgWOWCharSetting::OnCbnSelchangeComboSkinColor)
	ON_CBN_SELCHANGE(IDC_COMBO_HAIR_TYPE, &CDlgWOWCharSetting::OnCbnSelchangeComboHairType)
	ON_CBN_SELCHANGE(IDC_COMBO_HAIR_COLOR, &CDlgWOWCharSetting::OnCbnSelchangeComboHairColor)
	ON_CBN_SELCHANGE(IDC_COMBO_WHISKER_TYPE, &CDlgWOWCharSetting::OnCbnSelchangeComboWhiskerType)
	ON_CBN_SELCHANGE(IDC_COMBO_FACE_TYPE, &CDlgWOWCharSetting::OnCbnSelchangeComboFaceType)
	ON_CBN_SELCHANGE(IDC_COMBO_ANIMATION, &CDlgWOWCharSetting::OnCbnSelchangeComboAnimation)	
	
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_IS_BALD, &CDlgWOWCharSetting::OnBnClickedCheckIsBald)
	ON_BN_CLICKED(IDC_EQUIPMENT_SETTING, &CDlgWOWCharSetting::OnBnClickedEquipmentSetting)
	ON_BN_CLICKED(IDC_PLAY_ANI, &CDlgWOWCharSetting::OnBnClickedPlayAni)
END_MESSAGE_MAP()


// CDlgWOWCharSetting 消息处理程序

bool CDlgWOWCharSetting::Init(CD3DWOWM2CharacterModel * pModel,CUSOObject * pUSOObject)
{
	m_pModel=pModel;
	m_pUSOObject=pUSOObject;

	CString Temp;
	int SkinColorMax;
	int HairColorMax;
	int FaceTypeMax;
	int HairTypeMax;	
	int WhiskerTypeMax;

	CBLZWOWDatabase::GetInstance()->GetCharMaxInfo(m_pModel->GetCharRace(),m_pModel->GetCharSex(),
		SkinColorMax,HairColorMax,FaceTypeMax,HairTypeMax,WhiskerTypeMax);

	m_cbSkinColor.ResetContent();
	for(int i=0;i<SkinColorMax;i++)
	{
		Temp.Format(_T("%d"),i);
		m_cbSkinColor.AddString(Temp);
	}
	m_cbSkinColor.SetCurSel(m_pModel->GetCharSkinColor());

	m_cbHairColor.ResetContent();
	for(int i=0;i<HairColorMax;i++)
	{
		Temp.Format(_T("%d"),i);
		m_cbHairColor.AddString(Temp);
	}
	m_cbHairColor.SetCurSel(m_pModel->GetCharHairColor());

	m_cbFaceType.ResetContent();
	for(int i=0;i<FaceTypeMax;i++)
	{
		Temp.Format(_T("%d"),i);
		m_cbFaceType.AddString(Temp);
	}
	m_cbFaceType.SetCurSel(m_pModel->GetCharFaceType());

	m_cbHairType.ResetContent();
	for(int i=0;i<HairTypeMax;i++)
	{
		Temp.Format(_T("%d"),i);
		m_cbHairType.AddString(Temp);
	}
	m_cbHairType.SetCurSel(m_pModel->GetCharHairType());

	m_cbWhiskerType.ResetContent();
	for(int i=0;i<WhiskerTypeMax;i++)
	{
		Temp.Format(_T("%d"),i);
		m_cbWhiskerType.AddString(Temp);
	}
	m_cbWhiskerType.SetCurSel(m_pModel->GetCharBeardType());

	m_cbAnimation.ResetContent();
	for(UINT i=0;i<m_pModel->GetModelResource()->GetAnimationCount();i++)
	{
		CD3DWOWM2ModelResource::ANIMATION_SEQUENCE * pAniInfo=m_pModel->GetModelResource()->GetAnimationInfoByIndex(i);
		Temp.Format(_T("%04u-%02u %s"),pAniInfo->AnimationID,pAniInfo->SubAnimationID,(LPCTSTR)pAniInfo->AnimationName);		
		m_cbAnimation.AddString(Temp);		
	}

	CD3DWOWM2ModelResource::ANIMATION_SEQUENCE * pAniInfo=m_pModel->GetModelResource()->FindAnimationInfo(
		m_pModel->GetCurAnimationID(),m_pModel->GetCurSubAnimationID());
	if(pAniInfo)
	{
		Temp.Format(_T("%04u-%02u %s"),pAniInfo->AnimationID,pAniInfo->SubAnimationID,(LPCTSTR)pAniInfo->AnimationName);
		int Sel=m_cbAnimation.FindString(0,Temp);		
		m_cbAnimation.SetCurSel(Sel);
	}

	m_cbAttachment.ResetContent();
	for(UINT i=0;i<m_pModel->GetModelResource()->GetAttachmentCount();i++)
	{
		CD3DWOWM2ModelResource::MODEL_ATTACHMENT * pAttachment=m_pModel->GetModelResource()->GetAttachmentByIndex(i);
		m_cbAttachment.AddString(pAttachment->Name);
		m_cbAttachment.SetItemData(i,(DWORD_PTR)pAttachment->ID);
	}

	m_IsBald=m_pModel->IsCharBald();

	UpdateData(false);

	return m_DlgEquipmentSetting.Init(m_pModel,m_pUSOObject);
}

BOOL CDlgWOWCharSetting::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_slPlaySpeed.SetRange(0,30);
	m_slPlaySpeed.SetTicFreq(1);
	m_slPlaySpeed.SetPos(10);
	m_PlaySpeedTip=_T("1.0x");
	m_IsBald=FALSE;

	m_DlgEquipmentSetting.Create(m_DlgEquipmentSetting.IDD,GetParent());
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgWOWCharSetting::OnBnClickedEnableAttachment()
{
	// TODO: 在此添加控件通知处理程序代码
	int Sel=m_cbAttachment.GetCurSel();
	UINT ID=(UINT)m_cbAttachment.GetItemData(Sel);
	m_pModel->EnableAttachment(ID);
	m_pUSOObject->ReBindChilds();
}

void CDlgWOWCharSetting::OnCbnSelchangeComboSkinColor()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pModel->SetCharSkinColor(m_cbSkinColor.GetCurSel());
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
}

void CDlgWOWCharSetting::OnCbnSelchangeComboHairType()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pModel->SetCharHairType(m_cbHairType.GetCurSel());
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
}

void CDlgWOWCharSetting::OnCbnSelchangeComboHairColor()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pModel->SetCharHairColor(m_cbHairColor.GetCurSel());
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
}

void CDlgWOWCharSetting::OnCbnSelchangeComboWhiskerType()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pModel->SetCharBeardType(m_cbWhiskerType.GetCurSel());
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
}

void CDlgWOWCharSetting::OnCbnSelchangeComboFaceType()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pModel->SetCharFaceType(m_cbFaceType.GetCurSel());
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
}

void CDlgWOWCharSetting::OnCbnSelchangeComboAnimation()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString Temp;
	m_cbAnimation.GetWindowText(Temp);

	UINT AniID,SubAniID;

	_stscanf_s(Temp,_T("%04u-%02u "),&AniID,&SubAniID);	
	m_pModel->Play(AniID,SubAniID,0,m_IsLoopPlay);

}


void CDlgWOWCharSetting::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(pScrollBar->GetSafeHwnd()==m_slPlaySpeed.GetSafeHwnd())
	{
		UpdateData(true);
		int Pos=m_slPlaySpeed.GetPos();
		m_PlaySpeedTip.Format(_T("%g"),(FLOAT)Pos/10.0f);
		m_pModel->SetPlaySpeed((FLOAT)Pos/10.0f);
		UpdateData(false);
	}	

	CPropertyConfigDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgWOWCharSetting::OnBnClickedCheckIsBald()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_pModel->SetCharBald(m_IsBald);
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	UpdateData(false);
}

void CDlgWOWCharSetting::OnBnClickedEquipmentSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	m_DlgEquipmentSetting.ShowWindow(SW_SHOW);
}

void CDlgWOWCharSetting::OnBnClickedPlayAni()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString Temp;
	m_cbAnimation.GetWindowText(Temp);

	UINT AniID,SubAniID;

	_stscanf_s(Temp,_T("%04u-%02u "),&AniID,&SubAniID);	
	m_pModel->Play(AniID,SubAniID,0,m_IsLoopPlay);
}
