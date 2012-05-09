/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgAniSetting.cpp                                        */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgAniSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"




// CDlgAniSetting �Ի���

IMPLEMENT_DYNAMIC(CDlgAniSetting, CPropertyConfigDialog)

CDlgAniSetting::CDlgAniSetting(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgAniSetting::IDD, pParent)
{
	m_pModel=NULL;
	m_pUSOObject=NULL;	
	m_IsLoopPlay=true;
	m_IsModual=false;
}

CDlgAniSetting::~CDlgAniSetting()
{
}

void CDlgAniSetting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyConfigDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ANIMATION, m_cbAnimation);
	DDX_Control(pDX, IDC_SLIDER_PLAY_SPEED, m_slPlaySpeed);
	DDX_Text(pDX, IDC_STATIC_PLAY_SPEED, m_PlaySpeedTip);
	DDX_Check(pDX, IDC_PLAY_LOOP, m_IsLoopPlay);
}


BEGIN_MESSAGE_MAP(CDlgAniSetting, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_PLAY_ANI, &CDlgAniSetting::OnBnClickedPlayAni)	
	ON_CBN_SELCHANGE(IDC_COMBO_ANIMATION, &CDlgAniSetting::OnCbnSelchangeComboAnimation)
		ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDlgAniSetting ��Ϣ�������

BOOL CDlgAniSetting::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_slPlaySpeed.SetRange(0,30);
	m_slPlaySpeed.SetTicFreq(1);
	m_slPlaySpeed.SetPos(10);
	m_PlaySpeedTip=_T("1.0x");
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgAniSetting::OnBnClickedPlayAni()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CString Temp;
	m_cbAnimation.GetWindowText(Temp);

	UINT AniID,SubAniID;

	_stscanf_s(Temp,_T("%04u-%02u "),&AniID,&SubAniID);	
	m_pModel->Play(AniID,SubAniID,0,m_IsLoopPlay);
}

bool CDlgAniSetting::Init(CD3DWOWM2Model * pModel,CUSOObject * pUSOObject)
{
	m_pModel=pModel;
	m_pUSOObject=pUSOObject;

	CString Temp;
	

	m_cbAnimation.ResetContent();
	for(UINT i=0;i<m_pModel->GetModelResource()->GetAnimationCount();i++)
	{
		CD3DWOWM2ModelResource::ANIMATION_SEQUENCE * pAniInfo=m_pModel->GetModelResource()->GetAnimationInfoByIndex(i);
		Temp.Format(_T("%04u-%02u %s(%d)"),pAniInfo->AnimationID,pAniInfo->SubAnimationID,(LPCTSTR)pAniInfo->AnimationName,
			pAniInfo->Length);
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

	UpdateData(false);

	return true;
}
void CDlgAniSetting::OnCbnSelchangeComboAnimation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CString Temp;
	m_cbAnimation.GetWindowText(Temp);

	UINT AniID,SubAniID;

	_stscanf_s(Temp,_T("%04u-%02u "),&AniID,&SubAniID);	
	m_pModel->Play(AniID,SubAniID,0,m_IsLoopPlay);
}

void CDlgAniSetting::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
