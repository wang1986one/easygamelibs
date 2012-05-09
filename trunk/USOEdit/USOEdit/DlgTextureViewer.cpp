/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgTextureViewer.cpp                                     */
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
/*      �ļ���:    DlgTextureViewer.cpp                                     */
/*      ��������:  2010��04��30��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgTextureViewer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgTextureViewer.h"

LPCSTR DEFAULT_TEXTURE_VIEW_RECT_FX=
"texture TexLay0 < string name = \"test.jpg\"; >;			\r\n"
"texture TexLay1 < string name = \"test1.jpg\"; >;			\r\n"
"technique tec0												\r\n"
"{															\r\n"
"    pass p0												\r\n"
"    {														\r\n"
"		MultiSampleAntialias = FALSE;						\r\n"
"		Lighting=false;										\r\n"
"		zenable = false;									\r\n"
"		zwriteenable = false;								\r\n"
"		CullMode = ccw;										\r\n"
"		fogenable = false;									\r\n"
"		Texture[0] = <TexLay0>;								\r\n"
"		AlphaTestEnable = false;							\r\n"
"		AlphaRef = 0x80;									\r\n"
"		AlphaFunc = GreaterEqual;							\r\n"
"		AlphaBlendEnable = true;							\r\n"
"		BlendOp = Add;										\r\n"
"		SrcBlend = SrcAlpha;								\r\n"
"		DestBlend = InvSrcAlpha;							\r\n"
"		Texture[0] = <TexLay0>;								\r\n"
"     	ColorOp[0] = SelectArg1;							\r\n"
"       ColorArg1[0] = Texture;								\r\n"
"       AlphaOp[0] = SelectArg1;							\r\n"
"       AlphaArg1[0] = Texture;								\r\n"
"		ColorOp[1] = disable;								\r\n"
"		AlphaOp[1] = disable;								\r\n"
"		AddressU[0] = clamp;								\r\n"
"		AddressV[0] = clamp;								\r\n"      
"		MinFilter[0] = none;								\r\n"
"       MagFilter[0] = none;								\r\n"
"       MipFilter[0] = none;								\r\n"
"		VertexShader = NULL;								\r\n"
"		PixelShader  = NULL;								\r\n"
"    }														\r\n"
"}															\r\n";


// CDlgTextureViewer �Ի���

IMPLEMENT_DYNAMIC(CDlgTextureViewer, CDialog)

CDlgTextureViewer::CDlgTextureViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTextureViewer::IDD, pParent)
{
	m_pRenderThread=NULL;
	m_pTextureViewRect=NULL;
}

CDlgTextureViewer::~CDlgTextureViewer()
{
	SAFE_RELEASE(m_pTextureViewRect);
}

void CDlgTextureViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTextureViewer, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_EDIT_FX, &CDlgTextureViewer::OnBnClickedEditFx)
	ON_BN_CLICKED(IDC_SAVE_TEXTURE, &CDlgTextureViewer::OnBnClickedSaveTexture)
END_MESSAGE_MAP()


// CDlgTextureViewer ��Ϣ�������

BOOL CDlgTextureViewer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	CWnd * pWnd=GetDlgItem(IDC_STATIC_BOX);

	D3DDEVICE_PARAMS Params;
	Params.PresentParams.hDeviceWindow=pWnd->GetSafeHwnd();
	//Params.PresentParams.PresentationInterval=D3DPRESENT_INTERVAL_ONE;

	if(!m_SwapChain.Create(m_pRenderThread->GetDevice(),Params))
	{
		return FALSE;
	}

	m_Render.Create(m_pRenderThread->GetDevice());


	m_pTextureViewRect=new CTextureViewRect;
	m_pTextureViewRect->SetDevice(m_pRenderThread->GetDevice());
	m_Render.AddObject(m_pTextureViewRect);

	CD3DFX * pFX=new CD3DFX(m_pRenderThread->GetDevice()->GetFXManager());
	pFX->LoadFromMemory(DEFAULT_TEXTURE_VIEW_RECT_FX,strlen(DEFAULT_TEXTURE_VIEW_RECT_FX));
	m_pTextureViewRect->GetSubMeshMaterial(0)->SetFX(pFX);


	UINT ID=m_pRenderThread->AddRenderInstance(&m_SwapChain);

	m_pRenderThread->AddRender(ID,&m_Render);

	SetTimer(123456,30,NULL);



	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgTextureViewer::ShowTexture(CD3DTexture * pTexture)
{
	if(pTexture)
	{
		
		m_pTextureViewRect->SetTexture(pTexture);
		pTexture->AddUseRef();			

	
	}
	
}
void CDlgTextureViewer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent==123456&&IsWindowVisible())
	{
		float time=CEasyTimer::GetTime()/1000.0f;
		m_Render.Update(time);

		
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgTextureViewer::OnBnClickedEditFx()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgFXEditor Dlg;

	Dlg.m_FxContent=m_pTextureViewRect->GetSubMeshMaterial(0)->GetFX()->GetFXContent();
	if(Dlg.DoModal()==IDOK)
	{
		m_pTextureViewRect->GetSubMeshMaterial(0)->GetFX()->LoadFromMemory((LPCTSTR)Dlg.m_FxContent,Dlg.m_FxContent.GetLength()+1);
	}
}

void CDlgTextureViewer::OnBnClickedSaveTexture()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CD3DTexture * pTexture=m_pTextureViewRect->GetTexture();
	if(pTexture)
	{
		CFileDialog Dlg(false,_T("*.dds"),_T("Tex1.dds"));

		if(Dlg.DoModal()==IDOK)
		{
			D3DXSaveTextureToFile(Dlg.GetPathName(),D3DXIFF_DDS,pTexture->GetD3DTexture(),NULL);
		}
	}
}
