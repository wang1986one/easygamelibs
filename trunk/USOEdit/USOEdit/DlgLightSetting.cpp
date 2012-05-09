/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgLightSetting.cpp                                      */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgLightSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgLightSetting.h"




// CDlgLightSetting 对话框

IMPLEMENT_DYNAMIC(CDlgLightSetting, CDialog)

CDlgLightSetting::CDlgLightSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLightSetting::IDD, pParent)
	, m_TimeHour(0)
	, m_TimeMinute(0)
{
	m_pDoc=NULL;
	m_pLight=NULL;
	ZeroMemory(&m_LightData,sizeof(m_LightData));
}

CDlgLightSetting::~CDlgLightSetting()
{
}

void CDlgLightSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIGHT_TYPE, m_cbLightType);

	DDX_Text(pDX, IDC_POS_X, m_LightData.Position.x);
	DDX_Text(pDX, IDC_POS_Y, m_LightData.Position.y);
	DDX_Text(pDX, IDC_POS_Z, m_LightData.Position.z);

	DDX_Text(pDX, IDC_DIR_X, m_LightData.Direction.x);
	DDX_Text(pDX, IDC_DIR_Y, m_LightData.Direction.y);
	DDX_Text(pDX, IDC_DIR_Z, m_LightData.Direction.z);

	DDX_Text(pDX, IDC_AMBIENT_RED, m_LightData.Ambient.r);
	DDX_Text(pDX, IDC_AMBIENT_GREEN, m_LightData.Ambient.g);
	DDX_Text(pDX, IDC_AMBIENT_BLUE, m_LightData.Ambient.b);
	DDX_Text(pDX, IDC_AMBIENT_ALPHA, m_LightData.Ambient.a);

	DDX_Text(pDX, IDC_DIFFUSE_RED, m_LightData.Diffuse.r);
	DDX_Text(pDX, IDC_DIFFUSE_GREEN, m_LightData.Diffuse.g);
	DDX_Text(pDX, IDC_DIFFUSE_BLUE, m_LightData.Diffuse.b);
	DDX_Text(pDX, IDC_DIFFUSE_ALPHA, m_LightData.Diffuse.a);

	DDX_Text(pDX, IDC_SPECULAR_RED, m_LightData.Specular.r);
	DDX_Text(pDX, IDC_SPECULAR_GREEN, m_LightData.Specular.g);
	DDX_Text(pDX, IDC_SPECULAR_BLUE, m_LightData.Specular.b);
	DDX_Text(pDX, IDC_SPECULAR_ALPHA, m_LightData.Diffuse.a);

	DDX_Text(pDX, IDC_RANGE, m_LightData.Range);
	DDX_Text(pDX, IDC_ATTENUATION0, m_LightData.Attenuation0);
	DDX_Text(pDX, IDC_ATTENUATION1, m_LightData.Attenuation1);
	DDX_Text(pDX, IDC_ATTENUATION2, m_LightData.Attenuation2);
	DDX_Text(pDX, IDC_THETA, m_LightData.Theta);
	DDX_Text(pDX, IDC_PHI, m_LightData.Phi);
	DDX_Text(pDX, IDC_FALL_OFF2, m_TimeHour);
	DDX_Text(pDX, IDC_FALL_OFF3, m_TimeMinute);
}


BEGIN_MESSAGE_MAP(CDlgLightSetting, CDialog)
	ON_BN_CLICKED(IDC_APPLY, &CDlgLightSetting::OnBnClickedApply)
	ON_BN_CLICKED(IDC_SELECT_AMBIENT, &CDlgLightSetting::OnBnClickedSelectAmbient)
	ON_BN_CLICKED(IDC_SELECT_DIFFUSE, &CDlgLightSetting::OnBnClickedSelectDiffuse)
	ON_BN_CLICKED(IDC_SELECT_SPECULAR, &CDlgLightSetting::OnBnClickedSelectSpecular)
	ON_BN_CLICKED(IDC_USE_SCENE_LIGHT, &CDlgLightSetting::OnBnClickedUseSceneLight)
END_MESSAGE_MAP()


// CDlgLightSetting 消息处理程序

BOOL CDlgLightSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_cbLightType.AddString(_T("点光"));
	m_cbLightType.AddString(_T("锥形光"));
	m_cbLightType.AddString(_T("方向光"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgLightSetting::SetLight(CD3DLight * pLight)
{
	m_pLight=pLight;
	m_pLight->GetCurLight(m_LightData);
	m_cbLightType.SetCurSel(m_LightData.Type-1);
	UpdateData(false);
}



void CDlgLightSetting::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_LightData.Type=(D3DLIGHTTYPE)(m_cbLightType.GetCurSel()+1);
	m_pLight->SetLight(m_LightData);
	m_pLight->GetCurLight(m_LightData);
	UpdateData(false);
}

void CDlgLightSetting::OnBnClickedSelectAmbient()
{
	// TODO: 在此添加控件通知处理程序代码
	

	UpdateData(true);

	COLORREF Color=RGB(
		255*m_LightData.Ambient.r,
		255*m_LightData.Ambient.g,
		255*m_LightData.Ambient.b);

	CColorDialog Dlg(Color,CC_FULLOPEN);
	if(Dlg.DoModal()==IDOK)
	{
		Color=Dlg.GetColor();
		m_LightData.Ambient.r=(float)GetRValue(Color)/255.0f;
		m_LightData.Ambient.g=(float)GetGValue(Color)/255.0f;
		m_LightData.Ambient.b=(float)GetBValue(Color)/255.0f;
		UpdateData(false);
	}
}

void CDlgLightSetting::OnBnClickedSelectDiffuse()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	COLORREF Color=RGB(
		255*m_LightData.Diffuse.r,
		255*m_LightData.Diffuse.g,
		255*m_LightData.Diffuse.b
		);

	CColorDialog Dlg(Color,CC_FULLOPEN);
	if(Dlg.DoModal()==IDOK)
	{
		Color=Dlg.GetColor();
		m_LightData.Diffuse.r=(float)GetRValue(Color)/255.0f;
		m_LightData.Diffuse.g=(float)GetGValue(Color)/255.0f;
		m_LightData.Diffuse.b=(float)GetBValue(Color)/255.0f;
		UpdateData(false);
	}
}

void CDlgLightSetting::OnBnClickedSelectSpecular()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	COLORREF Color=RGB(
		255*m_LightData.Specular.r,
		255*m_LightData.Specular.g,
		255*m_LightData.Specular.b
		);

	CColorDialog Dlg(Color,CC_FULLOPEN);
	if(Dlg.DoModal()==IDOK)
	{
		Color=Dlg.GetColor();
		m_LightData.Specular.r=(float)GetRValue(Color)/255.0f;
		m_LightData.Specular.g=(float)GetGValue(Color)/255.0f;
		m_LightData.Specular.b=(float)GetBValue(Color)/255.0f;
		UpdateData(false);
	}
}

void CDlgLightSetting::OnBnClickedUseSceneLight()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	D3DCOLOR Diffuse=D3DCOLOR_XRGB(
		(BYTE)(255*m_LightData.Diffuse.r),
		(BYTE)(255*m_LightData.Diffuse.g),
		(BYTE)(255*m_LightData.Diffuse.b)
		);

	D3DCOLOR Ambient=D3DCOLOR_XRGB(
		(BYTE)(255*m_LightData.Ambient.r),
		(BYTE)(255*m_LightData.Ambient.g),
		(BYTE)(255*m_LightData.Ambient.b)
		);

	CD3DVector3 Pos;
	m_pDoc->GetD3DViewPort()->GetCameraPos(Pos.x,Pos.y,Pos.z);

	UINT Time=(m_TimeHour*60+m_TimeMinute)*60;

	CBLZWOWDatabase::GetInstance()->GetMapLightColor(m_pDoc->GetCurMapID(),Pos,DBC_LICI_LIGHT_DIFFUSE,Time,Diffuse);
	CBLZWOWDatabase::GetInstance()->GetMapLightColor(m_pDoc->GetCurMapID(),Pos,DBC_LICI_LIGHT_AMBIENT,Time,Ambient);

	m_LightData.Diffuse.r=(float)D3DCOLOR_RED(Diffuse)/255.0f;
	m_LightData.Diffuse.g=(float)D3DCOLOR_GREEN(Diffuse)/255.0f;
	m_LightData.Diffuse.b=(float)D3DCOLOR_BLUE(Diffuse)/255.0f;

	m_LightData.Ambient.r=(float)D3DCOLOR_RED(Ambient)/255.0f;
	m_LightData.Ambient.g=(float)D3DCOLOR_GREEN(Ambient)/255.0f;
	m_LightData.Ambient.b=(float)D3DCOLOR_BLUE(Ambient)/255.0f;

	m_pLight->SetLight(m_LightData);
	m_pLight->GetCurLight(m_LightData);

	UpdateData(false);

}
