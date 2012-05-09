/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgParticleEmitterParamViewer.cpp                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgParticleEmitterParamViewer.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgParticleEmitterParamViewer.h"

LPCTSTR g_szEmitterType[CD3DParticleEmitterResource::EMITTER_MAX]={_T("无"),_T("矩形"),_T("球形"),_T("未知")};


// CDlgParticleEmitterParamViewer 对话框

IMPLEMENT_DYNAMIC(CDlgParticleEmitterParamViewer, CPropertyConfigDialog)

CDlgParticleEmitterParamViewer::CDlgParticleEmitterParamViewer(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgParticleEmitterParamViewer::IDD, pParent)
	, m_Flag(_T(""))
	, m_SlowDown(0)
	, m_TextureCols(0)
	, m_TextureRows(0)
	, m_ParticleRotation(0)
	, m_FlagDescription(_T(""))
{
	m_IsModual=false;
}

CDlgParticleEmitterParamViewer::~CDlgParticleEmitterParamViewer()
{
}

void CDlgParticleEmitterParamViewer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyConfigDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FLAG, m_Flag);
	DDX_Control(pDX, IDC_COMBO_EMITTER_TYPE, m_cbEmitterType);
	DDX_Text(pDX, IDC_EDIT_SLOWDOWN, m_SlowDown);
	DDX_Control(pDX, IDC_COMBO_ROTATION, m_cbTextureRotation);
	DDX_Text(pDX, IDC_EDIT_TEXTURE_COL, m_TextureCols);
	DDX_Text(pDX, IDC_EDIT_TEXTURE_ROW, m_TextureRows);
	DDX_Text(pDX, ROTATION, m_ParticleRotation);
	DDX_Text(pDX, IDC_EDIT1, m_FlagDescription);
}


BEGIN_MESSAGE_MAP(CDlgParticleEmitterParamViewer, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_VIEW_PARAMS, &CDlgParticleEmitterParamViewer::OnBnClickedViewParams)
END_MESSAGE_MAP()


// CDlgParticleEmitterParamViewer 消息处理程序

BOOL CDlgParticleEmitterParamViewer::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	for(UINT i=0;i<CD3DParticleEmitterResource::EMITTER_MAX;i++)
	{
		m_cbEmitterType.AddString(g_szEmitterType[i]);
	}

	m_cbTextureRotation.AddString(_T("无"));
	m_cbTextureRotation.AddString(_T("+90"));
	m_cbTextureRotation.AddString(_T("-90"));

	m_DlgAniFrameViewer.Create(m_DlgAniFrameViewer.IDD,this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgParticleEmitterParamViewer::Init(CD3DParticleEmitterResource * pModelResource)
{
	m_pModelResource=pModelResource;
	m_Flag.Format(_T("0x%08X"),m_pModelResource->GetParticleEmitterInfo()->Flags);
	m_cbEmitterType.SetCurSel(m_pModelResource->GetParticleEmitterInfo()->EmitterType);
	m_SlowDown=m_pModelResource->GetParticleEmitterInfo()->Slowdown;
	switch(m_pModelResource->GetParticleEmitterInfo()->TextureTileRotation)
	{
	case 0:
		m_cbTextureRotation.SetCurSel(0);
		break;
	case 1:
		m_cbTextureRotation.SetCurSel(1);
		break;
	case -1:
		m_cbTextureRotation.SetCurSel(2);
		break;
	}
	m_TextureCols=m_pModelResource->GetParticleEmitterInfo()->TextureCols;
	m_TextureRows=m_pModelResource->GetParticleEmitterInfo()->TextureRows;
	m_ParticleRotation=m_pModelResource->GetParticleEmitterInfo()->Rotation*180/D3DX_PI;
	m_FlagDescription.Empty();
	if(m_pModelResource->GetParticleEmitterInfo()->Flags&CD3DParticleEmitterResource::PEF_GRAVITY_TRANSFORM)
	{
		m_FlagDescription+=_T("允许重力变换\r\n");
	}
	if(m_pModelResource->GetParticleEmitterInfo()->Flags&CD3DParticleEmitterResource::PEF_NO_TRAIL)
	{
		m_FlagDescription+=_T("粒子不追迹\r\n");
	}
	if(m_pModelResource->GetParticleEmitterInfo()->EmitterType==CD3DParticleEmitterResource::EMITTER_PLANE&&
		(m_pModelResource->GetParticleEmitterInfo()->Flags&CD3DParticleEmitterResource::PED_DIR_NO_HORIZONTAL_TRANS))
	{
		m_FlagDescription+=_T("粒子发射方向只有垂直变化\r\n");
	}
	if(m_pModelResource->GetParticleEmitterInfo()->Flags&CD3DParticleEmitterResource::PEF_HAVE_SPEED_VARIATION)
	{
		m_FlagDescription+=_T("启用加速度\r\n");
	}
	
	UpdateData(false);
}

void CDlgParticleEmitterParamViewer::OnBnClickedViewParams()
{
	// TODO: 在此添加控件通知处理程序代码
	m_DlgAniFrameViewer.InitParticleParams(m_pModelResource->GetParticleEmitterInfo());
	m_DlgAniFrameViewer.ShowWindow(SW_SHOW);
}
